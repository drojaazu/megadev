/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/echo.c
 * @brief Echo sound driver, 68000 side (main/echo.h)
 *
 * @details
 * Echo's own C API (c/echo.c of sikthehedgehog/echo commit e326c87, by Javier
 * Degirolmo, zlib license: lib/main/echo/LICENSE), adapted to the kit:
 * - The Z80 program is linked from runtime/echo_blob.s, not a C array.
 * - The Z80 bus is taken with main/z80.h and held with interrupts masked, so
 *   an interrupt handler that also uses Echo cannot release the bus while the
 *   interrupted code still relies on it.
 * - echo_set_stereo is declared in the header.
 * - echo_get_status's look-ahead tables cover command 0x09 (set stereo); the
 *   original read past their end while that command was queued.
 * The Z80 RAM layout and the command protocol are Echo's, unchanged.
 */

#include <cpu.h>
#include <main/echo.h>
#include <main/z80.h>
#include <types.h>

extern uint8_t const echo_blob[];
extern uint8_t const echo_blob_end[];

// Z80 RAM locations the 68000 side uses
#define LIST_ADDR_HI 0x1C00 // instrument list: address high byte (+ index)
#define LIST_ADDR_LO 0x1D00 // instrument list: address low byte
#define LIST_BANK    0x1E00 // instrument list: bank
#define DIRECT_BUF   0x1F00 // echo_play_direct event buffer
#define DIRECT_LEN   0x1F80 // its length; 0x80 and up while Echo parses it
#define VOLUMES      0x1FE0 // 16 global volumes
#define VOLUME_PCM   0x1FEC // PCM on (1) or off (0)
#define STATUS       0x1FF0
#define VOLUME_APPLY 0x1FF1 // 1 asks Echo to apply VOLUMES
#define USER_FLAGS   0x1FF2
#define SLOT2        0x1FF8 // second command slot
#define SLOT1        0x1FFC // first command slot: bank, addr lo, addr hi, cmd
#define PCM_HOOK     0x0008 // 0xC9 (ret) while no PCM plays

// Look-up tables for echo_set_volume
uint8_t const echo_fm_vol_table[0x40] = {
  0x7F, 0x7B, 0x77, 0x73, 0x70, 0x6C, 0x68, 0x65, 0x61, 0x5E, 0x5A, 0x57, 0x54,
  0x50, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C, 0x39, 0x36, 0x34, 0x31, 0x2F,
  0x2D, 0x2A, 0x28, 0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18, 0x16, 0x15,
  0x13, 0x12, 0x10, 0x0F, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05,
  0x04, 0x04, 0x03, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00};

uint8_t const echo_psg_vol_table[0x40] = {
  0x0F, 0x0F, 0x0E, 0x0E, 0x0D, 0x0D, 0x0C, 0x0C, 0x0B, 0x0B, 0x0B, 0x0A, 0x0A,
  0x0A, 0x09, 0x09, 0x08, 0x08, 0x08, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06,
  0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x03, 0x03, 0x03, 0x03, 0x03, 0x02,
  0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Argument bytes of each event, so echo_play_direct knows where a stream ends
static uint8_t const arg_table[256] = {
  1,          1, 1, 0, 1, 1, 1, 0,
  1,          1, 1, 1, 1, 0, 0, 0, // 00-0F key on
  0,          0, 0, 0, 0, 0, 0, 0,
  0,          0, 0, 0, 0, 0, 0, 0, // 10-1F key off
  1,          1, 1, 0, 1, 1, 1, 0,
  1,          1, 1, 1, 0, 0, 0, 0, // 20-2F set volume
  2,          2, 2, 0, 2, 2, 2, 0,
  2,          2, 2, 1, 0, 0, 0, 0, // 30-3F set frequency
  1,          1, 1, 0, 1, 1, 1, 0,
  1,          1, 1, 1, 0, 0, 0, 0, // 40-4F set instrument
  [0xF0] = 1, 1, 1, 0, 1, 1, 1, 0,
  2,          2, 1, 1, 0, 0, 1, 0, // F0-FF misc
};

/** Take the Z80 bus with interrupts masked; returns the status register */
static uint16_t take_bus(void)
{
  uint16_t const sr = mask_interrupts();
  z80_request_bus();
  return sr;
}

/** Return the Z80 bus and restore the interrupt mask */
static void give_bus(uint16_t const sr)
{
  z80_release_bus();
  restore_interrupts(sr);
}

/** Give Echo time to take a command while the bus is free */
static void delay(void)
{
  for (volatile int16_t i = 0x1FF; i >= 0; --i)
    ;
}

/** Pulse the Z80 reset line, which also resets the YM2612 */
static void reset_z80(void)
{
  *z80_reg_reset = Z80_RESET_ASSERT;
  for (volatile int16_t i = 30; i >= 0; --i)
    ;
  *z80_reg_reset = Z80_RESET_RELEASE;
}

/**
 * Find a free command slot, waiting for Echo if both are taken. Called and
 * returns with the bus held; *sr is the saved status register.
 */
static volatile uint8_t * free_slot(uint16_t * const sr)
{
  volatile uint8_t * slot = &z80_ram[SLOT1];
  if (slot[3] != ECHO_CMD_NONE) {
    slot = &z80_ram[SLOT2];
    while (slot[3] != ECHO_CMD_NONE) {
      give_bus(*sr);
      delay();
      *sr = take_bus();
    }
  }
  return slot;
}

void echo_init(void const * const * list)
{
  uint16_t const sr = mask_interrupts();
  reset_z80();
  z80_request_bus();

  // No commands pending: the instrument list is written directly below
  z80_ram[SLOT1 + 3] = ECHO_CMD_NONE;
  z80_ram[SLOT2 + 3] = ECHO_CMD_NONE;

  // Direct stream empty
  z80_ram[DIRECT_BUF] = 0xFF;
  z80_ram[DIRECT_LEN] = 0x00;

  // Instrument list, in the bank and address form Echo reads
  for (uint16_t i = 0; list[i] != NULL; ++i) {
    uint32_t const addr = (uint32_t) list[i];
    z80_ram[LIST_ADDR_HI + i] = (uint8_t) (((addr >> 8) & 0x7F) | 0x80);
    z80_ram[LIST_ADDR_LO + i] = (uint8_t) addr;
    z80_ram[LIST_BANK + i] =
      (uint8_t) (((addr >> 15) & 0x7F) | ((addr >> 16) & 0x80));
  }

  // The Z80 program, byte by byte (Z80 RAM takes byte accesses only)
  volatile uint8_t * dest = z80_ram;
  for (uint8_t const * src = echo_blob; src < echo_blob_end; ++src)
    *dest++ = *src;

  // Full volume, PCM on
  for (uint8_t i = 0; i < 16; ++i)
    z80_ram[VOLUMES + i] = 0;
  z80_ram[VOLUME_PCM] = 1;
  z80_ram[VOLUME_APPLY] = 1;

  reset_z80();
  z80_release_bus();
  restore_interrupts(sr);
}

void echo_send_command(uint8_t cmd)
{
  uint16_t                 sr = take_bus();
  volatile uint8_t * const slot = free_slot(&sr);
  slot[3] = cmd;
  give_bus(sr);
}

void echo_send_command_addr(uint8_t cmd, void const * addr)
{
  uint32_t const           param = (uint32_t) addr;
  uint16_t                 sr = take_bus();
  volatile uint8_t * const slot = free_slot(&sr);
  slot[3] = cmd;
  slot[1] = (uint8_t) param;
  slot[2] = (uint8_t) ((param >> 8) | 0x80);
  slot[0] = (uint8_t) (((param >> 15) & 0x7F) | ((param >> 16) & 0x80));
  give_bus(sr);
}

void echo_send_command_byte(uint8_t cmd, uint8_t byte)
{
  uint16_t                 sr = take_bus();
  volatile uint8_t * const slot = free_slot(&sr);
  slot[3] = cmd;
  slot[0] = byte;
  give_bus(sr);
}

void echo_play_bgm(void const * esf)
{
  echo_send_command_addr(ECHO_CMD_PLAYBGM, esf);
}

void echo_stop_bgm(void)
{
  echo_send_command(ECHO_CMD_STOPBGM);
}

void echo_pause_bgm(void)
{
  echo_send_command(ECHO_CMD_PAUSEBGM);
}

void echo_resume_bgm(void)
{
  echo_send_command(ECHO_CMD_RESUMEBGM);
}

void echo_play_sfx(void const * esf)
{
  echo_send_command_addr(ECHO_CMD_PLAYSFX, esf);
}

void echo_stop_sfx(void)
{
  echo_send_command(ECHO_CMD_STOPSFX);
}

void echo_play_direct(void const * esf)
{
  uint16_t sr = take_bus();

  // Wait while Echo is still parsing the buffer
  uint8_t len = z80_ram[DIRECT_LEN];
  while (len >= 0x80) {
    give_bus(sr);
    delay();
    sr = take_bus();
    len = z80_ram[DIRECT_LEN];
  }

  // Append the events, up to and including the 0xFF terminator
  uint8_t const *    src = esf;
  volatile uint8_t * dest = &z80_ram[DIRECT_BUF + len];
  for (;;) {
    uint8_t const event = *src++;
    *dest++ = event;
    ++len;
    if (event == 0xFF)
      break;
    for (uint8_t n = arg_table[event]; n > 0; --n) {
      *dest++ = *src++;
      ++len;
    }
  }
  z80_ram[DIRECT_LEN] = len;

  give_bus(sr);
}

void echo_set_volume(uint8_t vol)
{
  uint8_t const  fm_vol = echo_fm_vol_table[vol >> 2];
  uint8_t const  psg_vol = echo_psg_vol_table[vol >> 2];
  uint16_t const sr = take_bus();
  for (uint8_t i = 0; i < 8; ++i)
    z80_ram[VOLUMES + i] = fm_vol;
  for (uint8_t i = 8; i < 12; ++i)
    z80_ram[VOLUMES + i] = psg_vol;
  z80_ram[VOLUME_PCM] = (vol >= 0x40) ? 1 : 0;
  z80_ram[VOLUME_APPLY] = 1;
  give_bus(sr);
}

void echo_set_volume_ex(uint8_t const * vol_list)
{
  uint16_t const sr = take_bus();
  for (uint8_t i = 0; i < 16; ++i)
    z80_ram[VOLUMES + i] = vol_list[i];
  z80_ram[VOLUME_APPLY] = 1;
  give_bus(sr);
}

void echo_set_pcm_rate(uint8_t rate)
{
  echo_send_command_byte(ECHO_CMD_SETPCMRATE, rate);
}

void echo_set_stereo(int enable)
{
  echo_send_command_byte(ECHO_CMD_SETSTEREO, enable ? 1 : 0);
}

uint16_t echo_get_status(void)
{
  // A play or stop still waiting in a slot sets or clears its flag already,
  // so the status agrees with the command the program just sent
  static uint16_t const and_flags[] = {
    0xFFFF,
    0xFFFF,
    0xFFFF,
    0xFFFE,
    0xFFFF,
    0xFFFD,
    0xFFFF,
    0xFFFF,
    0xFFFF,
    0xFFFF};
  static uint16_t const or_flags[] = {
    0x0000,
    0x0000,
    0x0001,
    0x0000,
    0x0002,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000};

  uint16_t const sr = take_bus();
  uint16_t       status = z80_ram[STATUS];
  if (z80_ram[SLOT2 + 3] != ECHO_CMD_NONE)
    status |= ECHO_STAT_BUSY;
  if (z80_ram[DIRECT_BUF] != 0xFF)
    status |= ECHO_STAT_DIRBUSY;
  if (z80_ram[PCM_HOOK] != 0xC9)
    status |= ECHO_STAT_PCM;
  uint8_t const cmd1 = z80_ram[SLOT1 + 3];
  uint8_t const cmd2 = z80_ram[SLOT2 + 3];
  give_bus(sr);

  if (cmd1 <= ECHO_CMD_SETSTEREO)
    status = (status & and_flags[cmd1]) | or_flags[cmd1];
  if (cmd2 <= ECHO_CMD_SETSTEREO)
    status = (status & and_flags[cmd2]) | or_flags[cmd2];
  return status;
}

uint8_t echo_get_flags(void)
{
  uint16_t const sr = take_bus();
  uint8_t const  flags = z80_ram[USER_FLAGS];
  give_bus(sr);
  return flags;
}

void echo_set_flags(uint8_t flags)
{
  uint16_t const sr = take_bus();
  z80_ram[USER_FLAGS] |= flags;
  give_bus(sr);
}

void echo_clear_flags(uint8_t flags)
{
  uint16_t const sr = take_bus();
  z80_ram[USER_FLAGS] &= (uint8_t) ~flags;
  give_bus(sr);
}

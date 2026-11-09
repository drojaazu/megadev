#include <macro.h>
#include <sub/memmap.def.h>

// Module layout in Program RAM
GLOBAL_SYM(MODULE_ROM_ORIGIN, 0x10000);
GLOBAL_SYM(MODULE_ROM_LENGTH, 0xE000);
GLOBAL_SYM(MODULE_RAM_ORIGIN, 0x1E000);
GLOBAL_SYM(MODULE_RAM_LENGTH, 0x2000);

#include "shared.h"
#include <sub/cdrom.h>
#include <sub/gate_arr.h>
#include <sub/memmap.h>
#include <sub/pcm.h>
#include <cpu.h>

/*
 * The audio is streamed from the disc. The PCM chip plays channel 1 through
 * all 64 KB of wave RAM in a loop, and while it plays one half the Sub CPU
 * fills the other half, one 4 KB bank at a time, straight from the CDC by
 * DMA: two sectors per bank, since the DMA address register reaches one bank.
 * Nothing is staged in Program RAM, so the file can be any length.
 */

#define PCM_FILE     "AUDIO.PCM;1"
#define BANK_SECTORS 2  // 4 KB bank, 2 KB sectors
#define BANKS        16 // 64 KB of wave RAM

char const * const filenames[] = {"IPX.MMD;1"};

// channel 1: full volume, both sides, about 22 kHz, start and loop at 0
const PcmChannelSettings pcmSettings = {0xFF, 0xFF, 0x6B, 0x5, 0, 0, 0};

extern void sp_fatal();

// Byte sum of every bank loaded, read back from wave RAM: a check an
// emulator's debugger can compare with the file (see the README)
volatile u16 stream_sum;

static u8 play_bank()
{
  return *((volatile u8 *) PCM_PLAY_CH1_HI) >> 4;
}

// Load the given 4 KB of the file into the given bank
static void load_bank(u8 bank, u32 chunk)
{
  *pcmreg_ctrl = WAVEBANK(bank);
  if (
    load_file_part(
      CDROM_LOAD_PCM_DMA,
      PCM_FILE,
      (u8 *) 0,
      chunk * BANK_SECTORS,
      BANK_SECTORS) == 0)
    sp_fatal();
  volatile u8 const * wave = (volatile u8 const *) (PCM_RAM + 1);
  for (u16 i = 0; i < 0x1000; ++i)
    stream_sum += wave[i << 1];
}

static void stream_pcm()
{
  FileInfo const * const info = find_file_c(PCM_FILE);
  if (info == NULL)
    sp_fatal();
  u32 const chunks = (info->size + 0xFFF) >> 12;
  stream_sum = 0;

  pcm_clear_ram_c();
  pcm_config_channel_c(CHANNEL(1), &pcmSettings);

  // fill the lower half before starting
  u32 chunk = 0;
  for (; chunk < BANKS / 2 && chunk < chunks; ++chunk)
    load_bank(chunk, chunk);

  // we use the highest bit of the comflags to inform main that the audio is
  // playing (be careful about setting low bits on COMFLAGS when using the
  // default VBLANK handler in the Main Boot ROM Library; see the Predefined
  // Comm Flag Semantics section of main_bios.md)
  ga_sub_reg_comflags_sub |= 0x80;
  *pcmreg_ch_disable = 0xFE; // channel 1 on

  // fill whichever half the play head is not in
  for (; chunk < chunks; ++chunk) {
    u8 const bank = chunk % BANKS;
    while ((play_bank() >> 3) == (bank >> 3))
      ;
    load_bank(bank, chunk);
  }

  // let the last half play out, then stop
  u8 const last_half = ((chunks - 1) % BANKS) >> 3;
  while ((play_bank() >> 3) != last_half)
    ;
  while ((play_bank() >> 3) == last_half)
    ;
  *pcmreg_ch_disable = 0xFF;
  ga_sub_reg_comflags_sub &= ~0x80;
  ga_sub_reg_comstat3 = stream_sum;
}

void main()
{
  // std_init.s masks interrupts; the Sub BIOS needs them for CD-ROM access
  enable_interrupts();

  register u16 command, param1;

  do {
    do {
      command = ga_sub_reg_comcmd0;
    } while (command == 0);

    if (command != ga_sub_reg_comcmd0)
      continue;

    param1 = ga_sub_reg_comcmd1;

    switch (command) {
      case CMD_LOAD_WORDRAM:
        load_file(CDROM_LOAD_CDC, filenames[param1], (u8 *) WORD_RAM_2M);
        ga_sub_grant_2m();
        if (access_op_result != CDROM_RESULT_OK) {
          sp_fatal();
        }
        break;

      case CMD_PLAY_PCM:
        stream_pcm();
        break;
    }

    ga_sub_reg_comstat0 = ga_sub_reg_comcmd0;
    do {
      asm("nop");
      command = ga_sub_reg_comcmd0;
    } while (command != 0);

    do {
      asm("nop");
      command = ga_sub_reg_comcmd0;
    } while (command != 0);

    ga_sub_reg_comstat0 = 0;

  } while (1);
}

/**
 * Main CPU program: Echo on the Mega CD
 *
 * Disc build: a module that runs in place in 2M Word RAM (MODULE_ROM_ORIGIN
 * 0x200100, MMD_DEST 0), with the song, the effects and the instruments in
 * song.s linked into it, so they sit in Word RAM too. The Z80 cannot read
 * Main Work RAM, but it read Word RAM while the Main CPU owned it on the
 * hardware tested (docs/sound.md: this is contrary to Sega's documentation).
 * The Main CPU keeps Word RAM for as long as Echo plays.
 *
 * Mode 1 build: linked into the cartridge with its music, and called by
 * mode1_main.c. Echo's addresses leave out bit 22, so it cannot reach Mode
 * 1's Word RAM at 0x600000; it reads cartridge ROM as on a Mega Drive.
 *
 * The game logic runs on the Sub CPU (spx.c). Every VBLANK this module
 * passes the buttons held on pad 1 to it (COMCMD2) and passes its sound
 * commands on to Echo with sound_forward (main/sound.h).
 */
#include "shared.h"
#include "shortcuts.h"
#include <cpu.h>
#include <macro.h>
#include <main/crash.h>
#include <main/echo.h>
#include <main/gate_arr.h>
#include <main/io.h>
#include <main/memmap.h>
#include <main/sound.h>
#include <main/text.h>
#include <main/vdp.h>
#include <main/z80.h>
#include <stdio.h>

#if TARGET == MEGACD_MODE1
#define ENTRY      sound_main
#define WHERE_TEXT "MUSIC AND EFFECTS ARE IN CARTRIDGE ROM."
#else
/* Module layout: code and data in place in 2M Word RAM after the 0x100-byte
   header; variables and the stack in Work RAM */
GLOBAL_SYM(MODULE_ROM_ORIGIN, WORD_RAM + 0x100);
GLOBAL_SYM(MODULE_ROM_LENGTH, 0x40000 - 0x100);
GLOBAL_SYM(MODULE_RAM_ORIGIN, 0xFFF000);
GLOBAL_SYM(MODULE_RAM_LENGTH, 0x700);
GLOBAL_SYM(STACK_TOP, MODULE_RAM_ORIGIN);
GLOBAL_SYM(STACK_SIZE, 0x800);
GLOBAL_SYM(MMD_DEST, 0);
#define ENTRY      main
#define WHERE_TEXT "MUSIC AND EFFECTS ARE IN WORD RAM."
#endif

// song.s
extern void const * const instrument_list[];
extern u8 const           song[], sfx_beep[], sfx_chime[];

static void const * const bgm_table[] = {[BGM_SONG] = song};
static void const * const sfx_table[] = {
  [SFX_BEEP] = sfx_beep, [SFX_CHIME] = sfx_chime};

#define PLANE_A_ADDR    0xC000
#define PLANE_B_ADDR    0xE000
#define SPRITE_TBL_ADDR 0xB800

#define TEXT_TILE      0x20
#define HEADING_TILE   (0x20 | (1 << 13))
#define plane_xy(x, y) VDP_PLANE_XY(PLANE_A_ADDR, x, y, Width64)

static void print_at(char const * s, u8 x, u8 y)
{
  text_print(s, plane_xy(x, y), TEXT_TILE);
}

static void heading_at(char const * s, u8 x, u8 y)
{
  text_print(s, plane_xy(x, y), HEADING_TILE);
}

#if VIDEO == PAL
#define VIDEO_SIGNAL VDP_PAL_VIDEO
#else
#define VIDEO_SIGNAL 0
#endif

static u16 const vdp_init_regs[] = {
  VDP_REG_MODE1 | VDP_HICOLOR_ENABLE,
  VDP_REG_MODE2 | VDP_MD_DISPLAY_MODE | VDP_VBLANK_ENABLE | VIDEO_SIGNAL |
    VDP_DISPLAY_ENABLE,
  VDP_REG_PLA_ADDR | (PLANE_A_ADDR / 0x400),
  VDP_REG_WIN_ADDR | (0xa00 / 0x400),
  VDP_REG_PLB_ADDR | (PLANE_B_ADDR / 0x2000),
  VDP_REG_SPR_ADDR | (SPRITE_TBL_ADDR / 0x200),
  VDP_REG_SPR_ADDR2,
  VDP_REG_BGCOLOR,
  VDP_REG_HBLANK_COUNT,
  VDP_REG_MODE3,
  VDP_REG_MODE4 | VDP_WIDTH_40CELL_MASK,
  VDP_REG_HS_ADDR | (0xbc00 / 0x400),
  VDP_REG_PL_ADDR2,
  VDP_REG_AUTOINC | 2,
  VDP_REG_PL_SIZE | VDP_PL_64x32,
  VDP_REG_WIN_HPOS,
  VDP_REG_WIN_VPOS};

static void init_video()
{
  for (u8 i = 0; i < sizeof vdp_init_regs / sizeof vdp_init_regs[0]; ++i)
    vdp_ctrl = vdp_init_regs[i];

  vdp_ctrl_32 = to_vdp_addr(0) | VRAM_W;
  for (u16 i = 0; i < (0x10000 / 4); ++i)
    vdp_data_32 = 0;
  vdp_ctrl_32 = to_vdp_addr(0) | VSRAM_W;
  for (u8 i = 0; i < 40; ++i)
    vdp_data = 0;

  // line 0: black background, white text; line 1: yellow headings
  vdp_ctrl_32 = to_vdp_addr(0) | CRAM_W;
  vdp_data = 0x0000;
  vdp_data = 0x0EEE;
  vdp_ctrl_32 = to_vdp_addr(0x20) | CRAM_W;
  vdp_data = 0x0000;
  vdp_data = 0x00EE;

  vdp_load_1bpp_tiles(sysfont, SYSFONT_CHARS, TEXT_TILE, 1, 0);
}

/**
 * Read controller 1, buttons pressed set to 1.
 *
 * While Echo runs, an access to the I/O ports can corrupt a byte the Z80 is
 * fetching at that moment, so the read is done with the Z80 bus held.
 */
static u8 read_pad()
{
  u16 const sr = mask_interrupts();
  z80_request_bus();
  u8 const pad = ~read_input_joypad(io_data1);
  z80_release_bus();
  restore_interrupts(sr);
  return pad;
}

volatile bool vblank_done;

/**
 * Raise INT2 on the Sub CPU (it paces its program and its CD-ROM access by
 * it), hand it the buttons, and pass its sound command, if any, to Echo.
 */
__attribute__((interrupt)) static void vblank_handler()
{
  ga_main_reg_int2 = 1;
  ga_main_reg_comcmd2 = read_pad();
  sound_forward();
  vblank_done = true;
}

static void wait_vblank()
{
  vblank_done = false;
  while (! vblank_done)
    ;
}

static char const * const state_names[] = {
  [STATE_STOPPED] = "STOPPED", [STATE_PLAYING] = "PLAYING",
  [STATE_PAUSED] = "PAUSED "};

__attribute__((noreturn)) void ENTRY()
{
  char buf[24];

  disable_interrupts();
  init_video();

  heading_at("ECHO ON THE MEGA CD", 2, 2);
  print_at(WHERE_TEXT, 2, 4);
  print_at("THE SUB CPU SENDS THE COMMANDS.", 2, 5);
  heading_at("CONTROLS", 2, 8);
  print_at("A      BEEP", 2, 10);
  print_at("B      CHIME", 2, 11);
  print_at("C      STOP / PLAY", 2, 12);
  print_at("START  PAUSE / RESUME", 2, 13);
  heading_at("STATUS", 2, 16);
  print_at("MUSIC:", 2, 18);
  print_at("LAST COMMAND:", 2, 19);

  crash_install();

  // Echo, with its instruments in Word RAM; then the tables the Sub CPU's
  // commands number
  echo_init(instrument_list);
  sound_set_tables(bgm_table, sizeof bgm_table / sizeof bgm_table[0],
    sfx_table, sizeof sfx_table / sizeof sfx_table[0]);

  *exvec_vblank = vblank_handler;
  enable_interrupts();

  // Echo is ready: let the Sub CPU start
  ga_main_reg_comcmd0 = CMD_START;
  while (ga_main_reg_comstat0 != CMD_START)
    wait_vblank();
  ga_main_reg_comcmd0 = 0;

  for (;;) {
    wait_vblank();
    crash_check_sub();
    u16 const state = ga_main_reg_comstat1;
    print_at(state <= STATE_PAUSED && state_names[state] ? state_names[state]
                                                          : "?      ",
      16, 18);
    snprintf(buf, sizeof buf, "%04X", ga_main_reg_comcmd4);
    print_at(buf, 16, 19);
  }
}

#include <macro.h>
#include <main/memmap.def.h>

// Module layout: ROM begins at the start of Work RAM, overwriting the IP;
// RAM follows it, leaving a little over 61 KB for ROM and 512 bytes of RAM.
// MMD_DEST says where the module is copied after loading into Word RAM.
GLOBAL_SYM(MODULE_ROM_ORIGIN, WORK_RAM);
GLOBAL_SYM(MODULE_RAM_ORIGIN, 0xFFF000);
GLOBAL_SYM(MODULE_ROM_LENGTH, MODULE_RAM_ORIGIN - MODULE_ROM_ORIGIN);
GLOBAL_SYM(MODULE_RAM_LENGTH, 0x700);
GLOBAL_SYM(MMD_DEST, WORK_RAM);

#include "shared.h"
#include <main/bios.h>
#include <main/crash.h>
#include <main/gate_arr.h>
#include <main/io.def.h>
#include <main/text.h>
#include <main/vdp.h>
#include <types.h>

// The system font is loaded at tile 0x20, so a character's tile is its code
#define TEXT_TILE 0x20

void play_pcm()
{
  ga_main_reg_comcmd0 = CMD_PLAY_PCM;

  text_print(
    "Playing...",
    VDP_PLANE_XY(BIOS_VDP_DEFAULT_PLANEA, 1, 2, Width64),
    TEXT_TILE);

  // wait for the playing flag to clear
  while (ga_main_reg_comflags_sub & 0x80)
    crash_check_sub();

  while (ga_main_reg_comstat0 == 0)
    crash_check_sub();
  ga_main_reg_comcmd0 = 0;
  while (ga_main_reg_comstat0 != 0)
    crash_check_sub();

  text_print(
    "Done      ",
    VDP_PLANE_XY(BIOS_VDP_DEFAULT_PLANEA, 1, 2, Width64),
    TEXT_TILE);
}

void main()
{
  // crash handler for Main CPU exceptions (docs/crash_handler.md)
  crash_install();

  vdp_load_1bpp_tiles(sysfont, SYSFONT_CHARS, TEXT_TILE, 1, 0);
  bios_palette[1] = 0xEEE;
  bios_vdp_update_flags |= BIOS_MASK_COPY_PALETTE;
  bios_vblank_wait_default();
  crash_check_sub();

  text_print(
    "PCM Audio Playback",
    VDP_PLANE_XY(BIOS_VDP_DEFAULT_PLANEA, 1, 1, Width64),
    TEXT_TILE);

  play_pcm();

  text_print(
    "Press A to replay",
    VDP_PLANE_XY(BIOS_VDP_DEFAULT_PLANEA, 1, 4, Width64),
    TEXT_TILE);

  // main loop
  while (true) {
    bios_vblank_wait_default();
    crash_check_sub();

    if ((bios_joy1_hit & PAD_A)) {
      play_pcm();
    }
  }
}

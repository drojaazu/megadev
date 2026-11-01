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
#include <fixed.h>
#include <main/bios.h>
#include <main/exvec.h>
#include <main/gate_arr.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <system.h>
#include <types.h>

void play_pcm()
{
  ga_main_reg_comcmd0 = CMD_PLAY_PCM;

  bios_print(
    "Playing...\xff",
    (to_vdp_addr(BIOS_VDP_DEFAULT_PLANEA + VDP_PLANE_POS(1, 2, Width64)) |
     VRAM_W));

  // wait for the playing flag to clear
  while (ga_main_reg_comflags_sub & 0x80)
    exvec_check_sub();

  while (ga_main_reg_comstat0 == 0)
    exvec_check_sub();
  ga_main_reg_comcmd0 = 0;
  while (ga_main_reg_comstat0 != 0)
    exvec_check_sub();

  bios_print(
    "Done      \xff",
    (to_vdp_addr(BIOS_VDP_DEFAULT_PLANEA + VDP_PLANE_POS(1, 2, Width64)) |
     VRAM_W));
}

void load_pcm()
{
  bios_print(
    "Loading...\xff",
    (to_vdp_addr(BIOS_VDP_DEFAULT_PLANEA + VDP_PLANE_POS(1, 2, Width64)) |
     VRAM_W));
  ga_main_reg_comcmd0 = CMD_LOAD_PRGRAM;
  while (ga_main_reg_comstat0 == 0)
    exvec_check_sub();
  ga_main_reg_comcmd0 = 0;
  while (ga_main_reg_comstat0 != 0)
    exvec_check_sub();

  play_pcm();
}

void main()
{
  // crash handler for Main CPU exceptions (docs/crash_handler.md)
  exvec_install();

  bios_load_font_defaults();
  bios_palette[1] = 0xEEE;
  bios_vdp_update_flags |= BIOS_MASK_COPY_PALETTE;
  bios_vblank_wait_default();
  exvec_check_sub();

  bios_print(
    "PCM Audio Playback\xff",
    (to_vdp_addr(BIOS_VDP_DEFAULT_PLANEA + VDP_PLANE_POS(1, 1, Width64)) |
     VRAM_W));

  load_pcm();

  bios_print(
    "Press A to replay\xff",
    (to_vdp_addr(BIOS_VDP_DEFAULT_PLANEA + VDP_PLANE_POS(1, 4, Width64)) |
     VRAM_W));

  // main loop
  while (true)
  {
    bios_vblank_wait_default();
    exvec_check_sub();

    if ((bios_joy1_hit & PAD_A))
    {
      play_pcm();
    }
  }
}

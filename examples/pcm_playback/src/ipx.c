#include "shared.h"
#include <fixed.h>
#include <main/bios.h>
#include <main/gate_arr.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <system.h>
#include <types.h>
#include <macro.h>
#include <main/memmap.def.h>

/*
  Module layout.

  ROM begins at the start of Work RAM, overwriting the security code/IP, and
  RAM goes right after it. That gives a little over 61KB of ROM space and 512
  bytes of RAM; the split is arbitrary. MMD_DEST is where the code actually
  executes, i.e. where it is copied to after the Sub places it in Word RAM.
*/
GLOBAL_SYM(MODULE_ROM_ORIGIN, WORK_RAM);
GLOBAL_SYM(MODULE_RAM_ORIGIN, 0xFFF500);
GLOBAL_SYM(MODULE_ROM_LENGTH, MODULE_RAM_ORIGIN - MODULE_ROM_ORIGIN);
GLOBAL_SYM(MODULE_RAM_LENGTH, 0x200);
GLOBAL_SYM(MMD_DEST, WORK_RAM);

void play_pcm()
{
  ga_reg_comcmd0 = CMD_PLAY_PCM;

  bios_print(
    "Playing...\xff",
    (to_vdp_addr(BIOS_VDP_DEFAULT_PLANEA + VDP_PLANE_POS(1, 2, Width64)) |
     VRAM_W));

  // wait for the playing flag to clear
  while (ga_reg_comflags_sub & 0x80)
    ;

  while (ga_reg_comstat0 == 0)
    ;
  ga_reg_comcmd0 = 0;
  while (ga_reg_comstat0 != 0)
    ;

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
  ga_reg_comcmd0 = CMD_LOAD_PRGRAM;
  while (ga_reg_comstat0 == 0)
    ;
  ga_reg_comcmd0 = 0;
  while (ga_reg_comstat0 != 0)
    ;

  play_pcm();
}

void main()
{
  bios_load_font_defaults();
  bios_palette[1] = 0xEEE;
  bios_vdp_update_flags |= BIOS_MASK_COPY_PALETTE;
  bios_vblank_wait_default();

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

    if ((bios_joy1_hit & PAD_A))
    {
      play_pcm();
    }
  }
}

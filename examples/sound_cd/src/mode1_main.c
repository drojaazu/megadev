/**
 * Mode 1 build: the cartridge program
 *
 * Built only by `make MODE1=1` (see the makefile). Starts the Sub CPU with
 * the SP and the SPX already in Program RAM, as a disc boot would, then runs
 * the Main CPU program (main.c), which in this build is linked into the
 * cartridge with its music rather than loaded into Word RAM: Echo cannot
 * address Mode 1's Word RAM at 0x600000 (see main.c), but it reads cartridge
 * ROM. From there the example runs as it does from the disc.
 */
#include "shortcuts.h"
#include <cpu.h>
#include <main/bios.h>
#include <main/gate_arr.h>
#include <main/io.h>
#include <main/memmap.h>
#include <main/mode1.h>
#include <main/text.h>
#include <main/vdp.h>
#include <memory.h>
#include <stdio.h>

typedef struct {
  u32 size;
  u8  data[];
} DataChunk;

extern DataChunk const res_sp_mode1;

#define FONT_TILE 0x20

// Without a Mega CD there is no Boot ROM library: set up just enough of the
// VDP to show a red screen.
static __attribute__((noreturn)) void halt_no_megacd()
{
  vdp_ctrl = VDP_REG_MODE1 | VDP_HICOLOR_ENABLE;
  vdp_ctrl = VDP_REG_MODE2 | VDP_MD_DISPLAY_MODE | VDP_DISPLAY_ENABLE;
  vdp_ctrl = VDP_REG_BGCOLOR | 0;
  vdp_ctrl_32 = to_vdp_addr(0) | CRAM_W;
  vdp_data = 0x000E;
  for (;;)
    ;
}

static __attribute__((noreturn)) void halt_sub_error(u8 result)
{
  char buf[24];
  vdp_ctrl_32 = to_vdp_addr(0) | CRAM_W;
  vdp_data = 0x0000;
  vdp_data = 0x0EEE;
  vdp_load_1bpp_tiles(sysfont, SYSFONT_CHARS, FONT_TILE, 1, 0);
  snprintf(buf, sizeof buf, "SUB CPU ERROR %02X", result);
  text_print(buf, VDP_PLANE_XY(BIOS_VDP_DEFAULT_PLANEA, 2, 2, Width64),
    FONT_TILE);
  bios_vdp_disp_enable();
  for (;;)
    ;
}

// main.c's program; in the disc build it is the main module's main
extern __attribute__((noreturn)) void sound_main(void);

__attribute__((noreturn)) void main()
{
  disable_interrupts();

  if (! mode1_detect())
    halt_no_megacd();

  // The Boot ROM library state a disc boot leaves behind
  bios_load_vdp_regs_default();
  bios_clear_vram();
  bios_clear_tables();
  bios_clear_vsram();
  bios_clear_comm();
  // The controller ports, as the Boot ROM sets them up on a disc boot (not
  // through its jump table): TH as an output, without which the joypad read
  // in its VBLANK handler sees neither A nor Start
  init_joypads();

  u8 const result = mode1_load_sub(res_sp_mode1.data, res_sp_mode1.size);
  if (result != MODE1_OK)
    halt_sub_error(result);

  // The Boot ROM's VBLANK handler, as ip.s installs it: it also raises INT2
  // on the Sub CPU every frame
  bios_vblank_handler_flags = 0;
  *exvec_vblank = (void(*)) BIOS_VBLANK_HANDLER;
  enable_interrupts();

  bios_vdp_disp_enable();
  sound_main();
}

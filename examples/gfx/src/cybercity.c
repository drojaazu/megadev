
#include "io.h"
#include "main/bootlib.h"
#include "main/main.h"
#include "types.h"
#include "vdp.h"

extern u8 res_cybercity_bldg_cmp_nem[];
extern u8 res_cybercity_farbg_cmp_nem[];
extern u16 const res_cybercity_pal[];

extern u16 res_cybercity_bldg_map[];
extern u16 res_cybercity_farbg_map[];
extern u8 res_ship_chr[];
extern u16 const res_ship_pal[];

extern u8 res_ship_spr[];

struct SpriteObject ship_parts;

void null_func() { return; }

void vint_ex() { boot_copy_sprlist(); }

void (*spr_funcs[2])();

/*
        We mark this is as noreturn since this is an infinite loop that will
   never escape. Hopefully this will hint some optimizations to GCC.
   Unfortunately, it looks like GCC will still push the registers on entry,
   taking up valuable stack space. I have not found a way around this (the naked
   attribute is not available on m68k arch). Any suggestions to prevent this
   would be appreciated!
*/
__attribute__((noreturn)) void main() {

  spr_funcs[0] = null_func;
  spr_funcs[1] = null_func;

  *SPR_JMPTBL_PTR = spr_funcs;

  ship_parts.spriteDef = res_ship_spr;
  ship_parts.posX = 150 << 16;
  ship_parts.posY = 140 << 16;
  ship_parts.status = 1;
  ship_parts.uk1 = 0x20;

  // turn off the display while we init
  boot_vdp_disp_disable();
  // boot rom defaults are fine...
  boot_load_vdpregs_default();

  // We use the PlaneWidthTiles enum to assign the value in *tiles*, which is
  // more intuitive than the count in nmtbl entries
  // See the documentation on PLANE_WIDTH for more
  *PLANE_WIDTH = Width32;

  // The load VDP regs Boot ROM routines expect a zero terminated array of
  // raw register values
  // See the documentation on BOOT_LOAD_VDPREGS for more
  u16 vdp_planewidth_reg[] = {0x9010, 0};
  boot_load_vdpregs(vdp_planewidth_reg);

  // load the palettes
  // In general, use the VDPPTR macro for converting a VRAM address to the
  // VDP compatible format. It is written so that constant values will be
  // calculated at compile time, and variables will be calculated at runtime
  // with the optimized conversion code
  boot_dma_xfer_wordram((VDPPTR(0) | CRAM_W), res_cybercity_pal, 32 >> 1);
  boot_dma_xfer_wordram((VDPPTR(32) | CRAM_W), res_ship_pal, 32 >> 1);

  // boot_gfx_decomp requires that we set the VDP address first
  *VDP_CTRL_32 = VDPPTR(VRAM_AT(1)) | VRAM_W;
  boot_gfx_decomp(res_cybercity_bldg_cmp_nem);

  // first word of Nemesis compression is the tile count, with the msb
  // determining XOR mode so we can use this (with the msb cleared) to get the
  // next free tile +1 to account for the blank 0 tile
  u16 free_tile = ((*(u16 *)res_cybercity_bldg_cmp_nem) & 0x7fff) + 1;

  *VDP_CTRL_32 = VDPPTR(VRAM_AT(free_tile)) | VRAM_W;
  boot_gfx_decomp(res_cybercity_farbg_cmp_nem);

  free_tile += ((*(u16 *)res_cybercity_farbg_cmp_nem) & 0x7fff);

  boot_dma_xfer_wordram(VDPPTR(VRAM_AT(free_tile)) | VRAM_W, res_ship_chr,
                        1920 >> 1);

  boot_load_map(VDPPTR(NMT_POS_PLANE(0, 2, BOOT_PLANEA_ADDR)) | VRAM_W,
                res_cybercity_bldg_map[0] - 1, res_cybercity_bldg_map[1] - 1,
                res_cybercity_bldg_map + 2);
  boot_load_map(VDPPTR(BOOT_PLANEB_ADDR) | VRAM_W,
                res_cybercity_farbg_map[0] - 1, res_cybercity_farbg_map[1] - 1,
                res_cybercity_farbg_map + 2);

  boot_vdp_disp_enable();

  *VINT_EX_PTR = vint_ex;

  u16 scroll_a = 0;
  u8 scroll_b = 0;
  u8 wait_a = 0;
  u8 wait_b = 0;

  do {

    boot_process_spr_objs(&ship_parts, SPRITE_LIST, 0, 0x1a);

    *VINT_FLAGS = COPY_SPRLIST_MSK;
    boot_vint_wait_default();

    wait_a++;
    wait_b++;

    if (wait_a >= 2) {
      *VDP_CTRL_32 = (VDPPTR((BOOT_HSCROL_ADDR)) | VRAM_W);
      *VDP_DATA_16 = --scroll_a;
      wait_a = 0;
    }

    if (wait_b >= 4) {
      *VDP_CTRL_32 = (VDPPTR((BOOT_HSCROL_ADDR + 2)) | VRAM_W);
      *VDP_DATA_16 = --scroll_b;
      wait_b = 0;
    }

    if (*JOY1_HOLD & PAD_RIGHT_MSK) {
      ship_parts.posX += (3 << 16);
      wait_a++;
    }

    if (*JOY1_HOLD & PAD_LEFT_MSK) {
      ship_parts.posX -= (3 << 16);
    }

    if (*JOY1_HOLD & PAD_UP_MSK) {
      ship_parts.posY -= (3 << 16);
    }

    if (*JOY1_HOLD & PAD_DOWN_MSK) {
      ship_parts.posY += (3 << 16);
    }

  } while (1);
}

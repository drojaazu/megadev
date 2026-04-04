#include <main/bios.h>
#include <main/io.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <math.h>
#include <types.h>

// resource declarations
extern u8        res_cybercity_bldg_cmp_nem[];
extern u8        res_cybercity_farbg_cmp_nem[];
extern u16 const res_cybercity_pal[];

extern u16       res_cybercity_bldg_map[];
extern u16       res_cybercity_farbg_map[];
extern u8        res_ship_chr[];
extern u16 const res_ship_pal[];

extern SpriteLayout res_ship_layout;

Entity sprobj_ship;

void null_func()
{
  return;
}

void vblank_user()
{
  bios_copy_sprlist();
}

void (*spr_funcs[2])() = {null_func, null_func};

const vdp_cmd vdpptr_scroll = to_vdp_addr((BIOS_VDP_DEFAULT_HSCROLL)) | VRAM_W;

/*
  We mark this is as noreturn since this is an infinite loop that will
  never escape. Hopefully this will hint some optimizations to GCC.
  Unfortunately, it looks like GCC will still push the registers on entry,
  taking up valuable stack space. I have not found a way around this (the
  `naked` attribute is not available on m68k arch). Any suggestions to prevent
  this would be appreciated!
*/
__attribute__((noreturn)) void main()
{
  bios_entity_routines = spr_funcs;

  sprobj_ship.enable = true;
  sprobj_ship.layout = &res_ship_layout;
  sprobj_ship.pos_x = 150 << 16;
  sprobj_ship.pos_y = 140 << 16;
  sprobj_ship.jmptbl_offset = 0;
  sprobj_ship.sprite_flags = 0x20;

  // turn off the display while we init
  bios_vdp_disp_disable();
  // boot rom defaults are fine...
  bios_load_vdpregs_default();

  // We use the PlaneWidthTiles enum to assign the value in *tiles*, which is
  // more intuitive than the count in nametable entries, which is what it
  // expects See the documentation on bios_plane_width for more
  bios_plane_width = Width32;

  // The load VDP regs Boot ROM routines expect a zero terminated array of
  // raw register values
  // See the documentation on BIOS_LOAD_VDPREGS for more
  vdp_reg const vdp_planewidth_reg[] = {VDPREG_PL_SIZE | VDP_PL_32x64, 0};
  bios_load_vdpregs(vdp_planewidth_reg);

  // load the palettes
  // In general, use the to_vdp_addr macro for converting a VRAM address to the
  // VDP compatible format. It is written so that constant values will be
  // calculated at compile time, and variables will be calculated at runtime
  // with the optimized conversion code
  bios_dma_xfer_word_ram((to_vdp_addr(0) | CRAM_W), res_cybercity_pal, 32 >> 1);
  bios_dma_xfer_word_ram((to_vdp_addr(32) | CRAM_W), res_ship_pal, 32 >> 1);

  // bios_gfx_decomp requires that we set the VDP address first
  vdp_ctrl_32 = to_vdp_addr(vram_addr_from_tileidx(1)) | VRAM_W;
  bios_gfx_decomp(res_cybercity_bldg_cmp_nem);

  // first word of Nemesis compression is the tile count, with the msb
  // determining XOR mode so we can use this (with the msb cleared) to get the
  // next free tile +1 to account for the blank 0 tile
  u16 free_tile = ((*(u16 *) res_cybercity_bldg_cmp_nem) & 0x7FFF) + 1;

  vdp_ctrl_32 = to_vdp_addr(vram_addr_from_tileidx(free_tile)) | VRAM_W;
  bios_gfx_decomp(res_cybercity_farbg_cmp_nem);

  free_tile += ((*(u16 *) res_cybercity_farbg_cmp_nem) & 0x7FFF);

  bios_dma_xfer_word_ram(
    to_vdp_addr(vram_addr_from_tileidx(free_tile)) | VRAM_W,
    res_ship_chr,
    1920 >> 1);

  bios_load_map(
    to_vdp_addr(BIOS_VDP_DEFAULT_PLANEA + VDP_PLANE_POS(0, 2, Width32)) |
      VRAM_W,
    res_cybercity_bldg_map[0] - 1,
    res_cybercity_bldg_map[1] - 1,
    res_cybercity_bldg_map + 2);
  bios_load_map(
    to_vdp_addr(BIOS_VDP_DEFAULT_PLANEB) | VRAM_W,
    res_cybercity_farbg_map[0] - 1,
    res_cybercity_farbg_map[1] - 1,
    res_cybercity_farbg_map + 2);

  bios_vdp_disp_enable();

  *bios_vblank_user = vblank_user;

  uf32 scroll_a = 0.0;
  uf32 scroll_b = 0.0;

  do
  {
    bios_vblank_handler_flags = BIOS_FLAG_COPY_SPRLIST;
    bios_vblank_wait_default();

    bios_process_entities(&sprobj_ship, bios_sprlist, 0, 0x1A);

    // scroll the background layers
    scroll_a -= frac_to_uf32(0.9);
    scroll_b -= frac_to_uf32(0.6);

    // (vdp auto increment is set to 2 so we only need to set the address once)
    vdp_ctrl_32 = vdpptr_scroll;
    vdp_data_16 = uf32_to_int(scroll_a);
    vdp_data_16 = uf32_to_int(scroll_b);

    if (bios_joy1_hold & PAD_RIGHT)
    {
      // ship_parts.pos_x += frac_to_uf32(3);
      sprobj_ship.display_flags = 0;
      sprobj_ship.move_x += frac_to_f32(0.2);
      if (sprobj_ship.move_x > frac_to_f32(4))
        sprobj_ship.move_x = frac_to_f32(4);
    }

    if (bios_joy1_hold & PAD_LEFT)
    {
      // ship_parts.pos_x -= frac_to_f32(3);
      sprobj_ship.display_flags = 0x80;
      sprobj_ship.move_x -= frac_to_f32(0.2);
      if (sprobj_ship.move_x < frac_to_f32(-4))
        sprobj_ship.move_x = frac_to_f32(-4);
    }

    if (bios_joy1_hold & PAD_UP)
    {
      sprobj_ship.pos_y -= frac_to_uf32(3);
    }

    if (bios_joy1_hold & PAD_DOWN)
    {
      sprobj_ship.pos_y += frac_to_uf32(3);
    }

  } while (1);
}

#include "main/bootlib.h"
#include "main/io.h"
#include "main/memmap.h"
#include "main/vdp.h"
#include "math.h"
#include "types.h"

// resource declarations
extern u8 res_cybercity_bldg_cmp_nem[];
extern u8 res_cybercity_farbg_cmp_nem[];
extern u16 const res_cybercity_pal[];

extern u16 res_cybercity_bldg_map[];
extern u16 res_cybercity_farbg_map[];
extern u8 res_ship_chr[];
extern u16 const res_ship_pal[];

extern struct SpriteLayout res_ship_layout;

struct SpriteObject sprobj_ship;

void null_func()
{
	return;
}

void vint_ex()
{
	blib_copy_sprlist();
}

void (*spr_funcs[2])();

const VDPPTR vdpptr_scroll = VDPPTR((_BLIB_HSCROL_ADDR)) | VRAM_W;

/*
	We mark this is as noreturn since this is an infinite loop that will
	never escape. Hopefully this will hint some optimizations to GCC.
	Unfortunately, it looks like GCC will still push the registers on entry,
	taking up valuable stack space. I have not found a way around this (the `naked`
	attribute is not available on m68k arch). Any suggestions to prevent this
	would be appreciated!
*/
__attribute__((noreturn)) void main()
{

	spr_funcs[0] = null_func;
	spr_funcs[1] = null_func;

	BLIB_SPR_JMPTBL_PTR = spr_funcs;

	sprobj_ship.enable = true;
	sprobj_ship.layout = &res_ship_layout;
	sprobj_ship.pos_x = 150 << 16;
	sprobj_ship.pos_y = 140 << 16;
	sprobj_ship.jmptbl_offset = 0;
	sprobj_ship.sprite_flags = 0x20;

	// turn off the display while we init
	blib_vdp_disp_disable();
	// boot rom defaults are fine...
	blib_load_vdpregs_default();

	// We use the PlaneWidthTiles enum to assign the value in *tiles*, which is
	// more intuitive than the count in nametable entries, which is what it expects
	// See the documentation on BLIB_PLANE_WIDTH for more
	BLIB_PLANE_WIDTH = Width32;

	// The load VDP regs Boot ROM routines expect a zero terminated array of
	// raw register values
	// See the documentation on _BLIB_LOAD_VDPREGS for more
	u16 vdp_planewidth_reg[] = {_VDPREG10_PL_SIZE | VR10_W32 | VR10_H64, 0};
	blib_load_vdpregs(vdp_planewidth_reg);

	// load the palettes
	// In general, use the VDPPTR macro for converting a VRAM address to the
	// VDP compatible format. It is written so that constant values will be
	// calculated at compile time, and variables will be calculated at runtime
	// with the optimized conversion code
	blib_dma_xfer_wrdram((VDPPTR(0) | CRAM_W), res_cybercity_pal, 32 >> 1);
	blib_dma_xfer_wrdram((VDPPTR(32) | CRAM_W), res_ship_pal, 32 >> 1);

	// blib_gfx_decomp requires that we set the VDP address first
	VDP_CTRL_32 = VDPPTR(VRAM_AT(1)) | VRAM_W;
	blib_gfx_decomp(res_cybercity_bldg_cmp_nem);

	// first word of Nemesis compression is the tile count, with the msb
	// determining XOR mode so we can use this (with the msb cleared) to get the
	// next free tile +1 to account for the blank 0 tile
	u16 free_tile = ((*(u16 *) res_cybercity_bldg_cmp_nem) & 0x7fff) + 1;

	VDP_CTRL_32 = VDPPTR(VRAM_AT(free_tile)) | VRAM_W;
	blib_gfx_decomp(res_cybercity_farbg_cmp_nem);

	free_tile += ((*(u16 *) res_cybercity_farbg_cmp_nem) & 0x7fff);

	blib_dma_xfer_wrdram(VDPPTR(VRAM_AT(free_tile)) | VRAM_W, res_ship_chr, 1920 >> 1);

	blib_load_map(VDPPTR(NMT_POS_PLANE(0, 2, _BLIB_PLANEA_ADDR)) | VRAM_W,
		res_cybercity_bldg_map[0] - 1,
		res_cybercity_bldg_map[1] - 1,
		res_cybercity_bldg_map + 2);
	blib_load_map(VDPPTR(_BLIB_PLANEB_ADDR) | VRAM_W,
		res_cybercity_farbg_map[0] - 1,
		res_cybercity_farbg_map[1] - 1,
		res_cybercity_farbg_map + 2);

	blib_vdp_disp_enable();

	*BLIB_VINT_EX_PTR = vint_ex;

	uf32 scroll_a = 0.0;
	uf32 scroll_b = 0.0;

	do
	{
		BLIB_VINT_FLAGS = COPY_SPRLIST_MSK;
		blib_vint_wait_default();

		blib_process_sprobjs(&sprobj_ship, BLIB_SPRLIST, 0, 0x1a);

		// scroll the background layers
		scroll_a -= frac_to_uf32(0.9);
		scroll_b -= frac_to_uf32(0.6);

		// (vdp auto increment is set to 2 so we only need to set the address once)
		VDP_CTRL_32 = vdpptr_scroll;
		VDP_DATA_16 = uf32_to_int(scroll_a);
		VDP_DATA_16 = uf32_to_int(scroll_b);

		if (BLIB_JOY1_HOLD & PAD_RIGHT_MSK)
		{
			// ship_parts.pos_x += frac_to_uf32(3);
			sprobj_ship.display_flags = 0;
			sprobj_ship.move_x += frac_to_f32(0.2);
			if (sprobj_ship.move_x > frac_to_f32(4))
				sprobj_ship.move_x = frac_to_f32(4);
		}

		if (BLIB_JOY1_HOLD & PAD_LEFT_MSK)
		{
			// ship_parts.pos_x -= frac_to_f32(3);
			sprobj_ship.display_flags = 0x80;
			sprobj_ship.move_x -= frac_to_f32(0.2);
			if (sprobj_ship.move_x < frac_to_f32(-4))
				sprobj_ship.move_x = frac_to_f32(-4);
		}

		if (BLIB_JOY1_HOLD & PAD_UP_MSK)
		{
			sprobj_ship.pos_y -= frac_to_uf32(3);
		}

		if (BLIB_JOY1_HOLD & PAD_DOWN_MSK)
		{
			sprobj_ship.pos_y += frac_to_uf32(3);
		}

	} while (1);
}

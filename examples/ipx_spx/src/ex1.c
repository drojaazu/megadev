#include "bridge.h"
#include "ipx.h"
#include <main/bios.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <system.h>

extern u8			 next_module;
extern u8			 res_rain_chr;
extern u16		 res_rain_chr_sz;
extern Palette res_rain_pal;

void main()
{
	disable_interrupts();
	bios_load_pal_update(&res_rain_pal);
	bios_dma_xfer_word_ram(
		vdp_ptr(VRAMPTR(0x80)), &res_rain_chr, res_rain_chr_sz >> 1);
	enable_interrupts();

	bios_print(
		"Module 1\xff",
		(vdp_ptr(PLANE_POS(1, 1, Width64) + BIOS_VDP_DEFAULT_PLANEA) | VRAM_W));

	// init_particles is defined in the ipx
	init_particles(0x81, 0x82, 0, 0, 0, 0, 3, 3, 5, 1);

	do
	{
		bios_vint_wait_default();
		// process_particles is defined in the ipx
		process_particles();
	} while (! (bios_joy1_hit & PAD_START));

	next_module = FILE_EX2_MMD;
	return;
}

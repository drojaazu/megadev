#include "bridge.h"
#include "ipx.h"
#include <main/bios.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <system.h>
#include <types.h>

extern u8 next_module;
extern u8 res_snow_chr;
extern u16 res_snow_chr_sz;
extern Palette res_snow_pal;

void main()
{
	disable_interrupts();
	bios_load_pal_update(&res_snow_pal);
	bios_dma_xfer_word_ram(VDPPTR(VRAM_AT(0x80)), &res_snow_chr, res_snow_chr_sz >> 1);
	enable_interrupts();

	bios_print("Module 2\xff", (VDPPTR(PLANE_POS(1, 1, Width64) + _BIOS_VDP_DEFAULT_PLANEA_ADDR) | VRAM_W));

	init_particles(0x81, 0x82, 0, 0, 0, 0, 45, 1, 2, 1);

	do
	{
		bios_vint_wait_default();
		process_particles();
	} while (! (bios_joy1_hit & PAD_START));

	next_module = FILE_EX3_MMD;
	return;
}

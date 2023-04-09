
#include "ipx.h"
#include "main/bootlib.h"
#include "main/io.def.h"
#include "main/memmap.h"
#include "main/vdp.h"
#include "system.h"
#include "types.h"

extern u8 global_mode;
extern u8 res_snow_chr;
extern u16 res_snow_chr_sz;
extern Palette res_snow_pal;

void main()
{
	disable_interrupts();
	blib_load_pal_update(&res_snow_pal);
	blib_dma_xfer_wrdram(VDPPTR(VRAM_AT(0x80)), &res_snow_chr, res_snow_chr_sz >> 1);
	enable_interrupts();

	blib_print("Module 2\xff", (VDPPTR(NMT_POS_PLANE(1, 1, _BLIB_PLANEA_ADDR)) | VRAM_W));

	init_particles(0x81, 0x82, 0, 0, 0, 0, 45, 1, 2, 1);

	do
	{
		blib_vint_wait_default();
		process_particles();
	} while (! (BLIB_JOY1_PRESS & PAD_START_MSK));

	global_mode = 2;
	return;
}

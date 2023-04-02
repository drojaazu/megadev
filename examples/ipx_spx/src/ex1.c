
#include "ipx.h"
#include "main/bootlib.h"
#include "main/io_def.h"
#include "main/memmap.h"
#include "main/vdp.h"
#include "system.h"
#include "types.h"

extern u8 global_mode;
extern u8 res_rain_chr;
extern u16 res_rain_chr_sz;
extern Palette res_rain_pal;

void main()
{
	disable_interrupts();
	blib_load_pal_update(&res_rain_pal);
	blib_dma_xfer_wrdram(VDPPTR(VRAM_AT(0x80)), &res_rain_chr, res_rain_chr_sz >> 1);
	enable_interrupts();

	blib_print("Module 1\xff", (VDPPTR(NMT_POS_PLANE(1, 1, _BLIB_PLANEA_ADDR)) | VRAM_W));

	// init_particles is defined in the ipx
	init_particles(0x81, 0x82, 0, 0, 0, 0, 3, 3, 5, 1);

	do
	{
		blib_vint_wait_default();
		// process_particles is defined in the ipx
		process_particles();
	} while (! (BLIB_JOY1_PRESS & PAD_START_MSK));

	global_mode = 1;
	return;
}

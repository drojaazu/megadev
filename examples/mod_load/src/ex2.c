
#include "io_def.h"
#include "main/bootlib.h"
#include "main/memmap.h"
#include "types.h"
#include "vdp.h"

extern u16 global_mode;

void main()
{
	blib_print ("Example file Number Two!\xff",
		(to_vdpptr (NMT_POS_PLANE (9, 6, _BLIB_PLANEA_ADDR)) | VRAM_W));

	do
	{
		blib_vint_wait_default();
	} while (! (BLIB_JOY1_PRESS & PAD_START_MSK));

	global_mode = 2;
	return;
}

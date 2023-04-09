
#include "main/bootlib.h"
#include "main/io.def.h"
#include "main/memmap.h"
#include "main/vdp.h"
#include "types.h"

extern u16 global_mode;

void main()
{
	blib_print("Example file Number Three!\xff", (to_vdpptr(NMT_POS_PLANE(10, 10, _BLIB_PLANEA_ADDR)) | VRAM_W));

	do
	{
		blib_vint_wait_default();
	} while (! (BLIB_JOY1_PRESS & PAD_START_MSK));

	global_mode = 0;
	return;
}

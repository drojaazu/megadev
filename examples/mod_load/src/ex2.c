
#include "io_def.h"
#include "main/bootlib.h"
#include "main/main.h"
#include "types.h"
#include "vdp.h"

extern u16 global_mode;

void main()
{
	boot_print ("Example file Number Two!\xff", (to_vdpptr (NMT_POS_PLANE (9, 6, _BLIB_PLANEA_ADDR)) | VRAM_W));

	do
	{
		boot_vint_wait_default();
	} while (! (*JOY1_PRESS & PAD_START_MSK));

	global_mode = 2;
	return;
}


#include <main/bios.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <types.h>

extern u16 global_mode;

void main()
{
	bios_print("Example file Number Three!\xff", (to_vdpptr(NMT_POS_PLANE(10, 10, _BIOS_PLANEA_ADDR)) | VRAM_W));

	do
	{
		bios_vint_wait_default();
	} while (! (BIOS_JOY1_PRESS & PAD_START));

	global_mode = 0;
	return;
}

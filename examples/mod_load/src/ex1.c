
#include <main/bios.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <types.h>

extern u16 global_mode;

void main()
{
	bios_print(
		"Example file Number One!\xff", (to_vdpptr(PLANE_POS_PLANE(4, 3, _BIOS_VDP_DEFAULT_PLANEA_ADDR)) | VRAM_W));

	do
	{
		bios_vint_wait_default();
	} while (! (bios_joy1_hit & PAD_START));

	global_mode = 1;
	return;
}

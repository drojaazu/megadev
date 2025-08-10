#include "ipx.h"
#include <fixed.h>
#include <main/bios.h>
#include <main/gate_arr.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/mmd.h>
#include <main/vdp.h>
#include <system.h>
#include <types.h>

u8 global_mode;

void play_pcm()
{
	*GA_COMCMD0 = 0x10;

	bios_print("Playing...\xff", (VDPPTR(_BIOS_VDP_PLANEA_ADDR + PLANE_POS(1, 2, Width64)) | VRAM_W));

	// wait for the playing flag to clear
	while (*GA_COMFLAGS_SUB & 0x80)
		;

	while (*GA_COMSTAT0 == 0)
		;
	*GA_COMCMD0 = 0;
	while (*GA_COMSTAT0 != 0)
		;

	bios_print("Done      \xff", (VDPPTR(_BIOS_VDP_PLANEA_ADDR + PLANE_POS(1, 2, Width64)) | VRAM_W));
}

void load_pcm()
{
	bios_print("Loading...\xff", (VDPPTR(_BIOS_VDP_PLANEA_ADDR + PLANE_POS(1, 2, Width64)) | VRAM_W));
	*GA_COMCMD0 = 2;
	while (*GA_COMSTAT0 == 0)
		;
	*GA_COMCMD0 = 0;
	while (*GA_COMSTAT0 != 0)
		;

	play_pcm();
}

void main()
{
	// basic setup stuff we've covered elsewhere
	MLEVEL6_VECTOR = (void *(*) ) _BIOS_VINT_HANDLER;
	enable_interrupts();
	bios_load_font_defaults();
	bios_palette[1] = 0xeee;
	bios_vdp_update_flags |= VDPUPDATE_PAL;

	bios_print("PCM Audio Playback\xff", (VDPPTR(_BIOS_VDP_PLANEA_ADDR + PLANE_POS(1, 1, Width64)) | VRAM_W));

	load_pcm();

	bios_print("Press A to replay\xff", (VDPPTR(_BIOS_VDP_PLANEA_ADDR + PLANE_POS(1, 4, Width64)) | VRAM_W));

	// main loop
	while (true)
	{
		bios_vint_wait_default();

		if ((bios_joy1_hit & PAD_A))
		{
			play_pcm();
		}
	}
}

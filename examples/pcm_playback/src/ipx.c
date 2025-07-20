#include <fixed.h>
#include <main/bios.h>
#include <main/gate_array.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/init_mmd.h>
#include <main/vdp.h>
#include <system.h>
#include <types.h>
#include "ipx.h"

u8 global_mode;

void play_pcm()
{
	*GA_COMCMD0 = 0x10;

	bios_print("Playing...\xff", (VDPPTR(NMT_POS_PLANE(1, 2, _BIOS_PLANEA_ADDR)) | VDPOP_VRAM_W));

	// wait for the playing flag to clear
	while (*GA_COMFLAGS_SUB & 0x80)
		;

	while (*GA_COMSTAT0 == 0)
		;
	*GA_COMCMD0 = 0;
	while (*GA_COMSTAT0 != 0)
		;

	bios_print("Done      \xff", (VDPPTR(NMT_POS_PLANE(1, 2, _BIOS_PLANEA_ADDR)) | VDPOP_VRAM_W));
}

void load_pcm()
{
	bios_print("Loading...\xff", (VDPPTR(NMT_POS_PLANE(1, 2, _BIOS_PLANEA_ADDR)) | VDPOP_VRAM_W));
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
	BIOS_PALETTE[1] = 0xeee;
	BIOS_VDP_UPDATE_FLAGS |= VDPUPDATE_PAL;

	bios_print("PCM Audio Playback\xff", (VDPPTR(NMT_POS_PLANE(1, 1, _BIOS_PLANEA_ADDR)) | VDPOP_VRAM_W));

	load_pcm();

	bios_print("Press A to replay\xff", (VDPPTR(NMT_POS_PLANE(1, 4, _BIOS_PLANEA_ADDR)) | VDPOP_VRAM_W));

	// main loop
	while (true)
	{
		bios_vint_wait_default();

		if ((BIOS_JOY1_PRESS & PAD_A))
		{
			play_pcm();
		}
	}
}

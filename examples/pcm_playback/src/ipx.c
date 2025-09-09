#include "bridge.h"
#include <fixed.h>
#include <main/bios.h>
#include <main/gate_arr.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <system.h>
#include <types.h>

void play_pcm()
{
	*gareg_comcmd0 = CMD_PLAY_PCM;

	bios_print(
		"Playing...\xff",
		(vdp_ptr(BIOS_VDP_DEFAULT_PLANEA + PLANE_POS(1, 2, Width64)) | VRAM_W));

	// wait for the playing flag to clear
	while (*GA_COMFLAGS_SUB & 0x80)
		;

	while (*GA_COMSTAT0 == 0)
		;
	*gareg_comcmd0 = 0;
	while (*GA_COMSTAT0 != 0)
		;

	bios_print(
		"Done      \xff",
		(vdp_ptr(BIOS_VDP_DEFAULT_PLANEA + PLANE_POS(1, 2, Width64)) | VRAM_W));
}

void load_pcm()
{
	bios_print(
		"Loading...\xff",
		(vdp_ptr(BIOS_VDP_DEFAULT_PLANEA + PLANE_POS(1, 2, Width64)) | VRAM_W));
	*gareg_comcmd0 = CMD_LOAD_PRGRAM;
	while (*GA_COMSTAT0 == 0)
		;
	*gareg_comcmd0 = 0;
	while (*GA_COMSTAT0 != 0)
		;

	play_pcm();
}

void main()
{
	bios_load_font_defaults();
	bios_palette[1] = 0xEEE;
	bios_vdp_update_flags |= BIOS_VDPUPDATE_COPY_PALETTE_FLAG;
	bios_vint_wait_default();

	bios_print(
		"PCM Audio Playback\xff",
		(vdp_ptr(BIOS_VDP_DEFAULT_PLANEA + PLANE_POS(1, 1, Width64)) | VRAM_W));

	load_pcm();

	bios_print(
		"Press A to replay\xff",
		(vdp_ptr(BIOS_VDP_DEFAULT_PLANEA + PLANE_POS(1, 4, Width64)) | VRAM_W));

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

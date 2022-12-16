#include "ipx.h"
#include "main/bootlib.h"
#include "main/gatearr.h"
#include "main/io_def.h"
#include "main/memmap.h"
#include "main/mmd_exec.h"
#include "main/vdp.h"
#include "system.h"

u8 global_mode;

void play_pcm()
{
	*GA_COMCMD0 = 0x10;

	blib_print ("Playing...\xff",
		(VDPPTR (NMT_POS_PLANE (1, 2, _BLIB_PLANEA_ADDR)) | VRAM_W));

	// wait for the playing flag to clear
	while (*GA_COMFLAGS_SUB & 0x80)
		;

	while (*GA_COMSTAT0 == 0)
		;
	*GA_COMCMD0 = 0;
	while (*GA_COMSTAT0 != 0)
		;

	blib_print ("Done      \xff",
		(VDPPTR (NMT_POS_PLANE (1, 2, _BLIB_PLANEA_ADDR)) | VRAM_W));
}

void load_pcm()
{
	blib_print ("Loading...\xff",
		(VDPPTR (NMT_POS_PLANE (1, 2, _BLIB_PLANEA_ADDR)) | VRAM_W));
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
	MLEVEL6_VECTOR = (void *(*) ) _BLIB_VINT_HANDLER;
	enable_interrupts();
	blib_load_font_defaults();
	BLIB_PALETTE[1] = 0xeee;
	BLIB_VDP_UPDATE_FLAGS |= PAL_UPDATE_MSK;

	blib_print ("PCM Audio Playback\xff",
		(VDPPTR (NMT_POS_PLANE (1, 1, _BLIB_PLANEA_ADDR)) | VRAM_W));

	load_pcm();

	blib_print ("Press A to replay\xff",
		(VDPPTR (NMT_POS_PLANE (1, 4, _BLIB_PLANEA_ADDR)) | VRAM_W));

	// main loop
	while (true)
	{
		blib_vint_wait_default();

		if ((BLIB_JOY1_PRESS & PAD_A_MSK))
		{
			play_pcm();
		}
	}
}

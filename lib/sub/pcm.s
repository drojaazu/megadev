/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 */
 
#include "sub/memmap_def.h"
#include "sub/pcm_def.h"
#include "macros.s"

.section .text

/**
 * @sa pcm_clear_ram_c
 * @clobber d0-d2/a6
 */
GLABEL pcm_clear_ram
	// disable channels
	move.b #0xff, _PCM_CDISABLE
	moveq #0, d0
	move.b #0x80, d1

1:move.b d1, _PCM_CTRL
	move.w #0xfff, d2
	lea _PCM_RAM + 1, a6

2:move.b #0, (a6)
	addq #2, a6
	dbf d2, 2b
	addq #1, d0
	addq #1, d1
	cmpi.w #0x10, d0
	blt.b 1b
	rts

/**
 * @sa pcm_config_channel_c
 * @param[in] D0.b Channel
 * @param[in] A5.l Pointer to array of channel settings
 * @clobber d0/a5-a6
 */
GLABEL pcm_config_channel
	lea _PCM_ENV, a6
	move.b d0, _PCM_CTRL
	moveq #6, d0
1:move.b (a5)+, (a6)
	// short pause to wait for the change to take effect
	.rept 4
		nop
	.endr
	addq #2, a6
	dbf d0, 1b
	rts

/*
GLABEL pcm_config_channel
	movem.l d2-d7, -(sp)
	lea 0xff0000, a6
	move.b d0, _PCM_CTRL
	addq #1, a6
	moveq #6, d7
1:move.b (a5)+, (a6)
	moveq #4, d4
2:
	dbf d4, 2b
	addq #2, a6
	dbf d7, 1b
	
	movem.l (sp)+, d2-d7
	rts

*/

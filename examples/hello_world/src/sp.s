/*
	This is the initial Sub CPU side code
  This example is too simple to have anything going on here!
	Check out one of the other examples for something more substantial.
*/

#include <macros.s>
#include <sub/bios.def.h>
#include <sub/sub.macro.s>

.section .text

GLABEL sp_init
	// it's important to drv_init/cdb_stat here even if bios already did it
	// otherwise there may be issues with CD audio track playback
	lea drv_init_tracklist, a0
	BIOSCALL #BIOS_DRV_INIT
	// loop until done reading the disc TOC
1:BIOSCALL #BIOS_CDB_STAT
	andi.b	#0xF0, (CDSTAT).w
	bne			1b
	CLEAR_COMM_REGS
  rts

drv_init_tracklist:
	.byte 1, 0xFF

GLABEL sp_int2
GLABEL sp_main
GLABEL sp_user
  rts


/*
	This is the initial Sub CPU side code
  This example is too simple to have anything going on here!
	Check out one of the other examples for something more substantial.
*/

#include "macros.s"
#include "sub/bios_def.h"
#include "sub/macros.s"

.section .text

GLABEL sp_init
	// it's important to drvinit/cdbstat here even if bios already did it
	// otherwise there may be issues with CD audio track playback
	lea drvinit_tracklist, a0
	CDBIOS #DRVINIT
	// loop until done reading the disc TOC
1:CDBIOS #CDBSTAT
	andi.b	#0xf0, (_CDSTAT).w
	bne			1b
	CLEAR_COMM_REGS
  rts

drvinit_tracklist:
	.byte 1, 0xff

GLABEL sp_int2
GLABEL sp_main
GLABEL sp_user
  rts


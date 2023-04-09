#include "sub/memmap.def.h"
#include "sub/sub.macro.s"
#include "sub/cdrom.def.h"
#include "sub/cdrom.macro.s"
#include "macros.s"

.section .text

.global sp_text_org
.equ sp_text_org, _sp_text_org

// Here we include the CD-ROM file access code
#include "sub/cdrom.s"
.section .text

/*
	sp_init
	This routine is called once automatically by the BIOS before level 2
	interrupts (INT2) have been enabled.
*/
GLABEL sp_init
	// it's important to drvinit/cdbstat here even if bios already did it
	// otherwise there may be issues with CD audio track playback
	lea drvinit_tracklist, a0
	CDBIOS #_BIOS_DRVINIT
	// loop until done reading the disc TOC
1:CDBIOS #_BIOS_CDBSTAT
	andi.b	#0xf0, (_CDSTAT).w
	bne			1b
  CLEAR_COMM_REGS
  // Put Word RAM into 2M mode and assert control of it
  andi.w	#~(MSK_GA_RET | MSK_GA_MODE), _GA_MEMMODE
  // This sets up the CD-ROM access loop with initial settings. It only needs
  // to be called once, here in sp_init
  INIT_ACC_LOOP
  rts

drvinit_tracklist:
	.byte 1, 0xff

/*
  sp_int2
  The CD-ROM access code works in a loop. PROCESS_ACC_LOOP is the "pump" that
	keeps that loop flowing. We will need to call it on every INT2, which is sent
	from the Main CPU on every vblank (VINT).
*/
GLABEL sp_int2
  PROCESS_ACC_LOOP
  rts

/*
  sp_main
	This is called automatically by the BIOS after level 2 interrupts have been
	enabled.
*/
GLABEL sp_main
	// First, we need to cache all the file metadata so we can just use filenames
  // when loading data instead of needing sector offsets
	// We place the load dir command in the access_op variable and wait for the
	// loop to finish
	move.w  #ACC_OP_LOAD_DIR, access_op
	WAIT_FOR_ACC_OP
	cmpi.w	#RESULT_OK, d0
	bne			sp_fatal

	// load spx
	lea spx_filename, a0
	lea 0x10000, a1
	jbsr		load_file_sub
	cmpi.w  #RESULT_OK, d0  // check final status
  bne     sp_fatal  // had an error, drop everything and freak out

	jbra 0x10000

spx_filename:
.asciz "SPX.SMD;1"
.align 2


// If something big blows up, we'll light up the LEDs to let us know there
// was a problem
GLABEL sp_fatal
	// move the "fatal error" code to comstat0 so the Main CPU knows what's up
	move.w #0xff, _GA_COMSTAT0
	// make both LEDs blink (which is normally disallowed but Sega QA isn't
	// here to boss us around)
	moveq	#_LED_ERROR, d1
  CDBIOS #_BIOS_LEDSET
0:nop
	nop
	bra 0b

GLABEL sp_user
  rts

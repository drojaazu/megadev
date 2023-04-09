#include "sub/memmap.def.h"
#include "sub/sub.macro.s"
#include "sub/cdrom.def.h"
#include "sub/cdrom.macro.s"
#include "macros.s"

.section .text

// Here we include the CD-ROM file access code
#include "sub/cdrom.s"
.section .text
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
  sp_main
	This is called automatically by the BIOS after level 2 interrupts have been
	enabled. We'll put the majority of our SP code here, including the main loop.
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

  /*
	  The architecture of the command loop is like so: we constantly poll COMCMD0
		for a command sent from the Main CPU. When the value is not 0, we'll also
		pull the value in COMCMD1, which acts as a parameter. We'll then jump to the
		code for the matching command and pass the parameter as well. When that work
		is done, we'll inform the Main CPU that we're done over here and go back to
		monitoring COMCMD0/1
	*/
command_loop:
  move.w	_GA_COMCMD0, d0
	beq			command_loop
	cmp.w		_GA_COMCMD0, d0
	bne			command_loop
	#move.w	d0, d1
	move.w	_GA_COMCMD1, d1
	add.w		d0, d0
	move.w	command_tbl(pc,d0.w), d0
	jsr			command_tbl(pc,d0.w)
	//bclr		#INT1_GFX_BIT, GA_INTMASK+1
	jra			command_loop

/*
  Each command will branch here when it is complete in order to inform the
	Main CPU the work is done and get the comm registers back in "sync."
*/
command_complete_sync:
	move.w	_GA_COMCMD0, _GA_COMSTAT0
1:move.w	_GA_COMCMD0, d0
	bne			1b
	move.w	_GA_COMCMD0, d0
	bne			1b
	move.w	#0, _GA_COMSTAT0
	rts

/*
  The first entry is just a hardcoded value since there is no "command 0". A
	zero in COMCMD0 is just idle, and the "beq command_loop" ensures that we'll
	never get to the jump with "command 0".
*/
command_tbl:
	.word 0
	.word	cmd01_load_mmd-command_tbl
	.word	cmd02_play_cdda-command_tbl
	rts

/*
  Command 1 - Load MMD module
	The parameter (passed in from COMCMD1) will specify which file in particular
	to load. We'll put the data at the start of Word RAM and hand it over to the
	the Main CPU.
*/
cmd01_load_mmd:
  add.w d1,d1
	move.w  mmd_file_tbl(pc,d1.w), d1
	lea			mmd_file_tbl(pc,d1.w), a0
	WAIT_2M
	lea			_WRDRAM_2M, a1
	// load_file_ is a convenience function to get a file loaded. The filename is
	// A0 and the destination buffer is in A1
	jbsr		load_file_sub
	cmpi.w  #RESULT_OK, d0  // check final status
  bne     sp_fatal  // had an error, drop everything and freak out
	GRANT_2M
	bra		command_complete_sync

mmd_file_tbl:
  .word mmd_ex1-mmd_file_tbl
  .word mmd_ex2-mmd_file_tbl
	.word mmd_ex3-mmd_file_tbl

/*
	Command 2 - Play CD audio
	The requested audio track will be in d1 (from COMCMD1)
*/
cmd02_play_cdda:
	add.w d1, d1
	lea	cd_track(pc,d1.w), a0
	move.w	#0x400, d1
	CDBIOS #_BIOS_FDRSET
	CDBIOS #_BIOS_MSCPLAYR
	bra			command_complete_sync
cd_track:
	.word 2
	.word 3
	.word 4
	.word 5
	.word 6

GLABEL sp_user
  rts

// If something big blows up, we'll light up the LEDs to let us know there
// was a problem
sp_fatal:
	// make both LEDs blink (which is normally disallowed but Sega QA isn't
	// here to boss us around)
	moveq	#_LED_ERROR, d1
  CDBIOS #_BIOS_LEDSET
0:nop
	nop
	bra 0b

.section .rodata

/*
  When making your list of filenames to references, don't forget to:
	a) use capital letters only!
	b) include the version id at the end (affix ;1 to the end of the filename)!
	c) include an alignment after the filename to prevent address exceptions!
*/
mmd_ex1:
   .asciz "EX1.MMD;1"
  .align 2
mmd_ex2:
  .asciz "EX2.MMD;1"
  .align 2
mmd_ex3:
  .asciz "EX3.MMD;1"
  .align 2

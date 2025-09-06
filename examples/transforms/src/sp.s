#include <sub/sub.macro.s>
#include <sub/bios.def.h>
#include <sub/cdrom.macro.s>
#include <macros.s>

.section .text

/*
  Initialization on the Sub CPU side is a little more involved than the Main side.
  The CD System Program (the system code running on the Sub side, which encompasses
  the BIOS) requires a jump table with four entries, pointing to routines called at
  certain points during startup and runtime. (See the MEGA-CD BIOS MANUAL documentation
  for more specifics.)

  Megadev will automatically set up the header and the jump table, but the four entries
  in that table must have corresponding subroutines with the following names:

    - sp_init
    - sp_main
    - sp_int2
    - sp_user
*/

/*
  sp_init
  This routine is called once automatically by the BIOS before level 2
  interrupts (INT2) have been enabled.
*/
GLABEL sp_init

  // Clear out RAM
  moveq    #0, d0
  move.l   #_BSS_LENGTH, d1
  lea      _BSS_ORIGIN, a0
  bra 1f
0:move.l   d0, (a0)+
1:dbra     d1, 0b

  /*
    Per the documentation and disassemblies, drvinit/cdbstat should be called here
    even though already did it, otherwise there may be issues with CD audio track playback
  */
  lea drvinit_tracklist, a0
  BIOSCALL   #BIOS_DRVINIT
  // loop until done reading the disc TOC
1:BIOSCALL   #BIOS_CDBSTAT
  andi.b   #0xf0, (CDSTAT).w
  bne      1b
  CLEAR_COMM_REGS
  
  // Put Word RAM into 2M mode and assert control of it
  andi.w   #~(GA_RET | GA_MODE), _GAREG_MEMMODE
  
  // This sets up the CD-ROM access loop with initial settings. It only needs
  // to be called once, here in sp_init
  INIT_ACC_LOOP
  rts

drvinit_tracklist:
  .byte 1, 0xff

/*
  sp_int2
  INT2 is triggered from the Main CPU side on every VBLANK interrupt.

  The CD-ROM access code works in a loop. PROCESS_ACC_LOOP is the "pump" that
  keeps that loop flowing.
*/
GLABEL sp_int2
  PROCESS_ACC_LOOP
  rts

/*
  sp_main
  This is called once automatically by the BIOS after level 2 interrupts have been
  enabled. It is essentially the entry point to our main program on the Sub side.
*/
GLABEL sp_main
  // First, we need to cache all the file metadata so we can just use filenames
  // when loading data instead of needing sector offsets
  // We place the load dir command in the access_op variable and wait for the
  // loop to finish
  move.w   #CDROM_LOAD_FILE_LIST, access_op
  WAIT_FOR_ACC_OP
  cmpi.w   #CDROM_RESULT_OK, d0
  bne      sp_fatal

  // load spx
  lea      spx_filename, a0
  lea      0x10000, a1
  jbsr     load_file_sub
  cmpi.w   #CDROM_RESULT_OK, d0  // check final status
  bne      sp_fatal  // had an error, drop everything and freak out
  jbra     0x10000

spx_filename:
.asciz "SPX.SMD;1"
.align 2


// If something big blows up, we'll light up the LEDs to let us know there
// was a problem
GLABEL sp_fatal
  // move the "fatal error" code to comstat0 so the Main CPU knows what's up
  move.w   #0xff, _GAREG_COMSTAT0
  // make both LEDs blink (which is normally disallowed but Sega QA isn't
  // here to boss us around)
  moveq    #BIOS_LED_ERROR, d1
  BIOSCALL   BIOS_LEDSET
0:nop
  nop
  bra      0b

GLABEL sp_user
  rts

// Here we include the CD-ROM file access code
#include <sub/cdrom.s>

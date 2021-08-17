#include "main/main_def.h"
#include "main/bootlib_def.h"
#include "main/main_macros.s"
#include "vdp_def.h"
#include "macros.s"

.section .text

// the security code for the region must *always* be at the very beginning of
// the IP code
  .incbin "sec_us.bin"

ip_entry:
  // First, disable all interrupts while we get things set up
  ori #0x700,sr

  move.l	#BOOT_VINT, (_MLEVEL6 + 2)

  jbsr BOOT_LOAD_VDPREGS_DEFAULT

  jbsr BOOT_CLEAR_VRAM

  jbsr BOOT_LOAD_FONT_DEFAULTS
  
  // The font uses palette entry #1, so we'll manually set that to white
  move.l #0xC0020000, (_VDP_CTRL)
  move.w #0x0EEE, (_VDP_DATA)

  // And finally enable the display
  jbsr BOOT_VDP_DISP_ENABLE

  jbsr BOOT_CLEAR_COMM

  // and restore interrupts
  andi #0xF8FF,sr

  move.w #0, (global_mode)

prep_load:
  movea.l (0), sp  // Reset the stack since we're "starting fresh"

  jbsr BOOT_CLEAR_NMTBL // clear the screen

  GRANT_2M  // give Word RAM to Sub

  move.w	#1, _GA_COMCMD0	//send the command to sub
	move.w  (global_mode), _GA_COMCMD1  // send the param to sub
0:tst.w		_GA_COMSTAT0			//wait for response on status reg #0
	beq			0b
	move.w	#0, _GA_COMCMD0	//send idle command
1:tst.w		_GA_COMSTAT0			//wait for response (wait for 0 from Sub)
	bne			1b

  jbsr MMD_EXEC  // launch the loaded module
  bra prep_load

// Include the code for the MMD loader here
#include "mmd_exec.s"

.section .bss
.global global_mode
global_mode: .word 0


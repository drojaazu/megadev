
// Boot security block - This must be at the top of your IP!
#include <security.s>

.section .text

#include <main/memmap.def.h>
#include <main/bios.def.h>
#include <main/main.macro.s>
#include <main/vdp.def.h>
#include <main/gate_arr.macros.s>
#include <macros.s>


ip_entry:
  // First, disable all interrupts while we get things set up
  ori #0x700,sr

  move.l	#_BIOS_VINT_HANDLER, (_MLEVEL6 + 2)

  jbsr _BIOS_LOAD_VDPREGS_DEFAULT

  jbsr _BIOS_CLEAR_VRAM

  jbsr _BIOS_LOAD_FONT_DEFAULTS
  
  // The font uses palette entry #1, so we'll manually set that to white
  move.l #0xC0020000, (_VDP_CTRL)
  move.w #0x0EEE, (_VDP_DATA)

  // And finally enable the display
  jbsr _BIOS_VDP_DISP_ENABLE

  jbsr _BIOS_CLEAR_COMM

  // and restore interrupts
  andi #0xF8FF,sr

  move.w #0, (global_mode)

prep_load:
  movea.l (0), sp  // Reset the stack since we're "starting fresh"

  jbsr _BIOS_CLEAR_TABLES // clear the screen

  GRANT_2M  // give Word RAM to Sub

  move.w	#1, _GAREG_COMCMD0	//send the command to sub
	move.w  (global_mode), _GAREG_COMCMD1  // send the param to sub
0:tst.w		_GAREG_COMSTAT0			//wait for response on status reg #0
	beq			0b
	move.w	#0, _GAREG_COMCMD0	//send idle command
1:tst.w		_GAREG_COMSTAT0			//wait for response (wait for 0 from Sub)
	bne			1b

  jbsr INIT_MMD  // launch the loaded module
  bra prep_load

// Include the code for the MMD loader here
#include "main/mmd.macros.s"

.section .bss
.global global_mode
global_mode: .word 0


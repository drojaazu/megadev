
.section .text

#include <main/main.def.h>
#include <main/bios.def.h>
#include <main/gate_arr.macros.s>
#include <main/main.macro.s>
#include <macros.s>
#include <system.macros.s>

ip_entry:
  // First, disable all interrupts while we do some basic init
  DISABLE_INTERRUPTS

  // clear ram
  moveq    #0, d0
  move.l   #_BSS_LENGTH, d1
  lea      _BSS_ORIGIN, a0
  bra 1f
0:move.l   d0, (a0)+
1:dbra     d1, 0b

  // disable VDP display and set Mega Drive video mode
  move.w   #(_VDPREG_MODE2 | 0x44), (_VDP_CTRL)

  // clear out VRAM
  // (note: this does not clear CRAM!)
  // This is a Boot ROM library call that makes use of the VDP register cache
  // Even if you don't plan to use the Boot ROM library, this call is safe
  // to use here as the memory for the register cache is not yet in use
  jbsr     _BIOS_CLEAR_VRAM

  // set palette entry 0 (background) to black
  move.l   #0xC0000000, (_VDP_CTRL)
  move.w   #0x0000, (_VDP_DATA)

  // clear the Gate Array communication registers
  CLEAR_COMM_REGS

  // point the VINT vector to the simple one within the Boot Library
  move.l   #_BIOS_VINT_HANDLER, (_MLEVEL6 + 2)

  // And finally enable the display
  jbsr     _BIOS_VDP_DISP_ENABLE

  // our example IP here is super tiny, and while it should remain quite
  // small, you could put a very simple message/graphic here to indicate
  // the game is starting up

  // restore interrupts to allow the cdrom data to flow
  ENABLE_INTERRUPTS

  GRANT_2M  // give Word RAM to Sub
  move.w	#0xfe, _GAREG_COMCMD0	//send the load IPX command to sub
0:tst.w		_GAREG_COMSTAT0				//wait for response on status reg #0
	beq			0b
	move.w	#0, _GAREG_COMCMD0			//send ack
1:tst.w		_GAREG_COMSTAT0				//wait for response (wait for 0 from Sub)
	bne			1b
  WAIT_2M

  // Everything is almost ready to go, so let's re-enable the display
  jbsr     _BIOS_VDP_DISP_ENABLE

  // Reset the stack since we're starting fresh
  movea.l  (0), sp

  // the initial module is a special case, so we won't use the standard MMD loader
  // (doing so would cause the mmd loader code to be overwritten as the
  // ipx is copied into work ram (where we are now) and everything would
  // fall apart)
  // instead, we'll jump right into the IPX entry currently in Word RAM
  // which will copy itself into Work RAM
  jbra     _WORD_RAM + 0x100

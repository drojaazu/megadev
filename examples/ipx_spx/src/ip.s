
// Boot security block - This must be at the top of your IP!
#include "sec_check.s"

.section .text

#include "main/gatearr_macros.s"
#include "main/memmap_def.h"
#include "main/bootlib_def.h"
#include "main/macros.s"
#include "main/vdp_def.h"
#include "macros.s"
#include "ipx_layout.s"

ip_entry:
  // First, disable all interrupts while we do some basic init
  ori #0x700,sr
  
	// clear ram
  moveq #0, d0
  move.l #(MODULE_RAM_LENGTH / 4), d7
  lea MODULE_RAM_ORIGIN, a0
0:move.l d0, (a0)+
  dbra d7, 0b

  // set palette entry 0 (background) to black
  move.l #0xC0000000, (_VDP_CTRL)
  move.w #0x0000, (_VDP_DATA)

  // disable VDP display and maintain MD mode (mode 5)
  move.w #(_VDP_MODE2 | 0x44), (_VDP_CTRL)

	jbsr _BLIB_LOAD_VDPREGS_DEFAULT

  // clear out VRAM
  // (note: this does not clear CRAM!)
	// This is a Boot ROM library call that makes use of the VDP register cache
	// Even if you don't plan to use the Boot ROM library, this call is safe
	// to use here as the memory will not be preserved after we jump to the IPX
  jbsr _BLIB_CLEAR_VRAM

	// our example IP here is super tiny, and while it should remain quite
	// small, you could put a very simple message/graphic here to indicate
	// the game is starting up

  // clear the Gate Array communication registers
  CLEAR_COMM_REGS

  // point VINT vector to the minimal, temporary handler
	// (note that we use _MLEVEL6 *+ 2*, as the first two bytes are
	// a jmp/bra opcode)
	move.l #vint_temp, (_MLEVEL6 + 2)

  // restore interrupts to allow the cdrom data to flow
	andi #0xF8FF,sr

  // make IPX file request to SP
	GRANT_2M
  move.w	#0xfe, _GA_COMCMD0	//send the load IPX command to sub
0:tst.w		_GA_COMSTAT0				//wait for response on status reg #0
	beq			0b
	move.w	#0, _GA_COMCMD0			//send ack
1:tst.w		_GA_COMSTAT0				//wait for response (wait for 0 from Sub)
	bne			1b
	WAIT_2M

	jbsr _BLIB_VDP_DISP_ENABLE

	// Reset the stack since we're starting fresh
	movea.l (0), sp

	// the IPX is a special case, so we won't use the standard MMD loader
	// (doing so would cause the mmd loader code to be overwritten as the
	// ipx is copied into work ram (where we are now) and everything would
	// fall apart)
	// instead, we'll jump right into the IPX entry currently in Word RAM
	// which will copy itself into Work RAM
	jbra _WRDRAM + 0x100

  // minimal VINT handler
	// the sub cpu must receive level 2 interrupts in order to keep
	// the cdrom subsystem moving
	// so we need this tiny li'l vint handler to make it happen and
	// get the ipx loaded
  vint_temp:
    bset.b #GA_RESET_IFL2_BIT, (_GA_RESET)
    rte

	.align 2

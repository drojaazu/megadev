
.section .text

#include <main/main.def.h>
#include <main/bios.def.h>
#include <main/gate_arr.macros.s>
#include <main/gate_arr.def.h>
#include <main/vdp.def.h>
#include <macros.s>
#include <system.macros.s>
#include "mmd_layout.s"
#include "shared.h"

  // First, disable all interrupts while we do some basic init
  DISABLE_INTERRUPTS

  // clear ram
  moveq    #0, d0
  move.l   #_BSS_LENGTH, d1
  lea      _BSS_ORIGIN, a0
  bra 1f
0:move.l   d0, (a0)+
1:dbra     d1, 0b

  // Next, begin to initialiaze video output (the VDP)
  // We will use the default VDP settings provided by the Main BIOS
  // See the comments for BIOS_LOAD_DEFAULT_VDPREGS for details on what those settings are.
  jbsr     BIOS_LOAD_DEFAULT_VDPREGS

  // Clear out VRAM in case there's any junk left over after the system startup graphics
  // (note: this does not clear CRAM!)
  // This is a BIOS call that makes use of the VDP register cache
  // Even if you don't plan to use the Main BIOS library, this call is safe
  // to use here as the memory will not be preserved after we jump to the IPX
  jbsr     BIOS_CLEAR_VRAM

  // Clear the Gate Array communication registers
  // Again, we conveniently have a BIOS call to take of this
  jbsr     BIOS_CLEAR_COMM

  // point VBLANK vector to the minimal, temporary handler
  // We need to set up the VBLANK interrupt (VBLANK) handler before we can turn interrupts back on.
  // (Which needs to happen soon, as this keeps the Sub CPU on the Mega CD side in "sync" by alerting
  // it when a VBLANK occurs.)
  // Once again, the built-in BIOS has a handler that can help us. It uses a couple of bits in the
  // BIOS_VBLANK_HANDLER_FLAGS variable to call an user function and to update the sprite table (neither of which
  // we need right now, so we should make sure that is cleared out.)
  // We set the pointer to the handler in the system vector jump table. The +2 is because the first two
  // bytes are the JMP opcode and we want to set the address to which it jumps.
  move     #0, (BIOS_VBLANK_HANDLER_FLAGS)
  move.l   #BIOS_VBLANK_HANDLER, (EXVEC_LEVEL6)

  // Restore interrupts to allow VBLANKs to fire and ultimately allow CD-ROM data to flow
  ENABLE_INTERRUPTS

  GRANT_2M  // give Word RAM to Sub
  move.w   #FILE_IPX, GA_REG_COMCMD1  // send the param to sub
  move.w   #CMD_LOAD_FILE, GA_REG_COMCMD0	//send the command to sub
0:tst.w    GA_REG_COMSTAT0			//wait for response on status reg #0
  beq      0b
  move.w   #0, GA_REG_COMCMD0	//send idle command
1:tst.w    GA_REG_COMSTAT0			//wait for response (wait for 0 from Sub)
  bne      1b
  WAIT_2M

  // Everything is almost ready to go, so let's re-enable the display
  jbsr     BIOS_VDP_DISP_ENABLE



  // Reset the stack since we're starting fresh
  movea.l  (0), sp

  // the initial module is a special case, so we won't use the standard MMD loader
  // (doing so would cause the mmd loader code to be overwritten as the
  // ipx is copied into work ram (where we are now) and everything would
  // fall apart)
  // instead, we'll jump right into the IPX entry currently in Word RAM
  // which will copy itself into Work RAM
  jbra     WORD_RAM + 0x100

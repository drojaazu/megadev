
.section .text

#include <main/main.def.h>
#include <main/gate_arr.macros.s>
#include <main/bios.def.h>
#include <main/main.macro.s>
#include <macros.s>
#include <system.macros.s>
#include "ipx_layout.s"
#include "shared.h"

  // First, disable all interrupts while we do some basic init
  DISABLE_INTERRUPTS
  
  // Clear out RAM used by the IP
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

  // point VINT vector to the minimal, temporary handler
  // We need to set up the VBLANK interrupt (VINT) handler before we can turn interrupts back on.
  // (Which needs to happen soon, as this keeps the Sub CPU on the Mega CD side in "sync" by alerting
  // it when a VBLANK occurs.)
  // Once again, the built-in BIOS has a handler that can help us. It uses a couple of bits in the
  // BIOS_VINT_HANDLER_FLAGS variable to call an user function and to update the sprite table (neither of which
  // we need right now, so we should make sure that is cleared out.)
  // We set the pointer to the handler in the system vector jump table. The +2 is because the first two
  // bytes are the JMP opcode and we want to set the address to which it jumps.
  move     #0, (BIOS_VINT_HANDLER_FLAGS)
  move.l   #BIOS_VINT_HANDLER, (EXVEC_LEVEL6)

  // Restore interrupts to allow VINTs to fire and ultimately allow CD-ROM data to flow
  ENABLE_INTERRUPTS

  // From here, we load our first "real" file from disc and jump to its code. You can modify this to change
  // e.g. the value sent to COMCMD0, but it should ultimately accomplish the same task of loading the first file.

  // This is the basic pattern for sending commands to the Sub CPU:
  //   - Give Word RAM access to the Sub side so it can put data in there
  //   - Set values in COMCMD registers (the Sub side is monitoring for values in these registers)
  //   - Check COMSTAT in a tight loop for a non-zero value (the response from Sub side)
  //   - Clear the COMCMD registers as the ACK to the Sub side
  //   - Check COMSTAT in a tight loop again to confirm everything is done
  //   - Wait for Word RAM ownership
  //   - Enjoy your freshly transferred data
  GRANT_2M
  move.w   #FILE_IPX_MMD, GAREG_COMCMD1
  move.w   #CMD_LOAD_FILE, GAREG_COMCMD0	//send the load IPX command to sub
0:tst.w    GAREG_COMSTAT0				//wait for response on status reg #0
  beq      0b
  move.w   #0, GAREG_COMCMD0			//send ack
1:tst.w    GAREG_COMSTAT0				//wait for response (wait for 0 from Sub)
  bne      1b
  WAIT_2M

  // Everything is almost ready to go, so let's re-enable the display
  jbsr     BIOS_VDP_DISP_ENABLE

  // Reset the stack since we're starting fresh
  movea.l  (0), sp

  // The last step is a little tricky.
  // The IPX is now present in Word RAM and since it's a module, we want to call the module loader to get it
  // to where it is expecting to run in memory. The IPX is configured to run from Work RAM, which is where we
  // in the IP are currently. Which means if we include the loader in the IP and run it from here... it will
  // eventually be overwritten as it copies over itself! Which causes a system crash.

  // The solution then is to include the loader in the IPX and have the code in its .init section call the
  // loader on itself. We can guarantee the code in .init will be at the very start of the module, which is
  // at offset 0x100 from the start of the file.

  // So... with the file in Word RAM, we jump directly to offset 0x100 and let it copy itself over and jump
  // to its own main().
  jbra     WORD_RAM + 0x100

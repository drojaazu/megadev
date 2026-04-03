
.section .text

#include <main/main.def.h>
#include <main/bios.def.h>
#include <main/gate_arr.macros.s>
#include <main/main.macro.s>
#include <macros.s>
#include <system.macros.s>
#include "shared.h"

/*
  Please see the ip.s file in the ipx_spx project for details about the process below.
*/

  DISABLE_INTERRUPTS

  // clear ram
  moveq    #0, d0
  move.l   #_BSS_LENGTH, d1
  lea      _BSS_ORIGIN, a0
  bra 1f
0:move.l   d0, (a0)+
1:dbra     d1, 0b

  jbsr     BIOS_LOAD_DEFAULT_VDPREGS
  jbsr     BIOS_CLEAR_VRAM
  jbsr     BIOS_CLEAR_COMM

  move     #0, (BIOS_VBLANK_HANDLER_FLAGS)
  move.l   #BIOS_VBLANK_HANDLER, (EXVEC_LEVEL6)

  ENABLE_INTERRUPTS

  GRANT_2M
  move.w   #FILE_IPX_MMD, GAREG_COMCMD1
  move.w   #CMD_LOAD_WORDRAM, GAREG_COMCMD0
0:tst.w    GAREG_COMSTAT0
  beq      0b
  move.w   #0, GAREG_COMCMD0
1:tst.w    GAREG_COMSTAT0
  bne      1b
  WAIT_2M

  jbsr     BIOS_VDP_DISP_ENABLE

  // Reset the stack since we're starting fresh
  movea.l  (0), sp

  jbra     WORD_RAM + 0x100

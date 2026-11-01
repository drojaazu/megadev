/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file exvec_sub.s
 * @brief Sub CPU crash handler (sub/exvec.h)
 */

#include "macro.s"
#include "sub/memmap.def.h"
#include "sub/gate_arr.def.h"
#include "exvec.def.h"

.section .text

.macro EXVEC_ENTRY name, id, frame
SUB \name
  move.w  #\id, -(sp)
  jbra    exvec_capture_\frame
.endm

EXVEC_ENTRY exvec_address_error,  EXVEC_ID_ADDRESS_ERROR,  group0
EXVEC_ENTRY exvec_illegal,        EXVEC_ID_ILLEGAL,        short
EXVEC_ENTRY exvec_zero_divide,    EXVEC_ID_ZERO_DIVIDE,    short
EXVEC_ENTRY exvec_chk,            EXVEC_ID_CHK,            short
EXVEC_ENTRY exvec_trapv,          EXVEC_ID_TRAPV,          short
EXVEC_ENTRY exvec_privilege,      EXVEC_ID_PRIVILEGE,      short
EXVEC_ENTRY exvec_trace,          EXVEC_ID_TRACE,          short
EXVEC_ENTRY exvec_line_1010,      EXVEC_ID_LINE_1010,      short
EXVEC_ENTRY exvec_line_1111,      EXVEC_ID_LINE_1111,      short

exvec_capture_group0:
  ori.w   #0x0700, sr
  movem.l d0-d7/a0-a6, (exvec_sub_snapshot + EXVEC_SNAP_D)
  lea     exvec_sub_snapshot, a0
  move.w  (sp)+, EXVEC_SNAP_VECTOR(a0)
  move.w  (sp)+, EXVEC_SNAP_ACCESS_INFO(a0)
  move.l  (sp)+, EXVEC_SNAP_ACCESS_ADDR(a0)
  move.w  (sp)+, EXVEC_SNAP_IR(a0)
  bra.s   exvec_capture_common

exvec_capture_short:
  ori.w   #0x0700, sr
  movem.l d0-d7/a0-a6, (exvec_sub_snapshot + EXVEC_SNAP_D)
  lea     exvec_sub_snapshot, a0
  move.w  (sp)+, EXVEC_SNAP_VECTOR(a0)
  clr.l   EXVEC_SNAP_ACCESS_ADDR(a0)
  clr.l   EXVEC_SNAP_ACCESS_INFO(a0)  // and IR

exvec_capture_common:
  move.w  (sp)+, EXVEC_SNAP_SR(a0)
  move.l  (sp)+, EXVEC_SNAP_PC(a0)
  move.l  sp, (EXVEC_SNAP_A + 28)(a0)
  move.l  sp, d0
  btst    #0, d0
  bne.s   1f
  movea.l sp, a1
  lea     EXVEC_SNAP_STACK(a0), a2
  moveq   #(EXVEC_STACK_LONGS - 1), d0
0:move.l  (a1)+, (a2)+
  dbra    d0, 0b
  // tell the Main CPU where the snapshot is, the signal last
1:move.l  a0, d0
  move.w  d0, (GA_SUB_REG_COMSTAT7)
  swap    d0
  move.w  d0, (GA_SUB_REG_COMSTAT6)
  move.w  #EXVEC_SUB_SIGNAL, (GA_SUB_REG_COMSTAT5)
2:stop    #0x2700
  bra.s   2b

/**
 * @fn exvec_install
 * @brief Install the handlers in the Sub BIOS jump table (sub/exvec.h)
 */
SUB exvec_install
  move.l  #exvec_address_error, (EXVEC_ADDRERR)
  move.l  #exvec_illegal, (EXVEC_ILLEGAL)
  move.l  #exvec_zero_divide, (EXVEC_ZERODIV)
  move.l  #exvec_chk, (EXVEC_CHK)
  move.l  #exvec_trapv, (EXVEC_TRAPV)
  move.l  #exvec_privilege, (EXVEC_PRIVERR)
  move.l  #exvec_trace, (EXVEC_TRACE)
  move.l  #exvec_line_1010, (EXVEC_LINE1010)
  move.l  #exvec_line_1111, (EXVEC_LINE1111)
  rts

.section .bss
.align 4
exvec_sub_snapshot: .space EXVEC_SNAP_SIZE

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file crash_sub.s
 * @brief Sub CPU crash handler (sub/crash.h)
 */

#include "macro.s"
#include "sub/memmap.def.h"
#include "sub/gate_arr.def.h"
#include "crash.def.h"

.section .text

.macro CRASH_ENTRY name, id, frame
SUB \name
  move.w  #\id, -(sp)
  jbra    crash_capture_\frame
.endm

CRASH_ENTRY exvec_address_error,  EXVEC_ID_ADDRESS_ERROR,  group0
CRASH_ENTRY exvec_illegal,        EXVEC_ID_ILLEGAL,        short
CRASH_ENTRY exvec_zero_divide,    EXVEC_ID_ZERO_DIVIDE,    short
CRASH_ENTRY exvec_chk,            EXVEC_ID_CHK,            short
CRASH_ENTRY exvec_trapv,          EXVEC_ID_TRAPV,          short
CRASH_ENTRY exvec_privilege,      EXVEC_ID_PRIVILEGE,      short
CRASH_ENTRY exvec_trace,          EXVEC_ID_TRACE,          short
CRASH_ENTRY exvec_line_1010,      EXVEC_ID_LINE_1010,      short
CRASH_ENTRY exvec_line_1111,      EXVEC_ID_LINE_1111,      short

crash_capture_group0:
  ori.w   #0x0700, sr
  movem.l d0-d7/a0-a6, (crash_sub_snapshot + CRASH_D)
  lea     crash_sub_snapshot, a0
  move.w  (sp)+, CRASH_VECTOR(a0)
  move.w  (sp)+, CRASH_ACCESS_INFO(a0)
  move.l  (sp)+, CRASH_ACCESS_ADDR(a0)
  move.w  (sp)+, CRASH_IR(a0)
  bra.s   crash_capture_common

crash_capture_short:
  ori.w   #0x0700, sr
  movem.l d0-d7/a0-a6, (crash_sub_snapshot + CRASH_D)
  lea     crash_sub_snapshot, a0
  move.w  (sp)+, CRASH_VECTOR(a0)
  clr.l   CRASH_ACCESS_ADDR(a0)
  clr.l   CRASH_ACCESS_INFO(a0)  // and IR

crash_capture_common:
  move.w  (sp)+, CRASH_SR(a0)
  move.l  (sp)+, CRASH_PC(a0)
  move.l  sp, (CRASH_A + 28)(a0)
  move.l  sp, d0
  btst    #0, d0
  bne.s   1f
  movea.l sp, a1
  lea     CRASH_STACK(a0), a2
  moveq   #(CRASH_STACK_LONGS - 1), d0
0:move.l  (a1)+, (a2)+
  dbra    d0, 0b
  // tell the Main CPU where the snapshot is, the signal last
1:move.l  a0, d0
  move.w  d0, (GA_SUB_REG_COMSTAT7)
  swap    d0
  move.w  d0, (GA_SUB_REG_COMSTAT6)
  move.w  #CRASH_SUB_SIGNAL, (GA_SUB_REG_COMSTAT5)
2:stop    #0x2700
  bra.s   2b

/**
 * @fn crash_install
 * @brief Install the handlers in the Sub BIOS jump table (sub/crash.h)
 */
SUB crash_install
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
crash_sub_snapshot: .space CRASH_SIZE

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file crash_main.s
 * @brief Main CPU crash handler entry points
 */

#include "macro.s"
#include "main/memmap.def.h"
#include "crash.def.h"

.section .text

/*
 * One entry point per exception: push the exception number and capture.
 * Bus and address errors push a 7-word frame (access info, access address,
 * instruction, SR, PC); the others push SR and PC.
 */
.macro CRASH_ENTRY name, id, frame
SUB \name
  move.w  #\id, -(sp)
  jbra    crash_capture_\frame
.endm

CRASH_ENTRY exvec_bus_error,      EXVEC_ID_BUS_ERROR,      group0
CRASH_ENTRY exvec_address_error,  EXVEC_ID_ADDRESS_ERROR,  group0
CRASH_ENTRY exvec_illegal,        EXVEC_ID_ILLEGAL,        short
CRASH_ENTRY exvec_zero_divide,    EXVEC_ID_ZERO_DIVIDE,    short
CRASH_ENTRY exvec_chk,            EXVEC_ID_CHK,            short
CRASH_ENTRY exvec_trapv,          EXVEC_ID_TRAPV,          short
CRASH_ENTRY exvec_privilege,      EXVEC_ID_PRIVILEGE,      short
CRASH_ENTRY exvec_trace,          EXVEC_ID_TRACE,          short
CRASH_ENTRY exvec_line_1010,      EXVEC_ID_LINE_1010,      short
CRASH_ENTRY exvec_line_1111,      EXVEC_ID_LINE_1111,      short
CRASH_ENTRY exvec_uninitialized,  EXVEC_ID_UNINITIALIZED,  short
CRASH_ENTRY exvec_spurious,       EXVEC_ID_SPURIOUS,       short

/**
 * @fn exvec_address_or_illegal
 * @brief Entry for the Mega CD system vector table entry that the Boot ROM
 * shares between address errors and illegal instructions
 * @details An address error frame has the faulting access address, always odd,
 * where the shorter frame has the PC, always even.
 */
SUB exvec_address_or_illegal
  btst    #0, 5(sp)
  jbne    exvec_address_error
  jbra    exvec_illegal

crash_capture_group0:
  ori.w   #0x0700, sr
  movem.l d0-d7/a0-a6, (crash_main_snapshot + CRASH_D)
  lea     crash_main_snapshot, a0
  move.w  (sp)+, CRASH_VECTOR(a0)
  move.w  (sp)+, CRASH_ACCESS_INFO(a0)
  move.l  (sp)+, CRASH_ACCESS_ADDR(a0)
  move.w  (sp)+, CRASH_IR(a0)
  bra.s   crash_capture_common

crash_capture_short:
  ori.w   #0x0700, sr
  movem.l d0-d7/a0-a6, (crash_main_snapshot + CRASH_D)
  lea     crash_main_snapshot, a0
  move.w  (sp)+, CRASH_VECTOR(a0)
  clr.l   CRASH_ACCESS_ADDR(a0)
  clr.l   CRASH_ACCESS_INFO(a0)  // and IR

crash_capture_common:
  move.w  (sp)+, CRASH_SR(a0)
  move.l  (sp)+, CRASH_PC(a0)
  move.l  sp, (CRASH_A + 28)(a0)
  // the stack above the frame, unless the stack pointer itself is unusable
  move.l  sp, d0
  btst    #0, d0
  bne.s   1f
  movea.l sp, a1
  lea     CRASH_STACK(a0), a2
  moveq   #(CRASH_STACK_LONGS - 1), d0
0:move.l  (a1)+, (a2)+
  dbra    d0, 0b
1:lea     crash_main_stack_top, sp
  pea     crash_str_main
  pea     (a0)
  jsr     crash_report

/**
 * @fn crash_install
 * @brief Install the handlers in the system vector table (main/crash.h)
 */
SUB crash_install
  move.l  #exvec_address_or_illegal, (EXVEC_ADDRERR)
  move.l  #exvec_zero_divide, (EXVEC_ZERODIV)
  move.l  #exvec_chk, (EXVEC_CHK)
  move.l  #exvec_trapv, (EXVEC_TRAPV)
  move.l  #exvec_privilege, (EXVEC_PRIVERR)
  move.l  #exvec_trace, (EXVEC_TRACE)
  move.l  #exvec_line_1010, (EXVEC_LINE1010)
  move.l  #exvec_line_1111, (EXVEC_LINE1111)
  rts

.section .rodata
crash_str_main: .asciz "MAIN"

.section .bss
.align 4
crash_main_snapshot: .space CRASH_SIZE
// the report runs on its own stack: a crash often means the stack is broken
crash_main_stack: .space 512
crash_main_stack_top:

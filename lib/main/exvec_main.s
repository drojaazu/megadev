/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file exvec_main.s
 * @brief Main CPU crash handler entry points (main/exvec.h)
 */

#include "macro.s"
#include "main/memmap.def.h"
#include "exvec.def.h"

.section .text

/*
 * One entry point per exception: push the exception number and capture.
 * Bus and address errors push a 7-word frame (access info, access address,
 * instruction, SR, PC); the others push SR and PC.
 */
.macro EXVEC_ENTRY name, id, frame
SUB \name
  move.w  #\id, -(sp)
  jbra    exvec_capture_\frame
.endm

EXVEC_ENTRY exvec_bus_error,      EXVEC_ID_BUS_ERROR,      group0
EXVEC_ENTRY exvec_address_error,  EXVEC_ID_ADDRESS_ERROR,  group0
EXVEC_ENTRY exvec_illegal,        EXVEC_ID_ILLEGAL,        short
EXVEC_ENTRY exvec_zero_divide,    EXVEC_ID_ZERO_DIVIDE,    short
EXVEC_ENTRY exvec_chk,            EXVEC_ID_CHK,            short
EXVEC_ENTRY exvec_trapv,          EXVEC_ID_TRAPV,          short
EXVEC_ENTRY exvec_privilege,      EXVEC_ID_PRIVILEGE,      short
EXVEC_ENTRY exvec_trace,          EXVEC_ID_TRACE,          short
EXVEC_ENTRY exvec_line_1010,      EXVEC_ID_LINE_1010,      short
EXVEC_ENTRY exvec_line_1111,      EXVEC_ID_LINE_1111,      short
EXVEC_ENTRY exvec_uninitialized,  EXVEC_ID_UNINITIALIZED,  short
EXVEC_ENTRY exvec_spurious,       EXVEC_ID_SPURIOUS,       short

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

exvec_capture_group0:
  ori.w   #0x0700, sr
  movem.l d0-d7/a0-a6, (exvec_main_snapshot + EXVEC_SNAP_D)
  lea     exvec_main_snapshot, a0
  move.w  (sp)+, EXVEC_SNAP_VECTOR(a0)
  move.w  (sp)+, EXVEC_SNAP_ACCESS_INFO(a0)
  move.l  (sp)+, EXVEC_SNAP_ACCESS_ADDR(a0)
  move.w  (sp)+, EXVEC_SNAP_IR(a0)
  bra.s   exvec_capture_common

exvec_capture_short:
  ori.w   #0x0700, sr
  movem.l d0-d7/a0-a6, (exvec_main_snapshot + EXVEC_SNAP_D)
  lea     exvec_main_snapshot, a0
  move.w  (sp)+, EXVEC_SNAP_VECTOR(a0)
  clr.l   EXVEC_SNAP_ACCESS_ADDR(a0)
  clr.l   EXVEC_SNAP_ACCESS_INFO(a0)  // and IR

exvec_capture_common:
  move.w  (sp)+, EXVEC_SNAP_SR(a0)
  move.l  (sp)+, EXVEC_SNAP_PC(a0)
  move.l  sp, (EXVEC_SNAP_A + 28)(a0)
  // the stack above the frame, unless the stack pointer itself is unusable
  move.l  sp, d0
  btst    #0, d0
  bne.s   1f
  movea.l sp, a1
  lea     EXVEC_SNAP_STACK(a0), a2
  moveq   #(EXVEC_STACK_LONGS - 1), d0
0:move.l  (a1)+, (a2)+
  dbra    d0, 0b
1:lea     exvec_main_stack_top, sp
  pea     exvec_str_main
  pea     (a0)
  jsr     exvec_report

/**
 * @fn exvec_install
 * @brief Install the handlers in the system vector table (main/exvec.h)
 */
SUB exvec_install
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
exvec_str_main: .asciz "MAIN"

.section .bss
.align 4
exvec_main_snapshot: .space EXVEC_SNAP_SIZE
// the report runs on its own stack: a crash often means the stack is broken
exvec_main_stack: .space 512
exvec_main_stack_top:

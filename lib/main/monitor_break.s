/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file monitor_break.s
 * @brief Debug monitor exception entries: breakpoint (TRAP #15) and trace
 *
 * @details
 * Both entries save the CPU state into monitor_snapshot (CrashState layout,
 * crash.def.h) and call C: monitor_trace_hit decides whether a trace
 * exception stops (a breakpoint in trace mode, a single step, re-arming a
 * patched breakpoint) and monitor_stopped runs the command loop until the
 * host continues. The C side may change the snapshot, including PC and SR; the
 * state is restored from it and execution resumes with rte. Runs on the
 * interrupted program's stack.
 */

#include "macro.s"
#include "crash.def.h"
#include "main/monitor.def.h"

.section .text

/**
 * @fn monitor_trap_entry
 * @brief TRAP #15: a breakpoint, patched or written in the code
 */
SUB monitor_trap_entry
  move.w  #MONITOR_VECTOR_TRAP, -(sp)
  bra.s   monitor_capture

/**
 * @fn monitor_trace_entry
 * @brief Trace exception: one instruction executed with the T bit set
 */
SUB monitor_trace_entry
  // Fast path: while trace mode breakpoints are armed, most trace exceptions
  // are not hits. Compare the PC against the armed slots before saving
  // anything; only a hit, a pending step or re-arm, or an empty table (so the
  // T bit gets cleared) takes the full path.
  tst.b   monitor_trace_slow
  bne.s   0f
  tst.b   monitor_trace_armed
  beq.s   0f
  movem.l d0-d1/a0, -(sp)
  move.l  14(sp), d0                // PC from the frame, above 3 saved longs and SR
  lea     monitor_bps, a0
  moveq   #(MONITOR_BP_SLOTS - 1), d1
1:cmpi.b  #MONITOR_BP_TRACE, 6(a0)
  bne.s   2f
  cmp.l   (a0), d0
  beq.s   3f
2:addq.l  #8, a0
  dbra    d1, 1b
  movem.l (sp)+, d0-d1/a0
  rte
3:movem.l (sp)+, d0-d1/a0
0:move.w  #MONITOR_VECTOR_TRACE, -(sp)

monitor_capture:
  movem.l d0-d7/a0-a6, (monitor_snapshot + CRASH_D)
  lea     monitor_snapshot, a0
  move.w  (sp)+, CRASH_VECTOR(a0)
  move.w  (sp)+, CRASH_SR(a0)
  move.l  (sp)+, CRASH_PC(a0)
  move.l  sp, (CRASH_A + 28)(a0)
  clr.l   CRASH_ACCESS_ADDR(a0)
  clr.l   CRASH_ACCESS_INFO(a0)
  movea.l sp, a1
  lea     CRASH_STACK(a0), a2
  moveq   #(CRASH_STACK_LONGS - 1), d0
0:move.l  (a1)+, (a2)+
  dbra    d0, 0b

  cmpi.w  #MONITOR_VECTOR_TRACE, CRASH_VECTOR(a0)
  bne.s   1f
  jsr     monitor_trace_hit         // d0 = 0: not a stop, resume at once
  tst.w   d0
  beq.s   monitor_resume
1:jsr     monitor_stopped           // returns when the host continues

monitor_resume:
  lea     monitor_snapshot, a0
  movea.l (CRASH_A + 28)(a0), sp
  move.l  CRASH_PC(a0), -(sp)
  move.w  CRASH_SR(a0), -(sp)
  movem.l CRASH_D(a0), d0-d7/a0-a6
  rte

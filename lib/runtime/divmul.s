/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/divmul.s
 * @brief 32-bit multiply, divide and modulo helpers for the 68000
 *
 * @details
 * GCC calls __mulsi3 for 32-bit `*`, and __udivsi3, __umodsi3, __divsi3 and
 * __modsi3 for 32-bit `/` and `%`. These normally come from libgcc, but the
 * toolchain's copy is assembled for a 68020 and Megadev does not link it: its
 * modulo routines, for one, reach the divide routines with `bsr.l`, which a
 * 68000 decodes as a branch to an odd address. Arguments are on the stack, the
 * result is in D0, and D1/A0/A1 may be clobbered, as GCC expects. Division by
 * zero raises the 68000 zero divide exception.
 */

.section .text

/* Low 32 bits of the product: (ah * bl + al * bh) << 16 + al * bl */
.global __mulsi3
__mulsi3:
  move.w  4(sp), d0     // a high
  mulu.w  10(sp), d0    // * b low
  move.w  8(sp), d1     // b high
  mulu.w  6(sp), d1     // * a low
  add.w   d1, d0
  swap    d0
  clr.w   d0
  move.w  6(sp), d1     // a low
  mulu.w  10(sp), d1    // * b low
  add.l   d1, d0
  rts

/*
 * Unsigned 32-bit divide
 * in:  d0 dividend, d1 divisor
 * out: d0 quotient, d1 remainder
 * clobber: a0
 */
udivmod32:
  cmpi.l  #0x10000, d1
  bcc.s   2f
  // divisor fits in 16 bits: divide the high word, then remainder:low word
  movea.l d0, a0
  clr.w   d0
  swap    d0
  divu.w  d1, d0        // d0.w = high quotient, upper word = remainder
  move.w  d0, -(sp)
  clr.w   d0
  move.w  a0, d0        // d0 = remainder:low word
  divu.w  d1, d0        // d0.w = low quotient, upper word = remainder
  move.l  d0, d1
  clr.w   d1
  swap    d1            // d1 = remainder
  swap    d0
  move.w  (sp)+, d0
  swap    d0            // d0 = high quotient:low quotient
  rts
2:
  // divisor needs 32 bits: shift and subtract, 32 steps
  movea.l d1, a0
  moveq   #0, d1
  move.w  #31, -(sp)
3:add.l   d0, d0
  addx.l  d1, d1
  bcs.s   4f            // remainder overflowed: it is larger than the divisor
  cmp.l   a0, d1
  bcs.s   5f
4:sub.l   a0, d1
  addq.l  #1, d0
5:subq.w  #1, (sp)
  bpl.s   3b
  addq.l  #2, sp
  rts

.global __udivsi3
__udivsi3:
  move.l  4(sp), d0
  move.l  8(sp), d1
  bsr.w   udivmod32
  rts

.global __umodsi3
__umodsi3:
  move.l  4(sp), d0
  move.l  8(sp), d1
  bsr.w   udivmod32
  move.l  d1, d0
  rts

/* Quotient is negative when the signs differ */
.global __divsi3
__divsi3:
  move.l  4(sp), d0
  bpl.s   1f
  neg.l   d0
1:move.l  8(sp), d1
  bpl.s   2f
  neg.l   d1
2:bsr.w   udivmod32
  tst.l   4(sp)
  bmi.s   3f
  tst.l   8(sp)
  bmi.s   4f
  rts
3:tst.l   8(sp)
  bmi.s   5f
4:neg.l   d0
5:rts

/* Remainder takes the sign of the dividend */
.global __modsi3
__modsi3:
  move.l  4(sp), d0
  bpl.s   1f
  neg.l   d0
1:move.l  8(sp), d1
  bpl.s   2f
  neg.l   d1
2:bsr.w   udivmod32
  move.l  d1, d0
  tst.l   4(sp)
  bpl.s   3f
  neg.l   d0
3:rts

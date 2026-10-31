/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/memmove.s
 * @brief memmove, as the compiler expects to find it
 */

#include "macro.s"

.section .text

/**
 * @fn memmove
 * @brief void * memmove(void * dest, void const * src, size_t n)
 * @return dest, in both d0 and a0: m68k-elf callers read d0, m68k-linux-gnu a0
 *
 * @details
 * Byte copy with a 32-bit count. The regions may overlap: the copy runs
 * backwards when dest lies above src.
 */
SUB memmove
  move.l  4(sp), a1
  move.l  8(sp), a0
  move.l  a1, d0
  move.l  12(sp), d1
  beq.s   3f
  subq.l  #1, d1
  cmp.l   a0, a1
  bls.s   1f
  add.l   d1, a0
  add.l   d1, a1
  addq.l  #1, a0
  addq.l  #1, a1
2:move.b  -(a0), -(a1)
  dbra    d1, 2b
  sub.l   #0x10000, d1
  bcc.s   2b
  bra.s   3f
1:move.b  (a0)+, (a1)+
  dbra    d1, 1b
  sub.l   #0x10000, d1
  bcc.s   1b
3:move.l  d0, a0
  rts

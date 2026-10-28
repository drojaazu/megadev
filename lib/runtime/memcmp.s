/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/memcmp.s
 * @brief memcmp, as the compiler expects to find it
 */

#include "macros.s"

.section .text

/**
 * @fn memcmp
 * @brief int memcmp(void const * a, void const * b, size_t n)
 * @return 0 if equal; otherwise the difference between the first pair of
 * differing bytes, taken as unsigned, so negative when a < b
 */
SUB memcmp
  move.l  4(sp), a0
  move.l  8(sp), a1
  move.l  12(sp), d1
  moveq   #0, d0
  tst.l   d1
  beq.s   3f
  subq.l  #1, d1
1:cmpm.b  (a1)+, (a0)+
  bne.s   2f
  dbra    d1, 1b
  sub.l   #0x10000, d1
  bcc.s   1b
  rts
2:move.b  -1(a0), d0
  moveq   #0, d1
  move.b  -1(a1), d1
  sub.l   d1, d0
3:rts

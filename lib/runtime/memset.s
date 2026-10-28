/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/memset.s
 * @brief memset, as the compiler expects to find it
 */

#include "macros.s"

.section .text

/**
 * @fn memset
 * @brief void * memset(void * dest, int value, size_t n)
 * @return dest
 *
 * @details
 * Byte fill with a 32-bit count. Only the low byte of value is used.
 */
SUB memset
  move.l  4(sp), a0
  move.b  11(sp), d1
  move.l  12(sp), d0
  beq.s   2f
  subq.l  #1, d0
1:move.b  d1, (a0)+
  dbra    d0, 1b
  sub.l   #0x10000, d0
  bcc.s   1b
2:move.l  4(sp), d0
  rts

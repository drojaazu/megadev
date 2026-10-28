/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/memcpy.s
 * @brief memcpy, as the compiler expects to find it
 */

#include "macros.s"

.section .text

/**
 * @fn memcpy
 * @brief void * memcpy(void * dest, void const * src, size_t n)
 * @return dest
 *
 * @details
 * Byte copy with a 32-bit count. Regions must not overlap; use memmove if
 * they may.
 */
SUB memcpy
  move.l  4(sp), a1
  move.l  8(sp), a0
  move.l  12(sp), d1
  move.l  a1, d0
  beq.s   2f
  subq.l  #1, d1
1:move.b  (a0)+, (a1)+
  dbra    d1, 1b
  sub.l   #0x10000, d1
  bcc.s   1b
2:rts

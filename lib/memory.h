/**
 * \file
 * \brief Simple memory utilities
 */

#ifndef MEGADEV__MEMORY_H
#define MEGADEV__MEMORY_H

#include "types.h"

/**
 * \fn memset8
 * \brief Set a range of memory to an 8bit value
 */
static inline void memset8(u8 value, u8 * dest, u32 length) {
  --length;
  asm(R"(
	1:move.b %0, (%1)+
		dbf 1b, %2
	)"
      :
      : "d"(value), "a"(dest), "d"(length)
      : "cc");
}

/**
 * \fn memset16
 * \brief Set a range of memory to a 16bit value
 */
static inline void memset16(u16 value, u8 * dest, u32 length) {
  --length;
  asm(R"(
	1:move.w %0, (%1)+
		dbf 1b, %2
	)"
      :
      : "d"(value), "a"(dest), "d"(length)
      : "cc");
}

/**
 * \fn memset32
 * \brief Set a range of memory to a 32bit value
 */
static inline void memset32(u16 value, u8 * dest, u32 length) {
  --length;
  asm(R"(
	1:move.l %0, (%1)+
		dbf 1b, %2
	)"
      :
      : "d"(value), "a"(dest), "d"(length)
      : "cc");
}

/**
 * \fn memset8
 * \brief Copy a range of 8bit memory values
 */
static inline void memcpy8(u8 * src, u8 * dest, u32 length) {
  --length;
  asm(R"(
	1:move.b (%0)+, (%1)+
		dbf 1b, %2
	)"
      :
      : "a"(src), "a"(dest), "d"(length)
      : "cc");
}

/**
 * \fn memset16
 * \brief Copy a range of 16bit memory values
 */
static inline void memcpy16(u16 * src, u16 * dest, u32 length) {
  --length;
  asm(R"(
	1:move.w (%0)+, (%1)+
		dbf 1b, %2
	)"
      :
      : "a"(src), "a"(dest), "d"(length)
      : "cc");
}

/**
 * \fn memset32
 * \brief Copy a range of 32bit memory values
 */
static inline void memcpy32(u32 * src, u32 * dest, u32 length) {
  --length;
  asm(R"(
	1:move.l (%0)+, (%1)+
		dbf 1b, %2
	)"
      :
      : "a"(src), "a"(dest), "d"(length)
      : "cc");
}

#endif

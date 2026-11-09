/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file memory.h
 * @brief Simple memory utilities
 * @details
 * Though these are similar to the common C memory commands, they have
 * been divided based on the size of the values on which the operation will be
 * performed. This is to optimize their use and it will be up to the developer
 * to determine which call should be used for their purpose.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

/**
 * @brief Set a range of memory to an 8-bit value
 */
/** @brief Fill length bytes at dest with value
 * @details Inline 68000 loop; a length of 0 does nothing. */
static inline void memset8(uint8_t value, void * dest, uint32_t length)
{
  if (length == 0)
    return;
  --length;
  asm volatile(
    "\
1:move.b %[value], (%[dest])+ \n\
  dbra %[length], 1b \n\
  sub.l #0x10000, %[length] \n\
  bcc 1b \n\
		"
    : [dest] "+a"(dest), [length] "+d"(length)
    : [value] "d"(value)
    : "cc");
}

/**
 * @brief Set a range of memory to a 16-bit value
 */
/** @brief Fill length words at dest with value
 * @details Inline 68000 loop; a length of 0 does nothing. */
static inline void memset16(uint16_t value, void * dest, uint32_t length)
{
  if (length == 0)
    return;
  --length;
  asm volatile(
    "\
1:move.w %[value], (%[dest])+ \n\
  dbra %[length], 1b \n\
  sub.l #0x10000, %[length] \n\
  bcc 1b \n\
		"
    : [dest] "+a"(dest), [length] "+d"(length)
    : [value] "d"(value)
    : "cc");
}

/**
 * @brief Set a range of memory to a 32-bit value
 */
/** @brief Fill length longs at dest with value
 * @details Inline 68000 loop; a length of 0 does nothing. */
static inline void memset32(uint32_t value, void * dest, uint32_t length)
{
  if (length == 0)
    return;
  --length;
  asm volatile(
    "\
1:move.l %[value], (%[dest])+ \n\
  dbra %[length], 1b \n\
  sub.l #0x10000, %[length] \n\
  bcc 1b \n\
		"
    : [dest] "+a"(dest), [length] "+d"(length)
    : [value] "d"(value)
    : "cc");
}

/**
 * @brief Copy a range of 8bit memory values
 */
/** @brief Copy length bytes from src to dest
 * @details Inline 68000 loop; a length of 0 does nothing. */
static inline void memcpy8(uint8_t * src, uint8_t * dest, uint32_t length)
{
  if (length == 0)
    return;
  --length;
  asm volatile(
    "\
1:move.b (%[src])+, (%[dest])+ \n\
  dbra %[length], 1b \n\
  sub.l #0x10000, %[length] \n\
  bcc 1b \n\
		"
    : [src] "+a"(src), [dest] "+a"(dest), [length] "+d"(length)
    :
    : "cc");
}

/**
 * @brief Copy a range of 16bit memory values
 */
/** @brief Copy length words from src to dest
 * @details Inline 68000 loop; a length of 0 does nothing. */
static inline void
memcpy16(uint16_t const * src, uint16_t * dest, uint32_t length)
{
  if (length == 0)
    return;
  --length;
  asm volatile(
    "\
1:move.w (%[src])+, (%[dest])+ \n\
  dbra %[length], 1b \n\
  sub.l #0x10000, %[length] \n\
  bcc 1b \n\
		"
    : [src] "+a"(src), [dest] "+a"(dest), [length] "+d"(length)
    :
    : "cc");
}

/**
 * @brief Copy a range of 32bit memory values
 */
/** @brief Copy length longs from src to dest
 * @details Inline 68000 loop; a length of 0 does nothing. */
static inline void
memcpy32(uint32_t const * src, uint32_t * dest, uint32_t length)
{
  if (length == 0)
    return;
  --length;
  asm volatile(
    "\
1:move.l (%[src])+, (%[dest])+ \n\
  dbra %[length], 1b \n\
  sub.l #0x10000, %[length] \n\
  bcc 1b \n\
		"
    : [src] "+a"(src), [dest] "+a"(dest), [length] "+d"(length)
    :
    : "cc");
}

#ifdef __cplusplus
}
#endif

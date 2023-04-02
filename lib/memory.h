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

#ifndef MEGADEV__MEMORY_H
#define MEGADEV__MEMORY_H

#include "types.h"

/**
 * @fn memset8
 * @brief Set a range of memory to an 8-bit value
 */
static inline void memset8 (u8 value, u8 * dest, u32 length)
{
	--length;
	asm (
		"\
		1:move.b %2, (%0)+ \n\
			dbf 1b, %1 \n\
		"
		: "+a"(dest), "+d"(length)
		: "d"(value)
		: "cc");
}

/**
 * @fn memset16
 * @brief Set a range of memory to a 16-bit value
 */
static inline void memset16 (u16 value, u8 * dest, u32 length)
{
	--length;
	asm (
		"\
	1:move.w %0, (%1)+ \n\
		dbf 1b, %2 \n\
		"
		:
		: "d"(value), "a"(dest), "d"(length)
		: "cc");
}

/**
 * @fn memset32
 * @brief Set a range of memory to a 32-bit value
 */
static inline void memset32 (u16 value, u8 * dest, u32 length)
{
	--length;
	asm (
		"\
	1:move.l %0, (%1)+ \n\
		dbf 1b, %2 \n\
		"
		:
		: "d"(value), "a"(dest), "d"(length)
		: "cc");
}

/**
 * @fn memset8
 * @brief Copy a range of 8bit memory values
 */
static inline void memcpy8 (u8 * src, u8 * dest, u32 length)
{
	--length;
	asm (
		"\
	1:move.b (%0)+, (%1)+ \n\
		dbf 1b, %2 \n\
		"
		:
		: "a"(src), "a"(dest), "d"(length)
		: "cc");
}

/**
 * @fn memset16
 * @brief Copy a range of 16bit memory values
 */
static inline void memcpy16 (u16 * src, u16 * dest, u32 length)
{
	--length;
	asm (
		"\
	1:move.w (%0)+, (%1)+ \n\
		dbf 1b, %2 \n\
		"
		:
		: "a"(src), "a"(dest), "d"(length)
		: "cc");
}

/**
 * @fn memset32
 * @brief Copy a range of 32bit memory values
 */
static inline void memcpy32 (u32 * src, u32 * dest, u32 length)
{
	--length;
	asm (
		"\
	1:move.l (%0)+, (%1)+ \n\
		dbf 1b, %2 \n\
		"
		:
		: "a"(src), "a"(dest), "d"(length)
		: "cc");
}

#endif

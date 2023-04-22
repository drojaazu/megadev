/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file kos_cmp.h
 * @brief Decompression routine for "Kosinski" compressed data
 *
 * @note Taken from Wonder Library code
 */

#include "types.h"

extern void Kos_Decomp();

static inline void dcmp_kosinski(void * src, void * dst)
{
	register u32 A0 asm("a0") = (u32) src;
	register u32 A1 asm("a1") = (u32) dst;

	asm volatile(
		"\
			jsr %p0 \n\
		"
		: "+a"(A0), "+a"(A1)
		: "i"(Kos_Decomp), "a"(A0), "a"(A1)
		: "d2", "d3", "d4", "d5", "d6", "cc");
};

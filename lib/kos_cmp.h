/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file kos_cmp.h
 * @brief Decompression routine for "Kosinski" compressed data
 *
 * @note Taken from Wonder Library code
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

/**
   * @brief Kosinski decompressor (lib/kos_cmp.s)
   * @details Assembly entry: A0 source, A1 destination; clobbers d2-d6. Call it
   * from C through dcmp_kosinski.
   */
extern void Kos_Decomp();

/**
   * @brief Decompress Kosinski data
   * @param src Compressed data
   * @param dst Destination buffer, large enough for the decompressed size
   */
static inline void dcmp_kosinski(void * src, void * dst)
{
  register uint32_t A0 asm("a0") = (uint32_t) src;
  register uint32_t A1 asm("a1") = (uint32_t) dst;

  asm volatile(
    "\
			jsr %p2 \n\
		"
    : "+a"(A0), "+a"(A1)
    : "i"(Kos_Decomp), "a"(A0), "a"(A1)
    : "d2", "d3", "d4", "d5", "d6", "cc");
};

#ifdef __cplusplus
}
#endif

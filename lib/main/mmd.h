/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file mmd.h
 * @brief Set up and run a loaded MMD formatted module
 * @note This expects a module to be already loaded at the start of Word RAM
 */

#ifndef MEGADEV__MAIN_INIT_MMD_H
#define MEGADEV__MAIN_INIT_MMD_H

#include <main/gate_arr.h>
#include <main/memmap.h>

static inline void const * init_mmd()
{
  register void const * mmd_entry;
  register u32          scratch_d0, scratch_a0, scratch_a1;

  wait_2m();

  // clang-format off
	asm volatile(
		"\
  move.l   2(%[wrdram]), %[scratch_d0] \n\
  beq      1f \n\
  movea.l  %[scratch_d0], %[scratch_a0] \n\
  lea      0x100(%[wrdram]), %[scratch_a1] \n\
  move.w   6(%[wrdram]), %[scratch_d0] \n\
0:move.l   (%[scratch_a1])+, (%[scratch_a0])+ \n\
  dbf      %[scratch_d0], 0b \n\
1:move.l   12(%[wrdram]), %[scratch_d0] \n\
  beq      2f \n\
  move.l   %[scratch_d0], %c[mlevel4]+2 \n\
2:move.l   16(%[wrdram]), %[scratch_d0] \n\
  beq      3f \n\
  move.l   %[scratch_d0], %c[mlevel6]+2 \n\
3:btst     #6, (%[wrdram]) \n\
  beq      4f \n\
6:bset     #%c[ga_dmna], %c[ga_memmode]+1 \n\
  btst     #%c[ga_dmna], %c[ga_memmode]+1 \n\
  beq      6b \n\
4: movea.l 8(%[wrdram]), %[mmd_entry] \n\
	"
		:
			[mmd_entry] "=a"(mmd_entry),
			[scratch_d0] "=d"(scratch_d0),
			[scratch_a0] "=a"(scratch_a0),
			[scratch_a1] "=a"(scratch_a1)
		:
			[wrdram] "a"(WORD_RAM),
			[mlevel4] "i"(EXVEC_LEVEL4),
			[mlevel6] "i"(EXVEC_LEVEL6),
			[ga_dmna] "i"(GA_BIT_DMNA),
			[ga_memmode] "i"(GA_REG_MEMMODE)
		:
			"cc");
  // clang-format on

  return mmd_entry;
}

#endif

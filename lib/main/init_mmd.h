/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file init_mmd.h
 * @brief Set up and run a loaded MMD formatted module
 * @note This expects a module to be already loaded at the start of Word RAM
 */

#ifndef MEGADEV__MAIN_INIT_MMD_H
#define MEGADEV__MAIN_INIT_MMD_H

#include "main/gate_array.def.h"

static inline void const * init_mmd()
{
	register void const * mmd_entry;

	wait_2m();

	asm volatile(
		"\
  move.l   2(%[wrdram]), d0 \n\
  beq      1f \n\
  movea.l  d0, a2 \n\
  lea      0x100(%[wrdram]), a1 \n\
  move.w   6(%[wrdram]), d0 \n\
0:move.l   (a1)+, (a2)+ \n\
  dbf      d0, 0b \n\
1:move.l   12(%[wrdram]), d0 \n\
  beq      2f \n\
  move.l   d0, _MLEVEL4+2 \n\
2:move.l   16(%[wrdram]), d0 \n\
  beq      3f \n\
  move.l   d0, _MLEVEL6+2 \n\
3:btst     #6, (%[wrdram]) \n\
  beq      4f \n\
  GRANT_2M \n\
4: movea.l 8(%[wrdram]), a0 \n\
		"
		:
			[mmd_entry] "=a"(mmd_entry)
		:
			[wrdram] "a"(_WRDRAM),
			"i"(_MLEVEL4),
			"i"(_MLEVEL6),
			"i"(GA_RET_BIT),
			"i"(GA_DMNA_BIT),
			"i"(_GAREG_MEMMODE)
		:
		"d0", "a1", "a2");
	
	return mmd_entry;
}

#endif

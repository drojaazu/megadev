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
	register void const * A0 __asm__("a0");

	__asm__(
		"\
		5:btst #%c4, %c6+1 \n\
			beq 5b \n\
			movea.l  %c1+8, a0 \n\
			move.l   %c1+2, d0 \n\
			beq      1f \n\
			movea.l  d0, a2 \n\
			lea      %c1+0x100, a1 \n\
			move.w   %c1+6, d0 \n\
		0:move.l   (a1)+, (a2)+ \n\
			dbf      d0, 0b \n\
		1:move.l   %c1+0xc, d0 \n\
			beq      2f \n\
			move.l   d0, %c2+2 \n\
		2:move.l   %c1+0x10, d0 \n\
			beq      3f \n\
			move.l   d0, %c3+2 \n\
		3:btst     #6, %c1 \n\
			beq      4f \n\
		6:bset #%c5,%c6+1 \n\
			btst #%c5,%c6+1 \n\
			beq 6b \n\
		4: \n\
		"
		: "=a"(A0)
		: "i"(_WRDRAM), "i"(_MLEVEL4), "i"(_MLEVEL6), "i"(GA_RET_BIT), "i"(GA_DMNA_BIT), "i"(_GAREG_MEMMODE)
		: "d0", "a1", "a2");
	
	return A0;
}

#endif

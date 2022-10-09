/**
 * @file sub.h
 * @brief C wrappers for Sub CPU side hardware mappings, Gate Array (GA) registers and BIOS vectors
 */

#ifndef MEGADEV__CD_GA_SUB_H
#define MEGADEV__CD_GA_SUB_H

#include "sub/sub_def.h"
#include "types.h"

#define DMAADDR_WORDRAM1M(addr) (((addr) &0x3FFF) >> 3)
#define DMAADDR_WORDRAM2M(addr) (((addr) &0x7FFF) >> 3)
#define DMAADDR_PCM(addr) (((addr) &0x03FF) >> 3)
#define DMAADDR_PRGRAM(addr) ((addr) >> 3)

#define PCM_ENV ((volatile u8 *) _PCM_ENV)
#define PCM_PAN ((volatile u8 *) _PCM_PAN)
#define PCM_FDL ((volatile u8 *) _PCM_FDL)
#define PCM_FDH ((volatile u8 *) _PCM_FDH)
#define PCM_LSL ((volatile u8 *) _PCM_LSL)
#define PCM_LSH ((volatile u8 *) _PCM_LSH)
#define PCM_ST ((u8 volatile *) _PCM_ST)
#define PCM_CTRL ((volatile u8 *) _PCM_CTRL)
#define PCM_CDISABLE ((volatile u8 *) _PCM_CDISABLE)

// TODO - Verify which registers get trashed
static inline void bios_waitvsync()
{
	asm(
		"\
		#move.l a6, -(sp) \n\
		jsr %p0 \n\
		#move.l (sp)+, a6 \n\
		"
		:
		: "i"(_WAITVSYNC)
		: "d0", "d2", "d3", "a0", "a1", "a2");
}

#endif

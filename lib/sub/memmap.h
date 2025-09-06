/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file memmap.h
 * @brief C wrappers for Sub CPU memory map & system vectors
 */

#ifndef MEGADEV__SUB_MEMMAP_H
#define MEGADEV__SUB_MEMMAP_H

#include <sub/memmap.def.h>
#include <types.h>

#define DMAADDR_WORDRAM1M(addr) (((addr) & 0x3FFF) >> 3)
#define DMAADDR_WORDRAM2M(addr) (((addr) & 0x7FFF) >> 3)
#define DMAADDR_PCM(addr)				(((addr) & 0x03FF) >> 3)
#define DMAADDR_PRGRAM(addr)		((addr) >> 3)

#define SP_INT2 (*((void volatile *(*) )(USERCALL2 + 2)))

#endif

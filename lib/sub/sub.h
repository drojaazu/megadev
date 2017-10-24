/**
 * \file sub/sub.h
 * \brief C wrappers for hardware mapping, Gate Array (GA) registers and BIOS
 * vectors on the Sub CPU side
 */

#ifndef MEGADEV__CD_GA_SUB_H
#define MEGADEV__CD_GA_SUB_H

#include "types.h"
#include "sub/sub_def.h"

#define GA_MEMORYMODE (*(volatile u16*)_GA_MEMORYMODE)

#define GA_COMFLAGS_MAIN ((volatile const u8*)_GA_COMFLAGS)
#define GA_COMFLAGS_SUB ((volatile u8*)_GA_COMFLAGS+1)

#define GA_COMCMD0 ((volatile const u16*)_GA_COMCMD0)
#define GA_COMCMD1 ((volatile const u16*)_GA_COMCMD1)
#define GA_COMCMD2 ((volatile const u16*)_GA_COMCMD2)
#define GA_COMCMD3 ((volatile const u16*)_GA_COMCMD3)
#define GA_COMCMD4 ((volatile const u16*)_GA_COMCMD4)
#define GA_COMCMD5 ((volatile const u16*)_GA_COMCMD5)
#define GA_COMCMD6 ((volatile const u16*)_GA_COMCMD6)
#define GA_COMCMD7 ((volatile const u16*)_GA_COMCMD7)

#define GA_COMSTAT0 ((volatile u16*)_GA_COMSTAT0)
#define GA_COMSTAT1 ((volatile u16*)_GA_COMSTAT1)
#define GA_COMSTAT2 ((volatile u16*)_GA_COMSTAT2)
#define GA_COMSTAT3 ((volatile u16*)_GA_COMSTAT3)
#define GA_COMSTAT4 ((volatile u16*)_GA_COMSTAT4)
#define GA_COMSTAT5 ((volatile u16*)_GA_COMSTAT5)
#define GA_COMSTAT6 ((volatile u16*)_GA_COMSTAT6)
#define GA_COMSTAT7 ((volatile u16*)_GA_COMSTAT7)

// TODO - Verify which registers get trashed
static inline void bios_waitvsync() {
  asm(R"(
  #move.l a6, -(sp)
	jsr %p0
	#move.l (sp)+, a6
	)"
	  :
		:
		"i"(_WAITVSYNC)
    :
		"d0","d2", "d3", "a0", "a1", "a2"
	);
}

/**
 * \fn wait_2m
 * Wait for Sub CPU access to 2M Word RAM
 */
static inline void wait_2m() {
	asm(R"(
1:btst %0, %p1
  beq 1b
	)"
	  :
	  :
		"i"(MEMORYMODE_DMNA_BIT),"i"(_GA_MEMORYMODE+1)
	);
}

/**
 * \fn grant_2m
 * Grant 2M Word RAM access to the Main CPU and wait for confirmation
 */
static inline void grant_2m() {
	asm(R"(
1:bset %0, %p1
  btst %0, %p1
	beq 1b
	)"
	  :
		:
		"i"(MEMORYMODE_RET_BIT),"i"(_GA_MEMORYMODE+1)
	);
}

/**
 * \fn clear_comm_regs
 * Clears the Main comm registers (COMSTAT) and flags
 */
static inline void clear_comm_regs() {
	asm(R"(
  lea %p0, a0
  moveq #0, d0
  move.b d0, -0x11(a0) /* lower byte of comm flags */
  move.l d0, (a0)+
  move.l d0, (a0)+
  move.l d0, (a0)+
  move.l d0, (a0)+
	)"
	  :
		:
		"i"(_GA_COMSTAT0)
		:
		"d0", "a0"
	);
}


#endif

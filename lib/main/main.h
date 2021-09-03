/**
 * \file main/main.h
 * \brief C wrappers for hardware mapping, Gate Array (GA) registers and BIOS
 * vectors on the Main CPU side
 */

#ifndef MEGADEV__CD_GA_MAIN_H
#define MEGADEV__CD_GA_MAIN_H

#include "main/main_def.h"
#include "types.h"

#define MAIN_2M_BASE ((volatile void *)_MAIN_2M_BASE)
#define MAIN_PRGRAM ((volatile void *)_MAIN_PRGRAM)

#define GA_COMFLAGS_MAIN ((volatile u8 *)_GA_COMFLAGS)
#define GA_COMFLAGS_SUB ((volatile const u8 *)_GA_COMFLAGS + 1)

#define GA_COMCMD0 ((volatile u16 *)_GA_COMCMD0)
#define GA_COMCMD1 ((volatile u16 *)_GA_COMCMD1)
#define GA_COMCMD2 ((volatile u16 *)_GA_COMCMD2)
#define GA_COMCMD3 ((volatile u16 *)_GA_COMCMD3)
#define GA_COMCMD4 ((volatile u16 *)_GA_COMCMD4)
#define GA_COMCMD5 ((volatile u16 *)_GA_COMCMD5)
#define GA_COMCMD6 ((volatile u16 *)_GA_COMCMD6)
#define GA_COMCMD7 ((volatile u16 *)_GA_COMCMD7)

#define GA_COMSTAT0 ((volatile const u16 *)_GA_COMSTAT0)
#define GA_COMSTAT1 ((volatile const u16 *)_GA_COMSTAT1)
#define GA_COMSTAT2 ((volatile const u16 *)_GA_COMSTAT2)
#define GA_COMSTAT3 ((volatile const u16 *)_GA_COMSTAT3)
#define GA_COMSTAT4 ((volatile const u16 *)_GA_COMSTAT4)
#define GA_COMSTAT5 ((volatile const u16 *)_GA_COMSTAT5)
#define GA_COMSTAT6 ((volatile const u16 *)_GA_COMSTAT6)
#define GA_COMSTAT7 ((volatile const u16 *)_GA_COMSTAT7)

/**
 * \def GA_MEMMODE
 * \sa _GA_MEMMODE
 */
#define GA_MEMMODE ((volatile u16 *)_GA_MEMMODE)

// We only provide wrappers for the system jump table vectors as trying to
// change the two-byte asm opcode prefix via C would be unnecessarily
// convoluted. If the opcode needs to be changed from JSR (which should
// never need to be done unless you're trying to be fancy), it can be done
// through ASM

// L6 interrupt = VINT
#define MLEVEL6_VECTOR ((volatile void *(*))(_MLEVEL6 + 2))
// L4 interrupt = HINT
#define MLEVEL4_VECTOR ((volatile void *)(_MLEVEL4 + 2))
// L2 interrupt = Ext. port
#define MLEVEL2_VECTOR ((volatile void *(*))(_MLEVEL2 + 2))

/**
 * \fn wait_2m
 * Wait for Main CPU access to 2M Word RAM
 */
static inline void wait_2m() {
  asm(R"(
1:btst %0, %p1
  beq 1b
)" ::"i"(GA_RET_BIT),
      "i"(_GA_MEMMODE + 1));
}

/**
 * \fn grant_2m
 * Grant 2M Word RAM access to the Sub CPU and wait for confirmation
 */
static inline void grant_2m() {
  asm(R"(
1:bset %0, %p1
  btst %0, %p1
	beq 1b
)" ::"i"(GA_DMNA_BIT),
      "i"(_GA_MEMMODE + 1));
}

/**
 * \fn clear_comm_regs
 * Clears the Main comm registers (COMCMD) and flags
 */
static inline void clear_comm_regs() {
  asm(R"(
  lea %p0, a0
  moveq #0, d0
  move.b d0, -2(a0) /* upper byte of comm flags */
  move.l d0, (a0)+
  move.l d0, (a0)+
  move.l d0, (a0)+
  move.l d0, (a0)+
)" ::"i"(_GA_COMCMD0)
      : "d0", "a0");
}

#endif

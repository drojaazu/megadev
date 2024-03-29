#ifndef MEGADEV__MAIN_GATEARR_MACROS_S
#define MEGADEV__MAIN_GATEARR_MACROS_S

#include "main/gatearr_def.h"

/**
 * CLEAR_COMM_REGS
 * Clears the Main comm registers (COMCMD) and flags
 * BREAK: d0, a0
 */
.macro CLEAR_COMM_REGS
	lea _GA_COMCMD0, a0
	moveq #0, d0
	move.b d0, -2(a0) // upper byte of comm flags
	move.l d0, (a0)+
	move.l d0, (a0)+
	move.l d0, (a0)+
	move.l d0, (a0)+
.endm

/**
 * WAIT_2M
 * Wait for Main CPU access to 2M Word RAM
 */
.altmacro
.macro WAIT_2M
LOCAL loop

loop:
	btst #GA_RET_BIT, _GA_MEMMODE+1
	beq loop
.endm

/**
 * GRANT_2M
 * Grant 2M Word RAM access to the Sub CPU and wait for confirmation
 */
.altmacro
.macro GRANT_2M
LOCAL loop

loop:
	bset #GA_DMNA_BIT, _GA_MEMMODE+1
	btst #GA_DMNA_BIT, _GA_MEMMODE+1
	beq loop
.endm

#endif

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_array.s
 * @brief Helper macros for use with the Gate Array on the Main CPU side
 */

#ifndef MEGADEV__MAIN_GATE_ARRAY_S
#define MEGADEV__MAIN_GATE_ARRAY_S

#include "main/gate_array.def.h"

/**
 * WAIT_2M
 * Wait for Main CPU access to 2M Word RAM
 */
.altmacro
.macro WAIT_2M
LOCAL loop

loop:
	btst #GA_RET_BIT, _GAREG_MEMMODE+1
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
	bset #GA_DMNA_BIT,_GAREG_MEMMODE+1
	btst #GA_DMNA_BIT,_GAREG_MEMMODE+1
	beq loop
.endm



/**
 * CLEAR_COMM_REGS
 * Clears the Main comm registers (COMCMD) and flags
 * BREAK: d0, a0
 */
.macro CLEAR_COMM_REGS
	lea _GAREG_COMCMD0, a0
	moveq #0, d0
	move.b d0, -2(a0) // upper byte of comm flags
	move.l d0, (a0)+
	move.l d0, (a0)+
	move.l d0, (a0)+
	move.l d0, (a0)+
.endm



/**
 * RESET_GA
 * Pattern to reset gate array
 * Taken from Wonder Libary
 */
.macro RESET_GA
	move.w #0xFF00,_GAREG_MEMMODE
	move.b #0x3,(_GAREG_RESET + 1)
	nop
	move.b #0x3,(_GAREG_RESET + 1)
	move.b #0x2,(_GAREG_RESET + 1)
	move.b #0x0,(_GAREG_RESET + 1)
.endm

#endif

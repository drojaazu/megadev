/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.macros.s
 * @brief Macros for Sub CPU side Gate Array registers
 */

#pragma once

/**
 * CLEAR_COMM_REGS
 * Clears the Sub comm registers (COMSTAT) and flags
 * BREAK: d0, a0
 */
.macro CLEAR_COMM_REGS
	lea GA_REG_COMSTAT0, a0
	moveq   #0, d0
	move.b  d0, -0x11(a0) // lower byte of comm flags
	move.l  d0, (a0)+
	move.l  d0, (a0)+
	move.l  d0, (a0)+
	move.l  d0, (a0)+
.endm

/**
 * WAIT_2M
 * Wait for Sub CPU access to 2M Word RAM
 */
.altmacro
.macro WAIT_2M
LOCAL loop

loop:
	btst #BIT_GA_REG_DMNA, GA_REG_MEMMODE+1
	beq loop
.endm

/**
 * GRANT_2M
 * Grant 2M Word RAM access to the Main CPU and wait for confirmation
 */
.altmacro
.macro GRANT_2M
LOCAL loop

loop:
	bset    #BIT_GA_REG_RET, GA_REG_MEMMODE+1
	btst    #BIT_GA_REG_RET, GA_REG_MEMMODE+1
	beq     loop
.endm


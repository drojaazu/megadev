/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.macro.s
 * @brief Helper macros for use with the Gate Array on the Main CPU side
 */

#pragma once
#include "main/gate_arr.def.h"

/**
 * WAIT_2M
 * Wait for Main CPU access to 2M Word RAM
 */
.altmacro
.macro WAIT_2M
LOCAL loop

loop:
	btst     #GA_RETURN_2M_POS, GA_REG_MEMMODE
	beq      loop
.endm

/**
 * GRANT_2M
 * Grant 2M Word RAM access to the Sub CPU and wait for confirmation
 */
.altmacro
.macro GRANT_2M
LOCAL loop

loop:
  bset     #GA_DMNA_POS,GA_REG_MEMMODE
  btst     #GA_DMNA_POS,GA_REG_MEMMODE
  beq      loop
.endm



/**
 * CLEAR_COMM_REGS
 * Clears the Main comm registers (COMCMD) and flags
 * BREAK: d0, a0
 */
.macro CLEAR_COMM_REGS
  lea      GA_REG_COMCMD0, a0
  moveq    #0, d0
  move.b   d0, -2(a0) // upper byte of comm flags
  move.l   d0, (a0)+
  move.l   d0, (a0)+
  move.l   d0, (a0)+
  move.l   d0, (a0)+
.endm



/**
 * RESET_GA
 * Pattern to reset gate array
 * Taken from Wonder Libary
 *
 * The gate array performs a forced reset only in response to this exact
 * sequence of accesses, so the width and address of each one is load bearing --
 * in particular the first must be a WORD write to 0xA12002, which is why it is
 * written through GA_REG_WP rather than as two byte writes. See BR-4 for the
 * one respect in which this differs from the documented pattern.
 */
.macro RESET_GA
  move.w   #0xFF00,GA_REG_WP
  move.b   #0x3,GA_REG_SUBCPU
  nop
  move.b   #0x3,GA_REG_SUBCPU
  move.b   #0x2,GA_REG_SUBCPU
  move.b   #0x0,GA_REG_SUBCPU
.endm

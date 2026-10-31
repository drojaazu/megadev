/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.macro.s
 * @brief Helper macros for use with the Gate Array on the Main CPU side
 */

#pragma once
#include "main/gate_arr.def.h"

/**
 * GA_MAIN_WAIT_2M
 * Wait for Main CPU access to 2M Word RAM
 */
.altmacro
.macro GA_MAIN_WAIT_2M
LOCAL loop

loop:
	btst     #GA_MAIN_RETURN_2M_POS, GA_MAIN_REG_MEMMODE
	beq      loop
.endm

/**
 * GA_MAIN_GRANT_2M
 * Grant 2M Word RAM access to the Sub CPU and wait for confirmation
 */
.altmacro
.macro GA_MAIN_GRANT_2M
LOCAL loop

loop:
  bset     #GA_MAIN_DMNA_POS,GA_MAIN_REG_MEMMODE
  btst     #GA_MAIN_DMNA_POS,GA_MAIN_REG_MEMMODE
  beq      loop
.endm



/**
 * GA_MAIN_CLEAR_COMM_REGS
 * Clears the Main comm registers (COMCMD) and flags
 * BREAK: d0, a0
 */
.macro GA_MAIN_CLEAR_COMM_REGS
  lea      GA_MAIN_REG_COMCMD0, a0
  moveq    #0, d0
  move.b   d0, -2(a0) // upper byte of comm flags
  move.l   d0, (a0)+
  move.l   d0, (a0)+
  move.l   d0, (a0)+
  move.l   d0, (a0)+
.endm



/**
 * GA_MAIN_RESET
 * Pattern to reset gate array
 * Taken from Wonder Libary
 *
 * The gate array performs a forced reset only in response to this exact
 * sequence of accesses, so the width and address of each one is load bearing --
 * in particular the first must be a WORD write to 0xA12002, which is why it is
 * written through GA_MAIN_REG_WP rather than as two byte writes.
 */
.macro GA_MAIN_RESET
  move.w   #0xFF00,GA_MAIN_REG_WP
  move.b   #0x3,GA_MAIN_REG_SUBCPU
  nop
  move.b   #0x3,GA_MAIN_REG_SUBCPU
  move.b   #0x2,GA_MAIN_REG_SUBCPU
  move.b   #0x0,GA_MAIN_REG_SUBCPU
.endm

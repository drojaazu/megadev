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
	btst     #GA_MAIN_BIT_RETURN_2M, GA_MAIN_REG_MEMMODE+1
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
  bset     #GA_MAIN_BIT_DMNA,GA_MAIN_REG_MEMMODE+1
  btst     #GA_MAIN_BIT_DMNA,GA_MAIN_REG_MEMMODE+1
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
 */
.macro GA_MAIN_RESET
  move.w   #0xFF00,GA_MAIN_REG_MEMMODE
  move.b   #0x3,(GA_MAIN_REG_RESET + 1)
  nop
  move.b   #0x3,(GA_MAIN_REG_RESET + 1)
  move.b   #0x2,(GA_MAIN_REG_RESET + 1)
  move.b   #0x0,(GA_MAIN_REG_RESET + 1)
.endm

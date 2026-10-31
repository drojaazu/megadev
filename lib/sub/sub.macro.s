/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub.macro.s
 * Sub CPU Gate Array and misc. utilities for use with the Sub CPU
 */

#pragma once

#include <sub/memmap.def.h>
#include <sub/gate_arr.def.h>
#include <sub/bios.def.h>

/**
 * @fn BURAM
 * @param fcode BRAM function code
 * @brief Call the specified internal Backup RAM function
 * @clobber d0
 */
.macro BURAM fcode
	move.w  \fcode, d0
	jsr     BURAM
.endm

/**
 * @fn BIOSCALL
 * @param fcode BIOS function code
 * @brief Call the specified internal BIOS function
 * @clobber d0
 */
.macro BIOSCALL fcode
	move.w  \fcode, d0
	jsr     CDBIOS
.endm

/**
 * @fn CDBOOT
 * @param fcode CD Boot function code
 * @brief Call the specified internal CD Boot function
 * @clobber d0
 */
.macro CDBOOT fcode
	move.w  \fcode, d0
	jsr     CDBOOT
.endm

/**
 * GA_SUB_CLEAR_COMM_REGS
 * Clears the Sub comm registers (COMSTAT) and flags
 * BREAK: d0, a0
 */
.macro GA_SUB_CLEAR_COMM_REGS
	lea GA_SUB_REG_COMSTAT0, a0
	moveq   #0, d0
	move.b  d0, -0x11(a0) // lower byte of comm flags
	move.l  d0, (a0)+
	move.l  d0, (a0)+
	move.l  d0, (a0)+
	move.l  d0, (a0)+
.endm

/**
 * GA_SUB_WAIT_2M
 * Wait for Sub CPU access to 2M Word RAM
 */
.altmacro
.macro GA_SUB_WAIT_2M
LOCAL loop

loop:
	btst #GA_SUB_DMNA_POS, GA_SUB_REG_MEMMODE
	beq loop
.endm

/**
 * GA_SUB_GRANT_2M
 * Grant 2M Word RAM access to the Main CPU and wait for confirmation
 */
.altmacro
.macro GA_SUB_GRANT_2M
LOCAL loop

loop:
	bset    #GA_SUB_RETURN_2M_POS, GA_SUB_REG_MEMMODE
	btst    #GA_SUB_RETURN_2M_POS, GA_SUB_REG_MEMMODE
	beq     loop
.endm

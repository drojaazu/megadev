/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub.macro.s
 * Sub CPU Gate Array and misc. utilities for use with the Sub CPU
 */

#ifndef MEGADEV__SUB_MACROS_S
#define MEGADEV__SUB_MACROS_S

#include <sub/memmap.def.h>
#include <sub/gate_arr.def.h>
#include <sub/bios.def.h>

/**
 * @macro BURAM
 * @param fcode BRAM function code
 * @brief Call the specified internal Backup RAM function
 * @clobber d0
 */
.macro BURAM fcode
	move.w  \fcode, d0
	jsr     BURAM
.endm

/**
 * @macro BIOSCALL
 * @param fcode BIOS function code
 * @brief Call the specified internal BIOS function
 * @clobber d0
 */
.macro BIOSCALL fcode
	move.w  \fcode, d0
	jsr     CDBIOS
.endm

/**
 * @macro CDBOOT
 * @param fcode CD Boot function code
 * @brief Call the specified internal CD Boot function
 * @clobber d0
 */
.macro CDBOOT fcode
	move.w  \fcode, d0
	jsr     CDBOOT
.endm

/**
 * CLEAR_COMM_REGS
 * Clears the Sub comm registers (COMSTAT) and flags
 * BREAK: d0, a0
 */
.macro CLEAR_COMM_REGS
	lea _GAREG_COMSTAT0, a0
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
	btst #BIT_GAREG_DMNA, _GAREG_MEMMODE+1
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
	bset    #BIT_GAREG_RET, _GAREG_MEMMODE+1
	btst    #BIT_GAREG_RET, _GAREG_MEMMODE+1
	beq     loop
.endm

#endif

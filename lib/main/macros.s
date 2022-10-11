/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file macros.s
 * @brief General purpose asm macros for use with the Main CPU
 */

#ifndef macros_S
#define macros_S

#include "main/z80_def.h"

.altmacro
.macro Z80_DO_BUSREQ
LOCAL loop
	move.w #0x100, _Z80_BUSREQ
loop:
	btst #0,  _Z80_BUSREQ
	bne.s	loop
.endm

.macro Z80_DO_BUSRELEASE
	move.w  #0, _Z80_BUSREQ
.endm

.macro Z80_DO_RESET
	move.w  #0x000, (_Z80_RESET)
	.rept 8
		nop
	.endr
	move.w  #0x100, (_Z80_RESET)
.endm

#endif

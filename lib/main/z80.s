/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file z80.s
 * @brief Z80 CPU utilities
 */

#ifndef MEGADEV__MAIN_Z80_S
#define MEGADEV__MAIN_Z80_S

#include "macros.s"
#include "z80.def.h"

.section .text

.altmacro
.macro Z80_BUS_REQUEST
LOCAL loop
	move.w #0x100, _Z80_BUSREQ
loop:
	btst #0,  _Z80_BUSREQ
	bne.s	loop
.endm

.macro Z80_BUS_RELEASE
	move.w  #0, _Z80_BUSREQ
.endm

.macro Z80_CPU_RESET
	move.w  #0x000, (_Z80_RESET)
	.rept 8
	nop
	.endr
	move.w  #0x100, (_Z80_RESET)
.endm

/**
 * @sa init_z80
 * @param[in] A0.l Pointer to Z80 program to load
 * @param[in] D7.l Length of program data (in bytes)
 * @clobber a0-a1, d7
 * @ingroup z80
 * @warning Interrupts should be disabled before calling.
 */
 .altmacro
.macro Z80_INIT
LOCAL status_wait
LOCAL copy
  move.w  #_Z80_STOP_RESET, (_Z80_BUSREQ)
  move.w  #_Z80_STOP_RESET, (_Z80_RESET)
status_wait:
  btst    #_Z80_STATUS, (_Z80_BUSREQ)
  bne.s	  status_wait
  lea     (_Z80_RAM), a1
copy:
  move.b  (a0)+,(a1)+
  dbf	    d7, copy
  move.w  #0, (_Z80_RESET)
  move.w  #0, (_Z80_BUSREQ)
  move.w  #_Z80_STOP_RESET, (_Z80_RESET)
.endm

#endif
/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file z80.macros.s
 * @brief Z80 CPU utilities
 */

#ifndef MEGADEV__MAIN_Z80_S
#define MEGADEV__MAIN_Z80_S

#include "macros.s"
#include "z80.def.h"

.section .text

.altmacro
.macro Z80_REQUEST_BUS
LOCAL loop
	move.w #Z80_BUS_REQUEST, Z80_REG_BUSREQ
loop:
	btst #Z80_BIT_STATUS,  Z80_REG_BUSREQ
	bne.s	loop
.endm

.macro Z80_RELEASE_BUS
	move.w  #Z80_BUS_RELEASE, Z80_REG_BUSREQ
.endm

.macro Z80_RESET_CPU
	move.w  #Z80_RESET_ASSERT, (Z80_REG_RESET)
	.rept 8
	nop
	.endr
	move.w  #Z80_RESET_RELEASE, (Z80_REG_RESET)
.endm

/**
 * @sa z80_init
 * @param[in] A0.l Pointer to Z80 program to load
 * @param[in] D7.l Length of program data (in bytes - 1)
 * @clobber a0-a1, d7
 * @ingroup z80
 * @warning Interrupts should be disabled before calling.
 */
 .altmacro
.macro Z80_INIT
LOCAL status_wait
LOCAL copy
  move.w   #Z80_BUS_REQUEST, (Z80_REG_BUSREQ)
  move.w   #Z80_RESET_RELEASE, (Z80_REG_RESET)
status_wait:
  btst     #Z80_BIT_STATUS, (Z80_REG_BUSREQ)
  bne.s    status_wait
  lea      (Z80_RAM), a1
copy:
  move.b  (a0)+,(a1)+
  dbf	    d7, copy
  move.w   #Z80_RESET_ASSERT, (Z80_REG_RESET)
  move.w   #Z80_BUS_RELEASE, (Z80_REG_BUSREQ)
  move.w   #Z80_RESET_RELEASE, (Z80_REG_RESET)
.endm

#endif
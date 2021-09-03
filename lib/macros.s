/**
 * \file macros.s
 * \brief General purpose asm macros for use with the Main CPU
 */

#ifndef MACROS_S
#define MACROS_S

#include "z80_def.h"

.macro FUNC name, align=2
  //.section .text.asm.\name
  .global  \name
  .type   \name, @function
  .align \align
  \name:
.endm

// Global label
.macro GLABEL name, align=2
  .global \name
  .align \align
  \name:
.endm

// Global symbol
.macro GLOBAL name, value, align=2
  .align \align
  .global \name
  .equ \name, \value
.endm

.macro FILE path, label, align=2
  LOCAL file_end
  LOCAL file_start
  GLABEL \label
  .long file_end - file_start
file_start:
  .incbin \path
file_end:
.endm

.macro INTERRUPT_DISABLE
  ori #0x700, sr
.endm

.macro INTERRUPT_ENABLE
  andi #0xF8FF, sr
.endm

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
  move.w  #0x000, (_Z80_RESET)  // Assert reset line
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  move.w  #0x100, (_Z80_RESET) // Release reset line
.endm

/**
 * \brief Convert a value to binary coded decimal
 * \param[in] D0.w Value to convert
 * \param[out] D0.w Value as BCD
 * \break D1
 */
.macro HEX2BCD
	ext.l	d0
	divu.w	#0xa, d0
	move.b	d0, d1
	lsl.b	#4, d1
	swap	d0
	move	#0, ccr /* why?*/
	abcd	d1, d0
.endm

/**
 * \brief Push a value on to the stack
 * \param reg Register holding the value to push
 */
.macro PUSH reg
	move.l \reg, -(sp)
.endm

/**
 * \brief Pop a value from the stack
 * \param reg Register to hold the popped value
 */
.macro POP reg
	move.l  (sp)+, \reg
.endm

/**
 * \brief Push multiple values on to the stack
 * \param regs Register list
 */
.macro PUSHM regs
	movem.l \regs, -(sp)
.endm

/**
 * \brief Pop multiple values from the stack
 * \param regs Register list
 */
.macro POPM regs
	movem.l (sp)+, \regs
.endm

/*
  Compare strings at a1 and a2
  IN:
    a1 - ptr to string 1
    a2 - ptr to string 2
    d1 - length
  OUT:
    Z - set if no match
*/
.altmacro
.macro COMPARE_STRING
.local LOOP
.local END

  PUSHM    d1/a1-a2
LOOP:
  cmpm.b  (a1)+, (a2)+
  bne      END
  dbf      d1, LOOP
  moveq    #0, d1
END:
  POPM    d1/a1-a2  // movem does not affect registers; Z will be preserved
.endm

#endif

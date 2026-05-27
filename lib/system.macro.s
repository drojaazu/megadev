/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file system.s
 * @brief CPU System Control Operations
 */

#ifndef SYSTEM_S
#define SYSTEM_S

.macro DISABLE_INTERRUPTS
  ori #0x700, sr
.endm

.macro ENABLE_INTERRUPTS
  andi #0xF8FF, sr
.endm

.macro cpu_stop
  stop #0x700
.endm


/**
 * @brief Push a value on to the stack
 * @param reg Register holding the value to push
 */
.macro PUSH reg
	move.l \reg, -(sp)
.endm

/**
 * @brief Pop a value from the stack
 * @param reg Register to hold the popped value
 */
.macro POP reg
	move.l  (sp)+, \reg
.endm

/**
 * @brief Push multiple values on to the stack
 * @param regs Register list
 */
.macro PUSHM regs
	movem.l \regs, -(sp)
.endm

/**
 * @brief Pop multiple values from the stack
 * @param regs Register list
 */
.macro POPM regs
	movem.l (sp)+, \regs
.endm


#endif

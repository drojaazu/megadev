/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file str_util.s
 * @brief Utilities for converting hex values to ASCII string
 * print function
 */

#include "macro.s"
#include "str_util.def.h"

.section .text

/**
 * @fn hextoa8
 * @brief Convert 8 bit value to hexadecimal ascii string
 * param[in] d0.b value
 * param[in] a0.l ptr to store output string
 */
SUB hextoa8
	rol.b #4, d0
	jbsr hex_to_ascii
	move.b d1, (a0)+
	rol.b #4, d0
	jbsr hex_to_ascii
	move.b d1, (a0)+
	move.b #STRING_TERMINATOR,(a0)
	rts

/**
 * @fn hextoa16
 * @brief Convert 16 bit value to hexadecimal ascii string
 * param[in] d0.w value
 * param[in] a0.l ptr to store output string
 */
SUB hextoa16
	moveq #3, d7
1:rol.w #4, d0
	jbsr hex_to_ascii
	move.b d1, (a0)+
	dbra d7, 1b
	move.b #STRING_TERMINATOR,(a0)
	rts

/**
 * @fn hextoa32
 * @brief Convert 32 bit value to hexadecimal ascii string
 * param[in] d0.l value
 * param[in] a0.l ptr to store output string
 */
SUB hextoa32
	moveq #7, d7
1:rol.l #4, d0
	jbsr hex_to_ascii
	move.b d1, (a0)+
	dbra d7, 1b
	move.b #STRING_TERMINATOR,(a0)
	rts

hex_to_ascii:
	move.b d0, d1
	and.b #0x0F, d1
	cmp.b #0x09, d1
	bgt 2f
	add.b #0x30, d1
	rts
2:add.b #0x37, d1
	rts

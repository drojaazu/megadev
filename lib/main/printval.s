/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file printval.s
 * @brief Utilities for printing hexadecimal values using the Boot ROM library
 * print function
 */

#include "macros.s"

.section .text

/**
 * @fn printval_u8
 * @brief Print an 8 bit value as hexadecimal ascii
 * param[in] d0.b value
 * param[in] a0.l ptr to store output values
 */
FUNC printval_u8
	rol.b #4, d0
	jbsr hex_to_ascii
	move.b d1, (a0)+
	rol.b #4, d0
	jbsr hex_to_ascii
	move.b d1, (a0)+
	move.b #0xff,(a0)
	rts

/**
 * @fn printval_u16
 * @brief Print a 16 bit value as hexadecimal ascii
 * param[in] d0.w value
 * param[in] a0.l ptr to store output values
 */
FUNC printval_u16
	moveq #3, d7
1:rol.w #4, d0
	jbsr hex_to_ascii
	move.b d1, (a0)+
	dbf d7, 1b
	move.b #0xff,(a0)
	rts

/**
 * @fn printval_u32
 * @brief Print a 32 bit value as hexadecimal ascii
 * param[in] d0.l value
 * param[in] a0.l ptr to store output values
 */
FUNC printval_u32
	moveq #7, d7
1:rol.l #4, d0
	jbsr hex_to_ascii
	move.b d1, (a0)+
	dbf d7, 1b
	move.b #0xff,(a0)
	rts

hex_to_ascii:
	move.b d0, d1
	and.b #0x0f, d1
	cmp.b #0x09, d1
	bgt 2f
	add.b #0x30, d1
	rts
2:add.b #0x37, d1
	rts
/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file str_util.s
 * @brief Utilities for converting hex values to ASCII string
 * print function
 */

#include "macros.s"

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
	move.b #0xFF,(a0)
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
	dbf d7, 1b
	move.b #0xFF,(a0)
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
	dbf d7, 1b
	move.b #0xFF,(a0)
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

.macro ATOI
	moveq #0, d0
1:move.b (a0)+, d1
	cmp.b #0x20, d1  //check for space
	beq 5f
	cmp.b #0x0A, d1  //check for newline
	beq 5f
	cmp.b #0x60, d1 
	ble 2f
	sub.b #0x56, d1  // capital hex letters
	bra 4f
2:cmp.b #0x40, d1 
	ble 3f
	sub.b #0x36, d1  // lowercase hex letters
	bra 4f
3:sub.b #0x30, d1  // ascii digits
4:lsl.l #4, d0
	or.b d1, d0
	bra 1b
5:


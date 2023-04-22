/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file string.s
 * @brief String utilities
 */

#include "macros.s"

GLABEL ATOI
	moveq #0, d0
1:move.b (a0)+, d1
	cmp.b #0x20, d1  //check for space
	beq 5f
	cmp.b #0x0a, d1  //check for newline
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
5:rts
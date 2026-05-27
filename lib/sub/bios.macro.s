/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub/bios.macro.s
 * @brief Sub CPU side system library (BIOS) Macros
 */

#pragma once

/**
 * @macro BIOS_CALL
 * @param fcode BIOS function code
 * @brief Call the specified internal BIOS function
 * @clobber d0
 */
.macro BIOS_CALL fcode
	move.w  \fcode, d0
	jsr     BIOS_CALL_VECTOR
.endm


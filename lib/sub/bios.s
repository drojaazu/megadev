/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub/bios.s
 * @brief Sub CPU side system library (BIOS) utilities and macros
 */

/**
 * @macro BIOSCALL
 * @param fcode BIOS function code
 * @brief Call the specified internal BIOS function
 * @clobber d0
 */
.macro BIOSCALL fcode
	move.w  \fcode, d0
	jsr     CDBIOS_VECTOR
.endm


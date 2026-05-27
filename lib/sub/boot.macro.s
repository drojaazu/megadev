/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub/boot.macro.s
 * @brief Sub CD boot process macros
 */

/**
 * @macro CDBOOT
 * @param fcode CD Boot function code
 * @brief Call the specified internal CD Boot function
 * @clobber d0
 */
.macro CDBOOT fcode
	move.w  \fcode, d0
	jsr     CDBOOT
.endm

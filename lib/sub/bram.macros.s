/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file bram.macro.s
 * @brief Macros for Backup RAM usage
 */

#pragma once

#include <sub/bram.def.h>

/**
 * @macro BRAM_CALL
 * @param fcode BRAM function code
 * @brief Call the specified internal Backup RAM function
 * @clobber d0
 */
.macro BRAM_CALL fcode
	move.w  \fcode, d0
	jsr     BRAM_CALL_VECTOR
.endm

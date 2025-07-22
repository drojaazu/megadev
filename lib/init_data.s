/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file init_data.s
 * @brief Load initialized variables from the data section of ROM into memory
 */


/**
 * @def INIT_DATA
 * @brief Load p
 * @clobber d0, a1-a2
 */
.macro INIT_DATA
	lea _ROM_DATA_ORIGIN, a1
	lea _RAM_DATA_ORIGIN, a2
	mov.l #_RAM_DATA_LENGTH, d7
0:mov.l (a1)+, (a2)+
	dbf d0, 0b
.endm

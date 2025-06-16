/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file load_data.s
 * @brief Load initialized variables from the data section of ROM into memory
 */


/**
 * @def load_data
 * @brief Load p
 * @clobber d0, a1-a2
 */
SUB INIT_DATA
	lea _ROM_DATA, a1
	lea _DATA_ORIGIN, a2
	mov.l #_DATA_LENGTH, d7
0:mov.l (a1)+, (a2)+
	dbf d0, 0b
  rts
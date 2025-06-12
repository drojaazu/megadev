/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file load_data.s
 * @brief Load initialized variables from the data section of ROM into memory
 */


load_data:
	//ori #0x700,sr
	MODULE_RAM_ORIGIN
	
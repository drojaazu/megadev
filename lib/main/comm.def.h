/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file comm.def.h
 * @brief Serial communications utilities
 */

#ifndef MEGADEV__COMM_DEF_H
#define MEGADEV__COMM_DEF_H

#include <main/io.def.h>

/*
	External comm port
	The front controller ports and rear EXT port (only present on early model
	Mega Drives) can be put into serial mode for communication with external
	devices, e.g. with your PC as a debugging tool.
	For each of the EQU entries below, modify the number in the definition to
	change the port to be used for serial communication:
		1 - Player 1 port
		2 - Player 2 port
		3 - Rear EXT port
*/

/**
 * @todo These need to be made user-defined
 */
#define EXT_CTRL	 _IO_CTRL3
#define EXT_SCTRL	 _IO_SCTRL3
#define EXT_RXDATA _IO_RXDATA3
#define EXT_TXDATA _IO_TXDATA3

/*
	External comm port speed
	Sets the transfer speed of the external device port
*/
#define EXT_BAUD SCTRL_BAUD_4800

#endif

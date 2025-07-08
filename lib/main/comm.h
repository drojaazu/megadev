/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file comm.h
 * @brief Serial communications utilities
 */

#ifndef MEGADEV__COMM_H
#define MEGADEV__COMM_H

#include "main/io.def.h"
#include "types.h"
#include "system.h"
#include <main/z80.h>

/**
 * @fn init_ext
 * @brief Initialize IO port for serial communication
 */
static inline void init_ext_port()
{
	disable_interrupts();
	enable_interrupts();
}

/**
 * @fn ext_rx_c
 * @brief Wrapper for @ref ext_rx
 */
static inline u8 ext_rx_c()
{
	register u8 D0 asm("d0");
	asm(
		"\
			jsr ext_rx \n\
		"
		: "+d"(D0));

	return D0;
}

/**
 * @fn ext_rx_c
 * @brief Wrapper for @ref ext_rx
 */
static inline void ext_tx_c(register u8 data)
{
	register u8 D0 asm("d0") = data;

	asm(
		"\
			jsr ext_tx \n\
		"
		:
		: "d"(D0));
}

#endif

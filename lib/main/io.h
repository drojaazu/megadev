/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file io.h
 * @brief C wrappers for I/O registers
 */

#ifndef MEGADEV__IO_H
#define MEGADEV__IO_H

#include "main/io_def.h"
#include "types.h"

/**
 * @sa _IO_DATA1
 */
#define IO_DATA1 (*((u8 volatile *) _IO_DATA1))

/**
 * @sa _IO_DATA2
 */
#define IO_DATA2 (*((u8 volatile *) _IO_DATA2))

/**
 * @sa _IO_DATA3
 */
#define IO_DATA3 (*((u8 volatile *) _IO_DATA3))

/**
 * @sa _IO_CTRL1
 */
#define IO_CTRL1 (*((u8 volatile *) _IO_CTRL1))

/**
 * @sa _IO_CTRL2
 */
#define IO_CTRL2 (*((u8 volatile *) _IO_CTRL2))

/**
 * @sa _IO_CTRL3
 */
#define IO_CTRL3 (*((u8 volatile *) _IO_CTRL3))

/**
 * @sa _IO_TXDATA1
 */
#define IO_TXDATA1 (*((u8 volatile *) _IO_TXDATA1))

/**
 * @sa _IO_TXDATA2
 */
#define IO_TXDATA2 (*((u8 volatile *) _IO_TXDATA2))

/**
 * @sa _IO_TXDATA3
 */
#define IO_TXDATA3 (*((u8 volatile *) _IO_TXDATA3))

/**
 * @sa _IO_RXDATA1
 */
#define IO_RXDATA1 (*((u8 const volatile *) _IO_RXDATA1))

/**
 * @sa _IO_RXDATA2
 */
#define IO_RXDATA2 (*((u8 const volatile *) _IO_RXDATA2))

/**
 * @sa _IO_RXDATA3
 */
#define IO_RXDATA3 (*((u8 const volatile *) _IO_RXDATA3))

/**
 * @sa _IO_SCTRL1
 */
#define IO_SCTRL1 (*((u8 volatile *) _IO_SCTRL1))

/**
 * @sa _IO_SCTRL2
 */
#define IO_SCTRL2 (*((u8 volatile *) _IO_SCTRL2))

/**
 * @sa _IO_SCTRL3
 */
#define IO_SCTRL3 (*((u8 volatile *) _IO_SCTRL3))

/**
 * @fn init_ext
 * @brief Initialize IO port for serial communication
 */
extern void init_ext();

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

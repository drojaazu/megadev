/**
 * \file io.h
 * \brief C wrappers for I/O registers
 */

#ifndef MEGADEV__IO_H
#define MEGADEV__IO_H

#include "io_def.h"
#include "types.h"

/**
 * \var IO_DATA1
 * \brief Data port for port 1
 */
#define IO_DATA1 ((volatile u8 *)_IO_DATA1)

/**
 * \var IO_DATA2
 * \brief Data port for port 2
 */
#define IO_DATA2 ((volatile u8 *)_IO_DATA2)

/**
 * \var IO_DATA3
 * \brief Data port for port 3
 */
#define IO_DATA3 ((volatile u8 *)_IO_DATA3)

/**
 * \var IO_CTRL1
 * \brief Control port for port 1
 */
#define IO_CTRL1 ((volatile u8 *)_IO_CTRL1)

/**
 * \var IO_CTRL2
 * \brief Control port for port 2
 */
#define IO_CTRL2 ((volatile u8 *)_IO_CTRL2)

/**
 * \var IO_CTRL3
 * \brief Control port for port 3
 */
#define IO_CTRL3 ((volatile u8 *)_IO_CTRL3)

/**
 * \var IO_TXDATA1
 * \brief Transmit data port for port 1
 */
#define IO_TXDATA1 ((volatile u8 *)_IO_TXDATA1)

/**
 * \var IO_TXDATA2
 * \brief Transmit data port for port 2
 */
#define IO_TXDATA2 ((volatile u8 *)_IO_TXDATA2)

/**
 * \var IO_TXDATA3
 * \brief Transmit data port for port 3
 */
#define IO_TXDATA3 ((volatile u8 *)_IO_TXDATA3)

/**
 * \var IO_RXDATA1
 * \brief Receive data port for port 1
 */
#define IO_RXDATA1 ((volatile u8 const *)_IO_RXDATA1)

/**
 * \var IO_RXDATA2
 * \brief Receive data port for port 2
 */
#define IO_RXDATA2 ((volatile u8 const *)_IO_RXDATA2)

/**
 * \var IO_RXDATA3
 * \brief Receive data port for port 3
 */
#define IO_RXDATA3 ((volatile u8 const *)_IO_RXDATA3)

/**
 * \var IO_SCTRL1
 * \brief Serial control port for port 1
 */
#define IO_SCTRL1 ((volatile u8 *)_IO_SCTRL1)

/**
 * \var IO_SCTRL2
 * \brief Serial control port for port 2
 */
#define IO_SCTRL2 ((volatile u8 *)_IO_SCTRL2)

/**
 * \var IO_SCTRL3
 * \brief Serial control port for port 3
 */
#define IO_SCTRL3 ((volatile u8 *)_IO_SCTRL3)


/**
 * \fn init_ext
 * \brief Initialize IO port for serial communication
 */
extern void init_ext();

/**
 * \fn ext_rx_c
 * \brief Wrapper for \ref ext_rx
 */
inline u8 ext_rx_c() {
	register u8 d0_data asm("d0");
	asm("jsr ext_rx" : "=d"(d0_data));
	return d0_data;
}

/**
 * \fn ext_rx_c
 * \brief Wrapper for \ref ext_rx
 */
inline void ext_tx_c(u8 data) {
	register u8 d0_data asm("d0") = data;
	asm("jsr ext_tx" :: "d"(d0_data));
}

#endif

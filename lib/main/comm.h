/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file comm.h
 * @brief Serial communications utilities
 */

#ifndef MEGADEV__COMM_H
#define MEGADEV__COMM_H

#include <main/comm.def.h>
#include <main/io.def.h>
#include <main/z80.h>
#include <system.h>
#include <types.h>

/**
 * @fn init_ext
 * @brief Initialize IO port for serial communication
 */
static inline void init_ext_port()
{
	disable_interrupts();
	z80_bus_request();
	asm volatile(
		"\
  move.b   #%c[sctrl_settings], %c[ext_sctrl] \n\
  move.b   #0x7F, %c[ext_ctrl] \n\
	"
		:
		: [sctrl_settings] "i"(
				SCTRL_SERIAL_ENABLE | SCTRL_RX_INT_ENABLE | EXT_BAUD),
			[ext_sctrl] "i"(EXT_SCTRL),
			[ext_ctrl] "i"(EXT_CTRL)
		:);
	z80_bus_release();
	enable_interrupts();
}

/**
 * @fn ext_rx
 */
static inline u8 ext_rx()
{
	register u8	 rx_data;
	register u32 scratch_a;

	asm volatile(
		"\
  lea      %c[ext_sctrl], (%[scratch_a]) \n\
0:btst     #%c[sctrl_rx_ready], (%[scratch_a]) \n\
  beq      0b \n\
  move.b   (%c[ext_rx_data]), %[rx] \n\
	"
		: [rx] "=d"(rx_data), [scratch_a] "=&a"(scratch_a)
		: [sctrl_rx_ready] "i"(SCTRL_RX_READY),
			[ext_sctrl] "i"(EXT_SCTRL),
			[ext_rx_data] "i"(EXT_RXDATA)
		:);

	return rx_data;
}

/**
 * @fn ext_tx
 */
static inline void ext_tx(register u8 tx_data)
{
	register u32 scratch_a;

	asm volatile(
		"\
  lea      %c[ext_sctrl], (%[scratch_a]) \n\
0:btst     #%c[sctrl_tx_full], (%[scratch_a])	\n\
  bne      0b \n\
  move.b   %c[tx], (%c[ext_tx_data]) \n\
	"
		: [scratch_a] "=&a"(scratch_a)
		: [tx] "d"(tx_data),
			[sctrl_tx_full] "i"(SCTRL_TX_FULL),
			[ext_sctrl] "i"(EXT_SCTRL),
			[ext_tx_data] "i"(EXT_TXDATA)
		:);
}

#endif

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file comm.h
 * @brief Serial communication over a controller port or the EXT port
 *
 * @details
 * The port is selected with EXT_PORT and the speed with EXT_BAUD; see
 * comm.def.h. The Z80 shares the bus with the I/O area, so every access here
 * is bracketed by a Z80 bus request and runs with interrupts masked. The bus
 * is released between polls so that a slow link does not starve Z80 audio
 * playback.
 */

#pragma once

#include <main/comm.def.h>
#include <main/io.def.h>
#include <main/z80.h>
#include <system.h>
#include <types.h>

/**
 * @fn init_ext_port
 * @brief Configure the selected port for serial communication
 *
 * @details
 * Sets the pin directions, clears the data latch, then enables serial mode at
 * EXT_BAUD with the receive interrupt enabled. TH (bit 6) is left as an input:
 * it is not a UART pin, and an input cannot contend with the device at the
 * other end of the cable.
 */
static inline void init_ext_port()
{
  u16 const sr = mask_interrupts();
  z80_request_bus();
  asm volatile(
    "\
  move.b   #0x3F, %c[ext_ctrl] \n\
  move.b   #0x00, %c[ext_data] \n\
  move.b   #%c[sctrl_settings], %c[ext_sctrl] \n\
	"
    :
    : [sctrl_settings] "i"(
        SCTRL_SERIAL_ENABLE_MASK | SCTRL_RX_INT_ENABLE_MASK | FIELD_PREP(SCTRL_BAUD, EXT_BAUD)),
      [ext_sctrl] "i"(EXT_SCTRL),
      [ext_data] "i"(EXT_DATA),
      [ext_ctrl] "i"(EXT_CTRL)
    :);
  z80_release_bus();
  restore_interrupts(sr);
}

/**
 * @fn ext_status
 * @brief Read the serial status register
 *
 * @details
 * Returns the raw S-CTRL byte. Test it with SCTRL_RX_READY_MASK,
 * SCTRL_RX_ERR_MASK and SCTRL_TX_FULL_MASK. Poll with this rather than reading
 * the register directly.
 */
static inline u8 ext_status()
{
  u16 const sr = mask_interrupts();
  z80_request_bus();
  u8 const status = *((u8 volatile *) EXT_SCTRL);
  z80_release_bus();
  restore_interrupts(sr);
  return status;
}

/**
 * @fn ext_read_byte
 * @brief Read the receive register without waiting
 *
 * @details
 * For an interrupt handler that has already seen SCTRL_RX_READY_MASK in
 * @ref ext_status. Read the status before the data register: reading RxData
 * clears the ready and error bits.
 */
static inline u8 ext_read_byte()
{
  u16 const sr = mask_interrupts();
  z80_request_bus();
  u8 const rx_data = *((u8 volatile *) EXT_RXDATA);
  z80_release_bus();
  restore_interrupts(sr);
  return rx_data;
}

/**
 * @fn ext_rx
 * @brief Wait for a byte and return it
 *
 * @details
 * Blocks until a byte is ready. A receive error is not reported; where line
 * quality matters, poll @ref ext_status and test SCTRL_RX_ERR_MASK before
 * calling @ref ext_read_byte.
 */
static inline u8 ext_rx()
{
  while (!(ext_status() & SCTRL_RX_READY_MASK))
    ;

  return ext_read_byte();
}

/**
 * @fn ext_tx
 * @brief Send a byte, waiting for the transmitter to drain first
 */
static inline void ext_tx(u8 const tx_data)
{
  while (ext_status() & SCTRL_TX_FULL_MASK)
    ;

  u16 const sr = mask_interrupts();
  z80_request_bus();
  *((u8 volatile *) EXT_TXDATA) = tx_data;
  z80_release_bus();
  restore_interrupts(sr);
}

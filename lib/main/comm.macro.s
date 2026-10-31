/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file comm.macro.s
 * @brief Serial communication over a controller port or the EXT port
 *
 * @details
 * Assembly counterparts of the routines in comm.h. The port is selected with
 * EXT_PORT and the speed with EXT_BAUD; see comm.def.h. Each macro brackets its
 * hardware access with a Z80 bus request, because the Z80 shares the bus with
 * the I/O area. The caller is responsible for interrupt masking.
 */

#pragma once

#include "macro.s"
#include "main/io.def.h"
#include "main/comm.def.h"
#include "main/z80.macro.s"

/**
 * @fn INIT_EXT_PORT
 * @brief Configure the selected port for serial communication
 */
.macro INIT_EXT_PORT
  Z80_REQUEST_BUS
  move.b  #0x3F, EXT_CTRL
  move.b  #0x00, EXT_DATA
  move.b  #(SCTRL_SERIAL_ENABLE_MASK | SCTRL_RX_INT_ENABLE_MASK | FIELD_PREP(SCTRL_BAUD, EXT_BAUD)), EXT_SCTRL
  Z80_RELEASE_BUS
.endm

/**
 * @fn EXT_RX
 * @brief Wait for a byte and read it
 * @param[out] d0.b The byte received
 * @clobber d0, d1
 */
.altmacro
.macro EXT_RX
LOCAL loop
loop:
  Z80_REQUEST_BUS
  move.b  (EXT_SCTRL), d1
  Z80_RELEASE_BUS
  btst    #SCTRL_RX_READY_POS, d1
  beq     loop
  Z80_REQUEST_BUS
  move.b  (EXT_RXDATA), d0
  Z80_RELEASE_BUS
.endm

/**
 * @fn EXT_TX
 * @brief Send a byte, waiting for the transmitter to drain first
 * @param[in] d0.b The byte to send
 * @clobber d1
 */
.altmacro
.macro EXT_TX
LOCAL loop
loop:
  Z80_REQUEST_BUS
  move.b  (EXT_SCTRL), d1
  Z80_RELEASE_BUS
  btst    #SCTRL_TX_FULL_POS, d1
  bne     loop
  Z80_REQUEST_BUS
  move.b  d0, (EXT_TXDATA)
  Z80_RELEASE_BUS
.endm

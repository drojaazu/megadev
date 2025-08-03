/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file comm.macros.s
 * @brief Serial communications utilities
 */

#ifndef MEGADEV___MACROS_COMM_S
#define MEGADEV___MACROS_COMM_S

#include "macros.s"
#include "main/io.def.h"
#include "main/comm.def.h"

.section .text

/**
 * @fn INIT_EXT_PORT
 * @brief Initialize IO port for serial communication
 */
.macro INIT_EXT_PORT
  DISABLE_INTERRUPTS
  Z80_DO_BUSREQ
  move.b  #(SCTRL_SERIAL_ENABLE | SCTRL_RX_INT_ENABLE | EXT_BAUD), EXT_SCTRL
  move.b  #0x7f, EXT_CTRL
  Z80_DO_BUSRELEASE
  ENABLE_INTERRUPTS
.endm

/**
 * @fn ext_rx
 * @brief Read a byte from the external port
 * @param[out] D0.b 
 */
/* TODO: How does RERR play into this? */
.altmacro
.macro EXT_RX
LOCAL loop

loop:
  btst     #SCTRL_RX_READY, (EXT_SCTRL)	// check that we're ready to receive
  beq      loop
  move.b   (EXT_RXDATA), d0
.endm

/**
 * @fn ext_tx
 * @brief Transmit a byte to the external port
 * @param[in] D0.b Byte to transmit
 */
.macro EXT_TX
2:move.b  (EXT_SCTRL), d1
  btst    #SCTRL_TX_FULL, d1 // make sure transmit queue is not full
  bne     2b
  move.b  d0, EXT_TXDATA
  rts
.endm

#endif

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file comm.s
 * @brief Serial communications utilities
 */

#ifndef MEGADEV__COMM_S
#define MEGADEV__COMM_S

#include "macros.s"
#include "main/io.def.h"

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
.equ EXT_CTRL, IO_CTRL3
.equ EXT_SCTRL, IO_SCTRL3
.equ EXT_RXDATA, IO_RXDATA3
.equ EXT_TXDATA, IO_TXDATA3

/*
  External comm port speed
  Sets the transfer speed of the external device port
*/
.equ EXT_BAUD, SCTRL_BAUD_4800

.section .text

/**
 * @fn init_ext
 * @brief Initialize IO port for serial communication
 */
.macro INIT_EXT_PORT
  disable_interrupts
  Z80_DO_BUSREQ
  
  /* Set comm speed; Serial in/out mode; Enable Rx interrupt */
  move.b  #(SCTRL_SERIAL_ENABLE | SCTRL_RX_INT_ENABLE | EXT_BAUD), EXT_SCTRL
  move.b  #0x7f, EXT_CTRL
  
  Z80_DO_BUSRELEASE
  enable_interrupts
.endm

/**
 * @fn ext_rx
 * @brief Read a byte from the external port
 * @param[out] D0.b 
 */
/* TODO: How does RERR play into this? */
SUB ext_rx
1:btst #SCTRL_RX_READY, (EXT_SCTRL)	// check that we're ready to receive
  beq     1b
  move.b  (EXT_RXDATA), d0
  rts

/**
 * @fn ext_tx
 * @brief Transmit a byte to the external port
 * @param[in] D0.b Byte to transmit
 */
SUB ext_tx
2:move.b  (EXT_SCTRL), d1
  btst    #SCTRL_TX_FULL, d1 // make sure transmit queue is not full
  bne     2b
  move.b  d0, EXT_TXDATA
  rts

#endif

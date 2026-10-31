/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file comm.def.h
 * @brief Serial port selection for the external communication routines
 *
 * @details
 * The controller ports and the rear EXT port (present only on early Mega Drive
 * models) each carry a UART that can be used to talk to an external device,
 * such as a PC running a terminal. These definitions select which port and
 * speed the routines in comm.h and comm.macro.s use. Override them from the
 * project makefile:
 *
 *     CC_FLAGS+=-DEXT_PORT=3 -DEXT_BAUD=SCTRL_BAUD_1200
 */

#pragma once

#include <main/io.def.h>

/**
 * @def EXT_PORT
 * @brief Port used for serial communication: 1 (player 1), 2 (player 2) or 3
 * (rear EXT). Default 2.
 */
#ifndef EXT_PORT
#define EXT_PORT 2
#endif

#if EXT_PORT == 1
#define EXT_CTRL   IO_CTRL1
#define EXT_DATA   IO_DATA1
#define EXT_SCTRL  IO_SCTRL1
#define EXT_RXDATA IO_RXDATA1
#define EXT_TXDATA IO_TXDATA1
#elif EXT_PORT == 2
#define EXT_CTRL   IO_CTRL2
#define EXT_DATA   IO_DATA2
#define EXT_SCTRL  IO_SCTRL2
#define EXT_RXDATA IO_RXDATA2
#define EXT_TXDATA IO_TXDATA2
#elif EXT_PORT == 3
#define EXT_CTRL   IO_CTRL3
#define EXT_DATA   IO_DATA3
#define EXT_SCTRL  IO_SCTRL3
#define EXT_RXDATA IO_RXDATA3
#define EXT_TXDATA IO_TXDATA3
#else
#error "EXT_PORT must be 1, 2 or 3"
#endif

/**
 * @def EXT_BAUD
 * @brief Serial speed, one of the SCTRL_BAUD_* values. Default
 * SCTRL_BAUD_4800, the hardware maximum.
 */
#ifndef EXT_BAUD
#define EXT_BAUD SCTRL_BAUD_4800
#endif

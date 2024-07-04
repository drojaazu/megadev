/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file io.def.h
 * @brief I/O register definitions
 */

#ifndef MEGADEV__MAIN_IO_DEF_H
#define MEGADEV__MAIN_IO_DEF_H

/**
 * @defgroup ioports Main CPU / I/O Ports
 */

/**
 * @defgroup ioports_p Main CPU / I/O Ports / Parallel Mode
 */

/**
 * @defgroup ioports_s Main CPU / I/O Ports / Serial Mode
 */

/**
 * @def _IO_DATA1
 * @brief IO Data Port for Terminal 1
 * @ingroup ioports
 * @ingroup ioports_p
 */
#define _IO_DATA1 0xA10003

/**
 * @def _IO_DATA2
 * @brief IO Data Port for Terminal 2
 * @ingroup ioports
 * @ingroup ioports_p
 */
#define _IO_DATA2 0xA10005

/**
 * @def _IO_DATA3
 * @brief IO Data Port for Terminal 3
 * @ingroup ioports_p
 */
#define _IO_DATA3 0xA10007

/**
 * @def _IO_CTRL1
 * @brief IO Control Port for Terminal 1
 * @ingroup ioports_p
 */
#define _IO_CTRL1 0xA10009

/**
 * @def _IO_CTRL2
 * @brief IO Control Port for Terminal 2
 * @ingroup ioports_p
 */
#define _IO_CTRL2 0xA1000B

/**
 * @def _IO_CTRL3
 * @brief IO Control Port for Terminal 3
 * @ingroup ioports_p
 */
#define _IO_CTRL3 0xA1000D

/**
 * @def CTRL_PC0
 * @brief IO Control Port / Pin 0 Mode
 * @ingroup ioports_p
 */
#define CTRL_PC0 (1 << 0)

/**
 * @def CTRL_PC1
 * @brief IO Control Port / Pin 1 Mode
 * @ingroup ioports_p
 */
#define CTRL_PC1 (1 << 1)

/**
 * @def CTRL_PC2
 * @brief IO Control Port / Pin 2 Mode
 * @ingroup ioports_p
 */
#define CTRL_PC2 (1 << 2)

/**
 * @def CTRL_PC3
 * @brief IO Control Port / Pin 3 Mode
 * @ingroup ioports_p
 */
#define CTRL_PC3 (1 << 3)

/**
 * @def CTRL_PC4
 * @brief IO Control Port / Pin 4 Mode
 * @ingroup ioports_p
 */
#define CTRL_PC4 (1 << 4)

/**
 * @def CTRL_PC5
 * @brief IO Control Port / Pin 5 Mode
 * @ingroup ioports_p
 */
#define CTRL_PC5 (1 << 5)

/**
 * @def CTRL_PC6
 * @brief IO Control Port / Pin 6 Mode
 * @ingroup ioports_p
 */
#define CTRL_PC6 (1 << 6)

/**
 * @def CTRL_INT
 * @brief IO Control Port / INT2 on \TH enable.
 * @ingroup ioports_p
 */
#define CTRL_INT (1 << 7)

/**
 * @def _IO_TXDATA1
 * @brief Transmit Data Port for Terminal 1
 * @ingroup ioports_s
 */
#define _IO_TXDATA1 0xA1000F

/**
 * @def _IO_RXDATA1
 * @brief Receive Data Port for Terminal 1
 * @ingroup ioports_s
 */
#define _IO_RXDATA1 0xA10011

/**
 * @def _IO_SCTRL1
 * @brief Serial Control Port for Terminal 1
 * @ingroup ioports_s
 */
#define _IO_SCTRL1 0xA10013

/**
 * @def _IO_TXDATA2
 * @brief Transmit Data Port for Terminal 2
 * @ingroup ioports_s
 */
#define _IO_TXDATA2 0xA10015

/**
 * @def _IO_RXDATA2
 * @brief Receive Data Port for Terminal 2
 * @ingroup ioports_s
 */
#define _IO_RXDATA2 0xA10017

/**
 * @def _IO_SCTRL2
 * @brief Serial Control Port for Terminal 2
 * @ingroup ioports_s
 */
#define _IO_SCTRL2 0xA10019

/**
 * @def _IO_TXDATA3
 * @brief Transmit Data Port for Terminal 3
 * @ingroup ioports_s
 */
#define _IO_TXDATA3 0xA1001B

/**
 * @def _IO_RXDATA3
 * @brief Receive Data Port for Terminal 3
 * @ingroup ioports_s
 */
#define _IO_RXDATA3 0xA1001D

/**
 * @def _IO_SCTRL3
 * @brief Serial Control Port for Terminal 3
 * @ingroup ioports_s
 */
#define _IO_SCTRL3 0xA1001F

/**
 * @def SCTRL_TX_FULL
 * @brief IO Serial Control Register / Transmit buffer full flag
 * @ingroup ioports_s
 */
#define SCTRL_TX_FULL (1 << 0)

/**
 * @def SCTRL_RX_READY
 * @brief IO Serial Control Register / Receive buffer ready flag
 * @ingroup ioports_s
 */
#define SCTRL_RX_READY (1 << 1)

/**
 * @def SCTRL_RX_ERR
 * @brief IO Serial Control Register / Receive error flag
 * @ingroup ioports_s
 */
#define SCTRL_RX_ERR (1 << 2)

/**
 * @def SCTRL_RX_INT_ENABLE
 * @brief IO Serial Control Register / Enable INT2 on data receipt
 * @ingroup ioports_s
 */
#define SCTRL_RX_INT_ENABLE (1 << 3)

/**
 * @def SCTRL_SERIAL_OUT_ENABLE
 * @brief IO Serial Control Register / Enable serial output
 * @ingroup ioports_s
 */
#define SCTRL_SERIAL_OUT_ENABLE (1 << 4)

/**
 * @def SCTRL_SERIAL_IN_ENABLE
 * @brief IO Serial Control Register / Enable serial input
 * @ingroup ioports_s
 */
#define SCTRL_SERIAL_IN_ENABLE (1 << 5)

/**
 * @def SCTRL_SERIAL_ENABLE
 * @brief IO Serial Control Register / Enable serial input/output
 * @ingroup ioports_s
 */
#define SCTRL_SERIAL_ENABLE (0b11 << 4)

/**
 * @def SCTRL_BAUD_300
 * @brief IO Serial Control Register / Set baud rate 300
 * @ingroup ioports_s
 */
#define SCTRL_BAUD_300 (0b11 << 6)

/**
 * @def SCTRL_BAUD_1200
 * @brief IO Serial Control Register / Set baud rate 1200
 * @ingroup ioports_s
 */
#define SCTRL_BAUD_1200 (0b10 << 6)

/**
 * @def SCTRL_BAUD_2400
 * @brief IO Serial Control Register / Set baud rate 2400
 * @ingroup ioports_s
 */
#define SCTRL_BAUD_2400 (0b01 << 6)

/**
 * @def SCTRL_BAUD_4800
 * @brief IO Serial Control Register / Set baud rate 4800
 * @ingroup ioports_s
 */
#define SCTRL_BAUD_4800 (0b00 << 6)

/**
 * @brief Address space enabled by cartridge /TIME pin
 *
 * Size: 0x100 bytes
 */
#define _TIME_MAPPING 0xA13000

#endif

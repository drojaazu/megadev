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
 * Default bit mappings for joypad inputs when using the "standard" format of
 * SACBRLDU. This format is used the Mega CD's Main BIOS IO calls as well as
 * the read_input_joypad function in Megadev.
 */
#define PAD_UP    (1 << 0)
#define PAD_DOWN  (1 << 1)
#define PAD_LEFT  (1 << 2)
#define PAD_RIGHT (1 << 3)
#define PAD_A     (1 << 6)
#define PAD_B     (1 << 4)
#define PAD_C     (1 << 5)
#define PAD_ABC   (PAD_A | PAD_B | PAD_C)
#define PAD_START (1 << 7)

/**
 * @def IO_DATA1
 * @brief IO Data Port for Terminal 1
 * @ingroup ioports
 * @ingroup ioports_p
 */
#define IO_DATA1 0xA10003

/**
 * @def IO_DATA2
 * @brief IO Data Port for Terminal 2
 * @ingroup ioports
 * @ingroup ioports_p
 */
#define IO_DATA2 0xA10005

/**
 * @def IO_DATA3
 * @brief IO Data Port for Terminal 3
 * @ingroup ioports_p
 */
#define IO_DATA3 0xA10007

/**
 * @def IO_CTRL1
 * @brief IO Control Port for Terminal 1
 * @ingroup ioports_p
 */
#define IO_CTRL1 0xA10009

/**
 * @def IO_CTRL2
 * @brief IO Control Port for Terminal 2
 * @ingroup ioports_p
 */
#define IO_CTRL2 0xA1000B

/**
 * @def IO_CTRL3
 * @brief IO Control Port for Terminal 3
 * @ingroup ioports_p
 */
#define IO_CTRL3 0xA1000D

/**
 * @def CTRL_PC0
 * @brief IO Control Port / Pin 0 Mode (Up)
 * @ingroup ioports_p
 */
#define CTRL_PC0 (1 << 0)

/**
 * @def CTRL_PC1
 * @brief IO Control Port / Pin 1 Mode (Down)
 * @ingroup ioports_p
 */
#define CTRL_PC1 (1 << 1)

/**
 * @def CTRL_PC2
 * @brief IO Control Port / Pin 2 Mode (Right)
 * @ingroup ioports_p
 */
#define CTRL_PC2 (1 << 2)

/**
 * @def CTRL_PC3
 * @brief IO Control Port / Pin 3 Mode (Left)
 * @ingroup ioports_p
 */
#define CTRL_PC3 (1 << 3)

/**
 * @def CTRL_PC4
 * @brief IO Control Port / Pin 4 Mode (B/A)
 * @ingroup ioports_p
 */
#define CTRL_PC4 (1 << 4)

/**
 * @def CTRL_PC5
 * @brief IO Control Port / Pin 5 Mode (C/Start)
 * @ingroup ioports_p
 */
#define CTRL_PC5 (1 << 5)

/**
 * @def CTRL_PC6
 * @brief IO Control Port / Pin 6 Mode (TH)
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
 * @def IO_TXDATA1
 * @brief Transmit Data Port for Terminal 1
 * @ingroup ioports_s
 */
#define IO_TXDATA1 0xA1000F

/**
 * @def IO_RXDATA1
 * @brief Receive Data Port for Terminal 1
 * @ingroup ioports_s
 */
#define IO_RXDATA1 0xA10011

/**
 * @def IO_SCTRL1
 * @brief Serial Control Port for Terminal 1
 * @ingroup ioports_s
 */
#define IO_SCTRL1 0xA10013

/**
 * @def IO_TXDATA2
 * @brief Transmit Data Port for Terminal 2
 * @ingroup ioports_s
 */
#define IO_TXDATA2 0xA10015

/**
 * @def IO_RXDATA2
 * @brief Receive Data Port for Terminal 2
 * @ingroup ioports_s
 */
#define IO_RXDATA2 0xA10017

/**
 * @def IO_SCTRL2
 * @brief Serial Control Port for Terminal 2
 * @ingroup ioports_s
 */
#define IO_SCTRL2 0xA10019

/**
 * @def IO_TXDATA3
 * @brief Transmit Data Port for Terminal 3
 * @ingroup ioports_s
 */
#define IO_TXDATA3 0xA1001B

/**
 * @def IO_RXDATA3
 * @brief Receive Data Port for Terminal 3
 * @ingroup ioports_s
 */
#define IO_RXDATA3 0xA1001D

/**
 * @def IO_SCTRL3
 * @brief Serial Control Port for Terminal 3
 * @ingroup ioports_s
 */
#define IO_SCTRL3 0xA1001F

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
#define TIME_MAPPING 0xA13000

#endif

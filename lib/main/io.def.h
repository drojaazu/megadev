/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file io.def.h
 * @brief I/O register definitions
 */

#pragma once

/**
 * @defgroup ioports Main CPU / I/O Ports
 */

/**
 * @defgroup ioports_p Main CPU / I/O Ports / Parallel Mode
 * @ingroup ioports
 */

/**
 * @defgroup ioports_s Main CPU / I/O Ports / Serial Mode
 */

/**
 * Default bit mappings for joypad inputs when using the "standard" format of
 * SACBRLDU. This format is used the Mega CD's Main BIOS IO calls as well as
 * the read_input_joypad function in Megadev.
 */
#define PAD_UP    (1 << 0)                ///< D-pad up
#define PAD_DOWN  (1 << 1)                ///< D-pad down
#define PAD_LEFT  (1 << 2)                ///< D-pad left
#define PAD_RIGHT (1 << 3)                ///< D-pad right
#define PAD_A     (1 << 6)                ///< A button
#define PAD_B     (1 << 4)                ///< B button
#define PAD_C     (1 << 5)                ///< C button
#define PAD_ABC   (PAD_A | PAD_B | PAD_C) ///< Any of A, B, C
#define PAD_START (1 << 7)                ///< Start button

/**
 * @def IO_DATA1
 * @brief IO Data Port for Terminal 1
 * @ingroup ioports_p
 */
#define IO_DATA1 0xA10003

/**
 * @def IO_DATA2
 * @brief IO Data Port for Terminal 2
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
 * @brief IO Control Port / INT2 on TH enable.
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
 * @def SCTRL_TX_FULL_POS
 * @brief IO Serial Control Register / Transmit buffer full flag, bit position
 * @ingroup ioports_s
 */
#define SCTRL_TX_FULL_POS 0

/**
 * @def SCTRL_TX_FULL_MASK
 * @brief IO Serial Control Register / Transmit buffer full flag, mask
 * @ingroup ioports_s
 */
#define SCTRL_TX_FULL_MASK (1 << SCTRL_TX_FULL_POS)

/**
 * @def SCTRL_RX_READY_POS
 * @brief IO Serial Control Register / Receive buffer ready flag, bit position
 * @ingroup ioports_s
 */
#define SCTRL_RX_READY_POS 1

/**
 * @def SCTRL_RX_READY_MASK
 * @brief IO Serial Control Register / Receive buffer ready flag, mask
 * @ingroup ioports_s
 */
#define SCTRL_RX_READY_MASK (1 << SCTRL_RX_READY_POS)

/**
 * @def SCTRL_RX_ERR_POS
 * @brief IO Serial Control Register / Receive error flag, bit position
 * @ingroup ioports_s
 */
#define SCTRL_RX_ERR_POS 2

/**
 * @def SCTRL_RX_ERR_MASK
 * @brief IO Serial Control Register / Receive error flag, mask
 * @ingroup ioports_s
 */
#define SCTRL_RX_ERR_MASK (1 << SCTRL_RX_ERR_POS)

/**
 * @def SCTRL_RX_INT_ENABLE_POS
 * @brief IO Serial Control Register / Enable INT2 on data receipt flag, bit
 * position
 * @ingroup ioports_s
 */
#define SCTRL_RX_INT_ENABLE_POS 3

/**
 * @def SCTRL_RX_INT_ENABLE_MASK
 * @brief IO Serial Control Register / Enable INT2 on data receipt flag, mask
 * @ingroup ioports_s
 */
#define SCTRL_RX_INT_ENABLE_MASK (1 << SCTRL_RX_INT_ENABLE_POS)

/**
 * @def SCTRL_SERIAL_OUT_ENABLE_POS
 * @brief IO Serial Control Register / Enable serial output flag, bit position
 * @ingroup ioports_s
 */
#define SCTRL_SERIAL_OUT_ENABLE_POS 4

/**
 * @def SCTRL_SERIAL_OUT_ENABLE_MASK
 * @brief IO Serial Control Register / Enable serial output flag, mask
 * @ingroup ioports_s
 */
#define SCTRL_SERIAL_OUT_ENABLE_MASK (1 << SCTRL_SERIAL_OUT_ENABLE_POS)

/**
 * @def SCTRL_SERIAL_IN_ENABLE_POS
 * @brief IO Serial Control Register / Enable serial input flag, bit position
 * @ingroup ioports_s
 */
#define SCTRL_SERIAL_IN_ENABLE_POS 5

/**
 * @def SCTRL_SERIAL_IN_ENABLE_MASK
 * @brief IO Serial Control Register / Enable serial input flag, mask
 * @ingroup ioports_s
 */
#define SCTRL_SERIAL_IN_ENABLE_MASK (1 << SCTRL_SERIAL_IN_ENABLE_POS)

/**
 * @def SCTRL_SERIAL_ENABLE_MASK
 * @brief IO Serial Control Register / Enable serial input and output
 * @ingroup ioports_s
 */
#define SCTRL_SERIAL_ENABLE_MASK                               \
  (SCTRL_SERIAL_OUT_ENABLE_MASK | SCTRL_SERIAL_IN_ENABLE_MASK)

/**
 * @def SCTRL_BAUD_POS
 * @brief IO Serial Control Register / Baud rate field, bit position
 * @ingroup ioports_s
 */
#define SCTRL_BAUD_POS 6

/**
 * @def SCTRL_BAUD_WIDTH
 * @brief IO Serial Control Register / Baud rate field, width
 * @ingroup ioports_s
 */
#define SCTRL_BAUD_WIDTH 2

/**
 * @def SCTRL_BAUD_MASK
 * @brief IO Serial Control Register / Baud rate field, mask
 * @ingroup ioports_s
 */
#define SCTRL_BAUD_MASK FIELD_MASK(SCTRL_BAUD_POS, SCTRL_BAUD_WIDTH)

/**
 * @def SCTRL_BAUD_300
 * @brief Baud rate 300; a value for the SCTRL_BAUD field, placed with
 * FIELD_PREP
 * @ingroup ioports_s
 */
#define SCTRL_BAUD_300 0b11

/**
 * @def SCTRL_BAUD_1200
 * @brief Baud rate 1200; a value for the SCTRL_BAUD field, placed with
 * FIELD_PREP
 * @ingroup ioports_s
 */
#define SCTRL_BAUD_1200 0b10

/**
 * @def SCTRL_BAUD_2400
 * @brief Baud rate 2400; a value for the SCTRL_BAUD field, placed with
 * FIELD_PREP
 * @ingroup ioports_s
 */
#define SCTRL_BAUD_2400 0b01

/**
 * @def SCTRL_BAUD_4800
 * @brief Baud rate 4800; a value for the SCTRL_BAUD field, placed with
 * FIELD_PREP
 * @ingroup ioports_s
 */
#define SCTRL_BAUD_4800 0b00

/**
 * @brief Address space enabled by cartridge /TIME pin
 *
 * Size: 0x100 bytes
 */
#define TIME_MAPPING 0xA13000

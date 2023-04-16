/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file io.def.h
 * @brief I/O register definitions
 */

#ifndef MEGADEV__MAIN_IO_DEF_H
#define MEGADEV__MAIN_IO_DEF_H

/**
 * @var u8 _IO_DATA1
 * @brief Data port for port 1
 */
#define _IO_DATA1 0xA10003

/**
 * @var u8 _IO_DATA2
 * @brief Data port for port 2
 */
#define _IO_DATA2 0xA10005

/**
 * @var u8 _IO_DATA3
 * @brief Data port for port 3
 */
#define _IO_DATA3 0xA10007

/**
 * @var u8 _IO_CTRL1
 * @brief Control port for port 1
 */
#define _IO_CTRL1 0xA10009

/**
 * @var u8 _IO_CTRL2
 * @brief Control port for port 2
 */
#define _IO_CTRL2 0xA1000B

/**
 * @var u8 _IO_CTRL3
 * @brief Control port for port 3
 */
#define _IO_CTRL3 0xA1000D

/**
 * @var u8 _IO_TXDATA1
 * @brief Transmit data port for port 1
 */
#define _IO_TXDATA1 0xA1000F

/**
 * @var u8 _IO_RXDATA1
 * @brief Receive data port for port 1
 */
#define _IO_RXDATA1 0xA10011

/**
 * @var u8 _IO_SCTRL1
 * @brief Serial control port for port 1
 */
#define _IO_SCTRL1 0xA10013

/**
 * @var u8 _IO_TXDATA2
 * @brief Transmit data port for port 2
 */
#define _IO_TXDATA2 0xA10015

/**
 * @var u8 _IO_RXDATA2
 * @brief Receive data port for port 2
 */
#define _IO_RXDATA2 0xA10017

/**
 * @var u8 _IO_SCTRL2
 * @brief Serial control port for port 2
 */
#define _IO_SCTRL2 0xA10019

/**
 * @var u8 _IO_TXDATA3
 * @brief Transmit data port for port 3
 */
#define _IO_TXDATA3 0xA1001B

/**
 * @var u8 _IO_RXDATA3
 * @brief Receive data port for port 3
 */
#define _IO_RXDATA3 0xA1001D

/**
 * @var u8 _IO_SCTRL3
 * @brief Serial control port for port 3
 */
#define _IO_SCTRL3 0xA1001F

/**
 * Control port bits/masks
 */
#define CTRL_PC0_BIT 0
#define CTRL_PC1_BIT 1
#define CTRL_PC2_BIT 2
#define CTRL_PC3_BIT 3
#define CTRL_PC4_BIT 4
#define CTRL_PC5_BIT 5
#define CTRL_PC6_BIT 6
#define CTRL_INT_BIT 7

#define CTRL_PC0_MSK 1 << CTRL_PC0_BIT
#define CTRL_PC1_MSK 1 << CTRL_PC1_BIT
#define CTRL_PC2_MSK 1 << CTRL_PC2_BIT
#define CTRL_PC3_MSK 1 << CTRL_PC3_BIT
#define CTRL_PC4_MSK 1 << CTRL_PC4_BIT
#define CTRL_PC5_MSK 1 << CTRL_PC5_BIT
#define CTRL_PC6_MSK 1 << CTRL_PC6_BIT
#define CTRL_INT_MSK 1 << CTRL_INT_BIT

/**
 * Serial control port bits/masks
 */
#define SCTRL_TFUL_BIT 0
#define SCTRL_RRDY_BIT 1
#define SCTRL_RERR_BIT 2
#define SCTRL_RINT_BIT 3
#define SCTRL_SOUT_BIT 4
#define SCTRL_SIN_BIT 5
#define SCTRL_BAUD_BIT 6

#define SCTRL_TFUL_MSK 1 << SCTRL_TFUL_BIT
#define SCTRL_RRDY_MSK 1 << SCTRL_RRDY_BIT
#define SCTRL_RERR_MSK 1 << SCTRL_RERR_BIT
#define SCTRL_RINT_MSK 1 << SCTRL_RINT_BIT
#define SCTRL_SOUT_MSK 1 << SCTRL_SOUT_BIT
#define SCTRL_SIN_MSK 1 << SCTRL_SIN_BIT
#define SCTRL_BAUD_MSK 0b11 << SCTRL_BAUD_BIT

#define B300_MSK 3 << SCTRL_BAUD_BIT
#define B1200_MSK 2 << SCTRL_BAUD_BIT
#define B2400_MSK 1 << SCTRL_BAUD_BIT
#define B4800_MSK 0 << SCTRL_BAUD_BIT

/**
 * Input bits/masks
 */
#define PAD_UP_BIT 0
#define PAD_DOWN_BIT 1
#define PAD_LEFT_BIT 2
#define PAD_RIGHT_BIT 3
#define PAD_A_BIT 6
#define PAD_B_BIT 4
#define PAD_C_BIT 5
#define PAD_START_BIT 7

#define PAD_UP_MSK 1 << PAD_UP_BIT
#define PAD_DOWN_MSK 1 << PAD_DOWN_BIT
#define PAD_LEFT_MSK 1 << PAD_LEFT_BIT
#define PAD_RIGHT_MSK 1 << PAD_RIGHT_BIT
#define PAD_A_MSK (1 << PAD_A_BIT)
#define PAD_B_MSK (1 << PAD_B_BIT)
#define PAD_C_MSK (1 << PAD_C_BIT)
#define PAD_ABC_MSK (PAD_A_MSK | PAD_B_MSK | PAD_C_MSK)
#define PAD_START_MSK 1 << PAD_START_BIT

/**
 * @brief Address space enabled by cartridge /TIME pin
 *
 * Size: 0x100 bytes
 */
#define _TIME_MAPPING 0xA13000

#endif

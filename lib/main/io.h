/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file io.h
 * @brief C wrappers for I/O registers
 */

#ifndef MEGADEV__IO_H
#define MEGADEV__IO_H

#include "main/io.def.h"
#include "types.h"

typedef u8 volatile * IO_REGISTER;

/**
 * @sa _IO_DATA1
 */
#define IO_DATA1 ((IO_REGISTER) _IO_DATA1)

/**
 * @sa _IO_DATA2
 */
#define IO_DATA2 ((IO_REGISTER) _IO_DATA2)

/**
 * @sa _IO_DATA3
 */
#define IO_DATA3 ((IO_REGISTER) _IO_DATA3)

/**
 * @sa _IO_CTRL1
 */
#define IO_CTRL1 ((IO_REGISTER) _IO_CTRL1)

/**
 * @sa _IO_CTRL2
 */
#define IO_CTRL2 ((IO_REGISTER) _IO_CTRL2)

/**
 * @sa _IO_CTRL3
 */
#define IO_CTRL3 ((IO_REGISTER) _IO_CTRL3)

/**
 * @sa _IO_TXDATA1
 */
#define IO_TXDATA1 ((IO_REGISTER) _IO_TXDATA1)

/**
 * @sa _IO_TXDATA2
 */
#define IO_TXDATA2 ((IO_REGISTER) _IO_TXDATA2)

/**
 * @sa _IO_TXDATA3
 */
#define IO_TXDATA3 ((IO_REGISTER) _IO_TXDATA3)

/**
 * @sa _IO_RXDATA1
 */
#define IO_RXDATA1 ((const IO_REGISTER) _IO_RXDATA1)

/**
 * @sa _IO_RXDATA2
 */
#define IO_RXDATA2 ((const IO_REGISTER) _IO_RXDATA2)

/**
 * @sa _IO_RXDATA3
 */
#define IO_RXDATA3 ((const IO_REGISTER) _IO_RXDATA3)

/**
 * @sa _IO_SCTRL1
 */
#define IO_SCTRL1 ((IO_REGISTER) _IO_SCTRL1)

/**
 * @sa _IO_SCTRL2
 */
#define IO_SCTRL2 ((IO_REGISTER) _IO_SCTRL2)

/**
 * @sa _IO_SCTRL3
 */
#define IO_SCTRL3 ((IO_REGISTER) _IO_SCTRL3)

/**
 * @sa _TIME_MAPPING
 */
#define TIME_MAPPING ((u8)[0x100] _TIME_MAPPING)

static inline void init_joypads()
{
	u8 ctrl_pc6 = CTRL_PC6;
	u32 io_ctrl1 = (u32) _IO_CTRL1;
	u32 io_ctrl2 = (u32) _IO_CTRL2;
	
	__asm__(
		"\
		move.b  #%c0, (%c1) \n \
		move.b  #%c0, (%c2) \n \
		"
		: 
		: "i"(ctrl_pc6), "i"(io_ctrl1), "i"(io_ctrl2)
		: );
}

static inline u8 read_input_joypad(IO_REGISTER data_port)
{
	register u32 A0 __asm__("a0") = (u32) data_port;
	register u8 D0 __asm__("d0");
	u8 ctrl_pc6 = CTRL_PC6;
	
	__asm__(
		"\
		moveq	#%c2, d0 \n \
		move.b	#%c2, (a0) \n \
		nop \n \
		nop \n \
		move.b	(a0), d0 \n \
		andi.b	#0x3F, d0 \n \
		moveq	#0,d1 \n \
		move.b	#0, (a0) \n \
		nop \n \
		nop \n \
		move.b	(a0),d1 \n \
		andi.b	#0x30, d1 \n \
		lsl.b	#2, d1 \n \
		or.b	d1, d0 \n \
		"
		: "=d"(D0)
		: "a"(A0), "i"(ctrl_pc6)
		: "d1");
	
	return D0;
}

#endif

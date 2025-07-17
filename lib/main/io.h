/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file io.h
 * @brief I/O utilities
 */

#ifndef MEGADEV__MAIN_IO_H
#define MEGADEV__MAIN_IO_H

#include <main/io.def.h>
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
	asm(
		"\
  move.b   #%c[ctrl_pc6], (%c[io_ctrl1]) \n\
  move.b   #%c[ctrl_pc6], (%c[io_ctrl2]) \n\
		"
		: 
		:
			[ctrl_pc6] "i"(CTRL_PC6),
			[io_ctrl1] "i"(_IO_CTRL1),
			[io_ctrl2] "i"(_IO_CTRL2)
		:
			"cc"
	);
}


static inline u8 read_input_joypad(IO_REGISTER io_port)
{
	register u32 scratch_d;
	register u8 joypad_state;
	
	asm volatile(
		"\
  move.b   #%c[ctrl_pc6], (%[io_port]) \n\
  nop \n\
  nop \n\
  move.b   (%[io_port]), %[joypad_state] \n\
  move.b   #0, (%[io_port]) \n\
  nop \n\
  nop \n\
  move.b   (%[io_port]), %[scratch_d] \n\
  andi.b   #0x3F, %[joypad_state] \n\
  andi.b   #0x30, %[scratch_d] \n\
  lsl.b    #2, %[scratch_d] \n\
  or.b     d1, %[joypad_state] \n\
		"
		:
			[joypad_state] "=d"(joypad_state),
			[scratch_d] "=&d"(scratch_d)
		:
			[io_port] "a"(io_port),
			[ctrl_pc6] "i"(CTRL_PC6)
		:
			"cc"
	);
	
	return joypad_state;
}


#endif

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file io.h
 * @brief I/O utilities
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include <main/io.def.h>

/** @brief Pointer to an I/O port register */
typedef uint8_t volatile * io_reg;

/**
 * @sa IO_DATA1
 */
#define io_data1 ((io_reg) IO_DATA1)

/**
 * @sa IO_DATA2
 */
#define io_data2 ((io_reg) IO_DATA2)

/**
 * @sa IO_DATA3
 */
#define io_data3 ((io_reg) IO_DATA3)

/**
 * @sa IO_CTRL1
 */
#define io_ctrl1 ((io_reg) IO_CTRL1)

/**
 * @sa IO_CTRL2
 */
#define io_ctrl2 ((io_reg) IO_CTRL2)

/**
 * @sa IO_CTRL3
 */
#define io_ctrl3 ((io_reg) IO_CTRL3)

/**
 * @sa IO_TXDATA1
 */
#define io_txdata1 ((io_reg) IO_TXDATA1)

/**
 * @sa IO_TXDATA2
 */
#define io_txdata2 ((io_reg) IO_TXDATA2)

/**
 * @sa IO_TXDATA3
 */
#define io_txdata3 ((io_reg) IO_TXDATA3)

/**
 * @sa IO_RXDATA1
 */
#define io_rxdata1 ((const io_reg) IO_RXDATA1)

/**
 * @sa IO_RXDATA2
 */
#define io_rxdata2 ((const io_reg) IO_RXDATA2)

/**
 * @sa IO_RXDATA3
 */
#define io_rxdata3 ((const io_reg) IO_RXDATA3)

/**
 * @sa IO_SCTRL1
 */
#define io_sctrl1 ((io_reg) IO_SCTRL1)

/**
 * @sa IO_SCTRL2
 */
#define io_sctrl2 ((io_reg) IO_SCTRL2)

/**
 * @sa IO_SCTRL3
 */
#define io_sctrl3 ((io_reg) IO_SCTRL3)

/**
 * @sa TIME_MAPPING
 */
#define time_mapping (*((uint8_t (*)[0x100]) TIME_MAPPING))

/** @brief Set both control ports up for a 3 button pad: TH as output */
static inline void init_joypads()
{
  asm(
    "\
  move.b   #%c[ctrl_pc6], (%c[ctrl1]) \n\
  move.b   #%c[ctrl_pc6], (%c[ctrl2]) \n\
		"
    :
    : [ctrl_pc6] "i"(CTRL_PC6), [ctrl1] "i"(IO_CTRL1), [ctrl2] "i"(IO_CTRL2)
    : "cc");
}

/**
 * @brief Read a 3 button pad
 * @param io_port Data port of the controller (io_data1 or io_data2)
 * @return Button states in SACBRLDU order (PAD_*), active low: 0 = pressed
 */
static inline uint8_t read_input_joypad(io_reg io_port)
{
  register uint32_t scratch_d;
  register uint8_t  joypad_state;

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
  or.b     %[scratch_d], %[joypad_state] \n\
		"
    : [joypad_state] "=d"(joypad_state), [scratch_d] "=&d"(scratch_d)
    : [io_port] "a"(io_port), [ctrl_pc6] "i"(CTRL_PC6)
    : "cc");

  return joypad_state;
}

#ifdef __cplusplus
}
#endif

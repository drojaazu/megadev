/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.h
 * @brief C wrappers for Main CPU side Gate Array registers
 */

#ifndef MEGADEV__MAIN_GATE_ARR_H
#define MEGADEV__MAIN_GATE_ARR_H

#include "main/gate_arr.def.h"
#include <types.h>

typedef u16 volatile * ga_reg16;

/**
 * @sa GA_REG_COMM_FLAGS
 */
#define ga_reg_comflags_main ((volatile u8 *) GA_REG_COMM_FLAGS)

#define ga_reg_comflags_sub ((volatile const u8 *) GA_REG_COMM_FLAGS + 1)

/**
 * @copydoc GA_REG_RESET
 *
 */
#define ga_reg_reset ((ga_reg16) GA_REG_RESET)

/**
 * @copydoc GA_REG_MEMMODE
 *
 */
#define ga_reg_memmode ((ga_reg16) GA_REG_MEMMODE)

/**
 * @copydoc GA_REG_CDC_MODE
 *
 */
#define ga_reg_cdcmode ((ga_reg16) GA_REG_CDC_MODE)

/**
 * @copydoc GA_REG_HBLANKVECT
 *
 */
#define ga_reg_hblankvect ((ga_reg16) GA_REG_HBLANKVECT)

/**
 * @copydoc GA_REG_CDC_DATA
 *
 */
#define ga_reg_cdchostdata ((ga_reg16) GA_REG_CDC_DATA)

/**
 * @copydoc GA_REG_STOPWATCH
 *
 */
#define ga_reg_stopwatch ((ga_reg16) GA_REG_STOPWATCH)

/**
 * @copydoc GA_REG_COMM_FLAGS
 *
 */
#define ga_reg_comflags ((ga_reg16) GA_REG_COMM_FLAGS)

/**
 * @copydoc GA_REG_GA_REG_COMM_CMD0
 *
 */
#define ga_reg_comcmd0 ((ga_reg16) GA_REG_COMM_CMD0)

/**
 * @copydoc GA_REG_COMM_CMD1
 *
 */
#define ga_reg_comcmd1 ((ga_reg16) GA_REG_COMM_CMD1)

/**
 * @copydoc GA_REG_COMM_CMD2
 *
 */
#define ga_reg_comcmd2 ((ga_reg16) GA_REG_COMM_CMD2)

/**
 * @copydoc GA_REG_COMM_CMD3
 *
 */
#define ga_reg_comcmd3 ((ga_reg16) GA_REG_COMM_CMD3)

/**
 * @copydoc GA_REG_COMM_CMD4
 *
 */
#define ga_reg_comcmd4 ((ga_reg16) GA_REG_COMM_CMD4)

/**
 * @copydoc GA_REG_COMM_CMD5
 *
 */
#define ga_reg_comcmd5 ((ga_reg16) GA_REG_COMM_CMD5)

/**
 * @copydoc GA_REG_COMM_CMD6
 *
 */
#define ga_reg_comcmd6 ((ga_reg16) GA_REG_COMM_CMD6)

/**
 * @copydoc GA_REG_COMM_CMD7
 *
 */
#define ga_reg_comcmd7 ((ga_reg16) GA_REG_COMM_CMD7)

/**
 * @copydoc GA_REG_COMM_STAT0
 *
 */
#define ga_reg_comstat0 ((ga_reg16) GA_REG_COMM_STAT0)

/**
 * @copydoc GA_REG_COMM_STAT1
 *
 */
#define ga_reg_comstat1 ((ga_reg16) GA_REG_COMM_STAT1)

/**
 * @copydoc GA_REG_COMM_STAT2
 *
 */
#define ga_reg_comstat2 ((ga_reg16) GA_REG_COMM_STAT2)

/**
 * @copydoc GA_REG_COMM_STAT3
 *
 */
#define ga_reg_comstat3 ((ga_reg16) GA_REG_COMM_STAT3)

/**
 * @copydoc GA_REG_COMM_STAT4
 *
 */
#define ga_reg_comstat4 ((ga_reg16) GA_REG_COMM_STAT4)

/**
 * @copydoc GA_REG_COMM_STAT5
 *
 */
#define ga_reg_comstat5 ((ga_reg16) GA_REG_COMM_STAT5)

/**
 * @copydoc GA_REG_COMM_STAT6
 *
 */
#define ga_reg_comstat6 ((ga_reg16) GA_REG_COMM_STAT6)

/**
 * @copydoc GA_REG_COMM_STAT7
 *
 */
#define ga_reg_comstat7 ((ga_reg16) GA_REG_COMM_STAT7)

/**
 * @fn wait_2m
 * Wait for Main CPU access to 2M Word RAM
 */
static inline void wait_2m()
{
  asm volatile(
    "\
1:btst     #%c[ga_ret_bit], %c[ga_reg_memmmode] \n\
  beq      1b \n\
		"
    :
    : [ga_ret_bit] "i"(GA_CEDE_WDRAM2M_BIT),
      [ga_reg_memmmode] "i"(GA_REG_MEMMODE + 1));
}

/**
 * @fn grant_2m
 * Grant 2M Word RAM access to the Sub CPU and wait for confirmation
 */
static inline void grant_2m()
{
  asm volatile(
    "\
1:bset     #%c[ga_dmna_bit], %c[ga_reg_memmmode] \n\
  btst     #%c[ga_dmna_bit], %c[ga_reg_memmmode] \n\
  beq      1b \n\
		"
    :
    : [ga_dmna_bit] "i"(GA_ACQUIRE_WDRAM2M_BIT),
      [ga_reg_memmmode] "i"(GA_REG_MEMMODE + 1));
}

/**
 * @fn clear_comm_regs
 * Clears the Main comm registers (COMCMD) and flags
 */
static inline void clear_comm_regs()
{
  register u32 scratch_d, scratch_a;

  asm volatile(
    "\
  lea (%c[comcmd0]), %[scratch_a] \n\
  moveq    #0, %[scratch_d] \n\
  move.b   %[scratch_d], -2(%[scratch_a]) \n\
  move.l   %[scratch_d], (%[scratch_a])+ \n\
  move.l   %[scratch_d], (%[scratch_a])+ \n\
  move.l   %[scratch_d], (%[scratch_a])+ \n\
  move.l   %[scratch_d], (%[scratch_a])+ \n\
		"
    : [scratch_d] "=&d"(scratch_d), [scratch_a] "=&a"(scratch_a)
    : [comcmd0] "i"(GA_REG_COMM_CMD0));
}

/**
 * @fn reset_ga
 * Perform a system reset
 */
static inline void reset_ga()
{
  asm volatile(
    "\
  move.w   #0xFF00, %c[memmode] \n\
  move.b   #0x3, %c[reset] \n\
  nop \n\
  move.b   #0x3, %c[reset] \n\
  move.b   #0x2, %c[reset] \n\
  move.b   #0x0, %c[reset] \n\
		"
    :
    : [memmode] "i"(GA_REG_MEMMODE), [reset] "i"(GA_REG_RESET + 1)
    :);
}

#endif

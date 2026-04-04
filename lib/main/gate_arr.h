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

typedef u16 volatile * ga_reg;

/**
 * @sa GA_REG_COMFLAGS
 */
#define ga_reg_comflags_main ((volatile u8 *) GA_REG_COMFLAGS)

#define ga_reg_comflags_sub ((volatile const u8 *) GA_REG_COMFLAGS + 1)

/**
 * @copydoc GA_REG_RESET
 *
 */
#define ga_reg_reset ((ga_reg) GA_REG_RESET)

/**
 * @copydoc GA_REG_MEMMODE
 *
 */
#define ga_reg_memmode ((ga_reg) GA_REG_MEMMODE)

/**
 * @copydoc GA_REG_CDCMODE
 *
 */
#define ga_reg_cdcmode ((ga_reg) GA_REG_CDCMODE)

/**
 * @copydoc GA_REG_HBLANKVECT
 *
 */
#define ga_reg_hblankvect ((ga_reg) GA_REG_HBLANKVECT)

/**
 * @copydoc GA_REG_CDCHOSTDATA
 *
 */
#define ga_reg_cdchostdata ((ga_reg) GA_REG_CDCHOSTDATA)

/**
 * @copydoc GA_REG_STOPWATCH
 *
 */
#define ga_reg_stopwatch ((ga_reg) GA_REG_STOPWATCH)

/**
 * @copydoc GA_REG_COMFLAGS
 *
 */
#define ga_reg_comflags ((ga_reg) GA_REG_COMFLAGS)

/**
 * @copydoc GA_REG_GA_REG_COMCMD0
 *
 */
#define ga_reg_comcmd0 ((ga_reg) GA_REG_COMCMD0)

/**
 * @copydoc GA_REG_COMCMD1
 *
 */
#define ga_reg_comcmd1 ((ga_reg) GA_REG_COMCMD1)

/**
 * @copydoc GA_REG_COMCMD2
 *
 */
#define ga_reg_comcmd2 ((ga_reg) GA_REG_COMCMD2)

/**
 * @copydoc GA_REG_COMCMD3
 *
 */
#define ga_reg_comcmd3 ((ga_reg) GA_REG_COMCMD3)

/**
 * @copydoc GA_REG_COMCMD4
 *
 */
#define ga_reg_comcmd4 ((ga_reg) GA_REG_COMCMD4)

/**
 * @copydoc GA_REG_COMCMD5
 *
 */
#define ga_reg_comcmd5 ((ga_reg) GA_REG_COMCMD5)

/**
 * @copydoc GA_REG_COMCMD6
 *
 */
#define ga_reg_comcmd6 ((ga_reg) GA_REG_COMCMD6)

/**
 * @copydoc GA_REG_COMCMD7
 *
 */
#define ga_reg_comcmd7 ((ga_reg) GA_REG_COMCMD7)

/**
 * @copydoc GA_REG_COMSTAT0
 *
 */
#define ga_reg_comstat0 ((ga_reg) GA_REG_COMSTAT0)

/**
 * @copydoc GA_REG_COMSTAT1
 *
 */
#define ga_reg_comstat1 ((ga_reg) GA_REG_COMSTAT1)

/**
 * @copydoc GA_REG_COMSTAT2
 *
 */
#define ga_reg_comstat2 ((ga_reg) GA_REG_COMSTAT2)

/**
 * @copydoc GA_REG_COMSTAT3
 *
 */
#define ga_reg_comstat3 ((ga_reg) GA_REG_COMSTAT3)

/**
 * @copydoc GA_REG_COMSTAT4
 *
 */
#define ga_reg_comstat4 ((ga_reg) GA_REG_COMSTAT4)

/**
 * @copydoc GA_REG_COMSTAT5
 *
 */
#define ga_reg_comstat5 ((ga_reg) GA_REG_COMSTAT5)

/**
 * @copydoc GA_REG_COMSTAT6
 *
 */
#define ga_reg_comstat6 ((ga_reg) GA_REG_COMSTAT6)

/**
 * @copydoc GA_REG_COMSTAT7
 *
 */
#define ga_reg_comstat7 ((ga_reg) GA_REG_COMSTAT7)

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
    : [ga_ret_bit] "i"(GA_BIT_RETURN_2M),
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
    :
    [ga_dmna_bit] "i"(GA_BIT_DMNA), [ga_reg_memmmode] "i"(GA_REG_MEMMODE + 1));
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
    : [comcmd0] "i"(GA_REG_COMCMD0));
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

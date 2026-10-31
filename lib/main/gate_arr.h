/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.h
 * @brief C wrappers for Main CPU side Gate Array registers
 */

#pragma once

#include "main/gate_arr.def.h"
#include <types.h>

typedef u16 volatile * ga_main_reg;

/**
 * @sa GA_MAIN_REG_COMFLAGS
 */
#define ga_main_reg_comflags_main (*((volatile u8 *) GA_MAIN_REG_COMFLAGS))

#define ga_main_reg_comflags_sub (*((volatile const u8 *) GA_MAIN_REG_COMFLAGS + 1))

/**
 * @copydoc GA_MAIN_REG_RESET
 *
 */
#define ga_main_reg_reset (*((ga_main_reg) GA_MAIN_REG_RESET))

/**
 * @copydoc GA_MAIN_REG_MEMMODE
 *
 */
#define ga_main_reg_memmode (*((ga_main_reg) GA_MAIN_REG_MEMMODE))

/**
 * @copydoc GA_MAIN_REG_CDCMODE
 *
 */
#define ga_main_reg_cdcmode (*((ga_main_reg) GA_MAIN_REG_CDCMODE))

/**
 * @copydoc GA_MAIN_REG_HBLANKVECT
 *
 */
#define ga_main_reg_hblankvect (*((ga_main_reg) GA_MAIN_REG_HBLANKVECT))

/**
 * @copydoc GA_MAIN_REG_CDCHOSTDATA
 *
 */
#define ga_main_reg_cdchostdata (*((ga_main_reg) GA_MAIN_REG_CDCHOSTDATA))

/**
 * @copydoc GA_MAIN_REG_STOPWATCH
 *
 */
#define ga_main_reg_stopwatch (*((ga_main_reg) GA_MAIN_REG_STOPWATCH))

/**
 * @copydoc GA_MAIN_REG_COMFLAGS
 *
 */
#define ga_main_reg_comflags (*((ga_main_reg) GA_MAIN_REG_COMFLAGS))

/**
 * @copydoc GA_MAIN_REG_COMCMD0
 *
 */
#define ga_main_reg_comcmd0 (*((ga_main_reg) GA_MAIN_REG_COMCMD0))

/**
 * @copydoc GA_MAIN_REG_COMCMD1
 *
 */
#define ga_main_reg_comcmd1 (*((ga_main_reg) GA_MAIN_REG_COMCMD1))

/**
 * @copydoc GA_MAIN_REG_COMCMD2
 *
 */
#define ga_main_reg_comcmd2 (*((ga_main_reg) GA_MAIN_REG_COMCMD2))

/**
 * @copydoc GA_MAIN_REG_COMCMD3
 *
 */
#define ga_main_reg_comcmd3 (*((ga_main_reg) GA_MAIN_REG_COMCMD3))

/**
 * @copydoc GA_MAIN_REG_COMCMD4
 *
 */
#define ga_main_reg_comcmd4 (*((ga_main_reg) GA_MAIN_REG_COMCMD4))

/**
 * @copydoc GA_MAIN_REG_COMCMD5
 *
 */
#define ga_main_reg_comcmd5 (*((ga_main_reg) GA_MAIN_REG_COMCMD5))

/**
 * @copydoc GA_MAIN_REG_COMCMD6
 *
 */
#define ga_main_reg_comcmd6 (*((ga_main_reg) GA_MAIN_REG_COMCMD6))

/**
 * @copydoc GA_MAIN_REG_COMCMD7
 *
 */
#define ga_main_reg_comcmd7 (*((ga_main_reg) GA_MAIN_REG_COMCMD7))

/**
 * @copydoc GA_MAIN_REG_COMSTAT0
 *
 */
#define ga_main_reg_comstat0 (*((ga_main_reg) GA_MAIN_REG_COMSTAT0))

/**
 * @copydoc GA_MAIN_REG_COMSTAT1
 *
 */
#define ga_main_reg_comstat1 (*((ga_main_reg) GA_MAIN_REG_COMSTAT1))

/**
 * @copydoc GA_MAIN_REG_COMSTAT2
 *
 */
#define ga_main_reg_comstat2 (*((ga_main_reg) GA_MAIN_REG_COMSTAT2))

/**
 * @copydoc GA_MAIN_REG_COMSTAT3
 *
 */
#define ga_main_reg_comstat3 (*((ga_main_reg) GA_MAIN_REG_COMSTAT3))

/**
 * @copydoc GA_MAIN_REG_COMSTAT4
 *
 */
#define ga_main_reg_comstat4 (*((ga_main_reg) GA_MAIN_REG_COMSTAT4))

/**
 * @copydoc GA_MAIN_REG_COMSTAT5
 *
 */
#define ga_main_reg_comstat5 (*((ga_main_reg) GA_MAIN_REG_COMSTAT5))

/**
 * @copydoc GA_MAIN_REG_COMSTAT6
 *
 */
#define ga_main_reg_comstat6 (*((ga_main_reg) GA_MAIN_REG_COMSTAT6))

/**
 * @copydoc GA_MAIN_REG_COMSTAT7
 *
 */
#define ga_main_reg_comstat7 (*((ga_main_reg) GA_MAIN_REG_COMSTAT7))

/**
 * @fn ga_main_wait_2m
 * Wait for Main CPU access to 2M Word RAM
 */
static inline void ga_main_wait_2m()
{
  asm volatile(
    "\
1:btst     #%c[ga_ret_bit], %c[memmode_lo] \n\
  beq      1b \n\
		"
    :
    : [ga_ret_bit] "i"(GA_MAIN_BIT_RETURN_2M),
      [memmode_lo] "i"(GA_MAIN_REG_MEMMODE + 1));
}

/**
 * @fn ga_main_grant_2m
 * Grant 2M Word RAM access to the Sub CPU and wait for confirmation
 */
static inline void ga_main_grant_2m()
{
  asm volatile(
    "\
1:bset     #%c[ga_dmna_bit], %c[memmode_lo] \n\
  btst     #%c[ga_dmna_bit], %c[memmode_lo] \n\
  beq      1b \n\
		"
    :
    :
    [ga_dmna_bit] "i"(GA_MAIN_BIT_DMNA), [memmode_lo] "i"(GA_MAIN_REG_MEMMODE + 1));
}

/**
 * @fn ga_main_clear_comm_regs
 * Clears the Main comm registers (COMCMD) and flags
 */
static inline void ga_main_clear_comm_regs()
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
    : [comcmd0] "i"(GA_MAIN_REG_COMCMD0));
}

/**
 * @fn ga_main_reset
 * Perform a system reset
 */
static inline void ga_main_reset()
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
    : [memmode] "i"(GA_MAIN_REG_MEMMODE), [reset] "i"(GA_MAIN_REG_RESET + 1)
    :);
}

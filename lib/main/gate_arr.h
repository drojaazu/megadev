/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.h
 * @brief C wrappers for Main CPU side Gate Array registers
 */

#pragma once
#include "main/gate_arr.def.h"
#include <types.h>

typedef u16 volatile * ga_reg;

/**
 * @typedef ga_reg_ro
 * @brief A register this CPU may read but not write
 * @details Note that `ga_reg const` does NOT mean this: it is a const pointer
 * to a mutable register, which enforces nothing at the point of use.
 */
typedef u16 volatile const * ga_reg_ro;

/**
 * @typedef ga_reg8
 * @brief Pointer to one byte of a gate array register
 * @details Registers are 16 bit, but several are routinely accessed a byte at
 * a time; use with the _HI and _LO address definitions.
 */
typedef u8 volatile * ga_reg8;

/**
 * @typedef ga_reg8_ro
 * @brief A byte register this CPU may read but not write
 */
typedef u8 volatile const * ga_reg8_ro;

/**
 * @sa GA_REG_COMFLAGS_MAIN, GA_REG_COMFLAGS_SUB
 */
#define ga_reg_comflags_main (*((ga_reg8) GA_REG_COMFLAGS_MAIN))
#define ga_reg_comflags_sub (*((ga_reg8_ro) GA_REG_COMFLAGS_SUB))
/**
 * @copydoc GA_REG_INT2
 *
 */
#define ga_reg_int2 (*((ga_reg8) GA_REG_INT2))
#define ga_reg_subcpu (*((ga_reg8) GA_REG_SUBCPU))
/**
 * @copydoc GA_REG_WP
 *
 */
#define ga_reg_wp (*((ga_reg8) GA_REG_WP))
#define ga_reg_memmode (*((ga_reg8) GA_REG_MEMMODE))
/**
 * @copydoc GA_REG_CDCMODE
 *
 */
#define ga_reg_cdcmode (*((ga_reg8_ro) GA_REG_CDCMODE))
/**
 * @copydoc GA_REG_HBLANKVECT
 *
 */
#define ga_reg_hblankvect (*((ga_reg) GA_REG_HBLANKVECT))
/**
 * @copydoc GA_REG_CDCHOSTDATA
 *
 */
#define ga_reg_cdchostdata (*((ga_reg_ro) GA_REG_CDCHOSTDATA))
/**
 * @copydoc GA_REG_STOPWATCH
 *
 */
#define ga_reg_stopwatch (*((ga_reg_ro) GA_REG_STOPWATCH))

/**
 * @copydoc GA_REG_COMCMD0
 *
 */
#define ga_reg_comcmd0 (*((ga_reg) GA_REG_COMCMD0))
/**
 * @copydoc GA_REG_COMCMD1
 *
 */
#define ga_reg_comcmd1 (*((ga_reg) GA_REG_COMCMD1))
/**
 * @copydoc GA_REG_COMCMD2
 *
 */
#define ga_reg_comcmd2 (*((ga_reg) GA_REG_COMCMD2))
/**
 * @copydoc GA_REG_COMCMD3
 *
 */
#define ga_reg_comcmd3 (*((ga_reg) GA_REG_COMCMD3))
/**
 * @copydoc GA_REG_COMCMD4
 *
 */
#define ga_reg_comcmd4 (*((ga_reg) GA_REG_COMCMD4))
/**
 * @copydoc GA_REG_COMCMD5
 *
 */
#define ga_reg_comcmd5 (*((ga_reg) GA_REG_COMCMD5))
/**
 * @copydoc GA_REG_COMCMD6
 *
 */
#define ga_reg_comcmd6 (*((ga_reg) GA_REG_COMCMD6))
/**
 * @copydoc GA_REG_COMCMD7
 *
 */
#define ga_reg_comcmd7 (*((ga_reg) GA_REG_COMCMD7))
/**
 * @copydoc GA_REG_COMSTAT0
 *
 */
#define ga_reg_comstat0 (*((ga_reg_ro) GA_REG_COMSTAT0))
/**
 * @copydoc GA_REG_COMSTAT1
 *
 */
#define ga_reg_comstat1 (*((ga_reg_ro) GA_REG_COMSTAT1))
/**
 * @copydoc GA_REG_COMSTAT2
 *
 */
#define ga_reg_comstat2 (*((ga_reg_ro) GA_REG_COMSTAT2))
/**
 * @copydoc GA_REG_COMSTAT3
 *
 */
#define ga_reg_comstat3 (*((ga_reg_ro) GA_REG_COMSTAT3))
/**
 * @copydoc GA_REG_COMSTAT4
 *
 */
#define ga_reg_comstat4 (*((ga_reg_ro) GA_REG_COMSTAT4))
/**
 * @copydoc GA_REG_COMSTAT5
 *
 */
#define ga_reg_comstat5 (*((ga_reg_ro) GA_REG_COMSTAT5))
/**
 * @copydoc GA_REG_COMSTAT6
 *
 */
#define ga_reg_comstat6 (*((ga_reg_ro) GA_REG_COMSTAT6))
/**
 * @copydoc GA_REG_COMSTAT7
 *
 */
#define ga_reg_comstat7 (*((ga_reg_ro) GA_REG_COMSTAT7))
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
    : [ga_ret_bit] "i"(GA_RETURN_2M_POS),
      [ga_reg_memmmode] "i"(GA_REG_MEMMODE));
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
    [ga_dmna_bit] "i"(GA_DMNA_POS), [ga_reg_memmmode] "i"(GA_REG_MEMMODE));
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
    : [memmode] "i"(GA_REG_WP), [reset] "i"(GA_REG_SUBCPU)
    :);
}



/* The _hi/_lo accessors for 0xA12000 and 0xA12002 are gone: those registers are
 * now named byte registers in their own right (D17). ga_reg_reset_hi is
 * ga_reg_int2, ga_reg_reset_lo is ga_reg_subcpu, ga_reg_memmode_hi is
 * ga_reg_wp, and ga_reg_memmode_lo is ga_reg_memmode. */
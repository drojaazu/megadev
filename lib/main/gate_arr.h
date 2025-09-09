/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.h
 * @brief C wrappers for Main CPU side Gate Array registers
 */

#ifndef MEGADEV__MAIN_GATE_ARRAY_H
#define MEGADEV__MAIN_GATE_ARRAY_H

#include "main/gate_arr.def.h"
#include <types.h>

typedef u16 volatile * ga_reg;

/**
 * @sa GAREG_COMFLAGS
 */
#define GA_COMFLAGS_MAIN ((volatile u8 *) GAREG_COMFLAGS)

#define GA_COMFLAGS_SUB ((volatile const u8 *) GAREG_COMFLAGS + 1)

/**
 * @copydoc GAREG_RESET
 *
 */
#define ga_reset ((ga_reg) GAREG_RESET)

#define GA_RESET_SRES (1 << 0)
#define GA_RESET_SBRQ (1 << 1)

/**
 * @copydoc GAREG_MEMMODE
 *
 */
#define ga_memmode ((ga_reg) GAREG_MEMMODE)

/**
 * @copydoc GAREG_CDCMODE
 *
 */
#define ga_cdcmode ((ga_reg) GAREG_CDCMODE)

/**
 * @copydoc GAREG_HINTVECT
 *
 */
#define ga_hintvect ((ga_reg) GAREG_HINTVECT)

/**
 * @copydoc GAREG_CDCHOSTDATA
 *
 */
#define ga_cdchostdata ((ga_reg) GAREG_CDCHOSTDATA)

/**
 * @copydoc GAREG_STOPWATCH
 *
 */
#define ga_stopwatch ((ga_reg) GAREG_STOPWATCH)

/**
 * @copydoc GAREG_COMFLAGS
 *
 */
#define ga_comflags ((ga_reg) GAREG_COMFLAGS)

/**
 * @copydoc _GAREG_GAREG_COMCMD0
 *
 */
#define gareg_comcmd0 ((ga_reg) GAREG_COMCMD0)

/**
 * @copydoc GAREG_COMCMD1
 *
 */
#define gareg_comcmd1 ((ga_reg) GAREG_COMCMD1)

/**
 * @copydoc GAREG_COMCMD2
 *
 */
#define gareg_comcmd2 ((ga_reg) GAREG_COMCMD2)

/**
 * @copydoc GAREG_COMCMD3
 *
 */
#define gareg_comcmd3 ((ga_reg) GAREG_COMCMD3)

/**
 * @copydoc GAREG_COMCMD4
 *
 */
#define gareg_comcmd4 ((ga_reg) GAREG_COMCMD4)

/**
 * @copydoc GAREG_COMCMD5
 *
 */
#define gareg_comcmd5 ((ga_reg) GAREG_COMCMD5)

/**
 * @copydoc GAREG_COMCMD6
 *
 */
#define gareg_comcmd6 ((ga_reg) GAREG_COMCMD6)

/**
 * @copydoc GAREG_COMCMD7
 *
 */
#define gareg_comcmd7 ((ga_reg) GAREG_COMCMD7)

/**
 * @copydoc GAREG_COMSTAT0
 *
 */
#define GA_COMSTAT0 ((ga_reg) GAREG_COMSTAT0)

/**
 * @copydoc GAREG_COMSTAT1
 *
 */
#define GA_COMSTAT1 ((ga_reg) GAREG_COMSTAT1)

/**
 * @copydoc GAREG_COMSTAT2
 *
 */
#define GA_COMSTAT2 ((ga_reg) GAREG_COMSTAT2)

/**
 * @copydoc GAREG_COMSTAT3
 *
 */
#define GA_COMSTAT3 ((ga_reg) GAREG_COMSTAT3)

/**
 * @copydoc GAREG_COMSTAT4
 *
 */
#define GA_COMSTAT4 ((ga_reg) GAREG_COMSTAT4)

/**
 * @copydoc GAREG_COMSTAT5
 *
 */
#define GA_COMSTAT5 ((ga_reg) GAREG_COMSTAT5)

/**
 * @copydoc GAREG_COMSTAT6
 *
 */
#define GA_COMSTAT6 ((ga_reg) GAREG_COMSTAT6)

/**
 * @copydoc GAREG_COMSTAT7
 *
 */
#define GA_COMSTAT7 ((ga_reg) GAREG_COMSTAT7)

/**
 * @fn wait_2m
 * Wait for Main CPU access to 2M Word RAM
 */
static inline void wait_2m()
{
	asm volatile(
		"\
1:btst     #%c[ga_ret_bit], %c[gareg_memmmode] \n\
  beq      1b \n\
		"
		:
		: [ga_ret_bit] "i"(GA_RET_BIT), [gareg_memmmode] "i"(GAREG_MEMMODE + 1));
}

/**
 * @fn grant_2m
 * Grant 2M Word RAM access to the Sub CPU and wait for confirmation
 */
static inline void grant_2m()
{
	asm volatile(
		"\
1:bset     #%c[ga_dmna_bit], %c[gareg_memmmode] \n\
  btst     #%c[ga_dmna_bit], %c[gareg_memmmode] \n\
  beq      1b \n\
		"
		:
		: [ga_dmna_bit] "i"(GA_DMNA_BIT), [gareg_memmmode] "i"(GAREG_MEMMODE + 1));
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
		: [comcmd0] "i"(GAREG_COMCMD0));
}

/**
 * @fn clear_comm_regs
 * Clears the Main comm registers (COMCMD) and flags
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
		: [memmode] "i"(GAREG_MEMMODE), [reset] "i"(GAREG_RESET + 1)
		:);
}

#endif

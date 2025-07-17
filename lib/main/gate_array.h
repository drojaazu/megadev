/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_array.h
 * @brief C wrappers for Main CPU side Gate Array registers
 */

#ifndef MEGADEV__MAIN_GATE_ARRAY_H
#define MEGADEV__MAIN_GATE_ARRAY_H

#include "main/gate_array.def.h"

/**
 * @sa _GAREG_COMFLAGS
 */
#define GA_COMFLAGS_MAIN ((volatile u8 *) _GAREG_COMFLAGS)

#define GA_COMFLAGS_SUB ((volatile const u8 *) _GAREG_COMFLAGS + 1)

/**
 * @copydoc _GAREG_RESET
 *
 */
#define GA_RESET ((u16 volatile *) _GAREG_RESET)

#define GA_RESET_SRES (1 << 0)
#define GA_RESET_SBRQ (1 << 1)

/**
 * @copydoc _GAREG_MEMMODE
 *
 */
#define GA_MEMMODE ((u16 volatile *) _GAREG_MEMMODE)

/**
 * @copydoc _GAREG_CDCMODE
 *
 */
#define GA_CDCMODE ((u16 volatile *) _GAREG_CDCMODE)

/**
 * @copydoc _GAREG_HINTVECT
 *
 */
#define GA_HINTVECT ((u16 volatile *) _GAREG_HINTVECT)

/**
 * @copydoc _GAREG_CDCHOSTDATA
 *
 */
#define GA_CDCHOSTDATA ((u16 volatile *) _GAREG_CDCHOSTDATA)

/**
 * @copydoc _GAREG_STOPWATCH
 *
 */
#define GA_STOPWATCH ((u16 volatile *) _GAREG_STOPWATCH)

/**
 * @copydoc _GAREG_COMFLAGS
 *
 */
#define GA_COMFLAGS ((u16 volatile *) _GAREG_COMFLAGS)

/**
 * @copydoc _GAREG_GAREG_COMCMD0
 *
 */
#define GA_COMCMD0 ((u16 volatile *) _GAREG_COMCMD0)

/**
 * @copydoc _GAREG_COMCMD1
 *
 */
#define GA_COMCMD1 ((u16 volatile *) _GAREG_COMCMD1)

/**
 * @copydoc _GAREG_COMCMD2
 *
 */
#define GA_COMCMD2 ((u16 volatile *) _GAREG_COMCMD2)

/**
 * @copydoc _GAREG_COMCMD3
 *
 */
#define GA_COMCMD3 ((u16 volatile *) _GAREG_COMCMD3)

/**
 * @copydoc _GAREG_COMCMD4
 *
 */
#define GA_COMCMD4 ((u16 volatile *) _GAREG_COMCMD4)

/**
 * @copydoc _GAREG_COMCMD5
 *
 */
#define GA_COMCMD5 ((u16 volatile *) _GAREG_COMCMD5)

/**
 * @copydoc _GAREG_COMCMD6
 *
 */
#define GA_COMCMD6 ((u16 volatile *) _GAREG_COMCMD6)

/**
 * @copydoc _GAREG_COMCMD7
 *
 */
#define GA_COMCMD7 ((u16 volatile *) _GAREG_COMCMD7)

/**
 * @copydoc _GAREG_COMSTAT0
 *
 */
#define GA_COMSTAT0 ((u16 volatile *) _GAREG_COMSTAT0)

/**
 * @copydoc _GAREG_COMSTAT1
 *
 */
#define GA_COMSTAT1 ((u16 volatile *) _GAREG_COMSTAT1)

/**
 * @copydoc _GAREG_COMSTAT2
 *
 */
#define GA_COMSTAT2 ((u16 volatile *) _GAREG_COMSTAT2)

/**
 * @copydoc _GAREG_COMSTAT3
 *
 */
#define GA_COMSTAT3 ((u16 volatile *) _GAREG_COMSTAT3)

/**
 * @copydoc _GAREG_COMSTAT4
 *
 */
#define GA_COMSTAT4 ((u16 volatile *) _GAREG_COMSTAT4)

/**
 * @copydoc _GAREG_COMSTAT5
 *
 */
#define GA_COMSTAT5 ((u16 volatile *) _GAREG_COMSTAT5)

/**
 * @copydoc _GAREG_COMSTAT6
 *
 */
#define GA_COMSTAT6 ((u16 volatile *) _GAREG_COMSTAT6)

/**
 * @copydoc _GAREG_COMSTAT7
 *
 */
#define GA_COMSTAT7 ((u16 volatile *) _GAREG_COMSTAT7)

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
		:
			[ga_ret_bit] "i"(GA_RET_BIT),
			[gareg_memmmode] "i"(_GAREG_MEMMODE + 1)
		);
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
		:
			[ga_dmna_bit] "i"(GA_DMNA_BIT),
			[gareg_memmmode] "i"(_GAREG_MEMMODE + 1));
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
  lea (%c[gareg_comcmd0]), %[scratch_a] \n\
  moveq    #0, %[scratch_d] \n\
  move.b   %[scratch_d], -2(%[scratch_a]) \n\
  move.l   %[scratch_d], (%[scratch_a])+ \n\
  move.l   %[scratch_d], (%[scratch_a])+ \n\
  move.l   %[scratch_d], (%[scratch_a])+ \n\
  move.l   %[scratch_d], (%[scratch_a])+ \n\
		"
		:
			[scratch_d] "=&d"(scratch_d),
			[scratch_a] "=&a"(scratch_a)
		:
			[gareg_comcmd0] "i"(_GAREG_COMCMD0)
		);
}

/**
 * @fn clear_comm_regs
 * Clears the Main comm registers (COMCMD) and flags
 */
static inline void reset_ga()
{
	asm volatile(
		"\
  move.w   #0xFF00, %c[gareg_memmode] \n\
  move.b   #0x3, %c[gareg_reset] \n\
  nop \n\
  move.b   #0x3, %c[gareg_reset] \n\
  move.b   #0x2, %c[gareg_reset] \n\
  move.b   #0x0, %c[gareg_reset] \n\
		"
		:
		: 
			[gareg_memmode] "i"(_GAREG_MEMMODE),
			[gareg_reset] "i"(_GAREG_RESET + 1)
		: );
}

#endif

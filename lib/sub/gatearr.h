/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gatearr.h
 * @brief C wrappers for Sub CPU side Gate Array registers
 *
 */

#ifndef MEGADEV__SUB_GATEARR_H
#define MEGADEV__SUB_GATEARR_H

#include "gatearr_def.h"

/**
 * @def GA_RESET
 * @brief GA Reg 0 - Sub CPU Reset/ Bus Request
 * @ingroup sub_ga_regs
 * @sa _GA_RESET
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||LEDG|LEDR|Ver3|Ver2|Ver1|Ver0| |||RES0|
 *
 * @param RES0 Sub CPU reset / Version / LED Control
 * @details
 * W: 0: Reset / 1: No effect \n R: 0: Reset in progress / 1: Reset
 * possible
 * @param LEDR Red LED control
 * @details
 * RW: 0: Off / 1: On
 * @param LEDG Green LED control
 * @details
 * RW: 0: Off / 1: On
 * @param Ver ROM Version
 * @details
 * R: ROM Version
 */
#define GA_RESET ((volatile u16 *) _GA_RESET)

/**
 * @def GA_MEMMODE
 * @brief GA Reg 1 - Word RAM Memory Mode / RAM Write Protect / Priority Mode
 * @ingroup sub_ga_regs
 * @sa _GA_MEMMODE
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |WP7|WP6|WP5|WP4|WP3|WP2|WP1|WP0| |||PM1|PM0|MODE|DMNA|RET|
 *
 * @param WP Write protect Sub CPU RAM
 * @param PM Priority Mode
 * @param MODE Word RAM layout
 * @details 0: 2M, 1: 1M
 * @param DMNA Main CPU will not access Word RAM
 * @param RET In 2M mode: Give Word RAM control to Main CPU;
 * In 1M mode: Change 1M block ownership
 *
 */
#define GA_MEMMODE ((volatile u16 *) _GA_MEMMODE)

#define GA_COMFLAGS_MAIN ((volatile const u8 *) _GA_COMFLAGS)
#define GA_COMFLAGS_SUB ((volatile u8 *) _GA_COMFLAGS + 1)

#define GA_COMCMD0 ((u16 volatile const *) _GA_COMCMD0)
#define GA_COMCMD1 ((u16 volatile const *) _GA_COMCMD1)
#define GA_COMCMD2 ((u16 volatile const *) _GA_COMCMD2)
#define GA_COMCMD3 ((u16 volatile const *) _GA_COMCMD3)
#define GA_COMCMD4 ((u16 volatile const *) _GA_COMCMD4)
#define GA_COMCMD5 ((u16 volatile const *) _GA_COMCMD5)
#define GA_COMCMD6 ((u16 volatile const *) _GA_COMCMD6)
#define GA_COMCMD7 ((u16 volatile const *) _GA_COMCMD7)

#define GA_COMSTAT0 ((u16 volatile *) _GA_COMSTAT0)
#define GA_COMSTAT1 ((u16 volatile *) _GA_COMSTAT1)
#define GA_COMSTAT2 ((u16 volatile *) _GA_COMSTAT2)
#define GA_COMSTAT3 ((u16 volatile *) _GA_COMSTAT3)
#define GA_COMSTAT4 ((u16 volatile *) _GA_COMSTAT4)
#define GA_COMSTAT5 ((u16 volatile *) _GA_COMSTAT5)
#define GA_COMSTAT6 ((u16 volatile *) _GA_COMSTAT6)
#define GA_COMSTAT7 ((u16 volatile *) _GA_COMSTAT7)

#define GA_INT3TIMER ((u16 volatile *) _GA_INT3TIMER)

#define GA_INTMASK ((u16 volatile *) _GA_INTMASK)

#define GA_DMAADDR ((u16 volatile *) _GA_DMAADDR)

/**
 * @brief Wait for Sub CPU access to 2M Word RAM
 */
static inline void wait_2m()
{
	asm(
		"\
		1:btst %0, %p1 \n\
			beq 1b \n\
		"
		:
		: "i"(BIT_GA_DMNA), "i"(_GA_MEMMODE + 1));
}

/**
 * @brief Grant 2M Word RAM access to the Main CPU
 */
static inline void grant_2m()
{
	asm(
		"\
		1:bset %0, %p1 \n\
			btst %0, %p1 \n\
			beq 1b \n\
		"
		:
		: "i"(BIT_GA_RET), "i"(_GA_MEMMODE + 1));
}

/**
 * @brief Set Word RAM layout to 1M/1M
 */
static inline void set_1m()
{
	asm(
		"\
		1:bset %0, %p1 \n\
			btst %0, %p1 \n\
			beq 1b \n\
		"
		:
		: "i"(BIT_GA_MODE), "i"(_GA_MEMMODE + 1));
}

/**
 * @fn set_2m
 * @brief Set Word RAM layout to 2M
 */
static inline void set_2m()
{
	asm(
		"\
		1:bclr %0, %p1 \n\
			btst %0, %p1 \n\
			bne 1b \n\
		"
		:
		: "i"(BIT_GA_MODE), "i"(_GA_MEMMODE + 1));
}

/**
 * @fn clear_comm_regs
 * Clears the Main comm registers (COMSTAT) and flags
 */
static inline void clear_comm_regs()
{
	asm(
		"\
		lea %p0, a0 \n\
		moveq #0, d0 \n\
		move.b d0, -0x11(a0) /* lower byte of comm flags */ \n\
		move.l d0, (a0)+ \n\
		move.l d0, (a0)+ \n\
		move.l d0, (a0)+ \n\
		move.l d0, (a0)+ \n\
		"
		:
		: "i"(_GA_COMSTAT0)
		: "d0", "a0");
}

#endif
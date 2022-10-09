/**
 * @file gatearr.h
 * @brief C wrappers for Sub CPU side Gate Array registers
 */

#ifndef MEGADEV__MAIN_GATEARR_H
#define MEGADEV__MAIN_GATEARR_H

#include "gatearr_def.h"

/**
 * @sa _GA_COMFLAGS
 */
#define GA_COMFLAGS_MAIN ((volatile u8 *) _GA_COMFLAGS)

#define GA_COMFLAGS_SUB ((volatile const u8 *) _GA_COMFLAGS + 1)

#define GA_COMCMD0 ((u16 volatile *) _GA_COMCMD0)
#define GA_COMCMD1 ((u16 volatile *) _GA_COMCMD1)
#define GA_COMCMD2 ((u16 volatile *) _GA_COMCMD2)
#define GA_COMCMD3 ((u16 volatile *) _GA_COMCMD3)
#define GA_COMCMD4 ((u16 volatile *) _GA_COMCMD4)
#define GA_COMCMD5 ((u16 volatile *) _GA_COMCMD5)
#define GA_COMCMD6 ((u16 volatile *) _GA_COMCMD6)
#define GA_COMCMD7 ((u16 volatile *) _GA_COMCMD7)

#define GA_COMSTAT0 ((u16 volatile const *) _GA_COMSTAT0)
#define GA_COMSTAT1 ((u16 volatile const *) _GA_COMSTAT1)
#define GA_COMSTAT2 ((u16 volatile const *) _GA_COMSTAT2)
#define GA_COMSTAT3 ((u16 volatile const *) _GA_COMSTAT3)
#define GA_COMSTAT4 ((u16 volatile const *) _GA_COMSTAT4)
#define GA_COMSTAT5 ((u16 volatile const *) _GA_COMSTAT5)
#define GA_COMSTAT6 ((u16 volatile const *) _GA_COMSTAT6)
#define GA_COMSTAT7 ((u16 volatile const *) _GA_COMSTAT7)

/**
 * @note We do not dereference the pointers in the defines
 * as the bit sets need to be done on each byte.
 * This way the subscript [] operator can be used.
 *
 * TODO This is mentioned somewhere in the manual, where?
 *
 */

/**
 * @def GA_RESET
 * @brief Sub CPU Reset / Bus Request
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |IEN2| ||||||IFL2| ||||||SBRQ|SRES|
 *
 * @param SRES Sub CPU reset
 * @details W: 0: Reset / 1: Run \n R: 0: Reset in progress / 1: Running
 * @param SBRQ Sub CPU bus access request
 * @details W: 0: Cancel / 1: Request \n R: 0: Sub CPU running / 1: Acknowledge
 * @param IFL2 Send INT2 to Sub CPU
 * @details W: 0: Not used / 1: Send INT 2 to Sub CPU \n R: 0: INT 2 in progress
 * / 1: INT 2 not occurred yet
 * @param IEN2 Mask state of INT2 on Sub CPU
 * @details 0: Masked / 1: Enabled
 *
 * @note Only BTST bitwise operation allowed on this register
 */
#define GA_RESET ((u16 volatile *) _GA_RESET)

/**
 * @def GA_MEMMODE
 * @brief Word RAM Memory Mode / Sub CPU RAM Write Protect
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |WP7|WP6|WP5|WP4|WP3|WP2|WP1|WP0|BK1|BK0| |||MODE|DMNA|RET|
 *
 * @param WP Write protect Sub CPU RAM
 * @param BK PRG-RAM bank select
 * @param MODE Word RAM layout
 * @param DMNA Main CPU will not access Word RAM
 * @param RET Give Word RAM control to Main CPU
 *
 * WP Write protect an area of Sub CPU RAM from 0 to 0x1FE00 in increments
 * of 0x200
 * BK|PRG-RAM bank select for Main CPU access
 * (4M PRG-RAM divided into 1M banks)
 * MODE|Word RAM Mode
 *     |  Read Only: 0: 2M / 1: 1M/1M
 * DMNA|"Declaration of Main CPU No Access" on Word RAM
 *     |  Effect depends on MODE bit:
 *
 *
 *       MODE = 0 (2M):
 *        Write: 0: N/A / 1: Return Word RAM to Sub CPU
 *        Read: 0: Return Word RAM to Sub CPU In Progress
 *              1: Return Word RAM to Sub CPU Completed
 *       MODE = 1 (1M/1M):
 *        Write: 0: N/A / 1: Request 1M bank swap
 *        Read: 0: Bank swap In Progress
 *              1: Bank swap Completed
 * RET|Return Word RAM to Main CPU
 *      Effect depends on MODE bit:
 *			MODE = 0 (2M):
 *			 Read Only: 0: Return Word RAM to Main CPU In Progress
 *			            1: Return Word RAM to Main CPU Completed
 *       MODE = 1 (1M/1M):
 *        Read Only: 0: Word RAM Bank 0 attached to Main CPU, Bank 1 to Sub CPU
 *                   1: Word RAM Bank 0 attached to Sub CPU, Bank 1 to Main CPU
 */
#define GA_MEMMODE ((u16 volatile *) _GA_MEMMODE)

/**
 * @note The system jump table gives you six bytes to work with per entry,
 * which are usually implemented as a JSR (2 bytes) and a vector (4 bytes).
 * We only provide C wrappers for the last four bytes as a pointer, since
 * there is really no reason to change the JSR opcode. If such a change is
 * necessary for some reason, it should be done in asm.
 */

// L6 interrupt = VINT
#define MLEVEL6_VECTOR ((void volatile *(*) ) (_MLEVEL6 + 2))
// L4 interrupt = HINT
#define MLEVEL4_VECTOR ((void volatile *(*) ) (_MLEVEL4 + 2))
// L2 interrupt = Ext. port
#define MLEVEL2_VECTOR ((void volatile *(*) ) (_MLEVEL2 + 2))

/**
 * @fn wait_2m
 * Wait for Main CPU access to 2M Word RAM
 */
static inline void wait_2m()
{
	asm(
		"\
    1:btst %0, %p1 \n\
      beq 1b \n\
    "
		:
		: "i"(GA_RET_BIT), "i"(_GA_MEMMODE + 1));
}

/**
 * @fn grant_2m
 * Grant 2M Word RAM access to the Sub CPU and wait for confirmation
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
		: "i"(GA_DMNA_BIT), "i"(_GA_MEMMODE + 1));
}

/**
 * @fn clear_comm_regs
 * Clears the Main comm registers (COMCMD) and flags
 */
static inline void clear_comm_regs()
{
	asm(
		"\
    lea %p0, a0 \n\
    moveq #0, d0 \n\
    move.b d0, -2(a0) /* upper byte of comm flags */ \n\
    move.l d0, (a0)+ \n\
    move.l d0, (a0)+ \n\
    move.l d0, (a0)+ \n\
    move.l d0, (a0)+ \n\
    "
		:
		: "i"(_GA_COMCMD0)
		: "d0", "a0");
}

#endif

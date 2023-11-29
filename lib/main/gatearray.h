/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gatearray.h
 * @brief C wrappers for Main CPU side Gate Array registers
 */

#ifndef MEGADEV__MAIN_GATEARRAY_H
#define MEGADEV__MAIN_GATEARRAY_H

#include "main/gatearray.def.h"

/**
 * @sa _GA_COMFLAGS
 */
#define GA_COMFLAGS_MAIN ((volatile u8 *) _GA_COMFLAGS)

#define GA_COMFLAGS_SUB ((volatile const u8 *) _GA_COMFLAGS + 1)

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

#define GA_RESET_SRES (1 << 0)
#define GA_RESET_SBRQ (1 << 1)

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
 * @def _GA_CDCMODE
 * @brief CDC Mode
 * @sa _GA_CDCMODE
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |EDT|DSR| |||DD2|DD1|DD0| ||||||||
 *
 * @param EDT End of data transfer
 * @details R: All data from the CDC has been transferred
 * @param DSR Data set ready
 * @details R: Data from the CDC is present in the CDC Host Data register
 * @param DD Device destination
 * @details RW: Specifies the destination for CDC data transfer, as defined
 * below: |DD2|DD1|DD0|Destination|
 *   |:|:|:|:|
 *   |0|1|0|Main CPU|
 *   |0|1|1|Sub CPU|
 *   |1|0|0|PCM DMA|
 *   |1|1|1|In 2M Mode: Word RAM \n In 1M Mode: Sub CPU controlled Word RAM|
 *
 *   All other values for DD are invalid.
 */
#define GA_CDCMODE ((u16 volatile *) _GA_CDCMODE)

/**
 * @def GA_HINTVECT
 * @brief HINT Vector
 * @sa _GA_HINTVECT
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HIBF|HIBE|HIBD|HIBC|HIBB|HIBA|HIB9|HIB8|HIB7|HIB6|HIB5|HIB4|HIB3|HIB2|HIB1|HIB0|
 *
 * HIB: Specifies the lower word of the HINT (Level 4) interrupt vector
 *      The upper word is specied at the standard location (0x70), the value
 *      of which is 0x00FF by the Boot ROM.
 *
 * @warning Word access only!
 */
#define GA_HINTVECT ((u16 volatile *) _GA_HINTVECT)

/**
 * @def GA_CDCHOSTDATA
 * @brief CDC host data
 * @sa _GA_CDCHOSTDATA
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HD15|HD14|HD13|HD12|HD11|HD10|HD09|HD08|HD07|HD06|HD05|HD04|HD03|HD02|HD01|HD00|
 *
 * HD: CDC read data
 *
 * @warning Word access only!
 */
#define GA_CDCHOSTDATA ((u16 volatile *) _GA_CDCHOSTDATA)

/**
 * @def GA_STOPWATCH
 * @brief Stop watch
 * @sa _GA_STOPWATCH
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |||| |TD11|TD10|TD09|TD08|TD07|TD06|TD05|TD04|TD03|TD02|TD01|TD00|
 *
 * TD: Timer data
 *
 * @details One count is timed to 30.72µs
 *
 * @warning Word access only!
 */
#define GA_STOPWATCH ((u16 volatile *) _GA_STOPWATCH)

/**
 * @def GA_COMFLAGS
 * @brief Main/Sub CPU communication flags
 * @sa _GA_COMFLAGS
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |CFM7|CFM6|CFM5|CFM4|CFM3|CFM2|CFM1|CFM0|CFS7|CFS6|CFS5|CFS4|CFS3|CFS2|CFS1|CFS0|
 *
 * CFM: Comm flags for Main CPU
 * CFS: Comm flags for Sub CPU
 */
#define GA_COMFLAGS ((u16 volatile *) _GA_COMFLAGS)

/**
 * @def GA_COMCMD0
 * @brief Communication command (Main -> Sub)
 * @sa _GA_COMCMD0
 *
 * @details RW: 16 bit data
 */
#define GA_COMCMD0 ((u16 volatile *) _GA_COMCMD0)

/**
 * @def GA_COMCMD1
 * @brief Communication command (Main -> Sub)
 * @sa _GA_COMCMD1
 *
 * @details RW: 16 bit data
 */
#define GA_COMCMD1 ((u16 volatile *) _GA_COMCMD1)

/**
 * @def GA_COMCMD2
 * @brief Communication command (Main -> Sub)
 * @sa _GA_COMCMD2
 *
 * @details RW: 16 bit data
 */
#define GA_COMCMD2 ((u16 volatile *) _GA_COMCMD2)

/**
 * @def GA_COMCMD3
 * @brief Communication command (Main -> Sub)
 * @sa _GA_COMCMD3
 *
 * @details RW: 16 bit data
 */
#define GA_COMCMD3 ((u16 volatile *) _GA_COMCMD3)

/**
 * @def GA_COMCMD4
 * @brief Communication command (Main -> Sub)
 * @sa _GA_COMCMD4
 *
 * @details RW: 16 bit data
 */
#define GA_COMCMD4 ((u16 volatile *) _GA_COMCMD4)

/**
 * @def GA_COMCMD5
 * @brief Communication command (Main -> Sub)
 * @sa _GA_COMCMD5
 *
 * @details RW: 16 bit data
 */
#define GA_COMCMD5 ((u16 volatile *) _GA_COMCMD5)

/**
 * @def GA_COMCMD6
 * @brief Communication command (Main -> Sub)
 * @sa _GA_COMCMD6
 *
 * @details RW: 16 bit data
 */
#define GA_COMCMD6 ((u16 volatile *) _GA_COMCMD6)

/**
 * @def GA_COMCMD7
 * @brief Communication command (Main -> Sub)
 * @sa _GA_COMCMD7
 *
 * @details RW: 16 bit data
 */
#define GA_COMCMD7 ((u16 volatile *) _GA_COMCMD7)

/**
 * @def GA_COMSTAT0
 * @brief Communication status (Sub -> Main)
 * @sa _GA_COMSTAT0
 * @ingroup main_gatearr
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT0 ((u16 volatile *) _GA_COMSTAT0)

/**
 * @def GA_COMSTAT1
 * @brief Communication status (Sub -> Main)
 * @sa _GA_COMSTAT1
 * @ingroup main_gatearr
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT1 ((u16 volatile *) _GA_COMSTAT1)

/**
 * @def GA_COMSTAT2
 * @brief Communication status (Sub -> Main)
 * @sa _GA_COMSTAT2
 * @ingroup main_gatearr
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT2 ((u16 volatile *) _GA_COMSTAT2)

/**
 * @def GA_COMSTAT3
 * @brief Communication status (Sub -> Main)
 * @sa _GA_COMSTAT3
 * @ingroup main_gatearr
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT3 ((u16 volatile *) _GA_COMSTAT3)

/**
 * @def GA_COMSTAT4
 * @brief Communication status (Sub -> Main)
 * @sa _GA_COMSTAT4
 * @ingroup main_gatearr
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT4 ((u16 volatile *) _GA_COMSTAT4)

/**
 * @def GA_COMSTAT5
 * @brief Communication status (Sub -> Main)
 * @sa _GA_COMSTAT5
 * @ingroup main_gatearr
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT5 ((u16 volatile *) _GA_COMSTAT5)

/**
 * @def GA_COMSTAT6
 * @brief Communication status (Sub -> Main)
 * @sa _GA_COMSTAT6
 * @ingroup main_gatearr
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT6 ((u16 volatile *) _GA_COMSTAT6)

/**
 * @def GA_COMSTAT7
 * @brief Communication status (Sub -> Main)
 * @sa _GA_COMSTAT7
 * @ingroup main_gatearr
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT7 ((u16 volatile *) _GA_COMSTAT7)

/**
 * @fn wait_2m
 * Wait for Main CPU access to 2M Word RAM
 */
static inline void wait_2m()
{
	__asm__(
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
	__asm__(
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
	__asm__(
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

/**
 * \file main/main_def.h
 * \brief Hardware memory map, Gate Array (GA) register, and entry vector
 * definitions for the Main CPU side
 */

#ifndef MEGADEV__CD_MAIN_DEF_H
#define MEGADEV__CD_MAIN_DEF_H

#define SYSTEMUSE_BASE 0xfffd00

/**
 * Work RAM
 */
#define _WORK_RAM 0xff0000

/**
 * 2M/1M Mapping
 */
#define _MAIN_2M_BASE 0x200000 /* word RAM base in 2M mode */
#define _MAIN_1M_BASE 0x200000 /* word RAM base in 1M mode */
#define _MAIN_1M_VRAM 0x220000 /* VDP tiles for 1M mode */

/**
 * Access to Sub side PRG-RAM in 1M blocks
 */
#define _MAIN_PRGRAM 0x20000

/**
 * Initial Program (IP) enntry
 */
#define _IP_ENTRY 0xff0000

/**
 * System Jump Table
 */
#define _RESET 0xfffd00
#define _MLEVEL6 0xfffd06 /* VBLANK interrupt */
#define _MLEVEL4 0xfffd0c /* HBLANK interrupt */
#define _MLEVEL2 0xfffd12 /* External port interrupt */
#define _MTRAP00 0xfffd18
#define _MTRAP01 0xfffd1e
#define _MTRAP02 0xfffd24
#define _MTRAP03 0xfffd2a
#define _MTRAP04 0xfffd30
#define _MTRAP05 0xfffd36
#define _MTRAP06 0xfffd3c
#define _MTRAP07 0xfffd42
#define _MTRAP08 0xfffd48
#define _MTRAP09 0xfffd4e
#define _MTRAP10 0xfffd54
#define _MTRAP11 0xfffd5a
#define _MTRAP12 0xfffd60
#define _MTRAP13 0xfffd66
#define _MTRAP14 0xfffd6c
#define _MTRAP15 0xfffd72
#define _MONKERR 0xfffd78 // CHK Instruction
// the duplicate addresses on the next two entries is intentional
#define _MADRERR 0xfffd7e
#define _MCODERR 0xfffd7e
#define _MDIVERR 0xfffd84 // Zero Divide
#define _MTRPERR 0xfffd8a // TRAPV Instruction
#define _MNOCOD0 0xfffd90 // Line 1010
#define _MNOCOD1 0xfffd96 // Line 1111
#define _MSPVERR 0xfffd9c // Priv. Violation
#define _MTRACE 0xfffda2  // Trace
#define _VINT_EX 0xfffda8
#define _MBURAM 0xfffdae

/**
 * \def _GA_RESET
 * \brief Sub CPU Reset / Bus Request
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |IEN2| ||||||IFL2| ||||||SBRQ|SRES|
 *
 * \param SRES Sub CPU reset
 * \details W: 0: Reset / 1: Run \n R: 0: Reset in progress / 1: Running
 * \param SBRQ Sub CPU bus access request
 * \details W: 0: Cancel / 1: Request \n R: 0: Sub CPU running / 1: Acknowledge
 * \param IFL2 Send INT2 to Sub CPU
 * \details W: 0: Not used / 1: Send INT 2 to Sub CPU \n R: 0: INT 2 in progress
 * / 1: INT 2 not occurred yet
 * \param IEN2 Mask state of INT2 on Sub CPU
 * \details 0: Masked / 1: Enabled
 *
 * \note Only BTST bitwise operation allowed on this register
 */
#define _GA_RESET 0xA12000

//_GA_RESET
#define GA_RESET_IFL2_BIT 0

// _GA_RESET+1
#define GA_RESET_SRES_BIT 0
#define GA_RESET_SBRQ_BIT 1

#define GA_RESET_IFL2_MSK 1 << GA_RESET_IFL2_BIT // Trigger INT2 on Sub CPU
#define GA_RESET_SRES_MSK 1 << GA_RESET_SRES_BIT // Sub CPU reset
#define GA_RESET_SBRQ_MSK 1 << GA_RESET_SBRQ_BIT // Sub CPU bus request

/**
 * \def _GA_MEMMODE
 * \brief Word RAM Memory Mode / Sub CPU RAM Write Protect
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |WP7|WP6|WP5|WP4|WP3|WP2|WP1|WP0|BK1|BK0| |||MODE|DMNA|RET|
 *
 * \param WP Write protect Sub CPU RAM
 * \param BK PRG-RAM bank select
 * \param MODE Word RAM layout
 * \param DMNA Main CPU will not access Word RAM
 * \param RET Give Word RAM control to Main CPU
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
#define _GA_MEMMODE 0xA12002

#define MEMMODE_RET_BIT 0
#define MEMMODE_DMNA_BIT 1
#define MEMMODE_MODE_BIT 2
#define MEMMODE_BK0_BIT 6
#define MEMMODE_BK1_BIT 7
#define MEMMODE_WP0_BIT 8
#define MEMMODE_WP1_BIT 9
#define MEMMODE_WP2_BIT 10
#define MEMMODE_WP3_BIT 11
#define MEMMODE_WP4_BIT 12
#define MEMMODE_WP5_BIT 13
#define MEMMODE_WP6_BIT 14
#define MEMMODE_WP7_BIT 15

#define MEMMODE_RET_MSK 1 << MEMMODE_RET_BIT
#define MEMMODE_DMNA_MSK 1 << MEMMODE_DMNA_BIT
#define MEMMODE_MODE_MSK 1 << MEMMODE_MODE_BIT
#define MEMMODE_BK0_MSK 1 << MEMMODE_BK0_BIT
#define MEMMODE_BK1_MSK 1 << MEMMODE_BK1_BIT
#define MEMMODE_WP0_MSK 1 << MEMMODE_WP0_BIT
#define MEMMODE_WP1_MSK 1 << MEMMODE_WP1_BIT
#define MEMMODE_WP2_MSK 1 << MEMMODE_WP2_BIT
#define MEMMODE_WP3_MSK 1 << MEMMODE_WP3_BIT
#define MEMMODE_WP4_MSK 1 << MEMMODE_WP4_BIT
#define MEMMODE_WP5_MSK 1 << MEMMODE_WP5_BIT
#define MEMMODE_WP6_MSK 1 << MEMMODE_WP6_BIT
#define MEMMODE_WP7_MSK 1 << MEMMODE_WP7_BIT

/**
 * \def _GA_CDCMODE
 * \brief CDC Mode
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |EDT|DSR| |||DD2|DD1|DD0| ||||||||
 *
 * \param EDT End of data transfer
 * \details R: All data from the CDC has been transferred
 * \param DSR Data set ready
 * \details R: Data from the CDC is present in the CDC Host Data register
 * \param DD Device destination
 * \details RW: Specifies the destination for CDC data transfer, as defined
 * below: |DD2|DD1|DD0|Destination|
 *   |:|:|:|:|
 *   |0|1|0|Main CPU|
 *   |0|1|1|Sub CPU|
 *   |1|0|0|PCM DMA|
 *   |1|1|1|In 2M Mode: Word RAM \n In 1M Mode: Sub CPU controlled Word RAM|
 *
 *   All other values for DD are invalid.
 */
#define _GA_CDCMODE 0xA12004

/**
 * \def _GA_HINTVECT
 * \brief HINT Vector
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HIBF|HIBE|HIBD|HIBC|HIBB|HIBA|HIB9|HIB8|HIB7|HIB6|HIB5|HIB4|HIB3|HIB2|HIB1|HIB0|
 *
 * HIB: Specifies the lower word of the HINT (Level 4) interrupt vector
 *      The upper word is specied at the standard location (0x70), the value
 *      of which is 0x00FF by the Boot ROM.
 *
 * \warning Word access only!
 */
#define _GA_HINTVECT 0xA12006

/**
 * \def _GA_CDCHOSTDATA
 * \brief CDC host data
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HD15|HD14|HD13|HD12|HD11|HD10|HD09|HD08|HD07|HD06|HD05|HD04|HD03|HD02|HD01|HD00|
 *
 * HD: CDC read data
 *
 * \warning Word access only!
 */
#define _GA_CDCHOSTDATA 0xA12008 /* 16-bit CDC host data */

/**
 * \def _GA_STOPWATCH
 * \brief Stop watch
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |||| |TD11|TD10|TD09|TD08|TD07|TD06|TD05|TD04|TD03|TD02|TD01|TD00|
 *
 * TD: Timer data
 *
 * \details One count is timed to 30.72µs
 *
 * \warning Word access only!
 */
#define _GA_STOPWATCH 0xA1200C

/**
 * \def _GA_COMFLAGS
 * \brief Main/Sub CPU communication flags
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |CFM7|CFM6|CFM5|CFM4|CFM3|CFM2|CFM1|CFM0|CFS7|CFS6|CFS5|CFS4|CFS3|CFS2|CFS1|CFS0|
 *
 * CFM: Comm flags for Main CPU
 * CFS: Comm flags for Sub CPU
 */
#define _GA_COMFLAGS 0xA1200E

/**
 * \def _GA_COMCMD0
 * \brief Communication command (Main -> Sub)
 * \details RW: 16 bit data
 */
#define _GA_COMCMD0 0xA12010

/**
 * \def _GA_COMCMD1
 * \brief Communication command (Main -> Sub)
 * \details RW: 16 bit data
 */
#define _GA_COMCMD1 0xA12012

/**
 * \def _GA_COMCMD2
 * \brief Communication command (Main -> Sub)
 * \details RW: 16 bit data
 */
#define _GA_COMCMD2 0xA12014

/**
 * \def _GA_COMCMD3
 * \brief Communication command (Main -> Sub)
 * \details RW: 16 bit data
 */
#define _GA_COMCMD3 0xA12016

/**
 * \def _GA_COMCMD4
 * \brief Communication command (Main -> Sub)
 * \details RW: 16 bit data
 */
#define _GA_COMCMD4 0xA12018

/**
 * \def _GA_COMCMD5
 * \brief Communication command (Main -> Sub)
 */
#define _GA_COMCMD5 0xA1201A

/**
 * \def _GA_COMCMD6
 * \brief Communication command (Main -> Sub)
 * \details RW: 16 bit data
 */
#define _GA_COMCMD6 0xA1201C

/**
 * \def _GA_COMCMD7
 * \brief Communication command (Main -> Sub)
 * \details RW: 16 bit data
 */
#define _GA_COMCMD7 0xA1201E

/**
 * \def _GA_COMSTAT0
 * \brief Communication status (Sub -> Main)
 * \details R: 16 bit data
 */
#define _GA_COMSTAT0 0xA12020

/**
 * \def _GA_COMSTAT1
 * \brief Communication status (Sub -> Main)
 * \details R: 16 bit data
 */
#define _GA_COMSTAT1 0xA12022

/**
 * \def _GA_COMSTAT2
 * \brief Communication status (Sub -> Main)
 * \details R: 16 bit data
 */
#define _GA_COMSTAT2 0xA12024

/**
 * \def _GA_COMSTAT3
 * \brief Communication status (Sub -> Main)
 * \details R: 16 bit data
 */
#define _GA_COMSTAT3 0xA12026

/**
 * \def _GA_COMSTAT4
 * \brief Communication status (Sub -> Main)
 * \details R: 16 bit data
 */
#define _GA_COMSTAT4 0xA12028

/**
 * \def _GA_COMSTAT5
 * \brief Communication status (Sub -> Main)
 * \details R: 16 bit data
 */
#define _GA_COMSTAT5 0xA1202A

/**
 * \def _GA_COMSTAT6
 * \brief Communication status (Sub -> Main)
 * \details R: 16 bit data
 */
#define _GA_COMSTAT6 0xA1202C

/**
 * \def _GA_COMSTAT7
 * \brief Communication status (Sub -> Main)
 * \details R: 16 bit data
 */
#define _GA_COMSTAT7 0xA1202E

#endif

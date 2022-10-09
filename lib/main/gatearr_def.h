/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gatearr_def.h
 * @brief Main CPU side Gate Array registers
 */

#ifndef MEGADEV__MAIN_GATEARR_DEF_H
#define MEGADEV__MAIN_GATEARR_DEF_H

/**
 * @def _GA_RESET
 * @sa GA_RESET
 */
#define _GA_RESET 0xA12000

// ******* _GA_RESET
#define GA_RESET_IFL2_BIT 0

// ******* _GA_RESET+1
#define GA_RESET_SRES_BIT 0
#define GA_RESET_SBRQ_BIT 1

/**
 * @def GA_RESET_IFL2_MSK
 * @brief Send INT2 trigger to Sub CPU [_GA_RESET bitmask]
 */
#define GA_RESET_IFL2_MSK (1 << GA_RESET_IFL2_BIT)

/**
 * @def GA_RESET_SRES_MSK
 * @brief Sub CPU reset [_GA_RESET+1 bitmask]
 */
#define GA_RESET_SRES_MSK (1 << GA_RESET_SRES_BIT)

/**
 * @def GA_RESET_SBRQ_MSK
 * @brief Sub CPU bus access request [_GA_RESET+1 bitmask]
 */
#define GA_RESET_SBRQ_MSK (1 << GA_RESET_SBRQ_BIT)

/**
 * @def _GA_MEMMODE
 * @sa GA_MEMMODE
 */
#define _GA_MEMMODE 0xA12002

#define GA_RET_BIT 0
#define GA_DMNA_BIT 1
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

#define GA_RET_MSK 1 << GA_RET_BIT
#define MEMMODE_DMNA_MSK 1 << GA_DMNA_BIT
#define GA_MODE_MSK 1 << MEMMODE_MODE_BIT
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
 * @def _GA_CDCMODE
 * @brief CDC Mode
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
#define _GA_CDCMODE 0xA12004

/**
 * @def _GA_HINTVECT
 * @brief HINT Vector
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
#define _GA_HINTVECT 0xA12006

/**
 * @def _GA_CDCHOSTDATA
 * @brief CDC host data
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HD15|HD14|HD13|HD12|HD11|HD10|HD09|HD08|HD07|HD06|HD05|HD04|HD03|HD02|HD01|HD00|
 *
 * HD: CDC read data
 *
 * @warning Word access only!
 */
#define _GA_CDCHOSTDATA 0xA12008 /* 16-bit CDC host data */

/**
 * @def _GA_STOPWATCH
 * @brief Stop watch
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
#define _GA_STOPWATCH 0xA1200C

/**
 * @def _GA_COMFLAGS
 * @brief Main/Sub CPU communication flags
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |CFM7|CFM6|CFM5|CFM4|CFM3|CFM2|CFM1|CFM0|CFS7|CFS6|CFS5|CFS4|CFS3|CFS2|CFS1|CFS0|
 *
 * CFM: Comm flags for Main CPU
 * CFS: Comm flags for Sub CPU
 */
#define _GA_COMFLAGS 0xA1200E

/**
 * @def _GA_COMCMD0
 * @brief Communication command (Main -> Sub)
 * @details RW: 16 bit data
 */
#define _GA_COMCMD0 0xA12010

/**
 * @def _GA_COMCMD1
 * @brief Communication command (Main -> Sub)
 * @details RW: 16 bit data
 */
#define _GA_COMCMD1 0xA12012

/**
 * @def _GA_COMCMD2
 * @brief Communication command (Main -> Sub)
 * @details RW: 16 bit data
 */
#define _GA_COMCMD2 0xA12014

/**
 * @def _GA_COMCMD3
 * @brief Communication command (Main -> Sub)
 * @details RW: 16 bit data
 */
#define _GA_COMCMD3 0xA12016

/**
 * @def _GA_COMCMD4
 * @brief Communication command (Main -> Sub)
 * @details RW: 16 bit data
 */
#define _GA_COMCMD4 0xA12018

/**
 * @def _GA_COMCMD5
 * @brief Communication command (Main -> Sub)
 */
#define _GA_COMCMD5 0xA1201A

/**
 * @def _GA_COMCMD6
 * @brief Communication command (Main -> Sub)
 * @details RW: 16 bit data
 */
#define _GA_COMCMD6 0xA1201C

/**
 * @def _GA_COMCMD7
 * @brief Communication command (Main -> Sub)
 * @details RW: 16 bit data
 */
#define _GA_COMCMD7 0xA1201E

/**
 * @def _GA_COMSTAT0
 * @brief Communication status (Sub -> Main)
 * @details R: 16 bit data
 */
#define _GA_COMSTAT0 0xA12020

/**
 * @def _GA_COMSTAT1
 * @brief Communication status (Sub -> Main)
 * @details R: 16 bit data
 */
#define _GA_COMSTAT1 0xA12022

/**
 * @def _GA_COMSTAT2
 * @brief Communication status (Sub -> Main)
 * @details R: 16 bit data
 */
#define _GA_COMSTAT2 0xA12024

/**
 * @def _GA_COMSTAT3
 * @brief Communication status (Sub -> Main)
 * @details R: 16 bit data
 */
#define _GA_COMSTAT3 0xA12026

/**
 * @def _GA_COMSTAT4
 * @brief Communication status (Sub -> Main)
 * @details R: 16 bit data
 */
#define _GA_COMSTAT4 0xA12028

/**
 * @def _GA_COMSTAT5
 * @brief Communication status (Sub -> Main)
 * @details R: 16 bit data
 */
#define _GA_COMSTAT5 0xA1202A

/**
 * @def _GA_COMSTAT6
 * @brief Communication status (Sub -> Main)
 * @details R: 16 bit data
 */
#define _GA_COMSTAT6 0xA1202C

/**
 * @def _GA_COMSTAT7
 * @brief Communication status (Sub -> Main)
 * @details R: 16 bit data
 */
#define _GA_COMSTAT7 0xA1202E

#endif

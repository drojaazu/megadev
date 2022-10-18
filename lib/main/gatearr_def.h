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
 * @ingroup main_gatearr
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
 * @ingroup main_gatearr
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

#define MSK_GA_RET 1 << GA_RET_BIT
#define MEMMODE_DMNA_MSK 1 << GA_DMNA_BIT
#define MSK_GA_MODE 1 << MEMMODE_MODE_BIT
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
 * @sa GA_CDCMODE
 * @ingroup main_gatearr
 */
#define _GA_CDCMODE 0xA12004

/**
 * @def _GA_HINTVECT
 * @sa GA_HINTVECT
 * @ingroup main_gatearr
 */
#define _GA_HINTVECT 0xA12006

/**
 * @def _GA_CDCHOSTDATA
 * @sa GA_CDCHOSTDATA
 * @ingroup main_gatearr
 */
#define _GA_CDCHOSTDATA 0xA12008 /* 16-bit CDC host data */

/**
 * @def _GA_STOPWATCH
 * @sa GA_STOPWATCH
 * @ingroup main_gatearr
 */
#define _GA_STOPWATCH 0xA1200C

/**
 * @def _GA_COMFLAGS
 * @sa GA_COMFLAGS
 * @ingroup main_gatearr
 */
#define _GA_COMFLAGS 0xA1200E

/**
 * @def _GA_COMCMD0
 * @sa GA_COMCMD0
 * @ingroup main_gatearr
 */
#define _GA_COMCMD0 0xA12010

/**
 * @def _GA_COMCMD1
 * @sa GA_COMCMD1
 * @ingroup main_gatearr
 */
#define _GA_COMCMD1 0xA12012

/**
 * @def _GA_COMCMD2
 * @sa GA_COMCMD2
 * @ingroup main_gatearr
 */
#define _GA_COMCMD2 0xA12014

/**
 * @def _GA_COMCMD3
 * @sa GA_COMCMD3
 * @ingroup main_gatearr
 */
#define _GA_COMCMD3 0xA12016

/**
 * @def _GA_COMCMD4
 * @sa GA_COMCMD4
 * @ingroup main_gatearr
 */
#define _GA_COMCMD4 0xA12018

/**
 * @def _GA_COMCMD5
 * @sa GA_COMCMD5
 * @ingroup main_gatearr
 */
#define _GA_COMCMD5 0xA1201A

/**
 * @def _GA_COMCMD6
 * @sa GA_COMCMD6
 * @ingroup main_gatearr
 */
#define _GA_COMCMD6 0xA1201C

/**
 * @def _GA_COMCMD7
 * @sa GA_COMCMD7
 * @ingroup main_gatearr
 */
#define _GA_COMCMD7 0xA1201E

/**
 * @def _GA_COMSTAT0
 * @sa GA_COMSTAT0
 * @ingroup main_gatearr
 */
#define _GA_COMSTAT0 0xA12020

/**
 * @def _GA_COMSTAT1
 * @sa GA_COMSTAT1
 * @ingroup main_gatearr
 */
#define _GA_COMSTAT1 0xA12022

/**
 * @def _GA_COMSTAT2
 * @sa GA_COMSTAT2
 * @ingroup main_gatearr
 */
#define _GA_COMSTAT2 0xA12024

/**
 * @def _GA_COMSTAT3
 * @sa GA_COMSTAT3
 * @ingroup main_gatearr
 */
#define _GA_COMSTAT3 0xA12026

/**
 * @def _GA_COMSTAT4
 * @sa GA_COMSTAT4
 * @ingroup main_gatearr
 */
#define _GA_COMSTAT4 0xA12028

/**
 * @def _GA_COMSTAT5
 * @sa GA_COMSTAT5
 * @ingroup main_gatearr
 */
#define _GA_COMSTAT5 0xA1202A

/**
 * @def _GA_COMSTAT6
 * @sa GA_COMSTAT6
 * @ingroup main_gatearr
 */
#define _GA_COMSTAT6 0xA1202C

/**
 * @def _GA_COMSTAT7
 * @sa GA_COMSTAT7
 * @ingroup main_gatearr
 */
#define _GA_COMSTAT7 0xA1202E

#endif

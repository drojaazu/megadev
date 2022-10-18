/**
 * @file
 * @brief Function codes for the _CDBOOT vector
 */

#ifndef MEGADEV__SUB_CDBOOT_DEF_H
#define MEGADEV__SUB_CDBOOT_DEF_H

#include "sub/memmap_def.h"

/**
 * @def CBTINIT
 * @brief Initializes the CD boot system
 * @clobber d0-d1/a0-a1
 */
#define CBTINIT 0x0000

/**
 * @def CBTINT
 * @brief Calls the routine to manage interrupts
 * @clobber d0-d1/a0-a1
 *
 * @note Should be called every 16.6 msec
 */
#define CBTINT 0x0001

/**
 * @def CBTOPENDISC
 * @brief Opens the drive tray
 * @clobber d0-d1/a0-a1
 *
 * @param[out] CC OK
 * @param[out] CS BUSY
 *
 * @note Uses the DRVOPEN BIOS call.
 *
 * @note Returns BUSY only when interrupts are being handled by CBTINT.
 *
 */
#define CBTOPENDISC 0x0002

/**
 * @def CBTOPENSTAT
 * @brief Checks the status of the open tray request
 * @clobber d0-d1/a0-a1
 *
 * @param[out] CC COMPLETE
 * @param[out] CS BUSY
 *
 */
#define CBTOPENSTAT 0x0003

/**
 * @def CBTCHKDISC
 * @brief Begins disc boot
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to work RAM (0x800 bytes)
 * @param[out] CC OK
 * @param[out] CS BUSY
 *
 * @note Returns BUSY only when interrupts are being handled by CBTINT.
 *
 */
#define CBTCHKDISC 0x0004

#define DISC_NOTREADY 0xFF
#define DISC_NODISC 0x00
#define DISC_MUSIC 0x01
#define DISC_CDROM 0x02
#define DISC_MIXED 0x03
#define DISC_SYSTEM 0x04
#define DISC_DATA 0x05
#define DISC_BOOT 0x06
#define DISC_GAME 0x07

/**
 * @def CBTCHKSTAT
 * @brief Check the disc boot status and get disc type
 * @clobber d0-d1/a0-a1
 *
 * @param[out] CC COMPLETE
 * @param[out] CS BUSY
 * @param[out] D0.w Disc type
 *
 */
#define CBTCHKSTAT 0x0005

/**
 * @def CBTIPDISC
 * @brief No official documentation; needs research
 *
 * Presumably loads IP from disc
 */
#define CBTIPDISC 0x0006

/**
 * @def CBTIPSTAT
 * @brief No official documentation; needs research
 *
 * Presumably checks on IP load status
 */
#define CBTIPSTAT 0x0007

/**
 * @def CBTSPDISC
 * @brief No official documentation; needs research
 *
 * Presumably loads SP from disc
 */
#define CBTSPDISC 0x0008

/**
 * @def CBTSPSTAT
 * @brief No official documentation; needs research
 *
 * Presumably checks on SP load status
 */
#define CBTSPSTAT 0x0009

#endif

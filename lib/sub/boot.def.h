/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file boot.def.h
 * @brief CD boot process definitions
 */

#pragma once

/**
 * @defgroup sub_boot Sub CPU / CD Boot
 */

/**
 * @def BOOT_CALL_VECTOR
 * @brief CD boot system calls vector
 * @ingroup sub_boot
 */
#define BOOT_CALL_VECTOR 0x005F1C

/**
 * @def BOOT_INIT
 * @brief Initializes the CD boot system
 * @ingroup sub_boot
 * @clobber d0-d1/a0-a1
 */
#define BOOT_INIT 0x0000

/**
 * @def BOOT_INT_HANDLER
 * @brief Calls the routine to manage interrupts
 * @ingroup sub_boot
 * @clobber d0-d1/a0-a1
 *
 * @note Should be called every 16.6 msec
 */
#define BOOT_INT_HANDLER 0x0001

/**
 * @def BOOT_OPEN_DISC
 * @brief Opens the drive tray
 * @ingroup sub_boot
 * @clobber d0-d1/a0-a1
 *
 * @param[out] CC OK
 * @param[out] CS BUSY
 *
 * @note Uses the DRV_OPEN BIOS call.
 *
 * @note Returns BUSY only when interrupts are being handled by CBTINT.
 *
 */
#define BOOT_OPEN_DISC 0x0002

/**
 * @def BOOT_OPEN_DISC_STATUS
 * @brief Checks the status of the open tray request
 * @ingroup sub_boot
 * @clobber d0-d1/a0-a1
 *
 * @param[out] CC COMPLETE
 * @param[out] CS BUSY
 *
 */
#define BOOT_OPEN_DISC_STATUS 0x0003

/**
 * @def BOOT_CHECK_DISC
 * @brief Begins disc boot
 * @ingroup sub_boot
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to work RAM (0x800 bytes)
 * @param[out] CC OK
 * @param[out] CS BUSY
 *
 * @note Returns BUSY only when interrupts are being handled by CBTINT.
 *
 */
#define BOOT_CHECK_DISC 0x0004

#define BOOT_DISC_NOTREADY 0xFF
#define BOOT_DISC_NODISC   0x00
#define BOOT_DISC_MUSIC    0x01
#define BOOT_DISC_CDROM    0x02
#define BOOT_DISC_MIXED    0x03
#define BOOT_DISC_SYSTEM   0x04
#define BOOT_DISC_DATA     0x05
#define BOOT_DISC_BOOT     0x06
#define BOOT_DISC_GAME     0x07

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
#define BOOT_CHECK_DISC_STATUS 0x0005

/**
 * @def CBTIPDISC
 * @brief Preent in 32X CD example source but no official documentation
 * available; needs research
 *
 * Presumably loads IP from disc
 */
#define CBTIPDISC 0x0006

/**
 * @def CBTIPSTAT
 * @brief Preent in 32X CD example source but no official documentation
 * available; needs research
 *
 * Presumably checks on IP load status
 */
#define CBTIPSTAT 0x0007

/**
 * @def CBTSPDISC
 * @brief Preent in 32X CD example source but no official documentation
 * available; needs research
 *
 * Presumably loads SP from disc
 */
#define CBTSPDISC 0x0008

/**
 * @def CBTSPSTAT
 * @brief Preent in 32X CD example source but no official documentation
 * available; needs research
 *
 * Presumably checks on SP load status
 */
#define CBTSPSTAT 0x0009

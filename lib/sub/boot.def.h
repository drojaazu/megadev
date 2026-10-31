/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file boot.def.h
 * @brief CD Boot system call vector and function codes
 */

#pragma once

/**
 * @def BOOT_CALL_VECTOR
 * @brief CD Boot system calls vector
 * @segaalias _CDBOOT
 */
#define BOOT_CALL_VECTOR 0x005F1C

/**
 * @def BOOT_INIT
 * @brief Initializes the CD boot system
 * @clobber d0-d1/a0-a1
 * @segaalias CBTINIT
 */
#define BOOT_INIT 0x0000

/**
 * @def BOOT_INT
 * @brief Calls the routine to manage interrupts
 * @clobber d0-d1/a0-a1
 *
 * @note Should be called every 16.6 msec
 * @segaalias CBTINT
 */
#define BOOT_INT 0x0001

/**
 * @def BOOT_OPEN_DISC
 * @brief Opens the drive tray
 * @clobber d0-d1/a0-a1
 *
 * @param[out] CC OK
 * @param[out] CS BUSY
 *
 * @note Uses the DRV_OPEN BIOS call.
 *
 * @note Returns BUSY only when interrupts are being handled by BOOT_INT.
 *
 * @segaalias CBTOPENDISC
 */
#define BOOT_OPEN_DISC 0x0002

/**
 * @def BOOT_OPEN_STATUS
 * @brief Checks the status of the open tray request
 * @clobber d0-d1/a0-a1
 *
 * @param[out] CC COMPLETE
 * @param[out] CS BUSY
 *
 * @segaalias CBTOPENSTAT
 */
#define BOOT_OPEN_STATUS 0x0003

/**
 * @def BOOT_CHECK_DISC
 * @brief Begins disc boot
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to work RAM (0x800 bytes)
 * @param[out] CC OK
 * @param[out] CS BUSY
 *
 * @note Returns BUSY only when interrupts are being handled by BOOT_INT.
 *
 * @segaalias CBTCHKDISC
 */
#define BOOT_CHECK_DISC 0x0004

#define DISC_NOTREADY 0xFF
#define DISC_NODISC		0x00
#define DISC_MUSIC		0x01
#define DISC_CDROM		0x02
#define DISC_MIXED		0x03
#define DISC_SYSTEM		0x04
#define DISC_DATA			0x05
#define DISC_BOOT			0x06
#define DISC_GAME			0x07

/**
 * @def BOOT_CHECK_STATUS
 * @brief Check the disc boot status and get disc type
 * @clobber d0-d1/a0-a1
 *
 * @param[out] CC COMPLETE
 * @param[out] CS BUSY
 * @param[out] D0.w Disc type
 *
 * @segaalias CBTCHKSTAT
 */
#define BOOT_CHECK_STATUS 0x0005

/**
 * @def BOOT_LOAD_IP
 * @brief No official documentation. The name comes from the Sega 32X SDK includes.
 *
 * Presumably loads IP from disc
 * @segaalias CBTIPDISC
 */
#define BOOT_LOAD_IP 0x0006

/**
 * @def BOOT_LOAD_IP_STATUS
 * @brief No official documentation. The name comes from the Sega 32X SDK includes.
 *
 * Presumably checks on IP load status
 * @segaalias CBTIPSTAT
 */
#define BOOT_LOAD_IP_STATUS 0x0007

/**
 * @def BOOT_LOAD_SP
 * @brief No official documentation. The name comes from the Sega 32X SDK includes.
 *
 * Presumably loads SP from disc
 * @segaalias CBTSPDISC
 */
#define BOOT_LOAD_SP 0x0008

/**
 * @def BOOT_LOAD_SP_STATUS
 * @brief No official documentation. The name comes from the Sega 32X SDK includes.
 *
 * Presumably checks on SP load status
 * @segaalias CBTSPSTAT
 */
#define BOOT_LOAD_SP_STATUS 0x0009

/**
 * @defgroup sub_boot_sega Sega documentation names
 * @brief The names Sega's documentation uses for the definitions above
 * @{
 */
/** @aliasof BOOT_CALL_VECTOR */
#define CDBOOT BOOT_CALL_VECTOR
/** @aliasof BOOT_INIT */
#define CBTINIT BOOT_INIT
/** @aliasof BOOT_INT */
#define CBTINT BOOT_INT
/** @aliasof BOOT_OPEN_DISC */
#define CBTOPENDISC BOOT_OPEN_DISC
/** @aliasof BOOT_OPEN_STATUS */
#define CBTOPENSTAT BOOT_OPEN_STATUS
/** @aliasof BOOT_CHECK_DISC */
#define CBTCHKDISC BOOT_CHECK_DISC
/** @aliasof BOOT_CHECK_STATUS */
#define CBTCHKSTAT BOOT_CHECK_STATUS
/** @aliasof BOOT_LOAD_IP */
#define CBTIPDISC BOOT_LOAD_IP
/** @aliasof BOOT_LOAD_IP_STATUS */
#define CBTIPSTAT BOOT_LOAD_IP_STATUS
/** @aliasof BOOT_LOAD_SP */
#define CBTSPDISC BOOT_LOAD_SP
/** @aliasof BOOT_LOAD_SP_STATUS */
#define CBTSPSTAT BOOT_LOAD_SP_STATUS
/** @} */

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file mode1.def.h
 * @brief Mode 1 definitions (Mega CD driven from a cartridge)
 *
 * @details
 * In Mode 1 the cartridge is the boot device, the Boot ROM appears at
 * BOOT_ROM (0x400000) and the Sub CPU does nothing until the cartridge brings
 * it up: its BIOS lives compressed inside the Boot ROM and must be unpacked
 * into Program RAM by the Main CPU. The routines in mode1.s do that work.
 */

#pragma once

#include <main/memmap.def.h>

/**
 * @brief Main CPU view of Program RAM bank 0 in Mode 1
 */
#define MODE1_PRG_RAM 0x420000

/**
 * @brief Where the Sub CPU program (SP) is placed, as seen by the Main CPU
 * @details The Sub BIOS expects the SP at Program RAM 0x6000, the same place
 * the Boot ROM loads a disc's SP.
 */
#define MODE1_SP_DEST (MODE1_PRG_RAM + 0x6000)

/**
 * @brief Program RAM write protection set after unpacking the Sub BIOS
 * @details Protects 0x0000 to 0x53FF, in units of 0x200 bytes. Every Boot ROM
 * version writes this value; the space up to 0x6000 is the Sub BIOS's work
 * area and must stay writable.
 */
#define MODE1_PRG_RAM_WP 0x2A

/**
 * @brief Largest SP the Program RAM window can take from 0x6000
 */
#define MODE1_SP_MAX_SIZE 0x1A000

/**
 * @brief Return values of mode1_load_sub
 */
#define MODE1_OK              0
#define MODE1_ERR_NO_MEGACD   1 ///< No Boot ROM at BOOT_ROM
#define MODE1_ERR_NO_LOADER   2 ///< Sub BIOS image not found in the Boot ROM
#define MODE1_ERR_SUB_TIMEOUT 3 ///< Sub CPU did not come up
#define MODE1_ERR_SP_SIZE     4 ///< SP larger than MODE1_SP_MAX_SIZE

/**
 * @brief Number of polls of the Sub side comm flags before giving up
 */
#define MODE1_SUB_TIMEOUT 0x100000

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file bram.def.h
 * @brief Backup RAM (BRAM) definitions
 */

#pragma once

/**
 * @defgroup sub_bram Sub CPU / Backup RAM
 */

/**
 * @def BRAM_CALL_VECTOR
 * @brief Backup RAM system calls vector
 */
#define BRAM_CALL_VECTOR 0x005F16

/**
 * @def BRAM_INIT
 * @alias BRMINIT
 * @brief Prepares to read/write from Backup RAM
 * @ingroup sub_bram
 * @clobber d0-d1/a0
 *
 * @param[in] A0.l Pointer to work RAM buffer (0x640 bytes)
 * @param[in] A1.l Pointer to display string buffer (12 bytes)
 * @param[out] CC Sega formatted RAM present
 * @param[out] CS RAM unformatted or not present
 * @param[out] D0.w Size of backup RAM (from 2 to 0x100,
 *   in blocks of 0x1000 bytes)
 * @param[out] D1.w Status
 *   0: No RAM present
 *   1: Unformatted
 *   2: Other format
 * @param[out] A1.l Pointer to display strings
 *
 */
#define BRAM_INIT 0x0000

/**
 * @def BRAM_USAGE
 * @alias BRMSTAT
 * @brief Returns Backup RAM usage
 * @ingroup sub_bram
 * @clobber None
 *
 * @param[in] A1.l Pointer to display string buffer (12 bytes)
 * @param[out] D0.w Free space (in blocks of 0x1000 bytes)
 * @param[out] D1.w Number of files in directory
 *
 */
#define BRAM_USAGE 0x0001

/**
 * @def BRAM_FILE_SEARCH
 * @alias BRMSERCH
 * @brief Search for the requested file in Backup RAM
 * @ingroup sub_bram
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to filename
 * @param[out] CC File found
 * @param[out] CS File not found
 * @param[out] D0.w Size of file in blocks
 * @param[out] D1.b Mode
 *   0: Normal
 *  -1: Data protected with protect function
 * @param[out] A0.l Backup RAM start address
 *
 *
 * @note The filenames are 11 ASCII characters terminated with 0
 */
#define BRAM_FILE_SEARCH 0x0002

/**
 * @def BRAM_FILE_READ
 * @alias BRMREAD
 * @brief Read data from Backup RAM
 * @ingroup sub_bram
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to filename (11 bytes)
 * @param[in] A1.l Pointer to write buffer
 * @param[out] CC Read okay
 * @param[out] CS Error
 * @param[out] D0.w Size in blocks
 * @param[out] D1.b Mode
 *
 * @details
 * Mode
 *     0: Normal
 *  0xFF: Data protected
 *
 */
#define BRAM_FILE_READ 0x0003

/**
 * @def BRAM_FILE_WRITE
 * @alias BRMWRITE
 * @brief Write data to Backup RAM
 * @ingroup sub_bram
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to file info structure
 * @param[in] A1.l Pointer to save data
 * @param[in] D1.l Zero (see note)
 * @param[out] CC Write successful
 * @param[out] CS Write failed
 *
 * @details
 * File info structure (14 bytes)
 *   filename.b[11]
 *       mode.b  0x00: normal
 *               0xFF: encoded (with protect function)
 *  data_size.w  size of data in blocks (see note)
 *
 * @note
 * The size of a block of data is dependent on the data mode. When set to
 * normal (0), a block is 0x40 bytes in size; when set to encoded (0xFF), a
 * block is 0x20 bytes in size.
 *
 * @note
 * Per Tech Bulletin #1, D1 should be set to 0 when calling this routine.
 */
#define BRAM_FILE_WRITE 0x0004

/**
 * @def BRAM_FILE_DELETE
 * @alias BRMDEL
 * @brief Deletes data in Backup RAM
 * @ingroup sub_bram
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to filename
 * @param[out] CC Delete success
 * @param[out] CS Delete failed
 */
#define BRAM_FILE_DELETE 0x0005

/**
 * @def BRAM_FORMAT
 * @alias BRMFORMAT
 * @brief Formats the Backup RAM
 * @ingroup sub_bram
 * @clobber d0-d1/a0-a1
 *
 * @param[out] CC Format success
 * @param[out] CS Format failed
 *
 * @note Call @ref BRAM_INIT before calling this function!
 */
#define BRAM_FORMAT 0x0006

/**
 * @def BRAM_DIR
 * @alias BRMDIR
 * @brief Reads Backup RAM directory
 * @ingroup sub_bram
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to file pattern
 * @param[in] A1.l Pointer to directory data buffer
 * @param[in] D1.l File skip & Buffer size
 * @details File skip & Buffer size
 *   Upper word: Number of files to skip (for paging results)
 *   Lower word: Size of directory data buffer
 * @param[out] CC Read success
 * @param[out] CS Data to read exceeds directory buffer size
 *
 * @details The output buffer will be populated with save file entries matching
 * the requested file pattern. Each entry is 16 bytes. The first 12 is the
 * filename and terminator, followed by 1 word indicating the data start offset
 * and 1 word indicating the file size in blocks.
 *
 * @note As with the filename in other BRAM functions, the file pattern is a
 * maximum of 11 bytes with a 0 terminator. However, it uses * as a wildcard
 * to match files to return. The * should appear once, after an initial search
 * pattern. So, to find all files beginning with 'SONIC', the value passed
 * should be 'SONIC*'. The value does not need to fill the full 11 bytes and can
 * terminate immediately after the asterisk.
 */
#define BRAM_DIR 0x0007

/**
 * @def BRAM_VERIFY
 * @alias BRMVERIFY
 * @brief Verify data written to Backup RAM
 * @ingroup sub_bram
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to file info structure
 * @param[in] A0.l Pointer to save data
 * @param[out] CC Data matches
 * @param[out] CS Error
 * @param[out] D0.w Error number
 *   0xFF - Data does not match
 *   0x00 - File not found
 *
 * @details File info structure
 *   filename.b[11]
 *           flag.b  0x00: normal
 *                   0xFF: encoded (with protect function)
 *     block_size.w  0x00: 1 block = 0x40 bytes
 *                   0xFF: 1 block = 0x20 bytes
 */
#define BRAM_VERIFY 0x0008

/**
 * @def BRAM_UNKNOWN09
 * @brief Unknown; present in jump table but needs to be researched
 */
#define BRAM_UNKNOWN09 0x0009

/**
 * @def BRAM_UNKNOWN0A
 * @brief Unknown; present in jump table but needs to be researched
 */
#define BRAM_UNKNOWN0A 0x000A

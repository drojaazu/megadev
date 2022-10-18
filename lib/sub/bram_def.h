/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file bram_def.h
 * @brief Function codes for the _BURAM vector
 */

#ifndef MEGADEV__SUB_BRAM_DEF_H
#define MEGADEV__SUB_BRAM_DEF_H

#include "sub/memmap_def.h"

/**
 * @def BRMINIT
 * @brief Prepares to read/write from Backup RAM
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
#define BRMINIT 0x0000

/**
 * @def BRMSTAT
 * @brief Returns Backup RAM usage
 * @clobber None
 *
 * @param[in] A1.l Pointer to display string buffer (12 bytes)
 * @param[out] D0.w Free space (in blocks of 0x1000 bytes)
 * @param[out] D1.w Number of files in directory
 *
 */
#define BRMSTAT 0x0001

/**
 * @def BRMSERCH
 * @brief Search for the requested file in Backup RAM
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
#define BRMSERCH 0x0002

/**
 * @def BRMREAD
 * @brief Read data from Backup RAM
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to filename
 * @param[in] A1.l Pointer to write buffer
 * @param[out] CC Read okay
 * @param[out] CS Error
 * @param[out] D0.w Size in blocks
 * @param[out] D1.b Mode
 * @details Mode
 *     0: Normal
 *  0xff: Data protected
 *
 */
#define BRMREAD 0x0003

/**
 * @def BRMWRITE
 * @brief Write data to Backup RAM
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to file info structure
 * @param[in] A1.l Pointer to save data
 * @param[in] D1.l Zero (see note)
 * @param[out] CC Write successful
 * @param[out] CS Write failed
 * @details File info structure
 *   filename.b[11]
 *           mode.b  0x00: normal
 *                   0xFF: encoded (with protect function)
 *      data_size.w  size of data in blocks (see note)
 *
 * @note The size of a block of data is dependent on the data mode. When set to
 * normal (0), a block is 0x40 bytes in size; when set to encoded (0xFF), a
 * block is 0x20 bytes in size.
 *
 * @note Per Tech Bulletin #1, D1 should be set to 0 when calling this routine.
 *
 * @todo According to results from BRMDIR, it looks like some commercial game
 * save filenames terminate with 0xFF instead of 0. Need to check if this is
 * significant or just an alternate terminator supported by the BIOS.
 */
#define BRMWRITE 0x0004

/**
 * @def BRMDEL
 * @brief Deletes data in Backup RAM
 * @clobber d0-d1/a0-a1
 *
 * @param[in] A0.l Pointer to filename
 * @param[out] CC Delete success
 * @param[out] CS Delete failed
 */
#define BRMDEL 0x0005

/**
 * @def BRMFORMAT
 * @brief Formats the Backup RAM
 * @clobber d0-d1/a0-a1
 *
 * @param[out] CC Format success
 * @param[out] CS Format failed
 *
 * @note Call BRMINIT before calling this function!
 */
#define BRMFORMAT 0x0006

/**
 * @def BRMDIR
 * @brief Reads Backup RAM directory
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
#define BRMDIR 0x0007

/**
 * @def BRMVERIFY
 * @brief Verify data written to Backup RAM
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
#define BRMVERIFY 0x0008

#define BRM__UNKNOWN0 0x0009
#define BRM__UNKNOWN1 0x000A

#endif

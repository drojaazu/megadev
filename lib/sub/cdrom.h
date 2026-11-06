/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file cdrom.h
 * @brief C wrappers for CD-ROM file access
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "sub/bios.h"
#include "sub/cdrom.def.h"
#include "sub/memmap.h"
#include "types.h"

/**
 * @brief Destination of the file
 * @details For CDROM_LOAD_CDC, an address in the Sub CPU's memory map. For the
 * DMA operations, the offset within the destination memory in bytes, a
 * multiple of 8: into Word RAM or Program RAM, or into the 8 KB window the Sub
 * CPU sees of the selected wave RAM bank, where every wave RAM byte sits at an
 * odd address, so twice the wave RAM offset. The access loop writes
 * GA_SUB_REG_DMAADDR itself after selecting the destination, because writing
 * the destination resets that register.
 */
extern u8 * filebuff;

/**
 * @brief Pointer to the zero-terminated filename string
 * @details The filename is in 8.3 format, made up of ISO9660 d characters, and
 * includes the version suffix.
 */
extern char const * filename;

/**
 * @brief Place an access operation value in this variable to begin
 * a load process
 */
extern volatile u16 access_op;

/**
 * @brief Contains the status of an access operation after it has completed
 */
extern volatile u16 access_op_result;

/**
 * @brief Size of the last succesfully loaded file, in bytes
 */
extern volatile u32 filesize;

/**
 * @var part_offset
 * @brief Sectors into the file at which the next load operation starts
 * @details Ignored when part_count is 0. The loop does not reset it.
 */
extern volatile u32 part_offset;

/**
 * @var part_count
 * @brief Sectors the next load operation reads; 0 loads the whole file
 * @details With a count, the operation reads exactly that many sectors from
 * part_offset, whether or not the file is that long. The loop does not reset
 * it: load_file sets it to 0, load_file_part sets both.
 */
extern volatile u32 part_count;

/**
 * @brief Load a file from the disc and wait for it
 * @param access_operation CDROM_LOAD_CDC or one of the DMA operations
 * @param load_filename ISO 9660 name with version, e.g. "DATA.BIN;1"
 * @param buffer Destination: an address for CDROM_LOAD_CDC, the offset within
 * the destination memory for the DMA operations (see filebuff)
 * @return File size in bytes, or 0 on failure (see access_op_result)
 */
static inline u32
load_file(u16 const access_operation, char const * load_filename, u8 * buffer)
{
  if (access_operation == 0)
    return CDROM_RESULT_OK;

  filename = load_filename;
  filebuff = buffer;
  part_count = 0;
  access_op = access_operation;

  do
  {
    bios_wait_vsync();
  } while (access_op != CDROM_IDLE);

  if (access_op_result == CDROM_RESULT_OK)
    return filesize;
  else
    return 0;
}

/**
 * @brief Load part of a file from the disc and wait for it
 * @param access_operation CDROM_LOAD_CDC or one of the DMA operations
 * @param load_filename ISO 9660 name with version, e.g. "AUDIO.PCM;1"
 * @param buffer Destination, as for load_file
 * @param sector_offset First sector of the part, counted from the file's start
 * @param sector_count Sectors to read; the destination receives sector_count
 * times 2048 bytes
 * @return Bytes read, or 0 on failure (see access_op_result)
 * @details Reads exactly sector_count sectors even past the end of the file.
 * For a PCM DMA destination the count is at most 2: the DMA address register
 * reaches one 4 KB wave RAM bank.
 */
static inline u32 load_file_part(u16 const access_operation,
  char const * load_filename, u8 * buffer, u32 sector_offset, u32 sector_count)
{
  filename = load_filename;
  filebuff = buffer;
  part_offset = sector_offset;
  part_count = sector_count;
  access_op = access_operation;

  do
  {
    bios_wait_vsync();
  } while (access_op != CDROM_IDLE);

  if (access_op_result == CDROM_RESULT_OK)
    return filesize;
  else
    return 0;
}

/**
 * @brief A root directory entry as the access loop caches it
 */
typedef struct FileInfo
{
  char const filename[14]; ///< FILENAME.EXT;1, space padded
  u32        offset;       ///< First sector
  u32        size;         ///< Size in bytes
} FileInfo;

/**
 * @brief Look a file up in the cached root directory
 * @return Its entry, or NULL if there is none
 */
static inline FileInfo * find_file_c(char const * filename)
{
  register u32 A0 asm("a0") = (u32) filename;
  register u8  not_found;

  // find_file: a0 in is the name, a0 out the entry; carry set means not found
  asm volatile(
    "\
      jsr find_file \n\
      scs %[not_found] \n\
    "
    : [not_found] "=d"(not_found), "+a"(A0)
    :
    : "d0", "d1", "a1", "a2", "cc");

  return not_found ? NULL : (FileInfo *) A0;
}

#ifdef __cplusplus
}
#endif

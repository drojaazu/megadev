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
 * multiple of 8: into Word RAM, Program RAM or PCM wave RAM. The access loop
 * writes GA_SUB_REG_DMAADDR itself after selecting the destination, because
 * writing the destination resets that register.
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
  access_op = access_operation;
  if (access_op == 0)
    return CDROM_RESULT_OK;

  filename = load_filename;
  filebuff = buffer;

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
  register u32 a0_filename asm("a0") = (u32) filename;
  register u32 a0_fileinfo asm("a0");

  asm volatile(
    "\
			jsr find_file \n\
			bcc 1f \n\
			rts \n\
		1:lea 0,a0 \n\
			rts \n\
		"
    : "+a"(a0_fileinfo)
    : "a"(a0_filename)
    : "d0", "d1", "a2");

  return (FileInfo *) a0_fileinfo;
}

#ifdef __cplusplus
}
#endif

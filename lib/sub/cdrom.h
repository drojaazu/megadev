/**
 * @file
 * @brief C wrappers for CD-ROM file access
 */

#ifndef MEGADEV__CD_SUB_CDROM_H
#define MEGADEV__CD_SUB_CDROM_H

#include "sub/cdrom_def.h"
#include "sub/sub.h"
#include "types.h"

/**
 * @var filebuff
 * @brief Pointer to the buffer to store the output file
 * @details This is only used with the Sub CPU Direct Load operation!
 * For the DMA operations, use the GA_DMAADDR register to specify a
 * destination buffer.
 */
extern u8 * filebuff;

/**
 * @var filename
 * @brief Pointer to the zero-terminated filename string
 * @details The filename is in 8.3 format, made up of ISO9660 d characters, and
 * includes the version suffix.
 */
extern char const * filename;

/**
 * @var access_op
 * @brief Place an access operation value in this variable to begin
 * a load process
 */
extern volatile u16 access_op;

/**
 * @var access_op_result
 * @brief Contains the status of an access operation after it has completed
 */
extern volatile u16 access_op_result;

/**
 * @var filesize
 * @brief Size of the last succesfully loaded file, in bytes
 */
extern volatile u32 filesize;

/**
 * @fn load_file
 * @brief
 */
static inline u32 load_file (u16 const access_operation, char const * load_filename, u8 * buffer)
{
	access_op = access_operation;
	if (access_op == 0)
		return RESULT_OK;

	filename = load_filename;
	filebuff = buffer;

	do
	{
		bios_waitvsync();
	} while (access_op != ACC_OP_IDLE);

	if (access_op_result == RESULT_OK)
		return filesize;
	else
		return 0;
}

typedef struct FileInfo
{
	char const filename[14]; // FILENAME.EXT;1
	u32 offset;							 // in SECTORS
	u32 size;								 // in BYTES
} FileInfo;

// TODO file info struct!
static inline FileInfo * find_file_c (char const * filename)
{
	register u32 a0_filename asm("a0") = (u32) filename;
	register u32 a0_fileinfo asm("a0");

	asm volatile(R"(
  jsr find_file
  bcc 1f
  rts
1:lea 0,a0
  rts
  )"
							 : "+a"(a0_fileinfo)
							 : "a"(a0_filename)
							 : "d0", "d1", "a2");

	return (FileInfo *) a0_fileinfo;
}

#endif

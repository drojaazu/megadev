/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file bram.h
 * @brief C wrappers for Backup RAM usage
 */

#pragma once

#include "types.h"
#include <sub/bram.def.h>
#include <sub/memmap.def.h>

/**
 * @def bram_work_buff
 *
 * @warning
 * After the call to bram_init, this data should not be modified as it
 * will be used by other BRAM system calls
 */
u8 bram_work_buff[0x640];

u8 bram_string_buff[12];

typedef enum BramFormat
{
  NoRam = 0,
  Unformatted = 1,
  OtherFormat = 2,
  SegaFormatted = 3,
} BramFormat;

/**
 * @struct BramInitResult
 * @brief Results of bram_init
 */
typedef struct BramInitResult
{
  u16        bram_size;
  char *     strings;
  BramFormat status : 8;
} BramInitResult;

/**
 * @fn bram_init
 * @brief Initializes the backup RAM subsystem for use; should be called before
 * doing any reads/writes
 * @sa BRAM_INIT
 * @alias BRMINIT
 *
 * @note
 * The return status is normally only valid when there is an error, and
 * the carry flag determines the good or bad status of BRAM. However, to make
 * things easier to work with in C, we convert the carry flag status into a
 * fourth status, representing a non-failure. The BramStatus enum reflects the C
 * version status.
 *
 */
static inline void bram_init(BramInitResult * init_result)
{
  register u32 _bram_work_buff asm("a0") = (u32) bram_work_buff;
  register u32 _bram_string_buff asm("a1") = (u32) bram_string_buff;
  register u16 _BRAM_INIT asm("d0") = BRAM_INIT;

  register u16 _bram_size asm("d0");
  register u16 _bram_status asm("d1");

  asm(
    "\
			jsr %p2 \n\
			bcs 2f \n\
			move.w #3, d1 \n\
		2: \n\
		"
    : "=d"(_bram_size), "=d"(_bram_status)
    : "i"(BRAM_CALL_VECTOR),
      "d"(_BRAM_INIT),
      "a"(_bram_work_buff),
      "a"(_bram_string_buff)
    : "cc");

  init_result->bram_size = _bram_size;
  init_result->status = (BramFormat) _bram_status;
  init_result->strings = (char *) _bram_string_buff;
}

/**
 * @struct BramUsageResult
 * @brief Results of bram_get_status
 */
typedef struct BramUsageResult
{
  u16 free;
  u16 filecount;
} BramUsageResult;

/**
 * @fn bram_get_usage
 * @brief Get backup RAM file count and space free
 * @sa BRAM_USAGE
 * @alias BRMSTAT
 *
 */
static inline void bram_get_usage(BramUsageResult * usage_result)
{
  register u16 _BRAM_USAGE asm("d0") = BRAM_USAGE;
  register u32 _bram_string_buff asm("a1") = (u32) bram_string_buff;

  register u16 _free asm("d0");
  register u16 _filecount asm("d1");

  asm(
    "\
		jsr %p2 \n\
		"
    : "=d"(_free), "=d"(_filecount)
    : "i"(BRAM_CALL_VECTOR), "d"(_BRAM_USAGE), "a"(_bram_string_buff));

  usage_result->free = _free;
  usage_result->filecount = _filecount;
}

typedef enum BramFileMode
{
  Normal = 0,
  Protected = -1,
} BramFileMode;

typedef struct BramSearchResult
{
  u16          file_size;
  u8 *         dataptr;
  BramFileMode file_mode : 8;
} BramSearchResult;

/**
 * @fn bram_file_search
 * @brief Check that a file exists in backup RAM
 * @param[in] filename zero-terminated string, max of 11 characters
 * @param[out] search_result holds file metadata if found; null if not found
 * @sa BRAM_FILE_SEARCH
 * @alias BRMSERCH
 *
 */
static inline bool
bram_file_search(char const * filename, BramSearchResult * search_result)
{
  register u16 _BRAM_FILE_SEARCH asm("d0") = BRAM_FILE_SEARCH;
  register u32 _file_name asm("a0") = (u32) filename;

  register u16    _file_size asm("d0");
  register u8     _file_mode asm("d1");
  register void * _dataptr asm("a0");

  bool fail_flag;

  /*
   The method we were using for wrapping asm calls that return their status
   via the carry flag in C was to use the "asm goto" variant and test
   for/jump to the fail state as the last opcode, and otherwise dropping
   into the success state.

   While this works in theory, it fails because GCC does not support asm
   goto when outputs are specified. At least not for the m68k target.

   Binding outputs to registers as we had initially done here circumvents
   the need for specifying output variables to some extent, but since
   the compiler does not "know" that the registers holding output values
   were updated, it may assume their value has not changed and things will
   break.

   One solution online suggests using the Scc opcode to set a flag in RAM
   based on the CCR and then test against that in C after the fact, removing
   the need for a goto. While the addition of slow memory access is not
   ideal, the calls with this "issue" are not likely to appear in tight
   loops or timing critial use cases, and thus this seems to be the least
   worst path forward
  */

  asm(
    "jsr %p4 \n"
    "scs %3 \n"
    : "=d"(_file_size), "=d"(_file_mode), "=a"(_dataptr), "=d"(fail_flag)
    : "i"(BRAM_CALL_VECTOR), "d"(_BRAM_FILE_SEARCH), "a"(_file_name)
    : "a1", "cc");

  if (fail_flag)
    return false;

  search_result->dataptr = (u8 *) _dataptr;
  search_result->file_size = _file_size;
  search_result->file_mode = _file_mode;
  return true;
}

typedef struct BramReadResult
{
  u16          file_size;
  BramFileMode file_mode : 8;
} BramReadResult;

/**
 * @fn bram_file_read
 * @sa BRAM_FILE_READ
 * @alias BRMREAD
 */
static inline bool
bram_file_read(char const * filename, u8 * buffer, BramReadResult * read_result)
{
  register u16 _BRAM_FILE_READ asm("d0") = BRAM_FILE_READ;
  register u32 _filename asm("a0") = (u32) filename;
  register u32 _buffer asm("a1") = (u32) buffer;

  register u16 _file_size asm("d0");
  register u8  _file_mode asm("d1");

  bool fail_flag;

  // see comment in bram_file_search, which also applies here
  asm(
    "jsr %p3 \n"
    "scs %2 \n"
    : "=d"(_file_size), "=d"(_file_mode), "=d"(fail_flag)
    : "i"(BRAM_CALL_VECTOR), "d"(_BRAM_FILE_READ), "a"(_filename), "a"(_buffer)
    : "cc");

  if (fail_flag)
    return false;

  read_result->file_size = _file_size;
  read_result->file_mode = _file_mode;
  return true;
}

typedef struct BramFileInfo
{
  char const filename[11];
  u8         mode;
  u16        blocksize;
} BramFileInfo;

/**
 * @fn bram_file_write
 * @sa BRAM_FILE_WRITE
 * @alias BRMWRITE
 *
 */
static inline bool
bram_file_write(BramFileInfo const * file_info, u8 const * file_data)
{
  register u16 _BRAM_FILE_WRITE asm("d0") = BRAM_FILE_WRITE;
  register u32 _file_info asm("a0") = (u32) file_info;
  register u32 _file_data asm("a1") = (u32) file_data;

  asm goto(
    "\
			moveq #0, d1 \n\
			jsr %p0 \n\
			bcs %l[failure] \n\
		2: \n\
		"
    :
    : "i"(BRAM_CALL_VECTOR),
      "d"(_BRAM_FILE_WRITE),
      "a"(_file_info),
      "a"(_file_data)
    : "d1", "cc"
    : failure);

  return true;

failure:
  return false;
}

/**
 * @fn bram_file_delete
 * @sa BRAM_FILE_DELETE
 * @alias BRMDEL
 *
 */
static inline bool bram_file_delete(char const * filename)
{
  register u16 _BRAM_FILE_DELETE asm("d0") = BRAM_FILE_DELETE;
  register u32 A0 asm("a0") = (u32) filename;

  asm goto(
    "\
			jsr %p0 \n\
			bcs %l[failure] \n\
		"
    :
    : "i"(BRAM_CALL_VECTOR), "d"(_BRAM_FILE_DELETE), "a"(A0)
    : "d1", "a1", "cc"
    : failure);

  return true;

failure:
  return false;
}

/**
 * @fn bram_dir
 * @sa BRAM_DIR
 * @alias BRMDIR
 */
static inline bool bram_dir(
  char const * filename, u8 * dirbuffer, u16 const fileskip, u16 const dirsize)
{
  register u16 _BRAM_DIR asm("d0") = BRAM_DIR;
  register u32 a0_filename asm("a0") = (u32) filename;
  register u32 a1_dirbuffer asm("a1") = (u32) dirbuffer;
  register u32 d1_params asm("d1") = (u32) ((fileskip << 16) + dirsize);

  asm goto(
    "\
			jsr %p0 \n\
			bcs %l[too_large] \n\
		"
    :
    : "i"(BRAM_CALL_VECTOR),
      "d"(_BRAM_DIR),
      "d"(d1_params),
      "a"(a0_filename),
      "a"(a1_dirbuffer)
    : "cc"
    : too_large);

  return true;

too_large:
  return false;
}

/**
 * @fn bram_format
 * @sa BRAM_FORMAT
 * @alias BRMFORMAT
 */
static inline bool bram_format()
{
  register u16 _BRAM_FORMAT asm("d0") = BRAM_FORMAT;

  asm goto(
    "\
			jsr %p0 \n\
			bcs %l[failed] \n\
		"
    :
    : "i"(BRAM_CALL_VECTOR), "d"(_BRAM_FORMAT)
    : "d1", "a0", "a1", "cc"
    : failed);

  return true;

failed:
  return false;
}

typedef enum BramVerifyStatus
{
  NoMatch = -1,
  FileNotFound = 0,
  Match = 1
} BramVerifyStatus;

/**
 * @fn bram_verify
 * @sa BRAM_VERIFY
 * @alias BRMVERIFY
 *
 */
static inline BramVerifyStatus bram_verify(BramFileInfo const * file_info)
{
  register u16 _BRAM_VERIFY asm("d0") = BRAM_VERIFY;
  register u32 A0 asm("a0") = (u32) file_info;
  register u16 d0_result asm("d0");

  asm(
    "\
			jsr %p0 \n\
			bcs 1f \n\
			moveq #1, d0 \n\
		1: \n\
		"
    : "=d"(d0_result)
    : "i"(BRAM_CALL_VECTOR), "d"(_BRAM_VERIFY), "a"(A0)
    : "a1");

  return d0_result;
}

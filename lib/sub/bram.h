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
  SegaFormatted = 3,
  NoRam = 0,
  Unformatted = 1,
  OtherFormat = 2
} BramFormat;

/**
 * @struct BramInitResult
 * @brief Results of bram_init
 */
typedef struct BramInitResult
{
  u16             bram_size;
  enum BramFormat status;
  char *          strings;
} BramInitResult;

/**
 * @fn bram_init
 * @sa BRAM_INIT
 *
 * @note
 * The return status is normally only valid when there is an error, and
 * the carry flag determines the good or bad status of BRAM. However, to make
 * things easier to work with in C, we convert the carry flag status into a
 * fourth status, representing a non-failure. The BramStatus enum reflects the C
 * version status.
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
  init_result->status = (enum BramStatus) _bram_status;
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
 * @def bram_get_status
 * @sa BRAM_USAGE
 */
static inline void bram_get_usage(BramUsageResult * status_result)
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

  status_result->free = _free;
  status_result->filecount = _filecount;
}

typedef struct BramSearchResult
{
  u16  filesize;
  u16  mode;
  u8 * dataptr;
} BramSearchResult;

/**
 * @def bram_file_search
 * @sa BRAM_FILE_SEARCH
 */
static inline void
bram_file_search(char const * filename, BramSearchResult * search_result)
{
  register u16 _BRAM_FILE_SEARCH asm("d0") = BRAM_FILE_SEARCH;
  register u32 _file_name asm("a0") = (u32) filename;

  register u16    _file_size asm("d0");
  register u16    _file_mode asm("d1");
  register void * _dataptr asm("a0");

  // if the file is not found, we'll return null
  // the user should check that the dataptr member of the struct
  // is not null to determine the file was found
  asm(
    "\
			jsr %p3 \n\
			bcc 2f \n\
			lea 0, a0 \n\
		2: \n\
		"
    : "=d"(_file_size), "=d"(_file_mode), "=a"(_dataptr)
    : "i"(BRAM_CALL_VECTOR), "d"(_BRAM_FILE_SEARCH), "a"(_file_name)
    : "a1", "cc");

  if (_dataptr == NULL)
    return NULL;

  search_result->dataptr = (u8 *) _dataptr;
  search_result->filesize = _file_size;
  search_result->mode = _file_mode;
}

typedef struct BramReadResult
{
  bool success;
  u16  filesize;
  u8   mode;
} BramReadResult;

/**
 * @def bram_file_read
 * @sa BRAM_FILE_READ
 */
static inline void
bram_file_read(char const * filename, u8 * buffer, BramReadResult * read_result)
{
  register u16 d0_fcode asm("d0") = BRAM_FILE_READ;
  register u32 a0_filename asm("a0") = (u32) filename;
  register u32 a1_buffer asm("a1") = (u32) buffer;

  register u16 d0_size asm("d0");
  register u8  d1_mode asm("d1");

  asm(
    "\
		jsr %p2 \n\
		bcc 2f \n\
		move.w #0xFFFF, d0 \n\
	2: \n\
	"
    : "=d"(d0_size), "=d"(d1_mode)
    : "i"(BRAM_CALL_VECTOR), "d"(d0_fcode), "a"(a0_filename), "a"(a1_buffer));

  if (d0_size == 0xFFFF)
  {
    read_result->success = false;
  }
  else
  {
    read_result->success = true;
    read_result->filesize = d0_size;
    read_result->mode = d1_mode;
  }
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
 */
static inline bool bram_file_write(BramFileInfo const * params, u8 const * data)
{
  register u16 _BRAM_FILE_WRITE asm("d0") = BRAM_FILE_WRITE;
  register u32 a0_params asm("a0") = (u32) params;
  register u32 a1_data asm("a1") = (u32) data;

  asm goto(
    "\
			moveq #0, d1 \n\
			jsr %p0 \n\
			bcs %l[failed] \n\
		2: \n\
		"
    :
    : "i"(BRAM_CALL_VECTOR), "d"(_BRAM_FILE_WRITE), "a"(a0_params), "a"(a1_data)
    : "d1", "cc"
    : failed);

  return true;

failed:
  return false;
}

/**
 * @fn bram_file_delete
 * @sa BRAM_FILE_DELETE
 */
static inline bool bram_file_delete(char const * filename)
{
  register u16 _BRAM_FILE_DELETE asm("d0") = BRAM_FILE_DELETE;
  register u32 A0 asm("a0") = (u32) filename;

  asm goto(
    "\
			jsr %p0 \n\
			bcs %l[failed] \n\
		"
    :
    : "i"(BRAM_CALL_VECTOR), "d"(_BRAM_FILE_DELETE), "a"(A0)
    : "d1", "a1", "cc"
    : failed);

  return true;

failed:
  return false;
}

/**
 * @fn bram_dir
 * @sa BRAM_DIR
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
 * @sa BRMFORMAT
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
 * @sa BRMVERIFY
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

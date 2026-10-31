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

extern u8 bram_work_buff[0x640];
extern u8 bram_string_buff[12];

enum BramStatus
{
	SegaFormatted = 3,
	NoRam = 0,
	Unformatted = 1,
	OtherFormat = 2
};

/**
 * @struct BramInitResult
 * @brief Results of bram_init
 */
typedef struct BramInitResult
{
	u16							bram_size;
	enum BramStatus status;
	char *					strings;
} BramInitResult;

/**
 * @def bram_init
 * @sa BRAM_INIT
 *
 * @note The return status is normally only valid when there is an error, and
 * the carry flag determines the good or bad status of BRAM. However, to make
 * things easier to work with in C, we convert the carry flag status into a
 * fourth status, representing a non-failure. The BramStatus enum reflects the C
 * version status.
 */
static inline void bram_init(BramInitResult * init_status)
{
	register u32 a0_bram_work_buff asm("a0") = (u32) bram_work_buff;
	register u32 a1_bram_string_buff asm("a1") = (u32) bram_string_buff;
	register u16 d0_fcode asm("d0") = BRAM_INIT;

	register u16 d0_bram_size asm("d0");
	register u16 d1_bram_status asm("d1");

	asm(
		"\
			jsr %p2 \n\
			bcs 2f \n\
			move.w #3, d1 \n\
		2: \n\
		"
		: "=d"(d0_bram_size), "=d"(d1_bram_status)
		: "i"(BRAM_CALL_VECTOR),
			"d"(d0_fcode),
			"a"(a0_bram_work_buff),
			"a"(a1_bram_string_buff)
		: "cc");

	init_status->bram_size = d0_bram_size;
	init_status->status = (enum BramStatus) d1_bram_status;
	init_status->strings = (char *) a1_bram_string_buff;
}

/**
 * @struct BramStatusResult
 * @brief Results of bram_status
 */
typedef struct BramStatusResult
{
	u16 free;
	u16 filecount;
} BramStatusResult;

extern BramStatusResult bram_status_result;

/**
 * @def bram_status
 * @sa BRAM_STATUS
 */
static inline BramStatusResult * bram_status()
{
	register u16 d0_fcode asm("d0") = BRAM_STATUS;
	register u32 a1_bram_string_buff asm("a1") = (u32) bram_string_buff;

	register u16 d0_free asm("d0");
	register u16 d1_filecount asm("d1");

	asm(
		"\
		jsr %p2 \n\
		"
		: "=d"(d0_free), "=d"(d1_filecount)
		: "i"(BRAM_CALL_VECTOR), "d"(d0_fcode), "a"(a1_bram_string_buff));

	bram_status_result.free = d0_free;
	bram_status_result.filecount = d1_filecount;

	return &bram_status_result;
}

typedef struct BramSearchResult
{
	u16	 filesize;
	u16	 mode;
	u8 * dataptr;
} BramSearchResult;

extern BramSearchResult bram_search_result;

/**
 * @def bram_search
 * @sa BRAM_SEARCH
 */
static inline BramSearchResult * bram_search(char const * filename)
{
	register u16					d0_fcode asm("d0") = BRAM_SEARCH;
	register void const * a0_filename asm("a0") = filename;

	register u16		d0_filesize asm("d0");
	register u16		d1_filemode asm("d1");
	register void * a0_dataptr asm("a0");

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
		: "=d"(d0_filesize), "=d"(d1_filemode), "=a"(a0_dataptr)
		: "i"(BRAM_CALL_VECTOR), "d"(d0_fcode), "a"(a0_filename)
		: "a1", "cc");

	if (a0_dataptr == NULL)
		return NULL;

	bram_search_result.dataptr = (u8 *) a0_dataptr;
	bram_search_result.filesize = d0_filesize;
	bram_search_result.mode = d1_filemode;
	return &bram_search_result;
}

typedef struct BramReadResult
{
	bool success;
	u16	 filesize;
	u8	 mode;
} BramReadResult;

extern BramReadResult bram_read_result;

/**
 * @def bram_read
 * @sa BRAM_READ
 */
static inline BramReadResult * bram_read(char const * filename, u8 * buffer)
{
	register u16 d0_fcode asm("d0") = BRAM_READ;
	register u32 a0_filename asm("a0") = (u32) filename;
	register u32 a1_buffer asm("a1") = (u32) buffer;

	register u16 d0_size asm("d0");
	register u8	 d1_mode asm("d1");

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
		bram_read_result.success = false;
	}
	else
	{
		bram_read_result.success = true;
		bram_read_result.filesize = d0_size;
		bram_read_result.mode = d1_mode;
	}

	return &bram_read_result;
}

typedef struct BramFileInfo
{
	char const filename[11];
	u8				 mode;
	u16				 blocksize;
} BramFileInfo;

/**
 * @sa BRAM_WRITE
 */
static inline bool bram_write(BramFileInfo const * params, u8 const * data)
{
	register u16 d0_fcode asm("d0") = BRAM_WRITE;
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
		: "i"(BRAM_CALL_VECTOR), "d"(d0_fcode), "a"(a0_params), "a"(a1_data)
		: "d1", "cc"
		: failed);

	return true;

failed:
	return false;
}

/**
 * @sa BRAM_DELETE
 */
static inline bool bram_delete(char const (*filename)[11])
{
	register u16 D0 asm("d0") = BRAM_DELETE;
	register u32 A0 asm("a0") = (u32) filename;

	asm goto(
		"\
			jsr %p0 \n\
			bcs %l[failed] \n\
		"
		:
		: "i"(BRAM_CALL_VECTOR), "d"(D0), "a"(A0)
		: "d1", "a1", "cc"
		: failed);

	return true;

failed:
	return false;
}

/**
 * @sa BRAM_DIRECTORY
 */
static inline bool bram_directory(
	char const * filename, u8 * dirbuffer, u16 const fileskip, u16 const dirsize)
{
	register u16 d0_fcode asm("d0") = BRAM_DIRECTORY;
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
			"d"(d0_fcode),
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
 * @sa BRAM_FORMAT
 */
static inline bool bram_format()
{
	register u16 D0 asm("d0") = BRAM_FORMAT;

	asm goto(
		"\
			jsr %p0 \n\
			bcs %l[failed] \n\
		"
		:
		: "i"(BRAM_CALL_VECTOR), "d"(D0)
		: "d1", "a0", "a1", "cc"
		: failed);

	return true;

failed:
	return false;
}

enum BramVerifyStatus
{
	NoMatch = -1,
	FileNotFound = 0,
	Match = 1
};

/**
 * @sa BRAM_VERIFY
 */
static inline enum BramVerifyStatus bram_verify(BramFileInfo const * params)
{
	register u16 d0_fcode asm("d0") = BRAM_VERIFY;
	register u32 A0 asm("a0") = (u32) params;
	register u16 d0_result asm("d0");

	asm(
		"\
			jsr %p1 \n\
			bcs 1f \n\
			moveq #1, d0 \n\
		1: \n\
		"
		: "=d"(d0_result)
		: "i"(BRAM_CALL_VECTOR), "d"(d0_fcode), "a"(A0)
		: "a1");

	return d0_result;
}

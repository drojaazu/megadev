/**
 * \file
 * \brief C wrappers for Backup RAM usage
 */

#ifndef MEGADEV__SUB_BRAM_H
#define MEGADEV__SUB_BRAM_H

#include "sub/bram_def.h"
#include "sub/sub_def.h"
#include "types.h"

u8 bram_work_ram[0x640];
u8 bram_string_ram[12];

enum BramStatus {
  SegaFormatted = 3,
  NoRam = 0,
  Unformatted = 1,
  OtherFormat = 2
};

/**
 * \struct BrminitRes
 * \brief Results of bram_brminit
 */
typedef struct BrminitRes {
  u16 bram_size;
  enum BramStatus status;
  char * strings;
} BrminitRes;

u8 bram_work_ram[0x640];

u8 bram_string_buffer[12];

BrminitRes init_info;

/**
 * \sa BRMINIT
 * \note The return status is normally only valid when there is an error, and
 * the carry flag determines the good or bad status of BRAM. However, to make
 * things easier to work with in C, we convert the carry flag status into a
 * fourth status, representing a non-failure. The BramStatus enum reflects the C
 * version status.
 */
static inline BrminitRes * bram_brminit() {
  register u32 a0_bram_work_ram asm("a0") = (u32)bram_work_ram;
  register u32 a1_bram_string_ram asm("a1") = (u32)bram_string_ram;
  register u16 d0_fcode asm("d0") = BRMINIT;

  register u16 d0_bram_size asm("d0");
  register u16 d1_bram_status asm("d1");

  asm inline(R"(
  jsr %p2
  bcs 2f
  move.w #3, d1
2:
	)"
             : "=d"(d0_bram_size), "=d"(d1_bram_status)
             : "i"(_BURAM), "d"(d0_fcode), "a"(a0_bram_work_ram),
               "a"(a1_bram_string_ram)
             : "cc");

  init_info.bram_size = d0_bram_size;
  init_info.status = (enum BramStatus)d1_bram_status;
  init_info.strings = (char *)a1_bram_string_ram;

  return &init_info;
}

/**
 * \struct BrmstatRes
 * \brief Results of bram_brmstat
 */
typedef struct BrmstatRes {
  u16 free;
  u16 filecount;
} BrmstatRes;

BrmstatRes brmstat_results;

/**
 * \sa BRMSTAT
 */
static inline BrmstatRes * bram_brmstat() {
  register u16 d0_fcode asm("d0") = BRMSTAT;
  register u32 a1_bram_string_ram asm("a1") = (u32)bram_string_ram;

  register u16 d0_free asm("d0");
  register u16 d1_filecount asm("d1");

  asm inline("jsr %p2"
             : "=d"(d0_free), "=d"(d1_filecount)
             : "i"(_BURAM), "d"(d0_fcode), "a"(a1_bram_string_ram));

  brmstat_results.free = d0_free;
  brmstat_results.filecount = d1_filecount;

  return &brmstat_results;
}

typedef struct BrmserchRes {
  u16 filesize;
  u16 mode;
  u8 * dataptr;
} BrmserchRes;

BrmserchRes brmserch_results;

/**
 * \sa BRMSERCH
 */
static inline BrmserchRes * bram_brmserch(char const * filename) {
  register u16 d0_fcode asm("d0") = BRMSERCH;
  register u32 a0_filename asm("a0") = (u32)filename;

  register u16 d0_filesize asm("d0");
  register u16 d1_filemode asm("d1");
  register u32 a0_dataptr asm("a0");

  // if the file is not found, we'll return null
  // the user should check that the dataptr member of the struct
  // is not null to determine the file was found
  asm inline(R"(
  jsr %p3
  bcc 2f
  lea 0, a0
2:
	)"
             : "=d"(d0_filesize), "=d"(d1_filemode), "=a"(a0_dataptr)
             : "i"(_BURAM), "d"(d0_fcode), "a"(a0_filename)
             : "a1", "cc");

  if (a0_dataptr == NULL)
    return NULL;

  brmserch_results.dataptr = (u8 *)a0_dataptr;
  brmserch_results.filesize = d0_filesize;
  brmserch_results.mode = d1_filemode;
  return &brmserch_results;
}

typedef struct BrmreadRes {
  bool success;
  u16 filesize;
  u8 mode;
} BrmreadRes;

BrmreadRes brmread_results;

/**
 * \sa BRMREAD
 */
static inline BrmreadRes * bram_brmread(char const * filename, u8 * buffer) {
  register u16 d0_fcode asm("d0") = BRMREAD;
  register u32 a0_filename asm("a0") = (u32)filename;
  register u32 a1_buffer asm("a1") = (u32)buffer;

  register u16 d0_size asm("d0");
  register u8 d1_mode asm("d1");

  asm inline(R"(
		jsr %p2
		bcc 2f
		move.w #0xffff, d0
	2:
	)"
             : "=d"(d0_size), "=d"(d1_mode)
             : "i"(_BURAM), "d"(d0_fcode), "a"(a0_filename), "a"(a1_buffer));

  if (d0_size == 0xffff) {
    brmread_results.success = false;
  } else {
    brmread_results.success = true;
    brmread_results.filesize = d0_size;
    brmread_results.mode = d1_mode;
  }

  return &brmread_results;
}

typedef struct BramFileInfo {
  char const filename[11];
  u8 mode;
  u16 blocksize;
} BramFileInfo;

/**
 * \sa BRMWRITE
 */
static inline bool bram_brmwrite(BramFileInfo const * params, u8 const * data) {
  register u16 d0_fcode asm("d0") = BRMWRITE;
  register u32 a0_params asm("a0") = (u32)params;
  register u32 a1_data asm("a1") = (u32)data;

  asm inline goto(R"(
	moveq #0, d1
  jsr %p0
  bcs %l[failed]
2:
	)"
                  :
                  : "i"(_BURAM), "d"(d0_fcode), "a"(a0_params), "a"(a1_data)
                  : "d1", "cc"
                  : failed);

  return true;

failed:
  return false;
}

/**
 * \sa BRMDEL
 */
static inline bool bram_brmdel(char const (*filename)[11]) {
  register u16 d0_fcode asm("d0") = BRMDEL;
  register u32 a0_filename asm("a0") = (u32)filename;

  asm inline goto(R"(
	jsr %p0
	bcs %l[failed]
	)"
                  :
                  : "i"(_BURAM), "d"(d0_fcode), "a"(a0_filename)
                  : "d1", "a1", "cc"
                  : failed);

  return true;

failed:
  return false;
}

/**
 * \sa BRMDIR
 */
static inline bool bram_brmdir(char const * filename, u8 * dirbuffer,
                               u16 const fileskip, u16 const dirsize) {
  register u16 d0_fcode asm("d0") = BRMDIR;
  register u32 a0_filename asm("a0") = (u32)filename;
  register u32 a1_dirbuffer asm("a1") = (u32)dirbuffer;
  register u32 d1_params asm("d1") = (u32)((fileskip << 16) + dirsize);

  asm inline goto(R"(
	jsr %p0
	bcs %l[too_large]
	)"
                  :
                  : "i"(_BURAM), "d"(d0_fcode), "d"(d1_params),
                    "a"(a0_filename), "a"(a1_dirbuffer)
                  : "cc"
                  : too_large);

  return true;

too_large:
  return false;
}

/**
 * \sa BRMFORMAT
 */
static inline bool bram_brmformat() {
  register u16 d0_fcode asm("d0") = BRMFORMAT;

  asm inline goto(R"(
	jsr %p0
	bcs %l[failed]
	)"
                  :
                  : "i"(_BURAM), "d"(d0_fcode)
                  : "d1", "a0", "a1", "cc"
                  : failed);

  return true;

failed:
  return false;
}

enum BrmverifyStatus { NoMatch = -1, FileNotFound = 0, Match = 1 };

/**
 * \sa BRMVERIFY
 */
static inline enum BrmverifyStatus bram_brmverify(BramFileInfo const * params) {
  register u16 d0_fcode asm("d0") = BRMVERIFY;
  register u32 a0_params asm("a0") = (u32)params;
  register u16 d0_result asm("d0");

  asm inline(R"(
	jsr %p0
	bcs 1f
	moveq #1, d0
1:
	)"
             : "=d"(d0_result)
             : "i"(_BURAM), "d"(d0_fcode), "a"(a0_params)
             : "a1");

  return d0_result;
}

#endif
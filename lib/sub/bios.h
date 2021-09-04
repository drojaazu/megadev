/**
 * \file
 * \brief C wrappers for _CDBIOS calls
 */

#ifndef MEGADEV__SUB_BIOS_H
#define MEGADEV__SUB_BIOS_H

#include "sub/bios_def.h"
#include "types.h"

/**
 * \sa MSCSTOP
 */
static inline void bios_mscstop() {
  register u16 d0_fcode asm("d0") = MSCSTOP;

  asm("jsr %p1"
      : "+d"(d0_fcode)
      : "i"(_CDBIOS), "d"(d0_fcode)
      : "cc", "a0", "a1");
}

/**
 * \sa MSCPAUSEON
 */
static inline void bios_mscpauseon() {
  register u16 d0_fcode asm("d0") = MSCPAUSEON;

  asm("jsr %p1"
      : "+d"(d0_fcode)
      : "i"(_CDBIOS), "d"(d0_fcode)
      : "cc", "a0", "a1");
}

/**
 * \sa MSCPAUSEOFF
 */
static inline void bios_mscpauseoff() {
  register u16 d0_fcode asm("d0") = MSCPAUSEOFF;

  asm("jsr %p1"
      : "+d"(d0_fcode)
      : "i"(_CDBIOS), "d"(d0_fcode)
      : "cc", "a0", "a1");
};

/**
 * \sa MSCSCANFF
 */
static inline void bios_mscscanff() {
  register u16 d0_fcode asm("d0") = MSCSCANFF;

  asm("jsr %p1"
      : "+d"(d0_fcode)
      : "i"(_CDBIOS), "d"(d0_fcode)
      : "cc", "a0", "a1");
};

/**
 * \sa MSCSCANFR
 */
static inline void bios_mscscanfr() {
  register u16 d0_fcode asm("d0") = MSCSCANFR;

  asm("jsr %p1"
      : "+d"(d0_fcode)
      : "i"(_CDBIOS), "d"(d0_fcode)
      : "cc", "a0", "a1");
};

/**
 * \sa MSCSCANOFF
 */
static inline void bios_mscscanoff() {
  register u16 d0_fcode asm("d0") = MSCSCANOFF;

  asm("jsr %p1"
      : "+d"(d0_fcode)
      : "i"(_CDBIOS), "d"(d0_fcode)
      : "cc", "a0", "a1");
};

/**
 * \sa ROMPAUSEON
 */
static inline void bios_rompauseon() {
  register u16 d0_fcode asm("d0") = ROMPAUSEON;

  asm("jsr %p1"
      : "+d"(d0_fcode)
      : "i"(_CDBIOS), "d"(d0_fcode)
      : "cc", "a0", "a1");
};

/**
 * \sa ROMPAUSEOFF
 */
static inline void bios_rompauseoff() {
  register u16 d0_fcode asm("d0") = ROMPAUSEOFF;

  asm("jsr %p1"
      : "+d"(d0_fcode)
      : "i"(_CDBIOS), "d"(d0_fcode)
      : "cc", "a0", "a1");
};

/**
 * \sa DRVOPEN
 */
static inline void bios_drvopen() {
  register u16 d0_fcode asm("d0") = DRVOPEN;

  asm("jsr %p1"
      : "+d"(d0_fcode)
      : "i"(_CDBIOS), "d"(d0_fcode)
      : "cc", "a0", "a1");
};

struct DrvinitParams {
  u8 toc_track_number;
  u8 last_track;
};

/**
 * \sa DRVINIT
 */
static inline void bios_drvinit(struct DrvinitParams const * drvinit_param) {
  register u16 d0_fcode asm("d0") = DRVINIT;
  register u32 a0_drvinit_param asm("a0") = (u32)drvinit_param;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_drvinit_param)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_drvinit_param)
      : "cc", "a1");
};

/**
 * \sa MSCPLAY
 */
static inline void bios_mscplay(u16 const * track_number) {
  register u16 d0_fcode asm("d0") = MSCPLAY;
  register u32 a0_track_number asm("a0") = (u32)track_number;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_track_number)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number)
      : "cc", "a1");
};

/**
 * \sa MSCPLAY1
 */
static inline void bios_mscplay1(u16 const * track_number) {
  register u16 d0_fcode asm("d0") = MSCPLAY1;
  register u32 a0_track_number asm("a0") = (u32)track_number;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_track_number)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number)
      : "cc", "a1");
};

/**
 * \sa MSCPLAYR
 */
static inline void bios_mscplayr(u16 const * track_number) {
  register u16 d0_fcode asm("d0") = MSCPLAYR;
  register u32 a0_track_number asm("a0") = (u32)track_number;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_track_number)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number)
      : "cc", "a1");
};

/**
 * \sa MSCPLAYT
 */
static inline void bios_mscplayt(u32 const * timecode) {
  register u16 d0_fcode asm("d0") = MSCPLAYT;
  register u32 a0_time_code asm("a0") = (u32)timecode;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_time_code)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_time_code)
      : "cc", "a1");
};

/**
 * \sa MSCSEEK
 */
static inline void bios_mscseek(u32 const * track_number) {
  register u16 d0_fcode asm("d0") = MSCSEEK;
  register u32 a0_track_number asm("a0") = (u32)track_number;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_track_number)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number)
      : "cc", "a1");
};

/**
 * \sa MSCSEEKT
 */
static inline void bios_mscseekt(u32 const * timecode) {
  register u16 d0_fcode asm("d0") = MSCSEEKT;
  register u32 a0_time_code asm("a0") = (u32)timecode;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_time_code)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_time_code)
      : "cc", "a1");
};

/**
 * \sa ROMREAD
 */
static inline void bios_romread(u32 const * sector_number) {
  register u16 d0_fcode asm("d0") = ROMREAD;
  register u32 a0_sector_number asm("a0") = (u32)sector_number;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_sector_number)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_sector_number)
      : "cc", "a1");
};

/**
 * \sa ROMSEEK
 */
static inline void bios_romseek(u32 const * sector_number) {
  register u16 d0_fcode asm("d0") = ROMSEEK;
  register u32 a0_sector_number asm("a0") = (u32)sector_number;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_sector_number)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_sector_number)
      : "cc", "a1");
};

/**
 * \sa MSCSEEK1
 */
static inline void bios_mscseek1(u16 const * track_number) {
  register u16 d0_fcode asm("d0") = MSCSEEK1;
  register u32 a0_track_number asm("a0") = (u32)track_number;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_track_number)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number)
      : "cc", "a1");
};

struct RomreadParams {
  u32 start_sector;
  u32 sector_count;
};

/**
 * \sa ROMREADN
 */
static inline void bios_romreadn(struct RomreadParams const * param) {
  register u16 d0_fcode asm("d0") = ROMREADN;
  register u32 a0_param asm("a0") = (u32)param;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_param)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_param)
      : "cc", "a1");
};

/**
 * \sa ROMREADE
 */
static inline void bios_romreade(struct RomreadParams const * param) {
  register u16 d0_fcode asm("d0") = ROMREADE;
  register u32 a0_param asm("a0") = (u32)param;

  asm("jsr %p2"
      : "+d"(d0_fcode), "+a"(a0_param)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_param)
      : "cc", "a1");
};

/**
 * \sa CDBCHK
 */
static inline void bios_cdbchk() {
  register u16 d0_fcode asm("d0") = CDBCHK;

  asm("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc");
};

typedef struct CDBStatus {
  u16 bios_status;
  u16 led;
  u8 cdd_status[20];
  u32 volume;
  u32 header;
} CDBStatus;

/**
 * \sa CDBSTAT
 */
static inline CDBStatus const * bios_cdbstat() {
  register u16 d0_fcode asm("d0") = CDBSTAT;
  register u32 a0_ptr asm("a0");

  asm("jsr %p2"
      : "+d"(d0_fcode), "=a"(a0_ptr)
      : "i"(_CDBIOS), "d"(d0_fcode)
      : "cc", "d1");

  return (CDBStatus *)a0_ptr;
}

/**
 * \sa FDRSET
 */
static inline void bios_fdrset(u16 const volume) {
  register u16 d0_fcode asm("d0") = FDRSET;
  register u16 const d1_volume asm("d1") = volume;

  asm("jsr %p1"
      : "+d"(d0_fcode)
      : "i"(_CDBIOS), "d"(d0_fcode), "d"(d1_volume)
      : "cc");
};

/**
 * \sa FDRCHG
 */
static inline void bios_fdrchg(u32 const volume) {
  register u16 d0_fcode asm("d0") = FDRCHG;
  register u32 d1_volume asm("d1") = volume;

  asm("jsr %p1"
      : "+d"(d0_fcode), "+d"(d1_volume)
      : "i"(_CDBIOS), "d"(d0_fcode), "d"(d1_volume)
      : "cc");
};

/**
 * \sa CDCSTART
 */
static inline void bios_cdcstart() {
  register u16 d0_fcode asm("d0") = CDCSTART;

  asm("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "d1");
};

/**
 * \sa CDCSTOP
 */
static inline void bios_cdcstop() {
  register u16 d0_fcode asm("d0") = CDCSTOP;

  asm("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc");
};

/**
 * \fn bios_cdcstat
 * \sa CDCSTAT
 */
static inline void bios_cdcstat() {
  register u16 d0_fcode asm("d0") = CDCSTAT;

  asm("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "d1");
};

/**
 * \sa CDCREAD
 */
// headaches with asm goto and output operands... we'll come back to this later.
/*
static inline bool bios_cdcread() {
  register u16 d0_fcode asm("d0") = CDCREAD;

  asm goto(R"(jsr %p1)"
           : "+d"(d0_fcode)
           : "i"(_CDBIOS), "d"(d0_fcode)
           : "cc", "d1");

  return false;

sector_ready:
  return true;
};
*/

/**
 * \sa CDCTRN
 */
static inline void bios_cdctrn(u8 * sector_dest, u8 * header_dest) {
  register u16 d0_fcode asm("d0") = CDCTRN;
  register u8 * a0_sector_dest asm("a0") = sector_dest;
  register u8 * a1_header_dest asm("a1") = header_dest;

  asm("jsr %p3"
      : "+d"(d0_fcode), "+a"(a0_sector_dest), "+a"(a1_header_dest)
      : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_sector_dest), "a"(a1_header_dest)
      : "cc", "d1");
};

/**
 * \sa CDCACK
 */
static inline void bios_cdcack() {
  register u16 d0_fcode asm("d0") = CDCACK;

  asm("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc");
};

#endif
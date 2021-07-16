/**
 * \file sub/bios.h
 * \brief C wrappers for _CDBIOS calls
 */

#ifndef MEGADEV__CD_SUB_BIOS_H
#define MEGADEV__CD_SUB_BIOS_H

#include "sub/bios_def.h"
#include "types.h"

/**
 * \sa MSCSTOP
 */
inline void BIOS_MSCSTOP() {
  register u16 d0_fcode asm("d0") = MSCSTOP;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa MSCPAUSEON
 */
inline void BIOS_MSCPAUSEON() {
  register u16 d0_fcode asm("d0") = MSCPAUSEON;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa MSCPAUSEOFF
 */
inline void BIOS_MSCPAUSEOFF() {
  register u16 d0_fcode asm("d0") = MSCPAUSEOFF;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa MSCSCANFF
 */
inline void BIOS_MSCSCANFF() {
  register u16 d0_fcode asm("d0") = MSCSCANFF;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa MSCSCANFR
 */
inline void BIOS_MSCSCANFR() {
  register u16 d0_fcode asm("d0") = MSCSCANFR;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa MSCSCANOFF
 */
inline void BIOS_MSCSCANOFF() {
  register u16 d0_fcode asm("d0") = MSCSCANOFF;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa ROMPAUSEON
 */
inline void BIOS_ROMPAUSEON() {
  register u16 d0_fcode asm("d0") = ROMPAUSEON;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa ROMPAUSEOFF
 */
inline void BIOS_ROMPAUSEOFF() {
  register u16 d0_fcode asm("d0") = ROMPAUSEOFF;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa DRVOPEN
 */
inline void BIOS_DRVOPEN() {
  register u16 d0_fcode asm("d0") = DRVOPEN;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

struct DrvinitParams {
  u8 toc_track_number;
  u8 last_track;
};

/**
 * \sa DRVINIT
 */
inline void BIOS_DRVINIT(struct DrvinitParams const * drvinit_param) {
  register u16 d0_fcode asm("d0") = DRVINIT;
  register struct DrvinitParams const * a0_drvinit_param asm("a0") =
      drvinit_param;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_drvinit_param));
};

/**
 * \sa MSCPLAY
 */
inline void BIOS_MSCPLAY(u16 const * track_number) {
  register u16 d0_fcode asm("d0") = MSCPLAY;
  register u16 const * a0_track_number asm("a0") = track_number;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number));
};

/**
 * \sa MSCPLAY1
 */
inline void BIOS_MSCPLAY1(u16 const * track_number) {
  register u16 d0_fcode asm("d0") = MSCPLAY1;
  register u16 const * a0_track_number asm("a0") = track_number;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number));
};

/**
 * \sa MSCPLAYR
 */
inline void bios_mscplayr(u16 const * track_number) {
  register u16 d0_fcode asm("d0") = MSCPLAYR;
  register u16 const * a0_track_number asm("a0") = track_number;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number));
};

/**
 * \sa MSCPLAYT
 */
inline void BIOS_MSCPLAYT(u32 const * timecode) {
  register u16 d0_fcode asm("d0") = MSCPLAYT;
  register u32 a0_time_code asm("a0") = (u32)timecode;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_time_code));
};

/**
 * \sa MSCSEEK
 */
inline void BIOS_MSCSEEK(u32 const * track_number) {
  register u16 d0_fcode asm("d0") = MSCSEEK;
  register u32 const * a0_track_number asm("a0") = track_number;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number));
};

/**
 * \sa MSCSEEKT
 */
inline void BIOS_MSCSEEKT(u32 const * timecode) {
  register u16 d0_fcode asm("d0") = MSCSEEKT;
  register u32 a0_time_code asm("a0") = (u32)timecode;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_time_code));
};

/**
 * \sa ROMREAD
 */
inline void BIOS_ROMREAD(u32 const * sector_number) {
  register u16 d0_fcode asm("d0") = ROMREAD;
  register u32 a0_sector_number asm("a0") = (u32)sector_number;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_sector_number));
};

/**
 * ROMSEEK
 */
inline void BIOS_ROMSEEK(u32 const * sector_number) {
  register u16 d0_fcode asm("d0") = ROMSEEK;
  register u32 a0_sector_number asm("a0") = (u32)sector_number;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_sector_number));
};

/**
 * \sa ROMSEEK
 */
inline void BIOS_MSCSEEK1(u16 const * track_number) {
  register u16 d0_fcode asm("d0") = MSCSEEK1;
  register u16 const * a0_track_number asm("a0") = track_number;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number));
};

struct RomreadParams {
  u32 start_sector;
  u32 sector_count;
};

/**
 * \sa ROMREADN
 */
inline void BIOS_ROMREADN(struct RomreadParams const * param) {
  register u16 d0_fcode asm("d0") = ROMREADN;
  register u32 a0_param asm("a0") = (u32)param;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_param));
};

/**
 * \sa ROMREADE
 */
inline void BIOS_ROMREADE(struct RomreadParams const * param) {
  register u16 d0_fcode asm("d0") = ROMREADE;
  register u32 a0_param asm("a0") = (u32)param;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_param));
};

/**
 * \sa CDBCHK
 */
inline void BIOS_CDBCHK(u32 const * sector_number) {
  register u16 d0_fcode asm("d0") = CDBCHK;
  register u32 const * a0_sector_number asm("a0") = sector_number;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_sector_number));
};

/**
 * \sa CDBSTAT
 */
inline void BIOS_CDBSTAT() {
  register u16 d0_fcode asm("d0") = CDBSTAT;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
}

/**
 * \sa FDRSET
 */
inline void bios_fdrset(u16 const volume) {
  register u16 d0_fcode asm("d0") = FDRSET;
  register u16 const d1_volume asm("d1") = volume;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "d"(d1_volume));
};

/**
 * \sa FDRCHG
 */
inline void bios_fdrchg(u32 const volume) {
  register u16 d0_fcode asm("d0") = FDRCHG;
  register u32 const d1_volume asm("d1") = volume;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "d"(d1_volume));
};

/**
 * \sa CDCSTART
 */
inline void BIOS_CDCSTART() {
  register u16 d0_fcode asm("d0") = CDCSTART;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa CDCSTOP
 */
inline void BIOS_CDCSTOP() {
  register u16 d0_fcode asm("d0") = CDCSTOP;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa CDCSTAT
 */
inline void BIOS_CDCSTAT() {
  register u16 d0_fcode asm("d0") = CDCSTAT;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa CDCREAD
 */
inline void BIOS_CDCREAD() {
  register u16 d0_fcode asm("d0") = CDCREAD;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

/**
 * \sa CDCTRN
 */
inline void BIOS_CDCTRN(u8 * sector_dest, u8 * header_dest) {
  register u16 d0_fcode asm("d0") = CDCTRN;
  register u8 * a0_sector_dest asm("a0") = sector_dest;
  register u8 * a1_header_dest asm("a1") = header_dest;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode), "a"(a0_sector_dest),
      "a"(a1_header_dest));
};

/**
 * \sa CDCACK
 */
inline void BIOS_CDCACK() {
  register u16 d0_fcode asm("d0") = CDCACK;
  asm("jsr %p0" ::"i"(_CDBIOS), "d"(d0_fcode));
};

#endif
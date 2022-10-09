/**
 * @file
 * @brief C wrappers for _CDBIOS calls
 */

#ifndef MEGADEV__SUB_BIOS_H
#define MEGADEV__SUB_BIOS_H

#include "sub/bios_def.h"
#include "types.h"

/**
 * @sa _BIOS_MSCSTOP
 */
static inline void bios_mscstop()
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCSTOP;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "a0", "a1");
}

/**
 * @sa _BIOS_MSCPAUSEON
 */
static inline void bios_mscpauseon()
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCPAUSEON;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "a0", "a1");
}

/**
 * @sa _BIOS_MSCPAUSEOFF
 */
static inline void bios_mscpauseoff()
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCPAUSEOFF;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "a0", "a1");
};

/**
 * @sa _BIOS_MSCSCANFF
 */
static inline void bios_mscscanff()
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCSCANFF;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "a0", "a1");
};

/**
 * @sa _BIOS_MSCSCANFR
 */
static inline void bios_mscscanfr()
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCSCANFR;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "a0", "a1");
};

/**
 * @sa _BIOS_MSCSCANOFF
 */
static inline void bios_mscscanoff()
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCSCANOFF;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "a0", "a1");
};

/**
 * @sa _BIOS_ROMPAUSEON
 */
static inline void bios_rompauseon()
{
	register u16 d0_fcode asm("d0") = _BIOS_ROMPAUSEON;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "a0", "a1");
};

/**
 * @sa _BIOS_ROMPAUSEOFF
 */
static inline void bios_rompauseoff()
{
	register u16 d0_fcode asm("d0") = _BIOS_ROMPAUSEOFF;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "a0", "a1");
};

/**
 * @sa _BIOS_DRVOPEN
 */
static inline void bios_drvopen()
{
	register u16 d0_fcode asm("d0") = _BIOS_DRVOPEN;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "a0", "a1");
};

struct DrvinitParams
{
	u8 toc_track_number;
	u8 last_track;
};

/**
 * @sa _BIOS_DRVINIT
 */
static inline void bios_drvinit (struct DrvinitParams const * drvinit_param)
{
	register u16 d0_fcode asm("d0") = _BIOS_DRVINIT;
	register u32 a0_drvinit_param asm("a0") = (u32) drvinit_param;

	asm volatile("jsr %p2"
							 : "+d"(d0_fcode), "+a"(a0_drvinit_param)
							 : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_drvinit_param)
							 : "cc", "a1");
};

/**
 * @sa _BIOS_MSCPLAY
 */
static inline void bios_mscplay (u16 const * track_number)
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCPLAY;
	register u32 a0_track_number asm("a0") = (u32) track_number;

	asm volatile("jsr %p2"
							 : "+d"(d0_fcode), "+a"(a0_track_number)
							 : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number)
							 : "cc", "a1");
};

/**
 * @sa _BIOS_MSCPLAY1
 */
static inline void bios_mscplay1 (u16 const * track_number)
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCPLAY1;
	register u32 a0_track_number asm("a0") = (u32) track_number;

	asm volatile("jsr %p2"
							 : "+d"(d0_fcode), "+a"(a0_track_number)
							 : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number)
							 : "cc", "a1");
};

/**
 * @sa _BIOS_MSCPLAYR
 */

static inline void bios_mscplayr (u16 const * track_number)
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCPLAYR;
	register u32 a0_track_number asm("a0") = (u32) track_number;

	asm volatile("jsr %p2"
							 : "+d"(d0_fcode), "+a"(a0_track_number)
							 : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number)
							 : "cc", "a1");
};

/**
 * @sa _BIOS_MSCPLAYT
 */
static inline void bios_mscplayt (u32 const * timecode)
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCPLAYT;
	register u32 a0_time_code asm("a0") = (u32) timecode;

	asm volatile("jsr %p2"
							 : "+d"(d0_fcode), "+a"(a0_time_code)
							 : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_time_code)
							 : "cc", "a1");
};

/**
 * @sa _BIOS_MSCSEEK
 */
static inline void bios_mscseek (u16 const * track_number)
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCSEEK;
	register u32 a0_track_number asm("a0") = (u32) track_number;

	asm volatile("jsr %p2"
							 : "+d"(d0_fcode), "+a"(a0_track_number)
							 : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number)
							 : "cc", "a1");
};

/**
 * @sa _BIOS_MSCSEEKT
 */
static inline void bios_mscseekt (u32 const * timecode)
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCSEEKT;
	register u32 a0_time_code asm("a0") = (u32) timecode;

	asm volatile("jsr %p2"
							 : "+d"(d0_fcode), "+a"(a0_time_code)
							 : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_time_code)
							 : "cc", "a1");
};

/**
 * @sa _BIOS_ROMREAD
 */
static inline void bios_romread (u32 const * sector_number)
{
	register u16 d0_fcode asm("d0") = _BIOS_ROMREAD;
	register u32 a0_sector_number asm("a0") = (u32) sector_number;

	asm volatile("jsr %p2"
							 : "+d"(d0_fcode), "+a"(a0_sector_number)
							 : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_sector_number)
							 : "cc", "a1");
};

/**
 * @sa _BIOS_ROMSEEK
 */
static inline void bios_romseek (u32 const * sector_number)
{
	register u16 d0_fcode asm("d0") = _BIOS_ROMSEEK;
	register u32 a0_sector_number asm("a0") = (u32) sector_number;

	asm volatile("jsr %p2"
							 : "+d"(d0_fcode), "+a"(a0_sector_number)
							 : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_sector_number)
							 : "cc", "a1");
};

/**
 * @sa _BIOS_MSCSEEK1
 */
static inline void bios_mscseek1 (u16 const * track_number)
{
	register u16 d0_fcode asm("d0") = _BIOS_MSCSEEK1;
	register u32 a0_track_number asm("a0") = (u32) track_number;

	asm volatile("jsr %p2"
							 : "+d"(d0_fcode), "+a"(a0_track_number)
							 : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_track_number)
							 : "cc", "a1");
};

struct RomreadParams
{
	u32 start_sector;
	u32 sector_count;
};

/**
 * @sa _BIOS_ROMREADN
 */
static inline void bios_romreadn (struct RomreadParams const * param)
{
	register u16 d0_fcode asm("d0") = _BIOS_ROMREADN;
	register u32 a0_param asm("a0") = (u32) param;

	asm volatile("jsr %p2" : "+d"(d0_fcode), "+a"(a0_param) : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_param) : "cc", "a1");
};

/**
 * @sa _BIOS_ROMREADE
 */
static inline void bios_romreade (struct RomreadParams const * param)
{
	register u16 d0_fcode asm("d0") = _BIOS_ROMREADE;
	register u32 a0_param asm("a0") = (u32) param;

	asm volatile("jsr %p2" : "+d"(d0_fcode), "+a"(a0_param) : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_param) : "cc", "a1");
};

/**
 * @sa _BIOS_CDBCHK
 */
static inline void bios_cdbchk()
{
	register u16 d0_fcode asm("d0") = _BIOS_CDBCHK;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc");
};

typedef struct CDBStatus
{
	u16 bios_status;
	u16 led;
	u8 cdd_status[20];
	u32 volume;
	u32 header;
} CDBStatus;

/**
 * @sa _BIOS_CDBSTAT
 */
static inline CDBStatus const * bios_cdbstat()
{
	register u16 d0_fcode asm("d0") = _BIOS_CDBSTAT;
	register u32 a0_ptr asm("a0");

	asm volatile("jsr %p2" : "+d"(d0_fcode), "=a"(a0_ptr) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "d1");

	return (CDBStatus *) a0_ptr;
}

/**
 * @sa _BIOS_FDRSET
 */
static inline void bios_fdrset (u16 const volume)
{
	register u16 d0_fcode asm("d0") = _BIOS_FDRSET;
	register u16 const d1_volume asm("d1") = volume;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode), "d"(d1_volume) : "cc");
};

/**
 * @sa _BIOS_FDRCHG
 */
static inline void bios_fdrchg (u32 const volume)
{
	register u16 d0_fcode asm("d0") = _BIOS_FDRCHG;
	register u32 d1_volume asm("d1") = volume;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode), "d"(d1_volume) : "cc");
};

/**
 * @sa _BIOS_CDCSTART
 */
static inline void bios_cdcstart()
{
	register u16 d0_fcode asm("d0") = _BIOS_CDCSTART;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "d1");
};

/**
 * @sa _BIOS_CDCSTOP
 */
static inline void bios_cdcstop()
{
	register u16 d0_fcode asm("d0") = _BIOS_CDCSTOP;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc");
};

/**
 * @fn bios_cdcstat
 * @sa _BIOS_CDCSTAT
 */
static inline void bios_cdcstat()
{
	register u16 d0_fcode asm("d0") = _BIOS_CDCSTAT;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc", "d1");
};

/**
 * @sa _BIOS_CDCREAD
 */
// headaches with asm goto and output operands... we'll come back to this later.
/*
static inline bool bios_cdcread() {
	register u16 d0_fcode asm("d0") = _BIOS_CDCREAD;

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
 * @sa _BIOS_CDCTRN
 */
static inline void bios_cdctrn (u8 * sector_dest, u8 * header_dest)
{
	register u16 d0_fcode asm("d0") = _BIOS_CDCTRN;
	register u8 * a0_sector_dest asm("a0") = sector_dest;
	register u8 * a1_header_dest asm("a1") = header_dest;

	asm volatile("jsr %p3"
							 : "+d"(d0_fcode), "+a"(a0_sector_dest), "+a"(a1_header_dest)
							 : "i"(_CDBIOS), "d"(d0_fcode), "a"(a0_sector_dest), "a"(a1_header_dest)
							 : "cc", "d1");
};

/**
 * @sa _BIOS_CDCACK
 */
static inline void bios_cdcack()
{
	register u16 d0_fcode asm("d0") = _BIOS_CDCACK;

	asm volatile("jsr %p1" : "+d"(d0_fcode) : "i"(_CDBIOS), "d"(d0_fcode) : "cc");
};

#endif
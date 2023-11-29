/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file bios.h
 * @brief C wrappers for _CDBIOS calls
 */

#ifndef MEGADEV__SUB_BIOS_H
#define MEGADEV__SUB_BIOS_H

#include "sub/bios.def.h"
#include "sub/memmap.def.h"
#include "types.h"

/**
 * @fn bios_mscstop
 * @brief Stops playing CD audio if it is playing
 * @sa _BIOS_MSCSTOP
 * @ingroup bios_cdda
 */
static inline void bios_mscstop()
{
	register u16 D0 __asm__("d0") = _BIOS_MSCSTOP;
	register u16 D0 __asm__("d0") = _BIOS_MSCSTOP;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
}

/**
 * @fn bios_mscpauseon
 * @brief Pauses the drive when a track is playing
 * @sa _BIOS_MSCPAUSEON
 * @ingroup bios_cdda
 *
 * @note If the drive is left paused it will stop after a programmable delay
 * (see @ref _CDBPAUSE)
 */
static inline void bios_mscpauseon()
{
	register u16 D0 __asm__("d0") = _BIOS_MSCPAUSEON;
	register u16 D0 __asm__("d0") = _BIOS_MSCPAUSEON;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
}

/**
 * @fn bios_mscpauseoff
 * @brief Resumes playing a track after a pause
 * @sa _BIOS_MSCPAUSEOFF
 * @ingroup bios_cdda
 *
 * @note If the drive has timed out and stopped, the BIOS will seek to the pause
 * time (with the attendant delay) and resume playing
 */
static inline void bios_mscpauseoff()
{
	register u16 D0 __asm__("d0") = _BIOS_MSCPAUSEOFF;
	register u16 D0 __asm__("d0") = _BIOS_MSCPAUSEOFF;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_mscscanff
 * @brief Starts playing from the current position in fast forward
 * @sa _BIOS_MSCSCANFF
 * @ingroup bios_cdda
 */
static inline void bios_mscscanff()
{
	register u16 D0 __asm__("d0") = _BIOS_MSCSCANFF;
	register u16 D0 __asm__("d0") = _BIOS_MSCSCANFF;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_mscscanfr
 * @brief Starts playing from the current position in fast reverse
 * @sa _BIOS_MSCSCANFR
 * @ingroup bios_cdda
 */
static inline void bios_mscscanfr()
{
	register u16 D0 __asm__("d0") = _BIOS_MSCSCANFR;
	register u16 D0 __asm__("d0") = _BIOS_MSCSCANFR;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_mscscanoff
 * @brief Returns to normal play mode
 * @sa _BIOS_MSCSCANOFF
 * @ingroup bios_cdda
 *
 * @note If the drive was paused before the scan was initiated, it will be
 * returned to pause.
 */
static inline void bios_mscscanoff()
{
	register u16 D0 __asm__("d0") = _BIOS_MSCSCANOFF;
	register u16 D0 __asm__("d0") = _BIOS_MSCSCANOFF;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_rompauseon
 * @brief Stops reading data into the CDC and pauses
 * @sa _BIOS_ROMPAUSEON
 * @ingroup bios_cdrom
 */
static inline void bios_rompauseon()
{
	register u16 D0 __asm__("d0") = _BIOS_ROMPAUSEON;
	register u16 D0 __asm__("d0") = _BIOS_ROMPAUSEON;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_rompauseoff
 * @brief Resumes reading data into the CDC from the current logical sector
 * @sa _BIOS_ROMPAUSEOFF
 * @ingroup bios_cdrom
 */
static inline void bios_rompauseoff()
{
	register u16 D0 __asm__("d0") = _BIOS_ROMPAUSEOFF;
	register u16 D0 __asm__("d0") = _BIOS_ROMPAUSEOFF;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_drvopen
 * @brief Opens the CD drive door
 * @ingroup sub_bios
 * @sa _BIOS_DRVOPEN
 *
 * @note This is only applicable to Mega CD Model 1 hardware.
 */
static inline void bios_drvopen()
{
	register u16 D0 __asm__("d0") = _BIOS_DRVOPEN;
	register u16 D0 __asm__("d0") = _BIOS_DRVOPEN;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

typedef struct DrvinitParams
{
	u8 toc_track_number;
	u8 last_track;
} DrvinitParams;

/**
 * @fn bios_drvinit
 * @brief Closes the disk tray and reads the TOC from the CD
 * @sa _BIOS_DRVINIT
 * @ingroup sub_bios
 *
 * @details
 * Pauses for 2 seconds after reading the TOC. Waits for a DRVOPEN request if
 * there is no disk in the drive.
 */
static inline void bios_drvinit(struct DrvinitParams const * drvinit_param)
{
	register u16 D0 __asm__("d0") = _BIOS_DRVINIT;
	register u32 a0_drvinit_param __asm__("a0") = (u32) drvinit_param;
	register u16 D0 __asm__("d0") = _BIOS_DRVINIT;
	register u32 a0_drvinit_param __asm__("a0") = (u32) drvinit_param;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(a0_drvinit_param)
		: "i"(_CDBIOS), "d"(D0), "a"(a0_drvinit_param)
		: "cc", "a1");
};

/**
 * @fn bios_mscplay
 * @brief Starts CD audio playback at the specified track & continues playing
 * through subsequent tracks
 * @sa _BIOS_MSCPLAY
 * @ingroup bios_cdda
 */
static inline void bios_mscplay(u16 const * track_number)
{
	register u16 D0 __asm__("d0") = _BIOS_MSCPLAY;
	register u32 A0 __asm__("a0") = (u32) track_number;
	register u16 D0 __asm__("d0") = _BIOS_MSCPLAY;
	register u32 A0 __asm__("a0") = (u32) track_number;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(_CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_mscplay1
 * @brief Plays the specified track once then pauses
 * @sa _BIOS_MSCPLAY1
 * @ingroup bios_cdda
 */
static inline void bios_mscplay1(u16 const * track_number)
{
	register u16 D0 __asm__("d0") = _BIOS_MSCPLAY1;
	register u32 A0 __asm__("a0") = (u32) track_number;
	register u16 D0 __asm__("d0") = _BIOS_MSCPLAY1;
	register u32 A0 __asm__("a0") = (u32) track_number;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(_CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_mscplayr
 * @brief Plays the specified track on repeat
 * @sa _BIOS_MSCPLAYR
 * @ingroup bios_cdda
 */
static inline void bios_mscplayr(u16 const * track_number)
{
	register u16 D0 __asm__("d0") = _BIOS_MSCPLAYR;
	register u32 A0 __asm__("a0") = (u32) track_number;
	register u16 D0 __asm__("d0") = _BIOS_MSCPLAYR;
	register u32 A0 __asm__("a0") = (u32) track_number;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(_CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_mscplayt
 * @brief Starts playing from the specified timecode
 * @sa _BIOS_MSCPLAYT
 * @ingroup bios_cdda
 */
static inline void bios_mscplayt(u32 const * timecode)
{
	register u16 D0 __asm__("d0") = _BIOS_MSCPLAYT;
	register u32 A0 __asm__("a0") = (u32) timecode;
	register u16 D0 __asm__("d0") = _BIOS_MSCPLAYT;
	register u32 A0 __asm__("a0") = (u32) timecode;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(_CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_mscseek
 * @brief Seeks to the beginning of the specified track and pauses
 * @sa _BIOS_MSCSEEK
 * @ingroup bios_cdda
 */
static inline void bios_mscseek(u16 const * track_number)
{
	register u16 D0 __asm__("d0") = _BIOS_MSCSEEK;
	register u32 A0 __asm__("a0") = (u32) track_number;
	register u16 D0 __asm__("d0") = _BIOS_MSCSEEK;
	register u32 A0 __asm__("a0") = (u32) track_number;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(_CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_mscseekt
 * @brief Seeks to a specified timecode
 * @sa _BIOS_MSCSEEKT
 * @ingroup bios_cdda
 */
static inline void bios_mscseekt(u32 const * timecode)
{
	register u16 D0 __asm__("d0") = _BIOS_MSCSEEKT;
	register u32 A0 __asm__("a0") = (u32) timecode;
	register u16 D0 __asm__("d0") = _BIOS_MSCSEEKT;
	register u32 A0 __asm__("a0") = (u32) timecode;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(_CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_romread
 * @brief Begins reading data from the CDROM at the designated logical sector
 * @sa _BIOS_ROMREAD
 * @ingroup bios_cdrom
 *
 * @details
 * Executes a CDCSTART to begin the read, but does not stop
 * automatically.
 *
 * @note
 * ROMREAD actually pre-seeks by 2 sectors, but doesn't start passing data
 * to the CDC until the desired sector is reached.
 */
static inline void bios_romread(u32 const * sector_number)
{
	register u16 D0 __asm__("d0") = _BIOS_ROMREAD;
	register u32 A0 __asm__("a0") = (u32) sector_number;
	register u16 D0 __asm__("d0") = _BIOS_ROMREAD;
	register u32 A0 __asm__("a0") = (u32) sector_number;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(_CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def bios_romseek
 * @brief Seeks to the designated logical sector and pauses
 * @sa _BIOS_ROMSEEK
 * @ingroup bios_cdrom
 */
static inline void bios_romseek(u32 const * sector_number)
{
	register u16 D0 __asm__("d0") = _BIOS_ROMSEEK;
	register u32 A0 __asm__("a0") = (u32) sector_number;
	register u16 D0 __asm__("d0") = _BIOS_ROMSEEK;
	register u32 A0 __asm__("a0") = (u32) sector_number;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(_CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def bios_mscseek1
 * @brief Seek to the beginning of the selected track and pause; when BIOS
 * detects a pause state, the track is played once
 * @sa _BIOS_MSCSEEK1
 * @ingroup bios_cdda
 */
static inline void bios_mscseek1(u16 const * track_number)
{
	register u16 D0 __asm__("d0") = _BIOS_MSCSEEK1;
	register u32 A0 __asm__("a0") = (u32) track_number;
	register u16 D0 __asm__("d0") = _BIOS_MSCSEEK1;
	register u32 A0 __asm__("a0") = (u32) track_number;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(_CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

struct RomreadParams
{
	u32 start_sector;
	u32 sector_count;
};

/**
 * @def _BIOS_ROMREADN
 * @brief Same function as @ref bios_romread but stops after reading the
 * requested number of sectors
 * @ingroup bios_cdrom
 *

 */
static inline void bios_romreadn(struct RomreadParams const * param)
{
	register u16 D0 __asm__("d0") = _BIOS_ROMREADN;
	register u32 A0 __asm__("a0") = (u32) param;
	register u16 D0 __asm__("d0") = _BIOS_ROMREADN;
	register u32 A0 __asm__("a0") = (u32) param;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(_CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def _BIOS_ROMREADE
 * @ingroup bios_cdrom
 * @brief Same as @ref bios_romread, but reads between two logical sectors
 */
static inline void bios_romreade(struct RomreadParams const * param)
{
	register u16 D0 __asm__("d0") = _BIOS_ROMREADE;
	register u32 A0 __asm__("a0") = (u32) param;
	register u16 D0 __asm__("d0") = _BIOS_ROMREADE;
	register u32 A0 __asm__("a0") = (u32) param;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(_CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def bios_cdbchk
 * @brief Query the BIOS on the status of the last command
 * @sa _BIOS_CDBCHK
 */
static inline void bios_cdbchk()
{
	register u16 D0 __asm__("d0") = _BIOS_CDBCHK;
	register u16 D0 __asm__("d0") = _BIOS_CDBCHK;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc");
};

struct cdbstat
{
	u16 bios_status;
	u16 led;
	u8 cdd_status[20];
	u32 volume;
	u32 header;
};

/**
 * @def _BIOS_CDBSTAT
 * @brief Retrieve CD BIOS status
 *
 * Please refer to the BIOS manual for details about the returned data
 */
static inline struct cdbstat const * bios_cdbstat()
{
	register u16 D0 __asm__("d0") = _BIOS_CDBSTAT;
	register u32 A0 __asm__("a0");
	register u16 D0 __asm__("d0") = _BIOS_CDBSTAT;
	register u32 A0 __asm__("a0");

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p2"
		: "+d"(D0), "=a"(A0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "d1");

	return (struct cdbstat *) A0;
}

/**
 * @def _BIOS_FDRSET
 * @brief Sets the audio volume
 * @sa _BIOS_FDRSET
 * @ingroup bios_fader
 *
 * @details Setting the upper bit of the value changes the master volume
 *   16 bit volume         (0x0000 to 0x0400)
 *   16 bit master volume  (0x8000 to 0x8400)
 *
 * @note The master volume sets a maximum level which the volume level
 * canot exceed.
 *
 * @note There is a delay of up to 13ms before the volume begins to change
 * and another 23ms for the new volume level to take effect.
 */
static inline void bios_fdrset(u16 const volume)
{
	register u16 D0 __asm__("d0") = _BIOS_FDRSET;
	register u16 const D1 __asm__("d1") = volume;
	register u16 D0 __asm__("d0") = _BIOS_FDRSET;
	register u16 const D1 __asm__("d1") = volume;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0), "d"(D1)
		: "cc");
};

/**
 * @def bios_fdrchg
 * @brief  Ramps the audio volume from its current level to a new level at
 * the requested rate
 * @sa _BIOS_FDRCHG
 * @ingroup bios_fader

 * @details The input is two 16 bit values
 *   High word: new volume (min 0x0000, max 0x0400)
 *    Low word: change rate in steps per vblank
 *        0x0001 - slow
 *        0x0200 - fast
 *        0x0400 - immediate
 *
 * @note There is a delay of up to 13ms before the volume begins to change.
 */
static inline void bios_fdrchg(u32 const volume)
{
	register u16 D0 __asm__("d0") = _BIOS_FDRCHG;
	register u32 D1 __asm__("d1") = volume;
	register u16 D0 __asm__("d0") = _BIOS_FDRCHG;
	register u32 D1 __asm__("d1") = volume;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0), "d"(D1)
		: "cc");
};

/**
 * @sa _BIOS_CDCSTART
 */
static inline void bios_cdcstart()
{
	register u16 D0 __asm__("d0") = _BIOS_CDCSTART;
	register u16 D0 __asm__("d0") = _BIOS_CDCSTART;

	__asm__ volatile(
	__asm__ volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "d1");
};

/**
 * @sa _BIOS_CDCSTOP
 */
static inline void bios_cdcstop()
{
	register u16 D0 __asm__("d0") = _BIOS_CDCSTOP;
	register u16 D0 __asm__("d0") = _BIOS_CDCSTOP;

	__asm__ volatile(
	__asm__ volatile(
		"\
jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc");
};

/**
 * @fn bios_cdcstat
 * @sa _BIOS_CDCSTAT
 */
static inline void bios_cdcstat()
{
	register u16 D0 __asm__("d0") = _BIOS_CDCSTAT;
	register u16 D0 __asm__("d0") = _BIOS_CDCSTAT;

	__asm__ volatile(
	__asm__ volatile(
		"\
jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc", "d1");
};

/**
 * @sa _BIOS_CDCREAD
 */
// headaches with asm goto and output operands... we'll come back to this later.
/*
static inline bool bios_cdcread() {
	register u16 D0 __asm__("d0") = _BIOS_CDCREAD;

				asm goto(R"(jsr %p1)"
																				 : "+d"(D0)
																				 : "i"(_CDBIOS), "d"(D0)
																				 : "cc", "d1");

				return false;

sector_ready:
				return true;
};
*/

/**
 * @sa _BIOS_CDCTRN
 */
static inline void bios_cdctrn(u8 * sector_dest, u8 * header_dest)
{
	register u16 D0 __asm__("d0") = _BIOS_CDCTRN;
	register u8 * a0_sector_dest __asm__("a0") = sector_dest;
	register u8 * a1_header_dest __asm__("a1") = header_dest;
	register u16 D0 __asm__("d0") = _BIOS_CDCTRN;
	register u8 * a0_sector_dest __asm__("a0") = sector_dest;
	register u8 * a1_header_dest __asm__("a1") = header_dest;

	__asm__ volatile(
	__asm__ volatile(
		"\
jsr %p3"
		: "+d"(D0), "+a"(a0_sector_dest), "+a"(a1_header_dest)
		: "i"(_CDBIOS), "d"(D0), "a"(a0_sector_dest), "a"(a1_header_dest)
		: "cc", "d1");
};

/**
 * @sa _BIOS_CDCACK
 */
static inline void bios_cdcack()
{
	register u16 D0 __asm__("d0") = _BIOS_CDCACK;
	register u16 D0 __asm__("d0") = _BIOS_CDCACK;

	__asm__ volatile(
	__asm__ volatile(
		"\
jsr %p1"
		: "+d"(D0)
		: "i"(_CDBIOS), "d"(D0)
		: "cc");
};

#endif
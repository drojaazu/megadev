/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub/bios.h
 * @brief Sub CPU side system library
 */

#ifndef MEGADEV__SUBBIOS_H
#define MEGADEV__SUBBIOS_H

#include "sub/bios.def.h"
#include "sub/memmap.def.h"
#include "types.h"

static inline void bios_waitvsync()
{
	asm volatile(
		"\
		jsr %p0"
		:
		: "i"(WAITVSYNC)
		: "cc");
}

/**
 * @fn bios_mscstop
 * @brief Stops playing CD audio if it is playing
 * @sa BIOS_MSCSTOP
 * @ingroup bios_cdda
 */
static inline void bios_mscstop()
{
	register u16 D0 asm("d0") = BIOS_MSCSTOP;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
}

/**
 * @fn bios_mscpauseon
 * @brief Pauses the drive when a track is playing
 * @sa BIOS_MSCPAUSEON
 * @ingroup bios_cdda
 *
 * @note If the drive is left paused it will stop after a programmable delay
 * (see @ref _CDBPAUSE)
 */
static inline void bios_mscpauseon()
{
	register u16 D0 asm("d0") = BIOS_MSCPAUSEON;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
}

/**
 * @fn bios_mscpauseoff
 * @brief Resumes playing a track after a pause
 * @sa BIOS_MSCPAUSEOFF
 * @ingroup bios_cdda
 *
 * @note If the drive has timed out and stopped, the BIOS will seek to the pause
 * time (with the attendant delay) and resume playing
 */
static inline void bios_mscpauseoff()
{
	register u16 D0 asm("d0") = BIOS_MSCPAUSEOFF;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_mscscanff
 * @brief Starts playing from the current position in fast forward
 * @sa BIOS_MSCSCANFF
 * @ingroup bios_cdda
 */
static inline void bios_mscscanff()
{
	register u16 D0 asm("d0") = BIOS_MSCSCANFF;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_mscscanfr
 * @brief Starts playing from the current position in fast reverse
 * @sa BIOS_MSCSCANFR
 * @ingroup bios_cdda
 */
static inline void bios_mscscanfr()
{
	register u16 D0 asm("d0") = BIOS_MSCSCANFR;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_mscscanoff
 * @brief Returns to normal play mode
 * @sa BIOS_MSCSCANOFF
 * @ingroup bios_cdda
 *
 * @note If the drive was paused before the scan was initiated, it will be
 * returned to pause.
 */
static inline void bios_mscscanoff()
{
	register u16 D0 asm("d0") = BIOS_MSCSCANOFF;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_rompauseon
 * @brief Stops reading data into the CDC and pauses
 * @sa BIOS_ROMPAUSEON
 * @ingroup bios_cdrom
 */
static inline void bios_rompauseon()
{
	register u16 D0 asm("d0") = BIOS_ROMPAUSEON;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_rompauseoff
 * @brief Resumes reading data into the CDC from the current logical sector
 * @sa BIOS_ROMPAUSEOFF
 * @ingroup bios_cdrom
 */
static inline void bios_rompauseoff()
{
	register u16 D0 asm("d0") = BIOS_ROMPAUSEOFF;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_drvopen
 * @brief Opens the CD drive door
 * @ingroup sub_bios
 * @sa BIOS_DRVOPEN
 *
 * @note This is only applicable to Mega CD Model 1 hardware.
 */
static inline void bios_drvopen()
{
	register u16 D0 asm("d0") = BIOS_DRVOPEN;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
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
 * @sa BIOS_DRVINIT
 * @ingroup sub_bios
 *
 * @details
 * Pauses for 2 seconds after reading the TOC. Waits for a DRVOPEN request if
 * there is no disk in the drive.
 */
static inline void bios_drvinit(struct DrvinitParams const * drvinit_param)
{
	register u16 D0 asm("d0") = BIOS_DRVINIT;
	register u32 a0_drvinit_param asm("a0") = (u32) drvinit_param;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(a0_drvinit_param)
		: "i"(CDBIOS), "d"(D0), "a"(a0_drvinit_param)
		: "cc", "a1");
};

/**
 * @fn bios_mscplay
 * @brief Starts CD audio playback at the specified track & continues playing
 * through subsequent tracks
 * @sa BIOS_MSCPLAY
 * @ingroup bios_cdda
 */
static inline void bios_mscplay(u16 const * track_number)
{
	register u16 D0 asm("d0") = BIOS_MSCPLAY;
	register u32 A0 asm("a0") = (u32) track_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_mscplay1
 * @brief Plays the specified track once then pauses
 * @sa BIOS_MSCPLAY1
 * @ingroup bios_cdda
 */
static inline void bios_mscplay1(u16 const * track_number)
{
	register u16 D0 asm("d0") = BIOS_MSCPLAY1;
	register u32 A0 asm("a0") = (u32) track_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_mscplayr
 * @brief Plays the specified track on repeat
 * @sa BIOS_MSCPLAYR
 * @ingroup bios_cdda
 */
static inline void bios_mscplayr(u16 const * track_number)
{
	register u16 D0 asm("d0") = BIOS_MSCPLAYR;
	register u32 A0 asm("a0") = (u32) track_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_mscplayt
 * @brief Starts playing from the specified timecode
 * @sa BIOS_MSCPLAYT
 * @ingroup bios_cdda
 */
static inline void bios_mscplayt(u32 const * timecode)
{
	register u16 D0 asm("d0") = BIOS_MSCPLAYT;
	register u32 A0 asm("a0") = (u32) timecode;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_mscseek
 * @brief Seeks to the beginning of the specified track and pauses
 * @sa BIOS_MSCSEEK
 * @ingroup bios_cdda
 */
static inline void bios_mscseek(u16 const * track_number)
{
	register u16 D0 asm("d0") = BIOS_MSCSEEK;
	register u32 A0 asm("a0") = (u32) track_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_mscseekt
 * @brief Seeks to a specified timecode
 * @sa BIOS_MSCSEEKT
 * @ingroup bios_cdda
 */
static inline void bios_mscseekt(u32 const * timecode)
{
	register u16 D0 asm("d0") = BIOS_MSCSEEKT;
	register u32 A0 asm("a0") = (u32) timecode;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_romread
 * @brief Begins reading data from the CDROM at the designated logical sector
 * @sa BIOS_ROMREAD
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
	register u16 D0 asm("d0") = BIOS_ROMREAD;
	register u32 A0 asm("a0") = (u32) sector_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def bios_romseek
 * @brief Seeks to the designated logical sector and pauses
 * @sa BIOS_ROMSEEK
 * @ingroup bios_cdrom
 */
static inline void bios_romseek(u32 const * sector_number)
{
	register u16 D0 asm("d0") = BIOS_ROMSEEK;
	register u32 A0 asm("a0") = (u32) sector_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def bios_mscseek1
 * @brief Seek to the beginning of the selected track and pause; when BIOS
 * detects a pause state, the track is played once
 * @sa BIOS_MSCSEEK1
 * @ingroup bios_cdda
 */
static inline void bios_mscseek1(u16 const * track_number)
{
	register u16 D0 asm("d0") = BIOS_MSCSEEK1;
	register u32 A0 asm("a0") = (u32) track_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

struct RomreadParams
{
	u32 start_sector;
	u32 sector_count;
};

/**
 * @def BIOS_ROMREADN
 * @brief Same function as @ref bios_romread but stops after reading the
 * requested number of sectors
 * @ingroup bios_cdrom
 *

 */
static inline void bios_romreadn(struct RomreadParams const * param)
{
	register u16 D0 asm("d0") = BIOS_ROMREADN;
	register u32 A0 asm("a0") = (u32) param;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def BIOS_ROMREADE
 * @ingroup bios_cdrom
 * @brief Same as @ref bios_romread, but reads between two logical sectors
 */
static inline void bios_romreade(struct RomreadParams const * param)
{
	register u16 D0 asm("d0") = BIOS_ROMREADE;
	register u32 A0 asm("a0") = (u32) param;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def bios_cdbchk
 * @brief Query the BIOS on the status of the last command
 * @sa BIOS_CDBCHK
 *
 * @param[out] CC On clear, command is complete
 */
static inline bool bios_cdbchk()
{
	register u16 D0 asm("d0") = BIOS_CDBCHK;

	asm goto(
		"\
		jsr %p0 \n\
		bcc %l[cmd_complete] \n\
		"
		:
		: "i"(CDBIOS), "d"(D0)
		: "cc"
		: cmd_complete);

	return false;

cmd_complete:
	return true;
};

struct cdbstat
{
	u16 bios_status;
	u16 led;
	u8	cdd_status[20];
	u32 volume;
	u32 header;
};

/**
 * @def BIOS_CDBSTAT
 * @brief Retrieve CD BIOS status
 *
 * Please refer to the BIOS manual for details about the returned data
 */
static inline struct cdbstat const * bios_cdbstat()
{
	register u16 D0 asm("d0") = BIOS_CDBSTAT;
	register u32 A0 asm("a0");

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "=a"(A0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "d1");

	return (struct cdbstat *) A0;
}

/**
 * @def BIOS_FDRSET
 * @brief Sets the audio volume
 * @sa BIOS_FDRSET
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
	register u16			 D0 asm("d0") = BIOS_FDRSET;
	register u16 const D1 asm("d1") = volume;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0), "d"(D1)
		: "cc");
};

/**
 * @def bios_fdrchg
 * @brief  Ramps the audio volume from its current level to a new level at
 * the requested rate
 * @sa BIOS_FDRCHG
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
	register u16 D0 asm("d0") = BIOS_FDRCHG;
	register u32 D1 asm("d1") = volume;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0), "d"(D1)
		: "cc");
};

/**
 * @sa BIOS_CDCSTART
 */
static inline void bios_cdcstart()
{
	register u16 D0 asm("d0") = BIOS_CDCSTART;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "d1");
};

/**
 * @sa BIOS_CDCSTOP
 */
static inline void bios_cdcstop()
{
	register u16 D0 asm("d0") = BIOS_CDCSTOP;

	asm volatile(
		"\
jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc");
};

/**
 * @fn bios_cdcstat
 * @sa BIOS_CDCSTAT
 */
static inline void bios_cdcstat()
{
	register u16 D0 asm("d0") = BIOS_CDCSTAT;

	asm volatile(
		"\
jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "d1");
};

/**
 * @sa BIOS_CDCREAD
 */
// headaches with asm goto and output operands... we'll come back to this later.
/*
static inline bool bios_cdcread() {
	register u16 D0 asm("d0") = BIOS_CDCREAD;

				asm goto(R"(jsr %p1)"
																				 : "+d"(D0)
																				 : "i"(CDBIOS), "d"(D0)
																				 : "cc", "d1");

				return false;

sector_ready:
				return true;
};
*/

/**
 * @sa BIOS_CDCTRN
 */
static inline void bios_cdctrn(u8 * sector_dest, u8 * header_dest)
{
	register u16	D0 asm("d0") = BIOS_CDCTRN;
	register u8 * a0_sector_dest asm("a0") = sector_dest;
	register u8 * a1_header_dest asm("a1") = header_dest;

	asm volatile(
		"\
jsr %p3"
		: "+d"(D0), "+a"(a0_sector_dest), "+a"(a1_header_dest)
		: "i"(CDBIOS), "d"(D0), "a"(a0_sector_dest), "a"(a1_header_dest)
		: "cc", "d1");
};

/**
 * @sa BIOS_CDCACK
 */
static inline void bios_cdcack()
{
	register u16 D0 asm("d0") = BIOS_CDCACK;

	asm volatile(
		"\
jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc");
};

#endif
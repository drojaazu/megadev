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
 * @fn bios_msc_stop
 * @brief Stops playing CD audio if it is playing
 * @sa BIOS_MSC_STOP
 * @ingroup bios_cdda
 */
static inline void bios_msc_stop()
{
	register u16 D0 asm("d0") = BIOS_MSC_STOP;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
}

/**
 * @fn bios_msc_pauseon
 * @brief Pauses the drive when a track is playing
 * @sa BIOS_MSC_PAUSEON
 * @ingroup bios_cdda
 *
 * @note If the drive is left paused it will stop after a programmable delay
 * (see @ref _CDBPAUSE)
 */
static inline void bios_msc_pauseon()
{
	register u16 D0 asm("d0") = BIOS_MSC_PAUSEON;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
}

/**
 * @fn bios_msc_pauseoff
 * @brief Resumes playing a track after a pause
 * @sa BIOS_MSC_PAUSEOFF
 * @ingroup bios_cdda
 *
 * @note If the drive has timed out and stopped, the BIOS will seek to the pause
 * time (with the attendant delay) and resume playing
 */
static inline void bios_msc_pauseoff()
{
	register u16 D0 asm("d0") = BIOS_MSC_PAUSEOFF;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_msc_scanff
 * @brief Starts playing from the current position in fast forward
 * @sa BIOS_MSC_SCANFF
 * @ingroup bios_cdda
 */
static inline void bios_msc_scanff()
{
	register u16 D0 asm("d0") = BIOS_MSC_SCANFF;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_msc_scanfr
 * @brief Starts playing from the current position in fast reverse
 * @sa BIOS_MSC_SCANFR
 * @ingroup bios_cdda
 */
static inline void bios_msc_scanfr()
{
	register u16 D0 asm("d0") = BIOS_MSC_SCANFR;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_msc_scanoff
 * @brief Returns to normal play mode
 * @sa BIOS_MSC_SCANOFF
 * @ingroup bios_cdda
 *
 * @note If the drive was paused before the scan was initiated, it will be
 * returned to pause.
 */
static inline void bios_msc_scanoff()
{
	register u16 D0 asm("d0") = BIOS_MSC_SCANOFF;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_msc_play
 * @brief Starts CD audio playback at the specified track & continues playing
 * through subsequent tracks
 * @sa BIOS_MSC_PLAY
 * @ingroup bios_cdda
 */
static inline void bios_msc_play(u16 const * track_number)
{
	register u16 D0 asm("d0") = BIOS_MSC_PLAY;
	register u32 A0 asm("a0") = (u32) track_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_msc_play1
 * @brief Plays the specified track once then pauses
 * @sa BIOS_MSC_PLAY1
 * @ingroup bios_cdda
 */
static inline void bios_msc_play1(u16 const * track_number)
{
	register u16 D0 asm("d0") = BIOS_MSC_PLAY1;
	register u32 A0 asm("a0") = (u32) track_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_msc_playr
 * @brief Plays the specified track on repeat
 * @sa BIOS_MSC_PLAYR
 * @ingroup bios_cdda
 */
static inline void bios_msc_playr(u16 const * track_number)
{
	register u16 D0 asm("d0") = BIOS_MSC_PLAYR;
	register u32 A0 asm("a0") = (u32) track_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_msc_playt
 * @brief Starts playing from the specified timecode
 * @sa BIOS_MSC_PLAYT
 * @ingroup bios_cdda
 */
static inline void bios_msc_playt(u32 const * timecode)
{
	register u16 D0 asm("d0") = BIOS_MSC_PLAYT;
	register u32 A0 asm("a0") = (u32) timecode;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_msc_seek
 * @brief Seeks to the beginning of the specified track and pauses
 * @sa BIOS_MSC_SEEK
 * @ingroup bios_cdda
 */
static inline void bios_msc_seek(u16 const * track_number)
{
	register u16 D0 asm("d0") = BIOS_MSC_SEEK;
	register u32 A0 asm("a0") = (u32) track_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_msc_seekt
 * @brief Seeks to a specified timecode
 * @sa BIOS_MSC_SEEKT
 * @ingroup bios_cdda
 */
static inline void bios_msc_seekt(u32 const * timecode)
{
	register u16 D0 asm("d0") = BIOS_MSC_SEEKT;
	register u32 A0 asm("a0") = (u32) timecode;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def bios_msc_seek1
 * @brief Seek to the beginning of the selected track and pause; when BIOS
 * detects a pause state, the track is played once
 * @sa BIOS_MSC_SEEK1
 * @ingroup bios_cdda
 */
static inline void bios_msc_seek1(u16 const * track_number)
{
	register u16 D0 asm("d0") = BIOS_MSC_SEEK1;
	register u32 A0 asm("a0") = (u32) track_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @fn bios_drv_open
 * @brief Opens the CD drive door
 * @ingroup sub_bios
 * @sa BIOS_DRV_OPEN
 *
 * @note This is only applicable to Mega CD Model 1 hardware.
 */
static inline void bios_drv_open()
{
	register u16 D0 asm("d0") = BIOS_DRV_OPEN;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

typedef struct DriveInitParams
{
	u8 toc_track_number;
	u8 last_track;
} DriveInitParams;

/**
 * @fn bios_drv_init
 * @brief Closes the disk tray and reads the TOC from the CD
 * @sa BIOS_DRV_INIT
 * @ingroup sub_bios
 *
 * @details
 * Pauses for 2 seconds after reading the TOC. Waits for a DRV_OPEN request if
 * there is no disk in the drive.
 */
static inline void
bios_drv_init(struct DriveInitParams const * drive_init_params)
{
	register u16 D0 asm("d0") = BIOS_DRV_INIT;
	register u32 a0_drv_init_param asm("a0") = (u32) drive_init_params;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(a0_drv_init_param)
		: "i"(CDBIOS), "d"(D0), "a"(a0_drv_init_param)
		: "cc", "a1");
};

/**
 * @fn bios_rom_pauseon
 * @brief Stops reading data into the CDC and pauses
 * @sa BIOS_ROM_PAUSEON
 * @ingroup bios_cdrom
 */
static inline void bios_rom_pauseon()
{
	register u16 D0 asm("d0") = BIOS_ROM_PAUSEON;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_rom_pauseoff
 * @brief Resumes reading data into the CDC from the current logical sector
 * @sa BIOS_ROM_PAUSEOFF
 * @ingroup bios_cdrom
 */
static inline void bios_rom_pauseoff()
{
	register u16 D0 asm("d0") = BIOS_ROM_PAUSEOFF;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "a0", "a1");
};

/**
 * @fn bios_rom_read
 * @brief Begins reading data from the CDROM at the designated logical sector
 * @sa BIOS_ROM_READ
 * @ingroup bios_cdrom
 *
 * @details
 * Executes a CDC_START to begin the read, but does not stop
 * automatically.
 *
 * @note
 * ROM_READ actually pre-seeks by 2 sectors, but doesn't start passing data
 * to the CDC until the desired sector is reached.
 */
static inline void bios_rom_read(u32 const * sector_number)
{
	register u16 D0 asm("d0") = BIOS_ROM_READ;
	register u32 A0 asm("a0") = (u32) sector_number;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def bios_rom_seek
 * @brief Seeks to the designated logical sector and pauses
 * @sa BIOS_ROM_SEEK
 * @ingroup bios_cdrom
 */
static inline void bios_rom_seek(u32 const * sector_number)
{
	register u16 D0 asm("d0") = BIOS_ROM_SEEK;
	register u32 A0 asm("a0") = (u32) sector_number;

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
 * @def BIOS_ROM_READN
 * @brief Same function as @ref bios_rom_read but stops after reading the
 * requested number of sectors
 * @ingroup bios_cdrom
 *

 */
static inline void bios_rom_readn(struct RomreadParams const * param)
{
	register u16 D0 asm("d0") = BIOS_ROM_READN;
	register u32 A0 asm("a0") = (u32) param;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def BIOS_ROM_READE
 * @ingroup bios_cdrom
 * @brief Same as @ref bios_rom_read, but reads between two logical sectors
 */
static inline void bios_rom_reade(struct RomreadParams const * param)
{
	register u16 D0 asm("d0") = BIOS_ROM_READE;
	register u32 A0 asm("a0") = (u32) param;

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "+a"(A0)
		: "i"(CDBIOS), "d"(D0), "a"(A0)
		: "cc", "a1");
};

/**
 * @def bios_cdb_chk
 * @brief Query the BIOS on the status of the last command
 * @sa BIOS_CDB_CHK
 *
 * @param[out] CC On clear, command is complete
 */
static inline bool bios_cdb_chk()
{
	register u16 D0 asm("d0") = BIOS_CDB_CHK;

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

struct BiosStatus
{
	u16 bios_status;
	u16 led;
	u8	cdd_status[20];
	u32 volume;
	u32 header;
};

/**
 * @def BIOS_CDB_STAT
 * @brief Retrieve CD BIOS status
 *
 * Please refer to the BIOS manual for details about the returned data
 */
static inline struct BiosStatus const * bios_cdb_stat()
{
	register u16 D0 asm("d0") = BIOS_CDB_STAT;
	register u32 A0 asm("a0");

	asm volatile(
		"\
		jsr %p2"
		: "+d"(D0), "=a"(A0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "d1");

	return (struct BiosStatus *) A0;
}

/**
 * @def BIOS_FDR_SET
 * @brief Sets the audio volume
 * @sa BIOS_FDR_SET
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
static inline void bios_fdr_set(u16 const volume)
{
	register u16			 D0 asm("d0") = BIOS_FDR_SET;
	register u16 const D1 asm("d1") = volume;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0), "d"(D1)
		: "cc");
};

/**
 * @def bios_fdr_chg
 * @brief  Ramps the audio volume from its current level to a new level at
 * the requested rate
 * @sa BIOS_FDR_CHG
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
static inline void bios_fdr_chg(u32 const volume)
{
	register u16 D0 asm("d0") = BIOS_FDR_CHG;
	register u32 D1 asm("d1") = volume;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0), "d"(D1)
		: "cc");
};

/**
 * @sa BIOS_CDC_START
 */
static inline void bios_cdc_start()
{
	register u16 D0 asm("d0") = BIOS_CDC_START;

	asm volatile(
		"\
		jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc", "d1");
};

/**
 * @sa BIOS_CDC_STOP
 */
static inline void bios_cdc_stop()
{
	register u16 D0 asm("d0") = BIOS_CDC_STOP;

	asm volatile(
		"\
jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc");
};

/**
 * @fn bios_cdc_stat
 * @sa BIOS_CDC_STAT
 */
static inline void bios_cdc_stat()
{
	register u16 D0 asm("d0") = BIOS_CDC_STAT;

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
 * @sa BIOS_CDC_TRN
 */
static inline void bios_cdc_trn(u8 * sector_dest, u8 * header_dest)
{
	register u16	D0 asm("d0") = BIOS_CDC_TRN;
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
 * @sa BIOS_CDC_ACK
 */
static inline void bios_cdc_ack()
{
	register u16 D0 asm("d0") = BIOS_CDC_ACK;

	asm volatile(
		"\
jsr %p1"
		: "+d"(D0)
		: "i"(CDBIOS), "d"(D0)
		: "cc");
};

#endif
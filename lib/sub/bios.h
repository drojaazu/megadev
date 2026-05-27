/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub/bios.h
 * @brief Sub CPU side system library
 */

#pragma once

#include "sub/bios.def.h"
#include "sub/memmap.def.h"
#include "types.h"

/**
 * @fn bios_drive_open
 * @brief Opens the CD drive door
 * @sa BIOS_DRIVE_OPEN
 * @ingroup sub_bios_misc
 *
 * @note
 * This is only applicable to Mega CD Model 1 hardware.
 */
static inline void bios_drive_open()
{
  register u16 _BIOS_DRIVE_OPEN asm("d0") = BIOS_DRIVE_OPEN;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(_BIOS_DRIVE_OPEN)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_DRIVE_OPEN)
    : "cc", "a0", "a1");
};

/**
 * @def DriveInitParams
 * @brief Specifies the TOC track number and the last track to read from the TOC
 *
 * @details
 * This is for use with the BIOS_DRIVE_INIT call. Unless you have done some CD
 * mastering wizardry, the TOC should be at track 1. Set last_track to 255
 * (0xFF) to read in all tracks.
 *
 * In practically all cases, the values should be 1 and 255.
 */
typedef struct DriveInitParams
{
  u8 toc_track_number;
  u8 last_track;
} DriveInitParams;

/**
 * @fn bios_drive_init_ex
 * @brief Reads the TOC from the disc with track range parameters
 * @sa BIOS_DRIVE_INIT
 * @ingroup sub_bios_misc
 *
 * @details
 * Pauses for 2 seconds after reading the TOC. Waits for a BIOS_DRIVE_OPEN
 * request if there is no disk in the drive. Also closes the drive door for
 * Model 1 hardware
 *
 * @note
 * Consider using @ref bios_drive_init instead, which uses the standard settings
 */
static inline void
bios_drive_init_ex(struct DriveInitParams const * drive_init_params)
{

  register u16 _BIOS_DRIVE_INIT asm("d0") = BIOS_DRIVE_INIT;
  register u32 _drive_init_params asm("a0") = (u32) drive_init_params;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_DRIVE_INIT), "+a"(_drive_init_params)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_DRIVE_INIT), "a"(_drive_init_params)
    : "cc", "a1");
};

/**
 * @fn bios_drive_init
 * @brief Reads the TOC from the disc using standard settings
 * @sa BIOS_DRIVE_INIT
 * @ingroup sub_bios_misc
 *
 * @details
 * Pauses for 2 seconds after reading the TOC. Waits for a BIOS_DRIVE_OPEN
 * request if there is no disk in the drive. Also closes the drive door for
 * Model 1 hardware
 */
static inline void bios_drive_init()
{
  static DriveInitParams const std_drive_init = {0x01, 0xff};
  bios_drive_init(&std_drive_init);
}

/**
 * @fn bios_call_is_complete
 * @brief Query the completion status of the last command
 * @sa BIOS_CHECK_STATUS
 * @ingroup sub_bios_misc
 *
 * @param[out] CC On clear, command is complete
 *
 * @note Use bios_wait_for_command() for bios call completion checks as it is
 * better optimized.
 */
static inline bool bios_call_is_complete()
{
  register u16 D0 asm("d0") = BIOS_CHECK_STATUS;

  asm goto(
    "\
		jsr %p0 \n\
		bcc %l[cmd_complete] \n\
		"
    :
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc"
    : cmd_complete);

  return false;

cmd_complete:
  return true;
};

/**
 * @fn bios_wait_for_call_complete
 * @brief Wait for the last BIOS call to complete
 * @sa BIOS_CHECK_STATUS
 * @ingroup sub_bios_misc
 *
 */
static inline void bios_wait_for_call_complete()
{
  asm(
    "\
    1:move.w #%p1, d0 \n\
		jsr %p0 \n\
		bcs 1b \n\
		"
    :
    : "i"(BIOS_CALL_VECTOR), "i"(BIOS_CHECK_STATUS)
    : "cc", "d0");
}

struct BiosStatus
{
  u16 bios_status;
  u16 led;
  u8  cdd_status[20];
  u32 volume;
  u32 header;
};

/**
 * @fn bios_get_status
 * @brief Retrieve BIOS status details
 * @sa BIOS_GET_STATUS
 * @ingroup sub_bios_misc
 *
 * Please refer to the BIOS manual for details about the returned data
 */
static inline struct BiosStatus const * bios_get_status()
{
  register u16 D0 asm("d0") = BIOS_GET_STATUS;
  register u32 A0 asm("a0");

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "=a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "d1");

  return (struct BiosStatus *) A0;
}

/**
 * @fn bios_audio_play
 * @brief Starts CD audio playback at the specified track & continues playing
 * through subsequent tracks
 * @sa BIOS_AUDIO_PLAY
 * @ingroup sub_bios_cdda
 */
static inline void bios_audio_play(u16 const * track_number)
{
  register u16 _BIOS_AUDIO_PLAY asm("d0") = BIOS_AUDIO_PLAY;
  register u32 _track_number asm("a0") = (u32) track_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_AUDIO_PLAY), "+a"(_track_number)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_PLAY), "a"(_track_number)
    : "cc", "a1");
};

/**
 * @fn bios_audio_play_once
 * @brief Plays the specified track once then pauses
 * @sa BIOS_AUDIO_PLAY_ONCE
 * @ingroup sub_bios_cdda
 */
static inline void bios_audio_play_once(u16 const * track_number)
{
  register u16 _BIOS_AUDIO_PLAY_ONCE asm("d0") = BIOS_AUDIO_PLAY_ONCE;
  register u32 _track_number asm("a0") = (u32) track_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_AUDIO_PLAY_ONCE), "+a"(_track_number)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_PLAY_ONCE), "a"(_track_number)
    : "cc", "a1");
};

/**
 * @fn bios_audio_play_repeat
 * @brief Plays the specified track on repeat
 * @sa BIOS_AUDIO_PLAY_REPEAT
 * @ingroup sub_bios_cdda
 */
static inline void bios_audio_play_repeat(u16 const * track_number)
{
  register u16 _BIOS_AUDIO_PLAY_REPEAT asm("d0") = BIOS_AUDIO_PLAY_REPEAT;
  register u32 _track_number asm("a0") = (u32) track_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_AUDIO_PLAY_REPEAT), "+a"(_track_number)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_PLAY_REPEAT), "a"(_track_number)
    : "cc", "a1");
};

/**
 * @fn bios_audio_play_from
 * @brief Starts playing from the specified timecode
 * @sa BIOS_AUDIO_PLAY_FROM
 * @ingroup sub_bios_cdda
 */
static inline void bios_audio_play_from(u32 const * timecode)
{
  register u16 _BIOS_AUDIO_PLAY_FROM asm("d0") = BIOS_AUDIO_PLAY_FROM;
  register u32 _timecode asm("a0") = (u32) timecode;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_AUDIO_PLAY_FROM), "+a"(_timecode)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_PLAY_FROM), "a"(_timecode)
    : "cc", "a1");
};

/**
 * @fn bios_audio_seek
 * @brief Seeks to the beginning of the specified track and pauses
 * @sa BIOS_AUDIO_SEEK
 * @ingroup sub_bios_cdda
 */
static inline void bios_audio_seek(u16 const * track_number)
{
  register u16 _BIOS_AUDIO_SEEK asm("d0") = BIOS_AUDIO_SEEK;
  register u32 _track_number asm("a0") = (u32) track_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_AUDIO_SEEK), "+a"(_track_number)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_SEEK), "a"(_track_number)
    : "cc", "a1");
};

/**
 * @def bios_audio_seek_once
 * @brief Seek to the beginning of the selected track and pause; when BIOS
 * detects a pause state, the track is played once
 * @sa BIOS_AUDIO_SEEK_ONCE
 * @ingroup sub_bios_cdda
 */
static inline void bios_audio_seek_once(u16 const * track_number)
{
  register u16 _BIOS_AUDIO_SEEK_ONCE asm("d0") = BIOS_AUDIO_SEEK_ONCE;
  register u32 _track_number asm("a0") = (u32) track_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_AUDIO_SEEK_ONCE), "+a"(_track_number)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_SEEK_ONCE), "a"(_track_number)
    : "cc", "a1");
};

/**
 * @fn bios_audio_seek_from
 * @brief Seeks to a specified timecode
 * @sa BIOS_AUDIO_SEEK_FROM
 * @ingroup sub_bios_cdda
 */
static inline void bios_audio_seek_from(u32 const * timecode)
{
  register u16 _BIOS_AUDIO_SEEK_FROM asm("d0") = BIOS_AUDIO_SEEK_FROM;
  register u32 _timecode asm("a0") = (u32) timecode;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_AUDIO_SEEK_FROM), "+a"(_timecode)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_SEEK_FROM), "a"(_timecode)
    : "cc", "a1");
};

/**
 * @fn bios_audio_stop
 * @brief Stops playing CD audio if it is playing
 * @sa BIOS_AUDIO_STOP
 * @ingroup sub_bios_cdda
 */
static inline void bios_audio_stop()
{
  register u16 _BIOS_AUDIO_STOP asm("d0") = BIOS_AUDIO_STOP;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(_BIOS_AUDIO_STOP)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_STOP)
    : "cc", "a0", "a1");
}

/**
 * @fn bios_audio_pause_on
 * @brief Pauses the drive when a track is playing
 * @sa BIOS_MSC_PAUSEON
 * @ingroup sub_bios_cdda
 *
 * @note If the drive is left paused it will stop after a programmable delay
 * (see @ref _CDBPAUSE)
 */
static inline void bios_audio_pause_on()
{
  register u16 _BIOS_AUDIO_PAUSE_ON asm("d0") = BIOS_AUDIO_PAUSE_ON;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(_BIOS_AUDIO_PAUSE_ON)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_PAUSE_ON)
    : "cc", "a0", "a1");
}

/**
 * @fn bios_audio_pause_off
 * @brief Resumes playing a track after a pause
 * @sa BIOS_MSC_PAUSEOFF
 * @ingroup sub_bios_cdda
 *
 * @note If the drive has timed out and stopped, the BIOS will seek to the pause
 * time (with the attendant delay) and resume playing
 */
static inline void bios_audio_pause_off()
{
  register u16 D0 asm("d0") = BIOS_AUDIO_PAUSE_OFF;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "a0", "a1");
};

/**
 * @fn bios_audio_scan_ff
 * @brief Starts playing from the current position in fast forward
 * @sa BIOS_AUDIO_SCAN_FF
 * @ingroup sub_bios_cdda
 */
static inline void bios_audio_scan_ff()
{
  register u16 _BIOS_AUDIO_SCAN_FF asm("d0") = BIOS_AUDIO_SCAN_FF;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(_BIOS_AUDIO_SCAN_FF)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_SCAN_FF)
    : "cc", "a0", "a1");
};

/**
 * @fn bios_audio_scan_fr
 * @brief Starts playing from the current position in fast reverse
 * @sa BIOS_AUDIO_SCAN_FR
 * @ingroup sub_bios_cdda
 */
static inline void bios_audio_scan_fr()
{
  register u16 _BIOS_AUDIO_SCAN_FR asm("d0") = BIOS_AUDIO_SCAN_FR;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(_BIOS_AUDIO_SCAN_FR)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_SCAN_FR)
    : "cc", "a0", "a1");
};

/**
 * @fn bios_audio_scan_off
 * @brief Returns to normal play mode
 * @sa BIOS_MSC_SCANOFF
 * @ingroup sub_bios_cdda
 *
 * @note If the drive was paused before the scan was initiated, it will be
 * returned to pause.
 */
static inline void bios_audio_scan_off()
{
  register u16 _BIOS_AUDIO_SCAN_OFF asm("d0") = BIOS_AUDIO_SCAN_OFF;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(_BIOS_AUDIO_SCAN_OFF)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_AUDIO_SCAN_OFF)
    : "cc", "a0", "a1");
};

struct RomreadParams
{
  u32 start_sector;
  u32 sector_count;
};

/**
 * @fn bios_cdrom_read
 * @brief Begins reading data from the CDROM at the designated logical sector
 * @sa BIOS_ROM_READ
 * @ingroup sub_bios_cdrom
 *
 * @details
 * Executes a CDC_START to begin the read, but does not stop
 * automatically.
 *
 * @note
 * ROM_READ actually pre-seeks by 2 sectors, but doesn't start passing data
 * to the CDC until the desired sector is reached.
 */
static inline void bios_cdrom_read(u32 const * sector_number)
{
  register u16 _BIOS_ROM_READ asm("d0") = BIOS_ROM_READ;
  register u32 _sector_number asm("a0") = (u32) sector_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_ROM_READ), "+a"(_sector_number)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_ROM_READ), "a"(_sector_number)
    : "cc", "a1");
};

/**
 * @def bios_cdrom_read_count
 * @brief Same function as @ref bios_cdrom_read but stops after reading the
 * requested number of sectors
 * @ingroup sub_bios_cdrom
 *
 */
static inline void bios_cdrom_read_count(struct RomreadParams const * param)
{
  register u16 _BIOS_ROM_READ_COUNT asm("d0") = BIOS_ROM_READ_COUNT;
  register u32 _param asm("a0") = (u32) param;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_ROM_READ_COUNT), "+a"(_param)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_ROM_READ_COUNT), "a"(_param)
    : "cc", "a1");
};

/**
 * @def bios_cdrom_read_range
 * @ingroup sub_bios_cdrom
 * @brief Same as @ref bios_cdrom_read, but reads between two logical sectors
 */
static inline void bios_cdrom_read_range(struct RomreadParams const * param)
{
  register u16 _BIOS_ROM_READ_RANGE asm("d0") = BIOS_ROM_READ_RANGE;
  register u32 _param asm("a0") = (u32) param;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_ROM_READ_RANGE), "+a"(_param)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_ROM_READ_RANGE), "a"(_param)
    : "cc", "a1");
};

/**
 * @fn bios_cdrom_pause_on
 * @brief Stops reading data into the CDC and pauses
 * @sa BIOS_ROM_PAUSE_ON
 * @ingroup sub_bios_cdrom
 */
static inline void bios_cdrom_pause_on()
{
  register u16 _BIOS_ROM_PAUSE_ON asm("d0") = BIOS_ROM_PAUSE_ON;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(_BIOS_ROM_PAUSE_ON)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_ROM_PAUSE_ON)
    : "cc", "a0", "a1");
};

/**
 * @fn bios_cdrom_pause_off
 * @brief Resumes reading data into the CDC from the current logical sector
 * @sa BIOS_ROM_PAUSEOFF
 * @ingroup sub_bios_cdrom
 */
static inline void bios_cdrom_pause_off()
{
  register u16 _BIOS_ROM_PAUSE_OFF asm("d0") = BIOS_ROM_PAUSE_OFF;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(_BIOS_ROM_PAUSE_OFF)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_ROM_PAUSE_OFF)
    : "cc", "a0", "a1");
};

/**
 * @fn bios_rom_seek
 * @brief Seeks to the designated logical sector and pauses
 * @sa BIOS_ROM_SEEK
 * @ingroup sub_bios_cdrom
 */
static inline void bios_cdrom_seek(u32 const * sector_number)
{
  register u16 _BIOS_ROM_SEEK asm("d0") = BIOS_ROM_SEEK;
  register u32 _sector_number asm("a0") = (u32) sector_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(_BIOS_ROM_SEEK), "+a"(_sector_number)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_ROM_SEEK), "a"(_sector_number)
    : "cc", "a1");
};

/**
 * @fn bios_fader_set
 * @brief Sets the audio volume
 * @sa BIOS_FADER_SET
 * @ingroup sub_bios_fader
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
static inline void bios_fader_set(u16 const volume)
{
  register u16       _BIOS_FADER_SET asm("d0") = BIOS_FADER_SET;
  register u16 const _volume asm("d1") = volume;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(_BIOS_FADER_SET)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_FADER_SET), "d"(_volume)
    : "cc");
};

/**
 * @def bios_fader_change
 * @brief  Ramps the audio volume from its current level to a new level at
 * the requested rate
 * @sa BIOS_FADER_CHANGE
 * @ingroup sub_bios_fader

 * @details The input is two 16 bit values
 *   High word: new volume (min 0x0000, max 0x0400)
 *    Low word: change rate in steps per vblank
 *        0x0001 - slow
 *        0x0200 - fast
 *        0x0400 - immediate
 *
 * @note There is a delay of up to 13ms before the volume begins to change.
 */
static inline void bios_fader_change(u32 const volume)
{
  register u16 _BIOS_FADER_CHANGE asm("d0") = BIOS_FADER_CHANGE;
  register u32 _volume asm("d1") = volume;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(_BIOS_FADER_CHANGE)
    : "i"(BIOS_CALL_VECTOR), "d"(_BIOS_FADER_CHANGE), "d"(_volume)
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
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
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
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc");
};

/**
 * @fn bios_cdc_stat
 * @sa BIOS_CDC_STATUS
 */
static inline void bios_cdc_stat()
{
  register u16 D0 asm("d0") = BIOS_CDC_STATUS;

  asm volatile(
    "\
jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "d1");
};

/**
 * @sa BIOS_CDC_READ
 */
// headaches with asm goto and output operands... we'll come back to this later.
/*
static inline bool bios_cdcread() {
  register u16 D0 asm("d0") = BIOS_CDC_READ;

        asm goto(R"(jsr %p1)"
                                         : "+d"(D0)
                                         : "i"(BIOS_CALL_VECTOR), "d"(D0)
                                         : "cc", "d1");

        return false;

sector_ready:
        return true;
};
*/

/**
 * @fn bios_cdc_transfer
 * @sa BIOS_CDC_TRANSFER
 */
static inline void bios_cdc_transfer(u8 * sector_dest, u8 * header_dest)
{
  register u16  D0 asm("d0") = BIOS_CDC_TRANSFER;
  register u8 * a0_sector_dest asm("a0") = sector_dest;
  register u8 * a1_header_dest asm("a1") = header_dest;

  asm volatile(
    "\
jsr %p3"
    : "+d"(D0), "+a"(a0_sector_dest), "+a"(a1_header_dest)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(a0_sector_dest), "a"(a1_header_dest)
    : "cc", "d1");
};

/**
 * @fn bios_cdc_ack
 * @sa BIOS_CDC_ACK
 */
static inline void bios_cdc_ack()
{
  register u16 D0 asm("d0") = BIOS_CDC_ACK;

  asm volatile(
    "\
jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc");
};

static inline void bios_waitvsync()
{
  asm volatile(
    "\
		jsr %p0"
    :
    : "i"(WAITVSYNC)
    : "cc");
}

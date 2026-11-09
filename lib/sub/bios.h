/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub/bios.h
 * @brief Sub CPU side system library
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "sub/bios.def.h"
#include "sub/memmap.def.h"
#include "types.h"

/**
 * @brief Wait for the next INT2 from the Main CPU, which it raises once per frame
 * @sa BIOS_WAIT_VSYNC
 */
static inline void bios_wait_vsync()
{
  asm volatile(
    "\
		jsr %p0"
    :
    : "i"(BIOS_WAIT_VSYNC)
    : "cc");
}

/**
 * @brief Stops playing CD audio if it is playing
 * @sa BIOS_AUDIO_STOP
 * @ingroup bios_cdda
 */
static inline void bios_audio_stop()
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_STOP;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "a0", "a1");
}

/**
 * @brief Pauses the drive when a track is playing
 * @sa BIOS_AUDIO_PAUSE
 * @ingroup bios_cdda
 *
 * @note If the drive is left paused it will stop after a programmable delay
 * (see @ref BIOS_SET_PAUSE_TIMEOUT)
 */
static inline void bios_audio_pause()
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_PAUSE;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "a0", "a1");
}

/**
 * @brief Resumes playing a track after a pause
 * @sa BIOS_AUDIO_RESUME
 * @ingroup bios_cdda
 *
 * @note If the drive has timed out and stopped, the BIOS will seek to the pause
 * time (with the attendant delay) and resume playing
 */
static inline void bios_audio_resume()
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_RESUME;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "a0", "a1");
};

/**
 * @brief Starts playing from the current position in fast forward
 * @sa BIOS_AUDIO_SCAN_FORWARD
 * @ingroup bios_cdda
 */
static inline void bios_audio_scan_forward()
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_SCAN_FORWARD;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "a0", "a1");
};

/**
 * @brief Starts playing from the current position in fast reverse
 * @sa BIOS_AUDIO_SCAN_BACKWARD
 * @ingroup bios_cdda
 */
static inline void bios_audio_scan_backward()
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_SCAN_BACKWARD;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "a0", "a1");
};

/**
 * @brief Returns to normal play mode
 * @sa BIOS_AUDIO_SCAN_OFF
 * @ingroup bios_cdda
 *
 * @note If the drive was paused before the scan was initiated, it will be
 * returned to pause.
 */
static inline void bios_audio_scan_off()
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_SCAN_OFF;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "a0", "a1");
};

/**
 * @brief Starts CD audio playback at the specified track & continues playing
 * through subsequent tracks
 * @sa BIOS_AUDIO_PLAY
 * @ingroup bios_cdda
 */
static inline void bios_audio_play(uint16_t const * track_number)
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_PLAY;
  register uint32_t A0 asm("a0") = (uint32_t) track_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(A0)
    : "cc", "a1");
};

/**
 * @brief Plays the specified track once then pauses
 * @sa BIOS_AUDIO_PLAY_ONCE
 * @ingroup bios_cdda
 */
static inline void bios_audio_play_once(uint16_t const * track_number)
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_PLAY_ONCE;
  register uint32_t A0 asm("a0") = (uint32_t) track_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(A0)
    : "cc", "a1");
};

/**
 * @brief Plays the specified track on repeat
 * @sa BIOS_AUDIO_PLAY_REPEAT
 * @ingroup bios_cdda
 */
static inline void bios_audio_play_repeat(uint16_t const * track_number)
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_PLAY_REPEAT;
  register uint32_t A0 asm("a0") = (uint32_t) track_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(A0)
    : "cc", "a1");
};

/**
 * @brief Starts playing from the specified timecode
 * @sa BIOS_AUDIO_PLAY_FROM_TIME
 * @ingroup bios_cdda
 */
static inline void bios_audio_play_from_time(uint32_t const * timecode)
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_PLAY_FROM_TIME;
  register uint32_t A0 asm("a0") = (uint32_t) timecode;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(A0)
    : "cc", "a1");
};

/**
 * @brief Seeks to the beginning of the specified track and pauses
 * @sa BIOS_AUDIO_SEEK
 * @ingroup bios_cdda
 */
static inline void bios_audio_seek(uint16_t const * track_number)
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_SEEK;
  register uint32_t A0 asm("a0") = (uint32_t) track_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(A0)
    : "cc", "a1");
};

/**
 * @brief Seeks to a specified timecode
 * @sa BIOS_AUDIO_SEEK_TIME
 * @ingroup bios_cdda
 */
static inline void bios_audio_seek_time(uint32_t const * timecode)
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_SEEK_TIME;
  register uint32_t A0 asm("a0") = (uint32_t) timecode;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(A0)
    : "cc", "a1");
};

/**
 * @brief Seek to the beginning of the selected track and pause; when BIOS
 * detects a pause state, the track is played once
 * @sa BIOS_AUDIO_SEEK_PLAY_ONCE
 * @ingroup bios_cdda
 */
static inline void bios_audio_seek_play_once(uint16_t const * track_number)
{
  register uint16_t D0 asm("d0") = BIOS_AUDIO_SEEK_PLAY_ONCE;
  register uint32_t A0 asm("a0") = (uint32_t) track_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(A0)
    : "cc", "a1");
};

/**
 * @brief Opens the CD drive door
 * @ingroup bios_cdb
 * @sa BIOS_DRIVE_OPEN
 *
 * @note This is only applicable to Mega CD Model 1 hardware.
 */
static inline void bios_drive_open()
{
  register uint16_t D0 asm("d0") = BIOS_DRIVE_OPEN;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "a0", "a1");
};

/**
 * @brief Parameters of bios_drive_init
 * @ingroup bios_cdb
 */
typedef struct DriveInitParams {
  uint8_t toc_track_number; ///< First track to read the TOC for, usually 1
  uint8_t last_track;       ///< Last track to read, or 0xFF for all
} DriveInitParams;

/**
 * @brief Closes the disk tray and reads the TOC from the CD
 * @sa BIOS_DRIVE_INIT
 * @ingroup bios_cdb
 *
 * @details
 * Pauses for 2 seconds after reading the TOC. Waits for a DRV_OPEN request if
 * there is no disk in the drive.
 */
static inline void
bios_drive_init(struct DriveInitParams const * drive_init_params)
{
  register uint16_t D0 asm("d0") = BIOS_DRIVE_INIT;
  register uint32_t a0_drv_init_param asm("a0") = (uint32_t) drive_init_params;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(a0_drv_init_param)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(a0_drv_init_param)
    : "cc", "a1");
};

/**
 * @brief Stops reading data into the CDC and pauses
 * @sa BIOS_CDROM_PAUSE
 * @ingroup bios_cdrom
 */
static inline void bios_cdrom_pause()
{
  register uint16_t D0 asm("d0") = BIOS_CDROM_PAUSE;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "a0", "a1");
};

/**
 * @brief Resumes reading data into the CDC from the current logical sector
 * @sa BIOS_CDROM_RESUME
 * @ingroup bios_cdrom
 */
static inline void bios_cdrom_resume()
{
  register uint16_t D0 asm("d0") = BIOS_CDROM_RESUME;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "a0", "a1");
};

/**
 * @brief Begins reading data from the CDROM at the designated logical sector
 * @sa BIOS_CDROM_READ
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
static inline void bios_cdrom_read(uint32_t const * sector_number)
{
  register uint16_t D0 asm("d0") = BIOS_CDROM_READ;
  register uint32_t A0 asm("a0") = (uint32_t) sector_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(A0)
    : "cc", "a1");
};

/**
 * @brief Seeks to the designated logical sector and pauses
 * @sa BIOS_CDROM_SEEK
 * @ingroup bios_cdrom
 */
static inline void bios_cdrom_seek(uint32_t const * sector_number)
{
  register uint16_t D0 asm("d0") = BIOS_CDROM_SEEK;
  register uint32_t A0 asm("a0") = (uint32_t) sector_number;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(A0)
    : "cc", "a1");
};

/**
 * @brief Parameters of bios_cdrom_read_count and bios_cdrom_read_range
 * @ingroup bios_cdrom
 */
struct RomreadParams {
  uint32_t start_sector; ///< First logical sector
  uint32_t
    sector_count; ///< Number of sectors (read_count), or the last sector (read_range)
};

/**
 * @brief Same function as @ref bios_cdrom_read but stops after reading the
 * requested number of sectors
 * @ingroup bios_cdrom
 *

 */
static inline void bios_cdrom_read_count(struct RomreadParams const * param)
{
  register uint16_t D0 asm("d0") = BIOS_CDROM_READ_COUNT;
  register uint32_t A0 asm("a0") = (uint32_t) param;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(A0)
    : "cc", "a1");
};

/**
 * @ingroup bios_cdrom
 * @brief Same as @ref bios_cdrom_read, but reads between two logical sectors
 */
static inline void bios_cdrom_read_range(struct RomreadParams const * param)
{
  register uint16_t D0 asm("d0") = BIOS_CDROM_READ_RANGE;
  register uint32_t A0 asm("a0") = (uint32_t) param;

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "+a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(A0)
    : "cc", "a1");
};

/**
 * @brief Query the BIOS on the status of the last command
 * @sa BIOS_CHECK_COMMAND
 * @return true if the command is complete
 */
static inline bool bios_check_command()
{
  register uint16_t D0 asm("d0") = BIOS_CHECK_COMMAND;

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
 * @brief Status returned by bios_get_status
 * @details The layout of the CDBSTAT status table; see the BIOS manual for the
 * field values.
 * @ingroup bios_cdb
 */
struct BiosStatus {
  uint16_t
    bios_status; ///< BIOS status word: high byte drive state, low byte BIOS state
  uint16_t led;            ///< LED state
  uint8_t  cdd_status[20]; ///< The last CDD status, 20 bytes
  uint32_t volume;         ///< Current fader volume
  uint32_t header;         ///< Disc header: absolute time of the current sector
};

/**
 * @brief Retrieve CD BIOS status
 *
 * Please refer to the BIOS manual for details about the returned data
 */
static inline struct BiosStatus const * bios_get_status()
{
  register uint16_t D0 asm("d0") = BIOS_GET_STATUS;
  register uint32_t A0 asm("a0");

  asm volatile(
    "\
		jsr %p2"
    : "+d"(D0), "=a"(A0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc", "d1");

  return (struct BiosStatus *) A0;
}

/**
 * @brief Sets the audio volume
 * @sa BIOS_FADER_SET
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
static inline void bios_fader_set(uint16_t const volume)
{
  register uint16_t       D0 asm("d0") = BIOS_FADER_SET;
  register uint16_t const D1 asm("d1") = volume;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "d"(D1)
    : "cc");
};

/**
 * @brief  Ramps the audio volume from its current level to a new level at
 * the requested rate
 * @sa BIOS_FADER_CHANGE
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
static inline void bios_fader_change(uint32_t const volume)
{
  register uint16_t D0 asm("d0") = BIOS_FADER_CHANGE;
  register uint32_t D1 asm("d1") = volume;

  asm volatile(
    "\
		jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "d"(D1)
    : "cc");
};

/**
 * @sa BIOS_CDC_START
 */
static inline void bios_cdc_start()
{
  register uint16_t D0 asm("d0") = BIOS_CDC_START;

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
  register uint16_t D0 asm("d0") = BIOS_CDC_STOP;

  asm volatile(
    "\
jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc");
};

/**
 * @sa BIOS_CDC_STATUS
 */
static inline void bios_cdc_status()
{
  register uint16_t D0 asm("d0") = BIOS_CDC_STATUS;

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
static inline bool bios_cdc_read() {
	register uint16_t D0 asm("d0") = BIOS_CDC_READ;

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
 * @sa BIOS_CDC_TRANSFER
 */
static inline void
bios_cdc_transfer(uint8_t * sector_dest, uint8_t * header_dest)
{
  register uint16_t  D0 asm("d0") = BIOS_CDC_TRANSFER;
  register uint8_t * a0_sector_dest asm("a0") = sector_dest;
  register uint8_t * a1_header_dest asm("a1") = header_dest;

  asm volatile(
    "\
jsr %p3"
    : "+d"(D0), "+a"(a0_sector_dest), "+a"(a1_header_dest)
    : "i"(BIOS_CALL_VECTOR), "d"(D0), "a"(a0_sector_dest), "a"(a1_header_dest)
    : "cc", "d1");
};

/**
 * @sa BIOS_CDC_ACK
 */
static inline void bios_cdc_ack()
{
  register uint16_t D0 asm("d0") = BIOS_CDC_ACK;

  asm volatile(
    "\
jsr %p1"
    : "+d"(D0)
    : "i"(BIOS_CALL_VECTOR), "d"(D0)
    : "cc");
};

#ifdef __cplusplus
}
#endif

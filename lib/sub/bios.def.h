/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub/bios.def.h
 * @brief Sub CPU side system library (BIOS) definitions
 */

#pragma once

/**
 * @defgroup sub_bios Sub CPU / BIOS
 */

/**
 * @defgroup sub_bios_cdda Sub CPU / BIOS / CD Audio (CD-DA) Functions
 */

/**
 * @defgroup sub_bios_cdrom Sub CPU / BIOS / CD-ROM Access Functions
 */

/**
 * @defgroup sub_bios_cdc Sub CPU / BIOS / CD-ROM Data Controller (CDC)
 * Functions
 */

/**
 * @defgroup sub_bios_fader Sub CPU / BIOS / Audio Fader Functions
 */

/**
 * @defgroup bios_subcode Sub CPU / BIOS / Subcode Data Access Functions
 */

/**
 * @defgroup sub_bios_bram Sub CPU / BIOS / Backup RAM Functions
 */

/**
 * @defgroup sub_bios_misc Sub CPU / BIOS / Other BIOS Functions
 */

/**
 * @defgroup sub_bios_unknown Sub CPU / BIOS / Undocumented calls
 */

/**
 * @def BOOTSTAT
 */
#define BOOTSTAT 0x005EA0

/**
 * @def INT2FLAG
 */
#define INT2FLAG 0x005EA4

/**
 * @def USERMODE
 */
#define USERMODE 0x005EA6

/**
 * @def CDSTAT
 * @brief Contains the status after calling @ref _CDB_STAT
 */
#define CDSTAT 0x005E80

/**
 * System Jump Table
 * IN:
 * A1 ptr to user header (see MEGA CD BIOS MANUAL)
 * A0 ptr to jump table
 * BREAK A2
 * NOTES
 * Expects the user header to begin with string "MAIN"
 * Other valid strings include:
 *   "SYS"
 *   "SUB"
 *   "DAT"
 * but these must appear beginning at the 8th byte (as if they were filename
 * extensions); it seems there is no tangible difference between these
 * the flag
 * (the last byte, normally zero the filename) It looks like if the flag is
 * non-zero, it calls the start address as a subroutine (JSR) and checks for
 * carry clear (BCC) on return; on carry clear, it skips the jump table
 * installation that is called on a normaly flag = 0 flow
 *
 * "ptr to link module" is an offset relative to the start of the user header
 * it then goes through the process of checking flag, possibly running entry,
 * and installing jumps
 */
#define SETJMPTBL 0x005F0A

/**
 * @def WAITVSYNC
 */
#define WAITVSYNC 0x005F10

/**
 * @def BIOS_CALL_VECTOR
 * @brief General system calls vector
 * @ingroup sub_bios
 */
#define BIOS_CALL_VECTOR 0x005F22

/**
 * @def CDB_DRIVE_OPEN
 * @alias DRV_OPEN
 * @sa bios_drive_open
 * @ingroup sub_bios_misc
 * @clobber d0-d1/a0-a1
 */
#define CDB_DRIVE_OPEN 0x000A

/**
 * @def CDB_DRIVE_INIT
 * @sa bios_drive_init
 * @alias DRV_INIT
 * @clobber d0-d1/a1
 *
 * @param[in] A0.l Pointer to initilization parameters
 *
 * @details Takes a pointer to two bytes (initialization params):
 *  byte 1 - track number from which to read TOC (normally 0x01); if bit 7 of
 *           this value is set, BIOS will start to play the first track
             automatically
 *  byte 2 - last track to read (0xFF will read all tracks)
 */
#define CDB_DRIVE_INIT 0x0010

/**
 * @def CDB_CHECK_STATUS
 * @sa bios_cdb_chk
 * @alias CDB_CHK
 * @clobber d0
 * @ingroup sub_bios_misc
 *
 * @param[out] CC Command complete
 * @param[out] CS BIOS is busy
 */
#define CDB_CHECK_STATUS 0x0080

/**
 * @def CDB_GET_STATUS
 * @sa bios_get_status
 * @clobber d0-d1/a0
 * @ingroup sub_bios_misc
 *
 * @param[out] A0.l Pointer to status info structure
 *
 * @note
 * Documentation says A1 is clobbered as well, but we don't see any
 * sign of that in the disassemblies checked so far
 */
#define CDB_GET_STATUS 0x0081

/**
 * @def CDB_AUDIO_PLAY
 * @sa bios_audio_play
 * @alias MSC_PLAY
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define CDB_AUDIO_PLAY 0x0011

/**
 * @def CDB_AUDIO_PLAY_ONCE
 * @sa bios_audio_play_once
 * @alias MSC_PLAY1
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define CDB_AUDIO_PLAY_ONCE 0x0012

/**
 * @def CDB_AUDIO_PLAY_REPEAT
 * @sa bios_audio_play_repeat
 * @alias MSC_PLAYR
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define CDB_AUDIO_PLAY_REPEAT 0x0013

/**
 * @def CDB_AUDIO_PLAY_FROM
 * @sa bios_audio_play_from
 * @alias MSC_PLAYT
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_cdda
 *
 * @param[in] A0.l Pointer to BCD time code in the format mm:ss:ff:00
 *   (32 bit)
 */
#define CDB_AUDIO_PLAY_FROM 0x0014

/**
 * @def CDB_AUDIO_SEEK
 * @sa bios_audio_seek
 * @alias MSC_SEEK
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define CDB_AUDIO_SEEK 0x0015

/**
 * @def CDB_AUDIO_SEEK_ONCE
 * @sa bios_audio_seek_once
 * @alias MSC_SEEK1
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define CDB_AUDIO_SEEK_ONCE 0x0019

/**
 * @def CDB_AUDIO_SEEK_FROM
 * @sa bios_audio_seek_from
 * @alias MSC_SEEKT
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_cdda
 *
 * @param[in] A0.l Pointer to BCD time code in the format mm:ss:ff:00
 *   (32 bit)
 */
#define CDB_AUDIO_SEEK_FROM 0x0016

/**
 * @def CDB_AUDIO_STOP
 * @sa bios_audio_stop
 * @alias MSC_STOP
 * @ingroup sub_bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define CDB_AUDIO_STOP 0x0002

/**
 * @def CDB_AUDIO_PAUSE_ON
 * @sa bios_audio_pause_on
 * @alias MSC_PAUSEON
 * @ingroup sub_bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define CDB_AUDIO_PAUSE_ON 0x0003

/**
 * @def CDB_AUDIO_PAUSE_OFF
 * @sa bios_msc_pauseoff
 * @alias MSC_PAUSEOFF
 * @ingroup sub_bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define CDB_AUDIO_PAUSE_OFF 0x0004

/**
 * @def CDB_AUDIO_SCAN_FF
 * @sa bios_audio_scan_ff
 * @alias MSC_SCANFF
 * @ingroup sub_bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define CDB_AUDIO_SCAN_FF 0x0005

/**
 * @def CDB_AUDIO_SCAN_FR
 * @sa bios_audio_scan_fr
 * @alias MSC_SCANFR
 * @ingroup sub_bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define CDB_AUDIO_SCAN_FR 0x0006

/**
 * @def CDB_AUDIO_SCAN_OFF
 * @sa bios_audio_scan_off
 * @alias MSC_SCANOFF
 * @ingroup sub_bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define CDB_AUDIO_SCAN_OFF 0x0007

/**
 * @def CDB_ROM_READ
 * @sa bios_cdrom_read
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_cdrom
 *
 * @param[in] A0.l Pointer to the logical sector number (32 bit)
 */
#define CDB_ROM_READ 0x0017

/**
 * @def CDB_ROM_READ_COUNT
 * @sa bios_cdrom_read_count
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_cdrom
 *
 * @param[in] A0.l Pointer to a sector read info structure
 *
 * @details
 * The structure is made up of two 32 bit values:
 *   dc.l 0x00000001   // Logical start sector
 *   dc.l 0x00001234   // Number of sectors to read
 *
 * CDC_START is automatically executed. After all sectors are read, CDC_STOP is
 * automatically executed.
 */
#define CDB_ROM_READ_COUNT 0x0020

/**
 * @def CDB_ROM_READ_RANGE
 * @sa bios_cdrom_read_range
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_cdrom
 *
 * @param[in] A0.l Pointer to a sector read info structure
 *
 * OUT:
 *  none
 *
 * @details
 * The structure is made up of two 32 bit values:
 *   dc.l 0x00000001   // Logical start sector
 *   dc.l 0x00001234   // Logical end sector
 *
 * CDC_START is automatically executed. After all sectors are read, CDC_STOP is
 * automatically executed.
 */
#define CDB_ROM_READ_RANGE 0x0021

/**
 * @def CDB_ROM_PAUSE_ON
 * @sa bios_cdrom_pause_on
 * @alias ROM_PAUSEON
 * @ingroup sub_bios_cdrom
 * @clobber d0-d1/a0-a1
 */
#define CDB_ROM_PAUSE_ON 0x0008

/**
 * @def CDB_ROM_PAUSE_OFF
 * @sa bios_cdrom_pause_off
 * @alias ROM_PAUSEOFF
 * @ingroup sub_bios_cdrom
 * @clobber d0-d1/a0-a1
 */
#define CDB_ROM_PAUSE_OFF 0x0009

/**
 * @def CDB_ROM_SEEK
 * @sa bios_cdrom_seek
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_cdrom
 *
 * @param[in] A0.l Pointer to the logical sector number (32 bit)
 */
#define CDB_ROM_SEEK 0x0018

/**
 * @def CDB_FADER_SET
 * @sa bios_fader_set
 * @clobber d0-d1/a0
 * @ingroup sub_bios_fader
 *
 * @param[in] D1.w Volume
 */
#define CDB_FADER_SET 0x0085

/**
 * @def CDB_FADER_CHANGE
 * @sa bios_fdr_chg
 * @clobber d0-d1/a0
 * @ingroup sub_bios_fader
 *
 * @param[in] D1.l Volume & Ramp
 */
#define CDB_FADER_CHANGE 0x0086

/**
 * @def CDB_PAUSE_TIMEOUT
 * @brief Sets the time that the drive spins down from pause to standby
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_misc
 *
 * @param[in] D1.w Pause time (in ticks of 1/75 second)
 *
 * @details Normal ranges for this delay time are 0x1194 - 0xFFFE. A delay of
 * 0xFFFF prevents the drive from stopping, but can damage the drive if used
 * improperly.
 */
#define CDB_PAUSE_TIMEOUT 0x0084

/**
 * @def CDB_CDBTOCWRITE
 * @brief Writes data to disc TOC stored in memory
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_misc
 *
 * @param[in] A0.l Pointer to TOC data
 *
 * @details A TOC entry is four bytes long, where the first three bytes are
 * the BCD timecode of the start of the track and the lowest byte is the track
 * number. The most significant bit of the frame byte of the timecode indicates
 * whether the track is CD-ROM or CD-DA. Please refer to the BIOS manual
 * for more information.
 */
#define CDB_TOC_WRITE 0x0082

/**
 * @def CDB_CDBTOCREAD
 * @brief Reads the TOC entry for a given track
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_misc
 *
 * @param[in] D1.w Track number
 * @param[out] D0.l Track start timecode (BCD, low byte is track number)
 * @param[out] D1.b Track type (00 - CD-DA, 0xFF - CD-ROM)
 *
 * @note The low byte of D0 will be 0 on error.
 */
#define CDB_TOC_READ 0x0083

/**
 * @def CDB_CDC_START
 * @brief Starts reading data from the current logical sector into the CDC
 * @clobber d0-d1/a0
 * @ingroup sub_bios_cdc
 *
 * @note The BIOS pre-seeks by 2 to 4 sectors and data read actually begins
 * before the requested sector. It is up to the caller to identify the correct
 * starting sector (usually by checking the time codes in the headers as
 * they are read from the CDC buffer).
 */
#define CDB_CDC_START 0x0087

/**
 * @def CDB_CDC_STARTP
 * @brief No official documentation on this call; needs to be researched
 * @ingroup sub_bios_unknown
 */
#define CDB_CDC_STARTP 0x0088

/**
 * @def CDB_CDC_STOP
 * @brief Stop reading data into the CDC buffer
 * @clobber d0/a0
 * @ingroup sub_bios_cdc
 *
 * @note If a sector is being read when CDC_STOP is called, it is discarded.
 */
#define CDB_CDC_STOP 0x0089

/**
 * @def CDB_CDC_STATUS
 * @brief Query the status of the CDC buffer
 * @clobber d0-d1/a0
 * @ingroup sub_bios_cdc
 *
 * @param[out] CC Sector available for read
 * @param[out] CS Sector not ready
 */
#define CDB_CDC_STATUS 0x008A

/**
 * @def CDB_CDCREAD
 * @brief Reads sector of data in preparation for transfer
 * @clobber d0-d1/a0
 * @ingroup sub_bios_cdc
 *
 * @param[out] CC Sector ready for transfer
 * @param[out] CS Sector not ready
 * @param[out] D0.l Sector header timecode (BCD)
 * @details The low byte of D0 is the sector mode:
 *   00 -sub_bios_cdda
 *   01 -sub_bios_cdrom mode 1
 *   02 -sub_bios_cdrom mode 2
 *
 * @note Be sure to set the device destination register BEFORE calling CDCREAD!
 */
#define CDB_CDCREAD 0x008B

/**
 * @def CDB_CDC_TRANSFER
 * @brief Transfer one sector of data from the CDC to Sub CPU RAM
 * @clobber d0-d1/a0
 * @ingroup sub_bios_cdc
 *
 * @param[in] A0.l Pointer to sector destination buffer (0x920 bytes)
 * @param[in] A1.l Pointer to header destination buffer (4 bytes)
 * @param[out] CC Sector transfer completed successfully
 * @param[out] CS Sector transfer failed
 * @param[out] A0.l Pointer to next sector destination address (A0 + 0x920)
 * @param[out] A1.l Pointer to next header destination address (A1 + 4)
 *
 * @note The device destination must be set to Sub CPU read beforehand!
 */
#define CDB_CDC_TRANSFER 0x008C

/**
 * @def CDB_CDC_ACK
 * @brief Informs the CDC that the current sector has been read and that the
 * caller is ready for the next sector
 * @clobber d0
 * @ingroup sub_bios_cdc
 */
#define CDB_CDC_ACK 0x008D

/**
 * @def CDB_SUBCODE_INIT
 * @brief Initializes the BIOS for subcode reads
 * @ingroup bios_subcode
 *
 * @param[in] A0.l Pointer to work buffer (at least 0x750 bytes)
 */
#define CDB_SUBCODE_INIT 0x008E

/**
 * @def CDB_SUBCODE_READ_ENABLE
 * @brief Enables reading subcode data by the CDC
 * @clobber d0-d1/a0-a1
 * @ingroup bios_subcode
 *
 * @param[in] D1.w Subcode read flags
 *
 * @details Flags
 *    0: --------
 *    1: --RSTUVW
 *    2: PQ------
 *    3: PQRSTUVW
 */
#define CDB_SUBCODE_READ_ENABLE 0x008F

/**
 * @def CDB_SUBCODE_READ_DISABLE
 * @brief Disables reading subcode data by the CDC
 * @clobber d0-d1/a0-a1
 * @ingroup bios_subcode
 */
#define CDB_SUBCODE_READ_DISABLE 0x0090

/**
 * @def CDB_SUBCODE_STATUS
 * @brief Check subcode error status
 * @clobber d0-d1/a0-a1
 * @ingroup bios_subcode
 *
 * @param[out] D0.l errqcodecrc / errpackcirc / scdflag / restrcnt
 * @param[out] D1.l erroverrun / errpacketbufful / errqcodefufful /
 * errpackfufful
 */
#define CDB_SUBCODE_STATUS 0x0091

/**
 * @def CDB_SCDREAD
 * @brief Reads R through W subcode channels
 * @clobber d0-d1/a1
 * @ingroup bios_subcode
 *
 * @param[in] A0.l Address of the Q code buffer (minimum 24 bytes)
 * @param[out] CC Read successful
 * @param[out] CS Read failed
 * @param[out] A0.l Address of next Q code buffer (A0.l + 24)
 *
 */
#define CDB_SCDREAD 0x0092

/**
 * @def CDB_SCDPQ
 * @brief Gets P & Q codes from subcode
 * @clobber d0-d1/a1
 * @ingroup bios_subcode
 *
 * @param[in] A0.l Address of the Q code buffer (minimum 12 bytes)
 * @param[out] CC Read successful
 * @param[out] CS Read failed
 * @param[out] A0.l Address of next Q code buffer (A0.l + 12)
 *
 */
#define CDB_SCDPQ 0x0093

/**
 * @def CDB_SCDPQL
 * @brief Gets the last P & Q codes
 * @clobber d0-d1/a1
 * @ingroup bios_subcode
 *
 * @param[in] A0.l Address of the Q code buffer (minimum 12 bytes)
 * @param[out] CC Read successful
 * @param[out] CS Read failed
 * @param[out] A0.l Address of next Q code buffer (A0.l + 12)
 *
 */
#define CDB_SCDPQL 0x0094

/**
 * @def CDB_LEDSET
 * @brief Controls the status LEDs on the front of the CD unit
 * @clobber d0-d1/a0-a1
 * @ingroup sub_bios_misc
 *
 * @param[in] D1.w Status code
 *
 * @details
 * |MODE          |Green|Red  |System Indication
 * |:------------:|:---:|:---:|:----------------:|
 * |BIOS_LED_READY (0)  |on   |blink|CD ready / no disk|
 * |BIOS_LED_DISCIN (1) |on   |off  |CD ready / disk ok|
 * |BIOS_LED_ACCESS (2) |on   |on   |CD accessing      |
 * |BIOS_LED_STANDBY (3)|blink|off  |standby mode      |
 * |BIOS_LED_ERROR (4) |blink|blink|reserved          |
 * |BIOS_LED_MODE5 (5)  |blink|on   |reserved          |
 * |BIOS_LED_MODE6 (6)  |off  |blink|reserved          |
 * |BIOS_LED_MODE7 (7)  |off  |on   |reserved          |
 * |BIOS_LED_SYSTEM (?) |     |     |return control to BIOS|
 *
 */
#define BIOS_LEDSET 0x0095

#define BIOS_LED_READY   0
#define BIOS_LED_DISCIN  1
#define BIOS_LED_ACCESS  2
#define BIOS_LED_STANDBY 3
#define BIOS_LED_ERROR   4
#define BIOS_LED_MODE5   5
#define BIOS_LED_MODE6   6
#define BIOS_LED_MODE7   7

/**
 * @def CDB_CDCSETMODE
 * @brief Sets the mode in which the CD should be read
 * @clobber UNKNOWN (TODO)
 * @ingroup sub_bios_cdc
 *
 * @param[in] D1.w Mode bit flags
 *
 * @note Use the predefined BIOS_CDC_MODE0, BIOS_CDC_MODE1, BIOS_CDC_MODE2
 * values to set the CDC mode
 *
 * @details
 * d1.w  FEDCBA9876543210
 *                   ||||
 *                   |||+--> CD Mode 2
 *                   ||+---> CD-DA mode
 *                   |+----> transfer error block with data
 *                   +-----> re-read last data
 *   These are the allowed bit fields
 *   Bit
 *    0     CD Mode 2
 *    1     CD-DA Mode
 *    2     Transfer error block with data
 *    3     Re-read last data
 */
#define CDB_CDCSETMODE 0x0096

/**
 * @def BIOS_CDC_MODE0
 * @brief Put CDC in CD Audio (CDDA) mode
 */
#define BIOS_CDC_MODE0 2

/**
 * @def BIOS_CDC_MODE1
 * @brief Put CDC in full error correction mode
 */
#define BIOS_CDC_MODE1 0

/**
 * @def BIOS_CDC_MODE2
 * @brief Put CDC in CRC check only mode
 */
#define BIOS_CDC_MODE2 1

/**
 * @def CDB_WONDERREQ
 * @brief No documentation; research needed. Presumably exclusive to the
 *   WonderMega hardware
 * @ingroup sub_bios_unknown
 */
#define CDB_WONDERREQ 0x0097

/**
 * @def CDB_WONDERCHK
 * @brief No documentation; research needed. Presumably exclusive to the
 *   WonderMega hardware
 * @ingroup sub_bios_unknown
 */
#define CDB_WONDERCHK 0x0098

/**
 * @def CDB_UNKNOWN00
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup sub_bios_unknown
 */
#define CDB_UNKNOWN00 0x0000

/**
 * @def CDB_UNKNOWN01
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup sub_bios_unknown
 */
#define CDB_UNKNOWN01 0x0001

/**
 * @def CDB_UNKNOWN11
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup sub_bios_unknown
 */
#define CDB_UNKNOWN11 0x0011

/**
 * @def CDB_UNKNOWN12
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup sub_bios_unknown
 */
#define CDB_UNKNOWN12 0x0012

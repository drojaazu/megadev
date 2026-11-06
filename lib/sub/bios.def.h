/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub/bios.def.h
 * @brief Sub CPU side system library vectors & memory definitions
 */

#pragma once

/**
 * @defgroup bios_cdda Sub CPU / BIOS / CD-DA
 */

/**
 * @defgroup bios_cdrom Sub CPU / BIOS / CD-ROM
 */

/**
 * @defgroup bios_cdb Sub CPU / BIOS / CDB
 */

/**
 * @defgroup bios_cdc Sub CPU / BIOS / CDC
 */

/**
 * @defgroup bios_fader Sub CPU / BIOS / Fader
 */

/**
 * @defgroup bios_subcode Sub CPU / BIOS / Subcode data
 */

/**
 * @defgroup bios_bram Sub CPU / BIOS / Backup RAM
 */

/**
 * @defgroup bios_misc_sub Sub CPU / BIOS / Misc
 */

/**
 * @defgroup bios_unknown Sub CPU / BIOS / Undocumented calls
 */

/**
 * @def BIOS_BOOT_STATUS
 * @segaalias _BOOTSTAT
 */
#define BIOS_BOOT_STATUS 0x005EA0

/**
 * @def BIOS_INT2_FLAG
 * @segaalias _INT2FLAG
 */
#define BIOS_INT2_FLAG 0x005EA4

/**
 * @def BIOS_USER_MODE
 * @segaalias _USERMODE
 */
#define BIOS_USER_MODE 0x005EA6

/**
 * @def BIOS_CD_STATUS
 * @brief Contains the status after calling @ref BIOS_GET_STATUS
 * @segaalias _CDSTAT
 */
#define BIOS_CD_STATUS 0x005E80

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
 * @segaalias _SETJMPTBL
 */
#define BIOS_SET_JUMP_TABLE 0x005F0A

/**
 * @def BIOS_WAIT_VSYNC
 * @segaalias _WAITVSYNC
 */
#define BIOS_WAIT_VSYNC 0x005F10

/**
 * @def SP_INIT
 * @brief Sub CPU program entry: initialization, called once at boot
 * @segaalias _USERCALL0
 */
#define SP_INIT 0x005F28

/**
 * @def SP_MAIN
 * @brief Sub CPU program entry: main routine
 * @segaalias _USERCALL1
 */
#define SP_MAIN 0x005F2E

/**
 * @def SP_INT2
 * @brief Sub CPU program entry: level 2 interrupt handler
 * @segaalias _USERCALL2
 */
#define SP_INT2 0x005F34

/**
 * @def SP_USER
 * @brief Sub CPU program entry: user routine
 * @segaalias _USERCALL3
 */
#define SP_USER 0x005F3A

/**
 * @def BIOS_CALL_VECTOR
 * @brief General system calls vector
 * @segaalias _CDBIOS
 */
#define BIOS_CALL_VECTOR 0x005F22

/**
 * @def BIOS_UNKNOWN00
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup bios_unknown
 */
#define BIOS_UNKNOWN00 0x0000

/**
 * @def BIOS_UNKNOWN01
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup bios_unknown
 */
#define BIOS_UNKNOWN01 0x0001

/**
 * @def BIOS_AUDIO_STOP
 * @sa bios_audio_stop
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 * @segaalias MSCSTOP
 */
#define BIOS_AUDIO_STOP 0x0002

/**
 * @def BIOS_AUDIO_PAUSE
 * @sa bios_audio_pause
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 * @segaalias MSCPAUSEON
 */
#define BIOS_AUDIO_PAUSE 0x0003

/**
 * @def BIOS_AUDIO_RESUME
 * @sa bios_audio_resume
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 * @segaalias MSCPAUSEOFF
 */
#define BIOS_AUDIO_RESUME 0x0004

/**
 * @def BIOS_AUDIO_SCAN_FORWARD
 * @sa bios_audio_scan_forward
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 * @segaalias MSCSCANFF
 */
#define BIOS_AUDIO_SCAN_FORWARD 0x0005

/**
 * @def BIOS_AUDIO_SCAN_BACKWARD
 * @sa bios_audio_scan_backward
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 * @segaalias MSCSCANFR
 */
#define BIOS_AUDIO_SCAN_BACKWARD 0x0006

/**
 * @def BIOS_AUDIO_SCAN_OFF
 * @sa bios_audio_scan_off
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 * @segaalias MSCSCANOFF
 */
#define BIOS_AUDIO_SCAN_OFF 0x0007

/**
 * @def BIOS_CDROM_PAUSE
 * @sa bios_cdrom_pause
 * @ingroup bios_cdrom
 * @clobber d0-d1/a0-a1
 * @segaalias ROMPAUSEON
 */
#define BIOS_CDROM_PAUSE 0x0008

/**
 * @def BIOS_CDROM_RESUME
 * @sa bios_cdrom_resume
 * @ingroup bios_cdrom
 * @clobber d0-d1/a0-a1
 * @segaalias ROMPAUSEOFF
 */
#define BIOS_CDROM_RESUME 0x0009

/**
 * @def BIOS_DRIVE_OPEN
 * @sa bios_drive_open
 * @ingroup bios_misc_sub
 * @clobber d0-d1/a0-a1
 * @segaalias DRVOPEN
 */
#define BIOS_DRIVE_OPEN 0x000A

/**
 * @def BIOS_DRIVE_INIT
 * @sa bios_drive_init
 * @clobber d0-d1/a1
 *
 * @param[in] A0.l Pointer to initilization parameters
 *
 * @details Takes a pointer to two bytes (initialization params):
 *  byte 1 - track number from which to read TOC (normally 0x01); if bit 7 of
 *           this value is set, BIOS will start to play the first track
             automatically
 *  byte 2 - last track to read (0xFF will read all tracks)
 * @segaalias DRVINIT
 */
#define BIOS_DRIVE_INIT 0x0010

/**
 * @def BIOS_UNKNOWN11
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup bios_unknown
 */
#define BIOS_UNKNOWN11 0x0011

/**
 * @def BIOS_UNKNOWN12
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup bios_unknown
 */
#define BIOS_UNKNOWN12 0x0012

/**
 * @def BIOS_AUDIO_PLAY
 * @sa bios_audio_play
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 * @segaalias MSCPLAY
 */
#define BIOS_AUDIO_PLAY 0x0011

/**
 * @def BIOS_AUDIO_PLAY_ONCE
 * @sa bios_audio_play_once
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 * @segaalias MSCPLAY1
 */
#define BIOS_AUDIO_PLAY_ONCE 0x0012

/**
 * @def BIOS_AUDIO_PLAY_REPEAT
 * @sa bios_audio_play_repeat
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 * @segaalias MSCPLAYR
 */
#define BIOS_AUDIO_PLAY_REPEAT 0x0013

/**
 * @def BIOS_AUDIO_PLAY_FROM_TIME
 * @sa bios_audio_play_from_time
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to BCD time code in the format mm:ss:ff:00
 *   (32 bit)
 * @segaalias MSCPLAYT
 */
#define BIOS_AUDIO_PLAY_FROM_TIME 0x0014

/**
 * @def BIOS_AUDIO_SEEK
 * @sa bios_audio_seek
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 * @segaalias MSCSEEK
 */
#define BIOS_AUDIO_SEEK 0x0015

/**
 * @def BIOS_AUDIO_SEEK_TIME
 * @sa bios_audio_seek_time
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to BCD time code in the format mm:ss:ff:00
 *   (32 bit)
 * @segaalias MSCSEEKT
 */
#define BIOS_AUDIO_SEEK_TIME 0x0016

/**
 * @def BIOS_CDROM_READ
 * @sa bios_cdrom_read
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdrom
 *
 * @param[in] A0.l Pointer to the logical sector number (32 bit)
 * @segaalias ROMREAD
 */
#define BIOS_CDROM_READ 0x0017

/**
 * @def BIOS_CDROM_SEEK
 * @sa bios_cdrom_seek
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdrom
 *
 * @param[in] A0.l Pointer to the logical sector number (32 bit)
 * @segaalias ROMSEEK
 */
#define BIOS_CDROM_SEEK 0x0018

/**
 * @def BIOS_AUDIO_SEEK_PLAY_ONCE
 * @sa bios_audio_seek_play_once
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 * @segaalias MSCSEEK1
 */
#define BIOS_AUDIO_SEEK_PLAY_ONCE 0x0019

/**
 * @def BIOS_CDROM_READ_COUNT
 * @sa bios_cdrom_read_count
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdrom
 *
 * @param[in] A0.l Pointer to a sector read info structure
 *
 * @details
 * The structure is made up of two 32 bit values:
 *   dc.l 0x00000001   // First sector to read
 *   dc.l 0x00001234   // Number of sectors to read
 * @segaalias ROMREADN
 */
#define BIOS_CDROM_READ_COUNT 0x0020

/**
 * @def BIOS_CDROM_READ_RANGE
 * @sa bios_cdrom_read_range
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdrom
 *
 * @param[in] A0.l Pointer to a sector read info structure
 *
 * @details The structure is made up of two 32 bit values:
 *   dc.l 0x00000001   // First sector to read
 *   dc.l 0x00001234   // Number of logical sectors to read
 * OUT:
 *  none
 * @segaalias ROMREADE
 */
#define BIOS_CDROM_READ_RANGE 0x0021

/**
 * @def BIOS_CHECK_COMMAND
 * @sa bios_check_command
 * @clobber d0
 * @ingroup bios_cdb
 *
 * @param[out] CC Command complete
 * @param[out] CS BIOS is busy
 * @segaalias CDBCHK
 */
#define BIOS_CHECK_COMMAND 0x0080

/**
 * @def BIOS_GET_STATUS
 * @sa bios_get_status
 * @clobber d0-d1/a0
 * @ingroup bios_cdb
 *
 * @param[out] A0.l Pointer to status info structure
 *
 * @note
 * Documentation says A1 is clobbered as well, but we don't see any
 * sign of that in the disassemblies checked so far
 * @segaalias CDBSTAT
 */
#define BIOS_GET_STATUS 0x0081

/**
 * @def BIOS_TOC_WRITE
 * @brief Writes data to disc TOC stored in memory
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdb
 *
 * @param[in] A0.l Pointer to TOC data
 *
 * @details A TOC entry is four bytes long, where the first three bytes are
 * the BCD timecode of the start of the track and the lowest byte is the track
 * number. The most significant bit of the frame byte of the timecode indicates
 * whether the track is CD-ROM or CD-DA. Please refer to the BIOS manual
 * for more information.
 * @segaalias CDBTOCWRITE
 */
#define BIOS_TOC_WRITE 0x0082

/**
 * @def BIOS_TOC_READ
 * @brief Reads the TOC entry for a given track
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdb
 *
 * @param[in] D1.w Track number
 * @param[out] D0.l Track start timecode (BCD, low byte is track number)
 * @param[out] D1.b Track type (00 - CD-DA, 0xFF - CD-ROM)
 *
 * @note The low byte of D0 will be 0 on error.
 * @segaalias CDBTOCREAD
 */
#define BIOS_TOC_READ 0x0083

/**
 * @def BIOS_SET_PAUSE_TIMEOUT
 * @brief Sets the time that the drive spins down from pause to standby
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdb
 *
 * @param[in] D1.w Pause time (in ticks of 1/75 second)
 *
 * @details Normal ranges for this delay time are 0x1194 - 0xFFFE. A delay of
 * 0xFFFF prevents the drive from stopping, but can damage the drive if used
 * improperly.
 * @segaalias CDBPAUSE
 */
#define BIOS_SET_PAUSE_TIMEOUT 0x0084

/**
 * @def BIOS_FADER_SET
 * @sa bios_fader_set
 * @clobber d0-d1/a0
 * @ingroup bios_fader
 *
 * @param[in] D1.w Volume
 * @segaalias FDRSET
 */
#define BIOS_FADER_SET 0x0085

/**
 * @def BIOS_FADER_CHANGE
 * @sa bios_fader_change
 * @clobber d0-d1/a0
 * @ingroup bios_fader
 *
 * @param[in] D1.l Volume & Ramp
 * @segaalias FDRCHG
 */
#define BIOS_FADER_CHANGE 0x0086

/**
 * @def BIOS_CDC_START
 * @brief Starts reading data from the current logical sector into the CDC
 * @clobber d0-d1/a0
 * @ingroup bios_cdc
 *
 * @note The BIOS pre-seeks by 2 to 4 sectors and data read actually begins
 * before the requested sector. It is up to the caller to identify the correct
 * starting sector (usually by checking the time codes in the headers as
 * they are read from the CDC buffer).
 * @segaalias CDCSTART
 */
#define BIOS_CDC_START 0x0087

/**
 * @def BIOS_CDC_START_P
 * @brief No official documentation
 * @ingroup bios_unknown
 * @segaalias CDCSTARTP
 */
#define BIOS_CDC_START_P 0x0088

/**
 * @def BIOS_CDC_STOP
 * @brief Stop reading data into the CDC buffer
 * @clobber d0/a0
 * @ingroup bios_cdc
 *
 * @note If a sector is being read when CDC_STOP is called, it is discarded.
 * @segaalias CDCSTOP
 */
#define BIOS_CDC_STOP 0x0089

/**
 * @def BIOS_CDC_STATUS
 * @brief Query the status of the CDC buffer
 * @clobber d0-d1/a0
 * @ingroup bios_cdc
 *
 * @param[out] CC Sector available for read
 * @param[out] CS Sector not ready
 * @segaalias CDCSTAT
 */
#define BIOS_CDC_STATUS 0x008A

/**
 * @def BIOS_CDC_READ
 * @brief Reads sector of data in preparation for transfer
 * @clobber d0-d1/a0
 * @ingroup bios_cdc
 *
 * @param[out] CC Sector ready for transfer
 * @param[out] CS Sector not ready
 * @param[out] D0.l Sector header timecode (BCD)
 * @details The low byte of D0 is the sector mode:
 *   00 -bios_cdda
 *   01 -bios_cdrom mode 1
 *   02 -bios_cdrom mode 2
 *
 * @note Be sure to set the device destination register BEFORE calling CDCREAD!
 * @segaalias CDCREAD
 */
#define BIOS_CDC_READ 0x008B

/**
 * @def BIOS_CDC_TRANSFER
 * @brief Transfer one sector of data from the CDC to Sub CPU RAM
 * @clobber d0-d1/a0
 * @ingroup bios_cdc
 *
 * @param[in] A0.l Pointer to sector destination buffer (0x920 bytes)
 * @param[in] A1.l Pointer to header destination buffer (4 bytes)
 * @param[out] CC Sector transfer completed successfully
 * @param[out] CS Sector transfer failed
 * @param[out] A0.l Pointer to next sector destination address (A0 + 0x920)
 * @param[out] A1.l Pointer to next header destination address (A1 + 4)
 *
 * @note The device destination must be set to Sub CPU read beforehand!
 * @segaalias CDCTRN
 */
#define BIOS_CDC_TRANSFER 0x008C

/**
 * @def BIOS_CDC_ACK
 * @brief Informs the CDC that the current sector has been read and that the
 * caller is ready for the next sector
 * @clobber d0
 * @ingroup bios_cdc
 * @segaalias CDCACK
 */
#define BIOS_CDC_ACK 0x008D

/**
 * @def BIOS_SUBCODE_INIT
 * @brief Initializes the BIOS for subcode reads
 * @ingroup bios_subcode
 *
 * @param[in] A0.l Pointer to work buffer (at least 0x750 bytes)
 * @segaalias SCDINIT
 */
#define BIOS_SUBCODE_INIT 0x008E

/**
 * @def BIOS_SUBCODE_START
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
 * @segaalias SCDSTART
 */
#define BIOS_SUBCODE_START 0x008F

/**
 * @def BIOS_SUBCODE_STOP
 * @brief Disables reading subcode data by the CDC
 * @clobber d0-d1/a0-a1
 * @ingroup bios_subcode
 * @segaalias SCDSTOP
 */
#define BIOS_SUBCODE_STOP 0x0090

/**
 * @def BIOS_SUBCODE_STATUS
 * @brief Check subcode error status
 * @clobber d0-d1/a0-a1
 * @ingroup bios_subcode
 *
 * @param[out] D0.l errqcodecrc / errpackcirc / scdflag / restrcnt
 * @param[out] D1.l erroverrun / errpacketbufful / errqcodefufful /
 * errpackfufful
 * @segaalias SCDSTAT
 */
#define BIOS_SUBCODE_STATUS 0x0091

/**
 * @def BIOS_SUBCODE_READ
 * @brief Reads R through W subcode channels
 * @clobber d0-d1/a1
 * @ingroup bios_subcode
 *
 * @param[in] A0.l Address of the Q code buffer (minimum 24 bytes)
 * @param[out] CC Read successful
 * @param[out] CS Read failed
 * @param[out] A0.l Address of next Q code buffer (A0.l + 24)
 *
 * @segaalias SCDREAD
 */
#define BIOS_SUBCODE_READ 0x0092

/**
 * @def BIOS_SUBCODE_READ_PQ
 * @brief Gets P & Q codes from subcode
 * @clobber d0-d1/a1
 * @ingroup bios_subcode
 *
 * @param[in] A0.l Address of the Q code buffer (minimum 12 bytes)
 * @param[out] CC Read successful
 * @param[out] CS Read failed
 * @param[out] A0.l Address of next Q code buffer (A0.l + 12)
 *
 * @segaalias SCDPQ
 */
#define BIOS_SUBCODE_READ_PQ 0x0093

/**
 * @def BIOS_SUBCODE_READ_PQ_LAST
 * @brief Gets the last P & Q codes
 * @clobber d0-d1/a1
 * @ingroup bios_subcode
 *
 * @param[in] A0.l Address of the Q code buffer (minimum 12 bytes)
 * @param[out] CC Read successful
 * @param[out] CS Read failed
 * @param[out] A0.l Address of next Q code buffer (A0.l + 12)
 *
 * @segaalias SCDPQL
 */
#define BIOS_SUBCODE_READ_PQ_LAST 0x0094

/**
 * @def BIOS_LED_SET
 * @brief Controls the status LEDs on the front of the CD unit
 * @clobber d0-d1/a0-a1
 * @ingroup bios_misc_sub
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
 * @segaalias LEDSET
 */
#define BIOS_LED_SET 0x0095

/** @name LED modes for BIOS_LED_SET; see the table above */
///@{
#define BIOS_LED_READY   0 ///< Ready: green on, red off
#define BIOS_LED_DISCIN  1 ///< Disc in: green on, red on
#define BIOS_LED_ACCESS  2 ///< Access: green on, red blinking
#define BIOS_LED_STANDBY 3 ///< Standby: green blinking, red off
#define BIOS_LED_ERROR   4 ///< Error: both blinking
#define BIOS_LED_MODE5   5 ///< Reserved: green blinking, red on
#define BIOS_LED_MODE6   6 ///< Reserved: green off, red blinking
#define BIOS_LED_MODE7   7 ///< Reserved: green off, red on
///@}

/**
 * @def BIOS_CDC_SET_MODE
 * @brief Sets the mode in which the CD should be read
 * @clobber UNKNOWN (TODO)
 * @ingroup bios_cdc
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
 * @segaalias CDCSETMODE
 */
#define BIOS_CDC_SET_MODE 0x0096

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
 * @def BIOS_WONDER_REQUEST
 * @brief No official documentation. Presumably exclusive to the
 *   WonderMega hardware
 * @ingroup bios_unknown
 * @segaalias WONDERREQ
 */
#define BIOS_WONDER_REQUEST 0x0097

/**
 * @def BIOS_WONDER_CHECK
 * @brief No official documentation. Presumably exclusive to the
 *   WonderMega hardware
 * @ingroup bios_unknown
 * @segaalias WONDERCHK
 */
#define BIOS_WONDER_CHECK 0x0098

/**
 * @defgroup sub_bios_sega Sega documentation names
 * @brief The names Sega's documentation uses for the definitions above
 * @{
 */
/** @aliasof BIOS_AUDIO_STOP */
#define MSCSTOP BIOS_AUDIO_STOP
/** @aliasof BIOS_AUDIO_PAUSE */
#define MSCPAUSEON BIOS_AUDIO_PAUSE
/** @aliasof BIOS_AUDIO_RESUME */
#define MSCPAUSEOFF BIOS_AUDIO_RESUME
/** @aliasof BIOS_AUDIO_SCAN_FORWARD */
#define MSCSCANFF BIOS_AUDIO_SCAN_FORWARD
/** @aliasof BIOS_AUDIO_SCAN_BACKWARD */
#define MSCSCANFR BIOS_AUDIO_SCAN_BACKWARD
/** @aliasof BIOS_AUDIO_SCAN_OFF */
#define MSCSCANOFF BIOS_AUDIO_SCAN_OFF
/** @aliasof BIOS_CDROM_PAUSE */
#define ROMPAUSEON BIOS_CDROM_PAUSE
/** @aliasof BIOS_CDROM_RESUME */
#define ROMPAUSEOFF BIOS_CDROM_RESUME
/** @aliasof BIOS_DRIVE_OPEN */
#define DRVOPEN BIOS_DRIVE_OPEN
/** @aliasof BIOS_DRIVE_INIT */
#define DRVINIT BIOS_DRIVE_INIT
/** @aliasof BIOS_AUDIO_PLAY */
#define MSCPLAY BIOS_AUDIO_PLAY
/** @aliasof BIOS_AUDIO_PLAY_ONCE */
#define MSCPLAY1 BIOS_AUDIO_PLAY_ONCE
/** @aliasof BIOS_AUDIO_PLAY_REPEAT */
#define MSCPLAYR BIOS_AUDIO_PLAY_REPEAT
/** @aliasof BIOS_AUDIO_PLAY_FROM_TIME */
#define MSCPLAYT BIOS_AUDIO_PLAY_FROM_TIME
/** @aliasof BIOS_AUDIO_SEEK */
#define MSCSEEK BIOS_AUDIO_SEEK
/** @aliasof BIOS_AUDIO_SEEK_TIME */
#define MSCSEEKT BIOS_AUDIO_SEEK_TIME
/** @aliasof BIOS_AUDIO_SEEK_PLAY_ONCE */
#define MSCSEEK1 BIOS_AUDIO_SEEK_PLAY_ONCE
/** @aliasof BIOS_CDROM_READ */
#define ROMREAD BIOS_CDROM_READ
/** @aliasof BIOS_CDROM_SEEK */
#define ROMSEEK BIOS_CDROM_SEEK
/** @aliasof BIOS_CDROM_READ_COUNT */
#define ROMREADN BIOS_CDROM_READ_COUNT
/** @aliasof BIOS_CDROM_READ_RANGE */
#define ROMREADE BIOS_CDROM_READ_RANGE
/** @aliasof BIOS_CHECK_COMMAND */
#define CDBCHK BIOS_CHECK_COMMAND
/** @aliasof BIOS_GET_STATUS */
#define CDBSTAT BIOS_GET_STATUS
/** @aliasof BIOS_TOC_WRITE */
#define CDBTOCWRITE BIOS_TOC_WRITE
/** @aliasof BIOS_TOC_READ */
#define CDBTOCREAD BIOS_TOC_READ
/** @aliasof BIOS_SET_PAUSE_TIMEOUT */
#define CDBPAUSE BIOS_SET_PAUSE_TIMEOUT
/** @aliasof BIOS_FADER_SET */
#define FDRSET BIOS_FADER_SET
/** @aliasof BIOS_FADER_CHANGE */
#define FDRCHG BIOS_FADER_CHANGE
/** @aliasof BIOS_CDC_START */
#define CDCSTART BIOS_CDC_START
/** @aliasof BIOS_CDC_START_P */
#define CDCSTARTP BIOS_CDC_START_P
/** @aliasof BIOS_CDC_STOP */
#define CDCSTOP BIOS_CDC_STOP
/** @aliasof BIOS_CDC_STATUS */
#define CDCSTAT BIOS_CDC_STATUS
/** @aliasof BIOS_CDC_READ */
#define CDCREAD BIOS_CDC_READ
/** @aliasof BIOS_CDC_TRANSFER */
#define CDCTRN BIOS_CDC_TRANSFER
/** @aliasof BIOS_CDC_ACK */
#define CDCACK BIOS_CDC_ACK
/** @aliasof BIOS_CDC_SET_MODE */
#define CDCSETMODE BIOS_CDC_SET_MODE
/** @aliasof BIOS_SUBCODE_INIT */
#define SCDINIT BIOS_SUBCODE_INIT
/** @aliasof BIOS_SUBCODE_START */
#define SCDSTART BIOS_SUBCODE_START
/** @aliasof BIOS_SUBCODE_STOP */
#define SCDSTOP BIOS_SUBCODE_STOP
/** @aliasof BIOS_SUBCODE_STATUS */
#define SCDSTAT BIOS_SUBCODE_STATUS
/** @aliasof BIOS_SUBCODE_READ */
#define SCDREAD BIOS_SUBCODE_READ
/** @aliasof BIOS_SUBCODE_READ_PQ */
#define SCDPQ BIOS_SUBCODE_READ_PQ
/** @aliasof BIOS_SUBCODE_READ_PQ_LAST */
#define SCDPQL BIOS_SUBCODE_READ_PQ_LAST
/** @aliasof BIOS_LED_SET */
#define LEDSET BIOS_LED_SET
/** @aliasof BIOS_WONDER_REQUEST */
#define WONDERREQ BIOS_WONDER_REQUEST
/** @aliasof BIOS_WONDER_CHECK */
#define WONDERCHK BIOS_WONDER_CHECK
/** @aliasof BIOS_CALL_VECTOR */
#define CDBIOS BIOS_CALL_VECTOR
/** @aliasof BIOS_WAIT_VSYNC */
#define WAITVSYNC BIOS_WAIT_VSYNC
/** @aliasof BIOS_SET_JUMP_TABLE */
#define SETJMPTBL BIOS_SET_JUMP_TABLE
/** @aliasof BIOS_CD_STATUS */
#define CDSTAT BIOS_CD_STATUS
/** @aliasof BIOS_BOOT_STATUS */
#define BOOTSTAT BIOS_BOOT_STATUS
/** @aliasof BIOS_USER_MODE */
#define USERMODE BIOS_USER_MODE
/** @aliasof BIOS_INT2_FLAG */
#define INT2FLAG BIOS_INT2_FLAG
/** @aliasof SP_INIT */
#define USERCALL0 SP_INIT
/** @aliasof SP_MAIN */
#define USERCALL1 SP_MAIN
/** @aliasof SP_INT2 */
#define USERCALL2 SP_INT2
/** @aliasof SP_USER */
#define USERCALL3 SP_USER
/** @} */

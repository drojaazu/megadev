/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub/bios.def.h
 * @brief Sub CPU side system library vectors & memory definitions
 */

#ifndef MEGADEV__SUBBIOS_DEF_H
#define MEGADEV__SUBBIOS_DEF_H

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
 * @defgroup bios_misc Sub CPU / BIOS / Misc
 */

/**
 * @defgroup bios_unknown Sub CPU / BIOS / Undocumented calls
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

/*
 * @def CDSTAT
 * @brief Contains the status after calling @ref _CDBSTAT
 */
#define CDSTAT 0x005E80

/**
 * System Jump Table
 * IN:
 * A1 ptr to user header (see MEGA CD BIOS MANUAL)
 * A0 ptr to jump table
 * BREAK A2
 * NOTES
 * expects the user header to begin with MAIN
 * also checks for SYS, SUB, or DAT, but these must appear beginning the 8th
 * byte (it looks like there is no tangible difference between these) the flag
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
 * @def USERCALL0
 */
#define USERCALL0 0x005F28 /* SP Init */

/**
 * @def USERCALL1
 */
#define USERCALL1 0x005F2E /* SP Main */

/**
 * @def USERCALL2
 */
#define USERCALL2 0x005F34 /* SP INT2 */

/**
 * @def USERCALL3
 */
#define USERCALL3 0x005F3A /* SP User Call */

/**
 * @def CDBIOS
 * @brief General system calls vector
 */
#define CDBIOS 0x005F22

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
 * @def BIOS_MSCSTOP
 * @sa bios_mscstop
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define BIOS_MSCSTOP 0x0002

/**
 * @def BIOS_MSCPAUSEON
 * @sa bios_mscpauseon
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define BIOS_MSCPAUSEON 0x0003

/**
 * @def BIOS_MSCPAUSEOFF
 * @sa bios_mscpauseoff
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define BIOS_MSCPAUSEOFF 0x0004

/**
 * @def BIOS_MSCSCANFF
 * @sa bios_mscscanff
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define BIOS_MSCSCANFF 0x0005

/**
 * @def BIOS_MSCSCANFR
 * @sa bios_mscscanfr
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define BIOS_MSCSCANFR 0x0006

/**
 * @def BIOS_MSCSCANOFF
 * @sa bios_mscscanoff
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define BIOS_MSCSCANOFF 0x0007

/**
 * @def BIOS_ROMPAUSEON
 * @sa bios_rompauseon
 * @ingroup bios_cdrom
 * @clobber d0-d1/a0-a1
 */
#define BIOS_ROMPAUSEON 0x0008

/**
 * @def BIOS_ROMPAUSEOFF
 * @sa bios_rompauseoff
 * @ingroup bios_cdrom
 * @clobber d0-d1/a0-a1
 */
#define BIOS_ROMPAUSEOFF 0x0009

/**
 * @def BIOS_DRVOPEN
 * @sa bios_drvopen
 * @ingroup bios_misc
 * @clobber d0-d1/a0-a1
 */
#define BIOS_DRVOPEN 0x000A

/**
 * @def BIOS_DRVINIT
 * @sa bios_drvinit
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
#define BIOS_DRVINIT 0x0010

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
 * @def BIOS_MSCPLAY
 * @sa bios_mscplay
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define BIOS_MSCPLAY 0x0011

/**
 * @def BIOS_MSCPLAY1
 * @sa bios_mscplay1
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define BIOS_MSCPLAY1 0x0012

/**
 * @def BIOS_MSCPLAYR
 * @sa bios_mscplayr
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define BIOS_MSCPLAYR 0x0013

/**
 * @def BIOS_MSCPLAYT
 * @sa bios_mscplayt
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to BCD time code in the format mm:ss:ff:00
 *   (32 bit)
 */
#define BIOS_MSCPLAYT 0x0014

/**
 * @def BIOS_MSCSEEK
 * @sa bios_mscseek
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define BIOS_MSCSEEK 0x0015

/**
 * @def BIOS_MSCSEEKT
 * @sa bios_mscseekt
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to BCD time code in the format mm:ss:ff:00
 *   (32 bit)
 */
#define BIOS_MSCSEEKT 0x0016

/**
 * @def BIOS_ROMREAD
 * @sa bios_romread
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdrom
 *
 * @param[in] A0.l Pointer to the logical sector number (32 bit)
 */
#define BIOS_ROMREAD 0x0017

/**
 * @def BIOS_ROMSEEK
 * @sa bios_romseek
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdrom
 *
 * @param[in] A0.l Pointer to the logical sector number (32 bit)
 */
#define BIOS_ROMSEEK 0x0018

/**
 * @def BIOS_MSCSEEK1
 * @sa bios_mscseek1
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define BIOS_MSCSEEK1 0x0019

/**
 * @def BIOS_ROMREADN
 * @sa bios_romreadn
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdrom
 *
 * @param[in] A0.l Pointer to a sector read info structure
 *
 * @details
 * The structure is made up of two 32 bit values:
 *   dc.l 0x00000001   // First sector to read
 *   dc.l 0x00001234   // Number of sectors to read
 */
#define BIOS_ROMREADN 0x0020

/**
 * @def BIOS_ROMREADE
 * @sa bios_romreade
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
 */
#define BIOS_ROMREADE 0x0021

/**
 * @def BIOS_CDBCHK
 * @sa bios_cdbchk
 * @clobber d0
 * @ingroup bios_cdb
 *
 * @param[out] CC Command complete
 * @param[out] CS BIOS is busy
 */
#define BIOS_CDBCHK 0x0080

/**
 * @def BIOS_CDBSTAT
 * @sa bios_cdbstat
 * @clobber d0-d1/a0
 * @ingroup bios_cdb
 *
 * @param[out] A0.l Pointer to status info structure
 *
 * @note
 * Documentation says A1 is clobbered as well, but we don't see any
 * sign of that in the disassemblies checked so far
 */
#define BIOS_CDBSTAT 0x0081

/**
 * @def BIOS_CDBTOCWRITE
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
 */
#define BIOS_CDBTOCWRITE 0x0082

/**
 * @def BIOS_CDBTOCREAD
 * @brief Reads the TOC entry for a given track
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdb
 *
 * @param[in] D1.w Track number
 * @param[out] D0.l Track start timecode (BCD, low byte is track number)
 * @param[out] D1.b Track type (00 - CD-DA, 0xFF - CD-ROM)
 *
 * @note The low byte of D0 will be 0 on error.
 */
#define BIOS_CDBTOCREAD 0x0083

/**
 * @def BIOS_CDBPAUSE
 * @brief Sets the time that the drive spins down from pause to standby
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdb
 *
 * @param[in] D1.w Pause time (in ticks of 1/75 second)
 *
 * @details Normal ranges for this delay time are 0x1194 - 0xFFFE. A delay of
 * 0xFFFF prevents the drive from stopping, but can damage the drive if used
 * improperly.
 */
#define BIOS_CDBPAUSE 0x0084

/**
 * @def BIOS_FDRSET
 * @sa bios_fdrset
 * @clobber d0-d1/a0
 * @ingroup bios_fader
 *
 * @param[in] D1.w Volume
 */
#define BIOS_FDRSET 0x0085

/**
 * @def BIOS_FDRCHG
 * @sa bios_fdrchg
 * @clobber d0-d1/a0
 * @ingroup bios_fader
 *
 * @param[in] D1.l Volume & Ramp
 */
#define BIOS_FDRCHG 0x0086

/**
 * @def BIOS_CDCSTART
 * @brief Starts reading data from the current logical sector into the CDC
 * @clobber d0-d1/a0
 * @ingroup bios_cdc
 *
 * @note The BIOS pre-seeks by 2 to 4 sectors and data read actually begins
 * before the requested sector. It is up to the caller to identify the correct
 * starting sector (usually by checking the time codes in the headers as
 * they are read from the CDC buffer).
 */
#define BIOS_CDCSTART 0x0087

/**
 * @def BIOS_CDCSTARTP
 * @brief No official documentation on this call; needs to be researched
 * @ingroup bios_unknown
 */
#define BIOS_CDCSTARTP 0x0088

/**
 * @def BIOS_CDCSTOP
 * @brief Stop reading data into the CDC buffer
 * @clobber d0/a0
 * @ingroup bios_cdc
 *
 * @note If a sector is being read when CDCSTOP is called, it is discarded.
 */
#define BIOS_CDCSTOP 0x0089

/**
 * @def BIOS_CDCSTAT
 * @brief Query the status of the CDC buffer
 * @clobber d0-d1/a0
 * @ingroup bios_cdc
 *
 * @param[out] CC Sector available for read
 * @param[out] CS Sector not ready
 */
#define BIOS_CDCSTAT 0x008A

/**
 * @def BIOS_CDCREAD
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
 */
#define BIOS_CDCREAD 0x008B

/**
 * @def BIOS_CDCTRN
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
 */
#define BIOS_CDCTRN 0x008C

/**
 * @def BIOS_CDCACK
 * @brief Informs the CDC that the current sector has been read and that the
 * caller is ready for the next sector
 * @clobber d0
 * @ingroup bios_cdc
 */
#define BIOS_CDCACK 0x008D

/**
 * @def BIOS_SCDINIT
 * @brief Initializes the BIOS for subcode reads
 * @ingroup bios_subcode
 *
 * @param[in] A0.l Pointer to work buffer (at least 0x750 bytes)
 */
#define BIOS_SCDINIT 0x008E

/**
 * @def BIOS_SCDSTART
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
#define BIOS_SCDSTART 0x008F

/**
 * @def BIOS_SCDSTOP
 * @brief Disables reading subcode data by the CDC
 * @clobber d0-d1/a0-a1
 * @ingroup bios_subcode
 */
#define BIOS_SCDSTOP 0x0090

/**
 * @def BIOS_SCDSTAT
 * @brief Check subcode error status
 * @clobber d0-d1/a0-a1
 * @ingroup bios_subcode
 *
 * @param[out] D0.l errqcodecrc / errpackcirc / scdflag / restrcnt
 * @param[out] D1.l erroverrun / errpacketbufful / errqcodefufful /
 * errpackfufful
 */
#define BIOS_SCDSTAT 0x0091

/**
 * @def BIOS_SCDREAD
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
#define BIOS_SCDREAD 0x0092

/**
 * @def BIOS_SCDPQ
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
#define BIOS_SCDPQ 0x0093

/**
 * @def BIOS_SCDPQL
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
#define BIOS_SCDPQL 0x0094

/**
 * @def BIOS_LEDSET
 * @brief Controls the status LEDs on the front of the CD unit
 * @clobber d0-d1/a0-a1
 * @ingroup bios_misc
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

#define BIOS_LED_READY	 0
#define BIOS_LED_DISCIN	 1
#define BIOS_LED_ACCESS	 2
#define BIOS_LED_STANDBY 3
#define BIOS_LED_ERROR	 4
#define BIOS_LED_MODE5	 5
#define BIOS_LED_MODE6	 6
#define BIOS_LED_MODE7	 7

/**
 * @def BIOS_CDCSETMODE
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
 *   These are the allowed bit fields
 *   Bit
 *    0     CD Mode 2
 *    1    bios_cdda mode
 *    2     Transfer error block with data
 *    3     Re-read last data
 */
#define BIOS_CDCSETMODE 0x0096

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
 * @def BIOS_WONDERREQ
 * @brief No documentation; research needed. Presumably exclusive to the
 *   WonderMega hardware
 * @ingroup bios_unknown
 */
#define BIOS_WONDERREQ 0x0097

/**
 * @def BIOS_WONDERCHK
 * @brief No documentation; research needed. Presumably exclusive to the
 *   WonderMega hardware
 * @ingroup bios_unknown
 */
#define BIOS_WONDERCHK 0x0098

#endif

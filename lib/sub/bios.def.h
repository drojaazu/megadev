/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file bios.def.h
 * @brief Function codes for the _CDBIOS vector
 */

#ifndef MEGADEV__SUB_BIOS_DEF_H
#define MEGADEV__SUB_BIOS_DEF_H

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
 * @def _BIOS_UNKNOWN00
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup bios_unknown
 */
#define _BIOS_UNKNOWN00 0x0000

/**
 * @def _BIOS_UNKNOWN01
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup bios_unknown
 */
#define _BIOS_UNKNOWN01 0x0001

/**
 * @def _BIOS_MSCSTOP
 * @sa bios_mscstop
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define _BIOS_MSCSTOP 0x0002

/**
 * @def _BIOS_MSCPAUSEON
 * @sa bios_mscpauseon
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define _BIOS_MSCPAUSEON 0x0003

/**
 * @def _BIOS_MSCPAUSEOFF
 * @sa bios_mscpauseoff
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define _BIOS_MSCPAUSEOFF 0x0004

/**
 * @def _BIOS_MSCSCANFF
 * @sa bios_mscscanff
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define _BIOS_MSCSCANFF 0x0005

/**
 * @def _BIOS_MSCSCANFR
 * @sa bios_mscscanfr
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define _BIOS_MSCSCANFR 0x0006

/**
 * @def _BIOS_MSCSCANOFF
 * @sa bios_mscscanoff
 * @ingroup bios_cdda
 * @clobber d0-d1/a0-a1
 */
#define _BIOS_MSCSCANOFF 0x0007

/**
 * @def _BIOS_ROMPAUSEON
 * @sa bios_rompauseon
 * @ingroup bios_cdrom
 * @clobber d0-d1/a0-a1
 */
#define _BIOS_ROMPAUSEON 0x0008

/**
 * @def _BIOS_ROMPAUSEOFF
 * @sa bios_rompauseoff
 * @ingroup bios_cdrom
 * @clobber d0-d1/a0-a1
 */
#define _BIOS_ROMPAUSEOFF 0x0009

/**
 * @def _BIOS_DRVOPEN
 * @sa bios_drvopen
 * @ingroup bios_misc
 * @clobber d0-d1/a0-a1
 */
#define _BIOS_DRVOPEN 0x000A

/**
 * @def _BIOS_DRVINIT
 * @sa bios_drvinit
 * @clobber d0-d1/a1
 *
 * @param[in] A0.l Pointer to initilization parameters
 *
 * @details Takes a pointer to two bytes (initialization params):
 *  byte 1 - track number from which to read TOC (normally 0x01); if bit 7 of
 *           this value is set, BIOS will start to play the first track
						 automatically
 *  byte 2 - last track to read (0xff will read all tracks)
 */
#define _BIOS_DRVINIT 0x0010

/**
 * @def _BIOS_UNKNOWN11
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup bios_unknown
 */
#define _BIOS_UNKNOWN11 0x0011

/**
 * @def _BIOS_UNKNOWN12
 * @brief Unknown; present in jump table but needs to be researched
 * @ingroup bios_unknown
 */
#define _BIOS_UNKNOWN12 0x0012

/**
 * @def _BIOS_MSCPLAY
 * @sa bios_mscplay
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define _BIOS_MSCPLAY 0x0011

/**
 * @def _BIOS_MSCPLAY1
 * @sa bios_mscplay1
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define _BIOS_MSCPLAY1 0x0012

/**
 * @def _BIOS_MSCPLAYR
 * @sa bios_mscplayr
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define _BIOS_MSCPLAYR 0x0013

/**
 * @def _BIOS_MSCPLAYT
 * @sa bios_mscplayt
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to BCD time code in the format mm:ss:ff:00
 *   (32 bit)
 */
#define _BIOS_MSCPLAYT 0x0014

/**
 * @def _BIOS_MSCSEEK
 * @sa bios_mscseek
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define _BIOS_MSCSEEK 0x0015

/**
 * @def _BIOS_MSCSEEKT
 * @sa bios_mscseekt
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to BCD time code in the format mm:ss:ff:00
 *   (32 bit)
 */
#define _BIOS_MSCSEEKT 0x0016

/**
 * @def _BIOS_ROMREAD
 * @sa bios_romread
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdrom
 *
 * @param[in] A0.l Pointer to the logical sector number (32 bit)
 */
#define _BIOS_ROMREAD 0x0017

/**
 * @def _BIOS_ROMSEEK
 * @sa bios_romseek
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdrom
 *
 * @param[in] A0.l Pointer to the logical sector number (32 bit)
 */
#define _BIOS_ROMSEEK 0x0018

/**
 * @def _BIOS_MSCSEEK1
 * @sa bios_mscseek1
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdda
 *
 * @param[in] A0.l Pointer to track number (16 bit)
 */
#define _BIOS_MSCSEEK1 0x0019

/**
 * @def _BIOS_ROMREADN
 * @sa bios_romreadn
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdrom
 *
 * @param[in] A0.l Pointer to a sector read info structure
 *
 * @details
 * The structure is made up of two 32 bit values:
 *
 *       dc.l 0x00000001   // First sector to read
 *       dc.l 0x00001234   // Number of sectors to read
 */
#define _BIOS_ROMREADN 0x0020

/**
 * @def _BIOS_ROMREADE
 * @sa bios_romreade
 * @clobber d0-d1/a0-a1
 * @ingroup bios_cdrom
 *
 * @param[in] A0.l Pointer to a sector read info structure
 *
 * @details The structure is made up of two 32 bit values:
 *       dc.l 0x00000001   // First sector to read
 *       dc.l 0x00001234   // Number of logical sectors to read
 * OUT:
 *  none
 */
#define _BIOS_ROMREADE 0x0021

/**
 * @def _BIOS_CDBCHK
 * @sa bios_cdbchk
 * @clobber d0
 * @ingroup bios_cdb
 *
 * @param[out] CC Command has been executed
 * @param[out] CS BIOS is busy
 *
 * @note CC indicates the command has been executed, NOT necessarily
 * that it has completed.
 */
#define _BIOS_CDBCHK 0x0080

/**
 * @def _BIOS_CDBSTAT
 * @sa bios_cdbstat
 * @clobber d0-d1/a0
 * @ingroup bios_cdb
 *
 * @param[out] A0.l Pointer to status info structure
 */
#define _BIOS_CDBSTAT 0x0081

/**
 * @def _BIOS_CDBTOCWRITE
 * @ingroup Misc
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
#define _BIOS_CDBTOCWRITE 0x0082

/**
 * @def _BIOS_CDBTOCREAD
 * @ingroup Misc
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
#define _BIOS_CDBTOCREAD 0x0083

/**
 * @def _BIOS_CDBPAUSE
 * @ingroup Misc
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
#define _BIOS_CDBPAUSE 0x0084

/**
 * @def _BIOS_FDRSET
 * @sa bios_fdrset
 * @clobber d0-d1/a0
 * @ingroup bios_fader
 *
 * @param[in] D1.w Volume
 */
#define _BIOS_FDRSET 0x0085

/**
 * @def _BIOS_FDRCHG
 * @sa bios_fdrchg
 * @clobber d0-d1/a0
 * @ingroup bios_fader
 *
 * @param[in] D1.l Volume & Ramp
 */
#define _BIOS_FDRCHG 0x0086

/**
 * @def _BIOS_CDCSTART
 * @brief Starts reading data from the current logical sector into the CDC
 * @clobber d0-d1/a0
 * @ingroup bios_cdc
 *
 * @note The BIOS pre-seeks by 2 to 4 sectors and data read actually begins
 * before the requested sector. It is up to the caller to identify the correct
 * starting sector (usually by checking the time codes in the headers as
 * they are read from the CDC buffer).
 */
#define _BIOS_CDCSTART 0x0087

/**
 * @def _BIOS_CDCSTARTP
 * @brief No official documentation on this call; needs to be researched
 * @ingroup bios_unknown
 */
#define _BIOS_CDCSTARTP 0x0088

/**
 * @def _BIOS_CDCSTOP
 * @brief Stop reading data into the CDC buffer
 * @clobber d0/a0
 * @ingroup bios_cdc
 *
 * @note If a sector is being read when CDCSTOP is called, it is discarded.
 */
#define _BIOS_CDCSTOP 0x0089

/**
 * @def _BIOS_CDCSTAT
 * @brief Query the status of the CDC buffer
 * @clobber d0-d1/a0
 * @ingroup bios_cdc
 *
 * @param[out] CC Sector available for read
 * @param[out] CS Sector not ready
 */
#define _BIOS_CDCSTAT 0x008A

/**
 * @def _BIOS_CDCREAD
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
#define _BIOS_CDCREAD 0x008B

/**
 * @def _BIOS_CDCTRN
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
#define _BIOS_CDCTRN 0x008C

/**
 * @def _BIOS_CDCACK
 * @brief Informs the CDC that the current sector has been read and that the
 * caller is ready for the next sector
 * @clobber d0
 * @ingroup bios_cdc
 */
#define _BIOS_CDCACK 0x008D

/**
 * @def _BIOS_SCDINIT
 * @brief Initializes the BIOS for subcode reads
 * @ingroup bios_subcode
 *
 * @param[in] A0.l Pointer to work buffer (at least 0x750 bytes)
 */
#define _BIOS_SCDINIT 0x008E

/**
 * @def _BIOS_SCDSTART
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
#define _BIOS_SCDSTART 0x008F

/**
 * @def _BIOS_SCDSTOP
 * @brief Disables reading subcode data by the CDC
 * @clobber d0-d1/a0-a1
 * @ingroup bios_subcode
 */
#define _BIOS_SCDSTOP 0x0090

/**
 * @def _BIOS_SCDSTAT
 * @brief Check subcode error status
 * @clobber d0-d1/a0-a1
 * @ingroup bios_subcode
 *
 * @param[out] D0.l errqcodecrc / errpackcirc / scdflag / restrcnt
 * @param[out] D1.l erroverrun / errpacketbufful / errqcodefufful /
 * errpackfufful
 */
#define _BIOS_SCDSTAT 0x0091

/**
 * @def _BIOS_SCDREAD
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
#define _BIOS_SCDREAD 0x0092

/**
 * @def _BIOS_SCDPQ
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
#define _BIOS_SCDPQ 0x0093

/**
 * @def _BIOS_SCDPQL
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
#define _BIOS_SCDPQL 0x0094

#define _LED_READY 0
#define _LED_DISCIN 1
#define _LED_ACCESS 2
#define _LED_STANDBY 3
#define _LED_ERROR 4
#define _LED_MODE5 5
#define _LED_MODE6 6
#define _LED_MODE7 7

/**
 * @def _BIOS_LEDSET
 * @brief Controls the status LEDs on the front of the CD unit
 * @clobber d0-d1/a0-a1
 * @ingroup bios_misc
 *
 * @param[in] D1.w Status code
 *
 * @details
 * |MODE          |Green|Red  |System Indication
 * |:------------:|:---:|:---:|:----------------:|
 * |LEDREADY (0)  |on   |blink|CD ready / no disk|
 * |LEDDISCIN (1) |on   |off  |CD ready / disk ok|
 * |LEDACCESS (2) |on   |on   |CD accessing      |
 * |LEDSTANDBY (3)|blink|off  |standby mode      |
 * |LED_ERROR (4) |blink|blink|reserved          |
 * |LEDMODE5 (5)  |blink|on   |reserved          |
 * |LEDMODE6 (6)  |off  |blink|reserved          |
 * |LEDMODE7 (7)  |off  |on   |reserved          |
 * |LEDSYSTEM (?) |     |     |return control to BIOS|
 *
 */
#define _BIOS_LEDSET 0x0095

#define _BIOS_CDC_MODE0 2 // bios_cdda
#define _BIOS_CDC_MODE1 0 // bios_cdrom with full error correction
#define _BIOS_CDC_MODE2 1 // bios_cdrom with CRC only

/**
 * @def _BIOS_CDCSETMODE
 * @brief Sets the mode in which the CD should be read
 * @clobber UNKNOWN (TODO)
 * @ingroup bios_cdc
 *
 * @param[in] D1.w Mode bit flags
 *
 * @note Use the predefined CDC_MODE0, CDC_MODE1, CDC_MODE2 values
 * to set the CDC mode
 *
 * @details
 *   These are the allowed bit fields
 *   Bit
 *    0     CD Mode 2
 *    1    bios_cdda mode
 *    2     Transfer error block with data
 *    3     Re-read last data
 */
#define _BIOS_CDCSETMODE 0x0096

/**
 * @def _BIOS_WONDERREQ
 * @brief No documentation; research needed. Presumably exclusive to the
 *   WonderMega hardware
 * @ingroup bios_unknown
 */
#define _BIOS_WONDERREQ 0x0097

/**
 * @def _BIOS_WONDERCHK
 * @ingroup Misc
 * @brief No documentation; research needed. Presumably exclusive to the
 *   WonderMega hardware
 * @ingroup bios_unknown
 */
#define _BIOS_WONDERCHK 0x0098

#endif

/**
 * \file sub/bios_def.h
 * \brief Function codes for the _CDBIOS vector
 */

#ifndef MEGADEV__CD_SUB_BIOS_DEF_H
#define MEGADEV__CD_SUB_BIOS_DEF_H

#include "sub/sub_def.h"

/**
 * \def MSCSTOP
 * \group CD-DA
 * \brief Stops playing CD audio if it is playing
 * \break d0-d1/a0-a1
 */
#define MSCSTOP 0x0002

/**
 * 
 * \def MSCPAUSEON
 * \group CD-DA
 * \brief Pauses the drive when a track is playing
 * \break d0-d1/a0-a1
 * 
 * \note If the drive is left paused it will stop after a programmable delay
 * (see \ref _CDBPAUSE)
 */
#define MSCPAUSEON 0x0003

/**
 * \def MSCPAUSEOFF
 * \group CD-DA
 * \brief Resumes playing a track after a pause
 * \break d0-d1/a0-a1
 *
 * \note If the drive has timed out and stopped, the BIOS will seek to the pause
 * time (with the attendant delay) and resume playing
 */
#define MSCPAUSEOFF 0x0004

/**
 * \def MSCSCANFF
 * \group CD-DA
 * \brief Starts playing from the current position in fast forward
 * \break d0-d1/a0-a1
 */
#define MSCSCANFF 0x0005

/**
 * \def MSCSCANFR
 * \group CD-DA
 * \brief Starts playing from the current position in fast reverse
 * \break d0-d1/a0-a1
 */
#define MSCSCANFR 0x0006

/**
 * \def MSCSCANOFF
 * \group CD-DA
 * \brief Returns to normal play mode
 * \break d0-d1/a0-a1
 * 
 * \note If the drive was paused before the scan was initiated, it will be
 * returned to pause.
 */
#define MSCSCANOFF 0x0007

/**
 * \def ROMPAUSEON
 * \group CD-ROM
 * \brief Stops reading data into the CDC and pauses
 * \break d0-d1/a0-a1
 */
#define ROMPAUSEON 0x0008

/**
 * \def ROMPAUSEOFF
 * \group CD-ROM
 * \brief Resumes reading data into the CDC from the current logical sector
 * \break d0-d1/a0-a1
 */
#define ROMPAUSEOFF 0x0009

/**
 * \def DRVOPEN
 * \group Drive
 * \brief Opens the CD drive door
 * \break d0-d1/a0-a1
 * 
 * \note This is only applicable to Model 1 hardware.
 */
#define DRVOPEN 0x000A

/**
 * \def DRVINIT
 * \group Drive
 * \brief Closes the disk tray and reads the TOC from the CD
 * \break d0-d1/a1
 * 
 * \param[in] A0.l Pointer to initilization parameters
 *
 * \details Takes a pointer to two bytes (initialization params):
 *  byte 1 - track number from which to read TOC (normally 0x01); if bit 7 of
 *           this value is set, BIOS will start to play the first track
             automatically
 *  byte 2 - last track to read (0xff will read all tracks)
 *
 * Pauses for 2 seconds after reading the TOC. Waits for a DRVOPEN request if
 * there is no disk in the drive.
 */
#define DRVINIT 0x0010

/**
 * \def MSCPLAY
 * \group CD-DA
 * \brief Starts CD audio playback at the specified track & continues playing
 * through subsequent tracks
 * \break d0-d1/a0-a1
 *
 * \param[in] A0.l Pointer to track number (16 bit)
 */
#define MSCPLAY 0x0011

/**
 * \def MSCPLAY1
 * \group CD-DA
 * \brief Plays the specified track once then pause
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to track number (16 bit)
 */
#define MSCPLAY1 0x0012

/**
 * \def MSCPLAYR
 * \group CD-DA
 * \brief Plays the specified track on repeat
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to track number (16 bit)
 */
#define MSCPLAYR 0x0013

/**
 * \def MSCPLAYT
 * \group CD-DA
 * \brief Starts playing from the specified time
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to BCD time code in the format mm:ss:ff:00
 *   (32 bit)
 */
#define MSCPLAYT 0x0014

/**
 * \def MSCSEEK
 * \group CD-DA
 * \brief Seeks to the beginning of the specified track and pauses
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to track number (16 bit)
 */
#define MSCSEEK 0x0015

/**
 * \def MSCSEEKT
 * \group CD-DA
 * \brief Seeks to a specified time
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to BCD time code in the format mm:ss:ff:00
 *   (32 bit)
 */
#define MSCSEEKT 0x0016

/**
 * \def ROMREAD
 * \group CD-ROM
 * \brief Begins reading data from the CDROM at the designated logical sector
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to the logical sector number (32 bit)
 * 
 * \details Executes a CDCSTART to begin the read, but does not stop
 * automatically.
 * 
 * \note ROMREAD actually pre-seeks by 2 sectors, but doesn't start passing data
 * to the CDC until the desired sector is reached.
 */
#define ROMREAD 0x0017

/**
 * \def ROMSEEK
 * \group CD-ROM
 * \brief Seeks to the designated logical sector and pauses
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to the logical sector number (32 bit)
 */
#define ROMSEEK 0x0018

/**
 * \def MSCSEEK1
 * \group CD-DA
 * \brief Seek to the beginning of the selected track and pause; when BIOS
 * detects a pause state, the track is played once
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to track number (16 bit)
 */
#define MSCSEEK1 0x0019

/**
 * \def TESTENTRY
 * \group Misc
 * \brief No documentation; needs to be researched
 */
#define TESTENTRY 0x001E

/**
 * \def TESTENTRYLOOP
 * \group Misc
 * \brief No documentation; needs to be researched
 */
#define TESTENTRYLOOP 0x001F

/**
 * \def ROMREADN
 * \group CD-ROM
 * \brief Same function as ROMREAD, but stops after reading the requested
 * number of sectors
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to a sector read info structure
 * 
 * \details The structure is made up of two 32 bit values:
 *       dc.l 0x00000001   // First sector to read
 *       dc.l 0x00001234   // Number of sectors to read
 */
#define ROMREADN 0x0020

/**
 * \def ROMREADE
 * \group CD-ROM
 * \brief Same as ROMREAD, but reads between two logical sectors
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to a sector read info structure
 * 
 * \details The structure is made up of two 32 bit values:
 *       dc.l 0x00000001   // First sector to read
 *       dc.l 0x00001234   // Number of logical sectors to read
 * OUT:
 *  none
 */
#define ROMREADE 0x0021

/**
 * \def CDBCHK
 * \group Misc
 * \brief Query the BIOS on the status of the last command
 * \break d0-d1/a0-a1
 * 
 * \param[out] CC Command has been executed
 * \param[out] CS BIOS is busy
 * 
 * \note Success indicates the command has been executed, NOT necessarily
 * that it has completed.
 */
#define CDBCHK 0x0080

/**
 * \def CDBSTAT
 * \group Misc
 * \brief Retrieve CD BIOS status
 * \break d0-d1/a0-a1
 * 
 * \param[out] A0.l Pointer to status info structure
 * 
 * Please refer to the BIOS manual for details about the returned data
 */
#define CDBSTAT 0x0081

/**
 * \def CDBTOCWRITE
 * \group Misc
 * \brief Writes data to disc TOC stored in memory
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to TOC data
 * 
 * \details A TOC entry is four bytes long, where the first three bytes are
 * the BCD timecode of the start of the track and the lowest byte is the track
 * number. The most significant bit of the frame byte of the timecode indicates
 * whether the track is CD-ROM or CD-DA. Please refer to the BIOS manual for
 * more information.
 */
#define CDBTOCWRITE 0x0082

/**
 * \def CDBTOCREAD
 * \group Misc
 * \brief Reads the TOC entry for a given track
 * \break d0-d1/a0-a1
 * 
 * \param[in] D1.w Track number
 * \param[out] D0.l Track start timecode (BCD, low byte is track number)
 * \param[out] D1.b Track type (00 - CD-DA, 0xFF - CD-ROM)
 * 
 * \note The low byte of D0 will be 0 on error.
*/
#define CDBTOCREAD 0x0083

/**
 * \def CDBPAUSE
 * \group Misc
 * \brief Sets the time that the drive spins down from pause to standby
 * \break d0-d1/a0-a1
 * 
 * \param[in] D1.w Pause time (in ticks of 1/75 second)
 * 
 * \details Normal ranges for this delay time are 0x1194 - 0xFFFE. A delay of
 * 0xFFFF prevents the drive from stopping, but can damage the drive if used
 * improperly.
 */
#define CDBPAUSE 0x0084

/**
 * \def FDRSET
 * \group Fader
 * \brief Sets the audio volume
 * \break d0-d1/a0-a1
 * 
 * \param[in] D1.w Volume
 * 
 * \details Setting the upper bit of the value changes the master volume
 *   16 bit volume         (0x0000 to 0x0400)
 *   16 bit master volume  (0x8000 to 0x8400)
 * 
 * \note The master volume sets a maximum level which the volume level
 * canot exceed.
 * 
 * \note There is a delay of up to 13ms before the volume begins to change
 * and another 23ms for the new volume level to take effect. 
 */
#define FDRSET 0x0085

/**
 * \def FDRCHG
 * \group Fader
 * \brief  Ramps the audio volume from its current level to a new level at
 * the requested rate
 * \break d0-d1/a0-a1
 * 
 * \param[in] D1.l Volume & Ramp
 * \details The input is two 16 bit values
 *   High word: new volume (min 0x0000, max 0x0400)
 *    Low word: change rate in steps per vblank
 *        0x0001 - slow
 *        0x0200 - fast
 *        0x0400 - immediate
 * 
 * \note There is a delay of up to 13ms before the volume begins to change.
 */
#define FDRCHG 0x0086

/**
 * \def CDCSTART
 * \group CDC
 * \brief Starts reading data from the current logical sector into the CDC
 * \break d0-d1/a0-a1
 * 
 * \note The BIOS pre-seeks by 2 to 4 sectors and data read actually begins
 * before the requested sector. It is up to the caller to identify the correct
 * starting sector (usually by checking the time codes in the headers as
 * they are read from the CDC buffer).
 */
#define CDCSTART 0x0087

/**
 * \def CDCSTARTP
 * \group CDC
 * \brief No official documentation on this call; needs to be researched
 */
#define CDCSTARTP 0x0088

/**
 * \def CDCSTOP
 * \group CDC
 * \brief Stop reading data into the CDC buffer
 * \break d0-d1/a0-a1
 * 
 * \note If a sector is being read when CDCSTOP is called, it is discarded.
 */
#define CDCSTOP 0x0089

/**
 * \def CDCSTAT
 * \group CDC
 * \brief Query the status of the CDC buffer
 * \break d0-d1/a0-a1
 * 
 * \param[out] CC Sector available for read
 * \param[out] CS No sectors available yet
 */
#define CDCSTAT 0x008A

/**
 * \def CDCREAD
 * \group CDC
 * \brief Reads sector of data in preparation for transfer
 * \break d0-d1/a0-a1
 * 
 * \param[out] CS Sector not ready
 * \param[out] CC Sector ready for transfer
 * \param[out] D0.l Sector header timecode (BCD)
 * \details The low byte of D0 is the sector mode:
 *   00 - CD-DA
 *   01 - CD-ROM mode 1
 *   02 - CD-ROM mode 2
 * 
 * \note Be sure to set the device destination register BEFORE calling CDCREAD!
 */
#define CDCREAD 0x008B

/**
 * \def CDCTRN
 * \group CDC
 * \brief Transfer one sector of data from the CDC to Sub CPU RAM
 * \break d0-d1/a0-a1
 * 
 * \param[in] A0.l Pointer to sector destination buffer (0x920 bytes)
 * \param[in] A1.l Pointer to header destination buffer (4 bytes)
 * \param[out] CC Sector transfer completed successfully
 * \param[out] CS Sector transfer failed
 * \param[out] A0.l Pointer to next sector destination address (A0 + 0x920)
 * \param[out] A1.l Pointer to next header destination address (A1 + 4)
 * 
 * \note The device destination must be set to Sub CPU read beforehand!
 */
#define CDCTRN 0x008C

/**
 * \def CDCACK
 * \group CDC
 * \brief Informs the CDC that the current sector has been read and that the
 * caller is ready for the next sector
 * \break d0-d1/a0-a1
 */
#define CDCACK 0x008D

/**
 * \def SCDINIT
 * \group Subcodes
 * \brief Initializes the BIOS for subcode reads
 * 
 * \param[in] A0.l Pointer to work buffer (at least 0x750 bytes)
 */
#define SCDINIT 0x008E

/**
 * SCDSTART
 * \group Subcodes
 * \brief Enables reading subcode data by the CDC
 * \break d0-d1/a0-a1
 * 
 * \param[in] D1.w Subcode read flags
 * 
 * \details Flags
 *    0: --------
 *    1: --RSTUVW
 *    2: PQ------
 *    3: PQRSTUVW
 */
#define SCDSTART 0x008F

/**
 * \def SCDSTOP
 * \group Subcodes
 * \brief Disables reading subcode data by the CDC
 * \break d0-d1/a0-a1
 */
#define SCDSTOP 0x0090

/**
 * \def SCDSTAT
 * \group Subcodes
 * \brief Check subcode error status
 * \break d0-d1/a0-a1
 * 
 * \param[out] D0.l errqcodecrc / errpackcirc / scdflag / restrcnt
 * \param[out] D1.l erroverrun / errpacketbufful / errqcodefufful / errpackfufful
 */
#define SCDSTAT 0x0091

/**
 * \def SCDREAD
 * \group Subcodes
 * \brief Reads R through W subcode channels
 * \break d0-d1/a1
 * 
 * \param[in] A0.l Address of the Q code buffer (minimum 24 bytes)
 * \param[out] CC Read successful
 * \param[out] CS Read failed
 * \param[out] A0.l Address of next Q code buffer (A0.l + 24)
 * 
 */
#define SCDREAD 0x0092

/**
 * \def SCDPQ
 * \group Subcodes
 * \brief Gets P & Q codes from subcode
 * \break d0-d1/a1
 * 
 * \param[in] A0.l Address of the Q code buffer (minimum 12 bytes)
 * \param[out] CC Read successful
 * \param[out] CS Read failed
 * \param[out] A0.l Address of next Q code buffer (A0.l + 12)
 * 
 */
#define SCDPQ 0x0093

/**
 * \def SCDPQL
 * \group Subcodes
 * \brief Gets the last P & Q codes
 * \break d0-d1/a1
 * 
 * \param[in] A0.l Address of the Q code buffer (minimum 12 bytes)
 * \param[out] CC Read successful
 * \param[out] CS Read failed
 * \param[out] A0.l Address of next Q code buffer (A0.l + 12)
 * 
 */
#define SCDPQL 0x0094

#define LEDREADY 0
#define LEDDISCIN 1
#define LEDACCESS 2
#define LEDSTANDBY 3
#define LEDERROR 4
#define LEDMODE5 5
#define LEDMODE6 6
#define LEDMODE7 7

/**
 * \def LEDSET
 * \group Misc
 * \brief Controls the status LEDs on the front of the CD unit
 * \break d0-d1/a0-a1
 * 
 * \param[in] D1.w Status code
 * 
 * \details 
 * MODE          Ready (green)   Access (red)    System Indication
 * ---------------------------------------------------------------
 * LEDREADY (0)      on              blink       CD ready / no disk
 * LEDDISCIN (1)     on              off         CD ready / disk ok
 * LEDACCESS (2)     on              on          CD accessing
 * LEDSTANDBY (3)    blink           off         standby mode
 * LEDERROR (4)      blink           blink       reserved
 * LEDMODE5 (5)      blink           on          reserved
 * LEDMODE6 (6)      off             blink       reserved
 * LEDMODE7 (7)      off             on          reserved
 * LEDSYSTEM (?)                                 return control to BIOS
 */
#define LEDSET 0x0095


#define CDC_MODE0 2  // CD-DA
#define CDC_MODE1 0  // CD-ROM with full error correction
#define CDC_MODE2 1  // CD-ROM with CRC only

/**
 * \def CDCSETMODE
 * \group CDC
 * \brief Sets the mode in which the CD should be read
 * \break UNKNOWN (TODO)
 * 
 * \param[in] D1.w Mode bit flags
 * 
 * \note Use the predefined CDC_MODE0, CDC_MODE1, CDC_MODE2 values
 * to set the CDC mode 
 * 
 * \details
 *   These are the allowed bit fields
 *   Bit
 *    0     CD Mode 2
 *    1     CD-DA mode
 *    2     Transfer error block with data
 *    3     Re-read last data
 */
#define CDCSETMODE 0x0096

/**
 * \def WONDERREQ
 * \group Misc
 * \brief No documentation; research needed. Presumably exclusive to the 
 *   WonderMega hardware
 */
#define WONDERREQ 0x0097

/**
 * \def WONDERCHK
 * \group Misc
 * \brief No documentation; research needed. Presumably exclusive to the 
 *   WonderMega hardware
 */
#define WONDERCHK 0x0098

#endif

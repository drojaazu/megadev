/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file cd_boot.s
 * @brief Mega CD boot sector (header and IP/SP)
 * 
 * @warning
 * Do not change the order of the defines in this file!
 */

#define STR(x) XSTR(x)
#define XSTR(x) #x

// Stops the build, naming the define, when a header string is longer than its
// field. Without it the overrun shows only as "attempt to move .org backwards".
#define FIELD_FITS(start, size, message) \
	.if (. - start) > size; .error message; .endif

/** System Header **/

/*
https://forums.sonicretro.org/threads/sega-cd.41344/
https://www.retrodev.com/segacd.html
"The BIOS expects to find one of the following 16byte identifiers at the very beginning of the CD:
  'SEGADISC        '
  'SEGABOOTDISC    '
  'SEGADATADISC    '
  'SEGADISCSYSTEM  '
 The boot process is effected by which one is used, but I'm not exactly sure what the difference is at the moment.
 I do know that it only performs the security check if SEGABOOTDISC or SEGADISCSYSTEM are used, and it would appear
 that SEGADISC and SEGADATADISC are not bootable identifiers."
*/

.org 0
9: .ascii STR(HEADER_DISC_ID)   // Disc Type (must be one of the allowed values!)
FIELD_FITS(9b, 16, "HEADER_DISC_ID is longer than 16 characters")
.org 0x10, 0x20
9: .ascii STR(HEADER_VOL_ID)    // Volume Name
FIELD_FITS(9b, 11, "HEADER_VOL_ID is longer than 11 characters (default: PROJECT_ID in upper case)")
.org 0x1B, 0x20
.byte 0                      // ends the name, as on retail discs
.word 0x0100                 // Volume Version (BCD)
.word 0x0001                 // Type ()
.org 0x20
9: .ascii STR(HEADER_SYS_ID)    // System Name
FIELD_FITS(9b, 11, "HEADER_SYS_ID is longer than 11 characters (default: PROJECT_ID in upper case)")
.org 0x2B, 0x20
.byte 0                      // ends the name, as on retail discs
.word 0x0100                 // System Version (BCD)
.word 0                      // Always zero

/*
 * "The original BIOS seems to have been hard wired for an IP that stayed within
 * sector 0, so you have to kind of lie to it when IP gets bigger than that.
 * According to SOJ, the value in the IP Start field should point to the beginning
 * of the first byte in IP after the first $600 bytes. This would mean that you'd
 * always use $800 (since anything smaller would fit in sector 0 and you wouldn't
 * have to mess with any of this in the first place). It also implies that IP can't
 * extend beyond the end of sector 1, but I haven't confirmed that yet."
*/

.org 0x30
IP_OFFSET:   .long 0x800
IP_SIZE:     .long 0x800
IP_ENTRY:    .long 0
IP_WORK_RAM:  .long 0
SP_OFFSET:   .long 0x1000
SP_SIZE:     .long _sp_end-_sp_begin
SP_ENTRY:    .long 0
SP_WORK_RAM:  .long 0
.rept 11
.ascii       "                "
.endr

/** Disc Header **/

.org 0x100
9: .ascii STR(HEADER_HARDWARE_ID)  // Hardware Type (must be one of the allowed values!)
FIELD_FITS(9b, 16, "HEADER_HARDWARE_ID is longer than 16 characters")
.org 0x110, 0x20
9: .ascii STR(HEADER_COPYRIGHT)
FIELD_FITS(9b, 16, "HEADER_COPYRIGHT is longer than 16 characters")
.org 0x120, 0x20
9: .ascii STR(PROJECT_NAME_DOMESTIC)
FIELD_FITS(9b, 48, "PROJECT_NAME_DOMESTIC is longer than 48 characters (default: PROJECT_NAME)")
.org 0x150, 0x20
9: .ascii STR(PROJECT_NAME)
FIELD_FITS(9b, 48, "PROJECT_NAME is longer than 48 characters (default: PROJECT_ID)")
.org 0x180, 0x20
9: .ascii STR(HEADER_SOFTWARE_ID)
FIELD_FITS(9b, 16, "HEADER_SOFTWARE_ID is longer than 16 characters")
.org 0x190, 0x20
.ascii "J               "
.ascii "                "
.ascii "                "
.ascii "                "
.ascii "                "
.ascii "                "
.org 0x1F0, 0x20
9: .ascii STR(HEADER_REGION)
FIELD_FITS(9b, 16, "HEADER_REGION is longer than 16 characters")

// if all the above text is correct, we should be at 0x200 anyway
.org 0x200, 0x20

_ip_begin:
	.incbin "ip.bin"
_ip_end:

/*
	With the changes to the US/EU security code, the SP should now start
	at the second sector (0x1000)
*/
.org	0x1000
_sp_begin:
	.incbin "sp.bin"
_sp_end:

// fill out the rest of the boot sector
.align	0x8000

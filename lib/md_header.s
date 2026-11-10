/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file md_header.s
 * @brief Mega Drive software header
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

.org 0x100
9: .ascii STR(HEADER_HARDWARE_ID)
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
FIELD_FITS(9b, 14, "HEADER_SOFTWARE_ID is longer than 14 characters")
.org 0x18E, 0x20
// checksum, filled in after linking by tools/romfix.sh
.word  0
.org 0x190
#if TARGET == MEGACD_MODE1
// Device support: joypad and Mega CD. The C tells an emulator with a Boot ROM
// available to attach the Mega CD to this cartridge.
.ascii "JC"
#else
.ascii "J"
#endif
.org 0x1A0, 0x20
// ROM start
.long 0
// ROM end, filled in after linking by tools/romfix.sh
.long 0
// RAM start and end
.long 0x00FF0000
.long 0x00FFFFFF
// Extra memory (0x1B0) and modem (0x1BC): none
.ascii "            "
.ascii "            "
// Memo
.org 0x1F0, 0x20
9: .ascii STR(HEADER_REGION)
FIELD_FITS(9b, 16, "HEADER_REGION is longer than 16 characters")
.org 0x200, 0x20

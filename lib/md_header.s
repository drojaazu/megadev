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

.org 0x100
.ascii STR(HEADER_HARDWARE_ID)
.org 0x110, 0x20
.ascii STR(HEADER_COPYRIGHT)
.org 0x120, 0x20
.ascii STR(PROJECT_NAME_DOMESTIC)
.org 0x150, 0x20
.ascii STR(PROJECT_NAME)
.org 0x180, 0x20
.ascii STR(HEADER_SOFTWARE_ID)
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
.ascii STR(HEADER_REGION)
.org 0x200, 0x20

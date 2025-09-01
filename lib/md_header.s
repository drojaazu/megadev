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
HardwareType:  .ascii STR(HEADER_HARDWARE_ID)
.org 0x110, 0x20
Copyright:     .ascii STR(HEADER_COPYRIGHT)
.org 0x120, 0x20
TitleDomestic: .ascii STR(PROJECT_NAME_DOMESTIC)
.org 0x150, 0x20
TitleIntl:     .ascii STR(PROJECT_NAME)
.org 0x180, 0x20
SoftwareID:    .ascii STR(HEADER_SOFTWARE_ID)
.org 0x18E, 0x20
Checksum:      .word  0
.org 0x190
Devices:       .ascii "J"
.org 0x1A0, 0x20
RomRangeStart: .long 0x0
RomRangeEnd:   .long 0x3FFFFF
ExtraMemory:   .ascii "            "
//                     ############
Modem:         .ascii "            "
//                     ############
.fill 40
Region:       .ascii STR(HEADER_REGION)
.fill 13

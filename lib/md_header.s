/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file md_header.s
 * @brief Mega Drive software header
 * 
 * @warning
 * Do not change the order of the defines in this file!
 */

#ifdef SYSTEM_TYPE
HardwareType: .ascii HEADER_HARDWARE
#elif REGION == US
HardwareType: .ascii "SEGA GENESIS    "
//                    ################
#else
HardwareType: .ascii "SEGA MEGA DRIVE "
//                    ################
#endif

Copyright:     .ascii HEADER_COPYRIGHT
TitleDomestic: .ascii HEADER_NAME_JP
TitleIntl:     .ascii HEADER_NAME_INTL
SoftwareID:    .ascii HEADER_SOFT_ID
Checksum:      .word  0
Devices:       .ascii "J               "
//                     ################
RomRangeStart: .long 0x0
RomRangeEnd:   .long 0x3FFFFF
ExtraMemory:   .ascii "            "
//                     ############
Modem:         .ascii "            "
//                     ############
.fill 40
Region:       .ascii HEADER_REGION
.fill 13

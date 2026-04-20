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
# TODO calculate checksum
.word  0
.org 0x190
.ascii "J"
.org 0x1A0, 0x20
.long 0x0
# TODO fill out proper rom start/rom end
.long 0x3FFFFF
# TODO add variables for Extra Memory and Modem
.ascii "            "
//      ############
.ascii "            "
//      ############
.fill 40
.ascii STR(HEADER_REGION)
.fill 13

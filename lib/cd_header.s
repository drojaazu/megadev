/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file cd_header.s
 * @brief Mega CD software header
 * 
 * @warning
 * Do not change the order of the defines in this file!
 */

HardwareType:  .ascii HEADER_HARDWARE
Copyright:     .ascii HEADER_COPYRIGHT
TitleDomestic: .ascii HEADER_NAME_JP
TitleIntl:     .ascii HEADER_NAME_INTL
SoftwareID:    .ascii HEADER_SOFT_ID
IO:            .ascii "J               "
               .ascii "                "
               .ascii "                "
               .ascii "                "
               .ascii "                "
               .ascii "                "
Region:        .ascii HEADER_REGION
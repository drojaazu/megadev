/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file boot.s
 * @brief Mega CD boot sector (header and IP/SP)
 * 
 * @warning
 * Do not change the order of the defines in this file!
 */

#include "project.h"

DiscHeader:
DiscType: .ascii "SEGADISCSYSTEM  "		/*Disc Type (must be one of the allowed values)*/

/**
 * Volume ID, 11 bytes + 0 terminator
 */
VolumeName: .asciz HEADER_VOL_ID
VolumeSystem:	.word 0x100, 0x1				/*System ID, Type*/
SystemName:	.asciz "SEGASYSTEM "			/*System Name*/
SystemVersion:	.word 0,0							/*System Version, Type*/

// The US/EU security bins are much larger
// so we need to correct for this (as outlined in the Mega CD
// technial bulletins)
//IP_Addr:	.long _ip_begin-DiscHeader		/*IP Start Address*/
//IP_Size:	.long _ip_end-_ip_begin					/*IP End Address*/

/*
 * @note
 * > "The original BIOS seems to have been hard wired for an IP that stayed within
 * > sector 0, so you have to kind of lie to it when IP gets bigger than that.
 * > According to SOJ, the value in the IP Start field should point to the beginning
 * > of the first byte in IP after the first $600 bytes. This would mean that you'd
 * > always use $800 (since anything smaller would fit in sector 0 and you wouldn't
 * > have to mess with any of this in the first place). It also implies that IP can't
 * > extend beyond the end of sector 1, but I haven't confirmed that yet."
*/

IP_OFFSET:  .long 0x800
IP_SIZE:    .long 0x800
IP_ENTRY:   .long 0
IP_WORKRAM: .long 0
SP_OFFSET:  .long 0x1000
SP_SIZE:    .long _sp_end-_sp_begin
SP_ENTRY:   .long 0
SP_WORKRAM: .long 0
.ascii	"                "
.ascii	"                "
.ascii	"                "
.ascii	"                "
.ascii	"                "
.ascii	"                "
.ascii	"                "
.ascii	"                "
.ascii	"                "
.ascii	"                "
.ascii	"                "
		
# =======================================================================================
#  Game Header
# =======================================================================================	
HardwareType: .ascii HEADER_HARDWARE
Copyright:    .ascii HEADER_COPYRIGHT
TitleDomestic:   .ascii HEADER_NAME_JP
TitleIntl: .ascii HEADER_NAME_INTL
DiscID:       .ascii HEADER_SOFT_ID
IO:
  .ascii "J               "
  .ascii	"                "
  .ascii	"                "
  .ascii	"                "
  .ascii	"                "
  .ascii	"                "
Region:       .ascii HEADER_REGION

// if all the above text is correct, we should be at 0x200 anyway
.org 0x200

// the missing quote mark in the two .incbins below is *intentional*
// (it's because defines are not substituted in strings)
// DISC_PATH is set in makefile.global
_ip_begin:
	.incbin DISC_PATH.ip"
_ip_end:

/*
	With the changes to the US/EU security code, the SP should now start
	at the second sector (0x1000)
*/
.org	0x1000
_sp_begin:
	.incbin DISC_PATH.sp"
_sp_end:

// fill out the rest of the boot sector
.align	0x8000

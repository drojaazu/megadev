/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file boot.s
 * @brief Mega CD boot sector (header and IP/SP)
 * 
 * @note
 * > "The original BIOS seems to have been hard wired for an IP that stayed within
 * > sector 0, so you have to kind of lie to it when IP gets bigger than that.
 * > According to SOJ, the value in the IP Start field should point to the beginning
 * > of the first byte in IP after the first $600 bytes. This would mean that you'd
 * > always use $800 (since anything smaller would fit in sector 0 and you wouldn't
 * > have to mess with any of this in the first place). It also implies that IP can't
 * > extend beyond the end of sector 1, but I haven't confirmed that yet."
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

IP_Addr:	.long 0x800		              /*IP Start Address*/
IP_Size:	.long 0x800					        /*IP End Address*/

IP_Entry:	.long 0
IP_WorkRAM:	.long 0
SP_Addr:	.long 0x1000		/*SP location on disc*/
SP_Size:	.long _sp_end-_sp_begin					/*SP End Address */
SP_Entry:	.long 0
SP_WorkRAM:	.long 0
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
HardwareType:	.ascii "SEGA MEGA DRIVE "
Copyright:	  .ascii "(C)     2022.NOV"
NativeName:	  .ascii HEADER_NAME_JP
OverseasName:	.ascii HEADER_NAME_INTL
DiscID:		.ascii HEADER_SOFT_ID
IO:		.ascii "J               "
	.ascii	"                "
	.ascii	"                "
	.ascii	"                "
	.ascii	"                "
	.ascii	"                "
Region:		.ascii HEADER_REGION

// if all the above text is correct, we should be at 0x200 anyway
.org 0x200

/*
	We need to compile ip and sp seperately since they need their own
	memory map (ip runs from Work RAM, sp from PRG RAM)
*/
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

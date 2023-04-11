/**
 * @file vdp.macro.s
 * @brief Mega Drive Video Display Processor (VDP)
 */

#ifndef MEGADEV__MAIN_VDP_MACROS_S
#define MEGADEV__MAIN_VDP_MACROS_S

#define VDPPTR(addr) ((((addr) & 0x3FFF) << 16) + (((addr) & 0xC000) >> 14))

/**
 * @brief Convert 16 bit VRAM address to vdpptr format
 */
.macro TO_VDPPTR dreg=d0
	andi.l #0xffff, \dreg
	lsl.l #2, \dreg
	lsr.w #2, \dreg
	swap \dreg
.endm

#endif


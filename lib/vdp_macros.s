/**
 * \file
 * \brief Mega Drive Video Display Processor (VDP)
 */

#ifndef MEGADEV__VDP_MACROS_S
#define MEGADEV__VDP_MACROS_S

/**
 * \brief Convert 16 bit VRAM address to vdpptr format
 */
.macro TO_VDPPTR dreg=d0
  andi.l #0xffff, \dreg
  lsl.l #2, \dreg
  lsr.w #2, \dreg
  swap \dreg
.endm

#endif


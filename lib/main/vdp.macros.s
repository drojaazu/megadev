/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file vdp.macros.s
 * @brief VDP utility macros
 */

#ifndef MEGADEV__MAIN_VDP_MACROS_S
#define MEGADEV__MAIN_VDP_MACROS_S

/**
 * @brief Converts a 16 bit VRAM address into VDP format at runtime
 */
.macro TO_VDPPTR dreg=d0
  andi.l   #0xFFFF, \dreg
  lsl.l    #2, \dreg
  lsr.w    #2, \dreg
  swap     \dreg
.endm

/**
 * @brief Converts a VDP format address to a 16 bit VRAM address at runtime
 */
.macro VDPPTR_TO dreg=d0
  andi.l  #0x3FFF000C, \dreg
  ror.w    #2, \dreg
  lsr.l    #8, \dreg
  lsr.l    #6, \dreg
  ror.w    #2, \dreg
.endm

#endif

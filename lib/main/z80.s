/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file z80.s
 * @brief Z80 CPU utilities
 */

#ifndef MEGADEV__MAIN_Z80_S
#define MEGADEV__MAIN_Z80_S

#include "macros.s"
#include "z80.def.h"

.section .text

/**
 * @sa init_z80
 * @param[in] A0.l Pointer to Z80 program to load
 * @param[in] D7.l Length of program data (in bytes)
 * @clobber a0-a1, d7
 * @ingroup z80
 * @warning Interrupts should be disabled before calling.
 */
SUB init_z80
  move.w  #Z80_STOP_RESET, (Z80_BUSREQ)
  move.w  #Z80_STOP_RESET, (Z80_RESET)
0:btst    #Z80_STATUS, (Z80_BUSREQ)
  bne.s	  0b
  lea     (_Z80_RAM), a1
1:move.b  (a0)+,(a1)+
  dbf	    d7, 1b
  move.w  #0, (Z80_RESET)
  move.w  #0, (Z80_BUSREQ)
  move.w  #Z80_STOP_RESET, (Z80_RESET)
  rts

#endif
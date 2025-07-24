/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file init.macros.s
 * @brief Module initialization
 * @details Performs three main tasks:
 *          - Disables interrupts
 *          - Clears .bss section RAM
 *          - Copies .data section into RAM
 */

#ifndef INIT_MACROS_S
#define INIT_MACROS_S

.macro BASIC_INIT
  moveq    #0, d0
  move.l   #_BSS_LENGTH, d7
  lea      _BSS_ORIGIN, a0
  bra      1f
0:move.l   d0, (a0)+
1:dbf      d7, 0b
  // copy .data section to RAM
  move.l   #_RAM_DATA_LENGTH, d7
  lea      _ROM_DATA_ORIGIN, a0
  lea      _RAM_DATA_ORIGIN, a1
  bra      1f
0:mov.l    (a0)+, (a1)+
1:dbf      d7, 0b
.endm


#endif

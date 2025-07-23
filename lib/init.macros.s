/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file init.macros.s
 * @brief Initialization macros
 * @details Performs three main tasks:
 *          - Disables interrupts
 *          - Clears .bss section RAM
 *          - Copies .data section into RAM
 */

 
#ifndef INIT_MACROS_S
#define INIT_MACROS_S

.macro INIT_RAM
  lea      register_data, a5
  movem.l  (a5), d0-d2/a0-a2
  // clear RAM
1:move.l   d0, (a0)+
  dbf      d1, 1b
  // copy .data section to RAM
0:mov.l    (a1)+, (a2)+
  dbf      d2, 0b
.endm

register_data:
.long      0
.long      _BSS_LENGTH - 1
.long      _RAM_DATA_LENGTH - 1
.long      _BSS_ORIGIN
.long      _ROM_DATA_ORIGIN
.long      _RAM_DATA_ORIGIN

#endif

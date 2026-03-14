/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file mmd.macros.s
 */

#ifndef MEGADEV__MAIN_INIT_MMD_S
#define MEGADEV__MAIN_INIT_MMD_S

#include "macros.s"
#include <main/memmap.def.h>
#include <main/gate_arr.def.h>
#include <main/gate_arr.macros.s>

/**
 * @fn INIT_MMD
 * @brief Initialize MMD formatted module
 * @note This expects a module to be already loaded at the start of Word RAM
 * @return a0 pointer to module entry point
 * @clobber d0, a0-a2
 */
.macro INIT_MMD
  WAIT_2M
  lea      WORD_RAM, a0	//get MMD entry point
  move.l   2(a0), d0	//get MMD data destination
  beq      1f             //if no destination, skip the copy
  movea.l  d0, a2         //put destination in a2
  lea      0x100(a0), a1  //start of MMD Data section in a1
  move.w   6(a0), d0  //size of MMD Data in d7
0:move.l   (a1)+, (a2)+   //copy MMD Data to destination
  dbf      d0, 0b
1:move.l   12(a0), d0  //set HINT vector if provided
  beq      2f
  move.l   d0, EXVEC_LEVEL4
2:move.l   16(a0), d0
  beq      3f
  move.l   d0, EXVEC_LEVEL6
3:btst     #6, WORD_RAM    // if bit 6 is set, return 2m to sub 
  beq      4f
  GRANT_2M
4:movea.l  8(a0), a0
.endm

#endif

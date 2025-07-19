/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file io.s
 * @brief I/O utilities
 */

#ifndef MEGADEV__MAIN_IO_S
#define MEGADEV__MAIN_IO_S

#include "main/io.def.h"

/**
 * @macro INIT_JOYPADS
 * @brief Sets up Player 1 and 2 terminals for standard reads
 * @desc Sets TH pin to write mode for Control port for each normally used terminal. Note that this does not
 *       do any work on the third terminal on old MD models, normally used for serial comm.
 * @clobber a0
 */
.macro INIT_JOYPADS
  /* set TH pin to enable write */
  move.b  #CTRL_PC6, (_IO_CTRL1)
  move.b  #CTRL_PC6, (_IO_CTRL2)
.endm

/**
 * @macro READ_INPUT_JOYPAD
 * @brief Reads controller input from a 3 button joypad
 * @in A0 the Data Port for the desired controller terminal
 * @out D0 byte with the state for each input, in the form: SACBRLDU
 * @clobber D1
 */
.macro READ_INPUT_JOYPAD
  /* set TH pin to begin read sequence */
  move.b  #CTRL_PC6, (a0)
  nop
  nop
  move.b  (a0), d0
  /* clear TH pin for next read */
  move.b  #0, (a0)
  nop
  nop
  move.b  (a0), d1
  /* d0 = 00CBRLDU */
  andi.b  #0x3F, d0
  /* d1 = 00SA0000 */
  andi.b  #0x30, d1
  lsl.b   #2, d1
  or.b    d1, d0
.endm

#endif

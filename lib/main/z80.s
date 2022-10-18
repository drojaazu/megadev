#ifndef MEGADEV__MAIN_Z80_S
#define MEGADEV__MAIN_Z80_S

#include "macros.s"
#include "z80_def.h"

.section .text

/**
 * @fn load_z80_program
 * @brief Load data to the start of Z80 RAM
 * @param[in] A0.l Pointer to load data
 * @param[in] D0.w Data length
 * @clobber a1
 */
FUNC LOAD_Z80_PROGRAM
	INTERRUPT_DISABLE

	Z80_DO_RESET
	Z80_DO_BUSREQ

	lea _Z80_RAM, a1

	subq #1, d0
2:move.b  (a0)+, (a1)+
	dbf	d0, 2b

	Z80_DO_RESET
	Z80_DO_BUSRELEASE
	
	INTERRUPT_ENABLE
	rts

#endif
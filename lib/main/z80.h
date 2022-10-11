/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file z80.h
 * @brief C wrappers for Z80 hardware definitions
 */

#ifndef MEGADEV__Z80_H
#define MEGADEV__Z80_H

#include "types.h"
#include "z80_def.h"

#define Z80_RAM ((volatile u8 *) _Z80_RAM)
#define Z80_BUSREQ ((volatile u16 *) _Z80_BUSREQ)
#define Z80_RESET ((volatile u16 *) _Z80_RESET)

void load_z80_program (u8 * data, u16 length)
{
	register u32 a0_data asm("a0") = (u32) data;
	register u16 d0_length asm("d0") = length;

	asm volatile("jsr LOAD_Z80_PROGRAM"
							 : "+d"(d0_length), "+a"(a0_data)
							 : "d"(d0_length), "a"(a0_data)
							 : "a1", "cc");
};

#endif
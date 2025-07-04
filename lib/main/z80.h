/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file z80.h
 * @brief Z80 CPU Utilities
 */

#ifndef MEGADEV__MAIN_Z80_H
#define MEGADEV__MAIN_Z80_H

#include "types.h"
#include "z80.def.h"

#define Z80_RAM ((volatile u8 *) _Z80_RAM)
#define Z80_BUSREQ ((volatile u16 *) _Z80_BUSREQ)
#define Z80_RESET ((volatile u16 *) _Z80_RESET)

static inline void z80_bus_request()
{
	__asm__(
		"\
		move.w #0x100, _Z80_BUSREQ \n\
		0:btst #0, _Z80_BUSREQ \n\
		bne.s 0b \n\
		"
	:
	:
	:);
}

static inline void z80_bus_release()
{
	__asm__(
		"\
		move.w #0, _Z80_BUSREQ \n\
		"
	:
	:
	:);
}

static inline void z80_cpu_reset()
{
	__asm__(
		"\
		move.w  #0x000, (_Z80_RESET) \n\
		.rept 8 \n\
		nop \n\
		.endr \n\
		move.w  #0x100, (_Z80_RESET) \n\
		"
	:
	:
	:);
}

static inline z80_init()
{
	__asm__(
		"\
		move.w  #0x000, (_Z80_RESET) \n\
		.rept 8 \n\
		nop \n\
		.endr \n\
		move.w  #0x100, (_Z80_RESET) \n\
		"
	:
	:
	:);
}


static inline void load_z80_program(u8 * data, u16 length)
{
	register u32 a0_data __asm__("a0") = (u32) data;
	register u16 d0_length __asm__("d0") = length;

	__asm__ volatile("jsr LOAD_Z80_PROGRAM" : "+d"(d0_length), "+a"(a0_data) : "d"(d0_length), "a"(a0_data) : "a1", "cc");
};

#endif
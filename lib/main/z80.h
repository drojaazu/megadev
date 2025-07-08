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
	asm(
	"\
  move.w   #%c1, (%c0) \n\
0:btst     #%c2, %c0 \n\
  bne.s    0b \n\
	"
	:
	: "i"(_Z80_BUSREQ), "i"(_Z80_REQUEST_BUS), "i"(_Z80_STATUS_BIT)
	:
	);
}

static inline void z80_bus_release()
{
	asm(
	"\
  move.w   #%c1, %c0 \n\
	"
	:
	: "i"(_Z80_BUSREQ), "i"(_Z80_RELEASE_BUS)
	:
	);
}

static inline void z80_cpu_reset()
{
	asm(
	"\
  move.w   #%c1, (%c0) \n\
  .rept 8 \n\
  nop \n\
  .endr \n\
  move.w   #%c2, (%c0) \n\
	"
	:
	: "i"(_Z80_RESET), "i"(_Z80_ASSERT_RESET), "i"(_Z80_RELEASE_RESET)
	:
	);
}

static inline void z80_init(u8 const * data, u16 const length)
{
	asm(
	"\
  move.w   #%c4, (%c1) \n\
  move.w   #%c3, (%c0) \n\
0:btst     #%c6, (%c1) \n\
  bne.s    0b \n\
  lea      (%c7), a1 \n\
1:move.b  (a0)+,(a1)+ \n\
  dbf	    d7, 1b \n\
  move.w   #%c2, (%c0) \n\
  move.w   #%c5, (%c1) \n\
  move.w   #%c3, (%c0) \n\
	"
	:
	:
		"i"(_Z80_RESET),
		"i"(_Z80_BUSREQ),
		"i"(_Z80_ASSERT_RESET),
		"i"(_Z80_RELEASE_RESET),
		"i"(_Z80_REQUEST_BUS),
		"i"(_Z80_RELEASE_BUS),
		"i"(_Z80_STATUS_BIT),
		"i"(_Z80_RAM)
	:
	);
};

#endif
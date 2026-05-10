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

#define z80_ram        ((volatile u8 *) Z80_RAM)
#define z80_reg_busreq ((volatile u16 *) Z80_REG_BUSREQ)
#define z80_reg_reset  ((volatile u16 *) Z80_REG_RESET)

static inline void z80_request_bus()
{
  // clang-format off
	asm(
		"\
  move.w   #%c[_Z80_BUS_REQUEST], (%c[_Z80_REG_BUSREQ]) \n\
0:btst     #%c[_Z80_BIT_STATUS], %c[_Z80_REG_BUSREQ] \n\
  bne.s    0b \n\
	"
		:
		:
			[_Z80_REG_BUSREQ] "i"(Z80_REG_BUSREQ),
			[_Z80_BUS_REQUEST] "i"(Z80_BUS_REQUEST),
			[_Z80_BIT_STATUS] "i"(Z80_BIT_STATUS)
		:
	);
  // clang-format on
}

static inline void z80_release_bus()
{
  // clang-format off
	asm(
		"\
  move.w   #%c[_Z80_BUS_RELEASE], %c[_Z80_REG_BUSREQ] \n\
	"
		:
		:
			[_Z80_REG_BUSREQ] "i"(Z80_REG_BUSREQ),
			[_Z80_BUS_RELEASE] "i"(Z80_BUS_RELEASE)
		:
	);
  // clang-format on
}

static inline void z80_reset_cpu()
{
  // clang-format off
	asm(
		"\
  move.w   #%c[_Z80_RESET_ASSERT], (%c[_Z80_REG_RESET]) \n\
  .rept 8 \n\
  nop \n\
  .endr \n\
  move.w   #%c[_Z80_RESET_RELEASE], (%c[_Z80_REG_RESET]) \n\
	"
		:
		:
			[_Z80_REG_RESET] "i"(Z80_REG_RESET),
			[_Z80_RESET_ASSERT] "i"(Z80_RESET_ASSERT),
			[_Z80_RESET_RELEASE] "i"(Z80_RESET_RELEASE)
		:
			"cc"
	);
  // clang-format on
}

/**
 * @note z80_program_size is the size of the data in bytes MINUS ONE
 */
void z80_init(
  register void const * z80_program, register u16 const z80_program_size)
{
  register u32 scratch_a1;

  // clang-format off
	asm(
		"\
  move.w   #%c[_Z80_BUS_REQUEST], (%c[_Z80_REG_BUSREQ]) \n\
  move.w   #%c[_Z80_RESET_RELEASE], (%c[_Z80_REG_RESET]) \n\
0:btst     #%c[_Z80_BIT_STATUS], (%c[_Z80_REG_BUSREQ]) \n\
  bne.s    0b \n\
  lea      (%c[_Z80_RAM]), %[scratch_a1] \n\
1:move.b   (%[_z80_program])+,(%[scratch_a1])+ \n\
  dbf	     %[_z80_program_size], 1b \n\
  move.w   #%c[_Z80_RESET_ASSERT], (%c[_Z80_REG_RESET]) \n\
  move.w   #%c[_Z80_BUS_RELEASE], (%c[_Z80_REG_BUSREQ]) \n\
  move.w   #%c[_Z80_RESET_RELEASE], (%c[_Z80_REG_RESET]) \n\
	"
		:
			[scratch_a1] "=&a"(scratch_a1)
		:
			[_Z80_REG_RESET] "i"(Z80_REG_RESET),
			[_Z80_REG_BUSREQ] "i"(Z80_REG_BUSREQ),
			[_Z80_RESET_ASSERT] "i"(Z80_RESET_ASSERT),
			[_Z80_RESET_RELEASE] "i"(Z80_RESET_RELEASE),
			[_Z80_BUS_REQUEST] "i"(Z80_BUS_REQUEST),
			[_Z80_BUS_RELEASE] "i"(Z80_BUS_RELEASE),
			[_Z80_BIT_STATUS] "i"(Z80_BIT_STATUS),
			[_Z80_RAM] "i"(Z80_RAM),
			[_z80_program] "a"(z80_program),
			[_z80_program_size] "d"(z80_program_size)
		:
			"cc"
	);
  // clang-format on
};

#endif
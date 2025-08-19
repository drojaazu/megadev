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

#define Z80_RAM		 ((volatile u8 *) _Z80_RAM)
#define Z80_BUSREQ ((volatile u16 *) _Z80_BUSREQ)
#define Z80_RESET	 ((volatile u16 *) _Z80_RESET)

static inline void z80_bus_request()
{
	// clang-format off
	asm(
		"\
  move.w   #%c[z80_request_bus], (%c[z80_busreq]) \n\
0:btst     #%c[z80_status_bit], %c[z80_busreq] \n\
  bne.s    0b \n\
	"
		:
		:
			[z80_busreq] "i"(_Z80_BUSREQ),
			[z80_request_bus] "i"(_Z80_REQUEST_BUS),
			[z80_status_bit] "i"(_Z80_STATUS_BIT)
		:
	);
	// clang-format on
}

static inline void z80_bus_release()
{
	// clang-format off
	asm(
		"\
  move.w   #%c[z80_release_bus], %c[z80_busreq] \n\
	"
		:
		:
			[z80_busreq] "i"(_Z80_BUSREQ),
			[z80_release_bus] "i"(_Z80_RELEASE_BUS)
		:
	);
	// clang-format on
}

static inline void z80_cpu_reset()
{
	// clang-format off
	asm(
		"\
  move.w   #%c[z80_assert_reset], (%c[z80_reset]) \n\
  .rept 8 \n\
  nop \n\
  .endr \n\
  move.w   #%c[z80_release_reset], (%c[z80_reset]) \n\
	"
		:
		:
			[z80_reset] "i"(_Z80_RESET),
			[z80_assert_reset] "i"(_Z80_ASSERT_RESET),
			[z80_release_reset] "i"(_Z80_RELEASE_RESET)
		:
			"cc"
	);
	// clang-format on
}

/**
 * @note z80_program_size is the size of the data in bytes MINUS ONE
 */
void z80_init(
	register void const * z80_program,
	register u16 const		z80_program_size)
{
	register u32 scratch_a1;

	// clang-format off
	asm(
		"\
  move.w   #%c[z80_request_bus], (%c[z80_busreq]) \n\
  move.w   #%c[z80_release_reset], (%c[z80_reset]) \n\
0:btst     #%c[z80_status_bit], (%c[z80_busreq]) \n\
  bne.s    0b \n\
  lea      (%c[z80_ram]), %[scratch_a1] \n\
1:move.b   (%[z80_program])+,(%[scratch_a1])+ \n\
  dbf	     %[z80_program_size], 1b \n\
  move.w   #%c[z80_assert_reset], (%c[z80_reset]) \n\
  move.w   #%c[z80_release_bus], (%c[z80_busreq]) \n\
  move.w   #%c[z80_release_reset], (%c[z80_reset]) \n\
	"
		:
			[scratch_a1] "=&a"(scratch_a1)
		:
			[z80_reset] "i"(_Z80_RESET),
			[z80_busreq] "i"(_Z80_BUSREQ),
			[z80_assert_reset] "i"(_Z80_ASSERT_RESET),
			[z80_release_reset] "i"(_Z80_RELEASE_RESET),
			[z80_request_bus] "i"(_Z80_REQUEST_BUS),
			[z80_release_bus] "i"(_Z80_RELEASE_BUS),
			[z80_status_bit] "i"(_Z80_STATUS_BIT),
			[z80_ram] "i"(_Z80_RAM),
			[z80_program] "a"(z80_program),
			[z80_program_size] "d"(z80_program_size)
		:
			"cc"
	);
	// clang-format on
};

#endif
/**
 * [ M E G A D E V ]
 *
 * Compile-time assertions for the bit-index / mask distinction (INV-6).
 *
 * M68k bit opcodes (btst, bset, bclr, bchg) take a bit INDEX. Passing a mask
 * selects the wrong bit and fails silently -- there is no diagnostic, the code
 * just tests the wrong flag. lib/main/comm.h and comm.macros.s both did this
 * for two years.
 *
 * Every constant used as a bit-opcode operand must therefore have a _BIT
 * companion, and these assertions pin the relationship between the two.
 */

#include <main/io.def.h>
#include <types.h>

/* A mask is (1 << index). If these two ever drift apart, the btst call sites
 * silently start testing the wrong bit again. */
#define ASSERT_BIT_PAIR(name)                                                  \
	_Static_assert((name) == (1 << (name##_BIT)),                                \
		#name " must equal 1 << " #name "_BIT")

ASSERT_BIT_PAIR(SCTRL_TX_FULL);
ASSERT_BIT_PAIR(SCTRL_RX_READY);
ASSERT_BIT_PAIR(SCTRL_RX_ERR);
ASSERT_BIT_PAIR(SCTRL_RX_INT_ENABLE);
ASSERT_BIT_PAIR(SCTRL_SERIAL_OUT_ENABLE);
ASSERT_BIT_PAIR(SCTRL_SERIAL_IN_ENABLE);

/* The specific values the serial code depends on. */
_Static_assert(SCTRL_TX_FULL_BIT == 0, "TX full is bit 0");
_Static_assert(SCTRL_RX_READY_BIT == 1, "RX ready is bit 1");

/* And the trap itself: index and mask must NOT be interchangeable for any bit
 * above 0, which is precisely why passing the mask to btst was wrong. */
_Static_assert(SCTRL_RX_READY != SCTRL_RX_READY_BIT,
	"mask and index differ - passing the mask to btst tests the wrong bit");

/* SCTRL_SERIAL_ENABLE is a genuine multi-bit mask (output+input), not a single
 * flag, so it has no _BIT companion and must never reach a bit opcode. */
_Static_assert(SCTRL_SERIAL_ENABLE ==
		(SCTRL_SERIAL_OUT_ENABLE | SCTRL_SERIAL_IN_ENABLE),
	"SCTRL_SERIAL_ENABLE is the OR of the two enable bits");

/**
 * [ M E G A D E V ]
 *
 * Compile-time assertions for register field definitions (INV-6).
 *
 * Fields follow <SUBSYSTEM>_<FIELD>_<ASPECT> with _POS, _WIDTH and _MASK.
 * A single-bit flag is a field of width 1, so _POS doubles as the operand for
 * btst/bset/bclr/bchg. Field VALUES are stored unshifted and placed with
 * FIELD_PREP.
 *
 * M68k bit opcodes take a bit INDEX. Passing a mask selects the wrong bit and
 * fails silently -- there is no diagnostic, the code just tests the wrong flag.
 * lib/main/comm.h did exactly that for years (KB-11). These assertions pin the
 * relationships so it cannot recur.
 */

#include <build.def.h>
#include <main/gate_arr.def.h>
#include <main/io.def.h>
#include <main/vdp.def.h>
#include <types.h>

/* A field's mask must be exactly the bits it occupies, at its position. */
#define ASSERT_FIELD(name)                                                     \
	_Static_assert((name##_MASK) == (((1 << (name##_WIDTH)) - 1) << (name##_POS)), \
		#name ": MASK must equal WIDTH bits at POS")

/* --- Gate Array, Main CPU side ------------------------------------------ */

ASSERT_FIELD(GA_SUB_RESET);
ASSERT_FIELD(GA_SUB_REQ);
ASSERT_FIELD(GA_RAISE_INT2);
ASSERT_FIELD(GA_INT2_MASKING);
ASSERT_FIELD(GA_RETURN_2M);
ASSERT_FIELD(GA_DMNA);
ASSERT_FIELD(GA_WORDRAM_LAYOUT);
ASSERT_FIELD(GA_CDC_DEST);
ASSERT_FIELD(GA_MEMMODE_BANK);
ASSERT_FIELD(GA_MEMMODE_WP);

/* Values the hardware defines, unchanged by the rename. */
_Static_assert(GA_DMNA_POS == 1, "DMNA is bit 1 of memory mode");
_Static_assert(GA_RETURN_2M_POS == 0, "RET is bit 0 of memory mode");
_Static_assert(GA_MEMMODE_BANK_MASK == 0xC0, "bank select is bits 6-7");
_Static_assert(GA_MEMMODE_WP_MASK == 0xFF, "write protect is the low byte");

/* --- I/O serial control -------------------------------------------------- */

ASSERT_FIELD(SCTRL_TX_FULL);
ASSERT_FIELD(SCTRL_RX_READY);
ASSERT_FIELD(SCTRL_RX_ERR);
ASSERT_FIELD(SCTRL_RX_INT_ENABLE);
ASSERT_FIELD(SCTRL_SERIAL_OUT_ENABLE);
ASSERT_FIELD(SCTRL_SERIAL_IN_ENABLE);
ASSERT_FIELD(SCTRL_BAUD);

_Static_assert(SCTRL_TX_FULL_POS == 0, "TX full is bit 0");
_Static_assert(SCTRL_RX_READY_POS == 1, "RX ready is bit 1");

/* The bug this whole scheme exists to prevent: for any bit above 0, the mask
 * and the position are different numbers, so handing a mask to btst is wrong. */
_Static_assert(SCTRL_RX_READY_MASK != SCTRL_RX_READY_POS,
	"mask and position differ - passing the mask to btst tests the wrong bit");

/* The combined enable is genuinely two bits, so it has no single position. */
_Static_assert(SCTRL_SERIAL_ENABLE_MASK ==
		(SCTRL_SERIAL_OUT_ENABLE_MASK | SCTRL_SERIAL_IN_ENABLE_MASK),
	"SCTRL_SERIAL_ENABLE_MASK is the OR of the two enable bits");

/* --- field values are UNSHIFTED, and FIELD_PREP places them -------------- */

_Static_assert(SCTRL_BAUD_4800 == 0b00, "baud values are stored unshifted");
_Static_assert(SCTRL_BAUD_300 == 0b11, "baud values are stored unshifted");
_Static_assert(FIELD_PREP(SCTRL_BAUD, SCTRL_BAUD_300) == (0b11 << 6),
	"FIELD_PREP shifts a value into place");
_Static_assert(FIELD_GET(SCTRL_BAUD, FIELD_PREP(SCTRL_BAUD, SCTRL_BAUD_2400)) ==
		SCTRL_BAUD_2400,
	"FIELD_PREP and FIELD_GET round trip");
_Static_assert((FIELD_PREP(SCTRL_BAUD, SCTRL_BAUD_300) & ~SCTRL_BAUD_MASK) == 0,
	"FIELD_PREP never sets bits outside the field");

/* --- VDP ----------------------------------------------------------------- */

ASSERT_FIELD(VDP_INTERLACE);
ASSERT_FIELD(VDP_SHADOW_HIGHLIGHT);

/* These were previously named VDP_MASK_INTERLACE_*, but NONE is zero, which no
 * mask can be. They are values, and are now named and stored as such. */
_Static_assert(VDP_INTERLACE_NONE == 0, "interlace off is value 0, not a mask");
_Static_assert(VDP_INTERLACE_DOUBLE == 0b11, "stored unshifted");
_Static_assert(FIELD_PREP(VDP_INTERLACE, VDP_INTERLACE_DOUBLE) == (0b11 << 1),
	"placed at bit 1");

/* 40-cell width sets two NON-adjacent bits, so it cannot be a POS/WIDTH field
 * and is deliberately kept as a literal mask. */
_Static_assert(VDP_WIDTH_40CELL_MASK == 0x81, "bits 0 and 7");

/* --- byte access to 16-bit registers ------------------------------------ */

/* The gate array registers are 16 bit but several are routinely accessed a
 * byte at a time. _HI aliases the register itself and _LO is the next byte;
 * getting these the wrong way round would silently address the wrong half. */
_Static_assert(GA_REG_RESET_HI == GA_REG_RESET, "HI is the register address itself");
_Static_assert(GA_REG_RESET_LO == GA_REG_RESET + 1, "LO is the second byte");
_Static_assert(GA_REG_MEMMODE_HI == GA_REG_MEMMODE, "HI is the register address itself");
_Static_assert(GA_REG_MEMMODE_LO == GA_REG_MEMMODE + 1, "LO is the second byte");

/* The fields the code bit-tests all live in the low byte, which is why that is
 * the half everything addresses. */
_Static_assert(GA_DMNA_POS < 8, "DMNA is in the low byte of memory mode");
_Static_assert(GA_RETURN_2M_POS < 8, "RET is in the low byte of memory mode");
_Static_assert(GA_WORDRAM_LAYOUT_POS < 8, "MODE is in the low byte of memory mode");

/* --- string convention -------------------------------------------------- */

#include <str_util.def.h>

/* Megadev strings end with 0xFF, not NUL, because the Boot ROM print routines
 * require it and treat 0x00 as a newline. The C and assembly implementations
 * of hextoa* now share this one definition; they previously each hardcoded a
 * value and the C side omitted the terminator altogether. */
_Static_assert(STRING_TERMINATOR == 0xFF, "Boot ROM print expects an 0xFF terminator");
_Static_assert(STRING_TERMINATOR != 0, "0x00 is a newline here, not a terminator");

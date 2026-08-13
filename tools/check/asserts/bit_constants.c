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

/* A field on a byte register must fit inside that byte -- the invariant that
 * makes the D17 split model safe, since it is what lets _POS go straight to a
 * bit opcode with no FIELD_BYTE/FIELD_BPOS. */
#define ASSERT_BYTE_FIELD(name)                                                \
	ASSERT_FIELD(name);                                                          \
	_Static_assert((name##_POS) + (name##_WIDTH) <= 8,                           \
		#name ": a field on a byte register must fit in 8 bits");                  \
	_Static_assert(FIELD_BPOS(name) == (name##_POS),                             \
		#name ": on a byte register FIELD_BPOS is the identity")

/* 0xA12000, split into two byte registers (D17). */
_Static_assert(GA_REG_INT2 == 0xA12000, "INT2 control is the high byte");
_Static_assert(GA_REG_SUBCPU == GA_REG_INT2 + 1, "Sub CPU control is the low byte");

ASSERT_BYTE_FIELD(GA_SUB_RESET);
ASSERT_BYTE_FIELD(GA_SUB_REQ);
ASSERT_BYTE_FIELD(GA_RAISE_INT2);
ASSERT_BYTE_FIELD(GA_INT2_MASKING);

/* These two both sit at bit 0 and always did. As fields of one 16-bit register
 * that was an outright collision -- GA_RAISE_INT2 is IFL2 in the high byte and
 * GA_SUB_RESET is SRES in the low byte, and nothing in the source said so. */
_Static_assert(GA_RAISE_INT2_POS == GA_SUB_RESET_POS,
	"same bit number in different registers - which is why they are different "
	"registers now");
_Static_assert(GA_INT2_MASKING_POS == 7, "IEN2 is the top bit of GA_REG_INT2");

/* 0xA12002, likewise. */
_Static_assert(GA_REG_WP == 0xA12002, "write protect is the high byte");
_Static_assert(GA_REG_MEMMODE == GA_REG_WP + 1, "memory mode is the low byte");

ASSERT_BYTE_FIELD(GA_WP);
ASSERT_BYTE_FIELD(GA_RETURN_2M);
ASSERT_BYTE_FIELD(GA_DMNA);
ASSERT_BYTE_FIELD(GA_WORDRAM_LAYOUT);
ASSERT_BYTE_FIELD(GA_MEMMODE_BANK);

_Static_assert(GA_WP_MASK == 0xFF, "write protect is a whole byte of its own now");
_Static_assert(GA_MEMMODE_BANK_MASK == 0xC0, "bank select is bits 6-7 of the low byte");

/* The forced reset sequence is only recognised as an exact pattern of accesses,
 * and the first of them is a WORD write to 0xA12002 -- the address GA_REG_WP
 * names. Splitting the register must not have moved it. */
_Static_assert(GA_REG_WP == 0xA12002,
	"RESET_GA writes its magic word here; the forced reset depends on the address");

/* 0xA12004 has no low byte at all: every field is in the high byte and the
 * Main CPU may only read it, so the whole register is one read-only byte. */
_Static_assert(GA_REG_CDCMODE == 0xA12004, "CDC mode is the high byte");
ASSERT_BYTE_FIELD(GA_CDC_DEST);
ASSERT_BYTE_FIELD(GA_CDC_DSR);
ASSERT_BYTE_FIELD(GA_CDC_EDT);
_Static_assert(GA_CDC_DEST_MASK == 0x07, "device destination is bits 0-2 of that byte");

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

/* --- the split model (D17) ---------------------------------------------- */

/* Every Main-side register with fields in both halves is now a pair of byte
 * registers, so FIELD_BYTE/FIELD_BPOS have no users left on this side. They
 * remain for the Sub CPU's CDC mode register, which genuinely spans both bytes;
 * see bit_constants_sub.c and field_access.s. */
_Static_assert(GA_REG_SUBCPU - GA_REG_INT2 == 1 && GA_REG_MEMMODE - GA_REG_WP == 1,
	"each split pair is two adjacent bytes, high half first");

/* --- string convention -------------------------------------------------- */

#include <str_util.def.h>

/* Megadev strings end with 0xFF, not NUL, because the Boot ROM print routines
 * require it and treat 0x00 as a newline. The C and assembly implementations
 * of hextoa* now share this one definition; they previously each hardcoded a
 * value and the C side omitted the terminator altogether. */
_Static_assert(STRING_TERMINATOR == 0xFF, "Boot ROM print expects an 0xFF terminator");
_Static_assert(STRING_TERMINATOR != 0, "0x00 is a newline here, not a terminator");

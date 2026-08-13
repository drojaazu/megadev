/**
 * [ M E G A D E V ]
 *
 * Compile-time assertions for Sub CPU register field definitions (INV-6, D17).
 *
 * Separate from bit_constants.c because the Main and Sub gate array headers
 * define the same macro names with different values (OD-1); they cannot share
 * a translation unit.
 */

#include <build.def.h>
#include <sub/gate_arr.def.h>
#include <types.h>

#define ASSERT_FIELD(name)                                                     \
	_Static_assert((name##_MASK) == (((1 << (name##_WIDTH)) - 1) << (name##_POS)), \
		#name ": MASK must equal WIDTH bits at POS")

/* A field on a byte register must fit inside that byte. This is the invariant
 * that makes the split model safe: it is what lets _POS go straight to a bit
 * opcode with no FIELD_BYTE/FIELD_BPOS in sight. */
#define ASSERT_BYTE_FIELD(name)                                                \
	ASSERT_FIELD(name);                                                          \
	_Static_assert((name##_POS) + (name##_WIDTH) <= 8,                           \
		#name ": a field on a byte register must fit in 8 bits");                  \
	_Static_assert(FIELD_BPOS(name) == (name##_POS),                             \
		#name ": on a byte register FIELD_BPOS is the identity")

/* --- 0xFF8000, split into two byte registers (D17) ----------------------- */

_Static_assert(GA_REG_LED == 0xFF8000, "LED is the high byte, at the base address");
_Static_assert(GA_REG_SUBCTRL == GA_REG_LED + 1, "SUBCTRL is the low byte");

ASSERT_BYTE_FIELD(GA_LED_R);
ASSERT_BYTE_FIELD(GA_LED_G);
ASSERT_BYTE_FIELD(GA_VERSION);
ASSERT_BYTE_FIELD(GA_PERIPH_RESET);

_Static_assert(GA_LED_R_MASK == 0x01, "red is bit 0 of GA_REG_LED");
_Static_assert(GA_LED_G_MASK == 0x02, "green is bit 1 of GA_REG_LED");

/* The trap this split removes: bit 0 of the old 16-bit register was RES0, the
 * peripheral reset, while bit 0 of the LED byte is the red LED. One number,
 * two entirely different bits, and nothing in the source said which was meant
 * (KB-34). They are now fields of different registers and cannot be confused. */
_Static_assert(GA_LED_R_POS == GA_PERIPH_RESET_POS,
	"same bit number in different registers - which is exactly why they are "
	"different registers now");

/* --- 0xFF8002, likewise -------------------------------------------------- */

_Static_assert(GA_REG_WP == 0xFF8002, "write protect is the high byte");
_Static_assert(GA_REG_MEMMODE == GA_REG_WP + 1, "memory mode is the low byte");

ASSERT_BYTE_FIELD(GA_WP);
ASSERT_BYTE_FIELD(GA_RETURN_2M);
ASSERT_BYTE_FIELD(GA_DMNA);
ASSERT_BYTE_FIELD(GA_WORDRAM_LAYOUT);
ASSERT_BYTE_FIELD(GA_PRIORITY);

/* Word-wide access to memory mode was how write protection got cleared by
 * accident: the two concerns shared a register and nothing separated them. */
_Static_assert(GA_WP_MASK == 0xFF, "write protect is a whole byte of its own now");
_Static_assert(FIELD_PREP(GA_PRIORITY, GA_PRIORITY_OVERWRITE) == (0b10 << 3),
	"priority values are stored unshifted and placed by FIELD_PREP");

/* --- 0xFF8004 is NOT split: its fields span both halves ------------------ */

/* Kept 16-bit because CA sits in the low byte and the status flags in the high
 * one, but they are read and written together as one CDC transaction. This is
 * the case FIELD_BYTE/FIELD_BPOS still exists for. */
ASSERT_FIELD(GA_CDCMODE_CA);
ASSERT_FIELD(GA_CDCMODE_DD);
ASSERT_FIELD(GA_CDCMODE_DSR);
ASSERT_FIELD(GA_CDCMODE_EDT);
_Static_assert(GA_CDCMODE_DSR_POS >= 8 && GA_CDCMODE_CA_POS < 8,
	"CDC mode genuinely spans both bytes, so it stays a 16-bit register");

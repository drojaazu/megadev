/**
 * [ M E G A D E V ]
 *
 * Compile-time assertions for Sub CPU register field definitions (INV-6).
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

/* --- Gate Array, Sub CPU side ------------------------------------------- */

ASSERT_FIELD(GA_LED_R);
ASSERT_FIELD(GA_LED_G);

/* The LEDs are bits 8 and 9 of $FF8000, i.e. bits 0 and 1 of its HIGH byte.
 * They were defined as bits 0 and 1 of the word, which is the same numbering
 * seen through a byte-sized lens but names two entirely different bits of the
 * register -- and RES0 at bit 0 is one of them (KB-34). */
_Static_assert(GA_LED_R_POS == 8, "LED red is word bit 8");
_Static_assert(GA_LED_G_POS == 9, "LED green is word bit 9");
_Static_assert((GA_LED_R_MASK | GA_LED_G_MASK) == 0x0300, "both LEDs are in the high byte");

/* Which makes them exactly the case FIELD_BYTE exists for: `bset #8` on the
 * register address is taken modulo 8 and would set bit 0 of the high byte --
 * silently the wrong LED, with no diagnostic. */
_Static_assert(FIELD_BYTE(GA_REG_RESET, GA_LED_R) == GA_REG_RESET_HI,
	"the LEDs are reached through the high byte");
_Static_assert(FIELD_BPOS(GA_LED_R) == 0, "red is bit 0 of that byte");
_Static_assert(FIELD_BPOS(GA_LED_G) == 1, "green is bit 1 of that byte");

/**
 * [ M E G A D E V ]
 *
 * Assembly-side assertions for the field access macros (INV-12).
 *
 * These exist as a separate assembly translation unit because the C asserts
 * cannot see this failure mode at all: FIELD_BYTE is used mainly from assembly,
 * where the operand of a bit opcode has to be a byte address, and GNU as
 * accepts a much smaller expression grammar than C does. A first cut of
 * FIELD_BYTE used a ternary, which C compiles happily and GNU as rejects
 * outright -- and the C-only assertions passed it.
 *
 * .if/.error is the assembler's equivalent of _Static_assert: the expression is
 * evaluated at assembly time and a mismatch stops the build.
 */

#include <sub/gate_arr.def.h>

.macro ASSERT_EQ expr, want, msg
.if (\expr) != (\want)
.error "\msg"
.endif
.endm

/* The macros must expand at all, in the assembler's grammar. */
ASSERT_EQ FIELD_BYTE(GA_REG_RESET, GA_LED_R), GA_REG_RESET_HI, "LED is reached through the high byte"
ASSERT_EQ FIELD_BYTE(GA_REG_MEMMODE, GA_DMNA), GA_REG_MEMMODE_LO, "DMNA is in the low byte"

/* Both halves, at every boundary, so an off-by-one in the shift is caught. */
ASSERT_EQ FIELD_BYTE(GA_REG_RESET, GA_VERSION), GA_REG_RESET_LO, "a low-byte field -> low byte"
ASSERT_EQ FIELD_BPOS(GA_LED_R), 0, "word bit 8 is bit 0 of the high byte"
ASSERT_EQ FIELD_BPOS(GA_LED_G), 1, "word bit 9 is bit 1 of the high byte"
ASSERT_EQ FIELD_BPOS(GA_VERSION), GA_VERSION_POS, "low-byte fields are unchanged"

/* The result must always be a legal bit-opcode operand. Tested as idempotence
 * under & 7 rather than a comparison, because GNU as does not yield 1 for a
 * true relational expression the way C does. */
ASSERT_EQ (FIELD_BPOS(GA_LED_G) & 7), FIELD_BPOS(GA_LED_G), "FIELD_BPOS always yields 0-7"

/* And it must genuinely assemble into a bit instruction, not merely evaluate. */
.text
bset #FIELD_BPOS(GA_LED_R), FIELD_BYTE(GA_REG_RESET, GA_LED_R)
bclr #FIELD_BPOS(GA_LED_G), FIELD_BYTE(GA_REG_RESET, GA_LED_G)
btst #FIELD_BPOS(GA_DMNA), FIELD_BYTE(GA_REG_MEMMODE, GA_DMNA)

/* The CDC status flags are the live case: cdrom.s bit-tests them every sector,
 * and they sit at word bits 14 and 15. */
ASSERT_EQ FIELD_BYTE(GA_REG_CDCMODE, GA_CDCMODE_DSR), GA_REG_CDCMODE, "DSR is in the high byte"
ASSERT_EQ FIELD_BPOS(GA_CDCMODE_DSR), 6, "word bit 14 is bit 6 of the high byte"
ASSERT_EQ FIELD_BPOS(GA_CDCMODE_EDT), 7, "word bit 15 is bit 7 of the high byte"
btst #FIELD_BPOS(GA_CDCMODE_DSR), FIELD_BYTE(GA_REG_CDCMODE, GA_CDCMODE_DSR)

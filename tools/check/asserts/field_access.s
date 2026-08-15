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

/* The macros must expand at all, in the assembler's grammar.
 *
 * Since D17 the registers whose halves hold unrelated concerns are separate
 * byte registers, so the live users of these macros are the registers that
 * genuinely span both bytes -- chiefly CDC mode at 0xFF8004, which cdrom.s
 * bit-tests every sector. */
ASSERT_EQ FIELD_BYTE(GA_REG_CDCMODE, GA_CDCMODE_DSR), GA_REG_CDCMODE, "DSR is in the high byte"
ASSERT_EQ FIELD_BYTE(GA_REG_CDCMODE, GA_CDCMODE_CA), GA_REG_CDCMODE + 1, "CA is in the low byte"
ASSERT_EQ FIELD_BPOS(GA_CDCMODE_DSR), 6, "word bit 14 is bit 6 of the high byte"
ASSERT_EQ FIELD_BPOS(GA_CDCMODE_EDT), 7, "word bit 15 is bit 7 of the high byte"
ASSERT_EQ FIELD_BPOS(GA_CDCMODE_DD), 0, "word bit 8 is bit 0 of the high byte"

/* Both halves at the boundary, so an off-by-one in the shift is caught. */
ASSERT_EQ FIELD_BYTE(0, GA_CDCMODE_DD), 0, "bit 8 -> the register address itself"
ASSERT_EQ FIELD_BYTE(0, GA_CDCMODE_CA), 1, "bit 0 -> one past it"

/* The result must always be a legal bit-opcode operand. Tested as idempotence
 * under & 7 rather than a comparison, because GNU as does not yield 1 for a
 * true relational expression the way C does. */
ASSERT_EQ (FIELD_BPOS(GA_CDCMODE_EDT) & 7), FIELD_BPOS(GA_CDCMODE_EDT), "FIELD_BPOS always yields 0-7"

/* On a byte register the helpers must be the identity, or the split model and
 * the helper model would disagree about the same field. */
ASSERT_EQ FIELD_BPOS(GA_LED_R), GA_LED_R_POS, "byte-register fields are unchanged"
ASSERT_EQ FIELD_BPOS(GA_DMNA), GA_DMNA_POS, "byte-register fields are unchanged"

/* And they must genuinely assemble into bit instructions, not merely evaluate. */
.text
btst #FIELD_BPOS(GA_CDCMODE_DSR), FIELD_BYTE(GA_REG_CDCMODE, GA_CDCMODE_DSR)
btst #FIELD_BPOS(GA_CDCMODE_EDT), FIELD_BYTE(GA_REG_CDCMODE, GA_CDCMODE_EDT)

/* A split byte register needs no helper at all -- that is the point of D17. */
bset #GA_LED_R_POS, GA_REG_LED
bclr #GA_LED_G_POS, GA_REG_LED
btst #GA_DMNA_POS, GA_REG_MEMMODE

/**
 * Assembly-side assertions for the register field helpers.
 *
 * GNU as accepts a smaller expression grammar than C, and FIELD_BYTE and
 * FIELD_BPOS exist for bit instructions in assembly, so they are checked
 * here in that grammar and used in real instructions.
 */

#include <sub/gate_arr.def.h>

.macro ASSERT_EQ expr, want, msg
.if (\expr) != (\want)
.error "\msg"
.endif
.endm

/* CDC mode spans both bytes of a 16-bit register */
ASSERT_EQ FIELD_BYTE(GA_SUB_REG_CDCMODE, GA_SUB_CDCMODE_DSR), GA_SUB_REG_CDCMODE, "DSR is in the high byte"
ASSERT_EQ FIELD_BYTE(GA_SUB_REG_CDCMODE, GA_SUB_CDCMODE_CA), GA_SUB_REG_CDCMODE + 1, "CA is in the low byte"
ASSERT_EQ FIELD_BPOS(GA_SUB_CDCMODE_DD), 0, "word bit 8 is bit 0 of the high byte"
ASSERT_EQ FIELD_BPOS(GA_SUB_CDCMODE_DSR), 6, "word bit 14 is bit 6 of the high byte"
ASSERT_EQ FIELD_BPOS(GA_SUB_CDCMODE_EDT), 7, "word bit 15 is bit 7 of the high byte"
ASSERT_EQ (FIELD_BPOS(GA_SUB_CDCMODE_EDT) & 7), FIELD_BPOS(GA_SUB_CDCMODE_EDT), "FIELD_BPOS yields 0-7"

/* On a byte register the helpers are the identity */
ASSERT_EQ FIELD_BPOS(GA_SUB_LED_R), GA_SUB_LED_R_POS, "byte register position"
ASSERT_EQ FIELD_BPOS(GA_SUB_DMNA), GA_SUB_DMNA_POS, "byte register position"

/* The helpers must produce legal bit-instruction operands */
.text
  btst    #FIELD_BPOS(GA_SUB_CDCMODE_DSR), FIELD_BYTE(GA_SUB_REG_CDCMODE, GA_SUB_CDCMODE_DSR)
  btst    #FIELD_BPOS(GA_SUB_CDCMODE_EDT), FIELD_BYTE(GA_SUB_REG_CDCMODE, GA_SUB_CDCMODE_EDT)
  bset    #GA_SUB_LED_R_POS, GA_SUB_REG_LED
  bclr    #GA_SUB_LED_G_POS, GA_SUB_REG_LED
  btst    #GA_SUB_DMNA_POS, GA_SUB_REG_MEMMODE

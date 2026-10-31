/**
 * Compile-time assertions for the I/O serial control and VDP mode register 4
 * field definitions.
 */

#include <build.def.h>
#include <main/io.def.h>
#include <main/vdp.def.h>

#define ASSERT_FLAG(name) \
  _Static_assert((name##_MASK) == (1 << (name##_POS)), #name ": flag mask is 1 << POS")

#define ASSERT_FIELD(name)                                                            \
  _Static_assert((name##_MASK) == (((1 << (name##_WIDTH)) - 1) << (name##_POS)), \
    #name ": MASK is WIDTH bits at POS")

/* Serial control register */
ASSERT_FLAG(SCTRL_TX_FULL);
ASSERT_FLAG(SCTRL_RX_READY);
ASSERT_FLAG(SCTRL_RX_ERR);
ASSERT_FLAG(SCTRL_RX_INT_ENABLE);
ASSERT_FLAG(SCTRL_SERIAL_OUT_ENABLE);
ASSERT_FLAG(SCTRL_SERIAL_IN_ENABLE);
ASSERT_FIELD(SCTRL_BAUD);
_Static_assert(SCTRL_TX_FULL_POS == 0 && SCTRL_RX_READY_POS == 1 && SCTRL_RX_ERR_POS == 2
    && SCTRL_RX_INT_ENABLE_POS == 3 && SCTRL_SERIAL_OUT_ENABLE_POS == 4 && SCTRL_SERIAL_IN_ENABLE_POS == 5,
  "TFUL, RRDY, RERR, R_INT, SOUT, SIN");
_Static_assert(SCTRL_SERIAL_ENABLE_MASK == 0x30, "SOUT | SIN");
_Static_assert(SCTRL_BAUD_MASK == 0xC0, "baud is bits 6-7");
_Static_assert(SCTRL_BAUD_4800 == 0 && SCTRL_BAUD_2400 == 1 && SCTRL_BAUD_1200 == 2 && SCTRL_BAUD_300 == 3,
  "baud values, unshifted");
_Static_assert(FIELD_PREP(SCTRL_BAUD, SCTRL_BAUD_300) == 0xC0, "FIELD_PREP places a value");
_Static_assert(FIELD_GET(SCTRL_BAUD, FIELD_PREP(SCTRL_BAUD, SCTRL_BAUD_2400)) == SCTRL_BAUD_2400, "round trip");

/* VDP mode register 4 */
_Static_assert(VDP_WIDTH_40CELL_MASK == 0x81, "RS0 and RS1");
ASSERT_FLAG(VDP_SHADOW_HIGHLIGHT);
ASSERT_FIELD(VDP_INTERLACE);
_Static_assert(VDP_SHADOW_HIGHLIGHT_POS == 3, "STE");
_Static_assert(VDP_INTERLACE_MASK == 0x06, "LSM0-1");
_Static_assert(VDP_INTERLACE_NONE == 0 && VDP_INTERLACE_NORMAL == 2 && VDP_INTERLACE_DOUBLE == 3,
  "interlace values, unshifted");
_Static_assert(FIELD_PREP(VDP_INTERLACE, VDP_INTERLACE_DOUBLE) == 0x06, "FIELD_PREP places a value");

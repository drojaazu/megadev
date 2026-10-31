/**
 * Compile-time assertions for the Main CPU gate array definitions.
 *
 * Positions and widths come from the Mega-CD Hardware Manual, Main CPU
 * register section. A field's mask must be exactly the bits it occupies; a
 * field on a byte register must fit in that byte.
 */

#include <build.def.h>
#include <main/gate_arr.def.h>
#include <types.h>

#define ASSERT_FLAG(name) \
  _Static_assert((name##_MASK) == (1 << (name##_POS)), #name ": flag mask is 1 << POS")

#define ASSERT_FIELD(name)                                                            \
  _Static_assert((name##_MASK) == (((1 << (name##_WIDTH)) - 1) << (name##_POS)), \
    #name ": MASK is WIDTH bits at POS")

#define ASSERT_BYTE_FLAG(name) \
  ASSERT_FLAG(name);           \
  _Static_assert((name##_POS) < 8, #name ": fits in a byte register")

#define ASSERT_BYTE_FIELD(name) \
  ASSERT_FIELD(name);           \
  _Static_assert((name##_POS) + (name##_WIDTH) <= 8, #name ": fits in a byte register")

/* 0xA12000: INT2 control in the high byte, Sub CPU control in the low byte */
_Static_assert(GA_MAIN_REG_INT2 == 0xA12000, "INT2");
_Static_assert(GA_MAIN_REG_SUBCPU == 0xA12001, "SUBCPU");
ASSERT_BYTE_FLAG(GA_MAIN_RAISE_INT2);
ASSERT_BYTE_FLAG(GA_MAIN_INT2_MASKING);
ASSERT_BYTE_FLAG(GA_MAIN_SUB_RESET);
ASSERT_BYTE_FLAG(GA_MAIN_SUB_REQ);
_Static_assert(GA_MAIN_RAISE_INT2_POS == 0 && GA_MAIN_INT2_MASKING_POS == 7, "IFL2, IEN2");
_Static_assert(GA_MAIN_SUB_RESET_POS == 0 && GA_MAIN_SUB_REQ_POS == 1, "SRES, SBRQ");

/* 0xA12002: write protect in the high byte, memory mode in the low byte */
_Static_assert(GA_MAIN_REG_WP == 0xA12002, "WP");
_Static_assert(GA_MAIN_REG_MEMMODE == 0xA12003, "MEMMODE");
ASSERT_BYTE_FIELD(GA_MAIN_WP);
ASSERT_BYTE_FLAG(GA_MAIN_RETURN_2M);
ASSERT_BYTE_FLAG(GA_MAIN_DMNA);
ASSERT_BYTE_FLAG(GA_MAIN_WORDRAM_LAYOUT);
ASSERT_BYTE_FIELD(GA_MAIN_MEMMODE_BANK);
_Static_assert(GA_MAIN_WP_MASK == 0xFF, "WP0-7");
_Static_assert(GA_MAIN_RETURN_2M_POS == 0 && GA_MAIN_DMNA_POS == 1 && GA_MAIN_WORDRAM_LAYOUT_POS == 2,
  "RET, DMNA, MODE");
_Static_assert(GA_MAIN_MEMMODE_BANK_MASK == 0xC0, "BK0-1");

/* 0xA12004: CDC mode, read only, all fields in the high byte */
_Static_assert(GA_MAIN_REG_CDCMODE == 0xA12004, "CDCMODE");
ASSERT_BYTE_FIELD(GA_MAIN_CDC_DEST);
ASSERT_BYTE_FLAG(GA_MAIN_CDC_DSR);
ASSERT_BYTE_FLAG(GA_MAIN_CDC_EDT);
_Static_assert(GA_MAIN_CDC_DEST_MASK == 0x07, "DD0-2");
_Static_assert(GA_MAIN_CDC_DSR_POS == 6 && GA_MAIN_CDC_EDT_POS == 7, "DSR, EDT");
_Static_assert(GA_MAIN_CDC_DEST_MAIN == 2 && GA_MAIN_CDC_DEST_SUB == 3 && GA_MAIN_CDC_DEST_PCM == 4
    && GA_MAIN_CDC_DEST_PRG == 5 && GA_MAIN_CDC_DEST_WORD == 7,
  "device destination values, unshifted");
_Static_assert(FIELD_PREP(GA_MAIN_CDC_DEST, GA_MAIN_CDC_DEST_WORD) == 0x07, "FIELD_PREP places a value");

/* 0xA1200E: each CPU's flags are one byte */
_Static_assert(GA_MAIN_REG_COMFLAGS_MAIN == 0xA1200E, "COMFLAGS main");
_Static_assert(GA_MAIN_REG_COMFLAGS_SUB == 0xA1200F, "COMFLAGS sub");

/* Eight command words then eight status words */
_Static_assert(GA_MAIN_REG_COMCMD0 == 0xA12010 && GA_MAIN_REG_COMCMD7 == 0xA1201E, "COMCMD");
_Static_assert(GA_MAIN_REG_COMSTAT0 == 0xA12020 && GA_MAIN_REG_COMSTAT7 == 0xA1202E, "COMSTAT");

/* The rest */
_Static_assert(GA_MAIN_REG_HBLANKVECT == 0xA12006, "HBLANK vector");
_Static_assert(GA_MAIN_REG_CDCHOSTDATA == 0xA12008, "CDC host data");
_Static_assert(GA_MAIN_REG_STOPWATCH == 0xA1200C, "stopwatch");

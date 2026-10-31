/**
 * Compile-time assertions for the Sub CPU gate array definitions.
 *
 * Positions and widths come from the Mega-CD Hardware Manual and the Mega-CD
 * Software Development Manual, Sub CPU register sections. A field's mask must
 * be exactly the bits it occupies; a field on a byte register must fit in
 * that byte.
 */

#include <build.def.h>
#include <sub/gate_arr.def.h>
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

/* 0xFF8000: LEDs and version in the high byte, peripheral reset in the low */
_Static_assert(GA_SUB_REG_LED == 0xFF8000, "LED");
_Static_assert(GA_SUB_REG_SUBCTRL == 0xFF8001, "SUBCTRL");
ASSERT_BYTE_FLAG(GA_SUB_LED_R);
ASSERT_BYTE_FLAG(GA_SUB_LED_G);
ASSERT_BYTE_FIELD(GA_SUB_VERSION);
ASSERT_BYTE_FLAG(GA_SUB_PERIPH_RESET);
_Static_assert(GA_SUB_LED_R_MASK == 0x01 && GA_SUB_LED_G_MASK == 0x02, "LEDR, LEDG");
_Static_assert(GA_SUB_VERSION_MASK == 0xF0, "VER0-3");
_Static_assert(GA_SUB_PERIPH_RESET_POS == 0, "RES0");

/* 0xFF8002: write protect in the high byte, memory mode in the low byte */
_Static_assert(GA_SUB_REG_WP == 0xFF8002, "WP");
_Static_assert(GA_SUB_REG_MEMMODE == 0xFF8003, "MEMMODE");
ASSERT_BYTE_FIELD(GA_SUB_WP);
ASSERT_BYTE_FLAG(GA_SUB_RETURN_2M);
ASSERT_BYTE_FLAG(GA_SUB_DMNA);
ASSERT_BYTE_FLAG(GA_SUB_WORDRAM_LAYOUT);
ASSERT_BYTE_FIELD(GA_SUB_PRIORITY);
_Static_assert(GA_SUB_RETURN_2M_POS == 0 && GA_SUB_DMNA_POS == 1 && GA_SUB_WORDRAM_LAYOUT_POS == 2,
  "RET, DMNA, MODE");
_Static_assert(GA_SUB_PRIORITY_MASK == 0x18, "PM0-1");
_Static_assert(GA_SUB_PRIORITY_OFF == 0 && GA_SUB_PRIORITY_UNDERWRITE == 1 && GA_SUB_PRIORITY_OVERWRITE == 2,
  "priority values, unshifted");
_Static_assert(FIELD_PREP(GA_SUB_PRIORITY, GA_SUB_PRIORITY_OVERWRITE) == 0x10, "FIELD_PREP places a value");

/* 0xFF8004: CDC mode stays 16-bit; CA is in the low byte, the flags in the high */
_Static_assert(GA_SUB_REG_CDCMODE == 0xFF8004, "CDCMODE");
ASSERT_FIELD(GA_SUB_CDCMODE_CA);
ASSERT_FIELD(GA_SUB_CDCMODE_DD);
ASSERT_FLAG(GA_SUB_CDCMODE_UBR);
ASSERT_FLAG(GA_SUB_CDCMODE_DSR);
ASSERT_FLAG(GA_SUB_CDCMODE_EDT);
_Static_assert(GA_SUB_CDCMODE_CA_MASK == 0x000F, "CA0-3");
_Static_assert(GA_SUB_CDCMODE_DD_MASK == 0x0700, "DD0-2");
_Static_assert(GA_SUB_CDCMODE_UBR_POS == 13 && GA_SUB_CDCMODE_DSR_POS == 14 && GA_SUB_CDCMODE_EDT_POS == 15,
  "UBR, DSR, EDT");
_Static_assert(GA_SUB_CDCMODE_DD_MAINREAD == 2 && GA_SUB_CDCMODE_DD_SUBREAD == 3 && GA_SUB_CDCMODE_DD_PCMDMA == 4
    && GA_SUB_CDCMODE_DD_PRGDMA == 5 && GA_SUB_CDCMODE_DD_WRAMDMA == 7,
  "device destination values, unshifted");
_Static_assert(FIELD_PREP(GA_SUB_CDCMODE_DD, GA_SUB_CDCMODE_DD_WRAMDMA) == 0x0700, "FIELD_PREP places a value");
_Static_assert(FIELD_GET(GA_SUB_CDCMODE_DD, 0x0300) == GA_SUB_CDCMODE_DD_SUBREAD, "FIELD_GET extracts it");

/* 0xFF8006 to 0xFF800C */
_Static_assert(GA_SUB_REG_CDCRS1 == 0xFF8006, "CDCRS1");
ASSERT_FIELD(GA_SUB_CDCRS1_CD);
_Static_assert(GA_SUB_REG_CDCHOSTDATA == 0xFF8008, "CDC host data");
_Static_assert(GA_SUB_REG_DMAADDR == 0xFF800A, "DMA address");
_Static_assert(GA_SUB_REG_STOPWATCH == 0xFF800C, "stopwatch");
ASSERT_FIELD(GA_SUB_STOPWATCH_SW);
_Static_assert(GA_SUB_STOPWATCH_SW_MASK == 0x0FFF, "SW0-11");

/* 0xFF800E: each CPU's flags are one byte */
_Static_assert(GA_SUB_REG_COMFLAGS_MAIN == 0xFF800E, "COMFLAGS main");
_Static_assert(GA_SUB_REG_COMFLAGS_SUB == 0xFF800F, "COMFLAGS sub");

/* Eight command words then eight status words */
_Static_assert(GA_SUB_REG_COMCMD0 == 0xFF8010 && GA_SUB_REG_COMCMD7 == 0xFF801E, "COMCMD");
_Static_assert(GA_SUB_REG_COMSTAT0 == 0xFF8020 && GA_SUB_REG_COMSTAT7 == 0xFF802E, "COMSTAT");

/* 0xFF8030 and 0xFF8032: only the low byte is used, so the registers sit there */
_Static_assert(GA_SUB_REG_INT3TIMER == 0xFF8031, "INT3 timer");
_Static_assert(GA_SUB_REG_INTMASK == 0xFF8033, "interrupt mask");
ASSERT_BYTE_FLAG(GA_SUB_INT1);
ASSERT_BYTE_FLAG(GA_SUB_INT2);
ASSERT_BYTE_FLAG(GA_SUB_INT3);
ASSERT_BYTE_FLAG(GA_SUB_INT4);
ASSERT_BYTE_FLAG(GA_SUB_INT5);
ASSERT_BYTE_FLAG(GA_SUB_INT6);
_Static_assert((GA_SUB_INT1_MASK | GA_SUB_INT2_MASK | GA_SUB_INT3_MASK | GA_SUB_INT4_MASK | GA_SUB_INT5_MASK
                 | GA_SUB_INT6_MASK)
    == 0x7E,
  "levels 1-6 are bits 1-6");

/* 0xFF8034 CD fader: EFDT, FD00-10, DEF0-1, SSF tile the register but for bit 0 */
_Static_assert(GA_SUB_REG_CDFADER == 0xFF8034, "CD fader");
ASSERT_FIELD(GA_SUB_CDFADER_FD);
ASSERT_FLAG(GA_SUB_CDFADER_EFDT);
ASSERT_FIELD(GA_SUB_CDFADER_DEF);
ASSERT_FLAG(GA_SUB_CDFADER_SSF);
_Static_assert(GA_SUB_CDFADER_EFDT_MASK == 0x8000 && GA_SUB_CDFADER_FD_MASK == 0x7FF0
    && GA_SUB_CDFADER_DEF_MASK == 0x000C && GA_SUB_CDFADER_SSF_MASK == 0x0002,
  "EFDT, FD, DEF, SSF");
_Static_assert((GA_SUB_CDFADER_EFDT_MASK | GA_SUB_CDFADER_FD_MASK | GA_SUB_CDFADER_DEF_MASK
                 | GA_SUB_CDFADER_SSF_MASK)
    == 0xFFFE,
  "the fader fields tile the register, leaving bit 0");

/* 0xFF8036 CDD control */
_Static_assert(GA_SUB_REG_CDDCTRL == 0xFF8036, "CDD control");
ASSERT_FLAG(GA_SUB_CDDCTRL_DTS);
ASSERT_FLAG(GA_SUB_CDDCTRL_DRS);
ASSERT_FLAG(GA_SUB_CDDCTRL_HOCK);
ASSERT_FLAG(GA_SUB_CDDCTRL_DM);
_Static_assert(GA_SUB_CDDCTRL_DTS_POS == 0 && GA_SUB_CDDCTRL_DRS_POS == 1 && GA_SUB_CDDCTRL_HOCK_POS == 2
    && GA_SUB_CDDCTRL_DM_POS == 8,
  "DTS, DRS, HOCK, DM");

/* 0xFF8038 to 0xFF804A: five status registers then five command registers */
_Static_assert(GA_SUB_REG_CDDSTAT0 == 0xFF8038 && GA_SUB_REG_CDDSTAT4 == 0xFF8040, "CDDSTAT");
_Static_assert(GA_SUB_REG_CDDCMD0 == 0xFF8042 && GA_SUB_REG_CDDCMD4 == 0xFF804A, "CDDCMD");
ASSERT_FIELD(GA_SUB_CDDCOMM_HI);
ASSERT_FIELD(GA_SUB_CDDCOMM_LO);
_Static_assert(GA_SUB_CDDCOMM_HI_MASK == 0x0F00 && GA_SUB_CDDCOMM_LO_MASK == 0x000F, "two nibbles");

/* Font */
_Static_assert(GA_SUB_REG_FONTCOLOR == 0xFF804D, "font color is the low byte");
ASSERT_BYTE_FIELD(GA_SUB_FONTCOLOR_0);
ASSERT_BYTE_FIELD(GA_SUB_FONTCOLOR_1);
_Static_assert(GA_SUB_REG_FONTBITS == 0xFF804E && GA_SUB_REG_FONTDATA == 0xFF8050, "font bits, data");

/* 0xFF8058: graphics status in the high byte, stamp size in the low byte */
_Static_assert(GA_SUB_REG_GFXSTAT == 0xFF8058, "GFXSTAT");
_Static_assert(GA_SUB_REG_STAMPSIZE == 0xFF8059, "STAMPSIZE");
ASSERT_BYTE_FLAG(GA_SUB_GRON);
ASSERT_BYTE_FLAG(GA_SUB_STAMPSIZE_REPEAT);
ASSERT_BYTE_FLAG(GA_SUB_STAMPSIZE_32x32_STAMP);
ASSERT_BYTE_FLAG(GA_SUB_STAMPSIZE_16x16_SCREEN);
_Static_assert(GA_SUB_GRON_POS == 7, "GRON");
_Static_assert(GA_SUB_STAMPSIZE_REPEAT_POS == 0 && GA_SUB_STAMPSIZE_32x32_STAMP_POS == 1
    && GA_SUB_STAMPSIZE_16x16_SCREEN_POS == 2,
  "RPT, STS, SMS");

/* Stamp map entries */
ASSERT_FIELD(STAMP_SNO);
ASSERT_FIELD(STAMP_ROTATE);
ASSERT_FLAG(STAMP_HFLIP);
_Static_assert(STAMP_SNO_MASK == 0x07FF && STAMP_ROTATE_MASK == 0x6000 && STAMP_HFLIP_MASK == 0x8000,
  "SNO, RT, HFLP");
_Static_assert(STAMP_ROTATE_0 == 0 && STAMP_ROTATE_90 == 2 && STAMP_ROTATE_180 == 1 && STAMP_ROTATE_270 == 3,
  "rotation values: RT1 is the high bit of the field");

/* Image buffer registers */
_Static_assert(GA_SUB_REG_STAMPMAPBASE == 0xFF805A, "stamp map base");
_Static_assert(GA_SUB_REG_IMGBUFVSIZE == 0xFF805D, "image buffer V cell size is the low byte");
ASSERT_BYTE_FIELD(GA_SUB_IMGBUF_VCS);
_Static_assert(GA_SUB_REG_IMGBUFSTART == 0xFF805E, "image buffer start");
_Static_assert(GA_SUB_REG_IMGBUFOFFSET == 0xFF8061, "image buffer offset is the low byte");
ASSERT_BYTE_FIELD(GA_SUB_IMGBUF_DOT);
ASSERT_BYTE_FIELD(GA_SUB_IMGBUF_LN);
_Static_assert(GA_SUB_REG_IMGBUFHDOTSIZE == 0xFF8062 && GA_SUB_REG_IMGBUFVDOTSIZE == 0xFF8064, "H, V dot size");
ASSERT_FIELD(GA_SUB_IMGBUF_HW);
ASSERT_FIELD(GA_SUB_IMGBUF_VW);
_Static_assert(GA_SUB_REG_TRACEVECTBASE == 0xFF8066, "trace vector base");

/* 0xFF8068 subcode address: SAOR at bit 7, STA1-6 at bits 1-6 */
_Static_assert(GA_SUB_REG_SUBCODEADDR == 0xFF8068, "subcode address");
ASSERT_FIELD(GA_SUB_SUBCODE_STA);
ASSERT_FLAG(GA_SUB_SUBCODE_SAOR);
_Static_assert(GA_SUB_SUBCODE_SAOR_MASK == 0x0080 && GA_SUB_SUBCODE_STA_MASK == 0x007E, "SAOR, STA");
_Static_assert(GA_SUB_REG_SUBCODEBUF == 0xFF8100 && GA_SUB_REG_SUBCODEBUFIMG == 0xFF8180, "subcode buffers");

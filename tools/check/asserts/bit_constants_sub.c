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

/* --- 0xFF800E, split like the rest (D17, HW-9) --------------------------- */

/* The two halves have opposite access -- each CPU writes its own byte and reads
 * the other's -- so a single word accessor could not express either one. The
 * manual adds a second reason: a byte or word read of the pair may return stale
 * data if the other CPU writes at the same moment, while a single-bit test
 * cannot. Splitting them makes the bit test the natural spelling. */
_Static_assert(GA_REG_COMFLAGS_MAIN == 0xFF800E, "the Main CPU's flags are the high byte");
_Static_assert(GA_REG_COMFLAGS_SUB == GA_REG_COMFLAGS_MAIN + 1, "the Sub CPU's flags are the low byte");

/* Comm command is Main -> Sub and comm status is Sub -> Main, eight words each,
 * contiguous and in that order. */
_Static_assert(GA_REG_COMCMD0 == 0xFF8010 && GA_REG_COMCMD7 == GA_REG_COMCMD0 + 14,
	"eight comm command words");
_Static_assert(GA_REG_COMSTAT0 == 0xFF8020 && GA_REG_COMSTAT7 == GA_REG_COMSTAT0 + 14,
	"eight comm status words");

/* --- registers with one unused byte (D17a, provisional) ------------------ */

/* These live at the ODD address: their fields are in the low byte and the high
 * byte is unused, so the byte that carries the data is the one the name points
 * at. Getting this backwards would make every byte write a silent no-op. */
_Static_assert(GA_REG_INT3TIMER == 0xFF8031, "the timer value is in the low byte");
_Static_assert(GA_REG_INTMASK == 0xFF8033, "the interrupt mask is in the low byte");

ASSERT_BYTE_FIELD(GA_INT1);
ASSERT_BYTE_FIELD(GA_INT2);
ASSERT_BYTE_FIELD(GA_INT3);
ASSERT_BYTE_FIELD(GA_INT4);
ASSERT_BYTE_FIELD(GA_INT5);
ASSERT_BYTE_FIELD(GA_INT6);

/* Levels 1-6 occupy bits 1-6; bit 0 is not a level. */
_Static_assert(GA_INT1_POS == 1 && GA_INT6_POS == 6, "levels map to like-numbered bits");
_Static_assert((GA_INT1_MASK | GA_INT2_MASK | GA_INT3_MASK |
                GA_INT4_MASK | GA_INT5_MASK | GA_INT6_MASK) == 0x7E,
	"the six enables are bits 1-6, and bit 0 is unused");

/* --- CDD communication block, $FF8038-$FF804A ---------------------------- */

/* Ten registers carrying twenty 4 bit values, two per register. The first five
 * are what the drive sends back and the last five are what is sent to it, which
 * is why they are named for the direction rather than numbered 0-9 (DOC-21).
 * The boundary is the thing worth pinning: CDDSTAT4 and CDDCMD0 are adjacent,
 * so an off-by-one in the split would silently read commands as status. */
_Static_assert(GA_REG_CDDSTAT0 == 0xFF8038, "the status block starts here");
_Static_assert(GA_REG_CDDSTAT4 == GA_REG_CDDSTAT0 + 8, "five status registers");
_Static_assert(GA_REG_CDDCMD0 == GA_REG_CDDSTAT4 + 2, "commands follow immediately");
_Static_assert(GA_REG_CDDCMD4 == 0xFF804A, "the command block ends here");

/* Each register holds its two nibbles at bits 11-8 and 3-0; the other two
 * nibbles read as 0. */
_Static_assert(GA_CDDCOMM_HI_MASK == 0x0F00, "even-numbered nibble is bits 11-8");
_Static_assert(GA_CDDCOMM_LO_MASK == 0x000F, "odd-numbered nibble is bits 3-0");
_Static_assert((GA_CDDCOMM_HI_MASK & GA_CDDCOMM_LO_MASK) == 0, "the nibbles do not overlap");

/* --- trace vector step encoding (KB-41) --------------------------------- */

#include <xform.h>

/* Two's complement, confirmed by tracing Sonic CD's special stage. The manual's
 * bit table draws bit 15 as a separate "+/-" cell, which reads as sign and
 * magnitude; the trace disproved it. See SPEC.md KB-41. */
_Static_assert(to_xform_delta(1.0) == 0x0800, "1.0 is the scale itself");
_Static_assert(to_xform_delta(0.5) == 0x0400, "0.5 is half the scale");
_Static_assert(to_xform_delta(-0.5) == (s16) 0xFC00, "negative is ordinary two's complement");
_Static_assert(to_xform_delta(-1.0) == (s16) 0xF800, "-1.0 is the negation of 1.0");
_Static_assert(to_xform_delta(-1.0) == -to_xform_delta(1.0), "normal arithmetic applies");
_Static_assert(to_xform_pos(1.0) == 8, "one dot is 8 units of 1/8");

/* --- 0xFF8034 CD fader, field positions (DOC-26) ------------------------- */

/* The whole 16-bit layout is accounted for: EFDT, then eleven FD bits, then
 * the two de-emphasis bits, then SSF, leaving only bit 0 unused. */
ASSERT_FIELD(GA_CDFADER_FD);
ASSERT_FIELD(GA_CDFADER_EFDT);
ASSERT_FIELD(GA_CDFADER_DEF);
ASSERT_FIELD(GA_CDFADER_SSF);
_Static_assert(GA_CDFADER_EFDT_MASK == 0x8000, "EFDT is bit 15");
_Static_assert(GA_CDFADER_FD_MASK == 0x7FF0, "FD00-10 occupy bits 4-14");
_Static_assert(GA_CDFADER_DEF_MASK == 0x000C, "DEF1,DEF0 are bits 3-2");
_Static_assert(GA_CDFADER_SSF_MASK == 0x0002, "SSF is bit 1");
_Static_assert((GA_CDFADER_EFDT_MASK | GA_CDFADER_FD_MASK | GA_CDFADER_DEF_MASK
				   | GA_CDFADER_SSF_MASK)
				   == 0xFFFE,
	"the four fields tile the register, leaving only bit 0");

/* --- 0xFF8068 subcode address, field positions (DOC-25) ------------------ */

/* Read only, low byte only: SAOR at bit 7, STA6-1 at bits 6-1, bit 0 unused. */
ASSERT_FIELD(GA_SUBCODE_STA);
ASSERT_FIELD(GA_SUBCODE_SAOR);
_Static_assert(GA_SUBCODE_SAOR_MASK == 0x0080, "SAOR is bit 7");
_Static_assert(GA_SUBCODE_STA_MASK == 0x007E, "STA1-6 occupy bits 1-6");
_Static_assert((GA_SUBCODE_SAOR_MASK | GA_SUBCODE_STA_MASK) == 0x00FE,
	"the two fields fill the low byte but for bit 0");

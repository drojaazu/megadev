/**
 * [ M E G A D E V ]
 *
 * Compile-time semantic assertions for lib/fixed.h.
 *
 * Tier 0.1 compiles each header standalone, which proves a macro PARSES but
 * says nothing about what it EVALUATES to - an unexpanded macro is never
 * checked. These assertions expand everything and assert the results, so
 * "compiles fine, computes garbage" is caught by the gate rather than on
 * hardware. See SPEC.md section 6, Tier 1.5.
 *
 * The arithmetic is asserted through the UPPERCASE macro forms, because a
 * static inline call is never an integer constant expression in C. The inline
 * functions are thin wrappers around exactly these macros, so asserting the
 * macro asserts the function too - there is no second implementation to drift.
 */

#include <fixed.h>
#include <types.h>

/* --- storage and format ------------------------------------------------- */

_Static_assert(sizeof(fix16) == 2, "fix16 is 16-bit");
_Static_assert(sizeof(fix32) == 4, "fix32 is 32-bit");
_Static_assert(sizeof(ufix16) == 2, "ufix16 is 16-bit");
_Static_assert(sizeof(ufix32) == 4, "ufix32 is 32-bit");

_Static_assert(FIX16_INT_BITS + FIX16_FRAC_BITS == 16, "fix16 is Q10.6");
_Static_assert(FIX32_INT_BITS + FIX32_FRAC_BITS == 32, "fix32 is Q16.16");

_Static_assert((fix16) -1 < 0, "fix16 must be signed");
_Static_assert((ufix16) -1 > 0, "ufix16 must be unsigned");

/* --- scale, mask, constants --------------------------------------------- */

/* SCALE is 1<<n; MASK is (1<<n)-1. The old header called the scale a "mask",
 * which is what this pair now pins down. */
_Static_assert(FIX16_SCALE == 64, "Q10.6 scale");
_Static_assert(FIX16_FRAC_MASK == 63, "Q10.6 fractional mask");
_Static_assert(FIX16_SCALE != FIX16_FRAC_MASK, "scale and mask are different things");
_Static_assert(FIX32_SCALE == 65536L, "Q16.16 scale");
_Static_assert(FIX32_FRAC_MASK == 65535L, "Q16.16 fractional mask");

_Static_assert(FIX16_ONE == FIX16(1), "FIX16_ONE agrees with FIX16(1)");
_Static_assert(FIX16_HALF == FIX16(0.5), "FIX16_HALF agrees with FIX16(0.5)");
_Static_assert(FIX32_ONE == FIX32(1), "FIX32_ONE agrees with FIX32(1)");
_Static_assert(FIX32_HALF == FIX32(0.5), "FIX32_HALF agrees with FIX32(0.5)");

/* --- literals ----------------------------------------------------------- */

_Static_assert(FIX16(1) == 64, "1.0 in Q10.6");
_Static_assert(FIX16(2) == 128, "2.0 in Q10.6");
_Static_assert(FIX16(0.25) == 16, "0.25 in Q10.6");
_Static_assert(FIX16(-1) == -64, "-1.0 in Q10.6");
_Static_assert(FIX32(1) == 65536L, "1.0 in Q16.16");
_Static_assert(FIX32(0.5) == 32768L, "0.5 in Q16.16");
_Static_assert(FIX32(-2) == -131072L, "-2.0 in Q16.16");

/* Range: Q10.6 must reach a full screen coordinate, which 8.8 could not. */
_Static_assert(FIX16(319) == 319 * 64, "Q10.6 holds a 320px x coordinate");
_Static_assert(FIX16(319) < 32767, "and does so without overflowing s16");

/* --- conversions -------------------------------------------------------- */

_Static_assert(fix16_from_int(3) == FIX16(3), "runtime int conversion matches literal");
_Static_assert(fix16_to_int(FIX16(5)) == 5, "fix16 round trip");
_Static_assert(fix32_to_int(FIX32(7)) == 7, "fix32 round trip");
_Static_assert(fix32_from_int(9) == FIX32(9), "fix32 int conversion");

_Static_assert(fix16_to_int(FIX16(2.9)) == 2, "to_int truncates");
_Static_assert(fix16_to_int_round(FIX16(2.9)) == 3, "to_int_round rounds up");
_Static_assert(fix16_to_int_round(FIX16(2.1)) == 2, "to_int_round rounds down");

_Static_assert(fix16_frac(FIX16(2.5)) == FIX16(0.5), "fractional part");

_Static_assert(fix16_to_fix32(FIX16(3)) == FIX32(3), "widening is exact");
_Static_assert(fix32_to_fix16(FIX32(3)) == FIX16(3), "narrowing round trip");

/* --- arithmetic --------------------------------------------------------- */

/* Addition needs no helper; assert that, so nobody adds one. */
_Static_assert(FIX16(2) + FIX16(3) == FIX16(5), "addition is plain integer addition");
_Static_assert(FIX32(2) - FIX32(3) == FIX32(-1), "subtraction is plain too");

_Static_assert(FIX16_MUL(FIX16(2), FIX16(3)) == FIX16(6), "2 * 3 == 6");
_Static_assert(FIX16_MUL(FIX16(0.5), FIX16(0.5)) == FIX16(0.25), "0.5 * 0.5 == 0.25");
_Static_assert(FIX16_MUL(FIX16(-2), FIX16(3)) == FIX16(-6), "signed multiply");
_Static_assert(FIX16_MUL(FIX16(1), FIX16(7)) == FIX16(7), "multiplying by 1 is identity");

_Static_assert(FIX16_DIV(FIX16(6), FIX16(3)) == FIX16(2), "6 / 3 == 2");
_Static_assert(FIX16_DIV(FIX16(1), FIX16(4)) == FIX16(0.25), "1 / 4 == 0.25");
_Static_assert(FIX16_DIV(FIX16(-6), FIX16(3)) == FIX16(-2), "signed divide");
_Static_assert(FIX16_DIV_INT(FIX16(9), 3) == FIX16(3), "divide by plain integer");

/* fix32_mul decomposes into partial products because the 68000 has no 32-bit
 * multiply. These cover each path: high*high, low*low, the cross terms, and
 * sign handling where the high half is negative but the low half is not. */
_Static_assert(FIX32_MUL(FIX32(2), FIX32(3)) == FIX32(6), "high*high");
_Static_assert(FIX32_MUL(FIX32(0.5), FIX32(0.5)) == FIX32(0.25), "low*low");
_Static_assert(FIX32_MUL(FIX32(2.5), FIX32(2)) == FIX32(5), "cross terms");
_Static_assert(FIX32_MUL(FIX32(-2), FIX32(3)) == FIX32(-6), "negative * positive");
_Static_assert(FIX32_MUL(FIX32(-0.5), FIX32(2)) == FIX32(-1),
	"negative high half with non-zero low half");
_Static_assert(FIX32_MUL(FIX32(-2), FIX32(-3)) == FIX32(6), "negative * negative");
_Static_assert(FIX32_MUL(FIX32(1), FIX32(1234)) == FIX32(1234), "identity");

_Static_assert(FIX32_DIV_INT(FIX32(10), 4) == FIX32(2.5), "fix32 divide by integer");

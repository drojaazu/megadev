/**
 * [ M E G A D E V ]
 *
 * Compile-time semantic assertions for lib/fixed.h.
 *
 * Tier 0.1 compiles each header standalone, which proves a macro PARSES but
 * says nothing about what it EVALUATES to - a macro that is never expanded is
 * never checked. These assertions expand the macros and assert the result, so
 * the whole class of "compiles fine, computes garbage" defect is caught by the
 * gate rather than on hardware. See SPEC.md section 6, Tier 1.5.
 *
 * Nothing here is executed; _Static_assert is evaluated by the compiler.
 */

#include <fixed.h>
#include <types.h>

/* --- f16 is 8.8: 8 integer bits, 8 fractional bits ---------------------- */

_Static_assert(FIX16_INT_BITS == 8, "f16 is 8.8");
_Static_assert(FIX16_FRAC_BITS == 8, "f16 is 8.8");
_Static_assert(sizeof(f16) == 2, "f16 must hold 16 bits");

/* Converting an integer to 8.8 shifts it left by 8, so the result needs the
 * full 16 bits. Casting to char discards the integer part entirely. */
_Static_assert(int_to_f16(1) == 0x0100, "int_to_f16(1) must be 1.0 in 8.8");
_Static_assert(int_to_f16(3) == 0x0300, "int_to_f16(3) must be 3.0 in 8.8");
_Static_assert(int_to_uf16(1) == 0x0100, "int_to_uf16(1) must be 1.0 in 8.8");

/* Round trip */
_Static_assert(f16_to_int(int_to_f16(5)) == 5, "int -> f16 -> int must round trip");

/* --- f32 is 16.16 ------------------------------------------------------- */

_Static_assert(FIX32_INT_BITS == 16, "f32 is 16.16");
_Static_assert(FIX32_FRAC_BITS == 16, "f32 is 16.16");
_Static_assert(sizeof(f32) == 4, "f32 must hold 32 bits");

_Static_assert(int_to_f32(1) == 0x00010000L, "int_to_f32(1) must be 1.0 in 16.16");
_Static_assert(int_to_f32(5) == 0x00050000L, "int_to_f32(5) must be 5.0 in 16.16");
_Static_assert(int_to_uf32(1) == 0x00010000UL, "int_to_uf32(1) must be 1.0 in 16.16");

_Static_assert(f32_to_int(int_to_f32(7)) == 7, "int -> f32 -> int must round trip");

/* --- scale factors ------------------------------------------------------ */

/* NOTE: these are named *_FRAC_MASK but hold a SCALE factor (1 << frac_bits),
 * not a mask (which would be (1 << frac_bits) - 1). The value is correct for
 * how frac_to_* uses it; the name is misleading. See BACKLOG ARCH-11. */
_Static_assert(FIX16_FRAC_MASK == 256, "8.8 scale factor");
_Static_assert(FIX32_FRAC_MASK == 65536L, "16.16 scale factor");

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file fixed.h
 * @brief Fixed-point fractional numbers
 *
 * @details
 * The 68000 has no floating-point unit, so fractional values are held as
 * integers scaled by a power of two. A value in Qm.n format uses m bits for
 * the integer part and n bits for the fraction.
 *
 * | Type   | Storage | Format | Range           | Step    |
 * |--------|---------|--------|-----------------|---------|
 * | fix16  | s16     | Q10.6  | -512 .. 511.98  | 1/64    |
 * | ufix16 | u16     | Q10.6  | 0 .. 1023.98    | 1/64    |
 * | fix32  | s32     | Q16.16 | -32768 .. 32767 | 1/65536 |
 * | ufix32 | u32     | Q16.16 | 0 .. 65535.99   | 1/65536 |
 *
 * @note **fix16 is the type to reach for by default.** The 68000 has a
 * 16x16->32 multiply (MULS.W) and a 32/16->16 divide (DIVS.W) but no 32-bit
 * multiply, so fix16 arithmetic is a couple of instructions while fix32
 * multiplication needs partial products. Keep fix32 for accumulators and for
 * values that genuinely exceed +/-512.
 *
 * @note Q10.6 spans +/-512, which covers a 320x224 screen with room to spare.
 * The 8.8 format used previously reached only +/-128 and so could not hold a
 * horizontal screen coordinate at all.
 *
 * Addition and subtraction of two values **in the same format** are plain
 * integer operations - no helper is needed:
 *
 *     fix16 sum = a + b;
 *
 * Multiplication and division are not, since the scale factor would be
 * squared or cancelled. Use the helpers below.
 *
 * @warning FIX16() and FIX32() accept a floating-point expression, but only as
 * a **compile-time constant**, which the compiler folds to an integer. There
 * is no FPU and no soft-float library linked, so a runtime float will not
 * link.
 */

#ifndef MEGADEV__FIXED_H
#define MEGADEV__FIXED_H

#include "types.h"

/**
 * @typedef fix16
 * @brief Signed Q10.6 fixed-point value
 */
typedef s16 fix16;

/**
 * @typedef ufix16
 * @brief Unsigned Q10.6 fixed-point value
 */
typedef u16 ufix16;

/**
 * @typedef fix32
 * @brief Signed Q16.16 fixed-point value
 */
typedef s32 fix32;

/**
 * @typedef ufix32
 * @brief Unsigned Q16.16 fixed-point value
 */
typedef u32 ufix32;

/* ------------------------------------------------------------------ fix16 */

/** @brief Integer bits in a fix16 */
#define FIX16_INT_BITS 10
/** @brief Fractional bits in a fix16 */
#define FIX16_FRAC_BITS 6
/** @brief Scale factor; also the integer representation of 1.0 */
#define FIX16_SCALE (1 << FIX16_FRAC_BITS)
/** @brief Mask selecting the fractional bits */
#define FIX16_FRAC_MASK (FIX16_SCALE - 1)
/** @brief 1.0 as a fix16 */
#define FIX16_ONE FIX16_SCALE
/** @brief 0.5 as a fix16 */
#define FIX16_HALF (FIX16_SCALE >> 1)

/**
 * @def FIX16
 * @brief Convert a compile-time constant to fix16
 * @details Takes integers and fractions alike: FIX16(3), FIX16(0.25).
 * @warning Compile-time constants only; see the file-level warning.
 */
#define FIX16(value) ((fix16) ((value) *FIX16_SCALE))

/** @brief Convert a compile-time constant to ufix16 */
#define UFIX16(value) ((ufix16) ((value) *FIX16_SCALE))

/** @brief Convert a runtime integer to fix16 */
#define fix16_from_int(value) ((fix16) ((value) << FIX16_FRAC_BITS))

/** @brief Truncate a fix16 towards zero, giving an integer */
#define fix16_to_int(value) ((s16) ((value) >> FIX16_FRAC_BITS))

/** @brief Round a fix16 to the nearest integer */
#define fix16_to_int_round(value) ((s16) (((value) + FIX16_HALF) >> FIX16_FRAC_BITS))

/** @brief Truncate a ufix16 towards zero, giving an unsigned integer */
#define ufix16_to_int(value) ((u16) ((value) >> FIX16_FRAC_BITS))

/** @brief Convert a runtime unsigned integer to ufix16 */
#define ufix16_from_int(value) ((ufix16) ((value) << FIX16_FRAC_BITS))

/** @brief Fractional part of a fix16, still as a fix16 */
#define fix16_frac(value) ((fix16) ((value) &FIX16_FRAC_MASK))

/*
 * Each operation is defined once as a macro and once as an inline function
 * wrapping that macro. The macro is a constant expression, so it can be used
 * in initialisers and checked by _Static_assert; the function evaluates its
 * arguments exactly once and is type checked. Because the function is a thin
 * wrapper, the two forms cannot drift apart.
 *
 * @warning The UPPERCASE macro forms evaluate their arguments more than once.
 * Prefer the lowercase functions unless you need a constant expression.
 */

/** @brief Multiply two fix16 values (constant-expression form) */
#define FIX16_MUL(a, b) ((fix16) (((s32) (a) * (s32) (b)) >> FIX16_FRAC_BITS))

/** @brief Divide two fix16 values (constant-expression form) */
#define FIX16_DIV(a, b) ((fix16) ((((s32) (a)) << FIX16_FRAC_BITS) / (s32) (b)))

/** @brief Divide a fix16 by a plain integer (constant-expression form) */
#define FIX16_DIV_INT(a, b) ((fix16) ((a) / (b)))

/**
 * @brief Multiply two fix16 values
 * @details The product of two Qm.n values is Qm.2n, so it is shifted back down
 * by n. The intermediate is 32-bit: one MULS.W on the 68000.
 */
static inline fix16 fix16_mul(fix16 a, fix16 b) { return FIX16_MUL(a, b); }

/**
 * @brief Divide two fix16 values
 * @details The quotient of two Qm.n values is Q0, so the dividend is scaled up
 * by n first. The intermediate is 32-bit: one DIVS.W.
 * @warning Dividing by zero traps, as with any 68000 divide.
 */
static inline fix16 fix16_div(fix16 a, fix16 b) { return FIX16_DIV(a, b); }

/** @brief Divide a fix16 by a plain integer */
static inline fix16 fix16_div_int(fix16 a, s16 b) { return FIX16_DIV_INT(a, b); }

/* ------------------------------------------------------------------ fix32 */

/** @brief Integer bits in a fix32 */
#define FIX32_INT_BITS 16
/** @brief Fractional bits in a fix32 */
#define FIX32_FRAC_BITS 16
/** @brief Scale factor; also the integer representation of 1.0 */
#define FIX32_SCALE (1L << FIX32_FRAC_BITS)
/** @brief Mask selecting the fractional bits */
#define FIX32_FRAC_MASK (FIX32_SCALE - 1)
/** @brief 1.0 as a fix32 */
#define FIX32_ONE FIX32_SCALE
/** @brief 0.5 as a fix32 */
#define FIX32_HALF (FIX32_SCALE >> 1)

/**
 * @def FIX32
 * @brief Convert a compile-time constant to fix32
 * @warning Compile-time constants only; see the file-level warning.
 */
#define FIX32(value) ((fix32) ((value) *FIX32_SCALE))

/** @brief Convert a compile-time constant to ufix32 */
#define UFIX32(value) ((ufix32) ((value) *FIX32_SCALE))

/** @brief Convert a runtime integer to fix32 */
#define fix32_from_int(value) ((fix32) (((s32) (value)) << FIX32_FRAC_BITS))

/** @brief Truncate a fix32 towards zero, giving an integer */
#define fix32_to_int(value) ((s16) ((value) >> FIX32_FRAC_BITS))

/** @brief Round a fix32 to the nearest integer */
#define fix32_to_int_round(value) ((s16) (((value) + FIX32_HALF) >> FIX32_FRAC_BITS))

/** @brief Truncate a ufix32 towards zero, giving an unsigned integer */
#define ufix32_to_int(value) ((u16) ((value) >> FIX32_FRAC_BITS))

/** @brief Convert a runtime unsigned integer to ufix32 */
#define ufix32_from_int(value) ((ufix32) (((u32) (value)) << FIX32_FRAC_BITS))

/** @brief Fractional part of a fix32, still as a fix32 */
#define fix32_frac(value) ((fix32) ((value) &FIX32_FRAC_MASK))

/** @brief Widen a fix16 to a fix32; always exact */
#define fix16_to_fix32(value) ((fix32) (((s32) (value)) << (FIX32_FRAC_BITS - FIX16_FRAC_BITS)))

/** @brief Narrow a fix32 to a fix16, losing precision and range */
#define fix32_to_fix16(value) ((fix16) ((value) >> (FIX32_FRAC_BITS - FIX16_FRAC_BITS)))

/**
 * @brief Multiply two fix32 values
 *
 * @details
 * A Q16.16 product needs a 64-bit intermediate and the 68000 has no 32-bit
 * multiply, so each operand is split into a signed high half and an unsigned
 * low half and the partial products summed:
 *
 *     a*b = ah*bh << 32  +  (ah*bl + bh*al) << 16  +  al*bl
 *
 * then shifted right by 16 to return to Q16.16. This compiles to MULS.W/MULU.W
 * rather than pulling in a 64-bit helper routine, which matters because the
 * build links no support library.
 *
 * @note Several times the cost of fix16_mul. Prefer fix16 where range allows.
 */
#define FIX32_MUL(a, b)                                                        \
	((fix32) ((((s32) (s16) ((a) >> 16) * (s32) (s16) ((b) >> 16)) << 16)        \
		+ ((s32) (s16) ((a) >> 16) * (s32) (u16) ((b) &0xFFFF))                    \
		+ ((s32) (s16) ((b) >> 16) * (s32) (u16) ((a) &0xFFFF))                    \
		+ (s32) (((u32) (u16) ((a) &0xFFFF) * (u32) (u16) ((b) &0xFFFF)) >> 16)))

static inline fix32 fix32_mul(fix32 a, fix32 b) { return FIX32_MUL(a, b); }

/**
 * @brief Divide a fix32 by a plain integer
 * @details Only the dividend carries a scale factor, so the result stays in
 * Q16.16 and this is a single DIVS.W.
 * @note Full Q16.16 / Q16.16 division needs a 48-bit intermediate and is not
 * provided; see BACKLOG LIB-18.
 */
#define FIX32_DIV_INT(a, b) ((fix32) ((a) / (s32) (b)))

static inline fix32 fix32_div_int(fix32 a, s16 b) { return FIX32_DIV_INT(a, b); }

#endif

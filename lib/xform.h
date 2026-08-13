/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file xform.h
 * @brief Trace vector value conversion for the rotation/scaling hardware
 *
 * @details
 * The graphics operation hardware reads a trace vector table of four words per
 * output line: a start position and a per-pixel step, each as X and Y. The two
 * pairs use different fixed point formats, and the step format is **not** two's
 * complement. These helpers convert ordinary values into both.
 *
 * See docs/rotation_scaling.md for what the table means and how to build one.
 */

#pragma once

#include <types.h>

/**
 * @typedef xform_pos
 * @brief A trace vector start coordinate: 13 integer bits, 3 fractional
 * @details Unsigned. Covers 0 to 8191 with 1/8 dot precision. Sampling outside
 * the stamp map either wraps or reads 0, depending on GA_STAMPSIZE_REPEAT_MASK.
 */
typedef u16 xform_pos;

/**
 * @typedef xform_delta
 * @brief A trace vector step: sign bit, 4 integer bits, 11 fractional
 *
 * @warning **Sign and magnitude, not two's complement.** Bit 15 is the sign and
 * bits 14-0 are an unsigned magnitude. A negated `xform_delta` is not the
 * negation of the word, and casting a negative C integer into one produces a
 * value the hardware reads as a large step of the opposite sense. Use
 * to_xform_delta() or xform_delta_negate() rather than arithmetic.
 */
typedef u16 xform_delta;

#define XFORM_POS_INT_BITS 13
#define XFORM_POS_FRAC_BITS (16 - XFORM_POS_INT_BITS)
#define XFORM_POS_SCALE (1 << XFORM_POS_FRAC_BITS)

/**
 * @def to_xform_pos
 * @brief Convert a dot coordinate into a trace vector start position
 */
#define to_xform_pos(value) ((xform_pos) ((value) * (XFORM_POS_SCALE)))

/** @brief Magnitude bits of a step; bit 15 is the sign */
#define XFORM_DELTA_INT_BITS 4
#define XFORM_DELTA_FRAC_BITS 11
#define XFORM_DELTA_SCALE (1 << XFORM_DELTA_FRAC_BITS)
#define XFORM_DELTA_SIGN 0x8000u
#define XFORM_DELTA_MAG_MASK 0x7FFFu

/**
 * @def XFORM_DELTA_MAX
 * @brief Largest step magnitude the format can express, as a whole number
 * @details The magnitude field is 4 integer bits, so a step must be under 16
 * source dots per output pixel. This is the ceiling on how far the source can
 * be shrunk, and is the limit a pseudo-3D horizon runs into first.
 */
#define XFORM_DELTA_MAX 16

/**
 * @def to_xform_delta
 * @brief Convert a step in source dots per output pixel into a trace vector step
 * @details Handles the sign correctly. Suitable for compile time constants;
 * xform_delta_make() is the runtime form.
 * @warning Magnitudes of XFORM_DELTA_MAX or more cannot be represented and will
 * corrupt the sign bit.
 */
#define to_xform_delta(value)                                                  \
	((xform_delta) ((value) < 0                                                  \
			? (XFORM_DELTA_SIGN | (u16) ((-(value)) * XFORM_DELTA_SCALE))            \
			: (u16) ((value) *XFORM_DELTA_SCALE)))

/**
 * @fn xform_delta_make
 * @brief Build a trace vector step from a signed fixed point value at runtime
 * @param value Step scaled by XFORM_DELTA_SCALE, as an ordinary signed integer
 * @return The same step in the hardware's sign and magnitude format
 * @details This is the conversion to use when the step has been *calculated*
 * rather than written as a literal -- which is every per line value in a
 * perspective effect. Ordinary signed arithmetic produces two's complement; the
 * hardware does not read it that way.
 */
static inline xform_delta xform_delta_make (s16 value)
{
	return (value < 0) ? (xform_delta) (XFORM_DELTA_SIGN | (u16) (-value))
										 : (xform_delta) value;
}

/**
 * @fn xform_delta_negate
 * @brief Reverse the direction of a step
 * @details Toggles the sign bit. Negating the word would change the magnitude
 * as well, which is the trap this exists to avoid.
 */
static inline xform_delta xform_delta_negate (xform_delta value)
{
	return value ^ XFORM_DELTA_SIGN;
}

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
 * @details Ordinary signed two's complement, so normal C arithmetic on it is
 * correct. The manual's bit table draws bit 15 as a separate `+/-` cell, which
 * reads as sign-and-magnitude; that reading is **wrong**, and was disproved by
 * tracing Sonic CD (SPEC.md KB-41). Do not "fix" this back.
 */
typedef s16 xform_delta;

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
 * @warning Magnitudes of XFORM_DELTA_MAX or more overflow into the sign bit.
 * A perspective ground plane approaches this ceiling near the horizon, so clamp
 * the first drawn row rather than letting it wrap.
 */
#define to_xform_delta(value) ((xform_delta) ((value) * XFORM_DELTA_SCALE))


/**
 * @fn xform_delta_negate
 * @brief Reverse the direction of a step
 * @details Plain negation. Retained only so call sites read symmetrically with
 * to_xform_delta().
 */
static inline xform_delta xform_delta_negate (xform_delta value)
{
	return (xform_delta) -value;
}

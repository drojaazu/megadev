/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file trace.h
 * @brief Trace vector value conversion for the rotation/scaling hardware
 *
 * @details
 * The graphics operation hardware reads a trace vector table of four words per
 * output line: a start position and a per-pixel step, each as X and Y. The two
 * pairs use different fixed point formats; the step is signed two's complement.
 * These helpers convert ordinary values into both.
 *
 * See docs/rotation_scaling.md for what the table means and how to build one.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

/**
 * @typedef trace_pos
 * @brief A trace vector start coordinate: 13 integer bits, 3 fractional
 * @details Unsigned. Covers 0 to 8191 with 1/8 dot precision. Sampling outside
 * the stamp map either wraps or reads 0, depending on GA_SUB_STAMPSIZE_REPEAT_MASK.
 */
typedef uint16_t trace_pos;

/**
 * @typedef trace_delta
 * @brief A trace vector step: sign bit, 4 integer bits, 11 fractional
 *
 * @details Ordinary signed two's complement, so normal C arithmetic on it is
 * correct.
 */
typedef int16_t trace_delta;

#define TRACE_POS_INT_BITS 13 ///< Integer bits of a trace vector start position
#define TRACE_POS_FRAC_BITS                                                  \
  (16 -                                                                      \
   TRACE_POS_INT_BITS) ///< Fractional bits of a trace vector start position
#define TRACE_POS_SCALE                                        \
  (1 << TRACE_POS_FRAC_BITS) ///< One dot, as a start position

/**
 * @def to_trace_pos
 * @brief Convert a dot coordinate into a trace vector start position
 */
#define to_trace_pos(value) ((trace_pos) ((value) * (TRACE_POS_SCALE)))

/** @brief Magnitude bits of a step; bit 15 is the sign */
#define TRACE_DELTA_INT_BITS  4
#define TRACE_DELTA_FRAC_BITS 11 ///< Fractional bits of a trace vector step
#define TRACE_DELTA_SCALE (1 << TRACE_DELTA_FRAC_BITS) ///< One dot, as a step

/**
 * @def TRACE_DELTA_MAX
 * @brief Largest step magnitude the format can express, as a whole number
 * @details The magnitude field is 4 integer bits, so a step must be under 16
 * source dots per output pixel. This is the ceiling on how far the source can
 * be shrunk, and is the limit a pseudo-3D horizon runs into first.
 */
#define TRACE_DELTA_MAX 16

/**
 * @def to_trace_delta
 * @brief Convert a step in source dots per output pixel into a trace vector step
 * @warning Magnitudes of TRACE_DELTA_MAX or more overflow into the sign bit.
 * A perspective ground plane approaches this ceiling near the horizon, so clamp
 * the first drawn row rather than letting it wrap.
 */
#define to_trace_delta(value) ((trace_delta) ((value) * TRACE_DELTA_SCALE))

/**
 * @brief Reverse the direction of a step
 * @details Plain negation. Retained only so call sites read symmetrically with
 * to_trace_delta().
 */
static inline trace_delta trace_delta_negate(trace_delta value)
{
  return (trace_delta) -value;
}

#ifdef __cplusplus
}
#endif

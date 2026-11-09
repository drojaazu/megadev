/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file fixed.h
 * @brief Fixed-point fractional numbers
 *
 * @details
 * Two formats, in Q notation: Q8.8 in 16 bits and Q16.16 in 32 bits, each
 * signed or unsigned. The `_ONE` constant of a format is the value 1.0, so a
 * fraction scales by it and the integer part is the value shifted right by
 * `_FRAC_BITS`. The product of two values has twice the fractional bits and
 * must be shifted back down by `_FRAC_BITS`.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

typedef int16_t  f16;  ///< Signed Q8.8 fixed point
typedef int32_t  f32;  ///< Signed Q16.16 fixed point
typedef uint16_t uf16; ///< Unsigned Q8.8 fixed point
typedef uint32_t uf32; ///< Unsigned Q16.16 fixed point

/** @brief Integer bits of f16 and uf16 */
#define FIX16_INT_BITS 8
/** @brief Fractional bits of f16 and uf16 */
#define FIX16_FRAC_BITS (16 - FIX16_INT_BITS)
/** @brief 1.0 as f16 or uf16 */
#define FIX16_ONE (1 << FIX16_FRAC_BITS)

/** @brief Integer bits of f32 and uf32 */
#define FIX32_INT_BITS 16
/** @brief Fractional bits of f32 and uf32 */
#define FIX32_FRAC_BITS (32 - FIX32_INT_BITS)
/** @brief 1.0 as f32 or uf32 */
#define FIX32_ONE (1L << FIX32_FRAC_BITS)

/** @brief A fraction (such as 0.5) as f16 */
#define frac_to_f16(value) ((f16) ((value) * FIX16_ONE))
/** @brief An integer as f16 */
#define int_to_f16(value) ((f16) ((value) * FIX16_ONE))
/** @brief Integer part of an f16 */
#define f16_to_int(value) ((int8_t) ((value) >> FIX16_FRAC_BITS))

/** @brief A fraction as uf16 */
#define frac_to_uf16(value) ((uf16) ((value) * FIX16_ONE))
/** @brief An integer as uf16 */
#define int_to_uf16(value) ((uf16) ((value) * FIX16_ONE))
/** @brief Integer part of a uf16 */
#define uf16_to_int(value) ((uint8_t) ((value) >> FIX16_FRAC_BITS))

/** @brief A fraction as f32 */
#define frac_to_f32(value) ((f32) ((value) * FIX32_ONE))
/** @brief An integer as f32 */
#define int_to_f32(value) ((f32) ((value) * FIX32_ONE))
/** @brief Integer part of an f32 */
#define f32_to_int(value) ((int16_t) ((value) >> FIX32_FRAC_BITS))

/** @brief A fraction as uf32 */
#define frac_to_uf32(value) ((uf32) ((value) * FIX32_ONE))
/** @brief An integer as uf32 */
#define int_to_uf32(value) ((uf32) ((value) * FIX32_ONE))
/** @brief Integer part of a uf32 */
#define uf32_to_int(value) ((uint16_t) ((value) >> FIX32_FRAC_BITS))

#ifdef __cplusplus
}
#endif

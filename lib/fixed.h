/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file fixed.h
 * @brief Fixed-point fractional numbers
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef short f16;          ///< Signed 8.8 fixed point
typedef long f32;           ///< Signed 16.16 fixed point

typedef unsigned short uf16; ///< Unsigned 8.8 fixed point
typedef unsigned long uf32;  ///< Unsigned 16.16 fixed point

#define FIX16_INT_BITS 8 ///< Integer bits of f16 and uf16
#define FIX16_FRAC_BITS (16 - FIX16_INT_BITS) ///< Fractional bits of f16 and uf16
#define FIX16_FRAC_MASK (1 << FIX16_FRAC_BITS) ///< One, as f16 (1 << FIX16_FRAC_BITS)

#define FIX32_INT_BITS 16 ///< Integer bits of f32 and uf32
#define FIX32_FRAC_BITS (32 - FIX32_INT_BITS) ///< Fractional bits of f32 and uf32
#define FIX32_FRAC_MASK (1 << FIX32_FRAC_BITS) ///< One, as f32 (1 << FIX32_FRAC_BITS)

#define frac_to_f16(value) ((f16) ((value) * (FIX16_FRAC_MASK))) ///< A fraction (such as 0.5) as f16
#define int_to_f16(value) ((f16) ((value) * (1 << (FIX16_FRAC_BITS)))) ///< An integer as f16
#define f16_to_int(value) ((char) ((value) >> (FIX16_FRAC_BITS))) ///< Integer part of an f16

#define frac_to_uf16(value) ((uf16) ((value) * (FIX16_FRAC_MASK))) ///< A fraction as uf16
#define int_to_uf16(value) ((uf16) ((value) * (1 << (FIX16_FRAC_BITS)))) ///< An integer as uf16
#define uf16_to_int(value) ((unsigned char) ((value) >> (FIX16_FRAC_BITS))) ///< Integer part of a uf16

#define frac_to_f32(value) ((f32) ((value) * (FIX32_FRAC_MASK))) ///< A fraction as f32
#define int_to_f32(value) ((f32) ((value) * (1L << (FIX32_FRAC_BITS)))) ///< An integer as f32
#define f32_to_int(value) ((short) ((value) >> (FIX32_FRAC_BITS))) ///< Integer part of an f32

#define frac_to_uf32(value) ((uf32) ((value) * (FIX32_FRAC_MASK))) ///< A fraction as uf32
#define int_to_uf32(value) ((uf32) ((value) * (1UL << (FIX32_FRAC_BITS)))) ///< An integer as uf32
#define uf32_to_int(value) ((unsigned short) ((value) >> (FIX32_FRAC_BITS))) ///< Integer part of a uf32

#ifdef __cplusplus
}
#endif

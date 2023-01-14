/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file fpoint.h
 * @brief Fixed-point numbers
 */

#ifndef MEGADEV__FPOINT_H
#define MEGADEV__FPOINT_H

#include "stdint.h"

typedef int16_t f16;
typedef int32_t f32;

typedef uint16_t uf16;
typedef uint32_t uf32;

#define FIX16_INT_BITS 8
#define FIX16_FRAC_BITS (16 - FIX16_INT_BITS)
#define FIX16_FRAC_MASK (1 << FIX16_FRAC_BITS)

#define FIX32_INT_BITS 16
#define FIX32_FRAC_BITS (32 - FIX32_INT_BITS)
#define FIX32_FRAC_MASK (1 << FIX32_FRAC_BITS)

#define frac_to_f16(value) ((f16) ((value) * (FIX16_FRAC_MASK)))
#define int_to_f16(value) ((f16) ((value) << (FIX16_FRAC_BITS)))
#define f16_to_int(value) ((s16) ((value) >> (FIX16_FRAC_BITS)))

#define frac_to_uf16(value) ((uf16) ((value) * (FIX16_FRAC_MASK)))
#define int_to_uf16(value) ((uf16) ((value) << (FIX16_FRAC_BITS)))
#define uf16_to_int(value) ((u8) ((value) >> (FIX16_FRAC_BITS)))

#define frac_to_f32(value) ((f32) ((value) * (FIX32_FRAC_MASK)))
#define int_to_f32(value) ((f32) ((value) << (FIX32_FRAC_BITS)))
#define f32_to_int(value) ((s32) ((value) >> (FIX32_FRAC_BITS)))

#define frac_to_uf32(value) ((u32) ((value) * (FIX32_FRAC_MASK)))
#define int_to_uf32(value) ((u32) ((value) << (FIX32_FRAC_BITS)))
#define uf32_to_int(value) ((u16) ((value) >> (FIX32_FRAC_BITS)))

#endif

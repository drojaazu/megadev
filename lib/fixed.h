/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file fixed.h
 * @brief Fixed-point numbers
 */

#ifndef MEGADEV__FIXED_H
#define MEGADEV__FIXED_H

typedef short f16;
typedef long f32;

typedef unsigned short uf16;
typedef unsigned long uf32;

#define FIX16_INT_BITS 8
#define FIX16_FRAC_BITS (16 - FIX16_INT_BITS)
#define FIX16_FRAC_MASK (1 << FIX16_FRAC_BITS)

#define FIX32_INT_BITS 16
#define FIX32_FRAC_BITS (32 - FIX32_INT_BITS)
#define FIX32_FRAC_MASK (1 << FIX32_FRAC_BITS)

#define frac_to_f16(value) ((f16) ((value) * (FIX16_FRAC_MASK)))
#define int_to_f16(value) ((char) ((value) << (FIX16_FRAC_BITS)))
#define f16_to_int(value) ((char) ((value) >> (FIX16_FRAC_BITS)))

#define frac_to_uf16(value) ((uf16) ((value) * (FIX16_FRAC_MASK)))
#define int_to_uf16(value) ((unsigned char) ((value) << (FIX16_FRAC_BITS)))
#define uf16_to_int(value) ((unsigned char) ((value) >> (FIX16_FRAC_BITS)))

#define frac_to_f32(value) ((f32) ((value) * (FIX32_FRAC_MASK)))
#define int_to_f32(value) ((short) ((value) << (FIX32_FRAC_BITS)))
#define f32_to_int(value) ((short) ((value) >> (FIX32_FRAC_BITS)))

#define frac_to_uf32(value) ((uf32) ((value) * (FIX32_FRAC_MASK)))
#define int_to_uf32(value) ((unsigned short) ((value) << (FIX32_FRAC_BITS)))
#define uf32_to_int(value) ((unsigned short) ((value) >> (FIX32_FRAC_BITS)))

#endif

/**
 * \file math.h
 * \brief Numerical utilities
 */

#ifndef MEGADEV__MATH_H
#define MEGADEV__MATH_H

#include "types.h"

/*
#define FIX32_INT_BITS 22
#define FIX32_FRAC_BITS (32 - FIX32_INT_BITS)

#define FIX32_INT_MASK (((1 << FIX32_INT_BITS) - 1) << FIX32_FRAC_BITS)
#define FIX32_FRAC_MASK ((1 << FIX32_FRAC_BITS) - 1)

#define FIX16_INT_BITS 10
#define FIX16_FRAC_BITS (16 - FIX16_INT_BITS)

#define FIX16_INT_MASK (((1 << FIX16_INT_BITS) - 1) << FIX16_FRAC_BITS)
#define FIX16_FRAC_MASK ((1 << FIX16_FRAC_BITS) - 1)
*/

static inline u32 mulu(u16 multiplier, u16 multiplicand) {
  u32 product = multiplier;
  asm("mulu.w %1, %0" : "+d"(product) : "d"(multiplicand) : "cc");
  return product;
}

static inline s32 muls(u16 multiplier, u16 multiplicand) {
  u32 product = multiplier;
  asm("muls.w %1, %0" : "+d"(product) : "d"(multiplicand) : "cc");
  return product;
}

static inline u16 divu(u32 dividend, u16 divisor) {
  u32 quotient = dividend;
  asm("divu.w %1, %0" : "+d"(quotient) : "d"(divisor) : "cc");
  return quotient;
}

static inline s16 divs(s32 dividend, s16 divisor) {
  s32 quotient = dividend;
  asm("divs.w %1, %0" : "+d"(quotient) : "d"(divisor) : "cc");
  return quotient;
}

static inline u16 modu(u32 dividend, u16 divisor) {
  u32 remainder = dividend;
  asm(R"(divu.w %1, %0
      swap %0)"
      : "+d"(remainder)
      : "d"(divisor)
      : "cc");
  return remainder;
}

static inline s16 mods(s32 dividend, s16 divisor) {
  s32 remainder = dividend;
  asm(R"(divs.w %1, %0
      swap %0)"
      : "+d"(remainder)
      : "d"(divisor)
      : "cc");
  return remainder;
}

static inline u16 bcd(u16 value) {
  u16 temp;
  asm(R"(
		ext.l	%0
		divu.w #0xa, %0
		move.b %0, %1
		lsl.b	#4, %1
		swap %0
		move #0, ccr
		abcd %1, %0)"
      : "+d"(value)
      : "d"(temp)
      : "d1", "cc");
  return value;
}

#endif

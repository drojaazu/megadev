/**
 * \file
 * \brief Math functions
 */

#ifndef MEGADEV__MATH_H
#define MEGADEV__MATH_H

#include "types.h"

/*
  CPU opcode wrappers
*/

/**
 * @brief Performs unsigned multiplication using MULU
 */
static inline u32 mulu(u16 multiplicand, u16 multiplier) {
  u32 product = multiplier;
  asm("mulu.w %1, %0" : "+d"(product) : "d"(multiplicand) : "cc");
  return product;
}

/**
 * @brief Performs signed multiplication using MULS
 */
static inline s32 muls(s16 multiplicand, s16 multiplier) {
  s32 product = multiplier;
  asm("muls.w %1, %0" : "+d"(product) : "d"(multiplicand) : "cc");
  return product;
}

/**
 * @brief Performs unsigned division using DIVU
 */
static inline u16 divu(u32 dividend, u16 divisor) {
  u32 quotient = dividend;
  asm("divu.w %1, %0" : "+d"(quotient) : "d"(divisor) : "cc");
  return quotient;
}

/**
 * @brief Performs signed division using DIVS
 */
static inline s16 divs(s32 dividend, s16 divisor) {
  s32 quotient = dividend;
  asm("divs.w %1, %0" : "+d"(quotient) : "d"(divisor) : "cc");
  return quotient;
}

/**
 * @brief Performs unsigned division using DIVU and returns the remainder
 */
static inline u16 modu(u32 dividend, u16 divisor) {
  u32 modulus = dividend;
  asm(R"(
      divu.w %1, %0
      swap %0
      )"
      : "+d"(modulus)
      : "d"(divisor)
      : "cc");
  return (u16)modulus;
}

/**
 * @brief Performs signed division using DIVS and returns the remainder
 */
static inline s16 mods(s32 dividend, s16 divisor) {
  s32 modulus = dividend;
  asm(R"(
      divs.w %1, %0
      swap %0
      )"
      : "+d"(modulus)
      : "d"(divisor)
      : "cc");
  return (s16)modulus;
}

/**
 * @brief Performs unsigned division using DIVU and returns the remainder and
 * quotient in one 32 bit value
 *
 * @return u32: Quotient in lower word, remainder in upper word
 */
static inline u32 divu_full(u32 dividend, u16 divisor) {
  u32 remainder_quotient = dividend;
  asm("divu.w %1, %0" : "+d"(remainder_quotient) : "d"(divisor) : "cc");
  return remainder_quotient;
}

/**
 * @brief Performs signed division using DIVS and returns the remainder and
 * quotient in one 32 bit value
 *
 * @return u32: Quotient in lower word, remainder in upper word
 * @note Returns unsigned 32bit value; upper and lower words will need to be
 * reinterpreted as signed
 */
static inline u32 divs_full(s32 dividend, s16 divisor) {
  u32 remainder_quotient = dividend;
  asm("divs.w %1, %0" : "+d"(remainder_quotient) : "d"(divisor) : "cc");
  return remainder_quotient;
}

/**
 * @brief Performs unsigned division using DIVU and rounds the quotient based on
 * the remainder
 */
static inline u16 divu_round(u32 dividend, u16 divisor) {
  u32 quotient = dividend;
  u32 dummy1 = 0;
  u32 dummy2 = 0;
  asm(R"(
      mov.l %1, %3
      divu.w %1, %0
      mov.l %0, %2
      swap %2
      and #0xffff, %2
      lsl.l #1, %2
      cmp.w %3, %2
      blt 1f
      addq #1, %0
    1:)"
      : "+d"(quotient)
      : "d"(divisor), "d"(dummy1), "d"(dummy2)
      : "cc");
  return quotient;
}

/**
 * @brief Performs signed division using DIVS and rounds the quotient based on
 * the remainder
 */
static inline s16 divs_round(s32 dividend, s16 divisor) {
  s32 quotient = dividend;
  s32 dummy1 = 0;
  s32 dummy2 = 0;
  asm(R"(
      mov.l %1, %3
      divs.w %1, %0
      mov.l %0, %2
      swap %2
      and #0xffff, %2
      lsl.l #1, %2
      cmp.w %3, %2
      blt 1f
      addq #1, %0
    1:)"
      : "+d"(quotient)
      : "d"(divisor), "d"(dummy1), "d"(dummy2)
      : "cc");
  return quotient;
}

/*
  gcc operator functions
*/

s32 __mulsi3(s16 multiplier, s16 multiplicand) {
  return muls(multiplier, multiplicand);
}

s16 __divsi3(s32 dividend, s16 divisor) { return divs(dividend, divisor); }

u16 __udivsi3(u32 dividend, u16 divisor) { return divu(dividend, divisor); }

s16 __modsi3(s32 dividend, s16 divisor) { return mods(dividend, divisor); }

u16 __umodsi3(u32 dividend, u16 divisor) { return modu(dividend, divisor); }

// TODO: test making this s32 and removing the size from the asm and see if
// casting it will make it choose the correct size
static inline s16 abs(s16 val) {
  asm(R"(
      tst.w %0
      bpl 1f
      neg.w %0
      1:)"
      : "+d"(val)
      :
      : "cc");
  return val;
}

static inline bool is_neg(s16 val) {
  bool out = false;
  asm(R"(
      tst.w %1
      bpl 1f
      mov.b #1, %0
      1:)"
      : "+d"(out)
      : "d"(val)
      : "cc");
  return out;
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
      : "cc");
  return value;
}

/*
  simple fixed point math
  Q10.6 for 16 bit, Q24.8 for 32 bit by default
  Mostly taken from SGDK
  https://github.com/Stephane-D/SGDK
*/

typedef int16_t f16;
typedef int32_t f32;

#define FIX16_INT_BITS 10
#define FIX16_FRAC_BITS (16 - FIX16_INT_BITS)
#define FIX32_INT_BITS 24
#define FIX32_FRAC_BITS (32 - FIX32_INT_BITS)

#define frac_to_f16(value) ((f16)((value) * (1 << FIX16_FRAC_BITS)))
#define int_to_f16(value) ((f16)((value) << FIX16_FRAC_BITS))
#define f16_to_int(value) ((s16)((value) >> FIX16_FRAC_BITS))

#define frac_to_f32(value) ((f32)((value) * (1 << FIX32_FRAC_BITS)))
#define int_to_f32(value) ((f32)((value) << FIX32_FRAC_BITS))
#define f32_to_int(value) ((s32)((value) >> FIX32_FRAC_BITS))

#endif

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file math.h
 * @brief Math functions
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

/**
 * @brief Quotient and remainder of a 32-by-16 divide
 */
typedef struct {
  int16_t quot; ///< Quotient
  int16_t rem;  ///< Remainder
} Div16;

/**
 * @brief Performs unsigned multiplication using MULU
 */
static inline uint32_t mulu(uint16_t multiplicand, uint16_t multiplier)
{
  uint32_t product = multiplier;
  asm("mulu.w %1, %0" : "+d"(product) : "d"(multiplicand) : "cc");
  return product;
}

/**
 * @brief Performs signed multiplication using MULS
 */
static inline int32_t mul(int16_t multiplicand, int16_t multiplier)
{
  int32_t product = multiplier;
  asm("muls.w %1, %0" : "+d"(product) : "d"(multiplicand) : "cc");
  return product;
}

/**
 * @brief 32-by-16 unsigned divide with DIVU; quotient and remainder are 16 bits
 */
static inline Div16 divu16(uint32_t dividend, uint16_t divisor)
{
  Div16    out;
  uint32_t result = dividend;
  asm("divu.w %1, %0" : "+d"(result) : "d"(divisor) : "cc");
  out.quot = result & 0x0000ffff;
  out.rem = (result & 0xffff0000) >> 16;
  return out;
}

/**
 * @brief 32-by-16 signed divide with DIVS; quotient and remainder are 16 bits
 */
static inline Div16 divs16(int32_t dividend, int16_t divisor)
{
  Div16   out;
  int32_t result = dividend;
  asm("divs.w %1, %0" : "+d"(result) : "d"(divisor) : "cc");
  out.quot = (int16_t) (result & 0x0000ffff);
  out.rem = (int16_t) ((result & 0xffff0000) >> 16);
  return out;
}

/**
 * @brief Performs unsigned division using DIVU and rounds the quotient based on
 * the remainder
 */
static inline uint16_t divu_round(uint32_t dividend, uint16_t divisor)
{
  uint32_t quotient = dividend;
  uint32_t dummy1 = 0;
  uint32_t dummy2 = 0;
  asm(
    "\
		mov.l %1, %3 \n\
		divu.w %1, %0 \n\
		mov.l %0, %2 \n\
		swap %2 \n\
		and #0xFFFF, %2 \n\
		lsl.l #1, %2 \n\
		cmp.w %3, %2 \n\
		blt 1f \n\
		addq #1, %0 \n\
	1: \n\
	"
    : "+d"(quotient)
    : "d"(divisor), "d"(dummy1), "d"(dummy2)
    : "cc");
  return quotient;
}

/**
 * @brief Performs signed division using DIVS and rounds the quotient based on
 * the remainder
 */
static inline int16_t divs_round(int32_t dividend, int16_t divisor)
{
  int32_t quotient = dividend;
  int32_t dummy1 = 0;
  int32_t dummy2 = 0;
  asm(
    "\
		mov.l %1, %3 \n\
		divs.w %1, %0 \n\
		mov.l %0, %2 \n\
		swap %2 \n\
		and #0xFFFF, %2 \n\
		lsl.l #1, %2 \n\
		cmp.w %3, %2 \n\
		blt 1f \n\
		addq #1, %0 \n\
	1: \n\
		"
    : "+d"(quotient)
    : "d"(divisor), "d"(dummy1), "d"(dummy2)
    : "cc");
  return quotient;
}

/**
 * @brief Convert a binary value of 0 to 99 to packed BCD
 * @details The CD BIOS takes time values (minutes, seconds, frames) in BCD.
 */
static inline uint16_t bcd(uint16_t value)
{
  uint16_t temp;
  asm(
    "\
		ext.l	%0 \n\
		divu.w #0xA, %0 \n\
		move.b %0, %1 \n\
		lsl.b	#4, %1 \n\
		swap %0 \n\
		move #0, ccr \n\
		abcd %1, %0 \n\
		"
    : "+d"(value)
    : "d"(temp)
    : "cc");
  return value;
}

#ifdef __cplusplus
}
#endif

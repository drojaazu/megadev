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
  s16 quot;
  s16 rem;
} Div16;

/**
 * @brief Performs unsigned multiplication using MULU
 */
static inline u32 mulu(u16 multiplicand, u16 multiplier)
{
	u32 product = multiplier;
	asm("mulu.w %1, %0" : "+d"(product) : "d"(multiplicand) : "cc");
	return product;
}

/**
 * @brief Performs signed multiplication using MULS
 */
static inline s32 mul(s16 multiplicand, s16 multiplier)
{
	s32 product = multiplier;
	asm("muls.w %1, %0" : "+d"(product) : "d"(multiplicand) : "cc");
	return product;
}

/**
 * @brief 32-by-16 unsigned divide with DIVU; quotient and remainder are 16 bits
 */
static inline Div16 divu16(u32 dividend, u16 divisor)
{
	Div16 out;
	u32 result = dividend;
	asm("divu.w %1, %0" : "+d"(result) : "d"(divisor) : "cc");
	out.quot = result & 0x0000ffff;
	out.rem = (result & 0xffff0000) >> 16;
	return out;
}

/**
 * @brief 32-by-16 signed divide with DIVS; quotient and remainder are 16 bits
 */
static inline Div16 divs16(s32 dividend, s16 divisor)
{
	Div16 out;
	s32 result = dividend;
	asm("divs.w %1, %0" : "+d"(result) : "d"(divisor) : "cc");
	out.quot = (s16) (result & 0x0000ffff);
	out.rem = (s16) ((result & 0xffff0000) >> 16);
	return out;
}

/**
 * @brief Performs unsigned division using DIVU and rounds the quotient based on
 * the remainder
 */
static inline u16 divu_round(u32 dividend, u16 divisor)
{
	u32 quotient = dividend;
	u32 dummy1 = 0;
	u32 dummy2 = 0;
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
static inline s16 divs_round(s32 dividend, s16 divisor)
{
	s32 quotient = dividend;
	s32 dummy1 = 0;
	s32 dummy2 = 0;
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

static inline u16 bcd(u16 value)
{
	u16 temp;
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

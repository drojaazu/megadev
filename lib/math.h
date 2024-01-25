/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file math.h
 * @brief Math functions
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
static inline u32 mulu(u16 multiplicand, u16 multiplier)
{
	u32 product = multiplier;
	__asm__("mulu.w %1, %0" : "+d"(product) : "d"(multiplicand) : "cc");
	return product;
}

/**
 * @brief Performs signed multiplication using MULS
 */
static inline s32 muls(s16 multiplicand, s16 multiplier)
{
	s32 product = multiplier;
	__asm__("muls.w %1, %0" : "+d"(product) : "d"(multiplicand) : "cc");
	return product;
}

/**
 * @brief Performs unsigned division using DIVU
 */
static inline u16 divu(u32 dividend, u16 divisor)
{
	u32 quotient = dividend;
	__asm__("divu.w %1, %0" : "+d"(quotient) : "d"(divisor) : "cc");
	return quotient;
}

/**
 * @brief Performs signed division using DIVS
 */
static inline s16 divs(s32 dividend, s16 divisor)
{
	s32 quotient = dividend;
	__asm__("divs.w %1, %0" : "+d"(quotient) : "d"(divisor) : "cc");
	return quotient;
}

/**
 * @brief Performs unsigned division using DIVU and returns the remainder
 */
static inline u16 modu(u32 dividend, u16 divisor)
{
	u32 modulus = dividend;
	__asm__(
		"\
		divu.w %1, %0 \n\
		swap %0 \n\
		"
		: "+d"(modulus)
		: "d"(divisor)
		: "cc");
	return (u16) modulus;
}

/**
 * @brief Performs signed division using DIVS and returns the remainder
 */
static inline s16 mods(s32 dividend, s16 divisor)
{
	s32 modulus = dividend;
	__asm__(
		"\
		divs.w %1, %0 \n\
		swap %0 \n\
		"
		: "+d"(modulus)
		: "d"(divisor)
		: "cc");
	return (s16) modulus;
}

/**
 * @brief Performs unsigned division using DIVU and returns the remainder and
 * quotient in one 32 bit value
 *
 * @return u32: Quotient in lower word, remainder in upper word
 */
static inline u32 divu_full(u32 dividend, u16 divisor)
{
	u32 remainder_quotient = dividend;
	__asm__("divu.w %1, %0" : "+d"(remainder_quotient) : "d"(divisor) : "cc");
	return remainder_quotient;
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
	__asm__(
		"\
		mov.l %1, %3 \n\
		divu.w %1, %0 \n\
		mov.l %0, %2 \n\
		swap %2 \n\
		and #0xffff, %2 \n\
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
	__asm__(
		"\
		mov.l %1, %3 \n\
		divs.w %1, %0 \n\
		mov.l %0, %2 \n\
		swap %2 \n\
		and #0xffff, %2 \n\
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
	__asm__(
		"\
		ext.l	%0 \n\
		divu.w #0xa, %0 \n\
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

static inline s32 abs(s32 val)
{
	__asm__(
		"\
			tst.l %0 \n\
			bpl 1f \n\
			neg.l %0 \n\
		1: \n\
		"
		: "+d"(val)
		:
		: "cc");
	return val;
}

static inline s16 abs16(s16 val)
{
	__asm__(
		"\
			tst.w %0 \n\
			bpl 1f \n\
			neg.w %0 \n\
		1: \n\
		"
		: "+d"(val)
		:
		: "cc");
	return val;
}

#endif

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file vdp_def.h
 * @brief GCC internal functions
 */

#include "stdint.h"

s32 __mulsi3 (s16 multiplier, s16 multiplicand)
{
	return muls (multiplier, multiplicand);
}

s16 __divsi3 (s32 dividend, s16 divisor)
{
	return divs (dividend, divisor);
}

u16 __udivsi3 (u32 dividend, u16 divisor)
{
	return divu (dividend, divisor);
}

s16 __modsi3 (s32 dividend, s16 divisor)
{
	return mods (dividend, divisor);
}

u16 __umodsi3 (u32 dividend, u16 divisor)
{
	return modu (dividend, divisor);
}

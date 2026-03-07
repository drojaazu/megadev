/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file str_util.h
 * @brief C wrappers for hex string conversion functions
 */

#ifndef MEGADEV__MAIN_PRINTVAL_H
#define MEGADEV__MAIN_PRINTVAL_H

#include "types.h"

static inline u8 _hextoa(register u8 value)
{
	value &= 0x0f;
	if (value <= 9)
		value += 0x30;
	else
		value += 0x37;
	return value;
}

/**
 * @fn Convert an 8 bit value to a hexadecimal ASCII string
 */
static inline void hextoa8(register u8 value, register char * const string)
{
	register char * output = string;
	asm("rol.b #4, %[value]" : [value] "+d"(value));
	*output++ = _hextoa(value);
	asm("rol.b #4, %[value]" : [value] "+d"(value));
	*output++ = _hextoa(value);
}

/**
 * @fn Convert a 16 bit value to a hexadecimal ASCII string
 */
static inline void hextoa16(register u16 value, register char * const string)
{
	register char * output = string;
	asm("rol.w #4, %[value]" : [value] "+d"(value));
	*output++ = _hextoa(value);
	asm("rol.w #4, %[value]" : [value] "+d"(value));
	*output++ = _hextoa(value);
	asm("rol.w #4, %[value]" : [value] "+d"(value));
	*output++ = _hextoa(value);
	asm("rol.w #4, %[value]" : [value] "+d"(value));
	*output++ = _hextoa(value);
}

/**
 * @fn Convert a 32 bit value to a hexadecimal ASCII string
 */
static inline void hextoa32(register u32 value, register char * const string)
{
	register char * output = string;
	for (u8 i = 0; i < 2; ++i)
	{
		asm("rol.l #4, %[value]" : [value] "+d"(value));
		*output++ = _hextoa(value);
		asm("rol.l #4, %[value]" : [value] "+d"(value));
		*output++ = _hextoa(value);
		asm("rol.l #4, %[value]" : [value] "+d"(value));
		*output++ = _hextoa(value);
		asm("rol.l #4, %[value]" : [value] "+d"(value));
		*output++ = _hextoa(value);
	}
}

static bool strcmp(char const * str1, char const * str2)
{
	while (*str1 && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	return (*(const unsigned char *) str1 - *(const unsigned char *) str2) == 0;
}

static bool strcmpt(char const * str1, char const * str2, char terminator)
{
	while (*str1 != terminator && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	return (*(const unsigned char *) str1 - *(const unsigned char *) str2) == 0;
}

#endif

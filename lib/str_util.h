/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file str_util.h
 * @brief C wrappers for hex string conversion functions
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "str_util.def.h"
#include "types.h"

/** @brief ASCII digit for the low nibble of value: '0' to '9', 'A' to 'F' */
static inline uint8_t _hextoa(register uint8_t value)
{
  value &= 0x0f;
  if (value <= 9)
    value += 0x30;
  else
    value += 0x37;
  return value;
}

/**
 * @brief Convert an 8 bit value to a hexadecimal ASCII string
 */
static inline void hextoa8(register uint8_t value, register char * const string)
{
  register char * output = string;
  asm("rol.b #4, %[value]" : [value] "+d"(value));
  *output++ = _hextoa(value);
  asm("rol.b #4, %[value]" : [value] "+d"(value));
  *output++ = _hextoa(value);
  *output = STRING_TERMINATOR;
}

/**
 * @brief Convert a 16 bit value to a hexadecimal ASCII string
 */
static inline void
hextoa16(register uint16_t value, register char * const string)
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
  *output = STRING_TERMINATOR;
}

/**
 * @brief Convert a 32 bit value to a hexadecimal ASCII string
 */
static inline void
hextoa32(register uint32_t value, register char * const string)
{
  register char * output = string;
  for (uint8_t i = 0; i < 2; ++i) {
    asm("rol.l #4, %[value]" : [value] "+d"(value));
    *output++ = _hextoa(value);
    asm("rol.l #4, %[value]" : [value] "+d"(value));
    *output++ = _hextoa(value);
    asm("rol.l #4, %[value]" : [value] "+d"(value));
    *output++ = _hextoa(value);
    asm("rol.l #4, %[value]" : [value] "+d"(value));
    *output++ = _hextoa(value);
  }
  *output = STRING_TERMINATOR;
}

/**
 * @brief Compare two strings up to a terminator character
 * @param str1 String ended by terminator
 * @param str2 String to compare with
 * @param terminator The character that ends str1, in place of 0
 * @return true if equal up to the terminator
 */
static inline bool
strcmpt(char const * str1, char const * str2, char terminator)
{
  while (*str1 != terminator && (*str1 == *str2)) {
    str1++;
    str2++;
  }
  return (*(const unsigned char *) str1 - *(const unsigned char *) str2) == 0;
}

/**
 * @brief Terminate a C string for the Boot ROM print routines
 * @param string NUL-terminated string, changed in place
 * @return string, now terminated with STRING_TERMINATOR (0xFF) instead of NUL
 *
 * @details
 * For text formatted with snprintf and printed with bios_print.
 */
static inline char * str_to_bios(char * string)
{
  char * p = string;
  while (*p)
    ++p;
  *p = (char) STRING_TERMINATOR;
  return string;
}

#ifdef __cplusplus
}
#endif

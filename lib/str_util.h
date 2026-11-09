/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file str_util.h
 * @brief String helpers beyond the C library
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "str_util.def.h"
#include "types.h"

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

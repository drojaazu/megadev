/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file ctype.h
 * @brief Character classification, ASCII only
 *
 * @details
 * Standard names and semantics for the ASCII range. Arguments outside 0 to 127
 * are not letters, digits or spaces.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup libc
 *  @{ */

/** @brief c is '0' to '9' */
static inline int isdigit(int c)
{
  return c >= '0' && c <= '9';
}

/** @brief c is 'A' to 'Z' */
static inline int isupper(int c)
{
  return c >= 'A' && c <= 'Z';
}

/** @brief c is 'a' to 'z' */
static inline int islower(int c)
{
  return c >= 'a' && c <= 'z';
}

/** @brief c is a letter */
static inline int isalpha(int c)
{
  return isupper(c) || islower(c);
}

/** @brief c is a letter or digit */
static inline int isalnum(int c)
{
  return isalpha(c) || isdigit(c);
}

/** @brief c is a hexadecimal digit */
static inline int isxdigit(int c)
{
  return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

/** @brief c is a space, tab, newline, vertical tab, form feed or carriage
 * return */
static inline int isspace(int c)
{
  return c == ' ' || (c >= '\t' && c <= '\r');
}

/** @brief Upper case form of c, or c unchanged */
static inline int toupper(int c)
{
  return islower(c) ? c - ('a' - 'A') : c;
}

/** @brief Lower case form of c, or c unchanged */
static inline int tolower(int c)
{
  return isupper(c) ? c + ('a' - 'A') : c;
}

/** @} */

#ifdef __cplusplus
}
#endif

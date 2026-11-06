/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file string.h
 * @brief Standard string and memory functions
 *
 * @details
 * The subset of the C string library that Megadev provides, with standard
 * signatures and semantics. Strings are NUL-terminated as in C. Megadev's own
 * string conventions, such as the 0xFF terminator the Boot ROM print routines
 * expect, live in str_util.h.
 *
 * The functions are implemented in lib/runtime and linked only when used.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

/** @brief Copy n bytes from src to dest; the regions must not overlap
 * @return dest */
void * memcpy(void * dest, void const * src, size_t n);
/** @brief Copy n bytes from src to dest; the regions may overlap
 * @return dest */
void * memmove(void * dest, void const * src, size_t n);
/** @brief Fill n bytes at dest with the low byte of value
 * @return dest */
void * memset(void * dest, int value, size_t n);
/** @brief Compare n bytes
 * @return 0 if equal, negative if a sorts first, positive if b does */
int memcmp(void const * a, void const * b, size_t n);

/** @brief Length of a string, not counting the terminator */
size_t strlen(char const * s);
/** @brief Compare two strings
 * @return 0 if equal, negative if a sorts first, positive if b does */
int strcmp(char const * a, char const * b);
/** @brief Compare at most n characters of two strings
 * @return 0 if equal, negative if a sorts first, positive if b does */
int strncmp(char const * a, char const * b, size_t n);
/** @brief Copy a string, terminator included
 * @return dest */
char * strcpy(char * dest, char const * src);
/** @brief Copy at most n characters, padding dest with zeros to n
 * @return dest */
char * strncpy(char * dest, char const * src, size_t n);
/** @brief Append src to dest
 * @return dest */
char * strcat(char * dest, char const * src);
/** @brief Find the first occurrence of c in s
 * @return Pointer to it, or NULL */
char * strchr(char const * s, int c);

#ifdef __cplusplus
}
#endif

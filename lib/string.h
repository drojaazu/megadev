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

void * memcpy(void * dest, void const * src, size_t n);
void * memmove(void * dest, void const * src, size_t n);
void * memset(void * dest, int value, size_t n);
int memcmp(void const * a, void const * b, size_t n);

size_t strlen(char const * s);
int strcmp(char const * a, char const * b);
int strncmp(char const * a, char const * b, size_t n);
char * strcpy(char * dest, char const * src);
char * strncpy(char * dest, char const * src, size_t n);
char * strcat(char * dest, char const * src);
char * strchr(char const * s, int c);

#ifdef __cplusplus
}
#endif

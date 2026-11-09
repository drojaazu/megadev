/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file stdio.h
 * @brief Formatted output into a buffer
 *
 * @details
 * There are no streams and no FILE: the only stdio functions Megadev provides
 * format into a caller's buffer. Conversions: %d %i %u %x %X %c %s %%, with
 * an optional '-' or '0' flag, a field width, and the l length modifier. No
 * floating point.
 *
 * Output is NUL-terminated as in C. Print it with text_print (main/text.h),
 * or convert it with str_to_bios (str_util.h) for the Boot ROM print routines,
 * which expect a 0xFF terminator.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup libc
 *  @{ */

#include <stdarg.h>
#include <types.h>

/**
 * @brief Format into buf, writing at most n bytes including the terminator
 * @return The length the full output would have had, as the standard requires;
 * if it is n or more, the output was truncated
 */
int vsnprintf(char * buf, size_t n, char const * fmt, va_list args);

/**
 * @brief Format into buf, writing at most n bytes including the terminator
 * @return The length the full output would have had
 */
int snprintf(char * buf, size_t n, char const * fmt, ...);

/** @} */

#ifdef __cplusplus
}
#endif

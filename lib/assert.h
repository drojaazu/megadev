/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file assert.h
 * @brief Runtime assertions reported over the serial port
 *
 * @details
 * assert(expr) does nothing when expr is true. When it is false, it formats
 * the file, line and expression, sends the text over the serial port with
 * ext_tx, and stops the CPU. The port must already be configured with
 * init_ext_port; see main/comm.h. Define NDEBUG to compile assertions out.
 *
 * Main CPU only, since the serial port is a Main CPU peripheral.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG

/** @brief Does nothing: assertions are compiled out when NDEBUG is defined */
#define assert(expr) ((void) 0)

#else

#include <main/comm.h>
#include <stdio.h>
#include <system.h>

/** @brief Stringize x without expanding it; MEGADEV_ASSERT_STR expands first */
#define MEGADEV_ASSERT_STR_(x) #x
/** @brief Stringize x after macro expansion, used for __LINE__ */
#define MEGADEV_ASSERT_STR(x) MEGADEV_ASSERT_STR_(x)

/**
 * @brief Report a failed assertion over the serial port and stop the CPU
 * @param where File and line, as "file:line"
 * @param expr The assertion's text
 */
static inline void megadev_assert_fail(char const * where, char const * expr)
{
  char text[96];
  int const len = snprintf(text, sizeof text, "\r\nassertion failed: %s: %s\r\n", where, expr);
  for (int i = 0; i < len && i < (int) sizeof text - 1; ++i)
    ext_tx((u8) text[i]);
  cpu_stop();
}

/** @brief Stop with a report on the serial port if expr is false */
#define assert(expr) \
  ((expr) ? (void) 0 : megadev_assert_fail(__FILE__ ":" MEGADEV_ASSERT_STR(__LINE__), #expr))

#endif

#ifdef __cplusplus
}
#endif

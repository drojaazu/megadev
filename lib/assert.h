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

#ifdef NDEBUG

#define assert(expr) ((void) 0)

#else

#include <main/comm.h>
#include <stdio.h>
#include <system.h>

#define MEGADEV_ASSERT_STR_(x) #x
#define MEGADEV_ASSERT_STR(x) MEGADEV_ASSERT_STR_(x)

static inline void megadev_assert_fail(char const * where, char const * expr)
{
  char text[96];
  int const len = snprintf(text, sizeof text, "\r\nassertion failed: %s: %s\r\n", where, expr);
  for (int i = 0; i < len && i < (int) sizeof text - 1; ++i)
    ext_tx((u8) text[i]);
  cpu_stop();
}

#define assert(expr) \
  ((expr) ? (void) 0 : megadev_assert_fail(__FILE__ ":" MEGADEV_ASSERT_STR(__LINE__), #expr))

#endif

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/strtol.c
 * @brief Integer parsing: strtol and atoi
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static int digit_value(int c)
{
  if (isdigit(c))
    return c - '0';
  if (isalpha(c))
    return tolower(c) - 'a' + 10;
  return 99;
}

long strtol(char const * s, char ** end, int base)
{
  char const * p = s;
  while (isspace(*p))
    ++p;

  int negative = 0;
  if (*p == '+' || *p == '-')
  {
    negative = *p == '-';
    ++p;
  }

  if ((base == 0 || base == 16) && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')
    && isxdigit(p[2]))
  {
    base = 16;
    p += 2;
  }
  else if (base == 0)
    base = p[0] == '0' ? 8 : 10;

  // Accumulate as an unsigned magnitude, clamping on overflow
  unsigned long const limit = negative ? (unsigned long) LONG_MAX + 1 : (unsigned long) LONG_MAX;
  unsigned long value = 0;
  int overflow = 0;
  char const * start = p;
  for (;; ++p)
  {
    int const d = digit_value(*p);
    if (d >= base)
      break;
    if (value > (limit - (unsigned long) d) / (unsigned long) base)
      overflow = 1;
    else
      value = value * (unsigned long) base + (unsigned long) d;
  }

  if (p == start)
  {
    // No digits: nothing consumed
    if (end)
      *end = (char *) s;
    return 0;
  }
  if (end)
    *end = (char *) p;
  if (overflow)
    return negative ? LONG_MIN : LONG_MAX;
  return negative ? -(long) value : (long) value;
}

int atoi(char const * s)
{
  return (int) strtol(s, NULL, 10);
}


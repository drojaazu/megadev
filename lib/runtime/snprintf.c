/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/snprintf.c
 * @brief Integer-only formatted output into a buffer
 */

#include <stdio.h>
#include <string.h>

typedef struct
{
  char * buf;
  size_t n;      // buffer size, including the terminator
  size_t count;  // characters the full output would have
} Sink;

static void put(Sink * s, char c)
{
  if (s->count + 1 < s->n)
    s->buf[s->count] = c;
  ++s->count;
}

static void pad(Sink * s, int count, char c)
{
  while (count-- > 0)
    put(s, c);
}

static void put_field(Sink * s, char const * text, int len, int width, int left, char fill)
{
  int const padding = width - len;
  if (!left)
    pad(s, padding, fill);
  while (len-- > 0)
    put(s, *text++);
  if (left)
    pad(s, padding, ' ');
}

int vsnprintf(char * buf, size_t n, char const * fmt, va_list args)
{
  Sink s = {buf, n, 0};
  char digits[sizeof(unsigned long) * 3 + 2];

  for (; *fmt; ++fmt)
  {
    if (*fmt != '%')
    {
      put(&s, *fmt);
      continue;
    }
    ++fmt;

    int left = 0;
    char fill = ' ';
    for (;; ++fmt)
    {
      if (*fmt == '-')
        left = 1;
      else if (*fmt == '0')
        fill = '0';
      else
        break;
    }

    int width = 0;
    while (*fmt >= '0' && *fmt <= '9')
      width = width * 10 + (*fmt++ - '0');

    int is_long = 0;
    if (*fmt == 'l')
    {
      is_long = 1;
      ++fmt;
    }

    switch (*fmt)
    {
      case 'd':
      case 'i':
      case 'u':
      case 'x':
      case 'X':
      {
        int const is_signed = *fmt == 'd' || *fmt == 'i';
        unsigned long const base = (*fmt == 'x' || *fmt == 'X') ? 16 : 10;
        char const hex_a = *fmt == 'X' ? 'A' : 'a';
        unsigned long value;
        int negative = 0;
        if (is_signed)
        {
          long const v = is_long ? va_arg(args, long) : va_arg(args, int);
          negative = v < 0;
          value = negative ? 0UL - (unsigned long) v : (unsigned long) v;
        }
        else
          value = is_long ? va_arg(args, unsigned long) : va_arg(args, unsigned int);

        int len = 0;
        do
        {
          unsigned long const d = value % base;
          digits[sizeof digits - 1 - len++] = (char) (d < 10 ? '0' + d : hex_a + d - 10);
          value /= base;
        } while (value);

        if (negative)
        {
          if (fill == '0' && !left)
          {
            // Sign before the zero padding
            put(&s, '-');
            --width;
          }
          else
            digits[sizeof digits - 1 - len++] = '-';
        }
        put_field(&s, digits + sizeof digits - len, len, width, left, fill);
        break;
      }
      case 'c':
      {
        char const c = (char) va_arg(args, int);
        put_field(&s, &c, 1, width, left, ' ');
        break;
      }
      case 's':
      {
        char const * const str = va_arg(args, char const *);
        put_field(&s, str, (int) strlen(str), width, left, ' ');
        break;
      }
      case '%':
        put(&s, '%');
        break;
      case '\0':
        --fmt;  // let the loop see the terminator
        break;
      default:
        // Unknown conversion: emit it literally
        put(&s, '%');
        put(&s, *fmt);
        break;
    }
  }

  if (n)
    buf[s.count < n ? s.count : n - 1] = '\0';
  return (int) s.count;
}

int snprintf(char * buf, size_t n, char const * fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  int const count = vsnprintf(buf, n, fmt, args);
  va_end(args);
  return count;
}

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/string.c
 * @brief Standard string functions
 */

#include <string.h>

size_t strlen(char const * s)
{
  char const * p = s;
  while (*p)
    ++p;
  return (size_t) (p - s);
}

int strcmp(char const * a, char const * b)
{
  while (*a && *a == *b)
  {
    ++a;
    ++b;
  }
  return (int) (unsigned char) *a - (int) (unsigned char) *b;
}

int strncmp(char const * a, char const * b, size_t n)
{
  while (n && *a && *a == *b)
  {
    ++a;
    ++b;
    --n;
  }
  return n ? (int) (unsigned char) *a - (int) (unsigned char) *b : 0;
}

char * strcpy(char * dest, char const * src)
{
  char * d = dest;
  while ((*d++ = *src++))
    ;
  return dest;
}

char * strncpy(char * dest, char const * src, size_t n)
{
  char * d = dest;
  while (n && *src)
  {
    *d++ = *src++;
    --n;
  }
  while (n--)
    *d++ = '\0';
  return dest;
}

char * strcat(char * dest, char const * src)
{
  strcpy(dest + strlen(dest), src);
  return dest;
}

char * strchr(char const * s, int c)
{
  char const ch = (char) c;
  for (;; ++s)
  {
    if (*s == ch)
      return (char *) s;
    if (!*s)
      return NULL;
  }
}

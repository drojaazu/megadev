/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/sort.c
 * @brief Binary search and sort: bsearch and qsort
 */

#include <stdlib.h>

void * bsearch(void const * key, void const * base, size_t count, size_t size,
  int (*compare)(void const *, void const *))
{
  char const * lo = base;
  while (count)
  {
    size_t const mid = count / 2;
    char const * const probe = lo + mid * size;
    int const c = compare(key, probe);
    if (c == 0)
      return (void *) probe;
    if (c > 0)
    {
      lo = probe + size;
      count -= mid + 1;
    }
    else
      count = mid;
  }
  return NULL;
}

void qsort(void * base, size_t count, size_t size,
  int (*compare)(void const *, void const *))
{
  char * const b = base;
  for (size_t i = 1; i < count; ++i)
  {
    // Shift the element at i down past every larger neighbor, one byte swap
    // at a time, so no temporary of the element's size is needed
    for (size_t j = i; j > 0 && compare(b + (j - 1) * size, b + j * size) > 0; --j)
    {
      char * p = b + (j - 1) * size;
      char * q = b + j * size;
      for (size_t k = 0; k < size; ++k)
      {
        char const t = p[k];
        p[k] = q[k];
        q[k] = t;
      }
    }
  }
}

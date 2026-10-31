/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file stdlib.h
 * @brief Standard integer arithmetic, conversion, random number, search and
 * sort functions
 *
 * @details
 * The subset of the C standard library that Megadev provides, with standard
 * signatures and semantics. There is no heap: malloc and free are not here.
 *
 * The 68000-specific 32-by-16 divides are in math.h as divs16 and divu16.
 */

#pragma once

#include <types.h>

typedef struct
{
  int quot;
  int rem;
} div_t;

typedef struct
{
  long quot;
  long rem;
} ldiv_t;

static inline int abs(int value)
{
  return value < 0 ? -value : value;
}

static inline long labs(long value)
{
  return value < 0 ? -value : value;
}

static inline div_t div(int numer, int denom)
{
  div_t out = {numer / denom, numer % denom};
  return out;
}

static inline ldiv_t ldiv(long numer, long denom)
{
  ldiv_t out = {numer / denom, numer % denom};
  return out;
}

/**
 * @brief Parse a decimal integer, with optional leading whitespace and sign
 */
int atoi(char const * s);

/**
 * @brief Parse an integer in the given base, 2 to 36, or 0 to infer from a
 * 0x or 0 prefix. Sets *end to the first unparsed character when end is not
 * NULL. Out-of-range values are clamped to LONG_MAX or LONG_MIN.
 */
long strtol(char const * s, char ** end, int base);

/**
 * @brief Largest value returned by rand
 */
#define RAND_MAX 0x7FFFFFFF

/**
 * @brief Next pseudo-random number, 0 to RAND_MAX, from a 32-bit xorshift
 * generator. Deterministic for a given seed.
 */
int rand(void);

/**
 * @brief Seed the generator; a seed of 0 is replaced by a fixed non-zero seed
 */
void srand(unsigned int seed);

/**
 * @brief Binary search of a sorted array
 */
void * bsearch(void const * key, void const * base, size_t count, size_t size,
  int (*compare)(void const *, void const *));

/**
 * @brief Sort an array in place
 *
 * @details
 * An insertion sort: no recursion, no stack beyond one element, and fast on
 * the small, nearly-sorted arrays that are typical here. Its cost grows with
 * the square of the count, so it is not for large arrays or for sorting
 * every frame; a type-specific sort will beat the callback per comparison.
 */
void qsort(void * base, size_t count, size_t size,
  int (*compare)(void const *, void const *));

#define LONG_MAX __LONG_MAX__
#define LONG_MIN (-LONG_MAX - 1L)

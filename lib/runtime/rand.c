/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/rand.c
 * @brief Pseudo-random numbers: rand and srand
 */

#include <stdlib.h>

static unsigned long rand_state = 0x2545F491;

void srand(unsigned int seed)
{
  rand_state = seed ? seed : 0x2545F491;
}

int rand(void)
{
  // xorshift32
  unsigned long x = rand_state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  rand_state = x;
  return (int) (x & RAND_MAX);
}


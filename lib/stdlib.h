/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file stdlib.h
 * @brief POSIX-y stdlib
 */

#ifndef MEGADEV__STDLIB_H
#define MEGADEV__STDLIB_H

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

static inline int abs (int val)
{
	asm(
		"\
			tst.l %0 \n\
			bpl 1f \n\
			neg.l %0 \n\
		1: \n\
		"
		: "+d"(val)
		:
		: "cc");
	return val;
}

static inline short abs16 (short val)
{
	asm(
		"\
			tst.w %0 \n\
			bpl 1f \n\
			neg.w %0 \n\
		1: \n\
		"
		: "+d"(val)
		:
		: "cc");
	return val;
}

typedef struct {
  int quot;
  int rem;
} div_t;

#endif

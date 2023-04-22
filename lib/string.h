/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file string.h
 * @brief String utilities
 */

#include "types.h"

extern void(*ATOI);

static inline u32 atoi(char * string)
{
	register u32 D0 asm("d0");
	register u32 A0 asm("a0") = (u32) string;

	asm volatile(
		"\
			jsr ATOI \n\
		"
		: "=d"(D0), "+a"(A0)
		: "i"(&ATOI), "a"(A0)
		: "d1");

	return D0;
};
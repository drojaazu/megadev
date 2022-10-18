/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file printval.h
 * @brief C wrappers for hex string conversion functions
 */

#ifndef MEGADEV__MAIN_PRINTVAL_H
#define MEGADEV__MAIN_PRINTVAL_H

#include "types.h"

static inline void printval_u8_c (u8 const value, char * output)
{
	register u8 d0_value asm("d0") = value;
	register u32 a0_output asm("a0") = (u32) output;

	asm volatile("jbsr printval_u8"
							 : "+d"(d0_value), "+a"(a0_output)
							 : "d"(d0_value), "a"(a0_output)
							 : "d1");
}

static inline void printval_u16_c (u16 const value, char * output)
{
	register u16 d0_value asm("d0") = value;
	register u32 a0_output asm("a0") = (u32) output;

	asm volatile("jbsr printval_u16"
							 : "+d"(d0_value), "+a"(a0_output)
							 : "d"(d0_value), "a"(a0_output)
							 : "d1", "d7");
}

static inline void printval_u32_c (u32 const value, char * output)
{
	register u32 d0_value asm("d0") = value;
	register u32 a0_output asm("a0") = (u32) output;

	asm volatile("jbsr printval_u32"
							 : "+d"(d0_value), "+a"(a0_output)
							 : "d"(d0_value), "a"(a0_output)
							 : "d1", "d7");
}

#endif

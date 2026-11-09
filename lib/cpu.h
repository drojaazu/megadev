/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file cpu.h
 * @brief System utilities
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

/**
 * Disables all interrupt levels; preserves other bits
 */
static inline void disable_interrupts()
{
	asm("ori #0x700,sr");
}

/**
 * Enables all interrupt levels; preserves other bits
 */
static inline void enable_interrupts()
{
	asm("andi #0xF8FF,sr");
}

/**
 * @brief Disable all interrupt levels and return the previous status register
 *
 * @details
 * Pair with @ref restore_interrupts. Unlike @ref disable_interrupts and
 * @ref enable_interrupts, which set a level unconditionally, this restores the
 * caller's state and so nests safely. Use it in any routine that may run both
 * from the main program and from an interrupt handler.
 */
static inline u16 mask_interrupts()
{
	u16 sr;
	asm volatile("move.w %%sr, %0 \n\t ori #0x700, %%sr" : "=d"(sr) : : "cc");
	return sr;
}

/**
 * @brief Restore a status register saved by @ref mask_interrupts
 */
static inline void restore_interrupts(u16 const sr)
{
	asm volatile("move.w %0, %%sr" : : "d"(sr) : "cc");
}

/** @brief Issue the RESET instruction: pulses the reset line to the peripherals, the CPU itself keeps running */
static inline void cpu_reset()
{
	asm("reset");
}

/** @brief Halt the CPU with interrupts disabled; only a reset leaves this state */
static inline void cpu_stop()
{
	asm("stop #0x700");
}

#ifdef __cplusplus
}
#endif

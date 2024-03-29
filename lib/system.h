/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file system.h
 * @brief System utilities
 */

#ifndef MEGADEV__SYSTEM_H
#define MEGADEV__SYSTEM_H

/**
 * Disables all interrupt levels; preserves other bits
 */
static inline void disable_interrupts()
{
	asm ("or #0x700,sr");
}

/**
 * Enables all interrupt levels; preserves other bits
 */
static inline void enable_interrupts()
{
	asm ("and #0xF8FF,sr");
}

#endif

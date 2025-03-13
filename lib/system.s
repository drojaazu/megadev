/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file system.s
 * @brief System utilities
 */

#ifndef SYSTEM_S
#define SYSTEM_S

.macro disable_interrupts
  ori #0x700, sr
.endm

.macro enable_interrupts
  andi #0xF8FF, sr
.endm

#endif

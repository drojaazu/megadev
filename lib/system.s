/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file system.s
 * @brief CPU System Control Operations
 */

#ifndef SYSTEM_S
#define SYSTEM_S

.macro disable_interrupts
  ori #0x700, sr
.endm

.macro enable_interrupts
  andi #0xF8FF, sr
.endm

.macro cpu_stop
  stop #0x700
.endm

#endif

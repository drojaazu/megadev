/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file std_init.s
 * @brief Standard initialization for modules
 */

#include <macros.s>
#include <system.macro.s>
#include <init.macro.s>

.section .init

GLABEL init
  DISABLE_INTERRUPTS
  BASIC_INIT
  jmp      main

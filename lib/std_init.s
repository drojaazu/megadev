/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file std_init.s
 * @brief Standard initialization for modules
 */

#include <macros.s>
#include <system.macros.s>
#include <init.macros.s>

.section .init

GLABEL init
  DISABLE_INTERRUPTS
  BASIC_INIT
  jmp      main

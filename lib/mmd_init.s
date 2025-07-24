/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file std_init.s
 * @brief Standard initialization for modules
 */

#include <macros.s>
#include <system.s>
#include <init.macros.s>
#include <main/init_mmd.s>

.section .init

GLABEL init
  DISABLE_INTERRUPTS
  BASIC_INIT
  INIT_MMD
  jmp      (a0)

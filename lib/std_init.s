/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file std_init.s
 * @brief Standard initialization for modules
 */

#include <macro.s>
#include <cpu.macro.s>
#include <init.macro.s>

.section .init

GLABEL init
  DISABLE_INTERRUPTS
  BASIC_INIT
#ifdef INIT_SET_STACK
  move.l   #STACK_TOP, sp
#endif
  jmp      main

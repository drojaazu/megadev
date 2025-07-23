/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file std_init.s
 * @brief Standard initializor
 * @details Performs four main tasks:
 *          - Disables interrupts
 *          - Clears .bss section RAM
 *          - Copies .data section into RAM
 *          - Jumps to main
 */

#include <macros.s>
#include <system.s>
#include <init.macros.s>

// putting this code in .init ensures it will be at the start of the module
.section .init

GLABEL init
  DISABLE_INTERRUPTS
  INIT_RAM
  jmp      main

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file ipx_init.s
 * @brief Standard initialization for IPX (or any "self-copying" MMDs)
 */

#include <system.macros.s>
#include <init.macros.s>
#include <main/mmd.macros.s>

.section .init

  DISABLE_INTERRUPTS
  BASIC_INIT
  INIT_MMD
  jmp      (a0)

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file ipx_init.s
 * @brief Standard initialization for IPX (or any "self-copying" MMDs)
 */

#include <macros.s>
#include <system.s>
#include <init.macros.s>
#include <main/init_mmd.s>

.section .init

  DISABLE_INTERRUPTS
  BASIC_INIT
  INIT_MMD
  jmp      (a0)

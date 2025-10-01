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
  /*
    very important that we call INIT_MMD first, so that everything is copied to
    where it is expected when doing the init - but we need to save the MMD
    entry point returned in A0 beforehand, just in case, so we push/pop
  */
  INIT_MMD
  PUSH a0
  BASIC_INIT
  POP a0
  jmp      (a0)

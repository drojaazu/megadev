/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file md_init.s
 * @brief common Mega Drive init routines
 */

#include <system.macros.s>
#include "main/md_sys.def.h"
#include "main/io.def.h"
#include "main/vdp.def.h"
#include "main/z80.macros.s"

.align 2

z80_dummy_program:
  dc.w 0xAF01, 0xD91F, 0x1127, 0x0021, 0x2600, 0xF977, 0xEDB0, 0xDDE1
  dc.w 0xFDE1, 0xED47, 0xED4F, 0xD1E1, 0xF108, 0xD9C1, 0xD1E1, 0xF1F9
  dc.w 0xF3ED, 0x5636, 0xE9E9
z80_dummy_program_end:

.align 2
init_system:
  DISABLE_INTERRUPTS
  tst.l   (_IO_CTRL1 - 1)
  bne.s   0f
  tst.w   (_IO_CTRL3 - 1)
0:bne.s   skip_init

  // Check Version Number
  move.b  (_HW_VERSION), d0
  andi.b  #HW_REV, d0
  beq.s   skip_tmss
  move.l  #0x53454741, (_TMSS)

skip_tmss:
  // Read from the VDP control port to cancel any pending read/write command
  move.w  (VDP_CTRL), d0
  // TODO add VDP initialization (CRAM/VRAM clear)

  // clear work RAM
  move.l  #0x3fff,d7
  moveq   #0, d0
  lea     0xff0000, a0
1:move.l  d0, (a0)+
  dbra    d7, 1b

skip_init:
  // initialize Z80 with a dummy program
  lea     z80_dummy_program, a0
  move.w  #(z80_dummy_program_end - z80_dummy_program - 1), d7
  Z80_INIT
  ENABLE_INTERRUPTS
  jbra main

.align 2
.global EX_NULL
EX_NULL:
  rte

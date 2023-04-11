/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file md_init.s
 * @brief common Mega Drive init routines
 */

#include "main/md_sys.def.h"
#include "main/io.def.h"
#include "main/vdp.def.h"
#include "main/vdp.macro.s"

.align 2
init:
	ori     #0x700, sr
1:move.w  (_VDP_CTRL), d0
	btst    #1, d0
	bne     1b


	tst.b    (_IO_CTRL1)
	bne    2f
	tst.b    (_IO_CTRL3)
2:bne    user_main
	move.b  (_MD_VERSION), d0
	andi.b  #0x0f, d0
	beq.s   user_main
	move.l  #0x53454741, (_TMSS)
	lea     _VDP_CTRL, a0
	lea     _VDP_DATA, a1
	move.w  (a0), d0
	moveq   #0, d0
	move.l  #(VDPPTR(0) | CRAM_W), (_VDP_CTRL)
	move.w  #0x1f, d7
3:move.l  d0, (a1)
  dbf.w     d7, 3b
	// TODO initial vdp regs here
	// TODO z80 setup here
	move    #0x2700, sr


user_main:
	jmp main

.align 2
.global _EX_NULL
_EX_NULL:
	rte

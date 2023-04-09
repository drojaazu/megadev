/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file md_init.s
 * @brief common Mega Drive init routines
 */

#include "main/md_sys.def.h"
#include "main/io_def.h"
#include "main/vdp.def.h"

.align 2
init:
	move    #0x2700, sr
	tst.l   _IO_CTRL1
	bne.s   input_detect

	tst.w   _IO_CTRL3

input_detect:
	bne.s   warm_boot

	move.b  (_HW_VERSION), d0
	andi.b  #0x0f, d0
	beq.s   no_tmss

	move.l  #0x53454741, (_TMSS)
no_tmss:
	move.w  (_VDP_CTRL), d0
warm_boot:
	jmp main

.align 2
.global _EX_NULL
_EX_NULL:
	rte

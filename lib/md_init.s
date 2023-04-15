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
        move    #0x2700,%sr
        tst.l   0xa10008
        bne.s   SkipJoyDetect

        tst.w   0xa1000c

SkipJoyDetect:
        bne.s   SkipSetup

        lea     Table,%a5
        movem.w (%a5)+,%d5-%d7
        movem.l (%a5)+,%a0-%a4
// Check Version Number
        move.b  -0x10ff(%a1),%d0
        andi.b  #0x0f,%d0
        beq.s   WrongVersion

// Sega Security Code (SEGA)
        move.l  #0x53454741,0x2f00(%a1)
WrongVersion:
// Read from the control port to cancel any pending read/write command
        move.w  (%a4),%d0

// Configure a USER_STACK_LENGTH bytes user stack at bottom, and system stack on top of it

        move.w  %d7,(%a1)
        move.w  %d7,(%a2)

				move.l #0x3fff,d7
				moveq #0, d0
				lea 0xff0000, a0
			0:move.l d0, (a0)+
  			dbra d7, 0b

* Jump to initialisation process now...
SkipSetup:
        jmp     main


Table:
        dc.w    0x8000,0x3fff,0x0100
        dc.l    0xA00000,0xA11100,0xA11200,0xC00000,0xC00004



.align 2
.global _EX_NULL
_EX_NULL:
	rte

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file commsync.s
 * @brief Gate Array comm register sync for use with _BLIB_COMM_SYNC in the
 * Boot ROM library (EXPERIMENTAL)
 * 
 * This still needs a fair amount of research...
 */

#ifndef MEGADEV__SUB_COMMSYNC_S
#define MEGADEV__SUB_COMMSYNC_S

.section .text

#include "macros.s"
#include "memmap_def.h"

FUNC comm_sync
	lea _GA_COMFLAGS+1, a0
	btst #0, _GA_COMFLAGS
	beq 2f
	bset #0, (a0)
	lea _COMCMD0, a2
	lea _GA_COMCMD0, a1
	move.l (a1)+, (a2)+
	move.l (a1)+, (a2)+
	move.l (a1)+, (a2)+
	move.l (a1)+, (a2)+
	moveq #3, d0
1:move.l (a2), (a1)+
	clr.l	(a2)+
	dbf d0, 1b
	bchg #1, (a0)
2:rts

.section .data
.global _COMCMD0: .word 0
.global _COMCMD1: .word 0
.global _COMCMD2: .word 0
.global _COMCMD3: .word 0
.global _COMCMD4: .word 0
.global _COMCMD5: .word 0
.global _COMCMD6: .word 0
.global _COMCMD7: .word 0
.global _COMSTAT0: .word 0
.global _COMSTAT1: .word 0
.global _COMSTAT2: .word 0
.global _COMSTAT3: .word 0
.global _COMSTAT4: .word 0
.global _COMSTAT5: .word 0
.global _COMSTAT6: .word 0
.global _COMSTAT7: .word 0

#endif
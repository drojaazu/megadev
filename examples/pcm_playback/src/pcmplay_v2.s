#include "macros.s"
#include "sub/memmap_def.h"

// driver as a whole will consiste of two parts
// the outer piece will take a pointer to the pcm data and a length in bytes
// it will chunk the data into 32k blocks and send to the inner piece

// the inner piece will chunk the data into 4k blocks
// loop over each 4k block and send to pcm ram

.section .text

.global PCM_PLAYBACK
PCM_PLAYBACK:
	// ptr to data in a0
	// size of data in d0
	
	// backup the size
	//move.l d0, d2
	// reset playback flags
	move.b #0, playback_flags
	// get count of 32k blocks
	divu.w #0x8000, d0
	move.w d0, block_count
	swap d0
	move.w d0, block_mod

	move.w (block_count), d7
	beq 2f
	subq #1, d7
0:move.l #0x8000, d0
	// flip the "is upper block" flag
	bchg #0, playback_flags
	btst #0, playback_flags
	beq 1f
	// set initial wb select
	move.b #0x80, (wb_select)
1:PUSH d7
	jbsr PCM_LOAD_BUFFER
	POP d7
	// start playback
	// TODO does this cause problems setting each time in the loop?
	move.b #0b11111110, _PCM_CDISABLE

	jsr PCM_WAIT
	dbf d7, 0b

2:move.w (block_mod), d0
	beq 4f
	jbsr PCM_LOAD_BUFFER

4:rts

PCM_WAIT:
	lea _PCM_PLAY_CH1_H, a2
	btst #0, playback_flags
	beq 2f
1:nop
	cmp.b #0x7e, (a2)
	bls 1b
	bra 5f
2:nop
	cmp.b #0x7e, (a2)
	bhi 2b
5:rts


/**
 * @brief Loads one 32k buffer with data
 * IN:
 *  a0.l - ptr to pcm data
 *  d0.l - size of pcm data (32kb or less)
 * OUT:
 *  a0 will point to end of loaded data
 * CLOBBERED:
 *  d6-d7, a0-a1
 */
PCM_LOAD_BUFFER:
	// ptr to data in a0
	// size of data in d0
	// 0x1000 == 4k
	// and.w d0, #0xffff
	divu.w #0x1000, d0
	move.w d0, sblock_count
	swap d0
	move.w d0, sblock_mod

	move.w (sblock_count), d7
	// if 0 sblocks, jump to check the remainder
	beq 2f
	subq #1, d7
	// TODO sonic cd op streaming has 0xffe, hmm
0:move.w #0xfff, d6
	move.b (wb_select), (_PCM_CTRL)
	lea (0xFF2000+1), a1
1:move.b (a0)+,(a1)
	addq.l #2, a1
	dbf d6, 1b
	addq.b #1, (wb_select)
	dbf d7, 0b

2:move.w (sblock_mod), d6
	beq 4f
3:move.b (a0)+,(a1)
	addq.l #2, a1
	dbf d6, 3b

4:
	rts

.section .bss
block_count: .word 0
block_mod: .word 0
sblock_count: .word 0
sblock_mod: .word 0

wb_select: .byte 0
playback_flags: .byte 0
.align 2
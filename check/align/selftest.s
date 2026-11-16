| Alignment gate (make check, check-align): proves the scanner can fail.
| align_selftest_bad holds accesses it must report, one per line marked
| "bad"; align_selftest_good holds accesses it must accept.

	.text
align_selftest_bad:
	clr.w -3(sp)              | bad: odd stack offset
	move.w #0x6162, -3(fp)    | bad: odd frame offset
	move.l (a1), (a0)         | bad: pointer base
	move.w 1(a0), d0          | bad: pointer base, odd
	move.l (a0)+, d0          | bad: post-increment through a pointer
	clr.l 0xFF0001            | bad: absolute
	move.w (8,pc), d1         | bad: PC relative
	movem.l d0-d1, (a2)       | bad: register list to a pointer
	addq.l #1, 2(a3)          | bad: read-modify-write through a pointer
	rts

align_selftest_good:
	move.l 4(sp), a0
	move.l a0, -(sp)
	move.l (sp)+, d0
	clr.w -4(fp)
	movem.l d2-d3/a2, -(sp)
	move.b (a1)+, (a0)+
	move.b 1(a0), d0
	move.w d0, d1
	moveq #0, d0
	move.l #0x12345678, d0
	lea 1(a0), a1
	pea 3(sp)
	bsr.w align_selftest_bad
	rts

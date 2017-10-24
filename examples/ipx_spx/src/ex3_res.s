
.section .rodata
#include "macros.s"

GLABEL res_bubbles_chr
.incbin "../res/bubbles.chr"
.align 2
.global res_bubbles_chr_sz
res_bubbles_chr_sz: .word . - res_bubbles_chr

GLABEL res_bubbles_pal
.byte 16 * 2
.byte 4
.incbin "../res/bubbles.pal"
.align 2

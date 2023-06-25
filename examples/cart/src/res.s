
.section .rodata
#include "macros.s"

GLABEL res_rain_chr
.incbin "../res/rain.chr"
.align 2
.global res_rain_chr_sz
res_rain_chr_sz: .word . - res_rain_chr


GLABEL res_rain_pal
.byte 32 * 1
.byte 15
.incbin "../res/rain.pal"
.align 2

.align 	2
GLABEL res_sysfont_1bpp_chr
.incbin "sysfont.chr.1bpp"
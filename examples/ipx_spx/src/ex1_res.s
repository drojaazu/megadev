
.section .rodata
#include "macros.s"

GLABEL res_rain_chr
.incbin "../res/rain.chr"
.align 2
.global res_rain_chr_sz
res_rain_chr_sz: .word . - res_rain_chr


GLABEL res_rain_pal
.byte 16 * 2
.byte 4
.incbin "../res/rain.pal"
.align 2



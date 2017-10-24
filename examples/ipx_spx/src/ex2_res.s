
.section .rodata
#include "macros.s"

GLABEL res_snow_chr
.incbin "../res/snow.chr"
.align 2
.global res_snow_chr_sz
res_snow_chr_sz: .word . - res_snow_chr

GLABEL res_snow_pal
.byte 16 * 2
.byte 4
.incbin "../res/snow.pal"
.align 2

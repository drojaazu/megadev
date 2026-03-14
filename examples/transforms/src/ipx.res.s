
.section .rodata
#include <macros.s>

GLABEL res_pal_stamps
.byte 32 * 0
.byte 15
.incbin "stamps.md.pal"
.align 2


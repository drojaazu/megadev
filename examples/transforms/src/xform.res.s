
.section .rodata
#include <macros.s>

FILE "stampmap.bin", res_stamp_map

FILE "stamp01.md.chr", res_stamp01


GLABEL res_pal_stamp01
.byte 32 * 1
.byte 15
.incbin "stamp01.pal"
.align 2





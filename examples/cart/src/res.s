
.section .rodata
#include "macros.s"

FILE "basic_font.chr", res_basic_font

FILE "megadev_letters.chr", res_letters

GLABEL res_megadev_pal
.incbin "megadev.pal"


.section .rodata
#include "macros.s"

FILE "basic_font.md.chr", res_basic_font

FILE "megadev_letters.md.chr", res_letters

GLABEL res_megadev_pal
.incbin "megadev.md.pal"

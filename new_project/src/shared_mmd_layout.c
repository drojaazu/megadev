/*
  Layout shared by the transient modules (ex1, ex2, ex3).

  Each of these modules runs from Word RAM, where it is loaded, so MMD_DEST is
  0 and the origin is offset by 0x100 to clear the MMD header. Boot ROM memory
  begins at 0xFFF700, so that is the cutoff for RAM.

  Because the layout is identical for all three, it lives in its own source
  file that each module links against. A module with a layout all its own can
  instead declare it at the top of its main C file.
*/

#include <macro.h>

GLOBAL_SYM(MMD_DEST, 0);
GLOBAL_SYM(MODULE_ROM_ORIGIN, 0x200100);
GLOBAL_SYM(MODULE_ROM_LENGTH, 0x240000 - MODULE_ROM_ORIGIN);
GLOBAL_SYM(MODULE_RAM_ORIGIN, 0xFFC000);
GLOBAL_SYM(MODULE_RAM_LENGTH, 0xFFF700 - 0xFFC000);

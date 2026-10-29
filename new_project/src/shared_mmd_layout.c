/**
 * @file shared_mmd_layout.c
 * @brief Module layout shared by ex1, ex2 and ex3
 */

#include <macro.h>

// Run in place from 2M Word RAM, with RAM in Work RAM below the Boot ROM's
// area, which begins at 0xFFF700
GLOBAL_SYM(MMD_DEST, 0);
GLOBAL_SYM(MODULE_ROM_ORIGIN, 0x200100);
GLOBAL_SYM(MODULE_ROM_LENGTH, 0x240000 - MODULE_ROM_ORIGIN);
GLOBAL_SYM(MODULE_RAM_ORIGIN, 0xFFC000);
GLOBAL_SYM(MODULE_RAM_LENGTH, 0xFFF700 - 0xFFC000);

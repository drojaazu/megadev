/**
 * @file shared_mmd_layout.c
 * @brief Module layout shared by ex1, ex2 and ex3
 */

#include <macro.h>

#include "shared.h"
// Run in place from 2M Word RAM, with variables in Work RAM clear of the
// resident IPX (Work RAM map in shared.h)
GLOBAL_SYM(MMD_DEST, 0);
GLOBAL_SYM(MODULE_ROM_ORIGIN, 0x200100);
GLOBAL_SYM(MODULE_ROM_LENGTH, 0x240000 - MODULE_ROM_ORIGIN);
GLOBAL_SYM(MODULE_RAM_ORIGIN, MODULE_VARS_ORIGIN);
GLOBAL_SYM(MODULE_RAM_LENGTH, MODULE_VARS_LENGTH);

#include "macros.s"
#include "main/main_def.h"

GLOBAL MODULE_ROM_ORIGIN _WRKRAM

GLOBAL MODULE_RAM_ORIGIN 0xFFF500

GLOBAL MODULE_ROM_LENGTH, MODULE_RAM_ORIGIN - MODULE_ROM_ORIGIN

GLOBAL MODULE_RAM_LENGTH, 0x200

GLOBAL MMD_DEST _WRKRAM

#include "macros.s"
#include "main/memmap_def.h"

// ROM will begin at the start of Work RAM, overwriting the security code/IP
GLOBAL MODULE_ROM_ORIGIN _WRKRAM

// RAM will go right after ROM
GLOBAL MODULE_RAM_ORIGIN 0xFFF500

// that gives us a litte over 61KB for ROM space...
GLOBAL MODULE_ROM_LENGTH, MODULE_RAM_ORIGIN - MODULE_ROM_ORIGIN

// ... and 512 bytes of RAM. This layout is rather arbitrary.
GLOBAL MODULE_RAM_LENGTH, 0x200

// Finally, we must specify from where the code will actually execute, which is
// to say, to where it should be copied after being put in Word RAM by the Sub
GLOBAL MMD_DEST _WRKRAM

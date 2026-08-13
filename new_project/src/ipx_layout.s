// The IPX is memory resident: it is loaded once and other modules import its
// symbols. It therefore uses the RESIDENT_* layout names so its build metadata
// cannot collide with that of the modules importing it.
#include <macro.s>
#include <main/memmap.def.h>

// ROM will begin at the start of Work RAM, overwriting the security code/IP
GLOBAL RESIDENT_ROM_ORIGIN WORK_RAM

// RAM will go right after ROM
GLOBAL RESIDENT_RAM_ORIGIN 0xFFF500

// that gives us a litte over 61KB for ROM space...
GLOBAL RESIDENT_ROM_LENGTH, RESIDENT_RAM_ORIGIN - RESIDENT_ROM_ORIGIN

// ... and 512 bytes of RAM. This layout is rather arbitrary.
GLOBAL RESIDENT_RAM_LENGTH, 0x200

// Finally, we must specify from where the code will actually execute, which is
// to say, to where it should be copied after being put in Word RAM by the Sub
GLOBAL RESIDENT_DEST WORK_RAM

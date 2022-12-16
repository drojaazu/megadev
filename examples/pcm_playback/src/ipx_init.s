
#include "macros.s"
#include "main/memmap_def.h"
#include "main/macros.s"

// putting this code in .init ensures it will be at the start of the module
.section .init

// disable interrupts while we get things copied
// be sure to re-enable them in main()!
ori #0x700,sr

// we include the MMD loader here, which will copy the entire IPX to its final
// destination in Work RAM
#include "main/mmd_exec.s"

// After the MMD loader does it's thing, it will jump to a function called 
// main. We define this in ipx.c and go from there.

.section .text
// we'll also go ahead and throw in the include of printval into .text
// here
#include "main/printval.s"
#include "main/exception.s"

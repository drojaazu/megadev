
#include "macros.s"
#include "main/memmap_def.h"
#include "main/macros.s"

// putting this code in .init ensures it will be at the start of the module
.section .init

// disable interrupts while we get things copied
// be sure to re-enable them in main()!
ori #0x700,sr

#include "main/mmd_exec.s"

.section .text

#include "main/printval.s"
#include "main/exception.s"

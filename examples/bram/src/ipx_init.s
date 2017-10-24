
#include "macros.s"
#include "main/main_def.h"
#include "main/main_macros.s"

// putting this code in .init ensures it will be at the start of the module
.section .init

// disable interrupts while we get things copied
// be sure to re-enable them in main()!
ori #0x700,sr

#include "mmd_exec.s"

.section .text

#include "printval.s"
#include "exception.s"


#include <macros.s>
#include <main/memmap.def.h>
#include <main/main.macro.s>

// putting this code in .init ensures it will be at the start of the module
.section .init

// disable interrupts while we get things copied
// **be sure to re-enable them in main()!**
ori #0x700,sr

jbsr INIT_MMD
jbsr INIT_DATA

// reset the stack for a fresh start
// then jump to main
movea.l (0), sp
jmp (a0)

#include <init_data.s>
#include <main/init_mmd.s>
#include <printval.s>
#include <main/exception.s>

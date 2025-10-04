
#include <macros.s>
#include <main/memmap.def.h>
#include <main/main.macro.s>

// putting this code in .init ensures it will be at the start of the module
.section .init

// disable interrupts while we get things copied
// **be sure to re-enable them in main()!**
ori #0x700,sr

// This is a little bit tricky, because INIT_MMD and INIT_DATA will be part
// of the IPX as calls available to later loaded modules, which means we
// need them to global symbols. When linking, those global symbols will
// be positioned in the final destination within Work RAM... but at this point, we're
// not in Work RAM yet, we're still running from Word RAM - we need to call
// INIT_MMD to get things moved! So we create a couple local labels and
// call to those local labels instead.
jbsr mmd_init
jbsr data_init

// The MMD loader returns a pointer to main() in A0
jmp (a0)

data_init:
#include <init_data.s>
mmd_init:
#include <main/mmd.macros.s>


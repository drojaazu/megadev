#include "md_vectors.s"
#include "md_header.s"
#include "md_init.s"
// exception vectors go to the crash handler (main/crash.h)
#include "main/crash_cart.s"

GLOBAL INIT_SSP 0xFFFFFC00
GLOBAL INIT_PC init_system // init_system is located in md_init.s

/*
all exception vectors/traps are set to a simple `rte` opcode (ex_null in
md_init.s) since this is a simple example. You will want to assign the
exception vectors to your own handlers in a full program.
*/
GLOBAL INT1 ex_null
GLOBAL INT3 ex_null
GLOBAL INT5 ex_null
GLOBAL INT7 ex_null
GLOBAL TRAP_0 ex_null
GLOBAL TRAP_1 ex_null
GLOBAL TRAP_2 ex_null
GLOBAL TRAP_3 ex_null
GLOBAL TRAP_4 ex_null
GLOBAL TRAP_5 ex_null
GLOBAL TRAP_6 ex_null
GLOBAL TRAP_7 ex_null
GLOBAL TRAP_8 ex_null
GLOBAL TRAP_9 ex_null
GLOBAL TRAP_A ex_null
GLOBAL TRAP_B ex_null
GLOBAL TRAP_C ex_null
GLOBAL TRAP_D ex_null
GLOBAL TRAP_E ex_null
GLOBAL TRAP_F ex_null

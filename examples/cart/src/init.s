#include "md_vectors.s"
#include "md_header.s"
#include "md_init.s"

GLOBAL INIT_SSP 0xFFFFFC00
GLOBAL INIT_PC init_system // init_system is located in md_init.s

/*
all exception vectors/traps are set to a simple `rte` opcode (ex_null in
md_init.s) sicne this is a simple example. You will want to assign the
exception vectors to your own handlers in a full program.
*/
GLOBAL ERR_BUS ex_null
GLOBAL ERR_ADDRESS ex_null
GLOBAL ERR_ILLEGAL ex_null
GLOBAL ERR_ZERODIV ex_null
GLOBAL EX_CHK ex_null
GLOBAL EX_TRAPV ex_null
GLOBAL ERR_VIOLATION ex_null
GLOBAL EXEXVEC_TRACE ex_null
GLOBAL EX_LINE_1010 ex_null
GLOBAL EX_LINE_1111 ex_null
GLOBAL EX_UNINITIALIZED ex_null
GLOBAL EX_SPURIOUS ex_null
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

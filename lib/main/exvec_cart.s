/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file exvec_cart.s
 * @brief Point a cartridge's exception vectors at the crash handler
 *
 * @details
 * Include in the project's init.s, after md_vectors.s, in place of the GLOBAL
 * lines for these vectors (main/exvec.h). The program must also assemble
 * main/exvec_main.s.
 */

#include "macro.s"

GLOBAL ERR_BUS, exvec_bus_error
GLOBAL ERR_ADDRESS, exvec_address_error
GLOBAL ERR_ILLEGAL, exvec_illegal
GLOBAL ERR_ZERODIV, exvec_zero_divide
GLOBAL EX_CHK, exvec_chk
GLOBAL EX_TRAPV, exvec_trapv
GLOBAL ERR_VIOLATION, exvec_privilege
GLOBAL EX_TRACE, exvec_trace
GLOBAL EX_LINE_1010, exvec_line_1010
GLOBAL EX_LINE_1111, exvec_line_1111
GLOBAL EX_UNINITIALIZED, exvec_uninitialized
GLOBAL EX_SPURIOUS, exvec_spurious

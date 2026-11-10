/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file crash_cart.s
 * @brief Point a cartridge's exception vectors at the crash handler
 *
 * @details
 * Include in the project's init.s, after md_vectors.s, in place of the GLOBAL
 * lines for these vectors (main/crash.h). The program must also assemble
 * main/crash_main.s. With main/monitor_cart.s included before this file, the
 * trace vector goes to the debug monitor instead.
 */

#include "macro.s"

GLOBAL ERR_BUS, exvec_bus_error
GLOBAL ERR_ADDRESS, exvec_address_error
GLOBAL ERR_ILLEGAL, exvec_illegal
GLOBAL ERR_ZERODIV, exvec_zero_divide
GLOBAL EX_CHK, exvec_chk
GLOBAL EX_TRAPV, exvec_trapv
GLOBAL ERR_VIOLATION, exvec_privilege
#ifndef MONITOR_CART
GLOBAL EX_TRACE, exvec_trace // main/monitor_cart.s takes trace when included first
#endif
GLOBAL EX_LINE_1010, exvec_line_1010
GLOBAL EX_LINE_1111, exvec_line_1111
GLOBAL EX_UNINITIALIZED, exvec_uninitialized
GLOBAL EX_SPURIOUS, exvec_spurious

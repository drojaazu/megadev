/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file monitor_cart.s
 * @brief Point a cartridge's TRAP #15 and trace vectors at the debug monitor
 *
 * @details
 * Include in the project's init.s after md_vectors.s and BEFORE
 * main/crash_cart.s, which then leaves the trace vector to the monitor. The
 * program must also assemble main/monitor_break.s. Mega CD programs call
 * monitor_install instead (main/monitor.h).
 */

#include "macro.s"

#define MONITOR_CART

GLOBAL TRAP_F, monitor_trap_entry
GLOBAL EX_TRACE, monitor_trace_entry

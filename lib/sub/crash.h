/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file crash.h
 * @brief Sub CPU crash handler
 *
 * @details
 * The Sub CPU has no display, so its handler saves the CPU state in Program
 * RAM, signals the Main CPU through COMSTAT5 to COMSTAT7 (crash.def.h) and
 * halts. The Main CPU shows the report from crash_check_sub (main/crash.h).
 * The program must assemble crash_sub.s.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "crash.def.h"

/**
 * @brief Install the crash handlers in the Sub BIOS jump table
 * @details Overwrites the address error, illegal instruction, divide by zero,
 * CHK, TRAPV, privilege, trace and line 1010/1111 entries. Call from sp_init,
 * after the Sub BIOS has built its table.
 */
void crash_install(void);

#ifdef __cplusplus
}
#endif

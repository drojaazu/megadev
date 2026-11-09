/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file crash.h
 * @brief Main CPU crash handler
 *
 * @details
 * On an exception that means the program has failed (address error, illegal
 * instruction, divide by zero and so on), the handler saves the registers,
 * moves to its own stack, and shows the CPU state on screen with the debug
 * text path (main/text.h), and over the serial port when it is enabled
 * (main/comm.h). It does not return.
 *
 * Installing it:
 * - Cartridge, including Mode 1: include main/crash_cart.s in the project's
 *   init.s in place of the GLOBAL lines for the exception vectors. The ROM
 *   vector table then points at the handlers.
 * - Mega CD: call crash_install, which writes the handlers into the system
 *   vector table at 0xFFFD00. The Boot ROM sends bus errors to its reset code,
 *   so those are not caught.
 *
 * Sub CPU crashes are caught by the Sub CPU's handler (sub/crash.h) and shown
 * by crash_check_sub, called regularly on the Main CPU.
 *
 * The report uses the system font unless CRASH_FONT is set to BIOS in the project's
 * makefile, which uses the Boot ROM's font instead (Mega CD and Mode 1).
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <crash.def.h>
#include <types.h>

/**
 * @brief CPU state saved by the crash handlers (layout in crash.def.h)
 */
typedef struct CrashState {
  uint16_t vector;      ///< Exception number (EXVEC_ID_*)
  uint16_t sr;          ///< Status register when it occurred
  uint32_t pc;          ///< Program counter pushed by the CPU
  uint32_t access_addr; ///< Bus and address errors: the faulting access
  uint16_t
    access_info; ///< Bus and address errors: R/W, I/N and function code bits
  uint16_t ir;   ///< Bus and address errors: the instruction word
  uint32_t d[8]; ///< D0 to D7
  uint32_t a[8]; ///< A0 to A6, and A7 before the exception
  uint32_t stack[CRASH_STACK_LONGS]; ///< The stack from A7 upward
} CrashState;

/**
 * @brief Install the crash handlers in the system vector table
 * @details Mega CD and Mode 1 programs whose ROM vectors lead to the system
 * vector table. Overwrites the address error, illegal instruction, divide by
 * zero, CHK, TRAPV, privilege, trace and line 1010/1111 entries.
 */
void crash_install(void);

/**
 * @brief Show a Sub CPU crash, if one has happened
 * @details Call regularly, for example once per frame. After a Sub CPU crash
 * it takes the Sub CPU bus, reads the Sub CPU's snapshot from Program RAM,
 * and shows it as the Main CPU handler does; it then does not return.
 */
void crash_check_sub(void);

/**
 * @brief Show a crash state snapshot and stop
 * @param snapshot CPU state
 * @param cpu Name shown with it, such as "MAIN" or "SUB"
 */
void crash_report(CrashState const * snapshot, char const * cpu)
  __attribute__((noreturn));

#ifdef __cplusplus
}
#endif

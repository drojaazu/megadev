/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file exvec.h
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
 * - Cartridge, including Mode 1: include main/exvec_cart.s in the project's
 *   init.s in place of the GLOBAL lines for the exception vectors. The ROM
 *   vector table then points at the handlers.
 * - Mega CD: call exvec_install, which writes the handlers into the system
 *   vector table at 0xFFFD00. The Boot ROM sends bus errors to its reset code,
 *   so those are not caught.
 *
 * Sub CPU crashes are caught by the Sub CPU's handler (sub/exvec.h) and shown
 * by exvec_check_sub, called regularly on the Main CPU.
 *
 * The report uses font8x8 unless EXVEC_FONT is set to BIOS in the project's
 * makefile, which uses the Boot ROM's font instead (Mega CD and Mode 1).
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "exvec.def.h"
#include "types.h"

/**
 * @brief CPU state saved by the crash handlers (layout in exvec.def.h)
 */
typedef struct ExvecSnapshot
{
  u16 vector;
  u16 sr;
  u32 pc;
  u32 access_addr;
  u16 access_info;
  u16 ir;
  u32 d[8];
  u32 a[8];
  u32 stack[EXVEC_STACK_LONGS];
} ExvecSnapshot;

/**
 * @brief Install the crash handlers in the system vector table
 * @details Mega CD and Mode 1 programs whose ROM vectors lead to the system
 * vector table. Overwrites the address error, illegal instruction, divide by
 * zero, CHK, TRAPV, privilege, trace and line 1010/1111 entries.
 */
void exvec_install(void);

/**
 * @brief Show a Sub CPU crash, if one has happened
 * @details Call regularly, for example once per frame. After a Sub CPU crash
 * it takes the Sub CPU bus, reads the Sub CPU's snapshot from Program RAM,
 * and shows it as the Main CPU handler does; it then does not return.
 */
void exvec_check_sub(void);

/**
 * @brief Show a snapshot and stop
 * @param snapshot CPU state
 * @param cpu Name shown with it, such as "MAIN" or "SUB"
 */
void exvec_report(ExvecSnapshot const * snapshot, char const * cpu) __attribute__((noreturn));

#ifdef __cplusplus
}
#endif

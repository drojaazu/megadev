/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file crash.def.h
 * @brief Crash handler definitions shared by both CPUs, C and assembly
 *
 * @details
 * The crash handlers (main/crash.h, sub/crash.h) save the CPU state in a
 * snapshot laid out as below. The Sub CPU cannot show it, so it signals the
 * Main CPU through its comm status registers, which crash_check_sub reads.
 */

#pragma once

/**
 * @defgroup exvec_id Exception numbers
 * @brief The 68000 vector number of each exception, as stored in a snapshot
 * @{
 */
#define EXVEC_ID_BUS_ERROR     2  ///< Bus error
#define EXVEC_ID_ADDRESS_ERROR 3  ///< Address error (odd word access)
#define EXVEC_ID_ILLEGAL       4  ///< Illegal instruction
#define EXVEC_ID_ZERO_DIVIDE   5  ///< Division by zero
#define EXVEC_ID_CHK           6  ///< CHK instruction out of bounds
#define EXVEC_ID_TRAPV         7  ///< TRAPV with overflow set
#define EXVEC_ID_PRIVILEGE     8  ///< Privileged instruction in user mode
#define EXVEC_ID_TRACE         9  ///< Trace
#define EXVEC_ID_LINE_1010     10 ///< Line 1010 emulator
#define EXVEC_ID_LINE_1111     11 ///< Line 1111 emulator
#define EXVEC_ID_UNINITIALIZED 15 ///< Uninitialized interrupt vector
#define EXVEC_ID_SPURIOUS      24 ///< Spurious interrupt
/** @} */

/**
 * @defgroup crash_state Saved registers layout in a crash
 * @brief Byte offsets into a snapshot (CrashState in main/crash.h)
 * @{
 */
#define CRASH_VECTOR      0   ///< u16: exception number
#define CRASH_SR          2   ///< u16: status register when it occurred
#define CRASH_PC          4   ///< u32: program counter pushed by the CPU
#define CRASH_ACCESS_ADDR 8   ///< u32: bus/addr errors: the access
#define CRASH_ACCESS_INFO 12  ///< u16: bus/addr errors: R/W, I/N, FC
#define CRASH_IR          14  ///< u16: bus/addr errors: instruction
#define CRASH_D           16  ///< u32[8]: D0 to D7
#define CRASH_A           48  ///< u32[8]: A0 to A6, and A7 before the exception
#define CRASH_STACK       80  ///< u32[8]: the stack from A7 upward
#define CRASH_SIZE        112 ///< Size of a snapshot in bytes
/** @} */

/**
 * @brief Number of stack longs saved in a snapshot
 */
#define CRASH_STACK_LONGS 8

/**
 * @brief Value the Sub CPU writes to COMSTAT5 after a crash ('EX')
 * @details COMSTAT6 and COMSTAT7 then hold the high and low words of the
 * snapshot's address in Program RAM, as the Sub CPU sees it.
 */
#define CRASH_SUB_SIGNAL 0x4558

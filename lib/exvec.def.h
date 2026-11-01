/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file exvec.def.h
 * @brief Crash handler definitions shared by both CPUs, C and assembly
 *
 * @details
 * The crash handlers (main/exvec.h, sub/exvec.h) save the CPU state in a
 * snapshot laid out as below. The Sub CPU cannot show it, so it signals the
 * Main CPU through its comm status registers, which exvec_check_sub reads.
 */

#pragma once

/**
 * @defgroup exvec_id Exception numbers
 * @brief The 68000 vector number of each exception, as stored in a snapshot
 * @{
 */
#define EXVEC_ID_BUS_ERROR 2
#define EXVEC_ID_ADDRESS_ERROR 3
#define EXVEC_ID_ILLEGAL 4
#define EXVEC_ID_ZERO_DIVIDE 5
#define EXVEC_ID_CHK 6
#define EXVEC_ID_TRAPV 7
#define EXVEC_ID_PRIVILEGE 8
#define EXVEC_ID_TRACE 9
#define EXVEC_ID_LINE_1010 10
#define EXVEC_ID_LINE_1111 11
#define EXVEC_ID_UNINITIALIZED 15
#define EXVEC_ID_SPURIOUS 24
/** @} */

/**
 * @defgroup exvec_snapshot Snapshot layout
 * @brief Byte offsets into a snapshot (ExvecSnapshot in main/exvec.h)
 * @{
 */
#define EXVEC_SNAP_VECTOR 0       /* u16: exception number */
#define EXVEC_SNAP_SR 2           /* u16: status register when it occurred */
#define EXVEC_SNAP_PC 4           /* u32: program counter pushed by the CPU */
#define EXVEC_SNAP_ACCESS_ADDR 8  /* u32: bus and address errors: the access */
#define EXVEC_SNAP_ACCESS_INFO 12 /* u16: bus and address errors: R/W, I/N, FC */
#define EXVEC_SNAP_IR 14          /* u16: bus and address errors: instruction */
#define EXVEC_SNAP_D 16           /* u32[8]: D0 to D7 */
#define EXVEC_SNAP_A 48           /* u32[8]: A0 to A6, and A7 before the exception */
#define EXVEC_SNAP_STACK 80       /* u32[8]: the stack from A7 upward */
#define EXVEC_SNAP_SIZE 112
/** @} */

/**
 * @brief Number of stack longs saved in a snapshot
 */
#define EXVEC_STACK_LONGS 8

/**
 * @brief Value the Sub CPU writes to COMSTAT5 after a crash ('EX')
 * @details COMSTAT6 and COMSTAT7 then hold the high and low words of the
 * snapshot's address in Program RAM, as the Sub CPU sees it.
 */
#define EXVEC_SUB_SIGNAL 0x4558

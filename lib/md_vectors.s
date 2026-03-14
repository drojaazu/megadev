/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file md_vectors.s
 * @brief Mega Drive exception vector table
 */

#include "macros.s"

.org 0

dc.l INIT_SSP          // Initial stack pointer
dc.l INIT_PC           // Initial program entry
dc.l ERR_BUS           // Bus error
dc.l ERR_ADDRESS       // Address error
dc.l ERR_ILLEGAL       // Illegal instruction
dc.l ERR_ZERODIV       // Division by zero
dc.l EX_CHK            // CHK instruction
dc.l EX_TRAPV          // TRAPV instruction
dc.l ERR_VIOLATION     // Privilege violation
dc.l EXEXVEC_TRACE          // Trace exception
dc.l EX_LINE_1010      // Line A instruction
dc.l EX_LINE_1111      // Line F instruction
dc.l EX_NULL           // Unassigned, do not use
dc.l EX_NULL           // Unassigned, do not use
dc.l EX_NULL           // Unassigned, do not use
dc.l EX_UNINITIALIZED  // Uninitialized vector
dc.l EX_NULL           // Unassigned, do not use
dc.l EX_NULL           // Unassigned, do not use
dc.l EX_NULL           // Unassigned, do not use
dc.l EX_NULL           // Unassigned, do not use
dc.l EX_NULL           // Unassigned, do not use
dc.l EX_NULL           // Unassigned, do not use
dc.l EX_NULL           // Unassigned, do not use
dc.l EX_NULL           // Unassigned, do not use
dc.l EX_SPURIOUS       // Spurious
dc.l INT1              // Interrupt Level 1
dc.l INT2_EXT          // Interrupt Level 2 (Mega Drive I/O interrupt)
dc.l INT3              // Interrupt Level 3
dc.l INT4_HBLANK       // Interrupt Level 4 (Mega Drive HBLANK interrupt)
dc.l INT5              // Interrupt Level 5
dc.l INT6_VBLANK       // Interrupt Level 6 (Mega Drive VBLANK interrupt)
dc.l INT7              // Interrupt Level 7
dc.l TRAP_0            // TRAP instruction
dc.l TRAP_1            // TRAP instruction
dc.l TRAP_2            // TRAP instruction
dc.l TRAP_3            // TRAP instruction
dc.l TRAP_4            // TRAP instruction
dc.l TRAP_5            // TRAP instruction
dc.l TRAP_6            // TRAP instruction
dc.l TRAP_7            // TRAP instruction
dc.l TRAP_8            // TRAP instruction
dc.l TRAP_9            // TRAP instruction
dc.l TRAP_A            // TRAP instruction
dc.l TRAP_B            // TRAP instruction
dc.l TRAP_C            // TRAP instruction
dc.l TRAP_D            // TRAP instruction
dc.l TRAP_E            // TRAP instruction
dc.l TRAP_F            // TRAP instruction
// Remaining vectors are unassigned, do not use
dc.l EX_NULL, EX_NULL, EX_NULL, EX_NULL, EX_NULL, EX_NULL, EX_NULL, EX_NULL
dc.l EX_NULL, EX_NULL, EX_NULL, 	EX_NULL, EX_NULL, EX_NULL, EX_NULL, EX_NULL

GLOBAL INIT_SSP 0xFFFFFC00
GLOBAL INIT_PC init_system

GLOBAL ERR_BUS EX_NULL
GLOBAL ERR_ADDRESS EX_NULL
GLOBAL ERR_ILLEGAL EX_NULL
GLOBAL ERR_ZERODIV EX_NULL
GLOBAL EX_CHK EX_NULL
GLOBAL EX_TRAPV EX_NULL
GLOBAL ERR_VIOLATION EX_NULL
GLOBAL EXEXVEC_TRACE EX_NULL
GLOBAL EX_LINE_1010 EX_NULL
GLOBAL EX_LINE_1111 EX_NULL
GLOBAL EX_UNINITIALIZED EX_NULL
GLOBAL EX_SPURIOUS EX_NULL
GLOBAL INT1 EX_NULL
GLOBAL INT3 EX_NULL
GLOBAL INT5 EX_NULL
GLOBAL INT7 EX_NULL
GLOBAL TRAP_0 EX_NULL
GLOBAL TRAP_1 EX_NULL
GLOBAL TRAP_2 EX_NULL
GLOBAL TRAP_3 EX_NULL
GLOBAL TRAP_4 EX_NULL
GLOBAL TRAP_5 EX_NULL
GLOBAL TRAP_6 EX_NULL
GLOBAL TRAP_7 EX_NULL
GLOBAL TRAP_8 EX_NULL
GLOBAL TRAP_9 EX_NULL
GLOBAL TRAP_A EX_NULL
GLOBAL TRAP_B EX_NULL
GLOBAL TRAP_C EX_NULL
GLOBAL TRAP_D EX_NULL
GLOBAL TRAP_E EX_NULL
GLOBAL TRAP_F EX_NULL

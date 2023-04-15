/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file md_vectors.s
 * @brief Mega Drive exception vector table
 */

#include "macros.s"

.org 0

dc.l _INIT_SSP                  // Initial stack pointer
dc.l _INIT_PC                   // Initial program entry
dc.l _ERR_BUS                   // Bus Error
dc.l _ERR_ADDRESS               // Address Error
dc.l _ERR_ILLEGAL_INSTRUCTION   // Illegal Instruction
dc.l _ERR_ZERODIV               // Divide by Zero
dc.l _EX_CHK                    // CHK instruction
dc.l _EX_TRAPV                  // TRAPV instruction
dc.l _ERR_VIOLATION             // Privilege Violation
dc.l _EX_TRACE                  // Trace exception
dc.l _EX_LINE_1010              // Line A instruction
dc.l _EX_LINE_1111              // Line F instruction
dc.l _EX_NULL                   // Unassigned, do not use
dc.l _EX_NULL                   // Unassigned, do not use
dc.l _EX_NULL                   // Unassigned, do not use
dc.l _EX_UNINITIALIZED          // Uninitialized vector
dc.l _EX_NULL                   // Unassigned, do not use
dc.l _EX_NULL                   // Unassigned, do not use
dc.l _EX_NULL                   // Unassigned, do not use
dc.l _EX_NULL                   // Unassigned, do not use
dc.l _EX_NULL                   // Unassigned, do not use
dc.l _EX_NULL                   // Unassigned, do not use
dc.l _EX_NULL                   // Unassigned, do not use
dc.l _EX_NULL                   // Unassigned, do not use
dc.l _EX_SPURIOUS               // Spurious
dc.l _INT1                      // Interrupt Level 1
dc.l _INT_EXT                   // Interrupt Level 2 (Mega Drive I/O interrupt)
dc.l _INT3                      // Interrupt Level 3
dc.l _INT_HBLANK                // Interrupt Level 4 (Mega Drive HBLANK interrupt)
dc.l _INT5                      // Interrupt Level 5
dc.l _INT_VBLANK                // Interrupt Level 6 (Mega Drive VBLANK interrupt)
dc.l _INT7                      // Interrupt Level 7
dc.l _TRAP_0                    // TRAP instruction
dc.l _TRAP_1                    // TRAP instruction
dc.l _TRAP_2                    // TRAP instruction
dc.l _TRAP_3                    // TRAP instruction
dc.l _TRAP_4                    // TRAP instruction
dc.l _TRAP_5                    // TRAP instruction
dc.l _TRAP_6                    // TRAP instruction
dc.l _TRAP_7                    // TRAP instruction
dc.l _TRAP_8                    // TRAP instruction
dc.l _TRAP_9                    // TRAP instruction
dc.l _TRAP_A                    // TRAP instruction
dc.l _TRAP_B                    // TRAP instruction
dc.l _TRAP_C                    // TRAP instruction
dc.l _TRAP_D                    // TRAP instruction
dc.l _TRAP_E                    // TRAP instruction
dc.l _TRAP_F                    // TRAP instruction
// Remaining vectors are unassigned, do not use
dc.l _EX_NULL, _EX_NULL, _EX_NULL, _EX_NULL, _EX_NULL, _EX_NULL, _EX_NULL, _EX_NULL
dc.l _EX_NULL, _EX_NULL, _EX_NULL, 	_EX_NULL, _EX_NULL, _EX_NULL, _EX_NULL, _EX_NULL

GLOBAL _INIT_SSP 0xfffffc00
GLOBAL _INIT_PC init

GLOBAL _ERR_BUS _EX_NULL
GLOBAL _ERR_ADDRESS _EX_NULL
GLOBAL _ERR_ILLEGAL_INSTRUCTION _EX_NULL
GLOBAL _ERR_ZERODIV _EX_NULL
GLOBAL _EX_CHK _EX_NULL
GLOBAL _EX_TRAPV _EX_NULL
GLOBAL _ERR_VIOLATION _EX_NULL
GLOBAL _EX_TRACE _EX_NULL
GLOBAL _EX_LINE_1010 _EX_NULL
GLOBAL _EX_LINE_1111 _EX_NULL
GLOBAL _EX_UNINITIALIZED _EX_NULL
GLOBAL _EX_SPURIOUS _EX_NULL
GLOBAL _INT1 _EX_NULL
GLOBAL _INT_EXT _EX_NULL
GLOBAL _INT3 _EX_NULL
GLOBAL _INT_HBLANK _EX_NULL
GLOBAL _INT5 _EX_NULL
// GLOBAL _INT_VBLANK _EX_NULL
GLOBAL _INT7 _EX_NULL
GLOBAL _TRAP_0 _EX_NULL
GLOBAL _TRAP_1 _EX_NULL
GLOBAL _TRAP_2 _EX_NULL
GLOBAL _TRAP_3 _EX_NULL
GLOBAL _TRAP_4 _EX_NULL
GLOBAL _TRAP_5 _EX_NULL
GLOBAL _TRAP_6 _EX_NULL
GLOBAL _TRAP_7 _EX_NULL
GLOBAL _TRAP_8 _EX_NULL
GLOBAL _TRAP_9 _EX_NULL
GLOBAL _TRAP_A _EX_NULL
GLOBAL _TRAP_B _EX_NULL
GLOBAL _TRAP_C _EX_NULL
GLOBAL _TRAP_D _EX_NULL
GLOBAL _TRAP_E _EX_NULL
GLOBAL _TRAP_F _EX_NULL

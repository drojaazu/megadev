/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file memmap.def.h
 * @brief Sub CPU side memory map & system vectors
 */

#pragma once

/**
 * @def PRG_RAM
 * @brief Program RAM (PRG RAM)
 *
 * @details Size: 4Mbit
 */
#define PRG_RAM 0x000000

/**
 * @def PRG_RAM_BANK1
 * @brief PRG RAM 1Mbit Bank 0
 *
 * @details PRG RAM can be accessed in 1M banks by the Main CPU.
 * These provide quick access to each bank
 */
#define PRG_RAM_BANK1 0x00000

/**
 * @def PRG_RAM_BANK2
 * @brief PRG RAM 1Mbit Bank 1
 *
 * @sa PRG_RAM_BANK1
 */
#define PRG_RAM_BANK2 0x20000

/**
 * @def PRG_RAM_BANK3
 * @brief PRG RAM 1Mbit Bank 2
 *
 * @sa PRG_RAM_BANK1
 */
#define PRG_RAM_BANK3 0x40000

/**
 * @def PRG_RAM_BANK4
 * @brief PRG RAM 1Mbit Bank 3
 *
 * @sa PRG_RAM_BANK1
 */
#define PRG_RAM_BANK4 0x60000

/**
 * @def WORD_RAM_2M
 * @brief Word RAM access (2M mode)
 */
#define WORD_RAM_2M 0x080000 /*word RAM base in 2M bit mode*/

/**
 * @def WORD_RAM_1M
 * @brief Word RAM access (1M mode)
 */
#define WORD_RAM_1M 0x0C0000 /*word RAM base in 1M bit mode*/

/**
 * @name Sub BIOS vector table
 * @brief The Sub BIOS's exception and interrupt jump table: one `jmp xxx.l` per
 * vector, 0x5F40 to 0x5FFF. Each EXVEC_* names the address operand of its
 * entry, as the Main CPU's do, so a handler is installed with
 * `move.l #handler, (EXVEC_LEVEL2)`. Vectors 3 to 11, the seven interrupt
 * levels and the sixteen TRAPs; there is no bus error entry.
 */
///@{
#define EXVEC_ADDRERR	 0x005F42 ///< Address error
#define EXVEC_ILLEGAL	 0x005F48 ///< Illegal instruction
#define EXVEC_ZERODIV	 0x005F4E ///< Division by zero
#define EXVEC_CHK			 0x005F54 ///< CHK instruction out of bounds
#define EXVEC_TRAPV		 0x005F5A ///< TRAPV with overflow set
#define EXVEC_PRIVERR	 0x005F60 ///< Privileged instruction in user mode
#define EXVEC_TRACE		 0x005F66 ///< Trace
#define EXVEC_LINE1010 0x005F6C ///< Line 1010 emulator (A-line opcode)
#define EXVEC_LINE1111 0x005F72 ///< Line 1111 emulator (F-line opcode)
#define EXVEC_LEVEL1	 0x005F78 ///< Level 1 interrupt: graphics operation complete
#define EXVEC_LEVEL2	 0x005F7E ///< Level 2 interrupt: INT2 from the Main CPU
#define EXVEC_LEVEL3	 0x005F84 ///< Level 3 interrupt: gate array timer
#define EXVEC_LEVEL4	 0x005F8A ///< Level 4 interrupt: CDD complete
#define EXVEC_LEVEL5	 0x005F90 ///< Level 5 interrupt: CDC complete
#define EXVEC_LEVEL6	 0x005F96 ///< Level 6 interrupt: sub-code buffer full
#define EXVEC_LEVEL7	 0x005F9C ///< Level 7 interrupt: unused
#define EXVEC_TRAP0		 0x005FA2 ///< TRAP #0
#define EXVEC_TRAP1		 0x005FA8 ///< TRAP #1
#define EXVEC_TRAP2		 0x005FAE ///< TRAP #2
#define EXVEC_TRAP3		 0x005FB4 ///< TRAP #3
#define EXVEC_TRAP4		 0x005FBA ///< TRAP #4
#define EXVEC_TRAP5		 0x005FC0 ///< TRAP #5
#define EXVEC_TRAP6		 0x005FC6 ///< TRAP #6
#define EXVEC_TRAP7		 0x005FCC ///< TRAP #7
#define EXVEC_TRAP8		 0x005FD2 ///< TRAP #8
#define EXVEC_TRAP9		 0x005FD8 ///< TRAP #9
#define EXVEC_TRAPA		 0x005FDE ///< TRAP #10
#define EXVEC_TRAPB		 0x005FE4 ///< TRAP #11
#define EXVEC_TRAPC		 0x005FEA ///< TRAP #12
#define EXVEC_TRAPD		 0x005FF0 ///< TRAP #13
#define EXVEC_TRAPE		 0x005FF6 ///< TRAP #14
#define EXVEC_TRAPF		 0x005FFC ///< TRAP #15
///@}

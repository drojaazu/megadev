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

/*
 * Sub BIOS exception and interrupt jump table: one `jmp xxx.l` per vector,
 * 0x5F40 to 0x5FFF. Each EXVEC_* names the address operand of its entry, as
 * the Main CPU's do, so a handler is installed with
 * move.l #handler, (EXVEC_LEVEL2). Vectors 3 to 11, the seven interrupt
 * levels and the sixteen TRAPs; there is no bus error entry.
 */
#define EXVEC_ADDRERR	 0x005F42
#define EXVEC_ILLEGAL	 0x005F48
#define EXVEC_ZERODIV	 0x005F4E
#define EXVEC_CHK			 0x005F54
#define EXVEC_TRAPV		 0x005F5A
#define EXVEC_PRIVERR	 0x005F60
#define EXVEC_TRACE		 0x005F66
#define EXVEC_LINE1010 0x005F6C
#define EXVEC_LINE1111 0x005F72
#define EXVEC_LEVEL1	 0x005F78 /* Graphics operation complete */
#define EXVEC_LEVEL2	 0x005F7E /* INT 2 from Main CPU */
#define EXVEC_LEVEL3	 0x005F84 /* GA Timer Interrupt */
#define EXVEC_LEVEL4	 0x005F8A /* CDD complete */
#define EXVEC_LEVEL5	 0x005F90 /* CDC complete */
#define EXVEC_LEVEL6	 0x005F96 /* Subcode buffer full */
#define EXVEC_LEVEL7	 0x005F9C
#define EXVEC_TRAP0		 0x005FA2
#define EXVEC_TRAP1		 0x005FA8
#define EXVEC_TRAP2		 0x005FAE
#define EXVEC_TRAP3		 0x005FB4
#define EXVEC_TRAP4		 0x005FBA
#define EXVEC_TRAP5		 0x005FC0
#define EXVEC_TRAP6		 0x005FC6
#define EXVEC_TRAP7		 0x005FCC
#define EXVEC_TRAP8		 0x005FD2
#define EXVEC_TRAP9		 0x005FD8
#define EXVEC_TRAPA		 0x005FDE
#define EXVEC_TRAPB		 0x005FE4
#define EXVEC_TRAPC		 0x005FEA
#define EXVEC_TRAPD		 0x005FF0
#define EXVEC_TRAPE		 0x005FF6
#define EXVEC_TRAPF		 0x005FFC

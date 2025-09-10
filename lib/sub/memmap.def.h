/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file memmap.def.h
 * @brief Sub CPU side memory map & system vectors
 */

#ifndef MEGADEV__SUB_MEMMAP_DEF_H
#define MEGADEV__SUB_MEMMAP_DEF_H

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

#define EXVEC_ADDRERR	 0x005F40
#define EXVEC_ILLEGAL	 0x005F46
#define EXVEC_ZERODIV	 0x005F4C
#define EXVEC_CHK			 0x005F52
#define EXVEC_TRAPV		 0x005F58
#define EXVEC_PRIVERR	 0x005F5E
#define EXVEC_TRACE		 0x005F64
#define EXVEC_LINE1010 0x005F6A
#define EXVEC_LINE1111 0x005F70
#define EXVEC_LEVEL1	 0x005F76 /* Graphics operation complete */
#define EXVEC_LEVEL2	 0x005F7C /* INT 2 from Main CPU */
#define EXVEC_LEVEL3	 0x005F82 /* GA Timer Interrupt */
#define EXVEC_LEVEL4	 0x005F88 /* CDD complete */
#define EXVEC_LEVEL5	 0x005F8E /* CDC complete */
#define EXVEC_LEVEL6	 0x005F94 /* Subcode buffer full */
#define EXVEC_LEVEL7	 0x005F9A
#define EXVEC_TRAP0		 0x005FA0
#define EXVEC_TRAP1		 0x005FA6
#define EXVEC_TRAP2		 0x005FAC
#define EXVEC_TRAP3		 0x005FB2
#define EXVEC_TRAP4		 0x005FB8
#define EXVEC_TRAP5		 0x005FBE
#define EXVEC_TRAP6		 0x005FC4
#define EXVEC_TRAP7		 0x005FCA
#define EXVEC_TRAP8		 0x005FD0
#define EXVEC_TRAP9		 0x005FD6
#define EXVEC_TRAPA		 0x005FDC
#define EXVEC_TRAPB		 0x005FE2
#define EXVEC_TRAPC		 0x005FE8
#define EXVEC_TRAPD		 0x005FEE
#define EXVEC_TRAPE		 0x005FF4
#define EXVEC_TRAPF		 0x005FFA

#endif

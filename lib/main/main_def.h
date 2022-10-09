/**
 * @file
 * @brief Hardware memory map, Gate Array (GA) register, and entry vector
 * definitions for the Main CPU side
 *
 * @note All bit definitions are byte relative and should be applied to the
 * registers cast to an 8 bit type.
 */

#ifndef MEGADEV__CD_MAIN_DEF_H
#define MEGADEV__CD_MAIN_DEF_H

#define SYSTEMUSE_BASE 0xfffd00

/**
 * Work RAM
 */
#define _WRKRAM 0xff0000

/**
 * @brief Base address of Word RAM in 2M mode
 *
 */
#define _WRDRAM 0x200000

/**
 * @brief Base address of Word RAM in 1M mode
 *
 */
#define _WRDRAM_1M_0 0x200000

/**
 * @brief Base address of VDP tiles in 1M mode
 *
 */
#define _WRDRAM_1M_1 0x220000

/**
 * @brief Bass address for Sub CPU PRG RAM 1M mapping
 */
#define _PRGRAM 0x20000

/**
 * Initial Program (IP) enntry
 */
#define _IP_ENTRY 0xff0000

/**
 * System Jump Table
 */
#define _RESET 0xfffd00
#define _MLEVEL6 0xfffd06 /* VBLANK interrupt */
#define _MLEVEL4 0xfffd0c /* HBLANK interrupt */
#define _MLEVEL2 0xfffd12 /* External port interrupt */
#define _MTRAP00 0xfffd18
#define _MTRAP01 0xfffd1e
#define _MTRAP02 0xfffd24
#define _MTRAP03 0xfffd2a
#define _MTRAP04 0xfffd30
#define _MTRAP05 0xfffd36
#define _MTRAP06 0xfffd3c
#define _MTRAP07 0xfffd42
#define _MTRAP08 0xfffd48
#define _MTRAP09 0xfffd4e
#define _MTRAP10 0xfffd54
#define _MTRAP11 0xfffd5a
#define _MTRAP12 0xfffd60
#define _MTRAP13 0xfffd66
#define _MTRAP14 0xfffd6c
#define _MTRAP15 0xfffd72
#define _MONKERR 0xfffd78 // CHK Instruction
// the duplicate addresses on the next two entries is intentional
#define _MADRERR 0xfffd7e
#define _MCODERR 0xfffd7e
#define _MDIVERR 0xfffd84 // Zero Divide
#define _MTRPERR 0xfffd8a // TRAPV Instruction
#define _MNOCOD0 0xfffd90 // Line 1010
#define _MNOCOD1 0xfffd96 // Line 1111
#define _MSPVERR 0xfffd9c // Priv. Violation
#define _MTRACE 0xfffda2	// Trace
#define _VINT_EX 0xfffda8
#define _MBURAM 0xfffdae

#endif

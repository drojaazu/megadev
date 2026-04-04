/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file memmap.def.h
 * @brief Main CPU side memory map & system vectors
 *
 * @note All bit definitions are byte relative and should be applied to the
 * registers cast to an 8 bit type.
 */

#ifndef MEGADEV__MAIN_MEMMAP_DEF_H
#define MEGADEV__MAIN_MEMMAP_DEF_H

/**
 * @note Officially marked as "not for general use" in the English language
 * documentation, but this area is used by the Main BIOS in a number of ways,
 * including palette and comm register caches.
 */
#define SYSTEMUSE_BASE 0xFFFD00

/**
 * Work RAM
 */
#define WORK_RAM 0xFF0000

/**
 * @brief Base address of Word RAM in 2M mode
 *
 */
#if TARGET == MEGACD_MODE1
#define WORD_RAM 0x600000
#else
#define WORD_RAM 0x200000
#endif

/**
 * @brief Base address of Word RAM in 1M mode
 *
 */
#if TARGET == MEGACD_MODE1
#define WORD_RAM_1M_BANK1 0x600000
#else
#define WORD_RAM_1M_BANK1 0x200000
#endif

/**
 * @brief Base address of VDP tiles in 1M mode
 *
 */
#if TARGET == MEGACD_MODE1
#define WORD_RAM_1M_BANK2 0x620000
#else
#define WORD_RAM_1M_BANK2 0x220000
#endif

/**
 * @brief Bass address for Sub CPU PRG RAM 1M mapping
 */
#if TARGET == MEGACD_MODE1
#define PRG_RAM_1M 0x420000
#else
#define PRG_RAM_1M 0x020000
#endif

/**
 * System Jump Table
 */
#define EXVEC_RESET    0xFFFD02
#define EXVEC_LEVEL6   0xFFFD08 /* VBLANK interrupt */
#define EXVEC_VBLANK   EXVEC_LEVEL6
#define EXVEC_LEVEL4   0xFFFD0E /* HBLANK interrupt */
#define EXVEC_HBLANK   EXVEC_LEVEL4
#define EXVEC_LEVEL2   0xFFFD14 /* External port interrupt */
#define EXVEC_EXT      EXVEC_LEVEL2
#define EXVEC_TRAP0    0xFFFD1A
#define EXVEC_TRAP1    0xFFFD20
#define EXVEC_TRAP2    0xFFFD26
#define EXVEC_TRAP3    0xFFFD2C
#define EXVEC_TRAP4    0xFFFD32
#define EXVEC_TRAP5    0xFFFD38
#define EXVEC_TRAP6    0xFFFD3E
#define EXVEC_TRAP7    0xFFFD44
#define EXVEC_TRAP8    0xFFFD4A
#define EXVEC_TRAP9    0xFFFD50
#define EXVEC_TRAPA    0xFFFD56
#define EXVEC_TRAPB    0xFFFD5C
#define EXVEC_TRAPC    0xFFFD62
#define EXVEC_TRAPD    0xFFFD68
#define EXVEC_TRAPE    0xFFFD6E
#define EXVEC_TRAPF    0xFFFD74
#define EXVEC_CHK      0xFFFD7A // CHK Instruction
#define EXVEC_ADDRERR  0xFFFD80 // Misaligned memory access
#define EXVEC_ILLEGAL  0xFFFD80 // Invalid Instruction
#define EXVEC_ZERODIV  0xFFFD86 // Division by zero
#define EXVEC_TRAPV    0xFFFD8C // TRAPV Instruction
#define EXVEC_LINE1010 0xFFFD92 // Line 1010 Instruction
#define EXVEC_LINE1111 0xFFFD98 // Line 1111 Instruction
#define EXVEC_PRIVERR  0xFFFD9E // Privilege Violation
#define EXVEC_TRACE    0xFFFDA4 // Trace

#endif

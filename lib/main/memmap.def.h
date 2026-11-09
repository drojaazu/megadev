/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file memmap.def.h
 * @brief Main CPU side memory map & system vectors
 *
 * @note All bit definitions are byte relative and should be applied to the
 * registers cast to an 8 bit type.
 */

#pragma once

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
 * @brief Base address of the Boot ROM as seen by the Main CPU
 * @details The Boot ROM is at the bottom of the address space when the system
 * boots from the Mega CD. When it boots from a cartridge (Mode 1) the
 * cartridge takes that place and the Boot ROM moves to 0x400000.
 */
#if TARGET == MEGACD_MODE1
#define BOOT_ROM 0x400000
#else
#define BOOT_ROM 0x000000
#endif

/**
 * @brief Size of the Boot ROM
 */
#define BOOT_ROM_SIZE 0x20000

/**
 * @brief Bass address for Sub CPU PRG RAM 1M mapping
 */
#if TARGET == MEGACD_MODE1
#define PRG_RAM_1M 0x420000
#else
#define PRG_RAM_1M 0x020000
#endif

/**
 * @name System vector table
 * @brief The Boot ROM's jump table at 0xFFFD00, one `jmp xxx.l` per vector.
 * Each EXVEC_* names the address operand of its entry, so a handler is
 * installed with `*exvec_vblank = handler` or `move.l #handler,
 * (EXVEC_LEVEL6)`.
 */
///@{
#define EXVEC_RESET  0xFFFD02     ///< Reset
#define EXVEC_LEVEL6 0xFFFD08     ///< Level 6 interrupt: VBLANK
#define EXVEC_VBLANK EXVEC_LEVEL6 ///< VBLANK interrupt (EXVEC_LEVEL6)
#define EXVEC_LEVEL4 0xFFFD0E     ///< Level 4 interrupt: HBLANK
#define EXVEC_HBLANK EXVEC_LEVEL4 ///< HBLANK interrupt (EXVEC_LEVEL4)
#define EXVEC_LEVEL2 0xFFFD14     ///< Level 2 interrupt: external port
#define EXVEC_EXT    EXVEC_LEVEL2 ///< External port interrupt (EXVEC_LEVEL2)
#define EXVEC_TRAP0  0xFFFD1A     ///< TRAP #0
#define EXVEC_TRAP1  0xFFFD20     ///< TRAP #1
#define EXVEC_TRAP2  0xFFFD26     ///< TRAP #2
#define EXVEC_TRAP3  0xFFFD2C     ///< TRAP #3
#define EXVEC_TRAP4  0xFFFD32     ///< TRAP #4
#define EXVEC_TRAP5  0xFFFD38     ///< TRAP #5
#define EXVEC_TRAP6  0xFFFD3E     ///< TRAP #6
#define EXVEC_TRAP7  0xFFFD44     ///< TRAP #7
#define EXVEC_TRAP8  0xFFFD4A     ///< TRAP #8
#define EXVEC_TRAP9  0xFFFD50     ///< TRAP #9
#define EXVEC_TRAPA  0xFFFD56     ///< TRAP #10
#define EXVEC_TRAPB  0xFFFD5C     ///< TRAP #11
#define EXVEC_TRAPC  0xFFFD62     ///< TRAP #12
#define EXVEC_TRAPD  0xFFFD68     ///< TRAP #13
#define EXVEC_TRAPE  0xFFFD6E     ///< TRAP #14
#define EXVEC_TRAPF  0xFFFD74     ///< TRAP #15
#define EXVEC_CHK    0xFFFD7A     ///< CHK instruction out of bounds
#define EXVEC_ADDRERR                                              \
  0xFFFD80 ///< Address error; shares its entry with EXVEC_ILLEGAL
#define EXVEC_ILLEGAL                                                    \
  0xFFFD80 ///< Illegal instruction; shares its entry with EXVEC_ADDRERR
#define EXVEC_ZERODIV  0xFFFD86 ///< Division by zero
#define EXVEC_TRAPV    0xFFFD8C ///< TRAPV with overflow set
#define EXVEC_LINE1010 0xFFFD92 ///< Line 1010 emulator (A-line opcode)
#define EXVEC_LINE1111 0xFFFD98 ///< Line 1111 emulator (F-line opcode)
#define EXVEC_PRIVERR  0xFFFD9E ///< Privileged instruction in user mode
#define EXVEC_TRACE    0xFFFDA4 ///< Trace
///@}

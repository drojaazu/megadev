/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file z80.def.h
 * @brief Z80 hardware definitions
 */

#ifndef MEGADEV__MAIN_Z80_DEF_H
#define MEGADEV__MAIN_Z80_DEF_H

/**
 * @defgroup z80 Main CPU / Z80
 */

/* Z80 Control */
#define _Z80_RAM 0xA00000
#define _Z80_RAM_SIZE 0x2000

#define _Z80_FM1_SEL 0xA04000
#define _Z80_FM1_DATA 0xA04001
#define _Z80_FM2_SEL 0xA04002
#define _Z80_FM2_DATA 0xA04003

#define _Z80_BANK 0xA06000

#define _Z80_PSG 0xA07F11

/**
 * @def _Z80_BUSREQ
 * @brief Z80 Bus Request
 * @ingroup z80
 */
#define _Z80_BUSREQ 0xA11100

/**
 * @def _Z80_RESET
 * @brief Z80 Reset
 * @ingroup z80
 */
#define _Z80_RESET 0xA11200

#endif

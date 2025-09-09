/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file memmap.h
 * @brief C wrappers for main CPU side hardware memory map & system vectors
 */

#ifndef MEGADEV__MAIN_MEMMAP_H
#define MEGADEV__MAIN_MEMMAP_H

#include "main/memmap.def.h"

/**
 * @sa WORD_RAM
 */
#define word_ram ((volatile void *) WORD_RAM)

/**
 * @sa PRG_RAM_1M
 */
#define prg_ram_1m ((volatile void *) PRG_RAM_1M)

/**
 * @note The system jump table gives you six bytes to work with per entry,
 * which are usually implemented as a JSR (2 bytes) and a vector (4 bytes).
 * We only provide C wrappers for the last four bytes as a pointer, since
 * there is really no reason to change the JSR opcode. If such a change is
 * necessary for some reason, it should be done in asm.
 */

#endif

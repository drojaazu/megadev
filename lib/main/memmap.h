/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file memmap.h
 * @brief C wrappers for main CPU side hardware memory map & system vectors
 */

#ifndef MEGADEV__MAIN_MEMMAP_H
#define MEGADEV__MAIN_MEMMAP_H

#include <main/memmap.def.h>

/**
 * @def word_ram
 * @sa WORD_RAM
 */
#define word_ram ((volatile void *) WORD_RAM)

/**
 * @def prg_ram_1m
 * @sa PRG_RAM_1M
 */
#define prg_ram_1m ((volatile void *) PRG_RAM_1M)

#endif

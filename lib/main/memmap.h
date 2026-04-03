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
#define word_ram ((char volatile *) WORD_RAM)

/**
 * @def prg_ram_1m
 * @sa PRG_RAM_1M
 */
#define prg_ram_1m ((char volatile *) PRG_RAM_1M)

// We have included C pointer definitions for only the most commonly
// used/useful system vectors: vblank, hblank, ext
// You can manually create pointer definitions for the other vectors
// (see memmap.def.h) if needed using the same pattern.

/**
 * @def exvec_vblank
 * @brief VBLANK system interrupt vector
 * @sa EXVEC_VBLANK
 */
#define exvec_vblank ((void(* volatile *)) EXVEC_VBLANK)

/**
 * @def exvec_vblank
 * @brief HBLANK system interrupt vector
 * @sa EXVEC_HBLANK
 */
#define exvec_hblank ((void(* volatile *)) EXVEC_HBLANK)

/**
 * @def exvec_vblank
 * @brief External port interrupt vector
 * @sa EXVEC_EXT
 */
#define exvec_ext ((void(* volatile *)) EXVEC_EXT)

#endif

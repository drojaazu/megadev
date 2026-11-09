/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file memmap.h
 * @brief C wrappers for Sub CPU memory map & system vectors
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <sub/memmap.def.h>
#include <types.h>

/**
 * @brief Program RAM as a byte pointer
 * @sa PRG_RAM
 */
#define prg_ram ((volatile char *) PRG_RAM)

/**
 * @def word_ram_2m
 * @sa WORD_RAM_2M
 */
#define word_ram_2m ((volatile char *) WORD_RAM_2M)

/**
 * @def word_ram_1m
 * @sa WORD_RAM_1M
 */
#define word_ram_1m ((volatile char *) WORD_RAM_1M)

/**
 * @name CDC DMA address register values
 * @brief Convert a destination address into the value GA_SUB_REG_DMAADDR takes:
 * the address in 8 byte units, limited to the destination's size
 */
///@{
#define DMAADDR_WORDRAM1M(addr)                     \
  (((addr) & 0x3FFF) >> 3) ///< Word RAM in 1M mode
#define DMAADDR_WORDRAM2M(addr)                     \
  (((addr) & 0x7FFF) >> 3) ///< Word RAM in 2M mode
#define DMAADDR_PCM(addr) \
  (((addr) & 0x1FFF) >>   \
   3) ///< PCM wave RAM: addr in the Sub CPU's 8 KB bank window (twice the wave RAM offset)
#define DMAADDR_PRGRAM(addr) ((addr) >> 3) ///< Program RAM
///@}

#ifdef __cplusplus
}
#endif

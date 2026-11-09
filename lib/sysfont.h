/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sysfont.h
 * @brief 8x8 ASCII font, characters 0x20 to 0x7F (lib/runtime/sysfont.c)
 *
 * @details
 * One byte per row, eight rows per character, the leftmost pixel in bit 7.
 * Load it with vdp_load_1bpp_tiles (main/text.h). Public domain; see the
 * source for its origin.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

/**
 * @brief Number of characters in sysfont, 0x20 (space) to 0x7F
 */
#define SYSFONT_CHARS 96

/**
 * @brief The font, indexed by character minus 0x20
 */
extern uint8_t const sysfont[SYSFONT_CHARS][8];

#ifdef __cplusplus
}
#endif

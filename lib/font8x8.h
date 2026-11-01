/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file font8x8.h
 * @brief 8x8 ASCII font, characters 0x20 to 0x7F (lib/runtime/font8x8.c)
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
 * @brief Number of characters in font8x8, 0x20 (space) to 0x7F
 */
#define FONT8X8_CHARS 96

/**
 * @brief The font, indexed by character minus 0x20
 */
extern u8 const font8x8[FONT8X8_CHARS][8];

#ifdef __cplusplus
}
#endif

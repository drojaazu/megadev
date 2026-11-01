/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file text.h
 * @brief Text output to a plane, without the Boot ROM
 *
 * @details
 * A minimal path for debug text that works on the Mega Drive alone, in Mode
 * 1 and in Mega CD programs, with or without the Boot ROM library:
 *
 * - vdp_load_1bpp_tiles expands a 1bpp font (font8x8, or the Boot ROM font
 *   from mode1_find_font) into VRAM in chosen colors
 * - text_print writes a NUL-terminated string at a plane position from
 *   VDP_PLANE_XY, as formatted by snprintf
 *
 * Both write to the VDP ports, so call them where nothing else, such as a
 * VBLANK handler, is using the VDP at the same time. For the Boot ROM's own
 * print routine and its 0xFF-terminated strings, see bios_print and
 * str_to_bios.
 *
 * Main CPU only.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "font8x8.h"
#include "main/vdp.h"
#include "types.h"

/**
 * @brief Expand one row of 1bpp pixels to a row of a 4bpp tile
 * @param bits Eight pixels, the leftmost in bit 7
 * @param fg Palette index (0 to 15) for set pixels
 * @param bg Palette index (0 to 15) for clear pixels
 * @return The row as written to VRAM, the leftmost pixel in the top nibble
 */
u32 text_expand_row(u8 bits, u8 fg, u8 bg);

/**
 * @brief Load 1bpp 8x8 tiles into VRAM as 4bpp tiles
 * @param src Tile data, eight bytes per tile, the leftmost pixel in bit 7
 * @param count Number of tiles
 * @param tile_index First tile to write in VRAM
 * @param fg Palette index (0 to 15) for set pixels
 * @param bg Palette index (0 to 15) for clear pixels
 *
 * @code
 * vdp_load_1bpp_tiles(font8x8, FONT8X8_CHARS, 0x20, 1, 0);
 * @endcode
 */
void vdp_load_1bpp_tiles(void const * src, u16 count, u16 tile_index, u8 fg, u8 bg);

/**
 * @brief Write a NUL-terminated string to a plane
 * @param string Text; characters outside 0x20 to 0x7F print as spaces
 * @param pos Position of the first character (VDP_PLANE_XY)
 * @param tile_base Nametable entry of the space character: its tile index,
 * plus palette and priority bits, which apply to every character
 *
 * @code
 * char buf[32];
 * snprintf(buf, sizeof buf, "Score %u", score);
 * text_print(buf, VDP_PLANE_XY(0xC000, 1, 1, Width64), 0x20);
 * @endcode
 */
void text_print(char const * string, vdp_cmd pos, u16 tile_base);

#ifdef __cplusplus
}
#endif

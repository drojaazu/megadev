/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/text_expand.c
 * @brief 1bpp to 4bpp row expansion for vdp_load_1bpp_tiles (main/text.h)
 *
 * @details
 * Kept apart from the VDP code so it can be tested on the build host.
 */

#include <types.h>

uint32_t text_expand_row(uint8_t bits, uint8_t fg, uint8_t bg)
{
  uint32_t row = 0;
  for (uint8_t i = 0; i < 8; ++i) {
    row <<= 4;
    row |= (bits & 0x80) ? (fg & 0xF) : (bg & 0xF);
    bits <<= 1;
  }
  return row;
}

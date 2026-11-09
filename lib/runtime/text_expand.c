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

u32 text_expand_row(u8 bits, u8 fg, u8 bg)
{
  u32 row = 0;
  for (u8 i = 0; i < 8; ++i) {
    row <<= 4;
    row |= (bits & 0x80) ? (fg & 0xF) : (bg & 0xF);
    bits <<= 1;
  }
  return row;
}

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/text.c
 * @brief Text output to a plane, without the Boot ROM (main/text.h)
 */

#include <main/text.h>

void vdp_load_1bpp_tiles(void const * src, u16 count, u16 tile_index, u8 fg, u8 bg)
{
  u8 const * bits = src;
  vdp_ctrl_32 = to_vdp_addr(tile_offset(tile_index)) | VRAM_W;
  for (u32 n = (u32) count * 8; n; --n)
    vdp_data_32 = text_expand_row(*bits++, fg, bg);
}

void text_print(char const * string, vdp_cmd pos, u16 tile_base)
{
  vdp_ctrl_32 = pos;
  for (; *string; ++string)
  {
    u8 const c = (u8) *string;
    vdp_data = tile_base + ((c >= 0x20 && c < 0x80) ? c - 0x20 : 0);
  }
}

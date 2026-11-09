/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/text.c
 * @brief Text output to a plane, without the Boot ROM (main/text.h)
 */

#include <main/text.h>

void vdp_load_1bpp_tiles(
  void const * src, uint16_t count, uint16_t tile_index, uint8_t fg, uint8_t bg)
{
  uint8_t const * bits = src;
  vdp_ctrl_32 = to_vdp_addr(tile_offset(tile_index)) | VRAM_W;
  for (uint32_t n = (uint32_t) count * 8; n; --n)
    vdp_data_32 = text_expand_row(*bits++, fg, bg);
}

void text_print(char const * string, vdp_cmd pos, uint16_t tile_base)
{
  vdp_ctrl_32 = pos;
  for (; *string; ++string) {
    uint8_t const c = (uint8_t) *string;
    vdp_data = tile_base + ((c >= 0x20 && c < 0x80) ? c - 0x20 : 0);
  }
}

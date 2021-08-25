/**
 * \file vdp.h
 * \brief C wrappers for VDP usage
 */

#ifndef MEGADEV__VDP_H
#define MEGADEV__VDP_H

#include "types.h"
#include "vdp_def.h"

typedef struct Sprite {
  u16 : 6;
  u16 pos_y : 10;
  u8 : 4;
  u8 width : 2;
  u8 height : 2;
  u8 : 1;
  u8 next : 7;
  u8 priority : 1;
  u8 palette : 2;
  u8 v_flip : 1;
  u8 h_flip : 1;
  u16 tile : 11;
  u8 : 7;
  u16 pos_x : 9;
} Sprite;

typedef union SpriteEx {
  u32 as_u32[2];
  u16 as_u16[4];
  u8 as_u8[8];
  Sprite as_struct;
} SpriteEx;

/**
 * \def VDP_CTRL_32
 * \brief VDP control port (32 bit)
 */
#define VDP_CTRL_32 ((volatile u32 *)_VDP_CTRL)

/**
 * \def VDP_DATA_32
 * \brief VDP data port (32 bit)
 * \note A 32 bit write to this port is the equivalent to two consecutive
 * 16 bit writes
 */
#define VDP_DATA_32 ((volatile u32 *)_VDP_DATA)

/**
 * \def VDP_CTRL_16
 * \brief VDP control port (16 bit)
 */
#define VDP_CTRL_16 ((volatile u16 *)_VDP_CTRL)

/**
 * \def VDP_DATA_16
 * \brief VDP data port (16 bit)
 */
#define VDP_DATA_16 ((volatile u16 *)_VDP_DATA)

/**
 * \def NMT_POS
 * \brief Generates the nametable offset for a tile at pos x/y
 * \param x horizontal position in the tilemap
 * \param y vertical position in the tilemap
 */
#define NMT_POS(x, y) ((y * (*PLANE_WIDTH)) + (x << 1))

/**
 * \def NMT_POS_PLANE
 * \brief Generates the address of a nametable tile at pos x/y
 */
#define NMT_POS_PLANE(x, y, plane_addr) (NMT_POS(x, y) + plane_addr)

/**
 * \def AT_TILE
 * \brief Byte offset to the specified tile index
 * (assuming tile data begins at 0 in VRAM)
 */
#define AT_TILE(tile_idx) ((tile_idx) << 5)

/**
 * \def TILE_AT
 * \brief Tile index of the specified byte offset
 * (assuming tile data begins at 0 in VRAM)
 */
#define TILE_AT(offset) ((offset) >> 5)

/**
 * \def VDPPTR
 * \brief Converts a 16 bit VRAM address into VDP format at compile time if
 * possible
 */
#define VDPPTR(addr)                                                           \
  (__builtin_constant_p(addr)                                                  \
       ? (unsigned)((((addr)&0x3FFF) << 16) + (((addr)&0xC000) >> 14))         \
       : to_vdpptr(addr))

/**
 * \fn to_vdpptr
 * \brief Converts a 16 bit VRAM address into VDP format at runtime
 */
static inline u32 to_vdpptr(u16 addr) {
  u32 vdpptr = (u32)addr;
  asm(R"(
		lsl.l #2, %0
		lsr.w #2, %0
		swap %0
	)"
      : "+d"(vdpptr)
      :
      : "cc");

  return vdpptr;
}

/**
 * \fn vdpptr_to
 * \brief Converts a VDP format address to a 16 bit VRAM address at runtime
 */
static inline u16 vdpptr_to(u32 vdpptr) {
  u32 out = vdpptr;

  asm(R"(
		#andi.l #0x3fff000c, %0
		ror.w #2, %0
		lsr.l #8, %0
		lsr.l #6, %0
		ror.w #2, %0
	)"
      : "+d"(out)
      :
      : "cc");

  return (u16)out;
}
#endif

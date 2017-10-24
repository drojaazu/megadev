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
	u8 size : 4;
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

/**
 * \def VDP_CTRL_32
 * \brief VDP control port (32 bit)
 */
#define VDP_CTRL_32 ((volatile u32*)_VDP_CTRL)

/**
 * \def VDP_DATA_32
 * \brief VDP data port (32 bit)
 * \note A 32 bit write to this port is the equivalent to two consecutive
 * 16 bit writes
 */
#define VDP_DATA_32 ((volatile u32*)_VDP_DATA)

/**
 * \def VDP_CTRL_16
 * \brief VDP control port (16 bit)
 */
#define VDP_CTRL_16 ((volatile u16*)_VDP_CTRL)

/**
 * \def VDP_DATA_16
 * \brief VDP data port (16 bit)
 */
#define VDP_DATA_16 ((volatile u16*)_VDP_DATA)

/**
 * \def NMT_POS
 * \brief Generates the nametable offset for a tile at pos x/y
 * \param x horizontal position in the tilemap
 * \param y vertical position in the tilemap
 * \param width width of the tilemap (32/64/128)
 */
#define NMT_POS(x, y, width) (((y * width) + x) * 2)

/**
 * \def NMT_POS_PLANE
 * \brief Generates the address of a nametable tile at pos x/y
 */
#define NMT_POS_PLANE(x, y, width, plane_addr) \
  (NMT_POS(x, y, width) + plane_addr)

/**
 * \def AT_TILE
 * \brief Byte offset to the specified tile index
 * (assuming tile data begins at 0 in VRAM)
 */
#define AT_TILE(tile_idx) (tile_idx << 5)

/**
 * \def VDPPTR
 * \brief Converts a 16 bit VRAM address into VDP format at compile time if
 * possible
 */
#define VDPPTR(addr) \
  (__builtin_constant_p(addr) ? \
  (((addr & 0x3FFF) << 16) + ((addr & 0xC000) >> 14)) : to_vdpptr(addr))

/**
 * \fn to_vdpptr
 * \brief Converts a 16 bit VRAM address into VDP format at runtime
 */
static inline u32 to_vdpptr(u16 addr) {
  u32 vdpptr = (u32)addr;

  // The casting above should take care of and'ing the lower word
  asm(R"(
  #andi.l #0xffff, %0
  lsl.l #2, %0
  lsr.w #2, %0
  swap %0
  )" : "+d"(vdpptr));

  return vdpptr;
}

#endif

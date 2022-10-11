/**
 * @file
 * @brief C wrappers for VDP usage
 */

#ifndef MEGADEV__VDP_H
#define MEGADEV__VDP_H

#include "types.h"
#include "vdp_def.h"

/**
 * @brief Semantic typedef for a VRAM address and VDP operation, formatted for
 * use on the VDP Control port (32-bit)
 *
 */
typedef u32 VDPPTR;

/**
 * @brief Semantic typedef for a VDP register and its value, formatted for use
 * on the VDP Control port (16-bit)
 *
 */
typedef u16 VDPREG;

typedef struct Sprite
{
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

typedef union SpriteEx
{
	u32 as_u32[2];
	u16 as_u16[4];
	u8 as_u8[8];
	s32 as_s32[2];
	s16 as_s16[4];
	s8 as_s8[8];
	Sprite as_struct;
} SpriteEx;

/**
 * @def VDP_CTRL_32
 * @brief VDP control port (32 bit)
 */
#define VDP_CTRL_32 (*((u32 volatile *) _VDP_CTRL))

/**
 * @def VDP_DATA_32
 * @brief VDP data port (32 bit)
 * @note A 32 bit write to this port is the equivalent to two consecutive
 * 16 bit writes
 */
#define VDP_DATA_32 (*((u32 volatile *) _VDP_DATA))

/**
 * @def VDP_CTRL_16
 * @brief VDP control port (16 bit)
 */
#define VDP_CTRL_16 (*((u16 volatile *) _VDP_CTRL))

/**
 * @def VDP_DATA_16
 * @brief VDP data port (16 bit)
 */
#define VDP_DATA_16 (*((u16 volatile *) _VDP_DATA))

/**
 * @def NMT_POS
 * @brief Generates the nametable offset for a tile at pos x/y
 * @param x horizontal position in the tilemap
 * @param y vertical position in the tilemap
 */
#define NMT_POS(x, y) ((y * (BLIB_PLANE_WIDTH)) + (x << 1))

#define NMT_POS2(x, y, width) (y * width + (x << 1))

/**
 * @def NMT_POS_PLANE
 * @brief Generates the address of a nametable tile at pos x/y
 */
#define NMT_POS_PLANE(x, y, plane_addr) (NMT_POS (x, y) + plane_addr)

#define NMT_POS_PLANE2(x, y, width, plane_addr) \
	(NMT_POS2 (x, y, width) + plane_addr)

/**
 * @def VRAM_AT
 * @brief VRAM address for the given tile index
 * (assuming tile data begins at 0 in VRAM)
 */
#define VRAM_AT(chridx) ((chridx) << 5)

/**
 * @def TILE_AT
 * @brief Tile index of the specified VRAM address
 * (assuming tile data begins at 0 in VRAM)
 */
#define TILE_AT(vram_addr) ((vram_addr) >> 5)

/**
 * @def VDPPTR
 * @brief Converts a 16 bit VRAM address into VDP format at compile time if
 * possible
 */
#define VDPPTR(addr) \
	(__builtin_constant_p (addr) \
			? (unsigned) ((((addr) &0x3FFF) << 16) + (((addr) &0xC000) >> 14)) \
			: to_vdpptr (addr))

/**
 * @fn to_vdpptr
 * @brief Converts a 16 bit VRAM address into VDP format at runtime
 */
static inline u32 to_vdpptr (u16 addr)
{
	u32 vdpptr = (u32) addr;
	asm(
		"\
		lsl.l #2, %0 \n \
		lsr.w #2, %0 \n \
		swap %0 \n \
		"
		: "+d"(vdpptr)
		:
		: "cc");

	return vdpptr;
}

/**
 * @fn vdpptr_to
 * @brief Converts a VDP format address to a 16 bit VRAM address at runtime
 */
static inline u16 vdpptr_to (u32 vdpptr)
{
	u32 out = vdpptr;

	asm(
		"\
		#andi.l #0x3fff000c, %0 \
		ror.w #2, %0 \
		lsr.l #8, %0 \
		lsr.l #6, %0 \
		ror.w #2, %0 \
		"
		: "+d"(out)
		:
		: "cc");

	return (u16) out;
}
#endif

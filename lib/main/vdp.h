/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file vdp.h
 * @brief C wrappers for VDP usage
 */

#ifndef MEGADEV__MAIN_VDP_H
#define MEGADEV__MAIN_VDP_H

#include "main/vdp.def.h"
#include "types.h"

/**
 * @typedef VDP_ADDR
 * @brief Semantic typedef for a VRAM address *without* VDP operation, formatted for
 * use on the VDP Control port (32-bit).
 *
 */
typedef u32 VDP_ADDR;

/**
 * @typedef VDP_COMMAND
 * @brief Semantic typedef for a VRAM address and VDP operation, formatted for
 * use on the VDP Control port (32-bit).
 *
 */
typedef u32 VDP_COMMAND;

/**
 * @typedef VDP_REGISTER
 * @brief Semantic typedef for a VDP register and its value, formatted for use
 * on the VDP Control port (16-bit).
 *
 */
typedef u16 VDP_REGISTER;

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
 * @def VDP_CTRL_16
 * @brief VDP Control Port
 * @ingroup vdp_port
 * @param[read] Returns VDP status
 * @param[write] Set VDP register value / Set partial VDP I/O address
 *
 * @sa _VDP_CTRL
 */
#define VDP_CTRL_16 (*((u16 volatile *) _VDP_CTRL))
#define VDP_CTRL VDP_CTRL_16

/**
 * @def VDP_CTRL_32
 * @brief VDP Control Port (32 bit Write)
 * @ingroup vdp_port
 * @param[write] Set VDP register values / Set VDP I/O address
 *
 * @sa _VDP_CTRL
 */
#define VDP_CTRL_32 (*((u32 volatile *) _VDP_CTRL))

/**
 * @def VDP_DATA_16
 * @brief VDP Data Port
 * @ingroup vdp_port
 * @param[read] Data read
 * @param[write] Data write
 *
 * @sa _VDP_DATA
 */
#define VDP_DATA_16 (*((u16 volatile *) _VDP_DATA))
#define VDP_DATA VDP_DATA_16

/**
 * @def VDP_DATA_32
 * @brief VDP Data Port (32 bit Write)
 * @ingroup vdp_port
 * @param[write] Data write
 *
 * @sa _VDP_DATA
 */
#define VDP_DATA_32 (*((u32 volatile *) _VDP_DATA))

/**
 * @def VDP_HVCOUNTER
 * @brief Reports the current position of the electron beam on the screen
 * @ingroup vdp
 * @param[read]
 * \n Upper byte: Vertical position
 * \n Lower byte: Horizontal position
 *
 * @sa _VDP_HVCOUNTER
 */
#define VDP_HVCOUNTER (*((u16 volatile *) _VDP_HVCOUNTER))

/**
 * @enum PlaneWidthTiles
 * @brief Helper for working with plane widths specified in tiles by specifying width in bytes
 */
typedef enum PlaneWidthTiles
{
	Width32 = 64,
	Width64 = 128,
	Width128 = 256
} PlaneWidthTiles;

/**
 * @def NMT_POS
 * @brief Generates the nametable offset for a tile at pos x/y, determining
 * the plane width dynamically (calculated at runtime)
 * @param x horizontal position in the tilemap
 * @param y vertical position in the tilemap
 */
#define NMT_POS(x, y) ((y * (BLIB_PLANE_WIDTH)) + (x << 1))

/**
 * @def NMT_POS_32
 * @brief Generates the nametable offset for a tile at pos x/y for a plane set
 * to width 32 (calculated at compile time)
 * @param x horizontal position in the tilemap
 * @param y vertical position in the tilemap
 */
#define NMT_POS_32(x, y) ((y) *Width32 + ((x) << 1))

/**
 * @def NMT_POS_64
 * @brief Generates the nametable offset for a tile at pos x/y for a plane set
 * to width 64 (calculated at compile time)
 * @param x horizontal position in the tilemap
 * @param y vertical position in the tilemap
 */
#define NMT_POS_64(x, y) ((y) *Width64 + ((x) << 1))

/**
 * @def NMT_POS_128
 * @brief Generates the nametable offset for a tile at pos x/y for a plane set
 * to width 128 (calculated at compile time)
 * @param x horizontal position in the tilemap
 * @param y vertical position in the tilemap
 */
#define NMT_POS_128(x, y) ((y) *Width128 + ((x) << 1))

/**
 * @def NMT_POS_PLANE
 * @brief Generates the address of a tile at pos x/y, determining
 * the plane width dynamically (calculated at runtime)
 * @param x horizontal position in the tilemap
 * @param y vertical position in the tilemap
 * @param plane_addr VRAM address of plane
 */
#define NMT_POS_PLANE(x, y, plane_addr) (NMT_POS(x, y) + (plane_addr))

/**
 * @def NMT_POS_PLANE
 * @brief Generates the address of a tile at pos x/y for a plane set
 * to width 32 (calculated at compile time)
 * @param x horizontal position in the tilemap
 * @param y vertical position in the tilemap
 * @param plane_addr VRAM address of plane
 */

#define NMT_POS_PLANE_32(x, y, plane_addr) (NMT_POS_32(x, y) + (plane_addr))
/**
 * @def NMT_POS_PLANE_64
 * @brief Generates the address of a tile at pos x/y for a plane set
 * to width 64 (calculated at compile time)
 * @param x horizontal position in the tilemap
 * @param y vertical position in the tilemap
 * @param plane_addr VRAM address of plane
 */
#define NMT_POS_PLANE_64(x, y, plane_addr) (NMT_POS_64(x, y) + (plane_addr))

/**
 * @def NMT_POS_PLANE_128
 * @brief Generates the address of a tile at pos x/y for a plane set
 * to width 128 (calculated at compile time)
 * @param x horizontal position in the tilemap
 * @param y vertical position in the tilemap
 * @param plane_addr VRAM address of plane
 */
#define NMT_POS_PLANE_128(x, y, plane_addr) (NMT_POS_128(x, y) + (plane_addr))

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
 * @def VDP_COMMAND
 * @brief Converts a 16 bit VRAM address into VDP format at compile time if
 * possible
 */
#define VDPPTR(addr) \
	(__builtin_constant_p(addr) ? (unsigned) ((((addr) &0x3FFF) << 16) + (((addr) &0xC000) >> 14)) : to_vdpptr(addr))

/**
 * @fn to_vdpptr
 * @brief Converts a 16 bit VRAM address into VDP format at runtime
 */
static inline VDP_ADDR to_vdpptr(u16 addr)
{
	u32 vdpptr = (u32) addr;
	__asm__(
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
static inline u16 vdpptr_to(VDP_ADDR vdp_addr)
{
	u32 out = vdp_addr;

	__asm__(
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


static inline u8 vdp_dma_transfer(u8 const * source, VDP_COMMAND dest, u16 const length)
{
	register u32 D0 __asm__("d0") = (u32) dest;
	register u32 D1 __asm__("d1") = (u32) source;
	register u16 D2 __asm__("d2") = (u16) length;
	
	__asm__(
		"\
		lea      (%c0).l, A6 \n \
		asr.l    #0x1, d1 \n \
		move.l   #0x940000, d3 \n \
		move.w   d2, d3 \n \
		lsl.l    #0x8, d3 \n \
		move.w   #0x9300, d3 \n \
		move.b   d2, d3 \n \
		move.l   d3,(A6) \n \
		move.l   #0x960000, d3 \n \
		move.w   d1, d3 \n \
		lsl.l    #0x8, d3 \n \
		move.w   #0x9500, d3 \n \
		move.b   d1, d3 \n \
		move.l   d3, (A6) \n \
		swap     d1 \n \
		move.w   #0x9700, d3 \n \
		move.b   d1, d3 \n \
		move.w   d3,(A6) \n \
		ori.l    #0x40000080, d0 \n \
		swap     d0 \n \
		move.w   d0, (A6) \n \
		swap     d0 \n \
		move.w   d0, -(SP) \n \
		move.w   (SP)+, (A6) \n \
		"
		: 
		: "i"(_VDP_CTRL), "d"(D0), "d"(D1), "d"(D2)
		: "a6");
	
	return D0;
}

#endif

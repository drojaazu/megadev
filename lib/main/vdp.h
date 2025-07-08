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
 * @typedef VDP_ADDRESS
 * @brief Semantic typedef for a VRAM address *without* VDP operation, formatted for
 * use on the VDP Control port (32-bit).
 *
 */
typedef u32 VDP_ADDRESS;

/**
 * @typedef VDP_COMMAND
 * @brief Semantic typedef for a VRAM address *with* VDP operation, formatted for
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
 * @enum PlaneWidth
 * @brief Helper for working with plane widths specified in tiles by specifying width in bytes
 * @note Use with NMT_POS helper below
 */
typedef enum PlaneWidth
{
	Width32 = 64,
	Width64 = 128,
	Width128 = 256
} PlaneWidth;

/**
 * @def NMT_POS
 * @brief Generates the nametable offset for a tile at pos x/y for a given plane width
 * @param x horizontal position in the tilemap
 * @param y vertical position in the tilemap
 * @param width width of the the plane (in bytes, so tile width * 2)
 */
#define NMT_POS(x, y, width) ((y * (width)) + (x << 1))

/**
 * @fn to_vdpptr
 * @brief Converts a 16 bit VRAM address into VDP format at runtime
 */
static inline VDP_ADDRESS to_vdpptr(u16 addr)
{
	u32 vdpptr = (u32) addr;
	asm(
		"\
		lsl.l #2, %0 \n\
		lsr.w #2, %0 \n\
		swap %0 \n\
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
static inline u16 vdpptr_to(VDP_ADDRESS vdp_addr)
{
	u32 out = vdp_addr;

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


/**
 * @def vdp_dma_transfer
 * @ingroup vdp
 * @warning Setting/clearing the DMA Enable bit on VDP Mode Register 2 is the responsibility of the user
 */
static inline void vdp_dma_transfer(u8 const * source, VDP_COMMAND dest, u16 const length)
{
	register u32 scratch_d, scratch_a;

	asm volatile(
		"\
  lea      (%c[vdp_ctrl]).l, %[scratch_a] \n\
  asr.l    #0x1, %[source] \n\
  move.l   #0x940000, %[scratch_d] \n\
  move.w   %[length], %[scratch_d] \n\
  lsl.l    #0x8, %[scratch_d] \n\
  move.w   #0x9300, %[scratch_d] \n\
  move.b   %[length], %[scratch_d] \n\
  move.l   %[scratch_d], (%[scratch_a]) \n\
  move.l   #0x960000, %[scratch_d] \n\
  move.w   %[source], %[scratch_d] \n\
  lsl.l    #0x8, %[scratch_d] \n\
  move.w   #0x9500, %[scratch_d] \n\
  move.b   %[source], %[scratch_d] \n\
  move.l   %[scratch_d], (%[scratch_a]) \n\
  swap     %[source] \n\
  move.w   #0x9700, %[scratch_d] \n\
  move.b   %[source], %[scratch_d] \n\
  move.w   %[scratch_d], (%[scratch_a]) \n\
  ori.l    #0x40000080, %[dest] \n\
  swap     %[dest] \n\
  move.w   %[dest], (%[scratch_a]) \n\
  swap     %[dest] \n\
  move.w   %[dest], -(SP) \n\
  move.w   (SP)+, (%[scratch_a]) \n\
		"
		: 
			[scratch_d] "=&d"(scratch_d),
			[scratch_a] "=&a"(scratch_a)
		:
			[vdp_ctrl] "i"(_VDP_CTRL),
			[dest] "d"(dest),
			[source] "d"(source),
			[length] "d"(length)
	);
}

static inline void vdp_dma_fill(VDP_COMMAND dest, u16 const count, u8 const value)
{
	u32 scratch_d = 0, scratch_a = 0;
	
	asm(
		"\
		lea      (%c0).l, %[scratch_a] \n\
		move.l   #0x00940000, %[scratch_d] \n\
		move.w   %[count].w, %[scratch_d].w \n\
		lsl.l    #0x8, %[scratch_d] \n\
		move.w   #0x9300, %[scratch_d].w \n\
		move.b   %[count].b, %[scratch_d].b \n\
		move.l   %[scratch_d], (%[scratch_a]) \n\
		move.w   #0x9780, (%[scratch_a]) \n\
		ori.l    #0x40000080, %[dest] \n\
		move.l   %[dest], (%[scratch_a]) \n\
		move.b   %[value].b, (-0x4,%[scratch_a]) \n\
	0:move.w   (%[scratch_a]), %[scratch_d].w \n\
		btst.l   0x1, %[scratch_d] \n\
		bne.b    0b \n\
		"
		: 
		:
			"i"(_VDP_CTRL),
			[dest] "d"(dest),
			[count] "d"(count),
			[value] "d"(value),
			[scratch_d] "d"(scratch_d),
			[scratch_a] "a"(scratch_a)
		:
	);
}


#endif

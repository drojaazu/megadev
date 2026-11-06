/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file vdp.h
 * @brief C wrappers for VDP usage
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "main/vdp.def.h"
#include "types.h"

/**
 * @typedef vram_addr
 * @brief Semantic typedef for a VRAM address *not* in VDP Control Port format
 *
 */
typedef u16 vram_addr;

/**
 * @typedef vdp_addr
 * @brief Semantic typedef for a VRAM address formatted for use on the VDP
 *        Control port (32-bit).
 *
 */
typedef u32 vdp_addr;

/**
 * @typedef vdp_cmd
 * @brief Semantic typedef for a VRAM address *with VDP operation*, formatted
 *        for use on the VDP Control port (32-bit).
 *
 */
typedef u32 vdp_cmd;

/**
 * @typedef vdp_reg
 * @brief Semantic typedef for a VDP register and its value, formatted for
 *        use on the VDP Control port (16-bit).
 *
 */
typedef u16 vdp_reg;

/**
 * @brief One entry of the VDP sprite attribute table
 */
typedef struct Sprite
{
  s16          : 6; ///< Unused
  s16 pos_y    : 10; ///< Vertical position, 128 is the top of the screen
  s16          : 4; ///< Unused
  s16 width    : 2; ///< Width in tiles minus 1
  s16 height   : 2; ///< Height in tiles minus 1
  s16          : 1; ///< Unused
  s16 next     : 7; ///< Index of the next sprite in the display list, 0 ends it
  s16 priority : 1; ///< Priority over the planes
  s16 palette  : 2; ///< Palette line
  s16 v_flip   : 1; ///< Vertical flip
  s16 h_flip   : 1; ///< Horizontal flip
  s16 tile     : 11; ///< First tile index
  s16          : 7; ///< Unused
  s16 pos_x    : 9; ///< Horizontal position, 128 is the left edge of the screen
} Sprite;

/**
 * @brief A sprite table entry with word, byte and long views of its 8 bytes
 */
typedef union SpriteEx
{
  u32    as_u32[2]; ///< Two unsigned longs
  u16    as_u16[4]; ///< Four unsigned words
  u8     as_u8[8]; ///< Eight unsigned bytes
  s32    as_s32[2]; ///< Two signed longs
  s16    as_s16[4]; ///< Four signed words
  s8     as_s8[8]; ///< Eight signed bytes
  Sprite as_struct; ///< The fields
} SpriteEx;

/**
 * @def vdp_ctrl_16
 * @brief VDP Control Port
 * @ingroup vdp_port
 * @param[read] Returns VDP status
 * @param[write] Set VDP register value / Set partial VDP I/O address
 *
 * @sa VDP_CTRL
 */
#define vdp_ctrl_16 (*(u16 volatile *) VDP_CTRL)

/**
 * @def vdp_ctrl
 * @brief VDP Control Port
 * @ingroup vdp_port
 * @aliasof vdp_ctrl_16
 */
#define vdp_ctrl vdp_ctrl_16

/**
 * @def vdp_ctrl_32
 * @brief VDP Control Port (32 bit Write)
 * @ingroup vdp_port
 * @param[write] Set VDP register values / Set VDP I/O address
 *
 * @sa VDP_CTRL
 */
#define vdp_ctrl_32 (*((u32 volatile *) VDP_CTRL))

/**
 * @def vdp_data_16
 * @brief VDP Data Port
 * @ingroup vdp_port
 * @param[read] Data read
 * @param[write] Data write
 *
 * @sa VDP_DATA
 */
#define vdp_data_16 (*((u16 volatile *) VDP_DATA))

/**
 * @def vdp_data
 * @brief VDP Data Port
 * @ingroup vdp_port
 * @aliasof vdp_data_16
 */
#define vdp_data vdp_data_16

/**
 * @def vdp_data_32
 * @brief VDP Data Port (32 bit Write)
 * @ingroup vdp_port
 * @param[write] Data write
 *
 * @sa VDP_DATA
 */
#define vdp_data_32 (*((u32 volatile *) VDP_DATA))

/**
 * @def vdp_hvcounter
 * @brief Reports the current position of the electron beam on the screen
 * @ingroup vdp
 * @param[read]
 * \n Upper byte: Vertical position
 * \n Lower byte: Horizontal position
 *
 * @sa VDP_HVCOUNTER
 */
#define vdp_hvcounter (*((u16 volatile *) VDP_HVCOUNTER))

/** @brief Vertical position byte of the HV counter */
#define vdp_hvcounter_v (*((u8 volatile *) VDP_HVCOUNTER))

/** @brief Horizontal position byte of the HV counter */
#define vdp_hvcounter_h (*((u8 volatile *) (VDP_HVCOUNTER + 1)))

/**
 * @enum PlaneWidth
 * @brief Helper for working with plane widths specified in tiles by specifying
 * width in bytes
 * @note Use with VDP_PLANE_POS helper below
 */
typedef enum PlaneWidth
{
  Width32 = 64,
  Width64 = 128,
  Width128 = 256
} PlaneWidth;

/**
 * @def VDP_PLANE_POS
 * @brief Generates the nametable offset for a tile at pos x/y for a given plane
 * width
 * @param x horizontal position in the tilemap
 * @param y vertical position in the tilemap
 * @param width width of the the plane (in bytes, so tile width * 2)
 */
#define VDP_PLANE_POS(x, y, width) (((y) * (width)) + ((x) << 1))

/**
 * @def VDP_PLANE_XY
 * @brief VRAM write command for the cell at x/y of a plane
 * @param plane_addr VRAM address of the plane's nametable
 * @param x horizontal position in cells
 * @param y vertical position in cells
 * @param width width of the plane (PlaneWidth, in bytes)
 *
 * @details
 * The position argument of text_print and bios_print. With the Boot ROM's
 * default layout, plane A is at BIOS_VDP_DEFAULT_PLANEA and is Width64.
 */
#define VDP_PLANE_XY(plane_addr, x, y, width) \
  (to_vdp_addr((plane_addr) + VDP_PLANE_POS(x, y, width)) | VRAM_W)

/**
 * @brief Converts a 16 bit VRAM address into VDP format at runtime
 * @note Consider using the to_vdp_addr macro instead to use compile time
 * evaluation if available
 */
static inline vdp_addr to_vdp_addr_runtime(u16 addr)
{
  vdp_addr vdp_addr = (u32) addr;

  // clang-format off
	asm(
		"\
  lsl.l    #2, %[vdp_addr] \n\
  lsr.w    #2, %[vdp_addr] \n\
  swap     %[vdp_addr] \n\
		"
		:
			[vdp_addr] "+d"(vdp_addr)
		:
		:
			"cc"
	);
  // clang-format on

  return vdp_addr;
}

/**
 * @brief Converts a VDP format address to a 16 bit VRAM address at runtime
 */
static inline u16 to_vram_addr_runtime(vdp_addr vdp_addr)
{
  u32 vram_addr = vdp_addr;

  // clang-format off
	asm(
		"\
  ror.w    #2, %[vram_addr] \n\
  lsr.l    #8, %[vram_addr] \n\
  lsr.l    #6, %[vram_addr] \n\
  ror.w    #2, %[vram_addr] \n\
		"
		:
			[vram_addr] "+d"(vram_addr)
		:
		:
			"cc"
	);
  // clang-format on

  return (u16) vram_addr;
}

/**
 * @def to_vdp_addr
 * @brief Converts a 16 bit VRAM address into VDP format at compile time if
 * possible
 */
#define to_vdp_addr(vram_addr)                      \
  (__builtin_constant_p(vram_addr)                  \
     ? (unsigned) ((((vram_addr) & 0x3FFF) << 16) | \
                   (((vram_addr) & 0xC000) >> 14))  \
     : to_vdp_addr_runtime(vram_addr))

// TODO: create similar macro for to_vram_addr

/** @brief VRAM byte offset of a tile index (32 bytes per tile) */
#define tile_offset(tile_index) ((tile_index) << 5)

// the below is causing weird breakage when compiling...
/*
void vdp_dma_fill(
  vdp_cmd		dest,
  u16 const count,
  u8 const	value)
{
  register u32 scratch_d, scratch_a;

  // clang-format off
  asm volatile(
    "\
  lea      (%c[vdp_ctrlport]).l, %[scratch_a] \n\
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
      [scratch_d] "=&d"(scratch_d),
      [scratch_a] "=&a"(scratch_a)
    :
      [vdp_ctrlport] "i"(VDP_CTRL),
      [dest] "d"(dest),
      [count] "d"(count),
      [value] "d"(value)
    :
      "cc"
  );
  // clang-format on
}
*/

/**
 * @ingroup vdp
 * @warning Setting/clearing the DMA Enable bit on VDP Mode Register 2 is the
 * responsibility of the user
 */
static inline void vdp_dma_transfer(char const * source, vdp_cmd dest, u16 const length)
{
  register u32 scratch_d, scratch_a;

  // clang-format off
	asm volatile(
		"\
  lea      (%c[vdp_ctrlport]).l, %[scratch_a] \n\
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
			[vdp_ctrlport] "i"(VDP_CTRL),
			[dest] "d"(dest),
			[source] "d"(source),
			[length] "d"(length)
		:
			"cc"
	);
  // clang-format on
}

#ifdef __cplusplus
}
#endif

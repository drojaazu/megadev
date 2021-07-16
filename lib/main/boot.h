/**
 * \file boot.h
 * \brief C wrappers for Boot ROM calls
 */

#ifndef MEGADEV__CD_MAIN_BOOT_H
#define MEGADEV__CD_MAIN_BOOT_H

#include "main/boot_def.h"
#include "types.h"
#include "vdp.h"

struct SpriteObject {
  u16 status;
  u16 flags;
  u8 * spriteDef;
  u32 posX;
  u32 posY;
  u32 moveX;
  u32 moveY;
  u8 uk1;
  u8 uk2;
};

typedef struct Palette {
  u8 cram_offset;
  u8 length; // length in words (i.e. color entries)
  u16 colors[];
} Palette;

/**
 * \sa _DECOMP_BUFFER
 */
#define DECOMP_BUFFER ((u8 *)_DECOMP_BUFFER)

/**
 * \sa _SPRITE_LIST
 */
#define SPRITE_LIST ((Sprite *)_SPRITE_LIST)

/**
 * \sa _PALETTE
 */
#define PALETTE ((u16 *)_PALETTE)

/**
 * \sa _VINT_EX_PTR
 */
#define VINT_EX_PTR ((void *(*))_VINT_EX_PTR)

/**
 * \sa _VDP_REGS
 */

/**
 * \sa _COMFLAGS
 */
#define COMFLAGS ((volatile u16 *)_COMFLAGS)

/**
 * \sa _COMFLAGS_MAIN
 */
#define COMFLAGS_MAIN ((volatile u8 *)_COMFLAGS_MAIN)

/**
 * \sa _COMFLAGS_SUB
 */
#define COMFLAGS_SUB ((volatile u8 const *)_COMFLAGS_SUB)

/**
 * \sa _COMCMD0
 */
#define COMCMD0 ((volatile u16 *)_COMCMD0)

/**
 * \sa _COMCMD1
 */
#define COMCMD1 ((volatile u16 *)_COMCMD1)

/**
 * \sa _COMCMD2
 */
#define COMCMD2 ((volatile u16 *)_COMCMD2)

/**
 * \sa _COMCMD3
 */
#define COMCMD3 ((volatile u16 *)_COMCMD3)

/**
 * \sa _COMCMD4
 */
#define COMCMD4 ((volatile u16 *)_COMCMD4)

/**
 * \sa _COMCMD5
 */
#define COMCMD5 ((volatile u16 *)_COMCMD5)

/**
 * \sa _COMCMD6
 */
#define COMCMD6 ((volatile u16 *)_COMCMD6)

/**
 * \sa _COMCMD7
 */
#define COMCMD7 ((volatile u16 *)_COMCMD7)

/**
 * \sa _COMSTAT0
 */
#define COMSTAT0 ((volatile u16 const *)_COMSTAT0)

/**
 * \sa _COMSTAT1
 */
#define COMSTAT1 ((volatile u16 const *)_COMSTAT1)

/**
 * \sa _COMSTAT2
 */
#define COMSTAT2 ((volatile u16 const *)_COMSTAT2)

/**
 * \sa _COMSTAT3
 */
#define COMSTAT3 ((volatile u16 const *)_COMSTAT3)

/**
 * \sa _COMSTAT4
 */
#define COMSTAT4 ((volatile u16 const *)_COMSTAT4)

/**
 * \sa _COMSTAT5
 */
#define COMSTAT5 ((volatile u16 const *)_COMSTAT5)

/**
 * \sa _COMSTAT6
 */
#define COMSTAT6 ((volatile u16 const *)_COMSTAT6)

/**
 * \sa _COMSTAT7
 */
#define COMSTAT7 ((volatile u16 const *)_COMSTAT7)

/**
 * \sa _JOY1_TYPE
 */
#define JOY1_TYPE ((u8 *)_JOY1_TYPE)

/**
 * \sa _JOY1_HOLD
 */
#define JOY1_HOLD ((volatile u8 const *)_JOY1_HOLD)

/**
 * \sa _JOY1_PRESS
 */
#define JOY1_PRESS ((volatile u8 const *)_JOY1_PRESS)

/**
 * \sa _JOY2_HOLD
 */
#define JOY2_HOLD ((volatile u8 const *)_JOY2_HOLD)

/**
 * \sa _JOY2_PRESS
 */
#define JOY2_PRESS ((volatile u8 const *)_JOY2_PRESS)

/**
 * \sa _JOY1_DELAY
 */
#define JOY1_DELAY ((volatile u8 *)_JOY1_DELAY)

/**
 * \sa _JOY2_DELAY
 */
#define JOY2_DELAY ((volatile u8 *)_JOY2_DELAY)

/**
 * \sa _VINT_FLAGS
 */
#define VINT_FLAGS ((u8 *)_VINT_FLAGS)

/**
 * \sa _VINT_COUNTER
 */
#define VINT_COUNTER ((u8 *)_VINT_COUNTER)

/**
 * \sa _VINT_SKIP_GFX_UPDATE
 */
#define VINT_SKIP_GFX_UPDATE ((u8 *)_VINT_SKIP_GFX_UPDATE)

/**
 * \sa _VDP_UPDATE_FLAGS
 */
#define VDP_UPDATE_FLAGS ((u8 *)_VDP_UPDATE_FLAGS)

/**
 * \sa _RANDOM
 */
#define RANDOM ((u16 const *)_RANDOM)

/**
 * \sa _FONT_TILE_BASE
 */
#define FONT_TILE_BASE ((u16 *)_FONT_TILE_BASE)

/**
 * \enum PlaneWidthTiles
 * \brief Use with \ref _PLANE_WIDTH to represent the width in tiles instead
 * of bytes
 */
typedef enum PlaneWidthTiles {
  Width32 = 64,
  Width64 = 128,
  Width128 = 256
} PlaneWidthTiles;

/**
 * \sa _PLANE_WIDTH
 * \note Use the \ref PlaneWidthTiles enum to make it semantically clear what
 * the width is in tiles
 */
#define PLANE_WIDTH ((u16 *)_PLANE_WIDTH)

/**
 * \sa _SPRTBL_PTR
 */
#define SPRTBL_PTR ((void *)_SPRTBL_PTR)

/**
 * \sa _SPR_JMPTBL_PTR
 */
#define SPR_JMPTBL_PTR ((void *(*))_SPR_JMPTBL_PTR)

/**
 * \sa _FADEIN_PAL_INDEX
 */
#define FADEIN_PAL_INDEX ((u8 *)_FADEIN_PAL_INDEX)

/**
 * \sa _FADEIN_PAL_LENGTH
 */
#define FADEIN_PAL_LENGTH ((u8 *)_FADEIN_PAL_LENGTH)

/**
 * \sa _FADEIN_INCREMENT
 */
#define FADEIN_INCREMENT ((u8 *)_FADEIN_INCREMENT)

/**
 * \sa _FADEIN_TARGET_PAL_PTR
 */
#define FADEIN_TARGET_PAL_PTR ((u8 *)_FADEIN_TARGET_PAL_PTR)

/*
  Boot ROM function wrappers
*/

/**
 * \sa BOOT_VINT
 */
static inline void boot_vint() { asm("jsr %p0" ::"i"(BOOT_VINT)); }

/**
 * \sa BOOT_SET_HINT_DEFAULT
 */
static inline void boot_set_hint_default(void * hint_routine) {
  register u32 a1_ptr asm("a1") = (u32)hint_routine;
  asm("jsr %p0" ::"i"(BOOT_SET_HINT_DEFAULT), "a"(a1_ptr));
}

/**
 * \sa BOOT_UPDATE_INPUT
 */
static inline void boot_update_inputs() {
  asm(R"(
  move.l a6, -(sp)
	jsr %p0
	move.l (sp)+, a6)" ::"i"(BOOT_UPDATE_INPUTS)
      : "d6", "d7", "a5");
}

/**
 * \sa BOOT_CLEAR_VRAM
 */
static inline void boot_clear_vram() {
  asm(R"(
  move.l a6, -(sp)
	jsr %p0
	move.l (sp)+, a6)" ::"i"(BOOT_CLEAR_VRAM)
      : "d0", "d1", "d2", "d3");
}

/**
 * \sa BOOT_CLEAR_NMTBL
 */
static inline void boot_clear_nmtbl() {
  asm(R"(
  move.l a6, -(sp)
	jsr %p0
	move.l (sp)+, a6)" ::"i"(BOOT_CLEAR_NMTBL)
      : "d0", "d1", "d2", "d3");
}

/**
 * \sa BOOT_CLEAR_VSRAM
 */
static inline void boot_clear_vsram() {
  asm("jsr %p0" ::"i"(BOOT_CLEAR_VSRAM) : "d0", "d1", "d2");
}

/**
 * \sa BOOT_LOAD_VDPREGS_DEFAULT
 */
static inline void boot_load_vdpregs_default() {
  asm("jsr %p0" ::"i"(BOOT_LOAD_VDPREGS_DEFAULT) : "d0", "d1", "a1", "a2");
}

/**
 * \sa BOOT_LOAD_VDPREGS
 */
static inline void boot_load_vdpregs(void const * vdp_reg_data) {
  register u32 a1_vdpregs asm("a1") = (u32)vdp_reg_data;
  asm("jsr %p0" ::"i"(BOOT_LOAD_VDPREGS), "a"(a1_vdpregs) : "d0", "d1", "a2");
}

/**
 * \sa BOOT_VDP_FILL
 */
static inline void boot_vdp_fill(u32 vdpptr, u16 length, u16 value) {
  register u32 d0_vdpptr asm("d0") = vdpptr;
  register u16 d1_length asm("d1") = length;
  register u16 d2_value asm("d2") = value;
  asm("jsr %p0" ::"i"(BOOT_VDP_FILL), "d"(d0_vdpptr), "d"(d1_length),
      "d"(d2_value));
}

/**
 * \sa BOOT_VDP_FILL_CLEAR
 */
static inline void boot_vdp_fill_clear(u32 vdpptr, u16 length) {
  register u32 d0_vdpptr asm("d0") = vdpptr;
  register u16 d1_length asm("d1") = length;
  asm("jsr %p0" ::"i"(BOOT_VDP_FILL_CLEAR), "d"(d0_vdpptr), "d"(d1_length)
      : "d2");
}

/**
 * \sa BOOT_DMA_FILL_CLEAR
 */
static inline void boot_dma_fill_clear(u32 vdpptr, u16 length) {
  register u32 d0_vdpptr asm("d0") = vdpptr;
  register u16 d1_length asm("d1") = length;
  asm(R"(
  move.l a6, -(sp)
	jsr %p0
	move.l (sp)+, a6)" ::"i"(BOOT_DMA_FILL_CLEAR),
      "d"(d0_vdpptr), "d"(d1_length)
      : "d2", "d3");
}

/**
 * \sa BOOT_DMA_FILL
 */
static inline void boot_dma_fill(u32 vdpptr, u16 length, u16 value) {
  register u32 d0_vdpptr asm("d0") = vdpptr;
  register u16 d1_length asm("d1") = length;
  register u16 d2_value asm("d2") = value;
  asm(R"(
  move.l a6, -(sp)
	jsr %p0
	move.l (sp)+, a6)" ::"i"(BOOT_DMA_FILL),
      "d"(d0_vdpptr), "d"(d1_length), "d"(d2_value)
      : "d3");
}

/**
 * \sa BOOT_LOAD_MAP
 */
static inline void boot_load_map(u32 vdpptr, u16 width, u16 height,
                                 void * map) {
  register u32 d0_vdpptr asm("d0") = vdpptr;
  register u16 d1_width asm("d1") = width;
  register u16 d2_height asm("d2") = height;
  register u32 a1_map asm("a1") = (u32)map;
  asm("jsr %p0" ::"i"(BOOT_LOAD_MAP), "d"(d0_vdpptr), "d"(d1_width),
      "d"(d2_height), "a"(a1_map)
      : "d3", "a5");
}

/**
 * \sa BOOT_GFX_DECOMP
 */
static inline void boot_gfx_decomp(u8 * data) {
  register u32 a1_data asm("a1") = (u32)data;
  asm("jsr %p0" ::"i"(BOOT_GFX_DECOMP), "a"(a1_data));
}

/**
 * \sa BOOT_VDP_DISP_ENABLE
 */
static inline void boot_vdp_disp_enable() {
  asm("jsr %p0" ::"i"(BOOT_VDP_DISP_ENABLE));
}

/**
 * \sa BOOT_VDP_DISP_DISABLE
 */
static inline void boot_bdp_disp_disable() {
  asm("jsr %p0" ::"i"(BOOT_VDP_DISP_DISABLE));
}

/**
 * \sa BOOT_VINT_WAIT_DEFAULT
 */
static inline void boot_vint_wait_default() {
  asm("jsr %p0" ::"i"(BOOT_VINT_WAIT_DEFAULT) : "d0");
}

/**
 * \sa BOOT_VINT_WAIT
 */
static inline void boot_vint_wait(u8 flags) {
  register u8 d0_flags asm("d0") = flags;
  asm("jsr %p0" ::"i"(BOOT_VINT_WAIT), "d"(d0_flags));
}

extern bool vdp_pal_fadeout(u8 index, u8 length);

/**
 * \sa BOOT_LOAD_FONT_DEFAULTS
 */
static inline void boot_load_font_defaults() {
  asm("jsr %p0" ::"i"(BOOT_LOAD_FONT_DEFAULTS)
      : "d0", "d1", "d2", "d3", "d4", "a1", "a5");
};

/**
 * \sa BOOT_INPUT_DELAY
 */
static inline void boot_input_delay(u8 * input, bool use_2p) {
  register u32 a1_input asm("a1") = (u32)input;
  register u16 d0_use_2p asm("d0") = (u16)use_2p;

  asm("jsr %p0"
      :
      : "i"(BOOT_INPUT_DELAY), "a"(a1_input), "d"(d0_use_2p)
      : "d1", "a5");
}

/**
 * \sa BOOT_CLEAR_COMM
 */
static inline void boot_clear_comm() {
  asm("jsr %p0" : : "i"(BOOT_CLEAR_COMM) : "d0", "a6");
}

/**
 * \sa BOOT_PRINT
 */
static inline void boot_print(char const * string, u32 vdpptr_pos) {
  register u32 a1_string asm("a1") = (u32)string;
  register u32 d0_vdpptr_pos asm("d0") = vdpptr_pos;

  asm(R"(jsr %p0)" ::"i"(BOOT_PRINT), "a"(a1_string), "d"(d0_vdpptr_pos)
      : "d1", "d2", "a5");
};

/**
 * \sa BOOT_NMTBL_FILL
 */
static inline void boot_nmtbl_fill(u32 vdpptr, u16 width, u16 height,
                                   u16 value) {
  register u32 d0_vdpptr asm("d0") = vdpptr;
  register u32 d1_width asm("d1") = width;
  register u32 d2_height asm("d2") = height;
  register u32 d3_value asm("d3") = value;

  asm(R"(
    move.l a6, -(sp)
    jsr %p0
    move.l (sp)+, a6
  )" ::"i"(BOOT_NMTBL_FILL),
      "d"(d0_vdpptr), "d"(d1_width), "d"(d2_height), "d"(d3_value)
      : "d1", "d2", "a5");
};

/**
 * \sa BOOT_DMA_XFER
 */
static inline void boot_dma_xfer(u32 vdpptr_dest, u8 const * source,
                                 u16 length) {
  register u32 d0_vdpptr_dest asm("d0") = vdpptr_dest;
  register u32 d1_source asm("d1") = (u32)source;
  register u16 d2_length asm("d2") = length;

  asm(R"(
  move.l a6, -(sp)
  jsr %p0
  move.l (sp)+, a6
)" ::"i"(BOOT_DMA_XFER),
      "d"(d0_vdpptr_dest), "d"(d1_source), "d"(d2_length)
      : "d3");
};

/**
 * \sa BOOT_DMA_XFER_WORDRAM
 */
static inline void boot_dma_xfer_wordram(u32 const vdpptr_dest,
                                         void const * source,
                                         u16 const length) {
  register u32 d0_vdpptr_dest asm("d0") = vdpptr_dest;
  register u32 d1_source asm("d1") = (u32)source;
  register u16 d2_length asm("d2") = length;

  asm(R"(
	move.l a6, -(sp)
  jsr %p0
	move.l (sp)+, a6
)" ::"i"(BOOT_DMA_XFER_WORDRAM),
      "d"(d0_vdpptr_dest), "d"(d1_source), "d"(d2_length)
      : "d3");
};

/**
 * \sa BOOT_DMA_COPY
 */
static inline void boot_dma_copy(u32 vdpptr_dest, u16 source, u16 length) {
  register u32 d0_vdpptr asm("d0") = vdpptr_dest;
  register u16 d1_source asm("d1") = source;
  register u16 d2_length asm("d2") = length;

  asm(R"(
  move.l a6, -(sp)
  jsr %p0
  move.l (sp)+, a6
)" ::"i"(BOOT_DMA_COPY),
      "d"(d0_vdpptr), "d"(d1_source), "d"(d2_length)
      : "d3");
};

/**
 * \sa BOOT_COPY_SPRLIST
 */
static inline void boot_copy_sprlist() {
  asm("jsr %p0" ::"i"(BOOT_COPY_SPRLIST) : "d4", "a4");
};

/**
 * \sa BOOT_CLEAR_RAM
 */
static inline void boot_clear_ram(void * address, u32 long_count) {
  register u32 a0_address asm("a0") = (u32)address;
  register u32 d7_long_count asm("d7") = long_count;

  asm(R"(
	move.l a6, -(sp)
	jsr %p0
	move.l (sp)+, a6
)" ::"i"(BOOT_CLEAR_RAM),
      "d"(d7_long_count), "a"(a0_address));
};

/**
 * \sa BOOT_LOAD_PAL
 */
static inline void boot_load_pal(Palette const * pal_data) {
  register u32 a1_pal_data asm("a1") = (u32)pal_data;

  asm(R"(jsr %p0)" ::"i"(BOOT_LOAD_PAL), "a"(a1_pal_data) : "d0");
};

/**
 * \sa BOOT_LOAD_PAL_UPDATE
 */
static inline void boot_load_pal_update(Palette const * pal_data) {
  register u32 a1_pal_data asm("a1") = (u32)pal_data;

  asm(R"(jsr %p0)" ::"i"(BOOT_LOAD_PAL_UPDATE), "a"(a1_pal_data) : "d0");
};

/**
 * \sa BOOT_PROCESS_SPR_OBJS
 */
static inline void boot_process_spr_objs(struct SpriteObject * const obj_array,
                                         Sprite * const sprtbl_cache,
                                         u16 const obj_count,
                                         u16 const obj_size) {
  register u32 a0_obj_array asm("a0") = (u32)obj_array;
  register u32 a1_sprtbl_cache asm("a1") = (u32)sprtbl_cache;
  register u16 d0_obj_count asm("d0") = (u16)obj_count;
  register u16 d1_obj_size asm("d1") = (u16)obj_size;

  asm(R"(jsr %p0)" ::"i"(BOOT_PROCESS_SPR_OBJS), "a"(a0_obj_array),
      "a"(a1_sprtbl_cache), "d"(d0_obj_count), "d"(d1_obj_size)
      : "d2", "d3", "d4", "d6", "a2");
};

/**
 * \sa BOOT_PRNG
 */
static inline void boot_prng() { asm("jsr %p0" : : "i"(BOOT_PRNG) : "d0"); };

/**
 * \sa BOOT_PRNG_MOD
 */
static inline u16 boot_prng_mod(u16 const modulo) {
  register u16 d0_modulo asm("d0") = modulo;
  register u16 d0_random asm("d0");

  asm("jsr %p1" : "+d"(d0_random) : "i"(BOOT_PRNG_MOD), "d"(d0_modulo) : "d1");

  return d0_random;
};

typedef struct DmaTransfer {
  u16 length;
  u32 vdpptr;
  u32 source;
} DmaTransfer;

/**
 * \sa BOOT_DMA_QUEUE
 */
static inline void boot_dma_queue(DmaTransfer const queue[]) {
  register u32 a1_queue asm("a1") = (u32)queue;

  asm(R"(
	move.l a6, -(sp)
  jsr %p0
	move.l (sp)+, a6
)" ::"i"(BOOT_DMA_QUEUE),
      "a"(a1_queue)
      : "d0", "d1", "d2", "d3");
}

#endif

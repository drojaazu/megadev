/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file main/bios.h
 * @brief Main CPU side system library
 *
 * @details
 * ## Memory Layout
 *  The Boot ROM library uses Work RAM from 0xFFF700 to 0xFFFC00 for much of
 *  its functionality. That range is divided into these blocks:
 *
 *      FFF700  +----------------+
 *              | Decompression  |
 *              |         Buffer |
 *      FFF900  +----------------+
 *              | Sprite List    |
 *              |          Cache |
 *      FFFB80  +----------------+
 *              | Palette Cache  |
 *      FFFC00  +----------------+
 *
 * It further uses space from 0xFFFDA8 onward for variables, some of which are
 * documented and useful and have wrappers in this header.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "fixed.h"
#include "main/bios.def.h"
#include "main/vdp.h"
#include "types.h"

/**
 * @brief One sprite of a SpriteLayout: position relative to the entity, size,
 * attributes and first tile
 * @details The Boot ROM's entity routine turns each mapping into a VDP sprite
 * table entry. The layout matches the sprite attribute table, with the
 * position fields relative to the entity instead of absolute.
 * @ingroup bios_vdp
 */
struct SpriteMapping {
  int8_t relpos_y;        ///< Vertical offset from the entity position
  uint8_t           : 4;  ///< Unused
  uint8_t  width    : 2;  ///< Width in tiles minus 1 (0 to 3)
  uint8_t  height   : 2;  ///< Height in tiles minus 1 (0 to 3)
  uint8_t  priority : 1;  ///< Sprite priority bit
  uint8_t  pal_line : 2;  ///< Palette line (0 to 3)
  uint8_t  v_flip   : 1;  ///< Vertical flip
  uint8_t  h_flip   : 1;  ///< Horizontal flip
  uint16_t chridx   : 11; ///< First tile index
  int8_t   relpos_x;      ///< Horizontal offset from the entity position
  int8_t
    mirrored_relpos_x; ///< Horizontal offset used when the entity is flipped (ENTITY_HFLIP)
} __attribute__((__packed__));

/**
 * @brief The sprites that make up an entity's appearance
 * @ingroup bios_vdp
 */
typedef struct SpriteLayout {
  uint8_t mapping_count; ///< Number of entries in mappings
  // this byte is a bit of a mystery: it is copied to 0x19 of the Entity
  // and if the h flip flag is set, it will add +1 to the value in the object.
  // It does not have any effect on the object and its purpose is unknown
  uint8_t
    unknown; ///< Copied into Entity::unknown; purpose unknown, see the comment above
  struct SpriteMapping mappings[]; ///< The sprites, mapping_count of them
} SpriteLayout;

/** @name Entity display flags (Entity::display_flags)
 * @ingroup bios_vdp */
///@{
#define ENTITY_HFLIP 1 << 7 ///< Draw the entity mirrored horizontally
#define ENTITY_HIDE  1 << 1 ///< Do not draw the entity
///@}

/**
 * @brief A sprite object for the Boot ROM's entity routine (bios_process_entities)
 * @details 26 bytes; a program may append its own fields and pass the real
 * size as obj_size. The position and movement are 16.16 fixed point.
 * @ingroup bios_vdp
 */
typedef struct {
  uint16_t enable : 1; ///< Entity is active
  uint16_t jmptbl_offset
    : 15; ///< Offset into the entity jump table (BIOS_ENTITY_ROUTINES)
  // uses the display flags defined above; the rest of the bits are user
  // definable
  uint8_t display_flags; ///< ENTITY_HFLIP, ENTITY_HIDE; the other bits are free
  // this byte is not used by blib, so presumably it can be used however
  // you'd like
  uint8_t user; ///< Not used by the Boot ROM; free for the program
  struct SpriteLayout const * layout; ///< Sprites to draw

  f32 pos_x;  ///< Horizontal position
  f32 pos_y;  ///< Vertical position
  f32 move_x; ///< Added to pos_x every frame
  f32 move_y; ///< Added to pos_y every frame
  // this byte is ORed on to the byte starting with the priority
  // bit in the SpriteMapping struct
  uint8_t
    sprite_flags; ///< ORed into the attribute byte of every sprite (priority, palette, flips)
  // the second byte of the SpriteLayout header is copied here
  uint8_t unknown; ///< Copy of SpriteLayout::unknown, plus 1 when flipped
} Entity;

/**
 * @brief Palette data for the Boot ROM's palette routines
 * @ingroup bios_vdp
 */
typedef struct Palette {
  uint8_t
    cram_offset;    ///< CRAM address to load at, in colors (the palette index)
  uint8_t length;   ///< Number of colors minus 1
  int16_t colors[]; ///< The colors, in CRAM format
} Palette;

/**
 * @def bios_work_buffer
 * @brief Work RAM for graphics decompression routines
 * @sa BIOS_WORK_BUFFER
 * @ingroup bios_cmp
 */
#define bios_work_buffer (*((int8_t (*)[0x200]) BIOS_WORK_BUFFER))

/**
 * @def bios_sprlist
 * @brief Sprite list buffer
 * @ingroup bios_vdp
 * @sa BIOS_SPRITE_CACHE
 *
 * @details
 * Size: 0x280 bytes
 */
#define bios_sprlist (*((Sprite(*)[80]) BIOS_SPRITE_CACHE))
// #define bios_sprlist ((Sprite *)BIOS_SPRITE_CACHE)

// #define bios_sprlist_len 0x280

/**
 * @def bios_palette
 * @brief CRAM (palette) buffer
 * @ingroup bios_vdp
 * @sa BIOS_PALETTE_CACHE
 */
#define bios_palette (*((int16_t (*)[64]) BIOS_PALETTE_CACHE))

// #define BIOS_PALETTE_SZ 0x80

/**
 * @def bios_pal1
 * @brief Palette line #0 buffer
 * @ingroup bios_vdp
 * @sa BIOS_PAL1
 */
#define bios_pal1 (*((int16_t (*)[16]) BIOS_PAL1))

/**
 * @def bios_pal2
 * @brief Palette line #1 buffer
 * @ingroup bios_vdp
 * @sa BIOS_PAL2
 */
#define bios_pal2 (*((int16_t (*)[16]) BIOS_PAL2))

/**
 * @def bios_pal3
 * @brief Palette line #2 buffer
 * @ingroup bios_vdp
 * @sa BIOS_PAL3
 */
#define bios_pal3 (*((int16_t (*)[16]) BIOS_PAL3))

/**
 * @def bios_pal4
 * @brief Palette line #3 buffer
 * @ingroup bios_vdp
 * @sa BIOS_PAL4
 */
#define bios_pal4 (*((int16_t (*)[16]) BIOS_PAL4))

/**
 * @def bios_vblank_user
 * @brief Pointer to the VBLANK_USER routine used in the Boot ROM VBLANK
 * handler.
 * @sa BIOS_VBLANK_USER
 */
#define bios_vblank_user ((void(* volatile *)) BIOS_VBLANK_USER)

/**
 * @def bios_vdp_regs
 * @brief VDP registers buffer
 * @ingroup bios_vdp
 *
 * @details Buffer of all VDP registers (except DMA regs), making up 19
 * entries. You will need to keep these updated manually unless you use
 * BIOS_LOAD_VDP_REGS
 *
 * Size: 16bit * 19 = 0x26 bytes
 *
 * @sa BIOS_VDP_REG_CACHE
 */
// #define bios_vdp_regs (*((volatile uint16_t(*)[19]) BIOS_VDP_REG_CACHE))
#define bios_vdp_regs ((volatile uint16_t *) BIOS_VDP_REG_CACHE)

/**
 * @def bios_comflags_main
 * @brief GA comm flags for Main CPU buffer
 * @sa BIOS_COMMFLAGS_MAIN_CACHE
 */
#define bios_comflags_main ((volatile uint8_t *) BIOS_COMMFLAGS_MAIN_CACHE)

/**
 * @def bios_comflags_sub
 * @brief GA comm flags for Sub CPU buffer
 * @sa BIOS_COMMFLAGS_SUB_CACHE
 */
#define bios_comflags_sub ((volatile uint8_t const *) BIOS_COMMFLAGS_SUB_CACHE)

/**
 * @def bios_comcmd
 * @brief Array of cached GA COMCMD (Main -> Sub) registers
 * @sa BIOS_COMCMD
 */
#define bios_comcmd (*((volatile uint16_t (*)[8]) BIOS_COMCMD))

/**
 * @def bios_comstat
 * @brief Array of cached GA COMSTAT (Main -> Sub) registers
 * @sa BIOS_COMSTAT
 */
#define bios_comstat (*((volatile uint16_t const(*)[8]) BIOS_COMSTAT))

/**
 * @def bios_joy1_type
 * @sa BIOS_JOY1_TYPE
 * @ingroup bios_input
 */
#define bios_joy1_type (*((uint8_t *) BIOS_JOY1_TYPE))

/**
 * @def bios_joy2_type
 * @sa BIOS_JOY2_TYPE
 * @ingroup bios_input
 */
#define bios_joy2_type ((uint8_t *) BIOS_JOY2_TYPE)

/**
 * @def bios_joy1_hold
 * @brief Port 1 controller input holds
 * @sa BIOS_JOY1_HOLD
 * @ingroup bios_input
 */
#define bios_joy1_hold (*((volatile uint8_t const *) BIOS_JOY1_HOLD))

/**
 * @def bios_joy1_hit
 * @brief Port 1 controller input hits (single-press)
 * @sa BIOS_JOY1_HIT
 * @ingroup bios_input
 */
#define bios_joy1_hit (*((volatile uint8_t const *) BIOS_JOY1_HIT))

/**
 * @def bios_joy2_hold
 * @brief Port 2 controller input holds
 * @sa BIOS_JOY2_HOLD
 * @ingroup bios_input
 */
#define bios_joy2_hold (*((volatile uint8_t const *) BIOS_JOY2_HOLD))

/**
 * @def bios_joy2_hit
 * @brief Port 2 controller input hits (single-press)
 * @sa BIOS_JOY2_HIT
 * @ingroup bios_input
 */
#define bios_joy2_hit (*((volatile uint8_t const *) BIOS_JOY2_HIT))

/**
 * @def bios_joy1_repeat_delay
 * @sa BIOS_JOY1_REPEAT_DELAY
 * @ingroup bios_input
 *
 * @note For use with @ref bios_input_repeat_delay
 */
#define bios_joy1_repeat_delay (*((volatile uint8_t *) BIOS_JOY1_REPEAT_DELAY))

/**
 * @def bios_joy2_repeat_delay
 * @sa BIOS_JOY2_REPEAT_DELAY
 * @ingroup bios_input
 *
 * @note For use with @ref bios_input_repeat_delay
 */
#define bios_joy2_repeat_delay (*((volatile uint8_t *) BIOS_JOY2_REPEAT_DELAY))

/**
 * @def bios_vblank_handler_flags
 * @sa BIOS_VBLANK_HANDLER_FLAGS
 * @ingroup bios_int
 *
 * @details
 * Used by the Boot ROM VBLANK/VBLANK_WAIT routines for graphics updates
 * during vblank.
 *
 * Bit 0 - Copy sprite list to VDP
 * Bit 1 - Call VBLANK_USER vector during vblank
 */
#define bios_vblank_handler_flags (*((uint8_t *) BIOS_VBLANK_HANDLER_FLAGS))

/**
 * @def bios_vblank_counter
 * @brief Incremented by 1 on each vertical blank (VBLANK)
 * @sa BIOS_VBLANK_COUNTER
 * @ingroup bios_int
 */
#define bios_vblank_counter (*((volatile uint8_t *) BIOS_VBLANK_COUNTER))

/**
 * @def bios_vblank_skip_gfx
 * @sa BIOS_VBLANK_FAST_FLAG
 * @ingroup bios_int
 *
 * @details Skips these operations during vblank:
 * CRAM copy, sprite list copy, increment vblank counter
 *
 * Will still perform IO updates, though
 */
#define bios_vblank_skip_gfx (*((uint8_t *) BIOS_VBLANK_FAST_FLAG))

/**
 * @def bios_vdp_update_flags
 * @sa BIOS_VDP_UPDATE_FLAGS
 * @ingroup bios_int
 */
#define bios_vdp_update_flags (*((uint8_t *) BIOS_VDP_UPDATE_FLAGS))

/**
 * @def bios_random
 * @brief Contains a random 16 bit value
 * @sa BIOS_RANDOM
 * @ingroup bios_misc
 *
 * @note
 * You must make a call to @ref BIOS_PRNG on each vblank in order to
 * use this!
 */
#define bios_random (*((uint16_t const *) BIOS_RANDOM))

/**
 * @def bios_font_tile_base
 * @sa BIOS_FONT_TILE_BASE
 * @ingroup bios_misc
 *
 * @details
 * The value added to each character byte when calling _PRINT_STRING.
 * The font can begin no earlier than tile index 0x20
 */
#define bios_font_tile_base (*((uint16_t *) BIOS_FONT_TILE_BASE))

/**
 * @def bios_plane_width
 * @brief Cached value of the plane width as defined in VDP reg. 0x10.
 * @ingroup bios_vdp
 * @sa BIOS_VDP_DEFAULT_PLANE_WIDTH
 *
 * @note
 * This value is stored as BYTES, which is effectively the width in
 * tiles * 2. Example: if the plane width is 32 in the register, this value
 * must be 64, and so on.
 * @note
 * This needs to be manually updated each time the register is changed!!
 * @note
 * Use the @ref PlaneWidth enum to make it semantically clear what
 * the width is in tiles
 */
#define bios_plane_width (*((uint16_t *) BIOS_PLANE_WIDTH_CACHE))

/**
 * @def bios_entity_routines
 * @brief Pointer to the jump table for entity processing
 * @sa BIOS_ENTITY_ROUTINES
 * @ingroup bios_vdp
 */
#define bios_entity_routines (*((void const **) BIOS_ENTITY_ROUTINES))

/**
 * @def bios_fadein_cram_index
 * @brief Palette offset on which the fade in palette should begin
 * @sa BIOS_FADEIN_PAL_INDEX
 * @ingroup bios_vdp
 */
#define bios_fadein_cram_index (*((uint8_t *) BIOS_FADEIN_PAL_INDEX))

/**
 * @def bios_fadein_pal_length
 * @brief Number of entries in the fade in palette
 * @sa BIOS_FADEIN_PAL_LENGTH
 * @ingroup bios_vdp
 */
#define bios_fadein_pal_length (*((uint8_t *) BIOS_FADEIN_PAL_LENGTH))

/**
 * @def bios_fadein_step
 * @brief Indicates if a fade in is still in progress
 * @sa BIOS_FADEIN_STEP
 * @ingroup bios_vdp
 */
#define bios_fadein_step (*((volatile uint16_t *) BIOS_FADEIN_STEP))

/**
 * @brief Pointer to the target fade in palette
 * @sa BIOS_FADEIN_TARGET_PAL
 * @ingroup bios_vdp
 */
#define bios_fadein_target_pal (*((uint8_t *) BIOS_FADEIN_TARGET_PAL))

/**
 * @brief Jump to internal user BIOS
 * @ingroup bios_system
 * @warning Register clobbers not yet documented!
 *
 * @details Leads to Mega CD title screen.
 */
static inline void bios_entry()
{
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_ENTRY));
}

/**
 * @brief Perform a system reset
 * @ingroup bios_system
 * @warning Register clobbers not yet documented!
 *
 * @details On cold boot, checks for TMSS, clears RAM, writes
 * Z80 program, writes PSG data, then moves on to checking hardware region and
 * clearing VRAM.
 *
 * On warm boot, skips directly to region check, then jumps to
 * the reset vector. Leads to Mega CD title screen.
 *
 */
static inline void bios_reset()
{
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_RESET));
}

/**
 * @brief Initialize system
 * @ingroup bios_system
 * @warning Register clobbers not yet documented!
 *
 * @details Sets default VDP regs, clears VRAM, sets default vectors, inits
 * controllers, loads Z80 data, transfers Sub CPU BIOS, loads CD player program
 */
static inline void bios_init()
{
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_INIT));
}

/**
 * @brief System init with stack pointer set
 * @ingroup bios_system
 * @warning Register clobbers not yet documented!
 *
 * @details Same as @ref bios_init but sets the stack pointer first
 */
static inline void bios_init_sp()
{
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_INIT_SP));
}

/**
 * @brief Vertical Blank interrupt handler
 * @ingroup bios_int
 *
 * @details Copies GA comm registers to the RAM
 * mirrors, sends INT2 (VBLANK ocurred) to Sub CPU, updates VDP palette from
 * RAM, calls VBLANK_USER, updates IO (controllers)
 */
static inline void bios_vblank_handler()
{
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_VBLANK_HANDLER));
}

/**
 * @brief Sets the HBLANK vector
 * @ingroup bios_int
 *
 * @details Sets the HBLANK vector in the system jump table, and sets the Gate
 * Array HBLANK register to the system jump table entry, and enables the
 * interrupt on the VDP
 *
 * @note The VDP register cache will be updated
 */
static inline void bios_set_hblank(void * hblank_handler)
{
  register uint32_t A1 asm("a1") = (uint32_t) hblank_handler;
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_SET_HBLANK), "a"(A1)
    : "cc");
}

/**
 * @brief Update state of P1/P2 controllers
 * @ingroup bios_input
 */
static inline void bios_read_joypad()
{
  asm volatile(
    "\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
    "
    :
    : "i"(BIOS_READ_JOYPAD)
    : "d6", "d7", "a5");
}

/**
 * @brief Controller types bios_detect_controller reports
 * @ingroup bios_input
 */
typedef enum ControllerType {
  Joypad = CONTROLLER_JOYPAD,
  MegaMouse = CONTROLLER_MEGAMOUSE,
  MultiTap = CONTROLLER_MULTITAP
} ControllerType;

/**
 * @brief Detect the type of controller plugged in
 * @ingroup bios_input
 * @param io_data_port Joypad data port (io_data1 or io_data2)
 * @return Controller type
 *
 * @note
 * This is called during normal system init and with the results stored
 * in @ref bios_joy1_type and @ref bios_joy2_type
 */
static inline uint8_t bios_detect_controller(uint8_t volatile * io_data_port)
{
  register uint32_t A0 asm("a0") = (uint32_t) io_data_port;
  register uint8_t  D7 asm("d7");
  asm volatile(
    "\
  move.l a6, -(sp) \n\
  movea.l %2, a6 \n\
  jsr %c1 \n\
  move.l (sp)+, a6 \n\
    "
    : "=d"(D7)
    : "i"(BIOS_DETECT_CONTROLLER), "a"(A0)
    : "cc");

  return D7;
}

/**
 * @brief Clear all of VRAM and VSRAM via DMA Fill
 * @ingroup bios_vdp
 *
 * @note This does not clear CRAM.
 *
 * @todo this isn't clearing the sprite list for some reason...?
 *
 */
static inline void bios_clear_vram()
{
  asm volatile(
    "\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
    "
    :
    : "i"(BIOS_CLEAR_VRAM)
    : "d0", "d1", "d2", "d3");
}

/**
 * @brief Clear nametables and sprite list
 * @ingroup bios_vdp
 *
 * @note This works only with the Boot ROM default VRAM layout
 */
static inline void bios_clear_tables()
{
  asm volatile(
    "\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
    "
    :
    : "i"(BIOS_CLEAR_TABLES)
    : "d0", "d1", "d2", "d3");
}

/**
 * @brief Clear VSRAM
 * @ingroup bios_vdp
 */
static inline void bios_clear_vsram()
{
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_CLEAR_VSRAM)
    : "d0", "d1", "d2");
}

/**
 * @brief Loads the Boot ROM default VDP register defaults
 * @ingroup bios_vdp
 * @details
    0x8004
      - HBLANK disabled
      - 9-bit (standard) color mode
    0x8124
      - Mega Drive graphics mode
      - NTSC (is almost certainly set to PAL on such hardware)
      - VBLANK enabled
      - Disable display
    0x9011
      - Plane size: 64x64 cells (512x512 pixels)
    0x8B00
      - Scroll mode: full screen
    0x8C81
      - 40 cell (320px) width
    0x8328
      - Window nametable: 0xA000
    0x8230
      - Plane A nametable: 0xC000
    0x8407
      - Plane B nametable: 0xE000
    0x855C
      - Sprite table: 0xB800
    0x8D2F
      - Horiz Scroll table: 0xBC00
    0x8700
      - Background color 0
    0x8A00
      - HBLANK scanline count: 0
    0x8F02
      - Auto-increment 2
    0x9100
      - Window plane X position: 0
    0x9200
      - Window plane Y position: 0
 */
static inline void bios_load_vdp_regs_default()
{
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_LOAD_DEFAULT_VDP_REGS)
    : "d0", "d1", "a1", "a2");
}

/**
 * @brief Load values into multiple VDP registers
 * @ingroup bios_vdp
 *
 * @details Register data is an array of word sized values,
 * where the upper byte is the register ID
 * (e.g. 80, 81, etc) and the lower byte is
 * the value, with the list terminated by 0.
 */
static inline void bios_load_vdp_regs(vdp_reg const * vdp_reg_data)
{
  register uint32_t A1 asm("a1") = (uint32_t) vdp_reg_data;

  asm volatile(
    "\
  jsr %p1 \n\
    "
    : "+a"(A1)
    : "i"(BIOS_LOAD_VDP_REGS), "a"(A1)
    : "cc", "d0", "d1", "a2");
}

/**
 * @brief Fill a region of VDP memory with a value
 * @ingroup bios_vdp
 *
 * @details This is a simple data transfer via the VDP data port rather than
 * DMA.
 */
static inline void
bios_vdp_fill(uint32_t vdp_addr, uint16_t length, uint16_t value)
{
  register uint32_t D0 asm("d0") = vdp_addr;
  register uint16_t D1 asm("d1") = length;
  register uint16_t D2 asm("d2") = value;
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_VDP_FILL), "d"(D0), "d"(D1), "d"(D2));
}

/**
 * @brief Fill a region of VDP memory with 0
 * @ingroup bios_vdp
 *
 * @details This is a simple data transfer via the VDP data port rather than
 * DMA.
 */
static inline void bios_vdp_fill_clear(uint32_t vdp_addr, uint16_t length)
{
  register uint32_t D0 asm("d0") = vdp_addr;
  register uint16_t D1 asm("d1") = length;
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_VDP_FILL_CLEAR), "d"(D0), "d"(D1)
    : "d2");
}

/**
 * @brief Fill a region of VDP memory with zero
 * @ingroup bios_vdp
 * @sa BIOS_DMA_FILL_CLEAR
 */
static inline void bios_dma_fill_clear(uint32_t vdp_addr, uint16_t length)
{
  register uint32_t D0 asm("d0") = vdp_addr;
  register uint16_t D1 asm("d1") = length;
  asm volatile(
    "\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
    "
    :
    : "i"(BIOS_DMA_FILL_CLEAR), "d"(D0), "d"(D1)
    : "d2", "d3");
}

/**
 * @brief Fill a region of VDP memory with a value
 * @sa BIOS_DMA_FILL
 * @ingroup bios_vdp
 */
static inline void
bios_dma_fill(uint32_t vdp_addr, uint16_t length, uint16_t value)
{
  register uint32_t D0 asm("d0") = vdp_addr;
  register uint16_t D1 asm("d1") = length;
  register uint16_t D2 asm("d2") = value;
  asm volatile(
    "\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
    "
    :
    : "i"(BIOS_DMA_FILL), "d"(D0), "d"(D1), "d"(D2)
    : "d3");
}

/**
 * @brief Fill a region of a nametable with map data
 * @sa BIOS_LOAD_MAP
 * @ingroup bios_vdp
 *
 * @details The map data should be an array of word values in the standard
 * nametable entry format.
 */
static inline void bios_load_map(
  uint32_t const vdp_addr,
  uint16_t const width,
  uint16_t const height,
  void const *   map)
{
  register uint32_t D0 asm("d0") = vdp_addr;
  register uint16_t D1 asm("d1") = width;
  register uint16_t D2 asm("d2") = height;
  register uint32_t A1 asm("a1") = (uint32_t) map;

  asm volatile(
    "\
  jsr %p1 \n\
    "
    : "+d"(D2)
    : "i"(BIOS_LOAD_MAP), "d"(D0), "d"(D1), "d"(D2), "a"(A1)
    : "d3", "a5", "cc");
}

/**
 * @brief Sets the HBLANK vector for a Work RAM destination
 * @sa BIOS_SET_HBLANK_WORK_RAM
 * @ingroup bios_int
 *
 * @details
 * Sets the specified vector in the system jump table, and sets the
 * Gate Array HBLANK register to the specified vector, and enables the interrupt
 * on the VDP.
 *
 * This is functionally identical to @ref bios_set_hblank, however this version
 * sets the GA HBLANK register directly to the specified vector. Since the GA
 * register is only 16 bits, it uses only the lower word of the address and
 * expects the routine to be locaed in Work RAM, i.e. 0xFFxxxx. This means if
 * the specified HBLANK routine is located elsewhere (such as Word RAM), you
 * must use @ref bios_set_hblank instead.
 *
 * @details
 * The VDP register buffer (BIOS_VDP_REG_CACHE) is updated with this call.
 */
static inline void bios_set_hblank_workram(void * hblank_handler)
{
  register uint32_t A1 asm("a1") = (uint32_t) hblank_handler;

  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_SET_HBLANK_WORK_RAM), "a"(A1)
    : "cc");
}

/**
 * @brief Disables horizontal interrupts on the VDP
 * @ingroup bios_int
 *
 * @details The VDP register buffer (VDP_REGS) is updated with this call.
 */
static inline void bios_disable_hblank()
{
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_DISABLE_HBLANK)
    : "cc");
}

/**
 * @sa BIOS_GFX_DECOMP
 * @ingroup bios_cmp
 */
static inline void bios_gfx_decomp(uint8_t const * data)
{
  register uint32_t a1_data asm("a1") = (uint32_t) data;
  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_GFX_DECOMP), "a"(a1_data));
}

/**
 * @sa BIOS_VDP_DISP_ENABLE
 * @ingroup bios_vdp
 */
static inline void bios_vdp_disp_enable()
{
  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_VDP_DISP_ENABLE));
}

/**
 * @sa BIOS_VDP_DISP_DISABLE
 * @ingroup bios_vdp
 */
static inline void bios_vdp_disp_disable()
{
  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_VDP_DISP_DISABLE));
}

/**
 * @brief Wait for VBLANK interrupt with default flags
 * @ingroup bios_int
 * @sa BIOS_VBLANK_WAIT_DEFAULT
 *
 * @details This will set the default VBLANK flags (copy sprite list & call
 * VBLANK_USER) before waiting for VBLANK
 *
 * @note This will also make a call to _PRNG
 *
 * @warning This will enable all interrupts before waiting for the VBLANK!
 */
static inline void bios_vblank_wait_default()
{
  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_VBLANK_WAIT_DEFAULT)
    : "d0");
}

/**
 * @brief Wait for VBLANK interrupt
 * @ingroup bios_int
 * @sa BIOS_VBLANK_WAIT
 *
 * @note This will also make a call to _PRNG
 *
 * @warning This will enable all interrupts before waiting for the VBLANK!
 */
static inline void bios_vblank_wait(uint8_t flags)
{
  register uint8_t D0 asm("d0") = flags;
  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_VBLANK_WAIT), "d"(D0));
}

/**
 * @brief Fade a range of the color palette to black
 * @ingroup bios_vdp
 *
 * @param palette_index First color, as a CRAM color index
 * @param length Number of colors
 * @return true when the fade is complete, false while it is still in progress
 *
 * @details Call once per frame until it returns true.
 *
 * @note Sets the palette update flag on _GFX_REFRESH
 */
static inline bool bios_pal_fadeout(uint8_t palette_index, uint8_t length)
{
  register uint16_t D0 asm("d0") = (uint16_t) (palette_index << 1);
  register uint16_t D1 asm("d1") = (uint16_t) length;

  asm goto(
    "\
  		jsr %c0 \n\
			beq %l[fade_complete] \n\
    "
    :
    : "i"(BIOS_PAL_FADEOUT), "d"(D0), "d"(D1)
    : "cc"
    : fade_complete);

  return false;

fade_complete:
  return true;
}

/**
 * @brief Load the internal 1bpp ASCII font with default settings
 * @ingroup bios_misc
 *
 * @details
 * This will place the tiles starting at index 0x20, making it compatible with
 * _PRINT_STRING, and sets the font color to index 1.
 */
static inline void bios_load_font_defaults()
{
  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_LOAD_FONT_DEFAULTS)
    : "d0", "d1", "d2", "d3", "d4", "a1", "a5");
};

/**
 * @brief Load the 1bpp graphics into VDP
 * @ingroup bios_misc
 */
static inline void bios_load_1bpp_tiles(
  void * chr_data, uint16_t tile_count, vdp_cmd dest, uint32_t color_pattern)
{
  register uint32_t D0 asm("d0") = dest;
  register uint32_t D1 asm("d1") = color_pattern;
  register uint16_t D2 asm("d2") = tile_count;
  register uint32_t A1 asm("a1") = (uint32_t) chr_data;

  asm volatile(
    "\
			jsr %p2 \n\
    "
    : "+d"(D2), "+a"(A1)
    : "i"(BIOS_LOAD_1BPP_TILES), "a"(A1), "d"(D0), "d"(D1), "d"(D2)
    : "d3", "d4", "a5");
};

/**
 * @brief Generates a brief delay after initially pressing the D-pad
 * @ingroup bios_input
 *
 * @param[in] input Pointer to the byte which will hold the output D-pad value (A1)
 * @param[in] use_2p If false, use P1 input; if true, use P2 input (D0)
 *
 * @details
 * This is useful for working with cursors on menus as it creates a brief
 * pause when holding a D-pad direction. You'll need to test against the
 * output variable (set in a1) rather than the standard input mirror in order
 * to use this correctly.
 */
static inline void bios_input_repeat_delay(uint8_t * input, bool use_2p)
{
  register uint32_t A1 asm("a1") = (uint32_t) input;
  register uint16_t D0 asm("d0") = (uint16_t) use_2p;

  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_INPUT_REPEAT_DELAY), "a"(A1), "d"(D0)
    : "cc", "d1", "a5");
}

/**
 * @brief Fill a region of a nametable with consecutive tiles
 * @details Writes a width by height block of tile indices starting at chridx,
 * row by row, so that a stamp (an image stored as consecutive tiles) appears
 * in place.
 * @param width Width in tiles
 * @param height Height in tiles
 * @param dest Nametable address of the top left tile
 * @param chridx First tile index
 * @ingroup bios_vdp
 */
static inline void bios_load_stamp_tilemap(
  uint16_t width, uint16_t height, vdp_addr dest, uint16_t chridx)
{
  register uint32_t D0 asm("d0") = (uint32_t) dest;
  register uint16_t D1 asm("d1") = width;
  register uint16_t D2 asm("d2") = height;
  register uint16_t D3 asm("d3") = chridx;

  asm volatile(
    "\
			jsr %c3 \n\
    "
    : "+d"(D0), "+d"(D2), "+d"(D3)
    : "i"(BIOS_LOAD_STAMP_MAP), "d"(D0), "d"(D1), "d"(D2), "d"(D3)
    : "cc", "d4", "d5", "d6", "a5");
}

/**
 * @brief Clears all Gate Array communication registers
 * @ingroup bios_comm
 *
 * @details This clears the COMFLAGS and COMCMD registers directly as well as
 * their RAM buffers
 */
static inline void bios_clear_comm()
{
  asm volatile(
    "\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
    "
    :
    : "i"(BIOS_CLEAR_COMM)
    : "d0", "memory");
}

/**
 * @brief Displays an ASCII string
 * @ingroup bios_misc
 *
 * @param string The string, 0xFF terminated (see str_to_bios)
 * @param pos Nametable position as a VDP command (VDP_PLANE_XY)
 *
 * @details Strings are terminated with 0xFF and use 0x00 for newline.
 * The value in BIOS_FONT_TILE_BASE is added to each character byte, but no
 * other transformations are done. This means that the font must begin at index
 * 0x20 at the earliest (where BIOS_FONT_TILE_BASE is 0). Note that this can
 * only use palette line 0.
 */
static inline void bios_print(char const * string, vdp_cmd pos)
{
  register uint32_t A1 asm("a1") = (uint32_t) string;
  register uint32_t D0 asm("d0") = pos;

  asm volatile(
    "\
			jsr %p1 \n\
    "
    : "+a"(A1)
    : "i"(BIOS_PRINT), "a"(A1), "d"(D0)
    : "d1", "d2", "a5");
};

/**
 * @brief Fill a region of a nametable with a value
 * @param pos Nametable position of the top left tile, as a VDP command
 * @param width Width in tiles
 * @param height Height in tiles
 * @param value Nametable entry to write
 * @ingroup bios_vdp
 */
static inline void
bios_plane_fill(vdp_cmd pos, uint16_t width, uint16_t height, uint16_t value)
{
  register uint32_t D0 asm("d0") = pos;
  register uint32_t D1 asm("d1") = width;
  register uint32_t D2 asm("d2") = height;
  register uint32_t D3 asm("d3") = value;

  asm volatile(
    "\
    	move.l a6, -(sp) \n\
    	jsr %p1 \n\
    	move.l (sp)+, a6 \n\
  	"
    : "+d"(D2)
    : "i"(BIOS_PLANE_FILL), "d"(D0), "d"(D1), "d"(D2), "d"(D3)
    : "cc", "d5", "a5");
};

/**
 * @brief Performs a data transfer to VRAM via DMA
 * @param dest VRAM destination as a VDP command
 * @param source Source address
 * @param length Length in words
 * @ingroup bios_vdp
 */
static inline void
bios_dma_xfer(vdp_cmd dest, uint8_t const * source, uint16_t length)
{
  register uint32_t D0 asm("d0") = dest;
  register uint32_t D1 asm("d1") = (uint32_t) source;
  register uint16_t D2 asm("d2") = length;

  asm volatile(
    "\
  		move.l a6, -(sp) \n\
  		jsr %c0 \n\
  		move.l (sp)+, a6 \n\
    "
    :
    : "i"(BIOS_DMA_XFER), "d"(D0), "d"(D1), "d"(D2)
    : "d3");
};

/**
 * @brief Performs a data transfer from Word RAM to VRAM via DMA
 * @param dest VRAM destination as a VDP command
 * @param source Source address in Word RAM
 * @param length Length in words
 * @ingroup bios_vdp
 *
 * @details There is a well-documented issue with performing a DMA transfer from
 * Word RAM to VRAM which must be accounted for by writing the final word of
 * data to the data port. This subroutine takes care of that extra step.
 */
static inline void bios_dma_xfer_word_ram(
  vdp_cmd const dest, void const * source, uint16_t const length)
{
  register uint32_t D0 asm("d0") = dest;
  register uint32_t D1 asm("d1") = (uint32_t) source;
  register uint16_t D2 asm("d2") = length;

  asm volatile(
    "\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
    "
    :
    : "i"(BIOS_DMA_XFER_WORD_RAM), "d"(D0), "d"(D1), "d"(D2)
    : "cc", "d3");
};

/**
 * @brief Copy data within VRAM via DMA
 * @param vdpptr_dest Destination VRAM address as a VDP command
 * @param source Source VRAM address
 * @param length Length in bytes
 * @ingroup bios_vdp
 */
static inline void
bios_dma_copy(uint32_t vdpptr_dest, uint16_t source, uint16_t length)
{
  register uint32_t D0 asm("d0") = vdpptr_dest;
  register uint16_t D1 asm("d1") = source;
  register uint16_t D2 asm("d2") = length;

  asm volatile(
    "\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
    "
    :
    : "i"(BIOS_DMA_COPY), "d"(D0), "d"(D1), "d"(D2)
    : "d3");
};

/**
 * @brief Copies sprite list buffer to VDP via DMA
 * @ingroup bios_vdp
 *
 * @details This uses the default Boot ROM VRAM layout (i.e. sprite list at
 * 0xB800) Will only perform the copy of bit 0 of VBLANK_FLAGS is set (so this
 * is likely intended to be called from VBLANK, probably VBLANK_USER)
 */
static inline void bios_copy_sprlist()
{
  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_COPY_SPRLIST)
    : "d4", "a4");
};

/**
 * @brief Clear a region of memory
 * @param address Start of the region, long aligned
 * @param long_count Size in longs minus 1
 * @ingroup bios_misc
 */
static inline void bios_clear_ram(void * address, uint32_t long_count)
{
  register uint32_t A0 asm("a0") = (uint32_t) address;
  register uint32_t D7 asm("d7") = long_count;

  asm volatile(
    "\
			move.l a6, -(sp) \n\
			jsr %c0 \n\
			move.l (sp)+, a6 \n\
    "
    :
    : "i"(BIOS_CLEAR_RAM), "d"(D7), "a"(A0));
};

/**
 * @brief Load palette to RAM buffer
 * @param pal_data The palette
 * @ingroup bios_vdp
 *
 * @details The color palette is loaded but the "Palette Update" flag is not set
 */
static inline void bios_load_pal(Palette const * pal_data)
{
  register uint32_t A1 asm("a1") = (uint32_t) pal_data;

  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_LOAD_PAL), "a"(A1)
    : "d0");
};

/**
 * @brief Load palette to RAM buffer and set Palette Update flag
 * @param pal_data The palette
 * @ingroup bios_vdp
 */
static inline void bios_load_pal_update(Palette const * pal_data)
{
  register uint32_t A1 asm("a1") = (uint32_t) pal_data;

  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_LOAD_PAL_UPDATE), "a"(A1)
    : "d0");
};

/**
 * @brief Transfer palette cache to CRAM via DMA
 * @ingroup bios_vdp
 *
 * @note Uses bios_vdp_update_flags
 */
static inline void bios_copy_pal()
{
  asm volatile(
    "\
			jsr %c0 \n\
    "
    :
    : "i"(BIOS_COPY_PAL)
    : "cc", "a4", "d4");
};

/**
 * @brief Update/display sprite objects
 * @param obj_array The entities
 * @param sprtbl_cache Sprite table cache to fill (bios_sprlist)
 * @param obj_count Number of entities minus 1
 * @param obj_size Size of one entity in bytes (sizeof(Entity) plus any program fields)
 * @ingroup bios_vdp
 */
static inline void bios_process_entities(
  Entity const * obj_array,
  Sprite const * sprtbl_cache,
  uint16_t const obj_count,
  uint16_t const obj_size)
{
  register uint32_t A0 asm("a0") = (uint32_t) obj_array;
  register uint32_t A1 asm("a1") = (uint32_t) sprtbl_cache;
  register uint16_t D0 asm("d0") = (uint16_t) obj_count;
  register uint16_t D1 asm("d1") = (uint16_t) obj_size;

  asm volatile(
    "\
  jsr %c2 \n\
    "
    : "+a"(A0), "+a"(A1)
    : "i"(BIOS_PROCESS_ENTITIES), "a"(A0), "a"(A1), "d"(D0), "d"(D1)
    : "d2", "d3", "d4", "d6", "a2");
};

/**
 * @brief Generate a new random number limited with the given modulus
 * @param modulo Modulus
 * @return Random number from 0 to modulo - 1
 * @ingroup bios_misc
 *
 * @details The modulus can be used to specify an upper boundary of the random
 * number. To specify a minimum boundary, the modulus should be set like so:
 *   mod = (upper - lower) + 1
 * Finally, the lower bound should be added to the random number result.
 * Psuedo code:
 *   num = prng_mod((upper - lower) + 1) + lower
 */
static inline uint16_t bios_prng_mod(uint16_t const modulo)
{
  // TODO can we use the same variable here for in/out?
  register uint16_t d0_modulo asm("d0") = modulo;
  register uint16_t d0_random asm("d0");

  asm volatile(
    "\
  jsr %p1 \n\
    "
    : "+d"(d0_random)
    : "i"(BIOS_PRNG_MOD), "d"(d0_modulo)
    : "d1");

  return d0_random;
};

/**
 * @brief Generate a new random number
 * @ingroup bios_misc
 *
 * @details The number will be stored in bios_random
 */
static inline void bios_prng()
{
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_PRNG)
    : "d0");
};

/**
 * @brief Sets the target color palette for a fadein
 * @param palette The target palette
 * @ingroup bios_vdp
 *
 * @note This must be called before using bios_pal_fadein
 */
static inline void bios_set_fadein_pal(Palette const * palette)
{
  register uint32_t a1_bios_palette asm("a1") = (uint32_t) palette;
  register uint32_t a1_change asm("a1");

  asm volatile(
    "\
  jsr %p1 \n\
    "
    : "=a"(a1_change)
    : "i"(BIOS_SET_FADEIN_PAL), "a"(a1_bios_palette));
};

/**
 * @brief Fade a range of the color palette from black
 * @ingroup bios_vdp
 *
 * @details The fade-in routine works a little bit differently from the
 * fade-out counterpart. Instead of checking the Z flag, the value of
 * BIOS_FADEIN_STEP should be checked. If it is > 0, the fade is not yet
 * complete.
 *
 * This may be a bug in the implementation. Using the Z flag to check for
 * completion *could* work, as the code checks whether BIOS_FADEIN_STEP is
 * zero with a TST opcode. However, this is followed by setting the VDP flags
 * for a palette update, which will set the Z flag if palette update flag was
 * not already set.
 */
static inline void bios_pal_fadein()
{
  asm volatile(
    "\
  jsr %c0 \n\
    "
    :
    : "i"(BIOS_PAL_FADEIN));
}

/**
 * For use with bios_dma_queue
 */
/**
 * @brief One entry of a bios_dma_queue list
 * @ingroup bios_vdp
 */
typedef struct DmaTransfer {
  uint16_t length;   ///< Length in words
  uint32_t vdp_addr; ///< VRAM destination as a VDP command
  uint32_t source;   ///< Source address
} DmaTransfer;

/**
 * @brief Transfer queued VDP bound data via DMA
 * @param[in] queue Pointer to the queue (A1)
 * @ingroup bios_vdp
 *
 * @details The queue is an array of DMA transfer entries in this format:
 *     0.w Data length
 *     2.l Destination (vdp_addr)
 *     6.l Source address
 * The list should be terminated with a 0 word. Note that this system is
 * extremely basic and does not account for DMA bandwidth, etc. Moreover, no
 * array management is done and the list will need to be cleared by the user.
 */
static inline void bios_dma_queue(DmaTransfer const * queue)
{
  register uint32_t a1_queue asm("a1") = (uint32_t) queue;
  register uint32_t a1_change asm("a1");

  asm volatile(
    "\
  move.l a6, -(sp) \n\
  jsr %p1 \n\
  move.l (sp)+, a6 \n\
    "
    : "=a"(a1_change)
    : "i"(BIOS_DMA_QUEUE), "a"(a1_queue)
    : "d0", "d1", "d2", "d3");
}

#ifdef __cplusplus
}
#endif

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file bios.h
 * @brief C wrappers for Boot ROM calls
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

#ifndef MEGADEV__MAIN_BOOTLIB_H
#define MEGADEV__MAIN_BOOTLIB_H

#include "fixed.h"
#include "main/bios.def.h"
#include "main/vdp.h"
#include "types.h"

struct SpriteMapping
{
	s8 relpos_y;
	u8 : 4;
	u8 width : 2;
	u8 height : 2;
	u8 priority : 1;
	u8 pal_line : 2;
	u8 v_flip : 1;
	u8 h_flip : 1;
	u16 chridx : 11;
	s8 relpos_x;
	s8 mirrored_relpos_x;
} __attribute__((__packed__));

struct SpriteLayout
{
	u8 mapping_count;
	// this byte is a bit of a mystery: it is copied to 0x19 of the Entity
	// and if the h flip flag is set, it will add +1 to the value in the object.
	// It does not have any effect on the object and its purpose is unknown
	u8 unknown;
	struct SpriteMapping mappings[];
};

// BIOS defined Entity display flags
#define ENTITY_HFLIP 1 << 7
#define ENTITY_HIDE 1 << 1

// size: 26 bytes
struct Entity
{
	u16 enable : 1;
	u16 jmptbl_offset : 15;
	// uses the display flags defined above; the rest of the bits are user
	// definable
	u8 display_flags;
	// this byte is not used by blib, so presumably it can be used however
	// you'd like
	u8 user;
	struct SpriteLayout const * layout;
	f32 pos_x;
	f32 pos_y;
	f32 move_x;
	f32 move_y;
	// this byte is ORed on to the byte starting with the priority
	// bit in the SpriteMapping struct
	u8 sprite_flags;
	// the second byte of the SpriteLayout header is copied here
	u8 unknown;
};

/**
 * @struct Palette
 * cram_offset - address in CRAM to load palette - that is, the palette index
 * length - the number of colors in the palette to load MINUS ONE
 *
 * @ingroup bios_vdp
 */
typedef struct Palette
{
	u8 cram_offset;
	u8 length; // length in words (i.e. color entries) MINUS ONE
	s16 colors[];
} Palette;

/**
 * @def* bios_work_buffer
 * @brief Work RAM for graphics decompression routines
 * @sa _BIOS_WORK_BUFFER
 * @ingroup bios_cmp
 */
// TODO test this
#define bios_work_buffer (*(u8[0x200]) _BIOS_WORK_BUFFER)
// #define bios_work_buffer ((u8 *) _BIOS_WORK_BUFFER)

// TODO check that the LEN/SZ's in this section can be replaced with sizeof()
/**
 * @def BIOS_WORK_BUFFER_LEN
 * @brief Size of @ref bios_work_buffer
 */
// #define BIOS_WORK_BUFFER_LEN 0x200

/**
 * @def Sprite* bios_sprlist
 * @brief Sprite list buffer
 * @ingroup bios_vdp
 * @sa _BIOS_SPRLIST
 *
 * @details
 * Size: 0x280 bytes
 */
#define bios_sprlist (*((Sprite(*)[80]) _BIOS_SPRLIST))
// #define bios_sprlist ((Sprite *)_BIOS_SPRLIST)

// #define bios_sprlist_len 0x280

/**
 * @def s16* bios_palette
 * @brief CRAM (palette) buffer
 * @ingroup bios_vdp
 * @sa _BIOS_PALETTE
 */
#define bios_palette (*((s16(*)[64]) _BIOS_PALETTE))

// #define BIOS_PALETTE_SZ 0x80

/**
 * @def bios_pal1
 * @brief Palette line #0 buffer
 * @ingroup bios_vdplib/main
 * @sa _BIOS_PAL1
 */
#define bios_pal1 (*((s16(*)[16]) _BIOS_PAL1))

/**
 * @def bios_pal2
 * @brief Palette line #1 buffer
 * @ingroup bios_vdp
 * @sa _BIOS_PAL2
 */
#define bios_pal2 (*((s16(*)[16]) _BIOS_PAL2))

/**
 * @def bios_pal3
 * @brief Palette line #2 buffer
 * @ingroup bios_vdp
 * @sa _BIOS_PAL3
 */
#define bios_pal3 (*((s16(*)[16]) _BIOS_PAL3))

/**
 * @def bios_pal4
 * @brief Palette line #3 buffer
 * @ingroup bios_vdp
 * @sa _BIOS_PAL4
 */
#define bios_pal4 (*((s16(*)[16]) _BIOS_PAL4))

/**
 * @def void* bios_vint_user
 * @brief Pointer to the VINT_USER routine used in the Boot ROM VINT handler.
 * @sa _BIOS_VINT_USER
 */
#define bios_vint_user ((volatile void *(*) ) _BIOS_VINT_USER)

/**
 * @def* bios_vdp_regs
 * @brief VDP registers buffer
 * @ingroup bios_vdp
 *
 * @details Buffer of all VDP registers (except DMA regs), making up 19
 * entries. You will need to keep these updated manually unless you use
 * _BIOS_LOAD_VDPREGS
 *
 * Size: 16bit * 19 = 0x26 bytes
 *
 * @sa _BIOS_VDP_REGS
 */
// #define bios_vdp_regs (*((volatile u16(*)[19]) _BIOS_VDP_REGS))
#define bios_vdp_regs ((volatile u16 *) _BIOS_VDP_REGS)

/**
 * @def bios_comflags_main
 * @brief GA comm flags for Main CPU buffer
 * @sa _BIOS_COMFLAGS_MAIN
 */
#define bios_comflags_main ((volatile u8 *) _BIOS_COMFLAGS_MAIN)

/**
 * @def const bios_comflags_sub
 * @brief GA comm flags for Sub CPU buffer
 * @sa _BIOS_COMFLAGS_SUB
 */
#define bios_comflags_sub ((volatile u8 const *) _BIOS_COMFLAGS_SUB)

/**
 * @def* bios_comcmd
 * @brief Array of cached GA COMCMD (Main -> Sub) registers
 * @sa _BIOS_COMCMD
 */
#define bios_comcmd (*((volatile u16(*)[8]) _BIOS_COMCMD))

/**
 * @def bios_comstat
 * @brief Array of cached GA COMSTAT (Main -> Sub) registers
 * @sa _BIOS_COMSTAT
 */
#define bios_comstat (*((volatile u16 const(*)[8]) _BIOS_COMSTAT))

/**
 * @def bios_joy1_mouse_data
 * @sa _BIOS_JOY1_MOUSE_DATA
 * @ingroup bios_input
 */
#define bios_joy1_mouse_data (*((u16 *) _BIOS_JOY1_MOUSE_DATA))

/**
 * @def bios_joy1_mouse_dx
 * @brief Mouse delta X
 * @sa _BIOS_JOY1_MOUSE_DX
 * @ingroup bios_input
 */
#define bios_joy1_mouse_dx (*((u16 *) _BIOS_JOY1_MOUSE_DX))

/**
 * @def bios_joy1_mouse_dy
 * @brief Mouse delta Y
 * @sa _BIOS_JOY1_MOUSE_DY
 * @ingroup bios_input
 */
#define bios_joy1_mouse_dy (*((u16 *) _BIOS_JOY1_MOUSE_DY))

/**
 * @def bios_joy2_mouse_data
 * @sa _BIOS_JOY2_MOUSE_DATA
 * @ingroup bios_input
 */
#define bios_joy2_mouse_data (*((u16 *) _BIOS_JOY2_MOUSE_DATA))

/**
 * @def bios_joy2_mouse_dx
 * @brief Mouse delta X
 * @sa _BIOS_JOY2_MOUSE_DX
 * @ingroup bios_input
 */
#define bios_joy2_mouse_dx (*((u16 *) _BIOS_JOY2_MOUSE_DX))

/**
 * @def bios_joy2_mouse_dy
 * @brief Mouse delta Y
 * @sa _BIOS_JOY2_MOUSE_DY
 * @ingroup bios_input
 */
#define bios_joy2_mouse_dy (*((u16 *) _BIOS_JOY2_MOUSE_DY))

/**
 * @def bios_joy1_type
 * @sa _BIOS_JOY1_TYPE
 * @ingroup bios_input
 */
#define bios_joy1_type (*((u8 *) _BIOS_JOY1_TYPE))

/**
 * @def bios_joy2_type
 * @sa _BIOS_JOY2_TYPE
 * @ingroup bios_input
 */
#define bios_joy2_type ((u8 *) _BIOS_JOY2_TYPE)

/**
 * @def bios_joy1_hold
 * @brief Port 1 controller input holds
 * @sa _BIOS_JOY1_HOLD
 * @ingroup bios_input
 */
#define bios_joy1_hold (*((volatile u8 const *) _BIOS_JOY1_HOLD))

/**
 * @def bios_joy1_hit
 * @brief Port 1 controller input hits (single-press)
 * @sa _BIOS_JOY1_HIT
 * @ingroup bios_input
 */
#define bios_joy1_hit (*((volatile u8 const *) _BIOS_JOY1_HIT))

/**
 * @def bios_joy2_hold
 * @brief Port 2 controller input holds
 * @sa _BIOS_JOY2_HOLD
 * @ingroup bios_input
 */
#define bios_joy2_hold (*((volatile u8 const *) _BIOS_JOY2_HOLD))

/**
 * @def bios_joy2_hit
 * @brief Port 2 controller input hits (single-press)
 * @sa _BIOS_JOY2_HIT
 * @ingroup bios_input
 */
#define bios_joy2_hit (*((volatile u8 const *) _BIOS_JOY2_HIT))

/**
 * @def bios_joy1_repeat_delay
 * @sa _BIOS_JOY1_REPEAT_DELAY
 * @ingroup bios_input
 *
 * @note For use with @ref bios_input_repeat_delay
 */
#define bios_joy1_repeat_delay (*((volatile u8 *) _BIOS_JOY1_REPEAT_DELAY))

/**
 * @def BIOS_JOY2_DELAY
 * @sa _BIOS_JOY2_DELAY
 * @ingroup bios_input
 *
 * @note For use with @ref bios_input_repeat_delay
 */
#define BIOS_JOY2_DELAY (*((volatile u8 *) _BIOS_JOY2_DELAY))

/**
 * @def bios_vint_handler_flags
 * @sa _BIOS_VINT_HANDLER_FLAGS
 * @ingroup bios_int
 *
 * @details
 * Used by the Boot ROM VINT/VINT_WAIT routines for graphics updates
 * during vblank.
 *
 * Bit 0 - Copy sprite list to VDP
 * Bit 1 - Call VINT_USER vector during vblank
 */
#define bios_vint_handler_flags (*((u8 *) _BIOS_VINT_HANDLER_FLAGS))

/**
 * @def bios_vint_counter
 * @brief Incremented by 1 on each vertical blank (VINT)
 * @sa _BIOS_VINT_COUNTER
 * @ingroup bios_int
 */
#define bios_vint_counter (*((volatile u8 *) _BIOS_VINT_COUNTER))

/**
 * @def bios_vint_skip_gfx
 * @sa _BIOS_VINT_SKIP_GFX
 * @ingroup bios_int
 *
 * @details Skips these operations during vblank:
 * CRAM copy, sprite list copy, increment vblank counter
 *
 * Will still perform IO updates, though
 */
#define bios_vint_skip_gfx (*((u8 *) _BIOS_VINT_SKIP_GFX))

/**
 * @def bios_vdp_update_flags
 * @sa _BIOS_VDP_UPDATE_FLAGS
 * @ingroup bios_int
 */
#define bios_vdp_update_flags (*((u8 *) _BIOS_VDP_UPDATE_FLAGS))

/**
 * @def bios_random
 * @brief Contains a random 16 bit value
 * @sa _BIOS_RANDOM
 * @ingroup bios_misc
 *
 * @note
 * You must make a call to @ref BIOS_PRNG on each vblank in order to
 * use this!
 */
#define bios_random (*((u16 const *) _BIOS_RANDOM))

/**
 * @def bios_font_tile_base
 * @sa _BIOS_FONT_TILE_BASE
 * @ingroup bios_misc
 *
 * @details
 * The value added to each character byte when calling _PRINT_STRING.
 * The font can begin no earlier than tile index 0x20
 */
#define bios_font_tile_base (*((u16 *) _BIOS_FONT_TILE_BASE))

/**
 * @def bios_plane_width
 * @brief Cached value of the plane width as defined in VDP reg. 0x10.
 * @ingroup bios_vdp
 * @sa _BIOS_VDP_DEFAULT_PLANE_WIDTH
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
#define bios_plane_width (*((u16 *) _BIOS_VDP_DEFAULT_PLANE_WIDTH))

/**
 * @def bios_entity_routines
 * @brief Pointer to the jump table for entity processing
 * @sa _BIOS_ENTITY_ROUTINES
 * @ingroup bios_vdp
 */
#define bios_entity_routines (*((void const **) _BIOS_ENTITY_ROUTINES))

/**
 * @def bios_fadein_cram_index
 * @brief Palette offset on which the fade in palette should begin
 * @sa _BIOS_FADEIN_CRAM_INDEX
 * @ingroup bios_vdp
 */
#define bios_fadein_cram_index (*((u8 *) _BIOS_FADEIN_CRAM_INDEX))

/**
 * @def bios_fadein_pal_length
 * @brief Number of entries in the fade in palette
 * @sa _BIOS_FADEIN_PAL_LENGTH
 * @ingroup bios_vdp
 */
#define bios_fadein_pal_length (*((u8 *) _BIOS_FADEIN_PAL_LENGTH))

/**
 * @def bios_fadein_step
 * @brief Indicates if a fade in is still in progress
 * @sa _BIOS_FADEIN_STEP
 * @ingroup bios_vdp
 */
#define bios_fadein_step (*((volatile u16 *) _BIOS_FADEIN_STEP))

/**
 * @def _BIOS_FADEIN_TARGET_PAL
 * @brief Pointer to the target fade in palette
 * @sa _BIOS_FADEIN_TARGET_PAL
 * @ingroup bios_vdp
 */
#define bios_fadein_target_pal (*((u8 *) _BIOS_FADEIN_TARGET_PAL))

/**
 * @fn bios_entry
 * @brief Jump to internal user BIOS
 * @ingroup bios_system
 * @warning Register clobbers not yet documented!
 *
 * @details Leads to Mega CD title screen.
 */
static inline void bios_entry()
{
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_ENTRY));
}

/**
 * @fn bios_reset
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
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_RESET));
}

/**
 * @fn bios_init
 * @brief Initialize system
 * @ingroup bios_system
 * @warning Register clobbers not yet documented!
 *
 * @details Sets default VDP regs, clears VRAM, sets default vectors, inits
 * controllers, loads Z80 data, transfers Sub CPU BIOS, loads CD player program
 */
static inline void bios_init()
{
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_INIT));
}

/**
 * @fn bios_init_sp
 * @brief System init with stack pointer set
 * @ingroup bios_system
 * @warning Register clobbers not yet documented!
 *
 * @details Same as @ref bios_init but sets the stack pointer first
 */
static inline void bios_init_sp()
{
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_INIT_SP));
}

/**
 * @fn bios_vint_handler
 * @brief Vertical Blank interrupt handler
 * @ingroup bios_int
 *
 * @details Copies GA comm registers to the RAM
 * mirrors, sends INT2 (VINT ocurred) to Sub CPU, updates VDP palette from
 * RAM, calls VINT_USER, updates IO (controllers)
 */
static inline void bios_vint_handler()
{
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_VINT_HANDLER));
}

/**
 * @fn bios_set_hint
 * @brief Sets the HINT vector
 * @ingroup bios_int
 *
 * @details Sets the HINT vector in the system jump table, and sets the Gate
 * Array HINT register to the system jump table entry, and enables the interrupt
 * on the VDP
 *
 * @note The VDP register cache will be updated
 */
static inline void bios_set_hint(void * hint_handler)
{
	register u32 A1 asm("a1") = (u32) hint_handler;
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_SET_HINT), "a"(A1)
		: "cc");
}

/**
 * @fn bios_update_inputs
 * @brief Update state of P1/P2 controllers
 * @ingroup bios_input
 */
static inline void bios_update_inputs()
{
	asm(
		"\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BIOS_UPDATE_INPUTS)
		: "d6", "d7", "a5");
}

typedef enum ControllerType
{
	Joypad = CONTROLLER_JOYPAD,
	MegaMouse = CONTROLLER_MEGAMOUSE,
	MultiTap = CONTROLLER_MULTITAP
} ControllerType;

/**
 * @def bios_detect_controller
 * @brief Detect the type of controller plugged in
 * @ingroup bios_input
 * @warning Register clobbers not yet documented!
 *
 * @note
 * This is called during normal system init and with the results stored
 * in @ref bios_joy1_type and @ref bios_joy2_type
 */
static inline u8 bios_detect_controller(u8 * io_data_port)
{
	register u32 A6 asm("a6") = (u32) io_data_port;
	register u8 D6 asm("d6");
	asm(
		"\
  jsr %c0 \n\
		"
		: "=d"(D6)
		: "i"(_BIOS_DETECT_CONTROLLER), "a"(A6)
		: "cc");

	return D6;
}

/**
 * @fn bios_clear_vram
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
	asm(
		"\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BIOS_CLEAR_VRAM)
		: "d0", "d1", "d2", "d3");
}

/**
 * @fn bios_clear_tables
 * @brief Clear nametables and sprite list
 * @ingroup bios_vdp
 *
 * @note This works only with the Boot ROM default VRAM layout
 */
static inline void bios_clear_tables()
{
	asm(
		"\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BIOS_CLEAR_TABLES)
		: "d0", "d1", "d2", "d3");
}

/**
 * @fn bios_clear_vsram
 * @brief Clear VSRAM
 * @ingroup bios_vdp
 */
static inline void bios_clear_vsram()
{
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_CLEAR_VSRAM)
		: "d0", "d1", "d2");
}

/**
 * @fn bios_load_vdpregs_default
 * @brief Loads the Boot ROM default VDP register defaults
 * @ingroup bios_vdp
 * @details
		0x8004
			- HINT disabled
			- 9-bit (standard) color mode
		0x8124
			- Mega Drive graphics mode
			- NTSC (is almost certainly set to PAL on such hardware)
			- VINT enabled
			- Disable display
		0x9011
			- Plane size: 512x512 cells
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
			- HINT scanline count: 0
		0x8F02
			- Auto-increment 2
		0x9100
			- Window plane X position: 0
		0x9200
			- Window plane Y position: 0
 */
static inline void bios_load_vdpregs_default()
{
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_LOAD_VDPREGS_DEFAULT)
		: "d0", "d1", "a1", "a2");
}

/**
 * @fn bios_load_vdpregs
 * @brief Load values into multiple VDP registers
 * @ingroup bios_vdp
 *
 * @details Register data is an array of word sized values,
 * where the upper byte is the register ID
 * (e.g. 80, 81, etc) and the lower byte is
 * the value, with the list terminated by 0.
 */
static inline void bios_load_vdpregs(VDPREG const * vdp_reg_data)
{
	register u32 A1 asm("a1") = (u32) vdp_reg_data;

	asm volatile(
		"\
  jsr %p1 \n\
		"
		: "+a"(A1)
		: "i"(_BIOS_LOAD_VDPREGS), "a"(A1)
		: "cc", "d0", "d1", "a2");
}

/**
 * @fn bios_vdp_fill
 * @brief Fill a region of VDP memory with a value
 * @ingroup bios_vdp
 *
 * @details This is a simple data transfer via the VDP data port rather than
 * DMA.
 */
static inline void bios_vdp_fill(u32 vdpptr, u16 length, u16 value)
{
	register u32 D0 asm("d0") = vdpptr;
	register u16 D1 asm("d1") = length;
	register u16 D2 asm("d2") = value;
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_VDP_FILL), "d"(D0), "d"(D1), "d"(D2));
}

/**
 * @fn bios_vdp_fill_clear
 * @brief Fill a region of VDP memory with 0
 * @ingroup bios_vdp
 *
 * @details This is a simple data transfer via the VDP data port rather than
 * DMA.
 */
static inline void bios_vdp_fill_clear(u32 vdpptr, u16 length)
{
	register u32 D0 asm("d0") = vdpptr;
	register u16 D1 asm("d1") = length;
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_VDP_FILL_CLEAR), "d"(D0), "d"(D1)
		: "d2");
}

/**
 * @fn bios_dma_fill_clear
 * @brief Fill a region of VDP memory with zero
 * @ingroup bios_vdp
 * @sa _BIOS_DMA_FILL_CLEAR
 */
static inline void bios_dma_fill_clear(u32 vdpptr, u16 length)
{
	register u32 D0 asm("d0") = vdpptr;
	register u16 D1 asm("d1") = length;
	asm(
		"\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BIOS_DMA_FILL_CLEAR), "d"(D0), "d"(D1)
		: "d2", "d3");
}

/**
 * @fn bios_dma_fill
 * @brief Fill a region of VDP memory with a value
 * @sa _BIOS_DMA_FILL
 * @ingroup bios_vdp
 */
static inline void bios_dma_fill(u32 vdpptr, u16 length, u16 value)
{
	register u32 D0 asm("d0") = vdpptr;
	register u16 D1 asm("d1") = length;
	register u16 D2 asm("d2") = value;
	asm(
		"\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BIOS_DMA_FILL), "d"(D0), "d"(D1), "d"(D2)
		: "d3");
}

/**
 * @fn bios_load_map
 * @brief Fill a region of a nametable with map data
 * @sa _BIOS_LOAD_MAP
 * @ingroup bios_vdp
 *
 * @details The map data should be an array of word values in the standard
 * nametable entry format.
 */
static inline void bios_load_map(u32 const vdpptr, u16 const width, u16 const height, void const * map)
{
	register u32 D0 asm("d0") = vdpptr;
	register u16 D1 asm("d1") = width;
	register u16 D2 asm("d2") = height;
	register u32 A1 asm("a1") = (u32) map;

	asm volatile(
		"\
  jsr %p1 \n\
		"
		: "+d"(D2)
		: "i"(_BIOS_LOAD_MAP), "d"(D0), "d"(D1), "d"(D2), "a"(A1)
		: "d3", "a5", "cc");
}

/**
 * @fn bios_set_hint_workram
 * @brief Sets the HINT vector for a Work RAM destination
 * @sa _BIOS_SET_HINT_WORKRAM
 * @ingroup bios_int
 *
 * @details
 * Sets the specified vector in the system jump table, and sets the
 * Gate Array HINT register to the specified vector, and enables the interrupt
 * on the VDP.
 *
 * This is functionally identical to @ref bios_set_hint, however this version
 * sets the GA HINT register directly to the specified vector. Since the GA
 * register is only 16 bits, it uses only the lower word of the address and
 * expects the routine to be locaed in Work RAM, i.e. 0xFFxxxx. This means if
 * the specified HINT routine is located elsewhere (such as Word RAM), you must
 * use @ref bios_set_hint instead.
 *
 * @details
 * The VDP register buffer (_BIOS_VDP_REGS) is updated with this call.
 */
static inline void bios_set_hint_workram(void * hint_handler)
{
	register u32 A1 asm("a1") = (u32) hint_handler;

	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_SET_HINT_WORKRAM), "a"(A1)
		: "cc");
}

/**
 * @fn bios_disable_hint
 * @brief Disables horizontal interrupts on the VDP
 * @ingroup bios_int
 *
 * @details The VDP register buffer (VDP_REGS) is updated with this call.
 */
static inline void bios_disable_hint()
{
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_DISABLE_HINT)
		: "cc");
}

/**
 * @sa _BIOS_GFX_DECOMP
 * @ingroup bios_cmp
 */
static inline void bios_gfx_decomp(u8 const * data)
{
	register u32 a1_data asm("a1") = (u32) data;
	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_GFX_DECOMP), "a"(a1_data));
}

/**
 * @sa _BIOS_VDP_DISP_ENABLE
 * @ingroup bios_vdp
 */
static inline void bios_vdp_disp_enable()
{
	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_VDP_DISP_ENABLE));
}

/**
 * @sa _BIOS_VDP_DISP_DISABLE
 * @ingroup bios_vdp
 */
static inline void bios_vdp_disp_disable()
{
	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_VDP_DISP_DISABLE));
}

/**
 * @fn bios_vint_wait_default
 * @brief Wait for VBLANK interrupt with default flags
 * @ingroup bios_int
 * @sa _BIOS_VINT_WAIT_DEFAULT
 *
 * @details This will set the default VINT flags (copy sprite list & call
 * VINT_USER) before waiting for VINT
 *
 * @note This will also make a call to _PRNG
 *
 * @warning This will enable all interrupts before waiting for the VINT!
 */
static inline void bios_vint_wait_default()
{
	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_VINT_WAIT_DEFAULT)
		: "d0");
}

/**
 * @fn bios_vint_wait
 * @brief Wait for VBLANK interrupt
 * @ingroup bios_int
 * @sa _BIOS_VINT_WAIT
 *
 * @note This will also make a call to _PRNG
 *
 * @warning This will enable all interrupts before waiting for the VINT!
 */
static inline void bios_vint_wait(u8 flags)
{
	register u8 D0 asm("d0") = flags;
	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_VINT_WAIT), "d"(D0));
}

/**
 * @fn bios_pal_fadeout
 * @brief Fade a range of the color palette to black
 * @ingroup bios_vdp
 *
 * @param[in] D0.w Palette index (*byte* offset, not word!)
 * @param[in] D1.w Length (In *words*, not bytes!)
 * @param[out] Z Palette incomplete
 *
 * @details If Z is set, the palette fade process is not yet complete
 * and subroutine should be called again on the next loop iteration.
 *
 * @note Sets the palette update flag on _GFX_REFRESH
 */
static inline bool bios_pal_fadeout(u8 palette_index, u8 length)
{
	register u16 D0 asm("d0") = (u16) (palette_index << 1);
	register u16 D1 asm("d1") = (u16) length;

	asm goto(
		"\
  		jsr %c0 \n\
			beq %l[fade_complete] \n\
		"
		:
		: "i"(_BIOS_PAL_FADEOUT), "d"(D0), "d"(D1)
		: "cc"
		: fade_complete);

	return false;

fade_complete:
	return true;
}

/**
 * @fn bios_load_font_defaults
 * @brief Load the internal 1bpp ASCII font with default settings
 * @ingroup bios_misc
 *
 * @details
 * This will place the tiles starting at index 0x20, making it compatible with
 * _PRINT_STRING, and sets the font color to index 1.
 */
static inline void bios_load_font_defaults()
{
	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_LOAD_FONT_DEFAULTS)
		: "d0", "d1", "d2", "d3", "d4", "a1", "a5");
};

/**
 * @fn bios_load_1bpp_tiles
 * @brief Load the 1bpp graphics into VDP
 * @ingroup bios_misc
 */
static inline void bios_load_1bpp_tiles(void * chr_data, u16 tile_count, VDPCMD dest, u32 color_pattern)
{
	register u32 D0 asm("d0") = dest;
	register u32 D1 asm("d1") = color_pattern;
	register u16 D2 asm("d2") = tile_count;
	register u32 A1 asm("a1") = (u32) chr_data;

	asm volatile(
		"\
			jsr %p2 \n\
		"
		: "+d"(D2), "+a"(A1)
		: "i"(_BIOS_LOAD_1BPP_TILES), "a"(A1), "d"(D0), "d"(D1), "d"(D2)
		: "d3", "d4", "a5");
};

/**
 * @fn bios_input_delay
 * @brief Generates a brief delay after initially pressing the D-pad
 * @ingroup bios_input
 *
 * @param[in] A1.l Pointer to byte which will hold the output D-pad value
 * @param[in] D0.w If 0, use P1 input; if non-zero, use P2 input
 * @ingroup bios_input
 *
 * @details
 * This is useful for working with cursors on menus as it creates a brief
 * pause when holding a D-pad direction. You'll need to test against the
 * output variable (set in a1) rather than the standard input mirror in order
 * to use this correctly.
 */
static inline void bios_input_repeat_delay(u8 * input, bool use_2p)
{
	register u32 A1 asm("a1") = (u32) input;
	register u16 D0 asm("d0") = (u16) use_2p;

	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_INPUT_REPEAT_DELAY), "a"(A1), "d"(D0)
		: "cc", "d1", "a5");
}

/**
 * @fn bios_clear_comm
 * @brief Clears all Gate Array communication registers
 * @ingroup bios_comm
 *
 * @details This clears the COMFLAGS and COMCMD registers directly as well as
 * their RAM buffers
 */
static inline void bios_clear_comm()
{
	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_CLEAR_COMM)
		: "d0", "a6");
}

/**
 * @fn bios_print
 * @brief Displays an ASCII string
 * @ingroup bios_misc
 *
 * @param[in] A1.l Pointer to string
 * @param[in] D0.l VRAM destination (vdpptr)
 *
 * @details Strings are terminated with 0xFF and use 0x00 for newline.
 * The value in _BIOS_FONT_TILE_BASE is added to each character byte, but no
 * other transformations are done. This means that the font must begin at index
 * 0x20 at the earliest (where _BIOS_FONT_TILE_BASE is 0). Note that this can
 * only use palette line 0.
 */
static inline void bios_print(char const * string, VDPCMD pos)
{
	register u32 A1 asm("a1") = (u32) string;
	register u32 D0 asm("d0") = pos;

	asm volatile(
		"\
			jsr %p1 \n\
		"
		: "+a"(A1)
		: "i"(_BIOS_PRINT), "a"(A1), "d"(D0)
		: "d1", "d2", "a5");
};

/**
 * @fn bios_nmtbl_fill
 * @brief Fill a region of a nametable with a value
 * @param[in] D0.l Address (vdpptr format)
 * @param[in] D1.w Width
 * @param[in] D2.w Height
 * @param[in] D3.w Value
 * @ingroup bios_vdp
 */
static inline void bios_nmtbl_fill(VDPCMD pos, u16 width, u16 height, u16 value)
{
	register u32 D0 asm("d0") = pos;
	register u32 D1 asm("d1") = width;
	register u32 D2 asm("d2") = height;
	register u32 D3 asm("d3") = value;

	asm volatile(
		"\
    	move.l a6, -(sp) \n\
    	jsr %p1 \n\
    	move.l (sp)+, a6 \n\
  	"
		: "+d"(D2)
		: "i"(_BIOS_NMTBL_FILL), "d"(D0), "d"(D1), "d"(D2), "d"(D3)
		: "cc", "d5", "a5");
};

/**
 * @fn bios_dma_xfer
 * @brief Performs a data transfer to VRAM via DMA
 * @param[in] D0.l VRAM destination address (vdpptr format)
 * @param[in] D1.l Source address
 * @param[in] D2.w Length (in words)
 * @ingroup bios_vdp
 */
static inline void bios_dma_xfer(VDPCMD dest, u8 const * source, u16 length)
{
	register u32 D0 asm("d0") = dest;
	register u32 D1 asm("d1") = (u32) source;
	register u16 D2 asm("d2") = length;

	asm(
		"\
  		move.l a6, -(sp) \n\
  		jsr %c0 \n\
  		move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BIOS_DMA_XFER), "d"(D0), "d"(D1), "d"(D2)
		: "d3");
};

/**
 * @fn bios_dma_xfer_word_ram
 * @brief Performs a data transfer from Word RAM to VRAM via DMA
 * @param[in] D0.l VRAM destination (vdpptr format)
 * @param[in] D1.l Source address
 * @param[in] D2.w Length (in words)
 * @ingroup bios_vdp
 *
 * @details There is a well-documented issue with performing a DMA transfer from
 * Word RAM to VRAM which must be accounted for by writing the final word of
 * data to the data port. This subroutine takes care of that extra step.
 */
static inline void bios_dma_xfer_word_ram(VDPCMD const dest, void const * source, u16 const length)
{
	register u32 D0 asm("d0") = dest;
	register u32 D1 asm("d1") = (u32) source;
	register u16 D2 asm("d2") = length;

	asm volatile(
		"\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BIOS_DMA_XFER_WORD_RAM), "d"(D0), "d"(D1), "d"(D2)
		: "cc", "d3");
};

/**
 * @fn bios_dma_copy
 * @brief Copy data within VRAM via DMA
 * @param[in] D0.l Destination VRAM address (vdpptr)
 * @param[in] D1.w Source VRAM address
 * @param[in] D2.w Length
 * @ingroup bios_vdp
 */
static inline void bios_dma_copy(u32 vdpptr_dest, u16 source, u16 length)
{
	register u32 D0 asm("d0") = vdpptr_dest;
	register u16 D1 asm("d1") = source;
	register u16 D2 asm("d2") = length;

	asm volatile(
		"\
  move.l a6, -(sp) \n\
  jsr %c0 \n\
  move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BIOS_DMA_COPY), "d"(D0), "d"(D1), "d"(D2)
		: "d3");
};

/**
 * @fn bios_copy_sprlist
 * @brief Copies sprite list buffer to VDP via DMA
 * @ingroup bios_vdp
 *
 * @details This uses the default Boot ROM VRAM layout (i.e. sprite list at
 * 0xB800) Will only perform the copy of bit 0 of VINT_FLAGS is set (so this is
 * likely intended to be called from VINT, probably VINT_USER)
 */
static inline void bios_copy_sprlist()
{
	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_COPY_SPRLIST)
		: "d4", "a4");
};

/**
 * @fn bios_clear_ram
 * @brief Clear a region of memory
 * @param[in] A0.l Pointer to memory region
 * @param[in] D7.l Size to clear (in longs) MINUS 1
 * @ingroup bios_misc
 */
static inline void bios_clear_ram(void * address, u32 long_count)
{
	register u32 A0 asm("a0") = (u32) address;
	register u32 D7 asm("d7") = long_count;

	asm(
		"\
			move.l a6, -(sp) \n\
			jsr %c0 \n\
			move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BIOS_CLEAR_RAM), "d"(D7), "a"(A0));
};

/**
 * @fn bios_load_pal
 * @brief Load palette to RAM buffer
 * @param[in] A1.l Pointer to palette data structure
 * @ingroup bios_vdp
 *
 * @details The color palette is loaded but the "Palette Update" flag is not set
 */
static inline void bios_load_pal(Palette const * pal_data)
{
	register u32 A1 asm("a1") = (u32) pal_data;

	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_LOAD_PAL), "a"(A1)
		: "d0");
};

/**
 * @fn bios_load_pal_update
 * @brief Load palette to RAM buffer and set Palette Update flag
 * @param[in] A1.l Pointer to palette data structure
 * @ingroup bios_vdp
 */
static inline void bios_load_pal_update(Palette const * pal_data)
{
	register u32 A1 asm("a1") = (u32) pal_data;

	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_LOAD_PAL_UPDATE), "a"(A1)
		: "d0");
};

/**
 * @fn bios_copy_pal
 * @brief Transfer palette cache to CRAM via DMA
 * @ingroup bios_vdp
 *
 * @note Uses bios_vdp_update_flags
 */
static inline void bios_copy_pal()
{
	asm(
		"\
			jsr %c0 \n\
		"
		:
		: "i"(_BIOS_COPY_PAL)
		: "cc", "a4", "d4");
};

/**
 * @fn bios_process_entities
 * @brief Update/display sprite objects
 * @param[in] A0.l Pointer to object array
 * @param[in] A1.l Pointer to sprite list buffer
 * @param[in] D0.w Number of objects
 * @param[in] D1.w Object size
 * @ingroup bios_vdp
 */
static inline void bios_process_entities(
	struct Entity const * obj_array, Sprite const * sprtbl_cache, u16 const obj_count, u16 const obj_size)
{
	register u32 A0 asm("a0") = (u32) obj_array;
	register u32 A1 asm("a1") = (u32) sprtbl_cache;
	register u16 D0 asm("d0") = (u16) obj_count;
	register u16 D1 asm("d1") = (u16) obj_size;

	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_PROCESS_ENTITIES), "a"(A0), "a"(A1), "d"(D0), "d"(D1)
		: "d2", "d3", "d4", "d6", "a2");
};

/**
 * @fn bios_prng_mod
 * @brief Generate a new random number limited with the given modulus
 * @param[in] D0.w Modulus
 * @param[out] D0.w Random number between 0 and modulus
 * @ingroup bios_misc
 *
 * @details The modulus can be used to specify an upper boundary of the random
 * number. To specify a minimum boundary, the modulus should be set like so:
 *   mod = (upper - lower) + 1
 * Finally, the lower bound should be added to the random number result.
 * Psuedo code:
 *   num = prng_mod((upper - lower) + 1) + lower
 */
static inline u16 bios_prng_mod(u16 const modulo)
{
	// TODO can we use the same variable here for in/out?
	register u16 d0_modulo asm("d0") = modulo;
	register u16 d0_random asm("d0");

	asm(
		"\
  jsr %p1 \n\
		"
		: "+d"(d0_random)
		: "i"(_BIOS_PRNG_MOD), "d"(d0_modulo)
		: "d1");

	return d0_random;
};

/**
 * @fn bios_prng
 * @brief Generate a new random number
 * @ingroup bios_misc
 *
 * @details The number will be stored in bios_random
 */
static inline void bios_prng()
{
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_PRNG)
		: "d0");
};

/**
 * @fn bios_set_fadein_pal
 * @brief Sets the target color palette for a fadein
 * @param[in] A1.l Pointer to target palette structure
 * @ingroup bios_vdp
 *
 * @note This must be called before using bios_pal_fadein
 */
static inline void bios_set_fadein_pal(Palette const * palette)
{
	register u32 a1_BIOS_PALETTE asm("a1") = (u32) palette;
	register u32 a1_change asm("a1");

	asm volatile(
		"\
  jsr %p1 \n\
		"
		: "=a"(a1_change)
		: "i"(_BIOS_SET_FADEIN_PAL), "a"(a1_BIOS_PALETTE));
};

/**
 * @fn bios_pal_fadein
 * @brief Fade a range of the color palette from black
 * @ingroup bios_vdp
 *
 * @details The fade-in routine works a little bit differently from the
 * fade-out counterpart. Instead of checking the Z flag, the value of
 * _BIOS_FADEIN_STEP should be checked. If it is > 0, the fade is not yet
 * complete.
 *
 * This may be a bug in the implementation. Using the Z flag to check for
 * completion *could* work, as the code checks whether _BIOS_FADEIN_STEP is
 * zero with a TST opcode. However, this is followed by setting the VDP flags
 * for a palette update, which will set the Z flag if palette update flag was
 * not already set.
 */
static inline void bios_pal_fadein()
{
	asm(
		"\
  jsr %c0 \n\
		"
		:
		: "i"(_BIOS_PAL_FADEIN));
}

/**
 * For use with bios_dma_queue
 */
typedef struct DmaTransfer
{
	u16 length;
	u32 vdpptr;
	u32 source;
} DmaTransfer;

/**
 * @brief Transfer queued VDP bound data via DMA
 * @param[in] A1.l Pointer to queue
 * @ingroup bios_vdp
 *
 * @details The queue is an array of DMA transfer entries in this format:
 *     0.w Data length
 *     2.l Destination (vdpptr)
 *     6.l Source address
 * The list should be terminated with a 0 word. Note that this system is
 * extremely basic and does not account for DMA bandwidth, etc. Moreover, no
 * array management is done and the list will need to be cleared by the user.
 */
static inline void bios_dma_queue(DmaTransfer const * queue)
{
	register u32 a1_queue asm("a1") = (u32) queue;
	register u32 a1_change asm("a1");

	asm volatile(
		"\
  move.l a6, -(sp) \n\
  jsr %p1 \n\
  move.l (sp)+, a6 \n\
		"
		: "=a"(a1_change)
		: "i"(_BIOS_DMA_QUEUE), "a"(a1_queue)
		: "d0", "d1", "d2", "d3");
}

#endif

/**
 * @file bootlib.h
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

#ifndef MEGADEV__CD_MAIN__BLIB_H
#define MEGADEV__CD_MAIN__BLIB_H

#include "main/bootlib_def.h"
#include "types.h"
#include "vdp.h"

struct SpriteObject
{
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

/**
 * @struct Palette
 * cram_offset - address in CRAM to load palette - that is, the palette index
 * length - the number of colors in the palette to load MINUS ONE
 *
 * @ingroup blib_vdp
 */
typedef struct Palette
{
	u8 cram_offset;
	u8 length; // length in words (i.e. color entries) MINUS ONE
	s16 colors[];
} Palette;

/**
 * @def BLIB_DECOMP
 * @brief Work RAM for graphics decompression routines
 * Size: 0x200 bytes
 */
#define BLIB_DECOMP ((u8 *) _BLIB_DECOMP)

/**
 * @def BLIB_SPRLIST
 * @brief Sprite list buffer
 * @ingroup blib_vdp
 *
 * @details
 * Size: 0x280 bytes
 */
#define BLIB_SPRLIST (*((Sprite (*)[80]) _BLIB_SPRLIST))
//#define BLIB_SPRLIST ((Sprite *)_BLIB_SPRLIST)

/**
 * @def BLIB_PALETTE
 * @brief CRAM (palette) buffer
 * @ingroup blib_vdp
 *
 * @details
 * Size: 0x80 bytes
 */
#define BLIB_PALETTE (*((s16 (*)[64]) _BLIB_PALETTE))

/**
 * @def BLIB_PAL0
 * @brief Palette line #0 buffer
 * @ingroup blib_vdp
 */
#define BLIB_PAL0 (*((s16 (*)[16]) _BLIB_PAL0))

/**
 * @def BLIB_PAL1
 * @brief Palette line #1 buffer
 * @ingroup blib_vdp
 */
#define BLIB_PAL1 (*((s16 (*)[16]) _BLIB_PAL1))

/**
 * @def BLIB_PAL2
 * @brief Palette line #2 buffer
 * @ingroup blib_vdp
 */
#define BLIB_PAL2 (*((s16 (*)[16]) _BLIB_PAL2))

/**
 * @def BLIB_PAL3
 * @brief Palette line #3 buffer
 * @ingroup blib_vdp
 */
#define BLIB_PAL3 (*((s16 (*)[16]) _BLIB_PAL3))

/**
 * @var void* BLIB_VINT_EX_PTR
 * @brief Pointer to the VINT_EX routine used in the Boot ROM VINT handler.
 */
#define BLIB_VINT_EX_PTR ((volatile void *(*) ) _BLIB_VINT_EX_PTR)

/**
 * @var u16* BLIB_VDPREGS
 * @brief VDP registers buffer
 * @ingroup blib_vdp
 *
 * @details Buffer of all VDP registers (except DMA regs), making up 19
 * entries. You will need to keep these updated manually unless you use
 * _BLIB_LOAD_VDPREGS
 *
 * Size: 16bit * 19 = 0x26 bytes
 *
 * @ingroup blib_vdp
 */
#define BLIB_VDPREGS (*((u16 (*)[19]) _BLIB_VDPREGS))

/**
 * @var u8 BLIB_COMFLAGS_MAIN
 * @brief GA comm flags for Main CPU buffer
 */
#define BLIB_COMFLAGS_MAIN ((volatile u8 *) _BLIB_COMFLAGS_MAIN)

/**
 * @var u8 const BLIB_COMFLAGS_SUB
 * @brief GA comm flags for Sub CPU buffer
 */
#define BLIB_COMFLAGS_SUB ((volatile u8 const *) _BLIB_COMFLAGS_SUB)

/**
 * @var u16* BLIB_COMCMD
 * @brief Array of cached GA COMCMD (Main -> Sub) registers

 */
#define BLIB_COMCMD (*((volatile u16 (*)[8]) _BLIB_COMCMD))

/**
 * @var u16 BLIB_COMSTAT
 * @brief Array of cached GA COMSTAT (Main -> Sub) registers
 */
#define BLIB_COMSTAT (*((volatile u16 const(*)[8]) _BLIB_COMSTAT))

/**
 * @var u16 BLIB_JOY1_MOUSE_DATA
 */
#define BLIB_JOY1_MOUSE_DATA (*((u16 *) _BLIB_JOY1_MOUSE_DATA))

/**
 * @var u16 BLIB_JOY1_MOUSE_DX
 * @brief Mouse delta X
 */
#define BLIB_JOY1_MOUSE_DX (*((u16 *) _BLIB_JOY1_MOUSE_DX))

/**
 * @var u16 BLIB_JOY1_MOUSE_DY
 * @brief Mouse delta Y
 */
#define BLIB_JOY1_MOUSE_DY (*((u16 *) _BLIB_JOY1_MOUSE_DY))

/**
 * @var u16 BLIB_JOY2_MOUSE_DATA
 */
#define BLIB_JOY2_MOUSE_DATA (*((u16 *) _BLIB_JOY2_MOUSE_DATA))

/**
 * @var u16 BLIB_JOY2_MOUSE_DX
 * @brief Mouse delta X
 */
#define BLIB_JOY2_MOUSE_DX (*((u16 *) _BLIB_JOY2_MOUSE_DX))

/**
 * @var u16 BLIB_JOY2_MOUSE_DY
 * @brief Mouse delta Y
 */
#define BLIB_JOY2_MOUSE_DY (*((u16 *) _BLIB_JOY2_MOUSE_DY))

/**
 * @var u8 BLIB_JOY1_TYPE
 */
#define BLIB_JOY1_TYPE (*((u8 *) _BLIB_JOY1_TYPE))

/**
 * @var u8 BLIB_JOY2_TYPE
 */
#define BLIB_JOY2_TYPE ((u8 *) _BLIB_JOY2_TYPE)

/**
 * @var u8 BLIB_JOY1_HOLD
 * @brief Port 1 controller latched input
 */
#define BLIB_JOY1_HOLD (*((volatile u8 const *) _BLIB_JOY1_HOLD))

/**
 * @var u8 BLIB_JOY1_PRESS
 * @brief Port 1 controller single press input
 */
#define BLIB_JOY1_PRESS (*((volatile u8 const *) _BLIB_JOY1_PRESS))

/**
 * @var u8 BLIB_JOY2_HOLD
 * @brief Port 2 controller latched input
 */
#define BLIB_JOY2_HOLD (*((volatile u8 const *) _BLIB_JOY2_HOLD))

/**
 * @var u8 BLIB_JOY2_PRESS
 * @brief Port 2 controller single press input
 */
#define BLIB_JOY2_PRESS (*((volatile u8 const *) _BLIB_JOY2_PRESS))

/**
 * @var u8 BLIB_JOY1_DELAY
 * @note For use with @ref BLIB_INPUT_DELAY
 */
#define BLIB_JOY1_DELAY (*((volatile u8 *) _BLIB_JOY1_DELAY))

/**
 * @var u8 BLIB_JOY2_DELAY
 * @note For use with @ref BLIB_INPUT_DELAY
 */
#define BLIB_JOY2_DELAY (*((volatile u8 *) _BLIB_JOY2_DELAY))

/**
 * @var u8 BLIB_VINT_FLAGS
 * @details Used by the Boot ROM VINT/VINT_WAIT routines for graphics updates
 * during vblank.
 *
 * Bit 0 - Copy sprite list to VDP
 * Bit 1 - Call VINT_EX vector during vblank
 */
#define BLIB_VINT_FLAGS (*((u8 *) _BLIB_VINT_FLAGS))

/**
 * @var u8 BLIB_VINT_COUNTER
 * @brief Incremented by 1 on each vertical blank (VINT)
 */
#define BLIB_VINT_COUNTER (*((volatile u8 *) _BLIB_VINT_COUNTER))

/**
 * @var u8 BLIB_VINT_SKIP_GFX
 *
 * @details Skips these operations during vblank:
 * CRAM copy, sprite list copy, increment vblank counter
 *
 * Will still perform IO updates, though
 */
#define BLIB_VINT_SKIP_GFX (*((u8 *) _BLIB_VINT_SKIP_GFX))

/**
 * @var u8 BLIB_VDP_UPDATE_FLAGS
 */
#define BLIB_VDP_UPDATE_FLAGS (*((u8 *) _BLIB_VDP_UPDATE_FLAGS))

/**
 * @var u16 BLIB_RANDOM
 * @brief Contains a random 16 bit value
 *
 * @note You must make a call to @ref BLIB_PRNG on each vblank in order to
 * use this!
 */
#define BLIB_RANDOM (*((u16 const *) _BLIB_RANDOM))

/**
 * @var u16 BLIB_FONT_TILE_BASE
 * @details The value added to each character byte when calling _PRINT_STRING.
 * The font can begin no earlier than tile index 0x20
 */
#define BLIB_FONT_TILE_BASE (*((u16 *) _BLIB_FONT_TILE_BASE))

/**
 * @enum PlaneWidthTiles
 * @brief Use with @ref _BLIB_PLANE_WIDTH to represent the width in tiles instead
 * of bytes
 */
typedef enum PlaneWidthTiles
{
	Width32 = 64,
	Width64 = 128,
	Width128 = 256
} PlaneWidthTiles;

/**
 * @var u16 BLIB_PLANE_WIDTH
 * @brief Cached value of the plane width as defined in VDP reg. 0x10.
 *
 * @note This value is stored as BYTES, which is effectively the width in
 * tiles * 2. Example: if the plane width is 32 in the register, this value
 * must be 64, and so on.
 * @note This needs to be manually updated each time the register is changed!!
 * @note Use the @ref PlaneWidthTiles enum to make it semantically clear what
 * the width is in tiles
 *
 * @ingroup blib_vdp
 */
#define BLIB_PLANE_WIDTH (*((u16 *) _BLIB_PLANE_WIDTH))

/**
 * @var u32 BLIB_SPRTBL_PTR
 */
#define BLIB_SPRTBL_PTR ((void *) _BLIB_SPRTBL_PTR)

/**
 * @var u32 BLIB_SPR_JMPTBL_PTR
 * @brief Pointer to the jump table for entity processing
 */
#define BLIB_SPR_JMPTBL_PTR ((u8 *) _BLIB_SPR_JMPTBL_PTR)

/**
 * @var u8 BLIB_FADEIN_PAL_INDEX
 * @brief Palette offset on which the fade in palette should begin
 */
#define BLIB_FADEIN_PAL_INDEX (*((u8 *) _BLIB_FADEIN_PAL_INDEX))

/**
 * @var u8 BLIB_FADEIN_PAL_LENGTH
 * @brief Number of entries in the fade in palette
 */
#define BLIB_FADEIN_PAL_LENGTH (*((u8 *) _BLIB_FADEIN_PAL_LENGTH))

/**
 * @var u16 BLIB_FADEIN_STEP
 * @brief Indicates if a fade in is still in progress
 */
#define BLIB_FADEIN_STEP (*((volatile u16 *) _BLIB_FADEIN_STEP))

/**
 * @var u32 _BLIB_FADEIN_TARGET_PAL_PTR
 * @brief Pointer to the target fade in palette
 */
#define BLIB_FADEIN_TARGET_PAL_PTR (*((u8 *) _BLIB_FADEIN_TARGET_PAL_PTR))

/*
	Boot ROM function wrappers
*/

/**
 * @fn blib_cdbios_entry
 * @brief Jump to internal CD BIOS
 * @ingroup blib_system
 * @warning Register breaks not yet documented!
 *
 * @details Leads to Mega CD title screen.
 */
static inline void blib_cdbios_entry()
{
	asm("jsr %p0" ::"i"(_BLIB_CDBIOS_ENTRY));
}

/**
 * @fn blib_reset
 * @brief Perform a system reset
 * @ingroup blib_system
 * @warning Register breaks not yet documented!
 *
 * @details On cold boot, checks for TMSS, clears RAM, writes
 * Z80 program, writes PSG data, then moves on to checking hardware region and
 * clearing VRAM.
 *
 * On warm boot, skips directly to region check, then jumps to
 * the reset vector. Leads to Mega CD title screen.
 *
 */
static inline void blib_reset()
{
	asm("jsr %p0" ::"i"(_BLIB_RESET));
}

/**
 * @fn blib_init
 * @brief Initialize system
 * @ingroup blib_system
 * @warning Register breaks not yet documented!
 *
 * @details Sets default VDP regs, clears VRAM, sets default vectors, inits
 * controllers, loads Z80 data, transfers Sub CPU BIOS, loads CD player program
 */
static inline void blib_init()
{
	asm("jsr %p0" ::"i"(_BLIB_INIT));
}

/**
 * @fn blib_init_sp
 * @brief System init with stack pointer set
 * @ingroup blib_system
 * @warning Register breaks not yet documented!
 *
 * @details Same as @ref blib_init but sets the stack pointer first
 */
static inline void blib_init_sp()
{
	asm("jsr %p0" ::"i"(_BLIB_INIT_SP));
}

/**
 * @fn blib_vint_handler
 * @brief Vertical Blank interrupt handler
 * @ingroup blib_interrupts
 *
 * @details Copies GA comm registers to the RAM
 * mirrors, sends INT2 (VINT ocurred) to Sub CPU, updates VDP palette from
 * RAM, calls VINT_EX, updates IO (controllers)
 */
static inline void blib_vint_handler()
{
	asm("jsr %p0" ::"i"(_BLIB_VINT_HANDLER));
}

/**
 * @fn blib_set_hint
 * @brief Sets the HINT vector
 * @ingroup blib_interrupts
 *
 * @details Sets the HINT vector in the system jump table, and sets the Gate
 * Array HINT register to the system jump table entry, and enables the interrupt
 * on the VDP
 *
 * @note The VDP register cache will be updated
 */
static inline void blib_set_hint (void * hint_handler)
{
	register u32 A1 asm("a1") = (u32) hint_handler;
	asm("jsr %p0" ::"i"(_BLIB_SET_HINT), "a"(A1) : "cc");
}

/**
 * @fn blib_update_inputs
 * @brief Update state of P1/P2 controllers
 * @param[out] _BLIB_JOY1_PRESS
 * @param[out] _BLIB_JOY1_HOLD
 * @param[out] _BLIB_JOY2_PRESS
 * @param[out] _BLIB_JOY2_HOLD
 * @ingroup blib_input
 *
 */
static inline void blib_update_inputs()
{
	asm(
		"\
  	move.l a6, -(sp) \n\
		jsr %p0 \n\
		move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BLIB_UPDATE_INPUTS)
		: "d6", "d7", "a5");
}

typedef enum ControllerType
{
	Joypad = _JOYTYPE_JOYPAD,
	MegaMouse = _JOYTYPE_MEGAMOUSE,
	MultiTap = _JOYTYPE_MULTITAP
} ControllerType;

/**
 * @def blib_detect_controller
 * @brief Detect the type of controller plugged in
 * @ingroup blib_input
 * @warning Register breaks not yet documented!
 *
 * @note
 * This is called during normal system init and with the results stored
 * in @ref BLIB_JOY1_TYPE and @ref BLIB_JOY2_TYPE
 */
static inline u8 blib_detect_controller (u8 * io_data_port)
{
	register u32 A6 asm("a6") = (u32) io_data_port;
	register u8 D6 asm("d6");
	asm(
		"\
		jsr %p0 \n\
		"
		: "=d"(D6)
		: "i"(_BLIB_SET_HINT), "a"(A6)
		: "cc");

	return D6;
}

/**
 * @fn blib_clear_vram
 * @brief Clear all of VRAM and VSRAM via DMA Fill
 * @ingroup blib_vdp
 *
 * @note This does not clear CRAM.
 *
 */
static inline void blib_clear_vram()
{
	asm(
		"\
  	move.l a6, -(sp) \n\
		jsr %p0 \n\
		move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BLIB_CLEAR_VRAM)
		: "d0", "d1", "d2", "d3");
}

/**
 * @fn blib_clear_tables
 * @brief Clear nametables and sprite list
 * @ingroup blib_vdp
 *
 * @note This works only with the Boot ROM default VRAM layout
 */
static inline void blib_clear_tables()
{
	asm(
		"\
  	move.l a6, -(sp) \n\
		jsr %p0 \n\
		move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BLIB_CLEAR_TABLES)
		: "d0", "d1", "d2", "d3");
}

/**
 * @fn blib_clear_vsram
 * @brief Clear VSRAM
 * @ingroup blib_vdp
 */
static inline void blib_clear_vsram()
{
	asm("jsr %p0" ::"i"(_BLIB_CLEAR_VSRAM) : "d0", "d1", "d2");
}

/**
 * @fn blib_load_vdpregs_default
 * @brief Loads the Boot ROM default VDP register defaults
 * @ingroup blib_vdp
 */
static inline void blib_load_vdpregs_default()
{
	asm(
		"\
		jsr %p0 \n\
		"
		:
		: "i"(_BLIB_LOAD_VDPREGS_DEFAULT)
		: "d0", "d1", "a1", "a2");
}

/**
 * @fn blib_load_vdpregs
 * @brief Load values into multiple VDP registers
 * @ingroup blib_vdp
 *
 * @details Register data is an array of word sized values,
 * where the upper byte is the register ID
 * (e.g. 80, 81, etc) and the lower byte is
 * the value, with the list terminated by 0.
 */
static inline void blib_load_vdpregs (u16 const * vdp_reg_data)
{
	register u32 A1 asm("a1") = (u32) vdp_reg_data;

	asm volatile(
		"\
		jsr %p1 \n\
		"
		: "+a"(A1)
		: "i"(_BLIB_LOAD_VDPREGS), "a"(A1)
		: "cc", "d0", "d1", "a2");
}

/**
 * @fn blib_vdp_fill
 * @brief Fill a region of VDP memory with a value
 * @ingroup blib_vdp
 *
 * @details This is a simple data transfer via the VDP data port rather than
 * DMA.
 */
static inline void blib_vdp_fill (u32 vdpptr, u16 length, u16 value)
{
	register u32 d0_vdpptr asm("d0") = vdpptr;
	register u16 d1_length asm("d1") = length;
	register u16 d2_value asm("d2") = value;
	asm(
		"\
		jsr %p0 \n\
		"
		:
		: "i"(_BLIB_VDP_FILL), "d"(d0_vdpptr), "d"(d1_length), "d"(d2_value));
}

/**
 * @fn blib_vdp_fill_clear
 * @brief Fill a region of VDP memory with 0
 * @ingroup blib_vdp
 *
 * @details This is a simple data transfer via the VDP data port rather than
 * DMA.
 */
static inline void blib_vdp_fill_clear (u32 vdpptr, u16 length)
{
	register u32 d0_vdpptr asm("d0") = vdpptr;
	register u16 d1_length asm("d1") = length;
	asm("jsr %p0" ::"i"(_BLIB_VDP_FILL_CLEAR), "d"(d0_vdpptr), "d"(d1_length) : "d2");
}

/**
 * @fn blib_dma_fill_clear
 * @brief Fill a region of VDP memory with zero
 * @param[in] D0.l Address (vdpptr format)
 * @param[in] D1.w Length (in words)
 * @ingroup blib_vdp
 */
static inline void blib_dma_fill_clear (u32 vdpptr, u16 length)
{
	register u32 d0_vdpptr asm("d0") = vdpptr;
	register u16 d1_length asm("d1") = length;
	asm(
		"\
  	move.l a6, -(sp) \n\
		jsr %p0 \n\
		move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BLIB_DMA_FILL_CLEAR), "d"(d0_vdpptr), "d"(d1_length)
		: "d2", "d3");
}

/**
 * @fn blib_dma_fill
 * @brief Fill a region of VDP memory with a value
 * @param[in] D0.l Address (vdpptr format)
 * @param[in] D1.w Length (in words)
 * @param[in] D2.w Value
 * @ingroup blib_vdp
 */
static inline void blib_dma_fill (u32 vdpptr, u16 length, u16 value)
{
	register u32 d0_vdpptr asm("d0") = vdpptr;
	register u16 d1_length asm("d1") = length;
	register u16 d2_value asm("d2") = value;
	asm(
		"\
  	move.l a6, -(sp) \n\
		jsr %p0 \n\
		move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BLIB_DMA_FILL), "d"(d0_vdpptr), "d"(d1_length), "d"(d2_value)
		: "d3");
}

/**
 * @fn blib_load_map
 * @brief Fill a region of a nametable with map data
 * @param[in] D0.l VRAM Address (vdpptr format)
 * @param[in] D1.w Map width
 * @param[in] D2.w Map height
 * @param[in] A1.l Pointer to map data
 * @ingroup blib_vdp
 *
 * @details The map data should be an array of word values in the standard
 * nametable entry format.
 */
static inline void blib_load_map (u32 const vdpptr, u16 const width, u16 const height, void const * map)
{
	register u32 d0_vdpptr asm("d0") = vdpptr;
	register u16 d1_width asm("d1") = width;
	register u16 d2_height asm("d2") = height;
	register u32 a1_map asm("a1") = (u32) map;
	asm("jsr %p0" ::"i"(_BLIB_LOAD_MAP), "d"(d0_vdpptr), "d"(d1_width), "d"(d2_height), "a"(a1_map) : "d3", "a5");
}

/**
 * @sa _BLIB_SET_HINT_WORKRAM
 */
static inline void blib_set_hint_workram (void * hint_handler)
{
	register u32 a1_ptr asm("a1") = (u32) hint_handler;
	asm("jsr %p0" ::"i"(_BLIB_SET_HINT_WORKRAM), "a"(a1_ptr) : "cc");
}

/**
 * @fn blib_disable_hint
 * @brief Disables horizontal interrupts on the VDP
 *
 * GROUP: Interrupts
 *
 * @details The VDP register buffer (VDP_REGS) is updated with this call.
 */
static inline void blib_disable_hint()
{
	asm("jsr %p0" ::"i"(_BLIB_DISABLE_HINT) : "cc");
}

/**
 * @sa _BLIB_GFX_DECOMP
 */
static inline void blib_gfx_decomp (u8 const * data)
{
	register u32 a1_data asm("a1") = (u32) data;
	asm("jsr %p0" ::"i"(_BLIB_GFX_DECOMP), "a"(a1_data));
}

/**
 * @sa _BLIB_VDP_DISP_ENABLE
 */
static inline void blib_vdp_disp_enable()
{
	asm("jsr %p0" ::"i"(_BLIB_VDP_DISP_ENABLE));
}

/**
 * @sa _BLIB_VDP_DISP_DISABLE
 */
static inline void blib_vdp_disp_disable()
{
	asm("jsr %p0" ::"i"(_BLIB_VDP_DISP_DISABLE));
}

/**
 * @sa _BLIB_VINT_HANDLER_WAIT_DEFAULT
 */
static inline void blib_vint_wait_default()
{
	asm("jsr %p0" ::"i"(_BLIB_VINT_HANDLER_WAIT_DEFAULT) : "d0");
}

/**
 * @sa _BLIB_VINT_HANDLER_WAIT
 */
static inline void blib_vint_wait (u8 flags)
{
	register u8 d0_flags asm("d0") = flags;
	asm("jsr %p0" ::"i"(_BLIB_VINT_HANDLER_WAIT), "d"(d0_flags));
}

/**
 * @fn blib_pal_fadeout
 * @brief Fade a range of the color palette to black
 *
 * @param[in] D0.w Palette index (*byte* offset, not word!)
 * @param[in] D1.w Length (In *words*, not bytes!)
 * @param[out] Z Palette incomplete
 * @ingroup blib_vdp
 *
 * @details If Z is set, the palette fade process is not yet complete
 * and subroutine should be called again on the next loop iteration.
 *
 * @note Sets the palette update flag on _GFX_REFRESH
 */
static inline bool blib_pal_fadeout (u8 palette_index, u8 length)
{
	register u16 d0_BLIB_PALETTE_index asm("d0") = (u16) (palette_index << 1);
	register u16 d1_length asm("d1") = (u16) length;

	asm inline goto(
		"\
  	jsr %p0 \n\
		beq %l[fade_complete] \n\
		"
		:
		: "i"(_BLIB_PAL_FADEOUT), "d"(d0_BLIB_PALETTE_index), "d"(d1_length)
		: "cc"
		: fade_complete);

	return false;

fade_complete:
	return true;
}

/**
 * @fn blib_load_font_defaults
 * @brief Load the internal 1bpp ASCII font with default settings
 * @ingroup blib_misc
 *
 * @details
 * This will place the tiles starting at index 0x20, making it compatible with
 * _PRINT_STRING, and sets the font color to index 1.
 */
static inline void blib_load_font_defaults()
{
	asm("jsr %p0" ::"i"(_BLIB_LOAD_FONT_DEFAULTS) : "d0", "d1", "d2", "d3", "d4", "a1", "a5");
};

/**
 * @fn blib_input_delay
 * @brief Generates a brief delay after initially pressing the D-pad
 * @param[in] A1.l Pointer to byte which will hold the output D-pad value
 * @param[in] D0.w If 0, use P1 input; if non-zero, use P2 input
 * @ingroup blib_input
 *
 * @details
 * This is useful for working with cursors on menus as it creates a brief
 * pause when holding a D-pad direction. You'll need to test against the
 * output variable (set in a1) rather than the standard input mirror in order
 * to use this correctly.
 */
static inline void blib_input_delay (u8 * input, bool use_2p)
{
	register u32 a1_input asm("a1") = (u32) input;
	register u16 d0_use_2p asm("d0") = (u16) use_2p;

	asm("jsr %p0" : : "i"(_BLIB_INPUT_DELAY), "a"(a1_input), "d"(d0_use_2p) : "d1", "a5");
}

/**
 * @fn blib_clear_comm
 * @brief Clears all Gate Array communication registers
 * @ingroup blib_comm
 *
 * @details This clears the COMFLAGS and COMCMD registers directly as well as
 * their RAM buffers
 */
static inline void blib_clear_comm()
{
	asm("jsr %p0" : : "i"(_BLIB_CLEAR_COMM) : "d0", "a6");
}

/**
 * @fn blib_print
 * @brief Displays an ASCII string
 * @param[in] A1.l Pointer to string
 * @param[in] D0.l VRAM destination (vdpptr)
 * @ingroup blib_misc
 *
 * @details Strings are terminated with 0xFF and use 0x00 for newline.
 * The value in _BLIB_FONT_TILE_BASE is added to each character byte, but no other
 * transformations are done. This means that the font must begin at index 0x20
 * at the earliest (where _BLIB_FONT_TILE_BASE is 0).
 * Note that this can only use palette line 0.
 */
static inline void blib_print (char const * string, u32 vdpptr_pos)
{
	register u32 a1_string asm("a1") = (u32) string;
	register u32 d0_vdpptr_pos asm("d0") = vdpptr_pos;

	asm volatile("jsr %p1" : "+a"(a1_string) : "i"(_BLIB_PRINT), "a"(a1_string), "d"(d0_vdpptr_pos) : "d1", "d2", "a5");
};

/**
 * @fn blib_nmtbl_fill
 * @brief Fill a region of a nametable with a value
 * @param[in] D0.l Address (vdpptr format)
 * @param[in] D1.w Width
 * @param[in] D2.w Height
 * @param[in] D3.w Value
 * @ingroup blib_vdp
 */
static inline void blib_nmtbl_fill (u32 vdpptr, u16 width, u16 height, u16 value)
{
	register u32 d0_vdpptr asm("d0") = vdpptr;
	register u32 d1_width asm("d1") = width;
	register u32 d2_height asm("d2") = height;
	register u32 d3_value asm("d3") = value;

	asm volatile(
		"\
    move.l a6, -(sp) \n\
    jsr %p1 \n\
    move.l (sp)+, a6 \n\
  	"
		: "+d"(d2_height)
		: "i"(_BLIB_NMTBL_FILL), "d"(d0_vdpptr), "d"(d1_width), "d"(d2_height), "d"(d3_value)
		: "cc", "d5", "a5");
};

/**
 * @fn blib_dma_xfer
 * @brief Performs a data transfer to VRAM via DMA
 * @param[in] D0.l VRAM destination address (vdpptr format)
 * @param[in] D1.l Source address
 * @param[in] D2.w Length (in words)
 * @ingroup blib_vdp
 */
static inline void blib_dma_xfer (u32 vdpptr_dest, u8 const * source, u16 length)
{
	register u32 d0_vdpptr_dest asm("d0") = vdpptr_dest;
	register u32 d1_source asm("d1") = (u32) source;
	register u16 d2_length asm("d2") = length;

	asm(
		"\
  	move.l a6, -(sp) \n\
  	jsr %p0 \n\
  	move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BLIB_DMA_XFER), "d"(d0_vdpptr_dest), "d"(d1_source), "d"(d2_length)
		: "d3");
};

/**
 * @fn blib_dma_xfer_wrdram
 * @brief Performs a data transfer from Word RAM to VRAM via DMA
 * @param[in] D0.l VRAM destination (vdpptr format)
 * @param[in] D1.l Source address
 * @param[in] D2.w Length (in words)
 * @ingroup blib_vdp
 *
 * @details There is a well-documented issue with performing a DMA transfer from
 * Word RAM to VRAM which must be accounted for by writing the final word of data
 * to the data port. This subroutine takes care of that extra step.
 */
static inline void blib_dma_xfer_wrdram (u32 const vdpptr_dest, void const * source, u16 const length)
{
	register u32 d0_vdpptr_dest asm("d0") = vdpptr_dest;
	register u32 d1_source asm("d1") = (u32) source;
	register u16 d2_length asm("d2") = length;

	asm(
		"\
		move.l a6, -(sp) \n\
  	jsr %p0 \n\
		move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BLIB_DMA_XFER_WORDRAM), "d"(d0_vdpptr_dest), "d"(d1_source), "d"(d2_length)
		: "d3");
};

/**
 * @fn blib_dma_copy
 * @brief Copy data within VRAM via DMA
 * @param[in] D0.l Destination VRAM address (vdpptr)
 * @param[in] D1.w Source VRAM address
 * @param[in] D2.w Length
 * @ingroup blib_vdp
 */
static inline void blib_dma_copy (u32 vdpptr_dest, u16 source, u16 length)
{
	register u32 d0_vdpptr asm("d0") = vdpptr_dest;
	register u16 d1_source asm("d1") = source;
	register u16 d2_length asm("d2") = length;

	asm(
		"\
	  move.l a6, -(sp) \n\
	  jsr %p0 \n\
	  move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BLIB_DMA_COPY), "d"(d0_vdpptr), "d"(d1_source), "d"(d2_length)
		: "d3");
};

/**
 * @fn blib_copy_sprlist
 * @brief Copies sprite list buffer to VDP via DMA
 * @ingroup blib_vdp
 *
 * @details This uses the default Boot ROM VRAM layout (i.e. sprite list at
 * 0xB800) Will only perform the copy of bit 0 of VINT_FLAGS is set (so this is
 * likely intended to be called from VINT, probably VINT_EX)
 */
static inline void blib_copy_sprlist()
{
	asm("jsr %p0" ::"i"(_BLIB_COPY_SPRLIST) : "d4", "a4");
};

/**
 * @fn blib_clear_ram
 * @brief Clear a region of memory
 * @param[in] A0.l Pointer to memory region
 * @param[in] D7.l Size to clear (in longs) MINUS 1
 * @ingroup blib_misc
 */
static inline void blib_clear_ram (void * address, u32 long_count)
{
	register u32 a0_address asm("a0") = (u32) address;
	register u32 d7_long_count asm("d7") = long_count;

	asm(
		"\
		move.l a6, -(sp) \n\
		jsr %p0 \n\
		move.l (sp)+, a6 \n\
		"
		:
		: "i"(_BLIB_CLEAR_RAM), "d"(d7_long_count), "a"(a0_address));
};

/**
 * @fn blib_load_pal
 * @brief Load palette to RAM buffer
 * @param[in] A1.l Pointer to palette data structure
 * @ingroup blib_vdp
 *
 * @details The color palette is loaded but the "Palette Update" flag is not set
 */
static inline void blib_load_pal (Palette const * pal_data)
{
	register u32 A1 asm("a1") = (u32) pal_data;

	asm(
		"\
		jsr %p0 \n\
		"
		:
		: "i"(_BLIB_LOAD_PAL), "a"(A1)
		: "d0");
};

/**
 * @fn blib_load_pal_update
 * @brief Load palette to RAM buffer and set Palette Update flag
 * @param[in] A1.l Pointer to palette data structure
 * @ingroup blib_vdp
 */
static inline void blib_load_pal_update (Palette const * pal_data)
{
	register u32 A1 asm("a1") = (u32) pal_data;

	asm(
		"\
		jsr %p0 \n\
		"
		:
		: "i"(_BLIB_LOAD_PAL_UPDATE), "a"(A1)
		: "d0");
};

/**
 * @fn blib_copy_pal
 * @brief Transfer palette cache to CRAM via DMA
 * @ingroup blib_vdp
 *
 * @note Uses BLIB_VDP_UPDATE_FLAGS
 */
static inline void blib_copy_pal()
{
	asm(
		"\
		jsr %p0 \n\
		"
		:
		: "i"(_BLIB_COPY_PAL)
		: "cc", "a4", "d4");
};

/**
 * @fn blib_process_spr_objs
 * @brief Update/display sprite objects
 * @param[in] A0.l Pointer to object array
 * @param[in] A1.l Pointer to sprite list buffer
 * @param[in] D0.w Number of objects
 * @param[in] D1.w Object size
 * @ingroup blib_vdp
 */
static inline void blib_process_spr_objs (
	struct SpriteObject * const obj_array, Sprite * const sprtbl_cache, u16 const obj_count, u16 const obj_size)
{
	register u32 A0 asm("a0") = (u32) obj_array;
	register u32 A1 asm("a1") = (u32) sprtbl_cache;
	register u16 D0 asm("d0") = (u16) obj_count;
	register u16 D1 asm("d1") = (u16) obj_size;

	asm(
		"\
		jsr %p0 \n\
		"
		:
		: "i"(_BLIB_PROCESS_SPR_OBJS), "a"(A0), "a"(A1), "d"(D0), "d"(D1)
		: "d2", "d3", "d4", "d6", "a2");
};

/**
 * @fn blib_prng_mod
 * @brief Generate a new random number limited with the given modulus
 * @param[in] D0.w Modulus
 * @param[out] D0.w Random number between 0 and modulus
 * @ingroup blib_misc
 *
 * @details The modulus can be used to specify an upper boundary of the random
 * number. To specify a minimum boundary, the modulus should be set like so:
 *   mod = (upper - lower) + 1
 * Finally, the lower bound should be added to the random number result.
 * Psuedo code:
 *   num = prng_mod((upper - lower) + 1) + lower
 */
static inline u16 blib_prng_mod (u16 const modulo)
{
	// TODO can we use the same variable here for in/out?
	register u16 d0_modulo asm("d0") = modulo;
	register u16 d0_random asm("d0");

	asm("jsr %p1" : "+d"(d0_random) : "i"(_BLIB_PRNG_MOD), "d"(d0_modulo) : "d1");

	return d0_random;
};

/**
 * @fn blib_prng
 * @brief Generate a new random number
 * @ingroup blib_misc
 *
 * @details The number will be stored in BLIB_RANDOM
 */
static inline void blib_prng()
{
	asm("jsr %p0" : : "i"(_BLIB_PRNG) : "d0");
};

/**
 * @fn blib_set_fadein_pal
 * @brief Sets the target color palette for a fadein
 * @param[in] A1.l Pointer to target palette structure
 * @ingroup blib_vdp
 *
 * @note This must be called before using blib_pal_fadein
 */
static inline void blib_set_fadein_pal (Palette const * palette)
{
	register u32 a1_BLIB_PALETTE asm("a1") = (u32) palette;
	register u32 a1_change asm("a1");

	asm volatile("jsr %p1" : "=a"(a1_change) : "i"(_BLIB_SET_FADEIN_PAL), "a"(a1_BLIB_PALETTE));
};

/**
 * @fn blib_pal_fadein
 * @brief Fade a range of the color palette from black
 * @ingroup blib_vdp
 *
 * @details The fade-in routine works a little bit differently from the
 * fade-out counterpart. Instead of checking the Z flag, the value of
 * _BLIB_FADEIN_STEP should be checked. If it is > 0, the fade is not yet complete.
 *
 * @note This may be a bug in the implementation. Using the Z flag *could*
 * work, as the code checks whether _BLIB_FADEIN_STEP is zero with a TST opcode.
 * However, this is followed by setting the VDP flags for a palette update,
 * which will set the Z flag if palette update flag was not already set.
 *
 */
static inline void blib_pal_fadein()
{

	asm inline(
		"\
  	jsr %p0 \n\
		"
		:
		: "i"(_BLIB_PAL_FADEIN));
}

/**
 * For use with blib_dma_queue
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
 * @ingroup blib_vdp
 *
 * @details The queue is an array of DMA transfer entries in this format:
 *     0.w Data length
 *     2.l Destination (vdpptr)
 *     6.l Source address
 * The list should be terminated with a 0 word. Note that this system is
 * extremely basic and does not account for DMA bandwidth, etc. Moreover, no
 * array management is done and the list will need to be cleared by the user.
 */
static inline void blib_dma_queue (DmaTransfer const * queue)
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
		: "i"(_BLIB_DMA_QUEUE), "a"(a1_queue)
		: "d0", "d1", "d2", "d3");
}

#endif

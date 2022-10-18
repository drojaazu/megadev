/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file bootlib_def.h
 * @brief Boot ROM call vector & memory definitions
 */

#ifndef MEGADEV__MAIN_BOOTLIB_DEF_H
#define MEGADEV__MAIN_BOOTLIB_DEF_H

/**
 * @defgroup blib_vdp Boot Library VDP related
 */

/**
 * @defgroup blib_input Boot Library user input related
 */

/**
 * @defgroup blib_mic Boot Library misc
 */

#define _WRKRAM_BLIB 0xFFF700

/**
 * @sa BLIB_BUFFER
 */
#define _BLIB_BUFFER 0xFFF700

/**
 * @sa BLIB_SPRLIST
 */
#define _BLIB_SPRLIST 0xFFF900

/**
 * @def _BLIB_PALETTE
 * @sa BLIB_PALETTE
 */
#define _BLIB_PALETTE 0xFFFB80

/**
 * @sa BLIB_PAL0
 */
#define _BLIB_PAL0 0xFFFB80

/**
 * @sa BLIB_PAL1
 */
#define _BLIB_PAL1 0xFFFBA0

/**
 * @sa BLIB_PAL2
 */
#define _BLIB_PAL2 0xFFFBC0

/**
 * @sa BLIB_PAL3
 */
#define _BLIB_PAL3 0xFFFBE0

/**
 * @sa BLIB_VINT_EX_PTR
 */
#define _BLIB_VINT_EX_PTR 0xFFFDAA

/**
 * @sa BLIB_VDPREGS
 */
#define _BLIB_VDPREGS 0xFFFDB4

/**
 * @sa BLIB_COMFLAGS_MAIN
 */
#define _BLIB_COMFLAGS_MAIN 0xfffdde

/**
 * @sa BLIB_COMFLAGS_SUB
 */
#define _BLIB_COMFLAGS_SUB 0xfffddf

/**
 * @sa BLIB_COMCMD
 * Size: 16bit * 8 = 0x10 bytes
 */
#define _BLIB_COMCMD 0xfffde0

/**
 * @var u16 _BLIB_COMCMD0
 * @brief GA COMCMD0 register cache
 */
#define _BLIB_COMCMD0 0xfffde0

/**
 * @var u16 _BLIB_COMCMD1
 * @brief GA COMCMD1 register cache
 */
#define _BLIB_COMCMD1 0xfffde2

/**
 * @var u16 _BLIB_COMCMD2
 * @brief GA COMCMD2 register cache
 */
#define _BLIB_COMCMD2 0xfffde4

/**
 * @var u16 _BLIB_COMCMD3
 * @brief GA COMCMD3 register cache
 */
#define _BLIB_COMCMD3 0xfffde6

/**
 * @var u16 _BLIB_COMCMD4
 * @brief GA COMCMD4 register cache
 */
#define _BLIB_COMCMD4 0xfffde8

/**
 * @var u16 _BLIB_COMCMD5
 * @brief GA COMCMD5 register cache
 */
#define _BLIB_COMCMD5 0xfffdea

/**
 * @var u16 _BLIB_COMCMD6
 * @brief GA COMCMD6 register cache
 */
#define _BLIB_COMCMD6 0xfffdec

/**
 * @var u16 _BLIB_COMCMD7
 * @brief GA COMCMD7 register cache
 */
#define _BLIB_COMCMD7 0xfffdee

/**
 * @sa BLIB_COMSTAT
 * Size: 16bit * 8 = 0x10 bytes
 */
#define _BLIB_COMSTAT 0xfffdf0

/**
 * @var u16 _BLIB_COMSTAT0
 * @brief GA COMSTAT0 register cache
 */
#define _BLIB_COMSTAT0 0xfffdf0

/**
 * @var u16 _BLIB_COMSTAT1
 * @brief GA COMSTAT1 register cache
 */
#define _BLIB_COMSTAT1 0xfffdf2

/**
 * @var u16 _BLIB_COMSTAT2
 * @brief GA COMSTAT2 register cache
 */
#define _BLIB_COMSTAT2 0xfffdf4

/**
 * @var u16 _BLIB_COMSTAT3
 * @brief GA COMSTAT3 register cache
 */
#define _BLIB_COMSTAT3 0xfffdf6

/**
 * @var u16 _BLIB_COMSTAT4
 * @brief GA COMSTAT4 register cache
 */
#define _BLIB_COMSTAT4 0xfffdf8

/**
 * @var u16 _BLIB_COMSTAT5
 * @brief GA COMSTAT5 register cache
 */
#define _BLIB_COMSTAT5 0xfffdfa

/**
 * @var u16 _BLIB_COMSTAT6
 * @brief GA COMSTAT6 register cache
 */
#define _BLIB_COMSTAT6 0xfffdfc

/**
 * @var u16 _BLIB_COMSTAT7
 * @brief GA COMSTAT7 register cache
 */
#define _BLIB_COMSTAT7 0xfffdfe

/**
 * @sa BLIB_JOY1_MOUSE_DATA
 */
#define _BLIB_JOY1_MOUSE_DATA 0xfffe00

/**
 * @sa BLIB_JOY1_MOUSE_DX
 */
#define _BLIB_JOY1_MOUSE_DX 0xfffe06

/**
 * @sa BLIB_JOY1_MOUSE_DY
 */
#define _BLIB_JOY1_MOUSE_DY 0xfffe08

/**
 * @sa BLIB_JOY2_MOUSE_DATA
 */
#define _BLIB_JOY2_MOUSE_DATA 0xfffe0c

/**
 * @sa BLIB_JOY2_MOUSE_DX
 */
#define _BLIB_JOY2_MOUSE_DX 0xfffe12

/**
 * @sa BLIB_JOY2_MOUSE_DY
 */
#define _BLIB_JOY2_MOUSE_DY 0xfffe14

/**
 * @sa BLIB_JOY1_TYPE
 */
#define _BLIB_JOY1_TYPE 0xfffe18

/**
 * @sa BLIB_JOY2_TYPE
 */
#define _BLIB_JOY2_TYPE 0xfffe19

/**
 * Controller types
 */
#define _JOYTYPE_JOYPAD 0x0D
#define _JOYTYPE_MEGAMOUSE 0x03
#define _JOYTYPE_MULTITAP 0x07

/**
 * @sa BLIB_JOY1_HOLD
 */
#define _BLIB_JOY1_HOLD 0xfffe20

/**
 * @sa BLIB_JOY1_PRESS
 */
#define _BLIB_JOY1_PRESS 0xfffe21

/**
 * @sa BLIB_JOY2_HOLD
 */
#define _BLIB_JOY2_HOLD 0xfffe22

/**
 * @sa BLIB_JOY2_PRESS
 */
#define _BLIB_JOY2_PRESS 0xfffe23

/**
 * @sa BLIB_JOY1_DELAY
 */
#define _BLIB_JOY1_DELAY 0xfffe24

/**
 * @sa BLIB_JOY2_DELAY
 */
#define _BLIB_JOY2_DELAY 0xfffe25

/**
 * @sa BLIB_VINT_FLAGS
 */
#define _BLIB_VINT_FLAGS 0xfffe26

/**
 * Flags for @ref _BLIB_VINT_FLAGS
 */
#define COPY_SPRLIST_BIT 0
#define CALL_VINT_EX_BIT 1

/**
 * Bitmasks for @ref _BLIB_VINT_FLAGS
 */
#define COPY_SPRLIST_MSK 1 << COPY_SPRLIST_BIT
#define CALL_VINT_EX_MSK 1 << CALL_VINT_EX_BIT

/**
 * @sa BLIB_VINT_COUNTER
 */
#define _BLIB_VINT_COUNTER 0xfffe27

/**
 * @sa BLIB_VINT_SKIP_GFX
 */
#define _BLIB_VINT_SKIP_GFX 0xfffe28

/**
 * @sa BLIB_VDP_UPDATE_FLAGS
 */
#define _BLIB_VDP_UPDATE_FLAGS 0xfffe29

/* _BLIB_VDP_UPDATE_FLAGS bits/masks */
#define PAL_UPDATE_BIT 0

#define PAL_UPDATE_MSK 1 << PAL_UPDATE_BIT

/**
 * @sa BLIB_RANDOM
 */
#define _BLIB_RANDOM 0xfffe2a

/**
 * @sa BLIB_FONT_TILE_BASE
 */
#define _BLIB_FONT_TILE_BASE 0xfffe2c

/**
 * @sa BLIB_PLANE_WIDTH
 */
#define _BLIB_PLANE_WIDTH 0xFFFE2E

/**
 * @sa BLIB_SPRTBL_PTR
 */
#define _BLIB_SPRTBL_PTR 0xFFFE30

/**
 * @sa BLIB_SPR_JMPTBL_PTR
 */
#define _BLIB_SPR_JMPTBL_PTR 0xFFFE34

/**
 * @sa BLIB_FADEIN_PAL_INDEX
 */
#define _BLIB_FADEIN_PAL_INDEX 0xfffe46

/**
 * @sa BLIB_FADEIN_PAL_LENGTH
 */
#define _BLIB_FADEIN_PAL_LENGTH 0xfffe47

/**
 * @sa BLIB_FADEIN_STEP
 */
#define _BLIB_FADEIN_STEP 0xfffe48

/**
 * @sa BLIB_FADEIN_TARGET_PAL_PTR
 */
#define _BLIB_FADEIN_TARGET_PAL_PTR 0xfffe4a

/******************************************************************************/

// This is the VRAM layout and commonly referenced settings
// when using the default VDP registers

#define _BLIB_WINDOW_ADDR 0xA000
#define _BLIB_PLANEA_ADDR 0xC000
#define _BLIB_PLANEB_ADDR 0xE000
#define _BLIB_SPRTBL_ADDR 0xB800
#define _BLIB_HSCROL_ADDR 0xBC00
#define _BLIB_BLIB_PLANE_WIDTH 128

/******************************************************************************/

/**
 * @sa blib_cdbios_entry
 */
#define _BLIB_CDBIOS_ENTRY 0x000280

/**
 * @sa blib_reset
 */
#define _BLIB_RESET 0x000284

/**
 * @sa blib_init
 */
#define _BLIB_INIT 0x000288

/**
 * @sa blib_init_sp
 */
#define _BLIB_INIT_SP 0x00028C

/**
 * @sa blib_vint_handler
 */
#define _BLIB_VINT_HANDLER 0x000290

/*
 * There are two functions for setting the HINT vector. Both are almost
 * identical but for one minor difference: HINT2 will set the address in A1
 * to both the Main CPU vector and the Gate Array vector (GA_HINTVECT), while
 * HINT1 sets A1 to the Main vector but sets the GA vector to the address of
 * the Main vector, i.e. 0xFFFD0C. It's unclear what this difference entails.
 */

/**
 * @sa blib_set_hint
 * @param[in] A1.l Pointer to HINT handler subroutine
 * @clobber None
 */
#define _BLIB_SET_HINT 0x000294

/**
 * @sa blib_update_inputs
 *
 * @param[out] _BLIB__BLIB_JOY1_PRESS
 * @param[out] _BLIB_JOY1_HOLD
 * @param[out] _BLIB_JOY2_PRESS
 * @param[out] _BLIB_JOY2_HOLD
 * @clobber d6-d7/a5-a6
 */
#define _BLIB_UPDATE_INPUTS 0x000298

/**
 * @sa blib_detect_controller
 * @param[in] a6.l Pointer to joypad data port
 * @param[out] d6.b Controller type
 */
#define _BLIB_DETECT_CONTROLLER 0x00029C

/**
 * @sa blib_clear_vram
 * @clobber d0-d3/a6
 */
#define _BLIB_CLEAR_VRAM 0x0002A0

/**
 * @sa blib_clear_tables
 * @clobber d0-d3/a6
 */
#define _BLIB_CLEAR_TABLES 0x0002A4

/**
 * @sa blib_clear_vsram
 * @clobber d0-d2
 */
#define _BLIB_CLEAR_VSRAM 0x0002A8

/**
 * @sa blib_load_vdpregs_default
 * @clobber d0-d1/a1-a2
 */
#define _BLIB_LOAD_VDPREGS_DEFAULT 0x0002AC

/**
 * @sa blib_load_vdpregs
 * @param[in] A1.l Pointer to register data
 * @clobber d0-d1/a2
 */
#define _BLIB_LOAD_VDPREGS 0x0002B0

/**
 * @sa blib_vdp_fill
 * @param[in] D0.l Address (vdpptr format)
 * @param[in] D1.w Length (in words)
 * @param[in] D2.w Value
 * @clobber d0-d2
 */
#define _BLIB_VDP_FILL 0x0002B4

/**
 * @sa blib_vdp_fill_clear
 * @param[in] D0.l Address (vdpptr)
 * @param[in] D1.w Length (in words)
 * @ingroup blib_vdp
 * @clobber d0-d2
 */
#define _BLIB_VDP_FILL_CLEAR 0x0002B8

/**
 * @def _BLIB_DMA_FILL_CLEAR
 * @sa blib_dma_fill_clear
 * @param[in] D0.l Address (vdpptr format)
 * @param[in] D1.w Length (in words)
 * @clobber d0-d3/a6
 */
#define _BLIB_DMA_FILL_CLEAR 0x0002BC

/**
 * @def _BLIB_DMA_FILL
 * @sa blib_dma_fill
 * @param[in] D0.l Address (vdpptr format)
 * @param[in] D1.w Length (in words)
 * @param[in] D2.w Value
 * @clobber d0-d3/a6
 */
#define _BLIB_DMA_FILL 0x0002C0

/**
 * @def _BLIB_LOAD_MAP
 * @sa blib_load_map
 *
 * @param[in] D0.l VRAM Address (vdpptr format)
 * @param[in] D1.w Map width
 * @param[in] D2.w Map height
 * @param[in] A1.l Pointer to map data
 * @clobber d0-d3/a1/a5
 */
#define _BLIB_LOAD_MAP 0x0002C4

/**
 * @fn _BLIB_LOAD_MAP_TEMPLATE
 * @brief Fill a region of a nametable with map data
 * @param[in] D0.l VRAM address (vdpptr format)
 * @param[in] D1.w Map width
 * @param[in] D2.w Map height
 * @param[in] D3.w Template
 * @param[in] A1.l Pointer to map data
 * @clobber d0-d3/a1/a5
 *
 * GROUP: VDP
 *
 * @details This is very similar to _VDP_LOAD_MAP, however, the input map data
 * is made up of only single bytes. The value is placed in the lower byte of D3,
 * and the word value is placed in the nametable. Since D3 is not cleared
 * beforehand, the upper byte of the word can be set before calling, making it
 * a "template" that applies to each tile.
 */
#define _BLIB_LOAD_MAP_TEMPLATE 0x0002C8

/**
 * @sa blib_nmtbl_fill
 * @clobber d0-d3/d5/a5
 */
#define _BLIB_NMTBL_FILL 0x0002CC

/**
 * @sa blib_dma_xfer
 * @clobber d0-d3/a6
 */
#define _BLIB_DMA_XFER 0x0002D0

/**
 * @sa blib_dma_xfer_wrdram
 * @clobber d0-d3/a6
 */
#define _BLIB_DMA_XFER_WORDRAM 0x0002D4

/**
 * @fn _BLIB_VDP_DISP_ENABLE
 * @brief Enable VDP output
 *
 * GROUP: VDP
 *
 * @details Sets bit 6 on VDP reg. #1. The VDP register buffer is updated.
 */
#define _BLIB_VDP_DISP_ENABLE 0x0002D8

/**
 * @fn _BLIB_VDP_DISP_DISABLE
 * @brief Disable VDP output
 *
 * GROUP: VDP
 *
 * @details Clears bit 6 on VDP reg. #1. The VDP register buffer is updated.
 */
#define _BLIB_VDP_DISP_DISABLE 0x0002DC

/**
 * @sa blib_load_pal
 * @clobber d0
 */
#define _BLIB_LOAD_PAL 0x0002E0

/**
 * @sa blib_load_pal_update
 * @clobber d0
 */
#define _BLIB_LOAD_PAL_UPDATE 0x0002E4

/**
 * @sa blib_copy_pal
 * @clobber a4,d4
 */
#define _BLIB_COPY_PAL 0x0002E8

/**
 * @fn _BLIB_GFX_DECOMP
 * @brief Decompress graphics data in the "Nemesis" format to VRAM
 * @param[in] A1.l Pointer to compressed data
 * @warning You must set the destination on the VDP control port before calling
 * this routine!
 *
 * GROUP: Decompression, VDP
 */
#define _BLIB_GFX_DECOMP 0x0002EC

/**
 * @fn _GFX_DECMP
 * @brief Decompress graphics data in the "Nemesis" format to RAM
 * @param[in] A1.l Pointer to compressed data
 * @param[in] A2.l Pointer to decompressed data buffer
 *
 * GROUP: Decompression
 */
#define _BLIB_GFX_DECOMP_RAM 0x0002F0

/**
 * @sa blib_process_sprobjs
 * @clobber d0-d4/d6/a2
 */
#define _blib_process_sprobjs 0x0002F4

/**
 * @sa blib_clear_ram
 * @clobber d6/a6
 */
#define _BLIB_CLEAR_RAM 0x0002F8

/**
 * @fn _UNKNOWN_1F
 *
 * Calls _CLEAR_REGION in a loop with d5 as the counter, but this may be buggy
 * since d7 should be down to 0 after the first iteration. Not sure what is
 * going on here...
 *
 * IN:
 *  a0.l - Pointer to memory region
 *  d7.l - Size to clear (in longs)
 * OUT:
 *  None
 * BREAK:
 *  d6/a6
 *
 * GROUP: Unknown
 */
#define _UNKNOWN_1F 0x0002FC

/**
 * @fn _BLIB_DISP_SPR_OBJ
 * @brief Display a sprite structure
 * @param[in] A0.l Pointer to parent sprite object
 * @param[in] D6.b Initial value for "next" sprite
 * @clobber d0-d4/a1-a2
 */
#define _BLIB_DISP_SPR_OBJ 0x000300

/**
 * @fn _BLIB_VINT_HANDLER_WAIT
 * @brief Wait for vertical interrupt
 * @clobber d0
 *
 * GROUP: Interrupts
 *
 * @note This will also make a call to _PRNG
 */
#define _BLIB_VINT_HANDLER_WAIT 0x00304

/**
 * @fn _BLIB_VINT_HANDLER_WAIT_DEFAULT
 * @brief Wait for vertical interrupt with default flags
 * @clobber d0
 *
 * GROUP: Interrupts
 *
 * @details This will set the default VINT flags (copy sprite list & call
 * VINT_EX) before waiting for VINT This will also make a call to _PRNG
 */
#define _BLIB_VINT_HANDLER_WAIT_DEFAULT 0x000308

/**
 * @sa blib_copy_sprlist
 * @clobber d4/a4
 */
#define _BLIB_COPY_SPRLIST 0x00030C

/**
 * @fn _UNKNOWN_24
 *
 * A very small routine, but it's unclear what it would have been used for.
 *
 * Manipulates d0 (bytes to long?) and copies to d1, swaps, upper word of d0
 * becomes outer loop, d1 inner loop, and copies long from a0 to a1 on each
 * iteration. After loop, jumps to a2.
 *
 * GROUP: UNKNOWN
 *
 * IN:
 *  d0 -
 *  a0 - Source data
 *  a1 - Dest data
 *  a2 - Ptr to jump after copy
 * OUT:
 *  None
 * BREAK:
 *  d1
 */
#define _UNKNOWN_24 0x000310

/**
 * @def _BLIB_SET_HINT_WORKRAM
 * @sa blib_set_hint_workram
 * @param[in] A1.l Pointer to HINT function
 *
 *
 */
#define _BLIB_SET_HINT_WORKRAM 0x000314

/**
 * @sa blib_disable_hint
 */
#define _BLIB_DISABLE_HINT 0x000318

/**
 * @sa blib_print
 * @clobber d1-d2/a5
 */
#define _BLIB_PRINT 0x00031C

/**
 * @fn _BLIB_LOAD_1BPP_TILES
 * @brief Displays an ASCII string
 * @param[in] A1.l Pointer to 1bpp graphics data
 * @param[in] D0.l VRAM destination (VDPPTR)
 * @param[in] D1.l Color bit map
 * @param[in] D2.l Tile count
 * @clobber d3-d4/a5
 * @ingroup blib_vdp
 *
 * @details The color bit map value is a long that must be in this format:
 * BB'BF'FB'FF
 * Where B and F are indices on the first coloe palette. B represents the back
 * ground color and F the foreground.
 * For example, to have your 1bpp graphics use palette index 2 for the "main"
 * color and a blank background (index 0), then put 00022022 in d1.
 */
#define _BLIB_LOAD_1BPP_TILES 0x000320

/**
 * @fn _BLIB_LOAD_FONT
 * @brief Load the internal 1bpp ASCII font
 * @param[in] D0.l VRAM destination (VDPPTR)
 * @param[in] D1.l Color bit map
 * @clobber d2-d4/a1/a5
 * @ingroup boot_misc
 *
 * @details
 * See the notes in _LOAD_1BPP_TILES for more info about the color bit map.
 * The VRAM destination should place the font no earlier than tile index
 * 0x20 if you are planning to use this with the _PRINT_STRING function.
 */
#define _BLIB_LOAD_FONT 0x000324

/**
 * @sa blib_load_font_defaults
 * @clobber d0-d4/a1/a5
 */
#define _BLIB_LOAD_FONT_DEFAULTS 0x000328

/**
 * @sa blib_input_delay
 * @clobber d1/a1/a5
 */
#define _BLIB_INPUT_DELAY 0x00032C

/**
 * @fn _BLIB_MAP_DECOMP
 * @brief Decompress Enigma data
 * @param[in] D0.w Start tile index
 * @param[in] A1.l Pointer to Enigma compressed data
 * @param[in] A2.l Pointer to output buffer
 * @ingroup boot_compression
 */
#define _BLIB_MAP_DECOMP 0x000330

/**
 * @fn _BLIB_LOAD_MAP_VERT
 * @brief Load map for a vertically-oriented contiguous group of tiles
 * @param[in] D0.l Destination VRAM address (vdpptr)
 * @param[in] D1.w Map width
 * @param[in] D2.w Map height
 * @param[in] D3.w Initial tile index
 * @clobber d4-d6/a5
 * @ingroup blib_vdp
 */
#define _BLIB_LOAD_MAP_VERT 0x000334

/**
 * @sa blib_prng_mod
 * @clobber d1
 */
#define _BLIB_PRNG_MOD 0x000338

/**
 * @sa blib_prng
 * @clobber d0
 */
#define _BLIB_PRNG 0x00033C

/**
 * @sa blib_clear_comm
 * @clobber d0/a6
 */
#define _BLIB_CLEAR_COMM 0x000340

/**
 * @fn _BLIB_COMM_SYNC
 * @brief Copies COMCMD cache to registers and COMSTAT registers to cache
 *
 */
#define _BLIB_COMM_SYNC 0x000344

/**
 * @fn _BLIB_UK_COMM_CDINFO
 * @brief UNKNOWN
 *
 * @details
 */
#define _BLIB_UK_COMM_CDINFO 0x000348

/**
 * @fn _BLIB_UK_COMMFLAGS_RELATED
 * @brief UNKNOWN
 *
 */
#define _BLIB_UK_COMMFLAGS_RELATED 0x00034C

#define _UNKNOWN_35 0x000350

#define _UNKNOWN_36 0x000354

#define _UNKNOWN_37 0x000358

#define _UNKNOWN_38 0x00035C

/**
 * @fn _BLIB_TRIGGER_IFL2
 * @brief Send INT 2 to Sub CPU
 * @clobber a5
 * @ingroup boot_interrupts
 */
#define _BLIB_TRIGGER_IFL2 0x000360

/**
 * @fn _BLIB_SEGA_LOGO
 * @brief Run the Sega logo startup code
 * @ingroup boot_system
 *
 * @note This should never need to be called from inside the game. It is
 * called automatically as part of the security code during startup.
 */
#define _BLIB_SEGA_LOGO 0x000364

/**
 * @fn _BLIB_SET_VINT
 * @brief Set a new VINT subroutine
 * @param[in] A1.l Pointed to VINT subroutinte
 * @ingroup boot_interrupts
 *
 * @note This is relatively useless as a subroutine. It simply moves
 * the adddress into _mlevel6+2. You may as well do the move yourself and skip
 * the stack push/extra cycles from the jsr.
 */
#define _BLIB_SET_VINT 0x000368

/**
 * @fn _BLIB_LOAD_MAP_HORIZ
 * @brief Load map for a horizontally-oriented contiguous group of tiles
 * @param[in] D0.l Destination VRAM address (vdpptr)
 * @param[in] D1.w Map width
 * @param[in] D2.w Map height
 * @param[in] D3.w Initial tile index
 * @clobber d4/a5
 * @ingroup blib_vdp
 */
#define _BLIB_LOAD_MAP_HORIZ 0x00036C

/**
 * @fn _UKNOWN_3B
 *
 * This is related to loading tilemaps to VRAM and is similar to
 * _LOAD_TILEMAP_SEQ, however it reads map data from a pointer and does some
 * odd calculation on D3, subtracting the width twice and then subtracting 2.
 * The value in D3 is added to the A1 pointer at the end of each row.
 * It's not clear what sort of value D3 is meant to have to start with and
 * it's not clear what exactly this routine is meant to do.
 *
 * GROUP: Unknown
 *
 * IN:
 *  d0.l - Destination vdpptr
 *  d1.w - Map width
 *  d2.w - Map height
 *  d3.w - ?
 *  a1.l - Pointer to pattern data
 * OUT:
 *  None
 * BREAK:
 *  d4/a5
 */
#define _UKNOWN_3B 0x000370

/**
 * @sa blib_dma_copy
 * @clobber d3/a6
 */
#define _BLIB_DMA_COPY 0x000374

#define _UNKNOWN_3D 0x000378

/**
 * @fn _BLIB_TO_BCD_BYTE
 * @brief Convert a byte value to BCD
 * @param[in] D1.b Hex value
 * @param[out] D1.b BCD value
 * @ingroup boot_misc
 */
#define _BLIB_TO_BCD_BYTE 0x00037C

/**
 * @fn _BLIB_TO_BCD
 * @brief Convert a word value to BCD
 * @param[in] D1.w Hex value
 * @param[out] D1.w BCD value
 * @ingroup boot_misc
 */
#define _BLIB_TO_BCD 0x000380

/**
 * @fn _BLIB_BLANK_DISPLAY
 * @brief Blanks the display
 * @ingroup blib_vdp

 * @details This routine clears palette index 0 (black) and disables VDP output.
 * @note The VDP register cache will be updated
 */
#define _BLIB_BLANK_DISPLAY 0x000384

/**
 * @sa blib_pal_fadeout
 */
#define _BLIB_PAL_FADEOUT 0x000388

/**
 * @sa blib_pal_fadein
 */
#define _BLIB_PAL_FADEIN 0x00038C

/**
 * @sa blib_set_fadein_pal
 */
#define _BLIB_SET_FADEIN_PAL 0x000390

/**
 * @sa blib_dma_queue
 */
#define _BLIB_DMA_QUEUE 0x000394

#define _BLIB_UNKNOWN_44 0x000398

#define _BLIB_UNKNOWN_45 0x00039C

#define _BLIB_UNKNOWN_46 0x0003A0

#endif

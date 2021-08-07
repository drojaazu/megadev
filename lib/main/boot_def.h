/**
 * \file main/boot_def.h
 * \brief Boot ROM call vector & memory definitions
 */

#ifndef MEGADEV__CD_MAIN_BOOT_DEF_H
#define MEGADEV__CD_MAIN_BOOT_DEF_H

/*
        The Boot ROM library uses Work RAM from 0xFFF700 to 0xFFFC00 for some of
   its functionality. That range is divided into these blocks:

    FFF700  +----------------+
            | Decompression  |
            |         Buffer |
    FFF900  +----------------+
            | Sprite List    |
            |          Cache |
    FFFB80  +----------------+
            | Palette Cache  |
    FFFC00  +----------------+
*/

#define BOOT_RAM_START 0xFFF700

/**
 * \def _DECOMP_BUFFER
 * \brief Work RAM for graphics decompression routines
 * Size: 0x200 bytes
 */
#define _DECOMP_BUFFER 0xFFF700

/**
 * \def _SPRITE_LIST
 * Sprite list RAM buffer
 * Size: 0x280 bytes
 */
#define _SPRITE_LIST 0xFFF900

/**
 * \def _PALETTE
 * CRAM (palette) buffer
 * Size: 0x80 bytes
 */
#define _PALETTE 0xFFFB80

#define _PALETTE0 0xFFFB80
#define _PALETTE1 0xFFFBA0
#define _PALETTE2 0xFFFBC0
#define _PALETTE3 0xFFFBE0

/**
 * \def u32 _VINT_EX_PTR
 * Pointer to the VINT_EX routine used in the Boot ROM VINT handler.
 */
#define _VINT_EX_PTR 0xFFFDAA

/**
 * \def _VDP_REGS
 *
 * RAM buffer of all VDP registers (except DMA regs), making up 19 entries.
 * You will need to keep these updated manually unless you use
 * BOOT_LOAD_VDPREGS
 * Size: 0x26 bytes
 *
 * \ingroup boot_vdp
 */
#define _VDP_REGS 0xFFFDB4

/**
 * \def u16 _COMFLAGS
 * GA comm flags register cache
 */
#define _COMFLAGS 0xfffdde

/**
 * \def u8 _COMFLAGS_MAIN
 * GA comm flags register cache for Main CPU
 */
#define _COMFLAGS_MAIN 0xfffdde

/**
 * \var u8 _COMFLAGS_SUB
 * GA comm flags register cache for Sub CPU
 */
#define _COMFLAGS_SUB 0xfffddf

/**
 * \def _COMCMD
 * GA comm command (Main -> Sub) register cache
 * Size: 0x10 bytes
 */
#define _COMCMD 0xfffde0

#define _COMCMD0 0xfffde0
#define _COMCMD1 0xfffde2
#define _COMCMD2 0xfffde4
#define _COMCMD3 0xfffde6
#define _COMCMD4 0xfffde8
#define _COMCMD5 0xfffdea
#define _COMCMD6 0xfffdec
#define _COMCMD7 0xfffdee

/**
 * \def _COMSTAT
 * GA comm status (Sub -> Main) register cache
 * Size: 0x10 bytes
 */
#define _COMSTAT 0xfffdf0

#define _COMSTAT0 0xfffdf0
#define _COMSTAT1 0xfffdf2
#define _COMSTAT2 0xfffdf4
#define _COMSTAT3 0xfffdf6
#define _COMSTAT4 0xfffdf8
#define _COMSTAT5 0xfffdfa
#define _COMSTAT6 0xfffdfc
#define _COMSTAT7 0xfffdfe

#define _JOY1_MOUSE_DATA 0xfffe00
#define _JOY1_MOUSE_DELTAX 0xfffe06
#define _JOY1_MOUSE_DELTAY 0xfffe08

#define _JOY2_MOUSE_DATA 0xfffe0c
#define _JOY2_MOUSE_DELTAX 0xfffe12
#define _JOY2_MOUSE_DELTAY 0xfffe14

#define _JOY1_TYPE 0xfffe18
#define _JOY2_TYPE 0xfffe19

/**
 * Controller types
 */
#define JOYTYPE_JOYPAD 0x0D
#define JOYTYPE_MEGAMOUSE 3
#define JOYTYPE_MULTITAP 7

/**
 * \def u8 _JOY1_HOLD
 * \brief Port 1 controller constant input
 */
#define _JOY1_HOLD 0xfffe20

/**
 * \def u8 _JOY1_PRESS
 * \brief Port 1 controller single input
 */
#define _JOY1_PRESS 0xfffe21

/**
 * \def u8 _JOY2_HOLD
 * \brief Port 2 controller constant input
 */
#define _JOY2_HOLD 0xfffe22

/**
 * \def u8 _JOY2_PRESS
 * \brief Port 2 controller single input
 */
#define _JOY2_PRESS 0xfffe23

/**
 * \def _JOY1_DELAY
 * \note For use with \ref BOOT_INPUT_DELAY
 */
#define _JOY1_DELAY 0xfffe24

/**
 * \def _JOY2_DELAY
 * \note For use with \ref BOOT_INPUT_DELAY
 */
#define _JOY2_DELAY 0xfffe25

/**
 * \def u8 _VINT_FLAGS
 * Used by the Boot ROM VINT/VINT_WAIT routines for graphics updates
 * during vblank.
 *
 * Bit 0 - Copy sprite list to VDP
 * Bit 1 - Call VINT_EX vector during vblank
 */
#define _VINT_FLAGS 0xfffe26

/**
 * Flags for \ref VINT_FLAGS
 */
#define COPY_SPRLIST_BIT 0
#define CALL_VINT_EX_BIT 1

/**
 * Bitmasks for \ref VINT_FLAGS
 */
#define COPY_SPRLIST_MSK 1 << COPY_SPRLIST_BIT
#define CALL_VINT_EX_MSK 1 << CALL_VINT_EX_BIT

/**
 * \def u8 _VINT_COUNTER
 * \brief Incremented by 1 on each VINT
 */
#define _VINT_COUNTER 0xfffe27

/**
 * \def u8 _VINT_SKIP_GFX_UPDATE
 * Skips extended operations during vblank - CRAM copy, sprite list copy,
 * increase vblank counter
 * Will still perform IO updates, though
 */
#define _VINT_SKIP_GFX_UPDATE 0xfffe28

/**
 * \def u8 _VDP_UPDATE_FLAGS
 */
#define _VDP_UPDATE_FLAGS 0xfffe29

/* _VDP_UPDATE_FLAGS bits/masks */
#define PAL_UPDATE_BIT 0

#define PAL_UPDATE_MSK 1 << PAL_UPDATE_BIT

/**
 * \def u16 _RANDOM
 * \brief Contains a random 16 bit value
 * \details You must make a call to \ref BOOT_PRNG on each vblank in order to
 * use this!
 */
#define _RANDOM 0xfffe2a

/**
 * \def u16 _FONT_TILE_BASE
 * The value added to each character byte when calling _PRINT_STRING. The font
 * can begin no earlier than tile index 0x20
 */
#define _FONT_TILE_BASE 0xfffe2c

/**
 * \def u16 _PLANE_WIDTH
 * Cached value of the plane width as defined in VDP reg. 0x10.
 * \note This value is stored as BYTES, which is effectively the width in
 * tiles * 2. Example: if the plane width is 32 in the register, this value
 * must be 64, and so on.
 * \note This needs to be manually updated each time the register is changed!!
 *
 * \ingroup boot_vdp
 */
#define _PLANE_WIDTH 0xFFFE2E

/**
 * \def u32 _SPRTBL_PTR
 */
#define _SPRTBL_PTR 0xFFFE30

/**
 * \def u32 _SPR_JMPTBL_PTR
 * Pointer to the jump table for entity processing
 */
#define _SPR_JMPTBL_PTR 0xFFFE34

/**
 * \def u8 _FADEIN_PAL_INDEX
 * \brief Palette offset on which the fade in palette should begin
 */
#define _FADEIN_PAL_INDEX 0xfffe46

/**
 * \def u8 _FADEIN_PAL_LENGTH
 * \size byte
 * \brief Number of entries in the fade in palette
 */
#define _FADEIN_PAL_LENGTH 0xfffe47

/**
 * \def u16 _FADEIN_STEP
 * \size word
 * \brief Indicates if a fade in is still in progress
 */
#define _FADEIN_STEP 0xfffe48

/**
 * \def u32 _FADEIN_TARGET_PAL_PTR
 * \size long
 * \brief Pointer to the target fade in palette
 */
#define _FADEIN_TARGET_PAL_PTR 0xfffe4a

/******************************************************************************/

// This is the VRAM layout and commonly referenced settings
// when using the default VDP registers

#define BOOT_WINDOW_ADDR 0xA000
#define BOOT_PLANEA_ADDR 0xC000
#define BOOT_PLANEB_ADDR 0xE000
#define BOOT_SPRTBL_ADDR 0xB800
#define BOOT_HSCROL_ADDR 0xBC00
#define BOOT_PLANE_WIDTH 128

/******************************************************************************/
/**
 * \fn BOOT_RESET
 * GROUP: System
 *
 * This is used as the default reset vector. Leads to Mega CD title screen.
 */
#define BOOT_RESET 0x000280

/**
 * \fn BOOT_ENTRY
 * GROUP: System
 *
 * This is the Boot ROM entry. On cold boot, checks for TMSS, clears RAM, writes
 * Z80 driver, writes PSG data, then moves on to checking hardware region and
 * clearing VRAM. On warm boot, skips directly to region check, then jumps to
 * the reset vector. Leads to Mega CD title screen.
 */
#define BOOT_ENTRY 0x000284

/**
 * \fn BOOT_INIT
 * GROUP: System
 *
 * Sets default VDP regs, clears VRAM, sets default vectors, inits controllers,
 * loads Z80 data, transfers Sub CPU BIOS, loads CD player program
 */
#define BOOT_INIT 0x000288

/**
 * \fn BOOT_INIT_SP
 * GROUP: System
 * Same as above, but sets the stack pointer first.
 */
#define BOOT_INIT_SP 0x00028C

/**
 * \fn BOOT_VINT
 * \brief Vertical Blank interrupt handler
 *
 * GROUP: Interrupts
 *
 * \details Copies GA comm registers to the RAM
 * mirrors, sends INT2 (VINT ocurred) to Sub CPU, updates VDP palette from
 * RAM, calls VINT_EX, updates IO (controllers).
 */
#define BOOT_VINT 0x000290

/*
 * There are two functions for setting the HINT vector. Both are almost
 * identical but for one minor difference: HINT2 will set the address in A1
 * to both the Main CPU vector and the Gate Array vector (GA_HINTVECT), while
 * HINT1 sets A1 to the Main vector but sets the GA vector to the address of
 * the Main vector, i.e. 0xFFFD0C. It's unclear what this difference entails.
 */

/**
 * \fn BOOT_SET_HINT_DEFAULT
 * \brief Sets the default HINT vector
 * \param[in] A1.w HINT vector
 * \break None
 *
 * GROUP: Interrupts
 *
 * Sets the HINT vector in the jump table and the Gate Array register
 * and enables the interrupt on the VDP.
 *
 * \note The VDP register buffer will be updated
 */
#define BOOT_SET_HINT_DEFAULT 0x000294

/**
 * \fn BOOT_UPDATE_INPUTS
 * \brief Update state of P1/P2 controllers
 * \param[out] JOY1_PRESS / JOY1_HOLD
 * \param[out] JOY2_PRESS / JOY2_HOLD
 *
 * GROUP: Input
 *
 * \break d6-d7/a5-a6
 */
#define BOOT_UPDATE_INPUTS 0x000298

/**
 * \fn BOOT_DETECT_CONTROLLER
 * \brief Detect the type of controller plugged in
 *
 * GROUP: Input
 *
 * Note that this is called during the normal system init and the type is
 * stored in JOY1_TYPE and JOY2_TYPE.
 * \param[in] a6.l Pointer to joypad data port
 * \param[out] d6.b Controller type
 */
#define BOOT_DETECT_CONTROLLER 0x00029C

/**
 * \fn BOOT_CLEAR_VRAM
 * \brief Clear all of VRAM and VSRAM via DMA Fill
 * \break d0-d3/a6
 *
 * GROUP: VDP
 *
 * \note This does not clear CRAM.
 *
 */
#define BOOT_CLEAR_VRAM 0x0002A0

/**
 * \fn BOOT_CLEAR_NMTBL
 * \brief Clear nametables and sprite list
 * \break d0-d3/a6
 *
 * GROUP: VDP
 *
 * \note This works only with the Boot ROM default VRAM layout
 */
#define BOOT_CLEAR_NMTBL 0x0002A4

/**
 * \fn BOOT_CLEAR_VSRAM
 * \brief Clear VSRAM
 * \break d0-d2
 *
 * GROUP: VDP
 *
 */
#define BOOT_CLEAR_VSRAM 0x0002A8

/**
 * \fn BOOT_LOAD_VDPREGS_DEFAULT
 * \brief Loads the Boot ROM default VDP register defaults
 * \break d0-d1/a1-a2
 *
 * GROUP: VDP
 *
 */
#define BOOT_LOAD_VDPREGS_DEFAULT 0x0002AC

/**
 * \fn BOOT_LOAD_VDPREGS
 * \brief Load values into multiple VDP registers
 * \param[in] A1.l Pointer to register data
 * \break d0-d1/a2
 *
 * GROUP: VDP
 *
 * \details Register data is an array of word sized values,
 * where the upper byte is the register ID
 * (e.g. 80, 81, etc) and the lower byte is
 * the value, with the list terminated by 0.
 */
#define BOOT_LOAD_VDPREGS 0x0002B0

/**
 * \fn BOOT_VDP_FILL
 * \brief Fill a region of VDP memory with a value
 * \param[in] D0.l Address (vdpptr format)
 * \param[in] D1.w Length (in words)
 * \param[in] D2.w Value
 * \break d0-d2
 *
 * GROUP: VDP
 *
 * \details This is a simple data transfer via the VDP data port rather than
 * DMA.
 */
#define BOOT_VDP_FILL 0x0002B4

/**
 * \fn BOOT_VDP_FILL_CLEAR
 * \brief Fill a region of VDP memory with 0
 * \param[in] D0.l Address (vdpptr)
 * \param[in] D1.w Length (in words)
 * \break d0-d2
 *
 * GROUP: VDP
 *
 * \details This is a simple data transfer via the VDP data port rather than
 * DMA.
 */
#define BOOT_VDP_FILL_CLEAR 0x0002B8

/**
 * \fn BOOT_DMA_FILL_CLEAR
 * \brief Fill a region of VDP memory with zero
 * \param[in] D0.l Address (vdpptr format)
 * \param[in] D1.w Length (in words)
 * \break d0-d3/a6
 *
 * GROUP: VDP
 *
 */
#define BOOT_DMA_FILL_CLEAR 0x0002BC

/**
 * \fn BOOT_DMA_FILL
 * \brief Fill a region of VDP memory with a value
 * \param[in] D0.l Address (vdpptr format)
 * \param[in] D1.w Length (in words)
 * \param[in] D2.w Value
 * \break d0-d3/a6
 *
 * GROUP: VDP
 *
 */
#define BOOT_DMA_FILL 0x0002C0

/**
 * \fn BOOT_LOAD_MAP
 * \brief Fill a region of a nametable with map data
 * \param[in] D0.l VRAM Address (vdpptr format)
 * \param[in] D1.w Map width
 * \param[in] D2.w Map height
 * \param[in] A1.l Pointer to map data
 * \break d0-d3/a1/a5
 *
 * GROUP: VDP
 *
 * \details The map data should be an array of word values in the standard
 * nametable entry format.
 */
#define BOOT_LOAD_MAP 0x0002C4

/**
 * \fn BOOT_LOAD_MAP_TEMPLATE
 * \brief Fill a region of a nametable with map data
 * \param[in] D0.l VRAM address (vdpptr format)
 * \param[in] D1.w Map width
 * \param[in] D2.w Map height
 * \param[in] D3.w Template
 * \param[in] A1.l Pointer to map data
 * \break d0-d3/a1/a5
 *
 * GROUP: VDP
 *
 * \details This is very similar to _VDP_LOAD_MAP, however, the input map data
 * is made up of only single bytes. The value is placed in the lower byte of D3,
 * and the word value is placed in the nametable. Since D3 is not cleared
 * beforehand, the upper byte of the word can be set before calling, making it
 * a "template" that applies to each tile.
 */
#define BOOT_LOAD_MAP_TEMPLATE 0x0002C8

/**
 * \fn BOOT_NMTBL_FILL
 * \brief Fill a region of a nametable with a value
 * \param[in] D0.l Address (vdpptr format)
 * \param[in] D1.w Width
 * \param[in] D2.w Height
 * \param[in] D3.w Value
 * \break d0-d3/d5/a5
 *
 * GROUP: VDP
 */
#define BOOT_NMTBL_FILL 0x0002CC

/**
 * \fn BOOT_DMA_XFER
 * \brief Performs a data transfer to VRAM via DMA
 * \param[in] D0.l VRAM destination address (vdpptr format)
 * \param[in] D1.l Source address
 * \param[in] D2.w Length (in words)
 * \break d0-d3/a6
 *
 * GROUP: VDP
 *
 */
#define BOOT_DMA_XFER 0x0002D0

/**
 * \fn BOOT_DMA_XFER_WORDRAM
 * \brief Performs a data transfer from Word RAM to VRAM via DMA
 * \param[in] D0.l VRAM destination (vdpptr format)
 * \param[in] D1.l Source address
 * \param[in] D2.w Length (in words)
 * \break d0-d3/a6
 *
 * GROUP: VDP
 *
 * \details There is a well-documented issue with doing DMA from Word RAM to
 VRAM
 * which must be accounted for by writing the final word of data to the
 * data port. This subroutine takes care of that extra step.

 */
#define BOOT_DMA_XFER_WORDRAM 0x0002D4

/**
 * \fn BOOT_VDP_DISP_ENABLE
 * \brief Enable VDP output
 *
 * GROUP: VDP
 *
 * \details Sets bit 6 on VDP reg. #1. The VDP register buffer is updated.
 */
#define BOOT_VDP_DISP_ENABLE 0x0002D8

/**
 * \fn BOOT_VDP_DISP_DISABLE
 * \brief Disable VDP output
 *
 * GROUP: VDP
 *
 * \details Clears bit 6 on VDP reg. #1. The VDP register buffer is updated.
 */
#define BOOT_VDP_DISP_DISABLE 0x0002DC

/**
 * \fn BOOT_LOAD_PAL
 * \brief Load palette to RAM buffer
 * \param[in] A1.l Pointer to palette data structure
 * \break d0
 *
 * GROUP: VDP
 *
 * \details The color palette is loaded but the "Palette Update" flag is not set
 */
#define BOOT_LOAD_PAL 0x0002E0

/**
 * \fn BOOT_LOAD_PAL_UPDATE
 * \brief Load palette to RAM buffer and set Palette Update flag
 * \param[in] A1.l Pointer to palette data structure
 * \break d0
 *
 * GROUP: VDP
 */
#define BOOT_LOAD_PAL_UPDATE 0x0002E4

/**
 * \fn BOOT_COPY_PAL
 * \brief Transfer palette cache to CRAM via DMA
 * \break a4,d4
 *
 * GROUP: VDP
 *
 * \note Uses _VDP_UPDATE_FLAGS
 */
#define BOOT_COPY_PAL 0x0002E8

/**
 * \fn BOOT_GFX_DECOMP
 * \brief Decompress graphics data in the "Nemesis" format to VRAM
 * \param[in] A1.l Pointer to compressed data
 * \warning You must set the destination on the VDP control port before calling
 * this routine!
 *
 * GROUP: Decompression, VDP
 */
#define BOOT_GFX_DECOMP 0x0002EC

/**
 * \fn _GFX_DECMP
 * \brief Decompress graphics data in the "Nemesis" format to RAM
 * \param[in] A1.l Pointer to compressed data
 * \param[in] A2.l Pointer to decompressed data buffer
 *
 * GROUP: Decompression
 */
#define BOOT_GFX_DECOMP_RAM 0x0002F0

/**
 * \fn BOOT_PROCESS_SPR_OBJS
 * \brief Update/display sprite objects
 * \param[in] A0.l Pointer to object array
 * \param[in] A1.l Pointer to sprite list buffer
 * \param[in] D0.w Number of objects
 * \param[in] D1.w Object size
 * \break d0-d4/d6/a2
 *
 * GROUP: VDP
 */
#define BOOT_PROCESS_SPR_OBJS 0x0002F4

/**
 * \fn BOOT_CLEAR_RAM
 * \brief Clear a region of memory
 * \param[in] A0.l Pointer to memory region
 * \param[in] D7.l Size to clear (in longs) MINUS 1
 * \break d6/a6
 *
 * GROUP: Misc
 */
#define BOOT_CLEAR_RAM 0x0002F8

/**
 * \fn _UNKNOWN_1F
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
 * \fn BOOT_DISP_SPR_OBJ
 * \brief Display a sprite structure
 * \param[in] A0.l Pointer to parent sprite object
 * \param[in] D6.b Initial value for "next" sprite
 * \break d0-d4/a1-a2
 */
#define BOOT_DISP_SPR_OBJ 0x000300

/**
 * \fn BOOT_VINT_WAIT
 * \brief Wait for vertical interrupt
 * \break d0
 *
 * GROUP: Interrupts
 *
 * \note This will also make a call to _PRNG
 */
#define BOOT_VINT_WAIT 0x00304

/**
 * \fn BOOT_VINT_WAIT_DEFAULT
 * \brief Wait for vertical interrupt with default flags
 * \break d0
 *
 * GROUP: Interrupts
 *
 * \details This will set the default VINT flags (copy sprite list & call
 * VINT_EX) before waiting for VINT This will also make a call to _PRNG
 */
#define BOOT_VINT_WAIT_DEFAULT 0x000308

/**
 * \fn BOOT_COPY_SPRLIST
 * \brief Copies sprite list buffer to VDP via DMA
 * \break d4/a4
 *
 * GROUP: VDP
 *
 * \details This uses the default Boot ROM VRAM layout (i.e. sprite list at
 * 0xB800) Will only perform the copy of bit 0 of VINT_FLAGS is set (so this is
 * likely intended to be called from VINT, probably VINT_EX)
 */
#define BOOT_COPY_SPRLIST 0x00030C

/**
 * \fn _UNKNOWN_24
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
 * \fn BOOT_SET_HINT
 * \brief Sets the HINT vector in the jump table and points the Gate Array
 * register to the jump table entry, and enables the interrupt on the VDP.
 * \param[in] A1.w Pointer to HINT vector
 *
 * GROUP: Interrupts
 *
 * \details The VDP register buffer (_VDP_REGS) is updated with this call.
 */
#define BOOT_SET_HINT 0x000314

/**
 * \fn BOOT_DISABLE_HINT
 * \brief Disables horizontal interrupts on the VDP
 *
 * GROUP: Interrupts
 *
 * \details The VDP register buffer (VDP_REGS) is updated with this call.
 */
#define BOOT_DISABLE_HINT 0x000318

/**
 * \fn BOOT_PRINT
 * \brief Displays an ASCII string
 * \param[in] A1.l Pointer to string
 * \param[in] D0.l VRAM destination (vdpptr)
 * \break d1-d2/a5
 *
 * GROUP: Misc
 *
 * \details
 * Strings are terminated with 0xFF and use 0x00 for newline.
 * The value in _FONT_TILE_BASE is added to each character byte, but no other
 * transformations are done. This means that the font must begin at index 0x20
 * at the earliest (where _FONT_TILE_BASE is 0).
 * Note that this can only use palette line 0.
 */
#define BOOT_PRINT 0x00031C

/**
 * \fn BOOT_LOAD_1BPP_TILES
 * \brief Displays an ASCII string
 * \param[in] A1.l Pointer to 1bpp graphics data
 * \param[in] D0.l VRAM destination (VDPPTR)
 * \param[in] D1.l Color bit map
 * \param[in] D2.l Tile count
 * \break d3-d4/a5
 *
 * GROUP: VDP
 *
 * \details
 * The color bit map value is a long that must be in this format:
 * BB'BF'FB'FF
 * Where B and F are indices on the first coloe palette. B represents the back
 * ground color and F the foreground.
 * For example, to have your 1bpp graphics use palette index 2 for the "main"
 * color and a blank background (index 0), then put 00022022 in d1.
 */
#define BOOT_LOAD_1BPP_TILES 0x000320

/**
 * \fn BOOT_LOAD_FONT
 * \brief Load the internal 1bpp ASCII font
 * \param[in] D0.l VRAM destination (VDPPTR)
 * \param[in] D1.l Color bit map
 * \break d2-d4/a1/a5
 *
 * GROUP: Misc
 *
 * \details
 * See the notes in _LOAD_1BPP_TILES for more info about the color bit map.
 * The VRAM destination should place the font no earlier than tile index
 * 0x20 if you are planning to use this with the _PRINT_STRING function.
 */
#define BOOT_LOAD_FONT 0x000324

/**
 * \fn BOOT_LOAD_FONT_DEFAULTS
 * \brief Load the internal 1bpp ASCII font with default settings
 * \break d0-d4/a1/a5
 *
 * GROUP: Misc
 *
 * \details
 * This will place the tiles starting at index 0x20, making it compatible with
 * _PRINT_STRING, and sets the font color to index 1.
 */
#define BOOT_LOAD_FONT_DEFAULTS 0x000328

/**
 * \fn BOOT_INPUT_DELAY
 * \brief Generates a brief delay after initially pressing the D-pad
 * \param[in] A1.l Pointer to byte which will hold the output D-pad value
 * \param[in] D0.w If 0, use P1 input; if non-zero, use P2 input
 * \break d1/a1/a5
 *
 * GROUP: Input
 *
 * \details
 * This is useful for working with cursors on menus as it creates a brief
 * pause when holding a D-pad direction. You'll need to test against the
 * output variable (set in a1) rather than the standard input mirror in order
 * to use this correctly.
 */
#define BOOT_INPUT_DELAY 0x00032C

/**
 * \fn BOOT_MAP_DECOMP
 * \brief Decompress Enigma data
 * \param[in] D0.w Start tile index
 * \param[in] A1.l Pointer to Enigma compressed data
 * \param[in] A2.l Pointer to output buffer
 *
 * GROUP: Decompression
 */
#define BOOT_MAP_DECOMP 0x000330

/**
 * \fn BOOT_LOAD_MAP_VERT
 * \brief Load map for a vertically-oriented contiguous group of tiles
 * \param[in] D0.l Destination VRAM address (vdpptr)
 * \param[in] D1.w Map width
 * \param[in] D2.w Map height
 * \param[in] D3.w Initial tile index
 * \break d4-d6/a5
 *
 * GROUP: VDP
 */
#define BOOT_LOAD_MAP_VERT 0x000334

/**
 * \fn _PRNG_MOD
 * \brief Generate a new random number limited with the given modulus
 * \param[in] D0.w Modulus
 * \param[out] D0.w Random number between 0 and modulus
 * \break d1
 *
 * GROUP: Misc
 *
 * \details The number will be stored in d0
 *
 * \note The modulus can be used to specify an upper boundary of the random
 * number. To specify a minimum boundary, the modulus should be set like so:
 *   mod = (upper - lower) + 1
 * Finally, the lower bound should be added to the random number result.
 * Psuedo code:
 *   num = prng_mod((upper - lower) + 1) + lower
 */
#define BOOT_PRNG_MOD 0x000338

/**
 * \fn _PRNG
 * \brief Generate a new random number
 * \break d0
 *
 * GROUP: Misc
 *
 * \details The number will be stored in _RANDOM
 */
#define BOOT_PRNG 0x00033C

/**
 * \fn BOOT_CLEAR_COMM
 * \brief Clears all Gate Array communication registers
 * \break d0/a6
 *
 * GROUP: Comm
 *
 * \details This clears the COMFLAGS and COMCMD registers directly as well as
 * their RAM buffers
 */
#define BOOT_CLEAR_COMM 0x000340

/**
 * \fn BOOT_COMM_SYNC
 * \brief Copies COMCMD cache to registers and COMSTAT registers to cache
 *
 */
#define BOOT_COMM_SYNC 0x000344

/**
 * \fn BOOT_UK_COMM_CDINFO
 * \brief UNKNOWN
 *
 * \details
 */
#define BOOT_UK_COMM_CDINFO 0x000348

/**
 * \fn BOOT_UK_COMMFLAGS_RELATED
 * \brief UNKNOWN
 *
 */
#define BOOT_UK_COMMFLAGS_RELATED 0x00034C

#define _UNKNOWN_35 0x000350

#define _UNKNOWN_36 0x000354

#define _UNKNOWN_37 0x000358

#define _UNKNOWN_38 0x00035C

/**
 * \fn BOOT_TRIGGER_IFL2
 * \brief Send INT 2 to Sub CPU
 * \break a5
 *
 * GROUP: Comm
 */
#define BOOT_TRIGGER_IFL2 0x000360

/**
 * \fn BOOT_SEGA_LOGO
 * \brief Run the Sega logo startup code
 *
 * \note This should never need to be called from inside the game. It is
 * called automatically as part of the security code during startup and is
 * included here only for reference.
 *
 * GROUP: System
 */
#define BOOT_SEGA_LOGO 0x000364

/**
 * \fn BOOT_SET_VINT
 * \brief Set a new VINT subroutine
 *
 * \param[in] A1.l Pointed to VINT subroutinte
 *
 * GROUP: Interrupts
 *
 * \note Honestly, this is relatively useless as a subroutine. It simply moves
 * the adddress into _mlevel6+2. You may as well do the move yourself and skip
 * the stack push/extra cycles from the jsr.
 */
#define BOOT_SET_VINT 0x000368

/**
 * \fn BOOT_LOAD_MAP_HORIZ
 * \brief Load map for a horizontally-oriented contiguous group of tiles
 *
 * \param[in] D0.l Destination VRAM address (vdpptr)
 * \param[in] D1.w Map width
 * \param[in] D2.w Map height
 * \param[in] D3.w Initial tile index
 * \break d4/a5
 *
 * GROUP: VDP
 */
#define BOOT_LOAD_MAP_HORIZ 0x00036C

/**
 * \fn _UKNOWN_3B
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
 * \fn BOOT_DMA_COPY
 * \brief Copy data within VRAM via DMA
 *
 * \param[in] D0.l Destination VRAM address (vdpptr)
 * \param[in] D1.w Source VRAM address
 * \param[in] D2.w Length
 * \break d3/a6
 *
 * GROUP: VDP
 */
#define BOOT_DMA_COPY 0x000374

#define _UNKNOWN_3D 0x000378

/**
 * \fn BOOT_TO_BCD_BYTE
 * \brief Convert a byte value to BCD
 *
 * \param[in] D1.b Hex value
 * \param[out] D1.b BCD value
 *
 * GROUP: Misc
 */
#define BOOT_TO_BCD_BYTE 0x00037C

/**
 * \fn BOOT_TO_BCD
 * \brief Convert a word value to BCD
 *
 * \param[in] D1.w Hex value
 * \param[out] D1.w BCD value
 *
 * GROUP: Misc
 */
#define BOOT_TO_BCD 0x000380

/**
 * \fn BOOT_BLANK_DISPLAY
 * \brief Blanks the display
 *
 * \details This routine clears palette index 0 (black) and disables VDP output.
 * \note The VDP register buffer will be updated
 *
 * GROUP: VDP
 */
#define BOOT_BLANK_DISPLAY 0x000384

/**
 * \fn BOOT_PAL_FADEOUT
 * \brief Fade a range of the color palette to black
 *
 * \param[in] D0.w Palette index (*byte* offset, not word!)
 * \param[in] D1.w Length (In *words*, not bytes!)
 * \param[out] Z Palette incomplete
 *
 * GROUP: VDP
 *
 * \details If Z is set, the palette fade process is not yet complete
 * and subroutine should be called against next loop.
 *
 * \note Sets the palette update flag on _GFX_REFRESH
 */
#define BOOT_PAL_FADEOUT 0x000388

/**
 * \fn BOOT_PAL_FADEIN
 * \brief Fade a range of the color palette from black
 *
 * GROUP: VDP
 *
 * \details The fade-in routine works a little bit differently from the
 * fade-out counterpart. Instead of checking the Z flag, the value of
 * _FADEIN_STEP should be checked. If it is > 0, the fade is not yet complete.
 *
 * \note This may be a bug in the implementation. Using the Z flag *could*
 * work, as the code checks whether _FADEIN_STEP is zero with a TST opcode.
 * However, this is followed by setting the VDP flags for a palette update,
 * which will set the Z flag if palette update flag was not already set.
 *
 */
#define BOOT_PAL_FADEIN 0x00038C

/**
 * \fn BOOT_SET_FADEIN_PAL
 * \brief Sets the target color palette for a fadein
 *
 * \param[in] A1.l Pointer to target palette structure
 *
 * \note This must be called before using BOOT_PAL_FADEIN
 *
 * GROUP: VDP
 */
#define BOOT_SET_FADEIN_PAL 0x000390

/**
 * \fn BOOT_DMA_QUEUE
 * \brief Transfer queued VDP bound data via DMA
 *
 * \param[in] A1.l Pointer to queue
 *
 * GROUP: VDP
 *
 * \details The queue is an array of DMA transfer entries in this format:
 *     0.w Data length
 *     2.l Destination (vdpptr)
 *     6.l Source address
 * The list should be terminated with a 0 word. Note that this system is
 * extremely basic and does not account for DMA bandwidth, etc. Moreover, no
 * array management is done and the list will need to be cleared by the user.
 */
#define BOOT_DMA_QUEUE 0x000394

#define _UNKNOWN_44 0x000398

#define _UNKNOWN_45 0x00039C

#define _UNKNOWN_46 0x0003A0

#endif

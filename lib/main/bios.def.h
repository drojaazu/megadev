/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file bios.def.h
 * @brief Boot ROM call vector & memory definitions
 */

#ifndef MEGADEV__MAIN_BOOTLIB_DEF_H
#define MEGADEV__MAIN_BOOTLIB_DEF_H

#include "io.def.h"

/**
 * @defgroup bios_vdp Main CPU / Boot Lib / VDP
 */

/**
 * @defgroup bios_input Main CPU / Boot Lib / User input
 */

/**
 * @defgroup bios_int Main CPU / Boot Lib / Interrupts
 */

/**
 * @defgroup bios_cmp Main CPU / Boot Lib / Compression
 */

/**
 * @defgroup bios_comm Main CPU / Boot Lib / CPU Comm
 */

/**
 * @defgroup bios_system Main CPU / Boot Lib / System calls
 */

/**
 * @defgroup bios_misc Main CPU / Boot Lib / Misc
 */

#define _WRKRAM_BLIB 0xFFF700

/**
 * @def _BIOS_BUFFER
 * @ingroup bios_cmp
 * @sa BIOS_BUFFER
 */
#define _BIOS_BUFFER 0xFFF700

/**
 * @def _BIOS_SPRLIST
 * @ingroup bios_vdp
 * @sa BIOS_SPRLIST
 */
#define _BIOS_SPRLIST 0xFFF900

/**
 * @def _BIOS_PALETTE
 * @ingroup bios_vdp
 * @sa BIOS_PALETTE
 */
#define _BIOS_PALETTE 0xFFFB80

/**
 * @def _BIOS_PAL0
 * @ingroup bios_vdp
 * @sa BIOS_PAL0
 */
#define _BIOS_PAL0 0xFFFB80

/**
 * @def _BIOS_PAL1
 * @ingroup bios_vdp
 * @sa BIOS_PAL1
 */
#define _BIOS_PAL1 0xFFFBA0

/**
 * @def _BIOS_PAL2
 * @ingroup bios_vdp
 * @sa BIOS_PAL2
 */
#define _BIOS_PAL2 0xFFFBC0

/**
 * @def _BIOS_PAL3
 * @ingroup bios_vdp
 * @sa BIOS_PAL3
 */
#define _BIOS_PAL3 0xFFFBE0

/**
 * @def _BIOS_VINT_EX_PTR
 * @ingroup bios_int
 * @sa BIOS_VINT_EX_PTR
 */
#define _BIOS_VINT_EX_PTR 0xFFFDAA

/**
 * @def _BIOS_VDPREGS
 * @ingroup bios_vdp
 * @sa BIOS_VDPREGS
 */
#define _BIOS_VDPREGS 0xFFFDB4

/**
 * @def _BIOS_COMFLAGS_MAIN
 * @ingroup bios_comm
 * @sa BIOS_COMFLAGS_MAIN
 */
#define _BIOS_COMFLAGS_MAIN 0xfffdde

/**
 * @def _BIOS_COMFLAGS_SUB
 * @ingroup bios_comm
 * @sa BIOS_COMFLAGS_SUB
 */
#define _BIOS_COMFLAGS_SUB 0xfffddf

/**
 * @def _BIOS_COMCMD
 * @ingroup bios_comm
 *
 * @note
 * Size: 16bit * 8 = 0x10 bytes
 *
 * @sa BIOS_COMCMD
 */
#define _BIOS_COMCMD 0xfffde0

/**
 * @def _BIOS_COMCMD0
 * @brief GA COMCMD0 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMCMD0 0xfffde0

/**
 * @def _BIOS_COMCMD1
 * @brief GA COMCMD1 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMCMD1 0xfffde2

/**
 * @def _BIOS_COMCMD2
 * @brief GA COMCMD2 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMCMD2 0xfffde4

/**
 * @def _BIOS_COMCMD3
 * @brief GA COMCMD3 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMCMD3 0xfffde6

/**
 * @def _BIOS_COMCMD4
 * @brief GA COMCMD4 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMCMD4 0xfffde8

/**
 * @def _BIOS_COMCMD5
 * @brief GA COMCMD5 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMCMD5 0xfffdea

/**
 * @def _BIOS_COMCMD6
 * @brief GA COMCMD6 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMCMD6 0xfffdec

/**
 * @def _BIOS_COMCMD7
 * @brief GA COMCMD7 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMCMD7 0xfffdee

/**
 * @def _BIOS_COMSTAT
 * @sa BIOS_COMSTAT
 * @ingroup bios_comm
 *
 * @note
 * Size: 16bit * 8 = 0x10 bytes
 */
#define _BIOS_COMSTAT 0xfffdf0

/**
 * @def _BIOS_COMSTAT0
 * @brief GA COMSTAT0 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMSTAT0 0xfffdf0

/**
 * @def _BIOS_COMSTAT1
 * @brief GA COMSTAT1 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMSTAT1 0xfffdf2

/**
 * @def _BIOS_COMSTAT2
 * @brief GA COMSTAT2 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMSTAT2 0xfffdf4

/**
 * @def _BIOS_COMSTAT3
 * @brief GA COMSTAT3 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMSTAT3 0xfffdf6

/**
 * @def _BIOS_COMSTAT4
 * @brief GA COMSTAT4 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMSTAT4 0xfffdf8

/**
 * @def _BIOS_COMSTAT5
 * @brief GA COMSTAT5 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMSTAT5 0xfffdfa

/**
 * @def _BIOS_COMSTAT6
 * @brief GA COMSTAT6 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMSTAT6 0xfffdfc

/**
 * @def _BIOS_COMSTAT7
 * @brief GA COMSTAT7 register cache
 * @ingroup bios_comm
 */
#define _BIOS_COMSTAT7 0xfffdfe

/**
 * @def _BIOS_JOY1_MOUSE_DATA
 * @ingroup bios_input
 *
 * @sa BIOS_JOY1_MOUSE_DATA
 */
#define _BIOS_JOY1_MOUSE_DATA 0xfffe00

/**
 * @def _BIOS_JOY1_MOUSE_DX
 * @ingroup bios_input
 * @sa BIOS_JOY1_MOUSE_DX
 */
#define _BIOS_JOY1_MOUSE_DX 0xfffe06

/**
 * @def _BIOS_JOY1_MOUSE_DY
 * @ingroup bios_input
 * @sa BIOS_JOY1_MOUSE_DY
 */
#define _BIOS_JOY1_MOUSE_DY 0xfffe08

/**
 * @def _BIOS_JOY2_MOUSE_DATA
 * @ingroup bios_input
 * @sa BIOS_JOY2_MOUSE_DATA
 */
#define _BIOS_JOY2_MOUSE_DATA 0xfffe0c

/**
 * @def _BIOS_JOY2_MOUSE_DX
 * @ingroup bios_input
 * @sa BIOS_JOY2_MOUSE_DX
 */
#define _BIOS_JOY2_MOUSE_DX 0xfffe12

/**
 * @def _BIOS_JOY2_MOUSE_DY
 * @ingroup bios_input
 * @sa BIOS_JOY2_MOUSE_DY
 */
#define _BIOS_JOY2_MOUSE_DY 0xfffe14

/**
 * @def _BIOS_JOY1_TYPE
 * @ingroup bios_input
 * @sa BIOS_JOY1_TYPE
 */
#define _BIOS_JOY1_TYPE 0xfffe18

/**
 * @def _BIOS_JOY2_TYPE
 * @ingroup bios_input
 * @sa BIOS_JOY2_TYPE
 */
#define _BIOS_JOY2_TYPE 0xfffe19

/**
 * @def CONTROLLER_JOYPAD
 * @ingroup bios_input
 */
#define CONTROLLER_JOYPAD 0x0D

/**
 * @def CONTROLLER_MEGAMOUSE
 * @ingroup bios_input
 */
#define CONTROLLER_MEGAMOUSE 0x03

/**
 * @def CONTROLLER_MULTITAP
 * @ingroup bios_input
 */
#define CONTROLLER_MULTITAP 0x07

/**
 * @def _BIOS_JOY1_HOLD
 * @ingroup bios_input
 * @sa BIOS_JOY1_HOLD
 */
#define _BIOS_JOY1_HOLD 0xfffe20

/**
 * @def _BIOS_JOY1_PRESS
 * @ingroup bios_input
 * @sa BIOS_JOY1_PRESS
 */
#define _BIOS_JOY1_PRESS 0xfffe21

/**
 * @def _BIOS_JOY2_HOLD
 * @ingroup bios_input
 * @sa BIOS_JOY2_HOLD
 */
#define _BIOS_JOY2_HOLD 0xfffe22

/**
 * @def _BIOS_JOY2_PRESS
 * @ingroup bios_input
 * @sa BIOS_JOY2_PRESS
 */
#define _BIOS_JOY2_PRESS 0xfffe23

/**
 * @def _BIOS_JOY1_DELAY
 * @ingroup bios_input
 * @sa BIOS_JOY1_DELAY
 */
#define _BIOS_JOY1_DELAY 0xfffe24

/**
 * @def _BIOS_JOY2_DELAY
 * @ingroup bios_input
 * @sa BIOS_JOY2_DELAY
 */
#define _BIOS_JOY2_DELAY 0xfffe25

/**
 * @def _BIOS_VINT_FLAGS
 * @ingroup bios_int
 * @sa BIOS_VINT_FLAGS
 */
#define _BIOS_VINT_FLAGS 0xfffe26

/**
 * Flags for @ref _BIOS_VINT_FLAGS
 * @ingroup bios_int
 */
#define COPY_SPRLIST_BIT 0
#define CALL_VINT_EX_BIT 1

/**
 * Bitmasks for @ref _BIOS_VINT_FLAGS
 */
#define COPY_SPRLIST_MSK 1 << COPY_SPRLIST_BIT
#define CALL_VINT_EX_MSK 1 << CALL_VINT_EX_BIT

/**
 * @def _BIOS_VINT_COUNTER
 * @ingroup bios_int
 * @sa BIOS_VINT_COUNTER
 */
#define _BIOS_VINT_COUNTER 0xfffe27

/**
 * @def _BIOS_VINT_SKIP_GFX
 * @ingroup bios_int
 * @sa BIOS_VINT_SKIP_GFX
 */
#define _BIOS_VINT_SKIP_GFX 0xfffe28

/**
 * @def _BIOS_VDP_UPDATE_FLAGS
 * @ingroup bios_vdp
 * @sa BIOS_VDP_UPDATE_FLAGS
 */
#define _BIOS_VDP_UPDATE_FLAGS 0xfffe29

/**
 * @def VDPUPDATE_PAL
 * @brief BLIB VDP Update Flags / Update Palette
 * @ingroup bios_vdp
 */
#define VDPUPDATE_PAL (1 << 0)

/**
 * @def _BIOS_RANDOM
 * @ingroup bios_misc
 * @sa BIOS_RANDOM
 */
#define _BIOS_RANDOM 0xfffe2a

/**
 * @def _BIOS_FONT_TILE_BASE
 * @ingroup bios_misc
 * @sa BIOS_FONT_TILE_BASE
 */
#define _BIOS_FONT_TILE_BASE 0xfffe2c

/**
 * @def _BIOS_PLANE_WIDTH
 * @ingroup bios_vdp
 * @sa BIOS_PLANE_WIDTH
 */
#define _BIOS_PLANE_WIDTH 0xFFFE2E

/**
 * @def _BIOS_SPRTBL_PTR
 * @ingroup bios_vdp
 * @sa BIOS_SPRTBL_PTR
 */
#define _BIOS_SPRTBL_PTR 0xFFFE30

/**
 * @def _BIOS_SPR_JMPTBL_PTR
 * @ingroup bios_vdp
 * @sa BIOS_SPR_JMPTBL_PTR
 */
#define _BIOS_SPR_JMPTBL_PTR 0xFFFE34

/**
 * @def _BIOS_FADEIN_PAL_INDEX
 * @ingroup bios_vdp
 * @sa BIOS_FADEIN_PAL_INDEX
 */
#define _BIOS_FADEIN_PAL_INDEX 0xfffe46

/**
 * @def _BIOS_FADEIN_PAL_LENGTH
 * @ingroup bios_vdp
 * @sa BIOS_FADEIN_PAL_LENGTH
 */
#define _BIOS_FADEIN_PAL_LENGTH 0xfffe47

/**
 * @def _BIOS_FADEIN_STEP
 * @ingroup bios_vdp
 * @sa BIOS_FADEIN_STEP
 */
#define _BIOS_FADEIN_STEP 0xfffe48

/**
 * @def _BIOS_FADEIN_TARGET_PAL_PTR
 * @ingroup bios_vdp
 * @sa BIOS_FADEIN_TARGET_PAL_PTR
 */
#define _BIOS_FADEIN_TARGET_PAL_PTR 0xfffe4a

/******************************************************************************/

// This is the VRAM layout and commonly referenced settings
// when using the default VDP registers

#define _BIOS_WINDOW_ADDR 0xA000
#define _BIOS_PLANEA_ADDR 0xC000
#define _BIOS_PLANEB_ADDR 0xE000
#define _BIOS_SPRTBL_ADDR 0xB800
#define _BIOS_HSCROL_ADDR 0xBC00
#define _BIOS_BIOS_PLANE_WIDTH 128

/******************************************************************************/

/**
 * @sa bios_bios_entry
 * @ingroup bios_system
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_ENTRY 0x400280
#else
#define _BIOS_ENTRY 0x000280
#endif

/**
 * @sa bios_reset
 * @ingroup bios_system
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_RESET 0x400284
#else
#define _BIOS_RESET 0x000284
#endif

/**
 * @sa bios_init
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_INIT 0x400288
#else
#define _BIOS_INIT 0x000288
#endif

/**
 * @sa bios_init_sp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_INIT_SP 0x40028C
#else
#define _BIOS_INIT_SP 0x00028C
#endif

/**
 * @sa bios_vint_handler
 * @ingroup bios_int
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_VINT_HANDLER 0x400290
#else
#define _BIOS_VINT_HANDLER 0x000290
#endif

/*
 * There are two functions for setting the HINT vector. Both are almost
 * identical but for one minor difference: HINT2 will set the address in A1
 * to both the Main CPU vector and the Gate Array vector (GA_HINTVECT), while
 * HINT1 sets A1 to the Main vector but sets the GA vector to the address of
 * the Main vector, i.e. 0xFFFD0C. It's unclear what this difference entails.
 */

/**
 * @sa bios_set_hint
 * @param[in] A1.l Pointer to HINT handler subroutine
 * @clobber None
 * @ingroup bios_int
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_SET_HINT 0x400294
#else
#define _BIOS_SET_HINT 0x000294
#endif

/**
 * @def _BIOS_UPDATE_INPUTS
 * @ingroup bios_input
 *
 * @param[out] [@ref _BIOS_JOY1_PRESS]
 * @param[out] [@ref _BIOS_JOY1_HOLD]
 * @param[out] [@ref _BIOS_JOY2_PRESS]
 * @param[out] [@ref _BIOS_JOY2_HOLD]
 * @clobber d6-d7/a5-a6
 * @sa bios_update_inputs
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_UPDATE_INPUTS 0x400298
#else
#define _BIOS_UPDATE_INPUTS 0x000298
#endif

/**
 * @def _BIOS_DETECT_CONTROLLER
 * @sa bios_detect_controller
 * @param[in] a6.l Pointer to joypad data port
 * @param[out] d6.b Controller type
 * @ingroup bios_input
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_DETECT_CONTROLLER 0x40029C
#else
#define _BIOS_DETECT_CONTROLLER 0x00029C
#endif

/**
 * @sa bios_clear_vram
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_CLEAR_VRAM 0x4002A0
#else
#define _BIOS_CLEAR_VRAM 0x0002A0
#endif

/**
 * @sa bios_clear_tables
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_CLEAR_TABLES 0x4002A4
#else
#define _BIOS_CLEAR_TABLES 0x0002A4
#endif

/**
 * @sa bios_clear_vsram
 * @clobber d0-d2
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_CLEAR_VSRAM 0x4002A8
#else
#define _BIOS_CLEAR_VSRAM 0x0002A8
#endif

/**
 * @sa bios_load_vdpregs_default
 * @clobber d0-d1/a1-a2
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_LOAD_VDPREGS_DEFAULT 0x4002AC
#else
#define _BIOS_LOAD_VDPREGS_DEFAULT 0x0002AC
#endif

/**
 * @sa bios_load_vdpregs
 * @param[in] A1.l Pointer to register data
 * @clobber d0-d1/a2
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_LOAD_VDPREGS 0x4002B0
#else
#define _BIOS_LOAD_VDPREGS 0x0002B0
#endif

/**
 * @sa bios_vdp_fill
 * @param[in] D0.l Address (vdp_command format)
 * @param[in] D1.w Length (in words)
 * @param[in] D2.w Value
 * @clobber d0-d2
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_VDP_FILL 0x4002B4
#else
#define _BIOS_VDP_FILL 0x0002B4
#endif

/**
 * @sa bios_vdp_fill_clear
 * @param[in] D0.l Address (vdpptr)
 * @param[in] D1.w Length (in words)
 * @ingroup bios_vdp
 * @clobber d0-d2
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_VDP_FILL_CLEAR 0x4002B8
#else
#define _BIOS_VDP_FILL_CLEAR 0x0002B8
#endif

/**
 * @def _BIOS_DMA_FILL_CLEAR
 * @sa bios_dma_fill_clear
 * @param[in] D0.l Address (vdp_command format)
 * @param[in] D1.w Length (in words)
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_DMA_FILL_CLEAR 0x4002BC
#else
#define _BIOS_DMA_FILL_CLEAR 0x0002BC
#endif

/**
 * @def _BIOS_DMA_FILL
 * @sa bios_dma_fill
 * @param[in] D0.l Address (vdp_command format)
 * @param[in] D1.w Length (in words)
 * @param[in] D2.w Value
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_DMA_FILL 0x4002C0
#else
#define _BIOS_DMA_FILL 0x0002C0
#endif

/**
 * @def _BIOS_LOAD_MAP
 * @sa bios_load_map
 *
 * @param[in] D0.l VRAM Address (vdp_command format)
 * @param[in] D1.w Map width
 * @param[in] D2.w Map height
 * @param[in] A1.l Pointer to map data
 * @clobber d0-d3/a1/a5
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_LOAD_MAP 0x4002C4
#else
#define _BIOS_LOAD_MAP 0x0002C4
#endif

/**
 * @fn _BIOS_LOAD_MAP_TEMPLATE
 * @brief Fill a region of a nametable with map data
 * @param[in] D0.l VRAM address (vdp_command format)
 * @param[in] D1.w Map width
 * @param[in] D2.w Map height
 * @param[in] D3.w Template
 * @param[in] A1.l Pointer to map data
 * @clobber d0-d3/a1/a5
 * @ingroup bios_vdp
 *
 * @details This is very similar to _VDP_LOAD_MAP, however, the input map data
 * is made up of only single bytes. The value is placed in the lower byte of D3,
 * and the word value is placed in the nametable. Since D3 is not cleared
 * beforehand, the upper byte of the word can be set before calling, making it
 * a "template" that applies to each tile.
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_LOAD_MAP_TEMPLATE 0x4002C8
#else
#define _BIOS_LOAD_MAP_TEMPLATE 0x0002C8
#endif

/**
 * @sa bios_nmtbl_fill
 * @clobber d0-d3/d5/a5
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_NMTBL_FILL 0x4002CC
#else
#define _BIOS_NMTBL_FILL 0x0002CC
#endif

/**
 * @sa bios_dma_xfer
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_DMA_XFER 0x4002D0
#else
#define _BIOS_DMA_XFER 0x0002D0
#endif

/**
 * @sa bios_dma_xfer_wrdram
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_DMA_XFER_WRDRAM 0x4002D4
#else
#define _BIOS_DMA_XFER_WRDRAM 0x0002D4
#endif

/**
 * @fn _BIOS_VDP_DISP_ENABLE
 * @brief Enable VDP output
 * @ingroup bios_vdp
 *
 * @details Sets bit 6 on VDP reg. #1. The VDP register buffer is updated.
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_VDP_DISP_ENABLE 0x4002D8
#else
#define _BIOS_VDP_DISP_ENABLE 0x0002D8
#endif

/**
 * @fn _BIOS_VDP_DISP_DISABLE
 * @brief Disable VDP output
 * @ingroup bios_vdp
 *
 * @details Clears bit 6 on VDP reg. #1. The VDP register buffer is updated.
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_VDP_DISP_DISABLE 0x4002DC
#else
#define _BIOS_VDP_DISP_DISABLE 0x0002DC
#endif

/**
 * @sa bios_load_pal
 * @clobber d0
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_LOAD_PAL 0x4002E0
#else
#define _BIOS_LOAD_PAL 0x0002E0
#endif

/**
 * @sa bios_load_pal_update
 * @clobber d0
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_LOAD_PAL_UPDATE 0x4002E4
#else
#define _BIOS_LOAD_PAL_UPDATE 0x0002E4
#endif

/**
 * @sa bios_copy_pal
 * @clobber a4,d4
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_COPY_PAL 0x4002E8
#else
#define _BIOS_COPY_PAL 0x0002E8
#endif

/**
 * @fn _BIOS_GFX_DECOMP
 * @brief Decompress graphics data in the "Nemesis" format to VRAM
 * @ingroup bios_vdp
 * @param[in] A1.l Pointer to compressed data
 *
 * @warning You must set the destination on the VDP control port before calling
 * this routine!
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_GFX_DECOMP 0x4002EC
#else
#define _BIOS_GFX_DECOMP 0x0002EC
#endif

/**
 * @fn _BIOS_GFX_DECOMP_RAM
 * @brief Decompress graphics data in the "Nemesis" format to RAM
 * @param[in] A1.l Pointer to compressed data
 * @param[in] A2.l Pointer to decompressed data buffer
 *
 * GROUP: Decompression
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_GFX_DECOMP_RAM 0x4002F0
#else
#define _BIOS_GFX_DECOMP_RAM 0x0002F0
#endif

/**
 * @sa bios_process_sprobjs
 * @clobber d0-d4/d6/a2
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_PROCESS_SPROBJS 0x4002F4
#else
#define _BIOS_PROCESS_SPROBJS 0x0002F4
#endif

/**
 * @sa bios_clear_ram
 * @clobber d6/a6
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_CLEAR_RAM 0x4002F8
#else
#define _BIOS_CLEAR_RAM 0x0002F8
#endif

/**
 * @fn _BIOS_UNKNOWN_1F
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
#if TARGET == MEGACD_MODE1
#define _BIOS_UNKNOWN_1F 0x4002FC
#else
#define _BIOS_UNKNOWN_1F 0x0002FC
#endif

/**
 * @fn _BIOS_DISP_SPROBJ
 * @brief Display a sprite structure
 * @param[in] A0.l Pointer to parent sprite object
 * @param[in] D6.b Initial value for "next" sprite
 * @clobber d0-d4/a1-a2
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_DISP_SPROBJ 0x400300
#else
#define _BIOS_DISP_SPROBJ 0x000300
#endif

/**
 * @sa bios_vint_wait
 * @clobber d0
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_VINT_HANDLER_WAIT 0x400304
#else
#define _BIOS_VINT_HANDLER_WAIT 0x000304
#endif

/**
 * @sa bios_vint_wait_default
 * @clobber d0
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_VINT_HANDLER_WAIT_DEFAULT 0x400308
#else
#define _BIOS_VINT_HANDLER_WAIT_DEFAULT 0x000308
#endif

/**
 * @sa bios_copy_sprlist
 * @clobber d4/a4
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_COPY_SPRLIST 0x40030C
#else
#define _BIOS_COPY_SPRLIST 0x00030C
#endif

/**
 * @fn _BIOS_UNKNOWN_24
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
#if TARGET == MEGACD_MODE1
#define _BIOS_UNKNOWN_24 0x400310
#else
#define _BIOS_UNKNOWN_24 0x000310
#endif

/**
 * @def _BIOS_SET_HINT_WORKRAM
 * @sa bios_set_hint_workram
 * @param[in] A1.l Pointer to HINT function
 *
 *
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_SET_HINT_WORKRAM 0x400314
#else
#define _BIOS_SET_HINT_WORKRAM 0x000314
#endif

/**
 * @sa bios_disable_hint
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_DISABLE_HINT 0x400318
#else
#define _BIOS_DISABLE_HINT 0x000318
#endif

/**
 * @sa bios_print
 * @clobber d1-d2/a5
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_PRINT 0x40031C
#else
#define _BIOS_PRINT 0x00031C
#endif

/**
 * @fn _BIOS_LOAD_1BPP_TILES
 * @brief Load 1bpp graphics into VDP
 * @param[in] A1.l Pointer to 1bpp graphics data
 * @param[in] D0.l VRAM destination (VDPCMD)
 * @param[in] D1.l Color bit map
 * @param[in] D2.l Tile count
 * @clobber d3-d4/a5
 * @ingroup bios_vdp
 *
 * @details The color bit map value is a long that must be in this format:
 * BB'BF'FB'FF
 * Where B and F are indices on the first coloe palette. B represents the back
 * ground color and F the foreground.
 * For example, to have your 1bpp graphics use palette index 2 for the "main"
 * color and a blank background (index 0), then put 00022022 in d1.
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_LOAD_1BPP_TILES 0x400320
#else
#define _BIOS_LOAD_1BPP_TILES 0x000320
#endif

/**
 * @fn _BIOS_LOAD_FONT
 * @brief Load the internal 1bpp ASCII font
 * @param[in] D0.l VRAM destination (VDPCMD)
 * @param[in] D1.l Color bit map
 * @clobber d2-d4/a1/a5
 * @ingroup bios_misc
 *
 * @details
 * See the notes in _LOAD_1BPP_TILES for more info about the color bit map.
 * The VRAM destination should place the font no earlier than tile index
 * 0x20 if you are planning to use this with the _PRINT_STRING function.
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_LOAD_FONT 0x400324
#else
#define _BIOS_LOAD_FONT 0x000324
#endif

/**
 * @sa bios_load_font_defaults
 * @clobber d0-d4/a1/a5
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_LOAD_FONT_DEFAULTS 0x400328
#else
#define _BIOS_LOAD_FONT_DEFAULTS 0x000328
#endif

/**
 * @sa bios_input_delay
 * @clobber d1/a1/a5
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_INPUT_DELAY 0x40032C
#else
#define _BIOS_INPUT_DELAY 0x00032C
#endif

/**
 * @fn _BIOS_MAP_DECOMP
 * @brief Decompress Enigma data
 * @param[in] D0.w Start tile index
 * @param[in] A1.l Pointer to Enigma compressed data
 * @param[in] A2.l Pointer to output buffer
 * @ingroup bios_cmp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_MAP_DECOMP 0x400330
#else
#define _BIOS_MAP_DECOMP 0x000330
#endif

/**
 * @fn _BIOS_LOAD_MAP_VERT
 * @brief Load map for a vertically-oriented contiguous group of tiles
 * @param[in] D0.l Destination VRAM address (vdpptr)
 * @param[in] D1.w Map width
 * @param[in] D2.w Map height
 * @param[in] D3.w Initial tile index
 * @clobber d4-d6/a5
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_LOAD_MAP_VERT 0x400334
#else
#define _BIOS_LOAD_MAP_VERT 0x000334
#endif

/**
 * @sa bios_prng_mod
 * @clobber d1
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_PRNG_MOD 0x400338
#else
#define _BIOS_PRNG_MOD 0x000338
#endif

/**
 * @sa bios_prng
 * @clobber d0
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_PRNG 0x40033C
#else
#define _BIOS_PRNG 0x00033C
#endif

/**
 * @sa bios_clear_comm
 * @clobber d0/a6
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_CLEAR_COMM 0x400340
#else
#define _BIOS_CLEAR_COMM 0x000340
#endif

/**
 * @fn _BIOS_COMM_SYNC
 * @brief Copies COMCMD cache to registers and COMSTAT registers to cache
 *
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_COMM_SYNC 0x400344
#else
#define _BIOS_COMM_SYNC 0x000344
#endif

/**
 * @fn _BIOS_UK_COMM_CDINFO
 * @brief UNKNOWN
 *
 * @details
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_UK_COMM_CDINFO 0x400348
#else
#define _BIOS_UK_COMM_CDINFO 0x000348
#endif

/**
 * @fn _BIOS_UK_COMMFLAGS_RELATED
 * @brief UNKNOWN
 *
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_UK_COMMFLAGS_RELATED 0x40034C
#else
#define _BIOS_UK_COMMFLAGS_RELATED 0x00034C
#endif

#if TARGET == MEGACD_MODE1
#define _BIOS_UNKNOWN_35 0x400350
#else
#define _BIOS_UNKNOWN_35 0x000350
#endif

#if TARGET == MEGACD_MODE1
#define _BIOS_UNKNOWN_36 0x400354
#else
#define _BIOS_UNKNOWN_36 0x000354
#endif

#if TARGET == MEGACD_MODE1
#define _BIOS_UNKNOWN_37 0x400358
#else
#define _BIOS_UNKNOWN_37 0x000358
#endif

#if TARGET == MEGACD_MODE1
#define _BIOS_UNKNOWN_38 0x40035C
#else
#define _BIOS_UNKNOWN_38 0x00035C
#endif

/**
 * @fn _BIOS_TRIGGER_IFL2
 * @brief Send INT 2 to Sub CPU
 * @clobber a5
 * @ingroup bios_int
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_TRIGGER_IFL2 0x400360
#else
#define _BIOS_TRIGGER_IFL2 0x000360
#endif

/**
 * @fn _BIOS_SEGA_LOGO
 * @brief Run the Sega logo startup code
 * @ingroup boot_system
 *
 * @note This should never need to be called from inside the game. It is
 * called automatically as part of the security code during startup.
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_SEGA_LOGO 0x400364
#else
#define _BIOS_SEGA_LOGO 0x000364
#endif

/**
 * @fn _BIOS_SET_VINT
 * @brief Set a new VINT subroutine
 * @param[in] A1.l Pointed to VINT subroutinte
 * @ingroup bios_int
 *
 * @note This is relatively useless as a subroutine. It simply moves
 * the adddress into _mlevel6+2. You may as well do the move yourself and skip
 * the stack push/extra cycles from the jsr.
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_SET_VINT 0x400368
#else
#define _BIOS_SET_VINT 0x000368
#endif

/**
 * @fn _BIOS_LOAD_MAP_HORIZ
 * @brief Load map for a horizontally-oriented contiguous group of tiles
 * @param[in] D0.l Destination VRAM address (vdpptr)
 * @param[in] D1.w Map width
 * @param[in] D2.w Map height
 * @param[in] D3.w Initial tile index
 * @clobber d4/a5
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_LOAD_MAP_HORIZ 0x40036C
#else
#define _BIOS_LOAD_MAP_HORIZ 0x00036C
#endif

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
#if TARGET == MEGACD_MODE1
#define _BIOS_UNKNOWN_3B 0x400370
#else
#define _BIOS_UNKNOWN_3B 0x000370
#endif

/**
 * @sa bios_dma_copy
 * @clobber d3/a6
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_DMA_COPY 0x400374
#else
#define _BIOS_DMA_COPY 0x000374
#endif

#if TARGET == MEGACD_MODE1
#define _BIOS_UNKNOWN_3D 0x400378
#else
#define _BIOS_UNKNOWN_3D 0x000378
#endif

/**
 * @fn _BIOS_TO_BCD_BYTE
 * @brief Convert a byte value to BCD
 * @param[in] D1.b Hex value
 * @param[out] D1.b BCD value
 * @ingroup bios_misc
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_TO_BCD_BYTE 0x40037C
#else
#define _BIOS_TO_BCD_BYTE 0x00037C
#endif

/**
 * @fn _BIOS_TO_BCD
 * @brief Convert a word value to BCD
 * @param[in] D1.w Hex value
 * @param[out] D1.w BCD value
 * @ingroup bios_misc
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_TO_BCD 0x400380
#else
#define _BIOS_TO_BCD 0x000380
#endif

/**
 * @fn _BIOS_BLANK_DISPLAY
 * @brief Blanks the display
 * @ingroup bios_vdp

 * @details This routine clears palette index 0 (black) and disables VDP output.
 * @note The VDP register cache will be updated
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_BLANK_DISPLAY 0x400384
#else
#define _BIOS_BLANK_DISPLAY 0x000384
#endif

/**
 * @sa bios_pal_fadeout
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_PAL_FADEOUT 0x400388
#else
#define _BIOS_PAL_FADEOUT 0x000388
#endif

/**
 * @sa bios_pal_fadein
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_PAL_FADEIN 0x40038C
#else
#define _BIOS_PAL_FADEIN 0x00038C
#endif

/**
 * @sa bios_set_fadein_pal
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_SET_FADEIN_PAL 0x400390
#else
#define _BIOS_SET_FADEIN_PAL 0x000390
#endif

/**
 * @sa bios_dma_queue
 */
#if TARGET == MEGACD_MODE1
#define _BIOS_DMA_QUEUE 0x400394
#else
#define _BIOS_DMA_QUEUE 0x000394
#endif

#if TARGET == MEGACD_MODE1
#define _BIOS_UNKNOWN_44 0x400398
#else
#define _BIOS_UNKNOWN_44 0x000398
#endif

#if TARGET == MEGACD_MODE1
#define _BIOS_UNKNOWN_45 0x40039C
#else
#define _BIOS_UNKNOWN_45 0x00039C
#endif

#if TARGET == MEGACD_MODE1
#define _BIOS_UNKNOWN_46 0x4003A0
#else
#define _BIOS_UNKNOWN_46 0x0003A0
#endif

/**
 * @brief These define the location of the compressed Sub CPU bios stored within the Boot ROM.
 * In order to use the Sub CPU BIOS in Mode 1, you will need to manually decompress it to
 * its proper location in PRG RAM
 *
 * Note that the location of the compressed BIOS is not constant across Mega CD revisions.
 */
// for all revisions EXCEPT US/EU Model 1 and LaserActive (Mega LD) hardware
#if TARGET == MEGACD_MODE1
#define _BIOS_SUB_BIOS 0x416000
#else
#define _BIOS_SUB_BIOS 0x016000
#endif

#endif

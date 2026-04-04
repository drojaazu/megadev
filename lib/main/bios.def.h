/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file main/bios.def.h
 * @brief Main CPU side system library vectors & memory definitions
 */

#ifndef MEGADEV__MAIN_BIOS_DEF_H
#define MEGADEV__MAIN_BIOS_DEF_H

/**
 * @defgroup bios_vdp Main CPU / BIOS / VDP
 */

/**
 * @defgroup bios_input Main CPU / BIOS / User input
 */

/**
 * @defgroup bios_int Main CPU / BIOS / Interrupts
 */

/**
 * @defgroup bios_cmp Main CPU / BIOS / Compression
 */

/**
 * @defgroup bios_comm Main CPU / BIOS / CPU Comm
 */

/**
 * @defgroup bios_system Main CPU / BIOS / System calls
 */

/**
 * @defgroup bios_misc Main CPU / BIOS / Misc
 */

/**
 * @def BIOS_WORK_BUFFER
 * @ingroup bios_cmp
 * @sa bios_work_buffer
 */
#define BIOS_WORK_BUFFER 0xFFF700

/**
 * @def BIOS_SPRITE_CACHE
 * @ingroup bios_vdp
 * @sa bios_sprlist
 */
#define BIOS_SPRITE_CACHE 0xFFF900

/**
 * @def BIOS_PALETTE_CACHE
 * @ingroup bios_vdp
 * @sa bios_palette
 */
#define BIOS_PALETTE_CACHE 0xFFFB80

/**
 * @def BIOS_PAL1
 * @ingroup bios_vdp
 * @sa bios_pal1
 */
#define BIOS_PAL1 0xFFFB80

/**
 * @def BIOS_PAL2
 * @ingroup bios_vdp
 * @sa bios_pal2
 */
#define BIOS_PAL2 0xFFFBA0

/**
 * @def BIOS_PAL3
 * @ingroup bios_vdp
 * @sa bios_pal3
 */
#define BIOS_PAL3 0xFFFBC0

/**
 * @def BIOS_PAL4
 * @ingroup bios_vdp
 * @sa bios_pal4
 */
#define BIOS_PAL4 0xFFFBE0

/**
 * @def BIOS_VBLANK_USER
 * @ingroup bios_int
 * @sa bios_vblank_user
 */
#define BIOS_VBLANK_USER 0xFFFDAA

/**
 * @def BIOS_VDPREG_CACHE
 * @ingroup bios_vdp
 * @sa bios_vdp_regs
 */
#define BIOS_VDPREG_CACHE 0xFFFDB4

/**
 * @def BIOS_COMMFLAGS_MAIN_CACHE
 * @ingroup bios_comm
 * @sa bios_comflags_main
 */
#define BIOS_COMMFLAGS_MAIN_CACHE 0xFFFDDE

/**
 * @def BIOS_COMMFLAGS_SUB_CACHE
 * @ingroup bios_comm
 * @sa bios_comflags_sub
 */
#define BIOS_COMMFLAGS_SUB_CACHE 0xFFFDDF

/**
 * @def BIOS_COMCMD
 * @ingroup bios_comm
 *
 * @note
 * Size: 16bit * 8 = 0x10 bytes
 *
 * @sa bios_comcmd
 */
#define BIOS_COMCMD 0xFFFDE0

/**
 * @def BIOS_COMCMD0
 * @brief GA COMCMD0 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMCMD0 0xFFFDE0

/**
 * @def BIOS_COMCMD1
 * @brief GA COMCMD1 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMCMD1 0xFFFDE2

/**
 * @def BIOS_COMCMD2
 * @brief GA COMCMD2 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMCMD2 0xFFFDE4

/**
 * @def BIOS_COMCMD3
 * @brief GA COMCMD3 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMCMD3 0xFFFDE6

/**
 * @def BIOS_COMCMD4
 * @brief GA COMCMD4 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMCMD4 0xFFFDE8

/**
 * @def BIOS_COMCMD5
 * @brief GA COMCMD5 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMCMD5 0xFFFDEA

/**
 * @def BIOS_COMCMD6
 * @brief GA COMCMD6 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMCMD6 0xFFFDEC

/**
 * @def BIOS_COMCMD7
 * @brief GA COMCMD7 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMCMD7 0xFFFDEE

/**
 * @def BIOS_COMSTAT
 * @sa bios_comstat
 * @ingroup bios_comm
 *
 * @note
 * Size: 16bit * 8 = 0x10 bytes
 */
#define BIOS_COMSTAT 0xFFFDF0

/**
 * @def BIOS_COMSTAT0
 * @brief GA COMSTAT0 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMSTAT0 0xFFFDF0

/**
 * @def BIOS_COMSTAT1
 * @brief GA COMSTAT1 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMSTAT1 0xFFFDF2

/**
 * @def BIOS_COMSTAT2
 * @brief GA COMSTAT2 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMSTAT2 0xFFFDF4

/**
 * @def BIOS_COMSTAT3
 * @brief GA COMSTAT3 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMSTAT3 0xFFFDF6

/**
 * @def BIOS_COMSTAT4
 * @brief GA COMSTAT4 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMSTAT4 0xFFFDF8

/**
 * @def BIOS_COMSTAT5
 * @brief GA COMSTAT5 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMSTAT5 0xFFFDFA

/**
 * @def BIOS_COMSTAT6
 * @brief GA COMSTAT6 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMSTAT6 0xFFFDFC

/**
 * @def BIOS_COMSTAT7
 * @brief GA COMSTAT7 register cache
 * @ingroup bios_comm
 */
#define BIOS_COMSTAT7 0xFFFDFE

/**
 * @def BIOS_JOY1_HOLD
 * @ingroup bios_input
 * @sa bios_joy1_hold
 */
#define BIOS_JOY1_HOLD 0xFFFE20

/**
 * @def BIOS_JOY1_HIT
 * @ingroup bios_input
 * @sa bios_joy1_hit
 */
#define BIOS_JOY1_HIT 0xFFFE21

/**
 * @def BIOS_JOY2_HOLD
 * @ingroup bios_input
 * @sa bios_joy2_hold
 */
#define BIOS_JOY2_HOLD 0xFFFE22

/**
 * @def BIOS_JOY2_HIT
 * @ingroup bios_input
 * @sa bios_joy2_hit
 */
#define BIOS_JOY2_HIT 0xFFFE23

/**
 * @def BIOS_JOY1_REPEAT_DELAY
 * @ingroup bios_input
 * @sa bios_joy1_repeat_delay
 */
#define BIOS_JOY1_REPEAT_DELAY 0xFFFE24

/**
 * @def BIOS_JOY2_REPEAT_DELAY
 * @ingroup bios_input
 * @sa BIOS_JOY2_REPEAT_DELAY
 */
#define BIOS_JOY2_REPEAT_DELAY 0xFFFE25

/**
 * @def BIOS_VBLANK_HANDLER_FLAGS
 * @ingroup bios_int
 * @sa bios_vblank_handler_flags
 */
#define BIOS_VBLANK_HANDLER_FLAGS 0xFFFE26

#define BIOS_BIT_COPY_SPRLIST 0
#define BIOS_BIT_DO_USERCALL  1

/**
 * @def BIOS_VBLANK_COPY_SPRLIST_FLAG
 * @brief
 * @note For use with @ref BIOS_VBLANK_HANDLER_FLAGS
 */
#define BIOS_FLAG_COPY_SPRLIST (1 << BIOS_BIT_COPY_SPRLIST)

/**
 * @def BIOS_VBLANK_USERCALL_FLAG
 * @brief
 * @note For use with @ref BIOS_VBLANK_HANDLER_FLAGS
 */
#define BIOS_FLAG_DO_USERCALL (1 << BIOS_BIT_DO_USERCALL)

/**
 * @def BIOS_VBLANK_COUNTER
 * @ingroup bios_int
 * @sa bios_vblank_counter
 */
#define BIOS_VBLANK_COUNTER 0xFFFE27

/**
 * @def BIOS_VBLANK_FAST_FLAG
 * @brief When set to non-zero, the BIOS VBLANK handler will skip the color
 * palette copy and the user call
 * @ingroup bios_int
 */
#define BIOS_VBLANK_FAST_FLAG 0xFFFE28

/**
 * @def BIOS_VDP_UPDATE_FLAGS
 * @ingroup bios_vdp
 */
#define BIOS_VDP_UPDATE_FLAGS 0xFFFE29

#define BIOS_BIT_COPY_PALETTE 1

/**
 * @def BIOS_FLAG_COPY_PALETTE
 * @brief VDP Update Flags / Update Palette
 * @ingroup bios_vdp
 */
#define BIOS_FLAG_COPY_PALETTE (1 << BIOS_BIT_COPY_PALETTE)

/**
 * @def BIOS_RANDOM
 * @ingroup bios_misc
 * @sa bios_random
 */
#define BIOS_RANDOM 0xFFFE2A

/**
 * @def BIOS_FONT_TILE_BASE
 * @ingroup bios_misc
 * @sa bios_font_tile_base
 */
#define BIOS_FONT_TILE_BASE 0xFFFE2C

/**
 * @def BIOS_PLANE_WIDTH_CACHE
 * @ingroup bios_vdp
 * @sa bios_plane_width
 */
#define BIOS_PLANE_WIDTH_CACHE 0xFFFE2E

/**
 * @def BIOS_ENTITY_ROUTINES
 * @ingroup bios_vdp
 * @sa bios_entity_routines
 */
#define BIOS_ENTITY_ROUTINES 0xFFFE34

/**
 * @def BIOS_FADEIN_PAL_INDEX
 * @ingroup bios_vdp
 * @sa bios_fadein_cram_index
 */
#define BIOS_FADEIN_PAL_INDEX 0xFFFE46

/**
 * @def BIOS_FADEIN_PAL_LENGTH
 * @ingroup bios_vdp
 * @sa bios_fadein_pal_length
 */
#define BIOS_FADEIN_PAL_LENGTH 0xFFFE47

/**
 * @def BIOS_FADEIN_STEP
 * @ingroup bios_vdp
 * @sa bios_fadein_step
 */
#define BIOS_FADEIN_STEP 0xFFFE48

/**
 * @def BIOS_FADEIN_TARGET_PAL
 * @ingroup bios_vdp
 * @sa bios_fadein_target_pal
 */
#define BIOS_FADEIN_TARGET_PAL 0xFFFE4A

// This is the VRAM layout and commonly referenced settings
// when using the default VDP registers

/**
 * @def BIOS_VDP_DEFAULT_WINDOW
 * @ingroup bios_vdp
 * @brief The default VRAM address for the Window
 */
#define BIOS_VDP_DEFAULT_WINDOW 0xA000

/**
 * @def BIOS_VDP_DEFAULT_PLANEA
 * @ingroup bios_vdp
 * @brief The default VRAM address for Plane A
 */
#define BIOS_VDP_DEFAULT_PLANEA 0xC000

/**
 * @def BIOS_VDP_DEFAULT_PLANEB
 * @ingroup bios_vdp
 * @brief The default VRAM address for Plane B
 */
#define BIOS_VDP_DEFAULT_PLANEB 0xE000

/**
 * @def BIOS_VDP_DEFAULT_SPRLIST
 * @ingroup bios_vdp
 * @brief The default VRAM address for the Sprite Table
 */
#define BIOS_VDP_DEFAULT_SPRLIST 0xB800

/**
 * @def BIOS_VDP_DEFAULT_HSCROLL
 * @ingroup bios_vdp
 * @brief The default VRAM address for the Horizontal Scroll Table
 */
#define BIOS_VDP_DEFAULT_HSCROLL 0xBC00

/**
 * @def BIOS_VDP_DEFAULT_PLANE_WIDTH
 * @ingroup bios_vdp
 * @brief The default Plane width
 */
#define BIOS_VDP_DEFAULT_PLANE_WIDTH 128

/**
 * @sa bios_bios_entry
 * @ingroup bios_system
 */
#if TARGET == MEGACD_MODE1
#define BIOS_ENTRY 0x400280
#else
#define BIOS_ENTRY 0x000280
#endif

/**
 * @sa bios_reset
 * @ingroup bios_system
 */
#if TARGET == MEGACD_MODE1
#define BIOS_RESET 0x400284
#else
#define BIOS_RESET 0x000284
#endif

/**
 * @sa bios_init
 */
#if TARGET == MEGACD_MODE1
#define BIOS_INIT 0x400288
#else
#define BIOS_INIT 0x000288
#endif

/**
 * @sa bios_init_sp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_INIT_SP 0x40028C
#else
#define BIOS_INIT_SP 0x00028C
#endif

/**
 * @sa bios_vblank_handler
 * @ingroup bios_int
 */
#if TARGET == MEGACD_MODE1
#define BIOS_VBLANK_HANDLER 0x400290
#else
#define BIOS_VBLANK_HANDLER 0x000290
#endif

/*
 * There are two functions for setting the HBLANK vector. Both are almost
 * identical but for one minor difference: HBLANK2 will set the address in A1
 * to both the Main CPU vector and the Gate Array vector (ga_hblankvect), while
 * HBLANK1 sets A1 to the Main vector but sets the GA vector to the address of
 * the Main vector, i.e. 0xFFFD0C. It's unclear what this difference entails.
 */

/**
 * @sa bios_set_hblank
 * @param[in] A1.l Pointer to HBLANK handler subroutine
 * @clobber None
 * @ingroup bios_int
 */
#if TARGET == MEGACD_MODE1
#define BIOS_SET_HBLANK 0x400294
#else
#define BIOS_SET_HBLANK 0x000294
#endif

/**
 * @def BIOS_READ_JOYPAD
 * @ingroup bios_input
 *
 * @param[out] [@ref BIOS_JOY1_HIT]
 * @param[out] [@ref BIOS_JOY1_HOLD]
 * @param[out] [@ref BIOS_JOY2_HIT]
 * @param[out] [@ref BIOS_JOY2_HOLD]
 * @clobber d6-d7/a5-a6
 * @sa BIOS_READ_JOYPAD
 */
#if TARGET == MEGACD_MODE1
#define BIOS_READ_JOYPAD 0x400298
#else
#define BIOS_READ_JOYPAD 0x000298
#endif

/**
 * @def BIOS_DETECT_CONTROLLER
 * @sa bios_detect_controller
 * @param[in] a6.l Pointer to joypad data port
 * @param[out] d7.b Controller type
 * @ingroup bios_input
 */
#if TARGET == MEGACD_MODE1
#define BIOS_DETECT_CONTROLLER 0x40029C
#else
#define BIOS_DETECT_CONTROLLER 0x00029C
#endif

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
 * @sa bios_clear_vram
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_CLEAR_VRAM 0x4002A0
#else
#define BIOS_CLEAR_VRAM 0x0002A0
#endif

/**
 * @sa bios_clear_tables
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_CLEAR_TABLES 0x4002A4
#else
#define BIOS_CLEAR_TABLES 0x0002A4
#endif

/**
 * @sa bios_clear_vsram
 * @clobber d0-d2
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_CLEAR_VSRAM 0x4002A8
#else
#define BIOS_CLEAR_VSRAM 0x0002A8
#endif

/**
 * @def BIOS_LOAD_DEFAULT_VDPREGS
 * @clobber d0-d1/a1-a2
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
      - HBLANK scanline count: 0
    0x8F02
      - Auto-increment 2
    0x9100
      - Window plane X position: 0
    0x9200
      - Window plane Y position: 0
 */
#if TARGET == MEGACD_MODE1
#define BIOS_LOAD_DEFAULT_VDPREGS 0x4002AC
#else
#define BIOS_LOAD_DEFAULT_VDPREGS 0x0002AC
#endif

/**
 * @sa bios_load_vdpregs
 * @param[in] A1.l Pointer to register data
 * @clobber d0-d1/a2
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_LOAD_VDPREGS 0x4002B0
#else
#define BIOS_LOAD_VDPREGS 0x0002B0
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
#define BIOS_VDP_FILL 0x4002B4
#else
#define BIOS_VDP_FILL 0x0002B4
#endif

/**
 * @sa bios_vdp_fill_clear
 * @param[in] D0.l Address (vdp_addr)
 * @param[in] D1.w Length (in words)
 * @ingroup bios_vdp
 * @clobber d0-d2
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_VDP_FILL_CLEAR 0x4002B8
#else
#define BIOS_VDP_FILL_CLEAR 0x0002B8
#endif

/**
 * @def BIOS_DMA_FILL_CLEAR
 * @sa bios_dma_fill_clear
 * @param[in] D0.l Address (vdp_command format)
 * @param[in] D1.w Length (in words)
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_DMA_FILL_CLEAR 0x4002BC
#else
#define BIOS_DMA_FILL_CLEAR 0x0002BC
#endif

/**
 * @def BIOS_DMA_FILL
 * @sa bios_dma_fill
 * @param[in] D0.l Address (vdp_command format)
 * @param[in] D1.w Length (in words)
 * @param[in] D2.w Value
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_DMA_FILL 0x4002C0
#else
#define BIOS_DMA_FILL 0x0002C0
#endif

/**
 * @def BIOS_LOAD_MAP
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
#define BIOS_LOAD_MAP 0x4002C4
#else
#define BIOS_LOAD_MAP 0x0002C4
#endif

/**
 * @def BIOS_LOAD_MAP_TEMPLATE
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
#define BIOS_LOAD_MAP_TEMPLATE 0x4002C8
#else
#define BIOS_LOAD_MAP_TEMPLATE 0x0002C8
#endif

/**
 * @sa bios_plane_fill
 * @clobber d0-d3/d5/a5
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_PLANE_FILL 0x4002CC
#else
#define BIOS_PLANE_FILL 0x0002CC
#endif

/**
 * @sa bios_dma_xfer
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_DMA_XFER 0x4002D0
#else
#define BIOS_DMA_XFER 0x0002D0
#endif

/**
 * @sa bios_dma_xfer_word_ram
 * @clobber d0-d3/a6
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_DMA_XFER_WORD_RAM 0x4002D4
#else
#define BIOS_DMA_XFER_WORD_RAM 0x0002D4
#endif

/**
 * @def BIOS_VDP_DISP_ENABLE
 * @brief Enable VDP output
 * @ingroup bios_vdp
 *
 * @details Sets bit 6 on VDP reg. #1. The VDP register buffer is updated.
 */
#if TARGET == MEGACD_MODE1
#define BIOS_VDP_DISP_ENABLE 0x4002D8
#else
#define BIOS_VDP_DISP_ENABLE 0x0002D8
#endif

/**
 * @def BIOS_VDP_DISP_DISABLE
 * @brief Disable VDP output
 * @ingroup bios_vdp
 *
 * @details Clears bit 6 on VDP reg. #1. The VDP register buffer is updated.
 */
#if TARGET == MEGACD_MODE1
#define BIOS_VDP_DISP_DISABLE 0x4002DC
#else
#define BIOS_VDP_DISP_DISABLE 0x0002DC
#endif

/**
 * @sa bios_load_pal
 * @clobber d0
 */
#if TARGET == MEGACD_MODE1
#define BIOS_LOAD_PAL 0x4002E0
#else
#define BIOS_LOAD_PAL 0x0002E0
#endif

/**
 * @sa bios_load_pal_update
 * @clobber d0
 */
#if TARGET == MEGACD_MODE1
#define BIOS_LOAD_PAL_UPDATE 0x4002E4
#else
#define BIOS_LOAD_PAL_UPDATE 0x0002E4
#endif

/**
 * @sa bios_copy_pal
 * @clobber a4,d4
 */
#if TARGET == MEGACD_MODE1
#define BIOS_COPY_PAL 0x4002E8
#else
#define BIOS_COPY_PAL 0x0002E8
#endif

/**
 * @def BIOS_GFX_DECOMP
 * @brief Decompress graphics data in the "Nemesis" format to VRAM
 * @ingroup bios_vdp
 * @param[in] A1.l Pointer to compressed data
 *
 * @warning You must set the destination on the VDP control port before calling
 * this routine!
 */
#if TARGET == MEGACD_MODE1
#define BIOS_GFX_DECOMP 0x4002EC
#else
#define BIOS_GFX_DECOMP 0x0002EC
#endif

/**
 * @def BIOS_GFX_DECOMP_RAM
 * @brief Decompress graphics data in the "Nemesis" format to RAM
 * @param[in] A1.l Pointer to compressed data
 * @param[in] A2.l Pointer to decompressed data buffer
 *
 * GROUP: Decompression
 */
#if TARGET == MEGACD_MODE1
#define BIOS_GFX_DECOMP_RAM 0x4002F0
#else
#define BIOS_GFX_DECOMP_RAM 0x0002F0
#endif

/**
 * @sa bios_process_entities
 * @clobber d0-d4/d6/a2
 */
#if TARGET == MEGACD_MODE1
#define BIOS_PROCESS_ENTITIES 0x4002F4
#else
#define BIOS_PROCESS_ENTITIES 0x0002F4
#endif

/**
 * @sa bios_clear_ram
 * @clobber d6/a6
 */
#if TARGET == MEGACD_MODE1
#define BIOS_CLEAR_RAM 0x4002F8
#else
#define BIOS_CLEAR_RAM 0x0002F8
#endif

/**
 * @def BIOS_UNKNOWN_1F
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
#define BIOS_UNKNOWN_1F 0x4002FC
#else
#define BIOS_UNKNOWN_1F 0x0002FC
#endif

/**
 * @def BIOS_DISP_SPROBJ
 * @brief Display a sprite structure
 * @param[in] A0.l Pointer to parent sprite object
 * @param[in] D6.b Initial value for "next" sprite
 * @clobber d0-d4/a1-a2
 */
#if TARGET == MEGACD_MODE1
#define BIOS_DISP_SPROBJ 0x400300
#else
#define BIOS_DISP_SPROBJ 0x000300
#endif

/**
 * @sa bios_vblank_wait
 * @clobber d0
 */
#if TARGET == MEGACD_MODE1
#define BIOS_VBLANK_WAIT 0x400304
#else
#define BIOS_VBLANK_WAIT 0x000304
#endif

/**
 * @sa bios_vblank_wait_default
 * @clobber d0
 */
#if TARGET == MEGACD_MODE1
#define BIOS_VBLANK_WAIT_DEFAULT 0x400308
#else
#define BIOS_VBLANK_WAIT_DEFAULT 0x000308
#endif

/**
 * @sa bios_copy_sprlist
 * @clobber d4/a4
 */
#if TARGET == MEGACD_MODE1
#define BIOS_COPY_SPRLIST 0x40030C
#else
#define BIOS_COPY_SPRLIST 0x00030C
#endif

/**
 * @def BIOS_UNKNOWN_24
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
#define BIOS_UNKNOWN_24 0x400310
#else
#define BIOS_UNKNOWN_24 0x000310
#endif

/**
 * @def BIOS_SET_HBLANK_WORK_RAM
 * @sa bios_set_hblank_workram
 * @param[in] A1.l Pointer to HBLANK function
 *
 *
 */
#if TARGET == MEGACD_MODE1
#define BIOS_SET_HBLANK_WORK_RAM 0x400314
#else
#define BIOS_SET_HBLANK_WORK_RAM 0x000314
#endif

/**
 * @sa bios_disable_hblank
 */
#if TARGET == MEGACD_MODE1
#define BIOS_DISABLE_HBLANK 0x400318
#else
#define BIOS_DISABLE_HBLANK 0x000318
#endif

/**
 * @sa bios_print
 * @clobber d1-d2/a5
 */
#if TARGET == MEGACD_MODE1
#define BIOS_PRINT 0x40031C
#else
#define BIOS_PRINT 0x00031C
#endif

/**
 * @def BIOS_LOAD_1BPP_TILES
 * @brief Load 1bpp graphics into VDP
 * @param[in] A1.l Pointer to 1bpp graphics data
 * @param[in] D0.l VRAM destination (vdp_cmd)
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
#define BIOS_LOAD_1BPP_TILES 0x400320
#else
#define BIOS_LOAD_1BPP_TILES 0x000320
#endif

/**
 * @def BIOS_LOAD_FONT
 * @brief Load the internal 1bpp ASCII font
 * @param[in] D0.l VRAM destination (vdp_cmd)
 * @param[in] D1.l Color bit map
 * @clobber d2-d4/a1/a5
 * @ingroup bios_misc
 *
 * @details
 * See the notes in @ref BIOS_LOAD_1BPP_TILES for more info about the color bit
 * map. The VRAM destination should place the font no earlier than tile index
 * 0x20 if you are planning to use this with the _PRINT_STRING function.
 */
#if TARGET == MEGACD_MODE1
#define BIOS_LOAD_FONT 0x400324
#else
#define BIOS_LOAD_FONT 0x000324
#endif

/**
 * @def BIOS_LOAD_FONT_DEFAULTS
 * @clobber d0-d4/a1/a5
 */
#if TARGET == MEGACD_MODE1
#define BIOS_LOAD_FONT_DEFAULTS 0x400328
#else
#define BIOS_LOAD_FONT_DEFAULTS 0x000328
#endif

/**
 * @def BIOS_INPUT_REPEAT_DELAY
 * @clobber d1/a1/a5
 */
#if TARGET == MEGACD_MODE1
#define BIOS_INPUT_REPEAT_DELAY 0x40032C
#else
#define BIOS_INPUT_REPEAT_DELAY 0x00032C
#endif

/**
 * @def BIOS_MAP_DECOMP
 * @brief Decompress Enigma data
 * @param[in] D0.w Start tile index
 * @param[in] A1.l Pointer to Enigma compressed data
 * @param[in] A2.l Pointer to output buffer
 * @ingroup bios_cmp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_MAP_DECOMP 0x400330
#else
#define BIOS_MAP_DECOMP 0x000330
#endif

/**
 * @def BIOS_LOAD_STAMP_MAP
 * @brief Load map for a vertically-oriented contiguous group of tiles
 * @param[in] D0.l Destination VRAM address (vdp_addr)
 * @param[in] D1.w Map width - 1
 * @param[in] D2.w Map height - 1
 * @param[in] D3.w Initial tile index
 * @clobber d4-d6/a5
 * @note Uses `plane_width` variable
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_LOAD_STAMP_MAP 0x400334
#else
#define BIOS_LOAD_STAMP_MAP 0x000334
#endif

/**
 * @def BIOS_PRNG_MOD
 * @clobber d1
 */
#if TARGET == MEGACD_MODE1
#define BIOS_PRNG_MOD 0x400338
#else
#define BIOS_PRNG_MOD 0x000338
#endif

/**
 * @def BIOS_PRNG
 * @clobber d0
 */
#if TARGET == MEGACD_MODE1
#define BIOS_PRNG 0x40033C
#else
#define BIOS_PRNG 0x00033C
#endif

/**
 * @def BIOS_CLEAR_COMM
 * @clobber d0/a6
 */
#if TARGET == MEGACD_MODE1
#define BIOS_CLEAR_COMM 0x400340
#else
#define BIOS_CLEAR_COMM 0x000340
#endif

/**
 * @def BIOS_COMM_SYNC
 * @brief Copies COMCMD cache to registers and COMSTAT registers to cache
 *
 */
#if TARGET == MEGACD_MODE1
#define BIOS_COMM_SYNC 0x400344
#else
#define BIOS_COMM_SYNC 0x000344
#endif

/**
 * @def BIOS_UK_COMM_CDINFO
 * @brief UNKNOWN
 *
 * @details
 */
#if TARGET == MEGACD_MODE1
#define BIOS_UK_COMM_CDINFO 0x400348
#else
#define BIOS_UK_COMM_CDINFO 0x000348
#endif

/**
 * @def BIOS_UK_COMMFLAGS_RELATED
 * @brief UNKNOWN
 *
 */
#if TARGET == MEGACD_MODE1
#define BIOS_UK_COMMFLAGS_RELATED 0x40034C
#else
#define BIOS_UK_COMMFLAGS_RELATED 0x00034C
#endif

#if TARGET == MEGACD_MODE1
#define BIOS_UNKNOWN_35 0x400350
#else
#define BIOS_UNKNOWN_35 0x000350
#endif

#if TARGET == MEGACD_MODE1
#define BIOS_UNKNOWN_36 0x400354
#else
#define BIOS_UNKNOWN_36 0x000354
#endif

#if TARGET == MEGACD_MODE1
#define BIOS_UNKNOWN_37 0x400358
#else
#define BIOS_UNKNOWN_37 0x000358
#endif

#if TARGET == MEGACD_MODE1
#define BIOS_UNKNOWN_38 0x40035C
#else
#define BIOS_UNKNOWN_38 0x00035C
#endif

/**
 * @def BIOS_TRIGGER_IFL2
 * @brief Send INT 2 to Sub CPU
 * @clobber a5
 * @ingroup bios_int
 */
#if TARGET == MEGACD_MODE1
#define BIOS_TRIGGER_IFL2 0x400360
#else
#define BIOS_TRIGGER_IFL2 0x000360
#endif

/**
 * @def BIOS_SEGA_LOGO
 * @brief Run the Sega logo startup code
 * @ingroup boot_system
 *
 * @note This should never need to be called from inside the game. It is
 * called automatically as part of the security code during startup.
 */
#if TARGET == MEGACD_MODE1
#define BIOS_SEGA_LOGO 0x400364
#else
#define BIOS_SEGA_LOGO 0x000364
#endif

/**
 * @def BIOS_SET_VBLANK
 * @brief Set a new VBLANK subroutine
 * @param[in] A1.l Pointed to VBLANK subroutinte
 * @ingroup bios_int
 *
 * @note This is relatively useless as a subroutine. It simply moves
 * the adddress into _mlevel6+2. You may as well do the move yourself and skip
 * the stack push/extra cycles from the jsr.
 */
#if TARGET == MEGACD_MODE1
#define BIOS_SET_VBLANK 0x400368
#else
#define BIOS_SET_VBLANK 0x000368
#endif

/**
 * @def BIOS_LOAD_MAP_HORIZ
 * @brief Load map for a horizontally-oriented contiguous group of tiles
 * @param[in] D0.l Destination VRAM address (vdp_addr)
 * @param[in] D1.w Map width
 * @param[in] D2.w Map height
 * @param[in] D3.w Initial tile index
 * @clobber d4/a5
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_LOAD_MAP_HORIZ 0x40036C
#else
#define BIOS_LOAD_MAP_HORIZ 0x00036C
#endif

/**
 * @def _BIOD_LOAD_TILEMAP_PARTIAL
 *
 * GROUP: bios_vdp
 *
 * IN:
 *  d0.l - Destination vdp_addr
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
#define _BIOD_LOAD_TILEMAP_PARTIAL 0x400370
#else
#define _BIOD_LOAD_TILEMAP_PARTIAL 0x000370
#endif

/**
 * @def BIOS_DMA_COPY
 * @clobber d3/a6
 */
#if TARGET == MEGACD_MODE1
#define BIOS_DMA_COPY 0x400374
#else
#define BIOS_DMA_COPY 0x000374
#endif

#if TARGET == MEGACD_MODE1
#define BIOS_UNKNOWN_3D 0x400378
#else
#define BIOS_UNKNOWN_3D 0x000378
#endif

/**
 * @def BIOS_TO_BCD_BYTE
 * @brief Convert a byte value to BCD
 * @param[in] D1.b Hex value
 * @param[out] D1.b BCD value
 * @ingroup bios_misc
 */
#if TARGET == MEGACD_MODE1
#define BIOS_TO_BCD_BYTE 0x40037C
#else
#define BIOS_TO_BCD_BYTE 0x00037C
#endif

/**
 * @def BIOS_TO_BCD
 * @brief Convert a word value to BCD
 * @param[in] D1.w Hex value
 * @param[out] D1.w BCD value
 * @ingroup bios_misc
 */
#if TARGET == MEGACD_MODE1
#define BIOS_TO_BCD 0x400380
#else
#define BIOS_TO_BCD 0x000380
#endif

/**
 * @def BIOS_BLANK_DISPLAY
 * @brief Blanks the display
 * @ingroup bios_vdp

 * @details This routine clears palette index 0 (black) and disables VDP output.
 * @note The VDP register cache will be updated
 */
#if TARGET == MEGACD_MODE1
#define BIOS_BLANK_DISPLAY 0x400384
#else
#define BIOS_BLANK_DISPLAY 0x000384
#endif

/**
 * @def BIOS_PAL_FADEOUT
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_PAL_FADEOUT 0x400388
#else
#define BIOS_PAL_FADEOUT 0x000388
#endif

/**
 * @def BIOS_PAL_FADEIN
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_PAL_FADEIN 0x40038C
#else
#define BIOS_PAL_FADEIN 0x00038C
#endif

/**
 * @def BIOS_SET_FADEIN_PAL
 * @ingroup bios_vdp
 */
#if TARGET == MEGACD_MODE1
#define BIOS_SET_FADEIN_PAL 0x400390
#else
#define BIOS_SET_FADEIN_PAL 0x000390
#endif

/**
 * @def BIOS_DMA_QUEUE
 */
#if TARGET == MEGACD_MODE1
#define BIOS_DMA_QUEUE 0x400394
#else
#define BIOS_DMA_QUEUE 0x000394
#endif

#if TARGET == MEGACD_MODE1
#define BIOS_UNKNOWN_44 0x400398
#else
#define BIOS_UNKNOWN_44 0x000398
#endif

#if TARGET == MEGACD_MODE1
#define BIOS_ADD_TIME_VALUES 0x40039C
#else
#define BIOS_ADD_TIME_VALUES 0x00039C
#endif

#if TARGET == MEGACD_MODE1
#define BIOS_UNKNOWN_46 0x4003A0
#else
#define BIOS_UNKNOWN_46 0x0003A0
#endif

/**
 * @def _COMPRESSED_SUB_BIOS
 * @brief These define the location of the compressed Sub CPU bios stored within
 * the Boot ROM. In order to use the Sub CPU BIOS in Mode 1, you will need to
 * manually decompress it to its proper location in PRG RAM
 *
 * @warning Tech Bulletin #3 indicates that the compressed Sub BIOS is at
 * 0x016000, but it looks like this is not consistent across Mega CD revisions!
 */
// below is the offset for all revisions EXCEPT US/EU Model 1 and LaserActive
// (Mega LD) hardware
#if TARGET == MEGACD_MODE1
#define _COMPRESSED_SUB_BIOS 0x416000
#else
#define _COMPRESSED_SUB_BIOS 0x016000
#endif

#endif

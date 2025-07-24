/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file vdp.def.h
 * @brief VDP hardware definitions
 */

#ifndef MEGADEV__MAIN_VDP_DEF_H
#define MEGADEV__MAIN_VDP_DEF_H

/**
 * @defgroup vdp Main CPU / VDP
 */

/**
 * @defgroup vdp_port Main CPU / VDP / Ports
 */

/**
 * @defgroup vdp_regs Main CPU / VDP / Registers
 */

// VDP ports

/**
 * @def _VDP_CTRL
 * @brief VDP Control Port
 * @ingroup vdp_port
 * @param[width] 16 bit
 * @param[read] Returns VDP status
 * @param[write] Set Main CPU / VDP / Register value / Set VDP I/O address
 * @details A 32 bit write is equivalent to two consecutive 16 bit writes
 *
 * @sa VDP_CTRL_16 VDP_CTRL_32
 */
#define _VDP_CTRL 0xC00004

/**
 * @def VRAM_R
 * @ingroup vdp_port
 * @brief VDP Control Port / Address Mode / VRAM Read
 * @sa _VDP_CTRL
 */
#define VRAM_R 0x00000000

/**
 * @def VRAM_W
 * @ingroup vdp_port
 * @brief VDP Control Port / Address Mode / VRAM Write
 * @sa _VDP_CTRL
 */
#define VRAM_W 0x40000000

/**
 * @def CRAM_R
 * @ingroup vdp_port
 * @brief VDP Control Port / Address Mode / CRAM Read
 * @sa _VDP_CTRL
 */
#define CRAM_R 0x00000020

/**
 * @def CRAM_W
 * @ingroup vdp_port
 * @brief VDP Control Port / Address Mode / CRAM Write
 * @sa _VDP_CTRL
 */
#define CRAM_W 0xC0000000

/**
 * @def VSRAM_R
 * @ingroup vdp_port
 * @brief VDP Control Port / Address Mode / VSRAM Read
 * @sa _VDP_CTRL
 */
#define VSRAM_R 0x00000010

/**
 * @def VSRAM_W
 * @ingroup vdp_port
 * @brief VDP Control Port / Address Mode / VSRAM Write
 * @sa _VDP_CTRL
 */
#define VSRAM_W 0x40000010

/**
 * @def VDPSTAT_PAL_HARDWARE
 * @brief VDP Control Port / VDP Status / PAL Hardware Flag
 *
 */
#define VDPSTAT_PAL_HARDWARE (1 << 0)

/**
 * @def VDPSTAT_DMA_IN_PROGRESS
 * @brief VDP Control Port / VDP Status / DMA In Progress Flag
 * @ingroup vdp_port
 *
 */
#define VDPSTAT_DMA_IN_PROGRESS (1 << 1)

/**
 * @def VDPSTAT_HBLANK_IN_PROGRESS
 * @brief VDP Control Port / VDP Status / HBLANK In Progress Flag
 * @ingroup vdp_port
 *
 */
#define VDPSTAT_HBLANK_IN_PROGRESS (1 << 2)

/**
 * @def VDPSTAT_VBLANK_IN_PROGRESS
 * @brief VDP Control Port / VDP Status / VBLANK In Progress Flag
 * @ingroup vdp_port
 *
 */
#define VDPSTAT_VBLANK_IN_PROGRESS (1 << 3)

/**
 * @def VDPSTAT_ODD_FRAME
 * @brief VDP Control Port / VDP Status / Odd Frame Flag
 * @ingroup vdp_port
 *
 */
#define VDPSTAT_ODD_FRAME (1 << 4)

/**
 * @def VDPSTAT_SPR_COLLISION
 * @brief VDP Control Port / VDP Status / Sprite Collision Flag
 *
 * @details Note: The VDPSTAT_SPR_COLLISION flag is a holdover from the
 * TMS9918, which could only show 4 sprites on a scanline. Since the Mega
 * Drive can show up to 20 sprites per scanline (16 in 32-cell mode), and
 * there's no indication as to which sprites overlapped, this flag is
 * generally considered to be useless.
 *
 */
#define VDPSTAT_SPR_COLLISION (1 << 5)

/**
 * @def VDPSTAT_SPR_LIMIT
 * @brief VDP Control Port / VDP Status / Sprite Limit Flag
 * @ingroup vdp_port
 *
 */
#define VDPSTAT_SPR_LIMIT (1 << 6)

/**
 * @def VDPSTAT_VINT_TRIGGERED
 * @brief VDP Control Port / VDP Status / VBLANK Interrupt Triggered Flag
 * @ingroup vdp_port
 *
 */
#define VDPSTAT_VINT_TRIGGERED (1 << 7)

/**
 * @def VDPSTAT_FIFO_FULL
 * @brief VDP Control Port / VDP Status / FIFO Full Flag
 * @ingroup vdp_port
 *
 */
#define VDPSTAT_FIFO_FULL (1 << 8)

/**
 * @def VDPSTAT_FIFO_EMPTY
 * @brief VDP Control Port / VDP Status / FIFO Empty Flag
 * @ingroup vdp_port
 *
 */
#define VDPSTAT_FIFO_EMPTY (1 << 9)

/**
 * @def _VDP_DATA
 * @brief VDP Data Port
 * @ingroup vdp_port
 * @param[width] 16 bit
 * @param[read] Data read
 * @param[write] Data write
 *
 * @details A 32 bit write is equivalent to two consecutive 16 bit writes
 *
 * @sa VDP_DATA_16 VDP_DATA_32
 */
#define _VDP_DATA 0xC00000

/**
 * @def _VDP_HVCOUNTER
 * @brief Reports the current position of the electron beam on the screen
 * @ingroup vdp
 * @param[width] 16 bit
 * @param[read]
 * \n Upper byte: Vertical position
 * \n Lower byte: Horizontal position
 *
 * @sa VDP_HVCOUNTER
 */
#define _VDP_HVCOUNTER 0xC00008

/**
 * @def _VDP_DEBUG_SELECT
 * @brief VDP Debug Port Selection
 * @ingroup vdp_port
 * @param[width] 16 bit
 * @param[write] Data write
 *
 * @note https://plutiedev.com/vdp-debug
 */
#define _VDP_DEBUG_SELECT 0xC00018

/**
 * @def _VDP_DEBUG
 * @brief VDP Debug Port
 * @ingroup vdp_port
 * @param[width] 16 bit
 * @param[read] Data read
 * @param[write] Data write
 *
 * @note https://plutiedev.com/vdp-debug
 */
#define _VDP_DEBUG 0xC0001C

/**
 * @defgroup vdp_reg00 Main CPU / VDP / Register 00 (Mode Register 1)
 */

/**
 * @def _VDPREG00
 * @brief Mode Register 1
 * @ingroup vdp_regs
 * @ingroup vdp_reg00
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||L|IE1| |CM|M3|DE|
 *
 * @param L [@ref VDP_LEFT_COL_BLANK] Blank leftmost 8 pixels to background color
 * @param IE1 [@ref VDP_HINT_ENABLE] Enable HBLANK interrupts
 * @param CM [@ref VDP_HICOLOR_ENABLE]
 * \n 1: normal 9-bit color mode (512 colour)
 * \n 0: low 3-bit color mode (8 colors)
 * @param M3 [@ref VDP_HVCOUNTER_FREEZE]
 * \n 1: freeze H/V counter on level 2 interrupt
 * \n 0: enable H/V counter
 * @param DE [@ref VDP_DISPLAY_DISABLE]
 * \n 1: disable display
 * \n 0: enable display
 *
 * @aliases _VDPREG_MODE1
 */
#define _VDPREG00 0x8000

/**
 * @def _VDPREG_MODE1
 * @ingroup vdp_regs
 * @ingroup vdp_reg00
 * @aliasof _VDPREG00
 */
#define _VDPREG_MODE1 _VDPREG00

/**
 * @def VDP_DISPLAY_DISABLE
 * @brief Disable Display
 * @ingroup vdp_regs
 * @ingroup vdp_reg00
 */
#define VDP_DISPLAY_DISABLE (1 << 0)

/**
 * @def VDP_HVCOUNTER_FREEZE
 * @brief Freeze HV Counter
 * @ingroup vdp_regs
 * @ingroup vdp_reg00
 *
 */
#define VDP_HVCOUNTER_FREEZE (1 << 1)

/**
 * @def VDP_HICOLOR_ENABLE
 * @brief Enable 9 bit Color Mode
 * @ingroup vdp_regs
 * @ingroup vdp_reg00
 *
 */
#define VDP_HICOLOR_ENABLE (1 << 2)

/**
 * @def VDP_HINT_ENABLE
 * @brief Enable HBLANK Interrupts
 * @ingroup vdp_regs
 * @ingroup vdp_reg00
 *
 */
#define VDP_HINT_ENABLE (1 << 4)

/**
 * @def VDP_LEFT_COL_BLANK
 * @brief Blank leftmost 8 pixels to background color
 * @ingroup vdp_regs
 * @ingroup vdp_reg00
 */
#define VDP_LEFT_COL_BLANK (1 << 5)

/**
 * @defgroup vdp_reg01 Main CPU / VDP / Register 01 (Mode Register 2)
 */

/**
 * @def _VDPREG01
 * @brief Mode Register 2
 * @ingroup vdp_regs
 * @ingroup vdp_reg01
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |VR|DE|IE0|M1|M2|M5| ||
 *
 * @param VR [@ref VDP_128K_VRAM_ENABLE]
 * \n Enable 128kB of VRAM (incompatible with standard consoles)
 * @param DE [@ref VDP_DISPLAY_ENABLE]
 * \n 1: enable display
 * \n 0: fill display with background colour
 * @param IE0 [@ref VDP_VINT_ENABLE]
 * \n Enable VBLANK interrupts (VINT)
 * @param M1 [@ref VDP_DMA_ENABLE]
 * \n 1: enable DMA operations
 * \n 0: ignore DMA operations
 * @param M2 [@ref VDP_PAL_VIDEO]
 * \n 1: 240 pixel (30 cell) PAL mode
 * \n 0: 224 pixel (28 cell) NTSC mode
 * @param M5 [@ref VDP_MD_DISPLAY_MODE]
 * \n 1: Mega Drive (mode 5) display
 * \n 0: Mark III/Master System (mode 4) display
 *
 * @aliases _VDPREG_MODE2
 */
#define _VDPREG01 0x8100

/**
 * @def _VDPREG_MODE2
 * @ingroup vdp_regs
 * @ingroup vdp_reg01
 * @aliasof _VDPREG01
 */
#define _VDPREG_MODE2 _VDPREG01

/**
 * @def VDP_MD_DISPLAY_MODE
 * @brief Display Mode
 * @ingroup vdp_regs
 * @ingroup vdp_reg01
 */
#define VDP_MD_DISPLAY_MODE (1 << 2)

/**
 * @def VDP_PAL_VIDEO
 * @brief Enable PAL video mode
 * @ingroup vdp_regs
 * @ingroup vdp_reg01
 */
#define VDP_PAL_VIDEO (1 << 3)

/**
 * @def VDP_DMA_ENABLE
 * @brief Enable DMA Operations
 * @ingroup vdp_regs
 * @ingroup vdp_reg01
 */
#define VDP_DMA_ENABLE (1 << 4)

/**
 * @def VDP_VINT_ENABLE
 * @brief Enable VBLANK Interrupts
 * @ingroup vdp_regs
 * @ingroup vdp_reg01
 */
#define VDP_VINT_ENABLE (1 << 5)

/**
 * @def VDP_DISPLAY_ENABLE
 * @brief Enable Display Output
 * @ingroup vdp_regs
 * @ingroup vdp_reg01
 */
#define VDP_DISPLAY_ENABLE (1 << 6)

/**
 * @def VDP_128K_VRAM_ENABLE
 * @brief Enable 128k VRAM Mode
 * @ingroup vdp_regs
 * @ingroup vdp_reg01
 */
#define VDP_128K_VRAM_ENABLE (1 << 7)

/**
 * @defgroup vdp_reg02 Main CPU / VDP / Register 02 (Plane A Name Table VRAM Address)
 */

/**
 * @def _VDPREG02
 * @brief Plane A Name Table VRAM Address
 * @ingroup vdp_regs
 * @ingroup vdp_reg02
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | |PA6*|PA5|PA4|PA3| |||
 *
 * @param PA
 * \n Bits 15-13 (PA5 to PA3) of Plane A nametable address in VRAM
 *
 * @details The Plane A register address is effectively the full address (which
 * must be a multiple of 0x2000) divided by 0x400
 * /note PA6 is used with 12k VRAM only
 *
 * @aliases _VDPREG_PLA_ADDR
 */
#define _VDPREG02 0x8200

/**
 * @def _VDPREG_PLA_ADDR
 * @ingroup vdp_regs
 * @ingroup vdp_reg02
 * @aliasof _VDPREG02
 */
#define _VDPREG_PLA_ADDR _VDPREG02

/**
 * @defgroup vdp_reg03 Main CPU / VDP / Register 03 (Window Name Table VRAM Address)
 */

/**
 * @def _VDPREG03
 * @brief Window Name Table VRAM Address
 * @ingroup vdp_regs
 * @ingroup vdp_reg03
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | |W6|W5|W4|W3|W2|W1| |
 *
 * @param W
 * \n Bits 15-11 (W5 to W1) of window nametable address in VRAM
 *
 * @note The Window register address is effectively the full address (which
 * must be a multiple of 0x800) divided by 0x400
 *
 * @note W1 is ignored in 40 cell mode, limiting the address to a multiple of
 * 0x1000
 * @note W6 is used with 128k VRAM only
 *
 * @aliases _VDPREG_WIN_ADDR
 */
#define _VDPREG03 0x8300

/**
 * @def _VDPREG_WIN_ADDR
 * @ingroup vdp_regs
 * @ingroup vdp_reg03
 * @aliasof _VDPREG03
 */
#define _VDPREG_WIN_ADDR _VDPREG03

/**
 * @defgroup vdp_reg04 Main CPU / VDP / Register 04 (Plane B Name Table VRAM Address)
 */

/**
 * @def _VDPREG04
 * @brief Plane B Name Table VRAM Address
 * @ingroup vdp_regs
 * @ingroup vdp_reg04
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||PB3|PB2|PB1|PB0|
 *
 * @param PB Plane B nametable address in VRAM
 *
 * @note The Plane B register address is effectively the full address (which
 * must be a multiple of 0x2000) divided by 0x2000
 *
 * @note PB3 is used with 128k VRAM only
 *
 * @aliases _VDPREG_PLB_ADDR
 */
#define _VDPREG04 0x8400

/**
 * @def _VDPREG_PLB_ADDR
 * @ingroup vdp_regs
 * @ingroup vdp_reg04
 * @aliasof _VDPREG04
 */
#define _VDPREG_PLB_ADDR _VDPREG04

/**
 * @defgroup vdp_reg05 Main CPU / VDP / Register 05 (Sprite Table VRAM Address)
 */

/**
 * @def _VDPREG05
 * @brief Sprite Table VRAM Address
 * @ingroup vdp_regs
 * @ingroup vdp_reg05
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |ST7|ST6|ST5|ST4|ST3|ST2|ST1|ST0|
 *
 * @param ST Sprite table address in VRAM
 *
 * @note The Sprite Table register address is effectively the full address
 * (which must be a multiple of 0x200) divided by 0x200
 * @note ST0 is ignored in 40 cell mode, limiting the address to a multiple
 * of 0x400
 * @note ST7 is used with 128k VRAM only
 *
 * @asliases _VDPREG_SPR_ADDR
 */
#define _VDPREG05 0x8500

/**
 * @def _VDPREG_SPR_ADDR
 * @ingroup vdp_regs
 * @ingroup vdp_reg05
 * @aliasof _VDPREG05
 */
#define _VDPREG_SPR_ADDR _VDPREG05

/**
 * @defgroup vdp_reg06 Main CPU / VDP / Register 06 (Sprite Table VRAM Address for 128k VRAM)
 */

/**
 * @def _VDPREG06
 * @brief Sprite Table VRAM Address (for 128k VRAM)
 * @ingroup vdp_regs
 * @ingroup vdp_reg06
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||SP5||||||
 *
 * @param SP5 Bit 16 of Sprite Table address
 *
 * @note SP5 is used with 128k VRAM only
 *
 * @aliases _VDPREG_SPR_ADDR2
 */
#define _VDPREG06 0x8600

/**
 * @def _VDPREG_SPR_ADDR2
 * @ingroup vdp_regs
 * @ingroup vdp_reg06
 * @aliasof _VDPREG06
 */
#define _VDPREG_SPR_ADDR2 _VDPREG06

/**
 * @defgroup vdp_reg07 Main CPU / VDP / Register 07 (Background Color)
 */

/**
 * @def _VDPREG07
 * @brief Background Color
 * @ingroup vdp_regs
 * @ingroup vdp_reg07
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||PL1|PL0|C3|C2|C1|C0|
 *
 * @param PL  Palette line
 * @param C Palette index
 *
 * @aliases _VDPREG_BGCOLOR
 */
#define _VDPREG07 0x8700

/**
 * @def _VDPREG_BGCOLOR
 * @ingroup vdp_regs
 * @ingroup vdp_reg07
 * @aliasof _VDPREG07
 */
#define _VDPREG_BGCOLOR _VDPREG07

/**
 * @defgroup vdp_reg08 Main CPU / VDP / Register 08 (Mark III/Master System horizontal scroll)
 */

/**
 * @def _VDPREG08
 * @brief Mark III/Master System horizontal scroll
 * @ingroup vdp_regs
 * @ingroup vdp_reg08
 * @note Unused for standard Mega Drive operation
 *
 * @aliases _VDPREG_M3_HSCROLL
 */
#define _VDPREG08 0x8800

/**
 * @def _VDPREG_M3_HSCROLL
 * @ingroup vdp_regs
 * @ingroup vdp_reg08
 * @aliasof _VDPREG08
 */
#define _VDPREG_M3_HSCROLL _VDPREG08

/**
 * @defgroup vdp_reg09 Main CPU / VDP / Register 09 (Mark III/Master System vertical scroll)
 */

/**
 * @def _VDPREG09
 * @brief Mark III/Master System vertical scroll register
 * @ingroup vdp_regs
 * @ingroup vdp_reg09
 * @note Unused for standard Mega Drive operation
 *
 * @aliases _VDPREG_M3_VSROLL
 */
#define _VDPREG09 0x8900

/**
 * @def _VDPREG_M3_VSROLL
 * @ingroup vdp_regs
 * @ingroup vdp_reg09
 * @aliasof _VDPREG09
 */
#define _VDPREG_M3_VSROLL _VDPREG09

/**
 * @defgroup vdp_reg0A Main CPU / VDP / Register 0A (HBLANK Interrupt Counter)
 */

/**
 * @def _VDPREG0A
 * @ingroup vdp_regs
 * @ingroup vdp_reg0A
 * @brief HBLANK Interrupt Counter
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |H7|H6|H5|H4|H3|H2|H1|H0|
 *
 * @param H Scanlines between HBLANK interrupts
 *
 * @aliases _VDPREG_HINT_COUNT
 */
#define _VDPREG0A 0x8A00

/**
 * @def _VDPREG_HINT_COUNT
 * @ingroup vdp_regs
 * @ingroup vdp_reg0A
 * @aliasof _VDPREG0A
 */
#define _VDPREG_HINT_COUNT _VDPREG0A

/**
 * @defgroup vdp_reg0B Main CPU / VDP / Register 0B (Mode Register 3)
 */

/**
 * @def _VDPREG0B
 * @brief Mode Register 3
 * @ingroup vdp_regs
 * @ingroup vdp_reg0B
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||IE2|VS|HS1|HS2|
 *
 * @param IE2 [@ref VDP_EXTINT_ENABLE]
 * \n Enable external interrupts
 * \n 1: Enable
 * \n 0: Disable
 * @param VS [@ref VDP_VSCROLL_MODE]
 * \n Vertical scrolling mode
 * \n 1: 16 pixel columns (1 word per column in VSRAM)
 * \n 0: Full screen (1 long only in VSRAM)
 * @param HS
 * \n Horizontal scrolling mode
 * \n 00: Full screen [@ref VDP_HSCROLL_FULLSCREEN]
 * \n 01: (Invalid)
 * \n 10: Single tile [@ref VDP_HSCROLL_TILE]
 * \n 11: Single pixel rows [@ref VDP_HSCROLL_PIXEL]
 *
 * @aliases _VDPREG_MODE3
 */
#define _VDPREG0B 0x8B00

/**
 * @def _VDPREG_MODE3
 * @ingroup vdp_regs
 * @ingroup vdp_reg0B
 * @aliasof _VDPREG0B
 */
#define _VDPREG_MODE3 _VDPREG0B

/**
 * @def VDP_VSCROLL_MODE
 * @ingroup vdp_regs
 * @ingroup vdp_reg0B
 * @brief Vertical Scrolling Mode
 */
#define VDP_VSCROLL_MODE (1 << 2)

/**
 * @def VDP_EXTINT_ENABLE
 * @ingroup vdp_regs
 * @ingroup vdp_reg0B
 * @brief Enable External Interrupts
 */
#define VDP_EXTINT_ENABLE (1 << 3)

/**
 * @def VDP_HSCROLL_FULLSCREEN
 * @ingroup vdp_regs
 * @ingroup vdp_reg0B
 * @brief Horizontal Scroll Mode / Full Screen
 */
#define VDP_HSCROLL_FULLSCREEN 0b00

/**
 * @def VDP_HSCROLL_TILE
 * @ingroup vdp_regs
 * @ingroup vdp_reg0B
 * @brief Horizontal Scroll Mode / Per Tile
 */
#define VDP_HSCROLL_TILE 0b10

/**
 * @def VDP_HSCROLL_PIXEL
 * @ingroup vdp_regs
 * @ingroup vdp_reg0B
 * @brief Horizontal Scroll Mode / Per Pixel
 */
#define VDP_HSCROLL_PIXEL 0b11

/**
 * @defgroup vdp_reg0C Main CPU / VDP / Register 0C (Mode Register 4)
 */

/**
 * @def _VDPREG0C
 * @brief Mode Register 4
 * @ingroup vdp_regs
 * @ingroup vdp_reg0C
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |RS1|VSY|HSY|EP|SH|LS1|LS0|RS0|
 *
 * @param RS  Screen width
 * \n 1: 40 cell mode (320 pixels)
 * \n 0: 32 cell mode (256 pixel width)
 * \n\em (Both RS0 and RS1 must be the same value.)
 * @param VSY Replace vertical sync signal with pixel bus clock
 * @param HSY (No documentation.)
 * @param EP External pixel bus enable
 * @param SH Enable shadow/highlight mode
 * \n [@ref VDP_SHADOW_HIGHLIGHT]
 * @param LS Interlace mode
 * \n 00: No interlace [@ref VDP_INTERLACE_NONE]
 * \n 01: Interlace normal resolution [@ref VDP_INTERLACE_NORMAL]
 * \n 10: No interlace [@ref VDP_INTERLACE_NONE]
 * \n 11: Interlace double resolution [@ref VDP_INTERLACE_DOUBLE]
 *
 * @aliases _VDPREG_MODE4
 */
#define _VDPREG0C 0x8C00

/**
 * @def _VDPREG_MODE4
 * @ingroup vdp_regs
 * @ingroup vdp_reg0C
 * @aliasof _VDPREG0C
 */
#define _VDPREG_MODE4 _VDPREG0C

/**
 * @def VDP_WIDTH_40CELL
 * @ingroup vdp_regs
 * @ingroup vdp_reg0C
 * @brief Screen Width / 40 Cell
 */
#define VDP_WIDTH_40CELL ((1 << 0) | (1 << 7))

/**
 * @def VDP_SHADOW_HIGHLIGHT
 * @ingroup vdp_regs
 * @ingroup vdp_reg0C
 * @brief Enable Shadow/Highlight Mode
 */
#define VDP_SHADOW_HIGHLIGHT (1 << 3)

/**
 * @def VDP_INTERLACE_NONE
 * @ingroup vdp_regs
 * @ingroup vdp_reg0C
 * @brief Interlace Mode / None
 */
#define VDP_INTERLACE_NONE (0b00 << 1)

/**
 * @def VDP_INTERLACE_NORMAL
 * @ingroup vdp_regs
 * @ingroup vdp_reg0C
 * @brief Interlace Mode / Normal
 */
#define VDP_INTERLACE_NORMAL (0b10 << 1)

/**
 * @def VDP_INTERLACE_DOUBLE
 * @ingroup vdp_regs
 * @ingroup vdp_reg0C
 * @brief Interlace Mode / Double
 */
#define VDP_INTERLACE_DOUBLE (0b11 << 1)

/**
 * @defgroup vdp_reg0D Main CPU / VDP / Register 0D (Horizontal Scroll Data VRAM Address)
 */

/**
 * @def _VDPREG0D
 * @brief Horizontal Scroll Data VRAM Address
 * @ingroup vdp_regs
 * @ingroup vdp_reg0D
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | |HS6|HS5|HS4|HS3|HS2|HS1|HS0|
 *
 * @param HS Horizontal scroll data address in VRAM
 *
 * @note The Horiz Scroll Data register address is effectively the full address
 * (which must be a multiple of 0x400) divided by 0x400
 *
 * @note HS6 is used with 128k VRAM only
 *
 * @aliases _VDPREG_HS_ADDR
 */
#define _VDPREG0D 0x8D00

/**
 * @def _VDPREG_HS_ADDR
 * @ingroup vdp_regs
 * @ingroup vdp_reg0D
 * @aliasof _VDPREG0D
 */
#define _VDPREG_HS_ADDR _VDPREG0D

/**
 * @defgroup vdp_reg0E Main CPU / VDP / Register 0B (Plane A/B Name Table VRAM Address for 128k VRAM)
 */

/**
 * @def _VDPREG0E
 * @brief Plane A/B Name Table VRAM Address (for 128k VRAM)
 * @ingroup vdp_regs
 * @ingroup vdp_reg0E
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | |||PB4| |||PA0|
 *
 * @param PB4 Bit 16 of Plane B nametable address
 * @param PA0 Bit 16 of Plane A nametable address
 *
 * @note Both PB4 and PA0 are used with 128k VRAM only
 *
 * @aliases _VDPREG_PL_ADDR2
 */
#define _VDPREG0E 0x8E00

/**
 * @def _VDPREG_PL_ADDR2
 * @ingroup vdp_regs
 * @ingroup vdp_reg0E
 * @aliasof _VDPREG0E
 */
#define _VDPREG_PL_ADDR2 _VDPREG0E

/**
 * @defgroup vdp_reg0F Main CPU / VDP / Register 0F (Auto-Increment Value)
 */

/**
 * @def _VDPREG0F
 * @brief Auto-Increment Value
 * @ingroup vdp_regs
 * @ingroup vdp_reg0F
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |INC7|INC6|INC5|INC4|INC3|INC2|INC1|INC0|
 *
 * @param INC Autoincrement value
 *
 * @details Value to be added to the VDP address register after each read/write to
 * the data port
 *
 * @note 2 is most common value in this register and many VDP related functions
 * assume that is the value set.
 *
 * @aliases _VDPREG_AUTOINC
 */
#define _VDPREG0F 0x8F00

/**
 * @def _VDPREG_AUTOINC
 * @ingroup vdp_regs
 * @ingroup vdp_reg0F
 * @aliasof _VDPREG0F
 */
#define _VDPREG_AUTOINC _VDPREG0F

/**
 * @defgroup vdp_reg10 Main CPU / VDP / Register 10 (Plane Dimensions)
 */

/**
 * @def _VDPREG10
 * @brief Plane Dimensions
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||H1|H0| ||W1|W0|
 *
 * @param H Height of plane A/B
 * \n 00: 32 cells (256 pixels) [@ref VDP_PL_W32]
 * \n 01: 64 cells (512 pixels) [@ref VDP_PL_W64]
 * \n 10: (Invalid)
 * \n 11: 128 cells (1024 pixels) [@ref VDP_PL_W128]
 * @param W Width of plane A/B
 * \n 00: 32 cells (256 pixels) [@ref VDP_PL_H32]
 * \n 01: 64 cells (512 pixels) [@ref VDP_PL_H64]
 * \n 10: (Invalid)
 * \n 11: 128 cells (1024 pixels) [@ref VDP_PL_H128]
 *
 * @note Height/width settings of 64x128 or 128x128 cells are invalid due to a
 * maximum plane size of 0x2000 bytes
 *
 * @aliases _VDPREG_PL_SIZE
 */
#define _VDPREG10 0x9000

/**
 * @def _VDPREG_PL_SIZE
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @aliasof _VDPREG10
 */
#define _VDPREG_PL_SIZE _VDPREG10

/**
 * @def VDP_PL_W32
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Width 32
 * @sa _VDPREG10
 */
#define VDP_PL_W32 0b0000

/**
 * @def VDP_PL_W64
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Width 64
 * @sa _VDPREG10
 */
#define VDP_PL_W64 0b0001

/**
 * @def VDP_PL_W128
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Width 128
 * @sa _VDPREG10
 */
#define VDP_PL_W128 0b0011

/**
 * @def VDP_PL_H32
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Height 32
 * @sa _VDPREG10
 */
#define VDP_PL_H32 0b0000

/**
 * @def VDP_PL_H64
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Height 64
 * @sa _VDPREG10
 */
#define VDP_PL_H64 0b1000

/**
 * @def VDP_PL_H128
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Height 128
 * @sa _VDPREG10
 */
#define VDP_PL_H128 0b1100

/**
 * @def VDP_PL_32x32
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Width 32 Height 32
 * @sa _VDPREG10
 */
#define VDP_PL_32x32 (VDP_PL_W32 | VDP_PL_H32)

/**
 * @def VDP_PL_32x64
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Width 32 Height 64
 * @sa _VDPREG10
 */
#define VDP_PL_32x64 (VDP_PL_W32 | VDP_PL_H64)

/**
 * @def VDP_PL_32x128
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Width 32 Height 128
 * @sa _VDPREG10
 */
#define VDP_PL_32x128 (VDP_PL_W32 | VDP_PL_H128)

/**
 * @def VDP_PL_64x32
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Width 64 Height 32
 * @sa _VDPREG10
 */
#define VDP_PL_64x32 (VDP_PL_W64 | VDP_PL_H32)

/**
 * @def VDP_PL_64x64
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Width 64 Height 64
 * @sa _VDPREG10
 */
#define VDP_PL_64x64 (VDP_PL_W64 | VDP_PL_H64)

/**
 * @def VDP_PL_64x128
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Width 64 Height 128
 * @sa _VDPREG10
 */
#define VDP_PL_64x128 (VDP_PL_W64 | VDP_PL_H128)

/**
 * @def VDP_PL_128x32
 * @ingroup vdp_regs
 * @ingroup vdp_reg10
 * @brief Plane Dimensions / Width 128 Height 32
 * @sa _VDPREG10
 */
#define VDP_PL_128x32 (VDP_PL_W128 | VDP_PL_H32)

/**
 * @defgroup vdp_reg11 Main CPU / VDP / Register 11 (Window Plane Horizontal Position)
 */

/**
 * @def _VDPREG11
 * @brief Window Plane Horizontal Position
 * @ingroup vdp_regs
 * @ingroup vdp_reg11
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |R| ||HP4|HP3|HP2|HP1|HP0|
 *
 * @param R Window edge selection
 * \n 1: Draw window from HP to right edge of screen
 * \n 0: Draw window from HP to left edge of screen
 * @param HP Horizontal position on screen to start drawing the
 * window plane (in cells)
 *
 * @aliases _VDPREG_WIN_HPOS
 */
#define _VDPREG11 0x9100

/**
 * @def _VDPREG_WIN_HPOS
 * @ingroup vdp_regs
 * @ingroup vdp_reg11
 * @aliasof _VDPREG11
 */
#define _VDPREG_WIN_HPOS _VDPREG11

/**
 * @defgroup vdp_reg12 Main CPU / VDP / Register 12 (Window Plane Vertical Position)
 */

/**
 * @def _VDPREG12
 * @brief Window Plane Vertical Position
 * @ingroup vdp_regs
 * @ingroup vdp_reg12
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |D| ||VP4|VP3|VP2|VP1|VP0|
 *
 * @param D Window edge selection
 * \n 1: Draw window from VP to bottom edge of screen
 * \n 0: Draw window from VP to top edge of screen
 * @param VP Vertical position on screen to start drawing the
 * window plane (in cells)
 *
 * @aliases _VDPREG_WIN_VPOS
 */
#define _VDPREG12 0x9200

/**
 * @def _VDPREG_WIN_VPOS
 * @ingroup vdp_regs
 * @ingroup vdp_reg12
 * @aliasof _VDPREG12
 */
#define _VDPREG_WIN_VPOS _VDPREG12

/**
 * @defgroup vdp_reg13 Main CPU / VDP / Register 13 (DMA Length Low Byte)
 */

/**
 * @def _VDPREG13
 * @brief DMA Length (Low Byte)
 * @ingroup vdp_regs
 * @ingroup vdp_reg13
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |L7|L6|L5|L4|L3|L2|L1|L0|
 *
 * @param L Low byte of DMA length in bytes, divided by 2
 *
 * @aliases _VDPREG_DMA_SZ1
 */
#define _VDPREG13 0x9300

/**
 * @def _VDPREG_DMA_SZ1
 * @ingroup vdp_regs
 * @ingroup vdp_reg13
 * @aliasof _VDPREG13
 */
#define _VDPREG_DMA_SZ1 _VDPREG13

/**
 * @defgroup vdp_reg14 Main CPU / VDP / Register 14 (DMA Length High Byte)
 */

/**
 * @def _VDPREG14
 * @brief DMA Length (High Byte)
 * @ingroup vdp_regs
 * @ingroup vdp_reg14
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |H7|H6|H5|H4|H3|H2|H1|H0|
 *
 * @param H High byte of DMA length in bytes, divided by 2
 *
 * @aliases _VDPREG_DMA_SZ2
 */
#define _VDPREG14 0x9400

/**
 * @def _VDPREG_DMA_SZ2
 * @ingroup vdp_regs
 * @ingroup vdp_reg14
 * @aliasof _VDPREG14
 */
#define _VDPREG_DMA_SZ2 _VDPREG14

/**
 * @defgroup vdp_reg15 Main CPU / VDP / Register 15 (DMA Source Low Byte)
 */

/**
 * @def _VDPREG15
 * @brief DMA Source (Low Byte)
 * @ingroup vdp_regs
 * @ingroup vdp_reg15
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |L7|L6|L5|L4|L3|L2|L1|L0|
 *
 * @param L Low byte of DMA source address, divided by 2
 *
 * @aliases _VDPREG_DMA_SRC1
 */
#define _VDPREG15 0x9500

/**
 * @def _VDPREG_DMA_SRC1
 * @ingroup vdp_regs
 * @ingroup vdp_reg15
 * @aliasof _VDPREG15
 */
#define _VDPREG_DMA_SRC1 _VDPREG15

/**
 * @defgroup vdp_reg16 Main CPU / VDP / Register 16 (DMA Source Mid Byte)
 */

/**
 * @def _VDPREG16
 * @brief DMA Source (Mid Byte)
 * @ingroup vdp_regs
 * @ingroup vdp_reg16
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |M7|M6|M5|M4|M3|M2|M1|M0|
 *
 * @param M Middle byte of DMA source address, divided by 2
 *
 * @aliases _VDPREG_DMA_SRC2
 */
#define _VDPREG16 0x9600

/**
 * @def _VDPREG_DMA_SRC2
 * @ingroup vdp_regs
 * @ingroup vdp_reg16
 * @aliasof _VDPREG16
 */
#define _VDPREG_DMA_SRC2 _VDPREG16

/**
 * @defgroup vdp_reg17 Main CPU / VDP / Register 17 (DMA Source High Byte)
 */

/**
 * @def _VDPREG17
 * @brief DMA Source (High Byte)
 * @ingroup vdp_regs
 * @ingroup vdp_reg17
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |T1|T0|H5|H4|H3|H2|H1|H0|
 *
 * @param H High byte of DMA source address, divided by 2
 * @param T DMA type
 * \n 0x: 68k to VRAM copy (T0 used as highest bit in source address)
 * \n 10: VRAM fill (source can be left blank)
 * \n 11: VRAM to VRAM copy
 *
 * @aliases _VDPREG_DMA_SRC3
 */
#define _VDPREG17 0x9700

/**
 * @def _VDPREG_DMA_SRC3
 * @ingroup vdp_regs
 * @ingroup vdp_reg17
 * @aliasof _VDPREG17
 */
#define _VDPREG_DMA_SRC3 _VDPREG17

// some simple colors for quick reference
#define COLOR_BLACK 0b000000000000
#define COLOR_WHITE 0b111011101110
#define COLOR_BLUE 0b111000000000
#define COLOR_GREEN 0b000011100000
#define COLOR_RED 0b000000001110


/**
 * @def VRAM_AT
 * @brief VRAM address for the given tile index
 * (assuming tile data begins at 0 in VRAM)
 */
#define VRAM_AT(chridx) ((chridx) << 5)

/**
 * @def TILE_AT
 * @brief Tile index of the specified VRAM address
 * (assuming tile data begins at 0 in VRAM)
 */
#define TILE_AT(vram_addr) ((vram_addr) >> 5)

/**
 * @def VDPCMD
 * @brief Converts a 16 bit VRAM address into VDP format at compile time if
 * possible
 */
#define VDPPTR(addr) \
	(__builtin_constant_p(addr) ? (unsigned) ((((addr) &0x3FFF) << 16) + (((addr) &0xC000) >> 14)) : to_vdpptr(addr))
	
#endif

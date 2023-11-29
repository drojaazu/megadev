/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file vdp.def.h
 * @brief VDP hardware definitions
 */

#ifndef MEGADEV__MAIN_VDP_DEF_H
#define MEGADEV__MAIN_VDP_DEF_H

// VDP ports

/**
 * @var _VDP_CTRL
 * @brief VDP control port
 * @details 16 bit wide - a 32 bit write is equivalent to two consecutive
 * 16 bit writes
 * @note Reading this port (16 bit) returns the VDP status
 */
#define _VDP_CTRL 0xC00004

#define STATUS_PAL_HARDWARE (1 << 0)
#define STATUS_DMA_IN_PROGRESS (1 << 1)
#define STATUS_HBLANK_IN_PROGRESS (1 << 2)
#define STATUS_VBLANK_IN_PROGRESS (1 << 3)
#define STATUS_ODD_FRAME (1 << 4)
/*
Note: The SPR_COLLISION flag is a holdover from the TMS9918, which could only show 
4 sprites on a scanline. Since the Mega Drive can show up to 20 sprites per scanline
(16 in 32-cell mode), and there's no indication as to which sprites overlapped, this
bit is useless and should be ignored. 
*/
#define STATUS_SPR_COLLISION (1 << 5)
#define STATUS_SPR_LIMIT (1 << 6)
#define STATUS_VINT_TRIGGERED (1 << 7)
#define STATUS_FIFO_FULL (1 << 8)
#define STATUS_FIFO_EMPTY (1 << 9)

/**
 * @var _VDP_DATA
 * @brief VDP data port
 * @details 16 bit wide - a 32 bit write is equivalent to two consecutive
 * 16 bit writes
 */
#define _VDP_DATA 0xC00000

/**
 * @var _VDP_HVCOUNT
 * @brief HV Counter
 */
#define _VDP_HVCOUNT 0xC00008

/**
 * @var _VDP_DEBUG
 * @brief VDP debug port
 */
#define _VDP_DEBUG 0xC0001C

// All VDP registers
/**
 * @var _VDPREG00_MODE1
 * @brief Mode Register 1
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||L|IE1| |CM|M3|DE|
 *
 * @param L Blank leftmost 8 pixels to background color
 * @param IE1 Enable horizontal interrupts
 * @param CM 1 = normal 9-bit color mode (512 colour); 0 = low 3-bit color mode
 * (8 colors)
 * @param M3 1 = freeze H/V counter on level 2 interrupt; 0 = enable
 * H/V counter
 * @param DE 0 = enable display; 1 = disable display
 */
#define _VDPREG00_MODE1 0x8000

#define VR00_DISPLAY_DISABLE (1 << 0)
#define VR00_HVCOUNTER_FREEZE (1 << 1)
#define VR00_HICOLOR_ENABLE (1 << 2)
#define VR00_HINT_ENABLE (1 << 4)
#define VR00_LEFT_COL_BLANK (1 << 5)

/**
 * @def _VDPREG01_MODE2
 * @brief Mode Register 2
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |VR|DE|IE0|M1|M2|M5| ||
 *
 * @param VR Enable 128kB of VRAM (incompatible with standard consoles)
 * @param DE 1 = enable display; 0 = fill display with background colour
 * @param IE0 Enable vertical interrupts (VINT)
 * @param M1 1 = enable DMA operations; 0 = ignore DMA operations
 * @param M2 1 = 240 pixel (30 cell) PAL mode; 0 = 224 pixel (28 cell) NTSC mode
 * @param M5 1 = Mega Drive (mode 5) display; 0 = Master System (mode 4) display
 */
#define _VDPREG01_MODE2 0x8100

#define VR01_MD_DISPLAY_ENABLE (1 << 2)
#define VR01_PAL_VIDEO (1 << 3)
#define VR01_DMA_ENABLE (1 << 4)
#define VR01_VINT_ENABLE (1 << 5)
#define VR01_DISPLAY_ENABLE (1 << 6)
#define VR01_128K_VRAM_ENABLE (1 << 7)

/**
 * @def _VDPREG02_PLA_ADDR
 * @brief Plane A Name Table VRAM Address
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | |PA6*|PA5|PA4|PA3| |||
 *
 * @param PA Bits 15-13 (PA5 to PA3) of Plane A nametable address in VRAM
 *
 * /details The Plane A register address is effectively the full address (which
 * must be a multiple of 0x2000) divided by 0x400
 * /note PA6 is used with 12k VRAM only
 */
#define _VDPREG02_PLA_ADDR 0x8200

/**
 * @def _VDPREG03_WD_ADDR
 * @brief Window Name Table VRAM Address
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | |W6|W5|W4|W3|W2|W1| |
 *
 * @param W Bits 15-11 (W5 to W1) of window nametable address in VRAM
 *
 * @note The Window register address is effectively the full address (which
 * must be a multiple of 0x800) divided by 0x400
 *
 * @note W1 is ignored in 40 cell mode, limiting the address to a multiple of
 * 0x1000
 * @note W6 is used with 128k VRAM only
 */
#define _VDPREG03_WD_ADDR 0x8300

/**
 * @def _VDPREG04_PLB_ADDR
 * @brief Plane B Name Table VRAM Address
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||PB3|PB2|PB1|PB0|
 *
 * @param PB Bits 15-13 (PB2 to PB0) of Plane B nametable address in VRAM
 *
 * @note The Plane B register address is effectively the full address (which
 * must be a multiple of 0x2000) divided by 0x2000
 * @note PB3 is used with 128k VRAM only
 */
#define _VDPREG04_PLB_ADDR 0x8400

/**
 * @def _VDPREG05_SPR_ADDR
 * @brief Sprite Table VRAM Address
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |ST7|ST6|ST5|ST4|ST3|ST2|ST1|ST0|
 *
 * @param ST Bits 15-9 (ST6 to ST0) of sprite table address in VRAM
 *
 * @note The Sprite Table register address is effectively the full address
 *          (which must be a multiple of 0x200) divided by 0x200
 * @note ST0 is ignored in 40 cell mode, limiting the address to a multiple
 * of 0x400
 * @note ST7 is used with 128k VRAM only
 */
#define _VDPREG05_SPR_ADDR 0x8500

/**
 * @def _VDPREG06_SPR_ADDR2
 * @brief Sprite Table VRAM Address (128k VRAM)
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||SP5||||||
 *
 * @param SP5 Bit 16 of Sprite Table address
 *
 * @note SP5 is used with 128k VRAM only
 */
#define _VDPREG06_SPR_ADDR2 0x8600

/**
 * @def _VDPREG07_BGCOLOR
 * @brief Background Color
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||PL1|PL0|C3|C2|C1|C0|
 *
 * @param PL Palette line
 * @param C Palette index (color)
 */
#define _VDPREG07_BGCOLOR 0x8700

/**
 * @def _VDPREG08_M3_HS
 * @brief Unused
 * @details Master System horizontal scroll register
 */
#define _VDPREG08_M3_HS 0x8800

/**
 * @def _VDPREG09_M3_VS
 * @brief Unused
 * @details Master System vertical scroll register
 */
#define _VDPREG09_M3_VS 0x8900

/**
 * @def _VDPREG0A_HINT_COUNT
 * @brief Horizontal Interrupt Counter
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |H7|H6|H5|H4|H3|H2|H1|H0|
 *
 * @param H Number of scanlines between horizontal interrupts
 */
#define _VDPREG0A_HINT_COUNT 0x8A00

/**
 * @def _VDPREG0B_MODE3
 * @brief Mode Register 3
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||IE2|VS|HS1|HS2|
 *
 * @param IE2 Enable external interrupts
 * \n 1: Enable
 * \n 0: Disable
 * @param VS Vertical scrolling mode
 * \n 1: 16 pixel columns (1 word per column in VSRAM)
 * \n 0: Full screen (1 long only in VSRAM)
 * @param HS Horizontal scrolling mode
 * \n 00: Full screen
 * \n 01: (Invalid)
 * \n 10: 8 pixel rows
 * \n 11: Single pixel rows
 */
#define _VDPREG0B_MODE3 0x8B00

#define VR0B_VSCROLL_16PIXEL (1 << 2)
#define VR0B_EXTINT_ENABLE (1 << 3)
#define VR0B_HSCROLL_FULLSCREEN 0b00
#define VR0B_HSCROLL_CELL 0b10
#define VR0B_HSCROLL_PIXEL 0b11

/**
 * @def _VDPREG0C_MODE4
 * @brief Mode Register 4
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |RS1|VSY|HSY|EP|SH|LS1|LS0|RS0|
 *
 * @param RS Screen width
 * \n 1: 40 cell mode (320 pixels)
 * \n 0: 32 cell mode (256 pixel width)
 * \n\em (Both RS0 and RS1 must be the same value.)
 * @param VSY Replace vertical sync signal with pixel bus clock
 * @param HSY (No documentation.)
 * @param EP Enable external pixel bus
 * @param SH Enable shadow/highlight mode
 * @param LS Interlace mode
 * \n 00: No interlace
 * \n 01: Interlace normal resolution
 * \n 10: No interlace
 * \n 11: Interlace double resolution
 */
#define _VDPREG0C_MODE4 0x8C00

#define VR0C_WIDTH_40CELL ((1 << 0) | (1 << 7))
#define VR0C_SHADOW_HIGHLIGHT (1 << 3)
#define VR0C_INTERLACE_NONE (0b00 << 1)
#define VR0C_INTERLACE_NORMAL (0b10 << 1)
#define VR0C_INTERLACE_DOUBLE (0b11 << 1)

/**
 * @def _VDPREG0D_HS_ADDR
 * @brief Horizontal Scroll Data VRAM Address
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | |HS6|HS5|HS4|HS3|HS2|HS1|HS0|
 *
 * @param HS Bits 15-10 (HS5 to HS0) of horizontal scroll data address in VRAM
 *
 * @note The Horiz Scroll Data register address is effectively the full address
 * (which must be a multiple of 0x400) divided by 0x400
 * @note HS6 is used with 128k VRAM only
 *
 */
#define _VDPREG0D_HS_ADDR 0x8D00

/**
 * @def _VDPREG0E_PL_ADDR2
 * @brief Plane A/B Name Table VRAM Address (128k VRAM)
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | |||PB4| |||PA0|
 *
 * @param PB4 Bit 16 of Plane B nametable address
 * @param PA0 Bit 16 of Plane A nametable address
 *
 * @note Both PB4 and PA0 are used with 128k VRAM only
 */
#define _VDPREG0E_PL_ADDR2 0x8E00

/**
 * @def _VDPREG0F_AUTOINC
 * @brief Auto-Increment Value
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |INC7|INC6|INC5|INC4|INC3|INC2|INC1|INC0|
 *
 * @param INC Value to be added to the VDP address register after each
 * read/write to the data port
 *
 * @note 2 is most common value in this register and many VDP related functions
 * assume that is the value set in this register.
 */
#define _VDPREG0F_AUTOINC 0x8F00

/**
 * @def _VDPREG10_PL_SIZE
 * @brief Plane Dimensions
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||H1|H0| ||W1|W0|
 *
 * @param H Height setting for planes A & B
 * \n 00: 32 cells (256 pixels)
 * \n 01: 64 cells (512 pixels)
 * \n 10: (Invalid)
 * \n 11: 128 cells (1024 pixels)
 * @param W Width setting for planes A & B
 * \n 00: 32 cells (256 pixels)
 * \n 01: 64 cells (512 pixels)
 * \n 10: (Invalid)
 * \n 11: 128 cells (1024 pixels)
 *
 * @note Height/width settings of 64x128 or 128x128 cells are invalid due to a
 * maximum plane size of 0x2000 bytes
 */
#define _VDPREG10_PL_SIZE 0x9000

// these defines can be ORed with the above vdp register to set a size
// e.g. _VDP_PL_SIZE | W32 | H64 for width of 32 tiles, height of 64 tiles
#define VR10_W32  (0x00 << 0)
#define VR10_W64  (0x01 << 0)
#define VR10_W128 (0x03 << 0)
#define VR10_H32  (0x00 << 4)
#define VR10_H64  (0x10 << 4)
#define VR10_H128 (0x30 << 4)

/**
 * @def _VDPREG11_WD_HPOS
 * @brief Window Plane Horizontal Position
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
 */
#define _VDPREG11_WD_HPOS 0x9100

/**
 * @def _VDPREG12_WD_VPOS
 * @brief Window Plane Vertical Position
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
 */
#define _VDPREG12_WD_VPOS 0x9200

/**
 * @def _VDPREG13_DMA_SZ1
 * @brief DMA Length (Low Byte)
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |L7|L6|L5|L4|L3|L2|L1|L0|
 *
 * L7-L0: Low byte of DMA length in bytes, divided by 2
 */
#define _VDPREG13_DMA_SZ1 0x9300

/**
 * @def _VDPREG14_DMA_SZ2
 * @brief DMA Length (High Byte)
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |H7|H6|H5|H4|H3|H2|H1|H0|
 *
 * H7-H0: High byte of DMA length in bytes, divided by 2
 */
#define _VDPREG14_DMA_SZ2 0x9400

/**
 * @def _VDPREG15_DMA_SRC1
 * @brief DMA Source (Low Byte)
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |L7|L6|L5|L4|L3|L2|L1|L0|
 *
 * L7-L0: Low byte of DMA source address, divided by 2
 */
#define _VDPREG15_DMA_SRC1 0x9500

/**
 * @def _VDPREG16_DMA_SRC2
 * @brief DMA Source (Mid Byte)
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |M7|M6|M5|M4|M3|M2|M1|M0|
 *
 * M7-M0: Middle byte of DMA source address, divided by 2
 */
#define _VDPREG16_DMA_SRC2 0x9600

/**
 * @def _VDPREG17_DMA_SRC3
 * @brief DMA Source (High Byte)
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |T1|T0|H5|H4|H3|H2|H1|H0|
 *
 * H5-H0: High byte of DMA source address, divided by 2
 * T1/T0: DMA type
 *        0x = 68k to VRAM copy
 *             (T0 is used as the highest bit in source address)
 *        10 = VRAM fill (source can be left blank)
 *        11 = VRAM to VRAM copy
 */
#define _VDPREG17_DMA_SRC3 0x9700

// VDP command/bus
// OR these values on to the vdpptr formatted base address
#define VRAM_R 0x00000000
#define VRAM_W 0x40000000
#define CRAM_R 0x00000020
#define CRAM_W 0xC0000000
#define VSRAM_R 0x00000010
#define VSRAM_W 0x40000010

// some simple colors for quick reference
#define COLOR_BLACK 0b000000000000
#define COLOR_WHITE 0b111011101110
#define COLOR_BLUE 0b111000000000
#define COLOR_GREEN 0b000011100000
#define COLOR_RED 0b000000001110

#endif

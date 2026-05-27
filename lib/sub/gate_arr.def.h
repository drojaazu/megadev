/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.def.h
 * @brief Sub CPU side Gate Array registers
 */

#pragma once

// clang-format off
/**
 * @defgroup sub__garegs Sub CPU / Gate Array
 */

/**
 * @defgroup sub__garegs__sysctrl Sub CPU / Gate Array / CPU and RAM Control
 * @ingroup sub__garegs
 */

/**
 * @defgroup sub__garegs__cdc Sub CPU / Gate Array / CDC Operation
 * @ingroup sub__garegs
 */

/**
 * @defgroup sub__garegs__cdd Sub CPU / Gate Array / CDD Operation
 * @ingroup sub__garegs
 */

/**
 * @defgroup sub__garegs__led Sub CPU / Gate Array / LED Operation
 * @ingroup sub__garegs
 */

 /**
 * @defgroup sub__garegs__comm Sub CPU / Gate Array / CPU Communication
 * @ingroup sub__garegs
 */

 /**
 * @defgroup sub__garegs__font Sub CPU / Gate Array / Font Generator
 * @ingroup sub__garegs
 */

 /**
 * @defgroup sub__garegs__gfx Sub CPU / Gate Array / Graphics Transforms
 * @ingroup sub__garegs
 */

  /**
 * @defgroup sub__garegs__subcode Sub CPU / Gate Array / Disc Subcode
 * @ingroup sub__garegs
 */

 /**
 * @defgroup sub__garegs__misc Sub CPU / Gate Array / Misc
 * @ingroup sub__garegs
 */
// clang-format on

/**
 * @def GA_REG_RESET
 * @brief CPU Reset / LED Control / ROM Version
 * @ingroup sub__garegs__sysctrl
 * @ingroup ga_reg_sub_led
 * @ingroup ga_reg_sub_misc
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | ||||||LEDG|LEDR|Ver3|Ver2|Ver1|Ver0| |||RES0|
 *
 * @param[r] RES0 CPU Reset
 * \n Reset time is 100ms.
 * - 0: Reset in progress
 * - 1: Reset complete / Reset possible
 * @param[w] RES0 CPU Reset
 * - 0: Request CPU Reset
 * - 1: No effect
 * @param[rw] LEDR Red LED control
 * - 0: Off
 * - 1: On
 * @param[rw] LEDG Green LED control
 * - 0: Off
 * - 1: On
 * @param[r] Ver ROM Version
 */
#define GA_REG_RESET 0xFF8000

#define GA_BIT_LED_R 0
#define GA_BIT_LED_G 1

/**
 * @def GA_MASK_LED_R
 * @brief Red LED Control
 * @sa ga_reg_reset
 * @ingroup ga_reg_sub_led
 */
#define GA_MASK_LED_R (1 << GA_BIT_LED_R)

/**
 * @def GA_MASK_LED_G
 * @brief Green LED Control
 * @sa ga_reg_reset
 * @ingroup ga_reg_sub_led
 */
#define GA_MASK_LED_G (1 << GA_BIT_LED_G)

/**
 * @def GA_MASK_VERSION
 * @brief ROM Version
 * @sa ga_reg_reset
 * @ingroup ga_reg_sub_misc
 */
#define GA_MASK_VERSION (0b1111 << 4)

/**
 * @def GA_REG_MEMMODE
 * @brief Word RAM Control / PRG RAM Write Protect Status / Priority Mode
 * @ingroup sub__garegs__sysctrl
 *
 * @details
 * | | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |WP7|WP6|WP5|WP4|WP3|WP2|WP1|WP0| | | |PM1|PM0|MODE|DMNA|RET|
 * |R|◯|◯|◯|◯|◯|◯|◯|◯| | | |◯|◯|◯|◯|◯|
 * |W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙| | | |◯|◯|◯|🗙|◯|
 *
 * @param WP Write protect Sub CPU RAM [ro]
 *
 * @param PM Priority Mode [rw]
 *    \n See official Mega CD manual for details
 *
 * @param MODE Word RAM layout [rw]
 *    \n Read:
 *    - 0: 2M
 *    - 1: 1M
 *    .
 *    Write:
 *    - 0: 2M
 *    - 1: 1M
 *    .
 * @param DMNA Main CPU will not access Word RAM [r]
 *
 * @param RET Word RAM control
 *    - 2M mode: Give Word RAM control to Main CPU
 *    - 1M mode: Swap 1M block ownership
 *
 * @sa GA_MASK_RETURN_2M
 */
#define GA_REG_MEMMODE 0xFF8002

#define GA_BIT_RETURN_2M      0
#define GA_BIT_DMNA           1
#define GA_BIT_WORDRAM_LAYOUT 2

/**
 * @def GA_MASK_RETURN_2M
 * @brief GA Reg 01 - Change Word RAM ownership bitmask
 * @ingroup sub__garegs__sysctrl
 */
#define GA_MASK_RETURN_2M (1 << GA_BIT_RETURN_2M)

/**
 * @def GA_MASK_DMNA
 * @brief GA Reg 01 - Main CPU no Word RAM access flag bitmask
 * @sa sub__garegs__sysctrl
 * @ingroup ga_reg_sub_memmode
 */
#define GA_MASK_DMNA (1 << GA_BIT_DMNA)

/**
 * @def GA_MASK_WORDRAM_LAYOUT
 * @brief GA Reg 01 - Word RAM layout bitmask
 * @sa sub__garegs__sysctrl
 * @ingroup ga_reg_sub_memmode
 */
#define GA_MASK_WORDRAM_LAYOUT (1 << GA_BIT_WORDRAM_LAYOUT)

/**
 * @def GA_REG_CDC_MODE
 * @brief CDC Mode and Registers #1
 * @ingroup sub__garegs__cdc
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |EDT|DSR|UBR| | |DD2|DD1|DD0| | | | |CA3|CA2|CA1|CA0|
 * |R|◯︎|◯|◯| | |◯|◯|◯| | | | |◯|◯|◯|◯|
 * |W|🗙|🗙|🗙| | |◯|◯|◯| | | | |◯|◯|◯|◯|
 *
 * @param CA CDC register address [rw]
 *    \n Details for this register can be found in the LC89510 manual
 *
 * @param DSR Data set ready [ro]
 *    \n Data from the CDC is present in the CDC Host Data register
 *
 * @param UBR Upper byte ready [ro]
 *    \n Indicates the upper byte has been sent from the CDC. Cleared when
 the lower byte is sent.`
 *
 * @param EDT End of data transfer [ro]
 *
 * @param DD Device destination [rw]
 *    \n Specifies the destination for CDC data transfer, as defined
 * here:
 *   |DD2|DD1|DD0|Destination|
 *   |:|:|:|:|
 *   |0|1|0|Main CPU|
 *   |0|1|1|Sub CPU|
 *   |1|0|0|PCM DMA|
 *   |1|0|1|PRG RAM DMA|
 *   |1|1|1|In 2M Mode: Word RAM \n In 1M Mode: Sub CPU controlled Word RAM|
 *
 *   All other values for DD are invalid.
 */
#define GA_REG_CDC_MODE 0xFF8004

#define GA_MASK_CDC_REGS1 (0b1111 << 0)

#define GA_MASK_CDC_DEST (0b111 << 8)

#define GA_MASK_CDC_MAIN_READ (0b010 << 8)
#define GA_MASK_CDC_SUB_READ  (0b011 << 8)
#define GA_MASK_CDC_PCM_DMA   (0b100 << 8)
#define GA_MASK_CDC_PRAM_DMA  (0b101 << 8)
#define GA_MASK_CDC_WRAM_DMA  (0b111 << 8)

/**
 * @def GA_REG_CDC_REG
 * @brief CDC Registers #2
 * @ingroup sub__garegs__cdc
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | | | |CD7|CD6|CD5|CD4|CD3|CD2|CD1|CD0|
 * |R| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 * |W| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param CD CDC register data
 * \n Details for this register can be found in the LC89510 manual
 */
#define GA_REG_CDC_REG 0xFF8006

// clang-format off
/**
 * @def GA_REG_CDC_DATA
 * @brief CDC Host Data
 * @ingroup sub__garegs__cdc
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |HD15|HD14|HD13|HD12|HD11|HD10|HD09|HD08|HD07|HD06|HD05|HD04|HD03|HD02|HD01|HD00|
 * |R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|
 *
 * @param HD CDC read data
 * [read] 2 bytes of data read by the CDC and ready to be transferred to
 * Main or Sub CPU memory
 */
// clang-format on
#define GA_REG_CDC_DATA 0xFF8008

/**
 * @def GA_REG_DMA_DEST
 * @brief CDC DMA Destination
 * @ingroup sub__garegs__cdc
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |A18|A17|A16|A15|A14|A13|A12|A11|A10|A09|A08|A07|A06|A05|A04|A03|
 * |R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |W|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param A DMA destination address
 * \n Specifies the address for CDC DMA transfer
 * - For PCM DMA: bits up to A12 are used
 * - For 1M Word RAM: bits up to A16 are used
 * - For 2M Word RAM: bits up to A17 are used
 * - For PRG-RAM: all bits are used
 *
 * Unused bits will be read as 0.
 */
#define GA_REG_DMA_DEST 0xFF800A

/**
 * @def GA_REG_STOPWATCH
 * @brief Stopwatch
 * @ingroup sub__garegs__misc
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | |SW11|SW10|SW09|SW08|SW07|SW06|SW05|SW04|SW03|SW02|SW01|SW00|
 * |R| | | | |◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |W| | | | |◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param SW CDC read data
 * @details R: Read current value / W: Reset the clock (0 only)
 * This is a general use timer, though it is primarily used for CDD/CDC
 * timing. Each tick is 30.72 microseconds.
 */
#define GA_REG_STOPWATCH 0xFF800C

/**
 * @def GA_REG_COMFLAGS
 * @brief Main/Sub CPU communication flags
 * @ingroup sub__garegs__comm
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * |CFM7|CFM6|CFM5|CFM4|CFM3|CFM2|CFM1|CFM0|CFS7|CFS6|CFS5|CFS4|CFS3|CFS2|CFS1|CFS0|
 * |R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * CFM: Comm flags for Main CPU
 * CFS: Comm flags for Sub CPU
 */
#define GA_REG_COMFLAGS 0xFF800E

/**
 * @def GA_REG_COMCMD0
 * @brief Comm Command 0 (Main -> Sub)
 * @ingroup sub__garegs__comm
 *
 * @details R: 16 bit data
 */
#define GA_REG_COMCMD0 0xFF8010

/**
 * @def GA_REG_COMCMD1
 * @brief Comm Command  (Main -> Sub)
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMCMD1 0xFF8012

/**
 * @def GA_REG_COMCMD2
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMCMD2 0xFF8014

/**
 * @def GA_REG_COMCMD3
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMCMD3 0xFF8016

/**
 * @def GA_REG_COMCMD4
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMCMD4 0xFF8018

/**
 * @def GA_REG_COMCMD5
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMCMD5 0xFF801A

/**
 * @def GA_REG_COMCMD6
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMCMD6 0xFF801C

/**
 * @def GA_REG_COMCMD7
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMCMD7 0xFF801E

/**
 * @def GA_REG_COMSTAT0
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMSTAT0 0xFF8020

/**
 * @def GA_REG_COMSTAT1
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMSTAT1 0xFF8022

/**
 * @def GA_REG_COMSTAT2
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMSTAT2 0xFF8024

/**
 * @def GA_REG_COMSTAT3
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMSTAT3 0xFF8026

/**
 * @def GA_REG_COMSTAT4
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMSTAT4 0xFF8028

/**
 * @def GA_REG_COMSTAT5
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMSTAT5 0xFF802A

/**
 * @def GA_REG_COMSTAT6
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMSTAT6 0xFF802C

/**
 * @def GA_REG_COMSTAT7
 * @ingroup sub__garegs__comm
 */
#define GA_REG_COMSTAT7 0xFF802E

/**
 * @def GA_REG_INT3TIMER
 * @brief timer, 30.72us lsb, 0->INT3
 * @ingroup sub__garegs__misc
 * @sa ga_reg_comstat7
 */
#define GA_REG_INT3TIMER 0xFF8030

/**
 * @def GA_REG_INTMASK
 * @brief interrupt control
 * @ingroup sub__garegs__misc
 * @sa ga_reg_intmask
 */
#define GA_REG_INTMASK 0xFF8032

#define BIT_GA_REG_INT1 1
#define BIT_GA_REG_INT2 2
#define BIT_GA_REG_INT3 3
#define BIT_GA_REG_INT4 4
#define BIT_GA_REG_INT5 5
#define BIT_GA_REG_INT6 6

#define MSK_GA_REG_INT1 1 << BIT_GA_REG_INT1
#define MSK_GA_REG_INT2 1 << BIT_GA_REG_INT2
#define MSK_GA_REG_INT3 1 << BIT_GA_REG_INT3
#define MSK_GA_REG_INT4 1 << BIT_GA_REG_INT4
#define MSK_GA_REG_INT5 1 << BIT_GA_REG_INT5
#define MSK_GA_REG_INT6 1 << BIT_GA_REG_INT6

/**
 * @def GA_REG_CDFADER
 * @brief fader control / spindle speed
 * @ingroup sub__garegs__misc
 * @sa ga_reg_cdfader
 */
#define GA_REG_CDFADER 0xFF8034

/**
 * @def GA_REG_CDDCTRL
 * @brief CDD control
 * @ingroup sub__garegs__cdd
 * @sa ga_reg_cddctrl
 */
#define GA_REG_CDDCTRL 0xFF8036

/**
 * @def GA_REG_CDDCOMM0
 * @brief CDD communication
 * @ingroup sub__garegs__cdd
 * @sa ga_reg_cddcomm0
 */
#define GA_REG_CDDCOMM0 0xFF8038

/**
 * @def GA_REG_CDDCOMM1
 * @brief CDD communication
 * @ingroup sub__garegs__cdd
 * @sa ga_reg_cddcomm1
 */
#define GA_REG_CDDCOMM1 0xFF803A

/**
 * @def GA_REG_CDDCOMM2
 * @brief CDD communication
 * @ingroup sub__garegs__cdd
 * @sa ga_reg_cddcomm2
 */
#define GA_REG_CDDCOMM2 0xFF803C

/**
 * @def GA_REG_CDDCOMM3
 * @brief CDD communication
 * @ingroup sub__garegs__cdd
 * @sa ga_reg_cddcomm3
 */
#define GA_REG_CDDCOMM3 0xFF803E

/**
 * @def GA_REG_CDDCOMM4
 * @brief CDD communication
 * @ingroup sub__garegs__cdd
 * @sa ga_reg_cddcomm4
 */
#define GA_REG_CDDCOMM4 0xFF8040

/**
 * @def GA_REG_CDDCOMM5
 * @brief CDD communication
 * @ingroup sub__garegs__cdd
 * @sa ga_reg_cddcomm5
 */
#define GA_REG_CDDCOMM5 0xFF8042

/**
 * @def GA_REG_CDDCOMM6
 * @brief CDD communication
 * @ingroup sub__garegs__cdd
 * @sa ga_reg_cddcomm6
 */
#define GA_REG_CDDCOMM6 0xFF8044

/**
 * @def GA_REG_CDDCOMM7
 * @brief CDD communication
 * @ingroup sub__garegs__cdd
 * @sa ga_reg_cddcomm7
 */
#define GA_REG_CDDCOMM7 0xFF8046

/**
 * @def GA_REG_CDDCOMM8
 * @brief CDD communication
 * @ingroup sub__garegs__cdd
 * @sa ga_reg_cddcomm8
 */
#define GA_REG_CDDCOMM8 0xFF8048

/**
 * @def GA_REG_CDDCOMM9
 * @brief CDD communication
 * @ingroup sub__garegs__cdd
 * @sa ga_reg_cddcomm9
 */
#define GA_REG_CDDCOMM9 0xFF804A

/**
 * @def GA_REG_FONTCOLOR
 * @brief source color values
 * @ingroup sub__garegs__font
 * @sa ga_reg_fontcolor
 */
#define GA_REG_FONTCOLOR 0xFF804C

/**
 * @def GA_REG_FONTBITS
 * @brief font data
 * @ingroup sub__garegs__font
 * @sa ga_reg_fontbits
 */
#define GA_REG_FONTBITS 0xFF804E

/**
 * @def GA_REG_FONTDATA
 * @brief read only
 * @ingroup sub__garegs__font
 * @sa ga_reg_fontdata
 */
#define GA_REG_FONTDATA 0xFF8050

/**
 * @def GA_REG_STAMPSIZE
 * @brief Stamp data sizes
 * @ingroup sub__garegs__gfx
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |GRON| | | | | | | | | | | | |SMS|STS|RPT|
 * |R|◯| | | | | | | | | | | | |◯|◯|◯|
 * |W|🗙| | | | | | | | | | | | |◯|◯|◯|
 *
 * @param RPT Repeat
 * @details RW:
 * @param STS Stamp size
 * @details RW:
 * @param SMS Stamp map size
 * @details RW:
 * @param GRON Graphics operation in progress
 * @details R:
 */
/**
 * @def GA_REG_STAMPSIZE
 * @sa ga_reg_stampsize
 * @ingroup sub__garegs__gfx
 */
#define GA_REG_STAMPSIZE 0xFF8058

#define GA_BIT_STAMPSIZE_REPEAT       0
#define GA_BIT_STAMPSIZE_32x32_STAMP  1
#define GA_BIT_STAMPSIZE_16x16_SCREEN 2

#define GA_MASK_STAMPSIZE_REPEAT       (1 << GA_BIT_STAMPSIZE_REPEAT)
#define GA_MASK_STAMPSIZE_32x32_STAMP  (1 << GA_BIT_STAMPSIZE_32x32_STAMP)
#define GA_MASK_STAMPSIZE_16x16_SCREEN (1 << GA_BIT_STAMPSIZE_16x16_SCREEN)

/**
 * @def GA_REG_STAMPMAPBASE
 * @sa ga_reg_stampmapbase
 * @ingroup sub__garegs__gfx
 * @note The location of the stamp map in Word RAM. The value you put in is the
 * offset relative to the start of Word RAM divided by 4. What the raw location
 * needs to be a multiple of depends on the sizes you have set:

    16x16px stamps, 256x256px stamp map: Multiples of 0x200
    32x32px stamps, 256x256px stamp map: Multiples of 0x80
    16x16px stamps, 4096x4096px stamp map: Multiples of 0x20000
    32x32px stamps, 4096x4096px stamp map: Multiples of 0x8000

 */
#define GA_REG_STAMPMAPBASE 0xFF805A

/**
 * @def GA_REG_IMGBUFVSIZE
 * @sa ga_reg_imgbufvsize
 * @ingroup sub__garegs__gfx
 */
#define GA_REG_IMGBUFVSIZE 0xFF805C

/**
 * @def GA_REG_IMGBUFSTART
 * @sa ga_reg_imgbufstart
 * @ingroup sub__garegs__gfx
 */
#define GA_REG_IMGBUFSTART 0xFF805E

/**
 * @def GA_REG_IMGBUFOFFSET
 * @sa ga_reg_imgbufoffset
 * @ingroup sub__garegs__gfx
 */
#define GA_REG_IMGBUFOFFSET 0xFF8060

/**
 * @def GA_REG_IMGBUFHDOTSIZE
 * @sa ga_reg_imgbufhdotsize
 * @ingroup sub__garegs__gfx
 */
#define GA_REG_IMGBUFHDOTSIZE 0xFF8062

/**
 * @def GA_REG_IMGBUFVDOTSIZE
 * @sa ga_reg_imgbufvdotsize
 * @ingroup sub__garegs__gfx
 */
#define GA_REG_IMGBUFVDOTSIZE 0xFF8064

/**
 * @def GA_REG_TRACEVECTBASE
 * @sa ga_reg_tracevectbase
 * @ingroup sub__garegs__gfx
 */
#define GA_REG_TRACEVECTBASE 0xFF8066

/**
 * @def GA_REG_SUBCODEADDR
 * @sa ga_reg_subcodeaddr
 * @ingroup sub__garegs__subcode
 */
#define GA_REG_SUBCODEADDR 0xFF8068

/**
 * @def GA_REG_SUBCODEBUF
 * @sa ga_reg_subcodebuf
 * @ingroup sub__garegs__subcode
 */
#define GA_REG_SUBCODEBUF 0xFF8100

/**
 * @def GA_REG_SUBCODEBUFIMG
 * @sa ga_reg_subcodebufimg
 * @ingroup sub__garegs__subcode
 */
#define GA_REG_SUBCODEBUFIMG 0xFF8180

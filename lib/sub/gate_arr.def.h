/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.def.h
 * @brief Sub CPU side Gate Array registers
 */

#ifndef MEGADEV__SUB_GATEARR_DEF_H
#define MEGADEV__SUB_GATEARR_DEF_H

/**
 * @defgroup ga_regs_sub Sub CPU / Gate Array / Registers
 */

/**
 * @defgroup ga_reg_sub_subctrl Sub CPU / Gate Array / Registers / Sub CPU &
 * Hardware Control
 */

/**
 * @def GA_REG_RESET
 * @brief Sub CPU & Hardware Control
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_subctrl
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||LEDG|LEDR|Ver3|Ver2|Ver1|Ver0| |||RES0|
 *
 * @param RES0 Sub CPU reset / Version / LED Control
 * \n [write] 0: Reset / 1: No effect
 * \n [read] 0: Reset in progress / 1: Reset
 * possible
 * @param LEDR Red LED control
 * \n 0: off
 * \n 1: on
 * @param LEDG Green LED control
 * \n 0: off
 * \n 1: On
 * @param Ver ROM Version
 * [read] ROM Version
 */
#define GA_REG_RESET 0xFF8000

/**
 * @def GA_LED_R
 * @brief Red LED Control
 * @sa ga_reg_reset
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_subctrl
 */
#define GA_LED_R (1 << 0)

/**
 * @def GA_LED_G
 * @brief Green LED Control
 * @sa ga_reg_reset
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_subctrl
 */
#define GA_LED_G (1 << 1)

/**
 * @def GA_MASK_VERSION
 * @brief ROM Version
 * @sa ga_reg_reset
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_subctrl
 */
#define GA_MASK_VERSION (0b1111 << 4)

/**
 * @defgroup ga_reg_sub_memmode Sub CPU / Gate Array / Registers / Memory Mode
 */

/**
 * @def GA_REG_MEMMODE
 * @brief GA Reg 01 - Word RAM Memory Mode / RAM Write Protect / Priority Mode
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_memmode
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |WP7|WP6|WP5|WP4|WP3|WP2|WP1|WP0| |||PM1|PM0|MODE|DMNA|RET|
 *
 * @param WP Write protect Sub CPU RAM
 * @param PM Priority Mode
 * @param MODE Word RAM layout
 * \n 0: 2M
 * \n 1: 1M
 * @param DMNA Main CPU will not access Word RAM
 * @param RET In 2M mode: Give Word RAM control to Main CPU;
 * In 1M mode: Change 1M block ownership
 *
 */
#define GA_REG_MEMMODE 0xFF8002

#define BIT_GA_REG_RET  0
#define BIT_GA_REG_DMNA 1
#define BIT_GA_REG_MODE 2

/**
 * @def GA_MASK_RETURN_2M
 * @brief GA Reg 01 - Change Word RAM ownership bitmask
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_memmode
 */
#define GA_MASK_RETURN_2M (1 << BIT_GA_REG_RET)

/**
 * @def MSK_GA_REG_DMNA
 * @brief GA Reg 01 - Main CPU no Word RAM access flag bitmask
 * @sa ga_reg_memmode
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_memmode
 */
#define MSK_GA_REG_DMNA (1 << BIT_GA_REG_DMNA)

/**
 * @def GA_MASK_WORDRAM_LAYOUT
 * @brief GA Reg 01 - Word RAM layout bitmask
 * @sa ga_reg_memmode
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_memmode
 */
#define GA_MASK_WORDRAM_LAYOUT (1 << BIT_GA_REG_MODE)

/**
 * @defgroup ga_reg_sub_cdcmode Sub CPU / Gate Array / Registers / CDC Mode &
 * CDC Registers #1
 */

/**
 * @def GA_REG_CDCMODE
 * @brief CDC Mode & CDC Registers #1
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cdcmode
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |EDT|DSR| |||DD2|DD1|DD0| ||||CA3|CA2|CA1|CA0|
 *
 * @param CA CDC register address
 * @details RW: Details for this register can be found in the LC89510 manual
 * @param DSR Data set ready
 * @details R: Data from the CDC is present in the CDC Host Data register
 * @param EDT End of data transfer
 * @details R: All data from the CDC has been transferred
 * @param DD Device destination
 * @details RW: Specifies the destination for CDC data transfer, as defined
 * below: |DD2|DD1|DD0|Destination|
 *   |:|:|:|:|
 *   |0|1|0|Main CPU|
 *   |0|1|1|Sub CPU|
 *   |1|0|0|PCM DMA|
 *   |1|1|1|In 2M Mode: Word RAM \n In 1M Mode: Sub CPU controlled Word RAM|
 *
 *   All other values for DD are invalid.
 */
#define GA_REG_CDCMODE 0xFF8004

/**
 * @defgroup ga_reg_sub_cdcregs2 Sub CPU / Gate Array / Registers / CDC
 * Registers #1
 */

/**
 * @def GA_REG_CDCRS1
 * @brief CDC Registers #2
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cdcregs2
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||||CD7|CD6|CD5|CD4|CD3|CD2|CD1|CD0|
 *
 * @param CD CDC register data
 * \n Details for this register can be found in the LC89510 manual
 */
#define GA_REG_CDCRS1 0xFF8006

/**
 * @defgroup ga_reg_sub_cdcdata Sub CPU / Gate Array / Registers / CDC Host Data
 */

/**
 * @def GA_REG_CDCHOSTDATA
 * @brief CDC Host Data
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cdcdata
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HD15|HD14|HD13|HD12|HD11|HD10|HD09|HD08|HD07|HD06|HD05|HD04|HD03|HD02|HD01|HD00|
 *
 * @param HD CDC read data
 * [read] 2 bytes of data read by the CDC and ready to be transferred to
 * Main or Sub CPU memory
 */
#define GA_REG_CDCHOSTDATA 0xFF8008

/**
 * @defgroup ga_reg_sub_dmaaddr Sub CPU / Gate Array / Registers / CDC DMA
 * Address
 */

/**
 * @def GA_REG_DMAADDR
 * @brief CDC DMA Address
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_dmaaddr
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |A18|A17|A16|A15|A14|A13|A12|A11|A10|A09|A08|A07|A06|A05|A04|A03|
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
#define GA_REG_DMAADDR 0xFF800A

/**
 * @defgroup ga_reg_sub_stopwatch Sub CPU / Gate Array / Registers / Stopwatch
 */

/**
 * @def GA_REG_STOPWATCH
 * @brief Stopwatch
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_stopwatch
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||SW11|SW10|SW09|SW08|SW07|SW06|SW05|SW04|SW03|SW02|SW01|SW00|
 *
 * @param SW CDC read data
 * @details R: Read current value / W: Reset the clock (0 only)
 * This is a general use timer, though it is primarily used for CDD/CDC
 * timing. Each tick is 30.72 microseconds.
 */
#define GA_REG_STOPWATCH 0xFF800C

/**
 * @defgroup ga_reg_sub_cpucomm Sub CPU / Gate Array / Registers / CPU
 * Communication
 */

/**
 * @def GA_REG_COMFLAGS
 * @brief Main/Sub CPU communication flags
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |CFM7|CFM6|CFM5|CFM4|CFM3|CFM2|CFM1|CFM0|CFS7|CFS6|CFS5|CFS4|CFS3|CFS2|CFS1|CFS0|
 *
 * CFM: Comm flags for Main CPU
 * CFS: Comm flags for Sub CPU
 */
#define GA_REG_COMFLAGS 0xFF800E

/**
 * @def GA_REG_COMCMD0
 * @brief Comm Command 0 (Main -> Sub)
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 *
 * @details R: 16 bit data
 */
#define GA_REG_COMCMD0 0xFF8010

/**
 * @def GA_REG_COMCMD1
 * @brief Comm Command  (Main -> Sub)
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMCMD1 0xFF8012

/**
 * @def GA_REG_COMCMD2
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMCMD2 0xFF8014

/**
 * @def GA_REG_COMCMD3
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMCMD3 0xFF8016

/**
 * @def GA_REG_COMCMD4
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMCMD4 0xFF8018

/**
 * @def GA_REG_COMCMD5
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMCMD5 0xFF801A

/**
 * @def GA_REG_COMCMD6
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMCMD6 0xFF801C

/**
 * @def GA_REG_COMCMD7
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMCMD7 0xFF801E

/**
 * @def GA_REG_COMSTAT0
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMSTAT0 0xFF8020

/**
 * @def GA_REG_COMSTAT1
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMSTAT1 0xFF8022

/**
 * @def GA_REG_COMSTAT2
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMSTAT2 0xFF8024

/**
 * @def GA_REG_COMSTAT3
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMSTAT3 0xFF8026

/**
 * @def GA_REG_COMSTAT4
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMSTAT4 0xFF8028

/**
 * @def GA_REG_COMSTAT5
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMSTAT5 0xFF802A

/**
 * @def GA_REG_COMSTAT6
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMSTAT6 0xFF802C

/**
 * @def GA_REG_COMSTAT7
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_cpucomm
 */
#define GA_REG_COMSTAT7 0xFF802E

/**
 * @def GA_REG_INT3TIMER
 * @sa ga_reg_comstat7
 * @ingroup ga_regs_sub
 */
#define GA_REG_INT3TIMER 0xFF8030

/**
 * @def GA_REG_INTMASK
 * @sa ga_reg_intmask
 * @ingroup ga_regs_sub
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
 * @sa ga_reg_cdfader
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDFADER 0xFF8034

/**
 * @def GA_REG_CDDCTRL
 * @sa ga_reg_cddctrl
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDDCTRL 0xFF8036

/**
 * @def GA_REG_CDDCOMM0
 * @sa ga_reg_cddcomm0
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDDCOMM0 0xFF8038

/**
 * @def GA_REG_CDDCOMM1
 * @sa ga_reg_cddcomm1
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDDCOMM1 0xFF803A

/**
 * @def GA_REG_CDDCOMM2
 * @sa ga_reg_cddcomm2
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDDCOMM2 0xFF803C

/**
 * @def GA_REG_CDDCOMM3
 * @sa ga_reg_cddcomm2
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDDCOMM3 0xFF803E

/**
 * @def GA_REG_CDDCOMM4
 * @sa ga_reg_cddcomm4
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDDCOMM4 0xFF8040

/**
 * @def GA_REG_CDDCOMM5
 * @sa ga_reg_cddcomm5
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDDCOMM5 0xFF8042

/**
 * @def GA_REG_CDDCOMM6
 * @sa ga_reg_cddcomm6
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDDCOMM6 0xFF8044

/**
 * @def GA_REG_CDDCOMM7
 * @sa ga_reg_cddcomm7
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDDCOMM7 0xFF8046

/**
 * @def GA_REG_CDDCOMM8
 * @sa ga_reg_cddcomm8
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDDCOMM8 0xFF8048

/**
 * @def GA_REG_CDDCOMM9
 * @sa ga_reg_cddcomm9
 * @ingroup ga_regs_sub
 */
#define GA_REG_CDDCOMM9 0xFF804A

/**
 * @def GA_REG_FONTCOLOR
 * @sa ga_reg_fontcolor
 * @ingroup ga_regs_sub
 */
#define GA_REG_FONTCOLOR 0xFF804C

/**
 * @def GA_REG_FONTBITS
 * @sa ga_reg_fontbits
 * @ingroup ga_regs_sub
 */
#define GA_REG_FONTBITS 0xFF804E

/**
 * @def GA_REG_FONTDATA
 * @sa ga_reg_fontdata
 * @ingroup ga_regs_sub
 */
#define GA_REG_FONTDATA 0xFF8050

/**
 * @def GA_REG_STAMPSIZE
 * @brief Stamp data sizes
 * @ingroup ga_regs_sub
 * @ingroup gfx_xform
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |GRON|||||||||||||SMS|STS|RPT|
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
 * @ingroup ga_regs_sub
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
 * @ingroup ga_regs_sub
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
 * @ingroup ga_regs_sub
 */
#define GA_REG_IMGBUFVSIZE 0xFF805C

/**
 * @def GA_REG_IMGBUFSTART
 * @sa ga_reg_imgbufstart
 * @ingroup ga_regs_sub
 */
#define GA_REG_IMGBUFSTART 0xFF805E

/**
 * @def GA_REG_IMGBUFOFFSET
 * @sa ga_reg_imgbufoffset
 * @ingroup ga_regs_sub
 */
#define GA_REG_IMGBUFOFFSET 0xFF8060

/**
 * @def GA_REG_IMGBUFHDOTSIZE
 * @sa ga_reg_imgbufhdotsize
 * @ingroup ga_regs_sub
 */
#define GA_REG_IMGBUFHDOTSIZE 0xFF8062

/**
 * @def GA_REG_IMGBUFVDOTSIZE
 * @sa ga_reg_imgbufvdotsize
 * @ingroup ga_regs_sub
 */
#define GA_REG_IMGBUFVDOTSIZE 0xFF8064

/**
 * @def GA_REG_TRACEVECTBASE
 * @sa ga_reg_tracevectbase
 * @ingroup ga_regs_sub
 */
#define GA_REG_TRACEVECTBASE 0xFF8066

/**
 * @def GA_REG_SUBCODEADDR
 * @sa ga_reg_subcodeaddr
 * @ingroup ga_regs_sub
 */
#define GA_REG_SUBCODEADDR 0xFF8068

/**
 * @def GA_REG_SUBCODEBUF
 * @sa ga_reg_subcodebuf
 * @ingroup ga_regs_sub
 */
#define GA_REG_SUBCODEBUF 0xFF8100

/**
 * @def GA_REG_SUBCODEBUFIMG
 * @sa ga_reg_subcodebufimg
 * @ingroup ga_regs_sub
 */
#define GA_REG_SUBCODEBUFIMG 0xFF8180

/**
 * ga_reg_cdcmode bit/mask settings
 */
#define GA_BIT_CDCMODE_DD0 5
#define GA_BIT_CDCMODE_DSR 6
#define GA_BIT_CDCMODE_EDT 7

// these aren't right...
// TODO clean these up
#define GA_BIT_CDC_MAINREAD 2
#define GA_BIT_CDC_SUBREAD  3
#define GA_BIT_CDC_PCMDMA   4
#define GA_BIT_CDC_PRAMDMA  5
#define GA_BIT_CDC_WRAMDMA  7

#define CDCMODE_CABITS           0x000F
#define CDCMODE_DDBITS           0x0700
#define GA_MASK_CDCMODE_DD0      1 << GA_BIT_CDCMODE_DD0
#define GA_MASK_CDCMODE_DSR      1 << GA_BIT_CDCMODE_DSR
#define GA_MASK_CDCMODE_EDT      1 << GA_BIT_CDCMODE_EDT
#define GA_MASK_CDCMODE_MAINREAD 1 << GA_BIT_CDC_MAINREAD
#define GA_MASK_CDCMODE_SUBREAD  1 << GA_BIT_CDC_SUBREAD
#define GA_MASK_CDCMODE_PCMDMA   1 << GA_BIT_CDC_PCMDMA
#define GA_MASK_CDCMODE_PRAMDMA  1 << GA_BIT_CDC_PRAMDMA
#define GA_MASK_CDCMODE_WRAMDMA  1 << GA_BIT_CDC_WRAMDMA

#define CDC_DEST_MAINREAD 2
#define CDC_DEST_SUBREAD  3
#define CDC_DEST_PCMDMA   4
#define CDC_DEST_PRAMDMA  5
#define CDC_DEST_WRAMDMA  7

#endif

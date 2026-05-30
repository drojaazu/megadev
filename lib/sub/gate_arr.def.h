/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.def.h
 * @brief Sub CPU side Gate Array registers
 */

#pragma once

// clang-format off
/**
 * @defgroup sub_garegs Gate Array Registers
 * @ingroup sub
 */

/**
 * @defgroup sub_garegs_sysctrl CPU and RAM Control
 * @ingroup sub_garegs
 */

/**
 * @defgroup sub_garegs_cdc CDC Operation
 * @ingroup sub_garegs
 */

/**
 * @defgroup sub_garegs_cdd CDD Operation
 * @ingroup sub_garegs
 */

/**
 * @defgroup sub_garegs_led LED Operation
 * @ingroup sub_garegs
 */

 /**
 * @defgroup sub_garegs_comm CPU Communication
 * @ingroup sub_garegs
 */

 /**
 * @defgroup sub_garegs_font Font Generator
 * @ingroup sub_garegs
 */

 /**
 * @defgroup sub_garegs_gfx Graphics Transforms
 * @ingroup sub_garegs
 */

  /**
 * @defgroup sub_garegs_subcode Disc Subcode Data
 * @ingroup sub_garegs
 */

 /**
 * @defgroup sub_garegs_misc Misc
 * @ingroup sub_garegs
 */
// clang-format on

#pragma region GA_REG_RESET

/**
 * @defgroup sub_garegs_reg01 Reg. #1 - CPU Reset / LED Control / ROM Version
 * @ingroup sub_garegs_sysctrl
 * @ingroup sub_garegs_led
 * @ingroup sub_garegs_misc
 */

/**
 * @def GA_REG_RESET
 * @brief CPU Reset / LED Control / ROM Version
 * @ingroup sub_garegs_reg01
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | |\b LEDG|\b LEDR|\b VER|||| | | |\b RES|
 * |\b R| | | | | | | |◯| | | | | |◯|◯|◯|
 * |\b W| | | | | | | | | | | | | |◯|◯|◯|
 *
 * @param[r] RES CPU Reset
 * \n Reset time is 100ms.
 * - 0: Reset in progress
 * - 1: Reset complete / Reset possible
 * @param[w] RES CPU Reset
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

/**
 * @def GA_REG_RESET_HI
 * @ingroup sub_garegs_reg01
 */
#define GA_REG_RESET_HI GA_REG_RESET

/**
 * @def GA_REG_RESET_LO
 * @ingroup sub_garegs_reg01
 */
#define GA_REG_RESET_LO (GA_REG_RESET + 1)

/**
 * @def GA_BIT_LED_R
 * @ingroup sub_garegs_reg01
 */
#define GA_BIT_LED_R 0

/**
 * @def GA_BIT_LED_G
 * @ingroup sub_garegs_reg01
 */
#define GA_BIT_LED_G 1

/**
 * @def GA_MASK_LED_R
 * @brief Red LED Control
 * @sa ga_reg_reset
 * @ingroup sub_garegs_reg01
 */
#define GA_MASK_LED_R (1 << GA_BIT_LED_R)

/**
 * @def GA_MASK_LED_G
 * @brief Green LED Control
 * @sa ga_reg_reset
 * @ingroup sub_garegs_reg01
 */
#define GA_MASK_LED_G (1 << GA_BIT_LED_G)

/**
 * @def GA_MASK_VERSION
 * @brief ROM Version
 * @sa ga_reg_reset
 * @ingroup sub_garegs_reg01
 */
#define GA_MASK_VERSION (0b1111 << 4)

#pragma endregion

#pragma region GA_REG_MEMMODE

/**
 * @defgroup sub_garegs_reg02 Reg. #2 - Word RAM Layout / PRGRAM Write Protect
 * @ingroup sub_garegs_sysctrl
 */

/**
 * @def GA_REG_MEMMODE
 * @brief Word RAM Control / PRG RAM Write Protect Status
 * @ingroup sub_garegs_reg02
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b WP|||||||| | | |\b PM||\b MODE|\b DMNA|\b RET|
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯| | | |◯|◯|◯|◯|◯|
 * |\b W| | | | | | | | | | | |◯|◯|◯| |◯|
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

/**
 * @def GA_BIT_RETURN_2M
 * @ingroup sub_garegs_reg02
 */
#define GA_BIT_RETURN_2M 0
/**
 * @def GA_BIT_DMNA
 * @ingroup sub_garegs_reg02
 */
#define GA_BIT_DMNA 1
/**
 * @def GA_BIT_WORDRAM_LAYOUT
 * @ingroup sub_garegs_reg02
 */
#define GA_BIT_WORDRAM_LAYOUT 2

/**
 * @def GA_MASK_RETURN_2M
 * @brief Change Word RAM ownership bitmask
 * @ingroup sub_garegs_reg02
 */
#define GA_MASK_RETURN_2M (1 << GA_BIT_RETURN_2M)

/**
 * @def GA_MASK_DMNA
 * @brief Main CPU no Word RAM access flag bitmask
 * @sa sub_garegs_sysctrl
 * @ingroup sub_garegs_reg02
 */
#define GA_MASK_DMNA (1 << GA_BIT_DMNA)

/**
 * @def GA_MASK_WORDRAM_LAYOUT
 * @brief Word RAM layout bitmask
 * @sa sub_garegs_sysctrl
 * @ingroup sub_garegs_reg02
 */
#define GA_MASK_WORDRAM_LAYOUT (1 << GA_BIT_WORDRAM_LAYOUT)

#pragma endregion

#pragma region GA_REG_CDC_MODE

/**
 * @defgroup sub_garegs_reg03 Reg. #3 - CDC Destination and Registers #1
 * @ingroup sub_garegs_cdc
 */

/**
 * @def GA_REG_CDC_MODE
 * @brief CDC Destination and Registers #1
 * @ingroup sub_garegs_reg03
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b EDT|\b DSR|\b UBR| | |\b DD||| | | | |\b CA||||
 * |\b R|◯︎|◯|◯| | |◯|◯|◯| | | | |◯|◯|◯|◯|
 * |\b W| | | | | |◯|◯|◯| | | | |◯|◯|◯|◯|
 *
 * @param CA CDC register address [rw]
 *    \n Details for this register can be found in the LC89510 manual
 *
 * @param DSR Data set ready [ro]
 *    \n Data from the CDC is present in the CDC Host Data register
 *
 * @param UBR Upper byte ready [ro]
 *    \n Indicates the upper byte has been sent from the CDC. Cleared when
 the lower byte is sent.
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
 *
 */
#define GA_REG_CDC_MODE 0xFF8004

/**
 * @def GA_REG_CDC_MODE_HI
 * @ingroup sub_garegs_reg03
 */
#define GA_REG_CDC_MODE_HI GA_REG_CDC_MODE

/**
 * @def GA_REG_CDC_MODE_LO
 * @ingroup sub_garegs_reg03
 */
#define GA_REG_CDC_MODE_LO (GA_REG_CDC_MODE + 1)

/**
 * @def GA_BIT_CDC_DATA_READY
 * @ingroup sub_garegs_reg03
 */
#define GA_BIT_CDC_DATA_READY 6

/**
 * @def GA_BIT_EDT
 * @ingroup sub_garegs_reg03
 */
#define GA_BIT_EDT 7

/**
 * @def GA_MASK_CDC_REGS1
 * @ingroup sub_garegs_reg03
 */
#define GA_MASK_CDC_REGS1 0b1111

/**
 * @def GA_MASK_CDC_DEST
 * @ingroup sub_garegs_reg03
 */
#define GA_MASK_CDC_DEST 0b111

/**
 * @def GA_MASK_CDC_MAIN_READ
 * @ingroup sub_garegs_reg03
 */
#define GA_MASK_CDC_MAIN_READ 0b010

/**
 * @def GA_MASK_CDC_SUB_READ
 * @ingroup sub_garegs_reg03
 */
#define GA_MASK_CDC_SUB_READ 0b011

/**
 * @def GA_MASK_CDC_PCM_DMA
 * @ingroup sub_garegs_reg03
 */
#define GA_MASK_CDC_PCM_DMA 0b100

/**
 * @def GA_MASK_CDC_PRAM_DMA
 * @ingroup sub_garegs_reg03
 */
#define GA_MASK_CDC_PRAM_DMA 0b101

/**
 * @def GA_MASK_CDC_WRAM_DMA
 * @ingroup sub_garegs_reg03
 */
#define GA_MASK_CDC_WRAM_DMA 0b111

#pragma endregion

#pragma region GA_REG_CDC_REG

/**
 * @defgroup sub_garegs_reg04 Reg. #4 - CDC Registers #2
 * @ingroup sub_garegs_cdc
 */

/**
 * @def GA_REG_CDC_REG
 * @brief CDC Registers #2
 * @ingroup sub_garegs_reg04
 *
 * @warning No bit level opcodes
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 s* | | | | | | | | | |CD||||||||
 * |R| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 * |W| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param CD CDC register data
 * \n Details for this register can be found in the LC89510 manual
 */
#define GA_REG_CDC_REG 0xFF8006

/**
 * @def GA_REG_CDC_REG_HI
 * @ingroup sub_garegs_reg04
 */
#define GA_REG_CDC_REG_HI GA_REG_CDC_REG

/**
 * @def GA_REG_CDC_REG_LO
 * @ingroup sub_garegs_reg04
 */
#define GA_REG_CDC_REG_LO (GA_REG_CDC_REG + 1)

#pragma endregion

#pragma region GA_REG_CDC_DATA
// clang-format off

/**
 * @defgroup sub_garegs_reg05 Reg. #5 - CDC Host Data
 * @ingroup sub_garegs_cdc
 */

/**
 * @def GA_REG_CDC_DATA
 * @brief CDC Host Data
 * @ingroup sub_garegs_reg05
 * 
 * @warning Word access only; Read only; No bit level opcodes
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b HD||||||||||||||||
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|
 *
 * @param HD CDC read data
 * [read] 2 bytes of data read by the CDC and ready to be transferred to
 * Main or Sub CPU memory
 */
// clang-format on
#define GA_REG_CDC_DATA 0xFF8008

#pragma endregion

#pragma region GA_REG_DMA_DEST

/**
 * @defgroup sub_garegs_reg06 Reg. #6 - CDC DMA Destination
 * @ingroup sub_garegs_cdc
 */

/**
 * @def GA_REG_DMA_DEST
 * @brief CDC DMA Destination
 * @ingroup sub_garegs_reg06
 *
 * @warning Word access only; Write only; No bit level opcodes
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |A18|A17|A16|A15|A14|A13|A12|A11|A10|A09|A08|A07|A06|A05|A04|A03|
 * |\b R|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|
 * |\b W|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @note Word Access
 *
 * @attention Write Only
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

#pragma endregion

#pragma region GA_REG_STOPWATCH

/**
 * @defgroup sub_garegs_reg07 Reg. #7 - Stopwatch
 * @ingroup sub_garegs_misc
 */

/**
 * @def GA_REG_STOPWATCH
 * @brief Stopwatch
 * @ingroup sub_garegs_reg07
 *
 * @warning Word access only; No bit level opcodes
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | |\b SW||||||||||||
 * |\b R| | | | |◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | |◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @note Word Access
 *
 * @param SW CDC read data
 * @details R: Read current value / W: Reset the clock (0 only)
 * This is a general use timer, though it is primarily used for CDD/CDC
 * timing. Each tick is 30.72 microseconds.
 */
#define GA_REG_STOPWATCH 0xFF800C

#define GA_MASK_STOPWATCH 0b111111111111

#pragma endregion

#pragma region GA_REG_COMFLAGS

/**
 * @defgroup sub_garegs_reg08 Reg. #8 - Communication Flags
 * @ingroup sub_garegs_comm
 */

/**
 * @def GA_REG_COMFLAGS
 * @brief Main/Sub CPU communication flags
 * @ingroup sub_garegs_reg08
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |CFM||||||||CFS||||||||
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * CFM: Comm flags for Main CPU
 * CFS: Comm flags for Sub CPU
 */
#define GA_REG_COMFLAGS 0xFF800E

#pragma endregion

#pragma region GA_REG_COMCMD

/**
 * @defgroup sub_garegs_reg09 Reg. #8-15 - Comm. Commands
 * @ingroup sub_garegs_comm
 */

/**
 * @def GA_REG_COMCMD0
 * @brief Comm Command 0 (Main -> Sub)
 * @ingroup sub_garegs_reg09
 *
 * @details R: 16 bit data
 */
#define GA_REG_COMCMD0 0xFF8010

/**
 * @def GA_REG_COMCMD1
 * @brief Comm Command  (Main -> Sub)
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMCMD1 0xFF8012

/**
 * @def GA_REG_COMCMD2
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMCMD2 0xFF8014

/**
 * @def GA_REG_COMCMD3
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMCMD3 0xFF8016

/**
 * @def GA_REG_COMCMD4
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMCMD4 0xFF8018

/**
 * @def GA_REG_COMCMD5
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMCMD5 0xFF801A

/**
 * @def GA_REG_COMCMD6
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMCMD6 0xFF801C

/**
 * @def GA_REG_COMCMD7
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMCMD7 0xFF801E

#pragma endregion

#pragma region GA_REG_COMSTAT

/**
 * @defgroup sub_garegs_reg16 Reg. #16-23 - Comm. Status
 * @ingroup sub_garegs_comm
 */

/**
 * @def GA_REG_COMSTAT0
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMSTAT0 0xFF8020

/**
 * @def GA_REG_COMSTAT1
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMSTAT1 0xFF8022

/**
 * @def GA_REG_COMSTAT2
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMSTAT2 0xFF8024

/**
 * @def GA_REG_COMSTAT3
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMSTAT3 0xFF8026

/**
 * @def GA_REG_COMSTAT4
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMSTAT4 0xFF8028

/**
 * @def GA_REG_COMSTAT5
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMSTAT5 0xFF802A

/**
 * @def GA_REG_COMSTAT6
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMSTAT6 0xFF802C

/**
 * @def GA_REG_COMSTAT7
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMSTAT7 0xFF802E

#pragma endregion

#pragma region GA_REG_INT3TIMER

/**
 * @defgroup sub_garegs_reg24 Reg. #24 - Timer
 * @ingroup sub_garegs_misc
 */

/**
 * @def GA_REG_INT3TIMER
 * @brief timer, 30.72us lsb, 0->INT3
 * @ingroup sub_garegs_reg24
 * @sa ga_reg_comstat7
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | |\b LEDG|\b LEDR|\b VER|||| | | |\b RES|
 * |\b R| | | | | | |◯|◯|◯|◯|◯|◯| | | |◯|
 * |\b W| | | | | | |◯|◯| | | | | | | |◯|
 */
#define GA_REG_INT3TIMER 0xFF8030

#pragma endregion

#pragma region GA_REG_INTMASK

/**
 * @defgroup sub_garegs_reg25 Reg. #25 - Interrupt Mask
 * @ingroup sub_garegs_sysctrl
 */

/**
 * @def GA_REG_INTMASK
 * @brief interrupt control
 * @ingroup sub_garegs_reg25
 * @sa ga_reg_intmask
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | |\b LEDG|\b LEDR|\b VER|||| | | |\b RES|
 * |\b R| | | | | | |◯|◯|◯|◯|◯|◯| | | |◯|
 * |\b W| | | | | | |◯|◯| | | | | | | |◯|
 */
#define GA_REG_INTMASK 0xFF8032

/**
 * @ingroup sub_garegs_reg25
 */
#define BIT_GA_REG_INT1 1

/**
 * @ingroup sub_garegs_reg25
 */
#define BIT_GA_REG_INT2 2

/**
 * @ingroup sub_garegs_reg25
 */
#define BIT_GA_REG_INT3 3

/**
 * @ingroup sub_garegs_reg25
 */
#define BIT_GA_REG_INT4 4

/**
 * @ingroup sub_garegs_reg25
 */
#define BIT_GA_REG_INT5 5

/**
 * @ingroup sub_garegs_reg25
 */
#define BIT_GA_REG_INT6 6

/**
 * @ingroup sub_garegs_reg25
 */
#define MSK_GA_REG_INT1 (1 << BIT_GA_REG_INT1)

/**
 * @ingroup sub_garegs_reg25
 */
#define MSK_GA_REG_INT2 (1 << BIT_GA_REG_INT2)

/**
 * @ingroup sub_garegs_reg25
 */
#define MSK_GA_REG_INT3 (1 << BIT_GA_REG_INT3)

/**
 * @ingroup sub_garegs_reg25
 */
#define MSK_GA_REG_INT4 (1 << BIT_GA_REG_INT4)

/**
 * @ingroup sub_garegs_reg25
 */
#define MSK_GA_REG_INT5 (1 << BIT_GA_REG_INT5)

/**
 * @ingroup sub_garegs_reg25
 */
#define MSK_GA_REG_INT6 (1 << BIT_GA_REG_INT6)

#pragma endregion

#pragma region GA_REG_CDFADER

/**
 * @defgroup sub_garegs_reg26 Reg. #26 - CD Fader / Spindle Control
 * @ingroup sub_garegs_misc
 */

/**
 * @def GA_REG_CDFADER
 * @brief fader control / spindle speed
 * @ingroup sub_garegs_reg26
 * @sa ga_reg_cdfader
 *
 * @warning Word access only; No bit level opcodes
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b EFDT|\b FD|||||||||||\b DEF||\b SSF| |
 * |\b R|◯| | | | | | | | | | | | | | | |
 * |\b W| |◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯| |
 *
 * @param FD Fader volume
 * \n Fader decay depth
 *
 * @param EFDT End of Fade Data Transfer
 *
 * @param DEF De-Emphasis Flag
 *
 * @param SSF Spindle Speed
 */
#define GA_REG_CDFADER 0xFF8034

/**
 * @def GA_MASK_FADER_VOLUME
 * @ingroup sub_garegs_reg26
 */
#define GA_MASK_FADER_VOLUME (0b1111111111 << 4)

/**
 * @def GA_MASK_DEMPHASIS
 * @ingroup sub_garegs_reg26
 */
#define GA_MASK_DEMPHASIS (0b11 << 2)

/**
 * @def GA_MASK_SPINDLE_SPEED
 * @ingroup sub_garegs_reg26
 */
#define GA_MASK_SPINDLE_SPEED (1 << 1)

#pragma endregion

#pragma region GA_REG_CDDCTRL

/**
 * @defgroup sub_garegs_reg27 Reg. #27 - CDD Control
 * @ingroup sub_garegs_cdd
 */

/**
 * @def GA_REG_CDDCTRL
 * @brief CDD control
 * @ingroup sub_garegs_reg27
 * @sa ga_reg_cddctrl
 *
 * @warning No bit level opcodes
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | ||\b DM| | | | | |\b HOCK|\b DRS|\b DTS|
 * |\b R| | | | | | |◯|◯|◯|◯|◯|◯| | | |◯|
 * |\b W| | | | | | |◯|◯| | | | | | | |◯|
 */
#define GA_REG_CDDCTRL 0xFF8036

#pragma endregion

#pragma region GA_REG_CDDCOMM

/**
 * @defgroup sub_garegs_reg28 Reg. #28-37 - CDD Communication
 * @ingroup sub_garegs_cdd
 */

/**
 * @def GA_REG_CDDCOMM0
 * @brief CDD communication
 * @ingroup sub_garegs_reg28
 * @sa ga_reg_cddcomm0
 *
 * @warning No bit level opcodes
 */
#define GA_REG_CDDCOMM0 0xFF8038

/**
 * @def GA_REG_CDDCOMM1
 * @brief CDD communication
 * @ingroup sub_garegs_reg28
 * @sa ga_reg_cddcomm1
 *
 * @warning No bit level opcodes
 */
#define GA_REG_CDDCOMM1 0xFF803A

/**
 * @def GA_REG_CDDCOMM2
 * @brief CDD communication
 * @ingroup sub_garegs_reg28
 * @sa ga_reg_cddcomm2
 *
 * @warning No bit level opcodes
 */
#define GA_REG_CDDCOMM2 0xFF803C

/**
 * @def GA_REG_CDDCOMM3
 * @brief CDD communication
 * @ingroup sub_garegs_reg28
 * @sa ga_reg_cddcomm3
 *
 * @warning No bit level opcodes
 */
#define GA_REG_CDDCOMM3 0xFF803E

/**
 * @def GA_REG_CDDCOMM4
 * @brief CDD communication
 * @ingroup sub_garegs_reg28
 * @sa ga_reg_cddcomm4
 *
 * @warning No bit level opcodes
 */
#define GA_REG_CDDCOMM4 0xFF8040

/**
 * @def GA_REG_CDDCOMM5
 * @brief CDD communication
 * @ingroup sub_garegs_reg28
 * @sa ga_reg_cddcomm5
 *
 * @warning No bit level opcodes
 */
#define GA_REG_CDDCOMM5 0xFF8042

/**
 * @def GA_REG_CDDCOMM6
 * @brief CDD communication
 * @ingroup sub_garegs_reg28
 * @sa ga_reg_cddcomm6
 *
 * @warning No bit level opcodes
 */
#define GA_REG_CDDCOMM6 0xFF8044

/**
 * @def GA_REG_CDDCOMM7
 * @brief CDD communication
 * @ingroup sub_garegs_reg28
 * @sa ga_reg_cddcomm7
 *
 * @warning No bit level opcodes
 */
#define GA_REG_CDDCOMM7 0xFF8046

/**
 * @def GA_REG_CDDCOMM8
 * @brief CDD communication
 * @ingroup sub_garegs_reg28
 * @sa ga_reg_cddcomm8
 *
 * @warning No bit level opcodes
 */
#define GA_REG_CDDCOMM8 0xFF8048

/**
 * @def GA_REG_CDDCOMM9
 * @brief CDD communication
 * @ingroup sub_garegs_reg28
 * @sa ga_reg_cddcomm9
 *
 * @warning No bit level opcodes
 */
#define GA_REG_CDDCOMM9 0xFF804A

#pragma endregion

#pragma region GA_REG_FONT_COLOR

/**
 * @defgroup sub_garegs_reg38 Reg. #38 - Font Palette Mapping
 * @ingroup sub_garegs_font
 */

/**
 * @def GA_REG_FONT_COLOR
 * @brief source color values
 * @ingroup sub_garegs_reg38
 * @sa ga_reg_fontcolor
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | | | |SC13|SC12|SC11|SC10|SC03|SC02|SC01|SC00|
 * |R| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 * |W| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 */
#define GA_REG_FONT_COLOR 0xFF804C

/**
 * @def GA_REG_FONT_COLOR_HI
 * @ingroup sub_garegs_reg38
 */
#define GA_REG_FONT_COLOR_HI GA_REG_FONT_COLOR

/**
 * @def GA_REG_FONT_COLOR_LO
 * @ingroup sub_garegs_reg38
 */
#define GA_REG_FONT_COLOR_LO (GA_REG_FONT_COLOR + 1)

#pragma endregion

#pragma region GA_REG_FONT_1BPP

/**
 * @defgroup sub_garegs_reg39 Reg. #39 - Font 1BPP Pattern
 * @ingroup sub_garegs_font
 */

/**
 * @def GA_REG_FONT_1BPP
 * @brief font data
 * @ingroup sub_garegs_reg39
 * @sa ga_reg_fontbits
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |SB||||||||||||||||
 * |R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |W|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 */
#define GA_REG_FONT_1BPP 0xFF804E

/**
 * @def GA_REG_FONT_1BPP_HI
 * @ingroup sub_garegs_reg39
 */
#define GA_REG_FONT_1BPP_HI GA_REG_FONT_1BPP

/**
 * @def GA_REG_FONT_1BPP_LO
 * @ingroup sub_garegs_reg39
 */
#define GA_REG_FONT_1BPP_LO (GA_REG_FONT_1BPP + 1)

#pragma endregion

#pragma region GA_REG_FONTDATA

/**
 * @defgroup sub_garegs_reg40 Reg. #40 - Font Data
 * @ingroup sub_garegs_font
 */

/**
 * @def GA_REG_FONTDATA
 * @brief read only
 * @ingroup sub_garegs_reg40
 * @sa ga_reg_fontdata
 */
#define GA_REG_FONTDATA 0xFF8050

#pragma endregion

#pragma region GA_REG_STAMPSIZE

/**
 * @defgroup sub_garegs_reg41 Reg. #41 - Stamp Size
 * @ingroup sub_garegs_gfx
 */

/**
 * @def GA_REG_STAMPSIZE
 * @brief Stamp data sizes
 * @ingroup sub_garegs_reg41
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
 * @ingroup sub_garegs_gfx
 */
#define GA_REG_STAMPSIZE 0xFF8058

/**
 * @def GA_BIT_STAMPSIZE_REPEAT
 * @ingroup sub_garegs_reg41
 */
#define GA_BIT_STAMPSIZE_REPEAT 0

/**
 * @def GA_BIT_STAMPSIZE_32x32_STAMP
 * @ingroup sub_garegs_reg41
 */
#define GA_BIT_STAMPSIZE_32x32_STAMP 1

/**
 * @def GA_BIT_STAMPSIZE_16x16_SCREEN
 * @ingroup sub_garegs_reg41
 */
#define GA_BIT_STAMPSIZE_16x16_SCREEN 2

/**
 * @def GA_MASK_STAMP_REPEAT
 * @ingroup sub_garegs_reg41
 */
#define GA_MASK_STAMP_REPEAT (1 << GA_BIT_STAMPSIZE_REPEAT)

/**
 * @def GA_MASK_STAMPSIZE_32x32_STAMP
 * @ingroup sub_garegs_reg41
 */
#define GA_MASK_STAMPSIZE_32x32_STAMP (1 << GA_BIT_STAMPSIZE_32x32_STAMP)

/**
 * @def GA_MASK_STAMPSIZE_16x16_SCREEN
 * @ingroup sub_garegs_reg41
 */
#define GA_MASK_STAMPSIZE_16x16_SCREEN (1 << GA_BIT_STAMPSIZE_16x16_SCREEN)

#pragma endregion

#pragma region GA_REG_STAMPMAPBASE

/**
 * @def GA_REG_STAMPMAPBASE
 * @sa ga_reg_stampmapbase
 * @ingroup sub_garegs_gfx
 * @note The location of the stamp map in Word RAM. The value you put in is the
 * offset relative to the start of Word RAM divided by 4. What the raw location
 * needs to be a multiple of depends on the sizes you have set:

    16x16px stamps, 256x256px stamp map: Multiples of 0x200
    32x32px stamps, 256x256px stamp map: Multiples of 0x80
    16x16px stamps, 4096x4096px stamp map: Multiples of 0x20000
    32x32px stamps, 4096x4096px stamp map: Multiples of 0x8000

 */
#define GA_REG_STAMPMAPBASE 0xFF805A

#pragma endregion

#pragma region GA_REG_IMGBUFVSIZE

/**
 * @def GA_REG_IMGBUFVSIZE
 * @sa ga_reg_imgbufvsize
 * @ingroup sub_garegs_gfx
 */
#define GA_REG_IMGBUFVSIZE 0xFF805C

#pragma endregion

#pragma region GA_REG_IMGBUFSTART

/**
 * @def GA_REG_IMGBUFSTART
 * @sa ga_reg_imgbufstart
 * @ingroup sub_garegs_gfx
 */
#define GA_REG_IMGBUFSTART 0xFF805E

#pragma endregion

#pragma region GA_REG_IMGBUFOFFSET

/**
 * @def GA_REG_IMGBUFOFFSET
 * @sa ga_reg_imgbufoffset
 * @ingroup sub_garegs_gfx
 */
#define GA_REG_IMGBUFOFFSET 0xFF8060

#pragma endregion

#pragma region GA_REG_IMGBUFHDOTSIZE

/**
 * @def GA_REG_IMGBUFHDOTSIZE
 * @sa ga_reg_imgbufhdotsize
 * @ingroup sub_garegs_gfx
 */
#define GA_REG_IMGBUFHDOTSIZE 0xFF8062

#pragma endregion

#pragma region GA_REG_IMGBUFVDOTSIZE

/**
 * @def GA_REG_IMGBUFVDOTSIZE
 * @sa ga_reg_imgbufvdotsize
 * @ingroup sub_garegs_gfx
 */
#define GA_REG_IMGBUFVDOTSIZE 0xFF8064

#pragma endregion

#pragma region GA_REG_TRACEVECTBASE

/**
 * @def GA_REG_TRACEVECTBASE
 * @sa ga_reg_tracevectbase
 * @ingroup sub_garegs_gfx
 */
#define GA_REG_TRACEVECTBASE 0xFF8066

#pragma endregion

#pragma region GA_REG_SUBCODEADDR

/**
 * @def GA_REG_SUBCODEADDR
 * @sa ga_reg_subcodeaddr
 * @ingroup sub_garegs_subcode
 */
#define GA_REG_SUBCODEADDR 0xFF8068

#pragma endregion

#pragma region GA_REG_SUBCODEBUF

/**
 * @def GA_REG_SUBCODEBUF
 * @sa ga_reg_subcodebuf
 * @ingroup sub_garegs_subcode
 */
#define GA_REG_SUBCODEBUF 0xFF8100

#pragma endregion

#pragma region GA_REG_SUBCODEBUFIMG

/**
 * @def GA_REG_SUBCODEBUFIMG
 * @sa ga_reg_subcodebufimg
 * @ingroup sub_garegs_subcode
 */
#define GA_REG_SUBCODEBUFIMG 0xFF8180
#pragma endregion

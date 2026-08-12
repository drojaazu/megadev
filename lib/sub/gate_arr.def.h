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
 * 
 * @warning Official documentation indicates subcode registers should not be
 * accessed by the user and that the related BIOS calls should be used instead.
 * 
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
 *
 * @details
 * \b GA_REG_RESET
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | |\b LED_G|\b LED_R|\b ROM_VER|||| | | |\b SUB_RESET|
 * |\b R| | | | | | |◯|◯|◯|◯|◯|◯| | | |◯|
 * |\b W| | | | | | |◯|◯| | | | | | | |◯|
 *
 * @param GA_SUB_RESET CPU Reset
 * @param GA_ROM_VER ROM Version
 * @param GA_LED_R Red LED control
 * @param GA_LED_G Green LED control
 */

/**
 * @def GA_REG_RESET
 * @brief CPU Reset / LED Control / ROM Version
 * @ingroup sub_garegs_reg01
 *
 */
#define GA_REG_RESET 0xFF8000

/**
 * @def GA_SUB_RESET_BIT
 * @ingroup sub_garegs_reg01
 *
 */
#define GA_SUB_RESET_BIT 0

/**
 * @def GA_SUB_RESET
 * @brief Reset Sup CPU
 * @ingroup sub_garegs_reg01
 * @alias RES
 *
 * @param Read
 * - 0: CPU is being reset
 * - 1: Reset complete, CPU operating
 * @param Write
 * - 0: Initiate CPU reset
 * - 1: No effect
 *
 * @details
 * Reset time is 100ms.
 */
#define GA_SUB_RESET (1 << GA_SUB_RESET_BIT)

/**
 * @def GA_LED_R_BIT
 * @ingroup sub_garegs_reg01
 *
 */
#define GA_LED_R_BIT 0

/**
 * @def GA_LED_R
 * @brief Red LED Control
 * @ingroup sub_garegs_reg01
 * @alias LEDR
 *
 * @param Read
 * - 0: Red LED is off
 * - 1: Red LED is on
 * @param Write
 * - 0: Turn red LED off
 * - 1: Turn red LED on
 */
#define GA_LED_R (1 << GA_LED_R_BIT)

/**
 * @def GA_LED_G_BIT
 * @ingroup sub_garegs_reg01
 *
 */
#define GA_LED_G_BIT 1

/**
 * @def GA_LED_G
 * @brief Green LED Control
 * @ingroup sub_garegs_reg01
 * @alias LEDG
 *
 * @param Read
 * - 0: Green LED is off
 * - 1: Green LED is on
 * @param Write
 * - 0: Turn green LED off
 * - 1: Turn green LED on
 */
#define GA_LED_G (1 << GA_LED_G_BIT)

/**
 * @def GA_ROM_VER
 * @brief ROM Version
 * @ingroup sub_garegs_reg01
 *
 * @param Read ROM version
 */
#define GA_ROM_VER (0b1111 << 4)

#pragma endregion

#pragma region GA_REG_MEMMODE

/**
 * @defgroup sub_garegs_reg02 Reg. #2 - Word RAM Layout / PRGRAM Write Protect
 * @ingroup sub_garegs_sysctrl
 *
 * @details
 * \b GA_REG_MEMMODE
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b WP|||||||| | | |\b PM||\b WDRAM_MODE|\b ACQUIRE_WDRAM|\b CEDE_WDRAM|
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯| | | |◯|◯|◯|◯|◯|
 * |\b W| | | | | | | | | | | |◯|◯|◯| |◯|
 *
 * @param CEDE_WDRAM Word RAM control
 * @param ACQUIRE_WDRAM Main CPU will not access Word RAM
 * @param WDRAM_MODE Word RAM layout
 * @param PM Priority Mode
 * @param WP Write protect Sub CPU RAM
 *
 */

/**
 * @def GA_REG_MEMMODE
 * @brief Word RAM Control / PRG RAM Write Protect Status
 * @ingroup sub_garegs_reg02
 *
 */
#define GA_REG_MEMMODE 0xFF8002

/**
 * @def GA_REG_RESET_HI
 * @ingroup sub_garegs_reg02
 */
#define GA_REG_MEMMODE_HI GA_REG_MEMMODE

/**
 * @def GA_REG_MEMMODE_LO
 * @ingroup sub_garegs_reg02
 */
#define GA_REG_MEMMODE_LO (GA_REG_MEMMODE + 1)

/**
 * @def GA_CEDE_WDRAM2M_BIT
 * @ingroup sub_garegs_reg02
 *
 */
#define GA_CEDE_WDRAM2M_BIT 0

// clang-format off
/**
 * @def GA_CEDE_WDRAM2M
 * @brief Change Word RAM ownership
 * @ingroup sub_garegs_reg02
 * @alias RET
 *
 * @param Read
 * - 0: Word RAM is not controlled by Main CPU
 * - 1: Word RAM is controlled by Main CPU
 * @param Write
 * - 0: Assign Word RAM control to Sub CPU
 * - 1: Assign Word RAM control to Main CPU
 *
 * @todo Regarding writing 0 in 2M mode: the "The Hardware" documentation has
 * nothing to say (undefined). The "Development Manual" documentation indicates
 * a write of 0 assigns to Sub CPU, though this seems redundant with DMNA. Need
 * to research effects on actual hardware.
 */
// clang-format on
#define GA_CEDE_WDRAM2M (1 << GA_CEDE_WDRAM2M_BIT)

/**
 * @def GA_SWAP_WDRAM1M_BIT
 * @ingroup sub_garegs_reg02
 */
#define GA_SWAP_WDRAM1M_BIT 0

/**
 * @def GA_SWAP_WDRAM1M
 * @brief Change Word RAM ownership bitmask
 * @ingroup sub_garegs_reg02
 * @alias RET
 *
 * @param Read
 * - 0: Main CPU controls Word RAM bank 0 and Sub CPU controls bank 1
 * - 1: Main CPU controls Word RAM bank 1 and Sub CPU controls bank 0
 * @param Write
 * - 0: Assign Word RAM bank 0 to Main CPU and bank 1 to Sub CPU
 * - 1: Assign Word RAM bank 1 to Main CPU and bank 0 to Sub CPU
 *
 */
#define GA_SWAP_WDRAM1M (1 << GA_SWAP_WDRAM1M_BIT)

/**
 * @def GA_ACQUIRE_WDRAM2M_BIT
 * @ingroup sub_garegs_reg02
 */
#define GA_ACQUIRE_WDRAM2M_BIT 1

/**
 * @def GA_ACQUIRE_WDRAM2M
 * @brief Main CPU no Word RAM access flag bitmask
 * @ingroup sub_garegs_reg02
 * @alias DMNA
 *
 * @param Read
 * - 0 (Word RAM 2M mode): Word RAM controlled by Main CPU
 * - 1 (Word RAM 2M mode): Word RAM control given to Sub CPU
 * - 0 (Word RAM 1M mode): Word RAM bank control swap completed
 * - 1 (Word RAM 1M mode): Word RAM bank control swap in progress
 */
#define GA_ACQUIRE_WDRAM2M (1 << GA_ACQUIRE_WDRAM2M_BIT)

/**
 * @def GA_WDRAM_MODE_BIT
 * @ingroup sub_garegs_reg02
 */
#define GA_WDRAM_MODE_BIT 2

/**
 * @def GA_WDRAM2M_MODE
 * @brief Word RAM layout bitmask
 * @ingroup sub_garegs_reg02
 * @alias MODE
 *
 * @param Read
 * - 0: Word RAM is in 2M mode
 * - 1: Word RAM is in 1M mode
 * @param Write
 * - 0: Set Word RAM mode to 2M
 * - 1: Set Word RAM mode to 1M
 */
#define GA_WDRAM2M_MODE (1 << GA_WDRAM_MODE_BIT)

#pragma endregion

#pragma region GA_REG_CDC_MODE

// clang-format off
/**
 * @defgroup sub_garegs_reg03 Reg. #3 - CDC Destination and Registers #1
 * @ingroup sub_garegs_cdc
 *
 * @details
 * \b GA_REG_CDC_MODE
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b TRANSFER_COMPLETE|\b CDC_DATA_READY|\b UBR| | |\b CDC_DESTINATION||| | | | |\b CDC_REGS1||||
 * |\b R|◯︎|◯|◯| | |◯|◯|◯| | | | |◯|◯|◯|◯|
 * |\b W| | | | | |◯|◯|◯| | | | |◯|◯|◯|◯|
 *
 * @param GA_CDC_REGS1 CDC register address
 * @param GA_DATA_DEST Device destination
 * @param UBR Upper byte ready
 * @param GA_DATA_READY Data set ready
 * @param GA_DATA_COMPLETE End of data transfer
 *
 */
// clang-format on

/**
 * @def GA_REG_CDC_MODE
 * @brief CDC Destination and Registers #1
 * @ingroup sub_garegs_reg03
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
 * @def GA_CDC_REGS1
 * @ingroup sub_garegs_reg03
 *
 */
#define GA_CDC_REGS1 0b1111

/**
 * @def GA_DATA_DEST
 * @brief Sets the CDC data transfer destination
 * @ingroup sub_garegs_reg03\
 * @alias DD
 *
 * @param Read Get data transfer destination
 * @param Write Set data transfer destination
 *
 * @details
 * Destination devices:
 *   |DD2|DD1|DD0|Destination|Symbol|
 *   |:|:|:|:|:|
 *   |0|1|0|Main CPU| @ref GA_CDC_DEST_MAIN |
 *   |0|1|1|Sub CPU| @ref GA_CDC_DEST_SUB |
 *   |1|0|0|PCM DMA| @ref GA_CDC_DEST_PCM_DMA |
 *   |1|0|1|PRG RAM DMA| @ref GA_CDC_DEST_SUB_DMA |
 *   |1|1|1|Sub CPU controlled Word RAM| @ref GA_CDC_DEST_WDRAM_DMA |
 *
 * All other values are invalid.
 *
 */
#define GA_DATA_DEST 0b111

/**
 * @def GA_CDC_DEST_MAIN
 * @ingroup sub_garegs_reg03
 */
#define GA_CDC_DEST_MAIN 0b010

/**
 * @def GA_CDC_DEST_SUB
 * @ingroup sub_garegs_reg03
 */
#define GA_CDC_DEST_SUB 0b011

/**
 * @def GA_CDC_DEST_PCM_DMA
 * @ingroup sub_garegs_reg03
 */
#define GA_CDC_DEST_PCM_DMA 0b100

/**
 * @def GA_CDC_DEST_SUB_DMA
 * @ingroup sub_garegs_reg03
 */
#define GA_CDC_DEST_SUB_DMA 0b101

/**
 * @def GA_CDC_DEST_WDRAM_DMA
 * @ingroup sub_garegs_reg03
 */
#define GA_CDC_DEST_WDRAM_DMA 0b111

/**
 * @def GA_DATA_READY_BIT
 * @ingroup sub_garegs_reg03
 */
#define GA_DATA_READY_BIT 6

/**
 * @def GA_DATA_READY
 * @ingroup sub_garegs_reg03
 * @alias DSR
 *
 * @param Read
 * - 0: Data not yet arrived from CDC / Data sent to destination from gate array
 * - 1: Data has arrived in the gate array from the CDC
 */
#define GA_DATA_READY (1 << GA_DATA_READY_BIT)

/**
 * @def GA_DATA_COMPLETE_BIT
 * @ingroup sub_garegs_reg03
 */
#define GA_DATA_COMPLETE_BIT 7

/**
 * @def GA_DATA_COMPLETE
 * @ingroup sub_garegs_reg03
 *
 * @param Read
 * - 0: Data still pending from CDC
 * - 1: All data from CDC has been sent
 *
 * @details
 * Reset when a data transfer destination is set (see \ref GA_DATA_DEST) or when
 * data transfer from the CDC begins.
 */
#define GA_DATA_COMPLETE (1 << GA_DATA_COMPLETE_BIT)

#pragma endregion

#pragma region GA_REG_CDC_REG

/**
 * @defgroup sub_garegs_reg04 Reg. #4 - CDC Registers #2
 * @ingroup sub_garegs_cdc
 *
 * @details
 * \b GA_REG_CDC_REG
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | | | |CDC_REGS1||||||||
 * |R| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 * |W| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param CDC_REGS1 CDC register data
 *
 * @warning No bit level opcodes
 */

/**
 * @def GA_REG_CDC_REG
 * @brief CDC Registers #2
 * @ingroup sub_garegs_reg04
 *
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

/**
 * @defgroup sub_garegs_reg05 Reg. #5 - CDC Host Data
 * @ingroup sub_garegs_cdc
 *
 * @warning Word access only; Read only; No bit level opcodes
 *
 * @details
 * \b GA_REG_CDC_DATA
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b CDC_DATA||||||||||||||||
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|
 *
 * @param CDC_DATA CDC read data
 */

/**
 * @def GA_REG_CDC_DATA
 * @brief CDC Host Data
 * @ingroup sub_garegs_reg05
 *
 * @warning Word access only; Read only; No bit level opcodes
 *
 * @details
 * Contains the two accumulated bytes from the CDC before transfer to their
 * destination.
 *
 */
#define GA_REG_CDC_DATA 0xFF8008

#pragma endregion

#pragma region GA_REG_DMA_ADDRESS

/**
 * @defgroup sub_garegs_reg06 Reg. #6 - CDC DMA Destination
 * @ingroup sub_garegs_cdc
 *
 * @warning Word access only; Write only; No bit level opcodes
 *
 * @details
 * \b GA_REG_DMA_ADDRESS
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

 *
 */

/**
 * @def GA_REG_DMA_ADDRESS
 * @brief CDC DMA Destination
 * @ingroup sub_garegs_reg06
 *
 * @warning Word access only; Write only; No bit level opcodes
 *
 * @details
 * Specifies the address for CDC DMA transfer
 *
 * - For PCM DMA: bits up to A12 are used
 * - For 1M Word RAM: bits up to A16 are used
 * - For 2M Word RAM: bits up to A17 are used
 * - For PRG-RAM: all bits are used
 *
 * Unused bits will be read as 0.
 *
 */
#define GA_REG_DMA_ADDRESS 0xFF800A

#define DMAADDR_WDRAM1M(addr) (((addr) & 0x3FFF) >> 3)
#define DMAADDR_WDRAM2M(addr) (((addr) & 0x7FFF) >> 3)
#define DMAADDR_PCM(addr)     (((addr) & 0x03FF) >> 3)
#define DMAADDR_PRGRAM(addr)  ((addr) >> 3)

#pragma endregion

#pragma region GA_REG_STOPWATCH

/**
 * @defgroup sub_garegs_reg07 Reg. #7 - Stopwatch
 * @ingroup sub_garegs_misc
 *
 * @warning Word access only; No bit level opcodes
 *
 * @details
 * \b GA_REG_STOPWATCH
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | |\b SW||||||||||||
 * |\b R| | | | |◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | |◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @note Word access only
 *
 * @param SW Timer data

 */

/**
 * @def GA_REG_STOPWATCH
 * @brief Stopwatch
 * @ingroup sub_garegs_reg07
 * @alias SW
 *
 * @param Read Current timer value (0 to 4095)
 * @param Write Resets timer (see below)
 *
 * @details
 * This is a general use timer, though it is primarily used for CDD/CDC
 * timing. Each tick is 30.72 microseconds.
 */
#define GA_REG_STOPWATCH 0xFF800C

#pragma endregion

#pragma region GA_REG_COMM_FLAGS

/**
 * @defgroup sub_garegs_reg08 Reg. #8 - Communication Flags
 * @ingroup sub_garegs_comm
 *
 * @details
 * \b GA_REG_COMM_FLAGS
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |CFM||||||||CFS||||||||
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * CFM: Comm flags for Main CPU
 * CFS: Comm flags for Sub CPU
 */

/**
 * @def GA_REG_COMM_FLAGS
 * @brief Main/Sub CPU communication flags
 * @ingroup sub_garegs_reg08
 */
#define GA_REG_COMM_FLAGS 0xFF800E

#define GA_REG_COMM_FLAGS_HI GA_REG_COMM_FLAGS

#define GA_REG_COMM_FLAGS_LO (GA_REG_COMM_FLAGS + 1)

#pragma endregion

#pragma region GA_REG_COMM_CMD

/**
 * @defgroup sub_garegs_reg09 Reg. #8-15 - Comm. Commands
 * @ingroup sub_garegs_comm
 *
 * @details
 * \b GA_REG_COMM_CMD
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |CMD||||||||||||||||
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|
 *
 */

/**
 * @def GA_REG_COMM_CMD0
 * @brief Comm Command 0 (Main -> Sub)
 * @ingroup sub_garegs_reg09
 *
 */
#define GA_REG_COMM_CMD0 0xFF8010

/**
 * @def GA_REG_COMM_CMD1
 * @brief Comm Command  (Main -> Sub)
 * @ingroup sub_garegs_reg09
 *
 */
#define GA_REG_COMM_CMD1 0xFF8012

/**
 * @def GA_REG_COMM_CMD2
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMM_CMD2 0xFF8014

/**
 * @def GA_REG_COMM_CMD3
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMM_CMD3 0xFF8016

/**
 * @def GA_REG_COMM_CMD4
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMM_CMD4 0xFF8018

/**
 * @def GA_REG_COMM_CMD5
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMM_CMD5 0xFF801A

/**
 * @def GA_REG_COMM_CMD6
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMM_CMD6 0xFF801C

/**
 * @def GA_REG_COMM_CMD7
 * @ingroup sub_garegs_reg09
 */
#define GA_REG_COMM_CMD7 0xFF801E

#pragma endregion

#pragma region GA_REG_COMM_STAT

/**
 * @defgroup sub_garegs_reg16 Reg. #16-23 - Comm. Status
 * @ingroup sub_garegs_comm
 *
 * @details
 * \b GA_REG_COMM_STAT
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |STAT||||||||||||||||
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 */

/**
 * @def GA_REG_COMM_STAT0
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMM_STAT0 0xFF8020

/**
 * @def GA_REG_COMM_STAT1
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMM_STAT1 0xFF8022

/**
 * @def GA_REG_COMM_STAT2
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMM_STAT2 0xFF8024

/**
 * @def GA_REG_COMM_STAT3
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMM_STAT3 0xFF8026

/**
 * @def GA_REG_COMM_STAT4
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMM_STAT4 0xFF8028

/**
 * @def GA_REG_COMM_STAT5
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMM_STAT5 0xFF802A

/**
 * @def GA_REG_COMM_STAT6
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMM_STAT6 0xFF802C

/**
 * @def GA_REG_COMM_STAT7
 * @ingroup sub_garegs_reg16
 */
#define GA_REG_COMM_STAT7 0xFF802E

#pragma endregion

#pragma region GA_INT3_TIMER

/**
 * @defgroup sub_garegs_reg24 Reg. #24 - Timer
 * @ingroup sub_garegs_misc
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | | | |\b TD||||||||
 * |\b R| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param TD Timer
 *
 */

/**
 * @def GA_INT3_TIMER
 * @brief timer, 30.72us lsb, 0->INT3
 * @ingroup sub_garegs_reg24
 * @sa ga_reg_comstat7
 *
 */
#define GA_INT3_TIMER 0xFF8030

#pragma endregion

#pragma region GA_INT_MASK

/**
 * @defgroup sub_garegs_reg25 Reg. #25 - Interrupt Mask
 * @ingroup sub_garegs_sysctrl
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | |\b LEDG|\b LEDR|\b VER|||| | | |\b RES|
 * |\b R| | | | | | |◯|◯|◯|◯|◯|◯| | | |◯|
 * |\b W| | | | | | |◯|◯| | | | | | | |◯|
 */

/**
 * @def GA_INT_MASK
 * @brief interrupt control
 * @ingroup sub_garegs_reg25
 * @sa GA_INTmask
 *
 */
#define GA_INT_MASK 0xFF8032

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT1_BIT 1

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT2_BIT 2

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT3_BIT 3

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT4_BIT 4

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT5_BIT 5

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT6_BIT 6

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT1 (1 << GA_INT1_BIT)

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT2 (1 << GA_INT2_BIT)

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT3 (1 << GA_INT3_BIT)

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT4 (1 << GA_INT4_BIT)

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT5 (1 << GA_INT5_BIT)

/**
 * @ingroup sub_garegs_reg25
 */
#define GA_INT6 (1 << GA_INT6_BIT)

#pragma endregion

#pragma region GA_REG_FADER

/**
 * @defgroup sub_garegs_reg26 Reg. #26 - CD Fader / Spindle Control
 * @ingroup sub_garegs_misc
 */

/**
 * @def GA_REG_FADER
 * @brief fader control / spindle speed
 * @ingroup sub_garegs_reg26
 * @sa GA_REG_FADER
 *
 * @warning Word access only; No bit level opcodes
 *
 * @warning Official documentation indicates that direct user access to this
 * register is prohibited. Consider using the Sub BIOS commands that wrap around
 * this register instead.
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
#define GA_REG_FADER 0xFF8034

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
 * @warning Official documentation indicates that direct user access to this
 * register is prohibited. Consider using the Sub BIOS commands that wrap around
 * this register instead.
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
 *
 * @warning Official documentation indicates that direct user access to this
 * register is prohibited. Consider using the Sub BIOS commands that wrap around
 * this register instead.
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
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | | | |SC13|SC12|SC11|SC10|SC03|SC02|SC01|SC00|
 * |R| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 * |W| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 */

/**
 * @def GA_REG_FONT_COLOR
 * @brief source color values
 * @ingroup sub_garegs_reg38
 * @sa ga_reg_fontcolor
 *
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
 *
 * @details
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |SB||||||||||||||||
 * |R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |W|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 */

/**
 * @def GA_REG_FONT_1BPP
 * @brief font data
 * @ingroup sub_garegs_reg39
 * @sa ga_reg_fontbits
 *
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
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |GRON| | | | | | | | | | | | |SMS|STS|RPT|
 * |R|◯| | | | | | | | | | | | |◯|◯|◯|
 * |W| | | | | | | | | | | | | |◯|◯|◯|
 *
 * @param RPT Repeat
 * @param STS Stamp size
 * @param SMS Stamp map size
 * @param GRON Graphics operation in progress
 */

/**
 * @def GA_REG_STAMPSIZE
 * @brief Stamp data sizes
 * @ingroup sub_garegs_reg41
 *
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
 * @defgroup sub_garegs_reg42 Reg. #42 - Stamp Map Base
 * @ingroup sub_garegs_gfx
 *
 * @warning Word access only; No bit level opcodes
 *
 * @note The location of the stamp map in Word RAM. The value you put in is the
 * offset relative to the start of Word RAM divided by 4. What the raw location
 * needs to be a multiple of depends on the sizes you have set:

    16x16px stamps, 256x256px stamp map: Multiples of 0x200
    32x32px stamps, 256x256px stamp map: Multiples of 0x80
    16x16px stamps, 4096x4096px stamp map: Multiples of 0x20000
    32x32px stamps, 4096x4096px stamp map: Multiples of 0x8000
 */

/**
 * @def GA_REG_STAMPMAPBASE
 * @brief Stamp Map Base
 * @ingroup sub_garegs_reg42
 *
 */
#define GA_REG_STAMPMAPBASE 0xFF805A

#pragma endregion

#pragma region GA_REG_IMGBUFVSIZE

/**
 * @defgroup sub_garegs_reg43 Reg. #43 - Image Buffer V Size
 * @ingroup sub_garegs_gfx
 *
 * @warning No bit level opcodes
 *
 */

/**
 * @def GA_REG_IMGBUFVSIZE
 * @ingroup sub_garegs_reg43
 *
 */
#define GA_REG_IMGBUFVSIZE 0xFF805C

#pragma endregion

#pragma region GA_REG_IMGBUFSTART

/**
 * @defgroup sub_garegs_reg44 Reg. #44 - Image Buffer Start
 * @ingroup sub_garegs_gfx
 *
 * @warning Word access only; No bit level opcodes
 *
 */

/**
 * @def GA_REG_IMGBUFSTART
 * @ingroup sub_garegs_reg44
 *
 */
#define GA_REG_IMGBUFSTART 0xFF805E

#pragma endregion

#pragma region GA_REG_IMGBUFOFFSET

/**
 * @defgroup sub_garegs_reg45 Reg. #45 - Image Buffer Offset
 * @ingroup sub_garegs_gfx
 *
 */

/**
 * @def GA_REG_IMGBUFOFFSET
 * @ingroup sub_garegs_reg45
 */
#define GA_REG_IMGBUFOFFSET 0xFF8060

#pragma endregion

#pragma region GA_REG_IMGBUFHDOTSIZE

/**
 * @defgroup sub_garegs_reg46 Reg. #46 - Image Buffer H Dot Size
 * @ingroup sub_garegs_gfx
 *
 * @warning Word access only; No bit level opcodes
 *
 */

/**
 * @def GA_REG_IMGBUFHDOTSIZE
 * @ingroup sub_garegs_reg46
 */
#define GA_REG_IMGBUFHDOTSIZE 0xFF8062

#pragma endregion

#pragma region GA_REG_IMGBUFVDOTSIZE

/**
 * @defgroup sub_garegs_reg47 Reg. #47 - Image Buffer V Dot Size
 * @ingroup sub_garegs_gfx
 *
 * @warning Word access only; No bit level opcodes
 *
 */

/**
 * @def GA_REG_IMGBUFVDOTSIZE
 * @ingroup sub_garegs_reg47
 */
#define GA_REG_IMGBUFVDOTSIZE 0xFF8064

#pragma endregion

#pragma region GA_REG_TRACEVECTBASE

/**
 * @defgroup sub_garegs_reg48 Reg. #48 - Trace Vector Base
 * @ingroup sub_garegs_gfx
 *
 * @warning Word access only; No bit level opcodes
 *
 */

/**
 * @def GA_REG_TRACEVECTBASE
 * @ingroup sub_garegs_reg48
 */
#define GA_REG_TRACEVECTBASE 0xFF8066

#pragma endregion

#pragma region GA_REG_SUBCODEADDR

/**
 * @defgroup sub_garegs_reg49 Reg. #49 - Subcode Address
 * @ingroup sub_garegs_subcode
 *
 * @details
 * \b GA_REG_SUBCODEADDR
 * | |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | | | |SAOR|STA|||||| |
 * |R| | | | | | | | |◯|◯|◯|◯|◯|◯|◯| |
 * |W| | | | | | | | | | | | | | | | |
 *
 * @param STA Subcode top address
 * @param SAOR Subcode address overrun
 *
 */

/**
 * @def GA_REG_SUBCODEADDR
 * @ingroup sub_garegs_reg49
 */
#define GA_REG_SUBCODEADDR 0xFF8068

#pragma endregion

#pragma region GA_REG_SUBCODEBUF

/**
 * @defgroup sub_garegs_reg50 Reg. #50 - Subcode Buffer
 * @ingroup sub_garegs_subcode
 *
 */

/**
 * @def GA_REG_SUBCODEBUF
 * @ingroup sub_garegs_reg50
 */
#define GA_REG_SUBCODEBUF 0xFF8100

#pragma endregion

#pragma region GA_REG_SUBCODEBUFIMG

/**
 * @defgroup sub_garegs_reg51 Reg. #51 - Subcode Buffer Image
 * @ingroup sub_garegs_subcode
 *
 */

/**
 * @def GA_REG_SUBCODEBUFIMG
 * @ingroup sub_garegs_reg51
 */
#define GA_REG_SUBCODEBUFIMG 0xFF8180

#pragma endregion

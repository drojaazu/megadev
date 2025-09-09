/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.def.h
 * @brief Main CPU side Gate Array registers
 */

#ifndef MEGADEV__GATE_ARRAY_DEF_H
#define MEGADEV__GATE_ARRAY_DEF_H

/**
 * @defgroup ga_regs_main Main CPU / Gate Array / Registers
 *
 * @note Unless otherwise indicated, all register bitmasks are byte-wise as
 * that is how the registers will often be accessed. Take note that several
 * registers are word access only, though these registers contain data that
 * would logically be accessed by word rather than by byte.
 */

/**
 * @defgroup ga_reg_main_subctrl Main CPU / Gate Array / Registers / Sub CPU
 * Control
 */

/**
 * @def GAREG_RESET
 * @brief Sub CPU Control
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_subctrl
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |IEN2| ||||||IFL2| ||||||SBRQ|SRES|
 *
 * @param
 * \n [width] 8 bit/16 bit
 * @param SRES Sub CPU reset
 * \n [write] 0: Reset / 1: Run
 * \n [read] 0: Reset in progress / 1: Running
 * @param SBRQ Sub CPU bus access request
 * \n [write] 0: Cancel request / 1: Request access
 * \n [read] 0: Sub CPU running / 1: Acknowledge
 * @param IFL2 Send INT2 to Sub CPU
 * \n [write] 0: Not used / 1: Raise INT2 on Sub CPU
 * \n [read] 0: INT2 in progress / 1: INT2 not occurred yet
 * @param IEN2 Mask state of INT2 on Sub CPU
 * @details 0: Masked / 1: Enabled
 *
 * @note Only BTST bitwise operation allowed on this register
 */
#define GAREG_RESET 0xA12000

/**
 * @def GA_SUB_RESET
 * @brief Sub CPU Reset
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_subctrl
 */
#define GA_SUB_RESET (1 << 0)

/**
 * @def GA_SUB_REQ
 * @brief Sub CPU Bus Access Request
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_subctrl
 */
#define GA_SUB_REQ (1 << 1)

/**
 * @def GA_RAISE_INT2_BIT
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_subctrl
 */
#define GA_RAISE_INT2_BIT 0

/**
 * @def GA_SUB_REQ
 * @brief Raise INT2 on Sub CPU
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_subctrl
 */
#define GA_RAISE_INT2 (1 << GA_RAISE_INT2_BIT)

/**
 * @def GA_INT2_MASK
 * @brief INT2 Masking on Sub CPU
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_subctrl
 */
#define GA_INT2_MASK (1 << 7)

/**
 * @defgroup ga_reg_main_memmode Main CPU / Gate Array / Registers / Memory Mode
 */

/**
 * @def GAREG_MEMMODE
 * @brief Memory Mode
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |WP7|WP6|WP5|WP4|WP3|WP2|WP1|WP0|BK1|BK0| |||MODE|DMNA|RET|
 *
 * @param [width] 8 bit/16 bit
 * @param WP Write protect Sub CPU RAM
 * @param BK PRG-RAM bank select
 * @param MODE Word RAM layout
 * \n 0: 2M
 * \n 1: 1M
 * @param DMNA Main CPU will not access Word RAM
 * \n\em In 2M mode:
 * \n [write] 0: N/A / 1: Return Word RAM to Sub CPU
 * \n [read] 0: Return Word RAM in process / 1: Return Word RAM complete
 * \n\em In 1M mode:
 * \n [write] 0: Bank swap in progress / 1: Bank swap complete
 * @param RET Give Word RAM control to Main CPU
 *
 * @details
 * WP Write protect an area of Sub CPU RAM from 0 to 0x1FE00 in increments
 * of 0x200
 * BK|PRG-RAM bank select for Main CPU access
 * (4M PRG-RAM divided into 1M banks)
 * MODE|Word RAM Mode
 *     |  Read Only: 0: 2M / 1: 1M/1M
 * RET|Return Word RAM to Main CPU
 *      Effect depends on MODE bit:
 *			MODE = 0 (2M):
 *			 Read Only: 0: Return Word RAM to Main CPU In Progress
 *			            1: Return Word RAM to Main CPU Completed
 *       MODE = 1 (1M/1M):
 *        Read Only: 0: Word RAM Bank 0 attached to Main CPU, Bank 1 to Sub CPU
 *                   1: Word RAM Bank 0 attached to Sub CPU, Bank 1 to Main CPU
 *
 */
#define GAREG_MEMMODE 0xA12002

/**
 * @def GA_RET_BIT
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_RET_BIT 0

/**
 * @def GA_RET
 * @brief Give Word RAM control to Main CPU
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_RET (1 << GA_RET_BIT)

/**
 * @def GA_DMNA_BIT
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_DMNA_BIT 1

/**
 * @def GA_DMNA
 * @brief Main CPU will not access Word RAM
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_DMNA (1 << GA_DMNA_BIT)

/**
 * @def GA_MODE
 * @brief Word RAM layout
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_MODE (1 << 2)

#define MEMMODE_BK0_MSK 0b11 << 6
#define MEMMODE_WP0_MSK 0xFF

/**
 * @defgroup ga_reg_main_cdcmode Main CPU / Gate Array / Registers / CDC Mode
 */

/**
 * @def GAREG_CDCMODE
 * @brief CDC Mode
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cdcmode
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |EDT|DSR| |||DD2|DD1|DD0| ||||||||
 *
 * @param [width] 8 bit/16 bit
 * @param EDT End of data transfer
 * [read] All data from the CDC has been transferred
 * @param DSR Data set ready
 * [read] Data from the CDC is present in the CDC Host Data register
 * @param DD Device destination
 * [read/write] Specifies the destination for CDC data transfer:
 *   |DD2|DD1|DD0|Destination|
 *   |:|:|:|:|
 *   |0|1|0|Main CPU|
 *   |0|1|1|Sub CPU|
 *   |1|0|0|PCM DMA|
 *   |1|1|1|In 2M Mode: Word RAM \n In 1M Mode: Sub CPU controlled Word RAM|
 *
 *   All other values for DD are invalid.
 */
#define GAREG_CDCMODE 0xA12004

/**
 * @def GA_CDC_DEST_MAIN
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cdcmode
 */
#define GA_CDC_DEST_MAIN 0b010

/**
 * @def GA_CDC_DEST_SUB
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cdcmode
 */
#define GA_CDC_DEST_SUB 0b011

/**
 * @def GA_CDC_DEST_PCM
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cdcmode
 */
#define GA_CDC_DEST_PCM 0b100

/**
 * @def GA_CDC_DEST_WORD
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cdcmode
 */
#define GA_CDC_DEST_WORD 0b111

/**
 * @defgroup ga_reg_main_hintvect Main CPU / Gate Array / Registers / HINT
 * Vector
 */

/**
 * @def GAREG_HINTVECT
 * @brief HINT Vector
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_hintvect
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HIBF|HIBE|HIBD|HIBC|HIBB|HIBA|HIB9|HIB8|HIB7|HIB6|HIB5|HIB4|HIB3|HIB2|HIB1|HIB0|
 *
 * @param [width] 16 bit
 * @param HIB Specifies the lower word of the HINT (Level 4) interrupt vector
 *      The upper word is specied at the standard location (0x70), the value
 *      of which is 0x00FF by the Boot ROM.
 *
 * @warning Bit level opcodes (BTST, BCLR, BSET) are undefined for this register
 */
#define GAREG_HINTVECT 0xA12006

/**
 * @defgroup ga_reg_main_cdcdata Main CPU / Gate Array / Registers / CDC Host
 * Data
 */

/**
 * @def GAREG_CDCHOSTDATA
 * @brief CDC Host Data
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cdcdata
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HD15|HD14|HD13|HD12|HD11|HD10|HD09|HD08|HD07|HD06|HD05|HD04|HD03|HD02|HD01|HD00|
 *
 * @param [width] 16 bit
 * @param HD CDC read data
 *
 * @warning Bit level opcodes (BTST, BCLR, BSET) are undefined for this register
 */
#define GAREG_CDCHOSTDATA 0xA12008

/**
 * @defgroup ga_reg_main_stopwatch Main CPU / Gate Array / Registers / Stopwatch
 */

/**
 * @def GAREG_STOPWATCH
 * @brief Stop watch
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_stopwatch
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |||| |TD11|TD10|TD09|TD08|TD07|TD06|TD05|TD04|TD03|TD02|TD01|TD00|
 *
 * @param [width] 16 bit
 * @param TD Timer data
 *
 * @details One count is timed to 30.72µs
 *
 * @warning Bit level opcodes (BTST, BCLR, BSET) are undefined for this register
 */
#define GAREG_STOPWATCH 0xA1200C

/**
 * @defgroup ga_reg_main_cpucomm Main CPU / Gate Array / Registers / CPU
 * Communication
 */

/**
 * @def GAREG_COMFLAGS
 * @brief CPU Communication Flags
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |CFM7|CFM6|CFM5|CFM4|CFM3|CFM2|CFM1|CFM0|CFS7|CFS6|CFS5|CFS4|CFS3|CFS2|CFS1|CFS0|
 *
 * @param [width] 8 bit/16 bit
 * @param CFM Comm flags for Main CPU
 * @param CFS Comm flags for Sub CPU
 */
#define GAREG_COMFLAGS 0xA1200E

/**
 * @def GAREG_COMCMD0
 * @brief Comm Command 0 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 */
#define GAREG_COMCMD0 0xA12010

/**
 * @def GAREG_COMCMD1
 * @brief Comm Command 1 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 */
#define GAREG_COMCMD1 0xA12012

/**
 * @def GAREG_COMCMD2
 * @brief Comm Command 2 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 */
#define GAREG_COMCMD2 0xA12014

/**
 * @def GAREG_COMCMD3
 * @brief Comm Command 3 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 */
#define GAREG_COMCMD3 0xA12016

/**
 * @def GAREG_COMCMD4
 * @brief Comm Command 4 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 */
#define GAREG_COMCMD4 0xA12018

/**
 * @def GAREG_COMCMD5
 * @brief Comm Command 5 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 */
#define GAREG_COMCMD5 0xA1201A

/**
 * @def GAREG_COMCMD6
 * @brief Comm Command 6 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 */
#define GAREG_COMCMD6 0xA1201C

/**
 * @def GAREG_COMCMD7
 * @brief Comm Command 7 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 */
#define GAREG_COMCMD7 0xA1201E

/**
 * @def GAREG_COMSTAT0
 * @brief Comm Status 0 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 */
#define GAREG_COMSTAT0 0xA12020

/**
 * @def GAREG_COMSTAT1
 * @brief Comm Status 1 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 */
#define GAREG_COMSTAT1 0xA12022

/**
 * @def GAREG_COMSTAT2
 * @brief Comm Status 2 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 */
#define GAREG_COMSTAT2 0xA12024

/**
 * @def GAREG_COMSTAT3
 * @brief Comm Status 3 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 */
#define GAREG_COMSTAT3 0xA12026

/**
 * @def GAREG_COMSTAT4
 * @brief Comm Status 4 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 */
#define GAREG_COMSTAT4 0xA12028

/**
 * @def GAREG_COMSTAT5
 * @brief Comm Status 5 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 */
#define GAREG_COMSTAT5 0xA1202A

/**
 * @def GAREG_COMSTAT6
 * @brief Comm Status 6 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 */
#define GAREG_COMSTAT6 0xA1202C

/**
 * @def GAREG_COMSTAT7
 * @brief Comm Status 7 (Main -> Sub)
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cpucomm
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 */
#define GAREG_COMSTAT7 0xA1202E

#endif

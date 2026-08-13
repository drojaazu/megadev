/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.def.h
 * @brief Main CPU side Gate Array registers
 */

#pragma once

/*
 * Per-register documentation groups.
 *
 * Registers are numbered by their word index from the base address, 0-based, to
 * match the vdp_regNN groups. The address is given in each title because the
 * hardware manual identifies these registers by address rather than by number.
 */

/**
 * @defgroup ga_reg_main_00 Main CPU / Gate Array / Register 00 (0xA12000) - Sub CPU Control
 * @ingroup ga_reg_main_subctrl
 */

/**
 * @defgroup ga_reg_main_01 Main CPU / Gate Array / Register 01 (0xA12002) - Memory Mode
 * @ingroup ga_reg_main_memmode
 */

/**
 * @defgroup ga_reg_main_02 Main CPU / Gate Array / Register 02 (0xA12004) - CDC Mode
 * @ingroup ga_reg_main_cdcmode
 */

/**
 * @defgroup ga_reg_main_03 Main CPU / Gate Array / Register 03 (0xA12006) - HBLANK Vector
 * @ingroup ga_reg_main_hblankvect
 */

/**
 * @defgroup ga_reg_main_04 Main CPU / Gate Array / Register 04 (0xA12008) - CDC Host Data
 * @ingroup ga_reg_main_cdcdata
 */

/**
 * @defgroup ga_reg_main_06 Main CPU / Gate Array / Register 06 (0xA1200C) - Stop watch
 * @ingroup ga_reg_main_stopwatch
 */

/**
 * @defgroup ga_reg_main_07 Main CPU / Gate Array / Register 07 (0xA1200E) - CPU Communication Flags
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_08 Main CPU / Gate Array / Register 08 (0xA12010) - Comm Command 0 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_09 Main CPU / Gate Array / Register 09 (0xA12012) - Comm Command 1 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_10 Main CPU / Gate Array / Register 10 (0xA12014) - Comm Command 2 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_11 Main CPU / Gate Array / Register 11 (0xA12016) - Comm Command 3 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_12 Main CPU / Gate Array / Register 12 (0xA12018) - Comm Command 4 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_13 Main CPU / Gate Array / Register 13 (0xA1201A) - Comm Command 5 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_14 Main CPU / Gate Array / Register 14 (0xA1201C) - Comm Command 6 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_15 Main CPU / Gate Array / Register 15 (0xA1201E) - Comm Command 7 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_16 Main CPU / Gate Array / Register 16 (0xA12020) - Comm Status 0 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_17 Main CPU / Gate Array / Register 17 (0xA12022) - Comm Status 1 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_18 Main CPU / Gate Array / Register 18 (0xA12024) - Comm Status 2 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_19 Main CPU / Gate Array / Register 19 (0xA12026) - Comm Status 3 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_20 Main CPU / Gate Array / Register 20 (0xA12028) - Comm Status 4 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_21 Main CPU / Gate Array / Register 21 (0xA1202A) - Comm Status 5 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_22 Main CPU / Gate Array / Register 22 (0xA1202C) - Comm Status 6 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */

/**
 * @defgroup ga_reg_main_23 Main CPU / Gate Array / Register 23 (0xA1202E) - Comm Status 7 (Main -> Sub)
 * @ingroup ga_reg_main_cpucomm
 */
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
 * @def GA_REG_RESET
 * @brief Sub CPU Control
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
 * @warning Of the bit operations, only BTST is permitted.
 * @ingroup ga_reg_main_00
 */
#define GA_REG_RESET 0xA12000

/**
 * @def GA_REG_RESET_HI
 * @brief High byte of @ref GA_REG_RESET
 * @ingroup ga_regs_main
 * @details The gate array registers are 16 bit, but this one is frequently
 * accessed a byte at a time. GA_REG_RESET_HI is an alias for the register address
 * itself; prefer it over the bare name when you mean a byte access, so the
 * width you intended is visible at the call site.
 */
#define GA_REG_RESET_HI GA_REG_RESET

/**
 * @def GA_REG_RESET_LO
 * @brief Low byte of @ref GA_REG_RESET
 * @ingroup ga_regs_main
 */
#define GA_REG_RESET_LO (GA_REG_RESET + 1)


/**
 * @def GA_SUB_RESET_MASK
 * @brief Sub CPU Reset
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_subctrl
 */
#define GA_SUB_RESET_POS 0
#define GA_SUB_RESET_WIDTH 1
#define GA_SUB_RESET_MASK FIELD_MASK(GA_SUB_RESET_POS, GA_SUB_RESET_WIDTH)

/**
 * @def GA_SUB_REQ_MASK
 * @brief Sub CPU Bus Access Request
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_subctrl
 */
#define GA_SUB_REQ_POS 1
#define GA_SUB_REQ_WIDTH 1
#define GA_SUB_REQ_MASK FIELD_MASK(GA_SUB_REQ_POS, GA_SUB_REQ_WIDTH)

/**
 * @def GA_RAISE_INT2_POS
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_subctrl
 */
#define GA_RAISE_INT2_POS 0
#define GA_RAISE_INT2_WIDTH 1

/**
 * @def GA_SUB_REQ_MASK
 * @brief Raise INT2 on Sub CPU
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_subctrl
 */
#define GA_RAISE_INT2_MASK FIELD_MASK(GA_RAISE_INT2_POS, GA_RAISE_INT2_WIDTH)

/**
 * @def GA_INT2_MASKING_MASK
 * @brief INT2 Masking on Sub CPU
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_subctrl
 */
#define GA_INT2_MASKING_POS 7
#define GA_INT2_MASKING_WIDTH 1
#define GA_INT2_MASKING_MASK FIELD_MASK(GA_INT2_MASKING_POS, GA_INT2_MASKING_WIDTH)

/**
 * @defgroup ga_reg_main_memmode Main CPU / Gate Array / Registers / Memory Mode
 */

/**
 * @def GA_REG_MEMMODE
 * @brief Memory Mode
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
 * @ingroup ga_reg_main_01
 */
#define GA_REG_MEMMODE 0xA12002

/**
 * @def GA_REG_MEMMODE_HI
 * @brief High byte of @ref GA_REG_MEMMODE
 * @ingroup ga_regs_main
 * @details The gate array registers are 16 bit, but this one is frequently
 * accessed a byte at a time. GA_REG_MEMMODE_HI is an alias for the register address
 * itself; prefer it over the bare name when you mean a byte access, so the
 * width you intended is visible at the call site.
 */
#define GA_REG_MEMMODE_HI GA_REG_MEMMODE

/**
 * @def GA_REG_MEMMODE_LO
 * @brief Low byte of @ref GA_REG_MEMMODE
 * @ingroup ga_regs_main
 */
#define GA_REG_MEMMODE_LO (GA_REG_MEMMODE + 1)


/**
 * @def GA_RETURN_2M_POS
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_RETURN_2M_POS 0
#define GA_RETURN_2M_WIDTH 1

/**
 * @def GA_RETURN_2M_MASK
 * @brief Give Word RAM control to Main CPU
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_RETURN_2M_MASK FIELD_MASK(GA_RETURN_2M_POS, GA_RETURN_2M_WIDTH)

/**
 * @def GA_DMNA_POS
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_DMNA_POS 1
#define GA_DMNA_WIDTH 1

/**
 * @def GA_DMNA_MASK
 * @brief Main CPU will not access Word RAM
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_DMNA_MASK FIELD_MASK(GA_DMNA_POS, GA_DMNA_WIDTH)

/**
 * @def GA_WORDRAM_LAYOUT_MASK
 * @brief Word RAM layout
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_WORDRAM_LAYOUT_POS 2
#define GA_WORDRAM_LAYOUT_WIDTH 1
#define GA_WORDRAM_LAYOUT_MASK FIELD_MASK(GA_WORDRAM_LAYOUT_POS, GA_WORDRAM_LAYOUT_WIDTH)

#define GA_MEMMODE_BANK_POS 6
#define GA_MEMMODE_BANK_WIDTH 2
#define GA_MEMMODE_BANK_MASK FIELD_MASK(GA_MEMMODE_BANK_POS, GA_MEMMODE_BANK_WIDTH)
#define GA_MEMMODE_WP_POS 0
#define GA_MEMMODE_WP_WIDTH 8
#define GA_MEMMODE_WP_MASK FIELD_MASK(GA_MEMMODE_WP_POS, GA_MEMMODE_WP_WIDTH)

/**
 * @defgroup ga_reg_main_cdcmode Main CPU / Gate Array / Registers / CDC Mode
 */

/**
 * @def GA_REG_CDCMODE
 * @brief CDC Mode
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
 * @ingroup ga_reg_main_02
 */
#define GA_REG_CDCMODE 0xA12004

/**
 * @def GA_CDC_DEST_MAIN
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cdcmode
 */
#define GA_CDC_DEST_POS 0
#define GA_CDC_DEST_WIDTH 3
#define GA_CDC_DEST_MASK FIELD_MASK(GA_CDC_DEST_POS, GA_CDC_DEST_WIDTH)

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
 * @defgroup ga_reg_main_hblankvect Main CPU / Gate Array / Registers / HBLANK
 * Vector
 */

/**
 * @def GA_REG_HBLANKVECT
 * @brief HBLANK Vector
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HIBF|HIBE|HIBD|HIBC|HIBB|HIBA|HIB9|HIB8|HIB7|HIB6|HIB5|HIB4|HIB3|HIB2|HIB1|HIB0|
 *
 * @param [width] 16 bit
 * @param HIB Specifies the lower word of the HBLANK (Level 4) interrupt vector
 *      The upper word is specied at the standard location (0x70), the value
 *      of which is 0x00FF by the Boot ROM.
 *
 * @warning Bit level opcodes (BTST, BCLR, BSET) are undefined for this register
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @ingroup ga_reg_main_03
 */
#define GA_REG_HBLANKVECT 0xA12006

/**
 * @defgroup ga_reg_main_cdcdata Main CPU / Gate Array / Registers / CDC Host
 * Data
 */

/**
 * @def GA_REG_CDCHOSTDATA
 * @brief CDC Host Data
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
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_main_04
 */
#define GA_REG_CDCHOSTDATA 0xA12008

/**
 * @defgroup ga_reg_main_stopwatch Main CPU / Gate Array / Registers / Stopwatch
 */

/**
 * @def GA_REG_STOPWATCH
 * @brief Stop watch
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
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_main_06
 */
#define GA_REG_STOPWATCH 0xA1200C

/**
 * @defgroup ga_reg_main_cpucomm Main CPU / Gate Array / Registers / CPU
 * Communication
 */

/**
 * @def GA_REG_COMFLAGS
 * @brief CPU Communication Flags
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |CFM7|CFM6|CFM5|CFM4|CFM3|CFM2|CFM1|CFM0|CFS7|CFS6|CFS5|CFS4|CFS3|CFS2|CFS1|CFS0|
 *
 * @param [width] 8 bit/16 bit
 * @param CFM Comm flags for Main CPU
 * @param CFS Comm flags for Sub CPU
 * @ingroup ga_reg_main_07
 */
#define GA_REG_COMFLAGS 0xA1200E

/**
 * @def GA_REG_COMCMD0
 * @brief Comm Command 0 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 * @ingroup ga_reg_main_08
 */
#define GA_REG_COMCMD0 0xA12010

/**
 * @def GA_REG_COMCMD1
 * @brief Comm Command 1 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 * @ingroup ga_reg_main_09
 */
#define GA_REG_COMCMD1 0xA12012

/**
 * @def GA_REG_COMCMD2
 * @brief Comm Command 2 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 * @ingroup ga_reg_main_10
 */
#define GA_REG_COMCMD2 0xA12014

/**
 * @def GA_REG_COMCMD3
 * @brief Comm Command 3 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 * @ingroup ga_reg_main_11
 */
#define GA_REG_COMCMD3 0xA12016

/**
 * @def GA_REG_COMCMD4
 * @brief Comm Command 4 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 * @ingroup ga_reg_main_12
 */
#define GA_REG_COMCMD4 0xA12018

/**
 * @def GA_REG_COMCMD5
 * @brief Comm Command 5 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 * @ingroup ga_reg_main_13
 */
#define GA_REG_COMCMD5 0xA1201A

/**
 * @def GA_REG_COMCMD6
 * @brief Comm Command 6 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 * @ingroup ga_reg_main_14
 */
#define GA_REG_COMCMD6 0xA1201C

/**
 * @def GA_REG_COMCMD7
 * @brief Comm Command 7 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param CMD Communication data
 * @ingroup ga_reg_main_15
 */
#define GA_REG_COMCMD7 0xA1201E

/**
 * @def GA_REG_COMSTAT0
 * @brief Comm Status 0 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 * @ingroup ga_reg_main_16
 */
#define GA_REG_COMSTAT0 0xA12020

/**
 * @def GA_REG_COMSTAT1
 * @brief Comm Status 1 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 * @ingroup ga_reg_main_17
 */
#define GA_REG_COMSTAT1 0xA12022

/**
 * @def GA_REG_COMSTAT2
 * @brief Comm Status 2 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 * @ingroup ga_reg_main_18
 */
#define GA_REG_COMSTAT2 0xA12024

/**
 * @def GA_REG_COMSTAT3
 * @brief Comm Status 3 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 * @ingroup ga_reg_main_19
 */
#define GA_REG_COMSTAT3 0xA12026

/**
 * @def GA_REG_COMSTAT4
 * @brief Comm Status 4 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 * @ingroup ga_reg_main_20
 */
#define GA_REG_COMSTAT4 0xA12028

/**
 * @def GA_REG_COMSTAT5
 * @brief Comm Status 5 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 * @ingroup ga_reg_main_21
 */
#define GA_REG_COMSTAT5 0xA1202A

/**
 * @def GA_REG_COMSTAT6
 * @brief Comm Status 6 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 * @ingroup ga_reg_main_22
 */
#define GA_REG_COMSTAT6 0xA1202C

/**
 * @def GA_REG_COMSTAT7
 * @brief Comm Status 7 (Main -> Sub)
 *
 * @param [width] 8 bit/16 bit
 * @param STAT Comm Status
 * @ingroup ga_reg_main_23
 */
#define GA_REG_COMSTAT7 0xA1202E

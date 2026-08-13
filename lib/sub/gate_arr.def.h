/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.def.h
 * @brief Sub CPU side Gate Array registers
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
 * @defgroup ga_reg_sub_00 Sub CPU / Gate Array / Register 00 (0xFF8000) - Sub CPU & Hardware Control
 * @ingroup ga_reg_sub_subctrl
 */

/**
 * @defgroup ga_reg_sub_01 Sub CPU / Gate Array / Register 01 (0xFF8002) - Word RAM Memory Mode / RAM Write Protect / Priority Mode
 * @ingroup ga_reg_sub_memmode
 */

/**
 * @defgroup ga_reg_sub_02 Sub CPU / Gate Array / Register 02 (0xFF8004) - CDC Mode & CDC Registers #1
 * @ingroup ga_reg_sub_cdcmode
 */

/**
 * @defgroup ga_reg_sub_03 Sub CPU / Gate Array / Register 03 (0xFF8006) - CDC Registers #2
 * @ingroup ga_reg_sub_cdcregs
 */

/**
 * @defgroup ga_reg_sub_04 Sub CPU / Gate Array / Register 04 (0xFF8008) - CDC Host Data
 * @ingroup ga_reg_sub_cdcdata
 */

/**
 * @defgroup ga_reg_sub_05 Sub CPU / Gate Array / Register 05 (0xFF800A) - CDC DMA Address
 * @ingroup ga_reg_sub_dmaaddr
 */

/**
 * @defgroup ga_reg_sub_06 Sub CPU / Gate Array / Register 06 (0xFF800C) - Stopwatch
 * @ingroup ga_reg_sub_stopwatch
 */

/**
 * @defgroup ga_reg_sub_07 Sub CPU / Gate Array / Register 07 (0xFF800E) - Main/Sub CPU communication flags
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_08 Sub CPU / Gate Array / Register 08 (0xFF8010) - Comm Command 0 (Main -> Sub)
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_09 Sub CPU / Gate Array / Register 09 (0xFF8012) - Comm Command  (Main -> Sub)
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_10 Sub CPU / Gate Array / Register 10 (0xFF8014) - Communication Command 2
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_11 Sub CPU / Gate Array / Register 11 (0xFF8016) - Communication Command 3
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_12 Sub CPU / Gate Array / Register 12 (0xFF8018) - Communication Command 4
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_13 Sub CPU / Gate Array / Register 13 (0xFF801A) - Communication Command 5
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_14 Sub CPU / Gate Array / Register 14 (0xFF801C) - Communication Command 6
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_15 Sub CPU / Gate Array / Register 15 (0xFF801E) - Communication Command 7
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_16 Sub CPU / Gate Array / Register 16 (0xFF8020) - Communication Status 0
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_17 Sub CPU / Gate Array / Register 17 (0xFF8022) - Communication Status 1
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_18 Sub CPU / Gate Array / Register 18 (0xFF8024) - Communication Status 2
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_19 Sub CPU / Gate Array / Register 19 (0xFF8026) - Communication Status 3
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_20 Sub CPU / Gate Array / Register 20 (0xFF8028) - Communication Status 4
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_21 Sub CPU / Gate Array / Register 21 (0xFF802A) - Communication Status 5
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_22 Sub CPU / Gate Array / Register 22 (0xFF802C) - Communication Status 6
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_23 Sub CPU / Gate Array / Register 23 (0xFF802E) - Communication Status 7
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_24 Sub CPU / Gate Array / Register 24 (0xFF8030) - Timer (INT3)
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_25 Sub CPU / Gate Array / Register 25 (0xFF8032) - Interrupt Mask
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_26 Sub CPU / Gate Array / Register 26 (0xFF8034) - CD Fader
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_27 Sub CPU / Gate Array / Register 27 (0xFF8036) - CDD Control
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_28 Sub CPU / Gate Array / Register 28 (0xFF8038) - CDD Communication 0
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_29 Sub CPU / Gate Array / Register 29 (0xFF803A) - CDD Communication 1
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_30 Sub CPU / Gate Array / Register 30 (0xFF803C) - CDD Communication 2
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_31 Sub CPU / Gate Array / Register 31 (0xFF803E) - CDD Communication 3
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_32 Sub CPU / Gate Array / Register 32 (0xFF8040) - CDD Communication 4
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_33 Sub CPU / Gate Array / Register 33 (0xFF8042) - CDD Communication 5
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_34 Sub CPU / Gate Array / Register 34 (0xFF8044) - CDD Communication 6
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_35 Sub CPU / Gate Array / Register 35 (0xFF8046) - CDD Communication 7
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_36 Sub CPU / Gate Array / Register 36 (0xFF8048) - CDD Communication 8
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_37 Sub CPU / Gate Array / Register 37 (0xFF804A) - CDD Communication 9
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_38 Sub CPU / Gate Array / Register 38 (0xFF804C) - Font Colour
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_39 Sub CPU / Gate Array / Register 39 (0xFF804E) - Font Bits
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_40 Sub CPU / Gate Array / Register 40 (0xFF8050) - Font Data
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_44 Sub CPU / Gate Array / Register 44 (0xFF8058) - Stamp Size
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_45 Sub CPU / Gate Array / Register 45 (0xFF805A) - Stamp Map Base Address
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_46 Sub CPU / Gate Array / Register 46 (0xFF805C) - Image Buffer V-cell Size
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_47 Sub CPU / Gate Array / Register 47 (0xFF805E) - Image Buffer Start Address
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_48 Sub CPU / Gate Array / Register 48 (0xFF8060) - Image Buffer Offset
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_49 Sub CPU / Gate Array / Register 49 (0xFF8062) - Image Buffer H-dot Size
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_50 Sub CPU / Gate Array / Register 50 (0xFF8064) - Image Buffer V-dot Size
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_51 Sub CPU / Gate Array / Register 51 (0xFF8066) - Trace Vector Base Address
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_52 Sub CPU / Gate Array / Register 52 (0xFF8068) - Sub-code Address
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_128 Sub CPU / Gate Array / Register 128 (0xFF8100) - GA_REG_SUBCODEBUF
 * @ingroup ga_reg_sub_cpucomm
 */

/**
 * @defgroup ga_reg_sub_192 Sub CPU / Gate Array / Register 192 (0xFF8180) - GA_REG_SUBCODEBUFIMG
 * @ingroup ga_reg_sub_cpucomm
 */
/**
 * @defgroup ga_regs_sub Sub CPU / Gate Array / Registers
 */

/**
 * @defgroup ga_reg_sub_subctrl Sub CPU / Gate Array / Registers / Sub CPU &
 * Hardware Control
 */

/**
 * @def GA_REG_RESET
 * @brief Peripheral reset, drive LEDs and gate array version
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |  |  |  |  |  |  |LEDG|LEDR|VER3|VER2|VER1|VER0|  |  |  |RES0|
 * |R |  |  |  |  |  |  | o| o| o| o| o| o|  |  |  | o|
 * |W |  |  |  |  |  |  | o| o|  |  |  |  |  |  |  | o|
 *
 * @param RES0 Peripheral reset.
 * \n [write] 0: reset the peripheral (1 is not used)
 * \n [read] 0: the peripheral is being reset / 1: the peripheral is operable
 * \n Roughly 100 ms after a reset the peripheral becomes operable and RES0
 * becomes 1 of its own accord.
 * @param VER0-3 Gate array chip version. Read only.
 * @param LEDR Red LED. 1: on, 0: off. Lit during CD access.
 * @param LEDG Green LED. 1: on, 0: off. Lit when the drive is ready.
 *
 * The two LEDs are read together as a drive state indicator, and the BIOS
 * drives them:
 *
 * | Green | Red | Meaning |
 * |---|---|---|
 * | 1 | 0 | Ready. No disc, or waiting for the TOC to finish reading |
 * | 1 | 0 | Ready. A disc is present and readable |
 * | 1 | 1 | Disc access in progress |
 * | either | 0 | Standby |
 *
 * Power-on and reset clear both LEDs. Any other combination requires a
 * special system mode.
 *
 * @sa ga_reg_reset
 * @ingroup ga_reg_sub_00
 */
#define GA_REG_RESET 0xFF8000

/**
 * @def GA_REG_RESET_HI
 * @brief High byte of @ref GA_REG_RESET
 * @ingroup ga_regs_sub
 * @details The gate array registers are 16 bit, but this one is frequently
 * accessed a byte at a time. GA_REG_RESET_HI is an alias for the register address
 * itself; prefer it over the bare name when you mean a byte access, so the
 * width you intended is visible at the call site.
 */
#define GA_REG_RESET_HI GA_REG_RESET

/**
 * @def GA_REG_RESET_LO
 * @brief Low byte of @ref GA_REG_RESET
 * @ingroup ga_regs_sub
 */
#define GA_REG_RESET_LO (GA_REG_RESET + 1)


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
 * @def GA_VERSION_MASK
 * @brief ROM Version
 * @sa ga_reg_reset
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_subctrl
 */
#define GA_VERSION_POS 4
#define GA_VERSION_WIDTH 4
#define GA_VERSION_MASK FIELD_MASK(GA_VERSION_POS, GA_VERSION_WIDTH)

/**
 * @defgroup ga_reg_sub_memmode Sub CPU / Gate Array / Registers / Memory Mode
 */

/**
 * @def GA_REG_MEMMODE
 * @brief Word RAM ownership and layout, PRG RAM write protection
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |WP7|WP6|WP5|WP4|WP3|WP2|WP1|WP0|  |  |  |PM1|PM0|MODE|DMNA|RET|
 * |R | o| o| o| o| o| o| o| o|  |  |  | o| o| o| o| o|
 * |W |  |  |  |  |  |  |  |  |  |  |  | o| o| o|  | o|
 *
 * @param WP0-7 Write protect for the lower PRG RAM. Read only from this side.
 * @param PM0-1 Priority mode, applied when the Sub CPU writes into Word RAM.
 * \n 0,0: off - all stamp map data is written to the image buffer
 * \n 0,1: underwrite - stamp data is written only where the buffer holds 0
 * \n 1,0: overwrite - only non-zero stamp data is written
 * \n 1,1: prohibited
 * @param MODE Word RAM layout. 0: 2M, 1: 1M.
 * @param DMNA Declaration of Main RAM No Access.
 * \n In 2M mode, writing 1 returns Word RAM to the Sub CPU. Reading 0 means
 * it has not been returned yet; 1 means it has.
 * \n In 1M mode, reading 1 means the Main CPU has requested a bank swap, and
 * reading 0 means the swap is complete. Setting RET also sets DMNA.
 * @param RET The counterpart to DMNA.
 * \n In 2M mode, writing 1 gives Word RAM to the Main CPU. Reading 0 means it
 * has not been given up yet; 1 means it has.
 *
 * @note DMNA and RET are the two halves of Word RAM handover: each declares
 * the transfer in one direction, and the bit you write is not the bit you
 * poll to confirm it.
 *
 * @sa ga_reg_memmode
 * @ingroup ga_reg_sub_01
 */
#define GA_REG_MEMMODE 0xFF8002

/**
 * @def GA_REG_MEMMODE_HI
 * @brief High byte of @ref GA_REG_MEMMODE
 * @ingroup ga_regs_sub
 * @details The gate array registers are 16 bit, but this one is frequently
 * accessed a byte at a time. GA_REG_MEMMODE_HI is an alias for the register address
 * itself; prefer it over the bare name when you mean a byte access, so the
 * width you intended is visible at the call site.
 */
#define GA_REG_MEMMODE_HI GA_REG_MEMMODE

/**
 * @def GA_REG_MEMMODE_LO
 * @brief Low byte of @ref GA_REG_MEMMODE
 * @ingroup ga_regs_sub
 */
#define GA_REG_MEMMODE_LO (GA_REG_MEMMODE + 1)


#define GA_RETURN_2M_POS 0
#define GA_RETURN_2M_WIDTH 1
#define GA_DMNA_POS 1
#define GA_DMNA_WIDTH 1
#define GA_WORDRAM_LAYOUT_POS 2
#define GA_WORDRAM_LAYOUT_WIDTH 1

/**
 * @def GA_RETURN_2M_MASK
 * @brief GA Reg 01 - Change Word RAM ownership bitmask
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_memmode
 */
#define GA_RETURN_2M_MASK FIELD_MASK(GA_RETURN_2M_POS, GA_RETURN_2M_WIDTH)

/**
 * @def GA_DMNA_MASK
 * @brief GA Reg 01 - Main CPU no Word RAM access flag bitmask
 * @sa ga_reg_memmode
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_memmode
 */
#define GA_DMNA_MASK FIELD_MASK(GA_DMNA_POS, GA_DMNA_WIDTH)

/**
 * @def GA_WORDRAM_LAYOUT_MASK
 * @brief GA Reg 01 - Word RAM layout bitmask
 * @sa ga_reg_memmode
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_memmode
 */
#define GA_WORDRAM_LAYOUT_MASK FIELD_MASK(GA_WORDRAM_LAYOUT_POS, GA_WORDRAM_LAYOUT_WIDTH)

/**
 * @defgroup ga_reg_sub_cdcmode Sub CPU / Gate Array / Registers / CDC Mode &
 * CDC Registers #1
 */

/**
 * @def GA_REG_CDCMODE
 * @brief CDC Mode & CDC Registers #1
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
 * @warning Of the bit operations, only BTST is permitted.
 * @ingroup ga_reg_sub_02
 */
#define GA_REG_CDCMODE 0xFF8004

/**
 * @defgroup ga_reg_sub_cdcregs2 Sub CPU / Gate Array / Registers / CDC
 * Registers #1
 */

/**
 * @def GA_REG_CDCRS1
 * @brief CDC Registers #2
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||||CD7|CD6|CD5|CD4|CD3|CD2|CD1|CD0|
 *
 * @param CD CDC register data
 * \n Details for this register can be found in the LC89510 manual
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_03
 */
#define GA_REG_CDCRS1 0xFF8006

/**
 * @defgroup ga_reg_sub_cdcdata Sub CPU / Gate Array / Registers / CDC Host Data
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
 * @param HD CDC read data
 * [read] 2 bytes of data read by the CDC and ready to be transferred to
 * Main or Sub CPU memory
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_04
 */
#define GA_REG_CDCHOSTDATA 0xFF8008

/**
 * @defgroup ga_reg_sub_dmaaddr Sub CPU / Gate Array / Registers / CDC DMA
 * Address
 */

/**
 * @def GA_REG_DMAADDR
 * @brief CDC DMA Address
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
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_05
 */
#define GA_REG_DMAADDR 0xFF800A

/**
 * @defgroup ga_reg_sub_stopwatch Sub CPU / Gate Array / Registers / Stopwatch
 */

/**
 * @def GA_REG_STOPWATCH
 * @brief Stopwatch
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
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_06
 */
#define GA_REG_STOPWATCH 0xFF800C

/**
 * @defgroup ga_reg_sub_cpucomm Sub CPU / Gate Array / Registers / CPU
 * Communication
 */

/**
 * @def GA_REG_COMFLAGS
 * @brief Main/Sub CPU communication flags
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |CFM7|CFM6|CFM5|CFM4|CFM3|CFM2|CFM1|CFM0|CFS7|CFS6|CFS5|CFS4|CFS3|CFS2|CFS1|CFS0|
 *
 * CFM: Comm flags for Main CPU
 * CFS: Comm flags for Sub CPU
 * @ingroup ga_reg_sub_07
 */
#define GA_REG_COMFLAGS 0xFF800E

/**
 * @def GA_REG_COMCMD0
 * @brief Comm Command 0 (Main -> Sub)
 *
 * @details R: 16 bit data
 * @ingroup ga_reg_sub_08
 */
#define GA_REG_COMCMD0 0xFF8010

/**
 * @def GA_REG_COMCMD1
 * @brief Comm Command  (Main -> Sub)
 * @ingroup ga_reg_sub_09
 */
#define GA_REG_COMCMD1 0xFF8012

/**
 * @def GA_REG_COMCMD2
 * @ingroup ga_reg_sub_10
 */
#define GA_REG_COMCMD2 0xFF8014

/**
 * @def GA_REG_COMCMD3
 * @ingroup ga_reg_sub_11
 */
#define GA_REG_COMCMD3 0xFF8016

/**
 * @def GA_REG_COMCMD4
 * @ingroup ga_reg_sub_12
 */
#define GA_REG_COMCMD4 0xFF8018

/**
 * @def GA_REG_COMCMD5
 * @ingroup ga_reg_sub_13
 */
#define GA_REG_COMCMD5 0xFF801A

/**
 * @def GA_REG_COMCMD6
 * @ingroup ga_reg_sub_14
 */
#define GA_REG_COMCMD6 0xFF801C

/**
 * @def GA_REG_COMCMD7
 * @ingroup ga_reg_sub_15
 */
#define GA_REG_COMCMD7 0xFF801E

/**
 * @def GA_REG_COMSTAT0
 * @ingroup ga_reg_sub_16
 */
#define GA_REG_COMSTAT0 0xFF8020

/**
 * @def GA_REG_COMSTAT1
 * @ingroup ga_reg_sub_17
 */
#define GA_REG_COMSTAT1 0xFF8022

/**
 * @def GA_REG_COMSTAT2
 * @ingroup ga_reg_sub_18
 */
#define GA_REG_COMSTAT2 0xFF8024

/**
 * @def GA_REG_COMSTAT3
 * @ingroup ga_reg_sub_19
 */
#define GA_REG_COMSTAT3 0xFF8026

/**
 * @def GA_REG_COMSTAT4
 * @ingroup ga_reg_sub_20
 */
#define GA_REG_COMSTAT4 0xFF8028

/**
 * @def GA_REG_COMSTAT5
 * @ingroup ga_reg_sub_21
 */
#define GA_REG_COMSTAT5 0xFF802A

/**
 * @def GA_REG_COMSTAT6
 * @ingroup ga_reg_sub_22
 */
#define GA_REG_COMSTAT6 0xFF802C

/**
 * @def GA_REG_COMSTAT7
 * @ingroup ga_reg_sub_23
 */
#define GA_REG_COMSTAT7 0xFF802E

/**
 * @def GA_REG_INT3TIMER
 * @sa ga_reg_comstat7
 * @ingroup ga_reg_sub_24
 */
#define GA_REG_INT3TIMER 0xFF8030

/**
 * @def GA_REG_INTMASK
 * @sa ga_reg_intmask
 * @ingroup ga_reg_sub_25
 */
#define GA_REG_INTMASK 0xFF8032

#define GA_INT1_POS 1
#define GA_INT1_WIDTH 1
#define GA_INT2_POS 2
#define GA_INT2_WIDTH 1
#define GA_INT3_POS 3
#define GA_INT3_WIDTH 1
#define GA_INT4_POS 4
#define GA_INT4_WIDTH 1
#define GA_INT5_POS 5
#define GA_INT5_WIDTH 1
#define GA_INT6_POS 6
#define GA_INT6_WIDTH 1

#define GA_INT1_MASK FIELD_MASK(GA_INT1_POS, GA_INT1_WIDTH)
#define GA_INT2_MASK FIELD_MASK(GA_INT2_POS, GA_INT2_WIDTH)
#define GA_INT3_MASK FIELD_MASK(GA_INT3_POS, GA_INT3_WIDTH)
#define GA_INT4_MASK FIELD_MASK(GA_INT4_POS, GA_INT4_WIDTH)
#define GA_INT5_MASK FIELD_MASK(GA_INT5_POS, GA_INT5_WIDTH)
#define GA_INT6_MASK FIELD_MASK(GA_INT6_POS, GA_INT6_WIDTH)

/**
 * @def GA_REG_CDFADER
 * @sa ga_reg_cdfader
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_26
 */
#define GA_REG_CDFADER 0xFF8034

/**
 * @def GA_REG_CDDCTRL
 * @sa ga_reg_cddctrl
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_27
 */
#define GA_REG_CDDCTRL 0xFF8036

/**
 * @def GA_REG_CDDCOMM0
 * @sa ga_reg_cddcomm0
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_28
 */
#define GA_REG_CDDCOMM0 0xFF8038

/**
 * @def GA_REG_CDDCOMM1
 * @sa ga_reg_cddcomm1
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_29
 */
#define GA_REG_CDDCOMM1 0xFF803A

/**
 * @def GA_REG_CDDCOMM2
 * @sa ga_reg_cddcomm2
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_30
 */
#define GA_REG_CDDCOMM2 0xFF803C

/**
 * @def GA_REG_CDDCOMM3
 * @sa ga_reg_cddcomm2
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_31
 */
#define GA_REG_CDDCOMM3 0xFF803E

/**
 * @def GA_REG_CDDCOMM4
 * @sa ga_reg_cddcomm4
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_32
 */
#define GA_REG_CDDCOMM4 0xFF8040

/**
 * @def GA_REG_CDDCOMM5
 * @sa ga_reg_cddcomm5
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_33
 */
#define GA_REG_CDDCOMM5 0xFF8042

/**
 * @def GA_REG_CDDCOMM6
 * @sa ga_reg_cddcomm6
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_34
 */
#define GA_REG_CDDCOMM6 0xFF8044

/**
 * @def GA_REG_CDDCOMM7
 * @sa ga_reg_cddcomm7
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_35
 */
#define GA_REG_CDDCOMM7 0xFF8046

/**
 * @def GA_REG_CDDCOMM8
 * @sa ga_reg_cddcomm8
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_36
 */
#define GA_REG_CDDCOMM8 0xFF8048

/**
 * @def GA_REG_CDDCOMM9
 * @sa ga_reg_cddcomm9
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_37
 */
#define GA_REG_CDDCOMM9 0xFF804A

/**
 * @def GA_REG_FONTCOLOR
 * @sa ga_reg_fontcolor
 * @ingroup ga_reg_sub_38
 */
#define GA_REG_FONTCOLOR 0xFF804C

/**
 * @def GA_REG_FONTBITS
 * @sa ga_reg_fontbits
 * @ingroup ga_reg_sub_39
 */
#define GA_REG_FONTBITS 0xFF804E

/**
 * @def GA_REG_FONTDATA
 * @sa ga_reg_fontdata
 * @ingroup ga_reg_sub_40
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
 * @ingroup ga_reg_sub_44
 */
#define GA_REG_STAMPSIZE 0xFF8058

#define GA_STAMPSIZE_REPEAT_POS 0
#define GA_STAMPSIZE_REPEAT_WIDTH 1
#define GA_STAMPSIZE_32x32_STAMP_POS 1
#define GA_STAMPSIZE_32x32_STAMP_WIDTH 1
#define GA_STAMPSIZE_16x16_SCREEN_POS 2
#define GA_STAMPSIZE_16x16_SCREEN_WIDTH 1

#define GA_STAMPSIZE_REPEAT_MASK FIELD_MASK(GA_STAMPSIZE_REPEAT_POS, GA_STAMPSIZE_REPEAT_WIDTH)
#define GA_STAMPSIZE_32x32_STAMP_MASK FIELD_MASK(GA_STAMPSIZE_32x32_STAMP_POS, GA_STAMPSIZE_32x32_STAMP_WIDTH)
#define GA_STAMPSIZE_16x16_SCREEN_MASK FIELD_MASK(GA_STAMPSIZE_16x16_SCREEN_POS, GA_STAMPSIZE_16x16_SCREEN_WIDTH)

/**
 * @def GA_REG_STAMPMAPBASE
 * @sa ga_reg_stampmapbase
 * @note The location of the stamp map in Word RAM. The value you put in is the
 * offset relative to the start of Word RAM divided by 4. What the raw location
 * needs to be a multiple of depends on the sizes you have set:

    16x16px stamps, 256x256px stamp map: Multiples of 0x200
    32x32px stamps, 256x256px stamp map: Multiples of 0x80
    16x16px stamps, 4096x4096px stamp map: Multiples of 0x20000
    32x32px stamps, 4096x4096px stamp map: Multiples of 0x8000
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_45
 */
#define GA_REG_STAMPMAPBASE 0xFF805A

/**
 * @def GA_REG_IMGBUFVSIZE
 * @sa ga_reg_imgbufvsize
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_46
 */
#define GA_REG_IMGBUFVSIZE 0xFF805C

/**
 * @def GA_REG_IMGBUFSTART
 * @sa ga_reg_imgbufstart
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_47
 */
#define GA_REG_IMGBUFSTART 0xFF805E

/**
 * @def GA_REG_IMGBUFOFFSET
 * @sa ga_reg_imgbufoffset
 * @ingroup ga_reg_sub_48
 */
#define GA_REG_IMGBUFOFFSET 0xFF8060

/**
 * @def GA_REG_IMGBUFHDOTSIZE
 * @sa ga_reg_imgbufhdotsize
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_49
 */
#define GA_REG_IMGBUFHDOTSIZE 0xFF8062

/**
 * @def GA_REG_IMGBUFVDOTSIZE
 * @sa ga_reg_imgbufvdotsize
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_50
 */
#define GA_REG_IMGBUFVDOTSIZE 0xFF8064

/**
 * @def GA_REG_TRACEVECTBASE
 * @sa ga_reg_tracevectbase
 * @warning Word access only. A byte access to this register can raise a
 * bus error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_51
 */
#define GA_REG_TRACEVECTBASE 0xFF8066

/**
 * @def GA_REG_SUBCODEADDR
 * @sa ga_reg_subcodeaddr
 * @ingroup ga_reg_sub_52
 */
#define GA_REG_SUBCODEADDR 0xFF8068

/**
 * @def GA_REG_SUBCODEBUF
 * @sa ga_reg_subcodebuf
 * @ingroup ga_reg_sub_128
*/
#define GA_REG_SUBCODEBUF 0xFF8100

/**
 * @def GA_REG_SUBCODEBUFIMG
 * @sa ga_reg_subcodebufimg
 * @ingroup ga_reg_sub_192
*/
#define GA_REG_SUBCODEBUFIMG 0xFF8180

/**
 * ga_reg_cdcmode bit/mask settings
 */
#define GA_CDCMODE_DD0_POS 5
#define GA_CDCMODE_DD0_WIDTH 1
#define GA_CDCMODE_DSR_POS 6
#define GA_CDCMODE_DSR_WIDTH 1
#define GA_CDCMODE_EDT_POS 7
#define GA_CDCMODE_EDT_WIDTH 1

// these aren't right...
// TODO clean these up
#define GA_CDCMODE_MAINREAD_POS 2
#define GA_CDCMODE_MAINREAD_WIDTH 1
#define GA_CDCMODE_SUBREAD_POS 3
#define GA_CDCMODE_SUBREAD_WIDTH 1
#define GA_CDCMODE_PCMDMA_POS 4
#define GA_CDCMODE_PCMDMA_WIDTH 1
#define GA_CDCMODE_PRAMDMA_POS 5
#define GA_CDCMODE_PRAMDMA_WIDTH 1
#define GA_CDCMODE_WRAMDMA_POS 7
#define GA_CDCMODE_WRAMDMA_WIDTH 1

#define CDCMODE_CABITS           0x000F
#define CDCMODE_DDBITS           0x0700
#define GA_CDCMODE_DD0_MASK FIELD_MASK(GA_CDCMODE_DD0_POS, GA_CDCMODE_DD0_WIDTH)
#define GA_CDCMODE_DSR_MASK FIELD_MASK(GA_CDCMODE_DSR_POS, GA_CDCMODE_DSR_WIDTH)
#define GA_CDCMODE_EDT_MASK FIELD_MASK(GA_CDCMODE_EDT_POS, GA_CDCMODE_EDT_WIDTH)
#define GA_CDCMODE_MAINREAD_MASK FIELD_MASK(GA_CDCMODE_MAINREAD_POS, GA_CDCMODE_MAINREAD_WIDTH)
#define GA_CDCMODE_SUBREAD_MASK FIELD_MASK(GA_CDCMODE_SUBREAD_POS, GA_CDCMODE_SUBREAD_WIDTH)
#define GA_CDCMODE_PCMDMA_MASK FIELD_MASK(GA_CDCMODE_PCMDMA_POS, GA_CDCMODE_PCMDMA_WIDTH)
#define GA_CDCMODE_PRAMDMA_MASK FIELD_MASK(GA_CDCMODE_PRAMDMA_POS, GA_CDCMODE_PRAMDMA_WIDTH)
#define GA_CDCMODE_WRAMDMA_MASK FIELD_MASK(GA_CDCMODE_WRAMDMA_POS, GA_CDCMODE_WRAMDMA_WIDTH)

#define CDC_DEST_MAINREAD 2
#define CDC_DEST_SUBREAD  3
#define CDC_DEST_PCMDMA   4
#define CDC_DEST_PRAMDMA  5
#define CDC_DEST_WRAMDMA  7

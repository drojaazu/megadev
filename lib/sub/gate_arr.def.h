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
 * @defgroup ga_reg_sub_28 Sub CPU / Gate Array / Register 28 (0xFF8038) - CDD Receiving Status 0 and 1
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_29 Sub CPU / Gate Array / Register 29 (0xFF803A) - CDD Receiving Status 2 and 3
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_30 Sub CPU / Gate Array / Register 30 (0xFF803C) - CDD Receiving Status 4 and 5
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_31 Sub CPU / Gate Array / Register 31 (0xFF803E) - CDD Receiving Status 6 and 7
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_32 Sub CPU / Gate Array / Register 32 (0xFF8040) - CDD Receiving Status 8 and 9
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_33 Sub CPU / Gate Array / Register 33 (0xFF8042) - CDD Transmission Commands 0 and 1
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_34 Sub CPU / Gate Array / Register 34 (0xFF8044) - CDD Transmission Commands 2 and 3
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_35 Sub CPU / Gate Array / Register 35 (0xFF8046) - CDD Transmission Commands 4 and 5
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_36 Sub CPU / Gate Array / Register 36 (0xFF8048) - CDD Transmission Commands 6 and 7
 * @ingroup ga_regs_sub
 */

/**
 * @defgroup ga_reg_sub_37 Sub CPU / Gate Array / Register 37 (0xFF804A) - CDD Transmission Commands 8 and 9
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
 * @def GA_REG_LED
 * @brief Drive state LEDs
 *
 * @details
 * The high byte of the hardware register at 0xFF8000. Split out as its own byte
 * register because it shares nothing with the low half beyond an address: see
 * SPEC.md D17.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | | | | | | | |\b LEDG|\b LEDR|
 * |\b R| | | | | | |◯|◯|
 * |\b W| | | | | | |◯|◯|
 *
 * @param LEDR Red LED. 1: on, 0: off. Lit during disc access.
 * @param LEDG Green LED. 1: on, 0: off. Lit when the drive is ready.
 *
 * The pair is read as a single drive state indicator, and the BIOS drives it:
 *
 * | Green | Red | Meaning |
 * |---|---|---|
 * | on | blinking | Ready. No disc present, or the TOC is still being read |
 * | on | off | Ready. A disc is present and readable |
 * | on | on | Disc access in progress |
 * | blinking | off | Standby |
 *
 * Power-on and reset clear both LEDs. Any other combination requires a special
 * system mode.
 *
 * @sa ga_reg_led
 * @ingroup ga_reg_sub_00
 */
#define GA_REG_LED 0xFF8000

/**
 * @def GA_REG_SUBCTRL
 * @brief Peripheral reset and gate array version
 *
 * @details
 * The low byte of the hardware register at 0xFF8000.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b VER3|\b VER2|\b VER1|\b VER0| | | |\b RES0|
 * |\b R|◯|◯|◯|◯| | | |◯|
 * |\b W| | | | | | | |◯|
 *
 * @param RES0 Peripheral reset.
 * \n [write] 0: reset the peripheral. Writing 1 is not used.
 * \n [read] 0: the peripheral is being reset / 1: the peripheral is operable
 * \n Roughly 100 ms after a reset the peripheral becomes operable and RES0
 * becomes 1 of its own accord.
 * @param VER0-3 Gate array chip version. Read only.
 *
 * @sa ga_reg_subctrl
 * @ingroup ga_reg_sub_00
 */
#define GA_REG_SUBCTRL 0xFF8001


/**
 * @def GA_LED_R_MASK
 * @brief Red LED Control
 * @details
 * Drives the red element of the LED on the front of the unit. Free for
 * application use; the Sub BIOS does not touch it after boot.
 * @sa ga_reg_led
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_subctrl
 */
#define GA_LED_R_POS 0
#define GA_LED_R_WIDTH 1
#define GA_LED_R_MASK FIELD_MASK(GA_LED_R_POS, GA_LED_R_WIDTH)

/**
 * @def GA_LED_G_MASK
 * @brief Green LED Control
 * @details
 * Drives the green element of the LED on the front of the unit. Free for
 * application use; the Sub BIOS does not touch it after boot.
 * @sa ga_reg_led
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_subctrl
 */
#define GA_LED_G_POS 1
#define GA_LED_G_WIDTH 1
#define GA_LED_G_MASK FIELD_MASK(GA_LED_G_POS, GA_LED_G_WIDTH)

/**
 * @def GA_VERSION_MASK
 * @brief Gate array chip version
 * @sa ga_reg_subctrl
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_subctrl
 */
#define GA_VERSION_POS 4
#define GA_VERSION_WIDTH 4
#define GA_VERSION_MASK FIELD_MASK(GA_VERSION_POS, GA_VERSION_WIDTH)

/**
 * @def GA_PERIPH_RESET_MASK
 * @brief Peripheral reset
 * @details Write 0 to reset the peripheral; reads 1 once it is operable again.
 * @warning This is bit 0 of GA_REG_SUBCTRL, not of GA_REG_LED. The two were a
 * single 16 bit register until D17, and a stray bit 0 write through the old
 * word accessor reset the peripheral instead of touching an LED (KB-34).
 * @sa ga_reg_subctrl
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_subctrl
 */
#define GA_PERIPH_RESET_POS 0
#define GA_PERIPH_RESET_WIDTH 1
#define GA_PERIPH_RESET_MASK FIELD_MASK(GA_PERIPH_RESET_POS, GA_PERIPH_RESET_WIDTH)

/**
 * @defgroup ga_reg_sub_memmode Sub CPU / Gate Array / Registers / Memory Mode
 */

/**
 * @def GA_REG_WP
 * @brief Program RAM write protection
 *
 * @details
 * The high byte of the hardware register at 0xFF8002. Protects the bottom of
 * Program RAM against writes in units of 512 bytes: each set bit covers one
 * block, so the field as a whole spans 0x000000 to 0x01FDFF. This is how the
 * Sub CPU's resident code is shielded from a stray write by the Main side.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b WP7|\b WP6|\b WP5|\b WP4|\b WP3|\b WP2|\b WP1|\b WP0|
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | | | | | |
 *
 * @param WP0-7 Write protect block enables.
 * @note Read only from the Sub CPU side. The Main CPU sets the protection; see
 * the Main CPU header for the writable view.
 * @sa ga_reg_wp
 * @ingroup ga_reg_sub_01
 */
#define GA_REG_WP 0xFF8002

/**
 * @def GA_REG_MEMMODE
 * @brief Word RAM ownership, layout and priority mode
 *
 * @details
 * The low byte of the hardware register at 0xFF8002. Split from the write
 * protect byte above it because the two share nothing but an address, and a
 * word-wide write to set the memory mode would silently clear the protection:
 * see SPEC.md D17.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | | | | |\b PM1|\b PM0|\b MODE|\b DMNA|\b RET|
 * |\b R| | | |◯|◯|◯|◯|◯|
 * |\b W| | | |◯|◯|◯| |◯|
 *
 * @param PM0-1 Priority mode, applied when the Sub CPU writes into Word RAM.
 * See GA_PRIORITY_MASK.
 * @param MODE Word RAM layout. 0: 2M, 1: 1M.
 * @param DMNA Declaration of Main RAM No Access.
 * \n In 2M mode, writing 1 returns Word RAM to the Sub CPU. Reading 0 means it
 * has not been returned yet; 1 means it has.
 * \n In 1M mode, reading 1 means the Main CPU has requested a bank swap, and
 * reading 0 means the swap is complete. Setting RET also sets DMNA.
 * @param RET The counterpart to DMNA.
 * \n In 2M mode, writing 1 gives Word RAM to the Main CPU. Reading 0 means it
 * has not been given up yet; 1 means it has.
 *
 * @note DMNA and RET are the two halves of Word RAM handover: each declares the
 * transfer in one direction, and the bit you write is not the bit you poll to
 * confirm it.
 * @note DMNA is read only from this side -- the Main CPU sets it. Writing it
 * here has no effect.
 *
 * @sa ga_reg_memmode
 * @ingroup ga_reg_sub_01
 */
#define GA_REG_MEMMODE 0xFF8003


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
 * @def GA_PRIORITY_MASK
 * @brief Word RAM priority mode
 * @details
 * Governs which pixels a Sub CPU write into Word RAM is allowed to change.
 * Values are stored unshifted; place them with FIELD_PREP.
 *
 * | Value | Effect |
 * |:|:|
 * | GA_PRIORITY_OFF | Every pixel is written |
 * | GA_PRIORITY_UNDERWRITE | Written only where the buffer already holds 0 |
 * | GA_PRIORITY_OVERWRITE | Only non-zero source pixels are written |
 *
 * @warning The fourth value is prohibited by the hardware documentation and has
 * no defined behaviour.
 * @sa ga_reg_memmode
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_memmode
 */
#define GA_PRIORITY_POS 3
#define GA_PRIORITY_WIDTH 2
#define GA_PRIORITY_MASK FIELD_MASK(GA_PRIORITY_POS, GA_PRIORITY_WIDTH)

#define GA_PRIORITY_OFF 0b00
#define GA_PRIORITY_UNDERWRITE 0b01
#define GA_PRIORITY_OVERWRITE 0b10

/**
 * @def GA_WP_MASK
 * @brief Program RAM write protect blocks
 * @sa ga_reg_wp
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_memmode
 */
#define GA_WP_POS 0
#define GA_WP_WIDTH 8
#define GA_WP_MASK FIELD_MASK(GA_WP_POS, GA_WP_WIDTH)

/**
 * @defgroup ga_reg_sub_cdcmode Sub CPU / Gate Array / Registers / CDC Mode &
 * CDC Registers #1
 */

/**
 * @def GA_REG_CDCMODE
 * @brief CDC mode / CDC register address
 *
 * @details
 * Selects which internal CDC register `GA_REG_CDCRS1` reads and writes, sets
 * where the CDC sends the data it recovers from the disc, and reports the
 * progress of that transfer.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b EDT|\b DSR|\b UBR| | |\b DD2|\b DD1|\b DD0| | | | |\b CA3|\b CA2|\b CA1|\b CA0|
 * |\b R|◯|◯| | | |◯|◯|◯| | | | |◯|◯|◯|◯|
 * |\b W| | | | | |◯|◯|◯| | | | |◯|◯|◯|◯|
 *
 * @param CA CDC register address
 * @details RW: Selects the CDC register exposed through GA_REG_CDCRS1. The
 * registers themselves are documented in the LC89510 manual, not here.
 * @param DD Device destination
 * @details RW: Where the CDC sends its data. See GA_CDCMODE_DD_MASK.
 * @param UBR Upper byte ready
 * @details R: The CDC has sent the high order byte. Cleared when the low order
 * byte follows.
 * @param DSR Data set ready
 * @details R: The CDC has sent the low order byte, so a full word is waiting in
 * GA_REG_CDCHOSTDATA. Cleared once that word has been read or transferred.
 * \n Under PCM DMA the unit is a byte rather than a word: the flag sets when
 * the gate array takes one byte from the CDC and clears when it hands that byte
 * to the PCM chip.
 * @param EDT End of data transfer
 * @details R: Every byte has been transferred out of the CDC. Cleared when the
 * device destination is written, set on completion, and cleared again when a
 * further transfer begins.
 *
 * @note Writing the device destination resets the DMA circuit, GA_REG_DMAADDR
 * included. After any reset other than power-on the destination bits keep their
 * previous value but the transfer circuit may be unstable, so set them again
 * rather than assuming they survived.
 * @warning Of the bit operations, only BTST is permitted.
 * @ingroup ga_reg_sub_02
 */
#define GA_REG_CDCMODE 0xFF8004

/**
 * @def GA_CDCMODE_CA_MASK
 * @brief CDC register address
 * @sa GA_REG_CDCMODE
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_02
 */
#define GA_CDCMODE_CA_POS 0
#define GA_CDCMODE_CA_WIDTH 4
#define GA_CDCMODE_CA_MASK FIELD_MASK(GA_CDCMODE_CA_POS, GA_CDCMODE_CA_WIDTH)

/**
 * @def GA_CDCMODE_DD_MASK
 * @brief CDC device destination
 * @details
 * Where the CDC sends the data it reads. Values are stored unshifted; place
 * them with FIELD_PREP.
 *
 * | Value | 2M mode | 1M mode |
 * |:|:|:|
 * | GA_CDCMODE_DD_MAINREAD | Main CPU read | Main CPU read |
 * | GA_CDCMODE_DD_SUBREAD | Sub CPU read | Sub CPU read |
 * | GA_CDCMODE_DD_PCMDMA | PCM sound source, by DMA | PCM sound source, by DMA |
 * | GA_CDCMODE_DD_PRGDMA | Program RAM, by DMA | Program RAM, by DMA |
 * | GA_CDCMODE_DD_WRAMDMA | Sub CPU side 2M Word RAM, by DMA | Sub CPU side 1M Word RAM, by DMA |
 *
 * The three remaining values (0, 1 and 6) are documented as "do not set" and
 * have no defined behaviour.
 * @warning The DMA destinations apply only while the target memory is attached
 * to the Sub CPU. DMA cannot be performed into memory attached to the Main CPU.
 * @sa GA_REG_CDCMODE
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_02
 */
#define GA_CDCMODE_DD_POS 8
#define GA_CDCMODE_DD_WIDTH 3
#define GA_CDCMODE_DD_MASK FIELD_MASK(GA_CDCMODE_DD_POS, GA_CDCMODE_DD_WIDTH)

#define GA_CDCMODE_DD_MAINREAD 0b010
#define GA_CDCMODE_DD_SUBREAD 0b011
#define GA_CDCMODE_DD_PCMDMA 0b100
#define GA_CDCMODE_DD_PRGDMA 0b101
#define GA_CDCMODE_DD_WRAMDMA 0b111

/**
 * @def GA_CDCMODE_UBR_MASK
 * @brief Upper byte ready
 * @sa GA_REG_CDCMODE
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_02
 */
#define GA_CDCMODE_UBR_POS 13
#define GA_CDCMODE_UBR_WIDTH 1
#define GA_CDCMODE_UBR_MASK FIELD_MASK(GA_CDCMODE_UBR_POS, GA_CDCMODE_UBR_WIDTH)

/**
 * @def GA_CDCMODE_DSR_MASK
 * @brief Data set ready
 * @note In the high byte of the register, so use FIELD_BYTE and FIELD_BPOS to
 * reach it with BTST.
 * @sa GA_REG_CDCMODE
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_02
 */
#define GA_CDCMODE_DSR_POS 14
#define GA_CDCMODE_DSR_WIDTH 1
#define GA_CDCMODE_DSR_MASK FIELD_MASK(GA_CDCMODE_DSR_POS, GA_CDCMODE_DSR_WIDTH)

/**
 * @def GA_CDCMODE_EDT_MASK
 * @brief End of data transfer
 * @note In the high byte of the register, so use FIELD_BYTE and FIELD_BPOS to
 * reach it with BTST.
 * @sa GA_REG_CDCMODE
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_02
 */
#define GA_CDCMODE_EDT_POS 15
#define GA_CDCMODE_EDT_WIDTH 1
#define GA_CDCMODE_EDT_MASK FIELD_MASK(GA_CDCMODE_EDT_POS, GA_CDCMODE_EDT_WIDTH)

/**
 * @defgroup ga_reg_sub_cdcregs2 Sub CPU / Gate Array / Registers / CDC
 * Registers #2
 */

/**
 * @def GA_REG_CDCRS1
 * @brief CDC register data
 *
 * @details
 * The data port for whichever internal CDC register GA_CDCMODE_CA_MASK
 * currently selects. The registers behind it belong to the LC89510 and are
 * documented in its manual, not here.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | | | |\b CD7|\b CD6|\b CD5|\b CD4|\b CD3|\b CD2|\b CD1|\b CD0|
 * |\b R| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param CD CDC register data
 * @details RW: The selected CDC register.
 * @warning No bit or read-modify-write instruction may be used on this
 * register -- not BTST, BSET, BCLR, and not CLR either. Read the whole value,
 * modify the copy, and write it back.
 * @ingroup ga_reg_sub_03
 */
#define GA_REG_CDCRS1 0xFF8006

/**
 * @def GA_CDCRS1_CD_MASK
 * @brief Selected CDC register data
 * @sa GA_REG_CDCRS1
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_03
 */
#define GA_CDCRS1_CD_POS 0
#define GA_CDCRS1_CD_WIDTH 8
#define GA_CDCRS1_CD_MASK FIELD_MASK(GA_CDCRS1_CD_POS, GA_CDCRS1_CD_WIDTH)

/**
 * @defgroup ga_reg_sub_cdcdata Sub CPU / Gate Array / Registers / CDC Host Data
 */

/**
 * @def GA_REG_CDCHOSTDATA
 * @brief CDC host data
 *
 * @details
 * The CDC accumulates two bytes here and the word is then read by the Main or
 * Sub CPU. Reading it releases the register, and the CDC loads the next two
 * bytes into it.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b HD|||||||||||||||
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|
 *
 * @param HD CDC read data
 * @details R: Two bytes read from the disc, ready to be moved into Main or Sub
 * CPU memory.
 *
 * @note Only meaningful once GA_CDCMODE_DSR_MASK is set. Reading it before then
 * yields whatever the previous transfer left behind.
 * @warning Read only, and word access only. A byte access can raise a bus
 * error, and there is nothing to write.
 * @warning Bit operation instructions are not permitted here.
 * @ingroup ga_reg_sub_04
 */
#define GA_REG_CDCHOSTDATA 0xFF8008

/**
 * @defgroup ga_reg_sub_dmaaddr Sub CPU / Gate Array / Registers / CDC DMA
 * Address
 */

/**
 * @def GA_REG_DMAADDR
 * @brief CDC DMA address
 *
 * @details
 * The destination address for a CDC DMA transfer. The register holds address
 * bits 3 through 18, so the address it describes is the register value shifted
 * left by three -- transfers are therefore aligned to 8 byte boundaries and the
 * bottom three address bits cannot be expressed at all.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b A18|\b A17|\b A16|\b A15|\b A14|\b A13|\b A12|\b A11|\b A10|\b A09|\b A08|\b A07|\b A06|\b A05|\b A04|\b A03|
 * |\b R|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|
 * |\b W|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param A DMA destination address, bits 3-18
 * @details W: How much of the field is usable depends on the destination
 * selected by GA_CDCMODE_DD_MASK, because each target is a different size:
 * - PCM sound source: up to A12; A13 through A18 must be written 0
 * - 1M mode Word RAM (either half): up to A16; A17 and A18 must be written 0
 * - 2M mode Word RAM: up to A17; A18 must be written 0
 * - Program RAM: the whole field is usable
 *
 * @note Reset whenever the device destination in GA_REG_CDCMODE is written, so
 * set the destination first and the address second.
 * @warning Write only, and word access only. A byte access can raise a bus
 * error, and the register does not read back -- keep your own copy if you need
 * to know what it holds.
 * @warning Bit operation instructions are not permitted here.
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
 * A free-running 12 bit counter, primarily used for CDD and CDC timing but
 * general purpose otherwise. One tick is 30.72 microseconds, so it counts 0 to
 * 4095 and then wraps to 0 and keeps going.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | |\b SW11|\b SW10|\b SW09|\b SW08|\b SW07|\b SW06|\b SW05|\b SW04|\b SW03|\b SW02|\b SW01|\b SW00|
 * |\b R| | | | |◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | |◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param SW Counter value.
 * @details R: the current count / W: **write 0 only**; any write restarts the
 * counter from 0, whatever value is written.
 * @warning Word access only. A byte access to this register can raise a bus
 * error.
 * @warning Bit operation instructions are not permitted here.
 * @ingroup ga_reg_sub_06
 */
#define GA_REG_STOPWATCH 0xFF800C

/**
 * @defgroup ga_reg_sub_cpucomm Sub CPU / Gate Array / Registers / CPU
 * Communication
 */

/**
 * @def GA_REG_COMFLAGS_MAIN
 * @brief Communication flags written by the Main CPU
 *
 * @details
 * The high byte of the hardware register at 0xFF800E. Eight flags the Main CPU
 * sets and the other side polls. The hardware assigns no meaning to any of
 * them; they are yours to define.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b CFM7|\b CFM6|\b CFM5|\b CFM4|\b CFM3|\b CFM2|\b CFM1|\b CFM0|
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | | | | | |
 *
 * @note Read only from this CPU -- the other side owns these flags.
 * @warning **Test one bit at a time.** If both CPUs read and write the flags at
 * the same moment the write lands correctly but the read may return stale data.
 * A single-bit test is immune to this; a byte or word read of the pair is not,
 * which is the reason the two halves are separate registers here.
 * @sa ga_reg_comflags_main
 * @ingroup ga_reg_sub_07
 */
#define GA_REG_COMFLAGS_MAIN 0xFF800E

/**
 * @def GA_REG_COMFLAGS_SUB
 * @brief Communication flags written by the Sub CPU
 *
 * @details
 * The low byte of the hardware register at 0xFF800E. Eight flags the Sub CPU
 * sets and the other side polls. The hardware assigns no meaning to any of
 * them; they are yours to define.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b CFS7|\b CFS6|\b CFS5|\b CFS4|\b CFS3|\b CFS2|\b CFS1|\b CFS0|
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @note Read/write from this CPU.
 * @warning **Test one bit at a time.** If both CPUs read and write the flags at
 * the same moment the write lands correctly but the read may return stale data.
 * A single-bit test is immune to this; a byte or word read of the pair is not,
 * which is the reason the two halves are separate registers here.
 * @sa ga_reg_comflags_sub
 * @ingroup ga_reg_sub_07
 */
#define GA_REG_COMFLAGS_SUB (0xFF800E + 1)

/**
 * @def GA_REG_COMCMD0
 * @brief Comm Command 0 (Main -> Sub)
 *
 * @details R: 16 bit data. **Read only from this side** -- the Main CPU writes it.
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
 * @brief General purpose timer, drives INT3
 *
 * @details
 * The low byte of the hardware register at 0xFF8030; the high byte is unused
 * and reads as 0. Writing n starts a countdown from n to 0 at one tick per
 * 30.72 microseconds, and level 3 is raised when it reaches 0. The timer then
 * reloads and repeats, so this is a periodic interrupt rather than a one shot.
 *
 * The resulting interval is (n + 1) x 30.72 microseconds.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b TD7|\b TD6|\b TD5|\b TD4|\b TD3|\b TD2|\b TD1|\b TD0|
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param TD Timer period, 1 to 255.
 * @note Writing 0 disables the timer; INT3 is not generated. 0 is also the
 * reset value, so the timer is off until it is set.
 * @warning **A read returns the value you last wrote, not the live count.**
 * There is no way to read how far the countdown has progressed.
 * @note Level 3 must also be enabled in GA_REG_INTMASK before anything is
 * delivered.
 * @sa ga_reg_int3timer, GA_INT3_MASK
 * @ingroup ga_reg_sub_24
 */
#define GA_REG_INT3TIMER 0xFF8031

/**
 * @def GA_REG_INTMASK
 * @brief Interrupt enable mask
 *
 * @details
 * The low byte of the hardware register at 0xFF8032; the high byte is unused
 * and reads as 0. One enable bit per interrupt level: 0 disables, 1 enables.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | | |\b IEN6|\b IEN5|\b IEN4|\b IEN3|\b IEN2|\b IEN1| |
 * |\b R| |◯|◯|◯|◯|◯|◯| |
 * |\b W| |◯|◯|◯|◯|◯|◯| |
 *
 * What each level signals:
 *
 * | Level | Source | Raised when |
 * |---|---|---|
 * | 1 | Graphics | A graphics operation finishes, in 2M mode |
 * | 2 | Mega Drive | The Main CPU raises a software interrupt |
 * | 3 | Timer | GA_REG_INT3TIMER counts down to 0 |
 * | 4 | CDD | Reception of status 7 completes |
 * | 5 | CDC | Errors are corrected, or buffering completes |
 * | 6 | Sub-code | A sub-code buffer fills |
 *
 * @note Bit 0 is unused; there is no level 0.
 * @note For proper synchronisation, the Main CPU should raise level 2 from its
 * vertical interrupt.
 * @sa ga_reg_intmask
 * @ingroup ga_reg_sub_25
 */
#define GA_REG_INTMASK 0xFF8033

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
 * @brief CD audio fader and de-emphasis
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS fader calls instead.
 * It is defined here for completeness and for the BIOS wrappers, not as an
 * interface to reach for.
 *
 * Volume is expressed as a *decay depth* rather than a level, and the hardware
 * ramps to it rather than jumping: the ramp takes 22 microseconds per unit of
 * depth. Depth is minimum (loudest) at `%10000000000` and maximum (silent) at
 * `%00000000000`. From `%00000000100` upward the attenuation in decibels is
 * -20 log(FD10..FD02 / 256), with the bottom two bits not used arithmetically.
 *
 * Fields:
 * - **FD00-10** — fade volume data, at bits 4 through 14.
 * - **EFDT** — end of fade data transfer. 1: busy, the previous value is still
 *   being shifted out to the fader; 0: ready, a new value may be set.
 * - **DEF0-1** — de-emphasis, for the pre-emphasis used by some early classical
 *   CDs. Normally off.
 *
 * | DEF1 | DEF0 | De-emphasis |
 * |---|---|---|
 * | 0 | 0 | Off |
 * | 0 | 1 | Fs = 44.1 kHz |
 * | 1 | 0 | Fs = 32 kHz |
 * | 1 | 1 | Fs = 48 kHz |
 *
 * @note Only the position of FD00-10 is transcribed as fact; the manual states
 * it in words. The bit table on that page is obscured in the scan, so the exact
 * positions of EFDT and DEF0-1 are **not** recorded here rather than guessed.
 * See SPEC.md section 7.
 * @warning Word access only. A byte access to this register can raise a bus
 * error.
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @sa ga_reg_cdfader
 * @ingroup ga_reg_sub_26
 */
#define GA_REG_CDFADER 0xFF8034

/**
 * @def GA_CDFADER_FD_MASK
 * @brief Fade volume data (decay depth)
 * @details Larger is quieter. See GA_REG_CDFADER for the scale and the ramp
 * timing.
 * @sa ga_reg_cdfader
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_26
 */
#define GA_CDFADER_FD_POS 4
#define GA_CDFADER_FD_WIDTH 11
#define GA_CDFADER_FD_MASK FIELD_MASK(GA_CDFADER_FD_POS, GA_CDFADER_FD_WIDTH)

/**
 * @def GA_REG_CDDCTRL
 * @brief CDD communication control
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS drive calls instead.
 *
 * Drives the serial link to the CD drive controller. Communication is started
 * once, by taking HOCK from 0 to 1; the two status bits then report which
 * direction a transfer is currently running in.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | | |\b D/M| | | | | |\b HOCK|\b DRS|\b DTS|
 * |\b R| | | | | | | |◯| | | | | |◯|◯|◯|
 * |\b W| | | | | | | | | | | | | |◯|◯|◯|
 *
 * @param DTS Data transmission status.
 * \n [read] 1: the communication buffer is being sent to the CDD
 * \n [write] **0 only**, which aborts the transfer in progress
 * @param DRS Data receiving status.
 * \n [read] 1: the CDD is sending into the communication buffer
 * \n [write] **0 only**, which aborts the transfer in progress
 * @param HOCK Host clock. 0 after power on; taking it 0 to 1 starts
 * communication with the CDD.
 * @param D/M Data or music. Read only. 1: the current CDD data is ROM data,
 * which includes the stop and pause states. 0: it is audio.
 *
 * @note A communication error aborts the transfer in progress within 240
 * microseconds, so DTS and DRS may clear without the transfer completing.
 * @warning BSET and BCLR may not be used on this register.
 * @note Positions confirmed against the clean SPHERE scan of printed page 32;
 * the fax copy is illegible at exactly these columns.
 * @sa ga_reg_cddctrl
 * @ingroup ga_reg_sub_27
 */
#define GA_REG_CDDCTRL 0xFF8036

/**
 * @def GA_CDDCTRL_DTS_MASK
 * @brief Data transmission status
 * @sa GA_REG_CDDCTRL
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_27
 */
#define GA_CDDCTRL_DTS_POS 0
#define GA_CDDCTRL_DTS_WIDTH 1
#define GA_CDDCTRL_DTS_MASK FIELD_MASK(GA_CDDCTRL_DTS_POS, GA_CDDCTRL_DTS_WIDTH)

/**
 * @def GA_CDDCTRL_DRS_MASK
 * @brief Data receiving status
 * @sa GA_REG_CDDCTRL
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_27
 */
#define GA_CDDCTRL_DRS_POS 1
#define GA_CDDCTRL_DRS_WIDTH 1
#define GA_CDDCTRL_DRS_MASK FIELD_MASK(GA_CDDCTRL_DRS_POS, GA_CDDCTRL_DRS_WIDTH)

/**
 * @def GA_CDDCTRL_HOCK_MASK
 * @brief Host clock; 0 to 1 starts CDD communication
 * @sa GA_REG_CDDCTRL
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_27
 */
#define GA_CDDCTRL_HOCK_POS 2
#define GA_CDDCTRL_HOCK_WIDTH 1
#define GA_CDDCTRL_HOCK_MASK FIELD_MASK(GA_CDDCTRL_HOCK_POS, GA_CDDCTRL_HOCK_WIDTH)

/**
 * @def GA_CDDCTRL_DM_MASK
 * @brief Data or music; read only
 * @note In the high byte, so use FIELD_BYTE and FIELD_BPOS to reach it with a
 * bit opcode.
 * @sa GA_REG_CDDCTRL
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_27
 */
#define GA_CDDCTRL_DM_POS 8
#define GA_CDDCTRL_DM_WIDTH 1
#define GA_CDDCTRL_DM_MASK FIELD_MASK(GA_CDDCTRL_DM_POS, GA_CDDCTRL_DM_WIDTH)

/**
 * @def GA_REG_CDDSTAT0
 * @brief CDD receiving status 0 and 1
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS drive calls.
 *
 * Two of the ten status nibbles the CDD returns. Read only in practice: the
 * drive fills them and the Sub CPU consumes them.
 *
 * The ten registers from 0xFF8038 to 0xFF804A carry twenty 4 bit values, two
 * per register: one in bits 11-8 and one in bits 3-0. The remaining nibbles
 * read as 0.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |0|0|0|0|\b RS0||||0|0|0|0|\b RS1||||
 *
 * @param RS0 Receiving Status 0, in bits 11-8.
 * @param RS1 Receiving Status 1, in bits 3-0.
 * @sa ga_reg_cddstat0, GA_CDDCOMM_HI_MASK, GA_CDDCOMM_LO_MASK
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_28
 */
#define GA_REG_CDDSTAT0 0xFF8038

/**
 * @def GA_REG_CDDSTAT1
 * @brief CDD receiving status 2 and 3
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS drive calls.
 *
 * Two of the ten status nibbles the CDD returns. Read only in practice: the
 * drive fills them and the Sub CPU consumes them.
 *
 * The ten registers from 0xFF8038 to 0xFF804A carry twenty 4 bit values, two
 * per register: one in bits 11-8 and one in bits 3-0. The remaining nibbles
 * read as 0.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |0|0|0|0|\b RS2||||0|0|0|0|\b RS3||||
 *
 * @param RS2 Receiving Status 2, in bits 11-8.
 * @param RS3 Receiving Status 3, in bits 3-0.
 * @sa ga_reg_cddstat1, GA_CDDCOMM_HI_MASK, GA_CDDCOMM_LO_MASK
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_29
 */
#define GA_REG_CDDSTAT1 0xFF803A

/**
 * @def GA_REG_CDDSTAT2
 * @brief CDD receiving status 4 and 5
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS drive calls.
 *
 * Two of the ten status nibbles the CDD returns. Read only in practice: the
 * drive fills them and the Sub CPU consumes them.
 *
 * The ten registers from 0xFF8038 to 0xFF804A carry twenty 4 bit values, two
 * per register: one in bits 11-8 and one in bits 3-0. The remaining nibbles
 * read as 0.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |0|0|0|0|\b RS4||||0|0|0|0|\b RS5||||
 *
 * @param RS4 Receiving Status 4, in bits 11-8.
 * @param RS5 Receiving Status 5, in bits 3-0.
 * @sa ga_reg_cddstat2, GA_CDDCOMM_HI_MASK, GA_CDDCOMM_LO_MASK
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_30
 */
#define GA_REG_CDDSTAT2 0xFF803C

/**
 * @def GA_REG_CDDSTAT3
 * @brief CDD receiving status 6 and 7
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS drive calls.
 *
 * Two of the ten status nibbles the CDD returns. Read only in practice: the
 * drive fills them and the Sub CPU consumes them.
 *
 * The ten registers from 0xFF8038 to 0xFF804A carry twenty 4 bit values, two
 * per register: one in bits 11-8 and one in bits 3-0. The remaining nibbles
 * read as 0.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |0|0|0|0|\b RS6||||0|0|0|0|\b RS7||||
 *
 * @param RS6 Receiving Status 6, in bits 11-8.
 * @param RS7 Receiving Status 7, in bits 3-0.
 * @note Reception of **Receiving Status 7** raises INT4, so that is the nibble
 * to treat as the "status is ready" signal -- it lives in GA_REG_CDDSTAT3.
 * @sa ga_reg_cddstat3, GA_CDDCOMM_HI_MASK, GA_CDDCOMM_LO_MASK
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_31
 */
#define GA_REG_CDDSTAT3 0xFF803E

/**
 * @def GA_REG_CDDSTAT4
 * @brief CDD receiving status 8 and 9
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS drive calls.
 *
 * Two of the ten status nibbles the CDD returns. Read only in practice: the
 * drive fills them and the Sub CPU consumes them.
 *
 * The ten registers from 0xFF8038 to 0xFF804A carry twenty 4 bit values, two
 * per register: one in bits 11-8 and one in bits 3-0. The remaining nibbles
 * read as 0.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |0|0|0|0|\b RS8||||0|0|0|0|\b RS9||||
 *
 * @param RS8 Receiving Status 8, in bits 11-8.
 * @param RS9 Receiving Status 9, in bits 3-0.
 * @sa ga_reg_cddstat4, GA_CDDCOMM_HI_MASK, GA_CDDCOMM_LO_MASK
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_32
 */
#define GA_REG_CDDSTAT4 0xFF8040

/**
 * @def GA_REG_CDDCMD0
 * @brief CDD transmission command 0 and 1
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS drive calls.
 *
 * Two of the ten command nibbles sent to the CDD. The Sub CPU fills them and
 * the drive consumes them.
 *
 * The ten registers from 0xFF8038 to 0xFF804A carry twenty 4 bit values, two
 * per register: one in bits 11-8 and one in bits 3-0. The remaining nibbles
 * read as 0.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |0|0|0|0|\b TC0||||0|0|0|0|\b TC1||||
 *
 * @param TC0 Transmission Command 0, in bits 11-8.
 * @param TC1 Transmission Command 1, in bits 3-0.
 * @sa ga_reg_cddcmd0, GA_CDDCOMM_HI_MASK, GA_CDDCOMM_LO_MASK
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_33
 */
#define GA_REG_CDDCMD0 0xFF8042

/**
 * @def GA_REG_CDDCMD1
 * @brief CDD transmission command 2 and 3
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS drive calls.
 *
 * Two of the ten command nibbles sent to the CDD. The Sub CPU fills them and
 * the drive consumes them.
 *
 * The ten registers from 0xFF8038 to 0xFF804A carry twenty 4 bit values, two
 * per register: one in bits 11-8 and one in bits 3-0. The remaining nibbles
 * read as 0.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |0|0|0|0|\b TC2||||0|0|0|0|\b TC3||||
 *
 * @param TC2 Transmission Command 2, in bits 11-8.
 * @param TC3 Transmission Command 3, in bits 3-0.
 * @sa ga_reg_cddcmd1, GA_CDDCOMM_HI_MASK, GA_CDDCOMM_LO_MASK
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_34
 */
#define GA_REG_CDDCMD1 0xFF8044

/**
 * @def GA_REG_CDDCMD2
 * @brief CDD transmission command 4 and 5
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS drive calls.
 *
 * Two of the ten command nibbles sent to the CDD. The Sub CPU fills them and
 * the drive consumes them.
 *
 * The ten registers from 0xFF8038 to 0xFF804A carry twenty 4 bit values, two
 * per register: one in bits 11-8 and one in bits 3-0. The remaining nibbles
 * read as 0.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |0|0|0|0|\b TC4||||0|0|0|0|\b TC5||||
 *
 * @param TC4 Transmission Command 4, in bits 11-8.
 * @param TC5 Transmission Command 5, in bits 3-0.
 * @sa ga_reg_cddcmd2, GA_CDDCOMM_HI_MASK, GA_CDDCOMM_LO_MASK
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_35
 */
#define GA_REG_CDDCMD2 0xFF8046

/**
 * @def GA_REG_CDDCMD3
 * @brief CDD transmission command 6 and 7
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS drive calls.
 *
 * Two of the ten command nibbles sent to the CDD. The Sub CPU fills them and
 * the drive consumes them.
 *
 * The ten registers from 0xFF8038 to 0xFF804A carry twenty 4 bit values, two
 * per register: one in bits 11-8 and one in bits 3-0. The remaining nibbles
 * read as 0.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |0|0|0|0|\b TC6||||0|0|0|0|\b TC7||||
 *
 * @param TC6 Transmission Command 6, in bits 11-8.
 * @param TC7 Transmission Command 7, in bits 3-0.
 * @sa ga_reg_cddcmd3, GA_CDDCOMM_HI_MASK, GA_CDDCOMM_LO_MASK
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_36
 */
#define GA_REG_CDDCMD3 0xFF8048

/**
 * @def GA_REG_CDDCMD4
 * @brief CDD transmission command 8 and 9
 *
 * @details
 * @warning **The hardware documentation prohibits application software from
 * accessing this register directly.** Use the Sub BIOS drive calls.
 *
 * Two of the ten command nibbles sent to the CDD. The Sub CPU fills them and
 * the drive consumes them.
 *
 * The ten registers from 0xFF8038 to 0xFF804A carry twenty 4 bit values, two
 * per register: one in bits 11-8 and one in bits 3-0. The remaining nibbles
 * read as 0.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |0|0|0|0|\b TC8||||0|0|0|0|\b TC9||||
 *
 * @param TC8 Transmission Command 8, in bits 11-8.
 * @param TC9 Transmission Command 9, in bits 3-0.
 * @warning Writing **Transmission Command 9** is what starts the transfer to
 * the CDD, so fill every other nibble first. It lives in the low half of this
 * register.
 * @sa ga_reg_cddcmd4, GA_CDDCOMM_HI_MASK, GA_CDDCOMM_LO_MASK
 * @warning Bit operation instructions are not permitted here; read the
 * register, modify the copy, and write the whole value back.
 * @ingroup ga_reg_sub_37
 */
#define GA_REG_CDDCMD4 0xFF804A

/**
 * @def GA_CDDCOMM_HI_MASK
 * @brief The even-numbered nibble of a CDD communication register
 * @sa GA_REG_CDDSTAT0
 * @ingroup ga_regs_sub
 */
#define GA_CDDCOMM_HI_POS 8
#define GA_CDDCOMM_HI_WIDTH 4
#define GA_CDDCOMM_HI_MASK FIELD_MASK(GA_CDDCOMM_HI_POS, GA_CDDCOMM_HI_WIDTH)

/**
 * @def GA_CDDCOMM_LO_MASK
 * @brief The odd-numbered nibble of a CDD communication register
 * @sa GA_REG_CDDSTAT0
 * @ingroup ga_regs_sub
 */
#define GA_CDDCOMM_LO_POS 0
#define GA_CDDCOMM_LO_WIDTH 4
#define GA_CDDCOMM_LO_MASK FIELD_MASK(GA_CDDCOMM_LO_POS, GA_CDDCOMM_LO_WIDTH)

/**
 * @def GA_REG_FONTCOLOR
 * @brief Colours the font expander substitutes for 0 and 1 bits
 *
 * @details
 * The low byte of the hardware register at 0xFF804C; the high byte is unused
 * and reads as 0.
 *
 * Together with GA_REG_FONTBITS and GA_REG_FONTDATA this forms a small hardware
 * accelerator that expands a 1 bit per pixel bitmap into Mega Drive 4 bits per
 * pixel pattern data. Write the two colour indices here, write a row of 16
 * source bits to GA_REG_FONTBITS, then read four words back from
 * GA_REG_FONTDATA.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b SC13|\b SC12|\b SC11|\b SC10|\b SC03|\b SC02|\b SC01|\b SC00|
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param SC0 Palette index written wherever a source bit is **0**.
 * @param SC1 Palette index written wherever a source bit is **1**.
 * @sa ga_reg_fontcolor, GA_FONTCOLOR_0_MASK, GA_FONTCOLOR_1_MASK
 * @ingroup ga_reg_sub_38
 */
#define GA_REG_FONTCOLOR 0xFF804D

/**
 * @def GA_FONTCOLOR_0_MASK
 * @brief Colour substituted for a source bit of 0
 * @sa GA_REG_FONTCOLOR
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_38
 */
#define GA_FONTCOLOR_0_POS 0
#define GA_FONTCOLOR_0_WIDTH 4
#define GA_FONTCOLOR_0_MASK FIELD_MASK(GA_FONTCOLOR_0_POS, GA_FONTCOLOR_0_WIDTH)

/**
 * @def GA_FONTCOLOR_1_MASK
 * @brief Colour substituted for a source bit of 1
 * @sa GA_REG_FONTCOLOR
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_38
 */
#define GA_FONTCOLOR_1_POS 4
#define GA_FONTCOLOR_1_WIDTH 4
#define GA_FONTCOLOR_1_MASK FIELD_MASK(GA_FONTCOLOR_1_POS, GA_FONTCOLOR_1_WIDTH)

/**
 * @def GA_REG_FONTBITS
 * @brief Source bitmap for the font expander, 16 pixels
 *
 * @details
 * Sixteen source pixels, one per bit. SBF is the leftmost and SB0 the
 * rightmost, matching the order the expanded pixels appear in GA_REG_FONTDATA.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b SBF|\b SBE|\b SBD|\b SBC|\b SBB|\b SBA|\b SB9|\b SB8|\b SB7|\b SB6|\b SB5|\b SB4|\b SB3|\b SB2|\b SB1|\b SB0|
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param SB Source bits. The documentation gives Chinese character fonts as the
 * motivating case, but nothing about the conversion is font specific.
 * @note Writing this register is what triggers the conversion; the result is
 * available in GA_REG_FONTDATA immediately afterwards.
 * @sa ga_reg_fontbits, GA_REG_FONTDATA
 * @ingroup ga_reg_sub_39
 */
#define GA_REG_FONTBITS 0xFF804E

/**
 * @def GA_REG_FONTDATA
 * @brief Expanded 4 bits per pixel output of the font expander
 *
 * @details
 * Four consecutive words at 0xFF8050, 0xFF8052, 0xFF8054 and 0xFF8056. Each of
 * the sixteen source bits becomes one 4 bit pixel, so the sixteen pixels fill
 * 64 bits, laid out ready to copy into VRAM.
 *
 * The order runs from the most significant source bit down: SBF becomes the top
 * nibble of the first word and SB0 the bottom nibble of the last, which is the
 * left to right pixel order Mega Drive pattern data uses.
 *
 * | Word | Pixels, high nibble first |
 * |---|---|
 * | 0xFF8050 | SBF, SBE, SBD, SBC |
 * | 0xFF8052 | SBB, SBA, SB9, SB8 |
 * | 0xFF8054 | SB7, SB6, SB5, SB4 |
 * | 0xFF8056 | SB3, SB2, SB1, SB0 |
 *
 * With GA_FONTCOLOR_1 set to 0xF and GA_FONTCOLOR_0 to 0x1, a source bit of 0
 * yields the pixel value 1 and a source bit of 1 yields 0xF.
 *
 * @warning Read only.
 * @sa ga_reg_fontdata, GA_REG_FONTBITS
 * @ingroup ga_reg_sub_40
 */
#define GA_REG_FONTDATA 0xFF8050

/**
 * @def GA_REG_GFXSTAT
 * @brief Graphics operation status
 *
 * @details
 * The high byte of the hardware register at 0xFF8058. Split from the size
 * configuration in its low byte because one is a status flag the hardware
 * drives and the other is configuration you write (SPEC.md D17).
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b GRON| | | | | | | |
 * |\b R|◯| | | | | | | |
 * |\b W| | | | | | | | |
 *
 * @param GRON Graphics operation in progress. 1: running, 0: complete.
 * @note Poll this to know when a rotation or scaling operation has finished.
 * It is the only completion signal the hardware gives, other than the level 1
 * interrupt (GA_INT1_MASK).
 * @warning Read only.
 * @sa ga_reg_gfxstat, GA_REG_STAMPSIZE
 * @ingroup ga_reg_sub_44
 */
#define GA_REG_GFXSTAT 0xFF8058

/**
 * @def GA_GRON_MASK
 * @brief Graphics operation in progress
 * @sa GA_REG_GFXSTAT
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_44
 */
#define GA_GRON_POS 7
#define GA_GRON_WIDTH 1
#define GA_GRON_MASK FIELD_MASK(GA_GRON_POS, GA_GRON_WIDTH)

/**
 * @def GA_REG_STAMPSIZE
 * @brief Stamp and stamp map geometry
 *
 * @details
 * The low byte of the hardware register at 0xFF8058.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | | | | | | |\b SMS|\b STS|\b RPT|
 * |\b R| | | | | |◯|◯|◯|
 * |\b W| | | | | |◯|◯|◯|
 *
 * @param RPT Repeat. 1: the stamp map tiles indefinitely. 0: anything outside
 * the map reads as 0.
 * @param STS Stamp size. 0: 16x16 dots, 1: 32x32 dots.
 * @param SMS Stamp map size. 0: one screen, 256x256 dots. 1: sixteen screens
 * square, 4096x4096 dots.
 *
 * The two size bits together decide how much Word RAM the map occupies, and
 * therefore how coarsely GA_REG_STAMPMAPBASE must be aligned.
 *
 * @sa ga_reg_stampsize, GA_REG_GFXSTAT, GA_REG_STAMPMAPBASE
 * @ingroup ga_reg_sub_44
 */
#define GA_REG_STAMPSIZE 0xFF8059

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
 * @brief Where the stamp map sits in Word RAM
 *
 * @details
 * The value written is the offset from the start of Word RAM divided by 4, so
 * register bit 15 corresponds to address bit A17.
 *
 * How many of the high bits are usable depends on how big the map is, which
 * follows from GA_REG_STAMPSIZE. A larger map needs coarser alignment:
 *
 * | Stamp | Map | Map occupies | Usable bits | Align the offset to |
 * |---|---|---|---|---|
 * | 16x16 | 256x256 | 0x200 | A17-A09 | 0x200 |
 * | 32x32 | 256x256 | 0x80 | A17-A07 | 0x80 |
 * | 16x16 | 4096x4096 | 0x20000 | A17 only | 0x20000 |
 * | 32x32 | 4096x4096 | 0x8000 | A17-A15 | 0x8000 |
 *
 * With a 4096x4096 map and 16x16 stamps only A17 remains, so the map may start
 * at just two places in Word RAM.
 *
 * @note The manual's own figures on printed page 36 label these two cases with
 * the stamp sizes **swapped** -- it calls SMS=1/STS=0 "32x32 dots" where STS=0
 * is 16x16 by its own definition on page 35. The address tables either side of
 * that text are self consistent, and the table above is derived from the map
 * geometry, which agrees with them. Do not "correct" this against page 36.
 * @note Only meaningful in 2M mode.
 * @warning Word access only. A byte access to this register can raise a bus
 * error.
 * @warning Bit operation instructions are not permitted here.
 * @sa ga_reg_stampmapbase, GA_REG_STAMPSIZE
 * @ingroup ga_reg_sub_45
 */
#define GA_REG_STAMPMAPBASE 0xFF805A

/**
 * @defgroup stamp_entry Sub CPU / Graphics / Stamp map entry
 * @brief Layout of one entry in a stamp map
 *
 * @details
 * A stamp map is an array of 16 bit entries, one per stamp position. This is
 * not a gate array register -- it is the data the rotation hardware reads from
 * Word RAM -- but the field layout belongs with the registers that describe it.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b HFLP|\b RT1|\b RT0|0|0|\b SNOA|\b SNO9|\b SNO8|\b SNO7|\b SNO6|\b SNO5|\b SNO4|\b SNO3|\b SNO2|\b SNO1|\b SNO0|
 *
 * Stamp generators begin at the start of 2M Word RAM, which the Sub CPU sees at
 * 0x080000, and are laid out exactly like Mega Drive sprite patterns: two dots
 * per byte, high nibble first, so the top nibble of the first byte is the top
 * left dot.
 * @{
 */

/**
 * @def STAMP_SNO_MASK
 * @brief Stamp number
 * @warning With 32x32 stamps the two low bits must be 0, since each stamp then
 * covers four 16x16 generator slots.
 */
#define STAMP_SNO_POS 0
#define STAMP_SNO_WIDTH 11
#define STAMP_SNO_MASK FIELD_MASK(STAMP_SNO_POS, STAMP_SNO_WIDTH)

/**
 * @def STAMP_ROTATE_MASK
 * @brief Rotation applied to this stamp, in 90 degree steps
 */
#define STAMP_ROTATE_POS 13
#define STAMP_ROTATE_WIDTH 2
#define STAMP_ROTATE_MASK FIELD_MASK(STAMP_ROTATE_POS, STAMP_ROTATE_WIDTH)

#define STAMP_ROTATE_0 0b00
#define STAMP_ROTATE_90 0b01
#define STAMP_ROTATE_180 0b10
#define STAMP_ROTATE_270 0b11

/**
 * @def STAMP_HFLIP_MASK
 * @brief Mirror the stamp horizontally
 */
#define STAMP_HFLIP_POS 15
#define STAMP_HFLIP_WIDTH 1
#define STAMP_HFLIP_MASK FIELD_MASK(STAMP_HFLIP_POS, STAMP_HFLIP_WIDTH)

/** @} */

/**
 * @def GA_REG_IMGBUFVSIZE
 * @brief Image buffer height, in cells
 *
 * @details
 * The low byte of the hardware register at 0xFF805C; the high byte is unused.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | | | | |\b VCS4|\b VCS3|\b VCS2|\b VCS1|\b VCS0|
 * |\b R| | | |◯|◯|◯|◯|◯|
 * |\b W| | | |◯|◯|◯|◯|◯|
 *
 * @param VCS Vertical size in cells, 0 to 31.
 * @warning **Store one less than the height you want.** The manual states it
 * plainly: set the value to (actual value - 1). A buffer 8 cells tall is
 * written as 7.
 * @warning Bit operation instructions are not permitted here.
 * @sa ga_reg_imgbufvsize
 * @ingroup ga_reg_sub_46
 */
#define GA_REG_IMGBUFVSIZE 0xFF805D

/**
 * @def GA_IMGBUF_VCS_MASK
 * @brief Image buffer height in cells, stored as height - 1
 * @sa GA_REG_IMGBUFVSIZE
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_46
 */
#define GA_IMGBUF_VCS_POS 0
#define GA_IMGBUF_VCS_WIDTH 5
#define GA_IMGBUF_VCS_MASK FIELD_MASK(GA_IMGBUF_VCS_POS, GA_IMGBUF_VCS_WIDTH)

/**
 * @def GA_REG_IMGBUFSTART
 * @brief Where the image buffer sits in Word RAM
 *
 * @details
 * Holds address bits A17 down to A05 in bits 15 down to 3, so the value is the
 * Word RAM offset divided by 4 and the buffer must begin on a 32 byte boundary.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b A17|\b A16|\b A15|\b A14|\b A13|\b A12|\b A11|\b A10|\b A09|\b A08|\b A07|\b A06|\b A05| | | |
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯| | | |
 * |\b W|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯| | | |
 *
 * @param A Image buffer base, bits 5-17.
 * @warning Word access only. A byte access to this register can raise a bus
 * error.
 * @warning Bit operation instructions are not permitted here.
 * @sa ga_reg_imgbufstart
 * @ingroup ga_reg_sub_47
 */
#define GA_REG_IMGBUFSTART 0xFF805E

/**
 * @def GA_REG_IMGBUFOFFSET
 * @brief Where writing starts inside the first cell of the image buffer
 *
 * @details
 * The low byte of the hardware register at 0xFF8060; the high byte is unused.
 * Together with GA_REG_IMGBUFSTART this fixes the exact dot the hardware begins
 * writing at: the start address picks the cell, this picks the dot within it.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | | | |\b LN2|\b LN1|\b LN0|\b DOT2|\b DOT1|\b DOT0|
 * |\b R| | |◯|◯|◯|◯|◯|◯|
 * |\b W| | |◯|◯|◯|◯|◯|◯|
 *
 * @param LN Line within the first cell, 0 to 7.
 * @param DOT Dot within that line, 0 to 7.
 * @sa ga_reg_imgbufoffset, GA_IMGBUF_LN_MASK, GA_IMGBUF_DOT_MASK
 * @ingroup ga_reg_sub_48
 */
#define GA_REG_IMGBUFOFFSET 0xFF8061

/**
 * @def GA_IMGBUF_DOT_MASK
 * @brief Starting dot within the first line
 * @sa GA_REG_IMGBUFOFFSET
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_48
 */
#define GA_IMGBUF_DOT_POS 0
#define GA_IMGBUF_DOT_WIDTH 3
#define GA_IMGBUF_DOT_MASK FIELD_MASK(GA_IMGBUF_DOT_POS, GA_IMGBUF_DOT_WIDTH)

/**
 * @def GA_IMGBUF_LN_MASK
 * @brief Starting line within the first cell
 * @sa GA_REG_IMGBUFOFFSET
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_48
 */
#define GA_IMGBUF_LN_POS 3
#define GA_IMGBUF_LN_WIDTH 3
#define GA_IMGBUF_LN_MASK FIELD_MASK(GA_IMGBUF_LN_POS, GA_IMGBUF_LN_WIDTH)

/**
 * @def GA_REG_IMGBUFHDOTSIZE
 * @brief Image buffer width, in dots
 *
 * @details
 * How many dots wide each written line is. Nine bits, so it spans both halves
 * of the register and stays 16 bit.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | | |\b HW08|\b HW07|\b HW06|\b HW05|\b HW04|\b HW03|\b HW02|\b HW01|\b HW00|
 * |\b R| | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param HW Horizontal dots to write, 0 to 511.
 * @note Where those dots land is set by GA_REG_IMGBUFSTART and
 * GA_REG_IMGBUFOFFSET together.
 * @warning Word access only. A byte access to this register can raise a bus
 * error.
 * @warning Bit operation instructions are not permitted here.
 * @sa ga_reg_imgbufhdotsize
 * @ingroup ga_reg_sub_49
 */
#define GA_REG_IMGBUFHDOTSIZE 0xFF8062

/**
 * @def GA_IMGBUF_HW_MASK
 * @brief Horizontal dots written per line
 * @sa GA_REG_IMGBUFHDOTSIZE
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_49
 */
#define GA_IMGBUF_HW_POS 0
#define GA_IMGBUF_HW_WIDTH 9
#define GA_IMGBUF_HW_MASK FIELD_MASK(GA_IMGBUF_HW_POS, GA_IMGBUF_HW_WIDTH)

/**
 * @def GA_REG_IMGBUFVDOTSIZE
 * @brief Image buffer height, in dots -- and the operation's progress counter
 *
 * @details
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | | | | | |\b VW07|\b VW06|\b VW05|\b VW04|\b VW03|\b VW02|\b VW01|\b VW00|
 * |\b R| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | | | | | |◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param VW Vertical dots to write, 0 to 255.
 * @warning **This register is destroyed by the operation it configures.** The
 * hardware decrements it as the operation runs and it reads 0 once complete, so
 * it must be written again before every graphics or numeric operation. Setting
 * it once and reusing it will silently perform a zero-height operation the
 * second time.
 * @note The flip side is that it can be read during an operation as a
 * remaining-lines counter.
 * @note Where the dots land is set by GA_REG_IMGBUFSTART and
 * GA_REG_IMGBUFOFFSET together.
 * @warning Word access only. A byte access to this register can raise a bus
 * error.
 * @warning Bit operation instructions are not permitted here.
 * @sa ga_reg_imgbufvdotsize
 * @ingroup ga_reg_sub_50
 */
#define GA_REG_IMGBUFVDOTSIZE 0xFF8064

/**
 * @def GA_IMGBUF_VW_MASK
 * @brief Vertical dots remaining to write
 * @sa GA_REG_IMGBUFVDOTSIZE
 * @ingroup ga_regs_sub
 * @ingroup ga_reg_sub_50
 */
#define GA_IMGBUF_VW_POS 0
#define GA_IMGBUF_VW_WIDTH 8
#define GA_IMGBUF_VW_MASK FIELD_MASK(GA_IMGBUF_VW_POS, GA_IMGBUF_VW_WIDTH)

/**
 * @def GA_REG_TRACEVECTBASE
 * @brief Trace vector table address -- and the operation trigger
 *
 * @details
 * Holds address bits A17 down to A03 in bits 15 down to 1, so the value is the
 * Word RAM offset divided by 4 and the table must begin on an 8 byte boundary.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b A17|\b A16|\b A15|\b A14|\b A13|\b A12|\b A11|\b A10|\b A09|\b A08|\b A07|\b A06|\b A05|\b A04|\b A03| |
 * |\b R|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|
 * |\b W|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯| |
 *
 * @warning **Writing this register starts the operation.** It is the trigger,
 * not merely a pointer, so every other rotation and scaling register must
 * already hold its final value. Completion raises a level 1 interrupt, and
 * GA_GRON_MASK reads 0.
 *
 * The pipeline it drives is: start position and deltas, through the stamp map,
 * through the stamp generators, into the image buffer.
 *
 * ### The trace vector table
 *
 * Four words per output line, at the address written here:
 *
 * | Word | Contents | Format |
 * |---|---|---|
 * | 0 | X start | 13 integer bits, 3 fractional |
 * | 1 | Y start | 13 integer bits, 3 fractional |
 * | 2 | delta X per dot | sign bit, 4 integer bits, 11 fractional |
 * | 3 | delta Y per dot | sign bit, 4 integer bits, 11 fractional |
 *
 * @warning The deltas are **sign and magnitude**, not two's complement: bit 15
 * is the sign and the remaining bits are an unsigned magnitude. Negating a
 * delta means toggling bit 15, not negating the word.
 *
 * @warning Write only, and word access only. A byte access to this register can
 * raise a bus error.
 * @warning Bit operation instructions are not permitted here.
 * @sa ga_reg_tracevectbase, GA_REG_GFXSTAT
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

#define CDC_DEST_MAINREAD 2
#define CDC_DEST_SUBREAD  3
#define CDC_DEST_PCMDMA   4
#define CDC_DEST_PRAMDMA  5
#define CDC_DEST_WRAMDMA  7

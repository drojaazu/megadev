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
 * @def GA_REG_INT2
 * @brief Level 2 interrupt to the Sub CPU
 *
 * @details
 * The high byte of the hardware register at 0xA12000. Split from the Sub CPU
 * bus control below it because the two halves share nothing but an address:
 * see SPEC.md D17.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b IEN2| | | | | | |\b IFL2|
 * |\b R|◯| | | | | | |◯|
 * |\b W| | | | | | | |◯|
 *
 * @param IFL2 Raise a level 2 interrupt on the Sub CPU.
 * \n [write] 1: generate the interrupt, provided IEN2 is set. Writing 0 is not
 * used.
 * \n [read] 0: the interrupt is still being serviced / 1: it has not been
 * taken yet
 * @param IEN2 Mask state of level 2 on the Sub CPU side. 0: masked, 1: enabled.
 * @note IEN2 is read only from this side -- the Sub CPU owns its own interrupt
 * mask. Raising an interrupt while it reads 0 does nothing.
 * @warning BSET and BCLR may not be used on this register; only BTST.
 * @sa ga_reg_int2
 * @ingroup ga_reg_main_00
 */
#define GA_REG_INT2 0xA12000

/**
 * @def GA_REG_SUBCPU
 * @brief Sub CPU reset and bus request
 *
 * @details
 * The low byte of the hardware register at 0xA12000.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | | | | | | | |\b SBRQ|\b SRES|
 * |\b R| | | | | | |◯|◯|
 * |\b W| | | | | | |◯|◯|
 *
 * @param SRES Sub CPU reset.
 * \n [write] 0: hold in reset / 1: run
 * \n [read] 0: the reset is still in progress / 1: the reset has finished
 * @param SBRQ Sub CPU bus request.
 * \n [write] 0: cancel the request / 1: request the bus
 * \n [read] 0: the Sub CPU is running / 1: the request has been granted
 *
 * @note Program RAM may only be touched from this side once SBRQ reads 1.
 * @warning BSET and BCLR may not be used on this register; only BTST.
 * @sa ga_reg_subcpu
 * @ingroup ga_reg_main_00
 */
#define GA_REG_SUBCPU 0xA12001


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
 * @def GA_REG_WP
 * @brief Program RAM write protection
 *
 * @details
 * The high byte of the hardware register at 0xA12002. Each set bit protects one
 * 512 byte block at the bottom of Program RAM, so the field spans 0x000000 to
 * 0x01FDFF. This is what keeps a stray write from this side out of the Sub
 * CPU's resident code.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b WP7|\b WP6|\b WP5|\b WP4|\b WP3|\b WP2|\b WP1|\b WP0|
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @param WP0-7 Write protect block enables.
 * @sa ga_reg_wp
 * @ingroup ga_reg_main_01
 */
#define GA_REG_WP 0xA12002

/**
 * @def GA_REG_MEMMODE
 * @brief Word RAM ownership and layout, Program RAM bank select
 *
 * @details
 * The low byte of the hardware register at 0xA12002. Split from the write
 * protect byte above it because a word-wide write to change the memory mode
 * would silently clear the protection: see SPEC.md D17.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b BK1|\b BK0| | | |\b MODE|\b DMNA|\b RET|
 * |\b R|◯|◯| | | |◯|◯|◯|
 * |\b W|◯|◯| | | |◯|◯| |
 *
 * @param BK0-1 Program RAM bank select. See GA_MEMMODE_BANK_MASK.
 * @param MODE Word RAM layout. 0: 2M, 1: 1M. Read only from this side.
 * @param DMNA Declaration of Main RAM No Access. Writing 1 hands Word RAM to
 * the Sub CPU; reading 1 means the handover is complete.
 * @param RET The counterpart to DMNA, read only from this side. 1 means Word
 * RAM has been returned to the Main CPU.
 *
 * @note RET is read only here and DMNA is read only on the Sub side: each CPU
 * writes the bit that gives the memory away and polls the one that takes it
 * back.
 * @sa ga_reg_memmode
 * @ingroup ga_reg_main_01
 */
#define GA_REG_MEMMODE 0xA12003


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

/**
 * @def GA_MEMMODE_BANK_MASK
 * @brief Program RAM bank select
 * @details
 * The Main CPU sees only 128kB of the 512kB of Program RAM at a time, as a
 * window at 0x020000. This field selects which of the four banks appears
 * there. Only meaningful while the Sub CPU is stopped or has granted the bus.
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_MEMMODE_BANK_POS 6
#define GA_MEMMODE_BANK_WIDTH 2
#define GA_MEMMODE_BANK_MASK FIELD_MASK(GA_MEMMODE_BANK_POS, GA_MEMMODE_BANK_WIDTH)

/**
 * @def GA_WP_MASK
 * @brief Program RAM write protect
 * @details
 * Protects the bottom of Program RAM from Main CPU writes in units of 512
 * bytes: each set bit protects one 512 byte block, covering 0x000000 to
 * 0x01FDFF in total. This is how the Sub CPU's resident code is shielded from
 * a stray write by the Main side.
 * @note A byte register in its own right since D17, so a single byte write
 * sets the whole field.
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_memmode
 */
#define GA_WP_POS 0
#define GA_WP_WIDTH 8
#define GA_WP_MASK FIELD_MASK(GA_WP_POS, GA_WP_WIDTH)

/**
 * @defgroup ga_reg_main_cdcmode Main CPU / Gate Array / Registers / CDC Mode
 */

/**
 * @def GA_REG_CDCMODE
 * @brief CDC mode
 *
 * @details
 * The Main CPU's view of the CDC transfer state. Every field is in the high
 * byte of the hardware register at 0xA12004, and the register is read only from
 * this side: the Sub CPU chooses the destination and the Main CPU watches the
 * transfer. The low byte, 0xA12005, is unused -- it reads as 0 and is
 * deliberately left undefined here (SPEC.md D17a).
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b EDT|\b DSR| | | |\b DD2|\b DD1|\b DD0|
 * |\b R|◯|◯| | | |◯|◯|◯|
 * |\b W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|
 *
 * @param EDT End of data transfer. R: every byte has been transferred out of
 * the CDC.
 * @param DSR Data set ready. R: a word from the CDC is waiting in
 * GA_REG_CDCHOSTDATA.
 * @param DD Device destination, set by the Sub CPU. See GA_CDC_DEST_MASK.
 *
 * @warning Read only. The Sub CPU owns this register; see its `$FF8004`, which
 * additionally carries the CDC register address in its low byte and is
 * therefore still a 16 bit register on that side.
 * @sa ga_reg_cdcmode
 * @ingroup ga_reg_main_02
 */
#define GA_REG_CDCMODE 0xA12004

/**
 * @def GA_CDC_DEST_MASK
 * @brief CDC data destination device
 * @details
 * Where the CDC sends the data it reads off the disc. The Main CPU may read
 * this field but only the Sub CPU may set it.
 * @sa GA_CDC_DEST_MAIN, GA_CDC_DEST_SUB, GA_CDC_DEST_PCM, GA_CDC_DEST_PRG,
 * GA_CDC_DEST_WORD
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cdcmode
 */
#define GA_CDC_DEST_POS 0
#define GA_CDC_DEST_WIDTH 3
#define GA_CDC_DEST_MASK FIELD_MASK(GA_CDC_DEST_POS, GA_CDC_DEST_WIDTH)

/**
 * @def GA_CDC_DSR_MASK
 * @brief Data set ready
 * @details R: A word from the CDC is waiting in GA_REG_CDCHOSTDATA.
 * @sa GA_REG_CDCMODE
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cdcmode
 */
#define GA_CDC_DSR_POS 6
#define GA_CDC_DSR_WIDTH 1
#define GA_CDC_DSR_MASK FIELD_MASK(GA_CDC_DSR_POS, GA_CDC_DSR_WIDTH)

/**
 * @def GA_CDC_EDT_MASK
 * @brief End of data transfer
 * @details R: Every byte has been transferred out of the CDC.
 * @sa GA_REG_CDCMODE
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cdcmode
 */
#define GA_CDC_EDT_POS 7
#define GA_CDC_EDT_WIDTH 1
#define GA_CDC_EDT_MASK FIELD_MASK(GA_CDC_EDT_POS, GA_CDC_EDT_WIDTH)

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
 * @def GA_CDC_DEST_PRG
 * @brief CDC data goes to Program RAM by DMA
 * @ingroup ga_regs_main
 * @ingroup ga_reg_main_cdcmode
 */
#define GA_CDC_DEST_PRG 0b101

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
 * @param HIB The **low** word of the HBLANK (level 4) interrupt vector.
 *
 * When H-INT fires, the vector normally at 0x70 is not used as written: the
 * hardware takes the high word from 0x70 and substitutes this register for the
 * low word, ignoring 0x72 entirely. The Boot ROM leaves 0x00FF in the high
 * word.
 * @note Reset leaves this register set to 0xFFFF.
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
 * @brief CDC host data
 *
 * @details
 * Two bytes recovered from the disc, ready to be moved into Main CPU memory.
 * Reading it releases the register and the CDC loads the next two bytes.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | |\b HD|||||||||||||||
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|
 *
 * @param HD CDC read data.
 * @note Only meaningful once GA_CDC_DSR_MASK is set.
 * @warning Read only, and word access only. A byte access can raise a bus
 * error, and there is nothing to write.
 * @warning Bit operation instructions are not permitted here.
 * @sa ga_reg_cdchostdata
 * @ingroup ga_reg_main_04
 */
#define GA_REG_CDCHOSTDATA 0xA12008

/**
 * @def GA_REG_SYSRESERVED
 * @brief Reserved by the system
 * @details 0xA1200A is documented only as reserved. Do not use it.
 * @ingroup ga_reg_main_05
 */
#define GA_REG_SYSRESERVED 0xA1200A

/**
 * @defgroup ga_reg_main_stopwatch Main CPU / Gate Array / Registers / Stopwatch
 */

/**
 * @def GA_REG_STOPWATCH
 * @brief Stopwatch, read only from this side
 *
 * @details
 * The same free-running 12 bit counter the Sub CPU sees at 0xFF800C: it counts
 * 0 to 4095 at 30.72 microseconds per tick and wraps.
 *
 * | |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
 * | | | | | |\b TD11|\b TD10|\b TD09|\b TD08|\b TD07|\b TD06|\b TD05|\b TD04|\b TD03|\b TD02|\b TD01|\b TD00|
 * |\b R| | | | |◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|
 *
 * @param TD Current count.
 * @warning **Read only from the Main CPU.** Only the Sub CPU can clear the
 * timer, by writing its own 0xFF800C. Writing here does nothing, so the Main
 * side can measure an interval but cannot choose when it starts.
 * @warning Word access only. A byte access to this register can raise a bus
 * error.
 * @warning Bit operation instructions are not permitted here.
 * @sa ga_reg_stopwatch
 * @ingroup ga_reg_main_06
 */
#define GA_REG_STOPWATCH 0xA1200C

/**
 * @defgroup ga_reg_main_cpucomm Main CPU / Gate Array / Registers / CPU
 * Communication
 */

/**
 * @def GA_REG_COMFLAGS_MAIN
 * @brief Communication flags written by the Main CPU
 *
 * @details
 * The high byte of the hardware register at 0xA1200E. Eight flags the Main CPU
 * sets and the other side polls. The hardware assigns no meaning to any of
 * them; they are yours to define.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b CFM7|\b CFM6|\b CFM5|\b CFM4|\b CFM3|\b CFM2|\b CFM1|\b CFM0|
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W|◯|◯|◯|◯|◯|◯|◯|◯|
 *
 * @note Read/write from this CPU.
 * @warning **Test one bit at a time.** If both CPUs read and write the flags at
 * the same moment the write lands correctly but the read may return stale data.
 * A single-bit test is immune to this; a byte or word read of the pair is not,
 * which is the reason the two halves are separate registers here.
 * @sa ga_reg_comflags_main
 * @ingroup ga_reg_main_07
 */
#define GA_REG_COMFLAGS_MAIN 0xA1200E

/**
 * @def GA_REG_COMFLAGS_SUB
 * @brief Communication flags written by the Sub CPU
 *
 * @details
 * The low byte of the hardware register at 0xA1200E. Eight flags the Sub CPU
 * sets and the other side polls. The hardware assigns no meaning to any of
 * them; they are yours to define.
 *
 * | |7|6|5|4|3|2|1|0|
 * |:|:|:|:|:|:|:|:|:|
 * | |\b CFS7|\b CFS6|\b CFS5|\b CFS4|\b CFS3|\b CFS2|\b CFS1|\b CFS0|
 * |\b R|◯|◯|◯|◯|◯|◯|◯|◯|
 * |\b W| | | | | | | | |
 *
 * @note Read only from this CPU -- the other side owns these flags.
 * @warning **Test one bit at a time.** If both CPUs read and write the flags at
 * the same moment the write lands correctly but the read may return stale data.
 * A single-bit test is immune to this; a byte or word read of the pair is not,
 * which is the reason the two halves are separate registers here.
 * @sa ga_reg_comflags_sub
 * @ingroup ga_reg_main_07
 */
#define GA_REG_COMFLAGS_SUB (0xA1200E + 1)

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

/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gatearr.h
 * @brief C wrappers for Sub CPU side Gate Array registers
 */

#ifndef MEGADEV__SUB_GATEARR_H
#define MEGADEV__SUB_GATEARR_H

#include "gatearr_def.h"

/**
 * @def GA_RESET
 * @brief GA Reg 00 - Sub CPU Reset/ Bus Request
 * @ingroup sub_ga_regs
 * @sa _GA_RESET
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||LEDG|LEDR|Ver3|Ver2|Ver1|Ver0| |||RES0|
 *
 * @param RES0 Sub CPU reset / Version / LED Control
 * @details
 * W: 0: Reset / 1: No effect \n R: 0: Reset in progress / 1: Reset
 * possible
 * @param LEDR Red LED control
 * @details
 * RW: 0: Off / 1: On
 * @param LEDG Green LED control
 * @details
 * RW: 0: Off / 1: On
 * @param Ver ROM Version
 * @details
 * R: ROM Version
 */
#define GA_RESET ((volatile u16 *) _GA_RESET)

/**
 * @def GA_MEMMODE
 * @brief GA Reg 01 - Word RAM Memory Mode / RAM Write Protect / Priority Mode
 * @ingroup sub_ga_regs
 * @sa _GA_MEMMODE
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |WP7|WP6|WP5|WP4|WP3|WP2|WP1|WP0| |||PM1|PM0|MODE|DMNA|RET|
 *
 * @param WP Write protect Sub CPU RAM
 * @param PM Priority Mode
 * @param MODE Word RAM layout
 * @details 0: 2M, 1: 1M
 * @param DMNA Main CPU will not access Word RAM
 * @param RET In 2M mode: Give Word RAM control to Main CPU;
 * In 1M mode: Change 1M block ownership
 *
 */
#define GA_MEMMODE ((volatile u16 *) _GA_MEMMODE)

/**
 * @def GA_CDCMODE
 * @brief GA Reg 02 - CDC Mode / CDC Registers #1
 * @ingroup sub_ga_regs
 * @sa _GA_CDCMODE
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
#define GA_CDCMODE ((volatile u16 *) _GA_CDCMODE)

/**
 * @def GA_CDCRS1
 * @brief GA Reg 03 - CDC Registers #2
 * @ingroup sub_ga_regs
 * @sa _GA_CDCRS1
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||||CD7|CD6|CD5|CD4|CD3|CD2|CD1|CD0|
 *
 * @param CD CDC register data
 * @details RW: Details for this register can be found in the LC89510 manual
 */
#define GA_CDCRS1 ((volatile u16 *) _GA_CDCRS1)

/**
 * @def GA_CDCHOSTDATA
 * @brief GA Reg 04 - CDC Host Data
 * @ingroup sub_ga_regs
 * @sa _GA_CDCHOSTDATA
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HD15|HD14|HD13|HD12|HD11|HD10|HD09|HD08|HD07|HD06|HD05|HD04|HD03|HD02|HD01|HD00|
 *
 * @param HD CDC read data
 * @details R: 2 bytes of data read by the CDC and ready to be transferred to
 * Main or Sub CPU memory
 */
#define GA_CDCHOSTDATA ((volatile u16 *) _GA_CDCHOSTDATA)

/**
 * @def GA_DMAADDR
 * @brief GA Reg 05 - CDC DMA Address
 * @ingroup sub_ga_regs
 * @sa _GA_DMAADDR
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |A18|A17|A16|A15|A14|A13|A12|A11|A10|A09|A08|A07|A06|A05|A04|A03|
 *
 * @param A DMA destination address
 * @details RW: Specifies the address for CDC DMA transfer
 * - For PCM DMA: bits up to A12 are used
 * - For 1M Word RAM: bits up to A16 are used
 * - For 2M Word RAM: bits up to A17 are used
 * - For PRG-RAM: all bits are used
 *
 * Unused bits will be read as 0.
 */
#define GA_DMAADDR ((volatile u16 *) _GA_DMAADDR)

/**
 * @def GA_STOPWATCH
 * @brief GA Reg 06 - Stopwatch
 * @ingroup sub_ga_regs
 * @sa _GA_STOPWATCH
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
#define GA_STOPWATCH ((volatile u16 *) _GA_STOPWATCH)

/**
 * @def GA_COMFLAGS
 * @brief GA Reg 07 - Main/Sub CPU communication flags
 * @ingroup sub_ga_regs
 * @sa _GA_COMFLAGS
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |CFM7|CFM6|CFM5|CFM4|CFM3|CFM2|CFM1|CFM0|CFS7|CFS6|CFS5|CFS4|CFS3|CFS2|CFS1|CFS0|
 *
 * CFM: Comm flags for Main CPU
 * CFS: Comm flags for Sub CPU
 */
#define GA_COMFLAGS ((volatile const u16 *) _GA_COMFLAGS)

#define GA_COMFLAGS_MAIN ((volatile const u8 *) _GA_COMFLAGS)
#define GA_COMFLAGS_SUB ((volatile u8 *) _GA_COMFLAGS + 1)

/**
 * @def GA_COMCMD0
 * @brief GA Reg 08 - Comm. command 0 (Main -> Sub)
 * @ingroup sub_ga_regs
 * @sa _GA_COMCMD0
 *
 * @details R: 16 bit data
 */
#define GA_COMCMD0 ((u16 volatile const *) _GA_COMCMD0)

/**
 * @def GA_COMCMD1
 * @brief GA Reg 09 - Comm. command (Main -> Sub)
 * @ingroup sub_ga_regs
 * @sa _GA_COMCMD1
 *
 * @details R: 16 bit data
 */
#define GA_COMCMD1 ((u16 volatile const *) _GA_COMCMD1)

/**
 * @def GA_COMCMD2
 * @brief GA Reg 0A - Comm. command (Main -> Sub)
 * @ingroup sub_ga_regs
 * @sa _GA_COMCMD2
 *
 * @details R: 16 bit data
 */
#define GA_COMCMD2 ((u16 volatile const *) _GA_COMCMD2)

/**
 * @def GA_COMCMD3
 * @brief GA Reg 0B - Comm. command (Main -> Sub)
 * @ingroup sub_ga_regs
 * @sa _GA_COMCMD3
 *
 * @details R: 16 bit data
 */
#define GA_COMCMD3 ((u16 volatile const *) _GA_COMCMD3)

/**
 * @def GA_COMCMD4
 * @brief GA Reg 0C - Comm. command (Main -> Sub)
 * @ingroup sub_ga_regs
 * @sa _GA_COMCMD4
 *
 * @details R: 16 bit data
 */
#define GA_COMCMD4 ((u16 volatile const *) _GA_COMCMD4)

/**
 * @def GA_COMCMD5
 * @brief GA Reg 0D - Comm. command (Main -> Sub)
 * @ingroup sub_ga_regs
 * @sa _GA_COMCMD5
 *
 * @details R: 16 bit data
 */
#define GA_COMCMD5 ((u16 volatile const *) _GA_COMCMD5)

/**
 * @def GA_COMCMD6
 * @brief GA Reg 0E - Comm. command (Main -> Sub)
 * @ingroup sub_ga_regs
 * @sa _GA_COMCMD6
 *
 * @details R: 16 bit data
 */
#define GA_COMCMD6 ((u16 volatile const *) _GA_COMCMD6)

/**
 * @def GA_COMCMD7
 * @brief GA Reg 0F - Comm. command (Main -> Sub)
 * @ingroup sub_ga_regs
 * @sa _GA_COMCMD7
 *
 * @details R: 16 bit data
 */
#define GA_COMCMD7 ((u16 volatile const *) _GA_COMCMD7)

/**
 * @def GA_COMSTAT0
 * @brief GA Reg 10 - Comm. status (Sub -> Main)
 * @ingroup sub_ga_regs
 * @sa _GA_COMSTAT0
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT0 ((u16 volatile *) _GA_COMSTAT0)

/**
 * @def GA_COMSTAT1
 * @brief GA Reg 11 - Comm. status (Sub -> Main)
 * @ingroup sub_ga_regs
 * @sa _GA_COMSTAT1
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT1 ((u16 volatile *) _GA_COMSTAT1)

/**
 * @def GA_COMSTAT2
 * @brief GA Reg 12 - Comm. status (Sub -> Main)
 * @ingroup sub_ga_regs
 * @sa _GA_COMSTAT2
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT2 ((u16 volatile *) _GA_COMSTAT2)

/**
 * @def GA_COMSTAT3
 * @brief GA Reg 13 - Comm. status (Sub -> Main)
 * @ingroup sub_ga_regs
 * @sa _GA_COMSTAT3
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT3 ((u16 volatile *) _GA_COMSTAT3)

/**
 * @def GA_COMSTAT4
 * @brief GA Reg 14 - Comm. status (Sub -> Main)
 * @ingroup sub_ga_regs
 * @sa _GA_COMSTAT4
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT4 ((u16 volatile *) _GA_COMSTAT4)

/**
 * @def GA_COMSTAT5
 * @brief GA Reg 15 - Comm. status (Sub -> Main)
 * @ingroup sub_ga_regs
 * @sa _GA_COMSTAT5
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT5 ((u16 volatile *) _GA_COMSTAT5)

/**
 * @def GA_COMSTAT6
 * @brief GA Reg 16 - Comm. status (Sub -> Main)
 * @ingroup sub_ga_regs
 * @sa _GA_COMSTAT6
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT6 ((u16 volatile *) _GA_COMSTAT6)

/**
 * @def GA_COMSTAT7
 * @brief GA Reg 17 - Comm. status (Sub -> Main)
 * @ingroup sub_ga_regs
 * @sa _GA_COMSTAT7
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT7 ((u16 volatile *) _GA_COMSTAT7)

/**
 * @def GA_INT3TIMER
 * @brief GA Reg 18 - Timer with interrupt
 * @sa _GA_INT3TIMER
 * @ingroup sub_ga_regs
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||||TD7|TD6|TD5|TD4|TD3|TD2|TD1|TD0|
 *
 * TD: Timer countdown value
 * @details The time ticks every 30.72 microseconds, continuously counting down
 * from the value inthis register until zero. When reaching zero, it triggers
 * a level 3 interrupt. If the value set is 0, no interrupt is generated.
 *
 * Note: The hardware manual states interval for interrupt is actually:
 * (n + 1) * 30.72
 * (probably to account for a 0 index)
 * This does not seem to properly reflected in emulators, but initial tests on
 * hardware show it to be accurate
 */
#define GA_INT3TIMER ((u16 volatile *) _GA_INT3TIMER)

/**
 * @def GA_INTMASK
 * @brief GA Reg 19 - Interrupt mask control
 * @sa _GA_INTMASK
 * @ingroup sub_ga_regs
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | |||||||||IEN6|IEN5|IEN4|IEN3|IEN2|IEN1| |
 *
 * IEN: Interrupt levels 1 to 6
 * @details RW: 1 - enable, 0 - disable
 *
 * @details
 * Interrupts:
 *   1: GFX
 *   2: MAIN
 *   3: TIMER
 *   4: CDD
 *   5: CDC
 *   6: SUBCODE
 */
#define GA_INTMASK ((u16 volatile *) _GA_INTMASK)

/**
 * @def GA_CDFADER
 * @brief GA Reg 1A - CD Audio Fader
 * @sa _GA_CDFADER
 * @ingroup sub_ga_regs
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |EFDT|FD10|FD09|FD08|FD07|FD06|FD05|FD04|FD03|FD02|FD01|FD00|DEF1|DEF0|SSF| |
 *
 * FD: Fade volume data (bits 4 to 14)
 * @details W: Volume data; see documentation for details on fade rate
 *
 * EFDT: End of Fade Data Transfer
 * @details R: 1 - busy; fade data transferring; 0 - ready; data can be set
 *
 * DEF: De-Emphasis Flag
 * @details: W: Set the emphasis flags; this is usually set to off as it is
 * only used by some old classical CDs, per the documentation. See said
 * documentation for more details.
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using the FDRSET and
 * FDRCHG BIOS functions.
 */
#define GA_CDFADER ((u16 volatile *) _GA_CDFADER)

/**
 * @def GA_CDDCTRL
 * @brief GA Reg 1B - CDD Control
 * @sa _GA_CDDCTRL
 * @ingroup sub_ga_regs
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | |||||||D/M| |||||HOCK|DRS|DTS|
 *
 * DTS: Data Transfer Status
 * @details R: 1 - indicates data is being transferr from the communications
 * buffer to the CDD; W: 0 - abort the communication transfer (only 0 can be
 * written to this bit)
 *
 * DRS: Data Receive Status
 * @details R: 1 - indicates data is being transferr from the CDD to the
 * communications buffer; W: 0 - abort the communication transfer (only 0 can
 * be written to this bit)
 *
 * HOCK: Host Clock
 * @details: W: 1 - Starts communication with the CDD
 *
 * D/M: Data/Music
 * @details: R: 0 - CDD data is audio data; 1 - CDD data is ROM data
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCTRL ((u16 volatile *) _GA_CDDCTRL)

/**
 * @def GA_CDDCOMM0
 * @brief GA Reg 1C - CDD Communication
 * @sa _GA_CDDCOMM0
 * @ingroup sub_ga_regs
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM0 ((u16 volatile *) _GA_CDDCOMM0)

/**
 * @def GA_CDDCOMM1
 * @brief GA Reg 1D - CDD Communication
 * @sa _GA_CDDCOMM1
 * @ingroup sub_ga_regs
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM1 ((u16 volatile *) _GA_CDDCOMM1)

/**
 * @def GA_CDDCOMM2
 * @brief GA Reg 1E - CDD Communication
 * @sa _GA_CDDCOMM2
 * @ingroup sub_ga_regs
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM2 ((u16 volatile *) _GA_CDDCOMM2)

/**
 * @def GA_CDDCOMM3
 * @brief GA Reg 1F - CDD Communication
 * @sa _GA_CDDCOMM3
 * @ingroup sub_ga_regs
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM3 ((u16 volatile *) _GA_CDDCOMM3)

/**
 * @def GA_CDDCOMM4
 * @brief GA Reg 20 - CDD Communication
 * @sa _GA_CDDCOMM4
 * @ingroup sub_ga_regs
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM4 ((u16 volatile *) _GA_CDDCOMM4)

/**
 * @def GA_CDDCOMM5
 * @brief GA Reg 21 - CDD Communication
 * @sa _GA_CDDCOMM5
 * @ingroup sub_ga_regs
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM5 ((u16 volatile *) _GA_CDDCOMM5)

/**
 * @def GA_CDDCOMM6
 * @brief GA Reg 22 - CDD Communication
 * @sa _GA_CDDCOMM6
 * @ingroup sub_ga_regs
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM6 ((u16 volatile *) _GA_CDDCOMM6)

/**
 * @def GA_CDDCOMM7
 * @brief GA Reg 23 - CDD Communication
 * @sa _GA_CDDCOMM7
 * @ingroup sub_ga_regs
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM7 ((u16 volatile *) _GA_CDDCOMM7)

/**
 * @def GA_CDDCOMM8
 * @brief GA Reg 24 - CDD Communication
 * @sa _GA_CDDCOMM8
 * @ingroup sub_ga_regs
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM8 ((u16 volatile *) _GA_CDDCOMM8)

/**
 * @def GA_CDDCOMM9
 * @brief GA Reg 25 - CDD Communication
 * @sa _GA_CDDCOMM9
 * @ingroup sub_ga_regs
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM9 ((u16 volatile *) _GA_CDDCOMM9)

/**
 * @def GA_FONTCOLOR
 * @brief GA Reg 26 - Font Color
 * @sa _GA_FONTCOLOR
 * @ingroup sub_ga_regs
 *
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||||SC13|SC12|SC11|SC10|SC03|SC02|SC01|SC00|
 *
 * SC00-03: Source Color Data for font bits set to 0
 * @details RW: Palette entry
 *
 * SC10-13: Source Color Data for font bits set to 1
 * @details RW: Palette entry
 */
#define GA_FONTCOLOR ((u16 volatile *) _GA_FONTCOLOR)

/**
 * @def GA_FONTBITS
 * @brief GA Reg 27 - 1bpp Font Data
 * @sa _GA_FONTBITS
 * @ingroup sub_ga_regs
 */
#define GA_FONTBITS ((u16 volatile *) _GA_FONTBITS)

/**
 * @def GA_FONTDATA
 * @brief GA Reg 28 - 4bpp Font Data
 * @sa _GA_FONTDATA
 * @ingroup sub_ga_regs
 *
 * @details
 * This register is a group of four 16-bit values which represent
 * the 4bpp VDP tile data converted from the 1bpp data in @ref GA_FONTBITS
 */
#define GA_FONTDATA ((u16 volatile *) _GA_FONTDATA)

/**
 * @def GA_STAMPSIZE
 * @brief GA Reg 29
 * @sa _GA_STAMPSIZE
 * @ingroup sub_ga_regs
 */
#define GA_STAMPSIZE ((u16 volatile *) _GA_STAMPSIZE)

/**
 * @def GA_STAMPMAPBASE
 * @brief GA Reg 2A
 * @sa _GA_STAMPMAPBASE
 * @ingroup sub_ga_regs
 */
#define GA_STAMPMAPBASE ((u16 volatile *) _GA_STAMPMAPBASE)

/**
 * @def GA_IMGBUFVSIZE
 * @brief GA Reg 2B
 * @sa _GA_IMGBUFVSIZE
 * @ingroup sub_ga_regs
 */
#define GA_IMGBUFVSIZE ((u16 volatile *) _GA_IMGBUFVSIZE)

/**
 * @def GA_IMGBUFSTART
 * @brief GA Reg 2C
 * @sa _GA_IMGBUFSTART
 * @ingroup sub_ga_regs
 */
#define GA_IMGBUFSTART ((u16 volatile *) _GA_IMGBUFSTART)

/**
 * @def GA_IMGBUFOFFSET
 * @brief GA Reg 2D
 * @sa _GA_IMGBUFOFFSET
 * @ingroup sub_ga_regs
 */
#define GA_IMGBUFOFFSET ((u16 volatile *) _GA_IMGBUFOFFSET)

/**
 * @def GA_IMGBUFHDOTSIZE
 * @brief GA Reg 2E
 * @sa _GA_IMGBUFHDOTSIZE
 * @ingroup sub_ga_regs
 */
#define GA_IMGBUFHDOTSIZE ((u16 volatile *) _GA_IMGBUFHDOTSIZE)

/**
 * @def GA_IMGBUFVDOTSIZE
 * @brief GA Reg 2F
 * @sa _GA_IMGBUFVDOTSIZE
 * @ingroup sub_ga_regs
 */
#define GA_IMGBUFVDOTSIZE ((u16 volatile *) _GA_IMGBUFVDOTSIZE)

/**
 * @def GA_TRACEVECTBASE
 * @brief GA Reg 30
 * @sa _GA_TRACEVECTBASE
 * @ingroup sub_ga_regs
 */
#define GA_TRACEVECTBASE ((u16 volatile *) _GA_TRACEVECTBASE)

/**
 * @def GA_SUBCODEADDR
 * @brief GA Reg 31
 * @sa _GA_SUBCODEADDR
 * @ingroup sub_ga_regs
 */
#define GA_SUBCODEADDR ((u16 volatile *) _GA_SUBCODEADDR)

/**
 * @def GA_SUBCODEBUF
 * @brief GA Reg 32 - Sub-code data buffer
 * @sa _GA_SUBCODEBUF
 * @ingroup sub_ga_regs
 *
 * @details
 * Size: 64 x 16-bit words (128 bytes)
 */
#define GA_SUBCODEBUF ((u16 volatile *) _GA_SUBCODEBUF)

/**
 * @def GA_SUBCODEBUFIMG
 * @brief GA Reg 33
 * @sa _GA_SUBCODEBUFIMG
 * @ingroup sub_ga_regs
 */
#define GA_SUBCODEBUFIMG ((u16 volatile *) _GA_SUBCODEBUFIMG)

/**
 * @fn wait_2m
 * @brief Wait for Sub CPU access to 2M Word RAM
 */
static inline void wait_2m()
{
	asm(
		"\
		1:btst %0, %p1 \n\
			beq 1b \n\
		"
		:
		: "i"(BIT_GA_DMNA), "i"(_GA_MEMMODE + 1));
}

/**
 * @fn grant_2m
 * @brief Grant 2M Word RAM access to the Main CPU and wait for confirmation
 */
static inline void grant_2m()
{
	asm(
		"\
		1:bset %0, %p1 \n\
			btst %0, %p1 \n\
			beq 1b \n\
		"
		:
		: "i"(BIT_GA_RET), "i"(_GA_MEMMODE + 1));
}

/**
 * @fn set_1m
 * @brief Set Word RAM layout to 1M/1M
 */
static inline void set_1m()
{
	asm(
		"\
		1:bset %0, %p1 \n\
			btst %0, %p1 \n\
			beq 1b \n\
		"
		:
		: "i"(BIT_GA_MODE), "i"(_GA_MEMMODE + 1));
}

/**
 * @fn set_2m
 * @brief Set Word RAM layout to 2M
 */
static inline void set_2m()
{
	asm(
		"\
		1:bclr %0, %p1 \n\
			btst %0, %p1 \n\
			bne 1b \n\
		"
		:
		: "i"(BIT_GA_MODE), "i"(_GA_MEMMODE + 1));
}

/**
 * @fn clear_comm_regs
 * Clears the comm status registers (COMSTAT) and flags (COMFLAGS)
 */
static inline void clear_comm_regs()
{
	asm(
		"\
		lea %p0, a0 \n\
		moveq #0, d0 \n\
		move.b d0, -0x11(a0) \n\
		move.l d0, (a0)+ \n\
		move.l d0, (a0)+ \n\
		move.l d0, (a0)+ \n\
		move.l d0, (a0)+ \n\
		"
		:
		: "i"(_GA_COMSTAT0)
		: "d0", "a0");
}

#endif
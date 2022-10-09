/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gatearr_def.h
 * @brief Sub CPU side Gate Array registers
 */

#ifndef MEGADEV__SUB_GATEARR_DEF_H
#define MEGADEV__SUB_GATEARR_DEF_H

/**
 * @defgroup sub_ga_regs Sub CPU Gate Array Registers
 */

/**
 * @def _GA_RESET
 * @sa GA_RESET
 * @ingroup sub_ga_regs
 */
#define _GA_RESET 0xFF8000

#define BIT_GA0_LEDR 0
#define BIT_GA0_LEDG 1

/**
 * @brief GA Reg 0 - LED Control bitmasks
 *
 */
#define MSK_GA0_LEDR 1 << BIT_GA0_LEDR
#define MSK_GA0_LEDG 1 << BIT_GA0_LEDG

/**
 * @def
 * @sa GA_MEMMODE
 * @ingroup sub_ga_regs
 */
#define _GA_MEMMODE 0xFF8002

// ******* _GA_MEMMODE+1
#define BIT_GA_RET 0
#define BIT_GA_DMNA 1
#define BIT_GA_MODE 2

/**
 * @def MSK_GA_RET
 * @brief Give 2M Word RAM to Main / Switch 1M Word RAM access [_GA_MEMMODE+1
 * bitmask]
 */
#define MSK_GA_RET (1 << BIT_GA_RET)

/**
 * @def MSK_GA_DMNA
 * @brief Main CPU will not access Word RAM [_GA_MEMMODE+1 bitmask]
 */
#define MSK_GA_DMNA (1 << BIT_GA_DMNA)

/**
 * @def MSK_GA_MODE
 * @brief Word RAM layout [_GA_MEMMODE+1 bitmask]
 */
#define MSK_GA_MODE (1 << BIT_GA_MODE)

/**
 * @def _GA_CDCMODE
 * @brief GA Reg 2 - CDC Mode / CDC Registers #1
 * @ingroup sub_ga_regs
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
#define _GA_CDCMODE 0xFF8004

/**
 * @def _GA_CDCRS1
 * @brief GA Reg 3 - CDC Registers #2
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||||CD7|CD6|CD5|CD4|CD3|CD2|CD1|CD0|
 *
 * @param CD CDC register data
 * @details RW: Details for this register can be found in the LC89510 manual
 */
#define _GA_CDCRS1 0xFF8006

/**
 * @def _GA_CDCHOSTDATA
 * @brief GA Reg 4 - CDC Host Data
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HD15|HD14|HD13|HD12|HD11|HD10|HD09|HD08|HD07|HD06|HD05|HD04|HD03|HD02|HD01|HD00|
 *
 * @param HD CDC read data
 * @details R: 2 bytes of data read by the CDC and ready to be transferred to
 * Main or Sub CPU memory
 */
#define _GA_CDCHOSTDATA 0xFF8008

/**
 * @def _GA_DMAADDR
 * @brief GA Reg 5 - CDC DMA Address
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
#define _GA_DMAADDR 0xFF800A

/**
 * @def _GA_STOPWATCH
 * @brief GA Reg 6 - Stopwatch
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
#define _GA_STOPWATCH 0xFF800C

/**
 * @def _GA_COMFLAGS
 * @brief GA Reg 7 - Main/Sub CPU communication flags
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |CFM7|CFM6|CFM5|CFM4|CFM3|CFM2|CFM1|CFM0|CFS7|CFS6|CFS5|CFS4|CFS3|CFS2|CFS1|CFS0|
 *
 * CFM: Comm flags for Main CPU
 * CFS: Comm flags for Sub CPU
 */
#define _GA_COMFLAGS 0xFF800E

/**
 * @def _GA_COMCMD0
 * @brief GA Reg 6 - Communication command (Main -> Sub)
 * @details R: 16 bit data
 */
#define _GA_COMCMD0 0xFF8010

/**
 * @def _GA_COMCMD1
 * @brief GA Reg 7 - Communication command (Main -> Sub)
 * @details R: 16 bit data
 */
#define _GA_COMCMD1 0xFF8012

/**
 * @def _GA_COMCMD2
 * @brief GA Reg 8 - Communication command (Main -> Sub)
 * @details R: 16 bit data
 */
#define _GA_COMCMD2 0xFF8014

/**
 * @def _GA_COMCMD3
 * @brief GA Reg 9 - Communication command (Main -> Sub)
 * @details R: 16 bit data
 */
#define _GA_COMCMD3 0xFF8016

/**
 * @def _GA_COMCMD4
 * @brief GA Reg A - Communication command (Main -> Sub)
 * @details R: 16 bit data
 */
#define _GA_COMCMD4 0xFF8018

/**
 * @def _GA_COMCMD5
 * @brief GA Reg B - Communication command (Main -> Sub)
 * @details R: 16 bit data
 */
#define _GA_COMCMD5 0xFF801A

/**
 * @def _GA_COMCMD6
 * @brief GA Reg C - Communication command (Main -> Sub)
 * @details R: 16 bit data
 */
#define _GA_COMCMD6 0xFF801C

/**
 * @def _GA_COMCMD7
 * @brief GA Reg D - Communication command (Main -> Sub)
 * @details R: 16 bit data
 */
#define _GA_COMCMD7 0xFF801E

/**
 * @def _GA_COMSTAT0
 * @brief GA Reg E - Communication status (Sub -> Main)
 * @details RW: 16 bit data
 */
#define _GA_COMSTAT0 0xFF8020

/**
 * @def _GA_COMSTAT1
 * @brief GA Reg F - Communication status (Sub -> Main)
 * @details RW: 16 bit data
 */
#define _GA_COMSTAT1 0xFF8022

/**
 * @def _GA_COMSTAT2
 * @brief GA Reg 10 - Communication status (Sub -> Main)
 * @details RW: 16 bit data
 */
#define _GA_COMSTAT2 0xFF8024

/**
 * @def _GA_COMSTAT3
 * @brief GA Reg 11 - Communication status (Sub -> Main)
 * @details RW: 16 bit data
 */
#define _GA_COMSTAT3 0xFF8026

/**
 * @def _GA_COMSTAT4
 * @brief GA Reg 12 - Communication status (Sub -> Main)
 * @details RW: 16 bit data
 */
#define _GA_COMSTAT4 0xFF8028

/**
 * @def _GA_COMSTAT5
 * @brief GA Reg 13 - Communication status (Sub -> Main)
 * @details RW: 16 bit data
 */
#define _GA_COMSTAT5 0xFF802A

/**
 * @def _GA_COMSTAT6
 * @brief GA Reg 14 - Communication status (Sub -> Main)
 * @details RW: 16 bit data
 */
#define _GA_COMSTAT6 0xFF802C

/**
 * @def _GA_COMSTAT7
 * @brief GA Reg 15 - Communication status (Sub -> Main)
 * @details RW: 16 bit data
 */
#define _GA_COMSTAT7 0xFF802E

/**
 * @def _GA_INT3TIMER
 * @brief GA Reg 16 - Timer with interrupt
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||||TD7|TD6|TD5|TD4|TD3|TD2|TD1|TD0|
 *
 * TD: Timer countdown value
 * @details The time ticks every 30.72 microseconds, continuously counting down
 * from the value inthis register until zero. When reaching zero, it triggers
 * a level 3 interrupt. If the value set is 0, no interrupt is generated.
 */
#define _GA_INT3TIMER 0xFF8030

/**
 * @def _GA_INTMASK
 * @brief GA Reg 17 - Interrupt mask control
 * @details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | |||||||||IEN6|IEN5|IEN4|IEN3|IEN2|IEN1| |
 *
 * IEN: Interrupt levels 1 to 6
 * @details RW: 1 - enable, 0 - disable
 *
 * Note: The hardware manual states interval for interrupt is actually:
 * (n + 1) * 30.72
 * (probably to account for a 0 index)
 * This does not seem to properly reflected in emulators, but initial tests on
 * hardware show it to be accurate
 */
#define _GA_INTMASK 0xFF8032

/**
 * GA_INTMASK bit/mask settings
 */
#define BIT_INT1_GFX 1
#define BIT_INT2_MD 2
#define BIT_INT3_TIMER 3
#define BIT_INT4_CDD 4
#define BIT_INT5_CDC 5
#define BIT_INT6_SUBCODE 6

#define MSK_INT1_GFX 1 << BIT_INT1_GFX
#define MSK_INT2_MD 1 << BIT_INT2_MD
#define MSK_INT3_TIMER 1 << BIT_INT3_TIMER
#define MSK_INT4_CDD 1 << BIT_INT4_CDD
#define MSK_INT5_CDC 1 << BIT_INT5_CDC
#define MSK_INT6_SUBCODE 1 << BIT_INT6_SUBCODE

/**
 * @def _GA_CDFADER
 * @brief GA Reg 18 - CD Audio Fader
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
#define _GA_CDFADER 0xFF8034

/**
 * @def _GA_CDDCTRL
 * @brief GA Reg 19 - CDD Control
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
#define _GA_CDDCTRL 0xFF8036

/**
 * @def _GA_CDDCOMM0
 * @brief GA Reg 1A - CDD Communication
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define _GA_CDDCOMM0 0xFF8038

/**
 * @def _GA_CDDCOMM1
 * @brief GA Reg 1B - CDD Communication
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define _GA_CDDCOMM1 0xFF803A

/**
 * @def _GA_CDDCOMM2
 * @brief GA Reg 1C - CDD Communication
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define _GA_CDDCOMM2 0xFF803C

/**
 * @def _GA_CDDCOMM3
 * @brief GA Reg 1D - CDD Communication
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define _GA_CDDCOMM3 0xFF803E

/**
 * @def _GA_CDDCOMM4
 * @brief GA Reg 1E - CDD Communication
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define _GA_CDDCOMM4 0xFF8040

/**
 * @def _GA_CDDCOMM5
 * @brief GA Reg 1F - CDD Communication
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define _GA_CDDCOMM5 0xFF8042

/**
 * @def _GA_CDDCOMM6
 * @brief GA Reg 20 - CDD Communication
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define _GA_CDDCOMM6 0xFF8044

/**
 * @def _GA_CDDCOMM7
 * @brief GA Reg 21 - CDD Communication
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define _GA_CDDCOMM7 0xFF8046

/**
 * @def _GA_CDDCOMM8
 * @brief GA Reg 22 - CDD Communication
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define _GA_CDDCOMM8 0xFF8048

/**
 * @def _GA_CDDCOMM9
 * @brief GA Reg 23 - CDD Communication
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define _GA_CDDCOMM9 0xFF804A

/**
 * @def _GA_FONTCOLOR
 * @brief GA Reg 24 - Font Color
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
#define _GA_FONTCOLOR 0xFF804C

/**
 * @def _GA_FONTBITS
 * @brief GA Reg 25
 */
#define _GA_FONTBITS 0xFF804E

/**
 * @def _GA_FONTDATA
 * @brief GA Reg 26
 */
#define _GA_FONTDATA 0xFF8050

/**
 * @def _GA_STAMPSIZE
 * @brief GA Reg 27
 */
#define _GA_STAMPSIZE 0xFF8058

/**
 * @def _GA_STAMPMAPBASE
 * @brief GA Reg 28
 */
#define _GA_STAMPMAPBASE 0xFF805A

/**
 * @def _GA_IMGBUFVSIZE
 * @brief GA Reg 29
 */
#define _GA_IMGBUFVSIZE 0xFF805C

/**
 * @def _GA_IMGBUFSTART
 * @brief GA Reg 2A
 */
#define _GA_IMGBUFSTART 0xFF805E

/**
 * @def _GA_IMGBUFOFFSET
 * @brief GA Reg 2B
 */
#define _GA_IMGBUFOFFSET 0xFF8060

/**
 * @def _GA_IMGBUFHDOTSIZE
 * @brief GA Reg 2C
 */
#define _GA_IMGBUFHDOTSIZE 0xFF8062

/**
 * @def _GA_IMGBUFVDOTSIZE
 * @brief GA Reg 2D
 */
#define _GA_IMGBUFVDOTSIZE 0xFF8064

/**
 * @def _GA_TRACEVECTBASE
 * @brief GA Reg 2E
 */
#define _GA_TRACEVECTBASE 0xFF8066

/**
 * @def _GA_SUBCODEADDR
 * @brief GA Reg 2F
 */
#define _GA_SUBCODEADDR 0xFF8068

/**
 * @def _GA_SUBCODEBUF
 * @brief GA Reg 30
 */
#define _GA_SUBCODEBUF 0xFF8100

/**
 * @def _GA_SUBCODEBUFIMG
 * @brief GA Reg 31
 */
#define _GA_SUBCODEBUFIMG 0xFF8180

/**
 * GA_CDCMODE bit/mask settings
 */
#define BIT_CDCMODE_DD0 5
#define BIT_CDCMODE_DSR 6
#define BIT_CDCMODE_EDT 7

// these aren't right...
#define BIT_CDC_MAINREAD 2
#define BIT_CDC_SUBREAD 3
#define BIT_CDC_PCMDMA 4
#define BIT_CDC_PRAMDMA 5
#define BIT_CDC_WRAMDMA 7

#define CDCMODE_CABITS 0x000F
#define CDCMODE_DDBITS 0x0700
#define MSK_CDCMODE_DD0 1 << BIT_CDCMODE_DD0
#define MSK_CDCMODE_DSR 1 << BIT_CDCMODE_DSR
#define MSK_CDCMODE_EDT 1 << BIT_CDCMODE_EDT
#define MSK_CDCMODE_MAINREAD 1 << BIT_CDC_MAINREAD
#define MSK_CDCMODE_SUBREAD 1 << BIT_CDC_SUBREAD
#define MSK_CDCMODE_PCMDMA 1 << BIT_CDC_PCMDMA
#define MSK_CDCMODE_PRAMDMA 1 << BIT_CDC_PRAMDMA
#define MSK_CDCMODE_WRAMDMA 1 << BIT_CDC_WRAMDMA

#define CDC_DEST_MAINREAD 2
#define CDC_DEST_SUBREAD 3
#define CDC_DEST_PCMDMA 4
#define CDC_DEST_PRAMDMA 5
#define CDC_DEST_WRAMDMA 7

#endif

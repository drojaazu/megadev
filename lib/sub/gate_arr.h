/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gate_arr.h
 * @brief C wrappers for Sub CPU side Gate Array registers
 */

#ifndef MEGADEV__SUB_GATEARR_H
#define MEGADEV__SUB_GATEARR_H

#include "gate_arr.def.h"
#include <types.h>

typedef u16 volatile * ga_reg;

#define ga_reset ((ga_reg) GAREG_RESET)

#define ga_memmode ((ga_reg) GAREG_MEMMODE)

#define ga_cdcmode ((ga_reg) GAREG_CDCMODE)

#define GA_CDCRS1 ((ga_reg) _GAREG_CDCRS1)

#define ga_cdchostdata ((ga_reg) GAREG_CDCHOSTDATA)

#define GA_DMAADDR ((ga_reg) _GAREG_DMAADDR)

#define ga_stopwatch ((ga_reg) GAREG_STOPWATCH)

#define ga_comflags ((ga_reg const) GAREG_COMFLAGS)

#define GA_COMFLAGS_MAIN ((volatile const u8 *) GAREG_COMFLAGS)
#define GA_COMFLAGS_SUB	 ((volatile u8 *) GAREG_COMFLAGS + 1)

#define gareg_comcmd0 ((ga_reg const) GAREG_COMCMD0)

/**
 * @def gareg_comcmd1
 * @brief GA Reg 09 - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GAREG_COMCMD1
 *
 * @details R: 16 bit data
 */
#define gareg_comcmd1 ((ga_reg const) GAREG_COMCMD1)

/**
 * @def gareg_comcmd2
 * @brief GA Reg 0A - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GAREG_COMCMD2
 *
 * @details R: 16 bit data
 */
#define gareg_comcmd2 ((ga_reg const) GAREG_COMCMD2)

/**
 * @def gareg_comcmd3
 * @brief GA Reg 0B - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GAREG_COMCMD3
 *
 * @details R: 16 bit data
 */
#define gareg_comcmd3 ((ga_reg const) GAREG_COMCMD3)

/**
 * @def gareg_comcmd4
 * @brief GA Reg 0C - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GAREG_COMCMD4
 *
 * @details R: 16 bit data
 */
#define gareg_comcmd4 ((ga_reg const) GAREG_COMCMD4)

/**
 * @def gareg_comcmd5
 * @brief GA Reg 0D - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GAREG_COMCMD5
 *
 * @details R: 16 bit data
 */
#define gareg_comcmd5 ((ga_reg const) GAREG_COMCMD5)

/**
 * @def gareg_comcmd6
 * @brief GA Reg 0E - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GAREG_COMCMD6
 *
 * @details R: 16 bit data
 */
#define gareg_comcmd6 ((ga_reg const) GAREG_COMCMD6)

/**
 * @def gareg_comcmd7
 * @brief GA Reg 0F - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GAREG_COMCMD7
 *
 * @details R: 16 bit data
 */
#define gareg_comcmd7 ((ga_reg const) GAREG_COMCMD7)

/**
 * @def GA_COMSTAT0
 * @brief GA Reg 10 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GAREG_COMSTAT0
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT0 ((ga_reg) GAREG_COMSTAT0)

/**
 * @def GA_COMSTAT1
 * @brief GA Reg 11 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GAREG_COMSTAT1
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT1 ((ga_reg) GAREG_COMSTAT1)

/**
 * @def GA_COMSTAT2
 * @brief GA Reg 12 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GAREG_COMSTAT2
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT2 ((ga_reg) GAREG_COMSTAT2)

/**
 * @def GA_COMSTAT3
 * @brief GA Reg 13 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GAREG_COMSTAT3
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT3 ((ga_reg) GAREG_COMSTAT3)

/**
 * @def GA_COMSTAT4
 * @brief GA Reg 14 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GAREG_COMSTAT4
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT4 ((ga_reg) GAREG_COMSTAT4)

/**
 * @def GA_COMSTAT5
 * @brief GA Reg 15 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GAREG_COMSTAT5
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT5 ((ga_reg) GAREG_COMSTAT5)

/**
 * @def GA_COMSTAT6
 * @brief GA Reg 16 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GAREG_COMSTAT6
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT6 ((ga_reg) GAREG_COMSTAT6)

/**
 * @def GA_COMSTAT7
 * @brief GA Reg 17 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GAREG_COMSTAT7
 *
 * @details RW: 16 bit data
 */
#define GA_COMSTAT7 ((ga_reg) GAREG_COMSTAT7)

/**
 * @def GA_INT3TIMER
 * @brief GA Reg 18 - Timer with interrupt
 * @sa _GAREG_INT3TIMER
 * @ingroup gatearray_sub
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
#define GA_INT3TIMER ((ga_reg) _GAREG_INT3TIMER)

/**
 * @def GA_INTMASK
 * @brief GA Reg 19 - Interrupt mask control
 * @sa _GAREG_INTMASK
 * @ingroup gatearray_sub
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
#define GA_INTMASK ((ga_reg) _GAREG_INTMASK)

/**
 * @def GA_CDFADER
 * @brief GA Reg 1A - CD Audio Fader
 * @sa _GAREG_CDFADER
 * @ingroup gatearray_sub
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
 * for direct access from user applications. Consider using the FDR_SET and
 * FDR_CHG BIOS functions.
 */
#define GA_CDFADER ((ga_reg) _GAREG_CDFADER)

/**
 * @def GA_CDDCTRL
 * @brief GA Reg 1B - CDD Control
 * @sa _GAREG_CDDCTRL
 * @ingroup gatearray_sub
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
#define GA_CDDCTRL ((ga_reg) _GAREG_CDDCTRL)

/**
 * @def GA_CDDCOMM0
 * @brief GA Reg 1C - CDD Communication
 * @sa _GAREG_CDDCOMM0
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM0 ((ga_reg) _GAREG_CDDCOMM0)

/**
 * @def GA_CDDCOMM1
 * @brief GA Reg 1D - CDD Communication
 * @sa _GAREG_CDDCOMM1
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM1 ((ga_reg) _GAREG_CDDCOMM1)

/**
 * @def GA_CDDCOMM2
 * @brief GA Reg 1E - CDD Communication
 * @sa _GAREG_CDDCOMM2
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM2 ((ga_reg) _GAREG_CDDCOMM2)

/**
 * @def GA_CDDCOMM3
 * @brief GA Reg 1F - CDD Communication
 * @sa _GAREG_CDDCOMM3
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM3 ((ga_reg) _GAREG_CDDCOMM3)

/**
 * @def GA_CDDCOMM4
 * @brief GA Reg 20 - CDD Communication
 * @sa _GAREG_CDDCOMM4
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM4 ((ga_reg) _GAREG_CDDCOMM4)

/**
 * @def GA_CDDCOMM5
 * @brief GA Reg 21 - CDD Communication
 * @sa _GAREG_CDDCOMM5
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM5 ((ga_reg) _GAREG_CDDCOMM5)

/**
 * @def GA_CDDCOMM6
 * @brief GA Reg 22 - CDD Communication
 * @sa _GAREG_CDDCOMM6
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM6 ((ga_reg) _GAREG_CDDCOMM6)

/**
 * @def GA_CDDCOMM7
 * @brief GA Reg 23 - CDD Communication
 * @sa _GAREG_CDDCOMM7
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM7 ((ga_reg) _GAREG_CDDCOMM7)

/**
 * @def GA_CDDCOMM8
 * @brief GA Reg 24 - CDD Communication
 * @sa _GAREG_CDDCOMM8
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM8 ((ga_reg) _GAREG_CDDCOMM8)

/**
 * @def GA_CDDCOMM9
 * @brief GA Reg 25 - CDD Communication
 * @sa _GAREG_CDDCOMM9
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define GA_CDDCOMM9 ((ga_reg) _GAREG_CDDCOMM9)

/**
 * @def GA_FONTCOLOR
 * @brief GA Reg 26 - Font Color
 * @sa _GAREG_FONTCOLOR
 * @ingroup gatearray_sub
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
#define GA_FONTCOLOR ((ga_reg) _GAREG_FONTCOLOR)

/**
 * @def GA_FONTBITS
 * @brief GA Reg 27 - 1bpp Font Data
 * @sa _GAREG_FONTBITS
 * @ingroup gatearray_sub
 */
#define GA_FONTBITS ((ga_reg) _GAREG_FONTBITS)

/**
 * @def GA_FONTDATA
 * @brief GA Reg 28 - 4bpp Font Data
 * @sa _GAREG_FONTDATA
 * @ingroup gatearray_sub
 *
 * @details
 * This register is a group of four 16-bit values which represent
 * the 4bpp VDP tile data converted from the 1bpp data in @ref GA_FONTBITS
 */
#define GA_FONTDATA ((ga_reg) _GAREG_FONTDATA)

/**
 * @def GA_STAMPSIZE
 * @brief GA Reg 29
 * @sa _GAREG_STAMPSIZE
 * @ingroup gatearray_sub
 */
#define GA_STAMPSIZE (*((ga_reg) _GAREG_STAMPSIZE))

/**
 * @def GA_STAMPMAPBASE
 * @brief GA Reg 2A
 * @sa _GAREG_STAMPMAPBASE
 * @ingroup gatearray_sub
 */
#define GA_STAMPMAPBASE (*((ga_reg) _GAREG_STAMPMAPBASE))

/**
 * @def GA_IMGBUFVSIZE
 * @brief GA Reg 2B
 * @sa _GAREG_IMGBUFVSIZE
 * @ingroup gatearray_sub
 */
#define GA_IMGBUFVSIZE (*((ga_reg) _GAREG_IMGBUFVSIZE))

/**
 * @def GA_IMGBUFSTART
 * @brief GA Reg 2C
 * @sa _GAREG_IMGBUFSTART
 * @ingroup gatearray_sub
 */
#define GA_IMGBUFSTART (*((ga_reg) _GAREG_IMGBUFSTART))

/**
 * @def GA_IMGBUFOFFSET
 * @brief GA Reg 2D
 * @sa _GAREG_IMGBUFOFFSET
 * @ingroup gatearray_sub
 */
#define GA_IMGBUFOFFSET (*((ga_reg) _GAREG_IMGBUFOFFSET))

/**
 * @def GA_IMGBUFHDOTSIZE
 * @brief GA Reg 2E
 * @sa _GAREG_IMGBUFHDOTSIZE
 * @ingroup gatearray_sub
 */
#define GA_IMGBUFHDOTSIZE (*((ga_reg) _GAREG_IMGBUFHDOTSIZE))

/**
 * @def GA_IMGBUFVDOTSIZE
 * @brief GA Reg 2F
 * @sa _GAREG_IMGBUFVDOTSIZE
 * @ingroup gatearray_sub
 */
#define GA_IMGBUFVDOTSIZE (*((ga_reg) _GAREG_IMGBUFVDOTSIZE))

/**
 * @def GA_TRACEVECTBASE
 * @brief GA Reg 30
 * @sa _GAREG_TRACEVECTBASE
 * @ingroup gatearray_sub
 */
#define GA_TRACEVECTBASE (*((ga_reg) _GAREG_TRACEVECTBASE))

/**
 * @def GA_SUBCODEADDR
 * @brief GA Reg 31
 * @sa _GAREG_SUBCODEADDR
 * @ingroup gatearray_sub
 */
#define GA_SUBCODEADDR ((ga_reg) _GAREG_SUBCODEADDR)

/**
 * @def GA_SUBCODEBUF
 * @brief GA Reg 32 - Sub-code data buffer
 * @sa _GAREG_SUBCODEBUF
 * @ingroup gatearray_sub
 *
 * @details
 * Size: 64 x 16-bit words (128 bytes)
 */
#define GA_SUBCODEBUF ((ga_reg) _GAREG_SUBCODEBUF)

/**
 * @def GA_SUBCODEBUFIMG
 * @brief GA Reg 33
 * @sa _GAREG_SUBCODEBUFIMG
 * @ingroup gatearray_sub
 */
#define GA_SUBCODEBUFIMG ((ga_reg) _GAREG_SUBCODEBUFIMG)

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
		: "i"(BIT_GAREG_DMNA), "i"(GAREG_MEMMODE + 1));
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
		: "i"(BIT_GAREG_RET), "i"(GAREG_MEMMODE + 1));
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
		: "i"(BIT_GAREG_MODE), "i"(GAREG_MEMMODE + 1));
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
		: "i"(BIT_GAREG_MODE), "i"(GAREG_MEMMODE + 1));
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
		: "i"(GAREG_COMSTAT0)
		: "d0", "a0");
}

#endif
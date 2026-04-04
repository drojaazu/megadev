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

#define ga_reg_reset ((ga_reg) GA_REG_RESET)

#define ga_reg_memmode ((ga_reg) GA_REG_MEMMODE)

#define ga_reg_cdcmode ((ga_reg) GA_REG_CDCMODE)

#define ga_reg_cdcrs1 ((ga_reg) GA_REG_CDCRS1)

#define ga_reg_cdchostdata ((ga_reg) GA_REG_CDCHOSTDATA)

#define ga_reg_dmaaddr ((ga_reg) GA_REG_DMAADDR)

#define ga_reg_stopwatch ((ga_reg) GA_REG_STOPWATCH)

#define ga_reg_comflags ((ga_reg const) GA_REG_COMFLAGS)

#define ga_reg_comflags_main ((volatile const u8 *) GA_REG_COMFLAGS)

#define ga_reg_comflags_sub ((volatile u8 *) GA_REG_COMFLAGS + 1)

#define ga_reg_comcmd0 ((ga_reg const) GA_REG_COMCMD0)

/**
 * @def ga_reg_comcmd1
 * @brief GA Reg 09 - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMCMD1
 *
 * @details R: 16 bit data
 */
#define ga_reg_comcmd1 ((ga_reg const) GA_REG_COMCMD1)

/**
 * @def ga_reg_comcmd2
 * @brief GA Reg 0A - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMCMD2
 *
 * @details R: 16 bit data
 */
#define ga_reg_comcmd2 ((ga_reg const) GA_REG_COMCMD2)

/**
 * @def ga_reg_comcmd3
 * @brief GA Reg 0B - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMCMD3
 *
 * @details R: 16 bit data
 */
#define ga_reg_comcmd3 ((ga_reg const) GA_REG_COMCMD3)

/**
 * @def ga_reg_comcmd4
 * @brief GA Reg 0C - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMCMD4
 *
 * @details R: 16 bit data
 */
#define ga_reg_comcmd4 ((ga_reg const) GA_REG_COMCMD4)

/**
 * @def ga_reg_comcmd5
 * @brief GA Reg 0D - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMCMD5
 *
 * @details R: 16 bit data
 */
#define ga_reg_comcmd5 ((ga_reg const) GA_REG_COMCMD5)

/**
 * @def ga_reg_comcmd6
 * @brief GA Reg 0E - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMCMD6
 *
 * @details R: 16 bit data
 */
#define ga_reg_comcmd6 ((ga_reg const) GA_REG_COMCMD6)

/**
 * @def ga_reg_comcmd7
 * @brief GA Reg 0F - Comm. command (Main -> Sub)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMCMD7
 *
 * @details R: 16 bit data
 */
#define ga_reg_comcmd7 ((ga_reg const) GA_REG_COMCMD7)

/**
 * @def ga_reg_comstat0
 * @brief GA Reg 10 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMSTAT0
 *
 * @details RW: 16 bit data
 */
#define ga_reg_comstat0 ((ga_reg) GA_REG_COMSTAT0)

/**
 * @def ga_reg_comstat1
 * @brief GA Reg 11 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMSTAT1
 *
 * @details RW: 16 bit data
 */
#define ga_reg_comstat1 ((ga_reg) GA_REG_COMSTAT1)

/**
 * @def ga_reg_comstat2
 * @brief GA Reg 12 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMSTAT2
 *
 * @details RW: 16 bit data
 */
#define ga_reg_comstat2 ((ga_reg) GA_REG_COMSTAT2)

/**
 * @def ga_reg_comstat3
 * @brief GA Reg 13 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMSTAT3
 *
 * @details RW: 16 bit data
 */
#define ga_reg_comstat3 ((ga_reg) GA_REG_COMSTAT3)

/**
 * @def ga_reg_comstat4
 * @brief GA Reg 14 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMSTAT4
 *
 * @details RW: 16 bit data
 */
#define ga_reg_comstat4 ((ga_reg) GA_REG_COMSTAT4)

/**
 * @def ga_reg_comstat5
 * @brief GA Reg 15 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMSTAT5
 *
 * @details RW: 16 bit data
 */
#define ga_reg_comstat5 ((ga_reg) GA_REG_COMSTAT5)

/**
 * @def ga_reg_comstat6
 * @brief GA Reg 16 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMSTAT6
 *
 * @details RW: 16 bit data
 */
#define ga_reg_comstat6 ((ga_reg) GA_REG_COMSTAT6)

/**
 * @def ga_reg_comstat7
 * @brief GA Reg 17 - Comm. status (Sub -> Main)
 * @ingroup gatearray_sub
 * @sa GA_REG_COMSTAT7
 *
 * @details RW: 16 bit data
 */
#define ga_reg_comstat7 ((ga_reg) GA_REG_COMSTAT7)

/**
 * @def ga_reg_int3timer
 * @brief GA Reg 18 - Timer with interrupt
 * @sa GA_REG_INT3TIMER
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
#define ga_reg_int3timer ((ga_reg) GA_REG_INT3TIMER)

/**
 * @def ga_reg_intmask
 * @brief GA Reg 19 - Interrupt mask control
 * @sa GA_REG_INTMASK
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
#define ga_reg_intmask ((ga_reg) GA_REG_INTMASK)

/**
 * @def ga_reg_cdfader
 * @brief GA Reg 1A - CD Audio Fader
 * @sa GA_REG_CDFADER
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
#define ga_reg_cdfader ((ga_reg) GA_REG_CDFADER)

/**
 * @def ga_reg_cddctrl
 * @brief GA Reg 1B - CDD Control
 * @sa GA_REG_CDDCTRL
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
#define ga_reg_cddctrl ((ga_reg) GA_REG_CDDCTRL)

/**
 * @def ga_reg_cddcomm0
 * @brief GA Reg 1C - CDD Communication
 * @sa GA_REG_CDDCOMM0
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define ga_reg_cddcomm0 ((ga_reg) GA_REG_CDDCOMM0)

/**
 * @def ga_reg_cddcomm1
 * @brief GA Reg 1D - CDD Communication
 * @sa GA_REG_CDDCOMM1
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define ga_reg_cddcomm1 ((ga_reg) GA_REG_CDDCOMM1)

/**
 * @def ga_reg_cddcomm2
 * @brief GA Reg 1E - CDD Communication
 * @sa GA_REG_CDDCOMM2
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define ga_reg_cddcomm2 ((ga_reg) GA_REG_CDDCOMM2)

/**
 * @def ga_reg_cddcomm3
 * @brief GA Reg 1F - CDD Communication
 * @sa GA_REG_CDDCOMM3
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define ga_reg_cddcomm3 ((ga_reg) GA_REG_CDDCOMM3)

/**
 * @def ga_reg_cddcomm4
 * @brief GA Reg 20 - CDD Communication
 * @sa GA_REG_CDDCOMM4
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define ga_reg_cddcomm4 ((ga_reg) GA_REG_CDDCOMM4)

/**
 * @def ga_reg_cddcomm5
 * @brief GA Reg 21 - CDD Communication
 * @sa GA_REG_CDDCOMM5
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define ga_reg_cddcomm5 ((ga_reg) GA_REG_CDDCOMM5)

/**
 * @def ga_reg_cddcomm6
 * @brief GA Reg 22 - CDD Communication
 * @sa GA_REG_CDDCOMM6
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define ga_reg_cddcomm6 ((ga_reg) GA_REG_CDDCOMM6)

/**
 * @def ga_reg_cddcomm7
 * @brief GA Reg 23 - CDD Communication
 * @sa GA_REG_CDDCOMM7
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define ga_reg_cddcomm7 ((ga_reg) GA_REG_CDDCOMM7)

/**
 * @def ga_reg_cddcomm8
 * @brief GA Reg 24 - CDD Communication
 * @sa GA_REG_CDDCOMM8
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define ga_reg_cddcomm8 ((ga_reg) GA_REG_CDDCOMM8)

/**
 * @def ga_reg_cddcomm9
 * @brief GA Reg 25 - CDD Communication
 * @sa GA_REG_CDDCOMM9
 * @ingroup gatearray_sub
 *
 * @details See the documentation for more info
 *
 * @details Official documentation indicates this register is not intended
 * for direct access from user applications. Consider using BIOS functions
 * for CD drive functionality.
 */
#define ga_reg_cddcomm9 ((ga_reg) GA_REG_CDDCOMM9)

/**
 * @def ga_reg_fontcolor
 * @brief GA Reg 26 - Font Color
 * @sa GA_REG_FONTCOLOR
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
#define ga_reg_fontcolor ((ga_reg) GA_REG_FONTCOLOR)

/**
 * @def ga_reg_fontbits
 * @brief GA Reg 27 - 1bpp Font Data
 * @sa GA_REG_FONTBITS
 * @ingroup gatearray_sub
 */
#define ga_reg_fontbits ((ga_reg) GA_REG_FONTBITS)

/**
 * @def ga_reg_fontdata
 * @brief GA Reg 28 - 4bpp Font Data
 * @sa GA_REG_FONTDATA
 * @ingroup gatearray_sub
 *
 * @details
 * This register is a group of four 16-bit values which represent
 * the 4bpp VDP tile data converted from the 1bpp data in @ref ga_reg_fontbits
 */
#define ga_reg_fontdata ((ga_reg) GA_REG_FONTDATA)

/**
 * @def ga_reg_stampsize
 * @brief GA Reg 29
 * @sa GA_REG_STAMPSIZE
 * @ingroup gatearray_sub
 */
#define ga_reg_stampsize (*((ga_reg) GA_REG_STAMPSIZE))

/**
 * @def ga_reg_stampmapbase
 * @brief GA Reg 2A
 * @sa GA_REG_STAMPMAPBASE
 * @ingroup gatearray_sub
 */
#define ga_reg_stampmapbase (*((ga_reg) GA_REG_STAMPMAPBASE))

/**
 * @def ga_reg_imgbufvsize
 * @brief GA Reg 2B
 * @sa GA_REG_IMGBUFVSIZE
 * @ingroup gatearray_sub
 */
#define ga_reg_imgbufvsize (*((ga_reg) GA_REG_IMGBUFVSIZE))

/**
 * @def ga_reg_imgbufstart
 * @brief GA Reg 2C
 * @sa GA_REG_IMGBUFSTART
 * @ingroup gatearray_sub
 */
#define ga_reg_imgbufstart (*((ga_reg) GA_REG_IMGBUFSTART))

/**
 * @def ga_reg_imgbufoffset
 * @brief GA Reg 2D
 * @sa GA_REG_IMGBUFOFFSET
 * @ingroup gatearray_sub
 */
#define ga_reg_imgbufoffset (*((ga_reg) GA_REG_IMGBUFOFFSET))

/**
 * @def ga_reg_imgbufhdotsize
 * @brief GA Reg 2E
 * @sa GA_REG_IMGBUFHDOTSIZE
 * @ingroup gatearray_sub
 */
#define ga_reg_imgbufhdotsize (*((ga_reg) GA_REG_IMGBUFHDOTSIZE))

/**
 * @def ga_reg_imgbufvdotsize
 * @brief GA Reg 2F
 * @sa GA_REG_IMGBUFVDOTSIZE
 * @ingroup gatearray_sub
 */
#define ga_reg_imgbufvdotsize (*((ga_reg) GA_REG_IMGBUFVDOTSIZE))

/**
 * @def ga_reg_tracevectbase
 * @brief GA Reg 30
 * @sa GA_REG_TRACEVECTBASE
 * @ingroup gatearray_sub
 */
#define ga_reg_tracevectbase (*((ga_reg) GA_REG_TRACEVECTBASE))

/**
 * @def ga_reg_subcodeaddr
 * @brief GA Reg 31
 * @sa GA_REG_SUBCODEADDR
 * @ingroup gatearray_sub
 */
#define ga_reg_subcodeaddr ((ga_reg) GA_REG_SUBCODEADDR)

/**
 * @def ga_reg_subcodebuf
 * @brief GA Reg 32 - Sub-code data buffer
 * @sa GA_REG_SUBCODEBUF
 * @ingroup gatearray_sub
 *
 * @details
 * Size: 64 x 16-bit words (128 bytes)
 */
#define ga_reg_subcodebuf ((ga_reg) GA_REG_SUBCODEBUF)

/**
 * @def ga_reg_subcodebufimg
 * @brief GA Reg 33
 * @sa GA_REG_SUBCODEBUFIMG
 * @ingroup gatearray_sub
 */
#define ga_reg_subcodebufimg ((ga_reg) GA_REG_SUBCODEBUFIMG)

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
    : "i"(BIT_GA_REG_DMNA), "i"(GA_REG_MEMMODE + 1));
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
    : "i"(BIT_GA_REG_RET), "i"(GA_REG_MEMMODE + 1));
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
    : "i"(BIT_GA_REG_MODE), "i"(GA_REG_MEMMODE + 1));
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
    : "i"(BIT_GA_REG_MODE), "i"(GA_REG_MEMMODE + 1));
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
    : "i"(GA_REG_COMSTAT0)
    : "d0", "a0");
}

#endif
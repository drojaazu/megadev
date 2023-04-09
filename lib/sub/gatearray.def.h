/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file gatearray.def.h
 * @brief Sub CPU side Gate Array registers
 */

#ifndef MEGADEV__SUB_GATEARR_DEF_H
#define MEGADEV__SUB_GATEARR_DEF_H

/**
 * @defgroup sub_ga_regs Sub CPU / Gate Array Registers
 */

/**
 * @def _GA_RESET
 * @sa GA_RESET
 * @ingroup sub_ga_regs
 */
#define _GA_RESET 0xFF8000

/**
 * @def BIT_GA0_LEDR
 * @brief GA Reg 0 - Red LED control bitpos
 * @sa GA_RESET
 * @ingroup sub_ga_regs
 */
#define BIT_GA_LEDR 0

/**
 * @def BIT_GA0_LEDG
 * @brief GA Reg 0 - Green LED control bitpos
 * @sa GA_RESET
 * @ingroup sub_ga_regs
 */
#define BIT_GA_LEDG 1

/**
 * @def BIT_GA0_VER
 * @brief GA Reg 0 - ROM Version bitpos
 * @sa GA_RESET
 * @ingroup sub_ga_regs
 */
#define BIT_GA_VER 4

/**
 * @def MSK_GA0_LEDR
 * @brief GA Reg 0 - Red LED control bitmask
 * @sa GA_RESET
 * @ingroup sub_ga_regs
 */
#define MSK_GA_LEDR 1 << BIT_GA_LEDR

/**
 * @def MSK_GA0_LEDG
 * @brief GA Reg 0 - Green LED control bitmask
 * @sa GA_RESET
 * @ingroup sub_ga_regs
 */
#define MSK_GA_LEDG 1 << BIT_GA_LEDG

/**
 * @def MSK_GA0_VER
 * @brief GA Reg 0 - ROM Version bitmask
 * @sa GA_RESET
 * @ingroup sub_ga_regs
 */
#define MSK_GA_VER 0xF << BIT_GA0_VER

/**
 * @def _GA_MEMMODE
 * @sa GA_MEMMODE
 * @ingroup sub_ga_regs
 */
#define _GA_MEMMODE 0xFF8002

#define BIT_GA_RET 0
#define BIT_GA_DMNA 1
#define BIT_GA_MODE 2

/**
 * @def MSK_GA_RET
 * @brief GA Reg 01 - Change Word RAM ownership bitmask
 * @sa GA_MEMMODE
 * @ingroup sub_ga_regs
 */
#define MSK_GA_RET (1 << BIT_GA_RET)

/**
 * @def MSK_GA_DMNA
 * @brief GA Reg 01 - Main CPU no Word RAM access flag bitmask
 * @sa GA_MEMMODE
 * @ingroup sub_ga_regs
 */
#define MSK_GA_DMNA (1 << BIT_GA_DMNA)

/**
 * @def MSK_GA_MODE
 * @brief GA Reg 01 - Word RAM layout bitmask
 * @sa GA_MEMMODE
 * @ingroup sub_ga_regs
 */
#define MSK_GA_MODE (1 << BIT_GA_MODE)

/**
 * @def _GA_CDCMODE
 * @sa GA_CDCMODE
 * @ingroup sub_ga_regs
 */
#define _GA_CDCMODE 0xFF8004

/**
 * @def _GA_CDCRS1
 * @sa GA_CDCRS1
 * @ingroup sub_ga_regs
 */
#define _GA_CDCRS1 0xFF8006

/**
 * @def _GA_CDCHOSTDATA
 * @sa GA_CDCHOSTDATA
 * @ingroup sub_ga_regs
 */
#define _GA_CDCHOSTDATA 0xFF8008

/**
 * @def _GA_DMAADDR
 * @sa GA_DMAADDR
 * @ingroup sub_ga_regs
 */
#define _GA_DMAADDR 0xFF800A

/**
 * @def _GA_STOPWATCH
 * @sa GA_STOPWATCH
 * @ingroup sub_ga_regs
 */
#define _GA_STOPWATCH 0xFF800C

/**
 * @def _GA_COMFLAGS
 * @sa GA_COMFLAGS
 * @ingroup sub_ga_regs
 */
#define _GA_COMFLAGS 0xFF800E

/**
 * @def _GA_COMCMD0
 * @sa GA_COMCMD0
 * @ingroup sub_ga_regs
 */
#define _GA_COMCMD0 0xFF8010

/**
 * @def _GA_COMCMD1
 * @sa GA_COMCMD1
 * @ingroup sub_ga_regs
 */
#define _GA_COMCMD1 0xFF8012

/**
 * @def _GA_COMCMD2
 * @sa GA_COMCMD2
 * @ingroup sub_ga_regs
 */
#define _GA_COMCMD2 0xFF8014

/**
 * @def _GA_COMCMD3
 * @sa GA_COMCMD3
 * @ingroup sub_ga_regs
 */
#define _GA_COMCMD3 0xFF8016

/**
 * @def _GA_COMCMD4
 * @sa GA_COMCMD4
 * @ingroup sub_ga_regs
 */
#define _GA_COMCMD4 0xFF8018

/**
 * @def _GA_COMCMD5
 * @sa GA_COMCMD5
 * @ingroup sub_ga_regs
 */
#define _GA_COMCMD5 0xFF801A

/**
 * @def _GA_COMCMD6
 * @sa GA_COMCMD6
 * @ingroup sub_ga_regs
 */
#define _GA_COMCMD6 0xFF801C

/**
 * @def _GA_COMCMD7
 * @sa GA_COMCMD7
 * @ingroup sub_ga_regs
 */
#define _GA_COMCMD7 0xFF801E

/**
 * @def _GA_COMSTAT0
 * @sa GA_COMSTAT0
 * @ingroup sub_ga_regs
 */
#define _GA_COMSTAT0 0xFF8020

/**
 * @def _GA_COMSTAT1
 * @sa GA_COMSTAT1
 * @ingroup sub_ga_regs
 */
#define _GA_COMSTAT1 0xFF8022

/**
 * @def _GA_COMSTAT2
 * @sa GA_COMSTAT2
 * @ingroup sub_ga_regs
 */
#define _GA_COMSTAT2 0xFF8024

/**
 * @def _GA_COMSTAT3
 * @sa GA_COMSTAT3
 * @ingroup sub_ga_regs
 */
#define _GA_COMSTAT3 0xFF8026

/**
 * @def _GA_COMSTAT4
 * @sa GA_COMSTAT4
 * @ingroup sub_ga_regs
 */
#define _GA_COMSTAT4 0xFF8028

/**
 * @def _GA_COMSTAT5
 * @sa GA_COMSTAT5
 * @ingroup sub_ga_regs
 */
#define _GA_COMSTAT5 0xFF802A

/**
 * @def _GA_COMSTAT6
 * @sa GA_COMSTAT6
 * @ingroup sub_ga_regs
 */
#define _GA_COMSTAT6 0xFF802C

/**
 * @def _GA_COMSTAT7
 * @sa GA_COMSTAT7
 * @ingroup sub_ga_regs
 */
#define _GA_COMSTAT7 0xFF802E

/**
 * @def _GA_INT3TIMER
 * @sa GA_COMSTAT7
 * @ingroup sub_ga_regs
 */
#define _GA_INT3TIMER 0xFF8030

/**
 * @def _GA_INTMASK
 * @sa GA_INTMASK
 * @ingroup sub_ga_regs
 */
#define _GA_INTMASK 0xFF8032

#define BIT_GA_INT1 1
#define BIT_GA_INT2 2
#define BIT_GA_INT3 3
#define BIT_GA_INT4 4
#define BIT_GA_INT5 5
#define BIT_GA_INT6 6

#define MSK_GA_INT1 1 << BIT_GA_INT1
#define MSK_GA_INT2 1 << BIT_GA_INT2
#define MSK_GA_INT3 1 << BIT_GA_INT3
#define MSK_GA_INT4 1 << BIT_GA_INT4
#define MSK_GA_INT5 1 << BIT_GA_INT5
#define MSK_GA_INT6 1 << BIT_GA_INT6

/**
 * @def _GA_CDFADER
 * @sa GA_CDFADER
 * @ingroup sub_ga_regs
 */
#define _GA_CDFADER 0xFF8034

/**
 * @def _GA_CDDCTRL
 * @sa GA_CDDCTRL
 * @ingroup sub_ga_regs
 */
#define _GA_CDDCTRL 0xFF8036

/**
 * @def _GA_CDDCOMM0
 * @sa GA_CDDCOMM0
 * @ingroup sub_ga_regs
 */
#define _GA_CDDCOMM0 0xFF8038

/**
 * @def _GA_CDDCOMM1
 * @sa GA_CDDCOMM1
 * @ingroup sub_ga_regs
 */
#define _GA_CDDCOMM1 0xFF803A

/**
 * @def _GA_CDDCOMM2
 * @sa GA_CDDCOMM2
 * @ingroup sub_ga_regs
 */
#define _GA_CDDCOMM2 0xFF803C

/**
 * @def _GA_CDDCOMM3
 * @sa GA_CDDCOMM2
 * @ingroup sub_ga_regs
 */
#define _GA_CDDCOMM3 0xFF803E

/**
 * @def _GA_CDDCOMM4
 * @sa GA_CDDCOMM4
 * @ingroup sub_ga_regs
 */
#define _GA_CDDCOMM4 0xFF8040

/**
 * @def _GA_CDDCOMM5
 * @sa GA_CDDCOMM5
 * @ingroup sub_ga_regs
 */
#define _GA_CDDCOMM5 0xFF8042

/**
 * @def _GA_CDDCOMM6
 * @sa GA_CDDCOMM6
 * @ingroup sub_ga_regs
 */
#define _GA_CDDCOMM6 0xFF8044

/**
 * @def _GA_CDDCOMM7
 * @sa GA_CDDCOMM7
 * @ingroup sub_ga_regs
 */
#define _GA_CDDCOMM7 0xFF8046

/**
 * @def _GA_CDDCOMM8
 * @sa GA_CDDCOMM8
 * @ingroup sub_ga_regs
 */
#define _GA_CDDCOMM8 0xFF8048

/**
 * @def _GA_CDDCOMM9
 * @sa GA_CDDCOMM9
 * @ingroup sub_ga_regs
 */
#define _GA_CDDCOMM9 0xFF804A

/**
 * @def _GA_FONTCOLOR
 * @sa GA_FONTCOLOR
 * @ingroup sub_ga_regs
 */
#define _GA_FONTCOLOR 0xFF804C

/**
 * @def _GA_FONTBITS
 * @sa GA_FONTBITS
 * @ingroup sub_ga_regs
 */
#define _GA_FONTBITS 0xFF804E

/**
 * @def _GA_FONTDATA
 * @sa GA_FONTDATA
 * @ingroup sub_ga_regs
 */
#define _GA_FONTDATA 0xFF8050

/**
 * @def _GA_STAMPSIZE
 * @sa GA_STAMPSIZE
 * @ingroup sub_ga_regs
 */
#define _GA_STAMPSIZE 0xFF8058

/**
 * @def _GA_STAMPMAPBASE
 * @sa GA_STAMPMAPBASE
 * @ingroup sub_ga_regs
 */
#define _GA_STAMPMAPBASE 0xFF805A

/**
 * @def _GA_IMGBUFVSIZE
 * @sa GA_IMGBUFVSIZE
 * @ingroup sub_ga_regs
 */
#define _GA_IMGBUFVSIZE 0xFF805C

/**
 * @def _GA_IMGBUFSTART
 * @sa GA_IMGBUFSTART
 * @ingroup sub_ga_regs
 */
#define _GA_IMGBUFSTART 0xFF805E

/**
 * @def _GA_IMGBUFOFFSET
 * @sa GA_IMGBUFOFFSET
 * @ingroup sub_ga_regs
 */
#define _GA_IMGBUFOFFSET 0xFF8060

/**
 * @def _GA_IMGBUFHDOTSIZE
 * @sa GA_IMGBUFHDOTSIZE
 * @ingroup sub_ga_regs
 */
#define _GA_IMGBUFHDOTSIZE 0xFF8062

/**
 * @def _GA_IMGBUFVDOTSIZE
 * @sa GA_IMGBUFVDOTSIZE
 * @ingroup sub_ga_regs
 */
#define _GA_IMGBUFVDOTSIZE 0xFF8064

/**
 * @def _GA_TRACEVECTBASE
 * @sa GA_TRACEVECTBASE
 * @ingroup sub_ga_regs
 */
#define _GA_TRACEVECTBASE 0xFF8066

/**
 * @def _GA_SUBCODEADDR
 * @sa GA_SUBCODEADDR
 * @ingroup sub_ga_regs
 */
#define _GA_SUBCODEADDR 0xFF8068

/**
 * @def _GA_SUBCODEBUF
 * @sa GA_SUBCODEBUF
 * @ingroup sub_ga_regs
 */
#define _GA_SUBCODEBUF 0xFF8100

/**
 * @def _GA_SUBCODEBUFIMG
 * @sa GA_SUBCODEBUFIMG
 * @ingroup sub_ga_regs
 */
#define _GA_SUBCODEBUFIMG 0xFF8180

/**
 * GA_CDCMODE bit/mask settings
 */
#define BIT_CDCMODE_DD0 5
#define BIT_CDCMODE_DSR 6
#define BIT_CDCMODE_EDT 7

// these aren't right...
// TODO clean these up
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

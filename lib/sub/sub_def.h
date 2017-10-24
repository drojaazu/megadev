/**
 * \file sub/sub_def.h
 * \brief Hardware memory map, Gate Array (GA) register, and entry vector definitions
 * for the Sub CPU side
 */

#ifndef MEGADEV__CD_SUB_DEF_H
#define MEGADEV__CD_SUB_DEF_H

/**
 * Program RAM (PRG RAM)
 */
#define PRG_RAM   0x000000

/**
 * The Sub side program (SP) begins at 0x6000
 * Memory before this point is used by BIOS and should not be written
 * by the user
 */
#define USER_BASE  0x006000

/**
 * PRG RAM can be accessed in 1M banks by the Main CPU
 * This provides quick access to each bank
 */
#define PRG_RAM0  0x000000
#define PRG_RAM1  0x020000
#define PRG_RAM2  0x040000
#define PRG_RAM3  0x060000

/**
 * Word RAM (2M)
 */
#define SUB_2M_BASE  0x080000      /*word RAM base in 2M bit mode*/
#define SUB_1M_BASE  0x0C0000      /*word RAM base in 1M bit mode*/

#define _BOOTSTAT  0x005EA0
#define _INT2FLAG  0x005EA4
#define _USERMODE  0x005EA6
#define _CDSTAT    0x005E80

/**
 * System Jump Table
 */
#define _SETJMPTBL  0x005F0A
#define _WAITVSYNC  0x005F10
#define _BURAM      0x005F16   /* Backup RAM call vector */
#define _CDBOOT     0x005F1C   /* CD Boot call vector */
#define _CDBIOS     0x005F22   /* BIOS call vector */
#define _USERCALL0  0x005F28   /* SP Init */
#define _USERCALL1  0x005F2E   /* SP Main */
#define _USERCALL2  0x005F34   /* SP INT2 */
#define _USERCALL3  0x005F3A   /* SP User Interrupt */
#define _ADRERR     0x005F40
#define _CODERR     0x005F46
#define _DIVERR     0x005F4C
#define _CHKERR     0x005F52
#define _TRPERR     0x005F58
#define _SPVERR     0x005F5E
#define _TRACE      0x005F64
#define _NOCOD0     0x005F6A
#define _NOCOD1     0x005F70
#define _LEVEL1     0x005F76
#define _LEVEL2     0x005F7C   /* INT 2 from Main CPU */
#define _LEVEL3     0x005F82   /* GA Timer Interrupt */
#define _LEVEL4     0x005F88
#define _LEVEL5     0x005F8E
#define _LEVEL6     0x005F94
#define _LEVEL7     0x005F9A
#define _TRAP00     0x005FA0
#define _TRAP01     0x005FA6
#define _TRAP02     0x005FAC
#define _TRAP03     0x005FB2
#define _TRAP04     0x005FB8
#define _TRAP05     0x005FBE
#define _TRAP06     0x005FC4
#define _TRAP07     0x005FCA
#define _TRAP08     0x005FD0
#define _TRAP09     0x005FD6
#define _TRAP10     0x005FDC
#define _TRAP11     0x005FE2
#define _TRAP12     0x005FE8
#define _TRAP13     0x005FEE
#define _TRAP14     0x005FF4
#define _TRAP15     0x005FFA


/**
 * Gate Array registers
 */
/**
 * \def _GA_RESET
 * \brief Sub CPU Reset / Bus Request
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||LEDG|LEDR|Ver3|Ver2|Ver1|Ver0| |||RES0|
 * 
 * \param RES0 Sub CPU reset
 * \details W: 0: Reset / 1: No effect \n R: 0: Reset in progress / 1: Reset possible
 * \param LEDR Red LED control
 * \details RW: 0: Off / 1: On
 * \param LEDG Green LED control
 * \details RW: 0: Off / 1: On
 * \param Ver ROM Version
 * \details Read Only
 */
#define _GA_RESET           0xFF8000

/**
 * \def _GA_MEMORYMODE
 * \brief Word RAM Memory Mode / RAM Write Protect / Priority Mode
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |WP7|WP6|WP5|WP4|WP3|WP2|WP1|WP0| |||PM1|PM0|MODE|DMNA|RET|
 * 
 * \param WP Write protect Sub CPU RAM
 * \param PM Priority Mode
 * \param MODE Word RAM layout
 * \param DMNA Main CPU will not access Word RAM
 * \param RET Give Word RAM control to Main CPU
 * 
 */
#define _GA_MEMORYMODE      0xFF8002

/**
 * \def _GA_CDCMODE
 * \brief CDC Mode / CDC Registers #1
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |EDT|DSR| |||DD2|DD1|DD0| ||||CA3|CA2|CA1|CA0|
 * 
 * \param CA CDC register address
 * \details RW: Details for this register can be found in the LC89510 manual
 * \param DSR Data set ready
 * \details R: Data from the CDC is present in the CDC Host Data register
 * \param EDT End of data transfer
 * \details R: All data from the CDC has been transferred
 * \param DD Device destination
 * \details RW: Specifies the destination for CDC data transfer, as defined below:
 *   |DD2|DD1|DD0|Destination|
 *   |:|:|:|:|
 *   |0|1|0|Main CPU|
 *   |0|1|1|Sub CPU|
 *   |1|0|0|PCM DMA|
 *   |1|1|1|In 2M Mode: Word RAM \n In 1M Mode: Sub CPU controlled Word RAM|
 * 
 *   All other values for DD are invalid.
 */
#define _GA_CDCMODE         0xFF8004

/**
 * \def _GA_CDCRS1
 * \brief CDC Registers #2
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||||CD7|CD6|CD5|CD4|CD3|CD2|CD1|CD0|
 * 
 * \param CD CDC register data
 * \details RW: Details for this register can be found in the LC89510 manual
 */
#define _GA_CDCRS1          0xFF8006

/**
 * \def _GA_CDCHOSTDATA
 * \brief CDC Host Data
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |HD15|HD14|HD13|HD12|HD11|HD10|HD09|HD08|HD07|HD06|HD05|HD04|HD03|HD02|HD01|HD00|
 * 
 * \param HD CDC read data
 * \details R: 2 bytes of data read by the CDC and ready to be transferred to
 * Main or Sub CPU memory
 */
#define _GA_CDCHOSTDATA     0xFF8008

/**
 * \def _GA_DMAADDR
 * \brief CDC DMA Address
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |A18|A17|A16|A15|A14|A13|A12|A11|A10|A09|A08|A07|A06|A05|A04|A03|
 * 
 * \param HD CDC read data
 * \details RW: Specifies the address for CDC DMA transfer
 * - For PCM DMA: bits up to A12 are used
 * - For 1M Word RAM: bits up to A16 are used
 * - For 2M Word RAM: bits up to A17 are used
 * - For PRG-RAM: all bits are used
 * 
 * Unused bits will be read as 0.
 */
#define _GA_DMAADDR         0xFF800A

/**
 * \def _GA_STOPWATCH
 * \brief Stopwatch
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||SW11|SW10|SW09|SW08|SW07|SW06|SW05|SW04|SW03|SW02|SW01|SW00|
 * 
 * \param SW CDC read data
 * \details R: Read current value / W: Reset the clock (0 only)
 * This is a general use timer, though it is primarily used for CDD/CDC
 * timing. Each tick is 30.72 microseconds.
 */
#define _GA_STOPWATCH       0xFF800C

/**
 * \def _GA_COMFLAGS
 * \brief Main/Sub CPU communication flags
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * |CFM7|CFM6|CFM5|CFM4|CFM3|CFM2|CFM1|CFM0|CFS7|CFS6|CFS5|CFS4|CFS3|CFS2|CFS1|CFS0|
 * 
 * CFM: Comm flags for Main CPU
 * CFS: Comm flags for Sub CPU
 */
#define _GA_COMFLAGS        0xFF800E

/**
 * \def _GA_COMCMD0
 * \brief Communication command (Main -> Sub)
 * \details R: 16 bit data
 */
#define _GA_COMCMD0         0xFF8010

/**
 * \def _GA_COMCMD1
 * \brief Communication command (Main -> Sub)
 * \details R: 16 bit data
 */
#define _GA_COMCMD1         0xFF8012

/**
 * \def _GA_COMCMD2
 * \brief Communication command (Main -> Sub)
 * \details R: 16 bit data
 */
#define _GA_COMCMD2         0xFF8014

/**
 * \def _GA_COMCMD3
 * \brief Communication command (Main -> Sub)
 * \details R: 16 bit data
 */
#define _GA_COMCMD3         0xFF8016

/**
 * \def _GA_COMCMD4
 * \brief Communication command (Main -> Sub)
 * \details R: 16 bit data
 */
#define _GA_COMCMD4         0xFF8018

/**
 * \def _GA_COMCMD5
 * \brief Communication command (Main -> Sub)
 * \details R: 16 bit data
 */
#define _GA_COMCMD5         0xFF801A

/**
 * \def _GA_COMCMD6
 * \brief Communication command (Main -> Sub)
 * \details R: 16 bit data
 */
#define _GA_COMCMD6         0xFF801C

/**
 * \def _GA_COMCMD7
 * \brief Communication command (Main -> Sub)
 * \details R: 16 bit data
 */
#define _GA_COMCMD7         0xFF801E

/**
 * \def _GA_COMSTAT0
 * \brief Communication status (Sub -> Main)
 * \details RW: 16 bit data
 */
#define _GA_COMSTAT0        0xFF8020

/**
 * \def _GA_COMSTAT1
 * \brief Communication status (Sub -> Main)
 * \details RW: 16 bit data
 */
#define _GA_COMSTAT1        0xFF8022

/**
 * \def _GA_COMSTAT2
 * \brief Communication status (Sub -> Main)
 * \details RW: 16 bit data
 */
#define _GA_COMSTAT2        0xFF8024

/**
 * \def _GA_COMSTAT3
 * \brief Communication status (Sub -> Main)
 * \details RW: 16 bit data
 */
#define _GA_COMSTAT3        0xFF8026

/**
 * \def _GA_COMSTAT4
 * \brief Communication status (Sub -> Main)
 * \details RW: 16 bit data
 */
#define _GA_COMSTAT4        0xFF8028

/**
 * \def _GA_COMSTAT5
 * \brief Communication status (Sub -> Main)
 * \details RW: 16 bit data
 */
#define _GA_COMSTAT5        0xFF802A

/**
 * \def _GA_COMSTAT6
 * \brief Communication status (Sub -> Main)
 * \details RW: 16 bit data
 */
#define _GA_COMSTAT6        0xFF802C

/**
 * \def _GA_COMSTAT7
 * \brief Communication status (Sub -> Main)
 * \details RW: 16 bit data
 */
#define _GA_COMSTAT7        0xFF802E

/**
 * \def _GA_INT3TIMER
 * \brief Timer with interrupt
 * \details
 * | F| E| D| C| B| A| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:|
 * | ||||||||TD7|TD6|TD5|TD4|TD3|TD2|TD1|TD0|
 * 
 * TD: Timer countdown value
 * \details The time ticks every 30.72 microseconds, continuously counting down
 * from the value inthis register until zero. When reaching zero, it triggers
 * a level 3 interrupt. If the value set is 0, no interrupt is generated.
 */
#define _GA_INT3TIMER       0xFF8030


#define _GA_INTMASK         0xFF8032
#define _GA_CDFADER         0xFF8034
#define _GA_CDDCONTROL      0xFF8036
#define _GA_CDDCOMM0        0xFF8038
#define _GA_CDDCOMM1        0xFF803A
#define _GA_CDDCOMM2        0xFF803C
#define _GA_CDDCOMM3        0xFF803E
#define _GA_CDDCOMM4        0xFF8040
#define _GA_CDDCOMM5        0xFF8042
#define _GA_CDDCOMM6        0xFF8044
#define _GA_CDDCOMM7        0xFF8046
#define _GA_CDDCOMM8        0xFF8048
#define _GA_CDDCOMM9        0xFF804A
#define _GA_FONTCOLOR       0xFF804C
#define _GA_FONTBITS        0xFF804E
#define _GA_FONTDATA        0xFF8050
#define _GA_STAMPSIZE       0xFF8058
#define _GA_STAMPMAPBASE    0xFF805A
#define _GA_IMGBUFVSIZE     0xFF805C
#define _GA_IMGBUFSTART     0xFF805E
#define _GA_IMGBUFOFFSET    0xFF8060
#define _GA_IMGBUFHDOTSIZE  0xFF8062
#define _GA_IMGBUFVDOTSIZE  0xFF8064
#define _GA_TRACEVECTBASE   0xFF8066
#define _GA_SUBCODEADDR     0xFF8068
#define _GA_SUBCODEBUF      0xFF8100
#define _GA_SUBCODEBUFIMG   0xFF8180

/**
 * GA_MEMORYMODE bit/mask settings
 * Note: GA registers are 16 bits, but we generally work with bit level tests
 * against the upper or lower byte specifically. Therefore the bit values and
 * masks are relative to their specific byte rather than the full width of the
 * register.
 */
#define MEMORYMODE_RET_BIT   0
#define MEMORYMODE_DMNA_BIT  1
#define MEMORYMODE_MODE_BIT  2
#define MEMORYMODE_PM_BIT    3

#define MEMORYMODE_RET_MSK   1 << MEMORYMODE_RET_BIT
#define MEMORYMODE_DMNA_MSK  1 << MEMORYMODE_DMNA_BITc
#define MEMORYMODE_MODE_MSK  1 << MEMORYMODE_MODE_BIT
#define MEMORYMODE_PM_MSK    3 << MEMORYMODE_PM_BIT

/**
 * GA_INTMASK bit/mask settings
 */
#define INT1_GFX_BIT      1
#define INT2_MD_BIT       2
#define INT3_TIMER_BIT    3
#define INT4_CDD_BIT      4
#define INT5_CDC_BIT      5
#define INT6_SUBCODE_BIT  6

#define INT1_GFX_MSK      1 << INT1_GFX_BIT
#define INT2_MD_MSK       1 << INT2_MD_BIT
#define INT3_TIMER_MSK    1 << INT3_TIMER_BIT
#define INT4_CDD_MSK      1 << INT4_CDD_BIT
#define INT5_CDC_MSK      1 << INT5_CDC_BIT
#define INT6_SUBCODE_MSK  1 << INT6_SUBCODE_BIT

/**
 * GA_CDCMODE bit/mask settings
 */
#define CDCMODE_DD0_BIT   5
#define CDCMODE_DSR_BIT   6
#define CDCMODE_EDT_BIT   7

#define CDC_MAINREAD_BIT  2
#define CDC_SUBREAD_BIT   3
#define CDC_PCMDMA_BIT    4
#define CDC_PRAMDMA_BIT   5
#define CDC_WRAMDMA_BIT   7

#define CDCMODE_CABITS    0x000F
#define CDCMODE_DDBITS    0x0700
#define CDCMODE_DD0_MSK   1 << CDCMODE_DD0_BIT
#define CDCMODE_DSR_MSK   1 << CDCMODE_DSR_BIT
#define CDCMODE_EDT_MSK   1 << CDCMODE_EDT_BIT
#define CDCMODE_MAINREAD_MSK  1 << CDC_MAINREAD_BIT
#define CDCMODE_SUBREAD_MSK   1 << CDC_SUBREAD_BIT
#define CDCMODE_PCMDMA_MSK    1 << CDC_PCMDMA_BIT
#define CDCMODE_PRAMDMA_MSK   1 << CDC_PRAMDMA_BIT
#define CDCMODE_WRAMDMA_MSK   1 << CDC_WRAMDMA_BIT



#endif

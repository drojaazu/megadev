/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file memmap.def.h
 * @brief Sub CPU memory map & system vectors
 */

#ifndef MEGADEV__SUB_MEMMAP_DEF_H
#define MEGADEV__SUB_MEMMAP_DEF_H

/**
 * @brief Program RAM (PRG RAM)
 *
 * @details Size: 4Mbit
 */
#define _PRGRAM 0x000000

/**
 * @brief PRG RAM 1Mbit Bank 0
 *
 * @details PRG RAM can be accessed in 1M banks by the Main CPU.
 * These provide quick access to each bank
 */
#define _PRGRAM_1M_0 0x00000

/**
 * @brief PRG RAM 1Mbit Bank 1
 *
 * @sa _PRGRAM_1M_0
 */
#define _PRGRAM_1M_1 0x20000

/**
 * @brief PRG RAM 1Mbit Bank 2
 *
 * @sa _PRGRAM_1M_0
 */
#define _PRGRAM_1M_2 0x40000

/**
 * @brief PRG RAM 1Mbit Bank 3
 *
 * @sa _PRGRAM_1M_0
 */
#define _PRGRAM_1M_3 0x60000

/**
 * @brief Beginning of user-available space in PRGRAM
 *
 * The Sub side program (SP) begins at 0x6000
 * Memory before this point is used by BIOS and should not be written
 * by the user
 */
#define _PRGRAM_USER 0x006000

/**
 * @brief Word RAM access (2M mode)
 */
#define _WRDRAM_2M 0x080000 /*word RAM base in 2M bit mode*/

/**
 * @brief Word RAM access (1M mode)
 */
#define _WRDRAM_1M 0x0C0000 /*word RAM base in 1M bit mode*/

#define _BOOTSTAT 0x005EA0
#define _INT2FLAG 0x005EA4
#define _USERMODE 0x005EA6
#define _CDSTAT 0x005E80

/**
 * System Jump Table
 */
#define _SETJMPTBL 0x005F0A
#define _WAITVSYNC 0x005F10
#define _BURAM 0x005F16			/* Backup RAM call vector */
#define _CDBOOT 0x005F1C		/* CD Boot call vector */
#define _CDBIOS 0x005F22		/* BIOS call vector */
#define _USERCALL0 0x005F28 /* SP Init */
#define _USERCALL1 0x005F2E /* SP Main */
#define _USERCALL2 0x005F34 /* SP INT2 */
#define _USERCALL3 0x005F3A /* SP User Call */
#define _ADRERR 0x005F40
#define _CODERR 0x005F46
#define _DIVERR 0x005F4C
#define _CHKERR 0x005F52
#define _TRPERR 0x005F58
#define _SPVERR 0x005F5E
#define _TRACE 0x005F64
#define _NOCOD0 0x005F6A
#define _NOCOD1 0x005F70
#define _SLEVEL1 0x005F76 /* Graphics operation complete */
#define _SLEVEL2 0x005F7C /* INT 2 from Main CPU */
#define _SLEVEL3 0x005F82 /* GA Timer Interrupt */
#define _SLEVEL4 0x005F88 /* CDD complete */
#define _SLEVEL5 0x005F8E /* CDC complete */
#define _SLEVEL6 0x005F94 /* Subcode buffer full */
#define _SLEVEL7 0x005F9A
#define _TRAP00 0x005FA0
#define _TRAP01 0x005FA6
#define _TRAP02 0x005FAC
#define _TRAP03 0x005FB2
#define _TRAP04 0x005FB8
#define _TRAP05 0x005FBE
#define _TRAP06 0x005FC4
#define _TRAP07 0x005FCA
#define _TRAP08 0x005FD0
#define _TRAP09 0x005FD6
#define _TRAP10 0x005FDC
#define _TRAP11 0x005FE2
#define _TRAP12 0x005FE8
#define _TRAP13 0x005FEE
#define _TRAP14 0x005FF4
#define _TRAP15 0x005FFA

#endif

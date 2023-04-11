/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file build.def.h
 * @brief Target hardware components definitions
 *
 * @details

 */

#ifndef MEGADEV__BUILD_DEF_H
#define MEGADEV__BUILD_DEF_H

// For use with the HW_TARGET definition
#define MEGADRIVE 0
#define MEGACD 1
#define MEGACD_MODE1 2

// For use with the REGION definition
#define JP 0
#define US 1
#define EU 2

// For use with the VIDEO definition
#define NTSC 0
#define PAL 1

// For use with VRAM_SIZE definition
#define VRAM_64K 0
#define VRAM_128K 1

#endif

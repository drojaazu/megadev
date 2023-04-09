/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file build.def.h
 * @brief Target hardware components definitions
 *
 * @details
 * These can be referenced within code by setting globale definitions
 * or during the make process via HW_CFG, e.g.:
 * make HW_CFG="REGION=JP VIDEO=NTSC"
 */

#ifndef MEGADEV__BUILD_DEF_H
#define MEGADEV__BUILD_DEF_H

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

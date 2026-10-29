/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file build.def.h
 * @brief Target hardware components definitions
 *
 */

#pragma once

// For use with the TARGET definition
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

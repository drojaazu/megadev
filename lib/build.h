/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file build.h
 * @brief Hardware configuration defaults and compile time sanity checking
 */

#ifndef MEGADEV__BUILD_H
#define MEGADEV__BUILD_H

#include "build.def.h"

#ifndef TARGET
#error TARGET not defined!
#endif

#ifndef PROJECT_NAME
#error PROJECT_NAME not defined!
#endif

#ifndef PROJECT_NAME_DOMESTIC
#define PROJECT_NAME_DOMESTIC PROJECT_NAME
#endif

#ifndef REGION
#error REGION not defined!
#endif

#ifndef VIDEO
#define VIDEO NTSC
#endif

#ifndef VRAM_SIZE
#define VRAM_SIZE VRAM_64K
#endif

#ifndef HARDWARE_ID
#if REGION == US
#define HARDWARE_ID "SEGA GENESIS    "
#else
#define HARDWARE_ID "SEGA MEGA DRIVE "
#endif
#endif

#ifndef HEADER_REGION
#if REGION == JP
#define HEADER_REGION "J  "
#endif
#if REGION == US
#define HEADER_REGION "U  "
#endif
#if REGION == EU
#define HEADER_REGION "E  "
#endif
#endif

#endif

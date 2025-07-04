/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file build.h
 * @brief Hardware configuration defaults and compile time sanity checking
 */

#ifndef MEGADEV__BUILD_H
#define MEGADEV__BUILD_H

#include "build.def.h"
// config.h should be located in the project source tree
#include "config.h"

#ifndef TARGET
#error TARGET not defined! Check your config.h file.
#endif

#ifndef HEADER_NAME_INTL
#error HEADER_NAME_INTL not defined! Check your config.h file.
#endif

#ifndef HEADER_NAME_JP
#define HEADER_NAME_JP HEADER_NAME_INTL
#endif

#ifndef REGION
#define REGION JP
#endif

#ifndef VIDEO
#define VIDEO NTSC
#endif

#ifndef VRAM_SIZE
#define VRAM_SIZE VRAM_64K
#endif

#ifndef HEADER_HARDWARE
#if REGION == US
#define HEADER_HARDWARE "SEGA GENESIS    "
#else
#define HEADER_HARDWARE "SEGA MEGA DRIVE "
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

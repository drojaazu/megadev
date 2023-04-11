/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file build.s
 * @brief Hardware configuration defaults and compile time sanity checking
 */

#include "build.def.h"
#include "project.h"

#ifndef HW_TARGET
#error HW_TARGET not defined!
#endif

#ifndef REGION
#define REGION JP
#endif

#ifndef VIDEO
#define VIDEO NTSC
#endif

#ifndef VRAM_SIZE
#define VRAM_64K
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

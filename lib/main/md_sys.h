/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file md_sys.h
 * @brief Mega Drive hardware version and TMSS access
 */

#pragma once
#include <main/md_sys.def.h>

#define hw_version (*((u8 const volatile *) HW_VERSION))

#define tmss (*((u8 volatile *) TMSS))

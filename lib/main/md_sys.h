/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file mmd.h
 * @brief Set up and run a loaded MMD formatted module
 * @note This expects a module to be already loaded at the start of Word RAM
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <main/md_sys.def.h>

#define hw_version (*((u8 const volatile *) HW_VERSION))

#define tmss (*((u8 volatile *) TMSS))

#ifdef __cplusplus
}
#endif

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

#define hw_version (*((uint8_t const volatile *) HW_VERSION))

#define tmss (*((uint8_t volatile *) TMSS))

#ifdef __cplusplus
}
#endif

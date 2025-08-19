/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file mmd.h
 * @brief Set up and run a loaded MMD formatted module
 * @note This expects a module to be already loaded at the start of Word RAM
 */

#ifndef MEGADEV__MAIN_MD_SYS_H
#define MEGADEV__MAIN_MD_SYS_H

#include <main/md_sys.def.h>

#define hardware_version (*((u8 const volatile *) _HW_VERSION))

#define tmss_vdp_unlock (*((u8 volatile *) _TMSS_VDP_UNLOCK))

#define tmss_bank_switch (*((u8 volatile *) _TMSS_BANK_SWITCH))

#endif

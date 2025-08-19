/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file md_sys.def.h
 * @brief Misc Mega Drive system registers
 */

#ifndef MEGADEV__MAIN_MD_SYS_DEF_H
#define MEGADEV__MAIN_MD_SYS_DEF_H

/**
 * @def _HW_VERSION
 * @brief Version register
 * @details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |RGN|CLK|EXT||VER3|VER2|VER1|VER0|
 * @param VER Hardware revision
 * @param EXT Expansion unit: 0 - Expansion unit connected; 1 - Expansion unit
 * not connected
 * @param CLK Clock: 0 - NTSC Clock (7.67Mhz); 1 - PAL Clock
 * (7.60Mhz)
 * @param RGN Hardware region: 0 - Domestic (Japan); 1 - International
 * (Europe or US)
 */
#define _HW_VERSION 0x0A10001

#define HW_REV (0b111 << 0)
#define EXT_PERIPHERAL (1 << 5)
#define HW_CLOCK (1 << 6)
#define HW_REGION (1 << 7)

/**
 * @def _TMSS_VDP_UNLOCK
 * @brief Part of the TMSS security system. The characters S E G A must be written to the port on startup and before
 *        any access to the VDP data port.
 */
#define _TMSS_VDP_UNLOCK 0x0A14000

/**
 * @def _TMSS
 * @brief Alias for @ref _TMSS_VDP_UNLOCK
 * @sa _TMSS_VDP_UNLOCK
 */
#define _TMSS _TMSS_VDP_UNLOCK

/**
 * @def _TMSS_BANK_SWITCH
 * @brief Used by the internal TMSS ROM to enable the cartridge
 * @note This should never be needed in standard development as is only present for reference
 */
#define _TMSS_BANK_SWITCH 0x0A14001

#endif

/**
 * \file
 * \brief Misc. Mega Drive system registers
 */

#ifndef MEGADEV__MD_SYS_DEF_H
#define MEGADEV__MD_SYS_DEF_H

/**
 * \def REG_VERSION
 * \brief Plane B Name Table VRAM Address
 * \details
 * | 7| 6| 5| 4| 3| 2| 1| 0|
 * |-:|-:|-:|-:|-:|-:|-:|-:|
 * |RGN|CLK|EXT||VER3|VER2|VER1|VER0|
 * \param VER Version number
 * \param EXT Expansion unit: 0 - Expansion unit connected; 1 - Expansion unit
 * not connected
 * \param CLK Clock: 0 - NTSC Clock (7.67Mhz); 1 - PAL Clock
 * (7.60Mhz)
 * \param RGN Hardware region: 0 - Domestic (Japan); 1 - International
 * (Europe or US)
 */
#define REG_VERSION 0x0A10001

#define REG_TMSS , 0x0A14000

#endif
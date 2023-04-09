/**
 * @file
 * Project header and build configuration
 */

#ifndef MEGADEV__PROJECT_H
#define MEGADEV__PROJECT_H

#include "build.def.h"

/*
 * Default build settings
 * The below global definitions can be used for conditional compilation within
 * your software. These are default values which can be changed at compile time
 * with the -D option.
 */

#ifndef REGION
/**
 * @def REGION
 * @brief Geographic region
 * @details Determines the geographic region of intended for the software, which
 * in turn determines which hardware with which it will be compatible.
 *
 * @note Valid values are JP (Japan), US (United States), and EU (Europe)
 */
#define REGION JP
#endif

#ifndef VIDEO
/**
 * @def VIDEO
 * @brief Video signal format
 * @details Determines the video signal format, which is generally tied to the
 * geographic region of the hardware.
 *
 * @note Valid values are NTSC and PAL. In general, use NTSC if the region
 * setting is JP or US, and PAL if the region setting is EU.
 */
#define VIDEO NTSC
#endif

#ifndef VRAM_SIZE
/**
 * @def VRAM_SIZE
 * @brief Hardware VRAM size
 * @details Determines if the VDP VRAM size is 64k or 128k. 64k is the standard
 * size on all Mega Drive/Genesis variants. The larger size was only available
 * on the Sega Teradrive hardware. However, the VDP in all MD versions is
 * capable of addressing the full 128k, and hardware hackers have modified the
 * original MD to add additional VRAM.
 *
 * Unless you are specifically targeting the Teradrive or a modified MD, this
 * should be left at 64k.
 *
 * @note Valid values are VRAM_64K and VRAM_128K
 */
#define VRAM_SIZE VRAM_64K
#endif

/*
 * Header settings
 * The below settings will appear in the program header, located within the boot
 * sector of the disc.
 *******************************************************************************
 * These are *size sensitive* and must occupy exactly the amount of space      *
 * listed. You can use the sizing bar (######) below each entry to ensure your *
 * text fits, and unused space should be filled with spaces.                   *
 *******************************************************************************
 */

/**
 * @def HEADER_VOL_ID
 * @brief Disc volume ID
 * @details This will be the "name" of the disc, as displayed in a file manager
 * when mounted on a PC
 * @note 11 bytes
 */
#define HEADER_VOL_ID "HELLO_WORLD"
//                    "###########"

/**
 * @def HEADER_HARDWARE
 * @brief Target hardware
 * @details
 * This line indicates the hardware on which the game is meant to run. Despite
 * obviously targeting the Mega CD, the actual identifier should be Mega Drive
 * or Genesis, e.g.:
 *  SEGA MEGA DRIVE
 *  SEGA GENESIS
 * @note 16 bytes
 * @todo autogenerate this based on hardware settings in the project makefile
 */
#define HEADER_HARDWARE "SEGA MEGA DRIVE "
//                      "################"

/**
 * @def HEADER_COPYRIGHT
 * @brief Publisher copyright & date
 * @details
 * This line indicates the publisher name or ID and the build date
 * In retail games from the time, it was commonly written with (C)
 * along with the published code then the year and month, e.g.:
 *  (C)SEGA 1994.AUG\n
 *  (C)T-76 1994.FEB\n
 * @note 16 bytes
 * @todo autogenerate the date
 */
#define HEADER_COPYRIGHT "(C)     2022.NOV"
//                       "################"

/**
 * @def HEADER_NAME_INTL
 * @brief International title
 * @details The name of the software, in plain ASCII encoding
 * @note 48 bytes
 */
#define HEADER_NAME_INTL "HELLO WORLD                                     "
//                       "################################################"

/**
 * @def HEADER_NAME_JP
 * @brief Domestic (Japanese) title
 *
 * @details The name of the software, in Shift-JIS encoding
 *
 * @note 48 bytes
 *
 * @note As Shift-JIS encompasses ASCII, plain text is acceptable here; Japanese
 * is not actually required and it can be (and usually is) exactly the same as
 * the international title. If you do choose to use Japanese text here, use
 * Shift-JIS encoding and keep in mind that the characters will be *two bytes*
 * in size and that the whole string must fit within the 48 byte allocated
 * space. The spacing bar will not work for Japanese characters.
 *
 */
#define HEADER_NAME_JP "HELLO WORLD                                     "
//                     "################################################"

/**
 * @def HEADER_SOFT_ID
 * @brief Software ID
 *
 * @details This is a catalogging code that was assigned by Sega when the
 * hardware was active in the market. It is meaningless at this point in time
 * and you are free to fill it out as you please. As a matter of practicality,
 * make it something that any existing software does not already use.
 *
 * @note 16 bytes
 *
 * @note The code traditionally appeared in the format: GM 00-0000-00
 * More information is available here:
 * https://plutiedev.com/rom-header#serial
 */
#define HEADER_SOFT_ID "GM 00-0000-00   "
//                     "################"

/**
 * @def HEADER_REGION
 * @brief Valid usage regions
 *
 * @details Defines the region(s) in which the software is valid
 *
 * @note 16 bytes
 *
 */
#if REGION == JP
#define HEADER_REGION "J               "
#endif
#if REGION == US
#define HEADER_REGION "U               "
#endif
#if REGION == EU
#define HEADER_REGION "E               "
#endif
// All regions, regardless of BIOS security code
// #define HEADER_REGION "JUE             "
//                    "################"

#endif

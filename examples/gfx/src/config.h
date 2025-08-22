/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file config.h
 * @brief Project configuration
 */

#ifndef MEGADEV__CONFIG_H
#define MEGADEV__CONFIG_H

#include "build.def.h"

/*
 * Hardware target
 * Valid values: MEGACD, MEGADRIVE, MEGACD_MODE1
 */
#define TARGET MEGACD

/*
 * Default hardware configuration
 * These values can be changed when running make with the HW_CFG variable to easily
 * build for different hardware, e.g.:
 * make HW_CFG="REGION=JP VIDEO=NTSC"
 * See README.md for more info on these settings
 */
// Valid values: US, EU, JP
#ifndef REGION
#define REGION US
#endif

// Valid values: NTSC, PAL
#ifndef VIDEO
#define VIDEO NTSC
#endif

// Valid values: VRAM_64K, VRAM_128K
#ifndef VRAM_SIZE
#define VRAM_SIZE VRAM_64K
#endif

/*
 * Header settings
 * The below settings will appear in the program header, located within the boot
 * sector of the disc.
 *******************************************************************************
 *    These are *size sensitive* and must occupy exactly the amount of space   *
 *   listed. You can use the sizing guide (######) below each entry to ensure  *
 *        your text fits, and unused space should be filled with spaces.       *
 *******************************************************************************
 */

/**
 * @def HEADER_VOL_ID
 * @brief Disc volume ID
 * @details
 * This will be the "name" of the disc, as displayed in a file manager when
 * mounted on a PC
 * @note 11 bytes
 */
#define HEADER_VOL_ID "GFX_EXAMPLE"
//                    "###########"

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
 * @def PROJECT_NAME
 * @brief International title
 * @details The name of the software, in plain ASCII encoding
 * @note 48 bytes
 */
#define PROJECT_NAME "GFX ROUTINES EXAMPLE                            "
//                       "################################################"

/**
 * @def PROJECT_NAME_DOMESTIC
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
#define PROJECT_NAME_DOMESTIC "GFX ROUTINES EXAMPLE                            "
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
 * @details Defines the region(s) in which the software is valid. This value will
 * be set automatically to match the region set above, but you can manually override
 * it here to set multiple regions if you wish
 *
 * @note 3 bytes
 *
 */
// #define HEADER_REGION "JUE"
//                    "###"

#endif

/**
 * @file
 * Project header and build configuration
 */

#ifndef MEGADEV__PROJECT_H
#define MEGADEV__PROJECT_H

#include "build_def.h"

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
//#define HEADER_REGION "JUE             "
//                    "################"

#endif

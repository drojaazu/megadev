/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file security.s
 * @brief Mega CD security block inclusion
 *
 * @note
 * This file should be included at the top of you IP asm source!
 */

.section .security

#error test!

#ifndef REGION
#error Hardware region not defined, cannot choose security code!
#endif

#if REGION == JP
#error JP
#endif

#if REGION == US
#error US
#endif

#if REGION == EU
#error EU
#endif

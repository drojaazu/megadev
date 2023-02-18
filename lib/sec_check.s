/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sec_check.s
 * @brief Mega CD security block inclusion
 *
 * @note
 * This file should be included at the top of you IP asm source!
 */

.section .text

#ifdef REGION
#if REGION == JP
.incbin	"sec_jp.bin"
#endif

#if REGION == US
.incbin "sec_us.bin"
#endif

#if REGION == EU
.incbin	"sec_eu.bin"
#endif
#endif
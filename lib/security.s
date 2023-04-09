/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file security.s
 * @brief Mega CD security block inclusion
 *
 * @note
 * This file should be included at the top of you IP asm source!
 */

.section .text

#ifdef REGION
#if REGION == JP
.incbin	"security_jp.bin"
#endif

#if REGION == US
.incbin "security_us.bin"
#endif

#if REGION == EU
.incbin	"security_eu.bin"
#endif
#endif
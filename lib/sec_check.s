/**
 * \file sec_check.s
 */

.section .text

// This file should be included at the top of you IP asm!

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
/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file init_mmd.h
 * @brief Set up and run a loaded MMD formatted module
 * @note This expects a module to be already loaded at the start of Word RAM
 */

#ifndef MEGADEV__MAIN_INIT_MMD_H
#define MEGADEV__MAIN_INIT_MMD_H

static inline void const * init_mmd()
{
	register void const * A0 __asm__("a0");

	__asm__(
		"\
			jbsr INIT_MMD \n\
		"
		: "+a"(A0)
		:
		: "d0", "a1", "a2");
	
	return A0;
}

#endif

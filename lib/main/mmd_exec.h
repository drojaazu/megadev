/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file mmd_exec.h
 * @brief Set up and run a loaded MMD formatted module
 * @note This expects a module to be already loaded at the start of Word RAM
 */

#ifndef MEGADEV__MAIN_MMD_EXEC_H
#define MEGADEV__MAIN_MMD_EXEC_H

static inline void mmd_exec()
{
	asm(
		"\
			jsr MMD_EXEC \n\
		"
		:
		:
		: "d0", "d7", "a0", "a1");
}

#endif

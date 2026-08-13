/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file str_util.def.h
 * @brief String handling constants
 */

#pragma once
/**
 * @def STRING_TERMINATOR
 * @brief Byte marking the end of a string in Megadev
 *
 * @details
 * Megadev strings end with 0xFF, not with a NUL. This is not a stylistic
 * choice: the Boot ROM print routines require it -- see BIOS_PRINT in
 * main/bios.h, "Strings are terminated with 0xFF and use 0x00 for newline" --
 * so 0x00 is a printable newline here rather than a terminator.
 *
 * Defined at the .def.h layer so the C and assembly implementations share one
 * value. They previously each hardcoded their own, and the C hextoa* routines
 * omitted the terminator entirely.
 */
#define STRING_TERMINATOR 0xFF

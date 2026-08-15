/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file macro.h
 * @brief General purpose C macros
 */

#pragma once

#define _MEGADEV_STR(x) #x
#define MEGADEV_STR(x) _MEGADEV_STR(x)

/**
 * @brief Define a global, compile time symbol
 *
 * Creates an absolute, global symbol with the given value. The symbol is a
 * link time constant visible to the linker script, not a variable: it occupies
 * no space in .rodata, .data, .bss or any other section.
 *
 * This is how a module declares its layout (MODULE_ROM_ORIGIN and friends)
 * from within a C source file. It is the C counterpart to the GLOBAL macro in
 * macro.s, and produces an identical symbol, so a layout may be written in
 * either language.
 *
 * Use at file scope only.
 *
 * The value is evaluated by the assembler rather than the C compiler, so it
 * must be an integer literal, another symbol defined this way, or arithmetic
 * over those. Anything needing the C front end - sizeof, enum constants,
 * casts - will not work here.
 *
 * @code
 * GLOBAL_SYM(MODULE_ROM_ORIGIN, WORK_RAM);
 * GLOBAL_SYM(MODULE_RAM_ORIGIN, 0xFFF500);
 * GLOBAL_SYM(MODULE_ROM_LENGTH, MODULE_RAM_ORIGIN - MODULE_ROM_ORIGIN);
 * @endcode
 *
 * @param name Symbol name
 * @param value Integer value
 */
#define GLOBAL_SYM(name, value) \
	__asm__(".global " #name "\n\t.equ " #name ", " MEGADEV_STR(value))

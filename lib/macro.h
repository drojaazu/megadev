/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file macro.h
 * @brief General purpose C macros
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define _MEGADEV_STR(x) #x
#define MEGADEV_STR(x) _MEGADEV_STR(x)

/**
 * @def GLOBAL_SYM
 * @brief Define a global, link-time symbol from C
 *
 * @details
 * Creates an absolute, global symbol with the given value. It is a link-time
 * constant visible to the linker script, not a variable: it occupies no space
 * in any section. The symbol is hidden, so it is not exported to modules that
 * link against this one (tools/exports.sh). This is how a module declares its layout (MODULE_ROM_ORIGIN
 * and friends) from a C source file; it is the counterpart of the GLOBAL
 * macro in macro.s and produces an identical symbol, so a layout may be
 * written in either language.
 *
 * Use at file scope only. The value is evaluated by the assembler, so it must
 * be an integer literal, another symbol defined this way, or arithmetic over
 * those; sizeof, enum constants and casts are not available.
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
  __asm__(".global " #name "\n\t.hidden " #name "\n\t.equ " #name ", " MEGADEV_STR(value))

#ifdef __cplusplus
}
#endif

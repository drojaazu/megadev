/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file str_util.def.h
 * @brief String conventions shared by C and assembly
 */

#pragma once

/**
 * @def STRING_TERMINATOR
 * @brief Byte that ends a string passed to the Boot ROM print routines,
 * which treat 0x00 as a newline
 */
#define STRING_TERMINATOR 0xFF

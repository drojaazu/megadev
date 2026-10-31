/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file mode1.h
 * @brief Mode 1 support (Mega CD driven from a cartridge)
 *
 * @details
 * Main CPU only. The program must assemble mode1.s and kos_cmp.s. See
 * mode1.def.h for the return values.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "main/mode1.def.h"
#include "types.h"

/**
 * @brief Check for a Mega CD
 * @return true if the Boot ROM is present at BOOT_ROM
 */
static inline bool mode1_detect()
{
  register u32 D0 asm("d0");
  asm volatile("jsr mode1_detect" : "=d"(D0) : : "cc");
  return D0 != 0;
}

/**
 * @brief Locate the Boot ROM's 1bpp ASCII font
 * @return Pointer to the 96 tile font, or NULL if it could not be found
 * @details Load it with bios_load_1bpp_tiles; bios_load_font and
 * bios_load_font_defaults do not work in Mode 1.
 */
static inline void * mode1_find_font()
{
  register u32 A1 asm("a1");
  asm volatile("jsr mode1_find_font" : "=a"(A1) : : "d0", "d1", "cc");
  return (void *) A1;
}

/**
 * @brief Bring up the Sub CPU with the given SP
 * @param sp Pointer to the SP binary
 * @param size Size of the SP binary in bytes
 * @return MODE1_OK or a MODE1_ERR_* value
 */
static inline u8 mode1_load_sub(void const * sp, u32 size)
{
  register u32 A0 asm("a0") = (u32) sp;
  register u32 D0 asm("d0") = size;
  asm volatile("jsr mode1_load_sub"
               : "+d"(D0), "+a"(A0)
               :
               : "d1", "a1", "cc", "memory");
  return (u8) D0;
}

#ifdef __cplusplus
}
#endif

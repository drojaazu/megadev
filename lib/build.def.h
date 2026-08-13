/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file build.def.h
 * @brief Target hardware components definitions
 *
 */

#ifndef MEGADEV__BUILD_DEF_H
#define MEGADEV__BUILD_DEF_H

// For use with the TARGET definition
#define MEGADRIVE 0
#define MEGACD 1
#define MEGACD_MODE1 2

// For use with the REGION definition
#define JP 0
#define US 1
#define EU 2

// For use with the VIDEO definition
#define NTSC 0
#define PAL 1

// For use with VRAM_SIZE definition
#define VRAM_64K 0
#define VRAM_128K 1

/**
 * @defgroup bitfields Bitfield accessors
 * @brief Insert and extract multi-bit register fields
 *
 * @details
 * Register fields are described by three constants following the
 * `<SUBSYSTEM>_<FIELD>_<ASPECT>` scheme (see docs/manual.md):
 *
 * - `_POS`   the bit position of the field's low bit
 * - `_WIDTH` how many bits it occupies
 * - `_MASK`  the bits it occupies, derived from POS and WIDTH
 *
 * Field *values* are stored **unshifted**; these macros place them. A
 * single-bit flag is simply a field of width 1, so the same scheme covers
 * both cases and `_POS` doubles as the operand for `btst` and friends.
 *
 * These are preprocessor-only and therefore legal in both C and assembly.
 * @{
 */

/**
 * @brief Build a field mask from its position and width
 */
#define FIELD_MASK(pos, width) (((1 << (width)) - 1) << (pos))

/**
 * @brief Place an unshifted value into its field
 * @param field Field name without the _POS/_MASK suffix, e.g. `VDP_INTERLACE`
 * @param value Unshifted value
 */
#define FIELD_PREP(field, value) ((((value)) << field##_POS) & field##_MASK)

/**
 * @brief Extract a field from a register value, unshifted
 * @param field Field name without the _POS/_MASK suffix
 * @param reg Register value
 */
#define FIELD_GET(field, reg) (((reg) &field##_MASK) >> field##_POS)

/** @} */

#endif

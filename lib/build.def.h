/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file build.def.h
 * @brief Target hardware components definitions
 *
 */

#pragma once

/** @name TARGET values, from the project makefile */
///@{
#define MEGADRIVE    0 ///< Mega Drive cartridge, no Mega CD
#define MEGACD       1 ///< Mega CD disc
#define MEGACD_MODE1 2 ///< Cartridge driving a Mega CD
///@}

/** @name REGION values, from the project makefile */
///@{
#define JP 0 ///< Japan
#define US 1 ///< North America
#define EU 2 ///< Europe
///@}

/** @name VIDEO values, from the project makefile */
///@{
#define NTSC 0 ///< 60 Hz, 224 lines
#define PAL  1 ///< 50 Hz, 240 lines
///@}

/** @name VRAM_SIZE values, from the project makefile */
///@{
#define VRAM_64K  0 ///< Standard 64 KB of VRAM
#define VRAM_128K 1 ///< 128 KB VRAM (only for Teradrive or modified Megadrive)
///@}

/**
 * @defgroup bitfields Register field helpers
 * @brief Build masks, and insert or extract register fields
 *
 * @details
 * A register field is described by constants named
 * `<SUBSYSTEM>_<FIELD>_<ASPECT>`:
 *
 * - `_POS`   the bit position of the field's low bit; also the operand for
 *            btst, bset and bclr
 * - `_WIDTH` how many bits it occupies, present only for fields wider than
 *            one bit; a field with no `_WIDTH` is a single-bit flag
 * - `_MASK`  the bits it occupies; `(1 << _POS)` for a flag, FIELD_MASK for
 *            a wider field so that it cannot disagree with POS and WIDTH
 *
 * Field values are stored unshifted and placed with FIELD_PREP or read with
 * FIELD_GET, which need only `_POS` and `_MASK` and so work for flags and
 * fields alike. All of these are preprocessor-only and legal in both C and
 * assembly.
 * @{
 */

/**
 * @brief Build a field mask from its position and width
 */
#define FIELD_MASK(pos, width) (((1 << (width)) - 1) << (pos))

/**
 * @brief Place an unshifted value into its field
 * @param field Field name without the _POS/_MASK suffix
 * @param value Unshifted value
 */
#define FIELD_PREP(field, value) (((value) << field##_POS) & field##_MASK)

/**
 * @brief Extract a field from a register value, unshifted
 * @param field Field name without the _POS/_MASK suffix
 * @param reg Register value
 */
#define FIELD_GET(field, reg) (((reg) & field##_MASK) >> field##_POS)

/**
 * @brief Address of the byte holding a field, for bit operations on a 16-bit
 * register
 *
 * @details
 * M68k bit instructions on memory operate on one byte, and take the bit
 * number modulo 8. A field at position 8 or above in a 16-bit register must
 * therefore be addressed through its high byte: the 68000 is big-endian, so
 * bits 15..8 are at the register address and bits 7..0 at the next byte.
 *
 *     btst #FIELD_BPOS(GA_SUB_CDCMODE_EDT), FIELD_BYTE(GA_SUB_REG_CDCMODE,
 * GA_SUB_CDCMODE_EDT)
 *
 * Written as arithmetic because GNU as has no conditional operator.
 */
#define FIELD_BYTE(reg, field) ((reg) + 1 - (((field##_POS) >> 3) & 1))

/**
 * @brief Bit number of a field within its own byte, for bit operations
 * @sa FIELD_BYTE
 */
#define FIELD_BPOS(field) ((field##_POS) & 7)

/** @} */

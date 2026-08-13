/**
 * @file ipx_api.def.h
 * @brief The IPX kernel's published interface
 *
 * @details
 * The IPX is memory resident: it is loaded once and stays put. Transient
 * modules call into it through a jump table at a fixed address rather than by
 * linking against its symbols, so the IPX can be rebuilt without relinking
 * every module that uses it. This is the same approach the Mega CD Boot ROM
 * takes with its own table (see docs/main_bios.md).
 *
 * @warning The table is APPEND ONLY. Reordering or removing an entry changes
 * what every previously built module jumps to, and nothing will diagnose it.
 * Add new entries at the end.
 */

#ifndef IPX_API_DEF_H
#define IPX_API_DEF_H

#include <main/memmap.def.h>

/** @brief The IPX loads at the start of Work RAM, so its table is here */
#define IPX_JMPTBL WORK_RAM

/** @brief Bytes reserved for the table; must match MODULE_JMPTBL_SIZE */
#define IPX_JMPTBL_SIZE 0x100

/** @brief One `jmp <abs.l>` is 6 bytes */
#define IPX_JMP_SIZE 6

/** @brief Shared variables sit immediately after the reserved table */
#define IPX_SHARED (IPX_JMPTBL + IPX_JMPTBL_SIZE)

/** @brief Address of table entry @p n */
#define IPX_ENTRY(n) (IPX_JMPTBL + ((n) *IPX_JMP_SIZE))

/**
 * @brief The published API, in table order. APPEND ONLY.
 *
 * Expanded into the table itself by ipx_jmptbl.s and into call macros by
 * ipx.h, so the two can never disagree about which entry is which.
 */
#define IPX_API_LIST     \
	X(0, init_particles)   \
	X(1, process_particles)

#endif

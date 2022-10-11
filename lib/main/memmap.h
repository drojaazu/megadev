/**
 * @file
 * @brief C wrappers for hardware mapping, Gate Array (GA) registers and BIOS
 * vectors on the Main CPU side
 */

#ifndef MEGADEV__CD_GA_MAIN_H
#define MEGADEV__CD_GA_MAIN_H

#include "main/memmap_def.h"

/**
 * @sa _WRDRAM
 */
#define WRDRAM ((volatile void *) _WRDRAM)

/**
 * @sa _PRGRAM
 */
#define PRGRAM ((volatile void *) _PRGRAM)

/**
 * @note The system jump table gives you six bytes to work with per entry,
 * which are usually implemented as a JSR (2 bytes) and a vector (4 bytes).
 * We only provide C wrappers for the last four bytes as a pointer, since
 * there is really no reason to change the JSR opcode. If such a change is
 * necessary for some reason, it should be done in asm.
 */

// L6 interrupt = VINT
#define MLEVEL6_VECTOR ((void volatile *(*) ) (_MLEVEL6 + 2))
// L4 interrupt = HINT
#define MLEVEL4_VECTOR ((void volatile *(*) ) (_MLEVEL4 + 2))
// L2 interrupt = Ext. port
#define MLEVEL2_VECTOR ((void volatile *(*) ) (_MLEVEL2 + 2))

#endif

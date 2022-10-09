/**
 * @file
 * @brief C wrappers for hardware mapping, Gate Array (GA) registers and BIOS
 * vectors on the Main CPU side
 */

#ifndef MEGADEV__CD_GA_MAIN_H
#define MEGADEV__CD_GA_MAIN_H

#include "main/main_def.h"
#include "types.h"

/**
 * @sa _WRDRAM
 */
#define WRDRAM ((volatile void *) _WRDRAM)

/**
 * @sa _PRGRAM
 */
#define PRGRAM ((volatile void *) _PRGRAM)

#endif

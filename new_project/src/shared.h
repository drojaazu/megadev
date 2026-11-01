// This file contains definitions that are shared by the Main CPU and Sub CPU
// This will primarily be commands, parameters and responses

#ifndef SHARED_H
#define SHARED_H

// Work RAM map. The IPX stays resident while the other modules run, so the
// regions must not overlap; the module layouts are derived from these.
#define IPX_ROM_ORIGIN 0xFF0000     // IPX code and read-only data
#define IPX_ROM_LENGTH 0x8000
#define MODULE_VARS_ORIGIN 0xFF8000 // variables of the modules run by the IPX
#define MODULE_VARS_LENGTH 0x7000
#define IPX_RAM_ORIGIN 0xFFF000     // IPX variables, including the crash handler's
#define IPX_RAM_LENGTH 0x700        // up to 0xFFF700, where the Boot ROM's work area begins

#define CMD_LOAD_FILE 1
#define CMD_PLAY_CDDA 2

#define FILE_IPX_MMD 0
#define FILE_EX1_MMD 1
#define FILE_EX2_MMD 2
#define FILE_EX3_MMD 3

#endif

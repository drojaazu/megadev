/*
				This was from an old version of the bram example, it MAY be a possible
	 way of aliasing/redefining a symbol from eg the linker script for use in
	 asm...
*/

#include "main/bootlib_def.h"

// We'll put all of our Main side memory layout definitions here so it's
// easier to track them all in one place

/*

FF0000  +----------------+
				| IPX ROM        |
				|                |
				|                |
				|                |
				|                |
FFE000  +----------------+
				| IPX RAM        |
				|                |
FFF600  +----------------+
				| IP Resident    |
FFF700  +----------------+
				| Boot ROM Use   |
				|                |
FFFC00  +----------------+
				| Stack          |
FFFD00  +----------------+
				| System / Boot  |
				|       ROM Use  |
FFFFFF  +----------------+

*/

// We first need to define where in memory the resident IP routines should be
// placed. We want to place it high up in the user block where it won't get in
// the way, so we'll place it 0x100 bytes before the end of the user block.
// Of course, the size of the user block depends on whether or not we plan to
// use Boot ROM functionality.

// Assuming we do plan to use the Boot ROM, we can just subtract 0x100 from the
// already defined beginning of Boot ROM work RAM. (Refer to the modules readme
// for a look at the Boot ROM memory map.)
#define RESIDENT_IP_ORIGIN (_BLIB_BUFFER - 0x100)

// If we don't plan to use the Boot ROM, then the upper bound is the System
// Use block at 0xFFFD00. However, this is also the location of the stack, so we
// allocate some space for that. 0x200 should be enough in general, though
// you can modify that if necessary.
// #define RESIDENT_IP_ORIGIN (SYSTEMUSE_BASE - 0x300)

/*
	Next we define the IPX boundaries. Of the 0xF600 bytes remaining, we'll
	allocate 0x1600 bytes for RAM and leave the rest for ROM.
*/
#define IPX_ROM_ORIGIN _ip_text_len
#define IPX_RAM_ORIGIN 0xFFF600
#define IPX_ROM_SIZE (IPX_RAM_ORIGIN - IPX_ROM_ORIGIN)
#define IPX_RAM_SIZE (RESIDENT_IP_ORIGIN - IPX_RAM_ORIGIN)

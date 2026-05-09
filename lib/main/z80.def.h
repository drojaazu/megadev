/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file z80.def.h
 * @brief Z80 hardware definitions
 * @todo need more details here, including specifying different address for Main
 * CPU and direct Z80 access, and defines and macros for YM2612 register access
 */

#ifndef MEGADEV__MAIN_Z80_DEF_H
#define MEGADEV__MAIN_Z80_DEF_H

/**
 * @defgroup z80 Main CPU / Z80
 */

#define Z80_RAM 0xA00000

#define Z80_RAM_SIZE 0x2000

#define Z80_FM1_SEL 0xA04000

#define Z80_FM1_DATA 0xA04001

#define Z80_FM2_SEL 0xA04002

#define Z80_FM2_DATA 0xA04003

#define Z80_BANK 0xA06000

#define Z80_PSG 0xA07F11

/*
Z80 Control Registers

If the 68k wishes to access anything in the Z80 address space, the Z80 must be
stopped. This can be accomplished through the register at $A11100. To stop the
Z80 and send a bus request, #$0100 must be written to $A11100. To see if the Z80
has stopped, bit 0 of $A11100 must be checked - if it's clear, the 68k may
access the bus, but wait if it is set. Once access to the Z80 area is complete,
#$0000 needs to be written to $A11100 to return the bus back to the Z80. No
waiting to see if the bus is returned is required here — it is handled
automatically.

However, if the Z80 is required to be reset (for example, to load a new program
to it's memory) this may be done by writing #$0000 to $A11200, but only when the
Z80 bus is requested. After returning the bus after loading the new program to
it's memory, the Z80 may be let go from reset by writing #$0100 to $A11200.
*/

/**
 * @def Z80_REG_BUSREQ
 * @brief Z80 Bus Request / Register
 * @ingroup z80
 */
#define Z80_REG_BUSREQ 0xA11100

/**
 * @def Z80_BUS_REQUEST
 * @ingroup z80
 * @brief Z80 Bus Request / Request control of Z80 data bus
 * @details Use with @ref z80_reg_busreq
 */
#define Z80_BUS_REQUEST 0x0100

/**
 * @def Z80_BUS_RELEASE
 * @ingroup z80
 * @brief Z80 Bus Request / Release control of Z80 data bus
 * @details Use with @ref z80_reg_busreq
 */
#define Z80_BUS_RELEASE 0x0000

/**
 * @def Z80_BIT_STATUS
 * @ingroup z80
 * @brief Z80 Bus Request / Z80 CPU Status
 * @details Read from @ref Z80_REG_BUSREQ
 * @return Bit set: Z80 CPU is operating / Bit clear: Z80 CPU is stopped
 */
#define Z80_BIT_STATUS 0

/**
 * @def Z80_REG_RESET
 * @ingroup z80
 * @brief Z80 Reset / Register
 */
#define Z80_REG_RESET 0xA11200

/**
 * @def Z80_RESET_ASSERT
 * @ingroup z80
 * @brief Z80 Reset / Assert reset of Z80
 * @details Use with @ref Z80_REG_RESET
 */
#define Z80_RESET_ASSERT 0x0000

/**
 * @def Z80_RESET_RELEASE
 * @ingroup z80
 * @brief Z80 Reset / Release reset of Z80
 * @details Use with @ref Z80_REG_RESET
 */
#define Z80_RESET_RELEASE 0x0100

#endif

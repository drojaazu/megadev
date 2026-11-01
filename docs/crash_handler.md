# Crash Handler

When the program fails with an exception, such as an address error, an illegal instruction or a divide by zero, the crash handler shows the CPU state on screen and sends it over the serial port. It covers the Main CPU on a cartridge, in Mode 1 and on the Mega CD, and the Sub CPU on the Mega CD and in Mode 1. It is linked only when installed.

# The report

    MAIN CPU: ADDRESS ERROR

    PC 000002DA  SR 2710
    ACCESS 00FF8001 READ DATA FC5
    IR 3028

    D0 xxxxxxxx  A0 xxxxxxxx
    ...
    D7 xxxxxxxx  A7 xxxxxxxx

    STACK
    xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx

- `PC` is the program counter the CPU pushed. For most exceptions it is the instruction after the one that failed. For an address error it is a few bytes past the failing instruction, which is shown by `IR`.
- `ACCESS` appears for address and bus errors: the address, whether it was a read or a write, and the 68000 function code.
- `A7` is the stack pointer before the exception, and `STACK` the eight longs above it, which usually include return addresses.

The handler saves the registers, moves to its own 512-byte stack, sets up the VDP itself and does not return. The text also goes out over the serial port configured by `EXT_PORT` (`main/comm.def.h`) when its serial output is enabled, as `init_ext_port` does.

# Main CPU

The program assembles `exvec_main.s`.

**Cartridge, including Mode 1.** Include `main/exvec_cart.s` in the project's `init.s` after `md_vectors.s`, in place of the `GLOBAL` lines for the exception vectors, as the `cart` example does. The ROM vector table then points at the handlers.

**Mega CD.** Call `exvec_install` (`main/exvec.h`). It writes the handlers into the system vector table at 0xFFFD00. The Boot ROM shares one entry between address errors and illegal instructions, and the handler tells them apart from the stack frame. Bus errors go to the Boot ROM's reset code and are not caught. A Mode 1 program whose vectors lead to the system vector table, like the `mode1` example, can use `exvec_install` too.

# Sub CPU

Include `sub/exvec_sub.s` in the SP and call `exvec_install` (`sub/exvec.h`) from `sp_init`, after the Sub BIOS has built its jump table.

The Sub CPU has no display. Its handler saves the CPU state in Program RAM, signals the Main CPU through COMSTAT5 to COMSTAT7 and halts. Call `exvec_check_sub` (`main/exvec.h`) on the Main CPU regularly, for example once per frame. After a Sub CPU crash, it takes the Sub CPU's bus, reads the saved state and shows the report with `SUB` in place of `MAIN`.

# Font

The report uses `font8x8` by default. Set `EXVEC_FONT:=BIOS` in the project's makefile to use the Boot ROM's font instead, on the Mega CD or in Mode 1. The font is read from the Boot ROM, so the Boot ROM library does not need to be in a working state.

# Memory

The handler uses about 640 bytes of the program's RAM: its saved state and its stack. A module whose RAM is smaller fails to link.

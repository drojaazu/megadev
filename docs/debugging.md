Debugging a Megadev program on real hardware: the serial debug monitor, breakpoints and single-stepping, GDB over the controller port, and loading builds without touching the console.

# Contents

- Overview
- Hardware
- Wiring the monitor into a program
- The command line host
- GDB
- Breakpoints
- Modules and other code that moves
- Loading builds from the host
- Limits
- Protocol

---

# Overview

Emulators show the program's state for free; the hardware does not. The debug monitor (`main/monitor.h`) is a small command interpreter linked into the Main CPU program that a host drives over the serial link of a controller port: it reads and writes memory and the VDP, sets breakpoints, single-steps, and reports and changes the registers of a stopped program. Two host programs talk to it, both in `tools/` and needing nothing beyond Python's standard library:

- `mdmon.py` runs one command at a time from the shell: dump memory, poke a value, set a breakpoint, show the registers.
- `mdgdb.py` serves the GDB remote serial protocol on a TCP port, so a 68000 GDB debugs the program with `break`, `continue`, `stepi`, `info registers` and `x`, with the symbols of the build.

The monitor is for programs that run. When the program dies with an exception, the crash handler (`docs/crash_handler.md`) shows the state instead; the two share the `CrashState` layout and can be linked together.

# Hardware

The link is the serial mode of a controller port, 4800 baud, 8 data bits, no parity, one stop bit. The Mega Drive's I/O pins are 5 V, so the adapter on the PC side must be a 5 V TTL serial adapter, not a 3.3 V one and never RS-232 levels. In serial mode the console transmits on TL (pin 6) and receives on TR (pin 9), with ground on pin 8: the adapter's RX goes to pin 6 and its TX to pin 9. Do not connect the adapter's power pin; the console provides 5 V on pin 5 for the controller, and the adapter is powered by the PC. The port is chosen with `EXT_PORT` (default: port 2) and the speed with `EXT_BAUD`, in the project's makefile:

    CC_FLAGS+=-DEXT_PORT=2

The host sees the adapter as `/dev/ttyUSB0` or similar; the user needs access to it, on most Linux systems membership of the `dialout` group. The link is slow: 4800 baud is 480 bytes per second, and a 128-byte read takes about a quarter of a second once the protocol overhead is counted. It is enough for breakpoints and inspecting state, not for dumping VRAM in bulk.

# Wiring the monitor into a program

Three steps, all on the Main CPU:

1. **Initialize and poll.** Call `monitor_init` once after the VDP is set up. It configures the serial port with the receive interrupt enabled. Call `monitor_poll` regularly, from the main loop or the VBLANK handler; it parses the complete frames that have arrived and answers them.

2. **Receive in the interrupt.** Bytes arrive faster than once per frame and the port holds only one, so receive must run from the external interrupt (level 2). Set `VDP_EXTINT_ENABLE` in VDP mode register 3 and have the level 2 handler call `monitor_rx_isr`:

        __attribute__((interrupt)) void INT2_EXT() { monitor_rx_isr(); }

3. **Vectors for breakpoints and stepping.** Breakpoints use `TRAP #15` and single-stepping uses the trace exception, both entered through `main/monitor_break.s`, which the project assembles alongside its other sources.
   - A cartridge, including Mode 1, includes `main/monitor_cart.s` in its `init.s` before `main/crash_cart.s`, which then leaves the trace vector to the monitor:

            #include "main/monitor_cart.s"
            #include "main/crash_cart.s"

   - A Mega CD program, or a Mode 1 program whose vectors lead to the system vector table, calls `monitor_install` after `monitor_init`. It writes the two handlers into the system vector table.

Without step 3 the monitor still identifies the program and reads and writes memory, but every breakpoint and step command is answered with an error, since there is nothing to catch the exceptions.

The monitor costs about 3.8 KB of ROM and 600 bytes of RAM: the receive ring, the frame buffer, the breakpoint table and the saved CPU state. Build the program as usual; the monitor adds no build flags. Projects that want to see C variables and source lines in GDB add debug information:

    CC_FLAGS+=-g

The kit compiles at `-O1`, which keeps most variables where GDB can find them. The build keeps the ELF next to each output (`build/NAME.cart.elf`, `build/ip.bin.elf`, `build/NAME.mmd.elf`); GDB loads symbols from those, the console runs the binary.

# The command line host

`tools/mdmon.py [-d DEVICE] [-b BAUD] [-t SECONDS] COMMAND ...` sends one command and prints the reply. Running it without a command lists them. The ones used most:

| Command | Effect |
|---|---|
| `hello` | Protocol version, target (Mega Drive, Mega CD, Mode 1), stopped flag |
| `rd ADDR COUNT`, `rdw ADDR COUNT` | Read bytes (byte accesses) or words (word accesses); `-o FILE` writes the bytes to a file |
| `wr ADDR HEXBYTES`, `wrw ADDR HEXWORDS` | Write bytes or words |
| `rdp ADDR COUNT` | Read one word port repeatedly, for the VDP data port and similar |
| `vram ADDR COUNT`, `cram`, `vramw ADDR HEXWORDS` | Read and write VRAM and CRAM through the VDP ports, as one atomic command |
| `bp ADDR`, `bc ADDR` | Set and clear a breakpoint; the reply says whether it was patched into RAM or runs in trace mode |
| `status` | Stopped or running, trace mode on or off, and the PC and exception vector of the stop |
| `regs` | The registers of a stopped program |
| `step`, `cont` | Execute one instruction, or continue |
| `halt` | Stop a running program at its next instruction |
| `wait [SECONDS]` | Poll until the program stops |

Addresses take any Python integer syntax: `0xFF0000` or `16711680`. The host refuses reads that touch the PSG at 0xC00010 to 0xC0001F, because a read of that range locks the 68000 (so does any other non-atomic access to a VDP port; use the `vram`, `cram` and `rdp` commands for those).

A session on a running program looks like this:

    $ tools/mdmon.py hello
    monitor v1, target Mega Drive, flags 00
    $ tools/mdmon.py bp 0x2F4
    breakpoint at 000002F4: patched
    $ tools/mdmon.py wait
    stopped at 000002F4 (breakpoint)
    $ tools/mdmon.py regs
    ...
    $ tools/mdmon.py cont

Memory can be read and written while the program runs; register access, stepping and continuing need a stopped program.

# GDB

GDB for the 68000 is not in most distributions; `work/trace/build-m68k-gdb.sh` in the kit's maintainer notes builds GDB 16.3 as `m68k-elf-gdb` next to the toolchain. Any GDB with the m68k target works, including a `gdb-multiarch` where the distribution has one.

Start the server, then connect GDB to it with the ELF of the running build:

    $ tools/mdgdb.py -d /dev/ttyUSB0 -p 1234
    mdgdb: monitor on /dev/ttyUSB0 at 4800, listening on :1234

    $ m68k-elf-gdb build/mygame.cart.elf
    (gdb) target remote :1234
    (gdb) break tick_rom
    (gdb) continue
    Breakpoint 1, 0x000002f4 in tick_rom ()
    (gdb) info registers
    (gdb) x/4xw &ram_ticks
    (gdb) stepi
    (gdb) detach

Attaching to a running program halts it, which is what GDB expects; `continue` lets it run. Ctrl-C while the program runs stops it at its next instruction, `detach` resumes it and closes the connection, `kill` closes the connection and leaves the program stopped. GDB's `break`, `tbreak`, `hbreak` and `delete` map onto the monitor's breakpoints; `stepi`, `nexti`, `step`, `next` and `finish` work through single-stepping, and the last three are slow at 4800 baud since every step is a round trip. `info registers`, `p $d0 = 5`, `set var`, `x`, `print` and `display` work on the stopped program; memory reads while the program runs are served live, so `display` in a running program is possible from a second command.

Without debug information GDB knows functions and global symbols from the ELF but not types: `print rom_ticks` reports an unknown type until the project builds with `-g`, and `break` on a data symbol needs the `*ADDR` form. A GDB built without expat warns that it cannot parse the target description; the server then serves GDB's default 68000 register layout with floating point registers, which read as zero. The warning is harmless.

# Breakpoints

A breakpoint is a `TRAP #15` instruction written over the first word of the instruction at the address, with the original word saved. When the trap is hit the monitor reports the stop at the breakpoint's address, and when the program continues from it the original instruction is put back for exactly one instruction, executed under trace, and the trap is written again. Eight breakpoints can be set at once.

Code the CPU cannot write to, which means cartridge ROM, cannot be patched. The monitor detects this when the written word does not read back and falls back to **trace mode**: the trace bit stays set while the program runs, every instruction raises a trace exception, and an assembly fast path compares the program counter against the armed addresses. The program keeps running, including its interrupt handlers, but at a small fraction of its speed; one trace-mode breakpoint is practical for catching a routine once, several are not. The reply to a breakpoint command says which mode was used, and `status` reports whether trace mode is on. Clearing the last trace-mode breakpoint turns it off at the next instruction. On a Mega CD everything but the Boot ROM is RAM, so every breakpoint in a disc program is patched; on a cartridge, code copied to Work RAM is patched and code in ROM traces.

While the program is stopped, the monitor loops on the serial link with the interrupt mask lowered to level 1. The program's own interrupt handlers keep running, so VBLANK work such as DMA queues continues, and a breakpoint inside an interrupt handler stops there with the handler's frame on the stack. The stopped program's registers can be changed, including the program counter and status register; GDB's `set $pc` and `jump` use this.

# Modules and other code that moves

A Mega CD program is several ELFs: the IP in Work RAM and the modules it loads, each linked at its own address (`docs/modules.md`). GDB's overlay commands (`overlay manual`, `overlay map`) are for one ELF whose sections share an address range and are loaded and unloaded by the program itself through an `_ovly_table` the target maintains. The kit's modules are separate programs at fixed addresses, so that machinery is the wrong fit and not needed: GDB can hold the symbols of several ELFs at once, each at its linked address.

    (gdb) file build/ip.bin.elf
    (gdb) target remote :1234
    (gdb) add-symbol-file build/title.mmd.elf
    (gdb) break title_loop
    ...
    (gdb) remove-symbol-file build/title.mmd.elf
    (gdb) add-symbol-file build/stage1.mmd.elf

Every module ELF is linked where it runs, so no address is given to `add-symbol-file`: a module with `MMD_DEST` is linked at its destination, and one that runs in place in Word RAM is linked at 0x200100. Two modules that occupy the same addresses in turn give GDB two symbols for one address if both are loaded, and GDB picks whichever it finds first, so remove a module's symbols when the program unloads it.

Breakpoints are the part to watch, because the monitor patches them into the code itself, and the code changes under it:

- **Set them while the module is resident.** A patch written before the module is loaded is overwritten by the load, and the monitor still believes it is armed. GDB inserts its breakpoints each time the program continues and removes them at each stop, so a breakpoint set while stopped in the IP, before the load, is inserted into whatever occupies the address at that moment. The reliable pattern is to break at the module's entry, which the IP knows (the pointer `init_mmd` returns, or the module's `main` once its symbols are added), and set the module's breakpoints from that stop.
- **Clear them before the module is unloaded.** When a breakpoint is cleared the monitor writes the saved word back, but only if the trap is still there; a module loaded over a patched breakpoint owns those bytes now, and the monitor leaves them alone. A breakpoint that is still set when the module is replaced is therefore harmless to the new code, but it is also gone, and the host does not know: delete it and set it again after the new load.
- **Word RAM must be on the Main CPU side.** A module in 2M Word RAM is unreadable to the Main CPU while the Sub CPU holds it. A breakpoint set then does not read back, the monitor takes it for ROM and falls back to trace mode, and the saved word is garbage. Set and clear Word RAM breakpoints while the Main CPU owns Word RAM, which is the case whenever the module runs.
- **Breakpoints in code that moves with it.** A module copied to Work RAM by `init_mmd` (one with `MMD_DEST`) is patched at its destination, where it runs. Breakpoints set in the Word RAM staging copy would be copied along as `TRAP #15` words and then trap at an address the monitor does not know, which stops the program with a breakpoint vector and no matching slot; the host sees the stop and can continue, but set breakpoints at the runtime address instead.

A cartridge program that copies routines into Work RAM (the `cart` example does, for self-modifying tests) has the same rules in miniature: the ROM copy traces, the RAM copy is patched, and a breakpoint survives only as long as the copy does.

# Loading builds from the host

A debugging loop that needs a trip to the console to press reset is a slow one. With a Mega EverDrive PRO on its USB port, `tools/prolink.py` loads and starts a cartridge build without touching the console:

    $ tools/prolink.py load build/mygame.cart
    524288 bytes loaded and started in 0.4 s

It resets the console through the cartridge, writes the ROM into the cartridge's memory and starts it, following the cartridge's own USB protocol. `prolink.py status` checks the cartridge, `prolink.py reset` pulses reset, and `prolink.py reset hard` recovers a console whose CPU is wedged, which the cartridge reports as status 05. The Mega EverDrive X7 has a different protocol and is not supported by the script.

Disc images run from the PRO's SD card, not from the cartridge's memory. `prolink.py disc build/mygame.iso` copies the image into the card's `usb-games` folder through the cartridge's file commands, at about 300 KB per second, has the cartridge compute the file's CRC-32 and compares it with the local file, then asks the menu to start it. Menus from before that command ignore the request, and the script says so; the image is on the card either way, and starting it is one pick in the menu's `usb-games` folder instead of a card swap. `prolink.py sdput FILE SDPATH` copies any file the same way and `sdcrc` compares one. Mode 1 programs are cartridges and load like any other.

Capturing the console's video output on the host, so that the screen is visible next to GDB, needs no kit support: any USB HDMI or composite capture device and a player with a low latency setting do it. Audio and video from the console reach the host with about half a second of delay.

# Limits

- **Main CPU only.** The Sub CPU has no serial port of its own. Its state after a crash comes through the crash handler (`crash_check_sub`); live debugging of the Sub CPU would relay through the Main CPU and the communication registers, and is not implemented.
- **Speed.** 4800 baud. Reading 1 KB takes about three seconds, and each GDB `step` or `next` is a series of round trips. Use `stepi` and breakpoints, not `next` through loops.
- **Trace mode is slow.** A breakpoint in ROM slows the program by a large factor while armed. Copy the routine to RAM, or break on it once and clear the breakpoint.
- **Interrupts keep running while stopped.** This keeps the display alive and lets DMA queues drain, but a VBLANK handler that reads the same variables the host is poking sees them change, and a program whose VBLANK handler expects the main loop to have run each frame will notice the stop.
- **No hardware watchpoints.** The 68000 has nothing to support them, so GDB falls back to software watchpoints, which single-step the program over the serial link and compare after every instruction: usable across a few instructions, not across a frame.
- **The monitor is in the program.** It stops working when the program overwrites its RAM, disables the external interrupt, or reprograms the controller port. A hung console is diagnosed with the crash handler and the console's own display, then with the monitor once the program lives again.
- **Serial reception needs the Z80 bus.** Every access to the I/O area requests the Z80 bus, as `main/comm.h` describes. A program that keeps the Z80 bus for long stretches delays the monitor's replies and can drop received bytes, which the monitor counts in `monitor_rx_dropped`.

# Protocol

For writing another host, or extending the monitor. Every frame, in both directions, is

    start, length, body..., checksum

where `start` is 0xA5 from host to console and 0x5A from console to host, `length` counts the body bytes (1 to 132), the first body byte is the command, or the status in a reply, and `checksum` is the XOR of `length` and every body byte. Values are big endian. A frame with a bad checksum or an unknown command is answered with a status and an empty body. `main/monitor.def.h` lists every command with its body and reply, the status codes and the breakpoint modes; `tools/mdmon.py` is a reference implementation in about 200 lines, and the GDB server builds on it.

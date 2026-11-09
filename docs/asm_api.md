Megadev assembly API: the routines and macros of `lib/*.s`, with their register conventions.

# Contents

- Scope
- Calling the Sub BIOS
- General macros
- Main CPU
- Sub CPU
- Compiler runtime

---

# Scope

Most of the kit is reached from C, and the C headers are its reference. This page is for the assembly caller: for every routine and macro in the `.s` files it gives the registers it reads, the registers it returns, and the registers it clobbers. The same information stays in the comment blocks of the source files, which Doxygen does not read.

Unless a row says otherwise, a routine clobbers nothing but the registers listed and the condition codes. Macros expand in place, so they also clobber whatever their listed registers are.

# Calling the Sub BIOS

`sub/sub.macro.s`

| Macro | In | Clobbers | Purpose |
|---|---|---|---|
| `BIOS_CALL fcode` | function arguments as the call expects | d0 | `move.w fcode, d0` then `jsr BIOS_CALL_VECTOR`. The call's own clobbers apply; `sub/bios.def.h` lists them per function. |
| `BRAM_CALL fcode` | as above | d0 | Backup RAM manager call through `BRAM_CALL_VECTOR`. |
| `BOOT_CALL fcode` | as above | d0 | CD Boot call through `BOOT_CALL_VECTOR`. |
| `GA_SUB_CLEAR_COMM_REGS` | | d0, a0 | Zero COMSTAT0 to COMSTAT7 and the Sub CPU's comm flags byte. |
| `GA_SUB_WAIT_2M` | | | Spin until the Main CPU has set DMNA, that is until the Sub CPU holds Word RAM in 2M mode. |
| `GA_SUB_GRANT_2M` | | | Set RET and spin until it reads back: hand 2M Word RAM to the Main CPU. |

# General macros

`macro.s`

| Macro | Purpose |
|---|---|
| `SUB name [, align]` | Define `name` as a global function label, aligned (default 2). Same as `GLABEL` with the symbol typed as a function. |
| `GLABEL name [, align]` | Define `name` as a global label, aligned (default 2). |
| `GLOBAL name, value [, align]` | Define `name` as a global, hidden absolute symbol equal to `value`. Used for `INIT_PC`, `INIT_SSP` and the vector table entries. |
| `FILE path, label [, align]` | `.incbin` the file as `label`, laid out as a `DataChunk`: `label_size` (a long, the byte count) then `label_data`. |
| `FILE_NOSIZE path, label [, align]` | `.incbin` the file at `label`, nothing else. |
| `HEX2BCD` | d0.w in: a value 0 to 99; d0.w out: the value as packed BCD. Clobbers d1. |
| `COMPARE_STRING` | a1, a2: strings; d1: length minus 1. Sets Z if the first `d1 + 1` bytes match. Registers are preserved. |

`cpu.macro.s`

| Macro | Purpose |
|---|---|
| `DISABLE_INTERRUPTS` | `ori #0x700, sr` |
| `ENABLE_INTERRUPTS` | `andi #0xF8FF, sr` |
| `CPU_STOP` | `stop #0x700`: halt with interrupts disabled |
| `PUSH reg` / `POP reg` | Push or pop one register |
| `PUSHM regs` / `POPM regs` | `movem` a register list to or from the stack |

`init.macro.s`

| Macro | Clobbers | Purpose |
|---|---|---|
| `BASIC_INIT` | d0, d7, a0, a1 | Module startup: zero `.bss` and copy initialized data from ROM to RAM, using the layout symbols every linker script defines (`docs/dev_in_c.md`). `std_init.s` and `ipx_init.s` wrap it. |

# Main CPU

## Gate array, `main/gate_arr.macro.s`

| Macro | Clobbers | Purpose |
|---|---|---|
| `GA_MAIN_WAIT_2M` | | Spin until RET is set: the Main CPU holds Word RAM in 2M mode. |
| `GA_MAIN_GRANT_2M` | | Set DMNA and spin until it reads back: hand 2M Word RAM to the Sub CPU. |
| `GA_MAIN_CLEAR_COMM_REGS` | d0, a0 | Zero COMCMD0 to COMCMD7 and the Main CPU's comm flags byte. |
| `GA_MAIN_RESET` | | The exact access sequence that forces a gate array reset. The width and address of each write is load bearing; do not reorder it. |

## Mode 1, `main/mode1.s`

| Routine | In | Out | Clobbers | Purpose |
|---|---|---|---|---|
| `mode1_detect` | | d0.l: 1 if a Boot ROM is at `BOOT_ROM`, else 0 | | Check for a Mega CD. |
| `mode1_init_vectors` | | | d0, d1, a0 | Build the system vector table at 0xFFFD00; `md_init.s` calls it for `MEGACD_MODE1`. |
| `mode1_find_font` | | a1.l: the Boot ROM's 96 tile 1bpp font, or 0 | d0, d1 | Locate the font to load with `BIOS_LOAD_1BPP_TILES`. |
| `mode1_load_sub` | a0.l: SP binary; d0.l: its size | d0.l: `MODE1_OK` or a `MODE1_ERR_*` value | d1, a0, a1 | Unpack the Sub BIOS, load the SP and start the Sub CPU. See `docs/mode1.md`. |

## VDP, `main/vdp.s` and `main/vdp.macro.s`

| Routine or macro | In | Clobbers | Purpose |
|---|---|---|---|
| `VDP_DMA_TRANSFER` | d0.l: destination as a VDP command; d1.l: source; d2.w: length in words | d0 to d3, a6 | DMA to VRAM. Enabling DMA in VDP mode register 2 is the caller's job. |
| `VDP_DMA_FILL` | d0.l: destination as a VDP command; d1.w: count in bytes; d2.b: value | d0, d3, a6 | DMA fill. Same note on mode register 2. |
| `TO_VDPPTR [dreg]` | dreg (default d0): a 16 bit VRAM address | | Convert in place to a VDP format address. |
| `VDPPTR_TO [dreg]` | dreg (default d0): a VDP format address | | Convert in place to a 16 bit VRAM address. |

## Input, `main/io.macro.s`

| Macro | In | Out | Clobbers | Purpose |
|---|---|---|---|---|
| `INIT_JOYPADS` | | | a0 | Set TH to output on control ports 1 and 2 for 3 button pads. |
| `READ_INPUT_JOYPAD` | a0: the port's data register | d0.b: buttons as SACBRLDU, 1 = pressed | d1 | Read a 3 button pad. |

## Serial port, `main/comm.macro.s`

| Macro | In | Out | Clobbers | Purpose |
|---|---|---|---|---|
| `INIT_EXT_PORT` | | | | Configure the port selected by `EXT_PORT` for serial communication at `EXT_BAUD`. |
| `EXT_RX` | | d0.b: the byte | d0, d1 | Wait for a byte and read it. |
| `EXT_TX` | d0.b: the byte | | d1 | Send a byte, waiting for the transmitter to drain first. |

## Z80, `main/z80.macro.s`

| Macro | In | Clobbers | Purpose |
|---|---|---|---|
| `Z80_REQUEST_BUS` | | | Request the Z80 bus and spin until granted. |
| `Z80_RELEASE_BUS` | | | Release the bus. |
| `Z80_RESET_CPU` | | | Pulse the Z80 reset line. |
| `Z80_INIT` | a0: program; d7: length minus 1 | a1, d7 | Take the bus, copy a program into Z80 RAM, reset the Z80 and release the bus. |

## Modules, `main/mmd.macro.s`

| Macro | Out | Clobbers | Purpose |
|---|---|---|---|
| `INIT_MMD` | a0: the module's entry point | d0, a0 to a2 | Prepare an MMD the Sub CPU has loaded to the start of Word RAM: copy it to its destination if asked, install its interrupt handlers, hand Word RAM back if its header says so. `docs/modules.md`. |

## Crash handler, `main/crash_main.s` and `main/crash_cart.s`

| Routine | Purpose |
|---|---|
| `crash_install` | Install the Main CPU handlers in the system vector table (Mega CD and Mode 1). Cartridges include `crash_cart.s` instead, which points the ROM vectors at the handlers at link time. |
| `exvec_address_or_illegal` | The entry the Boot ROM shares between address errors and illegal instructions; it tells them apart by the frame. Not called directly. |

`docs/crash_handler.md` has the rest.

## Startup files

`md_init.s`, `std_init.s`, `std_init_stack.s`, `main/ipx_init.s`, `main/ipx_init_stack.s`, `md_vectors.s`, `md_header.s` and `cd_boot.s` are included by a project rather than called. `docs/boot.md`, `docs/modules.md` and `docs/dev_in_c.md` describe what each sets up.

# Sub CPU

## CD-ROM access loop, `sub/cdrom.s` and `sub/cdrom.macro.s`

`docs/cdrom.md` explains the loop. The entry points an SP uses:

| Routine or macro | In | Out | Clobbers | Purpose |
|---|---|---|---|---|
| `INIT_ACC_LOOP` | | | | Prime the loop; call once in `sp_init`. |
| `PROCESS_ACC_LOOP` | | | a0 | Pump the loop; call last in `sp_int2`, since it returns to the BIOS itself. |
| `WAIT_FOR_ACC_OP` | | d0.w: result code | d1 | Wait until the current operation is idle, calling `BIOS_WAIT_VSYNC` each time. |
| `load_file_sub` | a0.l: file name; a1.l: destination address | d0.w: result code | d1 | Load a file by Sub CPU copy and wait for it. |
| `load_file_prg_dma` | a0.l: file name; a1.l: destination address in Program RAM, a multiple of 8 | d0.w: result code | d1 | Load a file into Program RAM by DMA and wait for it. |
| `find_file` | a0.l: file name | CC and a0.l: the directory entry; CS: not found | d0, d1, a1, a2 | Look a file up in the cached root directory. |
| `check_status` | | CS: busy; CC: idle, with d0.w the result and d1 the size in bytes (or sectors read on a load failure) | | Poll the loop. |

The variables `access_op`, `access_op_result`, `filename`, `filebuff`, `filesize`, `part_offset` and `part_count` are global; `sub/cdrom.h` documents them. For the DMA operations `filebuff` holds the destination offset within the target memory (for wave RAM, in the Sub CPU's 8 KB view of the bank); the loop writes the DMA address register itself. `part_count` sectors from `part_offset` load part of a file; 0 loads it all.

## PCM, `sub/pcm.s`

| Routine | In | Clobbers | Purpose |
|---|---|---|---|
| `pcm_clear_ram` | | d0 to d2, a6 | Silence every channel and zero all 16 wave RAM banks. |
| `pcm_config_channel` | d0.b: channel selector (`CHANNEL(n)`); a5.l: seven register values (`PcmChannelSettings`) | d0, a5, a6 | Write a channel's envelope, pan, frequency, loop point and start registers. |

## Crash handler, `sub/crash_sub.s`

| Routine | Purpose |
|---|---|
| `crash_install` | Install the Sub CPU handlers in the Sub BIOS jump table. The Main CPU reports a Sub CPU crash through `crash_check_sub`. |

# Compiler runtime

`runtime/memcpy.s`, `memmove.s`, `memset.s`, `memcmp.s` and `divmul.s` are what GCC calls on its own for block copies and 32 bit multiply, divide and modulo. They follow the C calling convention: arguments on the stack, result in d0. `memcpy`, `memmove` and `memset` also return their destination in a0, because the two m68k targets disagree on where a pointer comes back. They are not an assembly API; call them from C.

# Mode 1

In Mode 1 the Mega Drive boots from a cartridge with a Mega CD attached, and the cartridge program drives the Mega CD hardware: the Sub CPU, Program RAM, Word RAM, the PCM chip, the graphics unit and, with a disc inserted, the CD drive. Set `TARGET:=MEGACD_MODE1` in the project makefile. The `mode1` example is a working starting point.

# Memory map

With a cartridge as the boot device, the Mega CD moves up by 0x400000 on the Main CPU bus:

| Region | Mode 1 | Mega CD boot |
|---|---|---|
| Cartridge ROM | `0x000000` | `0x400000` |
| Boot ROM | `0x400000` | `0x000000` |
| Program RAM window (128 KB bank) | `0x420000` | `0x020000` |
| Word RAM (2M) | `0x600000` | `0x200000` |
| Gate array | `0xA12000` | `0xA12000` |

`main/memmap.def.h` and `main/bios.def.h` select the Mode 1 addresses when `TARGET` is `MEGACD_MODE1`. `BOOT_ROM` gives the Boot ROM base for either target.

# Starting the Mega CD

Nothing on the Mega CD runs until the cartridge starts it. The Sub CPU BIOS is stored compressed inside the Boot ROM, and on a disc boot the Boot ROM unpacks it into Program RAM before releasing the Sub CPU. In Mode 1 the cartridge does this with `mode1_load_sub` (`main/mode1.h`, `main/mode1.s`):

1. Hold the Sub CPU in reset and take its bus.
2. Select Program RAM bank 0 in 2M mode, with write protection off.
3. Find the Sub BIOS image in the Boot ROM and unpack it into Program RAM. Its location differs between Boot ROM versions, so it is found by searching the Boot ROM's loader code rather than by address.
4. Copy the Sub CPU program (SP) to Program RAM `0x6000`, where the Sub BIOS expects it.
5. Write protect the Sub BIOS, release reset, release the bus, and clear the comm registers.

The SP is the same kind of program a disc carries: a header and the `sp_init`, `sp_main`, `sp_int2` and `sp_user` entry points (`sub/sp_header.s`). The example builds it with the usual `sp.bin` rule and embeds it in the cartridge with the `FILE` macro. It may be up to `MODE1_SP_MAX_SIZE` bytes.

`mode1_load_sub` returns `MODE1_OK` or an error from `main/mode1.def.h`. Check for the hardware first with `mode1_detect`; with no Mega CD attached, the cartridge runs as a plain Mega Drive program.

The program must assemble `mode1.s` and `kos_cmp.s`, and define `INIT_PC` as its entry point, as the cartridge vector table already requires.

# System vector table

On a disc boot the Boot ROM builds the system vector table at `0xFFFD00`, the `jmp` entries through which exceptions and interrupts reach their handlers. In Mode 1 `md_init.s` builds it with `mode1_init_vectors` before enabling interrupts: the reset entry points at `INIT_PC`, the exception and interrupt entries at an `rte`, and the two subroutine entries (`BIOS_VBLANK_USER` and the Backup RAM manager entry) at an `rts`.

Point the cartridge's 68000 vector table at these entries, as the example's `init.s` does, and install handlers by writing their addresses to the `EXVEC_*` locations, the same as in a disc program.

# Boot ROM library

Most Boot ROM library calls work from a cartridge, at their Mode 1 addresses in `main/bios.def.h`. These do not, because they refer to the Boot ROM by absolute address, which in Mode 1 lands in the cartridge:

| Call | Alternative |
|---|---|
| `BIOS_ENTRY`, `BIOS_RESET`, `BIOS_INIT`, `BIOS_INIT_SP` | None needed: `md_init.s` and `mode1_init_vectors` do the setup. These never return. |
| `BIOS_LOAD_FONT`, `BIOS_LOAD_FONT_DEFAULTS` | `mode1_find_font` returns the font's address; load it with `BIOS_LOAD_1BPP_TILES`. |
| `BIOS_SEGA_LOGO` | None. |
| `BIOS_DMA_QUEUE` | Call `BIOS_DMA_XFER_WORD_RAM` directly. |
| Slot `0x398` (`BIOS_UNKNOWN_44`) | None. |

This is the same for every Boot ROM version checked: JP 1.00c, JP 2.00c, WonderMega 2, US 1.10, US Model 2 and CDX.

The Boot ROM library expects the RAM state a disc boot leaves behind. Before using it, load its defaults: `BIOS_LOAD_DEFAULT_VDP_REGS`, `BIOS_CLEAR_VRAM`, `BIOS_CLEAR_TABLES` and `BIOS_CLEAR_VSRAM`. The default VDP registers leave the display off; enable it with `BIOS_VDP_DISP_ENABLE`.

# Testing in an emulator

An emulator attaches the Mega CD to a cartridge when the cartridge header's device support field contains `C`. `md_header.s` writes `JC` there when `TARGET` is `MEGACD_MODE1`. The emulator needs a Mega CD Boot ROM; BlastEm reads it from `cdbios.bin` next to its executable. MAME's Mega CD systems have no cartridge slot and cannot run Mode 1.

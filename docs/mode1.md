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
2. Turn write protection off and clear all four Program RAM banks, as the Boot ROM does, then select bank 0 in 2M mode.
3. Find the Sub BIOS image in the Boot ROM and unpack it into Program RAM. Its location differs between Boot ROM versions, so it is found by searching the Boot ROM's loader code rather than by address.
4. Copy the Sub CPU program (SP) to Program RAM `0x6000`, where the Sub BIOS expects it.
5. Write protect the Sub BIOS, release reset, release the bus, and clear the comm registers.
6. Wait for the Sub BIOS to enable INT2, which the Main CPU sees as the IEN2 bit of `GA_MAIN_REG_INT2`. Every Sub BIOS does this early in its startup, once its self-check has passed.

The clear in step 2 is not optional. At startup the Sub BIOS checksums Program RAM from `0x200` up to a fixed end, and in the Japanese 1.00 Boot ROMs and the LaserActive 1.0x Boot ROMs the unpacked image ends short of that range. On a real console Program RAM holds a power-on pattern, not zeros, so without the clear the checksum fails and the Sub BIOS restarts forever, with interrupts off and the comm registers cleared on every pass. Emulators that start with zeroed RAM do not show this.

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

# Disc access

The Sub CPU reads the disc, with the Sub BIOS calls and the kit's access loop (`docs/cdrom.md`) exactly as on a disc boot. Three things differ from a disc program:

- **The SP initializes the drive.** On a disc boot the Boot ROM has read the TOC before the SP runs. In Mode 1 nothing has: the SP calls `BIOS_DRIVE_INIT` and waits for the status to settle before the directory can be read. With no disc in the drive that call waits for one, so do it on request from the Main CPU rather than in `sp_init`, and give the Main CPU a timeout. A cartridge must run without a disc.
- **INT2 comes from the cartridge.** The access loop advances on INT2, which the Main CPU raises through the gate array's IFL2 bit once per frame. `BIOS_VBLANK_HANDLER` does this in its comm sync; a program with its own VBLANK handler calls `BIOS_TRIGGER_IFL2` or `BIOS_SET_IFL2` itself. Keep bit 0 of the Sub CPU's comm flags clear or that sync loops.
- **Word RAM is at 0x600000.** The addresses in `main/memmap.def.h` follow `TARGET`. Ownership works as usual: the Main CPU gives Word RAM to the Sub CPU with `ga_main_grant_2m` before a load into it, the Sub CPU hands it back with `ga_sub_grant_2m` after, and the Main CPU waits for `RET` before reading.

The disc needs no boot sector: `ISO_BOOT` is empty for `MEGACD_MODE1`, so `make` masters a plain ISO 9660 image of `DISC_PATH`. Set `ISO_BOOT:=$(BUILD_PATH)/boot.bin`, and provide `ip.s` and `sp.s` for the disc, to master one that also boots on its own.

The `mode1` example does all of this: on command from the Main CPU its SP initializes the drive, caches the root directory and loads `DATA.BIN` into Word RAM twice, by Sub CPU copy and by CDC DMA, and the Main CPU checks the content against a sum computed at build time.

# Testing in an emulator

An emulator attaches the Mega CD to a cartridge when the cartridge header's device support field contains `C`. `md_header.s` writes `JC` there when `TARGET` is `MEGACD_MODE1`. The emulator needs a Mega CD Boot ROM; BlastEm reads it from `cdbios.bin` next to its executable. A disc goes in as BlastEm's lock-on media: `blastem -o project.iso project.cart`. MAME's Mega CD systems have no cartridge slot and cannot run Mode 1.

The `mode1` example runs in BlastEm, with its disc attached, with every Mega CD, Wondermega, CDX and LaserActive Boot ROM version tested (28, Japanese, US and European), except the Mega-LD 1.05 Boot ROM: with it, the Sub BIOS does not start and `mode1_load_sub` returns `MODE1_ERR_SUB_TIMEOUT`.

On hardware it runs on a Japanese Mega CD (Boot ROM 1.00, December 1991) with a Mega Drive and a Mega EverDrive X7: the Sub CPU starts, answers the Main CPU and counts INT2, and with no disc in the drive the disc step times out as designed. The disc loads have not been tested on hardware.

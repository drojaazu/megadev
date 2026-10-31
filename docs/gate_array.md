# Gate Array

The gate array is the set of registers through which software drives the Mega CD hardware: CD data transfer, the graphics unit, interrupts, memory arbitration, and communication between the Main and Sub CPUs. Sega's documentation also calls it the "Mega CD Engine."

The registers themselves are documented in the source, in `gate_arr.def.h` and `gate_arr.h` under both `lib/main` and `lib/sub`, from which Doxygen generates the reference. This document covers the conventions the definitions follow and the hardware constraints on accessing the registers.

# Two views of the same hardware

The Main and Sub CPUs each see the gate array at a different address, and not the same set of registers:

| | Main CPU | Sub CPU |
|---|---|---|
| Base address | `0xA12000` | `0xFF8000` |
| Definitions | `lib/main/gate_arr.def.h`, `gate_arr.h` | `lib/sub/gate_arr.def.h`, `gate_arr.h` |
| Prefix | `GA_MAIN_`, `ga_main_` | `GA_SUB_`, `ga_sub_` |

Use the header for the CPU the code runs on. The prefixes keep the two views distinct, so a program that drives the Mega CD from a cartridge (Mode 1) can include both.

# Definitions

## Registers

`GA_SUB_REG_<NAME>` is a register address. `ga_sub_reg_<name>` is the C accessor, used as a variable: `ga_sub_reg_comstat0 = 1;`. Each register's documentation group is titled with its hardware address, so a search for an address from Sega's manual finds the register even where Megadev defines it one byte along (see below).

Read-only registers have read-only accessors; assigning to one is a compile error. The accessor types are `ga_sub_reg` and `ga_sub_reg_ro` for 16-bit registers, `ga_sub_reg8` and `ga_sub_reg8_ro` for byte registers, and likewise for `ga_main_`.

## Fields

Register fields follow the project-wide scheme described in the manual under Register Field Naming: `_POS` for the bit position, `_MASK` for the bits occupied, and `_WIDTH` for fields wider than one bit. A field with no `_WIDTH` is a single-bit flag.

```
    btst    #GA_SUB_DMNA_POS, GA_SUB_REG_MEMMODE     ; assembly
    if (ga_sub_reg_memmode & GA_SUB_DMNA_MASK)       /* C */
```

Values a multi-bit field can hold are defined unshifted and placed with `FIELD_PREP`:

```
    ga_sub_reg_cdcmode = FIELD_PREP(GA_SUB_CDCMODE_DD, GA_SUB_CDCMODE_DD_WRAMDMA);
```

## Byte registers

Many gate array registers are 16 bits wide with unrelated settings in each byte. Megadev defines each such byte as a register of its own, with its own name and address, so a byte write to one cannot disturb the other and a field's `_POS` goes straight to a bit instruction:

| Address | High byte | Low byte |
|---|---|---|
| `0xFF8000` | `GA_SUB_REG_LED` (LEDR, LEDG, VER) | `GA_SUB_REG_SUBCTRL` (RES0) |
| `0xFF8002` | `GA_SUB_REG_WP` (write protect, read only) | `GA_SUB_REG_MEMMODE` (RET, DMNA, MODE, PM) |
| `0xFF800E` | `GA_SUB_REG_COMFLAGS_MAIN` (read only) | `GA_SUB_REG_COMFLAGS_SUB` |
| `0xFF8058` | `GA_SUB_REG_GFXSTAT` (GRON, read only) | `GA_SUB_REG_STAMPSIZE` (RPT, STS, SMS) |
| `0xA12000` | `GA_MAIN_REG_INT2` (IFL2, IEN2) | `GA_MAIN_REG_SUBCPU` (SRES, SBRQ) |
| `0xA12002` | `GA_MAIN_REG_WP` (write protect) | `GA_MAIN_REG_MEMMODE` (RET, DMNA, MODE, BK) |
| `0xA1200E` | `GA_MAIN_REG_COMFLAGS_MAIN` | `GA_MAIN_REG_COMFLAGS_SUB` (read only) |

```
    bset    #GA_SUB_LED_R_POS, GA_SUB_REG_LED        ; assembly
    ga_sub_reg_led |= GA_SUB_LED_R_MASK;             /* C */
```

The halves are adjacent with the high byte at the even address, so a word access to the pair is still possible by writing the address literally. It is not the default spelling.

Registers that use only one of their two bytes are defined at the byte that is used: `GA_SUB_REG_INT3TIMER` is `0xFF8031`, `GA_SUB_REG_INTMASK` is `0xFF8033`, `GA_SUB_REG_FONTCOLOR` is `0xFF804D`, `GA_SUB_REG_IMGBUFVSIZE` is `0xFF805D`, `GA_SUB_REG_IMGBUFOFFSET` is `0xFF8061`, and `GA_MAIN_REG_CDCMODE` is `0xA12004`. A byte write to the register name therefore reaches the hardware. The unused bytes have no definitions.

## Registers that stay 16-bit

Two kinds of register are not split:

- Word-only data registers: `GA_SUB_REG_CDCHOSTDATA`, `GA_SUB_REG_DMAADDR`, `GA_SUB_REG_STOPWATCH`, `GA_SUB_REG_CDFADER` and the address and size registers. They hold one value and permit no bit operations.
- Registers whose fields span both bytes but are accessed as one unit: `GA_SUB_REG_CDCMODE` at `0xFF8004`, with the CDC register address in the low byte and the status flags in the high byte.

For these, `_POS` counts from bit 0 of the word. A position of 8 or more cannot be given to a bit instruction on the register address, because the 68000's bit instructions on memory operate on one byte and take the bit number modulo 8. Two helpers in `build.def.h` select the byte and renumber the bit:

```
    btst    #FIELD_BPOS(GA_SUB_CDCMODE_EDT), FIELD_BYTE(GA_SUB_REG_CDCMODE, GA_SUB_CDCMODE_EDT)
```

# Access width and bit operations

All gate array registers can be accessed as words. Most can also be accessed as bytes, and most accept the bit instructions `btst`, `bset`, `bclr` and `bchg`. The exceptions are the table below; a byte access to a word-only register, or a bit instruction on a register that forbids them, can raise a bus error rather than produce a wrong value. Where bit instructions are forbidden, read the whole register, change the copy, and write it back.

Transcribed from the Mega-CD Hardware Manual, page 21, which states that unless otherwise mentioned, byte and word access and bit operations are allowed. `W/B` means word or byte access; `W` means word access only.

## Sub CPU side, base `0xFF8000`

| Offset | Register | Access | Bit instructions |
|---|---|---|---|
| `00` | LED, version, peripheral reset | W/B | yes |
| `02` | Write protect, memory mode | W/B | yes |
| `04` | CDC mode and register address | W/B | `btst` only |
| `06` | CDC register data | W/B | none, including `clr` |
| `08` | CDC host data, read only | W | none |
| `0A` | CDC DMA address, write only | W | none |
| `0C` | Stopwatch | W | none |
| `0E` | Communication flags | W/B | yes |
| `10` to `2E` | Communication command and status | W/B | yes |
| `30` | INT3 timer | W/B | yes |
| `32` | Interrupt mask | W/B | yes |
| `34` | CD fader (do not access directly; use the BIOS) | W | none |
| `36` | CDD control (do not access directly; use the BIOS) | W/B | no `bset` or `bclr` |
| `38` to `4A` | CDD status and command | W/B | none |
| `4C` | Font color | W/B | yes |
| `4E` | Font bits | W/B | yes |
| `50` to `56` | Font data | W/B | yes |
| `58` | Graphics status, stamp size | W/B | yes |
| `5A` | Stamp map base address | W | none |
| `5C` | Image buffer V cell size | W/B | none |
| `5E` | Image buffer start address | W | none |
| `60` | Image buffer offset | W/B | yes |
| `62` | Image buffer H dot size | W | none |
| `64` | Image buffer V dot size | W | none |
| `66` | Trace vector base address | W | none |
| `68` | Subcode address | W/B | yes |
| `100` to `1FE` | Subcode data | W/B | yes |

## Main CPU side, base `0xA12000`

| Offset | Register | Access | Bit instructions |
|---|---|---|---|
| `00` | INT2 control, Sub CPU control | W/B | `btst` only |
| `02` | Write protect, memory mode | W/B | yes |
| `04` | CDC mode, read only, high byte only | W/B | yes |
| `06` | H interrupt vector | W | yes |
| `08` | CDC host data, read only | W | none |
| `0C` | Stopwatch, read only; only the Sub CPU clears it | W | none |
| `0E` | Communication flags | W/B | yes |
| `10` to `1E` | Communication command, written by the Main CPU | W/B | yes |
| `20` to `2E` | Communication status, read only | W/B | yes |

Each register's Doxygen entry carries a `@warning` where an access restriction applies, and a per-bit table marking which bits are valid to read and to write.

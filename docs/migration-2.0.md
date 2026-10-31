# Migrating to Megadev 2.0

This guide lists every change in Megadev 2.0 that requires a change to a project written against 1.x. Each entry gives what was renamed or removed, what replaces it, and any change in behavior. Entries are added as the changes land, so this file is complete for whatever version of `develop` it ships with.

# Contents

- File names
- Registers
- Gate array names
- Gate array register model

---

# File names

## Assembly macro files use the `.macro.s` suffix

Every macro-only assembly file now ends in `.macro.s`; the plural `.macros.s` is gone, and the generic `macros.s` is now `macro.s`. Update the include lines in your sources:

| Was | Now |
|---|---|
| `macros.s` | `macro.s` |
| `init.macros.s` | `init.macro.s` |
| `system.macros.s` | `system.macro.s` |
| `main/comm.macros.s` | `main/comm.macro.s` |
| `main/gate_arr.macros.s` | `main/gate_arr.macro.s` |
| `main/io.macros.s` | `main/io.macro.s` |
| `main/mmd.macros.s` | `main/mmd.macro.s` |
| `main/vdp.macros.s` | `main/vdp.macro.s` |
| `main/z80.macros.s` | `main/z80.macro.s` |

The contents of the files are unchanged.

# Registers

## a6 is reserved

Compiled C never uses a6 (`-ffixed-a6` is now in the default compiler flags), and the Boot ROM wrappers in `main/bios.h` and the PCM wrappers in `sub/pcm.h` no longer save and restore a6 around the call. Code size shrinks and nothing changes for C callers. Hand-written assembly that kept a value in a6 across a call to one of those wrappers must now save it itself; inline assembly that used a6 as a working register should use another address register. See `docs/dev_in_c.md`, Register a6.

# Gate array names

## Every gate array name carries its CPU side

The Main CPU and Sub CPU see the gate array at different addresses, and 1.x used the same names for both views, so a program that included both headers got whichever definitions came first. Every name in `main/gate_arr.*` and `sub/gate_arr.*` now carries the side immediately after the `GA`/`ga` prefix. The rule:

| Kind | 1.x | 2.0 Main CPU | 2.0 Sub CPU |
|---|---|---|---|
| Constants | `GA_REG_COMCMD0`, `GA_MASK_DMNA`, `GA_BIT_RETURN_2M` | `GA_MAIN_REG_COMCMD0`, `GA_MAIN_MASK_DMNA`, ... | `GA_SUB_REG_COMCMD0`, `GA_SUB_MASK_DMNA`, ... |
| Register pointer type | `ga_reg` | `ga_main_reg` | `ga_sub_reg` |
| Register accessors | `ga_reg_comcmd0`, `ga_reg_memmode` | `ga_main_reg_comcmd0`, `ga_main_reg_memmode` | `ga_sub_reg_comcmd0`, `ga_sub_reg_memmode` |

Apply it mechanically: insert `MAIN_` or `SUB_` after `GA_`, and `main_` or `sub_` after `ga_`, according to which CPU the source file is built for. Values and addresses are unchanged.

The inline functions and assembly macros had no prefix at all and gain the full one:

| 1.x (both sides) | 2.0 Main CPU | 2.0 Sub CPU |
|---|---|---|
| `wait_2m()` | `ga_main_wait_2m()` | `ga_sub_wait_2m()` |
| `grant_2m()` | `ga_main_grant_2m()` | `ga_sub_grant_2m()` |
| `clear_comm_regs()` | `ga_main_clear_comm_regs()` | `ga_sub_clear_comm_regs()` |
| `set_1m()`, `set_2m()` | `ga_main_set_1m()`, `ga_main_set_2m()` | (Main only) |
| `reset_ga()` | (Sub only) | `ga_sub_reset()` |
| `WAIT_2M`, `GRANT_2M`, `CLEAR_COMM_REGS` | `GA_MAIN_WAIT_2M`, `GA_MAIN_GRANT_2M`, `GA_MAIN_CLEAR_COMM_REGS` | `GA_SUB_WAIT_2M`, `GA_SUB_GRANT_2M`, `GA_SUB_CLEAR_COMM_REGS` |
| `RESET_GA` | `GA_MAIN_RESET` | (Main only) |

Names outside the `GA_`/`ga_` families in these headers (`BIT_GA_REG_*`, `MSK_GA_REG_*`, `CDCMODE_*BITS`, `MEMMODE_*_MSK`) are replaced by the register model described below.

## Register accessors are values, not pointers

Every `ga_main_reg_*` and `ga_sub_reg_*` accessor now names the register itself, so a register is read and written like a variable: `ga_main_reg_comcmd0 = 2;` and `x = ga_sub_reg_comstat1;`. In 1.x most accessors were pointers and needed a `*`. Remove the `*` at each use. Where a pointer to the register is wanted, take its address: `&ga_main_reg_comcmd0`. The generated code is identical.

# Gate array register model

Every gate array register field now follows `<SUBSYSTEM>_<FIELD>_<ASPECT>` with `_POS`, `_MASK` and, for fields wider than one bit, `_WIDTH`; see the manual, Register Field Naming. The 1.x spellings `GA_BIT_*`, `GA_MASK_*`, `BIT_GA_REG_*` and `MSK_GA_REG_*` are gone. Side prefixes are shown for the Sub CPU; the Main CPU is the same with `MAIN`.

## Flags and fields

| 1.x | 2.0 |
|---|---|
| `GA_BIT_DMNA`, `GA_MASK_DMNA`, `BIT_GA_REG_DMNA` | `GA_SUB_DMNA_POS`, `GA_SUB_DMNA_MASK` |
| `GA_BIT_RETURN_2M`, `GA_MASK_RETURN_2M`, `BIT_GA_REG_RET` | `GA_SUB_RETURN_2M_POS`, `GA_SUB_RETURN_2M_MASK` |
| `GA_MASK_WORDRAM_LAYOUT`, `BIT_GA_REG_MODE` | `GA_SUB_WORDRAM_LAYOUT_POS`, `GA_SUB_WORDRAM_LAYOUT_MASK` |
| `GA_LED_R`, `GA_LED_G` | `GA_SUB_LED_R_MASK`, `GA_SUB_LED_G_MASK` (positions 0 and 1 of `GA_SUB_REG_LED`) |
| `GA_MASK_VERSION` | `GA_SUB_VERSION_MASK` |
| `BIT_GA_REG_INTn`, `MSK_GA_REG_INTn` | `GA_SUB_INTn_POS`, `GA_SUB_INTn_MASK` |
| `GA_BIT_STAMPSIZE_*`, `GA_MASK_STAMPSIZE_*` | `GA_SUB_STAMPSIZE_*_POS`, `GA_SUB_STAMPSIZE_*_MASK` |
| `GA_BIT_CDCMODE_DSR` (6), `GA_BIT_CDCMODE_EDT` (7), `CDCMODE_CABITS`, `CDCMODE_DDBITS` | `GA_SUB_CDCMODE_DSR_POS` (14), `GA_SUB_CDCMODE_EDT_POS` (15), `GA_SUB_CDCMODE_CA_MASK`, `GA_SUB_CDCMODE_DD_MASK`. Positions are relative to the 16-bit register; use `FIELD_BYTE`/`FIELD_BPOS` for bit instructions. |
| `GA_MASK_CDCMODE_MAINREAD` and friends | `GA_SUB_CDCMODE_DD_MAINREAD` and friends: values for the DD field, placed with `FIELD_PREP(GA_SUB_CDCMODE_DD, value)` |
| Main: `GA_MASK_SUB_RESET`, `GA_MASK_SUB_REQ`, `GA_BIT_RAISE_INT2`, `GA_MASK_INT2_MASK` | `GA_MAIN_SUB_RESET_MASK`, `GA_MAIN_SUB_REQ_MASK`, `GA_MAIN_RAISE_INT2_POS`, `GA_MAIN_INT2_MASKING_MASK` |
| Main: `GA_MASK_CDC_DEST_*`, `MEMMODE_BK0_MSK`, `MEMMODE_WP0_MSK` | `GA_MAIN_CDC_DEST_*` values, `GA_MAIN_MEMMODE_BANK_MASK`, `GA_MAIN_WP_MASK` |

## Registers that split into bytes

Where a 16-bit register's two bytes hold unrelated settings, 2.0 names each byte. Positions in these registers are relative to the byte, so a `btst` takes `_POS` and the register address directly, with no `+1`.

| 1.x register | 2.0 high byte | 2.0 low byte |
|---|---|---|
| Sub `GA_REG_RESET` 0xFF8000 | `GA_SUB_REG_LED` 0xFF8000 | `GA_SUB_REG_SUBCTRL` 0xFF8001 |
| Sub `GA_REG_MEMMODE` 0xFF8002 | `GA_SUB_REG_WP` 0xFF8002 (read only) | `GA_SUB_REG_MEMMODE` 0xFF8003 |
| Sub `GA_REG_COMFLAGS` 0xFF800E | `GA_SUB_REG_COMFLAGS_MAIN` 0xFF800E (read only) | `GA_SUB_REG_COMFLAGS_SUB` 0xFF800F |
| Sub `GA_REG_STAMPSIZE` 0xFF8058 | `GA_SUB_REG_GFXSTAT` 0xFF8058 (read only, GRON) | `GA_SUB_REG_STAMPSIZE` 0xFF8059 |
| Main `GA_REG_RESET` 0xA12000 | `GA_MAIN_REG_INT2` 0xA12000 | `GA_MAIN_REG_SUBCPU` 0xA12001 |
| Main `GA_REG_MEMMODE` 0xA12002 | `GA_MAIN_REG_WP` 0xA12002 | `GA_MAIN_REG_MEMMODE` 0xA12003 |
| Main `GA_REG_COMFLAGS` 0xA1200E | `GA_MAIN_REG_COMFLAGS_MAIN` 0xA1200E | `GA_MAIN_REG_COMFLAGS_SUB` 0xA1200F (read only) |

`GA_SUB_REG_MEMMODE` and `GA_MAIN_REG_MEMMODE` keep their names but are now one byte at the odd address. Code that wrote them as a word must write the byte: `andi.b #~(GA_SUB_RETURN_2M_MASK | GA_SUB_WORDRAM_LAYOUT_MASK), GA_SUB_REG_MEMMODE`.

Registers with one unused byte are defined at the byte that is used: `GA_SUB_REG_INT3TIMER` 0xFF8031, `GA_SUB_REG_INTMASK` 0xFF8033, `GA_SUB_REG_FONTCOLOR` 0xFF804D, `GA_SUB_REG_IMGBUFVSIZE` 0xFF805D, `GA_SUB_REG_IMGBUFOFFSET` 0xFF8061, and `GA_MAIN_REG_CDCMODE` 0xA12004. Drop any `+1` from accesses to them.

## Other renames

| 1.x | 2.0 |
|---|---|
| `GA_REG_CDDCOMM0` to `GA_REG_CDDCOMM4` | `GA_SUB_REG_CDDSTAT0` to `GA_SUB_REG_CDDSTAT4` (from the drive) |
| `GA_REG_CDDCOMM5` to `GA_REG_CDDCOMM9` | `GA_SUB_REG_CDDCMD0` to `GA_SUB_REG_CDDCMD4` (to the drive) |
| `ga_reg_comflags` (word) | removed; use the two byte accessors |
| `ga_reg_cdchostdata`, `ga_reg_stopwatch` (Main), comm status (Main), comm command (Sub) | read-only accessors; a write is a compile error |
| `GA_SUB_BIT_CDC_*` | removed; the destination values are `GA_SUB_CDCMODE_DD_*` |

Use the `GA_SUB_GRON_MASK` flag of `GA_SUB_REG_GFXSTAT` to wait for a graphics operation; the stamp size register no longer carries it.

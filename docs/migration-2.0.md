# Migrating to Megadev 2.0

This guide lists every change in Megadev 2.0 that requires a change to a project written against 1.x. Each entry gives what was renamed or removed, what replaces it, and any change in behavior. Entries are added as the changes land, so this file is complete for whatever version of `develop` it ships with.

# Contents

- File names
- Registers
- Gate array names

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

Names outside the `GA_`/`ga_` families in these headers (`BIT_GA_REG_*`, `MSK_GA_REG_*`, `CDC_DEST_*`, `CDCMODE_*BITS`, `MEMMODE_*_MSK`) are unchanged here; they are renamed by the register field naming change described below.

## Register accessors are values, not pointers

Every `ga_main_reg_*` and `ga_sub_reg_*` accessor now names the register itself, so a register is read and written like a variable: `ga_main_reg_comcmd0 = 2;` and `x = ga_sub_reg_comstat1;`. In 1.x most accessors were pointers and needed a `*`. Remove the `*` at each use. Where a pointer to the register is wanted, take its address: `&ga_main_reg_comcmd0`. The generated code is identical.

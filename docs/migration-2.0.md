# Migrating to Megadev 2.0

This guide lists every change in Megadev 2.0 that requires a change to a project written against 1.x. Each entry gives what was renamed or removed, what replaces it, and any change in behavior. Entries are added as the changes land, so this file is complete for whatever version of `develop` it ships with.

# Contents

- File names
- Registers

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

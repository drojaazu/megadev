# MEGADEV — Project Specification

> **Role of this document.** This is the normative reference for what Megadev must do, what rules its
> code must follow, what is known to be broken, and which decisions have been made and by whom.
> It is not a tutorial (see [docs/manual.md](docs/manual.md)) and not a task list (see
> [BACKLOG.md](BACKLOG.md)).
>
> **Status:** 2026-08-13. §8 has been re-tested against m68k gcc 14.2.0 in the devcontainer; two
> entries from the original by-inspection audit were false positives and are recorded as withdrawn.
> §7 records how confident we are in each *hardware* claim, which no toolchain can settle.

---

## 1. Purpose & Scope

Megadev is a development kit for the Sega Mega Drive and Sega Mega CD: headers, assembly routines,
linker scripts, a build layer, documentation and examples, so that a project need not start from
bare metal.

**Audience.** Developers who already know C or M68k assembly and have some familiarity with embedded
systems. Megadev is deliberately *less* friendly than SGDK. It favours flexibility for skilled
developers and a small external-tool footprint over hand-holding.

**Primary target.** The **Mega CD** is the reason this project exists and drives its complexity
(dual CPU, Gate Array arbitration, Word RAM banking, a disc boot chain, module loading). Mega Drive
cartridge support is real but secondary.

### Terminology: "BIOS" means two different things

The Mega CD's internal ROM provides a library of system calls on **both** CPUs, and Megadev calls
both of them "bios" — but they are not equivalent, and conflating them causes real errors:

| | Sub CPU side | Main CPU side |
|---|---|---|
| What it is | The actual Mega CD BIOS: CD drive control, CD audio, data reads, Backup RAM | A general-purpose utility library: controller input, DMA, palette/sprite caches, decompression, PRNG |
| Documentation | **Officially documented** in the *Mega-CD BIOS Manual* | **Entirely undocumented in English.** Understood only through reverse engineering |
| Megadev prefix | `bios_` / `BIOS_` (`lib/sub/bios.*`) | `bios_` / `BIOS_` (`lib/main/bios.*`) |
| Required? | Yes, a basic part of Mega CD development | **Entirely optional** |

**"Main BIOS" is a deliberate misnomer.** A more accurate name is "Boot ROM System Library", and
Megadev used the `bootlib` prefix for a time. `bios` was chosen for brevity in syscall prefixes and
for symmetry with the Sub side; the whole internal ROM is also colloquially "the BIOS". The
reasoning is recorded in full in [docs/main_bios.md](docs/main_bios.md) §Naming — read it before
renaming anything. (Sega's own use of "BIOS" for the Sub CPU *system calls* is not strictly correct
either.)

Practical consequence: **the Sega BIOS Manual documents the Sub side only.** It is not a source for
anything in `lib/main/bios.*`. Evidence for the Main side comes from disassembly, from *Sega-CD
Technical Bulletin #3*, and from shipped game source.

### In scope

- Mega CD disc projects (IP/SP boot chain, ISO mastering, MMD/SMD module system)
- Mega Drive cartridge projects
- C and M68k assembly, sharing one set of hardware definitions
- Hardware definitions for VDP, Gate Array (Main and Sub views), PCM (RF5C164), CDC/CD-ROM,
  Backup RAM, controller I/O, Z80 bus control
- Wrappers for the **Sub CPU BIOS** and the **Main CPU Boot ROM system library** — see the
  terminology note below, these are different things

### Explicitly out of scope / deferred

| Item | Status |
|---|---|
| Z80 assembly toolchain integration | Deferred. `megadev.make:20` declares `Z80_AS:=sjasmplus` but it is never used. `docs/manual.md:272` calls it "on the roadmap". |
| clang / LLVM as an alternative compiler | Deferred, speculative (`docs/manual.md:276`). |
| Mode 1 (Mega CD hardware driven from a cartridge) | **Intended, not yet implemented.** Settled as a target 2026-08-15 (§9 D23). The removed example survives only on the local branch `feature/md_cart` as `examples/cart_mode1/`; emulator support is unverified. |
| Main-CPU-side CD-ROM read path | Not supported; `docs/cdrom.md` states it "is not well understood". |
| C++ | **Declined**, not deferred. `docs/manual.md:274`: C++ is not felt to bring anything that would support embedded development better than native C. Users may retool the makefile themselves; this is unsupported. |
| A C standard library | Never. Builds are `-nostdlib -fno-builtin`. See `docs/dev_in_c.md`. |

---

## 2. Normative Architecture

### 2.1 The three-layer file scheme — the core invariant

Every source file in `lib/` is compiled through the C preprocessor: `megadev.make:158` assembles
with `gcc -x assembler-with-cpp`. That single fact is what makes the suffix scheme meaningful, and
it produces the project's most important rule:

> **INV-1 — `.def.h` files MUST contain only `#define` directives, `#include`s of other `.def.h`
> files, include guards, and comments.**
>
> No typedefs, no structs, no C casts, no function declarations, no storage. A `.def.h` is included
> by both C translation units and assembly sources; anything that is not legal in both languages
> breaks the assembly side.

| Suffix | Contents | Included by |
|---|---|---|
| `.def.h` | Addresses, bit positions, bitmasks, BIOS function codes. **Preprocessor-only.** | C **and** assembly |
| `.h` | C only: typedefs, structs, typed pointer macros, `static inline` functions (usually wrapping inline asm). | C |
| `.macro.s` | Assembly `.macro` definitions only. Emits no code, so it is safe to include repeatedly. | assembly |
| `.s` | Code-emitting assembly. Defines `.global` symbols and is linked in. | assembly (assembled directly) |

The intended pairing is `foo.def.h` (shared constants) → `foo.h` (C ergonomics) **and/or**
`foo.macro.s` (assembly ergonomics), plus `foo.s` where a real subroutine is warranted.

Additional rules, all mechanically checkable:

- **INV-2** — A `.h` or `.macro.s` MUST include the `.def.h` it depends on rather than relying on
  the caller's include order. *(Violated on `master` by `lib/main/vdp.macros.s`, which has no
  `#include` at all; and on `feature/sub_bios_overhaul` by `lib/sub/gate_arr.macro.s`,
  `lib/sub/bios.macro.s` and `lib/sub/boot.macro.s`.)*
- **INV-3** — A `.macro.s` MUST NOT emit code, and a code-emitting `.s` MUST NOT define macros.
  **Holds as of 2026-08-13.**
- **INV-4** — Every header MUST guard itself with `#pragma once` (§9 D13). **Holds everywhere as of
  2026-08-13.**
- **INV-5** — A `@file` doc tag MUST name the file it appears in.
- **INV-6** — Where a constant is used as an operand to a bit-level opcode (`btst`, `bset`, `bclr`,
  `bchg`), a companion giving the **bit index** MUST exist alongside the mask. Masks and indices are
  not interchangeable, and conflating them is a silent wrong-bit bug rather than a compile error.
  *(See KB-11.)*

  This is **long-standing project policy**, documented in `docs/manual.md` §"Bitwise Definition
  Naming", not a rule invented here. The naming scheme is settled in §9 **D12**: `_POS`, `_WIDTH`
  and `_MASK`, with values stored unshifted and placed by `FIELD_PREP`.
- **INV-12** — A `_POS` is **relative to the register it belongs to**, counting from that register's
  LSB — never from the byte the field happens to occupy within a wider register.

  Which register that is depends on the width the SDK models, settled in §9 **D17**: where the two
  halves of a 16-bit hardware register hold unrelated concerns, each byte is its own register and
  positions are 0–7. Registers that remain 16-bit (a single wide value, or fields spanning both
  halves) use word-relative positions per §9 **D16**.

  On a 16-bit register, a `_POS` of 8 or more cannot be handed to a bit opcode on a memory operand:
  the m68k takes the immediate **modulo 8**, so `btst #8` silently tests bit 0 of the same byte. Use
  `FIELD_BYTE(reg, field)` and `FIELD_BPOS(field)`. On a byte register no helper is needed, and Tier
  1.5 asserts that `FIELD_BPOS` is the identity there and that every field satisfies
  `POS + WIDTH <= 8`. *(See KB-34.)*
- **INV-8** — Every header MUST be self-contained: including it, and nothing else, into an empty
  translation unit must compile cleanly. Enforced by Tier 0.1 (§6).
- **INV-9** — A header MUST NOT define storage or a non-`static` function. Two translation units
  including it must link. Enforced by Tier 0.4 (§6). **Holds as of 2026-08-13** (42 pass, 0 fail).
- **INV-10** — A **hardware register** accessor dereferences itself, so it is used as `reg = x`
  rather than `*reg = x` (§9 D14). Enforced by Tier 1.
- **INV-11** — A **memory region** accessor stays indexable — it is *not* a scalar lvalue. Two forms
  are permitted:
  - a plain pointer, `((char volatile *) WORD_RAM)`, where no length is associated with the region;
  - a **sized** array lvalue, `(*((s16(*)[64]) BIOS_PALETTE_CACHE))`, where the extent is fixed by
    hardware. This is preferred when the size is known, because indexing still works *and*
    `sizeof` reports the region size — `BIOS_WORK_BUFFER_LEN` is derived that way rather than being
    a second hardcoded number that can drift.

  The distinction that matters is register versus region, not lvalue versus pointer: a register
  holds one value, a region holds many. Not mechanically enforced — telling a one-byte register from
  a one-byte region needs intent, not syntax — so it is a convention the reviewer applies.
- **STYLE-1** — Where two mnemonics assemble to the **identical encoding**, only the house spelling
  is written. Classified empirically against `m68k-linux-gnu-as`:

  | House spelling | Synonym (not used) | Encoding |
  |---|---|---|
  | `move` | `mov` | `2200` |
  | `dbra` | `dbf` | `51c8` |
  | `bcc` | `bhs` | `64fe` |
  | `bcs` | `blo` | `65fe` |

  Enforced by Tier 1.
- **STYLE-2** — **Prefer GNU branch-improvement pseudo-ops** — `jra`, `jbsr`, and the `jCC` family
  (`jhi jls jcc jcs jne jeq jvc jvs jpl jmi jge jlt jgt jle`) — where the target's reach is not
  known locally, which for Megadev means external symbols and cross-module calls. These are **not**
  synonyms: the assembler picks the smallest working encoding (`jbsr` emits a 2-byte `bsr.s` in
  reach, a 6-byte `jsr.l` out of reach), so rewriting them to a fixed instruction costs size and
  cycles. **Deliberately not machine-enforced** — a linter cannot tell a local label from a symbol
  that may end up out of reach, and flagging plain `bCC` on in-file branches would be noise.

### 2.2 Main / Sub separation

The Mega CD has two 68000s with separate address maps. The same peripheral appears at different
addresses to each:

| | Main CPU view | Sub CPU view |
|---|---|---|
| Gate Array base | `0xA12000` | `0xFF8000` |
| Defined in | `lib/main/gate_arr.def.h` | `lib/sub/gate_arr.def.h` |

> **INV-7 — Main-side and Sub-side definitions of the same peripheral MUST NOT share macro names.**

This is currently **violated**, and it is the most serious structural defect in the library: both
files define `GA_REG_RESET`, `GA_REG_MEMMODE`, `GA_REG_CDC_MODE`, `GA_REG_COMM_CMD0` and others with
*different values*, and both `.h` files define `typedef u16 volatile * ga_reg16` plus overlapping
`ga_reg_*` macros. Because their include guards differ, nothing prevents both from entering one
translation unit; whichever is included first silently wins. See KB-12 and §9 OD-1.

### 2.3 Module system (MMD / SMD)

Modules are relocatable program/data blobs loaded from disc at runtime.

- A Main-CPU module (`.mmd`) carries a **0x100-byte header emitted by the linker script**
  (`cfg/module_mmd.ld:31-42`): flags, destination address, a longword copy count of
  `((_ROM_LENGTH + _RAM_DATA_LENGTH) >> 2) - 1` (pre-decremented for a `dbf` loop), and entry,
  hblank and vblank vectors.
- A Sub-CPU module (`.smd`) has **no header** (`cfg/module_smd.ld`).
- Layout is supplied by the project as `GLOBAL` symbols — `MODULE_ROM_ORIGIN`, `MODULE_ROM_LENGTH`,
  `MODULE_RAM_ORIGIN`, `MODULE_RAM_LENGTH` — conventionally in a `*_layout.s` file.
- Cross-module symbol resolution uses `ld -R <other.elf>`, which imports symbols without code, so a
  transient module can call into a resident one.

### 2.4 Mega CD boot chain

1. The Boot ROM reads the disc header and the **security block**, which is region-specific
   (`lib/security.c`) and **MUST be first in `.text`** (`cfg/ip.ld`).
2. **IP** (Initial Program) is loaded to `0xFF0000` on the Main CPU. Size is user-definable via
   `IP_LENGTH` but must fit the boot sector — **3.5 KB** (`cfg/ip.ld:8-9`).
3. **SP** (System Program) is loaded to `0x6000` on the Sub CPU. Default `SP_LENGTH` is `0x4000`;
   the boot-sector ceiling is **28 KB** (`cfg/sp.ld:8-9`).
4. The SP must provide four entry points via its header jump table: `sp_init`, `sp_main`, `sp_int2`,
   `sp_user` (`docs/boot.md`).
5. `lib/cd_boot.s` `.incbin`s the built `ip.bin` and `sp.bin` and prepends the ASCII disc header;
   `mkisofs -G boot.bin` splices the result into the generic boot area.

Consequence: **every Mega CD project must provide `src/ip.s` and `src/sp.s`.**

---

## 3. Naming & API Conventions

These are normative and are the basis for the Tier 1 lint (§6).

- **Prefixes.** Hardware definitions carry a subsystem prefix: `GA_` (Gate Array), `VDP_`, `BIOS_`,
  `BRAM_`, `BOOT_`, `PCM_`, `SCTRL_`/`EXT_` (I/O). No unprefixed globals.
- **Case.** `UPPER_SNAKE_CASE` for `.def.h` constants and assembly macros; `lower_snake_case` for C
  macros and functions; `PascalCase` for types.
- **Bit constants.** `FOO_BIT` is a bit *index*; `FOO` is the corresponding *mask*, defined as
  `(1 << FOO_BIT)`. Both forms exist wherever bit-level opcodes are used (INV-6).
- **No libc shadowing.** Megadev MUST NOT define a name from the C standard library with different
  semantics. *(Currently violated: `strcmp` returns `bool` (`lib/str_util.h:68`), `strcpy` returns
  `void` (`lib/memory.h:120`), plus `abs`/`abs16` in `lib/math.h`.)*
- **No reserved identifiers.** Leading-underscore names at file scope are reserved to the
  implementation. *(Currently violated by `lib/sub/pcm.def.h:11-19`: `_PCM_ENV` etc.)*
- **Umbrella headers** (`lib/main/main.h`, `lib/sub/sub.h`) MUST include every public header for
  their CPU side. *(Currently violated: `main.h` omits `bios.h`, `comm.h`, `md_sys.h`, `mmd.h`;
  `sub.h` omits `bios.h`.)*
- **Doxygen.** Every public definition carries `@brief`. Assembly routines and macros that clobber
  registers carry `@clobber`. Custom commands MUST be declared in `Doxyfile` `ALIASES` before use.
  *(Currently violated: `@alias` is used 49 times and is not declared; `@macro`, `@in`, `@out`,
  `@desc` are used in `.macro.s` files and are not Doxygen commands at all.)*
- **Register-access macro form** — see §9 OD-4, unresolved.

---

## 4. Build Contract

### What a project must provide

Required (`megadev.make:30-48` errors if unset): `PROJECT_ID`, `TARGET`, `SRC_PATH`, `RES_PATH`,
`BUILD_PATH`. Mega CD projects additionally need `DISC_PATH`, `src/ip.s` and `src/sp.s`.

`MEGADEV_PATH` must point at the Megadev installation (`/opt/megadev` by convention).
**It is not currently sanity-checked**, so an unset value silently yields `LIB_PATH=/lib` and
confusing downstream failures — see KB-14.

### Guarantees the build layer owes the user

- **B-1** — `make` on a freshly cloned project produces a runnable image without manual steps.
  **Holds as of 2026-08-13.** Output directories are created by the rules that write into them.
  Enforced by Tier 0.3, which deliberately does *not* run `make init`.
- **B-2** — Editing a header rebuilds its dependents. **Holds as of 2026-08-13.** Compiles pass
  `-MMD -MP` and the generated `.d` files are included. Crucially, module targets also gained the
  objects as prerequisites (via `.SECONDEXPANSION:`) — without that edge the dependency information
  existed but was never consulted, because make had no reason to consider the object at all.
- **B-3** — Builds are reproducible. **Holds with `SOURCE_DATE_EPOCH` set**, verified byte-identical
  across clean builds. Two things had to be pinned, not one: `mkisofs -creation-date` for the volume,
  **and** the payload file mtimes, because ISO9660 records a timestamp per file. Pinning only the
  volume date still produced differing images once the modules were rebuilt.
- **B-4** — `make -j` is safe. **Holds as of 2026-08-13.** Projects declare `DISC_CONTENTS`, which
  the ISO depends on, so the image cannot be mastered before its modules exist. Verified with
  repeated `make -j8` builds from clean.
- **B-5** — Two sources with the same basename in different directories do not collide. **Holds as
  of 2026-08-13.** Object paths mirror the source path they are named by, so `sub/pcm.s` becomes
  `$(BUILD_PATH)/sub/pcm.s.o`.

B-1 through B-5 are the acceptance criteria for the build-system work in BACKLOG.md.

---

## 5. Versioning & Compatibility

- Version of record is the `VERSION` file. It is currently referenced by nothing — not by
  `megadev.make`, not by any header, and `Doxyfile` independently declares `PROJECT_NUMBER = 1`.
- Tags MUST be `vMAJOR.MINOR.PATCH`, annotated. *(History is inconsistent: both `1.0.0` and `v1.0.0`
  exist for the same release; `0.1.6`, `0.1.7` and `v1.2.0` are lightweight tags carrying no
  message or date.)*
- **Breaking API changes are permitted** and are released as a major version with a migration note
  (§9 D3). Megadev has a small user base and correcting the naming and layering now is cheaper than
  carrying it indefinitely.

---

## 6. Verification Policy

Megadev targets obsolete hardware, so "run the test suite" needs definition. Verification is tiered.

| Tier | What it proves | Status |
|---|---|---|
| **0 — Build gate** | The toolchain accepts the source, and the build behaves. | **Implemented and GREEN** (m68k gcc 14.2.0): headers 70, assembly 23 (2 excluded), ODR 42, symbols 115, 7 projects build with verified contents, incremental rebuild correct. |
| **1 — Convention lint** | The rules in §2–§3 actually hold. | **Implemented and GREEN** (15 baselined, down from 16). 25 unit tests, `make test`. |
| **2 — On-target tests** | The code computes the right answers on a real 68000. | **Specified, not built.** |
| **3 — Hardware validation** | Behaviour matches real Mega CD silicon. | Manual; tracked as provenance (§7). |

### Tier 0 — build gate

Three jobs, run inside the devcontainer image:

1. **Per-header compile.** One generated translation unit per `lib/**/*.h` containing only that
   header's `#include`, compiled `-m68000 -fsyntax-only -Wall -Wextra -Werror=infinite-recursion`.
   This requires each header to be classified **Main-valid / Sub-valid / both** — that classification
   is itself spec content and does not currently exist.
2. **Assemble every** `.s` and `.macro.s` via `gcc -x assembler-with-cpp -c`. Files that cannot
   assemble standalone *by contract* are listed with their reason in `tools/check/asm-exclude.txt`;
   everything else must assemble.
3. **Full build** of every example and `new_project` through to final `.iso` / `.cart`.
4. **One-definition-rule check.** Two translation units include the same header and are linked
   together (`-fno-common`). See INV-9.
5. **Symbol resolution.** Every undefined symbol in the lib objects is cross-referenced against
   everything lib defines; the remainder must be a documented external.
6. **Incremental rebuild.** Touching a library header must recompile its dependents, and an
   unchanged rebuild must do nothing. This exists because header dependency tracking can be lost
   without any build failing — which happened, and shipped.

> **What Tier 0.1 does NOT catch — measured, not assumed.**
> On its first real run the per-header compile reported **70 passed, 0 failed**, while the library
> demonstrably contains defects. A header compile cannot see:
> - **macro defects**, because a macro that is never expanded is never parsed. `time_mapping`
>   (`io.h:94`) and `bios_work_buffer` (`bios.h:108`) are both hard compile errors — but only in a
>   TU that *uses* them.
> - **inline-asm defects**, because the asm string of a `static inline` that is never instantiated
>   is never handed to the assembler.
> - **one-definition-rule defects**, which by construction need two TUs and a linker. This is why
>   Tier 0.4 exists; it found 5 failures on its first run, one of which (`z80_init`) the manual
>   audit had missed entirely.
>
> An earlier draft of this document claimed Tier 0 alone would have caught every defect in §8.
> That was wrong. Tier 0 catches assembly-level and standalone-compile defects; **the macro,
> semantic and cross-TU classes need Tier 0.4 and Tier 2.**

### Tier 1 — convention lint

Mechanical checks of INV-1 (`.def.h` contains only `#define`/comments — the load-bearing one),
INV-4 (guard name matches path), INV-5 (`@file` matches filename), `clang-format --dry-run --Werror`,
and Doxygen with `WARN_AS_ERROR`.

Because the library predates these rules, the lint uses a **baseline** of accepted existing
violations (`tools/check/baseline.txt`, currently 16). Anything not in the baseline fails. The
baseline should only ever shrink — that is the ratchet.

Result of the first run: **INV-1 holds everywhere on `master`** — every `.def.h` really is
preprocessor-only. The 16 accepted violations are 11 include-guard names and 5 wrong `@file` tags.

### Tier 1.5 — compile-time semantic assertions

Tier 0.1 proves a macro *parses*; it says nothing about what the macro *evaluates to*, because a
macro that is never expanded is never checked. Tier 1.5 expands them and asserts the results, in
`tools/check/asserts/`.

It runs in **both languages, deliberately**. `*.c` files use `_Static_assert`; `*.s` files use
`.if`/`.error` and are assembled for real. The assembly half is not redundant: GNU as accepts a much
smaller expression grammar than C, and a macro in a `.def.h` has to expand in both. The first cut of
`FIELD_BYTE` used a ternary — legal C, rejected outright by GNU as — and a C-only Tier 1.5 passed it
(KB-37). An assertion file that only ever exercises one of the two languages is checking half the
contract.

The assembly asserts also emit real instructions, not just `.if` expressions, so a macro that
evaluates correctly but cannot be used as an operand still fails.

### Tier 2 — on-target tests (specified, not built)

A `tests/` project built like any other Megadev project, run under a headless emulator
(BlastEm or Genesis Plus GX; both have workable Mega CD support). Assertions run on target and report
out-of-band: a result byte at a known RAM address, or over the existing serial-over-EXT-port channel
(`lib/main/comm.h`) — the transport already exists. CI dumps the result after N frames; non-zero fails.

**First target: differential C-vs-assembly tests.** Megadev implements the same routine twice — once
as a C `static inline` and once as an assembly macro or subroutine — in at least twelve places
(interrupt control, Z80 bus control, joypad read, Gate Array Word RAM handover, hex conversion, ...).
These are *supposed* to be equivalent, and they have already silently diverged: `hextoa8` writes an
`0xFF` terminator in `lib/str_util.s:26` and **no terminator at all** in `lib/str_util.h:26-32`.
Running both and comparing is the correct instrument for this entire class of bug, and the ~550 lines
of pure functions in `math.h`, `fixed.h`, `memory.h` and `str_util.*` are the natural first subjects —
two of them are outright wrong today (KB-3, KB-8).

### Tier 3 — hardware validation

Cannot be automated. Tracked as provenance in §7: no claim is silently promoted from "assumed" to
"true" without someone putting it on real hardware.

---

## 7. Hardware Claim Provenance

Every hardware assertion carries one of: `HW` (verified on real hardware, model noted), `EMU`
(emulator only), `DOC` (from Sega documentation or credible third-party research), `ASSUMED`.

### Primary sources

Official Sega documentation and source dumps are held outside the repo, on the `motoi-nas` library
share (verified present 2026-08-13):

- `/mnt/library/Retro Games/By Hardware/Sega Mega CD/`
  - `Development Documents/` — Mega-CD BIOS Manual, **Hardware Manual "The Hardware"**, Hardware
    Manual "PCM Sound Source", Disc Format Specifications, Mega-CD Outline, CTrac tooling, Cinepak
    bulletins.
  - `BIOS Revisions/` — dated Boot ROM dumps (JP 1.00C through 1.11 and later).
  - `Source Code/` — Batman Returns Sega CD project package.
**The gate array registers are documented in *two* Sega manuals, and they disagree.** Besides *The
Hardware*, the **Mega-CD Software Development Manual** (Ver 0.10, 1991-03-06 — seven months earlier)
covers the same register block. Added here 2026-08-14; it had been overlooked. Read both when a
register claim matters, and treat a disagreement as a live question rather than assuming the newer
document wins.

`Mega-CD Software Development.pdf` (43 pages) is the copy to use — like the Kodak *Hardware* scan it
carries an **OCR text layer**. PDF page = printed page **+ 7** (printed 19 = PDF 26), the same offset.

> **`Mega-CD Software Development Manual - Corrections Email - [1992-12-21] (Rex Sabio).pdf`** is a
> one-page errata from Sega of America and is worth more than its size suggests. It records four
> corrections found "while working with the Scaling Hardware":
>
> - **`RPT` in `$FF8058` is documented backwards in the Software Manual (printed p.19).** That manual
>   gives `0` = repeat; the errata says the bit "works exactly opposite". ***The Hardware* p.35 gives
>   `1` = repeat, and that is what megadev implements — verified at 600 dpi.** Recorded because the
>   next reader to consult the Software Manual will otherwise "fix" a correct SDK into a broken one.
> - Priority applies to **scaled pixel data** written to the image buffer — independent corroboration
>   that priority governs graphics-operation output, not CPU writes (see `docs/rotation_scaling.md`
>   §5). Sega's names there are Write Up / Write Down for what megadev calls `OVERWRITE`/`UNDERWRITE`,
>   with matching semantics.
> - `VCS0-4` in the image buffer V-cell size is the cell count **minus one** — matches megadev.
> - Trace vector start positions have **3** fractional bits (1.0 = `8`) and deltas **11** (1.0 =
>   `$0800`) — matches `lib/xform.h`.

- `/mnt/library/Retro Games/By Hardware/Sega Mega Drive/`
  - `Development Documents/` — Genesis Software Manual, Genesis Technical Bulletins, reference sheets.
  - `Dev Resources and Source Code/` — Batman Returns, Cliffhanger, Ex-Mutants project packages,
    SegaKit.
- `/mnt/library/Retro Games/By Hardware/Sega Super 32X/dev examples/CD_V2A/`
  - **Sega's own Mega CD source.** `INC/CDBIOS.INC` defines the boot-call function codes
    (`CBTINIT`…`CBTSPSTAT`) and the BRAM codes; `INC/MAINCPU.INC` is the file quoted in
    `docs/main_bios.md` on the "not for general use" memory area. This is the "32X CD example
    source" that `lib/sub/cdboot.def.h` refers to.
- `/mnt/library/Retro Games/By Organization/Sega/`
  - `Megadrive/Old Project Source Dumps/`, `Data Dumps/SEGA Disk Backups/Sega Developer Tools/` —
    a larger source tree than the by-hardware folders; further copies of the above.

**Sought and confirmed absent** (searched 2026-08-13): `ROM_UTIL.DOC` and `MAINENT.I`, the two files
*Sega-CD Technical Bulletin #3* points to for Main-side Boot ROM documentation. Their absence is why
`lib/main/bios.*` rests on reverse engineering. If either ever surfaces, it supersedes a great deal
of `docs/main_bios.md`.

**Present and under-used:** `Sega-CD_Technical_Bulletins.pdf` and `Sega-CD Technical Bulletins.pdf`
collect the numbered bulletins, including **#3** — the primary written evidence that the Main-side
calls were sanctioned for game use, and the source of the Work RAM equates in `docs/main_bios.md`.

> **A third, far better scan exists for eleven pages.**
> `Development Documents/mcd_manual_sphere/` holds clean, high-contrast scans (SPHERE, INC.) of
> *The Hardware* — `pg22`–`pg30`, `pg32`, `pg33` and `pg58`. **The filenames are not the printed page
> numbers**: `pg29`=29 and `pg30`=30, but `pg31`=**32** and `pg32`=**33**. Printed page 31 (the CD
> fader) is the one page in that range the set does not contain.
>
> Where a page exists here, read it instead of the fax — it is legible where the fax is destroyed,
> and it settled the `$FF8036` field positions that were previously ASSUMED. It also carries a
> previous reader's handwritten corrections (e.g. "unmeric" → "numeric" on page 30).

> **A third scan exists and is the best general-purpose copy: `Mega-CD Hardware Manual.pdf`.**
> Identified 2026-08-14; previously present in the share but not distinguished here from the fax
> scans. 97 pages against the fax scans' 66 and 70, produced on a Kodak Capture Pro scanner from a
> **Sega Ozisoft** copy (stamped `#161`, where the fax scans are `#66`), and — uniquely — it carries
> an **OCR text layer**, so `pdftotext` works on it and the manual is searchable.
>
> PDF page = printed page **+ 7** in the rotation/scaling range (printed 45 = PDF 52).
>
> It is **not an independent document**: 118 pages still carry the `8137437483` fax header, so it is
> largely the same transmission and the same content, and the SPEC warning below still applies. It is
> a mixed copy — some pages (e.g. printed 45) are clean non-fax originals, most are not. Its value is
> legibility plus searchability, not corroboration.
>
> **Checked and it does not help:** printed page 31 (`$FF8034`, the CD fader) is the fax page here
> too, with the same destroyed bit table, so the `EFDT`/`DEF` positions remain unrecorded. The prose
> *is* legible and confirms `FD00–10` at bits 4–14, `EFDT` as 1=Busy / 0=Ready, and the de-emphasis
> table (00=OFF, 01=44.1 kHz, 10=32 kHz, 11=48 kHz). Do not re-check this scan for those positions.

> **The two "translations" are two scans of the same fax, not independent translations.** Both the
> *(Rex Sabio)* and *(The Code Monkeys)* copies of *The Hardware* carry the identical transmission
> header (`8137437483 #24/48`), so comparing them verifies **legibility, not content** — a
> mistranslation is present in both. The Rex Sabio scan is the higher resolution of the two and is
> the one to read when a figure is unclear.
>
> Where a claim rests on wording that is ambiguous in the scan, record it as `ASSUMED` and say which
> page it came from, rather than treating agreement between the two files as corroboration.

#### Page map of *The Hardware*, Ver 1.0

The scan's PDF page number is the printed page number plus a **constant that changes partway
through**: **+4** up to and including printed page 39 (printed 21 = PDF 25, printed 39 = PDF 43) and **+3**
from printed page 55 on (printed 55 = PDF 58, printed 59 = PDF 62). **The dropped page is printed 45**
(located 2026-08-14): the Rex Sabio scan runs printed 40, 41, 42, 43, 44, then 46. Nothing is lost —
printed 45 is a **blank page**, confirmed against the Kodak scan below, which retains it and prints
"Blank Page" on it. Sections:

| Printed | Contents |
|---|---|
| 12–18 | §1 Mapping: 2M / 1M modes, Sub CPU map, Main CPU map |
| 19–21 | §2 Register table; **p.21 is the access-width and bit-operation table** |
| 22–25 | §3-1 Sub CPU init: `$FF8000`, `$FF8002`, Word RAM switching in 1M and 2M |
| **26–27** | §3-2 Sub CPU CDC: `$FF8004`, `$FF8006`, `$FF8008`, `$FF800A` |
| 28–29 | §3-3 Sub CPU communication: `$FF800C`, `$FF800E`, `$FF8010`–`$FF802E` |
| 30 | §3-4 timer `$FF8030`, §3-5 interrupt mask `$FF8032` |
| 31–33 | §3-6 CDD: fader `$FF8034`, control `$FF8036`, comms `$FF8038`–`$FF804A` |
| 34 | §3-7 colour operation: `$FF804C`, `$FF804E`, `$FF8050`–`$FF8056` |
| 35–54 | §3-8 rotation / scaling: `$FF8058`–`$FF8066`, stamps, image buffer |
| 55 | §3-9 sub-code: `$FF8068`, `$FF8100`–`$FF817E`, image at `$FF8180` |
| 56–60 | §4 Main CPU registers: `$A12000`–`$A1202E` |
| 61+ | Supplements; PCM sound source (RF5C164) appendix |

| Claim | Provenance | Source / note |
|---|---|---|
| Gate Array register map, Sub side | DOC | Sega BIOS manual + community research |
| Gate Array register map, Main side | DOC | as above |
| Gate Array field **bit positions** | DOC | *Hardware Manual - The Hardware*, pp. 21, 57, 58 (PDF pages 25, 60, 61 in the Rex Sabio scan). Page 57 gives `$A12002`: `WP0-7` in the high byte, `BK0,1` at bits 6-7, `MODE`/`DMNA`/`RET` at bits 2/1/0. Page 58 gives `$A12004`: `DD0-2` at bits 8-10. Page 21 is the per-register access-width and bit-operation table. |
| Register **names** | **origin, not corroboration** | Megadev's gate array register names were taken from Sega's own `INC/SUBCPU.INC` and `INCLUDE/CDMAP.I` (32X dev examples) when the project was written. An earlier note here called the match "convergence"; that was wrong — these files are the source of the naming, so agreement with them says nothing about whether the names are independently correct. |
| `_stampsize` is a **byte** at `$FF8059` | **DOC** | `CD_V1A/SOURCE/INCLUDE/CDMAP.I` line 76: `_stampsize equ $ffff8059 * byte`, against `* word` for every other register in the block. Sega's own memory map splits this register exactly as §9 **D17a** does, placing the size configuration at the odd address as a byte and leaving `GRON` in the high byte. D17a was derived from field geometry before this file was consulted, so this is genuine corroboration of the split — unlike the naming above. |
| Word bit positions `EDT` and `WP7` | **DOC** | `SUBCPU.INC`: `CDCMODE_EDT_MSK EQU $8000` and `MEMORYMODE_WP7_MSK EQU $8000`. Both confirm the fax transcription — EDT at word bit 15, and write protect occupying the **high** byte, which is the KB-34 correction. Whether megadev's original (wrong) positions also came from these files is unknown; the masks here are correct, so the error was introduced downstream of them. |
| Byte-wise addressing of the split registers | **DOC** | `WORK/sega_dev_stuff/STRIKE/NUGAME/SP.S` is production Sega-era game code and addresses these registers as D17 models them: `BCLR.B #2,$FF8003` and `BSET.B #0,$FF8003` for memory mode, `BTST.B #7,$FF800E` / `BSET.B #7,$FF800F` for the two halves of the communication flags, `MOVE.B $FF8004` for CDC mode. Sega's `SUBCPU.INC` meanwhile defines *word* masks for the same registers. Both views were in period use, which supports D17 being a modelling choice rather than a claim about the hardware. |
| `$FF8036` field positions | **DOC** | Confirmed 2026-08-14 against `mcd_manual_sphere/pg31.jpg` (printed page 32), which is legible where the fax is not: `DTS`=0, `DRS`=1, `HOCK`=2, `D/M`=8. Matches the values the SDK already carried, so those are promoted from ASSUMED. `DTS`/`DRS` accept only 0 on write, `HOCK` is fully writable, `D/M` is read only. |
| Stamp map base address alignment | **DOC, against the manual's own figure** | *The Hardware* printed **page 36** labels the two `SMS`/`STS` cases with the **stamp sizes swapped** — it calls `SMS=1`/`STS=0` "32x32 dots" where `STS=0` is 16x16 by the same manual's definition on page 35. The address tables either side of that text are self-consistent, and `lib/sub/gate_arr.def.h`'s alignment table is derived from the map geometry, which agrees with them. **Do not "correct" the alignment table against page 36.** Recorded here because the warning previously lived in the Doxygen comment, which is not where provenance belongs (D15). |
| `$FF8034` field positions | **DOC — resolved 2026-08-14** | Previously ASSUMED and believed unrecoverable: *The Hardware* p.31 is obscured in the fax and is the one page missing from the SPHERE set. **The Software Development Manual (printed p.16) carries the same register with a fully legible bit table**, which is why a second manual is worth reading. The full row, MSB first: `EFDT`, `FD10`…`FD00`, `DEF1`, `DEF0`, `SSF`, unused. So `EFDT` = 15, `FD00-10` = 4–14, `DEF1` = 3, `DEF0` = 2, **`SSF` = 1** — a spindle speed flag (0 normal, 1 double speed) that megadev did not have at all. `FD` at 4–14 agrees with *The Hardware*'s legend text, corroborating the row. Access: `EFDT` is read only, `FD`/`DEF`/`SSF` are write only, bit 0 is unused. Pinned by Tier 1.5 assertions, including that the four fields tile the register. |
| CDD and CD fader are **BIOS-only** | DOC | *The Hardware* pp. 31–32 both carry "Direct access to this register using application software is prohibited." Use the Sub BIOS calls. |
| Mega Drive ROM header checksum algorithm | DOC | Sum of 16-bit big-endian words from 0x200 to end of ROM, truncated to 16 bits. Hardware does not verify it; flashcarts and loaders read the ROM end field at 0x1A4. Implemented in `tools/romfix.py`. |
| Sub CPU BIOS function codes | DOC | *Mega-CD BIOS Manual* (official) |
| Main CPU Boot ROM system library | **Reverse-engineered** | No official English documentation exists at all. Corroborated by *Sega-CD Technical Bulletin #3*, which states plainly that "subroutine in the boot ROM may also be used" and refers to two files — `ROM_UTIL.DOC` and `MAINENT.I` — that are **still missing** (searched the library share 2026-08-13). Per-call detail in `docs/main_bios.md`. |
| Boot-call function codes (`CBT*`) | **DOC** | Verified byte-for-byte against Sega's own `CDBIOS.INC` in the 32X CD example source; see below. |
| Boot sector layout may be changed | **ASSUMED — open question** | `docs/boot.md:15`. Check *Mega-CD Disc Format Specifications* and the BIOS Manual before testing on hardware. |
| Writing 0 to Word RAM mode bits in 2M mode | **ASSUMED — open question** | `lib/sub/gate_arr.def.h` `@todo` cites *"the 'The Hardware' documentation"* — that is *Mega-CD Hardware Manual - The Hardware*, held in the library share above. |
| Main-CPU CD-ROM read path | **UNKNOWN** | `docs/cdrom.md`: "not well understood" |
| CDC DMA transfer speed / bus contention | **UNKNOWN** | `docs/cdrom.md`. Start with *Mega-CD Hardware Manual - The Hardware*. |
| BRAM cart bank/ID map | **ASSUMED** | `lib/main/bramcart.def.h` — from a spritesmind forum thread, not an official source. Cross-check against the BIOS Manual. |
| `HW_REV` is 3 bits | **SUSPECT** | `lib/main/md_sys.def.h:28` defines `(0b111 << 0)`; its own comment shows VER3..VER0 (4 bits). Gates the TMSS write in `md_init.s:32`. Settle against the *Genesis Software Manual*. |

---

## 8. Known Broken

Catalogued by inspection on 2026-08-13, then **re-tested against m68k gcc 14.2.0** in the
devcontainer the same day. Every ✅ row below is now backed by a compiler, linker or assembler
diagnostic, not by reading. Marked ✅ = present on `master`; ⚠️ = introduced on
`feature/sub_bios_overhaul` and not on master.

**Two entries from the original inspection were wrong and have been withdrawn:**

| Withdrawn | Why |
|---|---|
| ~~`mov.l` is not an M68k mnemonic~~ (was KB-1, KB-2) | **False positive.** GNU `as` accepts `mov` as an alias for `move`; `mov.l (a0)+,(a1)+` and `move.l (a0)+,(a1)+` both assemble to `0x22d8`. `BASIC_INIT` assembles cleanly. Retained only as a style rule (STYLE-1) at the maintainer's request, and now fixed. |
| ~~`cd_exception.s` fails to assemble~~ (part of KB-7) | The file **assembles cleanly**. `EXVECEXVEC_TRACE` is emitted as an undefined symbol reference, so if it is genuinely undefined it fails at *link*, not assembly. Not yet link-verified — see KB-7 below. |

| ID | Where | Defect | On master |
|---|---|---|---|
| KB-3 | `lib/math.h:46-47, 59-60` | `out.quot` assigned twice; `out.rem` **never assigned**. `divu()` and `div()` both return the wrong quotient and an uninitialised remainder. | ✅ |
| KB-4 | `lib/math.h:58` | `div()` documented as signed (DIVS) but emits `divu.w` | ✅ |
| KB-5 | `lib/memory.h:120` | **FIXED** 2026-08-13 — now `static inline`. **Link-verified.** `void strcpy(...)` — non-static function definition in a header; `multiple definition of 'strcpy'` when two TUs link. | ✅ |
| KB-6 | `lib/sub/bram.h:15,16` | **FIXED** 2026-08-13 — now `extern`, defined in `lib/sub/bram.c`. **Link-verified.** Tentative definitions in a header. **Five** duplicate symbols, not two: `bram_work_buff`, `bram_string_buff`, `brmstat_results`, `brmserch_results`, `brmread_results`. Fails under `-fno-common` (the GCC 10+ default). Propagates to `lib/sub/sub.h`. | ✅ |
| KB-7 | `lib/main/cd_exception.s:79` | `EXVECEXVEC_TRACE` — botched find-and-replace. The file assembles; the symbol becomes an undefined reference, so this surfaces at **link**. Not yet link-verified. | ✅ |
| KB-8 | `lib/fixed.h:34` | **Proven by `_Static_assert`**: `int_to_f32(1)` and `int_to_f32(5)` both evaluate to 0. Shifts left by 16 then casts to `short`. | ✅ |
| KB-9 | `lib/main/io.h:94` | **Compile-verified**: `error: expected expression before '[' token` when expanded. Zero references repo-wide. | ✅ |
| KB-10 | `lib/main/bios.h:108` | **Compile-verified**: `error: cast specifies array type` when expanded. The commented-out line 109 is the working version. | ✅ |
| KB-11 | `lib/main/comm.h:51,74` + `comm.macros.s:41,53` | **Proven by `_Static_assert`**: `SCTRL_TX_FULL == 1` and `SCTRL_RX_READY == 2` — masks. Passed to `btst` they select bits 1 and 2 instead of bits 0 and 1, in both the C and assembly copies. Root cause: `io.def.h` has no `_BIT` companions (INV-6). | ✅ |
| KB-12 | `lib/main/gate_arr.def.h` vs `lib/sub/gate_arr.def.h` | Same macro names, different values, non-matching include guards (INV-7) | ✅ |
| KB-34 | `lib/sub/gate_arr.def.h`, `lib/main/gate_arr.def.h` | **Fixed 2026-08-13.** Three field groups gave positions relative to their *byte* rather than their register, so the mask derived from each named the wrong bits: `GA_LED_R`/`GA_LED_G` at 0/1 rather than 8/9 (bit 0 of that register is **RES0, the peripheral reset**, so `ga_reg_reset | GA_LED_R_MASK` would have reset the peripheral instead of lighting the LED); `GA_MEMMODE_WP` at 0, deriving `0x00FF` for a field that occupies `0xFF00`; and the Main-side `GA_CDC_DEST` at 0 rather than 8. Confirmed against the Hardware Manual pages 57 and 58 (§7). All three are now register-relative (§9 D16, INV-12), with `FIELD_BYTE`/`FIELD_BPOS` for the bit-opcode case and Tier 1.5 assertions pinning the positions. `GA_MEMMODE_BANK` was checked on the same page and was already correct. None of the three had a use site, so nothing downstream changed. | ✅ |
| KB-35 | `lib/sub/gate_arr.def.h` | **Fixed 2026-08-13.** The `GA_CDCMODE_*` field block was wrong in three ways at once and carried its own `// these aren't right... // TODO clean these up`. `DSR`/`EDT` were byte-relative (6, 7) rather than word-relative (14, 15). `DD0` was position 5, which is neither. And `MAINREAD`/`SUBREAD`/`PCMDMA`/`PRAMDMA`/`WRAMDMA` were declared as `_POS`/`_WIDTH`/`_MASK` when the numbers 2, 3, 4, 5, 7 are **destination values**, duplicated verbatim from the `CDC_DEST_*` list below them — a value set masquerading as bit positions. Replaced with `GA_CDCMODE_{CA,DD,UBR,DSR,EDT}` at the positions the manual gives (p.26), `DD` as one 3-bit field with unshifted values per D12. | ✅ |
| KB-36 | `lib/sub/cdrom.s:339,451` | **Fixed 2026-08-13.** `btst #GA_CDCMODE_DSR_POS-8` with `DSR_POS` of 6 assembles to `btst #-2`, and the `EDT` site to `btst #-1`. Both were **accidentally correct**: the 68000 uses only the low three bits of the immediate, so `-2` selects bit 6 and `-1` selects bit 7, which is what the byte-relative constants meant. Two errors cancelling. Rewritten with `FIELD_BYTE`/`FIELD_BPOS`; the emitted code was diffed before and after and is identical but for the two immediates, now `#6` and `#7`. | ✅ |
| KB-37 | `lib/build.def.h` | **Fixed 2026-08-13.** `FIELD_BYTE` was first written with a ternary, which C accepts and **GNU as rejects outright** (`found '?', expected: ')'`) — in a macro whose whole purpose is bit opcodes in assembly. Tier 1.5 was C-only, so the gate passed it. Rewritten as `((reg) + 1 - ((POS >> 3) & 1))`, and Tier 1.5 now assembles `asserts/*.s` as well, so the assembly grammar is actually exercised. | ✅ |
| KB-38 | `lib/main/gate_arr.h` | **Fixed 2026-08-14.** All eight `ga_reg_comstat0..7` accessors were typed `ga_reg`, i.e. writable, but comm status is **Read Only** from the Main CPU (manual p.60) — the Sub CPU writes it. Nothing in the tree wrote them, so this was latent; now typed `ga_reg_ro` and verified to reject an assignment. The mirrored case on the Sub side (`ga_reg_comcmd0..7`) was already correct. | ✅ |
| KB-39 | `lib/main/gate_arr.h` | **Fixed 2026-08-14.** `ga_reg_stopwatch` and `ga_reg_cdchostdata` were typed `ga_reg`, i.e. writable, but both are **read only from the Main CPU** (manual pp. 58-59; the WR row of each is entirely `-`). For the stopwatch this is not a formality: only the Sub CPU can clear the timer, by writing its own `$FF800C`, so Main-side code that "resets" the stopwatch before timing something has been silently measuring from an arbitrary point. Same class as KB-38. | ✅ |
| KB-40 | `examples/transforms/src/spx.c:157` | **Introduced and fixed 2026-08-14.** The D17 split made `ga_reg_stampsize` a byte register, and the example's wait loop polled `ga_reg_stampsize & 0x8000` for GRON. The expression stayed valid C and folded to 0 — GCC emits **no diagnostic at any warning level**, verified against `-Wall -Wextra -Wconversion` — so the loop stopped waiting entirely and the example read the image buffer while the hardware was still writing it. This was a regression created by the refactor, not a pre-existing defect. Fixed to `ga_reg_gfxstat & GA_GRON_MASK`, and Tier 1 now rejects any byte-wide accessor masked with a constant above 0xFF, scanning `examples/` and `new_project/` as well as `lib/`. | ✅ |
| KB-41 | `lib/xform.h` | **WITHDRAWN 2026-08-14 — the "fix" was wrong; original behaviour restored.** I read the manual's bit table for the trace vector step (printed p.39), saw a separate `+/-` cell at bit 15, and concluded the format was sign-and-magnitude rather than two's complement. That was wrong, and megadev's original two's complement encoding was correct all along. **Disproved by tracing Sonic CD's special stage** (capture kept outside the repo, `megadev-testing/trace/sonic-cd-special-stage.log` — see D19): in a rotating ground plane `dx` and `dy` are components of one step vector, so their magnitudes must scale together and the direction must stay constant across rows. The capture shows `dx` falling +14.62 → +4.38 over rows 3–10. Read as two's complement, `dy` falls −15.97 → −4.29 — a constant ≈−47° direction scaled by distance. Read as sign-and-magnitude, `dy` would run −0.03 → −11.71, swinging the view through 69° in eight rows. Only two's complement is geometrically possible. **Lesson: a bit table drawn with a separate sign cell does not imply sign-and-magnitude**, and this would have silently broken every rotation in the pseudo-3D example while the maths looked correct. | ✅ |
| KB-43 | `lib/sub/gate_arr.h` | **Fixed 2026-08-14.** `ga_reg_cdchostdata` was typed `ga_reg`, i.e. writable, on the **Sub** side. The Software Development Manual (printed p.13) gives `$FF8008` as `HD15`–`HD00` with the RD row all `0/1` and **the WR row entirely dashes** — the register has no writable bits. This is the same defect KB-39 fixed on the Main side; that fix did not carry across, so the Sub side kept a writable accessor for a read-only register. Now `ga_reg_ro`. Latent: nothing in the tree wrote it. Found by the DOC-25 sweep, and the reason the sweep exists — the defect is invisible from *The Hardware* alone if you only transcribe field positions and not the access rows. | ✅ |
| KB-42 | `lib/sub/gate_arr.def.h` | **Fixed 2026-08-14.** `STAMP_ROTATE_90` and `STAMP_ROTATE_180` were swapped. The stamp map entry's rotation field is bits 14–13, named `RT1` and `RT0`, so the extracted field value is `RT1 × 2 + RT0` — but *The Hardware* p.42 indexes its orientation figure by `RT0 × 2 + RT1`, the reverse. The transcription of the bit positions was correct (verified at 600 dpi against the Kodak scan: `HFLP`, then `RT` spanning two columns labelled `1 , 0`, then two zero bits, then `SNO A`–`SNO 0`); only the four angle constants were derived wrongly from it. `RT0=0, RT1=1` is 90°, which extracts as `0b10`, not `0b01`. Confirmed two ways: the maintainer read the p.42 glyph as P turned counter-clockwise, and the `RT0=1, RT1=0` cell independently reads as P through 180°. 0° and 270° were unaffected — they are palindromic in the two bits, which is why the error could not show up at those values. **Latent: the constants had no use site anywhere in the tree**, so nothing rendered wrongly; the first user to rotate a stamp by 90° would have got 180°. The same page settled that rotation is applied *before* `HFLIP` (each cell of the combined row is the horizontal mirror of the rotate-only cell), now documented in `docs/rotation_scaling.md` §2.2. | ✅ |
| KB-33 | `lib/memory.h` | **FIXED** 2026-08-13 — every `memset*`/`memcpy*` counted with a single `dbra`, which decrements only the low 16 bits. Lengths above 65536 elements silently truncated (a full 256 KB Word RAM copy is 262,144 bytes, well past it) and a length of 0 underflowed into ~65536 iterations, writing far outside the buffer. | ✅ |
| KB-13 | `lib/str_util.s:19` vs `lib/str_util.h:26` | **FIXED** 2026-08-13 — C wrote no terminator while assembly wrote `0xFF`. The Boot ROM print routines require 0xFF and treat 0x00 as a newline, so the assembly was right. Both now share `STRING_TERMINATOR` from `lib/str_util.def.h`. | ✅ |
| KB-14 | `megadev.make:30-48` | `MEGADEV_PATH` is not sanity-checked; unset yields `LIB_PATH=/lib` | ✅ |
| KB-15 | `megadev.make` | `build/`/`disc/` never created; `make init` required on a fresh clone and undocumented (violates B-1) | ✅ |
| KB-16 | `megadev.make` | No header dependency tracking; `make clean` required after every edit (violates B-2) | ✅ |
| KB-17 | `Doxyfile:18` | All 9 `.md` paths in `INPUT` are wrong (docs live in `docs/`); 3 named files (`bios.md`, `ip_sp.md`, `start_here.md`) exist nowhere; `*.s` is in `FILE_PATTERNS` with no `EXTENSION_MAPPING`, so **no assembly file produces any output**. | ✅ |
| KB-18 | `examples/pcm_playback/disc/audio.pcm` | 262 KB **required** disc payload is gitignored, so a fresh clone silently builds a broken ISO | ✅ |
| KB-19 | `docs/` | `bootrom.md` referenced 8×, `design.md` 2× — **neither file exists** | ✅ |
| KB-20 | `lib/main/memmap.h:35` | `#define exvec_vblank (...)z` — stray trailing `z`; any use is a syntax error | ⚠️ branch only |
| KB-21 | `lib/sub/bios.h` | `bios_drive_init()` calls **itself** with an argument; should call `bios_drive_init_ex` | ⚠️ branch only |
| KB-22 | `lib/sub/memmap.def.h` | `#define SP_INIT USERALL0` — typo for `USERCALL0` | ⚠️ branch only |

| KB-24 | `lib/sub/gate_arr.macro.s:34,47,48` | references `BIT_GA_REG_DMNA` / `BIT_GA_REG_RET`, which exist nowhere | ⚠️ branch only |
| KB-25 | `lib/main/comm.macro.s:23,26` | calls `Z80_DO_BUSREQ`/`Z80_DO_BUSRELEASE`; the macros are `Z80_REQUEST_BUS`/`Z80_RELEASE_BUS` | ⚠️ branch only |
| KB-26 | `lib/sub/boot.macro.s` | `.macro CDBOOT` whose body is `jsr CDBOOT` — invokes itself | ⚠️ branch only |
| KB-27 | 39 files on `feature/sub_bios_overhaul` | Rename `macros.s` → `macro.s` (commit `bd4d06c`) not propagated; `main.macro.s` and `sub.macro.s` deleted but still included. **The branch does not build.** | ⚠️ branch only |
| KB-28 | `lib/main/z80.h:78` | **FIXED** 2026-08-13 — now `static inline`. **Link-verified — found by the gate, missed by the audit.** `z80_init` is a non-`static` function definition in a header (INV-9); `multiple definition of 'z80_init'` across two TUs. Propagates to `lib/main/comm.h`. | ✅ |
| KB-29 | `lib/main/vdp.s:62,64,65,70,72` | **FIXED** 2026-08-13 — size suffixes, `btst`, and the `vdp_ctrl`/`VDP_CTRL` symbol. **Assemble-verified.** `move.w d1.w, d3.w` and similar — register size suffixes GNU `as` rejects. 5 errors. **This is on `master`, not branch-only as first recorded.** No project references this file, so it has never been assembled. | ✅ |
| KB-30 | `lib/str_util.s:69` | **FIXED** 2026-08-13 — `ATOI` removed; see BACKLOG FEAT-10. **Assemble-verified.** `.macro ATOI` is never closed with `.endm`: `Error: unexpected end of file in macro 'atoi' definition`. **The whole file therefore cannot assemble**, and no project references it. Also an INV-3 violation. | ✅ |
| KB-32 | `lib/main/vdp.h:290` | **FIXED** 2026-08-13 — `vdp_dma_transfer` was a non-`static` definition in a header (INV-9), propagating to `main.h` and `bios.h`. Found only after the Python port aligned the ODR check's flags with the real build. Now `static inline`. | ✅ |
| KB-31 | `lib/sub/commsync.s:38-53` | **FIXED** 2026-08-13 — file deleted. **Assemble-verified.** `.global _COMCMD0: .word 0` — a label definition cannot follow `.global` on one line; 16 errors. Already deleted on `feature/sub_bios_overhaul`. | ✅ |

**A structural observation from the first gate run:** KB-29, KB-30 and KB-31 are all in files that no
example or template references. The library contains assembly that has **never once been assembled**.
That is the strongest available argument for Tier 0.2 as a standing gate rather than a one-off audit.

That KB-20 … KB-27 exist *only* on the feature branch, and went unnoticed across five commits, is
the argument for §6 in one line.

---

## 9. Decision Record

Format: `Dn` = decided; `ODn` = open decision awaiting an owner.

### D1 — SPEC.md scope *(Damian R, 2026-08-13)*
SPEC.md holds specification, conventions, decisions and known-broken. The backlog lives separately in
`BACKLOG.md`. **Why:** SPEC.md should be stable and readable; a backlog churns.

### D2 — Backlog lives in a tracked `BACKLOG.md` only *(Damian R, 2026-08-13)*
No GitHub Issues. **Why:** in-repo, works offline, and survives the project's multi-month dormant
stretches. Trade-off accepted: no assignment, labels, or PR linkage.

### D3 — Breaking API changes are permitted, released as a major version *(Damian R, 2026-08-13)*
Fix the naming and layering properly and ship as **2.0.0** with a migration note. **Why:** small user
base; a rename is already half-landed; this is the cheapest moment.

### D4 — Verification Tier 0 + Tier 1 now; Tier 2 specified; Tier 3 as provenance *(Damian R, 2026-08-13)*
**Why:** Tier 0 alone would have caught every defect in §8.

### D5 — CI is GitHub Actions wrapping a local `make check` *(Damian R, 2026-08-13)*
The Action is a thin wrapper so the local and CI paths cannot drift.

### D6 — GitHub is the canonical remote *(Damian R, 2026-08-13)*
`git@github.com:drojaazu/megadev.git`. The private remote `git@cloud.motoi.pro:megadev.git` was
removed from this clone on 2026-08-13 after its **SSH host key changed** and connection was refused.
The key change was *not* accepted — it needs human verification before that remote is trusted again.
See BACKLOG.md OPS-1.

### D7 — Macro files use the singular suffix `.macro.s` *(Damian R, landed 2026-08-13)*
Nine files renamed and all 57 referencing files updated, along with include guards (`_MACROS_S` →
`_MACRO_S`) and `docs/manual.md`. The tree previously held a mix: `main.macro.s`, `sub.macro.s` and
`cdrom.macro.s` were already singular while the rest were plural.

The original attempt (`bd4d06c`, on `feature/sub_bios_overhaul`) left 39 files including a file that
no longer existed, and nothing caught it — the branch simply stopped building for five commits. The
same rename is now mechanical, because Tier 0.2 assembles every `.s` and a dangling `#include`
fails immediately.

### D8 — The verification gate is written in Python *(Damian R, 2026-08-13)*
Stdlib only, in `tools/check/`, replacing six bash scripts. **Why:** the compile/assemble drivers
were fine as shell, but the lint is text analysis, and shell grows unwieldy and esoteric as it does.
Python buys unit tests for the `.def.h` parser (25 of them) and code a maintainer can still read
after a months-long gap. `python3` is now explicit in the devcontainer.

### D9 — Mnemonic house style *(Damian R, 2026-08-13)*
Pure synonyms get one spelling (STYLE-1); GNU branch-improvement pseudo-ops are **preferred, not
forbidden** (STYLE-2). **Why:** the pseudo-ops choose the smallest working encoding, so treating
them as style violations would actively cost size and cycles. `dbra` chosen over `dbf` (20
occurrences rewritten) as it states the intent — decrement and branch always — rather than the
condition-code encoding.

### D10 — Branching model is gitflow; branch names use slash prefixes *(Damian R, 2026-08-13)*
`master` receives **only** release merges and hotfixes — never direct commits. `develop` is the
integration branch; all work reaches it through a branch and a `--no-ff` merge. Releases are cut as
`release/vX.Y.Z` from `develop`, merged to `master`, and tagged.

Branch names use slash prefixes: `feature/`, `fix/`, `docs/`, `release/`, `hotfix/`. **Why:** the
repo already used `release/v1.2.0`, most tooling groups slash-separated names into folders, and the
old mixed style (`feature_carts` alongside `release/v1.2.0`) made the set hard to read. Existing
branches were renamed on 2026-08-13.

**This decision was made in response to a violation:** the SPEC, backlog, verification gate and
defect fixes were initially committed straight to `master`, leaving `develop` stranded at v1.2.0 —
the exact inverse of the intended shape. Nothing had been pushed, so `develop` was moved to the work
and `master` reset to `v1.2.0`. The pre-restructure state is preserved as tag
`pre-gitflow-restructure-2026-08-13`.

### D11 — Fixed-point API and the `_t` naming rule *(Damian R, 2026-08-13)*
Types are `fix16` (Q10.6), `fix32` (Q16.16), `ufix16`, `ufix32`; literal macros are `FIX16()` /
`FIX32()`; operations are `fix16_mul` / `fix16_div` / `fix32_mul` and friends.

**No `_t` suffix.** Every `_t` name in Megadev is a C standard library type being re-provided for the
freestanding build (`size_t`, `ptrdiff_t`, `intptr_t`, `int8_t` …). There are **zero** Megadev-original
types using `_t`, so the suffix carries meaning: *this stands in for a libc type*. Megadev's own types
are short lowercase (`u8`, `ga_reg`, `vdp_addr`) or PascalCase structs (`Sprite`). `div_t` is not a
counter-example but a defect — see BACKLOG ARCH-11.

**fix16 is Q10.6, not the previous 8.8.** Signed 8.8 spans ±128 and so could not hold a horizontal
screen coordinate on a 320×224 display, which is why the 16-bit type was unused in practice. Q10.6
spans ±512. This matters because the 68000 has a 16×16→32 multiply but **no 32-bit multiply**, so
fix16 arithmetic is one `MULS.W` while fix32 needs partial products — the cheap type should be the
usable one.

**Each operation is defined once as a macro, with the inline function a thin wrapper.** A
`static inline` call is never an integer constant expression in C, so `_Static_assert` cannot check
it. Defining the arithmetic as a macro makes it assertable and usable in initialisers; wrapping it in
a function gives type checking and single evaluation. Because the function calls the macro, the two
cannot drift — the same trap that produced the C/asm `hextoa` divergence (KB-13).

Verified behaviour-preserving: `boot.bin`, `ip.bin`, `sp.bin` and `cyber.mmd` are **byte-identical**
before and after the migration.

### D12 — Register fields use `<SUBSYSTEM>_<FIELD>_<ASPECT>` *(Damian R, 2026-08-13)*
Every register field carries `_POS`, `_WIDTH` and `_MASK`, with `_MASK` derived via `FIELD_MASK` so
it cannot disagree with the other two. Field *values* are stored **unshifted** and placed with
`FIELD_PREP` / read with `FIELD_GET` (`build.def.h`, preprocessor-only so both languages can use
them). Supersedes OD-8.

**Why this ordering:** subsystem, then field, then aspect, so everything about one field sorts
together in an index, symbol list or autocomplete. This matches CMSIS, ARM's convention for register
definitions (`ADC_CR1_AWDCH_Pos` / `_Msk`), which is the closest thing to an industry standard here.

**Why `_POS` and not `_BIT`:** for a multi-bit field the low bit's index is the shift amount, which
is exactly what placing a value needs; for a single-bit flag the same number is the bit index for
`btst`. One concept covers both, so a flag is simply a field of width 1. Naming it `_BIT` only
describes the degenerate case — the question that prompted this.

**What it replaced:** six spellings coexisted — `GA_BIT_*`/`GA_MASK_*` (prefix, 40 uses),
`BIT_GA_REG_*` and `MSK_GA_REG_*` (Sub side), `*_MSK` (two), and `*_BIT` (suffix, added here on
2026-08-13). Converting them also surfaced two defects: `GA_MASK_CDCMODE_*` and `MSK_GA_REG_INT*`
were defined as `1 << x` **without parentheses**, and `VDP_MASK_INTERLACE_*` / `GA_MASK_CDC_DEST_*` /
`SCTRL_BAUD_*` were *values* misnamed as masks — `VDP_MASK_INTERLACE_NONE` was `0`, which no mask
can be.

### D15 — Gate array registers are numbered by 0-based word index *(Damian R, 2026-08-13)*
Documentation groups are `ga_reg_<side>_NN`, titled `Register NN (0xFF80NN) - Name`, with the
per-register group nested inside its functional group.

The numbering is an **SDK convention, not a hardware fact**: the Hardware Manual identifies these
registers by address and never numbers them. It is 0-based to match the existing `vdp_regNN` groups,
where the number genuinely is the hardware's register selector, so the two peripherals read alike.
The address appears in every title because the number is ours and the address is Sega's — which
removes the ambiguity that made this an open question.

An earlier note here claimed the manual settled this. It does not; it settles only what the hardware
is called, not how the SDK presents it.

### D16 — Field positions are register-relative; byte access is derived *(Damian R, 2026-08-13)*
`_POS` counts from the LSB of the whole register. A field in the high half of a 16-bit register has a
`_POS` of 8 or more, and its `_MASK` follows from that (INV-12).

Three definitions did the opposite, giving positions relative to the byte the field sits in:
`GA_LED_R`/`GA_LED_G` at 0 and 1, `GA_MEMMODE_WP` at 0, and the Main-side `GA_CDC_DEST` at 0. Under
D12 the mask is derived from the position, so each produced a mask naming the wrong bits —
`GA_MEMMODE_WP_MASK` came out `0x00FF`, the exact complement of the byte it protects. The
byte-relative reading was self-consistent only as long as nobody wrote the mask down.

The alternative — keeping positions byte-relative and adding a `_BYTE` suffix to say which half — was
rejected because it makes `_MASK` meaningless without knowing the suffix, and every `&` and `|`
against the whole register then has to be audited by hand.

The cost is that a `_POS` is no longer a legal bit-opcode operand on a memory byte. That is now
explicit rather than assumed: `FIELD_BYTE(reg, field)` picks the half and `FIELD_BPOS(field)` reduces
the position, both in `lib/build.def.h`. Renumbering *without* them would have been worse than the
original bug — `bset #8` on the register address is taken modulo 8, so it would have kept assembling
and quietly moved the fault from the mask to the opcode.

`GA_MEMMODE_BANK` was checked against the same page and was already register-relative at bits 6–7.

### D17 — Registers whose halves hold unrelated concerns are split into byte registers *(Damian R, 2026-08-14)*
Where the two bytes of a 16-bit gate array register carry unrelated concerns, each byte becomes a
named register in its own right, and `_POS` is relative to **that byte**. Registers that hold a
single 16-bit value, or a field spanning both halves, stay 16-bit.

This supersedes the uniform word-relative rule of **D16**, which stands only for the registers that
remain 16-bit.

**This is a modelling choice, not a hardware boundary.** The hardware register really is 16 bits
wide and the two halves really are adjacent and word-addressable. What the split reflects is that the
*fields* are byte-organised — no field in either header straddles bit 7/8, verified mechanically —
and that the two halves carry unrelated concerns, so there is no operation that legitimately spans
them. Modelling them as one word meant every access site converted back to a byte, which is all
`FIELD_BYTE` / `FIELD_BPOS` ever did, and it made a word-wide write the path of least resistance for
setting a field in one half — which silently clobbers the other. Under the split, `GA_LED_R_POS` is 0
again, as it was before D16, but now because it is bit 0 of `GA_REG_LED` rather than by accident.

Because it is a modelling choice, it is one an advanced user may deliberately step outside of. What
governs a direct word access is the hardware's own access-width and bit-operation rules, transcribed
in `docs/gate_array.md`; the official manual is the reference beyond that. Explaining this to users
is **DOC-20**.

The trap KB-34 turned on becomes structurally impossible rather than merely documented: bit 0 of the
old `GA_REG_RESET` was `RES0`, the peripheral reset, while bit 0 of the LED byte is the red LED. One
number, two unrelated bits. They are now fields of two different registers.

Word-wide access to a split pair is still possible — the halves are adjacent and the high byte is
even-aligned — but it is no longer the default spelling, so clearing the write protection while
setting the memory mode has to be written on purpose.

**Scope.** Split only where the halves differ. Word-only data registers (`$FF8008`, `$FF800A`,
`$FF800C`, `$FF8034`, and the address/size registers) keep a single 16-bit name; they have no fields
and permit no bit operations. `$FF8004` also stays 16-bit: `CA` is in the low byte and the CDC status
flags in the high byte, but they are read and written as one CDC transaction.

**Enforced by** Tier 1.5: every field of a byte register must satisfy `POS + WIDTH <= 8`, and
`FIELD_BPOS` must be the identity on it.

#### Renames (2.0.0 migration)

No name keeps its old meaning at a new address without changing, except `GA_REG_MEMMODE`, which is
called out below.

| Was | Now | Note |
|---|---|---|
| `GA_REG_RESET` (word, `$FF8000`) | *removed* | split into the two below |
| `GA_REG_RESET_HI` / `ga_reg_reset_hi` | `GA_REG_LED` / `ga_reg_led` | `$FF8000` |
| `GA_REG_RESET_LO` / `ga_reg_reset_lo` | `GA_REG_SUBCTRL` / `ga_reg_subctrl` | `$FF8001` |
| `GA_REG_MEMMODE_HI` / `ga_reg_memmode_hi` | `GA_REG_WP` / `ga_reg_wp` | `$FF8002`, read only from the Sub side |
| **`GA_REG_MEMMODE`** (word, `$FF8002`) | **`GA_REG_MEMMODE`** (byte, `$FF8003`) | ⚠ **same name, new address and width** |
| `GA_REG_MEMMODE_LO` / `ga_reg_memmode_lo` | `GA_REG_MEMMODE` / `ga_reg_memmode` | `$FF8003` |
| `GA_LED_R_POS` 8, `GA_LED_G_POS` 9 | 0, 1 | now relative to `GA_REG_LED` |

The Main side is split the same way, for symmetry (`$A12000`, `$A12002`, `$A12004`):

| Was | Now | Note |
|---|---|---|
| `GA_REG_RESET` (word, `$A12000`) | *removed* | split into the two below |
| `GA_REG_RESET_HI` / `ga_reg_reset_hi` | `GA_REG_INT2` / `ga_reg_int2` | `$A12000`, IEN2 and IFL2 |
| `GA_REG_RESET_LO` / `ga_reg_reset_lo` | `GA_REG_SUBCPU` / `ga_reg_subcpu` | `$A12001`, SBRQ and SRES |
| `GA_REG_MEMMODE_HI` / `ga_reg_memmode_hi` | `GA_REG_WP` / `ga_reg_wp` | `$A12002`, writable from this side |
| **`GA_REG_MEMMODE`** (word, `$A12002`) | **`GA_REG_MEMMODE`** (byte, `$A12003`) | ⚠ same name, new address and width |
| `GA_MEMMODE_WP_*` | `GA_WP_*`, `_POS` 8 → 0 | named for symmetry with the Sub side |
| `GA_CDC_DEST_POS` 8 | 0 | `$A12004` is now a single read-only byte register |

Two things the Main side made visible that the Sub side did not:

- `GA_RAISE_INT2_POS` and `GA_SUB_RESET_POS` were **both 0**. As fields of one 16-bit register that
  was a straight collision; they are IFL2 in the high byte and SRES in the low byte, and nothing in
  the source said so. They are now fields of two different registers and the collision is gone.
- `$A12004` has **no low byte at all** — every field is in the high byte and the Main CPU may only
  read it (manual p.58, the WR row is entirely `-`). It becomes one read-only byte register rather
  than a split pair, and gains `GA_CDC_EDT` and `GA_CDC_DSR`, which the Main side never defined.

The split changed **no emitted code** on the Main side: every rewritten access resolves to the same
address at the same width, verified by diffing all 44 built ELF and BIN artifacts before and after.

`GA_REG_MEMMODE` is the one carried-over name, because the low byte *is* the memory mode and the
write protect was the passenger. Out-of-tree code that used it as a word will now address one byte
further along. There is no diagnostic for this in assembly, so it is called out here and pinned by a
Tier 1.5 assertion on its address; in C the accessor changed from `u16` to `u8` and most uses will
warn or fail.

New fields that had no definitions before: `GA_PERIPH_RESET` (`RES0`), `GA_PRIORITY` (`PM0-1`, with
values), and `GA_WP`.

### D17a — A register with one unused byte becomes a byte register; the unused byte stays undefined *(Damian R, 2026-08-14)*
D17 covers registers whose two halves hold *unrelated* concerns. It says nothing about registers
where one half holds *nothing*. Three such registers exist: the Main side's `$A12004` (all fields in
the high byte) and the Sub side's `$FF8030` and `$FF8032` (all fields in the low byte).

The extension: **such a register is modelled as a byte register at the address of the byte that is
actually used.** So `$A12004` keeps its address, while `GA_REG_INT3TIMER` becomes `0xFF8031` and
`GA_REG_INTMASK` becomes `0xFF8033`.

**Why.** Without it, `move.b #n, GA_REG_INT3TIMER` writes the unused high byte and silently does
nothing — the exact class of bug D17 exists to remove, and the one the commented-out line in
`examples/gfx/src/sp.s:89` was working around by hand with `ga_reg_intmask+1`. With the extension,
the natural byte spelling is correct by construction.

**The alternative, rejected:** keep them 16-bit with positions 0–7. That is defensible — nothing is
*wrong* about it, and it avoids moving an address. It was rejected because it leaves byte access to
these registers a trap, and byte access is the only sensible way to use them.

**The unused byte gets no definition of its own.** The three orphaned bytes — `$FF8030`, `$FF8032`
and `$A12005` — are deliberately left undefined rather than given an `_UNUSED` symbol.

The case for defining them is a researcher who wants to poke an "unused" byte on real hardware to
confirm it really is inert. That case is real but rare, and someone doing it is equipped to write the
address literally. The case against is that what such a researcher actually needs is *knowledge* —
that the byte reads 0 and ignores writes — and a `#define` conveys an address while saying nothing
about behaviour. So the fact is recorded where it is useful, in each register's documentation, and
no symbol is created to invite casual use.

Discoverability is handled separately and already works: each register's `@defgroup` title carries
the **hardware** address (`Register 24 (0xFF8030) - Timer (INT3)`), so someone reading the manual and
grepping for `0xFF8030` still lands on the right register even though the definition is at
`0xFF8031`.

Addresses are pinned by Tier 1.5 assertions.

### D18 — CDD communication registers are named for their direction *(Damian R, 2026-08-14)*
`GA_REG_CDDCOMM0-9` becomes `GA_REG_CDDSTAT0-4` and `GA_REG_CDDCMD0-4`. The ten registers are not
one homogeneous block: the manual (p.33) shows `$FF8038`–`$FF8040` carrying Receiving Status 0–9 and
`$FF8042`–`$FF804A` carrying Transmission Command 0–9. A single `CDDCOMM` name hid that the halves
run in opposite directions.

The pair matches `GA_REG_COMSTAT` / `GA_REG_COMCMD`, used for Main↔Sub communication, and keeps the
same convention on both: **the command is what the controlling CPU sends, the status is what comes
back.** For Main↔Sub the controller is the Main CPU; for the CDD it is the Sub CPU.

Register *numbering* is unchanged — the doc groups stay `ga_reg_sub_28` through `ga_reg_sub_37`,
since D15 keys those to the address rather than to the name.

### D19 — Investigation tooling is not part of the kit *(Damian R, 2026-08-14)*
Megadev is a development kit. The repository ships what a **user of the kit** needs; tools we write
to test a hypothesis or settle a hardware question do not qualify, however useful they were.

The dividing line is the audience, not the language or location:

- **Tracked** — `tools/check/` (the verification gate, run by CI and by `make check`) and
  `tools/romfix.py` (invoked by the build). A user's build depends on these.
- **Not tracked** — anything whose only role was answering a question for us. The MAME gate array
  tracer (`gatrace.lua`) is the founding case: it settled HW-10, and once that question was closed
  it had no remaining purpose for a kit user.

Such tools live in `megadev-testing/` alongside the repo, not inside it. Captures and other
evidence go with them. Where SPEC or BACKLOG cites that evidence, it is cited by its
`megadev-testing/` path with a note that it is outside the repo — the finding stays in the record
even though the apparatus does not.

Applied retroactively on 2026-08-14: `tools/trace/` was removed from history rather than deleted at
the tip, because its Sonic CD capture was 4 MB and would otherwise sit in every future clone. Four
commits existed only to add or amend it and were dropped.

### D20 — The module jump table is removed; resident APIs use `ld -R` *(Damian R, 2026-08-15)*
A resident module used to be able to publish a jump table at a fixed offset, reserved by a
`.jmptbl` / `.shared` block in each of the four module linker scripts, so that transient modules
could call into it without linking against it.

This was superseded by importing the resident module's symbols directly with `ld -R`, which needs
no fixed offsets and no hand-maintained table. The remaining obstacle — that the importing module
would see the resident module's `MODULE_ROM_ORIGIN` and friends collide with its own — is solved by
naming a resident module's layout symbols `RESIDENT_*` (`cfg/module_resident_*.ld`,
`megadev.make:198`). That is the documented and implemented mechanism (`docs/modules.md`).

The linker script blocks survived the change and were dead: nothing emitted `.jmptbl` or `.shared`,
no layout defined `MODULE_JMPTBL_SIZE` / `RESIDENT_JMPTBL_SIZE` / `*_SHARED_SIZE`, so both
`DEFINED()` guards always took the no-op branch. They are now removed from all four scripts. The
comment describing them was worse than the code, since it pointed at `docs/modules.md`, which
documents the replacement instead.

### D21 — A module's layout is declared in C, not a separate asm file *(Damian R, 2026-08-15)*
Each module used to need a `*_layout.s` file whose only job was to call the `GLOBAL` macro a few
times, because it was believed C could not define an absolute global symbol. It can: file-scope
basic `asm(".global X\n.equ X, value")`, wrapped as `GLOBAL_SYM` in `lib/macro.h`.

The symbol produced is identical — absolute, global, no storage in `.rodata`, `.data`, `.bss` or
anywhere else — so the linker cannot distinguish the two forms. Both spellings remain valid; the
asm `GLOBAL` macro is unchanged for projects written in assembly.

Placement follows the layout's scope: a layout belonging to one module goes at the top of that
module's C file, and a layout shared by several modules gets its own source file that each links
against (`new_project/src/shared_mmd_layout.c`, shared by ex1/ex2/ex3).

All eight `*_layout.s` files across the examples and the template were converted. Three of them
(`mode7`, `transforms`, and the template's copy included by `ip.s`) turned out to be dead: an `ip.s`
including a layout never used its symbols, since the `_BSS_*` symbols it does use come from
`cfg/ip.ld`. Those includes were removed too.

**Verification.** Every program binary — `ip.bin`, `sp.bin`, `boot.bin`, and every `.mmd` / `.smd` —
is byte-identical before and after, across all seven examples plus the template, including the
template's resident-module case with `ld -R` imports. Note that the `.iso` is **not** a valid
comparison target: ISO9660 volume and directory records embed a creation timestamp, so two builds
of identical sources differ in 19 bytes at 0x80B3, 0x8339-0x836E and 0xB817-0xB886. Compare the
constituent artifacts instead.

### D22 — a6 is reserved with `-ffixed-a6`; never name it in a clobber list *(Damian R, 2026-08-15)*
a6 is the m68k frame pointer register, and the Boot ROM treats it as scratch — `bios.def.h` documents
`@clobber ...a6` on a dozen routines, and `lib/main/vdp.s` and `lib/sub/pcm.s` clobber it too.

Naming a6 in an `asm` clobber list does not protect it. The clobber marks the register used, which
sets `frame_pointer_needed`; GCC then installs a6 as the frame pointer, and a frame pointer is fixed
and cannot be clobbered, so the clobber is discarded. Verified with GCC 14.2: under
`-fomit-frame-pointer` the compiler emits `link.w %fp`, spills a local to `-4(%fp)`, runs the asm
that declares it destroys a6, then reads `-4(%fp)` back. Under `-fno-omit-frame-pointer` it instead
errors, "%a6 cannot be used in `asm` here". This is the mechanism behind the long-standing
observation that a6 was "getting mangled even though it was in the clobber list".

`-ffixed-a6` is now in `CC_FLAGS` (`megadev.make:113`). GCC places no value in a6, so nothing needs
protecting, and the 11 `move.l a6,-(sp)` / `move.l (sp)+,a6` wrapper pairs in `lib/main/bios.h` plus
2 in `lib/sub/pcm.h` were removed, along with the `"a6"` clobber on `bios_clear_comm`. The explicit
`register u32 A6 asm("a6")` parameter binding in `bios_detect_controller` is unaffected and still
works.

**Measured.** Total code size across all example projects fell 62 bytes; the largest single change
was −28 (`bram`) and the only increase was +4 (`bram/spx.smd`). References to a6 in compiled C
objects fell from 56 to 2, and both survivors are switch jump-table data that objdump renders as
instructions. Each removed wrapper also drops 2 instructions and 4 bytes from every expansion of the
`static inline` that contained it, which is why hot calls like `bios_dma_xfer` get cheaper.

**The a6-is-scratch assumption, and how it was checked.** Removing the wrappers means a C function
that calls the BIOS now returns with a6 clobbered. That is safe for C callers (none hold a value in
a6) but assumes no *asm* caller does either — including the Boot ROM when it calls back into a user
routine such as `bios_vblank_user`. The documentary support is that the Boot ROM's own VBLANK path
calls `BIOS_READ_JOYPAD`, documented `@clobber d6-d7/a5-a6`, so the Boot ROM cannot itself hold a
live value in a6 across that path.

Confirmed at runtime in MAME on 2026-08-15 (provenance EMU): `gfx`, `bram` and `pcm_playback` all
ran correctly. `gfx` is the load-bearing case, since it installs `vblank_user` through
`bios_vblank_user` and calls `bios_copy_sprlist()` from inside that callback — the exact Boot ROM →
C path the assumption covers. `pcm_playback` covers the separate `lib/sub/pcm.h` clobber site, and
`bram` exercises the main-side wrappers heavily.

EMU is strong evidence for this particular class of change. Whether a routine keeps a value live in
a register across a call is a property of the Boot ROM's instruction stream, and the emulator
executes that real ROM image on an accurate 68000 core; unlike CDC timing or PCM output, there is no
analog or timing behaviour for the emulator to approximate. The one residual is Boot ROM revision
variance — the dumps under `BIOS Revisions/` are not all byte-identical, and this was exercised
against one of them.

Still unexercised: `bios_detect_controller`. No example calls it, so the `%c0` → `%c1` fix above is
confirmed only to compile, never to run. It is also the one function that passes a parameter *in*
a6, which makes it the single place where reserving a6 could interact with an explicit binding.

Also fixed here: `bios_detect_controller` referenced `%c0` where it needed `%c1`. With an output
operand present the immediate is operand 1, so the function emitted `jsr %d6` and failed to
assemble. It is `static inline` and unused by any example, so it had never been instantiated — which
is a gap in the gate: nothing forces a header's inline functions to be compiled. Every other asm
block in the file with an output operand correctly uses `%p1`/`%c1`.

### OD-1 — How to resolve the Main/Sub Gate Array namespace collision *(open)*
INV-7 is violated (KB-12). Options: prefix by CPU side (`GA_MAIN_*` / `GA_SUB_*`); rely solely on
path-derived include guards plus a hard rule that a TU may include only one side; or generate both
from one source with a base-address parameter. **The fact that would settle it:** whether any real
project needs both views in one translation unit — Mode 1 (OD-2) is the case that would force it.
Not settled unilaterally; affects the 2.0.0 API.

### OD-2 — Is Mode 1 a supported target? *(resolved 2026-08-15 → D23)*
Resolved: yes. See D23.

### D23 — Mode 1 is a supported target *(Damian R, 2026-08-15)*
Mega CD hardware driven from a cartridge is a target the kit intends to support. This was previously
undefined; `48167ff` had removed the Mode 1 example for "no real progress", and four parallel
branches suggested repeated unfinished attempts.

**This unblocks OD-1.** That decision was waiting on whether any real project needs both the Main and
Sub Gate Array views in one translation unit. Mode 1 is that case, so the namespace collision (KB-12,
INV-7) must now be resolved rather than deferred, and it affects the 2.0.0 API.

**Prior art, and it is fragile.** The removed example survives only as
`examples/cart_mode1/` on the local branch `feature/md_cart` — a makefile, `init.s`, `main.c`,
`res.s`, `example_rom.cart.def` and resources. That branch is dated 2024-01-29, has 21 commits not
present in `develop`, exists on no remote, and is written against the pre-`megadev.make` build
system (it still carries `makefile.global`). It is the only surviving Mode 1 example and must not be
deleted. Mega Drive cartridge support was originally built *as a prerequisite for Mode 1*, not as a
feature in its own right, which is why that work is entangled with the `cart` branches.

Plain Mega Drive ROM output, by contrast, is complete in `develop`: `cfg/md_cart.ld`,
`lib/md_header.s`, `lib/md_init.s`, `lib/md_vectors.s`, and the `cart` example, which builds an 8042
byte ROM and passes the gate.

**What remains open** is implementation, not intent:
- Emulator support is unverified. Mode 1 may not be emulated at all; ares and BlastEm are the
  candidates worth testing first on accuracy grounds. If none support it, verification is
  hardware-only, which raises the cost of every iteration.
- The OD-1 namespace resolution now becomes a prerequisite rather than a parallel question.
- The surviving example needs porting from `makefile.global` to `megadev.make` before it can even be
  built and observed.

### D13 — Include guards use `#pragma once` *(Damian R, 2026-08-13)*
All 56 headers converted; no `#ifndef` guards remain. Adoption had already begun by hand.

Verified before converting that `#pragma once` behaves correctly when a `.def.h` is pulled in by the
assembler (`gcc -x assembler-with-cpp`), including a doubled include — that mattered because the
`.def.h` layer is consumed by both languages.

The lint now requires it rather than checking a naming scheme, which removed 11 of the 13 remaining
baseline entries at a stroke. The other two were then fixed, so **the convention baseline is empty**:
every rule in §2–§3 holds across the whole library, and any new violation fails the gate outright.

### D14 — Register accessors are lvalues *(Damian R, 2026-08-13)*
`#define ga_reg_foo (*((ga_reg) GA_REG_FOO))`, used as `ga_reg_foo = x`.

Both forms were in use — `lib/sub/gate_arr.h` mixed them internally while `lib/main/vdp.h` used the
opposite convention throughout — so a caller could not tell from the name whether a `*` was needed.

**Decided on measurement, not preference.** Of 72 pointer-form accessors in the tree, exactly **two
uses** wanted the pointer rather than the value (`read_input_joypad(io_data1)`), and `&` recovers the
address in those cases. The lvalue form therefore charges the sigil to the rare case instead of the
common one. An earlier draft of this decision recommended the pointer form on the grounds that it
composed with pointer arithmetic; that was backwards — `ga_reg_foo + 1` silently meaning "the next
register" is a hazard, and explicit `_LO` definitions say it better.

Scope: **single registers**, identified by the register typedefs. Memory *regions* — `word_ram`,
`prg_ram` — are genuinely pointers to many objects and are indexed as such, so they are unaffected.

Enforced by INV-10, red-tested. Verified behaviour-preserving: `boot.bin`, `cyber.mmd` and the ISO
are byte-identical before and after the conversion.

### OD-5 — Should the audit's branch-only defects be fixed on the branch or after merge? *(open)*
KB-20 … KB-27 exist only on `feature/sub_bios_overhaul`. Fixing them there keeps the branch
self-consistent; deferring keeps the branch's diff focused on documentation.

---

## 10. Change Discipline

Adopted going forward, in response to 95 of 191 commits (49.7%) being titled `Checkpoint!`:

- **Commit messages state what changed, not why.** A subject line and, where a change spans several
  things, a few short bullets. The reasoning belongs in the decision record (§9) or the backlog
  entry, and the subject should name the relevant ID — `Register accessors become lvalues (D14,
  ARCH-4)` — so the two are linked without being duplicated. `Checkpoint!` is not a commit message;
  if a checkpoint is genuinely needed mid-work, say what state it captures.
- One concern per branch — a refactor and a logic change do not share a commit.
- Every release gets a tag **and** an entry in the changelog.
- Every item in §8 has an ID; when it is fixed, the fixing commit references that ID.

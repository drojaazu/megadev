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
| Mode 1 (Mega CD hardware driven from a cartridge) | **Undefined.** Commit `48167ff` removed the Mode 1 example as having "no real progress". See §9 OD-2. |
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
- **INV-4** — Every header MUST have an include guard whose name is derived from its path
  (`MEGADEV__<PATH>_<NAME>_<SUFFIX>`), so that the Main and Sub views of the same peripheral cannot
  collide. Choice of `#pragma once` vs `#ifndef` is settled in §9 OD-3.
- **INV-5** — A `@file` doc tag MUST name the file it appears in.
- **INV-6** — Where a constant is used as an operand to a bit-level opcode (`btst`, `bset`, `bclr`,
  `bchg`), a companion giving the **bit index** MUST exist alongside the mask. Masks and indices are
  not interchangeable, and conflating them is a silent wrong-bit bug rather than a compile error.
  *(See KB-11.)*

  This is **long-standing project policy**, documented in `docs/manual.md` §"Bitwise Definition
  Naming", not a rule invented here. The naming scheme is settled in §9 **D12**: `_POS`, `_WIDTH`
  and `_MASK`, with values stored unshifted and placed by `FIELD_PREP`.
- **INV-8** — Every header MUST be self-contained: including it, and nothing else, into an empty
  translation unit must compile cleanly. Enforced by Tier 0.1 (§6).
- **INV-9** — A header MUST NOT define storage or a non-`static` function. Two translation units
  including it must link. Enforced by Tier 0.4 (§6). **Holds as of 2026-08-13** (42 pass, 0 fail).
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
- **B-3** — Builds are reproducible. **Partly.** The date is now resolved once into `BUILD_DATE`
  rather than re-forked per compile, and honours `SOURCE_DATE_EPOCH`. The ISO itself is still not
  byte-reproducible because `mkisofs` embeds its own timestamps — see BACKLOG MAKE-6.
- **B-4** — `make -j` is safe. **Currently false**: the ISO's disc-file prerequisites come from
  `$(shell find ...)` evaluated at parse time (`megadev.make:136-137`), so on a first build the
  not-yet-existing modules are not prerequisites of the ISO.
- **B-5** — Two sources with the same basename in different directories do not collide. **Still
  false**: object names are `$(notdir)`-flattened into one `build/` directory. Now documented as a
  limitation in `docs/modules.md` rather than being a silent trap. See BACKLOG MAKE-4.

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
| **0 — Build gate** | The toolchain accepts the source. | **Implemented and GREEN** (m68k gcc 14.2.0): headers 70, assembly 23 (2 excluded by contract), ODR 42, all 7 projects build. |
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

> **Two independent translations exist** for several Mega-CD manuals — credited *(Rex Sabio)* and
> *(The Code Monkeys)*. The official English is in places ambiguous or plainly wrong, so where a
> claim rests on wording, **check both translations before recording it as `DOC`**; a disagreement
> between them is itself a signal to downgrade the claim to `ASSUMED` pending hardware testing.

| Claim | Provenance | Source / note |
|---|---|---|
| Gate Array register map, Sub side | DOC | Sega BIOS manual + community research |
| Gate Array register map, Main side | DOC | as above |
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
| KB-13 | `lib/str_util.s:19` vs `lib/str_util.h:26` | `hextoa8/16/32`: assembly writes an `0xFF` terminator, C writes **no terminator**. Same name, same documented contract, different behaviour. | ✅ |
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

### D7 — Macro files use the singular suffix `.macro.s` *(Damian R, in flight)*
Rename begun in commit `bd4d06c` on `feature/sub_bios_overhaul`. `docs/manual.md:342` still documents
the old plural `.macros.s`, which is correct for `master` and wrong for the branch. The rename is
incomplete (KB-27) and must land atomically with its consumers.

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

### OD-1 — How to resolve the Main/Sub Gate Array namespace collision *(open)*
INV-7 is violated (KB-12). Options: prefix by CPU side (`GA_MAIN_*` / `GA_SUB_*`); rely solely on
path-derived include guards plus a hard rule that a TU may include only one side; or generate both
from one source with a base-address parameter. **The fact that would settle it:** whether any real
project needs both views in one translation unit — Mode 1 (OD-2) is the case that would force it.
Not settled unilaterally; affects the 2.0.0 API.

### OD-2 — Is Mode 1 a supported target? *(open)*
Currently undefined. `48167ff` removed the example. Four parallel abandoned branches
(`md_cart`, `md_cart_dev`, `feature_carts`, `origin/md_cart`) suggest repeated unfinished attempts.
This decision gates OD-1.

### OD-3 — Include-guard style: `#pragma once` or `#ifndef` *(open)*
Currently split: most of `lib/sub/` uses `#pragma once`, everything else uses `#ifndef`, and `sub/`
is not internally consistent. `#ifndef` is what INV-4's path-derived naming needs in order to be
lint-checkable and to make collisions visible; `#pragma once` is terser. Pick one and enforce it.

### OD-4 — Register-access macro form: pointer or lvalue *(open)*
`lib/sub/gate_arr.h` has both — `ga_reg_stopwatch` is a pointer, `ga_reg_stampsize` (same file) is a
dereferenced lvalue. `main/vdp.h` is all-lvalue; `main/io.h` all-pointer. A caller cannot predict
whether `foo` or `*foo` is correct. Pick one for 2.0.0.

### OD-5 — Should the audit's branch-only defects be fixed on the branch or after merge? *(open)*
KB-20 … KB-27 exist only on `feature/sub_bios_overhaul`. Fixing them there keeps the branch
self-consistent; deferring keeps the branch's diff focused on documentation.

---

## 10. Change Discipline

Adopted going forward, in response to 95 of 191 commits (49.7%) being titled `Checkpoint!`:

- Commit messages state what changed and why. `Checkpoint!` is not a commit message; if a checkpoint
  is genuinely needed mid-work, say what state it captures.
- One concern per branch — a refactor and a logic change do not share a commit.
- Every release gets a tag **and** an entry in the changelog.
- Every item in §8 has an ID; when it is fixed, the fixing commit references that ID.

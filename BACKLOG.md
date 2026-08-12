# MEGADEV — Backlog

Single consolidated list of open work. Per [SPEC.md](SPEC.md) D2 this file *is* the tracker — there
is no external issue tracker.

**Conventions**
- Every item has a stable ID. Once assigned, an ID is never reused, even after the item is closed.
- Commits that close an item reference its ID (e.g. `Fix divu/div remainder (LIB-3, KB-3)`).
- `KB-n` IDs refer to verified defects catalogued in [SPEC.md §8](SPEC.md#8-known-broken); they are
  listed here by reference rather than duplicated.
- Severity: **S1** breaks the build or produces wrong results · **S2** real defect with a workaround
  · **S3** friction, inconsistency, hygiene · **S4** nice to have.
- Status: `open` · `in-progress` · `blocked` · `done`

Last swept: 2026-08-13. Sources: inline `TODO` markers (26 on `master`), prose in `docs/`, commit
messages, and the 2026-08-13 audit.

## Work in flight

| Branch | Base | Contains | State |
|---|---|---|---|
| `docs/spec-and-backlog` | `master` | SPEC.md, BACKLOG.md, README index, `docs/html/` ignore | ready to merge |
| `feat/verification-gate` | `master` | `make check`, `tools/check/*`, GitHub Actions workflow | lint verified; build gate needs a real toolchain run |
| `fix/doxygen-config` | `master` | Doxyfile repair, two doc-tag typos | verified with doxygen 1.16.1 |
| `fix/doc-cross-references` | `master` | `bootrom.md`/`design.md` link repair | verified |
| `feature_sub_bios_overhaul` | `master` | in-flight inline-documentation overhaul | **does not build** (BR-1); resume after the above land |
| `backup/feature_sub_bios_overhaul-2026-08-13` | — | backup pointer, plus tag `backup-sub-bios-overhaul-2026-08-13` | do not delete until the branch is pushed |

None of these branches has been pushed to any remote. Offline backups (bundle, patch, original
SPEC.md stub) are in `/mnt/motoi-s2/dev/megadev-backups/`.

---

## Correctness — verified defects

These are specified in [SPEC.md §8](SPEC.md#8-known-broken). **None are compile-verified** — no m68k
toolchain was available during the audit. VER-1 must land first so that fixes can be proven.

| ID | Sev | Status | Item |
|---|---|---|---|
| LIB-1 | S1 | open | KB-1, KB-2 — `mov.l` is not an M68k mnemonic. `lib/init.macros.s:27` is in `BASIC_INIT`, the startup path every module runs; `lib/math.h:75,77,103,105`. |
| LIB-2 | S1 | open | KB-3 — `divu()`/`div()` assign `out.quot` twice and never set `out.rem`. `lib/math.h:46-47,59-60`. |
| LIB-3 | S1 | open | KB-4 — `div()` documented as signed but emits `divu.w`. `lib/math.h:58`. |
| LIB-4 | S1 | open | KB-8 — `int_to_f32` casts a value shifted left by 16 to `short`; always yields 0. `lib/fixed.h:34`. |
| LIB-5 | S1 | open | KB-5 — non-static `strcpy` **definition** in `lib/memory.h:120`; multiple-definition link error. |
| LIB-6 | S1 | open | KB-6 — tentative definitions `bram_work_buff`/`bram_string_buff` in `lib/sub/bram.h:15,16`. |
| LIB-7 | S1 | open | KB-7 — `EXVECEXVEC_TRACE` botched find-and-replace, `lib/main/cd_exception.s:79`. |
| LIB-8 | S1 | open | KB-10 — illegal cast-to-array-type, `lib/main/bios.h:108`. |
| LIB-9 | S1 | open | KB-9 — `time_mapping` is not valid C and has zero references; delete. `lib/main/io.h:94`. |
| LIB-10 | S1 | open | KB-11 — `btst` given a mask instead of a bit index, in both the C and asm copies. Fix requires adding `_BIT` companions to `lib/main/io.def.h` (INV-6). |
| LIB-11 | S2 | open | KB-13 — `hextoa8/16/32` C and asm versions disagree on string termination. Decide the contract, then make both match; first subject for VER-3. |
| LIB-12 | S2 | open | `lib/memory.h` — every `memset*`/`memcpy*` uses a `dbf` loop with a **16-bit** counter. Lengths > 65536 silently truncate; length 0 wraps and loops 65536 times. Undocumented. Document or guard. |
| BR-1 | S1 | open | KB-27 — the `macros.s` → `macro.s` rename is unpropagated across 39 files; **`feature_sub_bios_overhaul` does not build.** Must land atomically with its consumers. |
| BR-2 | S1 | open | KB-20 … KB-26 — seven defects that exist only on `feature_sub_bios_overhaul`. See SPEC.md OD-5 for whether to fix on-branch or after merge. |
| BR-3 | S2 | open | `examples/bram/src/bram_demo_init.s:27` includes `<init_data.s>`, which exists at **no ref** in the repo. Determine intent; restore or remove. |

## Architecture

| ID | Sev | Status | Item |
|---|---|---|---|
| ARCH-1 | S1 | open | KB-12 / INV-7 — Main and Sub Gate Array headers define the same macro names with different values and non-matching guards. Blocked on SPEC.md **OD-1**. |
| ARCH-2 | S3 | blocked | Decide whether Mode 1 is a supported target (SPEC.md **OD-2**). Gates ARCH-1. Four abandoned branches exist (`md_cart`, `md_cart_dev`, `feature_carts`, `origin/md_cart`); commit `48167ff` removed the example. |
| ARCH-3 | S3 | open | Settle include-guard style (SPEC.md **OD-3**), then enforce via VER-2. |
| ARCH-4 | S3 | open | Settle pointer-vs-lvalue register-access macro form (SPEC.md **OD-4**). Affects 2.0.0. |
| ARCH-5 | S3 | open | Umbrella headers incomplete: `main.h` omits `bios.h`, `comm.h`, `md_sys.h`, `mmd.h`; `sub.h` omits `bios.h`. |
| ARCH-6 | S3 | open | INV-3 violation: `lib/str_util.s:69` defines `.macro ATOI` inside a code-emitting `.s`; the macro ends on a bare label with no `rts` and is unusable. Move or delete. |
| ARCH-7 | S3 | open | INV-2 violation: `lib/main/vdp.macros.s` has no `#include` at all and relies on caller include order. |
| ARCH-8 | S3 | open | Remove libc-shadowing names with non-libc semantics: `strcmp`→`bool` (`lib/str_util.h:68`), `strcpy`→`void` (`lib/memory.h:120`), `abs`/`abs16` (`lib/math.h`). Part of 2.0.0. |
| ARCH-9 | S3 | open | `lib/sub/pcm.def.h:11-19` uses reserved leading-underscore identifiers (`_PCM_ENV` …). Also `pcm.h` uniquely uses SCREAMING_CASE C macros, a `_c` function suffix, and camelCase struct fields. |
| ARCH-10 | S4 | open | Unprefixed global enumerators in `lib/sub/bram.h`: `NoRam`, `Unformatted`, `OtherFormat`, `SegaFormatted`, `NoMatch`, `FileNotFound`, `Match`. |

## Verification & CI

| ID | Sev | Status | Item |
|---|---|---|---|
| VER-1 | S1 | in-progress | **Tier 0 build gate** (SPEC.md §6): per-header compile, assemble every `.s`/`.macro.s`, full build of all examples + `new_project`. Prerequisite for proving every LIB-* fix. Requires classifying each header Main-valid / Sub-valid / both. |
| VER-2 | S2 | in-progress | **Tier 1 convention lint**: INV-1 (`.def.h` is `#define`-only), INV-4 (guard name matches path), INV-5 (`@file` matches filename), `clang-format --dry-run --Werror`, Doxygen `WARN_AS_ERROR`. Blocked on DOC-1 for the Doxygen part. |
| VER-3 | S2 | open | **Tier 2 on-target tests**: emulator harness + differential C-vs-asm tests over `math.h`, `fixed.h`, `memory.h`, `str_util.*`. Choose BlastEm or Genesis Plus GX; result protocol via RAM byte or the existing `comm.h` serial channel. |
| VER-4 | S3 | in-progress | GitHub Actions workflow wrapping `make check` (SPEC.md D5). |
| VER-5 | S3 | open | Pin the toolchain. `.devcontainer/Dockerfile` pins only `debian:13-slim` (mutable tag) and `clang-format-19`; gcc/binutils are whatever Debian ships. `etc/Dockerfile_alpine` *does* pin (`binutils-2.45`, `gcc-15.2.0`) — reconcile the two. |

## Build system

| ID | Sev | Status | Item |
|---|---|---|---|
| MAKE-1 | S1 | open | KB-16 / B-2 — no header dependency tracking; users must `make clean` after every edit. Documented as a limitation in `docs/modules.md:100`. Highest-friction item for end users. |
| MAKE-2 | S2 | open | KB-15 / B-1 — `build/` and `disc/` are never created; `make init` is required on a fresh clone and documented nowhere. |
| MAKE-3 | S2 | open | KB-14 — `MEGADEV_PATH` is not sanity-checked; unset silently yields `LIB_PATH=/lib`. |
| MAKE-4 | S2 | open | B-5 — object names are `$(notdir)`-flattened into one `build/`; `lib/main/gate_arr.macros.s` and a future `lib/sub/gate_arr.macro.s` collide silently. |
| MAKE-5 | S2 | open | B-4 — `make -j` unsafe: ISO prerequisites come from `$(shell find)` evaluated at parse time (`megadev.make:136-137`). |
| MAKE-6 | S3 | open | B-3 — non-reproducible builds: `$(shell date)` in `HEADER_COPYRIGHT` (`megadev.make:61`), re-forked on every compile because `CC_FLAGS` is recursively expanded. |
| MAKE-7 | S3 | open | `objcopy -O binary $@` with one argument does an in-place conversion (`megadev.make:231`); an interrupted build leaves an ELF named `boot.bin`. |
| MAKE-8 | S3 | open | `mkisofs` is hardcoded, unlike every other tool, and is provided by three different packages across distros. Make it a variable. |
| MAKE-9 | S3 | open | Every recipe is `@`-prefixed and `clean` redirects to `/dev/null`; failures produce no diagnosable output. |
| MAKE-10 | S3 | open | `.SECONDARY: $(BUILD_PATH)/*` (`megadev.make:147`) expands at parse time, so its meaning differs between a clean and an incremental build. |
| MAKE-11 | S4 | open | Dead code: `TOOLS_PATH` (points at a non-existent `tools/`), `AS`, `Z80_AS`, commented-out rule at 160-164, debug `echo` at 201. |
| MAKE-12 | S4 | open | `megadev.make:235` — make the ISO settings user-configurable. *(inline TODO)* |
| MAKE-13 | S3 | open | Orphan linker scripts: `cfg/module_mmd_newwork.ld` and `cfg/module_bin.ld` are referenced by no rule. Wire up or delete. `cfg/md_cart.ld` also uses a different symbol-naming convention (`_text_org` vs `_TEXT_ORIGIN`) from the others. |
| MAKE-14 | S2 | open | KB-18 — `examples/pcm_playback/disc/audio.pcm` (262 KB) is required at runtime but gitignored; a fresh clone builds a broken ISO. Narrow the `disc/` ignore so payload sources are tracked. |

## Documentation

| ID | Sev | Status | Item |
|---|---|---|---|
| DOC-1 | S2 | **done** | KB-17 — `Doxyfile` is broken three ways: all 9 `.md` `INPUT` paths are wrong (docs live in `docs/`), 3 named files exist nowhere (`bios.md`, `ip_sp.md`, `start_here.md`), and `*.s` has no `EXTENSION_MAPPING` so **no assembly file produces output**. Also `PROJECT_ID` is not a valid tag (should be `PROJECT_NAME`) and `PROJECT_NUMBER = 1` contradicts `VERSION`. Blocks VER-2. |
| DOC-2 | S2 | **done** | `@alias` is used 49 times but never declared in `Doxyfile` `ALIASES`; `@macro`, `@in`, `@out`, `@desc` are used throughout `.macro.s` files and are not Doxygen commands at all. |
| DOC-3 | S2 | **done** | KB-19 — `bootrom.md` is referenced 8× and `design.md` 2×; **neither exists**. Pre-rename ghosts of `main_bios.md` / `program_design.md`. |
| DOC-4 | S2 | open | `docs/gate_array.md` **ends mid-sentence** ("Some registers (namely the CD Fader control ") with no trailing newline; a second sentence dangles at line 43. |
| DOC-5 | S3 | open | `docs/main_bios.md` has ~39 headings with no body, including all of System Group, Input Reads, Decompression, and 9 of 10 Unknown Functions. Its table of contents lists 3 entries for ~15 sections. |
| DOC-6 | S3 | open | `docs/megacd_dev.md` and `docs/program_design.md` are ~65% the same document, including identical ASCII memory maps and identical typos. Merge or clearly split by purpose. |
| DOC-7 | S3 | open | `docs/manual.md` Further Reading omits `gate_array.md` and `program_design.md` — 2 of 10 docs are unreachable from the manual. |
| DOC-8 | S3 | open | No prose documentation exists for `sub/pcm.*`, `sub/bram.*`, `sub/bios.*`, `main/vdp.*`, `main/io.*`, `main/z80.*`, `main/comm.*`, `main/bramcart.def.h`, or the `lib/` utility headers — roughly 45 of 72 library files. |
| DOC-9 | S4 | open | `docs/manual.md` typos preserved across releases: "Masimum" (×2), "apepar", "relatve" (×3), "reccommend", "supporttools". |
| DOC-10 | S3 | open | ~29 `@file` / `@def` / `@fn` tags name the wrong file or symbol (e.g. `lib/main/md_sys.h:4-7` is a copy-paste of `mmd.h`'s header block). Enforced by INV-5 once VER-2 lands. |
| DOC-11 | S3 | open | Write a `CHANGELOG.md`. 13 release tags exist with no release notes anywhere. |
| DOC-12 | S3 | open | Write a `CONTRIBUTING.md`. SPEC.md actively solicits contributions and `docs/dev_in_c.md:85` asks for help with no channel to receive it. |

## Hygiene & repo operations

| ID | Sev | Status | Item |
|---|---|---|---|
| OPS-1 | S2 | **needs owner** | **The SSH host key for `cloud.motoi.pro` has changed.** Connection was refused on 2026-08-13 (`SHA256:0QUYZWVWxGjdB3k65UVa8/3bKx0zopnl7iKL2PPx0n8`, offending entry `~/.ssh/known_hosts:8`). The key was **not** accepted. The remote was removed from this clone per SPEC.md D6. Verify the server was legitimately rebuilt before trusting it again; re-add with `git remote add origin git@cloud.motoi.pro:megadev.git`. |
| OPS-2 | S3 | **done** | Add `docs/html/` to `.gitignore` — 385 generated files sit permanently in `git status`, one `git add .` from being committed. Commit `5013d70` removed the earlier exclusion. |
| OPS-3 | S3 | open | Prune stale branches: `temp` (idle 11 months), `md_cart_dev` (12 months), `md_cart` (2.5 years), `feature_serial_comm_example` (never merged), plus merged `feature_carts` and `release/*`. |
| OPS-4 | S3 | open | Normalise git tags: `1.0.0` and `v1.0.0` duplicate the same release; `0.1.6`, `0.1.7`, `v1.2.0` are lightweight tags with no message or date; `v0.1.2b` is not valid semver. |
| OPS-5 | S3 | open | `VERSION` is referenced by nothing — not `megadev.make`, no header, no ROM field. Either wire it into the build or drop it in favour of tags. |
| OPS-6 | S4 | open | `lib/sysfont.1bpp.chr` is a **0-byte** binary asset. Restore or delete. |
| OPS-7 | S3 | open | 11 files mix tabs and spaces internally despite `.editorconfig` and `.clang-format`; neither is enforced. Closed by VER-2. |
| OPS-8 | S4 | open | `lib/sub/sp_header.s:11` — the Sub CPU program header string is `.ascii "MAIN       "`. |
| OPS-9 | S3 | open | `.vscode/` is gitignored, but `.devcontainer/devcontainer.json` and `.vscode/c_cpp_properties.json` both hardcode `/usr/bin/m68k-linux-gnu-gcc`. New contributors get no working IntelliSense config. |

## Features & research

| ID | Sev | Status | Item |
|---|---|---|---|
| FEAT-1 | S4 | open | Z80 assembly support. `megadev.make:20` declares `Z80_AS:=sjasmplus`, unused. `docs/manual.md:272` calls it "on the roadmap". |
| FEAT-2 | S4 | open | Evaluate clang/LLVM as an alternative compiler (`docs/manual.md:276`). |
| FEAT-3 | S3 | open | `lib/md_header.s:25` — **ROM header checksum is never calculated.** Also `:31` proper rom start/end, `:33` Extra Memory and Modem fields. *(inline TODOs)* |
| FEAT-4 | S3 | open | `lib/md_init.s:39` — add VDP initialisation (CRAM/VRAM clear). *(inline TODO)* |
| FEAT-5 | S4 | open | `docs/modules.md:79,106` — make the module layout system more user-friendly. *(prose-only backlog item)* |
| FEAT-6 | S4 | open | Support the Main-CPU CD-ROM read path (`docs/cdrom.md`: "not well understood"). |
| FEAT-7 | S4 | open | `lib/main/vdp.h:238` — create a matching macro for `to_vram_addr`. *(inline TODO)* |
| FEAT-8 | S4 | open | `lib/sub/cdrom.h:84` — file info struct. *(inline TODO)* |
| FEAT-9 | S4 | open | `lib/main/comm.def.h:26` — comm definitions need to be user-definable. *(inline TODO)* |

## Hardware research

Items that can only be closed by testing on real hardware. See [SPEC.md §7](SPEC.md#7-hardware-claim-provenance).

| ID | Sev | Status | Item |
|---|---|---|---|
| HW-1 | S2 | open | `docs/boot.md:15` — verify on real hardware whether the boot sector layout can be changed. Currently ASSUMED. |
| HW-2 | S3 | open | `lib/sub/gate_arr.def.h` `@todo` — effects of writing 0 to the Word RAM mode bits in 2M mode. Currently ASSUMED. |
| HW-3 | S3 | open | `lib/main/md_sys.def.h:28` — `HW_REV` is defined as 3 bits but its own comment shows VER3..VER0 (4 bits). Gates the TMSS write in `md_init.s:32`. Resolve against hardware or documentation. |
| HW-4 | S4 | open | `docs/cdrom.md` — measure CDC DMA transfer speed and any bus-contention effects. Currently UNKNOWN. |
| HW-5 | S4 | open | `lib/main/bramcart.def.h` — BRAM cart bank/ID map came from a forum post; verify against a real RAM cart. |
| HW-6 | S3 | open | `lib/main/bios.h:590` — "@todo this isn't clearing the sprite list for some reason...?" An open bug filed as a code comment. |

## Low-value inline TODOs

Retained for completeness; each is a local question rather than a tracked commitment.
`lib/main/comm.macros.s:35` (RERR), `lib/main/bios.h:107,111,1390`, `lib/main/cd_exception.s:64,240`,
`lib/sub/bios.def.h:664` (`@clobber UNKNOWN`), `lib/sub/gate_arr.def.h:648`,
`lib/sub/cdrom.s:137,195,254,362`, `lib/main/z80.def.h:6`, `lib/xform.h:6`,
`examples/hello_world/src/ip.s:81`, `examples/pcm_playback/src/pcmplay_v2.s:44,94`.

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
| `docs/spec-and-backlog` | `develop` | SPEC.md, BACKLOG.md, README index, `docs/html/` ignore | ready to merge |
| `feature/verification-gate` | `develop` | `make check`, `tools/check/*`, GitHub Actions workflow | lint verified; build gate needs a real toolchain run |
| `fix/doxygen-config` | `develop` | Doxyfile repair, two doc-tag typos | verified with doxygen 1.16.1 |
| `fix/doc-cross-references` | `develop` | `bootrom.md`/`design.md` link repair | verified |
| `feature/sub_bios_overhaul` | `develop` | in-flight inline-documentation overhaul | **does not build** (BR-1); resume after the above land |
| `backup/feature_sub_bios_overhaul-2026-08-13` | — | backup pointer, plus tag `backup-sub-bios-overhaul-2026-08-13` | do not delete until the branch is pushed |

None of these branches has been pushed to any remote. Offline backups (bundle, patch, original
SPEC.md stub) are in `/mnt/motoi-s2/dev/megadev-backups/`.

---

## Correctness — verified defects

These are specified in [SPEC.md §8](SPEC.md#8-known-broken). **None are compile-verified** — no m68k
toolchain was available during the audit. VER-1 must land first so that fixes can be proven.

| ID | Sev | Status | Item |
|---|---|---|---|
| LIB-1 | S4 | **done** | ~~`mov.l` is not an M68k mnemonic~~ — **false positive**, GNU as accepts it as an alias for `move` (identical encoding, 0x22d8). Kept as style rule STYLE-1 and corrected in all 5 places. |
| LIB-2 | S1 | **done** | KB-3 — `divu()`/`div()` now set `out.rem`; `div()` uses `divs.w`. |
| LIB-3 | S1 | **done** | KB-4 — `div()` now emits `divs.w` as documented. |
| LIB-4 | S1 | **done** | KB-8 — all **four** `int_to_*` macros were always 0, not just `int_to_f32`. Fixed and locked by Tier 1.5 assertions. |
| LIB-5 | S1 | **done** | KB-5 — `strcpy` in `lib/memory.h` is now `static inline`. |
| LIB-6 | S1 | **done** | KB-6 — the five BRAM buffers are now `extern`, defined in `lib/sub/bram.c`. Projects using BRAM list `sub/bram.c` in their sources. |
| LIB-7 | S1 | **done** | KB-7 — doubled `EXVEC` rename artifacts repaired across 4 code sites + 2 doc references. Link-verified. |
| LIB-8 | S1 | **done** | KB-10 — `bios_work_buffer` uses the `(*((T(*)[N]) ADDR))` idiom; `BIOS_WORK_BUFFER_LEN` is now sizeof-derived. |
| LIB-9 | S1 | **done** | KB-9 — `time_mapping` fixed to the same idiom rather than deleted; it describes the real cartridge /TIME region. |
| LIB-10 | S1 | **done** | KB-11 — `_BIT` companions added for all six SCTRL flags (INV-6); the four `btst` sites now use indices. |
| LIB-11 | S2 | open | KB-13 — `hextoa8/16/32` C and asm versions disagree on string termination. Decide the contract, then make both match; first subject for VER-3. |
| LIB-12 | S2 | open | `lib/memory.h` — every `memset*`/`memcpy*` uses a `dbf` loop with a **16-bit** counter. Lengths > 65536 silently truncate; length 0 wraps and loops 65536 times. Undocumented. Document or guard. |
| BR-1 | S1 | open | KB-27 — the `macros.s` → `macro.s` rename is unpropagated across 39 files; **`feature/sub_bios_overhaul` does not build.** Must land atomically with its consumers. |
| BR-2 | S1 | open | KB-20 … KB-26 — seven defects that exist only on `feature/sub_bios_overhaul`. See SPEC.md OD-5 for whether to fix on-branch or after merge. |
| LIB-13 | S1 | **done** | KB-28 — `z80_init` in `lib/main/z80.h` is now `static inline`. |
| LIB-14 | S2 | **done** | KB-29 — `lib/main/vdp.s` fixed: register size suffixes removed, `btst.l 0x1` corrected to `btst #1`, and `vdp_ctrl` (a C macro the assembler never saw) replaced with `VDP_CTRL` from vdp.def.h. That last one was a latent **link** error affecting both routines, including the one that already assembled. Verified by disassembly: no relocations, no undefined symbols. |
| LIB-15 | S2 | **done** | KB-30 — the unfinished `ATOI` macro was removed from `lib/str_util.s`, which now assembles. Closes ARCH-6 (INV-3). See FEAT-10 for reimplementation. |
| LIB-16 | S2 | **done** | KB-31 — `lib/sub/commsync.s` deleted. Unreferenced by any project and could not assemble; already deleted on `feature/sub_bios_overhaul`. |
| LIB-17 | S1 | **done** | KB-32 — `vdp_dma_transfer` in `lib/main/vdp.h` is now `static inline`. Found by the ODR check only after the Python port aligned its flags with the real build. |
| BR-3 | S2 | open | `examples/bram/src/bram_demo_init.s:27` includes `<init_data.s>`, which exists at **no ref** in the repo. Determine intent; restore or remove. |

## Architecture

| ID | Sev | Status | Item |
|---|---|---|---|
| ARCH-1 | S1 | open | KB-12 / INV-7 — Main and Sub Gate Array headers define the same macro names with different values and non-matching guards. Blocked on SPEC.md **OD-1**. |
| ARCH-2 | S3 | blocked | Decide whether Mode 1 is a supported target (SPEC.md **OD-2**). Gates ARCH-1. Four abandoned branches exist (`md_cart`, `md_cart_dev`, `feature_carts`, `origin/md_cart`); commit `48167ff` removed the example. |
| ARCH-3 | S3 | open | Settle include-guard style (SPEC.md **OD-3**), then enforce via VER-2. |
| ARCH-4 | S3 | open | Settle pointer-vs-lvalue register-access macro form (SPEC.md **OD-4**). Affects 2.0.0. |
| ARCH-5 | S3 | open | Umbrella headers incomplete: `main.h` omits `bios.h`, `comm.h`, `md_sys.h`, `mmd.h`; `sub.h` omits `bios.h`. |
| ARCH-6 | S3 | **done** | INV-3 violation resolved by removing the `ATOI` macro from `lib/str_util.s` (LIB-15). |
| ARCH-7 | S3 | open | INV-2 violation: `lib/main/vdp.macros.s` has no `#include` at all and relies on caller include order. |
| ARCH-8 | S3 | open | Remove libc-shadowing names with non-libc semantics: `strcmp`→`bool` (`lib/str_util.h:68`), `strcpy`→`void` (`lib/memory.h:120`), `abs`/`abs16` (`lib/math.h`). Part of 2.0.0. |
| ARCH-9 | S3 | open | `lib/sub/pcm.def.h:11-19` uses reserved leading-underscore identifiers (`_PCM_ENV` …). Also `pcm.h` uniquely uses SCREAMING_CASE C macros, a `_c` function suffix, and camelCase struct fields. |
| ARCH-10 | S4 | open | Unprefixed global enumerators in `lib/sub/bram.h`: `NoRam`, `Unformatted`, `OtherFormat`, `SegaFormatted`, `NoMatch`, `FileNotFound`, `Match`. |

## Verification & CI

| ID | Sev | Status | Item |
|---|---|---|---|
| VER-1 | S1 | **done** | **Tier 0 build gate** (SPEC.md §6): per-header compile, assemble every `.s`/`.macro.s`, full build of all examples + `new_project`. Prerequisite for proving every LIB-* fix. Requires classifying each header Main-valid / Sub-valid / both. |
| VER-2 | S2 | **done** | **Tier 1 convention lint**: INV-1 (`.def.h` is `#define`-only), INV-4 (guard name matches path), INV-5 (`@file` matches filename), `clang-format --dry-run --Werror`, Doxygen `WARN_AS_ERROR`. Blocked on DOC-1 for the Doxygen part. |
| VER-5 | S2 | **done** | Tier 1.5 compile-time assertions and Tier 0.5 symbol resolution added; both red-tested. |
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
| DOC-4 | S2 | open | `docs/gate_array.md` ends mid-sentence with no trailing newline. **Exists only on `feature/sub_bios_overhaul`** — there is no such file on `develop` or `master`, so this is branch-scoped. |
| DOC-5 | S3 | open | `docs/main_bios.md`: the **prose is finished and substantial** (Introduction, Naming, Mysteries, Memory Usage, Taxonomy, Component Reference); it is the **per-call reference entries** that are empty — ~39 of them, plus raw notes in the Entities component. **Sources:** `BIOS Revisions/` ROM dumps for disassembly, *Sega-CD Technical Bulletin #3*, and the shipped Sega source packages. See SPEC.md §7. |
| DOC-6 | S3 | open | `docs/megacd_dev.md` and `docs/program_design.md` are ~65% the same document, including identical ASCII memory maps and identical typos. Merge or clearly split by purpose. |
| DOC-7 | S3 | **done** | `docs/manual.md` Further Reading omitted `program_design.md`; added. (`gate_array.md` is not listed because it does not exist on `develop` — see DOC-4.) |
| DOC-8 | S3 | open | No prose documentation for `sub/pcm.*`, `sub/bram.*`, `sub/bios.*`, `main/vdp.*`, `main/io.*`, `main/z80.*`, `main/comm.*`, or the `lib/` utility headers — roughly 45 of 72 files. **For PCM specifically:** *Mega-CD Hardware Manual - PCM Sound Source* (SPEC.md §7). |
| DOC-9 | S4 | open | `docs/manual.md` typos preserved across releases: "Masimum" (×2), "apepar", "relatve" (×3), "reccommend", "supporttools". |
| DOC-10 | S3 | open | ~29 `@file` / `@def` / `@fn` tags name the wrong file or symbol (e.g. `lib/main/md_sys.h:4-7` is a copy-paste of `mmd.h`'s header block). Enforced by INV-5 once VER-2 lands. |
| DOC-11 | S3 | open | Write a `CHANGELOG.md`. 13 release tags exist with no release notes anywhere. |
| DOC-12 | S3 | open | Write a `CONTRIBUTING.md`. SPEC.md actively solicits contributions and `docs/dev_in_c.md:85` asks for help with no channel to receive it. |

## Hygiene & repo operations

| ID | Sev | Status | Item |
|---|---|---|---|
| OPS-1 | S2 | **needs owner** | **The SSH host key for `cloud.motoi.pro` has changed.** Connection was refused on 2026-08-13 (`SHA256:0QUYZWVWxGjdB3k65UVa8/3bKx0zopnl7iKL2PPx0n8`, offending entry `~/.ssh/known_hosts:8`). The key was **not** accepted. The remote was removed from this clone per SPEC.md D6. Verify the server was legitimately rebuilt before trusting it again; re-add with `git remote add origin git@cloud.motoi.pro:megadev.git`. |
| OPS-2 | S3 | **done** | Add `docs/html/` to `.gitignore` — 385 generated files sit permanently in `git status`, one `git add .` from being committed. Commit `5013d70` removed the earlier exclusion. |
| OPS-3 | S3 | open | Prune stale branches (all renamed to the `feature/` prefix on 2026-08-13): `temp` (idle 11 months), `md_cart_dev` (12 months), `md_cart` (2.5 years), `feature_serial_comm_example` (never merged), plus merged `feature_carts` and `release/*`. |
| OPS-4 | S3 | open | Normalise git tags: `1.0.0` and `v1.0.0` duplicate the same release; `0.1.6`, `0.1.7`, `v1.2.0` are lightweight tags with no message or date; `v0.1.2b` is not valid semver. |
| OPS-5 | S3 | open | `VERSION` is referenced by nothing — not `megadev.make`, no header, no ROM field. Either wire it into the build or drop it in favour of tags. |
| OPS-6 | S4 | open | `lib/sysfont.1bpp.chr` is a **0-byte** binary asset. Restore or delete. |
| OPS-7 | S3 | open | 11 files mix tabs and spaces internally despite `.editorconfig` and `.clang-format`; neither is enforced. Closed by VER-2. |
| OPS-8 | S4 | open | `lib/sub/sp_header.s:11` — the Sub CPU program header string is `.ascii "MAIN       "`. |
| OPS-9 | S3 | open | `.vscode/` is gitignored, but `.devcontainer/devcontainer.json` and `.vscode/c_cpp_properties.json` both hardcode `/usr/bin/m68k-linux-gnu-gcc`. New contributors get no working IntelliSense config. |
| ARCH-12 | S3 | open | **Bit-index / mask naming is split three ways.** See SPEC.md **OD-8**. `manual.md` documents the prefix form (`GA_BIT_DMNA`/`GA_MASK_DMNA`, 40 uses); `feature/sub_bios_overhaul` uses the suffix form; the `SCTRL_*_BIT` companions added on 2026-08-13 also used suffix, and are the only suffix names on `develop`. Blocked on OD-8. |
| DOC-15 | S3 | open | `docs/cdrom.md` says the file list is cached with space for 128 files, adjustable via `dir_cache` in `sub/cdrom.s` — verify that buffer name and size still match the code. |
| DOC-13 | S3 | open | `lib/sub/cdboot.def.h` (`boot.def.h` on `feature/sub_bios_overhaul`) keeps the Sega names `CBTIPDISC`/`CBTIPSTAT`/`CBTSPDISC`/`CBTSPSTAT` with briefs reading "Preent in 32X CD example source" (typo x4). **Source located:** `Sega Super 32X/dev examples/CD_V2A/INC/CDBIOS.INC`. Megadev's values were verified byte-for-byte against it on 2026-08-13, so these can be promoted from hearsay to DOC and given real briefs. |
| OPS-10 | S4 | open | A stale `git stash` exists: `stash@{0}: WIP on out_path: bcd7db8`, an incomplete `OUT_PATH` → `BUILD_PATH` rename across 8 makefiles. It references `makefile.global` and `examples/ipx_spx/`, neither of which still exists, and the rename was evidently completed by other means since the tree now uses `BUILD_PATH`. Almost certainly obsolete — confirm and drop. |

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
| LIB-18 | S4 | open | Full Q16.16 / Q16.16 division (`fix32_div`). Needs a 48-bit intermediate, which the 68000's 32/16 DIVS.W cannot provide, so it needs a software long-division routine. `fix32_div_int` covers the common case today. |
| ARCH-11 | S3 | open | `div_t` in `lib/math.h` shadows `<stdlib.h>`'s `div_t` with a different layout (standard is `int quot; int rem;`). Part of the ARCH-8 libc-shadowing cluster alongside `div`, `divu`, `abs`, `strcpy`, `strcmp`. Note `_t` in Megadev means "stands in for a libc type" (see SPEC.md D11), so this name is actively misleading. |
| FEAT-10 | S4 | open | **Reimplement ASCII-to-integer conversion.** The old `ATOI` macro in `lib/str_util.s` was never finished: it took no macro arguments (hardcoded to `a0`/`d0`/`d1`) and ended on a bare label with no `.endm` or `rts`, so the file could not assemble. Removed in LIB-15; recoverable from git history. It parsed an ASCII **hex** string into an integer — the inverse of `hextoa8/16/32` in the same file. A reimplementation should take proper arguments and settle the terminator convention alongside LIB-11. |
| FEAT-9 | S4 | open | `lib/main/comm.def.h:26` — comm definitions need to be user-definable. *(inline TODO)* |

## Hardware research

Items that can only be closed by testing on real hardware. See [SPEC.md §7](SPEC.md#7-hardware-claim-provenance).

| ID | Sev | Status | Item |
|---|---|---|---|
| HW-1 | S2 | open | `docs/boot.md:15` — verify on real hardware whether the boot sector layout can be changed. **Source to read first:** *Mega-CD Disc Format Specifications* + BIOS Manual (see SPEC.md §7). |
| HW-2 | S3 | open | `lib/sub/gate_arr.def.h` `@todo` — effects of writing 0 to the Word RAM mode bits in 2M mode. **The `@todo` names its own source:** *Mega-CD Hardware Manual - The Hardware*, which is in the library share (SPEC.md §7). |
| HW-3 | S3 | open | `lib/main/md_sys.def.h:28` — `HW_REV` is 3 bits but its comment shows VER3..VER0 (4 bits). Gates the TMSS write in `md_init.s:32`. **Settle against the *Genesis Software Manual*.** |
| HW-4 | S4 | open | `docs/cdrom.md` — measure CDC DMA transfer speed and bus-contention effects. **Start with *Mega-CD Hardware Manual - The Hardware*.** |
| DOC-14 | S3 | open | Cross-check Megadev's Sub BIOS and BRAM function codes against `CD_V2A/INC/CDBIOS.INC`, the same way the `CBT*` codes were verified. It defines `BRMINIT`/`BRMSTAT`/`BRMSERCH`/`BRMREAD` and the Sub BIOS codes from an authoritative Sega source. |
| HW-5 | S4 | open | `lib/main/bramcart.def.h` — bank/ID map came from a forum post. **Cross-check the BIOS Manual** before promoting it above ASSUMED. |
| HW-6 | S3 | open | `lib/main/bios.h:590` — "@todo this isn't clearing the sprite list for some reason...?" An open bug filed as a code comment. **The `BIOS Revisions/` ROM dumps allow disassembling the actual routine.** |

## Low-value inline TODOs

Retained for completeness; each is a local question rather than a tracked commitment.
`lib/main/comm.macros.s:35` (RERR), `lib/main/bios.h:107,111,1390`, `lib/main/cd_exception.s:64,240`,
`lib/sub/bios.def.h:664` (`@clobber UNKNOWN`), `lib/sub/gate_arr.def.h:648`,
`lib/sub/cdrom.s:137,195,254,362`, `lib/main/z80.def.h:6`, `lib/xform.h:6`,
`examples/hello_world/src/ip.s:81`, `examples/pcm_playback/src/pcmplay_v2.s:44,94`.

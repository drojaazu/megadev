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
| LIB-12 | S2 | **done** | `memset8/16/32` and `memcpy8/16/32` used a single `dbra`, which decrements only the **low word**: anything past 65536 elements silently truncated, and a length of 0 underflowed into ~65536 iterations. Now a nested `dbra` (low word inner, high word outer) with a zero guard. Verified from the emitted code, including the 65536/65537 boundary. |
| BR-1 | S1 | **done** | KB-27 — the `.macros.s` → `.macro.s` rename is landed on `develop` directly (D7), not via the branch: 9 renames, 57 files updated, guards and docs included. Verified by the gate rather than by hope. |
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
| MAKE-1 | S1 | **done** | KB-16 / B-2 — header dependency tracking via `-MMD -MP` plus `.SECONDEXPANSION:` module→object edges. `make clean` is no longer needed; `docs/modules.md` updated. |
| MAKE-2 | S2 | **done** | KB-15 / B-1 — output directories are created by the rules that write into them. `make init` is no longer required, and Tier 0.3 proves it by not running it. |
| MAKE-3 | S2 | **done** | KB-14 — `MEGADEV_PATH` is now sanity-checked with a clear error. |
| MAKE-4 | S2 | **done** | B-5 — object paths mirror the source path (`sub/pcm.s` → `build/sub/pcm.s.o`). Removing the `$(notdir)` also removed the recursive sub-make, since objects are now real prerequisites. |
| MAKE-5 | S2 | **done** | B-4 — projects declare `DISC_CONTENTS` and the ISO depends on it. Verified with repeated `make -j8` from clean. |
| MAKE-6 | S3 | **done** | B-3 — `SOURCE_DATE_EPOCH` gives byte-identical ISOs. Required pinning **both** `mkisofs -creation-date` and the payload file mtimes; the volume date alone was not enough. |
| MAKE-7 | S3 | **done** | `boot.bin` is produced from a separate `boot.bin.o` rather than objcopied in place. |
| MAKE-8 | S3 | **done** | `mkisofs` is now the overridable `$(MKISOFS)`. |
| MAKE-9 | S3 | **done** | Recipes use `$(Q)`; build with `V=1` to see every command. |
| MAKE-10 | S3 | **done** | `.SECONDARY: $(BUILD_PATH)/*` expanded its wildcard at parse time, so it meant "all targets" on a clean tree and only pre-existing files on an incremental one. Now a bare `.SECONDARY:`, which states that intent and behaves identically on every run. |
| MAKE-11 | S4 | **done** | Dead `TOOLS_PATH`, `AS`, `Z80_AS` and the leftover debug `echo` removed. |
| MAKE-12 | S4 | **done** | `ISO_FLAGS` makes the mkisofs options overridable. |
| VER-7 | S1 | **done** | **Tier 0.6 — incremental rebuild.** Nothing detected the loss of header dependency tracking: the `-include` can be deleted and every build still succeeds, make simply stops noticing header changes. That regression shipped. This tier builds gfx, touches `lib/main/vdp.h`, and asserts something recompiles and that a following build is a no-op. Red-tested against the exact regression. |
| VER-6 | S2 | **done** | Tier 0.3 verified only the exit status, so a misexpanded prerequisite that made `mkisofs` master an **empty** disc still passed. It now checks every file in `DISC_CONTENTS` exists and is non-empty, and that an image was produced. Red-tested by reintroducing the bug: 5 projects fail. |
| MAKE-13 | S3 | open | `cfg/module_mmd_newwork.ld` deleted (dead WIP, in git history). `cfg/module_bin.ld` **kept and annotated**: it describes raw data modules, and the module rules already `filter %.bin` when gathering symbol references, so the design is half-present. Wiring it up needs an example to verify against — an unverified feature is worse than a documented gap. |
| MAKE-15 | S3 | **done (superseded)** | `-z muldefs` **removed**. It was only ever needed because `ld -R` imports a module's private build metadata (`MODULE_ROM_ORIGIN`, `MMD_DEST`, `_BSS_ORIGIN`, `main`…), which the importing module also defines. The symbol reference is now curated with `objcopy` before use, and a project may declare an explicit ABI in `<module>.exports`. Duplicate symbols are real errors again. |
| ARCH-13 | S3 | **done** | Memory resident modules build with `cfg/module_resident_*.ld`, naming their layout metadata `RESIDENT_*`. This removes the `ld -R` symbol collision at its source, so the symbol-stripping workaround added earlier is gone. Declared per project with `RESIDENT_MODULES`. |
| MAKE-16 | S2 | **done** | No `.DELETE_ON_ERROR:`, so a recipe failing part way left partial output that make then reported as **up to date**. Demonstrated: a failed recipe left a 7-byte file and the next `make` said "up to date". An interrupted `mkisofs`/`objcopy`/`ld` yielded a corrupt image the next build accepted. |
| MAKE-17 | S3 | **done** | `HEADER_VOL_ID`/`HEADER_SYS_ID` used `$(PROJECT_ID)` as printf's *format* string, so a `%` or leading `-` in a project name misbehaved. Now `printf '%s'`. The ISO also labelled itself `-V "$(PROJECT_ID)"`, a second differently-cased spelling; it now uses `HEADER_VOL_ID`. |
| MAKE-14 | S2 | **done** | KB-18 — disc payload assets now live in `$(RES_PATH)` (the convention `docs/disc.md` already stated) and are copied in via `DISC_ASSETS`. `examples/pcm_playback/res/audio.pcm` is tracked, so a fresh clone gets a working ISO. Verified: `AUDIO.PCM;1` is present in the image at 262,144 bytes. |

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
| ARCH-12 | S3 | **done** | Register field naming unified on `<SUBSYSTEM>_<FIELD>_<ASPECT>` with `_POS`/`_WIDTH`/`_MASK` (SPEC.md **D12**). Six prior spellings collapsed to one; two latent defects fixed on the way (missing parentheses in `GA_MASK_CDCMODE_*` and `MSK_GA_REG_INT*`; values misnamed as masks). |
| DOC-16 | S3 | **done** | `_HI`/`_LO` address definitions and `ga_reg8` C accessors added for `GA_REG_RESET` and `GA_REG_MEMMODE` on both CPU sides — the registers the library demonstrably byte-accesses. The 19 bare `+ 1` sites now use the named constants. Not applied blanket: offering `_LO` for a word-only register would invite a bus error. |
| DOC-17 | S3 | open | Registers with timing/sequencing constraints beyond access width — the CD fader (`GA_REG_CDFADER`, 0xFF8034) among them — are undocumented. `lib/sub/gate_arr.h:239` has only a one-line brief. Transcribe from *Mega-CD Hardware Manual - The Hardware*; do not write these from memory. |
| DOC-18 | S3 | open | **Register numbering is inconsistent.** `lib/sub/gate_arr.def.h` labels `GA_REG_MEMMODE` (0xFF8002) as "GA Reg 01" while `feature/sub_bios_overhaul` labels the same register "Reg. #2". One counts words, the other bytes. Settle against Sega's own numbering before applying per-register doc groups, since the group names encode it. |
| DOC-15 | S3 | open | `docs/cdrom.md` says the file list is cached with space for 128 files, adjustable via `dir_cache` in `sub/cdrom.s` — verify that buffer name and size still match the code. |
| DOC-13 | S3 | open | `lib/sub/cdboot.def.h` (`boot.def.h` on `feature/sub_bios_overhaul`) keeps the Sega names `CBTIPDISC`/`CBTIPSTAT`/`CBTSPDISC`/`CBTSPSTAT` with briefs reading "Preent in 32X CD example source" (typo x4). **Source located:** `Sega Super 32X/dev examples/CD_V2A/INC/CDBIOS.INC`. Megadev's values were verified byte-for-byte against it on 2026-08-13, so these can be promoted from hearsay to DOC and given real briefs. |
| OPS-10 | S4 | open | A stale `git stash` exists: `stash@{0}: WIP on out_path: bcd7db8`, an incomplete `OUT_PATH` → `BUILD_PATH` rename across 8 makefiles. It references `makefile.global` and `examples/ipx_spx/`, neither of which still exists, and the rename was evidently completed by other means since the tree now uses `BUILD_PATH`. Almost certainly obsolete — confirm and drop. |

## Features & research

| ID | Sev | Status | Item |
|---|---|---|---|
| FEAT-1 | S4 | open | Z80 assembly support. `megadev.make:20` declares `Z80_AS:=sjasmplus`, unused. `docs/manual.md:272` calls it "on the roadmap". |
| FEAT-2 | S4 | open | Evaluate clang/LLVM as an alternative compiler (`docs/manual.md:276`). |
| FEAT-3 | S3 | **done** | Cartridge ROM header is patched after linking by `tools/romfix.py`: ROM end is set to the real last byte and the Sega checksum computed over 0x200..EOF. Previously the header claimed 4 MB for an 8 KB ROM and a zero checksum. Cross-checked in Tier 0.3 with an independent recomputation. Note `:33` (Extra Memory / Modem fields) is still unaddressed. |
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

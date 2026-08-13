"""Tier 0 - the build gate: compile, assemble and link checks."""

from __future__ import annotations

from pathlib import Path

from . import toolchain as tc
from .report import Reporter, info

EXCLUDE_FILE = Path(__file__).resolve().parents[1] / "asm-exclude.txt"
ASSERTS_DIR = Path(__file__).resolve().parents[1] / "asserts"
ALLOW_FILE = Path(__file__).resolve().parents[1] / "symbol-allow.txt"


def _load_exclusions() -> dict[str, str]:
    """Assembly sources that cannot be assembled standalone, with reasons."""
    if not EXCLUDE_FILE.exists():
        return {}
    out = {}
    for line in EXCLUDE_FILE.read_text().splitlines():
        line = line.strip()
        if line and not line.startswith("#") and "|" in line:
            path, reason = line.split("|", 1)
            out[path.strip()] = reason.strip()
    return out


def headers() -> int:
    """Tier 0.1 - every header compiles standalone (INV-8).

    NOTE: this cannot see defects that only appear when a macro is expanded or
    an inline function is instantiated. See SPEC.md section 6.
    """
    info(f"Tier 0.1 - per-header compile ({tc.CC})")
    tc.require()
    tc.TMP.mkdir(parents=True, exist_ok=True)
    rep = Reporter("Headers")

    for rel in tc.lib_files(".h"):
        for target in tc.targets_for(rel):
            tu = tc.TMP / f"tu_{target}_{rel.replace('/', '_')}.c"
            tu.write_text(f"#include <{rel}>\n")
            res = tc.run(
                [tc.CC, *tc.C_FLAGS, f"-DTARGET={target}", "-fsyntax-only", "-x", "c", str(tu)]
            )
            item = f"lib/{rel} [TARGET={target}]"
            if res.returncode != 0:
                rep.fail(item, res.stderr)
            elif res.stderr.strip():
                rep.fail(f"{item} (warnings)", res.stderr)
            else:
                rep.ok()
    return rep.summarise()


def assemble() -> int:
    """Tier 0.2 - every assembly source assembles.

    Unresolved symbols are fine; that is the linker's business. We assert only
    that the assembler accepts the source.
    """
    info(f"Tier 0.2 - assemble sources ({tc.CC})")
    tc.require()
    tc.TMP.mkdir(parents=True, exist_ok=True)
    rep = Reporter("Assembly")
    exclusions = _load_exclusions()

    for rel in tc.lib_files(".s"):
        if rel in exclusions:
            rep.skip(f"lib/{rel}", exclusions[rel])
            continue
        obj = tc.TMP / f"{rel.replace('/', '_')}.o"
        res = tc.run(
            [tc.CC, *tc.ASM_FLAGS, "-DTARGET=MEGACD", "-c", str(tc.LIB / rel), "-o", str(obj)]
        )
        item = f"lib/{rel} [TARGET=MEGACD]"
        if res.returncode != 0:
            rep.fail(item, res.stderr)
        else:
            rep.ok()
    return rep.summarise()


def link() -> int:
    """Tier 0.4 - one-definition-rule check (INV-9).

    A header that defines storage or a non-static function compiles fine alone.
    The defect only appears when two TUs include it and are linked.
    """
    info("Tier 0.4 - one-definition-rule (two TUs, linked)")
    tc.require()
    tc.TMP.mkdir(parents=True, exist_ok=True)
    rep = Reporter("ODR")

    flags = [*tc.C_FLAGS, "-fno-common", "-nostdlib", "-c"]

    for rel in tc.lib_files(".h"):
        target = "MEGACD"
        tag = f"{rel.replace('/', '_')}_{target}"
        objs = []
        broken = False
        for n in (1, 2):
            src = tc.TMP / f"odr{n}_{tag}.c"
            obj = tc.TMP / f"odr{n}_{tag}.o"
            src.write_text(
                f"#include <{rel}>\nint odr_{n}(void);\nint odr_{n}(void){{return {n};}}\n"
            )
            if tc.run([tc.CC, *flags, f"-DTARGET={target}", str(src), "-o", str(obj)]).returncode:
                # Won't compile alone: that is Tier 0.1's finding, not ours.
                broken = True
                break
            objs.append(str(obj))
        if broken:
            continue

        res = tc.run([tc.LD, "-nostdlib", "-z", "noexecstack", *objs, "-o", "/dev/null"])
        stderr = "\n".join(
            l for l in res.stderr.splitlines() if "cannot find entry symbol" not in l
        ).strip()
        item = f"lib/{rel} [TARGET={target}]"
        if stderr:
            rep.fail(item, stderr)
        else:
            rep.ok()
    return rep.summarise()


def asserts() -> int:
    """Tier 1.5 - compile-time semantic assertions.

    Tier 0.1 proves a macro parses; it says nothing about what the macro
    evaluates to, because a macro that is never expanded is never checked.
    These translation units expand the macros and assert the results with
    _Static_assert, catching "compiles fine, computes garbage" without needing
    hardware or an emulator.
    """
    info("Tier 1.5 - compile-time semantic assertions")
    tc.require()
    rep = Reporter("Assertions")

    sources = sorted(ASSERTS_DIR.glob("*.c"))
    if not sources:
        rep.skip("no assertion sources found")
        return rep.summarise()

    for src in sources:
        res = tc.run(
            [tc.CC, *tc.C_FLAGS, "-DTARGET=MEGACD", "-fsyntax-only", "-x", "c", str(src)]
        )
        item = f"asserts/{src.name}"
        if res.returncode != 0:
            rep.fail(item, res.stderr)
        else:
            rep.ok(item, quiet=False)
    return rep.summarise()


def _load_allowlist() -> dict[str, str]:
    if not ALLOW_FILE.exists():
        return {}
    out = {}
    for line in ALLOW_FILE.read_text().splitlines():
        line = line.strip()
        if line and not line.startswith("#") and "|" in line:
            sym, reason = line.split("|", 1)
            out[sym.strip()] = reason.strip()
    return out


def symbols() -> int:
    """Tier 0.5 - cross-reference undefined symbols against what lib defines.

    An undefined symbol is legal until final link, so neither the assemble tier
    nor the ODR tier can see a typo'd or renamed symbol. This builds every lib
    object, collects what they define and what they leave undefined, and
    reports anything undefined that lib does not define and that is not a
    documented external. This is how the doubled EXVEC_TRACE rename surfaced.
    """
    info("Tier 0.5 - symbol resolution")
    tc.require()
    tc.TMP.mkdir(parents=True, exist_ok=True)
    rep = Reporter("Symbols")

    exclusions = _load_exclusions()
    objs: list[Path] = []

    for rel in tc.lib_files(".s"):
        if rel in exclusions:
            continue
        obj = tc.TMP / f"sym_{rel.replace('/', '_')}.o"
        if tc.run([tc.CC, *tc.ASM_FLAGS, "-DTARGET=MEGACD", "-c",
                   str(tc.LIB / rel), "-o", str(obj)]).returncode == 0:
            objs.append(obj)
    for rel in tc.lib_files(".c"):
        obj = tc.TMP / f"sym_{rel.replace('/', '_')}.o"
        if tc.run([tc.CC, *tc.C_FLAGS, "-DTARGET=MEGACD", "-c",
                   str(tc.LIB / rel), "-o", str(obj)]).returncode == 0:
            objs.append(obj)

    if not objs:
        rep.fail("symbol resolution", "no lib objects could be built")
        return rep.summarise()

    nm = f"{tc.M68K_PREFIX}nm"
    defined = set()
    for line in tc.run([nm, "--defined-only", *map(str, objs)]).stdout.splitlines():
        parts = line.split()
        if len(parts) >= 3:
            defined.add(parts[2])

    allow = _load_allowlist()
    undefined: dict[str, set[str]] = {}
    for obj in objs:
        for line in tc.run([nm, "-u", str(obj)]).stdout.splitlines():
            parts = line.split()
            if len(parts) >= 2 and parts[0] == "U":
                undefined.setdefault(parts[1], set()).add(obj.name)

    unresolved = {s: o for s, o in undefined.items() if s not in defined and s not in allow}

    for sym in sorted(unresolved):
        rep.fail(
            f"unresolved symbol: {sym}",
            "referenced by " + ", ".join(sorted(unresolved[sym]))
            + "\nNot defined anywhere in lib/ and not in tools/check/symbol-allow.txt."
            + "\nEither it is a typo, or it is a genuine external - add it to the allowlist with a reason.",
        )
    rep.passed = len(defined)
    return rep.summarise()

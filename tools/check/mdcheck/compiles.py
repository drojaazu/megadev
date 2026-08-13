"""Tier 0 - the build gate: compile, assemble and link checks."""

from __future__ import annotations

from pathlib import Path

from . import toolchain as tc
from .report import Reporter, info

EXCLUDE_FILE = Path(__file__).resolve().parents[1] / "asm-exclude.txt"


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

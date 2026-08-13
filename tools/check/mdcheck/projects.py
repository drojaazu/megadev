"""Tier 0.3 - build every example and the project template end to end.

The only check that exercises the linker scripts, the MMD header emitted by
cfg/module_mmd.ld, the IP/SP boot chain and ISO mastering.
"""

from __future__ import annotations

import shutil
import subprocess

from . import toolchain as tc
from .report import Reporter, info, warn


def _make(directory, *goals: str) -> subprocess.CompletedProcess[str]:
    return tc.run(
        ["make", "-C", str(directory), f"MEGADEV_PATH={tc.ROOT}", *goals]
    )


def run(clean_first: bool = True) -> int:
    info("Tier 0.3 - build examples and template")
    tc.require()

    if shutil.which("mkisofs") is None:
        warn("mkisofs not found - Mega CD projects will fail at the ISO step")

    projects = sorted(p for p in (tc.ROOT / "examples").iterdir() if p.is_dir())
    projects.append(tc.ROOT / "new_project")

    rep = Reporter("Projects")

    if clean_first:
        for p in projects:
            if (p / "makefile").exists():
                _make(p, "clean")

    for p in projects:
        if not (p / "makefile").exists():
            rep.skip(p.name, "no makefile")
            continue

        # Deliberately NOT running `make init` first. megadev.make now creates
        # its own output directories, so a fresh clone must build with a bare
        # `make` (SPEC.md B-1). This tier is what proves that stays true.
        res = _make(p)

        if res.returncode != 0:
            rep.fail(p.name, res.stdout + res.stderr)
            continue

        # Exit status alone is NOT enough. A misexpanded prerequisite once let
        # mkisofs master an image from an empty disc/ and still exit 0, so the
        # gate reported a completely broken ISO as a pass. Verify the build
        # actually produced what it said it would.
        missing = []
        contents = _make(p, "-s", "print-DISC_CONTENTS").stdout.split()
        for item in contents:
            f = p / item
            if not f.exists() or f.stat().st_size == 0:
                missing.append(f"{item} (declared in DISC_CONTENTS)")

        images = [f for f in list(p.glob("*.iso")) + list(p.glob("*.cart"))]
        if not images:
            missing.append("no .iso or .cart produced")
        else:
            missing += [f"{i.name} is empty" for i in images if i.stat().st_size == 0]

        if missing:
            rep.fail(p.name, "build exited 0 but did not produce:\n  " + "\n  ".join(missing))
        else:
            rep.ok(p.name, quiet=False)

    return rep.summarise()

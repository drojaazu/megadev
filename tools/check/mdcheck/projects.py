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

        # `make init` is required on a fresh clone: build/ and disc/ are
        # gitignored and megadev.make never creates them (BACKLOG MAKE-2).
        # When that is fixed, drop this and the gate will prove the fix.
        res = _make(p, "init")
        if res.returncode == 0:
            res = _make(p)

        if res.returncode == 0:
            rep.ok(p.name, quiet=False)
        else:
            rep.fail(p.name, res.stdout + res.stderr)

    return rep.summarise()

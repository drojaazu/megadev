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
        probe = _make(p, "-s", "print-DISC_CONTENTS")
        if probe.returncode != 0:
            # Previously this failure was swallowed, so the content check below
            # silently verified nothing at all for several commits.
            rep.fail(p.name, "could not read DISC_CONTENTS:\n" + probe.stderr)
            continue
        contents = probe.stdout.split()
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


def incremental() -> int:
    """Tier 0.6 - editing a header rebuilds its dependents (SPEC.md B-2).

    Nothing else detects the loss of this behaviour. The -include of the .d
    files can be deleted and every build still succeeds; make simply stops
    noticing header changes. That happened once, silently, and shipped.

    So this exercises the real behaviour rather than grepping the makefile:
    build clean, touch a library header, and assert that something recompiled
    and that a following build is a no-op.
    """
    info("Tier 0.6 - incremental rebuild on header change")
    tc.require()
    rep = Reporter("Incremental")

    project = tc.ROOT / "examples" / "gfx"
    header = tc.LIB / "main" / "vdp.h"
    if not (project / "makefile").exists() or not header.exists():
        rep.skip("gfx or lib/main/vdp.h missing")
        return rep.summarise()

    _make(project, "clean")
    if _make(project).returncode != 0:
        rep.fail("baseline build", "gfx must build before the check is meaningful")
        return rep.summarise()

    header.touch()
    res = _make(project)
    if res.returncode != 0:
        rep.fail("rebuild after touching a header", res.stdout + res.stderr)
        return rep.summarise()
    if "Compiling" not in res.stdout:
        rep.fail(
            "touching lib/main/vdp.h recompiled nothing",
            "Header dependency tracking is not working. Check that megadev.make\n"
            "still passes -MMD -MP and -includes the generated .d files, and that\n"
            "module targets take their objects as prerequisites.",
        )
        return rep.summarise()
    rep.ok("header change triggers a rebuild", quiet=False)

    res = _make(project)
    if "Compiling" in res.stdout:
        rep.fail("second build recompiled again", "a no-op build should do nothing")
    else:
        rep.ok("unchanged rebuild is a no-op", quiet=False)
    return rep.summarise()

#!/usr/bin/env python3
"""MEGADEV verification gate.

Usage:
    check.py all | headers | asm | link | conventions | projects
    check.py conventions --write-baseline

Run inside the devcontainer, where the m68k toolchain lives. See SPEC.md
section 6 for what each tier can and cannot prove.
"""

from __future__ import annotations

import argparse
import sys

from mdcheck import compiles, conventions, projects

TIERS = {
    "conventions": lambda a: conventions.run(write=a.write_baseline),
    "headers": lambda a: compiles.headers(),
    "link": lambda a: compiles.link(),
    "asserts": lambda a: compiles.asserts(),
    "symbols": lambda a: compiles.symbols(),
    "asm": lambda a: compiles.assemble(),
    "projects": lambda a: projects.run(),
    "incremental": lambda a: projects.incremental(),
}

# Cheapest first, so an obvious breakage fails fast.
ALL_ORDER = ["conventions", "headers", "asserts", "link", "asm", "symbols", "projects", "incremental"]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("tier", choices=["all", *TIERS])
    ap.add_argument(
        "--write-baseline",
        action="store_true",
        help="conventions only: accept the current findings as the baseline",
    )
    args = ap.parse_args()

    if args.tier != "all":
        return TIERS[args.tier](args)

    for name in ALL_ORDER:
        if status := TIERS[name](args):
            return status
        print()
    print("\033[1;32mAll checks passed.\033[0m")
    return 0


if __name__ == "__main__":
    sys.path.insert(0, str(__import__("pathlib").Path(__file__).resolve().parent))
    raise SystemExit(main())

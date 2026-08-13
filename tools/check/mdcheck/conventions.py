"""Tier 1 - convention lint.

Enforces the invariants in SPEC.md section 2:

  INV-1    .def.h files contain only preprocessor directives and comments.
           The load-bearing one: .def.h is included from assembly sources, so
           anything not legal in both languages breaks the assembly side.
  INV-4    Include guard name is derived from the file's path.
  INV-5    An @file doc tag names the file it appears in.
  STYLE-1  M68k mnemonic spelling (see MNEMONIC_SYNONYMS).

The library predates these rules, so results are compared against a baseline of
accepted existing violations. Anything not in the baseline fails; the baseline
should only ever shrink.
"""

from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path

from . import toolchain as tc
from .report import Reporter, info, warn

BASELINE = Path(__file__).resolve().parents[1] / "baseline.txt"


@dataclass(frozen=True, order=True)
class Finding:
    rule: str
    path: str
    detail: str

    def key(self) -> str:
        return f"{self.rule}|{self.path}|{self.detail}"

    @staticmethod
    def parse(line: str) -> "Finding":
        rule, path, detail = line.split("|", 2)
        return Finding(rule, path, detail)


# --------------------------------------------------------------------------
# STYLE-1: mnemonic spelling
#
# Classified empirically against m68k-linux-gnu-as (see build-check/mnemonics.sh):
# each pair below assembles to the IDENTICAL encoding, so choosing between them
# is purely a spelling decision.
#
#   move/mov   -> 2200      dbra/dbf -> 51c8
#   bcc/bhs    -> 64fe      bcs/blo  -> 65fe
#
# GNU's branch-improvement pseudo-ops (jra, jbsr, jhi, jls, jcc, jcs, jne, jeq,
# jvc, jvs, jpl, jmi, jge, jlt, jgt, jle) are deliberately NOT listed. They are
# not synonyms: the assembler picks the smallest working encoding, so `jbsr`
# emits a 2-byte bsr.s when the target is in reach and a 6-byte jsr.l when it is
# not. Rewriting them to a fixed instruction would cost size and cycles. House
# style is to PREFER them (SPEC.md STYLE-2).
# --------------------------------------------------------------------------

MNEMONIC_SYNONYMS: dict[str, str] = {
    "mov": "move",
    "dbf": "dbra",
    "bhs": "bcc",
    "blo": "bcs",
}

# A mnemonic may follow start-of-line, whitespace, a quote or backslash (inline
# asm strings) or a local label colon, as in `0:mov.l (a0)+,(a1)+`.
_MNEMONIC_RE = re.compile(
    r'(?:^|[\s"\\:])(' + "|".join(MNEMONIC_SYNONYMS) + r')(\.[bwlsBWLS])?(?=[\s]|$)'
)


def strip_comments(text: str) -> list[tuple[int, str]]:
    """Remove /* */ and // comments, keeping 1-based line numbers.

    Note: string literals are not tracked. A "/*" inside a string would confuse
    this, but that can only cause a missed finding, never a false one.
    """
    out: list[tuple[int, str]] = []
    in_block = False
    for lineno, raw in enumerate(text.splitlines(), start=1):
        line = raw
        result = []
        i = 0
        while i < len(line):
            if in_block:
                end = line.find("*/", i)
                if end == -1:
                    i = len(line)
                else:
                    in_block = False
                    i = end + 2
            else:
                start = line.find("/*", i)
                dslash = line.find("//", i)
                if dslash != -1 and (start == -1 or dslash < start):
                    result.append(line[i:dslash])
                    i = len(line)
                elif start != -1:
                    result.append(line[i:start])
                    in_block = True
                    i = start + 2
                else:
                    result.append(line[i:])
                    i = len(line)
        out.append((lineno, "".join(result).strip()))
    return out


def check_def_h(text: str, display: str) -> list[Finding]:
    """INV-1: only preprocessor directives and comments."""
    findings = []
    continued = False
    for lineno, line in strip_comments(text):
        was_continued = continued
        continued = line.endswith("\\")
        if not line or was_continued or line.startswith("#"):
            continue
        findings.append(
            Finding("INV-1", display, f"non-preprocessor content -> {lineno}: {line}")
        )
    return findings


def check_guard(text: str, rel: str, display: str) -> list[Finding]:
    """INV-4: every header guards itself with #pragma once.

    Settled in SPEC.md OD-3. Verified that #pragma once behaves correctly when
    a .def.h is pulled in by the assembler (gcc -x assembler-with-cpp),
    including a doubled include, so it is safe for the shared layer too.
    """
    if re.search(r"^\s*#pragma\s+once", text, re.M):
        return []

    m = re.search(r"^\s*#ifndef\s+([A-Za-z_]\w*)", text, re.M)
    if m:
        return [Finding("INV-4", display,
                        f"uses an #ifndef guard ({m.group(1)}); use #pragma once")]
    return [Finding("INV-4", display, "no include guard")]


def check_file_tag(text: str, rel: str, display: str) -> list[Finding]:
    """INV-5: @file names this file. An absent tag is not a violation here."""
    m = re.search(r"@file\s+(\S+)", text)
    if not m:
        return []
    if Path(m.group(1)).name == Path(rel).name:
        return []
    return [Finding("INV-5", display, f"@file says '{m.group(1)}'")]


_ESCAPE_RE = re.compile(r"\\[tnr]")


def check_mnemonics(text: str, display: str) -> list[Finding]:
    """STYLE-1: use the house spelling of synonymous mnemonics."""
    findings = []
    for lineno, raw in enumerate(text.splitlines(), start=1):
        stripped = raw.strip()
        if stripped.startswith(("*", "//")):
            continue
        # C inline asm is written as "\n\tmove.l %0,%1", so a mnemonic can be
        # preceded by a literal \t or \n escape rather than real whitespace.
        line = _ESCAPE_RE.sub(" ", raw)
        for m in _MNEMONIC_RE.finditer(line):
            found, size = m.group(1), m.group(2) or ""
            findings.append(
                Finding(
                    "STYLE-1",
                    display,
                    f"use {MNEMONIC_SYNONYMS[found]}, not {found} "
                    f"-> {lineno}: {found}{size}",
                )
            )
    return findings


def collect() -> list[Finding]:
    findings: list[Finding] = []
    for rel in tc.lib_files(".h", ".s", ".c"):
        text = (tc.LIB / rel).read_text(errors="replace")
        display = f"lib/{rel}"
        if rel.endswith(".def.h"):
            findings += check_def_h(text, display)
        if rel.endswith(".h"):
            findings += check_guard(text, rel, display)
        if rel.endswith((".h", ".s")):
            findings += check_mnemonics(text, display)
        findings += check_file_tag(text, rel, display)
    return sorted(set(findings))


def load_baseline() -> set[str]:
    if not BASELINE.exists():
        warn(f"no baseline at {BASELINE} - every finding will be reported as new")
        return set()
    return {
        line.strip()
        for line in BASELINE.read_text().splitlines()
        if line.strip() and not line.startswith("#")
    }


def write_baseline(findings: list[Finding]) -> None:
    BASELINE.write_text(
        "# Accepted pre-existing convention violations.\n"
        "# Generated by: tools/check/check.py conventions --write-baseline\n"
        "# This list should only ever SHRINK. See SPEC.md section 3.\n"
        + "".join(f"{f.key()}\n" for f in findings)
    )
    info(f"wrote baseline: {len(findings)} accepted violations -> {BASELINE}")


def run(write: bool = False) -> int:
    info("Tier 1 - convention lint")
    findings = collect()

    if write:
        write_baseline(findings)
        return 0

    accepted = load_baseline()
    current = {f.key(): f for f in findings}

    rep = Reporter("Conventions")
    for key in sorted(current.keys() - accepted):
        f = current[key]
        rep.fail(f"{f.rule} {f.path}", f.detail)
    rep.passed = len(accepted & current.keys())

    fixed = accepted - current.keys()
    if fixed:
        print(
            f"\n{len(fixed)} baseline violation(s) no longer present - "
            "remove them with --write-baseline:"
        )
        for key in sorted(fixed):
            print(f"  {key}")

    return rep.summarise()

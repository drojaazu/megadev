"""Result collection and reporting for the MEGADEV verification gate."""

from __future__ import annotations

import sys
from dataclasses import dataclass, field


def _colours(stream) -> dict[str, str]:
    if not stream.isatty():
        return dict.fromkeys(
            ("clear", "bold", "red", "green", "yellow", "cyan"), ""
        )
    return {
        "clear": "\033[0m",
        "bold": "\033[1m",
        "red": "\033[1;31m",
        "green": "\033[1;32m",
        "yellow": "\033[1;33m",
        "cyan": "\033[1;36m",
    }


C = _colours(sys.stdout)


def info(msg: str) -> None:
    print(f"{C['cyan']}==>{C['clear']} {msg}")


def warn(msg: str) -> None:
    print(f"{C['yellow']}WARN{C['clear']} {msg}")


@dataclass
class Reporter:
    """Collects pass/fail/skip results and prints a summary.

    `label` names the tier in the summary line.
    """

    label: str
    passed: int = 0
    skipped: int = 0
    failures: list[tuple[str, str]] = field(default_factory=list)

    def ok(self, item: str = "", quiet: bool = True) -> None:
        self.passed += 1
        if item and not quiet:
            print(f"  {C['green']}ok{C['clear']}   {item}")

    def fail(self, item: str, detail: str = "") -> None:
        self.failures.append((item, detail))
        print(f"{C['red']}FAIL{C['clear']} {item}")
        if detail:
            for line in detail.rstrip().splitlines():
                print(f"       {line}")

    def skip(self, item: str, reason: str = "") -> None:
        self.skipped += 1
        warn(f"skipped: {item}" + (f" - {reason}" if reason else ""))

    @property
    def failed(self) -> int:
        return len(self.failures)

    def summarise(self) -> int:
        """Print the summary. Returns a process exit status."""
        parts = [f"{C['green']}{self.passed} passed{C['clear']}"]
        if self.failed:
            parts.append(f"{C['red']}{self.failed} failed{C['clear']}")
        if self.skipped:
            parts.append(f"{self.skipped} skipped")
        print(f"\n{C['bold']}{self.label}{C['clear']}: " + ", ".join(parts))

        if self.failed:
            print(f"\n{C['bold']}Failed:{C['clear']}")
            for item, _ in self.failures:
                print(f"  {item}")
            return 1
        return 0

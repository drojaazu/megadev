"""Toolchain discovery and the compiler flags the gate shares with megadev.make."""

from __future__ import annotations

import os
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
LIB = Path(os.environ.get("MEGADEV_LIB", ROOT / "lib"))
TMP = Path(os.environ.get("CHECK_TMP", ROOT / "build-check"))

M68K_PREFIX = os.environ.get("M68K_PREFIX", "m68k-linux-gnu-")

# Deliberately NOT falling back to a bare $CC. Environments commonly export CC
# pointing at the host compiler; picking that up would check the source against
# x86 and report confident nonsense. Override with MEGADEV_CC.
CC = os.environ.get("MEGADEV_CC", f"{M68K_PREFIX}gcc")
LD = f"{M68K_PREFIX}ld"

# Placeholder header values. We are checking that the source is accepted, not
# building a real image, so these only have to be well-formed.
_DEFINES = [
    "-DPROJECT_ID=checkgate",
    "-DREGION=US",
    "-DVIDEO=NTSC",
    "-DVRAM_SIZE=VRAM_64K",
    "-DPROJECT_NAME=checkgate",
    "-DPROJECT_NAME_DOMESTIC=checkgate",
    '-DHEADER_HARDWARE_ID="SEGA GENESIS"',
    '-DHEADER_COPYRIGHT="(C) CHECK"',
    '-DHEADER_VOL_ID="CHECKGATE"',
    '-DHEADER_SYS_ID="CHECKGATE"',
    '-DHEADER_SOFTWARE_ID="GM 00-0000-00"',
    '-DHEADER_REGION="JUE"',
    '-DHEADER_DISC_ID="SEGADISCSYSTEM"',
]

BASE = ["-m68000", "-imacros", "build.def.h", "-I", str(LIB), *_DEFINES]

C_FLAGS = [
    *BASE,
    "-fno-builtin",
    "-Wall",
    "-Wextra",
    "-Wno-main",
    "-Werror=implicit-function-declaration",
    "-Werror=infinite-recursion",
    "-Wa,--register-prefix-optional",
]

ASM_FLAGS = [
    *BASE,
    f"-Wa,-I{LIB}",
    "-Wa,--register-prefix-optional",
    "-Wa,--bitwise-or",
    "-x",
    "assembler-with-cpp",
]


def run(cmd: list[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(cmd, capture_output=True, text=True)


def require() -> None:
    """Abort unless a real m68k compiler is available.

    Verifying -m68000 is not pedantry: without it a host compiler on PATH
    produces a full run of confident, meaningless results.
    """
    if shutil.which(CC) is None:
        print(f"FAIL cross compiler '{CC}' not found.", file=sys.stderr)
        print(
            "     Run the gate inside the devcontainer, or set "
            "M68K_PREFIX / MEGADEV_CC.",
            file=sys.stderr,
        )
        raise SystemExit(127)

    TMP.mkdir(parents=True, exist_ok=True)
    probe = TMP / "toolchain_probe.c"
    probe.write_text("int main(void){return 0;}\n")
    if run([CC, "-m68000", "-fsyntax-only", str(probe)]).returncode != 0:
        print(
            f"FAIL '{CC}' does not accept -m68000; it is not an m68k compiler.",
            file=sys.stderr,
        )
        print("     Refusing to run - results would be meaningless.", file=sys.stderr)
        raise SystemExit(127)
    probe.unlink(missing_ok=True)


def targets_for(rel: str) -> list[str]:
    """Which TARGET values a lib file must be valid under.

    lib/sub/* is Mega CD only; everything else must also hold for a plain
    Mega Drive cartridge build.
    """
    return ["MEGACD"] if rel.startswith("sub/") else ["MEGADRIVE", "MEGACD"]


def lib_files(*suffixes: str) -> list[str]:
    out: list[str] = []
    for path in sorted(LIB.rglob("*")):
        if path.is_file() and path.name.endswith(suffixes):
            out.append(str(path.relative_to(LIB)))
    return out

#!/usr/bin/env python3
"""Patch a Mega Drive cartridge ROM header to describe the ROM it is in.

Two fields cannot be known when the header is assembled, only after linking:

    0x1A4  ROM end address   (last valid byte, i.e. size - 1)
    0x18E  checksum          (sum of every 16-bit word from 0x200 to the end)

lib/md_header.s leaves both as placeholders -- the checksum as zero and the ROM
end hardcoded to 0x3FFFFF, which claims 4 MB no matter how large the ROM
actually is. Hardware does not check either field, but flashcarts and loaders
read the ROM end to decide how much to load, so a ROM that lies about its size
can work in an emulator and fail on real hardware.

Usage: romfix.py <rom file>
"""

from __future__ import annotations

import sys
from pathlib import Path

CHECKSUM_OFF = 0x18E
ROM_END_OFF = 0x1A4
CHECKSUM_START = 0x200
HEADER_MIN = 0x200


def checksum(data: bytes) -> int:
    """Sega's ROM checksum: 16-bit words from 0x200 to the end, truncated."""
    total = 0
    # An odd trailing byte cannot form a word and is excluded, matching the
    # behaviour of Sega's own tools.
    for i in range(CHECKSUM_START, len(data) - 1, 2):
        total += (data[i] << 8) | data[i + 1]
    return total & 0xFFFF


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        print(__doc__, file=sys.stderr)
        return 2

    path = Path(argv[1])
    data = bytearray(path.read_bytes())

    if len(data) < HEADER_MIN:
        print(f"romfix: {path} is only {len(data)} bytes; no ROM header present",
              file=sys.stderr)
        return 1

    rom_end = len(data) - 1
    data[ROM_END_OFF:ROM_END_OFF + 4] = rom_end.to_bytes(4, "big")

    # Computed after the ROM end is written, so the checksum covers the final
    # bytes of the image.
    csum = checksum(bytes(data))
    data[CHECKSUM_OFF:CHECKSUM_OFF + 2] = csum.to_bytes(2, "big")

    path.write_bytes(bytes(data))
    print(f"  ROM end 0x{rom_end:06X} ({len(data)} bytes), checksum 0x{csum:04X}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))

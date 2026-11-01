#!/usr/bin/env python3
"""Fill in the Mega Drive cartridge header fields that depend on the final ROM.

    0x1A4  ROM end address: the offset of the last byte (size - 1)
    0x18E  checksum: the sum of every 16-bit big-endian word from 0x200 to
           the end, truncated to 16 bits

lib/md_header.s writes zero to both; megadev.make runs this on every
cartridge after linking. A ROM with an odd length is padded with one 0xFF
byte, so every byte is part of a word and the end address is odd.

Usage: romfix.py <rom file>
"""

import sys
from pathlib import Path

CHECKSUM_OFFSET = 0x18E
ROM_END_OFFSET = 0x1A4
CHECKSUM_START = 0x200


def checksum(data: bytes) -> int:
    total = 0
    for i in range(CHECKSUM_START, len(data), 2):
        total += (data[i] << 8) | data[i + 1]
    return total & 0xFFFF


def main(argv: list) -> int:
    if len(argv) != 2:
        print(__doc__, file=sys.stderr)
        return 2

    path = Path(argv[1])
    data = bytearray(path.read_bytes())
    if len(data) < CHECKSUM_START:
        print(f"romfix: {path}: {len(data)} bytes, too small to hold a header", file=sys.stderr)
        return 1

    if len(data) % 2:
        data.append(0xFF)

    rom_end = len(data) - 1
    data[ROM_END_OFFSET:ROM_END_OFFSET + 4] = rom_end.to_bytes(4, "big")
    # The checksum starts at 0x200, after both header fields, so their order
    # does not matter.
    data[CHECKSUM_OFFSET:CHECKSUM_OFFSET + 2] = checksum(data).to_bytes(2, "big")

    path.write_bytes(data)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))

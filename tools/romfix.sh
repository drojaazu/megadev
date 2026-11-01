#!/bin/sh
# Fill in the Mega Drive cartridge header fields that depend on the final ROM.
#
#   0x1A4  ROM end address: the offset of the last byte (size - 1)
#   0x18E  checksum: the sum of every 16-bit big-endian word from 0x200 to
#          the end, truncated to 16 bits
#
# lib/md_header.s writes zero to both; megadev.make runs this on every
# cartridge after linking. A ROM with an odd length is padded with one 0xFF
# byte, so every byte is part of a word and the end address is odd.
#
# Usage: romfix.sh <rom file>

rom=$1
if [ -z "$rom" ]; then
  echo "usage: romfix.sh <rom file>" >&2
  exit 2
fi

size=$(wc -c < "$rom") || exit 1
if [ "$size" -lt 512 ]; then
  echo "romfix: $rom: $size bytes, too small to hold a header" >&2
  exit 1
fi

if [ $((size % 2)) -ne 0 ]; then
  printf '\377' >> "$rom"
  size=$((size + 1))
fi

# put <offset> <byte count> <value>: write value big-endian at offset
put() {
  bytes=
  i=$(($2 - 1))
  while [ $i -ge 0 ]; do
    bytes="$bytes\\0$(printf '%o' $((($3 >> (8 * i)) & 255)))"
    i=$((i - 1))
  done
  printf '%b' "$bytes" | dd of="$rom" bs=1 seek="$1" conv=notrunc 2> /dev/null
}

put $((0x1A4)) 4 $((size - 1))

sum=$(od -An -v -tu1 -j 512 "$rom" | awk '
  { for (i = 1; i <= NF; i++) s += (n++ % 2) ? $i : $i * 256 }
  END { print s % 65536 }')
put $((0x18E)) 2 "$sum"

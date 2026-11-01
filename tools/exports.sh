#!/bin/sh
# Print an assembly file defining, as absolute symbols, the symbols a linked
# program exports: its defined global symbols with default visibility. A module
# that runs alongside a resident program links this instead of the program
# itself, so a name defined in both is a "multiple definition" error.
#
# Hidden symbols are not exported: layout symbols (GLOBAL_SYM, GLOBAL), linker
# script symbols, and module entry points (lib/module_hidden.s).
#
# Usage: exports.sh <readelf> <linked ELF file>

if [ $# -ne 2 ]; then
  echo "usage: exports.sh <readelf> <linked ELF file>" >&2
  exit 2
fi

"$1" -sW "$2" | awk '
  ($5 == "GLOBAL" || $5 == "WEAK") && $6 == "DEFAULT" && $7 != "UND" && $8 != "" {
    printf ".global %s\n.equ %s, 0x%s\n", $8, $8, $2
  }'

#!/usr/bin/env bash
#
# [ M E G A D E V ]
#
# Tier 0.1 - per-header compile.
#
# Every header in lib/ must be self-contained: including it, and nothing else,
# into an empty translation unit must compile cleanly. This is what catches
# syntax errors, missing includes, illegal casts and self-recursive inline
# functions in headers that no example happens to pull in.
#
# Headers are classified by directory:
#   lib/main/*.h  - Main CPU. Valid for MEGADRIVE and MEGACD.
#   lib/sub/*.h   - Sub CPU. Valid for MEGACD only.
#   lib/*.h       - common. Valid for both.
#
# See SPEC.md section 6, INV-8.

source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

require_toolchain
mkdir -p "$CHECK_TMP"

# Mirrors megadev.make CC_FLAGS for the subset that affects preprocessing.
# Header values are placeholders - we are checking syntax, not building an image.
base_flags=(
	-m68000
	-imacros build.def.h
	-I "$LIB_PATH"
	-DPROJECT_ID=checkgate
	-DREGION=US
	-DVIDEO=NTSC
	-DVRAM_SIZE=VRAM_64K
	-DPROJECT_NAME=checkgate
	-DPROJECT_NAME_DOMESTIC=checkgate
	-DHEADER_HARDWARE_ID='"SEGA GENESIS"'
	-DHEADER_COPYRIGHT='"(C) CHECK"'
	-DHEADER_VOL_ID='"CHECKGATE"'
	-DHEADER_SYS_ID='"CHECKGATE"'
	-DHEADER_SOFTWARE_ID='"GM 00-0000-00"'
	-DHEADER_REGION='"JUE"'
	-DHEADER_DISC_ID='"SEGADISCSYSTEM"'
	-fno-builtin
	-Wall -Wextra -Wno-main
	-Werror=implicit-function-declaration
	-Werror=infinite-recursion
	-Wa,--register-prefix-optional
	-fsyntax-only
)

# targets_for <path relative to lib/>
targets_for() {
	case "$1" in
		sub/*)  printf 'MEGACD\n' ;;
		main/*) printf 'MEGADRIVE\nMEGACD\n' ;;
		*)      printf 'MEGADRIVE\nMEGACD\n' ;;
	esac
}

check_header() {
	local rel="$1" target="$2"
	local tu="$CHECK_TMP/tu_${target}_${rel//\//_}.c"

	printf '#include <%s>\n' "$rel" > "$tu"

	local output status
	output=$("$CC" "${base_flags[@]}" "-DTARGET=$target" -x c "$tu" 2>&1)
	status=$?

	if ((status == 0)); then
		# A clean exit with diagnostics still means warnings we asked about
		if [[ -n $output ]]; then
			fail_item "lib/$rel [TARGET=$target] (warnings)" "$output"
		else
			pass_item
		fi
	else
		fail_item "lib/$rel [TARGET=$target]" "$output"
	fi
}

info "Tier 0.1 - per-header compile ($CC)"

mapfile -t headers < <(cd "$LIB_PATH" && find . -name '*.h' -type f -printf '%P\n' | sort)

if ((${#headers[@]} == 0)); then
	err "no headers found under $LIB_PATH"
	exit 1
fi

for rel in "${headers[@]}"; do
	# .def.h files are checked here too: they must be valid C on their own.
	# Their assembly-legality is checked by conventions.sh (INV-1).
	while read -r target; do
		[[ -n $target ]] && check_header "$rel" "$target"
	done < <(targets_for "$rel")
done

summarise "Headers"

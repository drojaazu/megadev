#!/usr/bin/env bash
#
# [ M E G A D E V ]
#
# Tier 0.4 - one-definition-rule check.
#
# A header that defines storage or a non-inline function compiles perfectly
# well on its own. The defect only appears when TWO translation units include
# it and are linked together. Tier 0.1 cannot see this class of bug at all,
# which is why this check exists separately.
#
# Catches: non-static function definitions in headers, and tentative
# definitions (bare `u8 buf[N];`) that become duplicate symbols under
# -fno-common (the default since GCC 10).
#
# See SPEC.md section 6, INV-9.

source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

require_toolchain
mkdir -p "$CHECK_TMP"

LD="${M68K_PREFIX}ld"

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
	-fno-common
	-nostdlib
	-c
)

check_odr() {
	local rel="$1" target="$2"
	local tag="${rel//\//_}_$target"
	local c1="$CHECK_TMP/odr1_$tag.c" c2="$CHECK_TMP/odr2_$tag.c"
	local o1="$CHECK_TMP/odr1_$tag.o" o2="$CHECK_TMP/odr2_$tag.o"

	printf '#include <%s>\nint odr_one(void);\nint odr_one(void){return 0;}\n' "$rel" > "$c1"
	printf '#include <%s>\nint odr_two(void);\nint odr_two(void){return 1;}\n' "$rel" > "$c2"

	# If either TU will not compile on its own, that is Tier 0.1's finding,
	# not ours. Stay silent so failures are reported exactly once.
	"$CC" "${base_flags[@]}" "-DTARGET=$target" "$c1" -o "$o1" >/dev/null 2>&1 || return 0
	"$CC" "${base_flags[@]}" "-DTARGET=$target" "$c2" -o "$o2" >/dev/null 2>&1 || return 0

	local output status
	output=$("$LD" -nostdlib -z noexecstack "$o1" "$o2" -o /dev/null 2>&1 \
		| grep -v 'cannot find entry symbol')
	status=$?

	if [[ -n $output ]]; then
		fail_item "lib/$rel [TARGET=$target]" "$output"
	else
		pass_item
	fi
}

targets_for() {
	case "$1" in
		sub/*) printf 'MEGACD\n' ;;
		*)     printf 'MEGACD\n' ;;
	esac
}

info "Tier 0.4 - one-definition-rule (two TUs, linked)"

mapfile -t headers < <(cd "$LIB_PATH" && find . -name '*.h' -type f -printf '%P\n' | sort)

for rel in "${headers[@]}"; do
	while read -r target; do
		[[ -n $target ]] && check_odr "$rel" "$target"
	done < <(targets_for "$rel")
done

summarise "ODR"

#!/usr/bin/env bash
#
# [ M E G A D E V ]
#
# Tier 0.2 - assemble every assembly source.
#
# Assembly in lib/ is compiled through the C preprocessor, exactly as
# megadev.make:158 does it. Assembling each file in isolation catches invalid
# mnemonics, bad operand syntax, dangling #includes and undefined macros.
#
# Unresolved *symbols* are fine here - those are the linker's business. We are
# only asserting that the assembler accepts the source.
#
# See SPEC.md section 6.

source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

require_toolchain
mkdir -p "$CHECK_TMP"

base_flags=(
	-m68000
	-imacros build.def.h
	-I "$LIB_PATH"
	-Wa,-I"$LIB_PATH"
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
	-Wa,--register-prefix-optional
	-Wa,--bitwise-or
	-x assembler-with-cpp
	-c
)

targets_for() {
	case "$1" in
		sub/*)  printf 'MEGACD\n' ;;
		*)      printf 'MEGACD\n' ;;
	esac
}

check_asm() {
	local rel="$1" target="$2"
	local obj="$CHECK_TMP/${rel//\//_}.o"

	local output status
	output=$("$CC" "${base_flags[@]}" "-DTARGET=$target" \
		"$LIB_PATH/$rel" -o "$obj" 2>&1)
	status=$?

	if ((status == 0)); then
		pass_item
	else
		fail_item "lib/$rel [TARGET=$target]" "$output"
	fi
}

info "Tier 0.2 - assemble sources ($CC)"

# Both .s and .macro.s / .macros.s are covered. Macro files emit no code, so
# assembling them standalone is a pure syntax check of the macro bodies only if
# they are also expanded - which they are not. They are still checked here so
# that dangling #includes and preprocessor errors surface.
mapfile -t sources < <(cd "$LIB_PATH" && find . -name '*.s' -type f -printf '%P\n' | sort)

if ((${#sources[@]} == 0)); then
	err "no assembly sources found under $LIB_PATH"
	exit 1
fi

EXCLUDE_FILE="$MEGADEV_ROOT/tools/check/asm-exclude.txt"

excluded_reason() {
	[[ -f $EXCLUDE_FILE ]] || return 1
	grep -v '^#' "$EXCLUDE_FILE" | sed '/^$/d' \
		| awk -F'|' -v want="$1" '$1 == want { print $2; found=1 } END { exit !found }'
}

for rel in "${sources[@]}"; do
	if reason=$(excluded_reason "$rel"); then
		skip_item "lib/$rel - $reason"
		continue
	fi
	while read -r target; do
		[[ -n $target ]] && check_asm "$rel" "$target"
	done < <(targets_for "$rel")
done

summarise "Assembly"

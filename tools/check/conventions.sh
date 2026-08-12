#!/usr/bin/env bash
#
# [ M E G A D E V ]
#
# Tier 1 - convention lint.
#
# Mechanically enforces the invariants in SPEC.md section 2:
#
#   INV-1  .def.h files contain only preprocessor directives and comments.
#          This is the load-bearing one: .def.h is included from assembly
#          sources, so anything that is not legal in both languages breaks it.
#   INV-4  Include guard name is derived from the file's path.
#   INV-5  An @file doc tag names the file it appears in.
#
# The library predates these rules, so the gate uses a BASELINE of accepted
# existing violations. Anything not in the baseline fails. As violations are
# fixed, remove them from the baseline - it should only ever shrink.
#
# Regenerate the baseline (only when you mean to) with:
#   tools/check/conventions.sh --write-baseline

source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

BASELINE="$MEGADEV_ROOT/tools/check/baseline.txt"
WRITE_BASELINE=0
[[ ${1:-} == --write-baseline ]] && WRITE_BASELINE=1

FINDINGS=()

record() {
	# record <rule> <path> <detail>
	FINDINGS+=("$1|$2|$3")
}

# --- INV-1: .def.h is preprocessor-only ------------------------------------
#
# Strip comments and line continuations, then assert every remaining non-blank
# line is a preprocessor directive.
check_def_h() {
	local f="$1" rel="$2"
	local bad
	bad=$(awk '
		BEGIN { in_block = 0; cont = 0 }
		{
			line = $0
			# strip block comments, tracking multi-line state
			while (1) {
				if (in_block) {
					idx = index(line, "*/")
					if (idx == 0) { line = ""; break }
					line = substr(line, idx + 2); in_block = 0
				} else {
					idx = index(line, "/*")
					if (idx == 0) break
					rest = substr(line, idx + 2)
					line = substr(line, 1, idx - 1)
					in_block = 1
					# handle /* ... */ on one line
					jdx = index(rest, "*/")
					if (jdx > 0) { line = line substr(rest, jdx + 2); in_block = 0 }
					else break
				}
			}
			sub(/\/\/.*$/, "", line)          # line comments
			gsub(/^[ \t]+|[ \t]+$/, "", line) # trim

			was_cont = cont
            cont = (line ~ /\\$/) ? 1 : 0

			if (line == "") next
			if (was_cont) next                 # continuation of a directive
			if (line ~ /^#/) next              # a directive
			printf "%d: %s\n", FNR, line
		}
	' "$f")

	if [[ -n $bad ]]; then
		while IFS= read -r l; do
			record INV-1 "$rel" "non-preprocessor content -> $l"
		done <<< "$bad"
	fi
}

# --- INV-4: include guard matches path -------------------------------------
#
# Expected: MEGADEV__<PATH>_<NAME>_<EXT>, path relative to lib/, uppercased.
#   main/gate_arr.def.h -> MEGADEV__MAIN_GATE_ARR_DEF_H
#   types.h             -> MEGADEV__TYPES_H
expected_guard() {
	local rel="$1"
	printf 'MEGADEV__%s' "$(printf '%s' "$rel" \
		| tr '[:lower:]' '[:upper:]' \
		| sed -e 's#[/.]#_#g')"
}

check_guard() {
	local f="$1" rel="$2" display="$3"
	local want found
	want="$(expected_guard "$rel")"

	if grep -qE '^[[:space:]]*#pragma[[:space:]]+once' "$f"; then
		record INV-4 "$display" "uses #pragma once (see SPEC.md OD-3)"
		return
	fi

	found=$(grep -m1 -oE '^[[:space:]]*#ifndef[[:space:]]+[A-Za-z_][A-Za-z0-9_]*' "$f" \
		| awk '{print $2}')

	if [[ -z $found ]]; then
		record INV-4 "$display" "no include guard"
	elif [[ $found != "$want" ]]; then
		record INV-4 "$display" "guard is $found, expected $want"
	fi
}

# --- STYLE-1: canonical M68k mnemonics -------------------------------------
#
# GNU as accepts `mov` as an alias for `move` (both assemble to the identical
# encoding - verified: `mov.l (a0)+,(a1)+` and `move.l (a0)+,(a1)+` both emit
# 0x22d8). It is still not the canonical mnemonic, and mixing the two within
# one file is confusing. Applies to inline asm in .h files as well as .s.
check_mnemonics() {
	local f="$1" display="$2"
	local hits
	# `mov` may follow start-of-line, whitespace, a quote/backslash (inline asm
	# strings) or a local label colon, as in `0:mov.l (a0)+,(a1)+`.
	hits=$(grep -nE '(^|[[:space:]"\\:])mov\.[bwl][[:space:]]' "$f" \
		| grep -vE '^[0-9]+:[[:space:]]*(\*|//|#[^0-9])' || true)
	[[ -z $hits ]] && return
	while IFS= read -r l; do
		record STYLE-1 "$display" "use move, not mov -> ${l}"
	done <<< "$hits"
}

# --- INV-5: @file tag names this file --------------------------------------
check_file_tag() {
	local f="$1" rel="$2" base tag
	base="$(basename "$rel")"
	tag=$(grep -m1 -oE '@file[[:space:]]+[^[:space:]*]+' "$f" | awk '{print $2}')
	[[ -z $tag ]] && return                      # absent tag is not a violation here
	[[ "$(basename "$tag")" == "$base" ]] && return
	record INV-5 "$rel" "@file says '$tag'"
}

# --- run --------------------------------------------------------------------

info "Tier 1 - convention lint"

while IFS= read -r rel; do
	f="$LIB_PATH/$rel"
	case "$rel" in
		*.def.h) check_def_h "$f" "lib/$rel" ;;
	esac
	case "$rel" in
		*.h) check_guard "$f" "$rel" "lib/$rel" ;;
	esac
	case "$rel" in
		*.h|*.s) check_mnemonics "$f" "lib/$rel" ;;
	esac
	check_file_tag "$f" "lib/$rel"
done < <(cd "$LIB_PATH" && find . \( -name '*.h' -o -name '*.s' -o -name '*.c' \) -type f -printf '%P\n' | sort)

# Sorted, stable representation for baseline comparison
mapfile -t sorted_findings < <(printf '%s\n' "${FINDINGS[@]+"${FINDINGS[@]}"}" | LC_ALL=C sort -u | sed '/^$/d')

if ((WRITE_BASELINE)); then
	{
		printf '# Accepted pre-existing convention violations.\n'
		printf '# Generated by tools/check/conventions.sh --write-baseline\n'
		printf '# This list should only ever SHRINK. See SPEC.md section 3.\n'
		printf '%s\n' "${sorted_findings[@]+"${sorted_findings[@]}"}"
	} > "$BASELINE"
	info "wrote baseline: ${#sorted_findings[@]} accepted violations -> $BASELINE"
	exit 0
fi

if [[ -f $BASELINE ]]; then
	mapfile -t accepted < <(grep -v '^#' "$BASELINE" | sed '/^$/d' | LC_ALL=C sort -u)
else
	warn "no baseline at $BASELINE - every finding will be reported as new"
	accepted=()
fi

# New findings = current minus baseline. Fixed = baseline minus current.
mapfile -t new_findings < <(
	comm -23 \
		<(printf '%s\n' "${sorted_findings[@]+"${sorted_findings[@]}"}" | sed '/^$/d') \
		<(printf '%s\n' "${accepted[@]+"${accepted[@]}"}" | sed '/^$/d')
)
mapfile -t fixed_findings < <(
	comm -13 \
		<(printf '%s\n' "${sorted_findings[@]+"${sorted_findings[@]}"}" | sed '/^$/d') \
		<(printf '%s\n' "${accepted[@]+"${accepted[@]}"}" | sed '/^$/d')
)

for finding in "${new_findings[@]+"${new_findings[@]}"}"; do
	[[ -z $finding ]] && continue
	IFS='|' read -r rule path detail <<< "$finding"
	fail_item "$rule $path" "$detail"
done

CHECK_PASS=${#accepted[@]}

if ((${#fixed_findings[@]} > 0)); then
	printf '\n%s%d baseline violation(s) no longer present%s - remove them from\n' \
		"$C_GREEN" "${#fixed_findings[@]}" "$C_CLEAR"
	printf 'tools/check/baseline.txt with --write-baseline:\n'
	printf '  %s\n' "${fixed_findings[@]}"
fi

summarise "Conventions"

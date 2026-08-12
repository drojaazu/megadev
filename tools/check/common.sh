#!/usr/bin/env bash
#
# [ M E G A D E V ]
#
# Shared helpers for the verification gate. See SPEC.md section 6.
#
# Sourced by the check scripts; not executable on its own.

set -uo pipefail

# Repo root, regardless of where we were invoked from
MEGADEV_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
export MEGADEV_ROOT
export MEGADEV_PATH="${MEGADEV_PATH:-$MEGADEV_ROOT}"

# Overridable so the gate can be pointed at a fixture tree for self-testing
LIB_PATH="${LIB_PATH:-$MEGADEV_ROOT/lib}"
M68K_PREFIX="${M68K_PREFIX:-m68k-linux-gnu-}"

# Deliberately NOT inheriting a bare $CC. Many environments export CC pointing
# at the host compiler; picking that up would run the gate against x86 and
# report meaningless results. Override explicitly with MEGADEV_CC if needed.
CC="${MEGADEV_CC:-${M68K_PREFIX}gcc}"

# Where generated scratch files go. Kept out of the source tree.
CHECK_TMP="${CHECK_TMP:-$MEGADEV_ROOT/build-check}"

if [[ -t 1 ]]; then
	C_CLEAR=$'\033[0m'; C_BOLD=$'\033[1m'; C_RED=$'\033[1;31m'
	C_GREEN=$'\033[1;32m'; C_YELLOW=$'\033[1;33m'; C_CYAN=$'\033[1;36m'
else
	C_CLEAR=''; C_BOLD=''; C_RED=''; C_GREEN=''; C_YELLOW=''; C_CYAN=''
fi

CHECK_PASS=0
CHECK_FAIL=0
CHECK_SKIP=0
FAILED_ITEMS=()

info() { printf '%s==>%s %s\n' "$C_CYAN" "$C_CLEAR" "$*"; }
warn() { printf '%sWARN%s %s\n' "$C_YELLOW" "$C_CLEAR" "$*"; }
err()  { printf '%sFAIL%s %s\n' "$C_RED" "$C_CLEAR" "$*"; }

pass_item() { CHECK_PASS=$((CHECK_PASS + 1)); }

fail_item() {
	# fail_item <item> [detail...]
	local item="$1"; shift
	CHECK_FAIL=$((CHECK_FAIL + 1))
	FAILED_ITEMS+=("$item")
	err "$item"
	if (($# > 0)); then
		# Indent the compiler's own diagnostics so they read as subordinate
		printf '%s\n' "$*" | sed 's/^/       /'
	fi
}

skip_item() {
	CHECK_SKIP=$((CHECK_SKIP + 1))
	warn "skipped: $*"
}

require_toolchain() {
	if ! command -v "$CC" >/dev/null 2>&1; then
		err "cross compiler '$CC' not found."
		printf '       The gate must run inside the devcontainer, or with\n'
		printf '       M68K_PREFIX / MEGADEV_CC pointing at an m68k toolchain.\n'
		exit 127
	fi

	# Assert it really is an m68k compiler. Without this, a host compiler that
	# happens to be on PATH would produce confident, meaningless results.
	local probe="${CHECK_TMP}/toolchain_probe.c"
	mkdir -p "$CHECK_TMP"
	printf 'int main(void){return 0;}\n' > "$probe"
	if ! "$CC" -m68000 -fsyntax-only "$probe" >/dev/null 2>&1; then
		err "'$CC' does not accept -m68000; it is not an m68k compiler."
		printf '       Refusing to run - results would be meaningless.\n'
		exit 127
	fi
	rm -f "$probe"
}

# Print the summary and return a shell-appropriate exit status.
summarise() {
	local label="$1"
	printf '\n%s%s%s: %s%d passed%s' \
		"$C_BOLD" "$label" "$C_CLEAR" "$C_GREEN" "$CHECK_PASS" "$C_CLEAR"
	((CHECK_FAIL > 0)) && printf ', %s%d failed%s' "$C_RED" "$CHECK_FAIL" "$C_CLEAR"
	((CHECK_SKIP > 0)) && printf ', %d skipped' "$CHECK_SKIP"
	printf '\n'

	if ((CHECK_FAIL > 0)); then
		printf '\n%sFailed:%s\n' "$C_BOLD" "$C_CLEAR"
		printf '  %s\n' "${FAILED_ITEMS[@]}"
		return 1
	fi
	return 0
}

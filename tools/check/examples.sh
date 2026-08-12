#!/usr/bin/env bash
#
# [ M E G A D E V ]
#
# Tier 0.3 - build every example and the project template end to end.
#
# This is the only check that exercises the linker scripts, the module header
# emitted by cfg/module_mmd.ld, the IP/SP boot chain and ISO mastering.
#
# Note the explicit `make init`: build/ and disc/ are gitignored and are not
# created by megadev.make, so a fresh clone cannot `make` without it. That is
# BACKLOG MAKE-2 - when it is fixed, drop the init step here and this gate
# will prove the fix.

source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

require_toolchain

if ! command -v mkisofs >/dev/null 2>&1; then
	warn "mkisofs not found - Mega CD projects will fail at the ISO step"
fi

build_project() {
	local dir="$1" name="$2"
	local output status

	if [[ ! -f "$dir/makefile" ]]; then
		skip_item "$name (no makefile)"
		return
	fi

	output=$(make -C "$dir" MEGADEV_PATH="$MEGADEV_ROOT" init 2>&1 \
		&& make -C "$dir" MEGADEV_PATH="$MEGADEV_ROOT" 2>&1)
	status=$?

	if ((status == 0)); then
		pass_item
		printf '  %sok%s   %s\n' "$C_GREEN" "$C_CLEAR" "$name"
	else
		fail_item "$name" "$output"
	fi
}

clean_project() {
	local dir="$1"
	[[ -f "$dir/makefile" ]] || return 0
	make -C "$dir" MEGADEV_PATH="$MEGADEV_ROOT" clean >/dev/null 2>&1 || true
}

info "Tier 0.3 - build examples and template"

projects=()
while IFS= read -r d; do
	projects+=("$d")
done < <(find "$MEGADEV_ROOT/examples" -mindepth 1 -maxdepth 1 -type d | sort)
projects+=("$MEGADEV_ROOT/new_project")

if [[ ${CHECK_CLEAN_FIRST:-1} == 1 ]]; then
	for dir in "${projects[@]}"; do clean_project "$dir"; done
fi

for dir in "${projects[@]}"; do
	build_project "$dir" "$(basename "$dir")"
done

summarise "Projects"

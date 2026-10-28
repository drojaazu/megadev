# [ M E G A D E V ]   a Sega Mega CD devkit
#
# Maintainer tooling for the megadev repository itself.
#
# This file is NOT part of the kit a project uses. Projects include
# $(MEGADEV_PATH)/megadev.make and never read this Makefile, so nothing here
# can collide with a project's own targets. Everything a project sees lives in
# megadev.make.
#
# Targets:
#   make check   Verification gate. Must pass before a branch merges.
#   make clean   Remove the build output the gate produced.

MEGADEV_PATH := $(CURDIR)
export MEGADEV_PATH

PROJECTS := $(wildcard examples/*/) new_project/

.PHONY: check check-projects clean $(PROJECTS)

check: check-projects
	@printf '\ncheck: all passed\n'

# ---------------------------------------------------------------------------
# Loop 1: every example and the project template builds from clean, through
# its own makefile, exactly as a user project would.
# ---------------------------------------------------------------------------
check-projects: $(PROJECTS)

$(PROJECTS):
	@printf '== build %s\n' '$@'
	@$(MAKE) -s -C '$@' clean >/dev/null
	@$(MAKE) -s -C '$@' >/dev/null

clean:
	@for p in $(PROJECTS); do $(MAKE) -s -C "$$p" clean >/dev/null; done

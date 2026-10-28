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
HEADERS  := $(shell find lib -name '*.h' | sort)

# Hardware targets a header must be valid for (see lib/build.def.h).
TARGETS := MEGACD MEGADRIVE

CC := $(M68K_PREFIX)gcc
CHECK_DIR := build-check

# The flags megadev.make gives a project, minus the project-specific header
# strings, which no library header depends on.
CHECK_FLAGS := -m68000 -imacros build.def.h -Ilib -Wa,-Ilib \
	-DPROJECT_ID=check -DREGION=US -DVIDEO=NTSC \
	-fno-builtin -Wall -Wextra -Wno-main -Wa,--register-prefix-optional

ASM_SOURCES := $(shell find lib -name '*.s' | sort)

# Assembly files that by contract cannot assemble on their own. Each entry
# needs a reason. This list may only shrink.
#   lib/cd_boot.s    .incbin of the project's built ip.bin and sp.bin
#   lib/md_header.s  needs the project's fixed-length header strings; any
#                    default would hide a string that overruns its field.
#                    Exercised by the cart example build in loop 1.
ASM_STANDALONE_EXCLUDE := lib/cd_boot.s lib/md_header.s
ASM_STANDALONE := $(filter-out $(ASM_STANDALONE_EXCLUDE),$(ASM_SOURCES))

.PHONY: check check-projects check-headers check-asm clean $(PROJECTS)

check: check-projects check-headers check-asm
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

# ---------------------------------------------------------------------------
# Loop 2: every library header compiles on its own, for every hardware target.
# Catches a header that only works when something else was included first.
# ---------------------------------------------------------------------------
check-headers:
	@mkdir -p $(CHECK_DIR)
	@fail=0; \
	for h in $(HEADERS); do \
	  for t in $(TARGETS); do \
	    printf '#include <%s>\n' "$${h#lib/}" > $(CHECK_DIR)/header.c; \
	    if ! $(CC) $(CHECK_FLAGS) -DTARGET=$$t -fsyntax-only -Werror \
	         $(CHECK_DIR)/header.c 2> $(CHECK_DIR)/header.err; then \
	      printf 'FAIL header %s (%s)\n' "$$h" "$$t"; cat $(CHECK_DIR)/header.err; fail=1; \
	    fi; \
	  done; \
	done; \
	n=$$(echo $(HEADERS) | wc -w); \
	[ $$fail = 0 ] && printf '== headers: %s compile alone (%s)\n' "$$n" "$(TARGETS)" || exit 1

# ---------------------------------------------------------------------------
# Loop 3: every library assembly file assembles on its own. Catches files no
# project uses; three such files sat broken on master for years.
# ---------------------------------------------------------------------------
check-asm:
	@mkdir -p $(CHECK_DIR)
	@fail=0; \
	for s in $(ASM_STANDALONE); do \
	  if ! $(CC) $(CHECK_FLAGS) -DTARGET=MEGACD -Wa,--bitwise-or \
	       -x assembler-with-cpp -c "$$s" -o $(CHECK_DIR)/asm.o 2> $(CHECK_DIR)/asm.err; then \
	    printf 'FAIL asm %s\n' "$$s"; cat $(CHECK_DIR)/asm.err; fail=1; \
	  fi; \
	done; \
	n=$$(echo $(ASM_STANDALONE) | wc -w); x=$$(echo $(ASM_STANDALONE_EXCLUDE) | wc -w); \
	[ $$fail = 0 ] && printf '== asm: %s assemble alone, %s excluded by contract\n' "$$n" "$$x" || exit 1

clean:
	@for p in $(PROJECTS); do $(MAKE) -s -C "$$p" clean >/dev/null; done
	@rm -rf $(CHECK_DIR)

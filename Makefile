# [ M E G A D E V ]
#
# Makefile for developing MEGADEV ITSELF.
#
# This is not the build system for your own projects - that is megadev.make,
# which your project's own makefile includes. See docs/manual.md.
#
# The verification gate is described in SPEC.md section 6. Run it inside the
# devcontainer, where the m68k toolchain lives:
#
#   make check          everything
#   make check-headers  Tier 0.1 - every header compiles standalone
#   make check-asm      Tier 0.2 - every assembly source assembles
#   make check-examples Tier 0.3 - every example and the template builds
#   make lint           Tier 1   - convention lint
#   make format-check   clang-format, no changes written
#
# CI runs `make check` and nothing else, so the local and CI paths cannot drift.

CHECK_DIR := tools/check

.PHONY: all check check-headers check-link check-asm check-examples lint format format-check docs clean-check help

all: help

help:
	@printf 'MEGADEV development targets:\n\n'
	@printf '  check           run the full verification gate\n'
	@printf '  check-headers   Tier 0.1 - per-header compile\n'
	@printf '  check-link      Tier 0.4 - one-definition-rule across two TUs\n'
	@printf '  check-asm       Tier 0.2 - assemble every .s\n'
	@printf '  check-examples  Tier 0.3 - build examples and template\n'
	@printf '  lint            Tier 1   - convention lint\n'
	@printf '  format-check    clang-format --dry-run\n'
	@printf '  format          apply clang-format in place\n'
	@printf '  docs            generate Doxygen output\n'
	@printf '  clean-check     remove gate scratch files\n\n'
	@printf 'Run inside the devcontainer - these need the m68k toolchain.\n'

# Ordered cheapest-first so an obvious breakage fails fast.
check: lint check-headers check-link check-asm check-examples
	@printf '\n\033[1;32mAll checks passed.\033[0m\n'

check-headers:
	@$(CHECK_DIR)/headers.sh

check-link:
	@$(CHECK_DIR)/link.sh

check-asm:
	@$(CHECK_DIR)/asm.sh

check-examples:
	@$(CHECK_DIR)/examples.sh

lint:
	@$(CHECK_DIR)/conventions.sh

format-check:
	@find lib -name '*.h' -o -name '*.c' | xargs clang-format --dry-run --Werror

format:
	@find lib -name '*.h' -o -name '*.c' | xargs clang-format -i

# NOTE: the Doxyfile is currently broken - see BACKLOG.md DOC-1. This target
# will not produce complete output until that is fixed.
docs:
	@doxygen Doxyfile

clean-check:
	@rm -rf build-check

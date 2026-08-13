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
#   make check-link     Tier 0.4 - one-definition-rule across two TUs
#   make check-asm      Tier 0.2 - every assembly source assembles
#   make check-projects Tier 0.3 - every example and the template builds
#   make lint           Tier 1   - convention lint
#   make test           unit tests for the gate itself (no toolchain needed)
#
# CI runs `make check` and nothing else, so the local and CI paths cannot drift.

CHECK := tools/check/check.py

.PHONY: all check check-headers check-asserts check-link check-asm check-symbols check-projects lint test \
        baseline format format-check docs clean-check help

all: help

help:
	@printf 'MEGADEV development targets:\n\n'
	@printf '  check           run the full verification gate\n'
	@printf '  check-headers   Tier 0.1 - per-header compile\n'
	@printf '  check-asserts   Tier 1.5 - compile-time semantic assertions\n'
	@printf '  check-link      Tier 0.4 - one-definition-rule across two TUs\n'
	@printf '  check-asm       Tier 0.2 - assemble every .s\n'
	@printf '  check-symbols   Tier 0.5 - symbol resolution across lib\n'
	@printf '  check-projects  Tier 0.3 - build examples and template\n'
	@printf '  lint            Tier 1   - convention lint\n'
	@printf '  test            unit tests for the gate (no toolchain needed)\n'
	@printf '  baseline        re-accept current lint findings as the baseline\n'
	@printf '  format-check    clang-format --dry-run\n'
	@printf '  format          apply clang-format in place\n'
	@printf '  docs            generate Doxygen output\n'
	@printf '  clean-check     remove gate scratch files\n\n'
	@printf 'Run inside the devcontainer - these need the m68k toolchain.\n'

check:
	@$(CHECK) all

check-headers:
	@$(CHECK) headers

check-asserts:
	@$(CHECK) asserts

check-link:
	@$(CHECK) link

check-asm:
	@$(CHECK) asm

check-symbols:
	@$(CHECK) symbols

check-projects:
	@$(CHECK) projects

lint:
	@$(CHECK) conventions

baseline:
	@$(CHECK) conventions --write-baseline

test:
	@python3 -m unittest discover -s tools/check/tests -v

format-check:
	@find lib -name '*.h' -o -name '*.c' | xargs clang-format --dry-run --Werror

format:
	@find lib -name '*.h' -o -name '*.c' | xargs clang-format -i

docs:
	@doxygen Doxyfile

clean-check:
	@rm -rf build-check

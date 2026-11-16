#
# [ M E G A D E V ]   a Sega Mega CD devkit
#

# Compiler selection and check, shared by megadev.make (projects) and the
# kit's own Makefile (make check), so both use the same default.
#
# The kit is built and tested with the Megadev toolchain: a bare-metal
# m68k-elf GCC built for the 68000 (tools/build-toolchain.sh). Put its bin
# directory on PATH, or set M68K_PREFIX to the full path prefix of another
# compiler, e.g. M68K_PREFIX=/opt/megadev-toolchain/bin/m68k-elf-
M68K_PREFIX?=m68k-elf-

CC:=$(M68K_PREFIX)gcc

# Goals that never run the compiler skip the check below.
MEGADEV_NO_CC_GOALS:=clean docs print-%

# The check runs once per build: the first make to run it exports
# MEGADEV_TOOLCHAIN_CHECKED, so recursive makes stay quiet.
ifeq ($(MEGADEV_TOOLCHAIN_CHECKED),)
ifneq ($(if $(MAKECMDGOALS),$(filter-out $(MEGADEV_NO_CC_GOALS),$(MAKECMDGOALS)),all),)

MEGADEV_CC_MACHINE:=$(shell $(CC) -dumpmachine 2>/dev/null)

ifeq ($(MEGADEV_CC_MACHINE),)
$(error $(CC) not found. Build the Megadev toolchain with tools/build-toolchain.sh and put its bin directory on PATH, or set M68K_PREFIX to the compiler to use (docs/manual.md, "Megadev Toolchain"))
endif

# An m68k-linux compiler assumes a 68020 or later, which allows word and long
# accesses at odd addresses, and nothing turns that off (KB-68). The build
# goes on, since it is the user's choice; change warning to error below to
# refuse such a compiler instead.
define MEGADEV_LINUX_GCC_MESSAGE
$(CC) targets $(MEGADEV_CC_MACHINE).
  Linux m68k compilers assume the CPU allows word and long accesses at odd
  addresses. The 68000 does not: such code stops with an address error on a
  real console, though emulators that do not check alignment run it. Megadev
  supports the Megadev toolchain (tools/build-toolchain.sh). See
  docs/manual.md, "Why m68k-elf, and not your distribution's m68k-linux-gnu".
  Set MEGADEV_ALLOW_LINUX_GCC=1 to hide this.
endef

ifneq ($(findstring linux,$(MEGADEV_CC_MACHINE)),)
ifneq ($(MEGADEV_ALLOW_LINUX_GCC),1)
$(warning $(MEGADEV_LINUX_GCC_MESSAGE))
endif
endif

export MEGADEV_TOOLCHAIN_CHECKED:=1
endif
endif

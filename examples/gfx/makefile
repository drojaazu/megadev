################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

# This will be used for the ISO volume name
PROJECT_NAME=gfx

# the path to your source code
# (relative to the location of the makefile)
SRC_PATH:=src

# Resources path
# This is where graphics, audio and any other non-code content should be
# stored.
RES_PATH:=res

# Build path
# This is where compile time build artifacts will be stored, including
# symbol maps
BUILD_PATH:=build

# Optional compiler flags
# Here you can add any additional compiler flags
OPT_FLAGS:=-O1 -fstack-usage -fconserve-stack

# Z80 CPU code path
# (Z80 building is not currently supported... someday!)
#Z80_SRC_PATH:=z80_src

# Path to the Megadev directory
#MEGADEV_PATH:=/opt/megadev
MEGADEV_PATH:=/mnt/aine/Projects/dev/megadev

include $(MEGADEV_PATH)/megadev.make

.PHONY: disc clean

disc: disc/cyber.mmd disc.iso

clean:
	@rm disc.iso disc/*.mmd disc/*.smd build/*

disc/cyber.mmd: mmd_layout.s cybercity.c cybercity.res.s

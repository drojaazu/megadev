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

# Disc paths
# A space-delimited list of subdiorectories representing the content for each
# disc in your project
# This is used by the `init` rule to create directories before building
DISC_PATHS:=disc

# Optional compiler flags
# Here you can add any additional compiler flags
OPT_FLAGS:=-O1 -fconserve-stack

# (Z80 building is not currently supported... someday!)
# Path to the Megadev directory
MEGADEV_PATH:=/opt/megadev

include $(MEGADEV_PATH)/megadev.make

.PHONY: disc clean

disc: \
	init \
	disc/cyber.mmd \
	disc.iso

clean:
	@rm -f disc.iso disc/*.mmd disc/*.smd build/*

disc/cyber.mmd: \
	mmd_layout.s \
	cybercity.c \
	cybercity.res.s

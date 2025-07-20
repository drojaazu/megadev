################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

# This will be used for the ISO volume name
PROJECT_NAME?=gfx

## Directory paths are relative to the location of the makefile
## (which should be the root of your project directory)

# Source code path
# (relative to the location of the project makefile)
SRC_PATH:=src

# Resources path
# This is where graphics, audio and any other non-executable content should be
# stored.
RES_PATH:=res

# Build path
# This is where compile time build artifacts will be stored, including
# symbol maps
BUILD_PATH:=build

# Output path
# This will be the location of the final output Mega CD ISO and MD ROMs
OUT_PATH:=out

# Additional C compiler flags
CC_FLAGS:=-O1 -fconserve-stack -fomit-frame-pointer -fno-gcse

# Additional assembler flags
AS_FLAGS:=

# Additional linker flags
LD_FLAGS:=


# Path to the Megadev directory
MEGADEV_PATH?=/opt/megadev

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
	cybercity_init.s \
	cybercity.c \
	cybercity.res.s

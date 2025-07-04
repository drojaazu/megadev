################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

# This will be used for filename output
PROJECT_NAME=pcm

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
CC_FLAGS:=-O1 -fconserve-stack

# Additional assembler flags
AS_FLAGS:=

# Additional linker flags
LD_FLAGS:=


# Path to the Megadev directory
MEGADEV_PATH?=/opt/megadev

include $(MEGADEV_PATH)/megadev.make

.PHONY: disc clean copy_res

disc: \
	init \
	copy_res \
	disc/ipx.mmd \
	disc/spx.smd \
	disc.iso

clean:
	@rm -f disc.iso disc/*.mmd disc/*.smd build/*

copy_res:
	@cp -rf $(RES_PATH)/* disc

disc/ipx.mmd: \
	ipx_layout.s \
	ipx_init.s \
	ipx.c

disc/spx.smd: \
	spx_layout.s \
	spx.c \
	sub/pcm.s \
	pcmplay_v2.s \
	$(BUILD_PATH)/sp.bin

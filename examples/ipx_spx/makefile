################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

# This will be used for filename output
PROJECT_NAME=ipx_spx

## Directory paths are relative to the location of the makefile
## (which should be the root of your project directory)

# Source code path
SRC_PATH:=src

# Disc staging path
# This is the staging directory for fiels that will appear on the final iso for
# each Mega CD disc. Inside this path will be a subdirectory matching the stem of
# each .iso.def in your project. Any files in these subdirectories will be included
# in the iso image.
# (This option is ignored for cartridge ROM projects.)
DISCS_PATH:=discs

# Resources path
# This is where graphics, audio and any other non-executable content should be
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

# Path to the Megadev directory
MEGADEV_PATH:=/opt/megadev

include $(MEGADEV_PATH)/megadev.make

.PHONY: disc clean

disc: \
	init \
	disc/ipx.mmd \
	disc/ex1.mmd \
	disc/ex2.mmd \
	disc/ex3.mmd \
	disc/spx.smd \
	disc.iso

clean:
	@rm -f disc.iso disc/*.mmd disc/*.smd build/*

disc/ipx.mmd: ipx_layout.s ipx_init.s ipx.c $(BUILD_PATH)/ip.bin

disc/ex1.mmd: ex1.c shared_mmd_layout.s ex1_res.s disc/ipx.mmd

disc/ex2.mmd: ex2.c shared_mmd_layout.s ex2_res.s disc/ipx.mmd

disc/ex3.mmd: ex3.c shared_mmd_layout.s ex3_res.s disc/ipx.mmd

disc/spx.smd: spx_layout.s spx.c $(BUILD_PATH)/sp.bin


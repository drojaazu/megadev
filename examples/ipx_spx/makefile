################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

# This will be used for filename output
PROJECT_ID=ipx_spx

TARGET=MEGACD

PROJECT_NAME="IPX SPX EXAMPLE"

REGION=US

VIDEO=NTSC

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
	disc/ipx.mmd \
	disc/ex1.mmd \
	disc/ex2.mmd \
	disc/ex3.mmd \
	disc/spx.smd \
	disc.iso

clean:
	@rm -f disc.iso disc/*.mmd disc/*.smd build/*

disc/ipx.mmd: \
	ipx_layout.s \
	main/ipx_init.s \
	ipx.c \
	io.macros.s \
	$(BUILD_PATH)/ip.bin

disc/ex1.mmd: \
	shared_mmd_layout.s \
	std_init.s \
	ex1.c \
	ex1_res.s \
	disc/ipx.mmd

disc/ex2.mmd: \
	shared_mmd_layout.s \
	std_init.s \
	ex2.c \
	ex2_res.s \
	disc/ipx.mmd

disc/ex3.mmd: \
	shared_mmd_layout.s \
	std_init.s \
	ex3.c \
	ex3_res.s \
	disc/ipx.mmd

disc/spx.smd: \
	spx_layout.s \
	spx.c \
	$(BUILD_PATH)/sp.bin

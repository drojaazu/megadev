################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

## Project ID
# ASCII text only; maximum of 11 characters
# *** REQUIRED!***
PROJECT_ID=ipx_spx

## Build Target
# Determines for which hardware this project is intended
# Valid values: MEGADRIVE, MEGACD
# *** REQUIRED!***
TARGET=MEGACD

## Project Name
# ASCII text only; maximum of 48 characters
PROJECT_NAME="IPX SPX EXAMPLE"

## Hardware Region
# This is *very important* for Mega CD builds as it determines which security code to use. It can be changed at compile
# time by setting the variable when running make.
REGION=US

## Video Output
# Specifies the default video output for this game. It can be changed at compile time by setting the variable when
# running make.
# Valid values: NTSC, PAL
VIDEO=NTSC

## The following directory paths are relative to the location of this makefile
## (which should ideally be in the root of your project directory)

## Source code path
# This is the path to all code that will be referenced or directly copmiled.
# *** REQUIRED!***
SRC_PATH:=src

# Resources path
# This is where graphics, audio and any other non-executable content should be stored.
# *** REQUIRED!***
RES_PATH:=res

# Build path
# This is where compile time build artifacts will be stored, including symbol maps
# *** REQUIRED!***
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
# *** REQUIRED!***
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

################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

## Project ID
# ASCII text only; maximum of 11 characters
# *** REQUIRED!***
PROJECT_ID:=bram

## Build Target
# Determines for which hardware this project is intended
# Valid values: MEGADRIVE, MEGACD
# *** REQUIRED!***
TARGET:=MEGACD

## Project Name
# ASCII text only; maximum of 48 characters
PROJECT_NAME:="BRAM USAGE EXAMPLE"

HEADER_SOFTWARE_ID:="MEGADEV DEMO"

## Hardware Region
# This is *very important* for Mega CD builds as it determines which security code to use. It can be changed at compile
# time by setting the variable when running make.
REGION?=US

## Video Output
# Specifies the default video output for this game. It can be changed at compile time by setting the variable when
# running make.
# Valid values: NTSC, PAL
VIDEO?=NTSC

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

DISC_PATH:=disc

## Mode 1 build
# `make MODE1=1` builds $(PROJECT_ID).cart instead of the disc: a cartridge
# that runs this example on a Mega CD with no disc in the drive (docs/mode1.md).
# Its files are built in build/mode1, and the modules are embedded in the
# cartridge instead of placed on a disc.
ifeq ($(MODE1),1)
TARGET:=MEGACD_MODE1
BUILD_PATH:=$(BUILD_PATH)/mode1
DISC_PATH:=$(BUILD_PATH)
endif

# Files the build creates in DISC_PATH
DISC_CONTENTS:= \
	$(DISC_PATH)/bramdemo.mmd \
	$(DISC_PATH)/spx.smd

# Additional C compiler flags
CC_FLAGS?=-O1 -fconserve-stack -fomit-frame-pointer -fno-gcse

# Additional assembler flags
AS_FLAGS?=

# Additional linker flags
LD_FLAGS?=

# Path to the Megadev directory
# *** REQUIRED!***
MEGADEV_PATH?=/opt/megadev

# Include the Megadev parent makefile
# *** REQUIRED!***
include $(MEGADEV_PATH)/megadev.make

.PHONY: init all clean

init:
	@mkdir -p $(SRC_PATH)
	@mkdir -p $(RES_PATH)
	@mkdir -p $(BUILD_PATH)
	@mkdir -p $(DISC_PATH)

ifeq ($(MODE1),1)
all: $(PROJECT_ID).cart
else
all: \
	$(DISC_CONTENTS) \
	$(PROJECT_ID).iso
endif

# Without MODE1=1, removes the output of both builds; with it, only the Mode 1
# build's
clean:
ifeq ($(MODE1),1)
	@rm -rf $(PROJECT_ID).cart $(BUILD_PATH)/*
else
	@rm -rf $(PROJECT_ID).iso $(PROJECT_ID).cart $(DISC_PATH)/*.mmd $(DISC_PATH)/*.smd $(BUILD_PATH)/*
endif

$(DISC_PATH)/bramdemo.mmd: \
	main/ipx_init.s \
	crash_main.s \
	bram_demo.c

$(DISC_PATH)/spx.smd: \
	std_init.s \
	spx.c \
	$(BUILD_PATH)/sp.bin

# Mode 1 cartridge (make MODE1=1). mode1_init.s embeds the Sub CPU image and
# the first module.
$(PROJECT_ID).cart: \
	mode1_init.s \
	mode1_main.c \
	mode1.s \
	kos_cmp.s

$(BUILD_PATH)/mode1_init.s.o: $(BUILD_PATH)/sp_mode1.bin $(DISC_PATH)/bramdemo.mmd

# The Sub CPU image for mode1_load_sub, which copies it to Program RAM 0x6000:
# the SP, padded so that the SPX after it lands at 0x10000, where it is linked
# to run and where the SP jumps.
$(BUILD_PATH)/sp_mode1.bin: $(BUILD_PATH)/sp.bin $(DISC_PATH)/spx.smd
	@[ $$(wc -c < $<) -le $$((0x10000 - 0x6000)) ] || \
	  { echo "sp.bin runs into the SPX at 0x10000" >&2; exit 1; }
	@cp $< $@
	@truncate -s $$((0x10000 - 0x6000)) $@
	@cat $(DISC_PATH)/spx.smd >> $@

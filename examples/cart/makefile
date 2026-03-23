################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

## Project ID
# ASCII text only; maximum of 11 characters
# *** REQUIRED!***
PROJECT_ID:=md_rom

## Build Target
# Determines for which hardware this project is intended
# Valid values: MEGADRIVE, MEGACD
# *** REQUIRED!***
TARGET:=MEGADRIVE

## Project Name
# ASCII text only; maximum of 48 characters
PROJECT_NAME:="SIMPLE MD ROM"

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

all: \
	$(PROJECT_ID).cart

rom: \
	$(PROJECT_ID).cart

clean:
	@rm -f $(PROJECT_ID).cart $(BUILD_PATH)/* > /dev/null 2>&1

$(PROJECT_ID).cart: \
	init.s \
	main.c \
	res.s 

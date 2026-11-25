################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

# Echo on the Mega CD: the Sub CPU runs the game logic and sends sound
# commands; the Main CPU passes them to Echo. `make` builds the disc, where
# the Main program and its music run from Word RAM; `make MODE1=1` builds a
# Mode 1 cartridge of the same program, with the Main program and its music
# in cartridge ROM.

PROJECT_ID:=soundcd
TARGET:=MEGACD
PROJECT_NAME:="ECHO ON MEGA CD"
REGION?=US
VIDEO?=NTSC

SRC_PATH:=src
RES_PATH:=res
BUILD_PATH:=build
DISC_PATH:=disc

ifeq ($(MODE1),1)
TARGET:=MEGACD_MODE1
BUILD_PATH:=$(BUILD_PATH)/mode1
DISC_PATH:=$(BUILD_PATH)
endif

# Files the build creates in DISC_PATH (in Mode 1 the Main CPU program is in
# the cartridge, not a module)
ifeq ($(MODE1),1)
DISC_CONTENTS:= \
	$(DISC_PATH)/spx.smd
else
DISC_CONTENTS:= \
	$(DISC_PATH)/main.mmd \
	$(DISC_PATH)/spx.smd
endif

CC_FLAGS?=-O1 -fconserve-stack -fomit-frame-pointer -fno-gcse
AS_FLAGS?=
LD_FLAGS?=

MEGADEV_PATH?=/opt/megadev
include $(MEGADEV_PATH)/megadev.make

.PHONY: all clean

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

# The Main CPU module, run in place in Word RAM with the music
$(DISC_PATH)/main.mmd: \
	main/ipx_init_stack.s \
	crash_main.s \
	song.s \
	main.c

# song.s includes the instruments from res/
$(BUILD_PATH)/song.s.o: $(wildcard $(RES_PATH)/*)

# The Sub CPU program, loaded to Program RAM 0x10000 by the SP
$(DISC_PATH)/spx.smd: \
	std_init_stack.s \
	spx.c \
	$(BUILD_PATH)/sp.bin

# Mode 1: the Main CPU program and its music are linked into the cartridge
# (Echo cannot address Word RAM at 0x600000), not loaded into Word RAM
$(PROJECT_ID).cart: \
	mode1_init.s \
	mode1_main.c \
	mode1.s \
	kos_cmp.s \
	crash_main.s \
	song.s \
	main.c

$(BUILD_PATH)/mode1_init.s.o: $(BUILD_PATH)/sp_mode1.bin

# The Sub CPU image for mode1_load_sub, which copies it to Program RAM 0x6000:
# the SP, padded so that the SPX after it lands at 0x10000, where it is linked
# to run and where the SP jumps.
$(BUILD_PATH)/sp_mode1.bin: $(BUILD_PATH)/sp.bin $(DISC_PATH)/spx.smd
	@[ $$(wc -c < $<) -le $$((0x10000 - 0x6000)) ] || \
	  { echo "sp.bin runs into the SPX at 0x10000" >&2; exit 1; }
	@cp $< $@
	@truncate -s $$((0x10000 - 0x6000)) $@
	@cat $(DISC_PATH)/spx.smd >> $@

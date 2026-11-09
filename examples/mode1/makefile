# Mode 1 example: a cartridge that drives the Mega CD, with an optional disc

PROJECT_ID:=mode1
TARGET:=MEGACD_MODE1
PROJECT_NAME:="MODE 1 EXAMPLE"
REGION?=US
VIDEO?=NTSC

SRC_PATH:=src
RES_PATH:=res
BUILD_PATH:=build
DISC_PATH:=disc

# The disc carries one file, a test pattern the build generates (see below).
# It is not bootable (ISO_BOOT is empty for MEGACD_MODE1): the cartridge reads
# it.
DISC_CONTENTS:=$(DISC_PATH)/data.bin

CC_FLAGS?=-O1 -fconserve-stack -fomit-frame-pointer -fno-gcse
AS_FLAGS?=
LD_FLAGS?=

MEGADEV_PATH?=/opt/megadev
include $(MEGADEV_PATH)/megadev.make

.PHONY: init all clean

init:
	@mkdir -p $(SRC_PATH)
	@mkdir -p $(RES_PATH)
	@mkdir -p $(BUILD_PATH)
	@mkdir -p $(DISC_PATH)

all: \
	$(PROJECT_ID).cart \
	$(PROJECT_ID).iso

clean:
	@rm -rf $(PROJECT_ID).cart $(PROJECT_ID).iso $(DISC_PATH)/data.bin $(BUILD_PATH)/*

# The SP is built first and embedded in the cartridge by init.s
$(PROJECT_ID).cart: \
	$(BUILD_PATH)/sp.bin \
	init.s \
	main.c \
	mode1.s \
	crash_main.s \
	kos_cmp.s

$(BUILD_PATH)/init.s.o: $(BUILD_PATH)/sp.bin

# Test pattern: 4096 big-endian words counting up from 0, four sectors. The
# Main CPU sums the words it finds in Word RAM and compares with DATA_SUM,
# which is computed here from the file itself.
DATA_WORDS:=4096

$(DISC_PATH)/data.bin:
	$(call msg_info,Generating $(notdir $@))
	@mkdir -p $(dir $@)
	@printf "$$(LC_ALL=C awk 'BEGIN { for (i = 0; i < $(DATA_WORDS); i++) printf "\\%03o\\%03o", int(i / 256), i % 256 }')" > $@

$(BUILD_PATH)/data_sum.h: $(DISC_PATH)/data.bin
	@mkdir -p $(dir $@)
	@printf '#define DATA_SIZE %s\n#define DATA_SUM %s\n' \
	  $$(wc -c < $<) \
	  $$(od -An -v -tu1 $< | awk '{ for (i = 1; i <= NF; i++) { s += (n % 2 == 0) ? $$i * 256 : $$i; n++ } } END { printf "0x%04X", s % 65536 }') > $@

$(BUILD_PATH)/main.c.o: $(BUILD_PATH)/data_sum.h

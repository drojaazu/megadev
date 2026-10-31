# Mode 1 example: a cartridge that drives the Mega CD

PROJECT_ID:=mode1
TARGET:=MEGACD_MODE1
PROJECT_NAME:="MODE 1 EXAMPLE"
REGION?=US
VIDEO?=NTSC

SRC_PATH:=src
RES_PATH:=res
BUILD_PATH:=build

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

all: \
	$(PROJECT_ID).cart

clean:
	@rm -rf $(PROJECT_ID).cart $(BUILD_PATH)/*

# The SP is built first and embedded in the cartridge by init.s
$(PROJECT_ID).cart: \
	$(BUILD_PATH)/sp.bin \
	init.s \
	main.c \
	mode1.s \
	kos_cmp.s

$(BUILD_PATH)/init.s.o: $(BUILD_PATH)/sp.bin

#
# [ M E G A D E V ]   a Sega Mega CD devkit
#

# Global Makefile & Build Settings

# Toolchain Setup
# we assume all commands appear somewhere in $PATH
# if this is not the case, you will need to specify the full path in the
# prefix
M68K_PREFIX?=m68k-linux-gnu-

CC:=$(M68K_PREFIX)gcc
OBJCPY:=$(M68K_PREFIX)objcopy
NM:=$(M68K_PREFIX)nm
LD:=$(M68K_PREFIX)ld
AS:=$(M68K_PREFIX)as

# (Z80 building not yet supported)
Z80_AS:=sjasmplus

################################################################################
# STOP!
# You should not need to change anything below this line unless you really,
# really know what you're doing.
################################################################################

#### Project Sanity Check & Defaults

ifndef PROJECT_ID
	$(error PROJECT_ID not set! Please set this variable in your makefile.)
endif

ifndef TARGET
	$(error TARGET not set! Please set this variable in your makefile.)
endif

ifndef SRC_PATH
	$(error SRC_PATH not set! Please set this variable in your makefile.)
endif

ifndef RES_PATH
	$(error RES_PATH not set! Please set this variable in your makefile.)
endif

ifndef BUILD_PATH
	$(error BUILD_PATH not set! Please set this variable in your makefile.)
endif

REGION?=US
VIDEO?=NTSC
VRAM_SIZE?=VRAM_64K
PROJECT_NAME?=$(PROJECT_ID)
PROJECT_NAME_JP?=$(PROJECT_NAME)
ifeq ($(REGION),US)
	HEADER_HARDWARE_ID?="SEGA GENESIS"
else
	HEADER_HARDWARE_ID?="SEGA MEGA DRIVE"
endif
HEADER_VOL_ID?=$(shell printf $(PROJECT_ID) |tr '[:lower:]' '[:upper:]')
HEADER_COPYRIGHT?="(C)\ \ \ \ \ $(shell LC_TIME="C" date +"%Y.%b" |tr '[:lower:]' '[:upper:]')"
HEADER_SOFTWARE_ID?="GM 00-0000-00"
HEADER_REGION?="JUE"
HEADER_DISC_ID?="SEGADISCSYSTEM"
HEADER_SYS_ID?=$(shell printf $(PROJECT_ID) |tr '[:lower:]' '[:upper:]')

# Fancy colors cause we're fancy
CLEAR=\033[0m
BOLD=\033[1m
CYAN=\033[1;36m
RED=\033[1;31m
YELLOW=\033[1;33m
GREEN=\033[1;32m

##### Build Tool Paths

# Megadev library code (ASM and C)
LIB_PATH:=$(MEGADEV_PATH)/lib

# build time tools
TOOLS_PATH:=$(MEGADEV_PATH)/tools

# linker scripts
CFG_PATH:=$(MEGADEV_PATH)/cfg

# setup includes
INC:=-I$(SRC_PATH) -I$(LIB_PATH) -I$(RES_PATH) -I$(BUILD_PATH) 
# We have to manually pass the includes to the assembler
# Each entry here should match each entry in INC, with a prefix of "-Wa,"
AS_INC:=-Wa,-I$(SRC_PATH) -Wa,-I$(LIB_PATH) -Wa,-I$(RES_PATH) -Wa,-I$(BUILD_PATH)

# build flags
# Note: we include "-Wa,--register-prefix-optional" as a default C option
# because it is useful for inline asm. However, "-Wa,--bitwise-or" will
# cause issues with the GCC created asm, so we split that off and only use it
# with asm source files
CC_FLAGS+= \
	-m68000 \
	-imacros build.def.h \
	-DPROJECT_ID=$(PROJECT_ID) \
	-DTARGET=$(TARGET) \
	-DHEADER_HARDWARE_ID=$(HEADER_HARDWARE_ID) \
	-DREGION=$(REGION) \
	-DVIDEO=$(VIDEO) \
	-DVRAM_SIZE=$(VRAM_SIZE) \
	-DPROJECT_NAME=$(PROJECT_NAME) \
	-DPROJECT_NAME_JP=$(PROJECT_NAME_JP) \
	-DHEADER_COPYRIGHT=$(HEADER_COPYRIGHT) \
	-DHEADER_VOL_ID=$(HEADER_VOL_ID) \
	-DHEADER_SYS_ID=$(HEADER_SYS_ID) \
	-DHEADER_SOFTWARE_ID=$(HEADER_SOFTWARE_ID) \
	-DHEADER_REGION=$(HEADER_REGION) \
	-DHEADER_DISC_ID=$(HEADER_DISC_ID) \
	$(if $(DEBUG), -DDEBUG) \
	-fno-builtin \
	-Wall -Wextra -Wno-main -Wa,--register-prefix-optional
AS_FLAGS+= \
	-Wa,--bitwise-or
LD_FLAGS+= \
	-nostdlib -z noexecstack

define msg_info
	@echo "${BOLD}${CYAN}$(1)${CLEAR}"
endef

define msg_warning
	@echo "${BOLD}${RED}$(1)${CLEAR}"
endef

vpath %.c $(SRC_PATH):$(LIB_PATH):$(LIB_PATH)/sub:$(LIB_PATH)/main
vpath %.h $(SRC_PATH):$(LIB_PATH):$(LIB_PATH)/sub:$(LIB_PATH)/main
vpath %.s $(SRC_PATH):$(LIB_PATH):$(LIB_PATH)/sub:$(LIB_PATH)/main
vpath %.elf $(BUILD_PATH)
vpath %.c.o $(BUILD_PATH)
vpath %.s.o $(BUILD_PATH)

.SECONDARY: $(BUILD_PATH)/*

# need to specify paths here as they're called through a secondary make
$(BUILD_PATH)/%.c.o: %.c
	$(call msg_info,Compiling source $(notdir $^))
	@$(CC) $(CC_FLAGS) $(INC) -c $^ -o $@

$(BUILD_PATH)/%.s.o: %.s
	$(call msg_info,Compiling source $(notdir $^))
	@$(CC) $(CC_FLAGS) $(AS_FLAGS) $(INC) $(AS_INC) -x assembler-with-cpp -c $^ -o $@

#%.mmd.elf: %.s %.c %.h
#	@echo "mmd elf in: $^"
#	@echo "mmd elf out: $@"
#	echo "making mmd elf with: $^ $@"
#	$(LD) $(LD_FLAGS) -z muldefs -T $(CFG_PATH)/module_mmd.ld $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(symref)) -o $@

%.mmd:
# @echo "mmd in: $^"
# @echo "mmd out: $@"
	$(eval BUILD_SRC:=$(addprefix $(BUILD_PATH)/,$(notdir $(addsuffix .o, $(filter %.c %.h %.s, $^)))))
	$(eval BUILD_MOD:=$(filter %.mmd %.smd %.bin, $^))
# @echo "build mod: $(BUILD_MOD)"
	@$(if $(BUILD_MOD), $(MAKE) -s $(BUILD_MOD))
	@$(if $(BUILD_SRC), $(MAKE) -s $(BUILD_SRC))
	$(call msg_info,Linking module $(notdir $@))
	$(eval OUT_MOD_ELF:=$(addprefix $(BUILD_PATH)/,$(addsuffix .elf,$(notdir $@))))
	@$(LD) $(LD_FLAGS) -z muldefs -T $(CFG_PATH)/module_mmd.ld $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(addsuffix .elf,$(addprefix $(BUILD_PATH)/,$(notdir $(symref))))) -o $(OUT_MOD_ELF)
	@$(NM) -n $(OUT_MOD_ELF) > $(addsuffix .sym,$(OUT_MOD_ELF))
	@$(OBJCPY) -O binary $(OUT_MOD_ELF) $@

%.smd:
# @echo "smd in: $^"
# @echo "smd out: $@"
	$(eval BUILD_SRC:=$(addprefix $(BUILD_PATH)/,$(notdir $(addsuffix .o, $(filter %.c %.h %.s, $^)))))
	$(eval BUILD_MOD:=$(filter %.mmd %.smd %.bin, $^))
# @echo "build mod: $(BUILD_MOD)"
	@$(if $(BUILD_MOD), $(MAKE) -s $(BUILD_MOD))
	@$(if $(BUILD_SRC), $(MAKE) -s $(BUILD_SRC))
	$(call msg_info,Linking module $(notdir $@))
	$(eval OUT_MOD_ELF:=$(addprefix $(BUILD_PATH)/,$(addsuffix .elf,$(notdir $@))))
	@$(LD) $(LD_FLAGS) -z muldefs -T $(CFG_PATH)/module_smd.ld $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(addsuffix .elf,$(addprefix $(BUILD_PATH)/,$(notdir $(symref))))) -o $(OUT_MOD_ELF)
	@$(NM) -n $(OUT_MOD_ELF) > $(addsuffix .sym,$(OUT_MOD_ELF))
	@$(OBJCPY) -O binary $(OUT_MOD_ELF) $@


%.cart:
	@echo "rom elf in: $^"
	@echo "rom elf out: $@"
	$(eval BUILD_SRC:=$(addprefix $(BUILD_PATH)/,$(notdir $(addsuffix .o, $(filter %.c %.h %.s, $^)))))
	@$(if $(BUILD_SRC), $(MAKE) -s $(BUILD_SRC))
	$(call msg_info,Linking module $(notdir $@))
	$(eval OUT_CART_ELF:=$(addprefix $(BUILD_PATH)/,$(addsuffix .elf,$(notdir $@))))
	@$(LD) $(LD_FLAGS) -T $(CFG_PATH)/md_cart.ld $(BUILD_SRC) -o $(OUT_CART_ELF)
	@$(NM) -n $(OUT_CART_ELF) > $(addprefix $(BUILD_PATH)/,$(addsuffix .sym,$(notdir $@)))
	@$(OBJCPY) -O binary $(OUT_CART_ELF) $@

default: init boot

.PHONY: init boot

boot: $(BUILD_PATH)/boot.bin

init_build:
	@mkdir -p $(BUILD_PATH) $(DISC_PATHS)

# special rules for boot sector binaries
$(BUILD_PATH)/ip.bin: $(BUILD_PATH)/ip.bin.elf
	@$(OBJCPY) -O binary $< $@

#@$(CC) $(CC_FLAGS) -I$(MEGADEV_PATH)/lib -c $< -o $@
$(BUILD_PATH)/security.s.o: $(MEGADEV_PATH)/lib/security.c
	$(call msg_info,Creating security block)
	$(CC) $(CC_FLAGS) $(INC) -c $< -o $@

$(BUILD_PATH)/ip.bin.elf: $(BUILD_PATH)/security.s.o $(BUILD_PATH)/ip.s.o
	@$(LD) $(LD_FLAGS) -T$(CFG_PATH)/ip.ld -o$@ $^
	@$(NM) -n $@ > $(addprefix $(BUILD_PATH)/,$(addsuffix .sym,$(notdir $@)))

$(BUILD_PATH)/sp.bin: $(BUILD_PATH)/sp.bin.elf
	@$(OBJCPY) -O binary $< $@

$(BUILD_PATH)/sp.bin.elf: $(BUILD_PATH)/sp_header.s.o $(BUILD_PATH)/sp.s.o
	@$(LD) $(LD_FLAGS) -T$(CFG_PATH)/sp.ld -o$@ $^
	@$(NM) -n $@ > $(addprefix $(BUILD_PATH)/,$(addsuffix .sym,$(notdir $@)))

$(BUILD_PATH)/boot.bin: $(BUILD_PATH)/ip.bin $(BUILD_PATH)/sp.bin
	$(call msg_info,Generating boot sector...)
	@$(CC) $(CC_FLAGS) $(AS_FLAGS) $(INC) $(AS_INC) -x assembler-with-cpp -c $(LIB_PATH)/cd_boot.s -o$@
	@$(OBJCPY) -O binary $@

# this is used to trigger an ISO rebuild if there are any file changes in the disc dir
DISC_DIR_UPDATES = $(shell find $(DISC_PATH)/ -type d)
DISC_FILES_UPDATES = $(shell find $(DISC_PATH)/ -type f -name '*')

# TODO make the ISO settings user configurable
%.iso: $(BUILD_PATH)/boot.bin $(DISC_FILES_UPDATES) $(DISC_DIR_UPDATES)
	$(call msg_info,Generating ISO image $(notdir $@))
	@mkisofs -quiet -iso-level 1 -G $< -pad -V "$(PROJECT_ID)" \
		-sysid "MEGA_CD" -appid "" -publisher "" -preparer "" \
		-o $@ $(DISC_PATH)
	$(info Target: $(TARGET))
	$(info Region: $(REGION))
	$(info Video: $(VIDEO))

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
AR:=$(M68K_PREFIX)ar

# (Z80 building not yet supported)
Z80_AS:=sjasmplus

# ISO mastering tool and options. Override ISO_FLAGS to change the options
# wholesale; the boot sector, volume ID and output are always passed.
MKISOFS?=mkisofs
ISO_FLAGS?=-iso-level 1 -pad -sysid "MEGA_CD" -appid "" -publisher "" -preparer ""

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

ifndef MEGADEV_PATH
	$(error MEGADEV_PATH not set! It must point at the Megadev directory, e.g. /opt/megadev.)
endif

ifeq ($(wildcard $(MEGADEV_PATH)/lib/build.def.h),)
	$(error MEGADEV_PATH ($(MEGADEV_PATH)) does not contain the Megadev library.)
endif

REGION?=US
VIDEO?=NTSC
VRAM_SIZE?=VRAM_64K
PROJECT_NAME?=$(PROJECT_ID)
PROJECT_NAME_DOMESTIC?=$(PROJECT_NAME)
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
	-DPROJECT_NAME_DOMESTIC=$(PROJECT_NAME_DOMESTIC) \
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
# Each compile writes a .d file listing the headers it included; they are
# included at the end of this file so a header change rebuilds its users.
DEP_FLAGS:=-MMD -MP
LD_FLAGS+= \
	-nostdlib -z noexecstack

# Runtime support the compiler expects to find: memcpy and friends, and the
# 32-bit arithmetic helpers, all from lib/runtime. Built into an archive per
# project so that only the members a link references are pulled in. libgcc is
# deliberately not linked: distribution builds are not 68000 code, so a helper
# missing from lib/runtime must stop the link rather than crash on the console.
RUNTIME_SRC:=$(wildcard $(LIB_PATH)/runtime/*.s $(LIB_PATH)/runtime/*.c)
RUNTIME_OBJ:=$(patsubst $(LIB_PATH)/runtime/%,$(BUILD_PATH)/runtime/%.o,$(RUNTIME_SRC))
RUNTIME_LIB:=$(BUILD_PATH)/libmegadev.a
LD_LIBS:=$(RUNTIME_LIB)

$(BUILD_PATH)/runtime/%.s.o: $(LIB_PATH)/runtime/%.s
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(AS_FLAGS) $(DEP_FLAGS) $(INC) $(AS_INC) -x assembler-with-cpp -c $< -o $@

$(BUILD_PATH)/runtime/%.c.o: $(LIB_PATH)/runtime/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(DEP_FLAGS) $(INC) -c $< -o $@

$(RUNTIME_LIB): $(RUNTIME_OBJ)
	@$(AR) rcs $@ $^

define msg_info
	@printf "${BOLD}- ${CYAN}$(1)${CLEAR}\n"
endef

define msg_warning
	@printf "${BOLD}! ${RED}$(1)${CLEAR}\n"
endef

define msg_done
	@printf "${BOLD}* ${GREEN}$(1)${CLEAR}\n"
endef

# DISC_PATH is a staging directory: whatever is in it goes on the disc, however
# it got there. Two optional lists let the build produce and verify some of it:
#   DISC_CONTENTS  files the build creates in DISC_PATH (modules, usually)
#   DISC_ASSETS    files copied into DISC_PATH from RES_PATH, by name
# The ISO depends on both, so make -j cannot master before they exist.
DISC_CONTENTS+=$(addprefix $(DISC_PATH)/,$(DISC_ASSETS))

define disc_asset_rule
$(DISC_PATH)/$(1): $(RES_PATH)/$(1)
	$$(call msg_info,Staging $(1))
	@mkdir -p $$(dir $$@)
	@cp $$< $$@
endef
$(foreach a,$(DISC_ASSETS),$(eval $(call disc_asset_rule,$(a))))

# this is used to trigger an ISO rebuild if there are any file changes in the disc dir
ifdef DISC_PATH
	DISC_DIR_UPDATES = $(shell find $(DISC_PATH)/ -type d 2>/dev/null)
	DISC_FILES_UPDATES = $(shell find $(DISC_PATH)/ -type f -name '*' 2>/dev/null)
endif

vpath %.c $(SRC_PATH):$(LIB_PATH):$(LIB_PATH)/sub:$(LIB_PATH)/main
vpath %.h $(SRC_PATH):$(LIB_PATH):$(LIB_PATH)/sub:$(LIB_PATH)/main
vpath %.s $(SRC_PATH):$(LIB_PATH):$(LIB_PATH)/sub:$(LIB_PATH)/main
vpath %.elf $(BUILD_PATH)
vpath %.c.o $(BUILD_PATH)
vpath %.s.o $(BUILD_PATH)

# Keep every intermediate file, and remove a target whose recipe failed so a
# partial output is never taken as up to date on the next run.
.SECONDARY:
.DELETE_ON_ERROR:

.DEFAULT_GOAL:=all

# A module, cart or boot binary depends on the objects built from the sources
# its project lists, so that a header change (recorded in the .d files) reaches
# the link step. $$^ is the project's own prerequisite list, seen at second
# expansion.
.SECONDEXPANSION:
MODULE_OBJS=$(addprefix $(BUILD_PATH)/,$(notdir $(addsuffix .o,$(filter %.c %.s,$(1)))))

# need to specify paths here as they're called through a secondary make
$(BUILD_PATH)/%.c.o: %.c
	$(call msg_info,Compiling source $(notdir $<))
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(DEP_FLAGS) $(INC) -c $< -o $@

$(BUILD_PATH)/%.s.o: %.s
	$(call msg_info,Compiling source $(notdir $<))
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(AS_FLAGS) $(DEP_FLAGS) $(INC) $(AS_INC) -x assembler-with-cpp -c $< -o $@

#%.mmd.elf: %.s %.c %.h
#	@echo "mmd elf in: $^"
#	@echo "mmd elf out: $@"
#	echo "making mmd elf with: $^ $@"
#	$(LD) $(LD_FLAGS) -z muldefs -T $(CFG_PATH)/module_mmd.ld $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(symref)) -o $@

%.mmd: $$(call MODULE_OBJS,$$^)
# @echo "mmd in: $^"
# @echo "mmd out: $@"
	$(call msg_info,Building module $(notdir $@))
	@mkdir -p $(dir $@) $(BUILD_PATH)
	$(eval BUILD_SRC:=$(addprefix $(BUILD_PATH)/,$(notdir $(addsuffix .o, $(filter %.c %.h %.s, $^)))))
	$(eval BUILD_MOD:=$(filter %.mmd %.smd %.bin, $^))
# @echo "build mod: $(BUILD_MOD)"
	@$(if $(BUILD_MOD), $(MAKE) -s $(BUILD_MOD))
	@$(if $(BUILD_SRC), $(MAKE) -s $(BUILD_SRC))
	$(call msg_info,Linking module $(notdir $@))
	$(eval OUT_MOD_ELF:=$(addprefix $(BUILD_PATH)/,$(addsuffix .elf,$(notdir $@))))
	@$(MAKE) -s $(RUNTIME_LIB)
	@$(LD) $(LD_FLAGS) -z muldefs -T $(CFG_PATH)/module_mmd.ld $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(addsuffix .elf,$(addprefix $(BUILD_PATH)/,$(notdir $(symref))))) $(LD_LIBS) -o $(OUT_MOD_ELF)
	@$(NM) -n $(OUT_MOD_ELF) > $(addsuffix .sym,$(OUT_MOD_ELF))
	@$(OBJCPY) -O binary $(OUT_MOD_ELF) $@

%.smd: $$(call MODULE_OBJS,$$^)
# @echo "smd in: $^"
# @echo "smd out: $@"
	$(call msg_info,Building module $(notdir $@))
	@mkdir -p $(dir $@) $(BUILD_PATH)
	$(eval BUILD_SRC:=$(addprefix $(BUILD_PATH)/,$(notdir $(addsuffix .o, $(filter %.c %.h %.s, $^)))))
	$(eval BUILD_MOD:=$(filter %.mmd %.smd %.bin, $^))
# @echo "build mod: $(BUILD_MOD)"
	@$(if $(BUILD_MOD), $(MAKE) -s $(BUILD_MOD))
	@$(if $(BUILD_SRC), $(MAKE) -s $(BUILD_SRC))
	$(call msg_info,Linking module $(notdir $@))
	$(eval OUT_MOD_ELF:=$(addprefix $(BUILD_PATH)/,$(addsuffix .elf,$(notdir $@))))
	@$(MAKE) -s $(RUNTIME_LIB)
	@$(LD) $(LD_FLAGS) -z muldefs -T $(CFG_PATH)/module_smd.ld $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(addsuffix .elf,$(addprefix $(BUILD_PATH)/,$(notdir $(symref))))) $(LD_LIBS) -o $(OUT_MOD_ELF)
	@$(NM) -n $(OUT_MOD_ELF) > $(addsuffix .sym,$(OUT_MOD_ELF))
	@$(OBJCPY) -O binary $(OUT_MOD_ELF) $@

%.cart: $$(call MODULE_OBJS,$$^)
#	@echo "cart in: $^"
#	@echo "cart out: $@"
	$(call msg_info,Building cart ROM $(notdir $@))
	@mkdir -p $(dir $@) $(BUILD_PATH)
	$(eval BUILD_SRC:=$(addprefix $(BUILD_PATH)/,$(notdir $(addsuffix .o, $(filter %.c %.h %.s, $^)))))
	@$(if $(BUILD_SRC), $(MAKE) -s $(BUILD_SRC))
	$(call msg_info,Linking cart ROM $(notdir $@))
	$(eval OUT_CART_ELF:=$(addprefix $(BUILD_PATH)/,$(addsuffix .elf,$(notdir $@))))
	@$(MAKE) -s $(RUNTIME_LIB)
	@$(LD) $(LD_FLAGS) -T $(CFG_PATH)/md_cart.ld $(BUILD_SRC) $(LD_LIBS) -o $(OUT_CART_ELF)
	@$(NM) -n $(OUT_CART_ELF) > $(addprefix $(BUILD_PATH)/,$(addsuffix .sym,$(notdir $@)))
	@$(OBJCPY) -O binary $(OUT_CART_ELF) $@

# special rules for boot sector binaries
$(BUILD_PATH)/ip.bin: $(BUILD_PATH)/ip.bin.elf
	@$(OBJCPY) -O binary $< $@

$(BUILD_PATH)/security.s.o: $(MEGADEV_PATH)/lib/security.c
	$(call msg_info,Creating security block)
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(DEP_FLAGS) $(INC) -c $< -o $@

$(BUILD_PATH)/ip.bin.elf: $(BUILD_PATH)/security.s.o $(BUILD_PATH)/ip.s.o $(RUNTIME_LIB)
	@$(LD) $(LD_FLAGS) -T$(CFG_PATH)/ip.ld -o$@ $^
	@$(NM) -n $@ > $(addprefix $(BUILD_PATH)/,$(addsuffix .sym,$(notdir $@)))

$(BUILD_PATH)/sp.bin: $(BUILD_PATH)/sp.bin.elf
	@$(OBJCPY) -O binary $< $@

$(BUILD_PATH)/sp.bin.elf: $(BUILD_PATH)/sp_header.s.o $(BUILD_PATH)/sp.s.o $(RUNTIME_LIB)
	@$(LD) $(LD_FLAGS) -T$(CFG_PATH)/sp.ld -o$@ $^
	@$(NM) -n $@ > $(addprefix $(BUILD_PATH)/,$(addsuffix .sym,$(notdir $@)))

$(BUILD_PATH)/boot.bin: $(BUILD_PATH)/ip.bin $(BUILD_PATH)/sp.bin
	$(call msg_info,Generating boot sector...)
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(AS_FLAGS) $(INC) $(AS_INC) -x assembler-with-cpp -c $(LIB_PATH)/cd_boot.s -o$@
	@$(OBJCPY) -O binary $@



# TODO make the ISO settings user configurable
%.iso: $(BUILD_PATH)/boot.bin $(DISC_CONTENTS) $(DISC_FILES_UPDATES) $(DISC_DIR_UPDATES)

	$(call msg_info,Generating ISO image $(notdir $@))
	@mkdir -p $(DISC_PATH)
	@$(MKISOFS) -quiet $(ISO_FLAGS) -G $< -V "$(HEADER_VOL_ID)" -o $@ $(DISC_PATH)
	$(call msg_done,Completed build of $(PROJECT_ID) ($(TARGET) / $(REGION) / $(VIDEO)))

# `make print-NAME` shows the value of a variable, e.g. make print-DISC_CONTENTS
print-%:
	@printf '%s\n' '$($*)'

# Header dependencies recorded by DEP_FLAGS. Deleting this line does not break
# any build; it only stops header changes from triggering rebuilds.
-include $(shell test -d $(BUILD_PATH) && find $(BUILD_PATH) -name '*.d' 2>/dev/null)

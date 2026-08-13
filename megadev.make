#
# [ M E G A D E V ]   a Sega Mega CD devkit
#

# Global Makefile & Build Settings

# Toolchain Setup
# we assume all commands appear somewhere in $PATH
# if this is not the case, you will need to specify the full path in the
# prefix
M68K_PREFIX?=m68k-linux-gnu-

# NOTE: these are assigned unconditionally, not with ?=. GNU make PREDEFINES
# CC, LD and AS (as cc, ld, as), so ?= would silently keep the host tool and
# the build would fail in confusing ways. Override the toolchain with
# M68K_PREFIX instead.
CC:=$(M68K_PREFIX)gcc
OBJCPY:=$(M68K_PREFIX)objcopy
NM:=$(M68K_PREFIX)nm
LD:=$(M68K_PREFIX)ld

# ISO mastering. The name is provided by cdrtools, cdrkit and genisoimage
# depending on distro, so allow it to be overridden.
MKISOFS?=mkisofs

################################################################################
# STOP!
# You should not need to change anything below this line unless you really,
# really know what you're doing.
################################################################################

#### Project Sanity Check & Defaults

ifndef MEGADEV_PATH
	$(error MEGADEV_PATH not set! It must point at the Megadev installation, e.g. /opt/megadev.)
endif

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
PROJECT_NAME_DOMESTIC?=$(PROJECT_NAME)
ifeq ($(REGION),US)
	HEADER_HARDWARE_ID?="SEGA GENESIS"
else
	HEADER_HARDWARE_ID?="SEGA MEGA DRIVE"
endif
HEADER_VOL_ID?=$(shell printf $(PROJECT_ID) |tr '[:lower:]' '[:upper:]')
# Simply-expanded so the date is resolved once per build rather than on every
# compile. Set SOURCE_DATE_EPOCH for a reproducible build.
BUILD_DATE:=$(shell LC_TIME=C date $(if $(SOURCE_DATE_EPOCH),-u -d @$(SOURCE_DATE_EPOCH),) +"%Y.%b" | tr '[:lower:]' '[:upper:]')
HEADER_COPYRIGHT?="(C)\ \ \ \ \ $(BUILD_DATE)"
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

# Set V=1 for a verbose build that shows every command.
Q:=$(if $(V),,@)

# Megadev library code (ASM and C)
LIB_PATH:=$(MEGADEV_PATH)/lib

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
DEP_FLAGS=-MMD -MP

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
LD_FLAGS+= \
	-nostdlib -z noexecstack

define msg_info
	@printf "${BOLD}- ${CYAN}$(1)${CLEAR}\n"
endef

define msg_warning
	@printf "${BOLD}! ${RED}$(1)${CLEAR}\n"
endef

define msg_done
	@printf "${BOLD}* ${GREEN}$(1)${CLEAR}\n"
endef

# this is used to trigger an ISO rebuild if there are any file changes in the disc dir
ifdef DISC_PATH
	DISC_DIR_UPDATES = $(shell test -d $(DISC_PATH) && find $(DISC_PATH)/ -type d)
	DISC_FILES_UPDATES = $(shell test -d $(DISC_PATH) && find $(DISC_PATH)/ -type f -name '*')
endif

vpath %.c $(SRC_PATH):$(LIB_PATH):$(LIB_PATH)/sub:$(LIB_PATH)/main
vpath %.h $(SRC_PATH):$(LIB_PATH):$(LIB_PATH)/sub:$(LIB_PATH)/main
vpath %.s $(SRC_PATH):$(LIB_PATH):$(LIB_PATH)/sub:$(LIB_PATH)/main
vpath %.elf $(BUILD_PATH)
vpath %.c.o $(BUILD_PATH)
vpath %.s.o $(BUILD_PATH)

.SECONDARY: $(BUILD_PATH)/*

$(BUILD_PATH):
	$(Q)mkdir -p $@

ifdef DISC_PATH
$(DISC_PATH):
	$(Q)mkdir -p $@
endif

# Module targets must depend on their OBJECTS, not just their sources.
# The .d files give each object a dependency on the headers it included, but
# that edge is only consulted if make has a reason to consider the object at
# all. Secondary expansion lets the pattern rules below turn the source list
# supplied by the project ($$^ from its own target line) into the matching
# object list, so a changed header reaches the module through its object.
.SECONDEXPANSION:

# Object paths mirror the source path they were named with, so two sources
# with the same base name in different directories no longer overwrite each
# other (B-5). `sub/pcm.s` becomes `$(BUILD_PATH)/sub/pcm.s.o`.
MODULE_OBJS = $(addprefix $(BUILD_PATH)/,$(addsuffix .o,$(filter %.c %.s,$1)))

.DEFAULT_GOAL:=all

$(BUILD_PATH)/%.c.o: %.c | $(BUILD_PATH)
	$(call msg_info,Compiling source $<)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CC_FLAGS) $(DEP_FLAGS) $(INC) -c $< -o $@

$(BUILD_PATH)/%.s.o: %.s | $(BUILD_PATH)
	$(call msg_info,Compiling source $<)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CC_FLAGS) $(DEP_FLAGS) $(AS_FLAGS) $(INC) $(AS_INC) -x assembler-with-cpp -c $< -o $@

#%.mmd.elf: %.s %.c %.h
#	@echo "mmd elf in: $^"
#	@echo "mmd elf out: $@"
#	echo "making mmd elf with: $^ $@"
#	$(LD) $(LD_FLAGS) -z muldefs -T $(CFG_PATH)/module_mmd.ld $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(symref)) -o $@

%.mmd: $$(call MODULE_OBJS,$$^)
# @echo "mmd in: $^"
# @echo "mmd out: $@"
	$(Q)mkdir -p $(dir $@)
	$(call msg_info,Building module $(notdir $@))
	$(eval BUILD_SRC:=$(filter %.o,$^))
	$(eval BUILD_MOD:=$(filter %.mmd %.smd %.bin,$^))
# @echo "build mod: $(BUILD_MOD)"
	$(call msg_info,Linking module $(notdir $@))
	$(eval OUT_MOD_ELF:=$(addprefix $(BUILD_PATH)/,$(addsuffix .elf,$(notdir $@))))
	@$(LD) $(LD_FLAGS) -z muldefs -T $(CFG_PATH)/module_mmd.ld $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(addsuffix .elf,$(addprefix $(BUILD_PATH)/,$(notdir $(symref))))) -o $(OUT_MOD_ELF)
	@$(NM) -n $(OUT_MOD_ELF) > $(addsuffix .sym,$(OUT_MOD_ELF))
	@$(OBJCPY) -O binary $(OUT_MOD_ELF) $@

%.smd: $$(call MODULE_OBJS,$$^)
# @echo "smd in: $^"
# @echo "smd out: $@"
	$(Q)mkdir -p $(dir $@)
	$(call msg_info,Building module $(notdir $@))
	$(eval BUILD_SRC:=$(filter %.o,$^))
	$(eval BUILD_MOD:=$(filter %.mmd %.smd %.bin,$^))
# @echo "build mod: $(BUILD_MOD)"
	$(call msg_info,Linking module $(notdir $@))
	$(eval OUT_MOD_ELF:=$(addprefix $(BUILD_PATH)/,$(addsuffix .elf,$(notdir $@))))
	@$(LD) $(LD_FLAGS) -z muldefs -T $(CFG_PATH)/module_smd.ld $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(addsuffix .elf,$(addprefix $(BUILD_PATH)/,$(notdir $(symref))))) -o $(OUT_MOD_ELF)
	@$(NM) -n $(OUT_MOD_ELF) > $(addsuffix .sym,$(OUT_MOD_ELF))
	@$(OBJCPY) -O binary $(OUT_MOD_ELF) $@

%.cart: $$(call MODULE_OBJS,$$^)
#	@echo "cart in: $^"
#	@echo "cart out: $@"
	$(Q)mkdir -p $(dir $@)
	$(call msg_info,Building cart ROM $(notdir $@))
	$(eval BUILD_SRC:=$(filter %.o,$^))
	$(call msg_info,Linking cart ROM $(notdir $@))
	$(eval OUT_CART_ELF:=$(addprefix $(BUILD_PATH)/,$(addsuffix .elf,$(notdir $@))))
	@$(LD) $(LD_FLAGS) -T $(CFG_PATH)/md_cart.ld $(BUILD_SRC) -o $(OUT_CART_ELF)
	@$(NM) -n $(OUT_CART_ELF) > $(addprefix $(BUILD_PATH)/,$(addsuffix .sym,$(notdir $@)))
	@$(OBJCPY) -O binary $(OUT_CART_ELF) $@

# special rules for boot sector binaries
$(BUILD_PATH)/ip.bin: $(BUILD_PATH)/ip.bin.elf | $(BUILD_PATH)
	@$(OBJCPY) -O binary $< $@

$(BUILD_PATH)/security.s.o: $(MEGADEV_PATH)/lib/security.c | $(BUILD_PATH)
	$(call msg_info,Creating security block)
	@$(CC) $(CC_FLAGS) $(INC) -c $< -o $@

$(BUILD_PATH)/ip.bin.elf: $(BUILD_PATH)/security.s.o $(BUILD_PATH)/ip.s.o | $(BUILD_PATH)
	@$(LD) $(LD_FLAGS) -T$(CFG_PATH)/ip.ld -o$@ $^
	@$(NM) -n $@ > $(addprefix $(BUILD_PATH)/,$(addsuffix .sym,$(notdir $@)))

$(BUILD_PATH)/sp.bin: $(BUILD_PATH)/sp.bin.elf | $(BUILD_PATH)
	@$(OBJCPY) -O binary $< $@

$(BUILD_PATH)/sp.bin.elf: $(BUILD_PATH)/sp_header.s.o $(BUILD_PATH)/sp.s.o | $(BUILD_PATH)
	@$(LD) $(LD_FLAGS) -T$(CFG_PATH)/sp.ld -o$@ $^
	@$(NM) -n $@ > $(addprefix $(BUILD_PATH)/,$(addsuffix .sym,$(notdir $@)))

$(BUILD_PATH)/boot.bin.o: $(BUILD_PATH)/ip.bin $(BUILD_PATH)/sp.bin | $(BUILD_PATH)
	$(call msg_info,Generating boot sector...)
	$(Q)$(CC) $(CC_FLAGS) $(AS_FLAGS) $(INC) $(AS_INC) -x assembler-with-cpp -c $(LIB_PATH)/cd_boot.s -o $@

# Converted to a separate target rather than in place: an interrupted build
# previously left an ELF named boot.bin, which a re-run would happily objcopy
# a second time.
$(BUILD_PATH)/boot.bin: $(BUILD_PATH)/boot.bin.o | $(BUILD_PATH)
	$(Q)$(OBJCPY) -O binary $< $@



# ISO mastering options. Override ISO_FLAGS to change them wholesale, or set
# the individual header variables above.
ISO_FLAGS?=-iso-level 1 -pad -sysid "MEGA_CD" -appid "" -publisher "" -preparer ""

# Reproducible builds: mkisofs stamps the image with the current time, and
# ISO9660 records a timestamp per file, so BOTH have to be pinned. Verified:
# pinning only the volume date is not enough once the payload is rebuilt.
ifdef SOURCE_DATE_EPOCH
ISO_DATE_FLAGS=-creation-date $(shell date -u -d @$(SOURCE_DATE_EPOCH) +%Y%m%d%H%M%S)00
NORMALISE_DISC_MTIMES=find $(DISC_PATH) -exec touch -d @$(SOURCE_DATE_EPOCH) {} +
else
ISO_DATE_FLAGS=
NORMALISE_DISC_MTIMES=true
endif

%.iso: $(BUILD_PATH)/boot.bin $$(DISC_CONTENTS) $(DISC_FILES_UPDATES) $(DISC_DIR_UPDATES)
	$(Q)mkdir -p $(dir $@) $(DISC_PATH)
	$(call msg_info,Generating ISO image $(notdir $@))
	$(Q)$(NORMALISE_DISC_MTIMES)
	$(Q)$(MKISOFS) -quiet $(ISO_FLAGS) $(ISO_DATE_FLAGS) -G $< -V "$(PROJECT_ID)" \
		-o $@ $(DISC_PATH)
	$(call msg_done,Completed build of $(PROJECT_ID) ($(TARGET) / $(REGION) / $(VIDEO)))

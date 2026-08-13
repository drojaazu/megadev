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
HEADER_VOL_ID?=$(shell printf '%s' '$(PROJECT_ID)' | tr '[:lower:]' '[:upper:]')
# Simply-expanded so the date is resolved once per build rather than on every
# compile. Set SOURCE_DATE_EPOCH for a reproducible build.
BUILD_DATE:=$(shell LC_TIME=C date $(if $(SOURCE_DATE_EPOCH),-u -d @$(SOURCE_DATE_EPOCH),) +"%Y.%b" | tr '[:lower:]' '[:upper:]')
HEADER_COPYRIGHT?="(C)\ \ \ \ \ $(BUILD_DATE)"
HEADER_SOFTWARE_ID?="GM 00-0000-00"
HEADER_REGION?="JUE"
HEADER_DISC_ID?="SEGADISCSYSTEM"
HEADER_SYS_ID?=$(shell printf '%s' '$(PROJECT_ID)' | tr '[:lower:]' '[:upper:]')

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

# A recipe that fails part way leaves its partial output behind, and make then
# treats that file as up to date -- so an interrupted mkisofs, objcopy or ld
# yields a corrupt image that the NEXT build happily accepts. Demonstrated:
# without this, a failed recipe left a 7 byte file and make reported success.
.DELETE_ON_ERROR:

# Keep intermediates (objects, ELFs) rather than deleting them after a build.
# This was previously `.SECONDARY: $(BUILD_PATH)/*`, whose wildcard is expanded
# when the makefile is read: on a clean tree it matched nothing, which means
# "all targets are secondary", and on an incremental build it matched only the
# files that already existed. Bare .SECONDARY: states that intent directly and
# behaves the same on every run.
.SECONDARY:

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

# Memory resident modules -- those other modules import symbols from -- are
# built with a layout script that names its metadata RESIDENT_* instead of
# MODULE_*. Declare them in the project makefile:
#
#     RESIDENT_MODULES:=$(DISC_PATH)/ipx.mmd
#
# Without this, a transient module importing a resident one sees that module's
# MODULE_ROM_ORIGIN and friends collide with its own.
module_ld_script = $(if $(filter $(1),$(RESIDENT_MODULES)),$(CFG_PATH)/module_resident_$(2).ld,$(CFG_PATH)/module_$(2).ld)

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
#	$(LD) $(LD_FLAGS) -T $(call module_ld_script,$@,mmd) $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(symref)) -o $@

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
	@$(LD) $(LD_FLAGS) -T $(call module_ld_script,$@,mmd) $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(addsuffix .elf,$(addprefix $(BUILD_PATH)/,$(notdir $(symref))))) -o $(OUT_MOD_ELF)
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
	@$(LD) $(LD_FLAGS) -T $(call module_ld_script,$@,smd) $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(addsuffix .elf,$(addprefix $(BUILD_PATH)/,$(notdir $(symref))))) -o $(OUT_MOD_ELF)
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
	$(Q)$(MKISOFS) -quiet $(ISO_FLAGS) $(ISO_DATE_FLAGS) -G $< -V "$(HEADER_VOL_ID)" \
		-o $@ $(DISC_PATH)
	$(call msg_done,Completed build of $(PROJECT_ID) ($(TARGET) / $(REGION) / $(VIDEO)))

# Disc payload assets. Source assets live in $(RES_PATH) (as docs/disc.md
# already advises for audio) and are copied into $(DISC_PATH) at build time.
# $(DISC_PATH) holds build output and is gitignored, so an asset kept there
# directly is untracked, and a fresh clone silently produces an image missing
# its data. List the copy under DISC_CONTENTS to have it built and verified.
# Declared explicitly rather than as a $(DISC_PATH)/% pattern rule: a pattern
# would also match the module outputs, and make would try to build res/foo.mmd
# and copy it into place.
define disc_asset_rule
$(DISC_PATH)/$(1): $(RES_PATH)/$(1)
	$$(Q)mkdir -p $$(dir $$@)
	$$(Q)cp $$< $$@
endef
$(foreach a,$(DISC_ASSETS),$(eval $(call disc_asset_rule,$(a))))

# Introspection: `make print-DISC_CONTENTS` reports a variable's value, so
# tooling can check what the build intends to produce.
print-%:
	@echo '$($*)'

################################################################################
# Header dependency tracking
#
# -MMD writes a .d file alongside each object listing the headers it included,
# and -MP adds a phony target for each of those headers so that deleting one
# does not break the build with "No rule to make target".
#
# Including them here is what makes `make` notice a changed header. Without it,
# editing a .h leaves every dependent object stale, which is why the advice
# used to be to run `make clean` before every build.
#
# NOTE: this include is easy to delete by accident and nothing fails loudly
# when it is missing - the build simply stops noticing header changes. The
# verification gate checks for it explicitly (Tier 0.6).
################################################################################

-include $(shell test -d $(BUILD_PATH) && find $(BUILD_PATH) -name '*.d' 2>/dev/null)

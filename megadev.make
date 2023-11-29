#
# [ M E G A D E V ]   a Sega Mega CD devkit
#

# Global Makefile & Build Settings

# Toolchain Setup
# we assume all commands appear somewhere in $PATH
# if this is not the case, you will need to specify the full path in the
# prefix
M68K_PREFIX?=m68k-elf-

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

# TODO check for required values and fail if not set:
# MEGADEV_PATH
# PROJECT_NAME
# OUT_PATH
# SRC_PATH
# DISCS_PATH

# fancy colors cause we're fancy
CLEAR=\033[0m
BOLD=\033[1m
CYAN=\033[1;36m
RED=\033[1;31m
YELLOW=\033[1;33m
GREEN=\033[1;32m

## PATHS
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
CC_FLAGS:=-m68000 -imacros build.h -DPROJECT_NAME=$(PROJECT_NAME) $(addprefix -D, $(HW_CFG)) $(if $(DEBUG), -DDEBUG) $(OPT_FLAGS) -fno-builtin -fomit-frame-pointer -fno-gcse -Wall -Wextra -Wno-main -Wa,--register-prefix-optional
AS_FLAGS:=-Wa,--bitwise-or
LD_FLAGS:=-nostdlib

define msg_info
	@echo -e "${BOLD}${CYAN}$(1)${CLEAR}"
endef

define msg_warning
	@echo -e "${BOLD}${RED}$(1)${CLEAR}"
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
	@echo "mmd in: $^"
	@echo "mmd out: $@"
	$(eval BUILD_SRC:=$(addprefix $(BUILD_PATH)/,$(notdir $(addsuffix .o, $(filter %.c %.h %.s, $^)))))
	$(eval BUILD_MOD:=$(filter %.mmd %.smd %.bin, $^))
	@echo "build mod: $(BUILD_MOD)"
	$(if $(BUILD_MOD), $(MAKE) -s $(BUILD_MOD))
	$(if $(BUILD_SRC), $(MAKE) -s $(BUILD_SRC))
	$(call msg_info,Linking module $(notdir $@))
	$(eval OUT_MOD_ELF:=$(addprefix $(BUILD_PATH)/,$(addsuffix .elf,$(notdir $@))))
	$(LD) $(LD_FLAGS) -z muldefs -T $(CFG_PATH)/module_mmd.ld $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(addsuffix .elf,$(addprefix $(BUILD_PATH)/,$(notdir $(symref))))) -o $(OUT_MOD_ELF)
	@$(NM) -n $(OUT_MOD_ELF) > $(addsuffix .sym,$(OUT_MOD_ELF))
	@$(OBJCPY) -O binary $(OUT_MOD_ELF) $@


%.smd:
	@echo "smd in: $^"
	@echo "smd out: $@"
	$(eval BUILD_SRC:=$(addprefix $(BUILD_PATH)/,$(notdir $(addsuffix .o, $(filter %.c %.h %.s, $^)))))
	$(eval BUILD_MOD:=$(filter %.mmd %.smd %.bin, $^))
	@echo "build mod: $(BUILD_MOD)"
	$(if $(BUILD_MOD), $(MAKE) -s $(BUILD_MOD))
	$(if $(BUILD_SRC), $(MAKE) -s $(BUILD_SRC))
	$(call msg_info,Linking module $(notdir $@))
	$(eval OUT_MOD_ELF:=$(addprefix $(BUILD_PATH)/,$(addsuffix .elf,$(notdir $@))))
	$(LD) $(LD_FLAGS) -z muldefs -T $(CFG_PATH)/module_smd.ld $(BUILD_SRC) $(foreach symref,$(BUILD_MOD),-R $(addsuffix .elf,$(addprefix $(BUILD_PATH)/,$(notdir $(symref))))) -o $(OUT_MOD_ELF)
	@$(NM) -n $(OUT_MOD_ELF) > $(addsuffix .sym,$(OUT_MOD_ELF))
	@$(OBJCPY) -O binary $(OUT_MOD_ELF) $@

default: init boot

.PHONY: init boot

boot: $(BUILD_PATH)/boot.bin

init:
	@mkdir -p $(BUILD_PATH) $(DISC_PATHS)

# special rules for boot sector binaries
$(BUILD_PATH)/ip.bin: $(BUILD_PATH)/ip.bin.elf
	@$(OBJCPY) -O binary $< $@

$(BUILD_PATH)/ip.bin.elf: $(BUILD_PATH)/ip.s.o
	@$(LD) $(LD_FLAGS) -T$(CFG_PATH)/ip.ld -o$@ $<
	@$(NM) -n $@ > $(addprefix $(BUILD_PATH)/,$(addsuffix .sym,$(notdir $@)))

$(BUILD_PATH)/sp.bin: $(BUILD_PATH)/sp.bin.elf
	@$(OBJCPY) -O binary $< $@

$(BUILD_PATH)/sp.bin.elf: $(BUILD_PATH)/sp_header.s.o $(BUILD_PATH)/sp.s.o
	@$(LD) $(LD_FLAGS) -T$(CFG_PATH)/sp.ld -o$@ $^
	@$(NM) -n $@ > $(addprefix $(BUILD_PATH)/,$(addsuffix .sym,$(notdir $@)))

$(BUILD_PATH)/boot.bin: $(BUILD_PATH)/ip.bin $(BUILD_PATH)/sp.bin
	$(call msg_info,Generating boot sector...)
	@$(CC) $(CC_FLAGS) $(AS_FLAGS) $(INC) $(AS_INC) -x assembler-with-cpp -c $(LIB_PATH)/boot.s -o$@
	@$(OBJCPY) -O binary $@

# TODO make the ISO settings user configurable
%.iso: $(BUILD_PATH)/boot.bin
	$(call msg_info,Generating ISO image $(notdir $@))
	@mkisofs -quiet -iso-level 1 -G $< -pad -V "$(PROJECT_NAME)" \
		-sysid "MEGA_CD" -appid "" -publisher "" -preparer "" \
		-o $@ $(notdir $(basename $@))

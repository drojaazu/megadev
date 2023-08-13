################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

# This will be used for filename output
PROJECT_NAME=md_rom

## Directory paths are relative to the location of the makefile
## (which should be the root of your project directory)

# Source code path
# (relative to the location of the project makefile)
SRC_PATH:=src

# Resources path
# This is where graphics, audio and any other non-executable content should be
# stored.
RES_PATH:=res

# Output path
# This will be the location of the final output CD ISOs and MD ROMs as well
# as debugging output (namely, symbol lists)
OUT_PATH:=out

# Optional compiler flags
# Here you can add any additional compiler flags
OPT_FLAGS:=-O1 -fconserve-stack

# Z80 CPU code path
# Z80 building is not currently supported... someday!
#Z80_SRC_PATH:=z80_src

# Path to the Megadev directory
MEGADEV_PATH:=/opt/megadev

# The include below is required! Do not remove it!
include $(MEGADEV_PATH)/megadev.make

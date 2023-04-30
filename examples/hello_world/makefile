################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

# This will be used for filename output
PROJECT_NAME=helloworld

## Directory paths are relative to the location of the makefile
## (which should be the root of your project directory)

# Source code path
# (relative to the location of the project makefile)
SRC_PATH:=src

# Resources path
# This is where graphics, audio and any other non-executable content should be
# stored.
RES_PATH:=res

# Build path
# This is where compile time build artifacts will be stored, including
# symbol maps
BUILD_PATH:=build

# Disc paths
# A space-delimited list of subdiorectories representing the content for each
# disc in your project
# This is used by the `init` rule to create directories before building
DISC_PATHS:=disc

# Optional compiler flags
# Here you can add any additional compiler flags
OPT_FLAGS:=-O1 -fconserve-stack

# Path to the Megadev directory
MEGADEV_PATH:=/opt/megadev

include $(MEGADEV_PATH)/megadev.make

disc: \
	init \
	disc.iso

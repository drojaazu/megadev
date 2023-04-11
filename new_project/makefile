################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################`

# This will be used for filename output
PROJECT_NAME=helloworld

# the path to your source code
# (relative to the location of the project makefile)
SRC_PATH:=src

# Build project in debug mode
DEBUG:=1

## Directory paths are relative to the location of the makefile
## (which should probably be the root of your project directory)

# Resources directory path
# This is where graphics, audio and any other non-executable content should be
# stored.
RES_PATH:=res

# Disc directory path
# Any files in this directory will be included in the final ISO image
DISC_PATH:=disc

# Build directory path
# The file path for the build work directory (object files, address maps, etc)
BUILD_PATH:=build

# Output directory path
# This will be the location of the final output ISO, as well as the build
# work directory
OUT_PATH:=build

# Optional compiler flags
# Here you can add any additional compiler flags
OPT_FLAGS:=-O1 -fstack-usage -fconserve-stack

# Z80 CPU code path
# Z80 building is not currently supported... someday!
#Z80_SRC_PATH:=z80_src

# Path to the Megadev directory
MEGADEV_PATH:=/opt/megadev

# Don't change the line below!
include $(MEGADEV_PATH)/makefile.global

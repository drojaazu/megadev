################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################`

# This will be used for filename output
PROJECT_NAME=helloworld

# the path to your source code
# (relative to the location of the project makefile)
SRC_PATH:=src

## Directory paths are relative to the location of the makefile
## (which should probably be the root of your project directory)

# Resources directory path
# This is where graphics, audio and any other non-executable content should be
# stored.
RES_PATH:=res

# Output directory path
# This will be the location of the final output Mega CD ISO and MD ROMs
OUT_PATH:=out

# Build path
# This is where compile time build artifacts will be stored, including
# symbol maps
BUILD_PATH:=build

# Disc directory path
# Any files in this directory will be included in the final ISO image
DISC_PATH:=disc

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

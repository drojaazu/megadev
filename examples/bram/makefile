################################################################################
# MEGADEV - Development framework for Sega Mega CD hardware
# https://github.com/drojaazu/megadev
################################################################################

# This will be used for filename output
PROJECT_NAME=bram

# the path to your source code
# (relative to the location of the makefile)
SRC_PATH:=src

# Build project in debug mode
DEBUG:=1

# Resources path
# This is where graphics, audio and any other non-code content should be
# stored.
RES_PATH:=res

# Output path
# This will be the location of the final output ISO, as well as the build
# work directory
OUT_PATH:=build

# Disc path 
# This will contain all the files to be included in the final ISO
# You can include any extra files you'd like here
DISC_PATH:=disc

# Optional compiler flags
# Here you can add any additional compiler flags
OPT_FLAGS:=-O1 -fstack-usage -fconserve-stack

# Z80 CPU code path
# Z80 building is not currently supported... someday!
#Z80_SRC_PATH:=z80_src

# Path to the Megadev directory
MEGADEV_PATH:=/opt/megadev

include $(MEGADEV_PATH)/makefile.global

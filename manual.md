
Documentation

- readme
- bootlib
- cdrom - include information about file loading without the wrapper (move by offset, load into CDC in loop, etc); re-evaluate some of teh naming the whole CDROM access wrapper subsystem
- dev_in_c - touch up in general, especially the stack usage section
- disc
- ip_sp
- modules
- program design
- start here


Merge everything into one manual...

# Introduction

Megadev is a development kit for creating software for the Sega Mega Drive and Mega CD hardware. It is collection of utilities, definitions, C headers, documentation, and examples to make development for these systems less painful than starting from scratch.

Megadev is intended for those who already have some experience with C or M68k assembly programming and have at least a passing familiarity with embedded systems. It is less "friendly" than something like [SGDK](https://github.com/Stephane-D/SGDK), but this is due in large part to the increased complexity of the addition of the Mega CD (which SGDK does not support) and due to a desire for a development system that is more flexible for skilled developers and less reliant on an abundance of external utilities.

The original target hardware was the Mega CD in particular, with support for the vanilla Mega Drive being somewhat of a late addition. Moreover, there is already a wealth of information and examples for the MD. As such, most of the information introduced here is focused on the Mega CD side of things.

# Requirements

In order to compile code for the Mega Drive/CD, you will need a build environment that targets the M68000 CPU architecture. Megadev is designed to use the GNU development tools that have been built for this architecture (m68k-elf). In addition, you will need `mkisofs` to generate the .iso disc image for Mega CD games.

Your distribution may have packages for these tools, or you can compile them from scratch if necessary. Alternatively, we have included a Dockerfile that for creating a Dev Container in Visual Studio Code to streamline this process.

## Docker

This is the modern, preferred method for development as it should work on all platforms (Windows, MacOS, Linux). The simplest way to work with this is to copy the Dockerfile into your project directory and load the diretcory in Visual Studio Code with the Dev Containers extension installed (or any other IDE that supports Dev Containers).

## By Distro

### Arch Linux

Install the following packages from the AUR:

https://aur.archlinux.org/packages/m68k-elf-binutils/

https://aur.archlinux.org/packages/m68k-elf-gcc/

And install the following package from the standard repository:

`cdrtools`

### Debian

Install the following packages from the standard repository:

`binutils-m68k-linux-gnu`

`gcc-m68k-linux-gnu`

`genisoimage`

## Building From Source

If your distro does not contain pre-built packages for the GNU build tools with the M68000 target, you can build them from scratch. The source for binutils and gcc can be found here:

https://www.gnu.org/software/binutils/

https://www.gnu.org/software/gcc/

You *must* add `--target=m68k-elf` as an option when you run the configure script in order target the M68000 architecture! You will also need to modify the `M68K_PREFIX` variable in your makefile to indicate the prefix used for the tools. By default, this should be `mm68k-elf-`.

You may want to use a helper script [like the ones found here](https://github.com/kentosama/m68k-elf-gcc).

# Getting Started

Once your build environment is in place, you should attempt to compile and run one of the example projects. Try `hello_world` first and then `ipx_spx` after that, as these are the simplest examples of booting the Mega CD and loading files from disc.

To build the example project, run `make disc` from the root directory. This should generate a disc.iso file which can run in an emulator or burnt to disc.

After that, feel free to try the remaining examples. In all cases, check the `README.md` file for details about each example.

# Megadev Project Structure

discuss the makefile and using ips_spx as a skeleton

# The Hardware

## Official Sega Documentation

You should have a copy of the official documentation about the Mega Drive and Mega CD for reference. These are readily available online and not difficult to find.

Though they will primarily be used for reference, we still highly recommend reading through them to become familiar with the capabilities and expectations of the hardware.

We will highlight some of the important aspects of the hardware and its usage in the next sections.

## Boot Process (IP/SP)

To make a game bootable on the Mega CD, there are two primary elements that must be in place: the header and the security code.

The header is a string of text that appears at the very start of the disc's data, and the security code is a small bit of code that............



## Internal ROM (BIOS/Boot Library)

## Gate Array

# Development Considerations

## Program Architecture

## Development in C

# Megadev Concepts & Utilities

## Modules (MMD/SMD)

## CD-ROm Access Wrapper

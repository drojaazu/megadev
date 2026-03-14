
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

---


# [ M E G A D E V ]

*AV INTELLIGENT TERMINAL WITH OPTICAL DISC DRIVE*

Megadev is a development kit for the Sega Mega Drive and Mega CD hardware. It is collection of utilities, headers, documentation, and examples to make development for these systems less painful than starting from scratch.

# Preface

Megadev is intended for those who already have some experience with C or M68k assembly programming and have at least a passing familiarity with embedded systems. It is less "friendly" than something like [SGDK](https://github.com/Stephane-D/SGDK). This is due in large part to the focus on the Mega CD (which brings with it notably increased complexity) but also arises from a desire for a development system that is more flexible for skilled developers and less reliant on an abundance of external utilities.

## Kilo/Mega

As a note, we use such terms as "kilo" and "mega" when referring to base 2 values, meaning, for example, a kilobyte is 1,024 bytes and a megabit is 131,072 bytes. We acknowledge the necessity and importance of the differentiation between decimal and binary base identifiers in the engineering world but assert that words like "kibibyte" and "mebibyte" sound ridiculous. Since this framework deals exclusively with retro hardware that existed before SI prefix standardization, we choose to use the old naming system. **All SI prefixed numbers throughout the documentation are base 2.**

# Getting Started

## Hardware Documentation

You should have a copy of the official Mega Drive and Mega CD documentation, which are readily available online. These will give you a basic understanding of and a reference to the hardware.

You should have a basic technical familiarity with the hardware before even beginning to write any code.

## Build Environment

In order to compile code for the Mega Drive/CD, you will need a build environment that targets the M68000 CPU architecture. Megadev is designed to be used with GNU [binutils](https://www.gnu.org/software/binutils/) and [gcc](https://www.gnu.org/software/gcc/) compiled with the m68k-elf target. In addition, you will need `mkisofs` to generate the .iso disc image for Mega CD games.

### Docker / Dev Container

The simplest way to set up such a build environment is to use a [dev container](https://containers.dev/). This is the modern, preferred method for development as it should work on any host platform (Windows, Linux, MacOS). (You will need Docker installed on your workstation in order to use this method.)

The `new_project` directory includes `Dockerfile` and `.devcontainer.json` files to automatically spin up a development environment. Simply copy this directory into your personal workspace and open it in [VS Code](https://code.visualstudio.com/) or another IDE that supports dev containers and it should be automatically re-open as a container. The latest Megadev will be pulled from github and necessary supporttools will be installed. You are then ready to begin development!

The default Dockerfile uses the Debian "slim" distro. Alternatively, there is a smaller Alpine Linux based `Dockerfile_alpine` within the `etc` subdirectory. If you want a super lean container, this is a good option. However, precompiled M68k-targeted GNU build tools are not available within the repo and must be built from scratch. This is done automatically when building the container the first time, but keep in mind that it does take a significant amount of time to compile.

### Manual Installation - From Distro Repo

#### Arch Linux

Install the following packages from the AUR:

https://aur.archlinux.org/packages/m68k-elf-binutils/

https://aur.archlinux.org/packages/m68k-elf-gcc/

And install the following package from the standard repository:

`cdrtools`

#### Debian

Install the following packages from the standard repository:

`binutils-m68k-linux-gnu`

`gcc-m68k-linux-gnu`

`genisoimage`

### Manual Installation - From Source Code

If your distro does not maintain pre-built packages for the GNU build tools with the M68000 target, you can build them from scratch. The source for binutils and gcc can be found here:

https://www.gnu.org/software/binutils/

https://www.gnu.org/software/gcc/

You *must* add `--target=m68k-elf` as an option when you run the configure script in order target the M68000 architecture! You will also need to modify the `M68K_PREFIX` variable in the Megadev project makefile to indicate the prefix used for the tools. By default, this should be `m68k-elf-`.

You may wish to use a helper script [like the one found here](https://github.com/kentosama/m68k-elf-gcc).

### Windows

Development in Windows is not officially supported, though Megadev should work in an environment such as cygwin. We recommend using the Docker-based dev container for development in Windows.

## Install Megadev

(This step is not necessary if you are using the Dockerfile and a dev container.)

Clone the repo and place it in a wherever you would like (we recommend `/opt/megadev`). In the root of the directory is the `megadev.make` file which will be used as a system-wide base for all projects. Edit the settings in its top section as necessary to match your build environment.

## Build An Example Project

Once your build environment is in place, you should attempt to compile and run one of the example projects. We recommend starting with the `new_project` skeleton project as it contains a fully working simple example.

To build the project, run `make` within the root of the project directory. This will generate an ISO disc image file which can be run in an emulator or burnt to disc.

After that, check the `examples` subdirectory for other projects. Check the `README.md` file included with each for notes. (We reccommend copying these directories into your own workspace before building in order to leave the Megadev directory untouched.)

If you placed the Megadev installation somewhere other than `/opt/megadev`, be sure to edit the value of `MEGADEV_PATH` in the makefile for each example, or set it as an environment variable.

# Project Composition & Compilation

Megadev uses GNU `make` for its build system. At the bare minimum, *a Megadev project must contain a Makefile which contains certain required configuration variables* (discussed below) and *includes the Megadev parent Makefile*. You are free to lay out your project tree as you like so long as the `SRC_PATH`, `RES_PATH`, and `BUILD_PATH` variables are defined. There are a couple other settings that must be set for the build process to complete; refer to the next section for those.

We recommend copying and modifying an existing Makefile from one of the example projects as necessary.

## Makefile Configuration

Configuration settings must appear at the top of the Makefile, before including the `megadev.make` parent. Some configuration settings are required, some are recommended, and some are optional. **Please pay careful attention with these settings, as many have length restrictions.**

### Compile-time Configuration

All settings can be changed at compile time by including them in the make command. For example:

```
make REGION=EU VIDEO=PAL all
```

### Required Settings

All of these settings must apepar in your makefile with a valid value.

#### `SRC_PATH`

The path to your source code. This should be relatve to your project root.

#### `RES_PATH`

The path to your resources (graphics, sounds, etc). This should be relatve to your project root.

#### `BUILD_PATH`

The path to the compilation workspace, where build artifacts (such as object files and symbol lists) will be stored. This should be relatve to your project root.

#### `DISC_PATH`

The path to the staging directory where all final files that will appear on the disc will be stored. This will be used as the source when generating the ISO file.

#### `MEGADEV_PATH`

The full path to your Megadev installation.

#### `PROJECT_ID`

This is the "short" name for your project. It should contain no spaces and be a **maximum of 11 characters**. It will also be used as the filename for the final output disc/cartridge file.

#### `TARGET`

Determines the target hardware for which the project should be built. Valid values are: `MEGADRIVE` and `MEGACD`.

### Recommended Settings

The following settings have default values and are thus not required to appear in your makefile, but it may be a good idea to include them for clarity of intent.

#### `REGION`

Specifies the hardware region.

This is *very* important for Mega CD builds as there is security code that must match what the internal BIOS is expecting for its region, but less important for Mega Drive builds as the hardware does not do a region check.

Valid values: `JP`, `US`, `EU`.

If unspecified, defaults to `US`.

#### `VIDEO`

Specifies the video output mode.

This is not directly used by Megadev, but can be used as a preprocessor directive in your code to determine the VDP configuration for your build target.

Valid values: `PAL`, `NTSC`.

If unspecified, defaults to `NTSC`.

### Optional Settings

These settings are mostly related to the ROM header, occupying the space between 0x100 and 0x200 within a Mega Drive ROM or the IP of a Mega CD game. The header is, for the most part, not actually used by the hardware and existed only as an administrative requirement by Sega. Therefore, you can have these fields contain whatever you would like. However, we recommend that you follow the guidelines as some tools use the metadata found here to identify/classify the game.

Unless otherwise specified, all text values should be standard ASCII. Please refer to the official Mega Drive documentation (or the many websites that cover the topic) for more information about the ROM header fields.

#### `PROJECT_NAME`

This is the full name of your project.

**Maximum of 48 characters.**

If unspecified, uses the value of `PROJECT_ID`.

#### `PROJECT_NAME_DOMESTIC`

This is similar to `PROJECT_NAME`, but it is intended as the title of the game in its native language.

Its primary use was for Japanese game titles using Japanese characters. Officially, only ASCII and Shift-JIS encoded Japanese text can be used here, but you can use any encoding you'd like. (Many games simply used the same value as `PROJECT_NAME` above.)

**Maximum of 48 bytes.**
 
Be very careful when using non-ASCII encoding (such as Shift-JIS) as these may contain multi-byte characters. Which means a single character may consume two (or more) bytes. Ensure that your text *data* fits within 48 bytes, and do not rely on character count only.

If unspecified, uses the value of `PROJECT_NAME`.

#### `HEADER_VOL_ID`

The is the name of the volume (the file system). This is different from the volume identifier within the ISO9660 Primary Volume Descriptor.

**Masimum of 11 characters.**

If unspecified, uses the value of `PROJECT_ID`.

#### `HEADER_SYS_ID`

This is the name of the "developers operating system filename" (per official documentation), though this description is somewhat nebulous. This does not need to refer to an actual filename, and many games simply use this as a "short name" for the game, while others used it for the name of their game's "engine" or system.

**Masimum of 11 characters.**

If unspecified, uses the value of `PROJECT_ID`.

#### `HEADER_SOFTWARE_ID`

This is the "software code" that uniquely identifies your game.

Officially, this uses specific codes and a number designated by Sega. Refer to the Mega Drive documentation for what this should officially look like if you want to emulate the "look and feel" of a proper header.

**Maximum of 14 characters.**

If unspecified, it defaults to `GM 00-0000-00`.

#### `HEADER_COPYRIGHT`

This contains the text (C) following by a four letter company name or code, then the month and year of the game's production.

Officially, the company name/code must be assigned by Sega, but since this string is not used by hardware, you can create your own value or leave it blank. For that matter, you can make the entire string freeform, though we recommend leaving it as a version/date identifier in some form.

**Maximum of 16 characters.**

If unspecified, defaults to a blank company name and the current month/year, e.g. "(C)      AUG.2025"

#### `HEADER_REGION`

Specifies the hardware region.

This is different from the `REGION` setting above in that this appears only within the header and is not actually used by the hardware. Officially, only the letters J, U and E are valid, representing Japan, US, and European regions. Multiple regions can be specified.

**Maximum of 16 characters.**

If unspecified, defaults to "JUE".

#### `VRAM_SIZE`

Specifies the VRAM size in the hardware.

You will not need to change this unless you are developing specifically for the Sega Teradrive or a Mega Drive modded with extra VRAM.

This is not directly used by Megadev, but can be used as a preprocessor directive in your code to determine the VDP configuration for your build target.

Valid values are `VRAM_64K`, `VRAM_128K`. 

If unspecified, defaults to `VRAM_64K`.

### Restricted Settings

The following are set automatically by Megadev and do not need to be modified. They are checked (in part) by hardware and changing them may make your game non-compliant/unbootable! They are provided for experimental purposes, but we do *not* recommend changing them! 

#### `HEADER_HARDWARE_ID`

The hardware name appearing in the software header. This will normally be set automatically based on the `REGION` setting. **Security checks are performed against this string!**

#### `HEADER_DISC_ID`

Applies to Mega CD only. The disc type appearing in the disc header. **The wrong setting can make your disc unbootable!**


# Megadev Concepts & Features

We have aimed to make Megadev as flexible and un-opinionated as possible: for devs who want to micromanage their project, for those who want to make extensive use of a library of pre-written code to make life simple, and for those somewhere in between. While there are recommended processes and tools, we have aimed to make as many things as optional as possible so you can write your code how you like it.

This section will go over Megadev's features and how to use them.

## Supported Languages/Compilers

Megadev supports C (with GCC extensions) and M68000 ASM. These can be mixed freely.

We do not yet support Z80 ASM for the Mega Drive's Z80 coprocessor, but this is on the roadmap.

We do not officially support C++ as we do not feel it brings anything that would support embedded development better than native C does, but you are welcome to retool the makefile to get it working with gcc's C++ compiler.

We may explore support for clang (i.e. llvm) sometime in the future.

## Modules (MMD/SMD)

(This applies to Mega CD development only.)

There are many ways that the Mega CD could be used when it comes to loading code from disc, but a common paradigm is to use modules. These are essentially self-contained programs, with all the code (and sometimes graphics and other resources) it needs in one file.

A module can represent a single "piece" of your game: the title screen, one level of the game, the credits sequence, and so on.

Megadev provides tools for creating and loading modules easily. Please see `docs/modules.md` for more details.

As with other Megadive components, the use of modules is optional. You are free to come up with your own software execution architecture.

## CD-ROM Access Framework

(This applies to Mega CD development only.)

At the hardware level, accessing data on the disc is done via Sub CPU BIOS calls. The process is somewhat complex as loading is accomplished by specifying a range of sectors on the disc instead of using filenames. Moreover, the transfer process is multi step, involving setting the destination in the CDC Mode register, using the CDC BIOS calls to start the transfer, and then monitoring the transfer status for its completion.

Megadev provides a simplified framework for disc file access, accomplished by simply specifying a filename and an output buffer. Please see `docs/cdrom.md` for more details about how to use it.

As with other Megadive components, the use of the CD-ROM header is optional. You are free to write your own disc access system if you wish.

## Boot Sector Generation

(This applies to Mega CD development only.)

The boot sector on the CD contains the software header, the IP (Initial Program), and the SP (System Program). These are the first pieces of code in your game to run on each CPU: the IP on the Main side and the SP on the Sub side.

A key piece of making your game bootable is the inclusion of the security code. This is a small chunk of code and data that *must* appear at the start of the IP. It is responsible for calling the "Produced By or Under License From..." screen displayed before the game loads. The BIOS stores a byte-for-byte copy of this code and it is checked before running the disc to ensure that it is valid Mega CD software.

Megadev will automatically create the boot sector as long as you have `ip.s` and `sp.s` files in your project. This is the only requirement for a Megadev project aside from the makefile requirements mentioned above.

Though it is technically possible to create the boot sector from C source, we recommend leaving them as ASM as they are very low level and must be fast and predictable. We highly recommend using an IP/SP from one of the example projects as a base and modify them as necessary.

Please see `docs/boot.md` for more details.

## Filename Schema

All of Megadev's functionality is access through headers or other include files present within the `lib` subdirectory. Files and subdirectories within this directory follow naming rules to help you decide how/where they should be used.

### Main/Sub Specific files

Functionality specific to either side (Main CPU/Sub CPU) is kept within subdirectories `main` and `sub` within `lib`. Functionality that is main/sub agnostic is stored in the root of `lib`.

It is very important to prefix included files with these subdirectories as some files have the same filename.

For example, code meant for the Main CPU side may have:

```
#include <main/memmap.h>
#include <main/bios.h>
#include <main/vdp.h>
#include <types.h>
```

While the Sub CPU may have:

```
#include <sub/memmap.h>
#include <sub/bios.h>
#include <sub/bram.h>
#include <types.h>
```

### Macros (.macros.s)

Assembly files that have the `.macros.s` suffix contain only macros and can thus be included multiple times without worry of symbol duplication.

The choice for what code was written as a macro and what was written as a subroutine is based on our judgement of how small the code is and how often it will be used. Code that initializes some hardware will probably be a subroutine as it will rarely be called; but a short piece of code that sets up te VDP port for a transfer, for example, will be a macro.

### Definitions (.def.h)

Files with the `.def.h` contain only C style `#define` directives. They contain bit positions, memory addresses and other constant values, usually for interacting with hardware.

They are used in both C and ASM development. Header guards are in place so they can be included multiple places.

# Further Reading

There are a number of additional documents in the `docs` subdirectory. We recommend at least skimming through all of these before starting development.

`boot.md` - Discusses the boot process of the Mega CD and important considerations for the IP/SP.

`cdrom.md` - Describes how to use the CD-ROM file access framework.

`dev_in_c.md` - Discusses important concepts for developing code in C for an embedded system.

`disc.md` - Explains how to master a proper disc image for the Mega CD.

`main_bios.md` - Goes into deep detail about using the Main BIOS and its many utility calls.

`megacd_dev.md` - Considerations for program architecture and development on the Mega CD.

`modules.md` - Discusses the modules concept and how to use them.

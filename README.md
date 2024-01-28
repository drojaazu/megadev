# [ M E G A D E V ]
*AV INTELLIGENT TERMINAL WITH OPTICAL DISC DRIVE*

Megadev is a Sega Mega CD devkit in C and 68k asm using a modern GNU build toolchain.

## Warning - Beta Version

Megadev has recently entered a "beta" phase where it will be properly revisioned going forward. It is still, however, very early in development and there are likely remaining bugs and rough spots. Improvements and fixes are welcome via pull requests.

As I would like to see this project grow, I am willing to help support new projects using Megadev to get off the ground. Feel free to direct any questions or feedback to [@suddendesu](https://twitter.com/suddendesu) on twitter.

## Build Environment & Toolchain

The code is written for use with GNU development tools built with the m68k-elf target. At a minimum, you will need binutils for the assembler and linker. Optionally, you can install gcc for programming in C and gdb for debugging.

It is also assumed to be running in a *nix environment. Theoretically, it should work in something like e.g. Cygwin, but this has not been tested.

### Toolchain setup

Your distribution may have an M68k cross architecture binutils/gcc/gdb package in its repo; search there first.

If you're using Arch Linux, the tools are available in the AUR:

https://aur.archlinux.org/packages/m68k-elf-binutils/

https://aur.archlinux.org/packages/m68k-elf-gcc/

https://aur.archlinux.org/packages/m68k-elf-gcc-bootstrap/

If a prebuilt package is not available, you will need to build the tools from source:

https://www.gnu.org/software/binutils/

https://www.gnu.org/software/gcc/

Be sure to include ```--target=m68k-elf``` when running the configure script for each package.

The make scripts assume the m68k cross compiler tools are in your path variable. If not, you will need to manually specify the paths of the tools in the global makefil.

### Toolchain Manuals

GNU assembler (as): <https://sourceware.org/binutils/docs/as/index.html>

GNU linker (ld): <https://sourceware.org/binutils/docs/ld/index.html>

GNU debugger (gdb): <https://sourceware.org/gdb/current/onlinedocs/gdb/>

GNU compiler (gcc): <https://gcc.gnu.org/onlinedocs/gcc/>

## Getting Started

Clone the repo and place it in a location such as /opt/megadev. In the root of the directory is `megadev.make` which will be used as a system-wide base for all projects. Edit the settings in its top section as necessary to match your build environment.

To start a new project copy the `new_project` directory from within the Megadev location to you project directory. It contains three files: a makefile,  `project.h` and `ip.s`. Modify the makefile as you'd like to specify subdirectories and compilation options. Modify `project.h` as you'd like to reflect your project information, being careful to maintain the required text lengths. `ip.s` is a minimal IP, your program entry. Afterwards, do `make init` in the project directory to create the directories, and move `project.h` and `ip.s` to the source directory. You are ready to begin development.

If you have not already done so, you'll need to familiarize yourself with the official Mega CD documentation which is readily available online.

You should also read the included documentation, namely:

 - design.md - Discusses Mega CD program design concepts
 - ip_sp.md - Introduces the IP and SP and their peculiarities
 - modules.md - Introduces MMD modules and how to use them
 - cdrom.md - Introduces the Sub CPU side CDROM access API
 - bootrom.md - Main CPU Boot ROM library reference
 - disc.md - Notes on creating discs

We recommend looking at the example projects included with Megadev, located in the `examples` directory.

As a note, we use such terms as "kilo" and "mega" when referring to base 2 values, meaning a kilobyte is 1,024 bytes and a megabit is 131,072 bytes. The author acknowledges the necessity and importance of the differentiation between decimal and binary base number identifiers in the engineering world but asserts that words like "kibibyte" and "mebibyte" sound ridiculous and, since this framework deals exclusively with retro hardware that existed before SI prefix standardization, chooses to use the old naming system. All SI prefixed numbers throughout the documentation are base 2.

## Building

The project can be built with the `make` command in the root of your project which will build sources and create an ISO. You can check the global makefile for some more specific make targets.

The default target hardware is specified in the project makefile. You can also change the targets per build, e.g.

    make HW_CFG="REGION=US VIDEO=NTSC VRAM_SIZE=VRAM_64K"

`REGION`, `VIDEO` and `VRAM_SIZE` will be available as preprocessor definitions in your code, allowing for conditional compilation.

Valid values for `REGION` are:
 - `JP` (Japan)
 - `US` (USA)
 - `EU` (Europe)

Valid values for `VIDEO` are:
 - `NTSC` (for JP and US regions)
 - `PAL` (for European regions)

Value values for `VRAM_SIZE` are:
 - `VRAM_64K`
 - `VRAM_128K`

The VRAM size option only applies to the Sega Tera Drive or Mega Drives that have been modded with extra VRAM. Unless you are specifically targeting those devices with the intention of using extended VRAM, this should be left at `VRAM_64K`. This setting can be safely left off when specifying the hardware config on the command line, as it will default to 64k.

Built modules will be placed in the `disc` subdirectory (or whatever path you set in the project makefile). You can manually add additional files into this directory and they will be included in the final ISO image.

### make Targets

(default) - Runs `init`, `boot_sector`, `modules`, `iso`.

`clean` - Removes all artifacts from the build path.

`init` - Creates the necessary build paths; called automatically by default, so it really isn't necessary to be specified manually, except perhaps.

`boot_sector` - Builds the IP/SP and generates the disc boot sector.

`modules` - Links all modules and outputs to the disc staging directory.

`iso` - Builds the final ISO image from all files in the disc staging directory.

## Library Sources & Workspace Configuration

Be sure to add `$(megadev_path)/lib` to the include path of your IDE or project configuration.

Within the `lib` directory are the definitions, wrappers and source code for Megadev. In general, code is "assembly first" with C wrappers around asm calls. In order to facilitate the mixture of C/asm in projects, all memory addresses, registers, and other compile-time constants use C-style `#define` directive (including assembly source files which are passed through the C preprocessor). Any files in the lib directory ending in `.def.h` contain only such definitions and can be `#include`'ed in either C or asm source.

For asm source, files ending in `.macro.s` contain macros, as the name suggests. Because these and `.def.h` files do not contain immediate code, they can be `#include`'ed anywhere in your source code without issues.

For C source, include the header files ending in `.h` without the `.def` subextension in your code. (The relevent `.def.h` file will already be included in the C header.)

# Megadev Core Concepts & Functions

Megadev provides three main areas of functionality: Modules, CD-ROM Access and System ROM Mappings.

## Modules

The memory layout of a standard Mega Drive cartridge game is relatively simple, with one contiguous block of address space (4 Megabits) available to the program at all times. Things are not so simple with the Mega CD, however. Memory is broken up into three distinct blocks called Word RAM, PRG RAM and Work RAM, which are 2, 4, and 0.5 Megabits in size respectively.

The configuration and role of each block of available memory during runtime is dependent on the developer and should be one of the fundamental aspects to consider when planning your program architecture. Even with a solid memory map in hand, actually building your code to run at certain offsets and doing so efficiently can become very complicated, very quickly.

Modules attempt to make things a bit easier on the developer by providing a somewhat generalized system for compiling code and data into binaries. You can think of modules as very small "ROMs" on the disc. This is how most Mega CD games work: the title screen is a module, the options screen is a module, each stage is a module, and so on. It is one self-contained piece of the game as a whole.

This system was inspired by Sonic CD and implements the game's module format (MMD). In fact, with Megadev you can load and run some of the simple MMD files from Sonic CD as-is!

This is probably the most central concept of Megadev and having an understanding of how to create modules is necessary for creating games. See `modules.md` for more information.

## CD-ROM Access Wrapper

Using only the built-in Mega CD BIOS calls to retrieve data from the CD-ROM is a rather arduous process of specifying a sector offset, waiting for a number of sectors to be read, monitoring the CDC as it processes the input, and then finally having the data appear in a buffer. Megadev provides an API that wraps all this up so that only a filename and an output address are required.

This is not a required component of Megadev and you are free to write your own loading routines, but if you'd like to use it, see `cdrom.md` for more information.

## Internal ROM Mappings (Boot ROM/BIOS)

The Mega CD is equipped with an internal ROM containing the CD player and backup RAM manager programs as well as a library of functions available to games. This internal ROM is split into two parts: the Boot ROM which runs on the Main CPU and the BIOS which runs on the Sub CPU. These user libraries are extremely useful and, in the case of the BIOS, necessary for using the hardware.

The Sub CPU side BIOS code is well-understood thanks to the official documentation from Sega. These system calls deal with disc data access, CD audio playback, backup RAM usage and other aspects of the Mega CD-specific hardware. Megadev provides definitions for these calls as well as C wrappers for those that are most commonly used.

The Main CPU side Boot ROM code, however, is not well documented. In fact, it is somewhat of a mystery. Within the Boot ROM is an array of functions that are linked through a jump table, indicating they are meant to be accessible across ROM revisions and thus intended for use by games. While it seems most retail games do not make use of these system calls, at least some do (such as Keiō Yūgekitai, Switch, Alshark, Jangō World Cup, Ishii Hisaichi no Daiseikai, and others). This is solid evidence that the calls can be freely used by developers if they wish.

We do not have any official documentation on the Boot ROM library. It is possible there never was such documentation to begin with, or was never translated to English. We have three different sources of Mega CD development reference manuals, most of which is wholly redundant, yet there is no sign of Boot ROM documentation. One of the later Technical Bulletins makes mention of these calls in passing, indicating that they may be used by games, so it wasn't secret information. That same document mentions source code available on disks and via the Sega BBS (which we do not have) which may be the source of this information.

In any case, there have been multiple attempts to reverse engineer the internal ROM and we now have a pretty good understanding of what sort of functions are provided. There are a number of useful tools for IO and VDP usage, among other things. Megadev provides documentation, asm defines and C wrappers so you can take advantage of these tools in your code.

Using the Boot ROM functions is not required, but whether you choose to use it or not, we recommend you see `bootrom.md` as it also addresses important memory layout information.

# What's NOT present?

The Mega CD brought with it a wide array of new functions to the Mega Drive, and we have only supported a few of those. There is currently no support for Backup RAM expansion cart usage and, most notably, hardware graphics resizing/rotation.

Of course, by "support," we just mean there are currently no helper functions within Megadev for these abilities. All hardware mappings are present and can be referenced in your program. And we will certainly be working to expand Megadev with more functionality in the future!

# Special Thanks

 - The IRC crew for occasional testing on different hardware versions
 - UCC Black coffee

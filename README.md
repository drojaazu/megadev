# MEGADEV - Sega Mega CD Development Framework
*AV INTELLIGENT TERMINAL WITH OPTICAL DISC DRIVE*

## WORK IN PROGRESS - PREVIEW VERSION
This project has not had extensive testing outside of the developer's personal projects. It is certain that there are bugs throughout. It is also well understood that it is not quite user friendly yet and there are inconsistencies in naming choices and usage throughout.

Moreover, although the core components have been implemented, there remains much work to be done on documentation, support tools and further support for hardware functionality. At this point, the code as it is currently available should be considered a "preview version." We will begin versioning the releases as things come together further.

This is just a hobby project with no corporate or organizational support and I feel it's better to get it out to the public as-is at this point and welcome any and all community feedback and fixes while we continue to work on it.

Feel free to direct any questions or feedback to [@suddendesu](https://twitter.com/suddendesu) on twitter. Bug reports and contributions via github are most definitely welcome.

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
Clone the repo and place it in a location such as /opt/megadev. In the root of the directory is `makefile_global` (the global makefile) which will be used as a system-wide base for all projects. Edit it as necessary to match your build environment.

To start a new project copy the `new_project` directory from within the Megadev location to you project directory. It contains two files: a makefile and `project.h`. Modify the makefile as you'd like to specify subdirectories and compilation options. Modify `project.h` as you'd like to reflect your project information, being careful to maintain the required text lengths. Afterwards, do `make init` in the project directory to create the directories, and move `project.h` to the source directory. You are ready to begin development.

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

Final built binaries will be placed in the `disc` subdirectory (or whatever path you set in the project makefile). You can manually add whatever additional files you'd like into this directory and they will be included in the final ISO image.

## Library Sources & Workspace Configuration
Be sure to add `$(megadev_path)/lib` to the include path of your IDE or project configuration.

Within the `lib` directory are the definitions, wrappers and source code for Megadev. In general, code is "assembly first" with C wrappers around asm calls. In order to facilitate the mixture of C/asm in projects, all memory addresses, registers, and other compile-time constants use C-style `#define` macros, including assembly source files which are passed through the C preprocessor. Any files in the lib directory ending in `_def` contains only such definitions and can be `#include`'ed in either C or asm source.

For asm source, files ending in `_macros` contain macros, as the name suggests. Because these and `_def` files do not contain immediate code, they can be `#include`'ed anywhere in your source code without issues.

For C source, header files without `_def` or `_macros` in the filename should be the files to include in your code. (The relevent `_def` file will already be included in the C header.)

# Megadev Core Concepts & Functions
Megadev provides three main areas of functionality: Modules, CD-ROM Access and System ROM Mappings.

## Modules
The memory layout of a standard Mega Drive cartridge game is relatively simple, with a large contiguous block of address space (32 Megabits!) available to the program. Things are not so simple with the Mega CD, however. Memory is broken up into three distinct blocks (Word RAM, PRG RAM and Work RAM), all of which are extremely small in comparison (2, 4, and 0.5 Megabits, respectively).

The tradeoff is that the Compact Disc can contain many hundreds of megabits of data, but the configuration and role of each block of available memory during runtime is dependent on the developer and should be one of the fundamental aspects to keep in mind when planning your program architecture. Even with a solid memory map in hand, actually building your code to run at certain offsets and doing so efficiently can become very complicated, very quickly.

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
The Mega CD brought with it a wide array of new functions to the Mega Drive, and we have only supported a few of those. There is currently no support for Backup RAM expansion cart usage, PCM audio playback and, most notably, hardware graphics resizing/rotation.

Of course, by "support," we just mean there are currently no helper functions within Megadev for these abilities. All hardware mappings are present and can be referenced in your program. And we will certainly be working to expand Megadev with more functionality in the future!

# Special Thanks
 - The #nihongo crew for occasional testing on different hardware versions
 - @plutiedev for their abundant knowledge of the hardware
 - Ueshima Coffee for producing UCC Black

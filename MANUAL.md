
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

Megadev is intended for those who already have some experience with C or M68k assembly programming and have at least a passing familiarity with embedded systems. It is less "friendly" than something like [SGDK](https://github.com/Stephane-D/SGDK). This is due in large part to the addition of the Mega CD (which SGDK does not support) which brings with it increased complexity, but also arises from a desire for a development system that is more flexible for skilled developers and less reliant on an abundance of external utilities.

The original target hardware for the project was the Mega CD, with support for vanilla Mega Drive development being somewhat of a late addition. That aside, there is already a wealth of information about and examples for programming the Mega Drive. As such, most of the information introduced here is focused on the Mega CD aspect of the project.

# Getting Started

## Build Environment

In order to compile code for the Mega Drive/CD, you will need a build environment that targets the M68000 CPU architecture. Megadev is designed to be used with GNU [binutils](https://www.gnu.org/software/binutils/) and [gcc](https://www.gnu.org/software/gcc/) compiled with the m68k-elf target. In addition, you will need `mkisofs` to generate the .iso disc image for Mega CD games.

The simplest way to set up such a build environment is to use the included Dockerfile as a dev container. Otherwise, you can install the necessary tools manually, either via your distro's repository or compiling from scratch.

### Docker

The included Dockerfile can be used to set up a [dev container](https://containers.dev/) in a supported IDE such as [VS Code](https://code.visualstudio.com/). This is the modern, preferred method for development as it should work on any host platform (Windows, Linux, MacOS).

It uses a minimal Debian installation and installs all necessary build tools. The Megadev project is installed to `/opt/megadev` within the container. From there you can copy one of the examples into the workspace and begin development.

### Manual Installation - By Distro

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

### Manual Installation - Compiling From Source

If your distro does not contain pre-built packages for the GNU build tools with the M68000 target, you can build them from scratch. The source for binutils and gcc can be found here:

https://www.gnu.org/software/binutils/

https://www.gnu.org/software/gcc/

You *must* add `--target=m68k-elf` as an option when you run the configure script in order target the M68000 architecture! You will also need to modify the `M68K_PREFIX` variable in the Megadev project makefile to indicate the prefix used for the tools. By default, this should be `mm68k-elf-`.

You may wish to use a helper script [like the ones found here](https://github.com/kentosama/m68k-elf-gcc).

## Install Megadev

(This step is not necessary if you are using the Dockerfile and a dev container.)

Clone the repo and place it in a wherever you would like (we recommend /opt/megadev). In the root of the directory is the `megadev.make` file which will be used as a system-wide base for all projects. Edit the settings in its top section as necessary to match your build environment. Alternatively, set the values 

## Build An Example Project

Once your build environment is in place, you should attempt to compile and run one of the example projects. We recommend copying the examples to somewhere outside of the Megadev installation directory before compiling.

To build the example project, run `make disc` from the root directory. This should generate a disc.iso file which can run in an emulator or burnt to disc. You will need to modify the `MEGADEV_PATH` variable inside makefile within the project if you installed it somewhere besides `/opt/megadev`.

Try `hello_world` first and then `ipx_spx` after that, as these are the simplest examples of booting the Mega CD and loading files from disc.

After that, feel free to try the remaining examples. In all cases, check the `README.md` file for details about each example.

# Project Composition & Compilation

Megadev uses GNU `make` for its build system. At the bare minimum, a Megadev project must contain a Makefile which *contains certain required configuration variables* (discussed below) and *includes the Megadev parent Makefile*. You are free to lay out your project tree as you like so long as the `SRC_PATH`, `RES_PATH`, and `BUILD_PATH` variables are defined. There are a couple other settings that must be set for the build process to complete; refer to the next section for those.

We recommend modifying an existing Makefile from one of the example project to start.

## Makefile Configuration

Configuration settings must appear at the top of the Makefile, before including the `megadev.make` parent. Some configuration settings are required, some are recommended, and some are optional. **Please pay careful attention with these settings, as many have text length limits or other restrictions.**

All settings can be changed at compile time by including them in the make command. For example:

```
make REGION=EU VIDEO=PAL disc
```

### Required Settings

#### `SRC_PATH`

The path to your source code. This should be relatve to your project root.

#### `RES_PATH`

The path to your resources (graphics, sounds, etc). This should be relatve to your project root.

#### `BUILD_PATH`

The path to the compilation workspace, where build artifacts (including symbol lists) will be stored. This should be relatve to your project root.

#### `MEGADEV_PATH`

The path to your Megadev installation (NOT your project).

#### `PROJECT_ID`

This is the "short" name for your project. It should contain no spaces and be a **maximum of 11 characters**. It will also be used as the filename for the final output disc/cartridge file.

#### `TARGET`

Determines the target hardware for which the project should be built. Valid values are: `MEGADRIVE` and `MEGACD`.

#### `SRC_PATH`

The path to your source code. This can be relative to where the makefile is run (which should be yoru project root).

#### `RES_PATH`

The path to non-code resources. This can be relative to where the makefile is run (which should be yoru project root).

#### `BUILD_PATH`

The path to the where compile time build artifacts will be stored, including symbol lists. This can be relative to where the makefile is run (which should be yoru project root).

### Recommended Settings

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

These settings are mostly related to the ROM header, occupying the space between 0x100 and 0x200 within a Mega Drive ROM or the IP of a Mega CD game. The header is, for the most part, not used by the hardware and existed only as a requirement by Sega. Therefore, you can make these fields say whatever you would like. However, we recommend that you generally follow the expected content and rules expected of the header as some tools use the metadata here to identify/classify the game.

Unless otherwise specified, all text values should be standard ASCII. Please refer to the official Mega Drive documentation (or the many websites that cover the topic) for more information about the ROM header fields.

#### `PROJECT_NAME`

This is the full name of your project.

**Maximum of 48 characters.**

If unspecified, uses the value of `PROJECT_ID`.

#### `PROJECT_NAME_DOMESTIC`

This is similar to `PROJECT_NAME`, but it is intended as the title of the game in its native language.

Its primary use was for Japanese game titles using Japanese characters. Officially, only ASCII and Shift-JIS encoded Japanese text can be used here, but you can use any encoding you'd like. Most games simply used the same value for `PROJECT_NAME` and `PROJECT_NAME_DOMESTIC`

**Maximum of 48 bytes.**
 
Be very careful when using non-ASCII encoding (such as Shift-JIS) as these may be multi-byte per character. Which means a single character may consume two (or more) bytes. Ensure that your text *data* fits within 48 bytes, and do not rely on character count only.

If unspecified, uses the value of `PROJECT_NAME`.

#### `HEADER_VOL_ID`

The is the name of the volume (the file system). This is different from the volume identifier within the ISO9660 Primary Volume Descriptor, but it would 

**Masimum of 11 characters.**

If unspecified, uses the value of `PROJECT_ID`.

#### `HEADER_SYS_ID`

This is the name of the "developers operating system filename" (per official documentation). This does not need to refer to an actual filename, and many games simply use this as a "short name" for the game.

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

The following are set automatically by Megadev and do not need to be modified. They are checked (in part) by hardware and changing them may make your game non-compliant/unbootable! They are provided for experimental purposes, and we do *not* recommend changing them! 

#### `HEADER_HARDWARE_ID`

The hardware name appearing in the software header. This will normally be set automatically based on the `REGION` setting. **Security checks are performed against this string!**

#### `HEADER_DISC_ID`

Applies to Mega CD only. The disc type appearing in the disc header. **The wrong setting can make your disc unbootable!**


# Megadev Concepts & Utilities

We have aimed to make Megadev as flexible and un-opinionated as possible: for devs who want to micromanage their project, and for those who want to make extensive use of a library of pre-written code to make life simple, as well as for devs somewhere in between. While there are recommended processes and tools, we have aimed to make as many things as optional as possible so you can write your code how you like it.

This section will go over the basics of a Megadev project, compiling that project, and using the library.

[[the only requirment is the boot sector setup and makefile compliance]]

## Supported Languages

Megadev supports C (with GCC extensions) and M68000 ASM. These can be mixed freely.

We do not yet support Z80 ASM for the Mega Drive's Z80 coprocessor, but this is definitely on the roadmap.

We do not officially support C++ as we do not feel it brings anything that would support embedded development better than native C does, but you are welcome to retool the makefile to get it working with GCC's C++ compiler.

We may explore support for clang (i.e. llvm) sometime in the future.

## MEGADEV Library

Megadev brings with it a collection of headers and macros to provide common functionality on the hardware. 

### Filename Schema

#### Main/Sub Specific files

Functionality specific to either side (Main/Sub) is kept within a subdirectory within the library. Intuitively, code for the Main CPU (or other Megadrive specific components) is in `main/` and for the Sub CPU (or other Mega CD specific components) in `sub/`.

It is very important to prefix included files with this subdirectory as some files have the same filename.

For example:

```
#include <main/memmap.h>
#include <main/bios.h>
#include <main/vdp.h>
#include <types.h>
```

Here we include the memory layout specific to the Main side, the wrappers for the Main BIOS calls, and VDP utilities. All of these are prefixed with `main/` as they are specific to that side. Notice that `types.h` does not have a prefix as it can be used by both sides.

If you are doing Megadrive only development, only the `main/` side files will be used.

#### Macros

Assembly files that have the `.macros.s` suffix contain only macros and can thus be included multiple times without worry of symbol duplication.

The choice for what code was written as a macro and what was written as a subroutine is based on our judgement of how small the code is and how often it will be used. Code that initializes some hardware will probably be a subroutine as it will rarely be called; a short piece of code that sets up te VDP port for a transfer, for example, will be a macro.

#### Definitions

Files with the `.def.h` contain only C style `#define` directives. They contain bit positions, memory addresses and other constant values, usualy for interacting with hardware.

They are used in both C and ASM development. Header guards are in place so they can be included multiple places.

### Mega CD BIOS Calls

The Mega CD has a number of system calls available to developers thanks to its internal ROM, for both the Main and Sub CPUs. The calls for the Sub CPU, which deal primarily with CD access, are well-documented in the official Sega documentation. The Main side calls, however, do not have any documentation and we only know of their presence by reverse engineering. (Interestingly, many Japanese games use these "undocumented" system calls, so it may have been a case of simply not providing some documentation in English. We do not have any official documentation in Japanese to confirm this.)

Megadev provides C wrappers for these system calls on both CPUs, with comments. Please refer to the official Sega documentation for the Sub CPU system calls, and to main_bios.md for the Main CPU calls.

## Modules (MMD/SMD)

(This applies to Mega CD development only.)

There are many ways that the Mega CD could be used when it comes to loading code from disc and running it, but a common paradigm is to use modules. These are essentially self-contained programs, with all the code and graphics and other resources it needs.

The basic concept is to have a small memory-resident kernel that manages the whole program at a very low level, including the loading and unloading of modules. A module can represent any one piece of your game: the title screen, one level of the game, the credits sequence, and so on.

Megadev provides tools for creating and loading modules easily. Please see `modules.md` for more details.

(Note also that modules are optional. You are free to come up with your own design for file loading.)

## CD-ROM Access Framework

(This applies to Mega CD development only.)

Accessing data on the disc is done via commands provided by the BIOS on the Sub CPU side. The system in place is somewhat complex, as instead of using filenames, a start/end sector must be specified. Moreover, the transfer process is multi step, involving setting the destination in the CDC Mode register in the Gate Array and using the CDC BIOS calls to start the transfer and monitor its status.

The access framework provided within Megadev simplifies this process by simply specifying a filename and an output buffer. Please see `cdrom.md` for more details about how to use it.

(Note that this too is optional and you may write your own disc access system if you wish.)

## Boot Sector Generation

(This applies to Mega CD development only.)

The boot sector on the CD contains the software header, the IP (Initial Program), and the SP (System Program). These are the first pieces of code in your game for each CPU: the IP on the Main side and the SP on the Sub side.

A key piece of making your game bootable is the inclusion of the security code. This is a small chunk of code and data that *must* appear at the start of the IP. It is responsible for calling the "Produced By or Under License From..." screen displayed before the game loads. The BIOS stores a byte-for-byte copy of this code and it is checked before running the disc to ensure that it is valid Mega CD software.

Megadev will automatically create the boot sector as long as you have `ip.s` and `sp.s` files in your project. This is the only requirement for a Megadev project aside from the `config.h`/`Makefile` mentioned above.

Though it is technically possible to create the boot sector from C source, we highly recommend leaving them as ASM as they are very low level and must be fast and predictable. We highly recommend using an IP/SP from one of the example projects as a base and modify them as necessary.



# Mega CD Development

## Exception Vectors

Since both the Main and Sub CPUs are booted by the internal ROM, the system exception vector table is set by these programs. The system table is confgured such that any actionable exceptions are "forwarded" to a jump table with user-defined values.

On the Main side, this table begins at 0xFFFD00. This is located near the top of Work RAM and this should generally be considered the limit of Work RAM usage. It is not a 1:1 mapping of the execption vector list as some entries are missing, most notably the interrupt vectors except for levels 2, 4, and 6 (the EXT, HBLANK and VBLANK interrupts, respectively), as the other levels are not used by the system.

Entries within this table are user-defined, and you will almost certainly change at least the VBLANK vector to handle this important interrupt. Error vectors and TRAP vectors are also available, as well as the reset vector.

As expected from a jump table, each vector is prefixed with a JMP instruction to redirect to the handler. It is possible to change this instruction to any two byte opcode, though there is very little reason to do so. Perhaps the only use case here is to change the JMP to an RTE for unimplemented exception handlers. Should you wish to change the instruction, you will need to subtract 2 from the memory pointer for the entry.

## Main Side Stack Pointer

Officially, the space from 0xFFFD00 to 0xFFFFFF is reserved for system use and unavailable to the user, aside from changing exception vectors (see the Exception Vectors section above). Thus, 0xFFFD00 is the safe "end" of Work RAM available to the user and the address used for the stack pointer. The official documentation sets aside 256 bytes, from 0xFFFC00 to 0xFFFD00 as the stack.

However, within this reserved system space, only the exception vector jump table is of dire importance. The jump table ends at 0xFFFDB4, and from there, most of the variables used by the Boot ROM are stored. The space used by these variables ends at 0xFFFE58 (this may vary by revision), and from there to the end of Work RAM, it appears to be unused space.

What this means is, if you are not using the Main side BIOS library *at all*, then the entire space from the end of the jump table (0xFFFDB4) to the end of Work RAM is free to use. That is 588 bytes of space, more than double the 256 bytes allotted officialy. In this case, setting your stack pointer to 0 is acceptable and you can utilize lower Work RAM more fully, all the way up to 0xFFFD00.

Even if you are using the Main BIOS with its memory usage ending around 0xFFFE58, that is still 424 bytes of stack space, far more than the official allotment. You can perhaps get more if the BIOS calls you make do not use the space towards the end of the reserved block.

In summary, it's probably safe to set your stack pointer to 0 to gain some extra stack space rather than leaving it at 0xFFFC00 as it is from boot, especially if you are coding in C as your stack frames may be unnecessarily large at times. If you do see strange issues, check that your stack isn't getting too deep and interfering with/being damaged by memory space used by BIOS.

In all cases, the space from 0xFFFD00 to 0xFFFDB4 *must be preserved as the exception jump table*.

---

### The Kernel

Unlike a cartridge based Mega Drive game, where the entire program and all its data is present as one fixed, contiguous block of memory space, the parts of a Mega CD game come and go in the form of files loaded from disc. Because the data is transient, we need a "master program" that does the low level work of deciding which file to load, executing its code until completion, and then choosing the next file to run.

This is basically the concept of an operating system kernel: the core code that remains loaded into memory for the entire duration of the system's uptime. It manages the loading and execution of user programs as well as providing services and hardware interfaces to those programs. While we certainly won't need something as complex as the kind of kernel you would find in a PC operating system, having an ever-present supervisor program is the best way to design your Mega CD project.

Our kernel will have the largest scope, maintaining the global state of the entire program, and that state determines the flow of the program. For example: the system has booted, and we're at the kernel entry. We set the global state ID to 1 for 'intro screen' and enter our main loop. For state ID 1, we load `intro.mmd` into memory and jump to it. It fades in a couple nice logos, sets the global state to 2 for the title screen, then exits, and we're now back in the kernel. The kernel loops again and sees that we're in state ID 2 now, so it loads and executes the title screen. From there, the user selects 1P Start or 2P Versus or Options, setting either state 3, 4 or 5 respectively. After making the selection, it exits, and we're back in the kernel, which loads the appropriate file for the state. And so on. At the most basic level, the kernel will keep track of "where we are" in the program as a whole and load/run the appropriate file for that state.

Since the kernel is loaded in memory at all times, it can also provide funcionality to the transitory modules making up most of the program. For example, maybe our game uses lots of text, so why not put the font and the print routines inside the kernel. That way we don't need to compile that code/data into each module, reducing data redundancy, code size and (most importantly) load times. Another good use case for keeping data resident is a fancy loading screen graphic. Re-loading the loading screen from disc each time we load a file is a bit silly and we're just adding additional wait time due to another disc access. Instead we can keep our fancy loading graphics in memory and the kernel can display them when making a load call. 

Given the dual-microprocessor design of the Mega CD system, the kernel will be split into two parts, running independently on either CPU. The relationship between the two, as their names imply, is that the Main CPU kernel is the "leader," maintaining the state of the program and making requests for support data to the Sub CPU kernel. In more modern terms, we can think of this as a Server/Client architecture, where the Sub CPU kernel is the server, accepting requests from and sending responses to the client, the Main CPU. The two kernels tell each other about the status of these requests and keep in sync via the Gate Array communication registers.

### The Library

Earlier we said that the Mega Drive has no operating system, that it is a devoid of any code except for what is in the cartridge. The addition of the Mega CD hardware with its internal ROM brings a CD player, memory manager and, importantly for us, a library of system functions.

On the Sub CPU side, we have the BIOS calls which are used for data I/O (namely, CD-ROM and Backup RAM access) and for controlling some miscaellaneous Mega CD specific hardware components. These calls are documented in the official Sega docs and Megadev provides definitions, structures and wrappers to help you use them in your code. You will definitely be using these in your Sub CPU kernel.

On the Main CPU side, we have what we refer to as the Boot ROM library (abbreviated as `bootlib` throughout Megadev). This is a set of functions that provide a wide range of general functionality useful to Mega Drive games, such as VDP DMA operations, controller input reads, palette and sprite list caches, and much more. The catch is that some of these functions place certain expectations on your program structure and use precious Work RAM space. Moreover, these calls are not documented and our understanding of their use is based on reverse engineering.

While the Sub CPU BIOS calls are required and a basic part of Mega CD development, the Main CPU Boot ROM library is entirely optional to use. While helpful, some of the components of the library will affect your program architecture more fundamentall than others, and you should carefully examine how the pieces you wish to use will be integrated. Refer to bootrom.md for an in-depth look at this library of functions.


### Memory Planning
As we have discussed, there is no hardware delineation of read-only (ROM) and storage (RAM) for your Mega CD program. As stated in the main readme:

> The memory layout of a standard Mega Drive cartridge game is relatively simple, with a large contiguous block of address space (32 Megabits!) available for the program. Things are not so simple with the Mega CD, however. We have three seperate memory blocks (Word RAM, PRG RAM and Work RAM), all of which are extremely small in comparison (2, 4, and 0.5 Megabits, respectively).

As part of your program design, you must consider how those memory banks will be utilized and partitioned. Though each one is a block of writable memory, it's helpful to use the analogy of "ROM" and "RAM" for program code/data and for runtime memory, respectively, when speaking in terms of your program. So for the memory regions being used to run code (as opposed to being a simply buffer), we need to manually define the boundaries of "ROM" and "RAM" for the program.

While this adds a layer of complexity in your planning, it also offers some flexibility: the layouts are not required to be static and can be changed as needed. For example, maybe one module has lots of graphics data for something like a cutscene, but doesn't have much logic. It will need lots of ROM space but not much RAM. And maybe the next module will have actual gameplay, which will need a fair bit more RAM to keep track the game state. You can tailor the memory layout to each module as necessary.

#### Work RAM layout
The memory block to which you will need to pay particular attention is the Main CPU Work RAM. It is the smallest of the banks but arguably the most important. This is from where the "outermost" pieces of code will execute, which is to say the kernel. The size may be further restricted depending on how much of the Boot ROM library you choose to use. For that reason, we'll take a closer look at Work RAM.

Here is its default layout, with no user programs or Boot ROM library usage:

    FF0000  +----------------+
            | IP/AP Use      |
            |                |
            |                |
            =                =
            |                |
            |                |
            |                |
            | Top of Stack   |
    FFFD00  +----------------+
            | System Use     |
            |                |
    FFFFFF  +----------------+

The System Use area, beginning at 0xFFFD00, contains the jump table for the exceptions/interrupts. The jump table is a list of JMP commands: two bytes for the opcode and a four byte pointer. The M68000 vector table is defined by the Boot ROM when it is loaded into Main CPU memory space by the hardware at startup. The vectors point to locations within this System Use partition within Work RAM and cannot be changed. Therefore, this area is considered to be off limits except for changing the pointers.

In reality, the jump table only takes up 180 bytes, leaving the space from 0xFFFDB4 and onward free for use (588 bytes). However, this free space is used by many of the Boot ROM library functions, so keep that in mind if you plan to use the built-in library. See the bootrom.md file for more info.

The stack is allocated to 0xFFFD00 by default. This can, of course, be re-pointed to wherever you'd like, though with the default layout it's already in an optimal position. Recall that the stack fills upwards, so you'll want to allocate enough space for it when planning the space for your ROM/RAM partitions above it. Too little space and it will overflow up into your RAM, possible overwriting runtime memory or runtime memory corrupting the stack. Too much space and it becomes wasted memory. This is an area of your program that you will likely want to optimize, especially if you are primarily coding in C. Lots of nested function calls with registers pushes and allocated local variables will quickly fill up the stack. Prefer using global values whenever possible to prevent too much stack usage.

Let's map out a possible memory layout, apropos of nothing. As we said earlier, our goal is to define the boundaries of "ROM" (program code and resources) and "RAM" (runtime memory) for our program within the confines of usable space.

In our program architecture, we'll load modules to Word RAM and run them directly from there. This means Work RAM is exclusive to our kernel. That simplifies things. So first, let's say 512 bytes allocated to the stack. This should actually be more than enough. (The Boot ROM allocates only 256 bytes; see bootrom.md for much more on that.) That leaves us a little less than 63KB for our ROM and RAM partitions.

The kernel won't have lots of logic by itself, let's say 1KB for global game state amd some basic services (global mode, game save information, input mirrors, random number generator, etc). Let's also keep our graphics system in the kernel as well: a VDP register cache, a sprite cache, some space for decompressing graphics, a DMA queue. Maybe anothet 4KB. We want to also keep our task processor within the kernel. Let's say a task frame is 64 bytes, and want to support a maximum of 32 tasks, meaning we need to allocate 2KB for the task system.

That's about 7KB of RAM, leaving us with just under 56KB of ROM. Our memory map looks like this:

    FF0000  +----------------+
            | "ROM"          |
            |                |
            |                |
            |                |
            |                |
    FFDF00  +----------------+
            | "RAM"          |
            |                |
    FFFB00  +----------------+
            | Stack          |
    FFFD00  +----------------+
            | System Use     |
            |                |
    FFFFFF  +----------------+

The start address and length of the ROM and RAM sections would then be specified as global symbols within the module code. More on that in modules.md.

This example is not based on any real-world implementations, though you could use it as a generic layout to get things up and running. However, as we have said a couple times, once you bring the Boot ROM library into the picture, the space available for usage may change drastically. We'll continue with Work RAM mapping while using the library in bootrom.md.

#### Memory Layout in Other Regions

We have focused heavily on Work RAM because of its importance as the location of the kernel and its trickiness due to its small size. The other memory banks are, comparitively, easier to work with.

PRG RAM has comparitively huge storage. Of the 4Mbit (512KB) of space available, only 24KB is reserved for the BIOS. Here is where the SP, the Sub CPU side kernel, will execute. You are free to allocate as much or as little of the available PRG RAM space for it as you'd like.

Word RAM is, officially, a buffer shared between the two CPUs. You can execute code from this region, and many games do just that, but it should never be used for low level code (i.e. the kernel) on either CPU. Similarly, be careful when running your vertical/horizontal interrupts from Word RAM and be sure to repoint them to default handlers in the kernel (in Work RAM) when loading another module. This is because Word RAM is only available to one CPU at a time, and if an integral piece of code suddenly becomes unavailable due to the buffer ownership changing, the system will crash.

Word RAM also has two modes. In one mode, all 2Mbits is "owned" by one of the CPUs and is inaccessible to the other. One of the main aspects of syncing the Main and Sub CPU is reconciling ownership of Word RAM in this mode. In the other mode, Word RAM is split into 1Mbit banks, of which each is owned by one CPU. The ownership can be readily switched, making this mode useful for streaming data (such as video) which is loaded from disc via the Sub CPU into one bank while the Main CPU copies the data from the other bank to VRAM, and then switching banks to continue the process.

As Word RAM is highly transitory, there is less need to do any serious memory mapping. A module running from this space will still need "ROM" and "RAM" areas partitioned, but this is limited in scope to only that module and will disappear when it is unloaded anyway.



## Development in C


Writing your code in C rather than raw assembly can make development much easier.  However, there are some things you should keep in mind, especially if you are coming from a history of C/C++ development on PC.

### Standard Library

The equivalent of the standard library when developing for the Mega CD is the Boot ROM library. This provides a number of helpful tools, but it does not provide any of the equivalent headers you may be used to seeing (string.h, stdlib.h, stdio.h, etc). Megadev provides headers for the internal Boot ROM library system calls, as well as some additional helper functions and macros, but use of the internal library has caveats relating to memory usage of which you should be aware (see bios.md).

Development for a "bare metal" embedded system like the Megadrive/Mega CD requires a paradigm shift away from PC development. Here, there is no console so there is no printf. There is no OS kernel managing memory, so there is no malloc/free. Anything you want to happen, you will need to write yourself (see design.md).

It is theoretically possible to use newlib, a reimplementation of much of the C standard library for embedded systems. However, we feel this is a bit of overkill for something like the Mega Drive/CD system and thus have not done any integration testing with it. You are free to attempt this yourself, of course, and we welcome a report of your experience with it.

### Global Variable Initialization

In short, statically allocated variables (such as global variables declared outside of function scope) are not initialized even if you specify an initial value. E.g., if you declare and initialize a variable outside of a function like so:

	u16 initial_value = 100;

The value will not be set to 100 and will, instead, equal whatever already exists in memory at the location to which it was assigned by the linker. You will need to manually assign values to such variables before they are used.

The reason for this relates to the initialization process. Initial values need to be stored somewhere, and in execution containers like ELF and PE this data is included in the final binary. Such formats will automatically copy the initialization data to memory where these static variables are linked, and thus the initial value is already present before main execution begins.

Retro consoles like the Mega Drive/CD do not use such execution containers. There is no automatic initialization utility and code execution begins immediately.

Statically allocated variables are linked in the .data memory segment, so it is possible to copy this data as the first action in your program to manually do initialization. Indeed, this is on our to-do list for Megadev, but as it is not currently implemented be careful with global variable initialization.

### Integer Types
　・
We highly recommend *not* using the `int` type for integers. Instead, use the sized types (`char`, `short`, `long`) or better yet their typedefs (`s8`/`u8`, `s16`/`u16`, `s32`/`u32`). With gcc-m68k, int is four bytes in size, which may be a waste of space depending on the operation you are trying to perform. Memory usage should always be optimized.

### Fractional Types

The M68000 CPU does not have a floating point unit and use of the `float` type will only work using software emulation that is not included with Megadev.

Rather than floating point values, equivalent hardware from the era used fixed point math for fractional values. Megadev implements simple fixed point types and conversion macros in the fixed.h header. You can find examples of its usage in the `gfx` example project.

(If you wish to include floating point emulation, you will need to implement [the expected GCC routines](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html#Soft-float-library-routines).)

### Heap Allocation

As mentioned above, there is no malloc/free system for heap allocation. You could write one yourself, but it is likely to be more efficient to develop a custom memory provisioning system that meets the needs of your specific game rather than a generalized allocator.

Consider why you need to allocate space. It is generally to instantiate an "object," some piece of code which maintains state and a lifetime outside of the stack frame.

In the case of a retro game, such objects are generally sprites on the screen which share some attributes: an x and y position, a shape, flip and mirror flags, a tile index, a palette offset, and so on. So right away we have a shared structure for all objects that can be used as a base. If we decide to set aside, say, 32 bytes in memory for an object, we can fit all those display parameters as well as a pointer or jumptable offset to the object's code and have a bit of space leftover to store soem data specific to that object (like it's remaining HP or current attack pattern ID).

With a fixed size for an object's memory usage, we can specify a block of memory to hold a certain maximum number of objects as part of our memory layout planning. We can then write a simple object manager which will track the number of objects and provide a pointer to usable space within that block when requested to instantiate a new object.

You can see a rather simple example of this sort of setup in the `ipx_spx` example project.

### Stack Usage

Compared to hand-written assembly which often passes values by register, C tends to push values to the stack when calling functions. This means the stack can fill up very quickly, especially if you are using the default Boot ROM library memory layout which sets aside only 256 bytes for the stack (see `bios.md`).

There are some strategies we can employ to help mitigate the risk of an overflow:

- Allocate a large enough stack to begin with. Bootlib allocates 256 bytes, which (anecdotally speaking) is generally enough space for simple programs, but 512 bytes or even a full kilobyte may be necessary for a complex project.

- If you plan to use Bootlib, consider moving the stack to somewhere else in Work RAM. As mentioned above, 256 bytes may not be enough for more complex systems. Many Mega Drive games set the stack to offset 0, which works downward from the very top of RAM. (Of course, you'll need to account for this in your memory map and ensure your memory usage doesn't creep up into the space you've set aside for the stack.) Doing so also frees up the 256 bytes that were previously set aside for the stack for your own use. As a side note, if you are using the default Bootlib memory layout and you start to see mysterious changes to your color palette as it runs (especially palettes 3 and 4), you may have a stack overflow, as the palette cache is directly above the stack space.

- Keep the number of values passed in functions to a minimum. If you need to pass a lot of information, considering packing it into a struct and pass a single pointer instead.

- Try to keep the number of chained function calls to a minimum. A function calling a function calling a function calling a function will generate a stack frame for each call, which includes space for any local variables. If possible, call a function to do work on an object passed by pointer, then return and call the next pointer, and so on. Similarly, avoid recursive functions.





# Currently Unsupported Features and Known Issues/Limitations

## Lack of "naked" function support

One of the most frustrating aspects of working with GCC is the lack of flexibility in some (admittedly esoteric) aspects. Chief among these has been GCC's insistence on creating function prologue/epilogues for overlays (modules).

In summary, when a function is called, certain CPU registers may be modified as a side of that code. If those registers were in active use by the calling code, then upon returning things will be broken as the values were unexpectedly changed. To prevent this, values for in-use registers are saved to the stack for safekeeping and reloaded when the function has returned. This automatic saving and loading of registers (and possibly other function call utilities) is called the prologue and epilogue of a function.

The automatically generated epilogue/prologue may be unnecessary if the function in question is an entry for a module. The register push takes up valuable stack space in our limited RAM. Moreover, the compiler may also LINK the A6 register, which essentially removes that register from use. This is especially problematic since a number of Main BIOS calls make use of A6.

A function lacking an epilogue and prologue is called "naked," and unfortunately GCC does not provide a way to specifying a function as naked (at least not for the M68000 architecture). At this point, there doesn't seem to be a good solution for this. The stack can be manually reset by some inline ASM at the top of a function to recover the space used, but the (admitterly few) CPU cycles are still used for the push/pop. For BIOS calls using A6, we have wrapped these calls with a push/pop of A6, which keeps things stable but adds more CPU cycles...

If you need to be particular about speed, we recommend checking any BIOS calls being made that may use A6 and perhaps write your own inline ASM caller to deal with tha LINKed A6 issue more efficiently. Of course, this issue only applies to C programming; if you are developing in pure ASM, this is not a problem.

If anyone has any suggestions for how to solve for this, we greatly welcome your comments.


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

The included Dockerfile can be used to set up a [dev container](https://containers.dev/) in a supported IDE such as [VS Code](https://code.visualstudio.com/). This is the modern, preferred method for development as it should work on all platforms (Windows, MacOS, Linux).

It uses a minimal Debian installation and installs all necessary build tools. The Megadev project is installed to `/opt/megadev` within the countainer. From there youc an copy the skeletone project or one of the examples into the workspace and begin development.

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



# Understanding The Hardware

> "If you wish you wish to make an apple pie from scratch, you must first invent the universe." - Carl Sagan

Development for embedded hardware such as old video game consoles is markedly different from programming for a PC. There is no operating system, no standard library, no I/O abstraction layer, not even a simple `print` command to make text show up on the screen. The code you write is the sum of all the code that will run on the hardware.

Writing a game for retro hardware is, in a way, writing an "operating system" from scratch, which can be both thrilling and a frustrating chore. Before you can really dig into the fun stuff, you have to plan how to manage your hardware resources (especially VRAM) and how to architect your program. The complexity of this planning/design phase escalates once we bring the Mega CD into the picture. You now have a second CPU, non-persistent memory regions and buffer ownership to manage as well.

This is why, before anything else, you should familiarize yourself with retro hardware concepts, such as:

  - CRT screens, scanlines, and the vertical blanking interval
  - Indexed color (aka palette based) graphics
  - Tiles and tilemaps
  - System interrupts
  - The difference between CD-ROM (ISO9660) and CD-DA

There are a plethora of online tutorials that introduce development for the Mega Drive intended for absolute beginners. These should cover most of the items listed above in the context of the Mega Drive.

There is, however, comparitively little coverage of development for the Mega CD, so you will also want a copy of the official Sega developer documentation for the Mega CD. These documents are readily available online and easily found with a simple search. Though they will primarily be used for reference, we still highly recommend reading through them to become familiar with the capabilities, expectations and overall deisgn of the hardware. We will introduce some important Mega CD concepts in the next section, and the included example projects are well-commented too, but the official documentation is necessary for development.

Before starting a serious project, you should have a basic understanding of the following Mega Drive/Mega CD specific pieces and how they work together at a high level:

  - VDP usage, including DMA transfers
  - Reading from controllers
  - Which components are managed by which CPU (i.e. VDP & VRAM -> Main CPU, CD Drive & Internal BRAM -> Sub CPU)
  - Work RAM, Word RAM, PRG RAM
  - The Gate Array (at a very high level, you don't need to focus too much on registers...)
  - The Gate Array Comm Flags/Command/Status registers for CPU communication (...except for these)
  - The IP/SP

# Basic Mega CD Concepts

Here we will highlight some important components of programming for the Mega CD.

## Boot Process (IP/SP)

The boot sector on the CD contains the software header, the IP (Initial Program), and the SP (System Program). These are the first pieces of code in your game for each CPU: the IP on the Main side and the SP on the Sub side.

To make a game valid and bootable, there are two elements that must be in place: the header and the security code.

The header is a string of text that appears at the very beginning of the boot sector on the disc. It is very similar to the header found at the start of a Mega Drive game. The Mega CD version, however, has a disc identifier to indicate it is a bootable game.

The security code is a small chunk of code and data that *must* appear at the start of the IP. It is responsible for calling the "Produced By or Under License From..." screen displayed before the game loads. The BIOS stores a byte-for-byte copy of this code and it is checked before running the disc to ensure that it is valid Mega CD software.




## Internal ROM (BIOS/Boot Library)

The Mega CD includes an 1 megabit internal ROM that includes system calls and utilities to both the Main and Sub CPU, as well as the startup screen, the CD player and the memory manager. On startup, this ROM is loaded to the Main CPU side, which then decompresses and transfers the code for the Sub CPU side.







## Program Architecture



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

The equivalent of the standard library when developing for the Mega CD is the Boot ROM library. This provides a number of helpful tools, but it does not provide any of the equivalent headers you may be used to seeing (string.h, stdlib.h, stdio.h, etc). Megadev provides headers for the internal Boot ROM library system calls, as well as some additional helper functions and macros, but use of the internal library has caveats relating to memory usage of which you should be aware (see bootlib.md).

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

Compared to hand-written assembly which often passes values by register, C tends to push values to the stack when calling functions. This means the stack can fill up very quickly, especially if you are using the default Boot ROM library memory layout which sets aside only 256 bytes for the stack (see `bootlib.md`).

There are some strategies we can employ to help mitigate the risk of an overflow:

- Allocate a large enough stack to begin with. Bootlib allocates 256 bytes, which (anecdotally speaking) is generally enough space for simple programs, but 512 bytes or even a full kilobyte may be necessary for a complex project.

- If you plan to use Bootlib, consider moving the stack to somewhere else in Work RAM. As mentioned above, 256 bytes may not be enough for more complex systems. Many Mega Drive games set the stack to offset 0, which works downward from the very top of RAM. (Of course, you'll need to account for this in your memory map and ensure your memory usage doesn't creep up into the space you've set aside for the stack.) Doing so also frees up the 256 bytes that were previously set aside for the stack for your own use. As a side note, if you are using the default Bootlib memory layout and you start to see mysterious changes to your color palette as it runs (especially palettes 3 and 4), you may have a stack overflow, as the palette cache is directly above the stack space.

- Keep the number of values passed in functions to a minimum. If you need to pass a lot of information, considering packing it into a struct and pass a single pointer instead.

- Try to keep the number of chained function calls to a minimum. A function calling a function calling a function calling a function will generate a stack frame for each call, which includes space for any local variables. If possible, call a function to do work on an object passed by pointer, then return and call the next pointer, and so on. Similarly, avoid recursive functions.

# Megadev Concepts & Utilities

## Filename Schema

files in main/ for main CPU
files in sub/ for sub CPU`
.macros.s

.def.h

## Project Structure
discuss the makefile and using ips_spx as a skeleton

## Modules (MMD/SMD)

## CD-ROM Access Framework

Accessing data on the disc is done via commands provided by the BIOS on the Sub CPU side. The system in place is somewhat complex, as instead of using filenames, a start/end sector must be specified. Moreover, the transfer process is multi step, involving setting the destination in the CDC Mode register in the Gate Array and using the CDC BIOS calls to start the transfer and monitor its status.

The access framework provided within Megadev simplifies this process by simply specifying a filename and an output buffer.


---

The CD-ROM access wrapper provides a simplified method of retrieving files from disc. At a high level, it runs a loop on the Sub CPU that is "pumped" by an update call during INT2. As the access operation runs, it will pause at certain points and return from the interrupt. It saves this break point as a pointer and work resumes from that point at the next INT2. In this way, the Sub CPU is not entirely blocked waiting for time-consuming IO operations.

TODO: Most of this is asm focused, need to expand C coverage/documentation.

### Example

Please have a look at the `ipx_spx` sample project for an example of file loading using this system.

### Setup

CD-ROM access can only be done with the Sub CPU, so requests for files should be done via the communication registers within the gate array. The framework code should remain resident in memory, so we recommend including it within the SP that is loaded to the Sub CPU side during boot.

Include `sub/cdrom.macro.s` and `sub/cdrom.s`in the SP code. In your SP init subroutine (called `_usercall0` in the BIOS Manual), prime the access loop by calling the `INIT_ACC_LOOP` macro.

Somewhere in your INT2 subroutine (_usercall2), make a call to the `PROCESS_ACC_LOOP` macro to keep the access loop moving. You may want to put this at the end of the subroutine or push the registers before calling as it may clobber any number of registers.

Finally, in the early part of SP main subroutine (_usercall1), you'll want to load and cache the file information by setting ACC_OP_LOAD_DIR as the access operation and waiting for it to complete. There is space allocated for 128 files by default, but this can be adjusted to match your project by changing the size of the `dir_cache` buffer in `sub/cdrom.s`.

### Usage

At this point, you are ready to load files. There are four steps involved:
  - Set the pointer to the filename string in `filename`
	- Set the destination buffer in either the GA DMA register or `file_dest_ptr` depending on transfer type
	- Set the appropriate access operation
  - Wait for the operation to complete

You do not need to set the CDC device destination in the _GAREG_CDCMODE register. This will be done automatically depending on the access operation chosen.

### Filenames

The filename must conform to [ISO9660 Level 1 standards](https://wiki.osdev.org/ISO_9660#Filenames). Basically this means that filenames must contain:

- only UPPERCASE alphanumeric (A-Z, 0-9) characters and underscore
- a maximum of 8 characters with a 3 character extension
- the version identifier

Finally, the filename in memory must be a standard null termninated C string.

Note that you can keep your filenames as lowercase on your local system. The ISO creation process will convert them as necessary. (Also note that if you mount the ISO on your local system, they may appear lowercase in your file manager. However, they are UPPERCASE in the filesystem on disc, which is how you will be accessing them on the Mega CD.) You must limit the filename size to 8.3, however, and use only valid characters (A to Z, 0 to 9, and _).

You may not be familiar with the version identifier as is not shown in directory listings. It is a part of the ISO9660 standard, though, and is present within the filesystem table. In reality, it is not used by applications and foregoing a lot of unnecessary background information, it simply means your filenames will be appended with `;1` when referring to them within the filesystem.

As a quick example, you may have `title.mmd` on your local system. This will be represented as `TITLE.MMD;1` within the ISO filesystem on disc, and that is how you should refer to it in your code.

### Destination buffer

Please refer to the CDC section of the Mega CD Software Development documentation before or alongside this section.

The buffer where your data will be stored must be specified in one of two ways depending on the transfer type. If you plan to use DMA, you will need to set the destination address in the `GA_DMAADDR` register. If you plan to use non-DMA transfer via the CDC host data register, you will need to set the address in the `file_buff` pointer.

### Access operation

Finally, you will need to set the access operation on `access_op`. The operations for loading files are:

  ACC_OP_LOAD_CDC
  ACC_OP_LOAD_CDC_DMA
  ACC_OP_LOAD_PRG_DMA
  ACC_OP_LOAD_PCM_DMA

(Note that we do not currently support the Main CPU read option, mostly because it is not well understood.)

ACC_OP_LOAD_CDC will load the data to any memory address available to the Sub CPU, while the DMA options transfer data to a specific device given a relative address specified in the GA DMA register. ACC_OP_LOAD_CDC is the easiest to work with as you can simply specify an absolute address and be done. The DMA options likely provide greater transfer speed (limited by the optical drive, of course), though what sort of speed advantages or if any bus access issues may occur are unknown.

### Wait for data

After setting the access operation, you will need to wait for the data to completely load into your buffer. This is done by checking `access_op` in a loop (while calling the INT2 wait in the loop!) until the value has returned to ACC_OP_IDLE (i.e. 0). When that occurs, the transfer is either complete or failed. You should then check the `access_op_result` for the result code:

  RESULT_OK
	RESULT_LOAD_FAIL
	RESULT_NOT_FOUND

If the result is ok, your data should be ready in the buffer. The load fail result means there was a read failure of some sort (likely due to a damaged disc or old harware). Not found indicates the filename provided could not be found in the file system.

## Convenience routines

For simple file loads, you can use the `load_file_sub` convenience subroutine to package most of this up for you. Simply place the pointer to the filename in a0 and the pointer to the destination buffer in a1, call it, and check the result in d0.

The `get_acc_op_result` routine will check if the access operation is still busy, with carry clear (cc) indicating the operation is complete and carry set (cs) indicating it is still in progress.

There is also the WAIT_FOR_ACC_OP macro which will check get_acc_op_result in a loop until the operation is complete.

# Development Considerations for Mega CD development

Programming for the Mega Drive only is much simpler than programming for the Mega CD. Primarily, you do not have to worry about loading files from disc, transferring their data across buffers, and then cleaning up when that file's data is no longer needed. For a Mega Drive cartridge, all the data and code for the entire program is in a fixed location in memory, meaning less planning needs to be spent on architecting your program in terms of hardware.

Therefore the following sections are commentary and suggestions for planning a Mega CD game in particular.
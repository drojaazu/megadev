# Megadev Modules
Modules are the most fundamental aspect of Megadev. A module is a compiled binary, similar to a small ROM. It contains both program code and resource data (graphics, stage data, etc.) and can represent one piece of the whole of your project. For example, the title screen may be a module, the options screen a module, a gameplay stage a module, and so on.

Modules are generally meant to be ephemeral: they are loaded, executed and unloaded. Since they are likely to display graphics, read player inputs, etc, they usually run on the Main CPU, though the module system can certainly work on the Sub CPU side as well.

## Memory Layout
Because a module is a compiled program, we need to define the memory map it will use. A module can use any of the memory blocks available to the CPU on which it runs.

We have already discussed memory usage on a Mega CD system within the `design.md` file at a high level. Now we will examine how to implement that design.

Basically, we need to tell the linker at what memory offset the ROM and RAM sections begin (the origin) and the size of those sections (the length, in bytes). Normally we would need to create unique memory maps for each module that doesn't have identical memory usage, but in the interest of keeping things generic/flexible, Megadev has a system of using global symbols to make the link script more dynamic. The symbols are as follows:

    MODULE_ROM_ORIGIN
    MODULE_ROM_LENGTH
    MODULE_RAM_ORIGIN
    MODULE_RAM_LENGTH

For example, if we want our module to run on the Main CPU from Word RAM in 2Mbit mode with 1.5 Mbits for program/data (ROM) and 0.5 Mbits for runtime memory (RAM), we would specify the values like so:

    GLOBAL MODULE_ROM_ORIGIN 0x200000
    GLOBAL MODULE_ROM_LENGTH 0x180000
    GLOBAL MODULE_RAM_ORIGIN 0x380000
    GLOBAL MODULE_RAM_LENGTH 0x80000

(GLOBAL is a Megadev macro, a wrapper for .equ that also makes the symbol global.)

In terms of object sections, .text and .rodata sections will be placed in ROM; .data and .bss will be placed in RAM. There is also a .init section which is guaranteed to be at the beginning of the ROM space. It's recommended only one routine be placed in .init and used as an entry point at a known address.

## Module Entry and Execution Space
There are two requirements which must appear somewhere in the module's code: an entry point and a destination address. The entry point is simple enough: a global symbol named `main`. (For C, this means a function with the signature `void main()`.)

The destination address is a global symbol called `MMD_DEST`. It specifies to where in memory the module code should be copied before running. This is useful, for example, to copy a module into Work RAM after it has been loaded into Word RAM from the Sub CPU. However, if your code doesn't need to be moved and should run right where it is (which is usually Word RAM after being loaded), a destination is not necessary. In that case, `MMD_DEST` should be set to 0. (Note that `MMD_DEST` should always be defined, even if the value is 0.)

Things get tricky when you consider that modules have a 0x100 byte header. If a destination is specified, then everything *after* the header is copied. That is, the header is not copied to the destination and the code begins at the destination address. However, if you run it in place (with no destination specified), the code begins at 0x100, after the header which is in memory.

This is tricky because `MMD_DEST` and the `MODULE_ROM_ORIGIN` symbol discussed in the previous section are very similar in concept. `MODULE_ROM_ORIGIN` specifies *where the module data begins,* the data outside of its header. This means if `MMD_DEST` was specified, the ROM origin and the destination address will be the same. If no destination was specified, the ROM origin will be where the module was loaded *plus 0x100* to account for the header.

From the point of view of the Main CPU, you're almost always going to be following one of two patterns: load data to Word RAM and run it, or load data to Word RAM, copy it to Work RAM, and then run it. So, as a general rule for each pattern:
 
 - If you're going to run a module from 2Mbit Word RAM, set `MMD_DEST` to 0 and set `MODULE_ROM_ORIGIN` to 0x200100.

 - If you're going to copy the module to Work RAM (0xFF0000 for this example), set both `MMD_DEST` and `MODULE_ROM_ORIGIN` to 0xFF0000.

## Modules in Detail
Megadev uses the MMD format which originates from Sonic CD. These are binaries with a 0x100 byte header containing the following information:

    0x00.w Flags
    0x02.l Runtime location
    0x06.w Module size (in longs)
    0x08.l Pointer to main routine
    0x0C.l Pointer to HINT handler
    0x10.l Pointer to VINT handler
    0x14 to 0x100 Padding

The most important entry here is the pointer to the main routine. After the module is loaded and set up, the code will jump (not call) to this routine. You can also specify a new HINT/VINT handler to be installed before the jump to main, though these are optional. All three of these values are automatically determined via the link script, which look for the main routine as a global function called `main` and the interrupt the handlers as global functions called `hint` and `vint`.

The runtime location entry specifies the address from which the code will be executed. The MMD loader will first copy the binary portion of the module to this address before calling main. If this value is zero, the code will not be moved and will execute in place wherever it is located. This destination address is specified by a global symbol called MMD_DEST. (Note that if this value is zero, so that is executed in place, you will need to account for the 0x100 header when determining the ROM origin. See the second example at the end of this section.)

The module size is the size of the binary portion of the module, that is, the size of the module without the header. This value will be automatically calculated by the link script.

There is only one bit in the flags value, bit #6, which will return Word RAM control to the Sub CPU before jumping to main. (This appears to be the only bit used by Sonic CD.) You are free to use the rest of the bits as you wish, but we may associate additional functions to the lower bits of the word someday, if necessary. The flags are specified by a global symbol called MMD_FLAGS. Specifying the flags is optional.

This is followed by padding up to offset 0x100. This is not strictly necessary and we only do so because that's what Sonic CD does. You are free to extend into it with your own metadata (it would be a perfect place for identification text during debugging or for hidden "easter egg" text). You can reduce or remove it entirely if you wish, but you will need to modify the MMD loader code (in mmd_exec.s) and the .header section in the module LD script (module_mmd.ld) to account for the start of the module's binary section.

In summary, for a valid MMD module, you must specify an entry point by having a global function called `main` and you must specify a runtime address with a global symbol called MMD_DEST. You can also optionally specify a new HINT/VINT handler by having a global function called `hint` and `vint`, respectively, within your module code, and can also optionally specify optional flags with a global symbol called MMD_FLAGS.

(As a side note, there doesn't seem to be a way to manually define a symbol (i.e. a named memory address) in C. You will need to specify this in asm using .equ/.global. A good idea is to have a single asm definitions file and include it in your MMD def.)

Let's look at a couple theoretical examples. Say we have a module we want to run from Main side's Work RAM. We already have a memory resident bit of code in Work RAM taking up the first 0x400 bytes, so we want this module to run from 0xFF0400. The runtime location (set in the MMD_DEST global symbol) will be 0xFF0400. The MMD loader will automatically move the binary portion of the module from Word RAM to 0xFF0400 and then jump to it.

Another example. We want the module to run from Word RAM (at 0x200000) this time. Since the Sub CPU will load the MMD to Word RAM anyway, there's really no reason to move it anywhere. We set the runtime location (MMD_DEST) to 0 so it will be executed right where it sits. Just to make things more tricky, however, you will need to specify the ROM origin as 0x200100 in this case, *not* 0x200000. This is because of the 0x100 byte header. Since the ROM origin specifies the start of code, it needs to be set to the end of the header.

As a general rule, then, MMD_DEST will match MODULE_ROM_ORIGIN except for when MMD_DEST is 0 (i.e. run from Word RAM) in which case MODULE_ROM_ORIGIN should be 0x200100 (for Main CPU) or 0x80100 (for Sub CPU).

One thing we'd like to explore going forward is how to make this layout system more user friendly.

## Building modules
A module is created from a .def file, which is simply a text file containig a list of source code filenames. Each of these files are compiled seperately then linked together to make the final module binary. Basically, a def file is a compilation script, similar in concept to a makefile. Each .def file generates one module (mmd, smd or bin), which will be written to the disc output directory as part of the build process.

If you follow the usual C standard where headers and implementations are split into .h and .c files respectively, keep in mind that `#include`ing a header in your source is not enough. The implementation of that header (that is, the matching .c file) should be included in the .def file for that module.

Def files should end with `.mmd.def`, `.smd.def` or `.bin.def` depending on the type of module. Each line of the file should contain one filename without a path. Only source code files ending in .c and .s as well as module files (.mmd, .smd, .bin) for resident code are allowed. (We will discuss resident modules in the next section.)

For example, the title screen may be `title.mmd`, for which we will need `title.mmd.def` that has the list of all source code used in the module:

    title_memmap.s
    title.c
    title_res.s
    graphics.c

Those files will be compiled then linked together to create the final `title.mmd` on the disc.

## Memory Resident Modules
It may be more efficient to keep a portion of program code or resources in memory as you load different modules throughout the lifetime of the game. For example, you can keep your small loading screen code/graphics in memory so it itself does not need to be re-loaded from disc each time. Or you can keep some useful global functions that are available to all your code, such as VDP utilities. Sonic CD does this by loading an extended IP (IPX) into Work RAM which acts as a small "master" program to load and run modules from Word RAM and provide some other utilities. Memory resident modules are the basis for the "program kernel" concept that is discussed in the `design.md` file.

A memory resident module is built with a def file just like any other module. However, the modules which depend on it must specify it as a reference. This is done by including the resident module's filename in the def file. The resident module data will NOT be included in the output module, but will only be used as a reference when linking.

Returning to the title screen example above, let's say we have a kernel, `ipx.mmd`, resident in Work RAM and which contains some useful functions. In `title.mmd.def`, we would simple add the resident module's filename:

    title_memmap.s
    title.c
    title_res.s
    graphics.c
    ipx.mmd

There are two special cases for the IP and SP. Though they do not generate their own files on the disc, they can still be specified as a resident module with `ip.bin` and `sp.bin` in the def file.

Do not attempt to "chain" multiple resident modules together in a series of reliance. All resident modules must be fully built first so they can be referenced by later modules. If one resident module relies on another resident module, there is no way to guarantee which will be built first, and the build process may fail. You can, however, have multiple resident modules and even load them at the same time (provided their memory space does not overlap), so long as they do not reference each other.

Ultimately, the best use of this feature is to keep things simple. The "program kernel" concept discussed in `design.md` is probably the best use case: one single resident binary per CPU that is loaded once early on and is present for the lifetime of the game.

## Boot ROM Considerations
The Boot ROM is the Main CPU side code that resides within the internal Mega CD ROM. It contains the code for the built-in CD player and memory manager. It also contains a user-accessible "library" of utility functions. For more on that, please see `bootrom.md`.

Although the functions provided by the Boot ROM are very helpful, you must be aware that some of them exclusively use a significant chunk of the already limited Work RAM. 

### Without Boot ROM Functionality
Outside of the System Use area, you have 63.25kb (64,768 bytes) of space to be divided up into blocks for program/resource, RAM, and stack. 

Here is an example setup that does not use the Boot ROM at all:

    FF0000  +----------------+
            | IP/AP Use      |
            | (i.e. ROM)     |
            | (0xF000 bytes) |
            |                |
            |                |
    FFF000  +----------------+
            | RAM            |
            | (0xB00 bytes)  |
            |                |
    FFFB00  +----------------+
            | Stack          |
            | (0x200 bytes)  |
    FFFD00  +----------------+
            | System Use     |
            |                |
    FFFFFF  +----------------+

Expressed as Megadev definitions, this layout looks like this:

GLOBAL MODULE_ROM_ORIGIN 0xFF0000
GLOBAL MODULE_ROM_LENGTH 0xF000
GLOBAL MODULE_RAM_ORIGIN 0xFFF000
GLOBAL MODULE_RAM_LENGTH 0xB00

### Using Boot ROM Functionality
If you choose to use Boot ROM functions, your memory map will look like this, as defined in MEGA CD TECHNICAL BULLETIN #3:

    FF0000  +----------------+
            | IP/AP use      |
            |                |
            =                =
            |                |
            |                |
    FFF700  +----------------+
            | Boot ROM Use   |
            |                |
    FFFC00  +----------------+
            | Stack          |
    FFFD00  +----------------+
            | System / Boot  |
            |       ROM Use  |
    FFFFFF  +----------------+

Here, there is an additional 1,280 bytes used by the Boot ROM, from 0xFFF700 to 0xFFFC00, reducing our program use space to 61.75kb (63,232 bytes). Moreover, more of the previously free space in the System Use area is now taken up by several key Boot ROM components.

A major problem, however, is the stack, which has only 256 bytes of space. Without carefully monitoring your code this is very easy to overflow, especially if you are using C. (Immediately below the stack, at the top of the Boot ROM Use space, is the CRAM cache. If you start seeing incorrect colors on screen, especially on the fourth palette line, chances are you have a stack overflow.) You can, of course, move the stack to within the IP/AP Use area, but you will have to balance that with the size of your ROM/RAM usage.

You can optimize memory usage by only using certain pieces of the Boot ROM tools. If you only use things with a small memory footprint, such as the DMA transfer routines, you can use more of the reserved space within the Boot ROM exclusive area and within the System Use block at the end. You just need to account for what is used by the routines you employ, and that may require checking a Boot ROM disassembly to work around the memory locations that are used.

## Pitfalls of running code in Word RAM
Though the Mega CD manuals describe Word RAM as primarily for data exchange between Sub and Main, you can run code from here with no problem. Sonic CD does this with its module architecture. In fact, it's the easiest way of running simple games.

However, keep in mind that there may be unforeseen issues with this, primarily with interrupt handlers. Consider what happens if you set your VBLANK handler inside your module running in Word RAM, and then you go to load another module. What happens when you grant the Word RAM back to the Sub CPU, but your VINT is still pointing to that function in Word RAM? It's now no longer accessible, resulting in the VINT call failing and the Sub CPU not receiving INT2 (at best) or the whole system crashing due to reading invalid opcodes (at worst).

For that reason, make sure your interrupt handlers are in memory that won't be disrupted easily, or make sure that they have been repointed to such memory before making changes. If you are using a program kernel paradigm or a resident module, consider keeping your interrupt handlers there.

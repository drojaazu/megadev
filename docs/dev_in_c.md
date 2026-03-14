# Development Using C

Writing your code in C rather than raw assembly can make development much easier.  However, there are some things you should keep in mind, especially if you are coming from a history development on PC.

## Standard Library

There is no equivalent to a "standard library" like there is for PC development. This means common headers like `stdio.h` and `string.h` are not available. You literally only have C keywords available to you.

This development kit, Megadev, partially fills the role of standard library by providing helpful functions, defines and macros.

There is also the Main BIOS and Sub BIOS which provide a number of hardware utility functions.

Development for a "bare metal" embedded system like the Mega Drive/Mega CD requires a paradigm shift away from PC development. Here, there is no console so there is no printf. There is no OS kernel managing memory, so there is no malloc/free. Anything you want to happen, you will need to write yourself.

It is theoretically possible to use newlib, a reimplementation of much of the C standard library for embedded systems. However, we feel this is overkill for something like the relatively simple Mega Drive/CD system and thus have not done any integration testing with it. You are free to attempt this yourself, of course, and we welcome a report of your experience with it.

## Global Variable Initialization

In short, statically allocated variables (such as global variables declared outside of function scope) are not automatically initialized even if you specify an initial value. For example, if the following declaration is at the global scope, outside of a function:

	u16 initial_value = 100;

... then the value will not be set to 100 and will, instead, equal whatever already exists in memory at the location to which it was assigned by the linker.

The technical reason for is that specifying a default value will simply include that value in the .data section of the executable binary. It is up to the program initialization to copy values from .data into memory before main() is called. Executable containers like ELF and PE include code to take care of this copying automatically, but retro consoles like the Mega Drive/CD do not use such execution containers. There is no automatic initialization utility and code execution begins immediately.

Megadev includes a utility to copy the .data section into memory, in the `init.macros.s` file. Instead of using this directly, however, you may want to use either `main/ipx_init.s` or `std_init.s` for self-loading and standard modules respectively. Simply include this within the prerequisite list for your module and it will both zero out RAM and copy initialized variable data.

Please see the makefile for the example projects.

## Integer Types

We recommend *not* using the `int` type for integers. With gcc-m68k, int is four bytes in size, which may be a waste of space depending on the operation you are trying to perform. Memory usage should always be optimized.

Instead, use the sized types (`char`, `short`, `long`) or better yet their typedefs for clarity (`s8`/`u8`, `s16`/`u16`, `s32`/`u32`). 

## Fractional Types

The M68000 CPU does not have a floating point unit and use of the `float` type will only work using software emulation that is not included with Megadev.

Rather than floating point values, hardware from the era used fixed point math for fractional values. Megadev implements simple fixed point types and conversion macros in the `fixed.h` header. You can find examples of its usage in the `gfx` example project.

(If you wish to include floating point emulation, you will need to implement [the expected GCC routines](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html#Soft-float-library-routines).)

## Heap Allocation

As mentioned above, there is no malloc/free system for heap allocation. You could write one yourself, but it is likely to be more efficient to develop a custom memory provisioning system that meets the needs of your specific game rather than a generalized allocator.

Consider why you need to allocate space at runtime. It is generally to instantiate an "object," some piece of code which maintains state and a lifetime outside of the stack frame.

In the case of a retro game, such objects are generally sprites on the screen which share some attributes: an x and y position, a shape, flip and mirror flags, a tile index, a palette offset, and so on. So right away we have a shared structure for all objects that can be used as a base. If we decide to set aside, say, 32 bytes in memory for an object, we can fit all those display parameters as well as a pointer or jumptable offset to the object's code and have a bit of space leftover to store soem data specific to that object (like it's remaining HP or current attack pattern ID).

With a fixed size for an object's memory usage, we can specify a block of memory to hold a certain maximum number of objects as part of our memory layout planning. We can then write a simple object manager which will track the number of objects and provide a pointer to usable space within that block when requested to instantiate a new object.

In other words, consider what components may be instantiated and cleared during runtime and the maxium number of those components may be instantiated at one time, and pre-allocate space for them in your memory map.

You can see a rather simple example of this sort of setup in the `new_project` example.

## Stack Usage

Compared to hand-written assembly which often passes values by register, C tends to push values to the stack when calling functions. This means the stack can fill up very quickly, especially if you are using the default Boot ROM library memory layout which sets aside only 256 bytes for the stack (see `main_bios.md`).

There are some strategies we can employ to help mitigate the risk of an overflow:

- Allocate a large enough stack to begin with. The Main BIOS tools allocate 256 bytes, which is generally enough space for simple programs, but 512 bytes or even a full kilobyte may be necessary for something more complex.

- If you plan to use Bootlib, consider moving the stack to somewhere else in Work RAM. As mentioned above, 256 bytes may not be enough for more complex systems. Many Mega Drive games set the stack to offset 0, which works downward from the very top of RAM. (Of course, you'll need to account for this in your memory map and ensure your memory usage doesn't creep up into the space you've set aside for the stack.) Doing so also frees up the 256 bytes that were previously set aside for the stack for your own use. As a side note, if you are using the default Bootlib memory layout and you start to see mysterious changes to your color palette as it runs (especially palettes 3 and 4), you may have a stack overflow, as the palette cache is directly above the stack space.

- Keep the number of values passed in functions to a minimum. If you need to pass a lot of information, considering packing it into a struct and pass a single pointer instead.

- Try to keep the number of chained function calls to a minimum. A function calling a function calling a function calling a function will generate a stack frame for each call, which includes space for any local variables. If possible, call a function to do work on an object passed by pointer, then return and call the next pointer, and so on. Similarly, avoid recursive functions.

## Lack of "naked" function support

One of the most frustrating aspects of working with GCC is the lack of flexibility in some (admittedly esoteric) aspects. Chief among these has been GCC's insistence on creating function prologue/epilogues for overlays (modules).

In summary, when a function is called, certain CPU registers may be modified as a side of that code. If those registers were in active use by the calling code, then upon returning things will be broken as the values were unexpectedly changed. To prevent this, values for in-use registers are saved to the stack for safekeeping and reloaded when the function has returned. This automatic saving and loading of registers (and possibly other function call utilities) is called the prologue and epilogue of a function.

The automatically generated epilogue/prologue may be unnecessary if the function in question is an entry for a module. The register push takes up valuable stack space in our limited RAM. Moreover, the compiler may also LINK the A6 register, which essentially removes that register from use. This is especially problematic since a number of Main BIOS calls make use of A6.

A function lacking an epilogue and prologue is called "naked," and unfortunately GCC does not provide a way to specify a function as naked (at least not for the M68000 architecture). At this point, there doesn't seem to be a good solution for this. The stack can be manually reset by some inline ASM at the top of a function to recover the space used, but the (admitterly few) CPU cycles are still used for the push/pop. For BIOS calls using A6, we have wrapped these calls with a push/pop of A6, which keeps things stable but adds more CPU cycles...

If you need to be particular about speed, we recommend checking any BIOS calls being made that may use A6 and perhaps write your own inline ASM caller to deal with tha LINKed A6 issue more efficiently. Of course, this issue only applies to C programming; if you are developing in pure ASM, this is not a problem.

If anyone has any suggestions for how to solve for this, we greatly welcome your comments.

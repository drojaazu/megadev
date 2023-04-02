# Development using C

Writing your code in C rather than raw assembly can make development much easier.  However, there are some things you should keep in mind, especially if you are coming from a history of C/C++ development on PC.

## Standard Library

The equivalent of the standard library when developing for the Mega CD is the Boot ROM library. This provides a number of helpful tools, but it does not provide any of the equivalent headers you may be used to seeing (string.h, stdlib.h, stdio.h, etc). Megadev provides headers for the internal Boot ROM library system calls, as well as some additional helper functions and macros, but use of the internal library has caveats relating to memory usage of which you should be aware (see bootlib.md).

Development for a "bare metal" embedded system like the Megadrive/Mega CD requires a paradigm shift away from PC development. Here, there is no console so there is no printf. There is no OS kernel managing memory, so there is no malloc/free. Anything you want to happen, you will need to write yourself (see design.md).

It is theoretically possible to use newlib, a reimplementation of much of the C standard library for embedded systems. However, we feel this is a bit of overkill for something like the Mega Drive/CD system and thus have not done any integration testing with it. You are free to attempt this yourself, of course, and we welcome a report of your experience with it.

## Global Variable Initialization

In short, statically allocated variables (such as global variables declared outside of function scope) are not initialized even if you specify an initial value. E.g., if you declare and initialize a variable outside of a function like so:

	u16 initial_value = 100;

The value will not be set to 100 and will, instead, equal whatever already exists in memory at the location to which it was assigned by the linker. You will need to manually assign values to such variables before they are used.

The reason for this relates to the initialization process. Initial values need to stored somewhere, and in execution containers like ELF and PE this data is included in the final binary. Such formats will automatically copy the initialization data to memory where these static variables are linked, and thus the initial value is already present before execution begins.

Retro consoles like the Mega Drive/CD do not use such execution containers. There is no automatic initialization utility and code execution begins immediately.

Statically allocated variables are linked in the .data memory segment, so it is possible to copy this data as the first action in your program to manually do initialization. Indeed, this is on our to-do list for Megadev, but as it is not currently implemented be careful with global variable initialization.

## Integer Types

We highly recommend *not* using the `int` type for integers. Instead, use the sized types (`char`, `short`, `long`) or their typedefs (`s8`/`u8`, `s16`/`u16`, `s32`/`u32`). With gcc-m68k, int is four bytes in size, which may be a waste of space depending on the operation you are trying to perform. Memory usage should always be optimized.

## Fractional Types

The M68000 CPU does not have a floating point unit and use of the `float` type will only work using software emulation that is not included with Megadev.

Rather than floating point values, equivalent hardware from the era used fixed point math for fractional values. Megadev implements simple fixed point types and conversion macros in the fixed.h header. You can find examples of its usage in the `gfx` example project.

(If you wish to include floating point emulation, you will need to implement [the expected GCC routines](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html#Soft-float-library-routines).)

## Heap Allocation

As mentioned above, there is malloc/free system for heap allocation. You could write something yourself to track memory usage and allocate as necessary, but it is likely more efficient to develop a custom provisioning system that meets the needs of your specific game rather than a generalized allocator.

Consider why you would need to allocate space. This is generally to instantiate an "object," something which has state and a lifetime outside of the stack frame.

In the case of a retro game, such objects are generally sprites on the screen which share some attributes: an x and y position, a shape, flip and mirror flags, a tile index, a palette offset. So right away we have a base shared structure for all objects. If we decide to set aside, say, 32 bytes in memory for an object, we can fit all those display parameters as well as a pointer or jumptable offset to the object's code and have a bit of space leftover to store soem data specific to that object (like it's remaining HP or current attack pattern ID).

With a fixed size for an object's memory usage, we can specify a block of memory to hold a certain maximum number of objects as part of our memory layout planning. We can then write a simple object manager which will track the number of objects and provide a pointer to usable space within that block when requested to instantiate a new object.

You can see a rather simple example of this sort of setup in the `ipx_spx` example project.

## Stack Usage

Compared to hand-written assembly which often passes values by register, C tends to push values to the stack when calling functions. This means the stack can fill up very quickly, especially if you are using the default Boot ROM library memory layout, which sets aside only 256 bytes for the stack (see bootlib.md).

There are some strategies we can employ to help mitigate the risk of an overflow:

- Allocate a large enough stack to begin with. Bootlib allocates 256 bytes, which (anecdotally speaking) is generally enough space for simple programs, but 512 bytes or even a full kilobyte may be necessary for a complex project.

- If you plan to use Bootlib, consider moving the stack to somewhere else in Work RAM. As mentioned above, 256 bytes may not be enough for more complex systems. Many Mega Drive games set the stack to offset 0, which works downward from the very top of RAM. (Of course, you'll need to account for this in your memory map and ensure your memory usage doesn't creep up into the space you've set aside for the stack.) Doing so also frees up the 256 bytes that were previously set aside for the stack for your own use. As a side note, if you are using the default Bootlib memory layout and you start to see mysterious changes to your color palette as it runs (especially palettes 3 and 4), you may have a stack overflow, as the palette cache is directly above the stack space.

- Keep the number of values passed in functions to a minimum. If you need to pass a lot of information, considering packing that information into a struct and pass a single pointer to it.

- Try to keep the number of chained function calls to a minimum. A function calling a function calling a function calling a function will generate a stack frame for each call, which includes space for any local variables. If possible, call a function to do work on an object passed by pointer, then return and call the next pointer, and so on.

- Similarly, avoid recursive functions for the same reason as above.

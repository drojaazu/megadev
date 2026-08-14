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

Megadev includes a utility to copy the .data section into memory, in the `init.macro.s` file. Instead of using this directly, however, you may want to use either `main/ipx_init.s` or `std_init.s` for self-loading and standard modules respectively. Simply include this within the prerequisite list for your module and it will both zero out RAM and copy initialized variable data.

Please see the makefile for the example projects.

## Integer Types

We recommend *not* using the `int` type for integers. With gcc-m68k, int is four bytes in size, which may be a waste of space depending on the operation you are trying to perform. Memory usage should always be optimized.

Instead, use the sized types (`char`, `short`, `long`) or better yet their typedefs for clarity (`s8`/`u8`, `s16`/`u16`, `s32`/`u32`). 

## Fractional Types

The M68000 CPU does not have a floating point unit and use of the `float` type will only work using software emulation that is not included with Megadev.

Rather than floating point values, hardware from the era used fixed point math for fractional values. Megadev provides fixed point types, literal macros and arithmetic in the `fixed.h` header: `fix16` (Q10.6) and `fix32` (Q16.16), with unsigned `ufix16`/`ufix32` counterparts.

Prefer `fix16` where the range allows. The 68000 has a 16x16->32 multiply but no 32-bit multiply, so `fix16_mul` is a single instruction while `fix32_mul` must be built from partial products.

Addition and subtraction of two values in the same format are plain integer operations. Multiplication and division are not, since the scale factor would be squared or cancelled -- use `fix16_mul`, `fix16_div` and friends. You can find examples of fixed point usage in the `gfx` example project.

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

- If you plan to use the Main BIOS, consider moving the stack to somewhere else in Work RAM. As mentioned above, 256 bytes may not be enough for more complex systems. Many Mega Drive games set the stack to offset 0, which works downward from the very top of RAM. (Of course, you'll need to account for this in your memory map and ensure your memory usage doesn't creep up into the space you've set aside for the stack.) Doing so also frees up the 256 bytes that were previously set aside for the stack for your own use. As a side note, if you are using the default Main BIOS memory layout and you start to see mysterious changes to your color palette as it runs (especially palettes 3 and 4), you may have a stack overflow, as the palette cache is directly above the stack space.

- Keep the number of values passed in functions to a minimum. If you need to pass a lot of information, considering packing it into a struct and pass a single pointer instead.

- Try to keep the number of chained function calls to a minimum. A function calling a function calling a function calling a function will generate a stack frame for each call, which includes space for any local variables. If possible, call a function to do work on an object passed by pointer, then return and call the next pointer, and so on. Similarly, avoid recursive functions.

## Function prologues and the register save

When a function is called, certain CPU registers may be modified as a side effect of that code. If those registers were in active use by the calling code, then upon returning things will be broken as the values were unexpectedly changed. To prevent this, values for in-use registers are saved to the stack on entry and reloaded when the function returns. This automatic saving and loading is called the prologue and epilogue of a function.

On the m68k, GCC decides what to save with a single rule: a register is saved if the function touches it and it is not call-clobbered. Nothing else enters into it — in particular, marking a function `noreturn` has no effect on the prologue. It only removes the epilogue.

This matters because the m68k has just four call-clobbered registers (d0, d1, a0, a1). A function of any complexity exhausts those immediately and starts borrowing callee-saved registers, paying four bytes of prologue for each one it borrows. A module entry point commonly saves ten or eleven registers, or 40 to 44 bytes, against a default Boot ROM stack of only 256.

There is no `naked` attribute on m68k. It would not help if there were: GCC's own documentation states that only basic `asm` statements can safely appear in a naked function, and that mixing C code with them "cannot be depended upon to work reliably". `naked` exists to give an assembly-bodied routine a C declaration, which is a job already done here by ordinary `.s` files such as `main/ipx_init.s`.

If you need to reduce the saved set, the flags that work are `-fcall-used-<reg>` (make a callee-saved register call-clobbered, so it need not be saved) and `-ffixed-<reg>` (reserve it entirely). Both change the ABI for every function in the translation unit, so a function compiled this way must never be called from code compiled normally — no BIOS callbacks, no interrupt handlers, nothing reached from assembly. Isolating a module's entry point in its own source file is the usual way to satisfy that.

## The A6 register

Megadev compiles with `-ffixed-a6`, which reserves a6 so that GCC never places a value in it.

This is necessary because a6 is the m68k frame pointer register and the Boot ROM treats it as scratch, clobbering it in most of its routines. Those two facts do not coexist quietly. Naming a6 in an `asm` clobber list does not protect it: doing so marks the register as used, which forces GCC to install a6 as the frame pointer, and a frame pointer is fixed and cannot be clobbered — so the clobber is silently discarded and the function goes on addressing its locals through a register the BIOS is about to destroy. With `-fno-omit-frame-pointer` GCC at least rejects the clobber outright ("%a6 cannot be used in `asm` here"); without it, the result is a silent miscompile.

Reserving a6 removes the conflict at its source and costs nothing measurable — reserving it changed total code size across all example projects by well under a hundred bytes, in both directions. Because C holds nothing in a6, BIOS and library routines are free to clobber it, and no save/restore is needed around them.

**Do not put `"a6"` in an `asm` clobber list.** It is not merely redundant, it is harmful, for the reason above. If a routine takes a parameter in a6, bind it explicitly with `register u32 x asm("a6")`, which continues to work normally under `-ffixed-a6`.

# Development Using C

Writing your code in C rather than raw assembly can make development much easier.  However, there are some things you should keep in mind, especially if you are coming from a history development on PC.

## Standard Library

There is no C standard library in the PC development sense: no streams, no heap, no floating point values, no locales. Megadev provides a small freestanding subset of stdlib functions with standard names and semantics. Calls are linked only when used.

| Header | Provides |
|---|---|
| `string.h` | `memcpy`, `memmove`, `memset`, `memcmp`, `strlen`, `strcmp`, `strncmp`, `strcpy`, `strncpy`, `strcat`, `strchr` |
| `stdlib.h` | `abs`, `labs`, `div`, `ldiv`, `atoi`, `strtol`, `rand`, `srand`, `bsearch`, `qsort` |
| `stdio.h` | `snprintf`, `vsnprintf`: integer conversions, `%c`, `%s`, field width, `-` and `0` flags, `l` modifier |
| `ctype.h` | `isdigit`, `isalpha`, `isalnum`, `isspace`, `isupper`, `islower`, `isxdigit`, `toupper`, `tolower`, ASCII only |
| `assert.h` | `assert`, reporting over the serial port; see below |
| `types.h` | `size_t`, `ptrdiff_t`, the `intN_t` family and Megadev's `u8`, `s16` and so on |

Deliberately absent: `malloc` and `free`, `printf` to a console, `FILE`, floating point, `setjmp`, wide characters, time and locale. A name from the C standard that Megadev does provide has the standard signature; Megadev's own routines have their own names. The 68000's 32-by-16 hardware divides, for example, are `divs16` and `divu16` in `math.h`, while `div` in `stdlib.h` is the standard function. The Boot ROM's print routines expect a string ending in `0xFF`, not `0x00`; `str_to_bios` in `str_util.h` converts a C string, such as `snprintf` output, to that form.

`qsort` is an insertion sort. It is small and needs no extra memory, but its cost grows with the square of the element count, so it is not for large arrays or for sorting every frame; a sort written for the element type will beat the callback per comparison.

### Debug text

`main/text.h` puts text on screen without the Boot ROM, so it works on the Mega Drive alone, in Mode 1 and in Mega CD programs:

- `vdp_load_1bpp_tiles` expands a 1bpp font into VRAM in two palette colors. `font8x8` (`font8x8.h`) is a public domain 8x8 font of the printable ASCII characters; in a Mega CD program, the Boot ROM's font works as well.
- `text_print` writes a NUL-terminated string, such as `snprintf` output, at a position from `VDP_PLANE_XY` in `main/vdp.h`.

```c
vdp_load_1bpp_tiles(font8x8, FONT8X8_CHARS, 0x20, 1, 0);
snprintf(buf, sizeof buf, "Lives %u", lives);
text_print(buf, VDP_PLANE_XY(0xC000, 1, 1, Width64), 0x20);
```

The Boot ROM's own path remains: `bios_load_font_defaults` and `bios_print` with 0xFF-terminated strings. `str_to_bios` connects the two.

### Functions the compiler calls on its own

A few standard functions are always available, because GCC emits calls to them whether or not you write them, and regardless of `-nostdlib` and `-fno-builtin`. Copying or zero-initializing a large struct or array becomes a call to `memcpy` or `memset`, and 32-bit multiplication, division and modulo become calls to helper routines, since the 68000 only multiplies 16-bit and divides 32-by-16 in hardware. Megadev provides `memcpy`, `memmove`, `memset` and `memcmp` in `lib/runtime`, along with the 32-bit helpers (`__mulsi3`, `__udivsi3`, `__umodsi3`, `__divsi3`, `__modsi3`), and links them when they are used.

Megadev does not link libgcc, the compiler's own helper library, since prebuilt binaries may be inconsistent across repositories, and ultimately we cannot guarantee the library was built for the correct architecture. Megadev implements only the small subset of helper routines, so 64-bit integers and `float` and `double` values are not supported. 64-bit values should not be necessary on this hardware, and fixed-point numbers provide for fractional values.

The compiler may still accept some of these without a helper, such as 64-bit addition or copying a `float`, but do not rely on these. Operations that need a helper, such as 64-bit multiplication or `float` addition, fail at link time with an error such as `undefined reference to '__muldi3'`.

Functions marked `__attribute__((constructor))` or `__attribute__((destructor))` are not supported either: nothing calls them, so they fail at link time with `static constructors and destructors are not supported`.

### Assertions

`assert(expr)` from `assert.h` does nothing when the expression is true. When it is false, it formats the file, line and expression and sends them over the serial port with `ext_tx`, then stops the CPU. The port must have been configured with `init_ext_port` first; see `main/comm.h` and the `serial_debug` example for the wiring. It is Main CPU only, since the serial port is a Main CPU peripheral. Define `NDEBUG` to compile assertions out.

## Global Variable Initialization

Global and static variables are set up by startup code before `main` runs: those with an initial value receive it, and the rest are zeroed. The values are stored with the program and copied into RAM, since nothing else on the system does it.

| Program | Startup code |
|---|---|
| Module (MMD or SMD) | `std_init.s`, or `main/ipx_init.s` for a module that copies itself into place (IPX). List it in the module's prerequisites. |
| Cartridge | `md_init.s`, included by the project's `init.s`. It also clears all of Work RAM. |
| Boot sector IP and SP | None. Initialized variables are already in place when the Boot ROM loads them. Zeroed variables must be cleared by the program itself, as the examples do with `_BSS_ORIGIN` and `_BSS_LENGTH`. |

A module without startup code fails to link with `module has no init code`. A module that sets up its own RAM can opt out by defining `MODULE_NO_INIT`, for example with `GLOBAL_SYM(MODULE_NO_INIT, 1);` from `macro.h`; it must then clear and copy its variables itself, using the `BASIC_INIT` macro from `init.macro.s` or its own code.

See the example projects' makefiles.

### Layout symbols

Every linker script (`cfg/*.ld`) defines the same symbols describing the program's layout, for startup code and for programs that manage memory themselves. Lengths are in bytes; the `_LOOPSZ` forms are the length in longs, for a `dbf` loop.

| Symbol | Meaning |
|---|---|
| `_ROM_ORIGIN`, `_ROM_LENGTH` | Everything stored ahead of the initial values of `.data`: entry code or header, code and read-only data |
| `_TEXT_ORIGIN`, `_TEXT_LENGTH` | Code |
| `_RODATA_ORIGIN`, `_RODATA_LENGTH` | Read-only data |
| `_ROM_DATA_ORIGIN` | Where the initial values of `.data` are when startup code copies them. In the boot sector IP and SP, the same as `_RAM_DATA_ORIGIN`. |
| `_RAM_DATA_ORIGIN`, `_RAM_DATA_LENGTH`, `_RAM_DATA_LENGTH_LOOPSZ` | Initialized variables |
| `_BSS_ORIGIN`, `_BSS_LENGTH`, `_BSS_LENGTH_LOOPSZ` | Zeroed variables |
| `_RAM_LENGTH` | `_RAM_DATA_LENGTH` plus `_BSS_LENGTH` |

Modules also define `_INIT_ORIGIN` and `_INIT_LENGTH`, the startup code at the start of `_ROM_ORIGIN`.

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

## Register a6

GCC uses a6 as the frame pointer whenever it sets up a stack frame, which at `-O1` and above includes any function that takes the address of a local variable. The Boot ROM library uses a6 as a scratch register, so every Megadev wrapper for a Boot ROM or PCM call that changes a6 saves and restores it around the call.

The same applies to your own inline assembly:

- Save and restore a6 inside the assembly if it calls a routine that changes a6, or uses a6 itself.
- Do not rely on naming a6 in the clobber list. On m68k, GCC accepts the clobber and ignores it when a6 is the frame pointer, without a warning.
- Do not bind a C variable to a6 with `register ... asm("a6")`.

## Stack Usage

Compared to hand-written assembly which often passes values by register, C tends to push values to the stack when calling functions. This means the stack can fill up very quickly, especially if you are using the default Boot ROM library memory layout which sets aside only 256 bytes for the stack (see `main_bios.md`).

There are some strategies we can employ to help mitigate the risk of an overflow:

- Allocate a large enough stack to begin with. The Main BIOS tools allocate 256 bytes, which is generally enough space for simple programs, but 512 bytes or even a full kilobyte may be necessary for something more complex.

- If you plan to use the Main BIOS, consider moving the stack to somewhere else in Work RAM. As mentioned above, 256 bytes may not be enough for more complex systems. Many Mega Drive games set the stack to offset 0, which works downward from the very top of RAM. (Of course, you'll need to account for this in your memory map and ensure your memory usage doesn't creep up into the space you've set aside for the stack.) Doing so also frees up the 256 bytes that were previously set aside for the stack for your own use. As a side note, if you are using the default Main BIOS memory layout and you start to see mysterious changes to your color palette as it runs (especially palettes 3 and 4), you may have a stack overflow, as the palette cache is directly above the stack space.

- Keep the number of values passed in functions to a minimum. If you need to pass a lot of information, considering packing it into a struct and pass a single pointer instead.

- Try to keep the number of chained function calls to a minimum. A function calling a function calling a function calling a function will generate a stack frame for each call, which includes space for any local variables. If possible, call a function to do work on an object passed by pointer, then return and call the next pointer, and so on. Similarly, avoid recursive functions.

### Moving the Stack

A module that never returns can have its init code move the stack before `main` runs. In the prerequisite list for the module, use `std_init_stack.s` in place of `std_init.s`, or `main/ipx_init_stack.s` in place of `main/ipx_init.s`, then define the top of the stack with the rest of the module's layout. `STACK_SIZE` is optional:

```c
GLOBAL_SYM(STACK_TOP, 0xFFC000);
GLOBAL_SYM(STACK_SIZE, 0x1000);
```

The link fails if `STACK_TOP` is missing or odd, if the stack overlaps the module's code or RAM, or if an MMD's stack overlaps the exception jump table at 0xFFFD00 to 0xFFFDB4. A `STACK_TOP` of 0 starts at the top of Work RAM, since the first push wraps around to 0xFFFFFC, and has 588 bytes above the jump table; that space is only free if you are not using the Main BIOS (see `megacd_dev.md`). Without `STACK_SIZE`, only `STACK_TOP` itself is checked. These checks only cover the module's size as linked. They cannot tell you how deep the stack gets at runtime, and they do not know about other modules or Boot ROM memory, so the stack still needs a place in your memory map.

A cartridge's stack starts at `INIT_SSP`, defined in its `init.s`. The link fails if the cartridge's variables reach the stack, or reach `STACK_SIZE` bytes below `INIT_SSP` if `STACK_SIZE` is defined. A Mode 1 cartridge's variables and stack must also stay clear of the system vector table at 0xFFFD00 to 0xFFFDB4. The ROM must fit in the 4 MB of cartridge space.

Do not use these with a module that returns to its caller, such as a module called by an IPX. The return address is on the old stack.

## Lack of "naked" function support

One of the most frustrating aspects of working with GCC is the lack of flexibility in some (admittedly esoteric) aspects. Chief among these has been GCC's insistence on creating function prologue/epilogues for overlays (modules).

In summary, when a function is called, certain CPU registers may be modified as a side of that code. If those registers were in active use by the calling code, then upon returning things will be broken as the values were unexpectedly changed. To prevent this, values for in-use registers are saved to the stack for safekeeping and reloaded when the function has returned. This automatic saving and loading of registers (and possibly other function call utilities) is called the prologue and epilogue of a function.

The automatically generated epilogue/prologue may be unnecessary if the function in question is an entry for a module. The register push takes up valuable stack space in our limited RAM. Moreover, the compiler may also LINK the A6 register, which essentially removes that register from use. This is especially problematic since a number of Main BIOS calls make use of A6.

A function lacking an epilogue and prologue is called "naked," and unfortunately GCC does not provide a way to specify a function as naked (at least not for the M68000 architecture). At this point, there doesn't seem to be a good solution for this. Marking a function that never returns as `noreturn` removes the epilogue but not the pushes. The stack can be manually reset by some inline ASM at the top of a function to recover the space used, but the (admitterly few) CPU cycles are still used for the push, and it is only safe if the function has nothing on the stack: GCC addresses locals and spilled registers relative to SP, and counts the pushed registers in those offsets, so resetting SP moves them out from under the code. To give a module more stack room safely, see [Moving the Stack](#moving-the-stack), which happens before `main` is entered. For BIOS calls using A6, we have wrapped these calls with a push/pop of A6, which keeps things stable but adds more CPU cycles...

If you need to be particular about speed, we recommend checking any BIOS calls being made that may use A6 and perhaps write your own inline ASM caller to deal with tha LINKed A6 issue more efficiently. Of course, this issue only applies to C programming; if you are developing in pure ASM, this is not a problem.

If anyone has any suggestions for how to solve for this, we greatly welcome your comments.

# Boot ROM Library
The Boot ROM is the Main CPU half of the Mega CD internal program. This includes the built in CD player and backup RAM manager. Importantly for developers, it also includes a library of useful functions. These include tools for identifying and reading from controllers, random number generation, DMA functions and more. The tradeoff for using these functions is that they use a portion of the already limited Work RAM, reducing the amount available for your game.

## Official Documentation
As stated in the main readme file:

> The Main CPU side Boot ROM code, however, is not well documented. In fact, it is somewhat of a mystery. Within the Boot ROM is an array of functions that are linked at a jump table, indicating they are meant to be used across ROM revisions and thus intended for use by games. While it seems most games do not make use of these system calls, at least some do (such as Keio Yūgekitai, Switch, Alshark, Jangō World Cup, and Ishii Hisaichi no Daiseikai). This is solid evidence that the calls can be freely used by developers if they wish.

> We do not have any official documentation on the Boot ROM library. It is possible there never was such documentation to begin with, as we have three different source of Mega CD paperwork, most of which is wholly redundant, yet there is no sign of Boot ROM documentation. One of the later Technical Bulletins makes mention of these calls in passing and mentions they may be used by games, so it wasn't secret information. Its possible there was documentation that was never translated to English and was only available to Japanese developers.

The Tech Bulletin in question here is MEGA CD TECHNICAL BULLETIN #3. It is our only direct reference to the otherwise undocumented code. First it provides a memory map of the Main CPU side when the Boot ROM functions are in use:

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

 The area from 0xFFF700 to 0xFFFC00 is marked as reserved for Boot ROM use. If you choose to use the Boot ROM functions, you should keep this area reserved.

Further on in the tech bulletin, it describes the boot process in a little more detail, then describes the state ofthe Main CPU side after bootup (important parts highlighted):

> The loading occurs from the beginning of WORK RAM. The stack is $FFFDOO. The RAM's usage status is as follows. Please refer to the RAM and PORT assignments in 'MEM_MAP.ASS'. Subsequent to $fffd00 are jump table, etc., and therefore should not be used. **Those preceding $fffd00 could be freely used provided that boot ROM sample routine is not used.** Also, when setting jump address in the jump table, jump address must be set to a value equal to the label value plus 2. The label value should not be rewritten. **Subroutine in the boot ROM may also be used (please see mainent.i,rom_util.doc). Use "works" defined by the using routine.** Enter SEGA-specified security program at the head of IP. Please note that Europe and the U.S. have different contents and sizes. **Outside of the system area work RAM values could differ depending on the BOOT-ROM version; also keep in mind that the work RAM values are not guaranteed.**

The translation is a bit stilted, but we can get the sense that using the library code ("sample routines") was an option and that there was further information in at least two official Sega files (mainent.i, rom_util.doc). Unfortunately, we do not have a copy of the "official SDK" for the Mega CD. We do have some official code from later 32X dev kits which have examples for working with the Mega CD, but the files mentioned above in particular are not included.

After this, the tech bulletin provides a listing of several memory locations used by the library, things such as the Nemesis decompression buffer and the sprite cache, within the Boot ROM Use block. It also lists the vector jump table within the System Use area, beginning at 0xFFFD00. Interestingly, the VDP register cache is included in the System Use area.

The document warns us that RAM locations in the Boot ROM block are not guaranteed across revisions. Though I haven't thoroughly checked every single dump of the internal ROM, it looks like they are, indeed, the same across versions. Until we check everything single one, though, we can't say that with 100% certainty. This document also implies that RAM locations within the System Use area are fixed across versions, which hints that the VDP register cache within will never move. There are some other memory locations not listed in this document but which were determined through reverse engineering that are also in the System Use area. The implication, we can gather, is that these are also safe across versions.

That is the sum of our official documentation on the Boot ROM library code, aka the "sample routines." While it seems that most games chose not to make use of it, we know at least a few retail games did. That tells us it is safe to use in actual games and on real hardware.

# Memory Usage

# Library Reference
The routines and their associated variables within the Boot ROM library can be roughly divided into these groups: System, Interrupts, Input, VDP, Decompression, (Gate Array) Comm, Misc, and Unknown.

- System: Subroutines for system boot; these are generally unnecessary during runtime
- Interrupts: Subroutines related to interrupts and interrupt handlers
- Input: Subroutines related to controller input
- VDP: Subroutines related to VDP usage and graphics display
- Decompression: Subroutines to decompress data
- Comm: Subroutines related to the Gate Array comm registers
- Misc: Everything else
- Unknown: Subroutines that are not yet fully understood

You are free to pick and choose individual routines to use, but keep in mind that some are interdependent or rely on components that may not work with your project architecture. We have done our best to document the falls and their related concepts to help you decide what does and does not work for your project

Please see the `main/boot_def.h` file for the full list of functions.

## Using the Code
When coding in assembly, include the `boot_def.h` file. This contains definitions for the function entry points and variable addresses.

When coding in C, include `boot.h`. Be sure to include `boot.c` in the module definition as well.

## Components
Many of the functions work together and share data structures and memory locations. We refer to these shared pieces as components. For example, the VDP Register Cache is a component that is used in the DMA routines and in many of the graphics routines. Not all components are data, though. The Default VRAM Layout is a component that defines the memory map within VRAM that is expected by a number of functions. The components used by a given function will determine memory usage requirements (see the next section) and impose certain expectations on your program.

We will discuss the components and variables in use in each section.

## Memory Usage
Perhaps the biggest deciding factor for using the Boot ROM or not is its use of Work RAM. As we have discussed in design.md, Work RAM is the smallest of the runtime buffers available yet is the most important. Of the 63.25KB of space available in Work RAM, another 1.25KB could be lost if you choose to use the Boot ROM library.

That isn't a huge amount of space, admittedly, but consider that that 63.25KB of space will hold the kernel, resources, runtime RAM and the stack. It can fill up quickly, and 1.25KB of space may be valuable.

On the other hand, the space taken by the Boot ROM library would probably be similarly allocated by your own code, namely, a decompression buffer (for compressed graphics), a sprite list cache and a palette cache.

Even if you want to maximize available space in Work RAM, the Boot ROM has a number of functions that do not require that extra space and can be used freely. The library actually makes use of the free space within the System Use block, an area normally off limits anyway, so in some ways it can help you save a little space by storing values in the System Area that you might otherwise keep within the user area.

### Boot ROM Use Block
Let's take a closer look at that block of memory, which we will refer to as the Boot ROM Use block. It exists in Work RAM from 0xFFF700 and is 1,280 bytes in size:

    FFF700  +----------------+
            | Decompression  |
            |         Buffer |
    FFF900  +----------------+
            | Sprite List    |
            |          Cache |
    FFFB80  +----------------+
            | Palette Cache  |
    FFFC00  +----------------+

As you can see, it is neatly divided into three sub blocks. If, for example, you do not use any of the decompression routines, you are free to use the Decompression Buffer space for your own code. We will indicate which functions/components use these sub blocks in the reference below.

One very important thing to keep in mind is the stack. By default, the stack is set to 0xFFFD00. With the Boot ROM Use block ending at 0xFFFC00, this means there is only 256 bytes for the stack. This is generally enough for most cases, but some C code may not optimize well and create many nested function calls or large stack frames. Be mindful of this when developing in C. When coding asm, be careful of how many registers you push.

Alternatively, the stack can be moved to a lower location and allocated more space if you feel that is necessary. This would free up the 0xFFFC0 to 0xFFFD0 range for your own use as well.

### System Use Block
We've already examined the System Use Block closely in the design.md file. To summarize, it is a 768 byte block at 0xFFFD00, extending to the end of addressable memory. It is generally regarded as off-limits within official documentation, though only half of it is actually used in normal operation, meaning the rest of the space can be used freely.

However, when using the Boot ROM library, that extra free space becomes unavailable to use freely. The Boot ROM library uses it to store a number of variables, including some useful values like controller inputs. This means that you will be using the otherwise free space within the System Use area, but mostly in a read-only context in relation to library functions.

## Tiny Functions
Some of the functions available are quite small, consisting of little more than a move to a memory location. It may be beneficial to examine the disassembly and copy the code directly into your program either in place or as a macro, especially if you are making the call in a tight loop. It can save a couple ticks and the stack space associated with the call.

Such calls will be marked as "Tiny" in the reference below.

## Unclear Functions
As discussed above, the Boot ROM library functions do not have any official documentation. While most have been fully reverse engineered, there are still a handful that are still not understood, either partially or fully.

Such calls will be marked as "Unclear" along with notes about them. They should be closely evaluated before deciding whether to use them or not.

## Zero-Cost Functions
Some Boot ROM library calls do not use any RAM at all and can be called no matter what your memory constraints are. Such calls will be marked as "Zero-Cost" in the reference below.

## System

_BOOT_RESET

_BOOT_ENTRY

_BOOT_INIT

_BOOT_INIT_SP

_BOOT_SEGA_LOGO

## Interrupts - Components
These calls are related to interrupts - VBLANK, HBLANK and IFL2.

### VINT Component
The vertical blanking interrupt is very important to developing on the Mega Drive (and any CRT based retro game system). This is the period in which the electron beam inside the CRT is returning to the upper left position after reaching the end of displaying all the rows of a single frame and it is during this time when much of the game logic and data IO is processed.

The Boot ROM provides generic VINT handler and wait routines. The handler takes care of a number of common tasks: sync GA comm registers & cache, copy the palette cache to CRAM, and update controller inputs. The wait routine takes care of random number generation.

The VINT component is useful but requires a commitment to using the Boot ROM library in full due to how many other components it uses. You should familiarize yourself with the components it uses and design your program around them if you want to use the VINT routines. Or rather, if you decide to use most of the Boot ROM library functionality to begin with, the VINT handler help tie it all together.

The VINT handler also calls VINT_EX which points to a user-defined function that performs your game-specific logic during the VBLANK interval. You should point this to your own function in your init phase.

The VINT handler also uses a variable defined as `_VINT_FLAGS` which acts as both the "VBLANK occured" indicator and as a set of update flags. Only two bits are used by the handler: bit 0, which will copy the sprite list cache to VRAM when set, and bit 1, which will call VINT_EX when set. The value is always reset to zero at the end of the handler code, so the wait routines check that this value is zero to indicate a VBLANK has completed. Note that, although it uses bit 0 of `_VINT_FLAGS`, `BOOT_COPY_SPRLIST` is not actually called in the VINT routine. You will need to include a call to that routine in your VINT_EX function.

There are two VBLANK wait routines which are mostly identical. These calls enter a tight loop waiting for VINT_FLAGS to be reset to 0 (which will be set when VBLANK occurs), and will update the random number generator on each iteration. The only difference between the two is that `BOOT_VINT_WAIT_DEFAULT` will set both VINT Update flags (copy sprite list and call VINT_EX), while `BOOT_VINT_WAIT` will not set any flags beforehand.

Please see the `gfx` example which uses the VINT component.

## Interrupts - Functions

### `BOOT_VINT`
Components: VINT, GA Comm, Sprite Table Cache, Palette Cache, Input

A generic VINT handler. Calls `BOOT_COMM_SYNC`, `BOOT_COPY_PAL` and `BOOT_UPDATE_INPUTS` on each iteration. If bit 1 of `_VINT_FLAGS` is set, `_VINT_EX_PTR` will also be called. `_VINT_EX_PTR` points to a user-defined routine with game-specific logic to be run during VBLANK. We recommend including a call to `BOOT_COPY_SPRLIST` in your VINT_EX routine.

`BOOT_VINT` updates the `_VINT_COUNTER` variable by 1 on each iteration. It also uses the `_SKIP_GFX_UPDATES` variable. When this value is non-zero, it will skip the call to `BOOT_COPY_PAL` and `_VINT_EX_PTR` and skip the `_VINT_COUNTER` increment.

### `BOOT_SET_VINT`

(Tiny)

Sets the specified pointer as the VINT vector.

### `BOOT_VINT_WAIT`

Sets the specified value to `_VINT_FLAGS` and waits for the next VBLANK occurance, which resets `_VINT_FLAGS` to zero. This means `_VINT_FLAGS` must be set to a non-zero value in order to actually wait for a VBLANK occurance. If you want to wait for VBLANK without triggering a sprite list or palette copy, simply set an unused bit.

### `BOOT_VINT_WAIT_DEFAULT`

A wrapper for `BOOT_VINT_WAIT` that sets `_VINT_FLAGS` to 3 (bit 0 and 1 set) before waiting. This would be useful if you do not implement any extra `_VINT_FLAGS` and only use the two used by the library. In any other case, you should stick to `BOOT_VINT_WAIT` as it allows you to specify flags.

### `BOOT_SET_HINT_DEFAULT`

### `BOOT_SET_HINT`

### `BOOT_DISABLE_HINT`

### `BOOT_TRIGGER_IFL2`

## Input - Components
These calls are related to controllers and input from the player.

## Input - Functions

### `BOOT_UPDATE_INPUTS`

### `BOOT_INPUT_DELAY`

### `BOOT_DETECT_CONTROLLER`

## VDP/Graphics - Components
Unsurprisngly, the majority of the Boot ROM library functions are related to the VDP and VRAM. They cover most of the comomnly used tasks when working with graphics, which is extremely helpful. However, some have certain component requirements that may be incompatible or difficult to adapt to your program.

### Fixed VRAM Layout Component
The 64kb of VRAM is shared by tile graphics, name tables (tilemaps), the sprite table and the horizontal scroll table. How much space is allocated for each section can be varied, giving flexibility to the developer on how they choose to use the VDP.

However, a number of Boot ROM library routines expect the VRAM to have a specific layout. This layout is defined like so:

    0000  +----------------+
          | Tile Graphics  |
          |                |
          |                |
    A000  +----------------+
          | Window         |
    C000  +----------------+
          | Plane A        |
    E000  +----------------+
          | Plane B        |
    B800  +----------------+
          | Sprite Table   |
    BC00  +----------------+
          | Horiz. Scroll  |
          | Table          |
    FFFF  +----------------+

This is a pretty standard layout that would work well for many applications, but your usage may vary throughout your program. It is possible to change this layout so long as the the blocks specifically used by a certain call are in their expected location. For example, the sprite list update calls expect the sprite table to be at 0xB800, but it doesn't address anything else in VRAM, and thus the rest of the layout can be different so long as the sprite table is at 0xB800. For each call that expects this layout, we will mark which blocks are used/affected.

This layout is set when using the default VDP register values in `BOOT_LOAD_VDPREGS_DEFAULT`. 

### VDP Register Cache Component
Since the value of VDP registers cannot be read (except for the Status register), we must maintain a mirror of those values in RAM for reference in order to preserve settings when making bitwise changes. For example, when performing a DMA operation, we must first set bit #4 on Mode Register 2. Since we cannot do a bitwise operation and must set the whole register at once, we need to know the current value so we do not alter any other bit level settings.

The cache is an array of 18 words (36 bytes), one for each of the first 18 registers. (The DMA register values are not maintained, as they need to be set on each DMA operation anyway.) It is defined as `_VDP_REGS` within Megadev.

### Plane Width Cache Component
The width and height of planes can be adjusted in VDP Register 0x10 to one of three values. It is important to know the current width when working with tilemaps, as the nametable data is linear. That is, to arrive at the next line in the same column, we move forward by the width of the plane in bytes in nametable memory. Thus, the plane width must be cached.

All of the nametable (tilemap) related functions use the plane width cache, which is a byte value defined as `_PLANE_WIDTH`. Note that this value is the width *in bytes* rather than tiles. At two bytes per entry, this means the value should simply be double the width in tiles. I.E.: 32 tiles = 64 byte width, 64 tiles = 128 byte width, 128 tiles = 256 byte width.

Also note that this value must be manually updated whenever changing the width setting in VDP Register 0x10! The only exception to this is when using `BOOT_LOAD_VDPREGS_DEFAULT` which will update the plane width cache to match the default registers.

### DMA Component
DMA transfers to VRAM are a central piece of Mega Drive games, but there are quirks when doing them with Mega CD hardware. The Boot ROM library has functions for all the DMA operations as well as a simple queue system.

The VDP Register Cache component is required as Mode Register 2 is updated with each operation.

### Palette Cache Component
Rather than making multiple updates to CRAM via the VDP ports, the color palette is mirrored in RAM and dumped to CRAM all at once during a blanking interval.

The cache is an array of 64 words (128 bytes), a 1:1 copy of CRAM. Within Megadev, it is defined as `_PALETTE` with each of the subpalettes defined as `_PALETTE0`, `_PALETTE1`, `_PALETTE2` and `_PALETTE3`. It resides in Work RAM from 0xFFFB80 to 0xFFFC00 (see the Boot ROM Use Block section above).

The palette-related functions take a pointer to color data with a two byte header. The first byte is the offset into CRAM at which the colors should be loaded. This offset is in BYTES rather than entries. Each entry is 2 bytes so e.g. the second color of the second subpalette would be offset 34. The second byte is the number of colors to load, which is counted in ENTRIES rather than bytes. The color data follows immediately.

### Sprite Table Cache Component
Similar to the Palette Cache component, the Sprite Table Cache is a mirror of the sprite table in VRAM. Changes are made in the cache and dumped all at once to VRAM during a blanking interval.

The cache is an array of 80 sprite table entries (640 bytes), which is the maximum displayable on screen by the VDP.

See also Fixed VRAM Layout component.

### VDP Update Flags Component
Rather than unconditionally copying everything to the VDP on a blanking interval, we should only copy over data that had a change. This is done by maintaining a set of flags that indicate which parts need action and which do not need any updates. In the Boot ROM, this is a byte defined as `_VDP_UPDATE_FLAGS`.

Only one bit is actually used by the library functions: bit 0, to indicate the palette cache should be copied to CRAM. This bit must be used if you plan to use palette related functions like fade in/out.

The rest of the bits are free for you to use as needed.

### Entity Component
Entities - made up of multiple sprites

## VDP/Graphics - Functions

### `BOOT_DMA_FILL_CLEAR`
Components: DMA, VDP Register Cache

Clears VRAM at the specifed address for the given size (in words) using DMA.

### `BOOT_DMA_FILL`
Components: DMA, VDP Register Cache

Loads the specified value into VRAM at the specified address for the given size (in words) using DMA.

### `BOOT_DMA_XFER`
Components: DMA, VDP Register Cache

Performs a DMA transfer to VRAM.

### `BOOT_DMA_XFER_WORDRAM`
Components: DMA, VDP Register Cache

Performs a DMA transfer to VRAM for source data in Word RAM.

### `BOOT_DMA_COPY`
Components: DMA, VDP Register Cache

Performs a DMA copy.

### `BOOT_DMA_XFER_QUEUE`
Components: DMA, VDP Register Cache

A simple DMA queue. Processes a list of DMA transfers from Word RAM.

### `BOOT_CLEAR_VRAM`
Components: DMA, VDP Register Cache

Clear all of VRAM and VSRAM via DMA Fill. Also clears VSRAM with a call to `BOOT_CLEAR_VSRAM`.

### `BOOT_CLEAR_NMTBL`
Components: Fixed VRAM Layout (Sprite List, Plane A, Plane B, Window), Sprite Table Cache

Clears the nametables (tile mappings) and sprite table. Note that it only clears the first value in the sprite table, which sets the first sprite "next" value to 0 and prevents any sprites from displaying, but does not actually clear any memory in that region.

### `BOOT_CLEAR_VSRAM`
Clears VSRAM.

### `BOOT_LOAD_VDPREGS_DEFAULT`
Components: Fixed VRAM Layout (All), VDP Register Cache, Plane Width Cache

Loads the Boot ROM default VDP settings to the cache and to the registers. Also updates the `_PLANE_WIDTH` cached value.

TODO: enumerate the default values

### `BOOT_LOAD_VDPREGS`
Components: VDP Reg Cache

Loads a list of VDP settings to the cache and to registers. This does NOT update `_PLANE_WIDTH` when setting register 0x10.

### `BOOT_VDP_FILL` (Tiny)
Loads the specified value to VRAM at the specified address for the given size (in words).

### `BOOT_VDP_FILL_CLEAR` (Tiny)
Clears VRAM at the specified address for the given size (in words).

### `BOOT_LOAD_MAP`
Components: Plane Width Cache

Loads a tilemap to VRAM.

### `BOOT_LOAD_MAP_TEMPLATE`
Components: Plane Width Cache

Loads a tilemap to VRAM with a template value applied to each map entry.

### `BOOT_NMTBL_FILL`
Components: Plane Width Cache

Writes a given tilemap entry to VRAM with the given dimensions.

### `BOOT_VDP_DISP_ENABLE`
Components: VDP Register Cache

Enables VDP display (bit 6 on VDP register 1).

### `BOOT_VDP_DISP_DISABLE`
Components: VDP Register Cache

Disables VDP display (bit 6 on VDP register 1).

### `BOOT_LOAD_PAL`
Components: Palette Cache

Loads an array of colors to the palette cache. Does not update CRAM directly.

### `BOOT_LOAD_PAL_UPDATE`
Components: Palette Cache, VDP Update Flags

Loads an array of colors to the palette cache and sets the CRAM update flag. Does not update CRAM directly.

### `BOOT_COPY_PAL`
Components: VDP Reg Cache, DMA, Palette Cache, VDP Update Flags

Copies the palette cache to CRAM.

### `BOOT_PROCESS_ENTITIES`
Update the state of an array of entities. Please see the Entities section.

### `BOOT_DISP_ENTITY`
Display an entity. Please see the entities section.

### `BOOT_COPY_SPRLIST`
Components: Sprite Cache, VDP Register Cache, VINT Flags

Copies the sprite cache to the sprite table in VRAM.

### `BOOT_LOAD_1BPP_TILES`
Converts 1bpp formatted graphics data to 4bpp VDP format and copies to VRAM.

It is a form of compression as, logically, 1bpp graphics only use 1/4 of the space of 4bpp graphics. It would be most often used for simple fonts which only need to use one color. In fact, the Boot ROM's internal font is 1bpp and uses this routine to convert it to standard Mega Drive format when loading. The size of a font is likely not a concern for games using only the standard western alphabet, but when you consider east Asian languages like Japanese, Chinese and Korean and their multitudes of glyphs, having a way to quickly load and convert a large set of characters is helpful.

Of course, this is not strictly limited to fonts and you can use the routine however you'd like to convert 1bpp graphics to Mega Drive 4bpp tiles.

The function takes a pointer to the 1bpp data, a VDPPTR formatted VRAM destination, the number of tiles and the color definition. The color definition requires some explanation. It is a 32 bit value that acts as a mapping for each possible pair of 1 bit values. The full value is divided into four bytes visualized like so:

    00'0X'X0'XX

Where X is the palette index of the color to be used. So, for example, if you want the 1bpp graphics to use the color in palette index 1, the value to use would be:

    0x00011011

That should be enough information to implement in your code, but if you you want to understand why this value is necessary and formatted like it is, here is the algorithm in summary. Since the native VDP tile format is 4bpp, that means there are four bytes per tile row (8 pixels * 4 bits = 32 bits = 4 bytes). For 1bpp data, there is one byte per tile row (8 pixels * 1 bit = 8 bits = 1 byte). For every byte of 1bpp data, it rotates the value by 2 bits and masks those off. That 2 bit value is then used as a byte offset relative to the MSB of the 32 bit color definition, which contains an equivalent 4bpp 2 pixel representation of that value. For example, if 2 bits of 1bpp data are binary 10 (decimal 2), then it uses the value that is two bytes from the MSB of the color definition. That value is X0, which is appended to the output 4bpp data. Basically, for each possible pair of two bits, there is a corresponding pair of nibbles (4 bits) that matches it within the color definition.


### `BOOT_LOAD_MAP_VERT`
Components: Plane Width Cache

Loads a tilemap to VRAM based on sequential tiles which are vertically oriented.

### `BOOT_LOAD_MAP_HORIZ`
Properties: Plane Width Cache

Loads a tilemap to VRAM based on sequential tiles which are horizontally oriented.



### `BOOT_BLANK_DISPLAY`
Properties: VDP Reg Cache

Sets the background color to black and disables the display.

### `BOOT_PAL_FADEOUT`
Properties: VDP Update Flags, Palette Cache

Fades a palette range to black.

### `BOOT_PAL_FADEIN`
Properties: VDP Update Flags, Palette Cache, Other Memory

Fades a palette to a target palette.

### `BOOT_SET_FADEIN_PAL`
Properties: Other Memory

Sets the target palette for fade in.



## Decompression

### `BOOT_GFX_DECOMP`

### `BOOT_GFX_DECOMP_RAM`

### `BOOT_MAP_DECOMP`

## GA CPU Communication - Components
Functions related to inter-CPU communication via the Gate Array comm registers.

### Fixed Comm Flag Semantics
The GA_COMFLAGS register is intended to keep the two CPUs in sync by informing each other about their status. It is a 16 bit register, split into two bytes: the upper byte for the Main CPU and the lower byte for the Sub CPU. The flags do not have an any inherent semantic meaning and the developer is free to use any or none of the flags in their program.

There are a few Boot ROM library functions that use these flags, however, which inherently assigns meaning to those bits. The problem here is that we don't know what those meanings are due to a lack of documentation. We can only guess based on the context in which they appear as we reverse engineer the code. This includes understanding not just the library calls that use these bits, but what must be done on the Sub CPU side to correctly read, set and clear bits in response. Thankfully, the built in software (namely the CD player) uses these calls, meaning we have usage examples on the Sub CPU side to look at. Moreover, of the retail games identified so far that use the Boot ROM library, most of them make use of these calls and exhibit a similarity in their implementation (to the point of being nearly identical) that suggests they were built from example code.

At this point, we are still investigating and have only a rough idea of what the flags represent. Therefore, we do not recommend using the functions marked as using the Fixed Comm Flag Semantics component. However, **if you plan to use the built-in vblank handler (BOOT_VINT), keep in mind that it includes a call to one of these subroutines. Please see the notes for BOOT_COMM_SYNC.**

## GA CPU Communication - Functions

### `BOOT_CLEAR_COMM`
Clears all Gate Array COMCMD and COMFLAGS registers and the COMCMD and COMFLAGS cache.

### `BOOT_COMM_SYNC`
Components: Fixed Comm Flag Semantics

Triggers INT2 on the Sub CPU via the IFL2 bit (for this reason, it should be called during VBLANK). Also syncs the COMCMD cache values to the GA registers and the COMSTAT values from registers to the cache.

IFL2 is set on each call regardless, but the COMCMD/COMSTAT sync is dependent on bits 0 and 1 of Sub side COMFLAGS. If Sub side bit 0 is set, Main side's bit 0 will be set and bit 1 will be flipped. It will then loop waiting for Sub side bit 1 to either set or clear, depending on the state of Main's bit 0. When the state matches the expectation, it moves on to copying COMCMD cache to registers followed by COMSTAT registers to cache.

As mentioned in the Fixed Comm Flag Semantics component description, we recommend not using this function until we have a better understanding of the flags system on both CPUs.

Note that if you are using `BOOT_VINT` for your vblank handler, `BOOT_COMM_SYNC` is called as part of that subroutine, which is important as it takes care of the IFL2 bit. The best way to keep things flowing smoothly is to ensure bit 0 is always unset on the Sub side COMFLAGS. This will ensure that the COMCMD/COMSTAT sync is skipped and that a stuck loop is avoided.

### `BOOT_UK_COMM_CDINFO`
Components: Fixed Comm Flag Semantics

Copies various CD read status values sent from the Sub CPU via COMSTAT registers, including disc read absolute and relative timecodes. One use for getting this information in real time is to time the playback of CD audio with actions on the screen. Code that implements this call suggests it is mean to be used in conjunction with `BOOT_COMM_SYNC`, with the latter called at the start of VBLANK and `BOOT_UK_COMM_CDINFO` called at the end.

As mentioned in the Fixed Comm Flag Semantics component description, we recommend not using this function until we have a better understanding of the flags system on both CPUs.

### `BOOT_UK_COMMFLAGS_RELATED`
Components: Fixed Comm Flag Semantics

Waits for bit 6 on Sub side COMFLAGS to set, then clears Main side COMFLAGS bit 2.

As mentioned in the Fixed Comm Flag Semantics component description, we recommend not using this function until we have a better understanding of the flags system on both CPUs.

### `BOOT_SET_COMCMD0_1_CACHE`
Copies the word value in D0 and D1 into COMCMD0 and COMCMD1 cache, respectively.

Although this does not use the Fixed Comm Flag Semantics component directly, it is likely meant to be part of its system. Therefore we recommend reviewing its code closely to make sure it does exactly what you expect before using it.

### `BOOT_SET_COMCMD2_3_CACHE`
Copies the word value in D0 and D1 into COMCMD2 and COMCMD3 cache, respectively. In addition, if the value in D0 is 1, it also sets bit 2 on Main side COMFLAGS.

Although this does not use the Fixed Comm Flag Semantics component directly, it is likely meant to be part of its system. Therefore we recommend reviewing its code closely to make sure it does exactly what you expect before using it.

### `BOOT_SET_COMCMD4_5_CACHE`
Copies the word value in D0 and D1 into COMCMD4 and COMCMD5 cache, respectively.

Although this does not use the Fixed Comm Flag Semantics component directly, it is likely meant to be part of its system. Therefore we recommend reviewing its code closely to make sure it does exactly what you expect before using it.

### `BOOT_SET_COMCMD6_7_CACHE`
Copies the word value in D0 and D1 into COMCMD6 and COMCMD7 cache, respectively.

Although this does not use the Fixed Comm Flag Semantics component directly, it is likely meant to be part of its system. Therefore we recommend reviewing its code closely to make sure it does exactly what you expect before using it.

### `BOOT_SET_IFL2`
Sets the IFL2 bit on GA_MEMMODE to trigger INT2 on the Sub side. Should be called during VBLANK only.

## Misc - Components

### Random Number Generator Component
There is a simple multiply-with-carry psuedo random number generator available within the Boot ROM library. A random number can be obtained by reading the word value from `_RANDOM`. However, the `BOOT_PRNG` must be called on each VBLANK iteration in order to maintain a constantly random value. `BOOT_PRNG_MOD` can be used to get a value with a maximum range.

### Font & Print Component
There is only one print routine, which take a pointer to the text and a pointer to a VRAM nametable address where it should be displayed. Unlike C strings, text must be terminated by 0xFF, while 0x00 marks a newline.

There is a standard ASCII font in the Boot ROM data which can be used if you don't need/want your own. The functions `BOOT_LOAD_FONT` and `BOOT_LOAD_FONT_DEFAULTS` will load this font into VRAM for use with the print function. `BOOT_LOAD_FONT` takes a VDP pointer to the location in VRAM where the font should be loaded as well as a 1bpp color definition (see `BOOT_LOAD_1BPP_TILES`).

You must also set the `_FONT_TILE_BASE` variable to the tile index of the first tile of the font within VRAM *minus 32*. The reason for this is because ASCII codes begin at 32, and the print function simply matches an ASCII character to a tile index. Since `_FONT_TILE_BASE` cannot be negative, this implies the font must be loaded at tile index 32 or later, but cannot be placed before index 32. When placed at index 32, `_FONT_TILE_BASE` will be 0. This is the configuration the BIOS works with and you can observe this by viewing the VRAM tiles while the "Produced by or under license..." screen is being displayed.

The other font load function, `_FONT_LOAD_FONT_DEFAULTS` simplifies things by loading the font to index 32, setting `_FONT_TILE_BASE` to 0, and setting the 1bpp color definition to palette index #1. This is the simplest method to load the font and is useful in situations where you don't need to be meticulous about VRAM management.

## Misc - Functions

### `BOOT_PRNG`
Components: Random Number Generator

Generates a new 16bit random number, stored in `_RANDOM`.

### `BOOT_PRNG_MOD`
Components: Random Number Generator

Generates a random number with a specificed maximum value, stored in a return register.

### `BOOT_CLEAR_RAM`
Clears a region of RAM in 32bit iterations.

### `BOOT_LOAD_FONT`
Components: Font & Print Component

Load the internal Boot ROM font to VRAM.

### `BOOT_LOAD_FONT_DEFAULTS`
Components: Font & Print Component

Load the internal Boot ROM font to VRAM with default settings.

### `BOOT_PRINT`
Components: Font & Print Component, Plane Width Cache

Prints a string of ASCII text. The position of the text on screen is determined by the passed VDP address.

### `BOOT_TO_BCD`
Converts a specified native value to binary coded decimal (BCD).

### `BOOT_TO_BCD_BYTE`
Converts the specified 8bit value to binary coded decimal (BCD).

## Unknown Functions

### `BOOT_UNKNOWN_24`

### `BOOT_UNKNOWN_3B`

d0.l - VDP ptr
d1.w - width
d2.w - height
a1.l - ptr to pattern data

### `BOOT_UNKNOWN_3D`

### `BOOT_UNKNOWN_44`

### `BOOT_UNKNOWN_45`

### `BOOT_UNKNOWN_46`

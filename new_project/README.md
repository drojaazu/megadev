# New Project Template

This is a simple example that can work as a starting point for a new project. It illustrates module loading and execution as well as use of the Main CPU BIOS.

# Architecture

This is an example of loading a memory-resident master control program (kernel) on both of the sides (Main and Sub) of the hardware. We call these resident programs the IPX and SPX, the "extended" IP and SP, respectively. They are loaded early on and remain in memory for the lifetime of the program and provide commonly used functions for all the aspects of your game.

This also acts as an example of the module system. The kernel will manage the loading of different modules as the program runs. The IPX and SPX themselves are also built as modules.

The IPX will run on the Main side and SPX on the Sub side. As discussed in `docs/modules.md`, memory layout needs to be planned carefully, especially for the Main side where Work RAM is limited. In this example, we place a temporary MMD loader and a minimal VBLANK interrupt handler in the IP, and the CDROM access API in the SP. These are the major components necessary to get data loaded from the disc and then execute it. With that in place, we'll load the SPX and IPX modules and go from there.

In this example, we have various particles falling from the top to the bottom of the screen. The code for the sprite processing is located within the IPX while the different modules only provide different graphics and settings. It is an example of how code used across different modules can be stored in resident memory (in this case, the IPX) instead of having to be included each time.

The example here uses both an IPX and SPX, though you are free to use only one of the two on either side depending on your project.

## IPX

The IP is loaded automatically by the system on startup and is placed at the start of Work RAM, at 0xFF0000. The very first code it MUST run is the security code. That means the security code must begin at the start of your IP.

The IP must not exceed 3.5KB (3,584 bytes). That is the space allocated to it within the boot sector. Note that this does not include the security code, which fills an extra 1,412 bytes (for EU/US) or 342 bytes (for JP) of that space. What this means is that the IP has enough space to get things rolling, but that's about it.

This is where the IPX concept becomes helpful. The idea is to have the IP request the IPX to be loaded then copy its entirety into Work RAM to overwrite the IP and reclaim the space of the now useless security code.

This is simple enough at a high level, but we have to keep in mind from where this copy code is running. That is, if the IP running in Work RAM and is also copying to Work RAM, it's going to overwrite itself and everything will fall apart. The solution is simple: build the IPX as a module with a target destination in Work RAM and put the module loader at the IPX entry. In this way, when the IPX entry is called from *Word RAM* after being loaded, the module loader is run and the IPX is copied into *Work RAM*. This is a "self-loading module."

When this is complete, the IPX is now running as the kernel, the "operating system" for your game, loading and running modules depending on the global program mode.

## SPX

The SPX, while similar in concept, is a bit different. The usefulness of the IPX becomes clear when working with the hardware after a while: the IP in the boot sector is too small to have a significant amount of code/resources for use throughout the game and the security code is an ugly waste of space. The IPX solves this by being larger and overwriting the original IP.

The SP, however, can be quite large: it can be up to 28kb within the boot sector, which is plenty of space for code and some resources. Moreover, PRG-RAM has more than 7 times the usable space of Work RAM. We aren't squeezed for space and there is no unnecessary security code to overwrite. The real benefit of creating an SPX is the ability to write code in C instead of entirely in ASM.

Rather than overwriting the SP, we'll keep it where it is and "add on" to it with the SPX. Megadev allocates 16KB of space for the SP by default (though this is user definable), so we'll use that as the boundary. We'll load SPX at 16KB past the start of the user space in PRG-RAM, which begins at 0x6000.

Because both the SP and SPX are resident in memory, you may need to specify both the SP *and* SPX in the definition for subsequently loaded modules on the Sub side. This depends, of course, on if you use any of the functions within each domain. For example, the SP will almost certainly contain the CD-ROM access API. If you need to use it, you'll need to include SP in the definition file. However, if any CD-ROM access calls are wrapped in other functions within the SPX, then you will only need to include SPX.

## Limitations and Alternatives

This IPX/SPX architecture works well if each module will be run directly from Word RAM. Things become a bit more tricky if Word RAM needs to be used for other purposes. (For example, the scaling/rotation capabilities provided by the Mega CD unit requires transferring the graphics to the VDP via Word RAM.)

In these cases, you'll need your module code to run from Work RAM. There are a couple ways this can be accomplished.

First: you could plan your Work RAM usage such that there is enough space for the resident IPX and the transient modules to exist together. The IPX and the modules should be lean, however, so that they can fit in the limited Work RAM space. You will have to get creative when it comes to loading large data such as graphics: you could include the graphics in your module then specify the MODULE_ROM_LENGTH to only include the length of the code, then manually copy the graphics to VRAM from *Word RAM* after the code section is copied to *Work RAM*. Or you could do a multi-step process where the code-only module is loaded, then a graphics module is loaded separately after.

Or, second: do not use a Main side kernel and make all modules self-loading. That is, put the MMD loader at the entry for each module such that it copies itself to the start of *Work RAM* after it has loaded into *Word RAM*. This means each module will need to be entirely self-contained, with all utility functions that would normally be already loaded from a resident IPX kernel. The downside of this is that the modules will be a little larger and take a little longer to load, but this is probably negligible.

# Main BIOS Usage




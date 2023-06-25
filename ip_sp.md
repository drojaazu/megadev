# The IP and SP
The IP (Initial Program) and SP (Sub CPU Program) are small pieces of code that reside within the boot sector of the disc. They are the entry points for your program on the Main and Sub sides, respectively, and are loaded automatically by the Boot ROM on startup. The IP is loaded to 0xFF0000 (the start of the User block) on the main side, and the SP is loaded to 0x6000 (within PRG RAM block 0, at the end of the internal BIOS code) on the Sub side.

The IP and SP should be written in assembly. This is because two of the integral pieces of Megadev, the MMD loader and the CDROM access API, are written in assembly. As both are necessary for the basic flow of loading and executing files, we want them installed in memory as soon as possible and should thus be included in the IP/SP. Their code can be referenced in modules loaded later by including the IP/SP as references in the module def file.

(It *is* theoretically possible to write the IP/SP in C, compile it and the MMD/CDROM code seperately as objects, then link the two together, but considering the fact that the IP/SP are meant to be small init programs, we feel this is unnecessarily convoluted and that writing in C may yield code that is too big or not optimal enough.)

If assembly programming is not your strong point or if the IP/SP becomes overly complex while acting as your game's kernel, it may be best to keep the IP/SP tiny, with only the asm MMD loader/CDROM API in each one, and then load an "extended" kernel (what we call the IPX/SPX) to complement or replace the IP/SP. These IPX/SPX modules can be coded in C, making things easier to work with. The IPX_SPX example project uses this method and is a good skeleton for creating a project with some meat to it. See the readme within the the IPX_SPX example for further explanation of the extended kernel concept.

## Special notes about the IP
The size of the IP is quite constrained within the boot sector. It is allowed only 3.5k of space, part of which is required to be the security code. This is enough for a simple kernel, but it can easily grow in size if you begin to add resident data and global function calls. In such cases, we would once again refer to the extended IP/SP concept, but in general, the size limitations on the IP should be kept in mind when planning your program architecture.

As a side note, the IP can, theoretically, take up a larger share of the boot sector while reducing the size of the SP's share. However, there are quirks around specifying the IP start offset due to the size of the US/EU security code, and we need to test whether changing the boot sector layout will work on actual hardware before asserting that this is a viable option. For now, it is probably best to keep the IP within the known working boundaries and extend the kernel with code loaded from disc seperately.

## Special notes about the SP
The SP requires a header at its start, including a jump table defining the location of user code called from BIOS. (See Sections 4 and 5 of the BIOS Manual for more details.) Megadev provides a pre-defined header that is built along with the SP automatically, so you do not need to worry about setting this up yourself. However, you must globally define each of these functions with the following names:

    sp_init
    sp_main
    sp_int2
    sp_user

These correspond to `usercall0` through `usercall3` as defined in the BIOS manual. All four entries must be defined, even if they are not used. (Unused subroutines can contain a simple `rts` or `return` for asm or C, respectively.) Refer to the SP code within the example projects for examples.

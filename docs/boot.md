# The IP and SP

The IP (Initial Program) and SP (Sub Program) are small pieces of code that reside within the boot sector of the disc. They are the entry points for your program on the Main and Sub sides, respectively, and are loaded automatically by the Boot ROM on startup. The IP is loaded to 0xFF0000 (the start of the User block) on the main side, and the SP is loaded to 0x6000 (within PRG RAM block 0, at the end of the internal BIOS code) on the Sub side.

The IP has room for only 3.5 KB, part of which is the security code, so in practice it does little: it sets up the VDP and interrupts, asks the SP to load a larger program from disc, and jumps to it. That program, the IPX, is an ordinary module and can be written in C. The SP does the same on the Sub side when it outgrows its space. `new_project/src/ip.s` is the annotated reference for this pattern, and every disc example follows it.

The IP and SP should be written in assembly. This is because we want them small, predictable and optimal. Moreover, it's a good idea to include the CD-ROM access framework (if you plan to use it) in your SP, which is written in asm.

(It *is* theoretically possible to write the IP/SP in C, compile it and the MMD/CDROM code seperately as objects, then link the two together, but considering the fact that the IP/SP are meant to be small init programs, we feel this is unnecessarily convoluted and that writing in C may yield code that is too big/not optimal.)

If assembly programming is not your strong point or if the IP/SP becomes overly complex while acting as your game's kernel, it may be best to keep the IP/SP tiny, with only the asm MMD loader/CDROM API in each one, and then load an "extended" kernel (what we call the IPX/SPX) to complement or replace the IP/SP. These IPX/SPX modules can be coded in C, making things easier to work with. The `new_project` example uses this method and is a good skeleton for creating a project with some meat to it. See the readme and the code comments within the example for further explanation of the extended kernel concept.

## Special notes about the IP

The size of the IP is quite constrained within the boot sector. It is allowed only 3.5k of space, part of which is required to be the security code. This is enough for a simple kernel, but it can easily grow in size if you begin to add resident data and global function calls. In such cases, we would once again refer to the extended IP/SP concept. In general, however, the size limitations on the IP should be kept in mind when planning your program architecture.

As a side note, the IP can, theoretically, take up a larger share of the boot sector while reducing the size of the SP's share. However, there are quirks around specifying the IP start offset due to the size of the US/EU security code, and we need to test whether changing the boot sector layout will work on actual hardware before asserting that this is a viable option. For now, it is probably best to keep the IP within the known working boundaries.

## Special notes about the SP

The SP requires a header at its start, including a jump table defining the location of user code called from BIOS. (See Sections 4 and 5 of the BIOS Manual for more details.) Megadev provides a pre-defined header that is built along with the SP automatically, so you do not need to worry about setting this up yourself. However, you must globally define each of these functions with the following names:

    sp_init
    sp_main
    sp_int2
    sp_user

These correspond to `usercall0` through `usercall3` as defined in the BIOS manual. All four subroutines must be defined, even if they are not used. (Unused subroutines can contain a simple `rts` or `return` for asm or C, respectively.) Refer to the SP code within the example projects for examples.

### What the Boot ROM does with the header

The header `lib/sub/sp_header.s` builds is Sega's module header, the "user header" of the BIOS Manual: an eleven-byte name, a flag byte, a version word, a type word, a pointer to the next module, the module size, the offset of the jump table and the work RAM size. The Sub BIOS uses the same format for its own two modules, the boot loader `BOOT____SYS` and the Sub side of the CD player (`MAINBOOTUSR`, later `MAINBOOTCD2`), which sits at 0x6000 until the disc's SP replaces it. One parser handles all of them, and it is byte-identical in every Boot ROM revision. What it reads:

- **The name.** A module is accepted when the name starts with `MAIN`, or when its bytes 8 to 11 are `SYS`, `SUB` or `DAT` followed by a zero. Retail discs use `MAIN       ` or `MEGA_CD_SYS`. Any other name is ignored silently, and the SP never runs.
- **The flag.** Zero means the jump table offset points at a jump table. Non-zero means it points at code, which the BIOS calls first; if that code returns with the carry clear no jump table is installed. Nothing shipped uses it; leave it zero.
- **The jump table offset.** The table is a list of 16-bit offsets, relative to the table itself, ended by a zero word. For each entry the BIOS writes a `jmp` stub into its stub table, starting at `_USERCALL0` (0x5F28): init, main, level 2 interrupt, user.
- **The pointer to the next module.** The field the manual leaves unexplained. It is the distance in bytes from the start of this header to another complete header, which the BIOS parses the same way, appending that module's jump table entries after the previous module's. The chain continues through the next header's pointer until a zero pointer or an unrecognized name.

What the chain is for follows from the stub table. The Sub BIOS keeps one contiguous table of six-byte `jmp` stubs from 0x5EE0 to 0x5FFF, ending at the SP load address: 16 system call slots, of which the last four are `usercall0` to `usercall3`, then the nine exception stubs from `_ADRERR` (0x5F40), then the seven interrupt levels and the sixteen traps. The installer does not stop after four entries; it writes until the zero word. A jump table with 36 entries therefore installs the four user calls and then every exception, interrupt and trap handler of the Sub CPU, in table order. The chain only splits that one long table across separately built pieces, a `MAIN` module with the user calls, a `...SUB` module with handlers, a `...DAT` module with no table but a flagged start routine that runs once as a constructor, which is what a linker that concatenates modules with headers would produce (the manual's example names the header fields after linker segment symbols). Nothing it does is beyond a single longer table, and nothing a table does is beyond writing the stubs from `sp_init`, which is what every known disc does. None of 126 retail images in the library sets the pointer, the flag or the type; the kit writes zero.

One practical consequence: an SP can install its Sub CPU exception handlers through the header by extending the jump table past `sp_user`, instead of writing the vectors in `sp_init`. The kit does not do this today; `sub/crash.h` writes the vectors directly.

The version, type, module size and work RAM size are not read by the Sub BIOS; they are documentation.

The BIOS calls `sp_init` once, then calls `sp_main` once per VSYNC for as long as it returns with the carry clear, passing the previous return value in `d0`. The kit's SPs never return from `sp_main`: they load the rest of the Sub program and jump to it. A return with the carry set and `d0` = 0xFFFF has a special meaning on a system or boot disc: the BIOS runs its boot loader again, reloading the IP and SP from the disc, which restarts the game without a reset.

## Disc identifiers

The first 16 bytes of the disc, set with `HEADER_DISC_ID` in the project makefile, tell the Boot ROM what kind of disc it is. The Disc Format Specification documents two values; the Sub BIOS recognizes four, in every revision, and classifies the disc for `BOOT_CHECK_STATUS` (`CBTCHKSTAT`, `sub/boot.def.h`):

| Identifier | Disc type | Security code checked | Boots |
|---|---|---|---|
| `SEGADISCSYSTEM` | 4, `DISC_SYSTEM` | yes | yes |
| `SEGABOOTDISC` | 6, `DISC_BOOT` | yes | yes |
| `SEGADATADISC` | 5, `DISC_DATA` | no | no |
| `SEGADISC` | 7, `DISC_GAME` | no | no |

The BIOS reads up to 16 sectors from the start of the data track looking for one of the strings, so the identifier does not strictly have to be in sector 0. A disc with none of them, or a system or boot disc whose security code does not match the Boot ROM's region, is reported as type 3, which the manual calls a mixed disc. Only system and boot discs are started, and only those two are region locked; the kit's default `SEGADISCSYSTEM` is the right choice for anything that boots.

`SEGADATADISC` is the identifier for a disc that is never meant to boot: a data disc for a Mode 1 cartridge, for example. The Boot ROM recognizes it on any region's hardware without a security code, and a Mode 1 program reads it through the drive calls regardless. The difference only shows when such a disc is inserted into a console without the cartridge: the CD player treats it as a Mega CD disc that cannot be started, rather than as an unknown CD-ROM.

Verified in MAME with the `bram` example under the identifiers above, on the JP 2.00c and US Model 2 Boot ROMs, with security codes of both regions.

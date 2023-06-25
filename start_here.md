# Getting Started with Megadev

Megadev is a software development kit targeting Sega Mega Drive and Mega CD hardware.

# Hardware Support

The project began life targeting the Mega CD hardware exclusively, but building Mega Drive ROM images was added to the project as part of the addition of Mode 1 (Mega CD boot from cartridge) support. As such, most of the documentation focuses on the Mega CD aspect.

If you wish to create only Mega Drive games, SGDK may be a better option as it is a fair bit more user friendly than Megadev.

# Build Environment & Toolchain

Megadev works best in a POSIX based system (which means Linux in most cases). In theory, it should work in any environment where the GNU build tools are accessible but it may require some tinkering with the makefiles. (Cygwin and MinGW in Windows have not yet been formally tested.)

The build system is intended for use with GNU development tools built with the m68k-elf target to act as a cross compiler. If you are using Linux, be aware that the native build tools you likely have installed will not work as they target your machine's architecture (which is almost certainly not M68000). Check first if there are packages prefixed with `m68k-elf-` in your distribution's package manager and use those if they are available. For example, in Arch Linux, packages are available in the AUR:

https://aur.archlinux.org/packages/m68k-elf-binutils/

https://aur.archlinux.org/packages/m68k-elf-gcc/

## Building the Toolchain

If your distribution does not maintain packages, you will need to build them from scratch. The source is available on the GNU site:

https://www.gnu.org/software/binutils/

https://www.gnu.org/software/gcc/

It is very important that you include ```--target=m68k-elf``` when running the configure script for both binutils and gcc. This will target the M68000 architecture and generate a cross compiler suitable for use with Megadev.

The build system assumes the m68k cross compiler tools are in your path variable. If not, you will need to manually specify the paths of the tools in the global makefile.

# Megadev Setup

Clone the Megadev repo if you have not already done so into an appropriate location; `/opt/megadev` is a good choice. Ensure the permissions on the directory and its files are suitable for use by your user.

In the root of this directory is `makefile.global`. Edit it as necessary to match your environment.

## Example Projects

From here you should probably try compiling a couple of the example projects to confirm your setup is working. These are located within the `examples` subdirectory. Change to one of the subdirectories there (`gfx` is a good choice) and enter `make`. If all went well, a .iso file should be present in the `out` directory within the project which can be run by an emulator or burned to disc.

## Documentation

Before jumping into making a new project, it would be beneficial to read through the documentation presented in the various markdown files in the root of the Megadev directory:

 - program_design.md - Discusses Mega CD program design concepts
 - dev_in_c.md - Some important information if you plan to develop in C
 - modules.md - Introduces MMD modules and how to use them
 - ip_sp.md - Introduces the IP and SP and their peculiarities
 - cdrom.md - Introduces the Sub CPU side CDROM access API
 - bootlib.md - Main CPU Boot ROM library reference
 - disc.md - Notes on creating discs

You should also have a copy of the official Sega Mega Drive and Mega CD reference manuals or equivalent information available online. It is necessary to have a thorough understanding of the hardware in order to develop effective software, and this is especially true when dealing programming for the Mega CD where there are multiple CPUs to be managed.

## Kilo/Mega

As a note, we use such terms as "kilo" and "mega" when referring to base 2 values, meaning, for example, a kilobyte is 1,024 bytes and a megabit is 131,072 bytes. We acknowledge the necessity and importance of the differentiation between decimal and binary base identifiers in the engineering world but assert that words like "kibibyte" and "mebibyte" sound ridiculous. Since this framework deals exclusively with retro hardware that existed before SI prefix standardization, we choose to use the old naming system. All SI prefixed numbers throughout the documentation are base 2.

# Starting a Project

# Building

## make Targets


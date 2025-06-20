
Documentation

- readme
- bootlib
- cdrom - include information about file loading without the wrapper (move by offset, load into CDC in loop, etc); re-evaluate some of teh naming the whole CDROM access wrapper subsystem
- dev_in_c - touch up in general, especially the stack usage section
- disc
- ip_sp
- modules
- program design
- start here


Merge everything into one manual...

# Introduction

Megadev is a development kit for creating software for the Sega Mega Drive and Mega CD hardware. It is collection of utilities, definitions, C headers, documentation, and examples to make development for these systems less painful than starting from scratch.

Megadev is intended for those who already have some experience with C or M68k assembly programming and have at least a passing familiarity with embedded systems. It is less "friendly" than something like [SGDK](https://github.com/Stephane-D/SGDK), but this is due in large part to the increased complexity of the addition of the Mega CD (which SGDK does not support) and due to a desire for a development system that is more flexible for skilled developers and less reliant on an abundance of external utilities.

The original target hardware was the Mega CD in particular, with support for the vanilla Mega Drive being somewhat of a late addition. Moreover, there is already a wealth of information and examples for the MD. As such, most of the information introduced here is focused on the Mega CD side of things.

# Requirements

In order to compile code for the Mega Drive/CD, you will need a build environment that targets the M68000 CPU architecture. Megadev is designed to use the GNU development tools that have been built for this architecture (m68k-elf). In addition, you will need `mkisofs` to generate the .iso disc image for Mega CD games.

Your distribution may have packages for these tools, or you can compile them from scratch if necessary. Alternatively, we have included a Dockerfile that for creating a Dev Container in Visual Studio Code to streamline this process.

## Docker

This is the modern, preferred method for development as it should work on all platforms (Windows, MacOS, Linux). The simplest way to work with this is to copy the Dockerfile into your project directory and load the diretcory in Visual Studio Code with the Dev Containers extension installed (or any other IDE that supports Dev Containers).

## By Distro

### Arch Linux

Install the following packages from the AUR:

https://aur.archlinux.org/packages/m68k-elf-binutils/

https://aur.archlinux.org/packages/m68k-elf-gcc/

And install the following package from the standard repository:

`cdrtools`

### Debian

Install the following packages from the standard repository:

`binutils-m68k-linux-gnu`

`gcc-m68k-linux-gnu`

`genisoimage`

## Building From Source

If your distro does not contain pre-built packages for the GNU build tools with the M68000 target, you can build them from scratch. The source for binutils and gcc can be found here:

https://www.gnu.org/software/binutils/

https://www.gnu.org/software/gcc/

You *must* add `--target=m68k-elf` as an option when you run the configure script in order target the M68000 architecture! You will also need to modify the `M68K_PREFIX` variable in your makefile to indicate the prefix used for the tools. By default, this should be `mm68k-elf-`.

You may want to use a helper script [like the ones found here](https://github.com/kentosama/m68k-elf-gcc).

# Getting Started

Once your build environment is in place, you should attempt to compile and run one of the example projects. Try `hello_world` first and then `ipx_spx` after that, as these are the simplest examples of booting the Mega CD and loading files from disc.

To build the example project, run `make disc` from the root directory. This should generate a disc.iso file which can run in an emulator or burnt to disc.

After that, feel free to try the remaining examples. In all cases, check the `README.md` file for details about each example.

# Megadev Project Structure

discuss the makefile and using ips_spx as a skeleton

# The Hardware

## Official Sega Documentation

You should have a copy of the official documentation about the Mega Drive and Mega CD for reference. These are readily available online and not difficult to find.

Though they will primarily be used for reference, we still highly recommend reading through them to become familiar with the capabilities and expectations of the hardware.

We will highlight some of the important aspects of the hardware and its usage in the next sections.

## Boot Process (IP/SP)

To make a game bootable on the Mega CD, there are two primary elements that must be in place: the header and the security code.

The header is a string of text that appears at the very start of the disc's data, and the security code is a small bit of code that............



## Internal ROM (BIOS/Boot Library)

## Gate Array

# Development Considerations

## Program Architecture

## Development in C

# Megadev Concepts & Utilities

## Modules (MMD/SMD)

## CD-ROM Access Framework

Accessing data on the disc is done via commands provided by the BIOS on the Sub CPU side. The system in place is somewhat complex, as instead of using filenames, a start/end sector must be specified. Moreover, the transfer process is multi step, involving setting the destination in the CDC Mode register in the Gate Array and using the CDC BIOS calls to start the transfer and monitor its status.

The access framework provided within Megadev simplifies this process by simply specifying a filename and an output buffer.


---

The CD-ROM access wrapper provides a simplified method of retrieving files from disc. At a high level, it runs a loop on the Sub CPU that is "pumped" by an update call during INT2. As the access operation runs, it will pause at certain points and return from the interrupt. It saves this break point as a pointer and work resumes from that point at the next INT2. In this way, the Sub CPU is not entirely blocked waiting for time-consuming IO operations.

TODO: Most of this is asm focused, need to expand C coverage/documentation.

### Example

Please have a look at the `ipx_spx` sample project for an example of file loading using this system.

### Setup

CD-ROM access can only be done with the Sub CPU, so requests for files should be done via the communication registers within the gate array. The framework code should remain resident in memory, so we recommend including it within the SP that is loaded to the Sub CPU side during boot.

Include `sub/cdrom.macro.s` and `sub/cdrom.s`in the SP code. In your SP init subroutine (called `_usercall0` in the BIOS Manual), prime the access loop by calling the `INIT_ACC_LOOP` macro.

Somewhere in your INT2 subroutine (_usercall2), make a call to the `PROCESS_ACC_LOOP` macro to keep the access loop moving. You may want to put this at the end of the subroutine or push the registers before calling as it may clobber any number of registers.

Finally, in the early part of SP main subroutine (_usercall1), you'll want to load and cache the file information by setting ACC_OP_LOAD_DIR as the access operation and waiting for it to complete. There is space allocated for 128 files by default, but this can be adjusted to match your project by changing the size of the `dir_cache` buffer in `sub/cdrom.s`.

### Usage

At this point, you are ready to load files. There are four steps involved:
  - Set the pointer to the filename string in `filename`
	- Set the destination buffer in either the GA DMA register or `file_dest_ptr` depending on transfer type
	- Set the appropriate access operation
  - Wait for the operation to complete

You do not need to set the CDC device destination in the _GAREG_CDCMODE register. This will be done automatically depending on the access operation chosen.

### Filenames

The filename must conform to [ISO9660 Level 1 standards](https://wiki.osdev.org/ISO_9660#Filenames). Basically this means that filenames must contain:

- only UPPERCASE alphanumeric (A-Z, 0-9) characters and underscore
- a maximum of 8 characters with a 3 character extension
- the version identifier

Finally, the filename in memory must be a standard null termninated C string.

Note that you can keep your filenames as lowercase on your local system. The ISO creation process will convert them as necessary. (Also note that if you mount the ISO on your local system, they may appear lowercase in your file manager. However, they are UPPERCASE in the filesystem on disc, which is how you will be accessing them on the Mega CD.) You must limit the filename size to 8.3, however, and use only valid characters (A to Z, 0 to 9, and _).

You may not be familiar with the version identifier as is not shown in directory listings. It is a part of the ISO9660 standard, though, and is present within the filesystem table. In reality, it is not used by applications and foregoing a lot of unnecessary background information, it simply means your filenames will be appended with `;1` when referring to them within the filesystem.

As a quick example, you may have `title.mmd` on your local system. This will be represented as `TITLE.MMD;1` within the ISO filesystem on disc, and that is how you should refer to it in your code.

### Destination buffer

Please refer to the CDC section of the Mega CD Software Development documentation before or alongside this section.

The buffer where your data will be stored must be specified in one of two ways depending on the transfer type. If you plan to use DMA, you will need to set the destination address in the `GA_DMAADDR` register. If you plan to use non-DMA transfer via the CDC host data register, you will need to set the address in the `file_buff` pointer.

### Access operation

Finally, you will need to set the access operation on `access_op`. The operations for loading files are:

  ACC_OP_LOAD_CDC
  ACC_OP_LOAD_CDC_DMA
  ACC_OP_LOAD_PRG_DMA
  ACC_OP_LOAD_PCM_DMA

(Note that we do not currently support the Main CPU read option, mostly because it is not well understood.)

ACC_OP_LOAD_CDC will load the data to any memory address available to the Sub CPU, while the DMA options transfer data to a specific device given a relative address specified in the GA DMA register. ACC_OP_LOAD_CDC is the easiest to work with as you can simply specify an absolute address and be done. The DMA options likely provide greater transfer speed (limited by the optical drive, of course), though what sort of speed advantages or if any bus access issues may occur are unknown.

### Wait for data

After setting the access operation, you will need to wait for the data to completely load into your buffer. This is done by checking `access_op` in a loop (while calling the INT2 wait in the loop!) until the value has returned to ACC_OP_IDLE (i.e. 0). When that occurs, the transfer is either complete or failed. You should then check the `access_op_result` for the result code:

  RESULT_OK
	RESULT_LOAD_FAIL
	RESULT_NOT_FOUND

If the result is ok, your data should be ready in the buffer. The load fail result means there was a read failure of some sort (likely due to a damaged disc or old harware). Not found indicates the filename provided could not be found in the file system.

## Convenience routines

For simple file loads, you can use the `load_file_sub` convenience subroutine to package most of this up for you. Simply place the pointer to the filename in a0 and the pointer to the destination buffer in a1, call it, and check the result in d0.

The `get_acc_op_result` routine will check if the access operation is still busy, with carry clear (cc) indicating the operation is complete and carry set (cs) indicating it is still in progress.

There is also the WAIT_FOR_ACC_OP macro which will check get_acc_op_result in a loop until the operation is complete.

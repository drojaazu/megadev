
# CD-ROM Access Wrapper

The CD-ROM access wrapper provides a simplified method of retrieving data from disc. At a high level, it runs a loop on the Sub CPU that is "pumped" by an update call during INT2. As the access operation runs, it will pause at certain points and return from the interrupt. It saves this breakpoint as a pointer and work resumes from that point at the next INT2. In this way, the Sub CPU is not entirely blocked waiting for time-consuming IO operations.

## Setup

CD-ROM access can only be done with the Sub CPU, meaning the wrapper can only be included with Sub CPU side code. It should be included in the SP and remain resident in memory. It works the same from a Mode 1 cartridge's SP, with the differences listed in `docs/mode1.md`: the SP must initialize the drive itself, and INT2 comes from the cartridge program.

Include `sub/cdrom.s` or `sub/cdrom.h` in your code. In your SP init subroutine (`sp_init`), prime the access loop by calling the `INIT_ACC_LOOP` macro.

Somewhere in your INT2 subroutine (`sp_int2`), make a call to the `PROCESS_ACC_LOOP` macro to keep the access loop moving. You may want to put this at the end of the subroutine or push the registers before calling as it will clobber a number of registers.

Finally, in the early part of SP main subroutine (`sp_main`), you'll want to load and cache the file information by setting CDROM_LOAD_FILE_LIST as the access operation and waiting for it to complete. There is space allocated for 128 files by default, but this can be adjusted to match your project by changing the size of the `dir_cache` buffer in `sub/cdrom.s`.

## Usage

At this point, you are ready to load files. There are four steps involved:

- Set the pointer to the filename string in `filename`
- Set the destination buffer in either the Gate Array DMA register or `file_dest_ptr` depending on the operation
- Set the appropriate access operation
- Wait for the operation to complete

You do not need to set the CDC device destination in the GA_SUB_REG_CDCMODE register. This will be done automatically depending on the chosen access operation.

### Filename

The filename must conform to [ISO9660 Level 1 standards](https://wiki.osdev.org/ISO_9660#Filenames). Basically this means that filenames must contain:

- only UPPERCASE alphanumeric (A-Z, 0-9) characters and underscore
- a maximum of 8 characters with a 3 character extension
- the version identifier

Finally, the `filename` variable must be a standard null termninated C string.

Note that you can keep your filenames as lowercase on your local system. The ISO creation process will convert them as necessary. (Also note that if you mount the ISO on your local system, they may appear lowercase in your file manager. However, they are UPPERCASE in the filesystem on disc, which is how you will be accessing them on the Mega CD.) You must limit the filename size to 8.3, however, and use only valid characters (A to Z, 0 to 9, and _).

You may not be familiar with the version identifier as is not shown in directory listings. It is a part of the ISO9660 standard, however, and is present within the filesystem table. In reality, it is not actually used by applications and, foregoing a lot of unnecessary background information, it simply means your filenames will be appended with `;1` when referring to them within the filesystem.

As a quick example, you may have `title.mmd` on your local system. This will be represented as `TITLE.MMD;1` within the ISO filesystem on disc, and that is how you should refer to it in your code.

### Destination Buffer

Please refer to the CDC section of the Mega CD Software Development documentation before or alongside this section.

The destination is set in `filebuff` for every operation. For `CDROM_LOAD_CDC`, the non-DMA transfer through the CDC host data register, it is an address in the Sub CPU's memory map. For the DMA operations it is the offset within the destination memory, in bytes and a multiple of 8: an offset into Word RAM, an address in Program RAM, or, for wave RAM, an offset into the 8 KB window the Sub CPU sees of the selected bank, where every wave RAM byte sits at an odd address, so twice the offset within the bank. A sector therefore advances the wave RAM destination by 0x1000, and a bank takes exactly two. Do not write `GA_SUB_REG_DMAADDR` yourself: selecting the destination resets that register, and the 1.00 series Sub BIOS reselects it when a read starts, so the access loop writes the register before every CDC read, from `filebuff`.

### Access Operation

Next, you will need to set the access operation on `access_op`. The operations for loading files are:

```
CDROM_LOAD_CDC
CDROM_LOAD_CDC_DMA
CDROM_LOAD_PRG_DMA
CDROM_LOAD_PCM_DMA
```

(Note that we do not currently support the Main CPU read option, mostly because it is not well understood.)

CDROM_LOAD_CDC will load the data to any memory address available to the Sub CPU, while the DMA options transfer data to a specific device at the relative address in `filebuff`. CDROM_LOAD_CDC is the easiest to work with as you can simply specify an absolute address and be done. The DMA options likely provide greater transfer speed (limited by the optical drive, of course), though what sort of speed advantages or if any bus access issues may occur are unknown.

### Loading Part of a File

Two more variables select a part of the file: `part_offset`, the first sector to read counted from the start of the file, and `part_count`, how many sectors to read. With `part_count` at 0, the default, an operation loads the whole file and `part_offset` is ignored. With a count, the operation reads exactly that many sectors, whether or not the file is that long, and `filesize` reports the part's size. The loop does not reset either variable, so set them before every operation; in C, `load_file` clears the count and `load_file_part` sets both.

Parts are how data is streamed: a file larger than its destination is read a few sectors at a time into the region that is not in use, as `examples/pcm_playback` does with the PCM chip's wave RAM. They are also the only way to use the PCM DMA destination for anything larger than 4 KB: the DMA address register reaches one wave RAM bank, so a PCM DMA load is at most two sectors into the bank selected with `PCM_CTRL`.

### Wait for Completion

After setting the access operation, you will need to wait for the data to completely load into your buffer. This is done by checking `access_op` in a loop until the value has returned to CDROM_IDLE (i.e. 0). When that occurs, the transfer is complete. You should then check the `access_op_result` for the result code:

```
CDROM_RESULT_OK
CDROM_RESULT_LOAD_FAIL
CDROM_RESULT_NOT_FOUND
```

If the result is ok, your data should be ready in the buffer. The load fail result means there was a read failure of some sort (likely due to a damaged disc or old harware). Not found indicates the filename provided could not be found in the file system.

## Convenience Routines

For simple file loads, you can use the `load_file_sub` convenience subroutine to package most of this up for you. Simply place the pointer to the filename in a0 and the pointer to the destination buffer in a1, call it, and check the result in d0. `load_file_prg_dma` does the same for a DMA load into Program RAM, with the destination address in a1.

The `get_acc_op_result` routine will check if the access operation is still busy, with carry clear (cc) indicating the operation is complete and carry set (cs) indicating it is still in progress.

There is also the WAIT_FOR_ACC_OP macro which will check get_acc_op_result in a loop until the operation is complete.

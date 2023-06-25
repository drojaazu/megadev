
# CD-ROM Access Wrapper

The CD-ROM access wrapper provides a simplified method of retrieving data from disc. At a high level, it runs a loop on the Sub CPU that is "pumped" by an update call during INT2. As the access operation runs, it will pause at certain points and returns from the interrupt. It saves this break point to a pointer and work resumes from that point at the next INT2. In this way, the Sub CPU is not blocked waiting for time-consuming IO operations.

**Please have a look at the "mod_load" example (in particular, the `sp.s` file) for a file loading example.**

TODO: Most of this is asm focused, need to expand C coverage/documentation.

## Setup

CD-ROM access can only be done with the Sub CPU, meaning the wrapper can only be included with Sub CPU side code. It should be included in the SP and remain resident in memory. (See the modules readme for more info about memory resident code.)

Include `sub/cdrom.s` or `cd_sub_cdrom.c` in your code. In your SP init subroutine (called `_usercall0` in the BIOS Manual), prime the access loop by calling the `INIT_ACC_LOOP` macro.

Somewhere in your INT2 subroutine (_usercall2), make a call to the `PROCESS_ACC_LOOP` macro to keep the access loop moving. You may want to put this at the end of the subroutine or push the registers before calling as it may clobber any number of registers.

Finally, in the early part of SP main subroutine (_usercall1), you'll want to load and cache the file information by setting ACC_OP_LOAD_DIR as the access operation and waiting for it to complete. There is space allocated for 128 files by default, but this can be adjusted to match your project by changing the size of the `dir_cache` buffer in `sub/cdrom.s`.

## Usage

At this point, you are ready to load files. There are four steps involved:
  - Set the pointer to the filename string in `filename`
	- Set the destination buffer in either the GA DMA register or `file_dest_ptr` depending on transfer type
	- Set the appropriate access operation
  - Wait for the operation to complete

You do not need to set the CDC device destination in the _GA_CDCMODE register. This will be done automatically depending on the access operation chosen.

### Filename

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

After setting the access operation, you will need to wait for the data to completely load into your buffer. This is done by checking `access_op` in a loop (while calling the INT2 wait in the loop!) until the value has returned to ACC_OP_IDLE (i.e. 0). When that occurs, the transfer is complete. You should then check the `access_op_result` for the result code:

  RESULT_OK
	RESULT_LOAD_FAIL
	RESULT_NOT_FOUND

If the result is ok, your data should be ready in the buffer. The load fail result means there was a read failure of some sort (likely due to a damaged disc or old harware). Not found indicates the filename provided could not be found in the file system.

## Convenience routines

For simple file loads, you can use the `load_file_sub` convenience subroutine to package most of this up for you. Simply place the pointer to the filename in a0 and the pointer to the destination buffer in a1, call it, and check the result in d0.

The `get_acc_op_result` routine will check if the access operation is still busy, with carry clear (cc) indicating the operation is complete and carry set (cs) indicating it is still in progress.

There is also the WAIT_FOR_ACC_OP macro which will check get_acc_op_result in a loop until the operation is complete.

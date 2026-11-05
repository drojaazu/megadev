# mode1

A cartridge that drives the Mega CD (Mode 1). It detects the Boot ROM at
0x400000, unpacks the Sub BIOS into Program RAM, loads its own Sub CPU program
and then uses the Boot ROM library from the cartridge: default VDP registers,
VRAM clear and the VBLANK handler. Text is printed with the kit's `text_print`
and `font8x8` (`main/text.h`).

With a disc in the drive, the Main CPU has the Sub CPU initialize the drive,
read the root directory and load `DATA.BIN` into Word RAM twice: by Sub CPU
copy and by CDC DMA. It then sums the words it finds in Word RAM and compares
them with the sum the build computed from the file. Without a disc the drive
init never completes; the Main CPU times out, reports `DISC: TIMEOUT` and the
rest keeps running.

`make` builds `mode1.cart` and `mode1.iso`. The disc holds one file, an 8 KB
pattern the makefile generates; it has no boot sector and only the cartridge
reads it. In BlastEm: `blastem -o mode1.iso mode1.cart`.

The screen shows the detection result, the Sub CPU bring-up status, the BIOS
frame counter, a value sent to the Sub CPU through COMCMD0 and echoed back
through COMSTAT0, the Sub CPU's INT2 count, and the disc results.

Variables hold the same results for an emulator debugger (addresses in
`build/mode1.cart.sym`): `mode1_status`, `mode1_echo`, `mode1_disc`,
`mode1_load`, `mode1_load_sum`, `mode1_load_dma`, `mode1_load_dma_sum` and
`mode1_file_size`. A disc result is `0x0100 | result byte` from the access
loop (`0x0164` is success), or `0x02FF` timeout, `0x02FE` Word RAM not
returned, `0x02FD` content mismatch.

See `docs/mode1.md` for the Mode 1 details.

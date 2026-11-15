# bram

This is an example of using the Backup RAM functionality. It will list the directory contents, write a user selected value to the BRMEX file, and read back the data from that file.

Note that your Backup RAM must be formatted before this will work, otherwise it will report errors.

## Mode 1 build

`make MODE1=1` builds `bram.cart`, a cartridge that runs this example on a Mega CD
with no disc in the drive (Mode 1). The example code is unchanged:
`src/mode1_init.s` and `src/mode1_main.c` start the Sub CPU with the SP and
the SPX together and put the first Main CPU module in Word RAM, the work `ip.s`
and the disc do on a disc boot. See "Disc programs as cartridges" in
`docs/mode1.md`.

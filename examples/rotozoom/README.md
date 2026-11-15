# rotozoom

This is an example of the scaling, zoom, and rotation capabilities of the Mega CD's graphics processor.

P1 still will scroll the field. Hold the A button to resize the field.

## Mode 1 build

`make MODE1=1` builds `rotozoom.cart`, a cartridge that runs this example on a Mega CD
with no disc in the drive (Mode 1). The example code is unchanged:
`src/mode1_init.s` and `src/mode1_main.c` start the Sub CPU with the SP and
the SPX together and put the first Main CPU module in Word RAM, the work `ip.s`
and the disc do on a disc boot. See "Disc programs as cartridges" in
`docs/mode1.md`.

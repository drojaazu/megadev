# mode1

A cartridge that drives the Mega CD (Mode 1). It detects the Boot ROM at
0x400000, unpacks the Sub BIOS into Program RAM, loads its own Sub CPU program
and then uses the Boot ROM library from the cartridge: default VDP registers,
VRAM clear, the built-in font, text printing and the VBLANK handler.

The screen shows the detection result, the Sub CPU bring-up status, the BIOS
frame counter and a value sent to the Sub CPU through COMCMD0 and echoed back
through COMSTAT0.

Two variables hold the same results for an emulator debugger: `mode1_status`
and `mode1_echo` (addresses in `build/mode1.cart.sym`).

See `docs/mode1.md` for the Mode 1 details.

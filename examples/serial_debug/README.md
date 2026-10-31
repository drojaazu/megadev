# Serial debugging

Talks to a terminal on your PC over one of the Mega Drive's controller ports, so a running
program can log to your desk and you can type commands back at it.

The Mega Drive side prints a banner on startup, echoes what you type, and recognizes `peek` and
`poke` commands. The rest of the project is the particle demo from the project template, present only to give the
debug link something to run alongside.

> **Status.** The outbound path is verified: the banner reaches a host. The command interpreter is
> not verified; MAME cannot exercise it (see below) and it has not been confirmed on hardware. `peek`
> and `poke` currently reply with a fixed string rather than reading or writing memory.

## Wiring

Each controller port carries a hardware UART on two of its pins. `TL` is the transmit line and `TR`
is receive, as seen from the Mega Drive.

| DE-9 pin | line | direction | connect to |
| --- | --- | --- | --- |
| 6 | TL | Mega Drive → PC | adapter **RX** |
| 9 | TR | PC → Mega Drive | adapter **TX** |
| 8 | GND | — | adapter **GND** |

Pin numbering for TL and TR is from plutiedev's I/O ports reference; pin 8 as ground follows the
conventional Mega Drive controller pinout. Check against a pinout for your model before soldering.

### These are 5V TTL pins, not RS-232

The port "mimics the serial ports in old PCs, albeit at 5V". That difference matters:

- **A TTL-level USB-serial adapter connects directly.** An FTDI TTL-232R-5V cable, or a CP2102 /
  CH340 module with 5V logic, is the straightforward option. Confirm it is 5V tolerant.
- **A real PC COM port, or a USB adapter with a DE-9 shell, does not.** RS-232 swings to ±12V and
  is logically inverted relative to TTL. Wiring one directly will not work and risks damaging the
  console. Use a MAX232-class level shifter in between.
- **A microcontroller board works as a bridge**, which is what makes an Arduino a convenient
  stand-in: its pins are already 5V TTL, so it is level-compatible and can forward bytes to USB.

## Host side

The link is 8N1 at the rate the program selects — 4800 baud by default. Any terminal will do:

    screen /dev/ttyUSB0 4800
    minicom -D /dev/ttyUSB0 -b 4800

Match the baud on both ends. If output arrives as consistent garbage rather than nothing at all,
a baud mismatch is the first thing to check.

## Choosing the port and speed

Set these from the project makefile rather than editing the library:

    CC_FLAGS+=-DEXT_PORT=3
    CC_FLAGS+=-DEXT_BAUD=SCTRL_BAUD_1200

`EXT_PORT` is 1 (player 1), 2 (player 2, the default) or 3 (the rear EXT port). Port 3 exists only
on early Mega Drive models — it was removed on the Mega Drive 2 — so prefer a controller port
unless you know the target hardware has it.

`EXT_BAUD` is one of `SCTRL_BAUD_300`, `SCTRL_BAUD_1200`, `SCTRL_BAUD_2400` or `SCTRL_BAUD_4800`;
4800 is the hardware maximum.

## Running under MAME

MAME emulates the transmit path but **not** receive, so the log works and the command interpreter
does not. From `sega_md_ioport.cpp`: *"Serial reception - requires peripherals to be reworked."*

    : > out.bin
    mame segacd2 -cdrom serial.iso \
        -ctrl2 rs232 -ctrl2:rs232:com null_modem -bitb out.bin

The bitbanger opens the file rather than creating it, hence the `: >` first. Note also that MAME's
null modem defaults to 9600 in both directions while the Mega Drive tops out at 4800, so captured
bytes are garbled until the rate is changed in MAME's own menu — it cannot be set from the command
line.

## Library API

The port itself is driven through `main/comm.h`, which is usable outside this example:

| function | purpose |
| --- | --- |
| `init_ext_port()` | configure pin directions and enable serial mode |
| `ext_status()` | read S-CTRL; test with `SCTRL_RX_READY_MASK`, `SCTRL_RX_ERR_MASK`, `SCTRL_TX_FULL_MASK` |
| `ext_tx(byte)` | send, waiting for the transmitter to drain |
| `ext_rx()` | block until a byte arrives and return it |
| `ext_read_byte()` | read the data register without waiting, for use in an interrupt handler |

Read the status before touching the data register: reading `RxData` directly discards the ready and
error bits.

Each of these brackets its hardware access with a Z80 bus request and masks interrupts for the
duration, releasing between polls so a slow link cannot starve audio playback.

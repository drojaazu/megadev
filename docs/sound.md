Music and sound effects with the Echo sound driver: what it plays, where its data must live, how to start it, and the rules a program follows while it runs.

# Contents

- Overview
- Data
- Playing music and sound effects
- Rules while Echo runs
- Converting music
- Mega CD programs

---

# Overview

The kit includes Echo, a sound driver by Javier Degirolmo that runs on the Z80 and drives the YM2612 (six FM channels, one of which can play 8-bit PCM samples) and the PSG (three square wave channels and noise). The 68000 starts it and sends it commands; the Z80 does the rest, so music costs the 68000 almost nothing.

`main/echo.h` is Echo's own C API with Echo's function names, so Echo's documentation (`doc/` in its repository, github.com/sikthehedgehog/echo) applies as written. The kit links Echo 1.65's prebuilt Z80 program unmodified; no Z80 assembler is needed. Echo is distributed under the zlib license, kept with it in `lib/main/echo/LICENSE`. A program that never calls `echo_init` does not contain Echo.

# Data

Echo plays four kinds of data, all in Echo's own formats:

| Format | What it is |
|---|---|
| ESF | A stream of events: music, or a sound effect |
| EIF | An FM instrument: the YM2612 registers of one patch |
| EEF | A PSG instrument: a volume and pitch envelope |
| EWF | A PCM sample: unsigned 8-bit mono, about 10 kHz |

**All of it must be in cartridge ROM.** The Z80 reads streams and instruments itself, through its window onto 68000 memory, for as long as they play. That window reaches cartridge ROM. It does not reach Main Work RAM: on a Mega Drive every byte the Z80 reads there comes back as 0xFF. Some emulators allow it, so a program that keeps Echo data in RAM can work in an emulator and play noise on the console.

Include the files in an assembly source, which places them in ROM:

    .section .rodata
    .global song, piano, bass
    song:  .incbin "song.esf"
    piano: .incbin "piano.eif"
    bass:  .incbin "bass.eif"

# Playing music and sound effects

`echo_init` uploads Echo to the Z80 and loads the instrument list: pointers to each instrument in the order the streams number them, ended by `NULL`.

    #include <main/echo.h>

    extern uint8_t const song[], piano[], bass[], jump_sfx[];

    static void const * const instruments[] = {piano, bass, NULL};

    echo_init(instruments);
    echo_play_bgm(song);
    ...
    echo_play_sfx(jump_sfx);

A sound effect takes the channels its stream locks from the music and gives them back when it ends. `echo_get_status` reports what is playing; `echo_set_volume` sets a global volume; the rest is in `main/echo.h` and Echo's `doc/api-c.txt`.

Every Echo function may be called from the main program or from an interrupt handler: each takes the Z80 bus for a moment with interrupts masked.

# Rules while Echo runs

The Z80 shares the 68000's bus to fetch its data, and two things the 68000 does disturb it. While Echo runs, a program does both of these with the Z80 bus held:

- **DMA from 68000 memory to the VDP.** The 68000 bus belongs to the VDP during the transfer; a Z80 fetch in that time reads garbage.
- **Reading the controllers.** An access to the I/O ports (0xA10000 to 0xA1001F) shortens the Z80's bus cycle, and a Z80 fetch at that moment can read wrong data.

Hold the bus with interrupts masked, so that an interrupt handler that also takes the bus cannot release it in the middle:

    uint16_t const sr = mask_interrupts();
    z80_request_bus();
    // DMA or controller reads
    z80_release_bus();
    restore_interrupts(sr);

Keep these sections short. While the bus is held Echo stops, and a PCM sample stalls with it.

# Converting music

Echo's formats come from a tracker module through a converter. The kit's own converter, reading Furnace and DefleMask modules, is planned. Until then, the converters listed in Echo's README work. One note on BigEvilCorporation/dmf2esf: its binary output writes the first event of every stream as text, which Echo misreads; its assembly output (`-a`) is correct.

# Mega CD programs

Mode 1 programs are cartridges and use Echo as described here. Mega CD disc programs are not supported yet: a disc has no ROM, Main Work RAM is out of the Z80's reach, and whether the Z80 can read Word RAM has not been measured on a Mega CD.

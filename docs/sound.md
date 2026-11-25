Music and sound effects with the Echo sound driver: what it plays, where its data must live on each target, how to start it, and the rules a program follows while it runs.

# Contents

- Overview
- Data
- Playing music and sound effects
- Rules while Echo runs
- Converting music
- Mega CD programs
- Sound commands from the Sub CPU

---

# Overview

The kit includes Echo, a sound driver by Javier Degirolmo that runs on the Z80 and drives the YM2612 (six FM channels, one of which can play 8-bit PCM samples) and the PSG (three square wave channels and noise). The 68000 starts it and sends it commands; the Z80 does the rest, so music costs the 68000 almost nothing.

`main/echo.h` is Echo's own C API with Echo's function names, so Echo's documentation (`doc/` in its repository, github.com/sikthehedgehog/echo) applies as written. The kit links a prebuilt Z80 program from Echo 1.68a-dr.1 (github.com/drojaazu/echo), a fork of Echo 1.68a with two fixes. First, it comes in two builds whose FM and PSG notes are tuned to the NTSC or the PAL clocks, and the project makefile's `VIDEO` setting picks one (run `make clean` after changing it). Stock Echo has one build that plays PSG notes about 10 cents sharp of FM notes on either machine type, and up to 119 cents sharp at the top. The PAL build's values are calculated from the PAL clocks and have been tested in emulators only. Second, a PSG instrument's semitone shift (arpeggio) past either end of the note range holds at C-3 or B-8 instead of writing stray values to the PSG. Nothing else differs, and no Z80 assembler is needed; `lib/main/echo/NOTICE` describes the change. Echo is distributed under the zlib license, kept with it in `lib/main/echo/LICENSE`. A program that never calls `echo_init` does not contain Echo.

# Data

Echo plays four kinds of data, all in Echo's own formats:

| Format | What it is |
|---|---|
| ESF | A stream of events: music, or a sound effect |
| EIF | An FM instrument: the YM2612 registers of one patch |
| EEF | A PSG instrument: a volume and pitch envelope |
| EWF | A PCM sample: unsigned 8-bit mono, about 10 kHz |

**All of it must be where the Z80 can read it, for as long as it plays:** cartridge ROM on a Mega Drive cartridge or a Mode 1 program, 2M Word RAM owned by the Main CPU on a Mega CD disc (see Mega CD programs, and its warning). The Z80 reads streams and instruments itself, through its window onto 68000 memory. That window does not reach Main Work RAM: on a Mega Drive every byte the Z80 reads there comes back as 0xFF. Some emulators allow it, so a program that keeps Echo data in Work RAM can work in an emulator and play noise on the console.

Echo also has its own limit: it stores each address in a bank byte that holds address bits 15 to 21 and bit 23, not bit 22. It can address 0x000000 to 0x3FFFFF (cartridge ROM; on a disc, Program RAM's window and Word RAM at 0x200000) but not 0x400000 to 0x7FFFFF, where Mode 1 programs see the Mega CD's memory. An address there is read 0x400000 lower, as cartridge space.

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

`examples/sound` puts this together: a song, two sound effects (one on a channel the music leaves free, one that borrows a music channel), pause, stop and volume. Its `tools/gen_song.py` writes Echo's stream format directly from note lists.

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

> **Warning: Echo on a Mega CD disc goes against Sega's documentation and against how retail games did it.**
>
> - The Mega CD Hardware Manual says: "Do not apply following to Z80: Accessing the WORD-RAM. Accessing the PRG-RAM. Accessing all the area of 68000 CPU side. However, Z80 can use its 8K-RAM."
> - The Sega-CD Technical Bulletins (1993, sound section) say: "The MEGA-CD-side RAM can not be read from Z80. Therefore, the data should either be left in Z80 SRAM or it should be captured when exchanged with MAIN-CPU." and "PSG sound source can not be accessed from Z80. Therefore, to use PSG sound source, execute from MAIN-CPU." Sega's intended split is CD-DA for music, a PCM driver on the Sub CPU, an FM driver on the Z80 whose data lives in Z80 RAM, and the PSG driven by the Main CPU.
> - Retail games followed it. In a survey of 107 Mega CD titles in MAME (about 70 seconds each from boot), none read 68000 memory from the Z80; Z80 FM drivers kept their data in Z80 RAM, and every title that played PSG music drove it from the 68000.
>
> Echo does both forbidden things: it reads its music from Word RAM and writes the PSG from the Z80. It worked on the hardware below, which suggests Sega's rules are a design choice (a disc has no cartridge to bank into) rather than an electrical limit, but that is not established for every model.
>
> | Verified | Not verified |
> |---|---|
> | Mega Drive 2 + Japanese Mega-CD 2 (BIOS 2.00), disc on CD-R: music from Word RAM and PSG output correct | Mega CD 1 in disc mode; US and European units; CDX; Wondermega; other Mega Drive and Mega CD pairings |
> | Mega Drive 1 + Japanese Mega CD 1, Mode 1: Z80 reads of Word RAM correct (a test program; Echo itself plays from cartridge ROM in Mode 1) | Emulators other than MAME for disc programs |
>
> A program that must follow Sega's documentation uses CD-DA and the PCM chip on the Sub CPU on a disc instead of Echo.

**Mode 1 programs** are cartridges: Echo's data goes in cartridge ROM, as on a Mega Drive. Not in Word RAM: Mode 1 places it at 0x600000, out of Echo's address range (see Data).

**Disc programs** have no ROM, and Main Work RAM is out of the Z80's reach, so Echo's data goes in 2M Word RAM, owned by the Main CPU. The simplest arrangement, which `examples/sound_cd` uses, is a Main CPU module that runs in place from Word RAM (`MODULE_ROM_ORIGIN` 0x200100 and `MMD_DEST` 0; `docs/modules.md`) with the music linked into it: its streams and instruments are then at Word RAM addresses, and `echo_init` and the play calls take them as usual.

While Echo plays, the Main CPU keeps Word RAM. Before handing it to the Sub CPU, pause the music and stop sound effects (`echo_pause_bgm`, `echo_stop_sfx`) and wait a frame so that Echo has taken the commands; resume after Word RAM comes back. No example exercises this handover yet. In 1M mode the Main CPU sees only one bank at a time; Echo in 1M mode has not been tested.

# Sound commands from the Sub CPU

On the Mega CD the game logic often runs on the Sub CPU, which cannot reach the Z80. `sub/sound.h` and `main/sound.h` pass sound commands across: the Sub CPU calls `sound_command(op, arg)` with an operation from `sound.def.h` (play or stop music, pause, resume, play or stop a sound effect, volume), and the Main CPU calls `sound_forward()` once per VBLANK, which passes the command to Echo and acknowledges it. Music and effects are numbered in tables the Main CPU registers with `sound_set_tables`.

The commands use one comm register pair, COMSTAT4 from the Sub CPU and COMCMD4 back, chosen with `SOUND_COMM` (`sound.def.h`). One command is passed per frame: `sound_command` waits until the previous command has been acknowledged and then returns, so a second command in the same frame waits a frame. Nothing else in the kit uses that pair; the Sub CPU crash handler uses COMSTAT5 to COMSTAT7 after a crash.

`examples/sound_cd` uses the path for its controls: the Main CPU passes the buttons to the Sub CPU, which decides what plays. It works the same in the example's Mode 1 build. Verified on the Mega Drive 2 + Mega-CD 2 disc above and on the Mega Drive 1 + Mega CD 1 in Mode 1.

# sound_cd

Echo on the Mega CD: the game logic runs on the Sub CPU and sends sound
commands (`sub/sound.h`); the Main CPU passes them to Echo (`main/sound.h`).
See `docs/sound.md`, "Mega CD programs" and "Sound commands from the Sub CPU".

**The disc build goes against Sega's documentation and against how retail
games did it:** Echo's Z80 reads the music from Word RAM and writes the PSG,
which Sega's Mega CD documents say the Z80 may not do. It was verified on a
Mega Drive 2 with a Japanese Mega-CD 2 only; `docs/sound.md` lists what was
and was not tested.

| Button | Action |
| --- | --- |
| A | Sound effect: beep |
| B | Sound effect: chime |
| C | Stop or play the music |
| Start | Pause or resume the music |

The screen shows what the Sub CPU has the music doing and the last command
word the Main CPU passed on.

## Builds

- `make` builds the disc, `soundcd.iso`. The IP has the Sub CPU load
  `MAIN.MMD` into Word RAM and jumps into it. The module runs in place in
  Word RAM with the song linked into it, so Echo reads its data there. The
  Sub CPU program (`SPX.SMD`) starts the song when the Main CPU has Echo
  running, and turns the buttons the Main CPU passes it (COMCMD2) into sound
  commands.
- `make MODE1=1` builds a Mode 1 cartridge, `soundcd.cart`, with the same
  Sub CPU program. Here the Main CPU program and its music are linked into
  the cartridge: Echo cannot address Word RAM at 0x600000, where Mode 1 puts
  it, and reads cartridge ROM instead.

## Burning the disc

The image is under 400 KB, too short to burn: cdrdao refuses a track under
4 seconds. Pad it first; the zeros after the ISO 9660 volume are ignored:

    truncate -s $((1200*2048)) soundcd.iso

Build with `REGION=JP` (or `EU`) for a console of that region. The disc that
was tested was burned with cdrdao from a TOC file:

    CD_ROM

    TRACK MODE1
    DATAFILE "soundcd.iso"

    cdrdao write --device /dev/sr0 --driver generic-mmc --speed 4 soundcd.toc

## The music

`src/song.s` and the instruments in `res/` are copies of `examples/sound`'s,
which generates them with `tools/gen_song.py`.

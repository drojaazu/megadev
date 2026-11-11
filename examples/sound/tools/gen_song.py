#!/usr/bin/env python3
"""Generate the music and sound effects for examples/sound as Echo streams.

The song is written here as note lists, one part per channel, and turned into
an ESF stream: a run of events (note on, note off, set instrument, set volume)
separated by delays in ticks of 1/60 second. Echo's doc/esf.txt describes every
event used below.

Writes src/song.s: the instrument list, the instruments, the song and two
sound effects, all in .rodata so they land in cartridge ROM, which is where
Echo must find them.

    python3 tools/gen_song.py

The song is an arrangement of "4chords" by the maintainer of Megadev and is
under the same license as the rest of Megadev. The FM instruments, the PSG envelopes and the
drum samples are Echo's own (zlib license, lib/main/echo/LICENSE).
"""

import os
import sys

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "src", "song.s")

# ---------------------------------------------------------------- instruments
#
# Echo numbers instruments by their position in the list passed to echo_init.
# Each entry is (symbol, kind, data): data is a file in res/ to include, or the
# bytes of a PSG envelope (EEF: one volume per tick, $FE loops the next value,
# $FF ends; these two are copied from Echo's tester).

INSTRUMENTS = [
    ("instr_psg_flat",  "EEF", [0xFE, 0x00, 0xFF]),
    ("instr_psg_hihat", "EEF", [0x00, 0x01, 0x02, 0x04, 0x06, 0x08, 0x0C,
                                0xFE, 0x0F, 0xFF]),
    ("instr_bass",      "EIF", "bass.eif"),
    ("instr_square",    "EIF", "square.eif"),
    ("instr_strings",   "EIF", "string.eif"),
    ("instr_piano",     "EIF", "piano.eif"),
    ("instr_kick",      "EWF", "kick.ewf"),
    ("instr_snare",     "EWF", "snare.ewf"),
]
INSTR = {sym[len("instr_"):]: i for i, (sym, _, _) in enumerate(INSTRUMENTS)}

# ------------------------------------------------------------------- channels
#
# The low nibble of most events selects the channel.

FM1, FM2, FM3, FM4, FM5, FM6 = 0x0, 0x1, 0x2, 0x4, 0x5, 0x6
PSG1, PSG2, PSG3, NOISE = 0x8, 0x9, 0xA, 0xB
PCM = 0xC

NOTE_ON, NOTE_OFF, VOLUME, INSTRUMENT = 0x00, 0x10, 0x20, 0x40
LOCK, LOOP_POINT, GO_TO_LOOP, STOP = 0xE0, 0xFD, 0xFC, 0xFF

SEMITONES = {"C": 0, "C#": 1, "D": 2, "D#": 3, "E": 4, "F": 5,
             "F#": 6, "G": 7, "G#": 8, "A": 9, "A#": 10, "B": 11}


def midi(name):
    """'A4' -> MIDI note number (C4 = 60)."""
    return (int(name[-1]) + 1) * 12 + SEMITONES[name[:-1]]


def note_byte(channel, note):
    """The byte after a note on event, for a MIDI note number."""
    octave, semitone = divmod(note - 12, 12)        # octave 4 holds middle C
    if channel <= FM6:
        # FM: 32 * octave + 2 * semitone + 1, octave 0-7 (block 4 = C4)
        if not 0 <= octave <= 7:
            raise ValueError(f"FM note out of range: {note}")
        return 32 * octave + 2 * semitone + 1
    # square PSG: 24 * octave + 2 * semitone, octave 0 = C3
    octave -= 3
    if not 0 <= octave <= 5:
        raise ValueError(f"PSG note out of range: {note}")
    return 24 * octave + 2 * semitone


# ----------------------------------------------------------------- the stream

class Stream:
    """Collects timed events and writes them as ESF with delays between."""

    def __init__(self):
        self.events = []        # (tick, order, bytes)

    def add(self, tick, order, *data):
        self.events.append((tick, order, bytes(data)))

    # order within a tick: note offs, then setup, then note ons
    def off(self, tick, ch):
        self.add(tick, 0, NOTE_OFF | ch)

    def instrument(self, tick, ch, name):
        self.add(tick, 1, INSTRUMENT | ch, INSTR[name])

    def volume(self, tick, ch, vol):
        self.add(tick, 1, VOLUME | ch, vol)

    def on(self, tick, ch, value):
        self.add(tick, 2, NOTE_ON | ch, value)

    def note(self, tick, ch, name):
        self.on(tick, ch, note_byte(ch, midi(name)))

    def marker(self, tick, code):
        self.add(tick, 1, code)

    def encode(self, end_tick, end_code):
        out = bytearray()
        now = 0
        for tick, _, data in sorted(self.events, key=lambda e: (e[0], e[1])):
            out += delay(tick - now)
            now = tick
            out += data
        out += delay(end_tick - now)
        out.append(end_code)
        return bytes(out)


def delay(ticks):
    """Delay events: $Dn waits n+1 ticks (1-16), $FE nn waits nn (0 = 256)."""
    out = bytearray()
    while ticks > 256:
        out += b"\xFE\x00"
        ticks -= 256
    if ticks > 16:
        out += bytes([0xFE, ticks & 0xFF])
    elif ticks > 0:
        out.append(0xD0 + ticks - 1)
    return out


# ------------------------------------------------------------------- the song
#
# "4chords", by the maintainer of Megadev, arranged for Echo from the original
# recording (audiotool.com/track/4chords). C, G, Am, F, one chord a bar, a
# sixteenth note every 7 ticks (about 129 BPM). The parts enter one by one:
#
#   bars  1-4   chords, held                  (played once)
#   bars  5-12  chords gated, drums           (the loop starts here)
#   bars 13-20  bass
#   bars 21-36  lead melody, four times

STEP = 7
BAR = 16 * STEP

# The bass root is written an octave below where it sounds: bass.eif's only
# carrier runs at twice the note frequency (multiplier 2).
CHORDS = {
    #      bass root  chord voicing
    "C":  ("C1", ("C4", "E4", "G4")),
    "G":  ("G1", ("B3", "D4", "G4")),
    "Am": ("A1", ("C4", "E4", "A4")),
    "F":  ("F1", ("C4", "F4", "A4")),
}
CYCLE = ["C", "G", "Am", "F"]

# Lead melody, one list per bar: (note, length in sixteenths); None is a rest.
MELODY = [
    [("E5", 2), ("D5", 2), ("C5", 2), ("A4", 2), ("C5", 1), ("G4", 2), ("A4", 1),
     ("F4", 2), ("E4", 2)],
    [("F4", 4), ("E4", 3), (None, 1), ("E4", 1), ("G4", 1), ("C5", 6)],
    [(None, 2), ("A4", 2), ("G4", 6), (None, 6)],
    [("A4", 3), ("C5", 5), ("D5", 6), ("D#5", 2)],
]

# Sixteenths within a bar
GATE = (0, 3, 6, 9, 12)                 # chord hits, two sixteenths each
KICKS, CLAPS = (0, 3, 6, 9), (4, 12)
HATS = (1, 2, 7, 8, 10, 11, 14)
BASS = (2, 5, 8, 11, 14)

INTRO_BARS, BARS = 4, 32                # 32 bars loop: 5 to 36
BASS_FROM, MELODY_FROM = 8, 16          # loop bars where the parts enter

PAD = (FM3, FM4, FM5)


def song():
    s = Stream()

    # setup: instruments and volumes (FM 0x00 loudest to 0x7F, PSG 0x0 to 0xF)
    s.instrument(0, FM1, "bass")
    s.volume(0, FM1, 0x02)
    s.instrument(0, FM2, "square")
    s.volume(0, FM2, 0x06)
    for ch in PAD:
        s.instrument(0, ch, "strings")
        s.volume(0, ch, 0x10)
    s.instrument(0, NOISE, "psg_hihat")
    s.volume(0, NOISE, 0x05)

    # intro: the chords held, a bar each
    for bar in range(INTRO_BARS):
        t = bar * BAR
        for ch, name in zip(PAD, CHORDS[CYCLE[bar % 4]][1]):
            s.note(t, ch, name)
            s.off(t + BAR - 2, ch)

    start = INTRO_BARS * BAR
    s.marker(start, LOOP_POINT)
    for ch in PAD:                      # the chords turn to gated stabs
        s.instrument(start, ch, "piano")
        s.volume(start, ch, 0x0C)

    for bar in range(BARS):
        t = start + bar * BAR
        root, chord = CHORDS[CYCLE[bar % 4]]

        for step in GATE:
            for ch, name in zip(PAD, chord):
                s.note(t + step * STEP, ch, name)
                s.off(t + (step + 2) * STEP - 2, ch)

        for step in KICKS:
            s.on(t + step * STEP, PCM, INSTR["kick"])
        for step in CLAPS:
            s.on(t + step * STEP, PCM, INSTR["snare"])
        for step in HATS:
            s.on(t + step * STEP, NOISE, 0x04)  # white noise, high

        if bar >= BASS_FROM:
            for step in BASS:
                s.note(t + step * STEP, FM1, root)
                s.off(t + (step + 2) * STEP - 3, FM1)

        if bar >= MELODY_FROM:
            pos = t
            for name, length in MELODY[bar % 4]:
                if name is not None:
                    s.note(pos, FM2, name)
                    s.off(pos + length * STEP - 1, FM2)
                pos += length * STEP
            assert pos == t + BAR, f"melody bar {bar % 4 + 1} is not 16 sixteenths"

    end = start + BARS * BAR
    return s.encode(end, GO_TO_LOOP), end - start


def sfx_beep():
    """Two short PSG tones on PSG3, which the music leaves free."""
    s = Stream()
    s.marker(0, LOCK | PSG3)
    s.instrument(0, PSG3, "psg_flat")
    s.volume(0, PSG3, 0x02)
    s.note(0, PSG3, "C6")
    s.note(3, PSG3, "G6")
    s.off(8, PSG3)
    return s.encode(8, STOP)


def sfx_chime():
    """A rising FM arpeggio on FM5, taken from the music's chords.

    Echo restores the music's instrument and volume on FM5 when the effect ends.
    """
    s = Stream()
    s.marker(0, LOCK | FM5)
    s.instrument(0, FM5, "piano")
    s.volume(0, FM5, 0x00)
    for i, name in enumerate(("E5", "G#5", "B5", "E6")):
        s.note(i * 5, FM5, name)
    s.off(40, FM5)
    return s.encode(40, STOP)


# --------------------------------------------------------------------- output

def bytes_lines(data, indent="\t"):
    for i in range(0, len(data), 16):
        yield indent + ".byte\t" + ", ".join(f"0x{b:02X}" for b in data[i:i + 16])


def main():
    song_data, loop_ticks = song()
    beep = sfx_beep()
    chime = sfx_chime()

    lines = [
        "/**",
        " * Music, sound effects and instruments for examples/sound.",
        " *",
        " * GENERATED by tools/gen_song.py -- do not edit by hand.",
        " *",
        f" * song: {len(song_data)} bytes, loop {loop_ticks} ticks "
        f"({loop_ticks / 60:.1f} s)",
        " */",
        ".section .rodata",
        "",
        "/* Instrument list for echo_init, in the order the streams number them */",
        ".balign 2",
        ".global instrument_list",
        "instrument_list:",
    ]
    for i, (sym, kind, _) in enumerate(INSTRUMENTS):
        lines.append(f"\t.long\t{sym}\t/* {i}: {kind} */")
    lines += ["\t.long\t0", ""]

    for sym, kind, data in INSTRUMENTS:
        lines.append(f"{sym}:")
        if isinstance(data, str):
            lines.append(f'\t.incbin\t"{data}"')
        else:
            lines += bytes_lines(data)
    lines.append("")

    for sym, data in (("song", song_data), ("sfx_beep", beep), ("sfx_chime", chime)):
        lines += [f".global {sym}", f"{sym}:"]
        lines += bytes_lines(data)
        lines.append("")

    with open(OUT, "w") as f:
        f.write("\n".join(lines))
    print(f"{os.path.relpath(OUT)}: song {len(song_data)} bytes, "
          f"loop {loop_ticks / 60:.1f} s; beep {len(beep)}, chime {len(chime)}")


if __name__ == "__main__":
    sys.exit(main())

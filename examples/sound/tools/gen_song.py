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
under the same license as the rest of Megadev. The drum samples and the piano
of the second sound effect are Echo's own (zlib license,
lib/main/echo/LICENSE).
"""

import os
import sys

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "src", "song.s")

# ---------------------------------------------------------------- instruments
#
# Echo numbers instruments by their position in the list passed to echo_init.
# Each entry is (symbol, kind, data): data is a file in res/ to include, or the
# instrument's bytes.
#
# FM instruments (EIF) are the 29 YM2612 register values of one channel, built
# by fm() below from readable operator settings. PSG instruments (EEF) are one
# attenuation per tick (0 loudest, 15 silent); $FE marks the value to hold and
# $FF ends.


def op(mul=1, dt=0, tl=0, ar=31, d1r=0, d2r=0, sl=0, rr=15):
    """One FM operator. dt: 0-3 detune up, 4-7 down. mul 0 is half."""
    return dict(mul=mul, dt=dt, tl=tl, ar=ar, d1r=d1r, d2r=d2r, sl=sl, rr=rr)


def fm(alg, fb, op1, op2, op3, op4):
    """An EIF instrument. Operators are numbered as in Sega's manual (algorithm
    4 is op1 -> op2 plus op3 -> op4); EIF stores them in register order, which
    is op1, op3, op2, op4."""
    ops = (op1, op3, op2, op4)
    assert 0 <= alg <= 7 and 0 <= fb <= 7
    for o in ops:
        assert o["mul"] <= 15 and o["dt"] <= 7 and o["tl"] <= 127
        assert o["ar"] <= 31 and o["d1r"] <= 31 and o["d2r"] <= 31
        assert o["sl"] <= 15 and o["rr"] <= 15
    out = [fb << 3 | alg]
    out += [o["dt"] << 4 | o["mul"] for o in ops]
    out += [o["tl"] for o in ops]
    out += [o["ar"] for o in ops]                   # rate scaling 0
    out += [o["d1r"] for o in ops]                  # amplitude modulation off
    out += [o["d2r"] for o in ops]
    out += [o["sl"] << 4 | o["rr"] for o in ops]
    out += [0, 0, 0, 0]                             # SSG-EG off
    return out


# The patches imitate the original's synths. Two operator pairs (algorithm 4)
# each give a modulator, standing in for a filter, over a carrier.

# Bassline: a square wave and a square an octave down. Op2 at modulator ratio
# 2:1 gives odd harmonics, a square; op4 at multiplier 0.5 is the sub octave.
FM_BASS = fm(4, 0,
             op(mul=2, tl=34, d1r=5, sl=4),
             op(mul=1, tl=0, d1r=2, sl=1, rr=8),
             op(mul=1, tl=40, d1r=5, sl=4),
             op(mul=0, tl=4, d1r=2, sl=1, rr=8))

# Lead (revised in Furnace by the song's author): algorithm 6, a saw (op1
# feedback) driving op2, with op3 and op4 sounding beside it; slow attacks and
# a release that rings past the note.
FM_LEAD = fm(6, 5,
             op(mul=1, tl=30, ar=13, d1r=3, sl=2, rr=6),
             op(mul=1, dt=1, ar=17, d1r=7, sl=3, rr=10),
             op(mul=1, tl=35, ar=14, d1r=3, sl=2, rr=7),
             op(mul=1, dt=5, tl=3, ar=16, d1r=7, sl=3, rr=10))

# Chord synth: oscillator 1 an octave down, oscillator 2 synced to it, through
# a low-pass filter with a short filter envelope. Op4 at multiplier 0.5 is the
# low oscillator; the modulators decay like the filter closing.
FM_CHORD = fm(4, 3,
              op(mul=1, tl=30, d1r=9, d2r=3, sl=6),
              op(mul=1, tl=6, ar=30, d1r=5, d2r=2, sl=3, rr=9),
              op(mul=0, tl=30, d1r=8, d2r=2, sl=5),
              op(mul=0, ar=30, d1r=5, d2r=2, sl=3, rr=9))


def ramp(values, hold):
    return list(values) + [0xFE, hold, 0xFF]


INSTRUMENTS = [
    ("instr_psg_flat",  "EEF", [0xFE, 0x00, 0xFF]),
    # string fill: swell in, settle
    ("instr_psg_pad",   "EEF", ramp([0x0C, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05,
                                     0x05, 0x04, 0x04, 0x03, 0x03, 0x02, 0x02,
                                     0x02, 0x02, 0x03, 0x03, 0x03, 0x03], 0x04)),
    # drums on the noise channel, by decay length
    ("instr_hat_closed", "EEF", ramp([0x00, 0x01, 0x02, 0x04, 0x06, 0x08, 0x0C], 0x0F)),
    ("instr_hat_soft",  "EEF", ramp([0x03, 0x04, 0x06, 0x08, 0x0B, 0x0E], 0x0F)),
    ("instr_hat_open",  "EEF", ramp([0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03,
                                     0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08,
                                     0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E], 0x0F)),
    ("instr_crash",     "EEF", ramp([0x00] * 3 + [0x01] * 4 + [0x02] * 4 +
                                    [0x03] * 5 + [0x04] * 5 + [0x05] * 6 +
                                    [0x06] * 6 + [0x07] * 6 + [0x08] * 6 +
                                    [0x09] * 6 + [0x0A] * 6 + [0x0B] * 6 +
                                    [0x0C] * 6 + [0x0D] * 6 + [0x0E] * 6, 0x0F)),
    ("instr_bass",      "EIF", FM_BASS),
    ("instr_lead",      "EIF", FM_LEAD),
    ("instr_chord",     "EIF", FM_CHORD),
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
# Audiotool project (audiotool.com/track/4chords): notes and drum patterns as
# written there, synths approximated with FM and PSG. 130 BPM, a sixteenth note
# every 7 ticks. C, G, Am, F, a bar each:
#
#   bars  1-4   chord synth; the drums come in during bar 4   (played once)
#   bars  5-36  drums, string fill from bar 9, bass from 13,  (the loop)
#               lead from 21
#
# Channels: FM1 bass, FM2 lead, FM3-5 chords, FM6 PCM drums, PSG1-2 string
# fill, PSG3 free for sound effects, noise for hats and crash.

STEP = 7
BAR = 16 * STEP

# Note lists: (sixteenth in the bar, length in sixteenths, note)

# Chord synth, one bar per chord: a chopped pattern over three voices
def chord_bar(lo, mid, hi):
    return [(0, 2, hi), (0, 4, lo), (0, 4, mid),
            (4, 2, mid), (4, 4, lo), (4, 4, hi),
            (6, 2, mid),
            (8, 4, lo),
            (10, 2, mid), (10, 2, hi),
            (12, 4, lo),
            (14, 2, mid), (14, 2, hi)]

CHORDS = [chord_bar("C4", "E4", "G4"),              # C
          chord_bar("G4", "B4", "D5"),              # G
          chord_bar("A4", "C5", "E5"),              # Am
          chord_bar("F4", "A4", "C5")]              # F

BASS = [[(0, 8, "C2"), (8, 8, "E2")],
        [(0, 14, "G2"), (14, 2, "E2")],
        [(0, 4, "A2"), (4, 4, "C3"), (8, 4, "E2"), (12, 4, "A2")],
        [(0, 16, "F2")]]

LEAD = [[(0, 2, "E5"), (2, 2, "D5"), (4, 2, "C5"), (6, 2, "A4"),
         (8, 2, "G4"), (10, 2, "E4"), (12, 2, "F4"), (14, 2, "G4")],
        [(4, 2, "E4"), (10, 4, "C5"), (14, 2, "C4")],
        [(2, 2, "A4"), (4, 4, "G4")],
        [(0, 2, "A4"), (2, 1, "A4"), (3, 1, "C5"), (5, 2, "C5"),
         (8, 4, "D5"), (13, 1, "D5"), (14, 2, "D#5")]]

# String fill, four bars from its start: G for a bar, D for five beats, C for
# four. Played on two PSG channels, the second an octave down.
STRING_FILL = [(0, 16, "G5"), (16, 20, "D6"), (36, 16, "C6")]

# Drum patterns from the original's drum machine, one character per sixteenth
DRUMS = {
    "A1": dict(kick="xx.xx.xx.xx.xx..", snare="....x.......x...",
               closed=".xx.xx.xx.xx.x..", soft="..x...x...x...x.",
               open="..............x.", crash="................"),
    "A2": dict(kick="xx.xx.xx.x.x.xxx", snare="x...x...xx.x.xxx",
               closed=".xx.xx.xx.xx.x..", soft="..x...x...x...x.",
               open="................", crash="...............x"),
    "A3": dict(kick="xx..xxx.xxx..xx.", snare="x...xxx.xxx..xx.",
               closed=".xx.xx..x.x.x.x.", soft="..x...x...x...x.",
               open="................", crash="...............x"),
    "A4": dict(kick="........x.x.x.x.", snare="................",
               closed="................", soft="................",
               open="................", crash="..............x."),
}

INTRO_BARS = 4
LOOP_BARS = 32                          # bars 5 to 36


def drum_pattern(bar):
    """Pattern for a song bar (1-based), as arranged in the original."""
    if bar == 4:
        return "A4"
    if bar == 36:
        return "A3"
    if bar in (12, 20, 28):
        return "A2"
    return "A1" if bar > 4 else None


CHORD_VOICES = (FM3, FM4, FM5)


def chords(s, t, notes, busy):
    """Give each note the first chord voice free at its start."""
    for step, length, name in sorted(notes):
        on = t + step * STEP
        for ch in CHORD_VOICES:
            if busy[ch] <= on:
                break
        else:
            raise ValueError("more than three chord notes at once")
        off = on + length * STEP - 1
        busy[ch] = off + 1
        s.note(on, ch, name)
        s.off(off, ch)


def mono(s, t, ch, notes, gap=1):
    for step, length, name in notes:
        on = t + step * STEP
        s.note(on, ch, name)
        s.off(on + length * STEP - gap, ch)


def drums(s, t, name):
    p = DRUMS[name]
    for i in range(16):
        on = t + i * STEP
        # one PCM channel: the snare wins, and a kick right after a snare
        # is dropped so the snare can ring
        if p["snare"][i] == "x":
            s.on(on, PCM, INSTR["snare"])
        elif p["kick"][i] == "x" and not (i and p["snare"][i - 1] == "x"):
            s.on(on, PCM, INSTR["kick"])
        # one noise channel: the longest sound wins
        for part, instr, noise in (("crash", "crash", 0x05),
                                   ("open", "hat_open", 0x04),
                                   ("soft", "hat_soft", 0x04),
                                   ("closed", "hat_closed", 0x04)):
            if p[part][i] == "x":
                s.instrument(on, NOISE, instr)
                s.on(on, NOISE, noise)
                break


def song():
    s = Stream()

    # setup: instruments and volumes (FM 0x00 loudest to 0x7F, PSG 0x0 to 0xF)
    s.instrument(0, FM1, "bass")
    s.volume(0, FM1, 0x02)
    s.instrument(0, FM2, "lead")
    s.volume(0, FM2, 0x08)
    for ch in CHORD_VOICES:
        s.instrument(0, ch, "chord")
        s.volume(0, ch, 0x12)
    for ch in (PSG1, PSG2):
        s.instrument(0, ch, "psg_pad")
    s.volume(0, PSG1, 0x01)
    s.volume(0, PSG2, 0x02)
    s.volume(0, NOISE, 0x02)

    busy = {ch: 0 for ch in CHORD_VOICES}
    for bar in range(1, INTRO_BARS + LOOP_BARS + 1):
        t = (bar - 1) * BAR
        if bar == INTRO_BARS + 1:
            s.marker(t, LOOP_POINT)
            busy = {ch: t for ch in CHORD_VOICES}
        cycle = (bar - 1) % 4

        chords(s, t, CHORDS[cycle], busy)
        if drum_pattern(bar):
            drums(s, t, drum_pattern(bar))
        if bar >= 13:
            mono(s, t, FM1, BASS[cycle], gap=2)
        if bar >= 21:
            mono(s, t, FM2, LEAD[cycle])
        if bar >= 9 and (bar - 9) % 8 == 0:
            for step, length, name in STRING_FILL:
                on = t + step * STEP
                s.note(on, PSG1, name)
                s.note(on, PSG2, name[:-1] + str(int(name[-1]) - 1))
                for ch in (PSG1, PSG2):
                    s.off(on + length * STEP - 1, ch)

    start = INTRO_BARS * BAR
    end = (INTRO_BARS + LOOP_BARS) * BAR
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

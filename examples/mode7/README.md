# Pseudo-3D ground plane

A textured plane drawn in perspective using the gate array's rotation and
scaling hardware, with a camera that can be driven around it.

    D-pad up/down     move forwards and backwards
    D-pad left/right  turn
    A + left/right    strafe
    C                 reset the camera

## How it works

`docs/rotation_scaling.md` section 7 derives the maths; this is the
implementation of it. Per output row the Sub CPU works out how far away that
row is, walks that far forward from the camera to find the middle of the row,
then steps half a screen width sideways to find where the row starts. The
per-pixel step is that same sideways direction, sized by the row's distance --
which is what makes distant ground look compressed.

Three things keep it inside the hardware's limits and the 68000's abilities:

- **No division at runtime.** Distance is `K / d`, and `d` depends only on the
  screen row, so `tools/gen_tables.py` precomputes it. The generator also
  *checks the result*: the trace vector step format has four integer bits, so a
  step of 16.0 or more overflows into the sign bit. The current settings peak at
  6.09, about 38% of the ceiling. Sonic CD's own special stage runs to 14.6.
- **No trigonometry at runtime.** One sine table serves sine and cosine, read a
  quarter turn apart.
- **No 32-bit multiplies.** The 68000 multiplies 16x16 into 32, which is exactly
  what these fixed point products need, but GCC does not reliably select it at
  -O1 and falls back to `__mulsi3` in the innermost loop. `mul16()` states the
  instruction directly.

The trace vector table is built in Program RAM and block-copied into Word RAM in
one burst rather than written there entry by entry. Every Sub CPU access to Word
RAM during a graphics operation costs the operation 3 clocks, so scattered
writes are a real cost -- see `docs/rotation_scaling.md` section 6.

## Regenerating the tables

    python3 tools/gen_tables.py

Edit the constants at the top of that script to change the field of view, the
camera height, or how close to the horizon the plane is drawn. It refuses to
generate a table that would exceed the step ceiling.

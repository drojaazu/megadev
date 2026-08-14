# Rotation and Scaling

The Mega CD gate array contains a graphics unit that reads a large source image and writes a
transformed rectangle into an output buffer. Sega's documentation calls its use a *graphics
operation*; it is also referred to as the rotation/scaling hardware, the stamp hardware, or by
analogy with the SNES, "Mode 7".

Everything described here lives on the **Sub CPU side**, operates on **Word RAM**, and requires
**2M mode**.

---

## 1. What the hardware actually does

It is easy to assume the unit rotates or scales a bitmap. It does not. The operation it performs is:

> For each output line, walk a straight line across the source image, sampling one source pixel per
> output pixel.

The caller supplies, per output line, a **starting point** in the source image and a **step vector**.
The hardware repeats "sample, add step" across the line, then moves to the next line and reads a
fresh start point and step from a table.

Rotation, scaling, skewing and perspective are all consequences of what those start points and steps
are set to. The hardware has no notion of an angle or a zoom factor.

> **Why this matters.** A unit that only rotated bitmaps would be nearly useless for a ground plane,
> because a perspective view is not a rotation — nearer rows of the screen must sample the source
> more finely than distant rows. Because the step is supplied *per line*, each line can sample at a
> different rate, which is exactly what perspective needs. See [§7](#7-pseudo-3d-ground-planes).

---

## 2. The four data structures

| Structure | Where | What it is |
|---|---|---|
| Stamps | Word RAM | Square tiles of pixel data, 16×16 or 32×32 |
| Stamp map | Word RAM | Grid of 16-bit entries naming a stamp per cell |
| Trace vector table | Word RAM | Four words per output line: start point and step |
| Image buffer | Word RAM | Where output pixels are written |

All four are in Word RAM, and the unit reads and writes Word RAM while it runs. This is the root of
the timing behaviour in [§6](#6-timing-and-throughput).

### 2.1 Stamps

A stamp is a square tile in standard Mega Drive 4-bits-per-pixel pattern format: two dots per byte,
high nibble first. Stamps are laid out exactly like Mega Drive sprite patterns — cells run **down**
first, then across.

Stamps are 16×16 or 32×32 dots, selected by `GA_STAMPSIZE` bit `STS`, and the choice applies to
every stamp. Stamp storage begins at the start of 2M Word RAM (`0x080000` as the Sub CPU sees it).

| Stamp size | Bytes per stamp | Generator slots consumed |
|---|---|---|
| 16×16 | 0x80 | 1 |
| 32×32 | 0x200 | 4 |

With 32×32 stamps the low two bits of a stamp number must be zero, because each stamp occupies four
consecutive 16×16 slots.

### 2.2 Stamp map

The stamp map is a grid of 16-bit entries, one per stamp-sized cell of the source image:

| Bits | Field | Meaning |
|---|---|---|
| 15 | `HFLIP` | Mirror this stamp horizontally |
| 14–13 | `ROTATE` | 0°, 90°, 180°, 270° |
| 12–11 | — | Must be 0 |
| 10–0 | `SNO` | Stamp number |

`HFLIP` and `ROTATE` combine, giving eight orientations per stamp; the manual tabulates all eight
using the letter P as the test glyph (p.42).

**The rotation is applied first, then the horizontal flip.** In the manual's figure each cell of the
combined row is the exact horizontal mirror of the corresponding rotate-only cell, which is what
`HFLIP(ROTATE(stamp))` produces; applying the flip first would leave the 90° and 270° cells differing
from that mirror by 180°.

**Rotation is counter-clockwise**, and the two rotation bits are not ordered the way their names
suggest. The field occupies bits 14 and 13, named `RT1` and `RT0`, so the value you read out of it is
`RT1 × 2 + RT0` — but the manual's orientation figure is indexed by `RT0 × 2 + RT1`:

| `RT0` | `RT1` | Field value | Rotation |
|---|---|---|---|
| 0 | 0 | `0b00` | 0° |
| 0 | 1 | `0b10` | 90° counter-clockwise |
| 1 | 0 | `0b01` | 180° |
| 1 | 1 | `0b11` | 270° counter-clockwise |

Use `STAMP_ROTATE_90` and friends rather than writing the field by hand; 90° and 180° are the two
that catch people, because the field value is the reverse of what the bit names imply.

Its dimensions come from `GA_STAMPSIZE` bit `SMS`: either one screen (256×256 dots) or sixteen
screens square (4096×4096 dots). Combined with the stamp size, this fixes how much Word RAM the map
occupies and therefore how coarsely `GA_REG_STAMPMAPBASE` must be aligned:

| Stamps | Map | Map size | Base address alignment |
|---|---|---|---|
| 16×16 | 256×256 | 0x200 | 0x200 |
| 32×32 | 256×256 | 0x80 | 0x80 |
| 16×16 | 4096×4096 | 0x20000 | 0x20000 |
| 32×32 | 4096×4096 | 0x8000 | 0x8000 |

Bit `RPT` decides what happens when sampling leaves the map: either the map repeats indefinitely, or
everything outside it reads as pixel value 0.

### 2.3 Trace vector table

Four words per output line, at the address written to `GA_REG_TRACEVECTBASE`:

| Word | Contents | Format |
|---|---|---|
| 0 | X start | 13 integer bits, 3 fractional |
| 1 | Y start | 13 integer bits, 3 fractional |
| 2 | X step per output pixel | sign bit, 4 integer bits, 11 fractional |
| 3 | Y step per output pixel | sign bit, 4 integer bits, 11 fractional |

The table must hold one entry per output line, so it is `Vdot size × 8` bytes.

**The steps are ordinary signed two's complement**, so normal arithmetic on them is correct and a
negative step is just a negative number.

> The manual's bit table (p.39) draws bit 15 of the step as a separate `+/-` cell, which reads as
> sign-and-magnitude. That reading is wrong — it was adopted here once and disproved by tracing a
> shipped game (SPEC.md KB-41, withdrawn; HW-10). A bit table drawn with a separate sign cell does
> not imply sign-and-magnitude.

The formats impose hard limits:

- A start coordinate is 13 integer bits, so the addressable source area is 8192×8192 dots, with 1/8
  dot of sub-pixel precision.
- A step is 4 integer bits plus 11 fractional, so its magnitude must be **under 16 source dots per
  output pixel**, with a resolution of 1/2048. Anything requiring a larger step cannot be expressed.

> **What the numbers mean.** A step of 1.0 samples the source at the same rate as the output: no
> scaling. A step of 0.5 samples half a source pixel per output pixel, so the source appears at
> double size. A step of 4.0 skips three source pixels out of four, shrinking the source to a
> quarter. The 16.0 ceiling is therefore a limit on how far you can *shrink* — which is exactly what
> a distant horizon needs, and is the first wall you hit building a ground plane.

### 2.4 Image buffer

The destination. Its dimensions come from four registers, and the distinction between them is the
part most worth being careful about:

| Register | Unit | Meaning |
|---|---|---|
| `GA_REG_IMGBUFSTART` | — | Base address in Word RAM |
| `GA_REG_IMGBUFVSIZE` | cells | Height of the buffer as **allocated** |
| `GA_REG_IMGBUFVDOTSIZE` | dots | Height actually **written** |
| `GA_REG_IMGBUFHDOTSIZE` | dots | Width actually written |
| `GA_REG_IMGBUFOFFSET` | line, dot | Where writing starts inside the first cell |

The buffer is allocated as `Vcell × Hdot`, but only a `Vdot × Hdot` region is written.

Sega's own conceptual figure draws the buffer as a **cylinder**, and that is the useful mental
model: the allocated height is the circumference, and the written band wraps around it. Writing past
the bottom continues at the top.

> **Why a cylinder is useful.** For a scrolling view you do not want to re-render the whole buffer
> each frame. You render only the newly exposed band and advance the offset, letting the rest of the
> buffer stay as it is — the same trick as a scrolling tilemap, one dimension at a time.

Output cells are numbered **down first, then across**, matching Mega Drive sprite layout. Within an
operation the hardware generates data one output line at a time, writing that line across every cell
in the row before moving down.

Note the asymmetry: `Vcell` is in cells, `Vdot` and `Hdot` are in dots, and `Vcell` is stored as
**one less than** the number of cells.

---

## 3. Register summary

| Register | Address | Role |
|---|---|---|
| `GA_REG_GFXSTAT` | `$FF8058` | `GRON`: 1 while an operation is running |
| `GA_REG_STAMPSIZE` | `$FF8059` | `SMS`, `STS`, `RPT` |
| `GA_REG_STAMPMAPBASE` | `$FF805A` | Stamp map address |
| `GA_REG_IMGBUFVSIZE` | `$FF805D` | Buffer height in cells, minus one |
| `GA_REG_IMGBUFSTART` | `$FF805E` | Image buffer address |
| `GA_REG_IMGBUFOFFSET` | `$FF8061` | Start line and dot within the first cell |
| `GA_REG_IMGBUFHDOTSIZE` | `$FF8062` | Width in dots |
| `GA_REG_IMGBUFVDOTSIZE` | `$FF8064` | Height in dots — **consumed by the operation** |
| `GA_REG_TRACEVECTBASE` | `$FF8066` | Trace vector table address — **starts the operation** |

Addresses written to `GA_REG_STAMPMAPBASE`, `GA_REG_IMGBUFSTART` and `GA_REG_TRACEVECTBASE` are Word
RAM offsets **divided by 4**.

Two of these registers do something beyond storing a value, and both are easy to be caught by:

- **`GA_REG_TRACEVECTBASE` is the trigger.** Writing it starts the operation. Every other register
  must already hold its final value.
- **`GA_REG_IMGBUFVDOTSIZE` is destroyed by the operation.** The hardware decrements it as it runs
  and it reads 0 on completion. It must be rewritten before *every* operation. Setting it once and
  reusing it performs a zero-height operation the second time — silently.

---

## 4. Performing an operation

```
1. Acquire Word RAM in 2M mode.
2. Write the stamps, stamp map and trace vector table into Word RAM.
3. Set GA_REG_STAMPSIZE, GA_REG_STAMPMAPBASE.
4. Set GA_REG_IMGBUFSTART, GA_REG_IMGBUFVSIZE, GA_REG_IMGBUFOFFSET,
       GA_REG_IMGBUFHDOTSIZE, GA_REG_IMGBUFVDOTSIZE.
5. Write GA_REG_TRACEVECTBASE.            <- starts here
6. Wait for completion.
7. Hand Word RAM to the Main CPU, which DMAs the image buffer to VRAM.
```

Completion is signalled two ways: `GRON` in `GA_REG_GFXSTAT` returns to 0, and a level 1 interrupt is
raised. Polling `GRON` is simpler; the interrupt lets the Sub CPU do other work meanwhile, but see
the timing note below before assuming that is free.

```c
ga_reg_tracevectbase = TRACE_TABLE_OFFSET / 4;

while (ga_reg_gfxstat & GA_GRON_MASK)
    ;
```

---

## 5. Priority mode

`GA_PRIORITY` in `GA_REG_MEMMODE` governs which pixels a write into Word RAM may change, which makes
it a way to composite without a per-pixel test:

| Value | Effect |
|---|---|
| `GA_PRIORITY_OFF` | Every pixel is written |
| `GA_PRIORITY_UNDERWRITE` | Written only where the destination is 0 |
| `GA_PRIORITY_OVERWRITE` | Only non-zero source pixels are written |

`GA_PRIORITY_OVERWRITE` is transparency: source pixel 0 leaves the destination alone.
`GA_PRIORITY_UNDERWRITE` draws behind what is already there. The fourth encoding is prohibited.

**Which writes it applies to depends on the Word RAM mode**, and this is easy to get backwards:

| Word RAM mode | Priority applies to |
|---|---|
| 2M | Data written to the image buffer **by a graphics operation**. A CPU write is unaffected — Word RAM behaves as ordinary RAM. |
| 1M | Data written to a dot image area, CPU writes included. |

So in 2M mode you cannot use priority to composite sprites by hand with the CPU; it governs only what
the graphics unit lays down. Compositing by CPU write needs 1M mode.

Priority operates per pixel, so within a single word one nibble may be written and its neighbour
left alone (manual p.54).

---

## 6. Timing and throughput

One gate array clock is approximately 80 ns. The manual gives a cost model whose important term is
not the base cost but the interference:

- **Every Sub CPU access to Word RAM during an operation costs 3 clocks.**
- **Every DMA access to Word RAM during an operation costs 3 clocks.**
- A refresh penalty is added proportional to the total.

The base cost of an operation scales with `Vdot size` and the width.

> **The practical consequence.** The unit and the Sub CPU contend for the same memory. Code that
> polls `GRON` in a tight loop is reading Word RAM's neighbourhood constantly and is competing with
> the very operation it is waiting for. Worse is a design where the Sub CPU builds the trace vector
> table for the *next* frame while the current operation runs — that is a Word RAM write per table
> entry, each costing 3 clocks of the running operation.
>
> If a transform-based effect is unexpectedly slow, the first thing to examine is not the trace
> vector maths but how much the Sub CPU touches Word RAM while `GRON` is set.

Two structural approaches follow from this:

1. **Do CPU work outside Word RAM.** Build the trace vector table in Program RAM, then block-copy it
   into Word RAM before triggering. One burst of contention instead of a scattered stream.
2. **Use 1M mode to overlap.** In 1M/1M mode each CPU owns a separate half of Word RAM and they swap.
   The Sub CPU renders into its half while the Main CPU DMAs the other half to VRAM, with no
   contention and no waiting for a handover mid-frame.

---

## 7. Pseudo-3D ground planes

The most common use of this hardware is a textured ground plane drawn in perspective — the Sonic CD
special stage being the best-known example. This section derives the trace vector table for that
case.

### 7.1 The geometry

Set up a camera at height `h` above a flat plane, looking horizontally. For a screen row at distance
`d` **below the horizon line**, the view ray meets the ground at a distance

```
z = (h * f) / d
```

where `f` is a focal-length constant that controls the field of view. The only thing that matters
about this formula is its shape: **distance is proportional to one over the row's distance below the
horizon.** Rows just under the horizon are enormously far away; rows at the bottom of the screen are
close.

Because the ground row at distance `z` is `z` times wider in world terms than one at distance 1, the
source must be sampled `z` times more coarsely:

```
step = z * k
```

for a constant `k` fixing the horizontal scale.

With the camera at world position `(cx, cy)` and facing angle `θ`:

```
forward = ( sin θ,  cos θ )
right   = ( cos θ, -sin θ )
```

and for each screen row, with screen width `W`:

```
dX = step * right.x
dY = step * right.y

startX = cx + z * forward.x - (W/2) * dX
startY = cy + z * forward.y - (W/2) * dY
```

That is the entire trace vector entry. The correspondence is exact: `startX`, `startY`, `dX`, `dY`
are the four words of the table.

> **In words.** For one row of the screen: work out how far away that row is, walk that far forward
> from the camera to find the middle of the row, then step half a screen width to the left to find
> where the row starts. The step you take per pixel is the same direction you just walked sideways
> in, sized by how far away the row is. Distant rows take big steps across the texture, which is why
> distant ground looks compressed.

### 7.2 Making it work in fixed point

Three problems appear at once, and all three are consequences of the formats in
[§2.3](#23-trace-vector-table).

**The horizon is infinity.** As `d` approaches 0, `z` and `step` approach infinity. But `step` cannot
exceed 16.0. The fix is to start drawing some rows below the horizon: pick the first row such that
`step` is within range, and leave the rows above it to a background layer. This is why ground-plane
effects always have something — sky, haze, a distant object band — occupying the top of the screen.

**Division is expensive.** `z = h*f/d` needs a divide per row, and the 68000's `divu` is slow. Since
`d` only takes one value per screen row, precompute a table of `z` (or of `1/d`) indexed by screen
row. It never changes unless the camera height or field of view changes.

**Trigonometry is expensive.** `sin θ` and `cos θ` come from a table indexed by angle. One table
serves both if it is a full period and cosine is read at a quarter-period offset.

With those three tables, the per-row work reduces to a handful of multiplies and adds, with no
division and no trigonometry in the loop.

**Watch the intermediate width.** `z * right.x` multiplies two fixed-point numbers. On the 68000,
`muls` gives a 32-bit product from two 16-bit operands, so the product of two Q*n* values is Q*2n*
and must be shifted back down. Getting this wrong is the most common source of a ground plane that
renders but warps incorrectly — see `lib/fixed.h`, which defines the shift for each format so the
conversion is not open-coded at each site.

**Wrapping is free.** Start coordinates are unsigned and the map repeats when `RPT` is set, so a
camera position that runs off the edge of the texture wraps rather than needing a bounds test.

### 7.3 Suggested structure

```
Per frame:
  Sub CPU:  for each visible row:
                z    = z_table[row]                 (precomputed, no divide)
                step = z * k
                look up sin/cos for the camera angle
                write startX, startY, dX, dY        (into Program RAM)
            block-copy the table into Word RAM
            trigger the operation
            wait for GRON
            hand Word RAM to the Main CPU
  Main CPU: DMA the image buffer to VRAM
```

Building the table in Program RAM and copying it in one burst is the [§6](#6-timing-and-throughput)
point applied: it keeps the Sub CPU out of Word RAM while the operation runs.

---

## 8. Laying out Word RAM

All four structures live in the same 256 KB of Word RAM, and the graphics unit reads three of them
while writing the fourth. Nothing separates them but the base addresses you choose, so the layout is
a budget you have to plan rather than something the hardware arranges.

The Sub CPU sees 2M Word RAM at `0x080000`–`0x0BFFFF`. Sega's own worked examples (manual pp. 43–47)
all follow the same order, lowest address first:

```
0x080000  stamp generator data   (the stamp pixels)
          trace vector table     Vdot × 8 bytes
          image buffer           Vcell × Hcell × 0x20 bytes
0x0BFFFF  stamp map table        sized by the table in §2.2
```

### 8.1 Stamp number 0 is reserved

**Stamp 0 cannot be used as a stamp.** In the stamp map, entry 0 means *no character* — the cell
reads as pixel value 0 rather than as the stamp stored at slot 0.

The storage is not wasted, though. The manual is explicit that the area "operates as ordinary RAM"
and that the graphics circuit neither reads nor writes it, so slot 0 is free space for anything else
you need — a good home for the trace vector table you are about to copy in, or for the Sub CPU's own
working data.

The size of slot 0 is one stamp: `0x80` bytes for 16×16, `0x200` for 32×32.

### 8.2 How much is left for stamps

Everything not spent on the other three structures is available for stamp pixels, and the stamp map
is by far the largest consumer. Working from the map sizes in [§2.2](#22-stamp-map):

| Stamps | Map | Map costs | Stamp slots available | Distinct stamps addressable |
|---|---|---|---|---|
| 16×16 | 256×256 (1 screen) | 0x200 | limited by space, not numbering | 2048 (`SNO` is 11 bits) |
| 32×32 | 256×256 (1 screen) | 0x80 | limited by space, not numbering | 512 (`SNO` bits 10–2) |
| 16×16 | 4096×4096 (16 screen) | 0x20000 | **half of Word RAM is gone** | 2048 |
| 32×32 | 4096×4096 (16 screen) | 0x8000 | comfortable | 512 |

The 16-screen map with 16×16 stamps is the case worth thinking twice about: the map alone consumes
`0x20000` bytes — half the available Word RAM — before a single stamp, the trace vector table or the
image buffer is placed. Sega's note on the 16-screen layouts is blunt about this: the map may use any
of the eight banks, "however, in practice, some space should be left free for defining stamp data"
(p.49).

The one-screen modes are the opposite: the map is `0x200` or `0x80` bytes and can sit almost
anywhere, which is why the base address may be any of 512 or 2048 aligned slots (pp. 50–51).

### 8.3 What the numbers are counted in

Three different units appear in these registers and mixing them is the most common layout mistake:

| Quantity | Unit | Note |
|---|---|---|
| Trace vector table size | bytes | `Vdot × 8` — one 4-word entry per output **line** |
| Image buffer size | bytes | `Vcell × Hcell × 0x20` — a cell is 8×8 dots at 4bpp = `0x20` bytes |
| `GA_REG_IMGBUFVSIZE` | cells | stored as **one less** than the cell count |
| `GA_REG_IMGBUFVDOTSIZE`, `GA_REG_IMGBUFHDOTSIZE` | dots | the region actually written |
| Base address registers | Word RAM offset ÷ 4 | all three of them |

Note that the buffer is *allocated* by cell counts but *written* by dot counts. The manual states the
consequence directly: "Buffer size is determined by Vcell size and Hdot size, but the size of the
actual data area used in practice is determined by Vdot size and Hdot size" (p.53).

---

## 9. Offset, and the cells that do not get written

`GA_REG_IMGBUFOFFSET` shifts where writing begins inside the first cell, by a whole line and a whole
dot. It exists so a rendered band can be positioned without moving the buffer, which is what makes
the cylinder model in [§2.4](#24-image-buffer) usable for scrolling.

It carries a trap that is worth stating plainly, because the symptom looks like a maths error.

> **When you use a non-zero offset, `Vcell` size must be set larger than the actual cell size.**
>
> Shifting the written band by a dot pushes data out of one cell and into its neighbour. Sega's
> figure (p.54) shows the displaced pixels arriving in the adjacent cell, and the pixels they
> displace being left unchanged *regardless of the priority setting*. If the buffer is exactly as
> tall as the region being written, that overspill has nowhere to go.
>
> The exception: if `Hdot` size is 8 or less, there is no problem.

The related property is that a graphics operation does **not** write every byte of the buffer it was
given. The written region is `Vdot × Hdot`; the allocation is `Vcell × Hcell` cells. Where `Vdot` is
not a multiple of 8, whole bands of the buffer are never touched by the operation and retain whatever
was there before — old frame data, or uninitialised Word RAM.

If a transform renders correctly but is framed by garbage, this is the reason, and the fix is either
to size the buffer to match or to clear it once before first use.

Data is generated one output line at a time, across every cell of a row before moving down — the same
down-first cell order as Mega Drive sprite patterns. The operation completes, and the level 1
interrupt fires, when the last dot of the last cell is written (pp. 41, 53).

---

## 10. Common failure modes

| Symptom | Likely cause |
|---|---|
| Operation appears to do nothing the second time | `GA_REG_IMGBUFVDOTSIZE` not rewritten; it is consumed |
| Output is torn or partly stale | Word RAM handed over before `GRON` cleared |
| Wait loop returns immediately | Polling a byte register with a word mask; `GRON` is bit 7 of `GA_REG_GFXSTAT` |
| Ground plane warps near the horizon | Step exceeded 16.0 and wrapped; clamp the first drawn row |
| Everything is mirrored or inverted unexpectedly | Check the sign of the step, and that `HFLIP`/`ROTATE` in the stamp map entries are what you meant |
| Nothing renders at all | `GA_REG_TRACEVECTBASE` written before the other registers were set |
| Correct output, very slow | Sub CPU touching Word RAM during the operation |
| Rendered area is framed by garbage | Buffer allocated `Vcell × Hcell` but only `Vdot × Hdot` written; clear it once, or size it to match ([§9](#9-offset-and-the-cells-that-do-not-get-written)) |
| Smearing into the next cell after changing offset | `Vcell` size not set larger than the actual cell size ([§9](#9-offset-and-the-cells-that-do-not-get-written)) |
| A cell renders empty where a stamp was expected | Stamp map entry is 0, which means *no character*, not "stamp 0" ([§8.1](#81-stamp-number-0-is-reserved)) |
| CPU-drawn sprite ignores priority | In 2M mode priority governs only graphics-operation writes ([§5](#5-priority-mode)) |

---

## References

- Register-level detail: `lib/sub/gate_arr.def.h`, and [gate_array.md](gate_array.md)
- Fixed-point helpers: `lib/fixed.h`
- Worked examples: `examples/transforms`, `examples/mode7`
- *Mega-CD Hardware Manual — The Hardware*, Ver 1.0, §3-8 (printed pages 35–54). Sections 8 and 9
  above are drawn from the explanatory material on printed pages 40–54 specifically: the coordinate
  bit layout (p.40), the pipeline figure (p.41), stamp data organisation (p.42), the worked Word RAM
  layouts (pp. 43–47), stamp map base addressing (pp. 48–51), the image buffer concept and map
  (pp. 52–53), and offset and priority (p.54). Printed page 45 is a blank page.

# Rotation and Scaling

The Mega CD gate array contains a graphics unit that reads a large source image and writes a transformed rectangle into an output buffer. Sega's documentation calls one run of it a *graphics operation*; it is also known as the rotation and scaling hardware, the stamp hardware, or by analogy with the SNES, "Mode 7".

Everything described here is on the Sub CPU side, operates on Word RAM, and requires 2M mode.

# Contents

- What the hardware does
- The four data structures
- Register summary
- Performing an operation
- Priority mode
- Timing and throughput
- Pseudo-3D ground planes
- Laying out Word RAM
- Offset, and the cells that do not get written
- Common failure modes

---

# What the hardware does

The unit does not rotate or scale a bitmap as such. For each output line it walks a straight line across the source image, sampling one source pixel per output pixel. The caller supplies, per output line, a starting point in the source image and a step vector; the hardware repeats "sample, add step" across the line, then reads a fresh start point and step for the next line from a table.

Rotation, scaling, skewing and perspective are all consequences of what those start points and steps are. The hardware has no notion of an angle or a zoom factor. Because the step is supplied per line, each line can sample the source at a different rate, which is what a perspective ground plane needs.

# The four data structures

| Structure | Where | What it is |
|---|---|---|
| Stamps | Word RAM | Square tiles of pixel data, 16x16 or 32x32 |
| Stamp map | Word RAM | Grid of 16-bit entries naming a stamp per cell |
| Trace vector table | Word RAM | Four words per output line: start point and step |
| Image buffer | Word RAM | Where output pixels are written |

All four are in Word RAM, and the unit reads and writes Word RAM while it runs. This is the root of the timing behavior described later.

## Stamps

A stamp is a square tile in standard Mega Drive 4-bits-per-pixel pattern format: two dots per byte, high nibble first. Cells run down first, then across, as in Mega Drive sprite patterns.

Stamps are 16x16 or 32x32 dots, selected by the STS flag of `GA_SUB_REG_STAMPSIZE`, and the choice applies to every stamp. Stamp storage begins at the start of 2M Word RAM, `0x080000` as the Sub CPU sees it.

| Stamp size | Bytes per stamp | 16x16 slots consumed |
|---|---|---|
| 16x16 | 0x80 | 1 |
| 32x32 | 0x200 | 4 |

With 32x32 stamps the low two bits of a stamp number must be zero, since each stamp occupies four consecutive 16x16 slots.

## Stamp map

The stamp map is a grid of 16-bit entries, one per stamp-sized cell of the source image. The fields are defined in `sub/gate_arr.def.h`:

| Bits | Field | Meaning |
|---|---|---|
| 15 | `STAMP_HFLIP` | Mirror this stamp horizontally |
| 14 to 13 | `STAMP_ROTATE` | 0, 90, 180 or 270 degrees |
| 12 to 11 | | Must be 0 |
| 10 to 0 | `STAMP_SNO` | Stamp number |

Rotation is counter-clockwise and is applied before the horizontal flip, giving eight orientations per stamp. The two rotation bits are named RT1 (bit 14) and RT0 (bit 13), and the manual indexes its orientation figure by RT0 then RT1, so the field values are not in the order the bit names suggest:

| RT0 | RT1 | Field value | Rotation |
|---|---|---|---|
| 0 | 0 | `0b00` | 0 degrees |
| 0 | 1 | `0b10` | 90 degrees counter-clockwise |
| 1 | 0 | `0b01` | 180 degrees |
| 1 | 1 | `0b11` | 270 degrees counter-clockwise |

Use `STAMP_ROTATE_90` and the other named values rather than writing the field by hand.

The map's dimensions come from the SMS flag of `GA_SUB_REG_STAMPSIZE`: one screen (256x256 dots) or sixteen screens square (4096x4096 dots). Combined with the stamp size this fixes how much Word RAM the map occupies and how the base address must be aligned:

| Stamps | Map | Map size | Base address alignment |
|---|---|---|---|
| 16x16 | 256x256 | 0x200 | 0x200 |
| 32x32 | 256x256 | 0x80 | 0x80 |
| 16x16 | 4096x4096 | 0x20000 | 0x20000 |
| 32x32 | 4096x4096 | 0x8000 | 0x8000 |

The RPT flag decides what happens when sampling leaves the map: 1 repeats the map indefinitely, 0 makes everything outside it read as pixel value 0. The Mega-CD Software Development Manual describes this flag the other way around; Sega's later corrections and the Hardware Manual give the sense above, which is what the hardware does.

## Trace vector table

Four words per output line, at the address written to `GA_SUB_REG_TRACEVECTBASE`:

| Word | Contents | Format |
|---|---|---|
| 0 | X start | 13 integer bits, 3 fractional |
| 1 | Y start | 13 integer bits, 3 fractional |
| 2 | X step per output pixel | signed, 4 integer bits, 11 fractional |
| 3 | Y step per output pixel | signed, 4 integer bits, 11 fractional |

The table holds one entry per output line, so it is `Vdot x 8` bytes. The steps are ordinary two's complement, so normal arithmetic on them is correct and a negative step is a negative number. `lib/xform.h` provides `to_xform_pos` and `to_xform_delta` for the two formats.

The formats impose hard limits:

- A start coordinate is 13 integer bits, so the addressable source area is 8192x8192 dots, with 1/8 dot of sub-pixel precision.
- A step is 4 integer bits plus 11 fractional, so its magnitude must be under 16 source dots per output pixel, with a resolution of 1/2048.

A step of 1.0 samples the source at the same rate as the output. A step of 0.5 shows the source at double size. A step of 4.0 shrinks the source to a quarter. The 16.0 ceiling is therefore a limit on how far the source can be shrunk, which is what the distant rows of a ground plane need.

## Image buffer

The destination. Its dimensions come from four registers:

| Register | Unit | Meaning |
|---|---|---|
| `GA_SUB_REG_IMGBUFSTART` | | Base address in Word RAM |
| `GA_SUB_REG_IMGBUFVSIZE` | cells | Height of the buffer as allocated, stored as one less than the cell count |
| `GA_SUB_REG_IMGBUFVDOTSIZE` | dots | Height actually written |
| `GA_SUB_REG_IMGBUFHDOTSIZE` | dots | Width actually written |
| `GA_SUB_REG_IMGBUFOFFSET` | line, dot | Where writing starts inside the first cell |

The buffer is allocated as `Vcell x Hdot` but only a `Vdot x Hdot` region is written. Sega's figure draws the buffer as a cylinder: the allocated height is the circumference, the written band wraps around it, and writing past the bottom continues at the top. For a scrolling view, render only the newly exposed band and advance the offset.

Output cells are numbered down first, then across, matching Mega Drive sprite layout. The hardware generates one output line at a time, writing that line across every cell in the row before moving down.

# Register summary

| Register | Address | Role |
|---|---|---|
| `GA_SUB_REG_GFXSTAT` | `0xFF8058` | GRON: 1 while an operation is running |
| `GA_SUB_REG_STAMPSIZE` | `0xFF8059` | SMS, STS, RPT |
| `GA_SUB_REG_STAMPMAPBASE` | `0xFF805A` | Stamp map address |
| `GA_SUB_REG_IMGBUFVSIZE` | `0xFF805D` | Buffer height in cells, minus one |
| `GA_SUB_REG_IMGBUFSTART` | `0xFF805E` | Image buffer address |
| `GA_SUB_REG_IMGBUFOFFSET` | `0xFF8061` | Start line and dot within the first cell |
| `GA_SUB_REG_IMGBUFHDOTSIZE` | `0xFF8062` | Width in dots |
| `GA_SUB_REG_IMGBUFVDOTSIZE` | `0xFF8064` | Height in dots, consumed by the operation |
| `GA_SUB_REG_TRACEVECTBASE` | `0xFF8066` | Trace vector table address, starts the operation |

Addresses written to `GA_SUB_REG_STAMPMAPBASE`, `GA_SUB_REG_IMGBUFSTART` and `GA_SUB_REG_TRACEVECTBASE` are Word RAM offsets divided by 4.

Two registers do more than store a value:

- Writing `GA_SUB_REG_TRACEVECTBASE` starts the operation. Every other register must already hold its final value.
- `GA_SUB_REG_IMGBUFVDOTSIZE` is decremented by the operation and reads 0 on completion. It must be written before every operation; reusing the previous value performs a zero-height operation.

# Performing an operation

1. Acquire Word RAM in 2M mode.
2. Write the stamps, stamp map and trace vector table into Word RAM.
3. Set `GA_SUB_REG_STAMPSIZE` and `GA_SUB_REG_STAMPMAPBASE`.
4. Set `GA_SUB_REG_IMGBUFSTART`, `GA_SUB_REG_IMGBUFVSIZE`, `GA_SUB_REG_IMGBUFOFFSET`, `GA_SUB_REG_IMGBUFHDOTSIZE` and `GA_SUB_REG_IMGBUFVDOTSIZE`.
5. Write `GA_SUB_REG_TRACEVECTBASE`. The operation starts.
6. Wait for completion.
7. Hand Word RAM to the Main CPU, which DMAs the image buffer to VRAM.

Completion is signaled two ways: GRON in `GA_SUB_REG_GFXSTAT` returns to 0, and a level 1 interrupt is raised. Polling GRON is simpler; the interrupt lets the Sub CPU do other work meanwhile, subject to the timing note below.

```c
ga_sub_reg_tracevectbase = TRACE_TABLE_OFFSET / 4;

while (ga_sub_reg_gfxstat & GA_SUB_GRON_MASK)
  ;
```

# Priority mode

The `GA_SUB_PRIORITY` field of `GA_SUB_REG_MEMMODE` governs which pixels a write into Word RAM may change, which composites without a per-pixel test:

| Value | Effect |
|---|---|
| `GA_SUB_PRIORITY_OFF` | Every pixel is written |
| `GA_SUB_PRIORITY_UNDERWRITE` | Written only where the destination is 0 |
| `GA_SUB_PRIORITY_OVERWRITE` | Only non-zero source pixels are written |

Overwrite is transparency: source pixel 0 leaves the destination alone. Underwrite draws behind what is already there. The fourth encoding is prohibited.

Which writes it applies to depends on the Word RAM mode:

| Word RAM mode | Priority applies to |
|---|---|
| 2M | Data written to the image buffer by a graphics operation. CPU writes are unaffected. |
| 1M | Data written to a dot image area, CPU writes included. |

So in 2M mode priority cannot composite CPU-drawn sprites; it governs only what the graphics unit writes. Priority operates per pixel, so within one word one nibble may be written and its neighbor left alone.

# Timing and throughput

One gate array clock is about 80 ns. The base cost of an operation scales with the height and width written, and every Sub CPU or DMA access to Word RAM during an operation adds 3 clocks to it, plus a refresh penalty proportional to the total.

The unit and the Sub CPU contend for the same memory. Code that polls GRON in a tight loop while also touching Word RAM, or that builds the next frame's trace vector table in Word RAM while the current operation runs, slows the operation it is waiting for. Two approaches follow:

1. Do CPU work outside Word RAM. Build the trace vector table in Program RAM and block-copy it into Word RAM before triggering.
2. Use 1M mode to overlap. Each CPU owns one half of Word RAM and they swap: the Sub CPU renders into its half while the Main CPU DMAs the other half to VRAM.

# Pseudo-3D ground planes

The most common use of the unit is a textured ground plane drawn in perspective. This section derives the trace vector table for that case.

## The geometry

Place a camera at height `h` above a flat plane, looking horizontally. For a screen row at distance `d` below the horizon line, the view ray meets the ground at distance

```
z = (h * f) / d
```

where `f` is a focal-length constant controlling the field of view. Distance is proportional to one over the row's distance below the horizon: rows just under the horizon are far away, rows at the bottom of the screen are close.

A ground row at distance `z` covers `z` times more world width per screen pixel than one at distance 1, so the source must be sampled `z` times more coarsely:

```
step = z * k
```

for a constant `k` fixing the horizontal scale. With the camera at `(cx, cy)` facing angle `theta`, and screen width `W`:

```
forward = ( sin theta,  cos theta )
right   = ( cos theta, -sin theta )

dX = step * right.x
dY = step * right.y

startX = cx + z * forward.x - (W/2) * dX
startY = cy + z * forward.y - (W/2) * dY
```

`startX`, `startY`, `dX` and `dY` are the four words of the table entry for that row.

## Fixed point

Three problems follow from the trace vector formats:

- The horizon is infinity. As `d` approaches 0, `step` grows without bound, but a step cannot exceed 16.0. Start drawing some rows below the horizon, at the first row whose step is in range, and give the rows above it to a background layer.
- Division is expensive. `z = h * f / d` takes one value per screen row, so precompute a table of `z` indexed by row. It changes only with camera height or field of view.
- Trigonometry is expensive. Take `sin` and `cos` from a table indexed by angle; one full-period table serves both with a quarter-period offset for cosine.

With those tables the per-row work is a handful of multiplies and adds. Watch the intermediate width: the product of two fixed-point numbers has twice the fractional bits and must be shifted back down. `lib/fixed.h` defines the shift for each format.

Start coordinates are unsigned and the map repeats when RPT is set, so a camera position that runs off the edge of the texture wraps without a bounds test.

## Suggested structure

```
Per frame:
  Sub CPU:  for each visible row:
                z    = z_table[row]
                step = z * k
                look up sin/cos for the camera angle
                write startX, startY, dX, dY into Program RAM
            block-copy the table into Word RAM
            trigger the operation
            wait for GRON
            hand Word RAM to the Main CPU
  Main CPU: DMA the image buffer to VRAM
```

# Laying out Word RAM

All four structures share the 256 KB of Word RAM, and the graphics unit reads three of them while writing the fourth. Nothing separates them but the base addresses you choose. The Sub CPU sees 2M Word RAM at `0x080000` to `0x0BFFFF`. Sega's worked examples (Hardware Manual pages 43 to 47) use this order, lowest address first:

```
0x080000  stamp generator data   (the stamp pixels)
          trace vector table     Vdot x 8 bytes
          image buffer           Vcell x Hcell x 0x20 bytes
0x0BFFFF  stamp map table        sized per the stamp map table above
```

## Stamp number 0 is reserved

Stamp 0 cannot be used as a stamp: in the stamp map, entry 0 means no character, and the cell reads as pixel value 0. The storage for slot 0 is not read or written by the graphics unit and is ordinary RAM, so it can hold the trace vector table or other Sub CPU data. Its size is one stamp: 0x80 bytes for 16x16, 0x200 for 32x32.

## How much is left for stamps

Everything not spent on the other three structures is available for stamp pixels, and the stamp map is the largest consumer:

| Stamps | Map | Map size | Distinct stamps addressable |
|---|---|---|---|
| 16x16 | 256x256 | 0x200 | 2048 (SNO is 11 bits) |
| 32x32 | 256x256 | 0x80 | 512 (SNO bits 10 to 2) |
| 16x16 | 4096x4096 | 0x20000 | 2048 |
| 32x32 | 4096x4096 | 0x8000 | 512 |

The 16-screen map with 16x16 stamps consumes half of Word RAM before any stamp, trace vector table or image buffer is placed. The one-screen maps are small enough to sit almost anywhere.

## Units

Three different units appear in these registers:

| Quantity | Unit | Note |
|---|---|---|
| Trace vector table size | bytes | `Vdot x 8`, one entry per output line |
| Image buffer size | bytes | `Vcell x Hcell x 0x20`; a cell is 8x8 dots at 4 bpp |
| `GA_SUB_REG_IMGBUFVSIZE` | cells | stored as one less than the cell count |
| `GA_SUB_REG_IMGBUFVDOTSIZE`, `GA_SUB_REG_IMGBUFHDOTSIZE` | dots | the region actually written |
| Base address registers | Word RAM offset / 4 | all three |

The buffer is allocated by cell counts but written by dot counts.

# Offset, and the cells that do not get written

`GA_SUB_REG_IMGBUFOFFSET` shifts where writing begins inside the first cell, by whole lines and dots, so a rendered band can be positioned without moving the buffer.

With a non-zero offset, `Vcell` must be set larger than the cell height actually needed, unless `Hdot` is 8 or less. Shifting the written band by a dot pushes data out of one cell into its neighbor, and the displaced pixels are left unchanged regardless of the priority setting; if the buffer is exactly as tall as the written region, the overspill has nowhere to go.

A graphics operation also does not write every byte of the buffer it is given. The written region is `Vdot x Hdot` within an allocation of `Vcell x Hcell` cells. Where `Vdot` is not a multiple of 8, bands of the buffer are never touched and keep whatever they held before. If a transform renders correctly but is framed by garbage, size the buffer to match or clear it once before first use.

The operation completes, and the level 1 interrupt fires, when the last dot of the last cell is written.

# Common failure modes

| Symptom | Likely cause |
|---|---|
| Operation does nothing the second time | `GA_SUB_REG_IMGBUFVDOTSIZE` not rewritten; it is consumed |
| Output is torn or partly stale | Word RAM handed over before GRON cleared |
| Wait loop returns immediately | GRON is bit 7 of the byte register `GA_SUB_REG_GFXSTAT`; test `GA_SUB_GRON_MASK` |
| Ground plane warps near the horizon | Step exceeded 16.0 and wrapped; start lower on the screen |
| Everything is mirrored or inverted | Check the sign of the step, and `STAMP_HFLIP` and `STAMP_ROTATE` in the map entries |
| Nothing renders | `GA_SUB_REG_TRACEVECTBASE` written before the other registers were set |
| Correct output, very slow | Sub CPU touching Word RAM during the operation |
| Rendered area framed by garbage | Buffer allocated `Vcell x Hcell` but only `Vdot x Hdot` written |
| Smearing into the next cell after changing offset | `Vcell` not larger than the cell height needed |
| A cell renders empty where a stamp was expected | Map entry 0 means no character, not stamp 0 |
| CPU-drawn sprite ignores priority | In 2M mode priority governs only graphics-operation writes |

# References

- Register definitions: `lib/sub/gate_arr.def.h`, and [gate_array.md](gate_array.md)
- Fixed-point helpers: `lib/fixed.h`, `lib/xform.h`
- Worked examples: `examples/transforms` (the registers and a hand-driven trace vector), `examples/ground_plane` (a perspective ground plane with a camera)
- Mega-CD Hardware Manual, Ver 1.0, rotation and scaling section, pages 35 to 54

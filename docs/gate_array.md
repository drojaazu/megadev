# Gate Array

The gate array is the collection of registers for interacting with the Mega CD hardware. It also
facilitates communication between the Main and Sub CPUs. In some Sega documentation it is referred to
as the "Mega CD Engine," which is a perfect description of its role. The gate array is a central
piece of Mega CD software.

The registers are described in the official documentation from Sega. However, that documentation is
badly scanned and difficult to read at best, and incomprehensible due to bad translations and terse
descriptions at worst.

Reference information for the registers themselves lives in the source (`gate_arr.def.h` and
`gate_arr.h`, in both the `main` and `sub` directories), annotated so that Doxygen can generate a
modern reference from it. This document covers the conventions those annotations follow, and the
hardware constraints they describe.

## Two views of the same hardware

The Main and Sub CPUs each see the gate array at a different address:

| | Main CPU | Sub CPU |
|---|---|---|
| Base address | `0xA12000` | `0xFF8000` |
| Defined in | `lib/main/gate_arr.def.h` | `lib/sub/gate_arr.def.h` |

The two sides do not expose the same set of registers, and where they overlap the bit layouts often
differ. Use the header for the CPU you are writing for.

## Definitions and symbols

Everything gate-array related is prefixed `GA_`. Register fields follow the project-wide field
convention described in [manual.md](manual.md) under "Bitwise Definition Naming":

    GA_DMNA_POS     1                                     /* bit position   */
    GA_DMNA_WIDTH   1                                     /* bits occupied  */
    GA_DMNA_MASK    FIELD_MASK(GA_DMNA_POS, GA_DMNA_WIDTH) /* the mask      */

Use `_POS` with the bit opcodes and `_MASK` with logic operations:

    btst  #GA_DMNA_POS, GA_REG_MEMMODE    ; assembly
    if (ga_reg_memmode & GA_DMNA_MASK)    /* C */

— with the caveat below for any field above bit 7 of a register that is still 16 bit.

A single-bit flag is simply a field of width 1, so the same three definitions describe both flags and
wider fields. Multi-bit field *values* are stored unshifted and placed with `FIELD_PREP`.

### Some registers are 16 bit, some are a pair of byte registers

The hardware presents 16-bit registers, but its *flag* registers are organised a byte at a time: no
field in either header straddles bit 7/8. Where the two halves of a hardware register hold unrelated
concerns, Megadev models them as two named byte registers rather than one word:

    #define GA_REG_LED      0xFF8000   /* LEDR bit 0, LEDG bit 1        */
    #define GA_REG_SUBCTRL  0xFF8001   /* RES0 bit 0, VER0-3 bits 4-7   */

    #define GA_REG_INT2     0xA12000   /* IFL2 bit 0, IEN2 bit 7        */
    #define GA_REG_SUBCPU   0xA12001   /* SRES bit 0, SBRQ bit 1        */

`_POS` is then relative to that byte, so it goes straight to a bit opcode:

    bset  #GA_LED_R_POS, GA_REG_LED    ; assembly
    ga_reg_led |= GA_LED_R_MASK;       /* C */

This is why `GA_REG_RESET` no longer exists on either side. On the Sub side the LEDs and the
peripheral reset shared an address and nothing else, which made bit 0 ambiguous: bit 0 of the word is
`RES0`, the peripheral reset, and bit 0 of the LED byte is the red LED. The Main side had the same
problem in a sharper form — `GA_RAISE_INT2` and `GA_SUB_RESET` were *both* defined at bit 0, because
one is `IFL2` in the high byte and the other is `SRES` in the low byte. Splitting the registers makes
each of those pairs impossible to confuse.

The two sides are split symmetrically, so `GA_REG_WP` and `GA_REG_MEMMODE` mean the same thing in
both headers, at `$FF8002`/`$FF8003` and `$A12002`/`$A12003` respectively.

A word access to a split pair is still available, since the halves are adjacent and the high byte is
even-aligned. It is simply no longer the default spelling, so clearing the Program RAM write
protection while setting the memory mode is now something you have to write on purpose.

### When only one byte of a register is used

Some registers put every field in one byte and leave the other empty. Those become a byte register at
the address of the byte that carries the data — so `GA_REG_INT3TIMER` is `0xFF8031`, not `0xFF8030`.
Without that, the natural spelling `move.b #n, GA_REG_INT3TIMER` would write the empty half and do
nothing at all.

**The unused byte is left undefined on purpose.** There is no `_UNUSED` symbol for `0xFF8030`,
`0xFF8032` or `0xA12005`. If you are investigating whether one of those bytes really is inert, write
the address literally — and note that what you most likely want to know, that it reads 0 and ignores
writes, is in the register's documentation rather than in any macro.

The hardware address is still easy to find: each register's documentation group is titled with it,
so searching for the address printed in the official manual lands on the right register even when
the definition sits one byte along.

### Registers that stay 16 bit

Two kinds do not split:

- **Word-only data registers** — `GA_REG_CDCHOSTDATA`, `GA_REG_DMAADDR`, `GA_REG_STOPWATCH`,
  `GA_REG_CDFADER` and the address/size registers. They hold one wide value, have no fields, and
  forbid bit operations, so there is nothing to separate.
- **Registers whose fields span both halves** — the Sub side's `GA_REG_CDCMODE` (`$FF8004`) is the
  example: the CDC register address sits in the low byte and the status flags in the high byte, but
  they are read and written as one CDC transaction. (The Main side's `$A12004` is *not* one of these
  — it has no low byte at all and is a single read-only byte register.)

For those, `_POS` is relative to the whole word and a position of 8 or more is **not** a legal
bit-opcode operand: on a memory operand the 68000 takes the bit number modulo 8, so `btst #14` would
test bit 6 of the wrong byte. Two macros in `build.def.h` handle it:

    btst  #FIELD_BPOS(GA_CDCMODE_DSR), FIELD_BYTE(GA_REG_CDCMODE, GA_CDCMODE_DSR)

`FIELD_BYTE` picks the half and `FIELD_BPOS` renumbers within it. On a byte register both are the
identity, so they are always safe to use — but on a byte register they are also noise, and the plain
`_POS` is preferred.

## Bus width

All gate array registers are word sized (16 bit) and can be accessed as such. Most registers also
allow 8 bit access to the upper and lower bytes. Those that do not **must** be accessed as words —
a byte access to a word-only register can raise a bus or address error.

Where a register's documentation carries an access warning, take it literally: this is one of the
few places in Mega CD programming where getting it wrong produces a hardware exception rather than
merely a wrong value. The authoritative per-register table is below.

Registers that are accessed a byte at a time are **named** a byte at a time (see above), so there are
no `_HI` / `_LO` address aliases to keep straight. `GA_REG_LED` is the high byte of `0xFF8000` and
`GA_REG_SUBCTRL` is the low byte; `GA_REG_WP` and `GA_REG_MEMMODE` are the two halves of `0xFF8002`.
The C accessors are typed `ga_reg8`, and `ga_reg8_ro` where this CPU may only read:

    #define ga_reg_wp       (*((ga_reg8_ro) GA_REG_WP))    /* Sub side: read only */
    #define ga_reg_memmode  (*((ga_reg8) GA_REG_MEMMODE))

Note that `ga_reg const` does **not** express read-only — it is a const pointer to a mutable
register, which enforces nothing at the point of use. `ga_reg_ro` and `ga_reg8_ro` are the types that
do.

`GA_REG_COMFLAGS` follows the same pattern from the other direction: its high byte is the Main CPU's
flags and its low byte the Sub CPU's, so the accessors are `ga_reg_comflags_main` and
`ga_reg_comflags_sub`, read-only on the side that may not write.

## Bit level restrictions and read/write access

Bit level opcodes (`btst`, `bset`, `bclr`, `bchg` and friends) are limited or forbidden on some
registers. Where they are forbidden you must read the whole byte or word, operate on the copy in a
register, and write the whole value back. A `@warning` is included in the documentation of every
register where this applies.

Some registers or fields are further restricted to reads or writes only. This is distinct from
fields whose value is simply undefined in one direction. The generated documentation shows this per
bit: `◯` means the access is valid, a blank means it is undefined, and `🗙` means it is forbidden.

For example, the `VER` field of `GA_REG_SUBCTRL` is meaningful when read but not when written:

| |7|6|5|4|3|2|1|0|
|:|:|:|:|:|:|:|:|:|
| |\b VER3|\b VER2|\b VER1|\b VER0| | | |\b RES0|
|\b R|◯|◯|◯|◯| | | |◯|
|\b W| | | | | | | |◯|

Writing to `VER` is not allowed but is unlikely to crash anything. Fields like this carry an `@note`.

Byte registers get an 8-column table; the ones that remain 16 bit get all sixteen.

A register that is genuinely read- or write-only is a different matter — violating it will likely
raise a bus error. Those are marked `🗙` and carry a `@warning` rather than a `@note`:

| |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
| |\b REG_CDC_DATA||||||||||||||||
|\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
|\b W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|

## Access width and bit operations, per register

Transcribed from *Mega-CD Hardware Manual — The Hardware*, Ver 1.0 (1991-10-14), page 21,
"Initial values of registers after Power ON". Cross-read against both scans in the primary source
collection (see SPEC.md §7).

The manual states the default plainly: **"If not otherwise mentioned, byte/word access and bit
operation commands are allowed."** The table below is the exceptions, and they are the ones that
matter — a byte access to a word-only register can raise a bus error.

`W/B` = word or byte access. `W` = **word access only**.

### Sub CPU side, base `$FF8000`

| Offset | Register | Access | Bit ops |
|---|---|---|---|
| `00` | Reset / LED / version | W/B | yes |
| `02` | Memory mode / write protect | W/B | yes |
| `04` | CDC mode / register address | W/B | **btst only** |
| `06` | CDC register data | W/B | **no** — not even `CLR` |
| `08` | CDC host data — **read only** | **W** | **no** |
| `0A` | CDC DMA address — **write only** | **W** | **no** |
| `0C` | Stopwatch | **W** | **no** |
| `0E` | Communication flag | W/B | yes |
| `10`–`2E` | Communication command / status | W/B | yes |
| `30` | Timer (INT3) | W/B | yes |
| `32` | Interrupt mask | W/B | yes |
| `34` | **CD fader** — *direct access prohibited* | **W** | **no** |
| `36` | CDD control — *direct access prohibited* | W/B | **no BSET/BCLR** |
| `38`–`4A` | CDD status / command | W/B | **no** |
| `4C` | Font colour | W/B | yes |
| `4E` | Font bit | W/B | yes |
| `50`–`56` | Font data | W/B | yes |
| `58` | Stamp size | W/B | yes |
| `5A` | Stamp map base address | **W** | **no** |
| `5C` | Image buffer V-cell size | W/B | **no** |
| `5E` | Image buffer start address | **W** | **no** |
| `60` | Image buffer offset | W/B | yes |
| `62` | Image buffer H-dot size | **W** | **no** |
| `64` | Image buffer V-dot size | **W** | **no** |
| `66` | Trace vector base address | **W** | **no** |
| `68` | Sub-code address | W/B | yes |
| `100`–`1FE` | Sub-code data | W/B | yes |

### Main CPU side, base `$A12000`

| Offset | Register | Access | Bit ops |
|---|---|---|---|
| `00` | INT2 control / Sub CPU reset | W/B | **btst only** — BSET and BCLR are forbidden |
| `02` | Memory mode / write protect | W/B | yes |
| `04` | CDC mode — **read only**, high byte only | W/B | yes |
| `06` | H-INT vector | **W** | yes |
| `08` | CDC host data — **read only** | **W** | **no** |
| `0C` | Stopwatch — **read only**; only the Sub CPU clears it | **W** | **no** |
| `0E` | Communication flag | W/B | yes |
| `10`–`1E` | Communication command (Main writes) | W/B | yes |
| `20`–`2E` | Communication status — **read only** | W/B | yes |

This is why the CD fader (`GA_REG_CDFADER`, `$FF8034`) needs care: it is **word access only, with no bit
operations**. Read-modify-write the whole word.

The `04`/`06` rows were once recorded as uncertain, because the scan's row alignment on page 21 is
ambiguous about which of them carries "Only btst". The per-register pages settle it: page 26 places
no restriction beyond bit operations on `$FF8004`, and page 27 says of `$FF8006` that it "may not be
accessed using commands such as BTST, BCLR, BSET, CLR, etc." — note `CLR`, which is a
read-modify-write on the 68000 and so is caught by the same rule.


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

    btst  #GA_DMNA_POS, GA_REG_MEMMODE_LO    ; assembly
    if (ga_reg_memmode & GA_DMNA_MASK)       /* C */

— with the caveat below for any field above bit 7.

A single-bit flag is simply a field of width 1, so the same three definitions describe both flags and
wider fields. Multi-bit field *values* are stored unshifted and placed with `FIELD_PREP`.

### `_POS` counts from the register, not from the byte

The registers are 16 bit, and a `_POS` is always relative to the whole register: bit 0 is the LSB of
the word, and a field in the upper half has a position of 8 or more. `GA_LED_R_POS` is 8, not 0,
even though the red LED is the bottom bit of the byte you would actually write.

This matters because `_MASK` is derived from `_POS`. Anything counting from its own byte produces a
mask naming the wrong bits — `GA_MEMMODE_WP` once had a position of 0 and so a mask of `0x00FF`,
which is the exact complement of the byte it protects.

It also means **a `_POS` of 8 or more is not a legal bit-opcode operand.** On a memory operand the
68000 takes the bit number modulo 8, so `bset #8` on a register address sets bit 0 of that same
byte — it assembles, it runs, and it hits the wrong bit. Use the two helpers from `build.def.h`,
which pick the correct half and renumber within it:

    bset  #FIELD_BPOS(GA_LED_R), FIELD_BYTE(GA_REG_RESET, GA_LED_R)   ; assembly
    ga_reg_reset |= GA_LED_R_MASK;                                    /* C */

The C form needs neither helper: a word-wide read-modify-write on the whole register is already
correct, and the mask is already in the right place. The helpers exist for the assembly bit opcodes,
and for the C cases where a single-byte write is wanted.

## Bus width

All gate array registers are word sized (16 bit) and can be accessed as such. Most registers also
allow 8 bit access to the upper and lower bytes. Those that do not **must** be accessed as words —
a byte access to a word-only register can raise a bus or address error.

Where a register's documentation carries an access warning, take it literally: this is one of the
few places in Mega CD programming where getting it wrong produces a hardware exception rather than
merely a wrong value. The authoritative per-register table is below.

Because byte access is common, registers that are routinely used that way carry `_HI` and `_LO`
address definitions:

    #define GA_REG_MEMMODE     0xFF8002
    #define GA_REG_MEMMODE_HI  GA_REG_MEMMODE
    #define GA_REG_MEMMODE_LO  (GA_REG_MEMMODE + 1)

`_HI` is simply an alias for the register address. Prefer it over the bare name when you mean a byte
access, so the width you intended is visible at the point of use rather than implied. There are
matching C accessors typed as `ga_reg8`:

    #define ga_reg_memmode_hi ((ga_reg8) GA_REG_MEMMODE_HI)
    #define ga_reg_memmode_lo ((ga_reg8) GA_REG_MEMMODE_LO)

Currently provided for `GA_REG_RESET` and `GA_REG_MEMMODE` on both CPU sides — the registers the
library itself accesses a byte at a time. They are added where byte access is known to be used
rather than blanket, since offering `_LO` for a word-only register would invite a bus error.

Where the two halves have distinct meanings, they get names that say so instead. `GA_REG_COMFLAGS`
is the clearest case: its high byte is the Main CPU's flags and its low byte the Sub CPU's, so the
accessors are `ga_reg_comflags_main` and `ga_reg_comflags_sub`, each `const` on the side that may
only read them.

## Bit level restrictions and read/write access

Bit level opcodes (`btst`, `bset`, `bclr`, `bchg` and friends) are limited or forbidden on some
registers. Where they are forbidden you must read the whole byte or word, operate on the copy in a
register, and write the whole value back. A `@warning` is included in the documentation of every
register where this applies.

Some registers or fields are further restricted to reads or writes only. This is distinct from
fields whose value is simply undefined in one direction. The generated documentation shows this per
bit: `◯` means the access is valid, a blank means it is undefined, and `🗙` means it is forbidden.

For example, the `ROM_VER` field of `GA_REG_RESET` is meaningful when read but not when written:

| |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
| | | | | | | |\b LED_G|\b LED_R|\b ROM_VER|||| | | |\b SUB_RESET|
|\b R| | | | | | |◯|◯|◯|◯|◯|◯| | | |◯|
|\b W| | | | | | |◯|◯| | | | | | | |◯|

Writing to `ROM_VER` is not allowed but is unlikely to crash anything. Fields like this carry an
`@note`.

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
| `06` | CDC register data | W/B | **no** |
| `08` | CDC host data | **W** | **no** |
| `0A` | CDC DMA address | **W** | **no** |
| `0C` | Stopwatch | **W** | **no** |
| `0E` | Communication flag | W/B | yes |
| `10`–`2E` | Communication command / status | W/B | yes |
| `30` | Timer (INT3) | W/B | yes |
| `32` | Interrupt mask | W/B | yes |
| `34` | **CD fader** | **W** | **no** |
| `36` | CDD control | W/B | **no** |
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
| `00` | Reset / INT2 | W/B | **btst only** |
| `02` | Memory mode / write protect | W/B | yes |
| `04` | CDC mode | W/B | yes |
| `06` | H-INT vector | **W** | yes |
| `08` | CDC host data | **W** | **no** |
| `0C` | Stopwatch | **W** | **no** |
| `0E` | Communication flag | W/B | yes |
| `10`–`2E` | Communication command / status | W/B | yes |

This is why the CD fader (`GA_REG_CDFADER`, `$FF8034`) needs care: it is **word access only, with no bit
operations**. Read-modify-write the whole word.

> **One row is uncertain.** The scan's row alignment around offsets `04` and `06` is ambiguous as to
> which carries "Only btst". Both are recorded above on the reading that the two scans agree on, but
> if you are about to rely on bit operations against the CDC registers, check the page yourself.


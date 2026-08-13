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

    btst  #GA_DMNA_POS, GA_REG_MEMMODE+1     ; assembly
    if (*ga_reg_memmode & GA_DMNA_MASK)      /* C */

A single-bit flag is simply a field of width 1, so the same three definitions describe both flags and
wider fields. Multi-bit field *values* are stored unshifted and placed with `FIELD_PREP`.

## Bus width

All gate array registers are word sized (16 bit) and can be accessed as such. Most registers also
allow 8 bit access to the upper and lower bytes. Those that do not **must** be accessed as words —
a byte access to a word-only register can raise a bus or address error.

Where a register's documentation carries an access warning, take it literally: this is one of the
few places in Mega CD programming where getting it wrong produces a hardware exception rather than
merely a wrong value.

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

## Registers with additional constraints

Some registers carry timing or sequencing constraints beyond simple access width — the CD fader
control (`GA_REG_CDFADER`, `0xFF8034`) among them.

**These are not yet documented here.** The constraints are described in *Mega-CD Hardware Manual —
The Hardware*, which is held with the project's primary sources (see SPEC.md §7); they have not been
transcribed, and no claim about them should be made from memory. See BACKLOG DOC-17.

Gate Array
================

The gate array is the collection of registers for interacting with the Mega CD hardware. It also facilitates communication between the Main and Sub CPUs. In some Sega documentation, it is referred to as the "Mega CD Engine," which is a perfect description of its role. The gate array is a central piece of Mega CD software.

The gate array registers are already described in the official documentation from Sega. However, that documentation is badly scanned and difficult to read at best, and incomprehensible due to bad translations and terse descriptions at worst.

Reference information for the registers is already present in the source files (see `gate_arr.def.h` and `gate_arr.h` within both the `main` and `sub` directories) and is formatted to generate modern documentation via doxygen.

This document will go over some general information regarding gate array usage.

# Definitions / Symbols

All Megadev definitions and symbols related to the gate array are prefixed with `GA_`. Bit mask definitions are included for bit fields within most registers, where it is deemed useful. Bit offsets are also included for registers where bit level operations are allowed. In some cases, definition values are duplicated with different labels in order to indicate intention.

# Bus Width

All gate array registers are word sized (16 bit) and can be accessed as such. Most registers also allow 8 bit access of the upper and lower bytes. Those that do not allow byte sized access must only be accessed as words.

It is important that you access these registers properly to prevent bus or address errors.

In Megadev, for registers that allow 8 bit access, there will be additional definitions for the high and low bytes with `_HI` and `_LO` suffixes, respectively. For example:

```
#define GA_REG_RESET    0xFF8000
#define GA_REG_RESET_HI GA_REG_RESET
#define GA_REG_RESET_LO (GA_REG_RESET + 1)

```

While the `_HI` variant is simply an alias for the register, we recommend using it to semantically indicate the intention of byte access rather than word access. Registers only accessible at word width will not not have these variants.

They are also present for C development, with explicit type casting:

```
#define ga_reg_reset    ((ga_reg16) GA_REG_RESET)
#define ga_reg_reset_hi ((ga_reg8) GA_REG_RESET_HI)
#define ga_reg_reset_lo ((ga_reg8) GA_REG_RESET_LO)
```

# Bit Level Restrictions and Read/Write Access

Bit level opcode (such as BTST, BCLR, Bcc, etc) usage is limited or restricted on some registers. In such cases you will need to read the entire byte/word, perform any operations on the in-memory copy, then write the entire value back, if necessary. A warning is included 

Furthermore, some registers are explicitly read or write only. This is different from bitfields that have undefined states for reads/writes. For example, on the `GA_REG_RESET` registers, the `ROM_VER` field is valid for reads but not for writes, as illustrated in the Doxygen reference:

\b GA_REG_RESET

| |7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|
|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
| | | | | | | |\b LED_G|\b LED_R|\b ROM_VER|||| | | |\b SUB_RESET|
|\b R| | | | | | |◯|◯|◯|◯|◯|◯| | | |◯|
|\b W| | | | | | |◯|◯| | | | | | | |◯|

Writing to the `ROM_VER` field is not allowed, but it is also not likely to crash the program if it is done. Registers with undefined read/write operations will be noted with an `@note` tag.

However, for registers that are explicitly marked as read or write only, doing so will likely cause a bus error or other exception. In such cases, the fields are marked with an X in Doxygen:

\b GA_REG_CDC_DATA

| |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|:|
| |\b REG_CDC_DATA||||||||||||||||
|\b R|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|◯|
|\b W|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|🗙|

Such registers/fields will also be tagged with a more serious `@warning` indicator.

# Restricted Registers

Some registers (namely the CD Fader control 
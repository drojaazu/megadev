Gate Array
================

The gate array is the collection of registers for interacting with the Mega CD hardware. It also facilitates communication between the Main and Sub CPUs. In some Sega documentation, it is referred to as the "Mega CD Engine," which is a perfect description of its role. The gate array is a central piece of Mega CD software.

The gate array registers are already described in the official documentation from Sega. However, that documentation is badly scanned and difficult to read at best, and incomprehensible due to bad translations and terse descriptions at worst.

Reference information for the registers is already present in the source files (see `gate_arr.def.h` and `gate_arr.h` within both the `main` and `sub` directories) and is formatted to generate modern documentation via doxygen.

This document will go over some general information regarding gate array usage.

# Bus Width

All gate array registers are word sized (16 bit) and can be accessed as 16 bit values. Most registers also allow 8 bit access of the upper and lower bytes. Those that od not have byte sized access must only be accessed as words.

It is important that you access these registers properly to prevent bus or address errors.

In Megadev, for registers that allow 8 bit access, there will be additional definitions for the high and low bytes. For example:

```
#define GA_REG_RESET    0xFF8000
#define GA_REG_RESET_HI GA_REG_RESET
#define GA_REG_RESET_LO (GA_REG_RESET + 1)

```

While the `_HI` variant is simply an alias for the register, we recommend using it to semantically indicate the intention of byte access rather than word access. Registers that do not have `_HI` or `_LO` variants can only be accessed by word width.

These variants are also present for C development, with explicit type casting:

```
#define ga_reg_reset    ((ga_reg16) GA_REG_RESET)
#define ga_reg_reset_hi ((ga_reg8) GA_REG_RESET_HI)
#define ga_reg_reset_lo ((ga_reg8) GA_REG_RESET_LO)
```

# Bit Level Restrictions and Read/Write Access

Bit level opcode (such as BTST, BCLR, Bcc, etc) usage is limited or restricted on some registers. Furthermore, some registers are read or write only. In either case, here too registers must be accessed properly to prevent system errors.

Registers that have limited

| | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| | | | | | | |**LEDG**|**LEDR**|**VER**|||| | | |**RES**|
|**R**| | | | | | | |◯| | | | | |◯|◯|◯|
|**W**| | | | | | | | | | | | | |◯|◯|◯|


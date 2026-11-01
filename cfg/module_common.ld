/* Sections and checks shared by module_mmd.ld and module_smd.ld, included
inside their SECTIONS command. The including script sets _MODULE_ROM_LOAD, the
offset in the module file at which the .text section is stored. */

  .text MODULE_ROM_ORIGIN : AT ( _MODULE_ROM_LOAD ) SUBALIGN(2)
  {
    HIDDEN(_ROM_ORIGIN = .);
    HIDDEN(_INIT_ORIGIN = .);
    KEEP(*(.init))
    . = ALIGN(4);
    HIDDEN(_INIT_LENGTH = ABSOLUTE(. - _INIT_ORIGIN));
    HIDDEN(_TEXT_ORIGIN = .);
    *(.text*)
    . = ALIGN(4);
    HIDDEN(_TEXT_LENGTH = ABSOLUTE(. - _TEXT_ORIGIN));
    HIDDEN(_RODATA_ORIGIN = .);
    *(.rodata*)
    . = ALIGN(4);
    HIDDEN(_RODATA_LENGTH = ABSOLUTE(. - _RODATA_ORIGIN));
    HIDDEN(_ROM_LENGTH = ABSOLUTE(. - _ROM_ORIGIN));
    HIDDEN(_ROM_DATA_ORIGIN = .);
  } > MODULE_ROM

  .bss MODULE_RAM_ORIGIN : SUBALIGN(2)
  {
    HIDDEN(_BSS_ORIGIN = .);
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    HIDDEN(_BSS_LENGTH = ABSOLUTE(. - _BSS_ORIGIN));
    HIDDEN(_BSS_LENGTH_LOOPSZ = ABSOLUTE(_BSS_LENGTH) >> 2);
  } > MODULE_RAM

  .data : AT ( LOADADDR(.text) + SIZEOF(.text) ) SUBALIGN(2)
  {
    HIDDEN(_RAM_DATA_ORIGIN = .);
    *(.data*)
    . = ALIGN(4);
    HIDDEN(_RAM_DATA_LENGTH = ABSOLUTE(. - _RAM_DATA_ORIGIN));
    HIDDEN(_RAM_DATA_LENGTH_LOOPSZ = ABSOLUTE(_RAM_DATA_LENGTH) >> 2);
  } > MODULE_RAM

  /* Every module starts with init code (std_init.s or main/ipx_init.s), which
  clears .bss and copies .data before main. A module that does its own setup
  defines MODULE_NO_INIT. */
  ASSERT(_INIT_LENGTH > 0 || DEFINED(MODULE_NO_INIT), "module has no init code: add std_init.s (or main/ipx_init.s) to its sources, or define MODULE_NO_INIT")

  HIDDEN(_RAM_LENGTH = _RAM_DATA_LENGTH + _BSS_LENGTH);

  /* If the module sets a stack (std_init_stack.s, main/ipx_init_stack.s), it
  must not run into the module. STACK_SIZE is optional; without it, only
  STACK_TOP itself is checked. */
  HIDDEN(_STACK_TOP = DEFINED(STACK_TOP) ? ((STACK_TOP - 1) & 0xFFFFFF) + 1 : 0); /* 0 is the top of RAM, 0x1000000 */
  HIDDEN(_STACK_BASE = _STACK_TOP - (DEFINED(STACK_SIZE) ? STACK_SIZE : 0));
  HIDDEN(_IMAGE_ROM_BASE = ADDR(.text) & 0xFFFFFF);
  HIDDEN(_IMAGE_ROM_END = _IMAGE_ROM_BASE + SIZEOF(.text));
  HIDDEN(_IMAGE_RAM_BASE = ADDR(.bss) & 0xFFFFFF);
  HIDDEN(_IMAGE_RAM_END = _IMAGE_RAM_BASE + SIZEOF(.bss) + SIZEOF(.data));
  ASSERT(!(_STACK_TOP & 1), "STACK_TOP must be even")
  ASSERT(!_STACK_TOP || _STACK_BASE >= _IMAGE_ROM_END || _STACK_TOP <= _IMAGE_ROM_BASE, "stack overlaps the module's code")
  ASSERT(!_STACK_TOP || _STACK_BASE >= _IMAGE_RAM_END || _STACK_TOP <= _IMAGE_RAM_BASE, "stack overlaps the module's RAM")

  /* visibility references from lib/module_hidden.s */
  /DISCARD/ : { *(.megadev.hidden) }

  INCLUDE no_constructors.ld

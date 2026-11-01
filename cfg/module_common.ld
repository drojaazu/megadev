/* Sections and checks shared by module_mmd.ld and module_smd.ld, included
inside their SECTIONS command. The including script sets _MODULE_ROM_LOAD, the
offset in the module file at which the .rom section is stored. */

  .rom MODULE_ROM_ORIGIN : AT ( _MODULE_ROM_LOAD ) SUBALIGN(2)
  {
    _INIT_ORIGIN = .;
    *(.init)
    . = ALIGN(4);
    _INIT_LENGTH = ABSOLUTE(. - _INIT_ORIGIN);
    _TEXT_ORIGIN = .;
    *(.text*)
    . = ALIGN(4);
    _TEXT_LENGTH = ABSOLUTE(. - _TEXT_ORIGIN);
    _RODATA_ORIGIN = .;
    *(.rodata*)
    . = ALIGN(4);
    _RODATA_LENGTH = ABSOLUTE(. - _RODATA_ORIGIN);
    _ROM_LENGTH = ABSOLUTE(. - _INIT_ORIGIN);
    _ROM_LENGTH_LOOPSZ = ABSOLUTE(_ROM_LENGTH) >> 2;
    _ROM_DATA_ORIGIN = .;
  } > MODULE_ROM

  .ram_bss MODULE_RAM_ORIGIN : SUBALIGN(2)
  {
    _BSS_ORIGIN = .;
    *(.bss*)
    . = ALIGN(4);
    _BSS_LENGTH = ABSOLUTE(. - _BSS_ORIGIN);
    _BSS_LENGTH_LOOPSZ = ABSOLUTE(_BSS_LENGTH) >> 2;
  } > MODULE_RAM

  .ram_data : AT ( LOADADDR(.rom) + SIZEOF(.rom) ) SUBALIGN(2)
  {
    _RAM_DATA_ORIGIN = .;
    *(.data*)
    . = ALIGN(4);
    _RAM_DATA_LENGTH = ABSOLUTE(. - _RAM_DATA_ORIGIN);
    _RAM_DATA_LENGTH_LOOPSZ = ABSOLUTE(_RAM_DATA_LENGTH) >> 2;
  } > MODULE_RAM

  /* Every module starts with init code (std_init.s or main/ipx_init.s), which
  clears .bss and copies .data before main. A module that does its own setup
  defines MODULE_NO_INIT. */
  ASSERT(_INIT_LENGTH > 0 || DEFINED(MODULE_NO_INIT), "module has no init code: add std_init.s (or main/ipx_init.s) to its sources, or define MODULE_NO_INIT")

  /* If the module sets a stack (std_init_stack.s, main/ipx_init_stack.s), it
  must not run into the module. STACK_SIZE is optional; without it, only
  STACK_TOP itself is checked. */
  _STACK_TOP = DEFINED(STACK_TOP) ? ((STACK_TOP - 1) & 0xFFFFFF) + 1 : 0; /* 0 is the top of RAM, 0x1000000 */
  _STACK_BASE = _STACK_TOP - (DEFINED(STACK_SIZE) ? STACK_SIZE : 0);
  _IMAGE_ROM_BASE = ADDR(.rom) & 0xFFFFFF;
  _IMAGE_ROM_END = _IMAGE_ROM_BASE + SIZEOF(.rom);
  _IMAGE_RAM_BASE = ADDR(.ram_bss) & 0xFFFFFF;
  _IMAGE_RAM_END = _IMAGE_RAM_BASE + SIZEOF(.ram_bss) + SIZEOF(.ram_data);
  ASSERT(!(_STACK_TOP & 1), "STACK_TOP must be even")
  ASSERT(!_STACK_TOP || _STACK_BASE >= _IMAGE_ROM_END || _STACK_TOP <= _IMAGE_ROM_BASE, "stack overlaps the module's code")
  ASSERT(!_STACK_TOP || _STACK_BASE >= _IMAGE_RAM_END || _STACK_TOP <= _IMAGE_RAM_BASE, "stack overlaps the module's RAM")

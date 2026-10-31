OUTPUT_ARCH(m68k)

/* Main CPU Module (MMD)
  The following should be defines as globals in your MMD program:
  MMD_DEST - This is the location to which your MMD program (minus the 0x100
  header) should be moved after it has been loaded into Word RAM. If it should
  run from Word RAM, where it was loaded, set this to 0.
  MODULE_ROM_ORIGIN - This is the base location from where your program will run.
  It should match MMD_DEST *IF* that value is not set to 0. If it is set to 0,
  set ORIGIN to 0x200100 (Word RAM) - the 0x100 accounts for the header space.
  MODULE_ROM_LENGTH - The full size of the memory block in which your program
  will run. This is NOT the length of the text section! (We need to think 
  about renaming things...) For example, if you are running from Word RAM,
  this should be (0x40000 - 0x100), for the whole 2M block minus the header
  space. 
  MODULE_RAM_ORIGIN - The base location for RAM storage
  MODULE_RAM_LENGTH - The size of the block for RAM storage; similar notes as the
  LENGTH section above
  MMD_TEXT_SZ - This is the size of the program data (text section)
*/

MEMORY
{
  /* Mega Drive RAM */
  /* 0xFFF700 and up is used by the system - BIOS, stack, vectors, etc*/
  MODULE_ROM (rx): ORIGIN = MODULE_ROM_ORIGIN, LENGTH = MODULE_ROM_LENGTH
  MODULE_RAM (rw): ORIGIN = MODULE_RAM_ORIGIN, LENGTH = MODULE_RAM_LENGTH
}

SECTIONS
{
  ENTRY(main)
  .header 0 : SUBALIGN(2)
  {
    SHORT(DEFINED(MMD_FLAGS) ? MMD_FLAGS : 0)
    LONG(MMD_DEST)
    SHORT(((_ROM_LENGTH + _RAM_DATA_LENGTH) >> 2) - 1)  /* counted in longs, minus 1 for the dbf loop */
    LONG(DEFINED(init) ? init : main)
    LONG(DEFINED(hblank) ? hblank : 0)
    LONG(DEFINED(vblank) ? vblank : 0)
    . = 0x100;
  }

  .rom MODULE_ROM_ORIGIN : AT ( ADDR(.header) + SIZEOF(.header) ) SUBALIGN(2)
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

  .ram_data : AT ( ADDR(.header) + SIZEOF(.header) + SIZEOF(.rom) ) SUBALIGN(2)
  {
    _RAM_DATA_ORIGIN = .;
    *(.data*)
    . = ALIGN(4);
    _RAM_DATA_LENGTH = ABSOLUTE(. - _RAM_DATA_ORIGIN);
    _RAM_DATA_LENGTH_LOOPSZ = ABSOLUTE(_RAM_DATA_LENGTH) >> 2;
  } > MODULE_RAM

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
  ASSERT(_STACK_BASE >= 0xFFFDB4 || _STACK_TOP <= 0xFFFD00, "stack overlaps the exception jump table, 0xFFFD00 to 0xFFFDB4")
}

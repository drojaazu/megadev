OUTPUT_ARCH(m68k)

/* Main CPU module (MMD)

  The module defines, usually with GLOBAL_SYM in C:
    MMD_DEST           Where the loader copies the module after it is loaded
                       into Word RAM; 0 to run it from Word RAM.
    MODULE_ROM_ORIGIN  Where the module's code runs: MMD_DEST, or 0x200100 in
                       Word RAM (after the 0x100-byte header) when MMD_DEST is 0.
    MODULE_ROM_LENGTH  The size of the memory available for code, not the size
                       of the code.
    MODULE_RAM_ORIGIN  Where the module's variables are placed.
    MODULE_RAM_LENGTH  The size of the memory available for variables.
  Optional: MMD_FLAGS, hblank and vblank handlers (written to the header),
  STACK_TOP and STACK_SIZE, MODULE_NO_INIT. Shared sections and checks are in
  module_common.ld.
*/

MEMORY
{
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

  _MODULE_ROM_LOAD = ADDR(.header) + SIZEOF(.header); /* .rom follows the header */
  INCLUDE module_common.ld

  /* The Boot ROM forwards exceptions through this table, with or without Main
  BIOS calls. */
  ASSERT(_STACK_BASE >= 0xFFFDB4 || _STACK_TOP <= 0xFFFD00, "stack overlaps the exception jump table, 0xFFFD00 to 0xFFFDB4")
}

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

  _MODULE_ROM_LOAD = ADDR(.header) + SIZEOF(.header); /* .rom follows the header */
  INCLUDE module_common.ld

  /* The Boot ROM forwards exceptions through this table, with or without Main
  BIOS calls. */
  ASSERT(_STACK_BASE >= 0xFFFDB4 || _STACK_TOP <= 0xFFFD00, "stack overlaps the exception jump table, 0xFFFD00 to 0xFFFDB4")
}

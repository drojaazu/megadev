/* Layout script for a MEMORY RESIDENT module.
   Identical to its transient counterpart except that the layout symbols are
   named RESIDENT_* rather than MODULE_*. A transient module that imports a
   resident module's symbols with `ld -R` would otherwise see that module's
   MODULE_ROM_ORIGIN and friends collide with its own. Distinct names remove
   the clash at its source rather than filtering it out afterwards. */

OUTPUT_ARCH(m68k)

/* Main CPU Module (MMD)
  The following should be defines as globals in your MMD program:
  RESIDENT_DEST - This is the location to which your MMD program (minus the 0x100
  header) should be moved after it has been loaded into Word RAM. If it should
  run from Word RAM, where it was loaded, set this to 0.
  RESIDENT_ROM_ORIGIN - This is the base location from where your program will run.
  It should match RESIDENT_DEST *IF* that value is not set to 0. If it is set to 0,
  set ORIGIN to 0x200100 (Word RAM) - the 0x100 accounts for the header space.
  RESIDENT_ROM_LENGTH - The full size of the memory block in which your program
  will run. This is NOT the length of the text section! (We need to think 
  about renaming things...) For example, if you are running from Word RAM,
  this should be (0x40000 - 0x100), for the whole 2M block minus the header
  space. 
  RESIDENT_RAM_ORIGIN - The base location for RAM storage
  RESIDENT_RAM_LENGTH - The size of the block for RAM storage; similar notes as the
  LENGTH section above
  MMD_TEXT_SZ - This is the size of the program data (text section)
*/

MEMORY
{
  /* Mega Drive RAM */
  /* 0xFFF700 and up is used by the system - BIOS, stack, vectors, etc*/
  RESIDENT_ROM (rx): ORIGIN = RESIDENT_ROM_ORIGIN, LENGTH = RESIDENT_ROM_LENGTH
  RESIDENT_RAM (rw): ORIGIN = RESIDENT_RAM_ORIGIN, LENGTH = RESIDENT_RAM_LENGTH
}

SECTIONS
{
  ENTRY(main)
  .header 0 : SUBALIGN(2)
  {
    SHORT(DEFINED(RESIDENT_FLAGS) ? RESIDENT_FLAGS : 0)
    LONG(RESIDENT_DEST)
    SHORT(((_ROM_LENGTH + _RAM_DATA_LENGTH) >> 2) - 1)  /* counted in longs, minus 1 for the dbf loop */
    LONG(DEFINED(init) ? init : main)
    LONG(DEFINED(hblank) ? hblank : 0)
    LONG(DEFINED(vblank) ? vblank : 0)
    . = 0x100;
  }

  .rom RESIDENT_ROM_ORIGIN : AT ( ADDR(.header) + SIZEOF(.header) ) SUBALIGN(2)
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
  } > RESIDENT_ROM
  

  .ram_bss RESIDENT_RAM_ORIGIN : SUBALIGN(2)
  {
    _BSS_ORIGIN = .;
    *(.bss*)
    . = ALIGN(4);
    _BSS_LENGTH = ABSOLUTE(. - _BSS_ORIGIN);
    _BSS_LENGTH_LOOPSZ = ABSOLUTE(_BSS_LENGTH) >> 2;
  } > RESIDENT_RAM

  .ram_data : AT ( ADDR(.header) + SIZEOF(.header) + SIZEOF(.rom) ) SUBALIGN(2)
  {
    _RAM_DATA_ORIGIN = .;
    *(.data*)
    . = ALIGN(4);
    _RAM_DATA_LENGTH = ABSOLUTE(. - _RAM_DATA_ORIGIN);
    _RAM_DATA_LENGTH_LOOPSZ = ABSOLUTE(_RAM_DATA_LENGTH) >> 2;
  } > RESIDENT_RAM

}

/* Layout script for a MEMORY RESIDENT module.
   Identical to its transient counterpart except that the layout symbols are
   named RESIDENT_* rather than MODULE_*. A transient module that imports a
   resident module's symbols with `ld -R` would otherwise see that module's
   MODULE_ROM_ORIGIN and friends collide with its own. Distinct names remove
   the clash at its source rather than filtering it out afterwards. */

OUTPUT_ARCH(m68k)

/* Sub CPU Module (SMD)
  Does not contain a header, but does specify ROM and RAM regions
*/

MEMORY
{
  RESIDENT_ROM (rx): ORIGIN = RESIDENT_ROM_ORIGIN, LENGTH = RESIDENT_ROM_LENGTH
  RESIDENT_RAM (rw): ORIGIN = RESIDENT_RAM_ORIGIN, LENGTH = RESIDENT_RAM_LENGTH
}

SECTIONS
{
  ENTRY(main)
  .rom RESIDENT_ROM_ORIGIN : SUBALIGN(2)
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

  .ram_data : AT ( ADDR(.rom) + SIZEOF(.rom) ) SUBALIGN(2)
  {
    _RAM_DATA_ORIGIN = .;
    *(.data*)
    . = ALIGN(4);
    _RAM_DATA_LENGTH = ABSOLUTE(. - _RAM_DATA_ORIGIN);
    _RAM_DATA_LENGTH_LOOPSZ = ABSOLUTE(_RAM_DATA_LENGTH) >> 2;
  } > RESIDENT_RAM
}

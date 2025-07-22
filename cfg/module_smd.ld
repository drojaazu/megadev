OUTPUT_ARCH(m68k)

/* Sub CPU Module (SMD)
  Does not contain a header, but does specify ROM and RAM regions
*/

MEMORY
{
  MODULE_ROM (rx): ORIGIN = MODULE_ROM_ORIGIN, LENGTH = MODULE_ROM_LENGTH
  MODULE_RAM (rw): ORIGIN = MODULE_RAM_ORIGIN, LENGTH = MODULE_RAM_LENGTH
}

SECTIONS
{
  ENTRY(main)
  .text MODULE_ROM_ORIGIN : 
  {
    _INIT_ORIGIN = .;
    *(.init)
    _INIT_LENGTH = ABSOLUTE(. - _INIT_ORIGIN);
    _TEXT_ORIGIN = .;
    *(.text*)
    _TEXT_LENGTH = ABSOLUTE(. - _TEXT_ORIGIN);
    _RODATA_ORIGIN = .;
    *(.rodata*)
    _RODATA_LENGTH = ABSOLUTE(. - _RODATA_ORIGIN);
    /* Align to 4 since moving data by longs is more efficient */
    . = ALIGN(4);
    _ROM_LENGTH = ABSOLUTE(. - _INIT_ORIGIN) >> 2;
    _ROM_DATA_ORIGIN = .;
    . = ALIGN(4);
  } > MODULE_ROM

  .data MODULE_RAM_ORIGIN : AT ( ADDR(.text) + SIZEOF(.text) )
  {
    _RAM_DATA_ORIGIN = .;
    *(.data*)
    . = ALIGN(4);
    _RAM_DATA_LENGTH = ABSOLUTE(. - _RAM_DATA_ORIGIN) >> 2;
  } > MODULE_RAM

  .bss :
  {
    _BSS_ORIGIN = .;
    *(.bss*)
    _BSS_LENGTH = ABSOLUTE(. - _BSS_ORIGIN) >> 2;
  } > MODULE_RAM

}

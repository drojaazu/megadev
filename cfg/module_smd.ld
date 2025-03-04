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
    _SMD_ROM_ORIGIN = .;
		*(.init)
    *(.text*)
    *(.rodata*)
		/* Align to 4 since moving data by longs is more efficient */
		. = ALIGN(4);
		_SMD_ROM_LEN = ABSOLUTE(. - _SMD_ROM_ORIGIN);
  } > MODULE_ROM

  .data MODULE_RAM_ORIGIN : AT ( ADDR(.text) + SIZEOF(.text) )
  {
		_SMD_RAM_ORIGIN = .;
		_SMD_RAM_DATA_ORIGIN = .;
		*(.data*)
		_SMD_RAM_DATA_LENGTH = ABSOLUTE(. - _SMD_RAM_DATA_ORIGIN);
  } > MODULE_RAM

	.bss :
	{
		_SMD_RAM_BSS_ORIGIN = .;
		*(.bss*)
		_SMD_RAM_BSS_LENGTH = ABSOLUTE(. - _SMD_RAM_BSS_ORIGIN);
	} > MODULE_RAM
	_SMD_RAM_LENGTH = _SMD_RAM_DATA_LENGTH + _SMD_RAM_BSS_LENGTH;

}

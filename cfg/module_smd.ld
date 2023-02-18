OUTPUT_ARCH(m68k)

/* Sub CPU Module (SMD)
	Does not contain a header, but does specify ROM and RAM regions
*/

MEMORY
{
	MODULE_ROM (wx): ORIGIN = MODULE_ROM_ORIGIN, LENGTH = MODULE_ROM_LENGTH
	MODULE_RAM (w): ORIGIN = MODULE_RAM_ORIGIN, LENGTH = MODULE_RAM_LENGTH
}

SECTIONS
{
	ENTRY(main)
  .text MODULE_ROM_ORIGIN : 
  {
    _mod_rom_org = .;
		*(.init)
		_mod_text_org = .;
    *(.text*)
		_mod_text_end = .;
		_mod_text_len = ABSOLUTE(. - _mod_text_org);
    *(.rodata*)
		/* Align to 4 since moving data by longs is more efficient */
		. = ALIGN(4);
		_mod_rom_end = .;
		_mod_rom_len = ABSOLUTE(. - _mod_rom_org);
  } > MODULE_ROM

  .data MODULE_RAM_ORIGIN : AT ( ADDR(.text) + SIZEOF(.text) )
  {
		_mod_ram_org = .;
		_mod_data_org = .;
		*(.data*)
		_mod_data_end = .;
		_mod_data_len = ABSOLUTE(. - _mod_data_org);
  } > MODULE_RAM

	.bss :
	{
		_mod_bss_org = .;
		*(.bss*)
		_mod_bss_end = .;
		_mod_bss_len = ABSOLUTE(. - _mod_bss_org);
	} > MODULE_RAM
	_mod_ram_len = _mod_data_len + _mod_bss_len;

}

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
	MODULE_ROM (wx): ORIGIN = MODULE_ROM_ORIGIN, LENGTH = MODULE_ROM_LENGTH
	MODULE_RAM (w): ORIGIN = MODULE_RAM_ORIGIN, LENGTH = MODULE_RAM_LENGTH
}

SECTIONS
{
	ENTRY(main)
	.header 0 :
	{
		SHORT(DEFINED(MMD_FLAGS) ? MMD_FLAGS : 0)
		LONG(MMD_DEST)
		SHORT(_mod_rom_len >> 2)
		LONG(main)
		LONG(DEFINED(hint) ? hint : 0)
		LONG(DEFINED(vint) ? vint : 0)
		. = 0x100;
	}

	.text MODULE_ROM_ORIGIN : AT ( ADDR(.header) + SIZEOF(.header) )
  {
    _mod_rom_org = .;
		*(.init)
		_mod_text_org = .;
    *(.text*)
		_mod_text_end = .;
		_mod_text_len = ABSOLUTE(. - _mod_text_org);
    *(.rodata*)
		/* Align to 4 since the MMD loader reads size in words */
		. = ALIGN(4);
		_mod_rom_end = .;
		_mod_rom_len = ABSOLUTE(. - _mod_rom_org);
  } > MODULE_ROM

  .data MODULE_RAM_ORIGIN : AT ( ADDR(.header) + SIZEOF(.header) + SIZEOF(.text) )
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

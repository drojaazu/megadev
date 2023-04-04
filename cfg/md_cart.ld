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
	rom (rx): ORIGIN = 0x000000, LENGTH = 0xFF0000
	ram (w): ORIGIN = 0xFF0000, LENGTH = 0x10000
}

SECTIONS
{
	ENTRY(main)
	.text 0x000000 :
  {
    _rom_org = .;
		_text_org = .;
    *(.text*)
		_text_end = .;
		_text_len = ABSOLUTE(. - _text_org);
    *(.rodata*)
		_rom_end = .;
		_rom_len = ABSOLUTE(. - _rom_org);
  } > rom

  .data 0xFF0000 :
  {
		_ram_org = .;
		_data_org = .;
		*(.data*)
		_data_end = .;
		_data_len = ABSOLUTE(. - _data_org);
  } > ram

	.bss :
	{
		_bss_org = .;
		*(.bss*)
		_bss_end = .;
		_bss_len = ABSOLUTE(. - _bss_org);
	} > ram
	_ram_len = _data_len + _bss_len;
	
}

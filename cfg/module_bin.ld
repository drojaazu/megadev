OUTPUT_ARCH(m68k)

/* Raw data module
	This is for raw, non-executable data compiled into one binary file. This is
	likely to be used for resources (graphics, sounds, palettes, etc)
	Specifying ROM origin/length is optional but may be useful for bounds
	checking.
*/

MEMORY
{
	MODULE_ROM (wx): ORIGIN = (DEFINED(MODULE_ROM_ORIGIN) ? MODULE_ROM_ORIGIN : 0), LENGTH = (DEFINED(MODULE_ROM_LENGTH) ? MODULE_ROM_LENGTH : 0xffffffff)
}

SECTIONS
{
  MODULE_ROM_ORIGIN : 
  {
    _mod_rom_org = .;
		*(*)
		. = ALIGN(4);
		_mod_rom_end = .;
		_mod_rom_len = ABSOLUTE(. - _mod_rom_org);
  } > MODULE_ROM

}

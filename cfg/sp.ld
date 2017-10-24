OUTPUT_ARCH(m68k)

MEMORY
{
  /*
    SP is loaded to 0x6000 by the Boot ROM.

		The length is user-definable, but should not exceed the maximum size
		allowed within the boot sector (28KB).

		Default size is 0x4000 (16KB) which is enough to include the 4KB directory
		cache and 2KB sector buffer used by the CD-ROM access, with space to spare.
  */
  SP (wx): ORIGIN = 0x6000, LENGTH = (DEFINED(SP_LENGTH) ? SP_LENGTH : 0x4000)
}

SECTIONS
{
  .text :
  {
		_sp_org = .;
    _sp_text_org = .;
    *(.header*)
    *(.text*)
    *(.rodata*)
		_sp_text_end = .;
    _sp_text_len = ABSOLUTE(. - _sp_text_org);
  } > SP

  .data : AT ( ADDR(.text) + SIZEOF(.text) )
  {
    _sp_data_org = .;
    *(.data*)
		_sp_data_end = .;
    _sp_data_len = ABSOLUTE(. - _sp_data_org);
  } > SP

  .bss :
  {
    _sp_bss_org = .;
    *(.bss*)
		_sp_bss_end = .;
    _sp_bss_len = ABSOLUTE(. - _sp_bss_org);
  } > SP
	_sp_end = .;
	_sp_ram_len = _sp_data_len + _sp_bss_len;

}

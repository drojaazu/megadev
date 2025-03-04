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
  SP (rx): ORIGIN = 0x6000, LENGTH = (DEFINED(SP_LENGTH) ? SP_LENGTH : 0x4000)
}

SECTIONS
{
  .text (READONLY) :
  {
		_SP_ROM_ORIGIN = .;
    *(.header*)
    *(.text*)
    *(.rodata*)
    _SP_ROM_LENGTH = ABSOLUTE(. - _SP_ROM_ORIGIN);
  } > SP

  .data : AT ( ADDR(.text) + SIZEOF(.text) )
  {
    _SP_RAM_ORIGIN = .;
    _SP_RAM_DATA_ORIGIN = .;
    *(.data*)
		_sp_data_end = .;
    _SP_RAM_DATA_LENGTH = ABSOLUTE(. - _SP_RAM_DATA_ORIGIN);
  } > SP

  .bss :
  {
    _SP_RAM_BSS_ORIGIN = .;
    *(.bss*)
    _SP_RAM_BSS_LENGTH = ABSOLUTE(. - _SP_RAM_BSS_ORIGIN);
  } > SP
  _SP_RAM_LENGTH = _SP_RAM_DATA_LENGTH + _SP_RAM_BSS_LENGTH;


}

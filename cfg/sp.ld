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
		_TEXT_ORIGIN = .;
    *(.header*)
    *(.text*)
    *(.rodata*)
    _TEXT_LENGTH = ABSOLUTE(. - _TEXT_ORIGIN);
  } > SP

  .data : AT ( ADDR(.text) + SIZEOF(.text) )
  {
    _RAM_ORIGIN = .;
    _RAM_DATA_ORIGIN = .;
    *(.data*)
		. = ALIGN(4);
    _RAM_DATA_LENGTH = ABSOLUTE(. - _RAM_DATA_ORIGIN) >> 2;
  } > SP

  .bss :
  {
    _BSS_ORIGIN = .;
    *(.bss*)
    . = ALIGN(4);
    _BSS_LENGTH = ABSOLUTE(. - _BSS_ORIGIN);
  } > SP
  _RAM_LENGTH = _RAM_DATA_LENGTH + _BSS_LENGTH;


}

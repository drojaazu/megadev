OUTPUT_ARCH(m68k)

MEMORY
{
  /*
    IP is loaded to 0xFF0000 by the Boot ROM.
		
		The length is user-definable, but should not exceed the maximum size
		allowed within the boot sector (3.5KB).

		Default size is 0xE00, the full size allowed within the boot sector.
  */
  IP (rwx): ORIGIN = 0xFF0000, LENGTH = (DEFINED(IP_LENGTH) ? IP_LENGTH : 0xE00)
}

SECTIONS
{
  .text (READONLY) :
  {
		_TEXT_ORIGIN = .;
    *(.text*)
    *(.rodata*)
    _TEXT_LENGTH = ABSOLUTE(. - _TEXT_ORIGIN);
  } > IP

  .data : AT ( ADDR(.text) + SIZEOF(.text) )
  {
    _RAM_ORIGIN = .;
    _DATA_ORIGIN = .;
    *(.data*)
    . = ALIGN(4);
    _DATA_LENGTH = ABSOLUTE(. - _DATA_ORIGIN) >> 2;
  } > IP

  .bss :
  {
    _BSS_ORIGIN = .;
    *(.bss*)
    . = ALIGN(4);
    _BSS_LENGTH = ABSOLUTE(. - _BSS_ORIGIN);
  } > IP
  _RAM_LENGTH = _DATA_LENGTH + _BSS_LENGTH >> 2;

}

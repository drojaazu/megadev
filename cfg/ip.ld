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
		_IP_ROM_ORIGIN = .;
    *(.text*)
    *(.rodata*)
    _IP_ROM_LENGTH = ABSOLUTE(. - _IP_ROM_ORIGIN);
  } > IP

  .data : AT ( ADDR(.text) + SIZEOF(.text) )
  {
    _IP_RAM_ORIGIN = .;
    _IP_RAM_DATA_ORIGIN = .;
    *(.data*)
    _IP_RAM_DATA_LENGTH = ABSOLUTE(. - _IP_RAM_DATA_ORIGIN);
  } > IP

  .bss :
  {
    _IP_RAM_BSS_ORIGIN = .;
    *(.bss*)
    _IP_RAM_BSS_LENGTH = ABSOLUTE(. - _IP_RAM_BSS_ORIGIN);
  } > IP
  _IP_RAM_LENGTH = _IP_RAM_DATA_LENGTH + _IP_RAM_BSS_LENGTH;

}

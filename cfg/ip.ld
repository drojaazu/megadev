OUTPUT_ARCH(m68k)

MEMORY
{
  /*
    IP is loaded to 0xFF0000 by the Boot ROM.
		
		The length is user-definable, but should not exceed the maximum size
		allowed within the boot sector (3.5KB).

		Default size is 0xE00, the full size allowed within the boot sector.
  */
  IP (wx): ORIGIN = 0xFF0000, LENGTH = (DEFINED(IP_LENGTH) ? IP_LENGTH : 0xE00)
}

SECTIONS
{
  .text :
  {
		_ip_org = .;
    _ip_text_org = .;
    *(.text*)
    *(.rodata*)
		_ip_text_end = .;
    _ip_text_len = ABSOLUTE(. - _ip_text_org);
  } > IP

  .data  : AT ( ADDR(.text) + SIZEOF(.text) )
  {
    _ip_data_org = .;
    *(.data*)
		_ip_data_end = .;
    _ip_data_len = ABSOLUTE(. - _ip_data_org);
  } > IP

  .bss :
  {
    _ip_bss_org = .;
    *(.bss*)
		_ip_bss_end = .;
    _ip_bss_len = ABSOLUTE(. - _ip_bss_org);
  } > IP
	_ip_end = .;

}

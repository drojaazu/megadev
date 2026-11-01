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
  .text (READONLY) : SUBALIGN(2)
  {
    _ROM_ORIGIN = .;
    /* The security block runs straight into the IP code, which nothing else
    references, so all of it is kept even with --gc-sections. */
    KEEP(*(.security*))
    _TEXT_ORIGIN = .;
    KEEP(*(.text*))
    _TEXT_LENGTH = ABSOLUTE(. - _TEXT_ORIGIN);
    _RODATA_ORIGIN = .;
    *(.rodata*)
    _RODATA_LENGTH = ABSOLUTE(. - _RODATA_ORIGIN);
    _ROM_LENGTH = ABSOLUTE(. - _ROM_ORIGIN);
  } > IP

  /* The Boot ROM loads the IP as one image, so initialized data is stored
  where it runs, right after the code. */
  .data : SUBALIGN(2)
  {
    _RAM_DATA_ORIGIN = .;
    *(.data*)
    . = ALIGN(4);
    _RAM_DATA_LENGTH = ABSOLUTE(. - _RAM_DATA_ORIGIN);
    _RAM_DATA_LENGTH_LOOPSZ = ABSOLUTE(_RAM_DATA_LENGTH) >> 2;
  } > IP
  .bss : SUBALIGN(2)
  {
    _BSS_ORIGIN = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _BSS_LENGTH = ABSOLUTE(. - _BSS_ORIGIN);
    _BSS_LENGTH_LOOPSZ = ABSOLUTE(_BSS_LENGTH) >> 2;
  } > IP
  _RAM_LENGTH = _RAM_DATA_LENGTH + _BSS_LENGTH;

  _ROM_DATA_ORIGIN = _RAM_DATA_ORIGIN; /* initial values are already in place */
  INCLUDE no_constructors.ld
}

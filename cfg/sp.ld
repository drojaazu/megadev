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
  .text (READONLY) : SUBALIGN(2)
  {
    HIDDEN(_ROM_ORIGIN = .);
    KEEP(*(.header*))
    HIDDEN(_TEXT_ORIGIN = .);
    *(.text*)
    HIDDEN(_TEXT_LENGTH = ABSOLUTE(. - _TEXT_ORIGIN));
    HIDDEN(_RODATA_ORIGIN = .);
    *(.rodata*)
    HIDDEN(_RODATA_LENGTH = ABSOLUTE(. - _RODATA_ORIGIN));
    HIDDEN(_ROM_LENGTH = ABSOLUTE(. - _ROM_ORIGIN));
  } > SP

  /* The Boot ROM loads the SP as one image, so initialized data is stored
  where it runs, right after the code. */
  .data : SUBALIGN(2)
  {
    HIDDEN(_RAM_DATA_ORIGIN = .);
    *(.data*)
    . = ALIGN(4);
    HIDDEN(_RAM_DATA_LENGTH = ABSOLUTE(. - _RAM_DATA_ORIGIN));
    HIDDEN(_RAM_DATA_LENGTH_LOOPSZ = ABSOLUTE(_RAM_DATA_LENGTH) >> 2);
  } > SP

  .bss : SUBALIGN(2)
  {
    HIDDEN(_BSS_ORIGIN = .);
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    HIDDEN(_BSS_LENGTH = ABSOLUTE(. - _BSS_ORIGIN));
    HIDDEN(_BSS_LENGTH_LOOPSZ = ABSOLUTE(_BSS_LENGTH) >> 2);
  } > SP
  HIDDEN(_RAM_LENGTH = _RAM_DATA_LENGTH + _BSS_LENGTH);

  HIDDEN(_ROM_DATA_ORIGIN = _RAM_DATA_ORIGIN); /* initial values are already in place */
  INCLUDE no_constructors.ld
}

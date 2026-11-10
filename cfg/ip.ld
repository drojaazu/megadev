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

/* Code and data in separate segments, each with its own permissions: one
segment holding both is read, write and execute, which the linker warns
about. The image is still loaded as one piece; only the ELF changes. */
PHDRS
{
  text PT_LOAD FLAGS(5); /* read, execute */
  data PT_LOAD FLAGS(6); /* read, write */
}

SECTIONS
{
  .text (READONLY) : SUBALIGN(2)
  {
    HIDDEN(_ROM_ORIGIN = .);
    /* The security block runs straight into the IP code, which nothing else
    references, so all of it is kept even with --gc-sections. */
    KEEP(*(.security*))
    HIDDEN(_TEXT_ORIGIN = .);
    KEEP(*(.text*))
    HIDDEN(_TEXT_LENGTH = ABSOLUTE(. - _TEXT_ORIGIN));
    HIDDEN(_RODATA_ORIGIN = .);
    *(.rodata*)
    HIDDEN(_RODATA_LENGTH = ABSOLUTE(. - _RODATA_ORIGIN));
    HIDDEN(_ROM_LENGTH = ABSOLUTE(. - _ROM_ORIGIN));
  } > IP :text

  /* The Boot ROM loads the IP as one image, so initialized data is stored
  where it runs, right after the code. */
  .data : SUBALIGN(2)
  {
    HIDDEN(_RAM_DATA_ORIGIN = .);
    *(.data*)
    . = ALIGN(4);
    HIDDEN(_RAM_DATA_LENGTH = ABSOLUTE(. - _RAM_DATA_ORIGIN));
    HIDDEN(_RAM_DATA_LENGTH_LOOPSZ = ABSOLUTE(_RAM_DATA_LENGTH) >> 2);
  } > IP :data
  .bss : SUBALIGN(2)
  {
    HIDDEN(_BSS_ORIGIN = .);
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    HIDDEN(_BSS_LENGTH = ABSOLUTE(. - _BSS_ORIGIN));
    HIDDEN(_BSS_LENGTH_LOOPSZ = ABSOLUTE(_BSS_LENGTH) >> 2);
  } > IP :data
  HIDDEN(_RAM_LENGTH = _RAM_DATA_LENGTH + _BSS_LENGTH);

  HIDDEN(_ROM_DATA_ORIGIN = _RAM_DATA_ORIGIN); /* initial values are already in place */
  INCLUDE no_constructors.ld
}

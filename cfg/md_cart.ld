OUTPUT_ARCH(m68k)

/* Cartridge ROM

  The project's init.s includes md_vectors.s, md_header.s and md_init.s and
  defines INIT_SSP (initial stack pointer) and INIT_PC (entry point). The
  vector table and header start the ROM; code and read-only data follow, then
  the initial values of .data, which md_init.s copies to RAM. Optional:
  STACK_SIZE, reserved below INIT_SSP when checking RAM use.
*/

MEMORY
{
	/* cartridge space; in Mode 1 the Mega CD Boot ROM follows at 0x400000 */
	rom (rx): ORIGIN = 0x000000, LENGTH = 0x400000
	ram (rw): ORIGIN = 0xFF0000, LENGTH = 0x10000
}

SECTIONS
{
	ENTRY(main)
	.text 0x000000 : SUBALIGN(2)
  {
    _rom_org = .;
		_text_org = .;
    /* the vector table and header (md_vectors.s, md_header.s) start the ROM */
    KEEP(*(.vectors))
    *(.text*)
		_text_end = .;
		_text_len = ABSOLUTE(. - _text_org);
    *(.rodata*)
		_rom_end = .;
		_rom_len = ABSOLUTE(. - _rom_org);
  } > rom

  /* Initialized data runs from RAM; its initial values are stored in ROM
  after .text and copied by md_init.s. */
  .data 0xFF0000 : AT ( LOADADDR(.text) + SIZEOF(.text) ) SUBALIGN(2)
  {
		_ram_org = .;
		_data_org = .;
		*(.data*)
		_data_end = .;
		_data_len = ABSOLUTE(. - _data_org);
  } > ram

	.bss : SUBALIGN(2)
	{
		_bss_org = .;
		*(.bss*)
		*(COMMON)
		_bss_end = .;
		_bss_len = ABSOLUTE(. - _bss_org);
	} > ram
	_ram_len = _data_len + _bss_len;
	_data_load = LOADADDR(.data);

	/* The stack grows down from INIT_SSP (0 is the top of RAM). The program's
	RAM must stay below it, and below STACK_SIZE bytes of stack if that is set. */
	_STACK_TOP = DEFINED(INIT_SSP) ? ((INIT_SSP - 1) & 0xFFFFFF) + 1 : 0x1000000;
	_STACK_BASE = _STACK_TOP - (DEFINED(STACK_SIZE) ? STACK_SIZE : 0);
	ASSERT(_bss_end <= _STACK_BASE, "the cartridge's RAM overlaps the stack (INIT_SSP, STACK_SIZE)")
	/* Mode 1 keeps the system vector table at 0xFFFD00 to 0xFFFDB4
	(main/mode1.s). */
	ASSERT(!DEFINED(mode1_init_vectors) || (_bss_end <= 0xFFFD00 && (_STACK_TOP <= 0xFFFD00 || _STACK_BASE >= 0xFFFDB4)), "Mode 1: the cartridge's RAM or stack overlaps the system vector table, 0xFFFD00 to 0xFFFDB4")
	

  INCLUDE no_constructors.ld
}

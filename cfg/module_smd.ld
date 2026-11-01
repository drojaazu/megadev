OUTPUT_ARCH(m68k)

/* Sub CPU Module (SMD)
  Does not contain a header, but does specify ROM and RAM regions
*/

MEMORY
{
  MODULE_ROM (rx): ORIGIN = MODULE_ROM_ORIGIN, LENGTH = MODULE_ROM_LENGTH
  MODULE_RAM (rw): ORIGIN = MODULE_RAM_ORIGIN, LENGTH = MODULE_RAM_LENGTH
}

SECTIONS
{
  ENTRY(main)
  _MODULE_ROM_LOAD = MODULE_ROM_ORIGIN; /* no header: .rom starts the file */
  INCLUDE module_common.ld
}

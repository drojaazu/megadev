OUTPUT_ARCH(m68k)

/* Sub CPU module (SMD)

  No header: the file is the code, followed by the initial values of .data.
  The module defines MODULE_ROM_ORIGIN, MODULE_ROM_LENGTH, MODULE_RAM_ORIGIN
  and MODULE_RAM_LENGTH as for an MMD (see module_mmd.ld). Shared sections and
  checks are in module_common.ld.
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

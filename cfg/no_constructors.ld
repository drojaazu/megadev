/* Megadev has no code that runs static constructors or destructors, so a
program that needs them fails to link instead of silently skipping them.
Included inside the SECTIONS command of every linker script. */

.constructors (INFO) :
{
  *(.preinit_array*)
  *(.init_array*)
  *(.fini_array*)
  *(.ctors*)
  *(.dtors*)
}
ASSERT(SIZEOF(.constructors) == 0, "static constructors and destructors are not supported: C++ global objects with constructors or destructors, or __attribute__((constructor)) / ((destructor))")

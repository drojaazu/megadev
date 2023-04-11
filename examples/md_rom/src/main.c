#include "main/bootlib.h"
#include "main/io.h"
#include "main/memmap.h"
#include "system.h"
#include "types.h"

u8 p1_in;
u8 p2_in;

void testing()
{
	p2_in = IO_DATA2;
}

__attribute__((noreturn)) void main()
{
	MLEVEL6_VECTOR = (void *(*) ) _BLIB_VINT_HANDLER;
	enable_interrupts();
	while (true)
	{
		p1_in = IO_DATA1;
	}
}
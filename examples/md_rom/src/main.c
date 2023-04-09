#include "main/io.h"
#include "types.h"

u8 p1_in;
u8 p2_in;

void testing()
{
	p2_in = IO_DATA2;
}

__attribute__((noreturn)) void main()
{

	while (true)
	{
		p1_in = IO_DATA1;
	}
}

#include "bridge.h"
#include <sub/cdrom.h>
#include <sub/gate_arr.h>
#include <sub/memmap.h>
#include <system.h>

void load_ipx();

extern void sp_fatal();

char const * const filenames[] = {
	"IPX.MMD;1", "EX1.MMD;1", "EX2.MMD;1", "EX3.MMD;1"};

// It's a good idea to put SPX's main in .init to ensure it's at the very start
// of the code, since we jump to where we expect it to be in memory
void main()
{
	enable_interrupts();
	register u16 command, param1;
	do
	{

		do
		{
			command = *gareg_comcmd0;
		} while (command == 0);

		if (command != *gareg_comcmd0)
			continue;

		param1 = *gareg_comcmd1;

		switch (command)
		{

			// load MMD
			case CMD_LOAD_FILE:
				load_file(CDROM_LOAD_CDC, filenames[param1], (u8 *) WORD_RAM_2M);
				grant_2m();
				if (access_op_result != CDROM_RESULT_OK)
				{
					sp_fatal();
				}
				break;
		}

		// not reaching here?
		asm(".global test_label3\ntest_label3:");
		*GA_COMSTAT0 = *gareg_comcmd0;
		do
		{
			asm("nop");
			command = *gareg_comcmd0;
		} while (command != 0);

		do
		{
			asm("nop");
			command = *gareg_comcmd0;
		} while (command != 0);

		*GA_COMSTAT0 = 0;

	} while (1);
}

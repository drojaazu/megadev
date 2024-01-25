
#include "sub/cdrom.h"
#include "sub/gatearray.h"
#include "sub/memmap.h"

void load_ipx();

extern void sp_fatal();

char const * const filenames[] = {"EX1.MMD;1", "EX2.MMD;1", "EX3.MMD;1"};

// It's a good idea to put SPX's main in .init to ensure it's at the very start
// of the code, since we jump to where we expect it to be in memory
__attribute__((section(".init"))) void main()
{
	register u16 cmd0, cmd1;
	do
	{

		do
		{
			cmd0 = *GA_COMCMD0;
		} while (cmd0 == 0);

		if (cmd0 != *GA_COMCMD0)
			continue;

		cmd1 = *GA_COMCMD1;

		switch (cmd0)
		{

			// load MMD
			case 1:
				load_file(ACC_OP_LOAD_CDC, filenames[cmd1], (u8 *) _WRDRAM_2M);
				grant_2m();
				if (access_op_result != RESULT_OK)
				{
					sp_fatal();
				}
				break;

			// load IPX
			case 0xfe:
				load_file(ACC_OP_LOAD_CDC, "IPX.MMD;1", (u8 *) _WRDRAM_2M);
				grant_2m();
				if (access_op_result != RESULT_OK)
				{
					sp_fatal();
				}
				break;
		}

		// not reaching here?
		__asm__(".global test_label\ntest_label:");
		*GA_COMSTAT0 = *GA_COMCMD0;
		do
		{
			cmd0 = *GA_COMCMD0;
		} while (cmd0 != 0);

		do
		{
			cmd0 = *GA_COMCMD0;
		} while (cmd0 != 0);

		*GA_COMSTAT0 = 0;

	} while (1);
}

void load_ipx() {}

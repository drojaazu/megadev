#include "main/bootlib.h"
#include "main/gatearr.h"
#include "main/io_def.h"
#include "main/memmap.h"
#include "main/mmd_exec.h"
#include "main/printval.h"
#include "main/vdp.h"
#include "system.h"

#define SUB_WAIT \
	do \
	{ \
		asm("nop"); \
	} while (*GA_COMSTAT0 == 0);

#define SUB_ACK *GA_COMCMD0 = 0;

#define print_xy(x, y) \
	(VDPPTR (NMT_POS_PLANE (x, y, _BLIB_PLANEA_ADDR)) | VRAM_W)

char val_buffer[5];

// we'll need this information when we go to load actual file data
u16 filesize;
u16 is_protected;

static void braminit_test()
{
	blib_print ("Performing BRMINIT...\xff", print_xy (1, 1));

	*GA_COMCMD0 = 2;

	SUB_WAIT

	// bram_size is the total space on the unit, returned as the number of
	// 4KB (0x1000 byte) blocks. So we multiplay by 0x1000 to get the 'actual'
	// total space available
	int bram_size = (*GA_COMSTAT1) * 0x1000;
	int status = *GA_COMSTAT2;

	SUB_ACK

	blib_print ("Total Size: \xff", print_xy (1, 2));

	printval_u16_c (bram_size, val_buffer);

	blib_print (val_buffer, print_xy (13, 3));

	blib_print ("Status: \xff", print_xy (1, 3));

	printval_u16_c (status, val_buffer);

	blib_print (val_buffer, print_xy (13, 2));

	switch (status)
	{
		case 0:
			blib_print ("BRAM not present or not detected!\xff", print_xy (1, 4));
			break;
		case 1:
			blib_print ("BRAM is not formatted!\xff", print_xy (1, 4));
			break;
		case 2:
			blib_print ("BRAM is in a non-standard format!\xff", print_xy (1, 4));
			break;
		case 3:
			blib_print ("BRAM is formatted and ready to use!\xff", print_xy (1, 4));
			break;
	}
}

static void brmstat_test()
{
	blib_clear_tables();
	blib_print ("Performing BRMSTAT...\xff", print_xy (1, 1));

	*GA_COMCMD0 = 6;

	SUB_WAIT

	// bram_size is the total space on the unit, returned as the number of
	// 4KB (0x1000 byte) blocks. So we multiplay by 0x1000 to get the 'actual'
	// total space available
	u16 filecount = *GA_COMSTAT1;
	u16 free = *GA_COMSTAT2;

	SUB_ACK

	blib_print ("File count: \xff", print_xy (1, 3));
	printval_u16_c (filecount, val_buffer);
	blib_print (val_buffer, print_xy (13, 3));

	blib_print ("Free blocks: \xff", print_xy (1, 4));
	printval_u16_c (free, val_buffer);
	blib_print (val_buffer, print_xy (13, 4));
}

static bool brmserch_test()
{
	blib_print ("Performing BRMSERCH...\xff", print_xy (1, 1));

	*GA_COMCMD0 = 3;

	SUB_WAIT

	u16 found = *GA_COMSTAT1;
	filesize = *GA_COMSTAT2;
	is_protected = *GA_COMSTAT3;

	SUB_ACK

	if (found == 0xffff)
	{
		blib_print ("File BRMEX not found\xff", print_xy (1, 2));
		return false;
	}
	else
	{
		blib_print ("File BRMEX found\xff", print_xy (1, 2));

		blib_print (" File size: \xff", print_xy (1, 3));
		printval_u16_c (filesize, val_buffer);
		blib_print (val_buffer, print_xy (13, 3));

		blib_print (" Protected? \xff", print_xy (1, 4));
		printval_u16_c (is_protected, val_buffer);
		blib_print (val_buffer, print_xy (13, 4));
		return true;
	}
}

static void brmwrite_test()
{
	blib_clear_tables();

	u16 writeval = 0;

	blib_print ("Set a value to write: \xff", print_xy (1, 2));
	u8 delayed_input_val = 0;

	do
	{
		blib_vint_wait_default();

		// this is a great opportunity to show off the blib_input_delay function
		// in the boot rom library
		// to use it, we need to specify a byte which will hold the input value
		// and use that byte to compare for input instead of the standard memory
		// location; it will need to be called in a loop along with the vint
		// wait

		// now we get a nice, smooth value change when holding up or down instead
		// of the value changing way too quickly
		blib_input_delay (&delayed_input_val, false);

		if (delayed_input_val & PAD_UP_MSK)
		{
			++writeval;
		}
		if (delayed_input_val & PAD_DOWN_MSK)
		{
			--writeval;
		}

		if (delayed_input_val & PAD_RIGHT_MSK)
		{
			writeval += 0x100;
		}
		if (delayed_input_val & PAD_LEFT_MSK)
		{
			writeval -= 0x100;
		}

		printval_u16_c (writeval, val_buffer);
		blib_print (val_buffer, print_xy (23, 2));
	} while (! (BLIB_JOY1_PRESS & PAD_START_MSK));

	*(u16 *) WRDRAM = writeval;

	grant_2m();

	*GA_COMCMD0 = 5;

	SUB_WAIT

	int success = *GA_COMSTAT1;

	SUB_ACK

	wait_2m();

	if (success == 0xffff)
	{
		blib_print ("Error while writing file\xff", print_xy (1, 4));
	}
	else
	{
		blib_print ("Write successful\xff", print_xy (1, 4));
	}
}

static void brmread_test()
{
	blib_clear_tables();

	if (! brmserch_test())
		return;

	blib_print ("Performing BRMREAD...\xff", print_xy (1, 6));

	grant_2m();

	*GA_COMCMD0 = 4;

	SUB_WAIT

	int success = *GA_COMSTAT1;
	int size = *GA_COMSTAT2;
	int mode = *GA_COMSTAT3;

	SUB_ACK

	wait_2m();

	if (success == 0xffff)
	{
		blib_print ("Error reading file\xff", print_xy (1, 7));
		return;
	}

	u16 readval = *(u16 *) WRDRAM;

	blib_print ("Success reading file\xff", print_xy (1, 7));

	blib_print ("Value read: \xff", print_xy (1, 9));

	printval_u16_c (readval, val_buffer);

	blib_print (val_buffer, print_xy (12, 9));
}

void bram_brmdel_test()
{
	blib_clear_tables();

	blib_print ("Performing BRMDEL...\xff", print_xy (1, 1));

	*GA_COMCMD0 = 7;

	SUB_WAIT

	int success = *GA_COMSTAT1;

	SUB_ACK

	if (success == 0xffff)
	{
		blib_print ("Failed to delete file\xff", print_xy (1, 3));
	}
	else
	{
		blib_print ("File deleted successfully\xff", print_xy (1, 3));
	}
}

void bram_brmdir_test()
{
	blib_clear_tables();

	blib_print ("Performing BRMDIR...\xff", print_xy (1, 1));

	grant_2m();

	*GA_COMCMD0 = 8;

	SUB_WAIT

	int success = *GA_COMSTAT1;

	SUB_ACK

	wait_2m();

	if (success == 0xffff)
	{
		blib_print ("Data too big for buffer\xff", print_xy (1, 3));
		return;
	}

	blib_print ("Filename       Blocks\xff", print_xy (1, 3));

	u16 file_idx = 0;
	u16 curr_y = 4;
	char * filename;
	while (1)
	{
		if (*(u8 *) (WRDRAM + (file_idx * 16)) == 0)
			break;
		filename = (char *) (WRDRAM + (file_idx * 16));
		filename[11] = 0xff;
		blib_print (filename, print_xy (1, curr_y));
		printval_u16_c (*(u16 *) (WRDRAM + (file_idx * 16) + 14), val_buffer);
		blib_print (val_buffer, print_xy (16, curr_y));
		++curr_y;
		++file_idx;
	}
}

s8 menupos;
void main()
{
	// repoint the VINT vector to the boot rom library version
	MLEVEL6_VECTOR = (void *(*) ) _BLIB_VINT_HANDLER;
	// do not re-enable too quickly!!
	enable_interrupts();

	// setup boot rom library font
	blib_load_font_defaults();
	VDP_CTRL_32 = 0xC0020000;
	VDP_DATA_16 = 0x0EEE;

	// print strings end in 0xff, so let's fix up the value buffer so we don't
	// have to mess with it later
	val_buffer[4] = '\xff';

	braminit_test();
	blib_print ("Press Start to continue...\xff", print_xy (1, 10));

	do
	{
		blib_vint_wait_default();
	} while (! (BLIB_JOY1_PRESS & PAD_START_MSK));

	menupos = 0;

	do
	{
		blib_clear_tables();

		blib_print ("BRAM Options\xff", print_xy (1, 1));
		blib_print ("BRMSTAT\xff", print_xy (3, 3));
		blib_print ("BRMDIR\xff", print_xy (3, 4));
		blib_print ("BRMREAD\xff", print_xy (3, 5));
		blib_print ("BRMWRITE\xff", print_xy (3, 6));
		blib_print ("BRMVERIFY\xff", print_xy (3, 7));
		blib_print ("BRMDEL\xff", print_xy (3, 8));
		blib_print ("BRMFORMAT\xff", print_xy (3, 9));

		do
		{
			blib_vint_wait_default();
			for (int clearloop = 3; clearloop < 10; ++clearloop)
			{
				VDP_CTRL_32 =
					(VDPPTR (NMT_POS_PLANE (1, clearloop, _BLIB_PLANEA_ADDR)) | VRAM_W);
				VDP_DATA_16 = 0;
			}
			VDP_CTRL_32 =
				(VDPPTR (NMT_POS_PLANE (1, (menupos + 3), _BLIB_PLANEA_ADDR)) | VRAM_W);
			VDP_DATA_16 = '>';

			if ((BLIB_JOY1_PRESS & PAD_DOWN_MSK))
			{
				++menupos;
				if (menupos > 6)
					menupos = 0;
				continue;
			}

			if ((BLIB_JOY1_PRESS & PAD_UP_MSK))
			{
				--menupos;
				if (menupos < 0)
					menupos = 6;
				continue;
			}
		} while (! (BLIB_JOY1_PRESS & PAD_START_MSK));

		switch (menupos)
		{
			case 0:
				brmstat_test();
				break;
			case 1:
				bram_brmdir_test();
				break;
			case 2:
				brmread_test();
				break;
			case 3:
				brmwrite_test();
				break;
			case 4:
				break;
			case 5:
				bram_brmdel_test();
				break;
			case 6:
				break;
		}

		blib_print ("Press Start to continue...\xff", print_xy (1, 12));

		do
		{
			blib_vint_wait_default();
		} while (! (BLIB_JOY1_PRESS & PAD_START_MSK));

	} while (1);
}

#include <main/bios.h>
#include <main/gate_array.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/init_mmd.h>
#include <printval.h>
#include <main/vdp.h>
#include <system.h>
#include <types.h>

#define SUB_WAIT \
	do \
	{ \
		asm("nop"); \
	} while (*GA_COMSTAT0 == 0);

#define SUB_ACK *GA_COMCMD0 = 0;

#define print_xy(x, y) (VDPPTR(NMT_POS_PLANE(x, y, _BIOS_PLANEA_ADDR)) | VRAM_W)

char val_buffer[5];

static void bram_init()
{
	bios_print("Performing BRMINIT...\xff", print_xy(1, 1));

	*GA_COMCMD0 = 2;

	SUB_WAIT

	// bram_size is the total space on the unit, returned as the number of
	// 4KB (0x1000 byte) blocks. So we multiplay by 0x1000 to get the 'actual'
	// total space available
	int bram_size = (*GA_COMSTAT1) * 0x1000;
	int status = *GA_COMSTAT2;

	SUB_ACK

	bios_print("Total Size: \xff", print_xy(1, 2));

	printval_u16_c(bram_size, val_buffer);

	bios_print(val_buffer, print_xy(13, 3));

	bios_print("Status: \xff", print_xy(1, 3));

	printval_u16_c(status, val_buffer);

	bios_print(val_buffer, print_xy(13, 2));

	switch (status)
	{
		case 0:
			bios_print("BRAM not present or not detected!\xff", print_xy(1, 4));
			break;
		case 1:
			bios_print("BRAM is not formatted!\xff", print_xy(1, 4));
			break;
		case 2:
			bios_print("BRAM is in a non-standard format!\xff", print_xy(1, 4));
			break;
		case 3:
			bios_print("BRAM is formatted and ready to use!\xff", print_xy(1, 4));
			break;
	}
}

static void brmstat()
{
	bios_clear_tables();
	bios_print("Performing BRMSTAT...\xff", print_xy(1, 1));

	*GA_COMCMD0 = 6;

	SUB_WAIT

	// bram_size is the total space on the unit, returned as the number of
	// 4KB (0x1000 byte) blocks. So we multiplay by 0x1000 to get the 'actual'
	// total space available
	u16 filecount = *GA_COMSTAT1;
	u16 free = *GA_COMSTAT2;

	SUB_ACK

	bios_print("File count: \xff", print_xy(1, 3));
	printval_u16_c(filecount, val_buffer);
	bios_print(val_buffer, print_xy(13, 3));

	bios_print("Free blocks: \xff", print_xy(1, 4));
	printval_u16_c(free, val_buffer);
	bios_print(val_buffer, print_xy(13, 4));
}

static bool brmserch()
{
	bios_print("Performing BRMSERCH...\xff", print_xy(1, 1));

	*GA_COMCMD0 = 3;

	SUB_WAIT

	u16 found = *GA_COMSTAT1;
	u16 filesize = *GA_COMSTAT2;
	u16 is_protected = *GA_COMSTAT3;

	SUB_ACK

	if (found == 0xffff)
	{
		bios_print("File BRMEX not found\xff", print_xy(1, 2));
		return false;
	}
	else
	{
		bios_print("File BRMEX found\xff", print_xy(1, 2));

		bios_print(" File size: \xff", print_xy(1, 3));
		printval_u16_c(filesize, val_buffer);
		bios_print(val_buffer, print_xy(13, 3));

		bios_print(" Protected? \xff", print_xy(1, 4));
		printval_u16_c(is_protected, val_buffer);
		bios_print(val_buffer, print_xy(13, 4));
		return true;
	}
}

static void brmwrite()
{
	bios_clear_tables();

	u16 writeval = 0;

	bios_print("Set a value to write: \xff", print_xy(1, 2));
	u8 delayed_input_val = 0;

	do
	{
		bios_vint_wait_default();

		// this is a great opportunity to show off the bios_input_delay function
		// in the boot rom library
		// to use it, we need to specify a byte which will hold the input value
		// and use that byte to compare for input instead of the standard memory
		// location; it will need to be called in a loop along with the vint
		// wait

		// now we get a nice, smooth value change when holding up or down instead
		// of the value changing way too quickly
		bios_input_delay(&delayed_input_val, false);

		if (delayed_input_val & PAD_UP)
		{
			++writeval;
		}
		if (delayed_input_val & PAD_DOWN)
		{
			--writeval;
		}

		if (delayed_input_val & PAD_RIGHT)
		{
			writeval += 0x100;
		}
		if (delayed_input_val & PAD_LEFT)
		{
			writeval -= 0x100;
		}

		printval_u16_c(writeval, val_buffer);
		bios_print(val_buffer, print_xy(23, 2));
	} while (! (BIOS_JOY1_PRESS & PAD_START));

	*(u16 *) WRDRAM = writeval;

	grant_2m();

	*GA_COMCMD0 = 5;

	SUB_WAIT

	int success = *GA_COMSTAT1;

	SUB_ACK

	wait_2m();

	if (success == 0xffff)
	{
		bios_print("Error while writing file\xff", print_xy(1, 4));
	}
	else
	{
		bios_print("Write successful\xff", print_xy(1, 4));
	}
}

static void brmread()
{
	bios_clear_tables();

	if (! brmserch())
		return;

	bios_print("Performing BRMREAD...\xff", print_xy(1, 6));

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
		bios_print("Error reading file\xff", print_xy(1, 7));
		return;
	}

	u16 readval = *(u16 *) WRDRAM;

	bios_print("Success reading file\xff", print_xy(1, 7));

	bios_print("Value read: \xff", print_xy(1, 9));

	printval_u16_c(readval, val_buffer);

	bios_print(val_buffer, print_xy(12, 9));
}

void brmdel()
{
	bios_clear_tables();

	bios_print("Performing BRMDEL...\xff", print_xy(1, 1));

	*GA_COMCMD0 = 7;

	SUB_WAIT

	int success = *GA_COMSTAT1;

	SUB_ACK

	if (success == 0xffff)
	{
		bios_print("Failed to delete file\xff", print_xy(1, 3));
	}
	else
	{
		bios_print("File deleted successfully\xff", print_xy(1, 3));
	}
}

void brmdir()
{
	bios_clear_tables();

	bios_print("Performing BRMDIR...\xff", print_xy(1, 1));

	grant_2m();

	*GA_COMCMD0 = 8;

	SUB_WAIT

	int success = *GA_COMSTAT1;

	SUB_ACK

	wait_2m();

	if (success == 0xffff)
	{
		bios_print("Data too big for buffer\xff", print_xy(1, 3));
		return;
	}

	bios_print("Filename       Blocks\xff", print_xy(1, 3));

	u16 file_idx = 0;
	u16 curr_y = 4;
	char * filename;
	while (1)
	{
		if (*(u8 *) (WRDRAM + (file_idx * 16)) == 0)
			break;
		filename = (char *) (WRDRAM + (file_idx * 16));
		filename[11] = 0xff;
		bios_print(filename, print_xy(1, curr_y));
		printval_u16_c(*(u16 *) (WRDRAM + (file_idx * 16) + 14), val_buffer);
		bios_print(val_buffer, print_xy(16, curr_y));
		++curr_y;
		++file_idx;
	}
}

void main()
{
	// setup boot rom library font
	bios_load_font_defaults();
	VDP_CTRL_32 = 0xC0020000;
	VDP_DATA_16 = 0x0EEE;

	// print strings end in 0xff, so let's set up the value buffer so we don't
	// have to mess with it later
	val_buffer[4] = '\xff';

	// interrupts were disabled in the init section
	enable_interrupts();

	bram_init();
	bios_print("Press Start to continue...\xff", print_xy(1, 10));

	do
	{
		bios_vint_wait_default();
	} while (! (BIOS_JOY1_PRESS & PAD_START));

	s8 menupos = 0;

	do
	{
		bios_clear_tables();

		bios_print("BRAM Options\xff", print_xy(1, 1));
		bios_print("BRMSTAT\xff", print_xy(3, 3));
		bios_print("BRMDIR\xff", print_xy(3, 4));
		bios_print("BRMREAD\xff", print_xy(3, 5));
		bios_print("BRMWRITE\xff", print_xy(3, 6));
		bios_print("BRMVERIFY\xff", print_xy(3, 7));
		bios_print("BRMDEL\xff", print_xy(3, 8));
		bios_print("BRMFORMAT\xff", print_xy(3, 9));

		do
		{
			bios_vint_wait_default();
			for (int clearloop = 3; clearloop < 10; ++clearloop)
			{
				VDP_CTRL_32 = (VDPPTR(NMT_POS_PLANE(1, clearloop, _BIOS_PLANEA_ADDR)) | VRAM_W);
				VDP_DATA_16 = 0;
			}
			VDP_CTRL_32 = (VDPPTR(NMT_POS_PLANE(1, (menupos + 3), _BIOS_PLANEA_ADDR)) | VRAM_W);
			VDP_DATA_16 = '>';

			if ((BIOS_JOY1_PRESS & PAD_DOWN))
			{
				++menupos;
				if (menupos > 6)
					menupos = 0;
				continue;
			}

			if ((BIOS_JOY1_PRESS & PAD_UP))
			{
				--menupos;
				if (menupos < 0)
					menupos = 6;
				continue;
			}
		} while (! (BIOS_JOY1_PRESS & PAD_START));

		switch (menupos)
		{
			case 0:
				brmstat();
				break;
			case 1:
				brmdir();
				break;
			case 2:
				brmread();
				break;
			case 3:
				brmwrite();
				break;
			case 4:
				break;
			case 5:
				brmdel();
				break;
			case 6:
				break;
		}

		bios_print("Press Start to continue...\xff", print_xy(1, 12));

		do
		{
			bios_vint_wait_default();
		} while (! (BIOS_JOY1_PRESS & PAD_START));

	} while (1);
}

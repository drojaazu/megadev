
#include "bridge.h"
#include "xform.res.h"
#include <memory.h>
#include <sub/cdrom.h>
#include <sub/gate_arr.h>
#include <sub/memmap.h>
#include <system.h>

#define STAMP_DATA	0x0
#define STAMP_MAP		0x10000
#define TRACE_TABLE 0x20000
#define IMG_BUFFER	0x30000
#define IMG_WIDTH		256
#define IMG_HEIGHT	256

u16 (*trace_table)[IMG_HEIGHT][4] =
	(u16(*)[IMG_HEIGHT][4])(WORD_RAM_2M + TRACE_TABLE);

extern void sp_fatal();

char const * const filenames[] = {"IPX.MMD;1"};

void redraw();

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

			case CMD_REDRAW:
				redraw();
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

void redraw()
{
	wait_2m();
	// GA_STAMPSIZE
	// GA_STAMPMAPBASE
	// GA_IMGBUFVSIZE
	// GA_IMGBUFSTART
	// GA_IMGBUFOFFSET
	// GA_IMGBUFHDOTSIZE
	// GA_IMGBUFVDOTSIZE
	// GA_TRACEVECTBASE
	memcpy16(
		(u16 const *) &res_stamp01.data,
		(u16 *) (WORD_RAM_2M + STAMP_DATA),
		res_stamp01.size / 2);

	memcpy16(
		(u16 const *) &res_stamp_map.data,
		(u16 *) (WORD_RAM_2M + STAMP_MAP),
		res_stamp_map.size / 2);

	GA_STAMPSIZE = _GAREG_STAMPSIZE_REPEAT | _GAREG_STAMPSIZE_32x32_STAMP;
	GA_STAMPMAPBASE = (u16) ((WORD_RAM_2M + STAMP_MAP) / 4);
	GA_IMGBUFSTART = (u16) ((WORD_RAM_2M + IMG_BUFFER) / 4);
	GA_IMGBUFVSIZE = 31;
	GA_IMGBUFVDOTSIZE = IMG_HEIGHT;
	GA_IMGBUFHDOTSIZE = IMG_WIDTH;

	for (u16 trace_table_line = 0; trace_table_line < IMG_HEIGHT;
			 ++trace_table_line)
	{
		(*trace_table)[trace_table_line][0] = 0;
		(*trace_table)[trace_table_line][1] = trace_table_line;
		(*trace_table)[trace_table_line][2] = trace_table_line + 1;
		(*trace_table)[trace_table_line][3] = 0;
	}

	GA_TRACEVECTBASE = (u16) (TRACE_TABLE / 4);
}
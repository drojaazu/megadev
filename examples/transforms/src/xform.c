#include "xform.res.h"
#include <memory.h>
#include <sub/gate_arr.h>
#include <sub/memmap.def.h>
#include <types.h>

#define STAMP_DATA	0x0
#define STAMP_MAP		0x10000
#define TRACE_TABLE 0x20000
#define IMG_BUFFER	0x30000
#define IMG_WIDTH		256
#define IMG_HEIGHT	256

u16 trace_table[IMG_HEIGHT][4];

__attribute__((noreturn)) void main()
{

	// GA_STAMPSIZE
	// GA_STAMPMAPBASE
	// GA_IMGBUFVSIZE
	// GA_IMGBUFSTART
	// GA_IMGBUFOFFSET
	// GA_IMGBUFHDOTSIZE
	// GA_IMGBUFVDOTSIZE
	// GA_TRACEVECTBASE
	memcpy16(res_stamp01.data, (WORD_RAM_2M + STAMP_DATA), res_stamp01.size / 2);

	memcpy16(
		res_stamp_map.data, (WORD_RAM_2M + STAMP_MAP), res_stamp_map.size / 2);

	GA_STAMPSIZE = _GAREG_STAMPSIZE_REPEAT | _GAREG_STAMPSIZE_32x32_STAMP;
	GA_STAMPMAPBASE = (WORD_RAM_2M + STAMP_MAP) / 4;
	GA_IMGBUFSTART = (WORD_RAM_2M + IMG_BUFFER) / 4;
	GA_IMGBUFVSIZE = 31;
	GA_IMGBUFVDOTSIZE = IMG_HEIGHT;
	GA_IMGBUFHDOTSIZE = IMG_WIDTH;

	for (u16 trace_table_line = 0; trace_table_line < IMG_HEIGHT;
			 ++trace_table_line)
	{
	}
}
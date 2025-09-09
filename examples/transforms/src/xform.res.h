
#include "types.h"
#include <main/bios.h>

typedef struct
{
	u32 size;
	u8	data[];
} DataChunk;

extern DataChunk const res_stamp_map;

extern DataChunk const res_stamp01;



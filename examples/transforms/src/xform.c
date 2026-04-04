#include "xform.res.h"
#include <memory.h>
#include <sub/gate_arr.h>
#include <sub/memmap.def.h>
#include <types.h>

#define STAMP_DATA  0x0
#define STAMP_MAP   0x10000
#define TRACE_TABLE 0x20000
#define IMG_BUFFER  0x30000
#define IMG_WIDTH   256
#define IMG_HEIGHT  256

u16 trace_table[IMG_HEIGHT][4];

__attribute__((noreturn)) void main()
{

  // ga_reg_stampsize
  // ga_reg_stampmapbase
  // ga_reg_imgbufvsize
  // ga_reg_imgbufstart
  // ga_reg_imgbufoffset
  // ga_reg_imgbufhdotsize
  // ga_reg_imgbufvdotsize
  // ga_reg_tracevectbase
  memcpy16(res_stamp01.data, (WORD_RAM_2M + STAMP_DATA), res_stamp01.size / 2);

  memcpy16(
    res_stamp_map.data, (WORD_RAM_2M + STAMP_MAP), res_stamp_map.size / 2);

  ga_reg_stampsize = GA_MASK_STAMPSIZE_REPEAT | GA_MASK_STAMPSIZE_32x32_STAMP;
  ga_reg_stampmapbase = (WORD_RAM_2M + STAMP_MAP) / 4;
  ga_reg_imgbufstart = (WORD_RAM_2M + IMG_BUFFER) / 4;
  ga_reg_imgbufvsize = 31;
  ga_reg_imgbufvdotsize = IMG_HEIGHT;
  ga_reg_imgbufhdotsize = IMG_WIDTH;

  for (u16 trace_table_line = 0; trace_table_line < IMG_HEIGHT;
       ++trace_table_line)
  {
  }
}
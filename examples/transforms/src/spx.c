
#include "shared.h"
#include "xform.res.h"
#include <memory.h>
#include <sub/cdrom.h>
#include <sub/gate_arr.h>
#include <sub/memmap.h>
#include <system.h>
#include <xform.h>

#define STAMP_DATA_OFFSET  0x0
#define STAMP_MAP_OFFSET   0x10000
#define TRACE_TABLE_OFFSET 0x20000
#define IMG_BUFFER_OFFSET  0x30000
#define IMG_WIDTH          256
#define IMG_HEIGHT         256

typedef struct
{
  s16 x;
  s16 y;
  s16 delta_x;
  s16 delta_y;
} trace_entry;

trace_entry (*trace_table)[IMG_HEIGHT] =
  (trace_entry (*)[IMG_HEIGHT])(WORD_RAM_2M + TRACE_TABLE_OFFSET);

extern void sp_fatal();

char const * const filenames[] = {"IPX.MMD;1"};

void redraw(s16 trace_x, s16 trace_y, s16 trace_dx, s16 trace_dy);

// It's a good idea to put SPX's main in .init to ensure it's at the very start
// of the code, since we jump to where we expect it to be in memory
void main()
{
  enable_interrupts();

  wait_2m();

  memset32(0, (u32 *) WORD_RAM_2M, 0x20000 * 2);

  register s16 command, param1, param2, param3, param4;
  do
  {

    do
    {
      command = *ga_reg_comcmd0;
    } while (command == 0);

    if (command != *ga_reg_comcmd0)
      continue;

    param1 = *ga_reg_comcmd1;
    param2 = *ga_reg_comcmd2;
    param3 = *ga_reg_comcmd3;
    param4 = *ga_reg_comcmd4;

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
        redraw(param1, param2, param3, param4);
        break;
    }

    // not reaching here?
    asm(".global test_label3\ntest_label3:");
    *ga_reg_comstat0 = *ga_reg_comcmd0;
    do
    {
      asm("nop");
      command = *ga_reg_comcmd0;
    } while (command != 0);

    do
    {
      asm("nop");
      command = *ga_reg_comcmd0;
    } while (command != 0);

    *ga_reg_comstat0 = 0;

  } while (1);
}

void load_gfx()
{
  memcpy16(
    (u16 const *) &res_stamp01,
    (u16 *) (WORD_RAM_2M + STAMP_DATA_OFFSET + (4 * 4 * 32 * 1)),
    res_stamp01_size / 2);
  memcpy16(
    (u16 const *) &res_stamp02,
    (u16 *) (WORD_RAM_2M + STAMP_DATA_OFFSET + (4 * 4 * 32 * 2)),
    res_stamp02_size / 2);
  memcpy16(
    (u16 const *) &res_stamp03,
    (u16 *) (WORD_RAM_2M + STAMP_DATA_OFFSET + (4 * 4 * 32 * 3)),
    res_stamp03_size / 2);
  memcpy16(
    (u16 const *) &res_stamp04,
    (u16 *) (WORD_RAM_2M + STAMP_DATA_OFFSET + (4 * 4 * 32 * 4)),
    res_stamp04_size / 2);

  memcpy16(
    (u16 const *) &res_stamp_map,
    (u16 *) (WORD_RAM_2M + STAMP_MAP_OFFSET),
    res_stamp_map_size / 2);
}

void redraw(s16 trace_x, s16 trace_y, s16 trace_dx, s16 trace_dy)
{
  wait_2m();
  load_gfx();
  // ga_reg_stampsize
  // ga_reg_stampmapbase
  // ga_reg_imgbufvsize
  // ga_reg_imgbufstart
  // ga_reg_imgbufoffset
  // ga_reg_imgbufhdotsize
  // ga_reg_imgbufvdotsize
  // ga_reg_tracevectbase

  ga_reg_stampmapbase = (u16) (STAMP_MAP_OFFSET / 4);
  ga_reg_imgbufstart = (u16) (IMG_BUFFER_OFFSET / 4);
  ga_reg_stampsize = GA_MASK_STAMPSIZE_REPEAT | GA_MASK_STAMPSIZE_32x32_STAMP;
  ga_reg_imgbufvdotsize = IMG_HEIGHT;
  ga_reg_imgbufhdotsize = IMG_WIDTH;
  ga_reg_imgbufvsize = (IMG_HEIGHT / 8) - 1;
  ga_reg_imgbufoffset = 0;

  for (u16 trace_table_line = 0; trace_table_line < IMG_HEIGHT;
       ++trace_table_line)
  {
    (*trace_table)[trace_table_line].x = trace_x;
    (*trace_table)[trace_table_line].y = (trace_table_line << 3) + trace_y;
    (*trace_table)[trace_table_line].delta_x = trace_dx;
    (*trace_table)[trace_table_line].delta_y = trace_dy;
  }

  ga_reg_tracevectbase = (u16) (TRACE_TABLE_OFFSET / 4);

  while (ga_reg_stampsize & 0x8000)
  {
    asm("nop");
  }

  grant_2m();
}
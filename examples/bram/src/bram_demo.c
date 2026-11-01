#include <macro.h>
#include <main/memmap.def.h>

// Module layout: ROM begins at the start of Work RAM, overwriting the IP;
// RAM follows it, leaving a little over 61 KB for ROM and 512 bytes of RAM.
// MMD_DEST says where the module is copied after loading into Word RAM.
GLOBAL_SYM(MODULE_ROM_ORIGIN, WORK_RAM);
GLOBAL_SYM(MODULE_RAM_ORIGIN, 0xFFF500);
GLOBAL_SYM(MODULE_ROM_LENGTH, MODULE_RAM_ORIGIN - MODULE_ROM_ORIGIN);
GLOBAL_SYM(MODULE_RAM_LENGTH, 0x200);
GLOBAL_SYM(MMD_DEST, WORK_RAM);

#include <main/bios.h>
#include <main/gate_arr.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/mmd.h>
#include <main/vdp.h>
#include <stdio.h>
#include <str_util.h>
#include <system.h>
#include <types.h>

#define SUB_WAIT                   \
  do                               \
  {                                \
    asm("nop");                    \
  } while (ga_main_reg_comstat0 == 0);

#define SUB_ACK ga_main_reg_comcmd0 = 0;

#define print_xy(x, y) VDP_PLANE_XY(BIOS_VDP_DEFAULT_PLANEA, x, y, Width64)

// Format a value with snprintf and print it with the Boot ROM print routine,
// which needs the 0xFF terminator in place of C's NUL.
static void print_value(char const * format, int value, u8 x, u8 y)
{
  char buffer[24];
  snprintf(buffer, sizeof buffer, format, value);
  bios_print(str_to_bios(buffer), print_xy(x, y));
}

static void bram_init()
{
  bios_print("Performing BRAM_INIT...\xff", print_xy(1, 1));

  ga_main_reg_comcmd0 = 2;

  SUB_WAIT

  // bram_size is the total space on the unit, returned as the number of
  // 4KB (0x1000 byte) blocks. So we multiplay by 0x1000 to get the 'actual'
  // total space available
  int bram_size = (ga_main_reg_comstat1) * 0x1000;
  int status = ga_main_reg_comstat2;

  SUB_ACK

  bios_print("Total Size: \xff", print_xy(1, 2));

  print_value("%d bytes", bram_size, 13, 2);

  bios_print("Status: \xff", print_xy(1, 3));

  print_value("%d", status, 13, 3);

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
  bios_print("Performing BRAM_STATUS...\xff", print_xy(1, 1));

  ga_main_reg_comcmd0 = 6;

  SUB_WAIT

  // bram_size is the total space on the unit, returned as the number of
  // 4KB (0x1000 byte) blocks. So we multiplay by 0x1000 to get the 'actual'
  // total space available
  u16 filecount = ga_main_reg_comstat1;
  u16 free = ga_main_reg_comstat2;

  SUB_ACK

  bios_print("File count: \xff", print_xy(1, 3));
  print_value("%u", filecount, 13, 3);

  bios_print("Free blocks: \xff", print_xy(1, 4));
  print_value("%u", free, 13, 4);
}

static bool brmserch()
{
  bios_print("Performing BRAM_SEARCH...\xff", print_xy(1, 1));

  ga_main_reg_comcmd0 = 3;

  SUB_WAIT

  u16 found = ga_main_reg_comstat1;
  u16 filesize = ga_main_reg_comstat2;
  u16 is_protected = ga_main_reg_comstat3;

  SUB_ACK

  if (found == 0xFFFF)
  {
    bios_print("File BRMEX not found\xff", print_xy(1, 2));
    return false;
  }
  else
  {
    bios_print("File BRMEX found\xff", print_xy(1, 2));

    bios_print(" File size: \xff", print_xy(1, 3));
    print_value("%u", filesize, 13, 3);

    bios_print(" Protected? \xff", print_xy(1, 4));
    print_value("%u", is_protected, 13, 4);
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
    bios_vblank_wait_default();

    // this is a great opportunity to show off the bios_input_delay function
    // in the boot rom library
    // to use it, we need to specify a byte which will hold the input value
    // and use that byte to compare for input instead of the standard memory
    // location; it will need to be called in a loop along with the vblank
    // wait

    // now we get a nice, smooth value change when holding up or down instead
    // of the value changing way too quickly
    bios_input_repeat_delay(&delayed_input_val, false);

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

    print_value("%04X", writeval, 23, 2);
  } while (! (bios_joy1_hit & PAD_START));

  *(u16 *) word_ram = writeval;

  ga_main_grant_2m();

  ga_main_reg_comcmd0 = 5;

  SUB_WAIT

  int success = ga_main_reg_comstat1;

  SUB_ACK

  ga_main_wait_2m();

  if (success == 0xFFFF)
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

  bios_print("Performing BRAM_READ...\xff", print_xy(1, 6));

  ga_main_grant_2m();

  ga_main_reg_comcmd0 = 4;

  SUB_WAIT

  int success = ga_main_reg_comstat1;
  int size = ga_main_reg_comstat2;
  int mode = ga_main_reg_comstat3;

  SUB_ACK

  ga_main_wait_2m();

  if (success == 0xFFFF)
  {
    bios_print("Error reading file\xff", print_xy(1, 7));
    return;
  }

  u16 readval = *(u16 *) word_ram;

  bios_print("Success reading file\xff", print_xy(1, 7));

  bios_print("Value read: \xff", print_xy(1, 9));

  print_value("%04X", readval, 12, 9);
}

void brmdel()
{
  bios_clear_tables();

  bios_print("Performing BRAM_DELETE...\xff", print_xy(1, 1));

  ga_main_reg_comcmd0 = 7;

  SUB_WAIT

  int success = ga_main_reg_comstat1;

  SUB_ACK

  if (success == 0xFFFF)
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

  bios_print("Performing BRAM_DIRECTORY...\xff", print_xy(1, 1));

  ga_main_grant_2m();

  ga_main_reg_comcmd0 = 8;

  SUB_WAIT

  int success = ga_main_reg_comstat1;

  SUB_ACK

  ga_main_wait_2m();

  if (success == 0xFFFF)
  {
    bios_print("Data too big for buffer\xff", print_xy(1, 3));
    return;
  }

  bios_print("Filename       Blocks\xff", print_xy(1, 3));

  u16    file_idx = 0;
  u16    curr_y = 4;
  char * filename;
  while (1)
  {
    if (*(u8 *) (word_ram + (file_idx * 16)) == 0)
      break;
    filename = (char *) (word_ram + (file_idx * 16));
    filename[11] = 0xFF;
    bios_print(filename, print_xy(1, curr_y));
    print_value("%u", *(u16 *) (word_ram + (file_idx * 16) + 14), 16, curr_y);
    ++curr_y;
    ++file_idx;
  }
}

void main()
{
  // setup boot rom library font
  bios_load_font_defaults();
  bios_palette[1] = 0xEEE;
  bios_vdp_update_flags |= BIOS_MASK_COPY_PALETTE;

  // interrupts were disabled in the init section
  enable_interrupts();

  bram_init();
  bios_print("Press Start to continue...\xff", print_xy(1, 10));

  do
  {
    bios_vblank_wait_default();
  } while (! (bios_joy1_hit & PAD_START));

  s8 menupos = 0;

  do
  {
    bios_clear_tables();

    bios_print("BRAM Options\xff", print_xy(1, 1));
    bios_print("BRAM_STATUS\xff", print_xy(3, 3));
    bios_print("BRAM_DIRECTORY\xff", print_xy(3, 4));
    bios_print("BRAM_READ\xff", print_xy(3, 5));
    bios_print("BRAM_WRITE\xff", print_xy(3, 6));
    bios_print("BRAM_VERIFY\xff", print_xy(3, 7));
    bios_print("BRAM_DELETE\xff", print_xy(3, 8));
    bios_print("BRAM_FORMAT\xff", print_xy(3, 9));

    do
    {
      bios_vblank_wait_default();
      for (int clearloop = 3; clearloop < 10; ++clearloop)
      {
        vdp_ctrl_32 = print_xy(1, clearloop);
        vdp_data_16 = 0;
      }
      vdp_ctrl_32 = print_xy(1, menupos + 3);
      vdp_data_16 = '>';

      if ((bios_joy1_hit & PAD_DOWN))
      {
        ++menupos;
        if (menupos > 6)
          menupos = 0;
        continue;
      }

      if ((bios_joy1_hit & PAD_UP))
      {
        --menupos;
        if (menupos < 0)
          menupos = 6;
        continue;
      }
    } while (! (bios_joy1_hit & PAD_START));

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
      bios_vblank_wait_default();
    } while (! (bios_joy1_hit & PAD_START));

  } while (1);
}

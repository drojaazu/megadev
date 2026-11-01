#include <macro.h>
#include <main/memmap.def.h>

// Module layout: ROM begins at the start of Work RAM, overwriting the IP;
// RAM follows it, leaving a little over 61 KB for ROM and 512 bytes of RAM.
// MMD_DEST says where the module is copied after loading into Word RAM.
GLOBAL_SYM(MODULE_ROM_ORIGIN, WORK_RAM);
GLOBAL_SYM(MODULE_RAM_ORIGIN, 0xFFF000);
GLOBAL_SYM(MODULE_ROM_LENGTH, MODULE_RAM_ORIGIN - MODULE_ROM_ORIGIN);
GLOBAL_SYM(MODULE_RAM_LENGTH, 0x700);
GLOBAL_SYM(MMD_DEST, WORK_RAM);

#include <main/bios.h>
#include <main/exvec.h>
#include <main/gate_arr.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/mmd.h>
#include <main/vdp.h>
#include <stdio.h>
#include <system.h>
#include <types.h>
#include <main/text.h>

// font8x8 is loaded at tile 0x20, so a character's tile is its code
#define TEXT_TILE 0x20

#define SUB_WAIT                   \
  do                               \
  {                                \
    exvec_check_sub();                    \
  } while (ga_main_reg_comstat0 == 0);

#define SUB_ACK ga_main_reg_comcmd0 = 0;

#define print_xy(x, y) VDP_PLANE_XY(BIOS_VDP_DEFAULT_PLANEA, x, y, Width64)

// Format a value with snprintf and print it
static void print_value(char const * format, int value, u8 x, u8 y)
{
  char buffer[24];
  snprintf(buffer, sizeof buffer, format, value);
  text_print(buffer, print_xy(x, y), TEXT_TILE);
}

static void bram_init()
{
  text_print("Performing BRAM_INIT...", print_xy(1, 1), TEXT_TILE);

  ga_main_reg_comcmd0 = 2;

  SUB_WAIT

  // bram_size is the total space on the unit, returned as the number of
  // 4KB (0x1000 byte) blocks. So we multiplay by 0x1000 to get the 'actual'
  // total space available
  int bram_size = (ga_main_reg_comstat1) * 0x1000;
  int status = ga_main_reg_comstat2;

  SUB_ACK

  text_print("Total Size: ", print_xy(1, 2), TEXT_TILE);

  print_value("%d bytes", bram_size, 13, 2);

  text_print("Status: ", print_xy(1, 3), TEXT_TILE);

  print_value("%d", status, 13, 3);

  switch (status)
  {
    case 0:
      text_print("BRAM not present or not detected!", print_xy(1, 4), TEXT_TILE);
      break;
    case 1:
      text_print("BRAM is not formatted!", print_xy(1, 4), TEXT_TILE);
      break;
    case 2:
      text_print("BRAM is in a non-standard format!", print_xy(1, 4), TEXT_TILE);
      break;
    case 3:
      text_print("BRAM is formatted and ready to use!", print_xy(1, 4), TEXT_TILE);
      break;
  }
}

static void brmstat()
{
  bios_clear_tables();
  text_print("Performing BRAM_STATUS...", print_xy(1, 1), TEXT_TILE);

  ga_main_reg_comcmd0 = 6;

  SUB_WAIT

  // bram_size is the total space on the unit, returned as the number of
  // 4KB (0x1000 byte) blocks. So we multiplay by 0x1000 to get the 'actual'
  // total space available
  u16 filecount = ga_main_reg_comstat1;
  u16 free = ga_main_reg_comstat2;

  SUB_ACK

  text_print("File count: ", print_xy(1, 3), TEXT_TILE);
  print_value("%u", filecount, 13, 3);

  text_print("Free blocks: ", print_xy(1, 4), TEXT_TILE);
  print_value("%u", free, 13, 4);
}

static bool brmserch()
{
  text_print("Performing BRAM_SEARCH...", print_xy(1, 1), TEXT_TILE);

  ga_main_reg_comcmd0 = 3;

  SUB_WAIT

  u16 found = ga_main_reg_comstat1;
  u16 filesize = ga_main_reg_comstat2;
  u16 is_protected = ga_main_reg_comstat3;

  SUB_ACK

  if (found == 0xFFFF)
  {
    text_print("File BRMEX not found", print_xy(1, 2), TEXT_TILE);
    return false;
  }
  else
  {
    text_print("File BRMEX found", print_xy(1, 2), TEXT_TILE);

    text_print(" File size: ", print_xy(1, 3), TEXT_TILE);
    print_value("%u", filesize, 13, 3);

    text_print(" Protected? ", print_xy(1, 4), TEXT_TILE);
    print_value("%u", is_protected, 13, 4);
    return true;
  }
}

static void brmwrite()
{
  bios_clear_tables();

  u16 writeval = 0;

  text_print("Set a value to write: ", print_xy(1, 2), TEXT_TILE);
  u8 delayed_input_val = 0;

  do
  {
    bios_vblank_wait_default();
    exvec_check_sub();

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
    text_print("Error while writing file", print_xy(1, 4), TEXT_TILE);
  }
  else
  {
    text_print("Write successful", print_xy(1, 4), TEXT_TILE);
  }
}

static void brmread()
{
  bios_clear_tables();

  if (! brmserch())
    return;

  text_print("Performing BRAM_READ...", print_xy(1, 6), TEXT_TILE);

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
    text_print("Error reading file", print_xy(1, 7), TEXT_TILE);
    return;
  }

  u16 readval = *(u16 *) word_ram;

  text_print("Success reading file", print_xy(1, 7), TEXT_TILE);

  text_print("Value read: ", print_xy(1, 9), TEXT_TILE);

  print_value("%04X", readval, 12, 9);
}

void brmdel()
{
  bios_clear_tables();

  text_print("Performing BRAM_DELETE...", print_xy(1, 1), TEXT_TILE);

  ga_main_reg_comcmd0 = 7;

  SUB_WAIT

  int success = ga_main_reg_comstat1;

  SUB_ACK

  if (success == 0xFFFF)
  {
    text_print("Failed to delete file", print_xy(1, 3), TEXT_TILE);
  }
  else
  {
    text_print("File deleted successfully", print_xy(1, 3), TEXT_TILE);
  }
}

void brmdir()
{
  bios_clear_tables();

  text_print("Performing BRAM_DIRECTORY...", print_xy(1, 1), TEXT_TILE);

  ga_main_grant_2m();

  ga_main_reg_comcmd0 = 8;

  SUB_WAIT

  int success = ga_main_reg_comstat1;

  SUB_ACK

  ga_main_wait_2m();

  if (success == 0xFFFF)
  {
    text_print("Data too big for buffer", print_xy(1, 3), TEXT_TILE);
    return;
  }

  text_print("Filename       Blocks", print_xy(1, 3), TEXT_TILE);

  u16    file_idx = 0;
  u16    curr_y = 4;
  char * filename;
  while (1)
  {
    if (*(u8 *) (word_ram + (file_idx * 16)) == 0)
      break;
    filename = (char *) (word_ram + (file_idx * 16));
    filename[11] = '\0';
    text_print(filename, print_xy(1, curr_y), TEXT_TILE);
    print_value("%u", *(u16 *) (word_ram + (file_idx * 16) + 14), 16, curr_y);
    ++curr_y;
    ++file_idx;
  }
}

void main()
{
  // load the kit font for text
  vdp_load_1bpp_tiles(font8x8, FONT8X8_CHARS, TEXT_TILE, 1, 0);
  bios_palette[1] = 0xEEE;
  bios_vdp_update_flags |= BIOS_MASK_COPY_PALETTE;

  // crash handler for Main CPU exceptions (docs/crash_handler.md)
  exvec_install();

  // interrupts were disabled in the init section
  enable_interrupts();

  bram_init();
  text_print("Press Start to continue...", print_xy(1, 10), TEXT_TILE);

  do
  {
    bios_vblank_wait_default();
    exvec_check_sub();
  } while (! (bios_joy1_hit & PAD_START));

  s8 menupos = 0;

  do
  {
    bios_clear_tables();

    text_print("BRAM Options", print_xy(1, 1), TEXT_TILE);
    text_print("BRAM_STATUS", print_xy(3, 3), TEXT_TILE);
    text_print("BRAM_DIRECTORY", print_xy(3, 4), TEXT_TILE);
    text_print("BRAM_READ", print_xy(3, 5), TEXT_TILE);
    text_print("BRAM_WRITE", print_xy(3, 6), TEXT_TILE);
    text_print("BRAM_VERIFY", print_xy(3, 7), TEXT_TILE);
    text_print("BRAM_DELETE", print_xy(3, 8), TEXT_TILE);
    text_print("BRAM_FORMAT", print_xy(3, 9), TEXT_TILE);

    do
    {
      bios_vblank_wait_default();
      exvec_check_sub();
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

    text_print("Press Start to continue...", print_xy(1, 12), TEXT_TILE);

    do
    {
      bios_vblank_wait_default();
      exvec_check_sub();
    } while (! (bios_joy1_hit & PAD_START));

  } while (1);
}

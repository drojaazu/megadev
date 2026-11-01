#include <macro.h>
#include <sub/memmap.def.h>

// Module layout in Program RAM
GLOBAL_SYM(MODULE_ROM_ORIGIN, 0x10000);
GLOBAL_SYM(MODULE_ROM_LENGTH, 0xE000);
GLOBAL_SYM(MODULE_RAM_ORIGIN, 0x1E000);
GLOBAL_SYM(MODULE_RAM_LENGTH, 0x2000);

#include <sub/bram.h>
#include <sub/cdrom.h>
#include <sub/gate_arr.h>
#include <sub/memmap.h>
#include <system.h>

void load_ipx();

extern void sp_fatal();

char const * filenames[3];

BramFileInfo const file_info = {"BRMEX", 0, 1};

BramInitResult brminit_info;

void main()
{
  // std_init.s masks interrupts; the Sub BIOS needs them for CD-ROM access
  enable_interrupts();

  register u16 cmd0, cmd1;

  do
  {

    do
    {
      cmd0 = ga_sub_reg_comcmd0;
    } while (cmd0 == 0);

    if (cmd0 != ga_sub_reg_comcmd0)
      continue;

    cmd1 = ga_sub_reg_comcmd1;

    switch (cmd0)
    {

      // load MMD
      case 1:
        load_file(CDROM_LOAD_CDC, filenames[cmd1], (u8 *) WORD_RAM_2M);
        ga_sub_grant_2m();
        if (access_op_result != CDROM_RESULT_OK)
        {
          sp_fatal();
        }
        break;

      // BRAM_INIT
      case 2:
        asm("nop");

        bram_init(&brminit_info);
        ga_sub_reg_comstat1 = brminit_info.bram_size;
        ga_sub_reg_comstat2 = (u16) brminit_info.status;

        break;

      // BRAM_SEARCH
      case 3:
        asm("nop");

        // the filename must fill up 11 bytes followed by a 0 terminator
        // for a total of 12 bytes
        BramSearchResult * search = bram_search(file_info.filename);
        // null means file was not found
        if (search == NULL)
        {
          ga_sub_reg_comstat1 = 0xFFFF;
        }
        else
        {
          ga_sub_reg_comstat1 = 0;
          ga_sub_reg_comstat2 = search->filesize;
          ga_sub_reg_comstat3 = search->mode;
        }

        break;

      // BRAM_READ
      case 4:

        ga_sub_wait_2m();

        BramReadResult * read =
          bram_read(file_info.filename, (u8 *) WORD_RAM_2M);

        if (! read->success)
        {
          ga_sub_reg_comstat1 = 0xFFFF;
        }
        else
        {
          ga_sub_reg_comstat1 = 0;
          ga_sub_reg_comstat2 = read->filesize;
          ga_sub_reg_comstat3 = read->mode;
        }

        ga_sub_grant_2m();
        break;

      // BRAM_WRITE
      case 5:
        ga_sub_wait_2m();

        if (! bram_write(&file_info, (u8 *) WORD_RAM_2M))
        {
          ga_sub_reg_comstat1 = 0xFFFF;
        }
        ga_sub_grant_2m();
        break;

      // bram stats
      case 6:
        asm("nop");
        BramStatusResult * stats = bram_status();
        ga_sub_reg_comstat1 = stats->filecount;
        ga_sub_reg_comstat2 = stats->free;
        break;

      // brmdel
      case 7:
        asm("nop");
        if (bram_delete(&file_info.filename))
          ga_sub_reg_comstat1 = 0;
        else
          ga_sub_reg_comstat1 = 0xFFFF;
        break;

      // brmdir
      case 8:
        asm("nop");

        ga_sub_wait_2m();

        if (! bram_directory("*\0", (u8 *) WORD_RAM_2M, 0, 0x100))
          ga_sub_reg_comstat1 = 0xFFFF;
        else
          ga_sub_reg_comstat0 = 0;

        ga_sub_grant_2m();
        break;

      // load demo
      case 0xFE:
        load_file(CDROM_LOAD_CDC, "BRAMDEMO.MMD;1", (u8 *) WORD_RAM_2M);
        ga_sub_grant_2m();
        if (access_op_result != CDROM_RESULT_OK)
        {
          sp_fatal();
        }
        break;
    }

    ga_sub_reg_comstat0 = ga_sub_reg_comcmd0;
    do
    {
      cmd0 = ga_sub_reg_comcmd0;
    } while (cmd0 != 0);

    do
    {
      cmd0 = ga_sub_reg_comcmd0;
    } while (cmd0 != 0);

    ga_sub_reg_comstat0 = 0;
    ga_sub_reg_comstat1 = 0;
    ga_sub_reg_comstat2 = 0;
    ga_sub_reg_comstat3 = 0;

  } while (1);
}

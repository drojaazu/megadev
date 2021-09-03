
#include "sub/bram.h"
#include "sub/cdrom.h"
#include "sub/sub.h"

void load_ipx();

extern void sp_fatal();

char * filenames[3];

char * examplefile;

__attribute__((section(".init"))) void main() {

  register u16 cmd0, cmd1;

  examplefile = "BRMEX      ";

  do {

    do {
      cmd0 = *GA_COMCMD0;
    } while (cmd0 == 0);

    if (cmd0 != *GA_COMCMD0)
      continue;

    cmd1 = *GA_COMCMD1;

    switch (cmd0) {

    // load MMD
    case 1:
      load_file(ACC_OP_LOAD_CDC, filenames[cmd1], (u8 *)SUB_2M_BASE);
      grant_2m();
      if (access_op_result != RESULT_OK) {
        sp_fatal();
      }
      break;

    // BRMINIT
    case 2:
      asm("nop");

      BrminitRes * init = bram_brminit();
      *GA_COMSTAT1 = init->bram_size;
      *GA_COMSTAT2 = (u16)init->status;

      break;

    // BRMSERCH
    case 3:
      asm("nop");

      // the filename must fill up 11 bytes followed by a 0 terminator
      // for a total of 12 bytes
      BrmserchRes * search = bram_brmserch(examplefile);
      // null means file was not found
      if (search == NULL) {
        *GA_COMSTAT1 = 0xFFFF;
      } else {
        *GA_COMSTAT1 = 0;
        *GA_COMSTAT2 = search->filesize;
        *GA_COMSTAT3 = search->mode;
      }

      break;

    // BRMREAD
    case 4:

      wait_2m();

      BrmreadRes * read = bram_brmread(examplefile, (u8 *)SUB_2M_BASE);

      if (!read->success) {
        *GA_COMSTAT1 = 0xffff;
      } else {
        *GA_COMSTAT1 = 0;
        *GA_COMSTAT2 = read->filesize;
        *GA_COMSTAT3 = read->mode;
      }

      grant_2m();
      break;

    // BRMWRITE
    case 5:
      wait_2m();

      BrmwriteParams info;

      info.filename[0] = 'B';
      info.filename[1] = 'R';
      info.filename[2] = 'M';
      info.filename[3] = 'E';
      info.filename[4] = 'X';
      info.filename[5] = ' ';
      info.filename[6] = ' ';
      info.filename[7] = ' ';
      info.filename[8] = ' ';
      info.filename[9] = ' ';
      info.filename[10] = ' ';
      info.mode = 0;
      info.blocksize = 1;

      if (!bram_brmwrite(&info, (u8 *)SUB_2M_BASE)) {
        *GA_COMSTAT1 = 0xFFFF;
      }
      grant_2m();
      break;

    // bram stats
    case 6:
      asm("nop");
      BrmstatRes * stats = bram_brmstat();
      *GA_COMSTAT1 = stats->filecount;
      *GA_COMSTAT2 = stats->free;
      break;

    // brmdel
    case 7:
      asm("nop");
      if (bram_brmdel(examplefile))
        *GA_COMSTAT1 = 0;
      else
        *GA_COMSTAT1 = 0xffff;
      break;

    // brmdir
    case 8:
      asm("nop");

      wait_2m();
      asm(".global test_label\ntest_label:");

      if (!bram_brmdir("*\0", (u8 *)SUB_2M_BASE, 0, 0x100))
        *GA_COMSTAT1 = 0xffff;
      else
        *GA_COMSTAT0 = 0;

      grant_2m();
      break;

    // load IPX
    case 0xfe:
      load_file(ACC_OP_LOAD_CDC, "IPX.MMD;1", (u8 *)SUB_2M_BASE);
      grant_2m();
      if (access_op_result != RESULT_OK) {
        sp_fatal();
      }
      break;
    }

    *GA_COMSTAT0 = *GA_COMCMD0;
    do {
      cmd0 = *GA_COMCMD0;
    } while (cmd0 != 0);

    do {
      cmd0 = *GA_COMCMD0;
    } while (cmd0 != 0);

    *GA_COMSTAT0 = 0;
    *GA_COMSTAT1 = 0;
    *GA_COMSTAT2 = 0;
    *GA_COMSTAT3 = 0;

  } while (1);
}

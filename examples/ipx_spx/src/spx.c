
#include "sub/sub.h"
#include "sub/cdrom.h"

void load_ipx();

extern void sp_fatal();

char* filenames[3];

void main() {

  register u16 cmd0, cmd1;

  filenames[0] = "EX1.MMD;1";
  filenames[1] = "EX2.MMD;1";
  filenames[2] = "EX3.MMD;1";

/*
  asm("R(
1:move.w _GA_COMCMD0, %0
  beq    1b
  cmp.w  _GA_COMCMD0, %0
  bne    1b
  move.w  _GA_COMCMD1, %1
  )" : "=d"(cmd0), "=d"(cmd1));
*/
  do {

    do {
      cmd0 = *GA_COMCMD0;
    } while(cmd0 == 0);

    if(cmd0 != *GA_COMCMD0) continue;

    cmd1 = *GA_COMCMD1;

    switch(cmd0) {

      // load MMD
      case 1:
        load_file(ACC_OP_LOAD_WORD, filenames[cmd1], (u8*)SUB_2M_BASE);
        grant_2m();
        if(access_op_result != RESULT_OK) {
          sp_fatal();
        }
      break;

      // load IPX
      case 0xfe:
        load_file(ACC_OP_LOAD_WORD, "IPX.MMD;1", (u8*)SUB_2M_BASE);
        grant_2m();
        if(access_op_result != RESULT_OK) {
          sp_fatal();
        }
      break;
    }

    // not reaching here?
    asm(".global test_label\ntest_label:");
    *GA_COMSTAT0 = *GA_COMCMD0;
    do {
      cmd0 = *GA_COMCMD0;
    } while(cmd0 != 0);

    do {
      cmd0 = *GA_COMCMD0;
    } while(cmd0 != 0);

    *GA_COMSTAT0 = 0;

  } while(1);


}

void load_ipx() {
  

}

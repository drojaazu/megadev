/**
 * Mode 1 example: Main CPU program
 *
 * Detects the Mega CD, brings up the Sub CPU and uses the Boot ROM library
 * from the cartridge: default VDP registers, VRAM clear, the built-in font,
 * text printing and the VBLANK handler.
 */
#include <main/bios.def.h>
#include <main/bios.h>
#include <main/exvec.h>
#include <main/gate_arr.h>
#include <main/memmap.h>
#include <main/mode1.h>
#include <main/text.h>
#include <main/vdp.h>
#include <stdio.h>
#include <system.h>
#include <types.h>

typedef struct
{
  u32  size;
  char data[];
} DataChunk;

extern DataChunk const res_sp;

#define PLANE_A_ADDR 0xC000
#define FONT_TILE    0x20

#define print_at(str, x, y) text_print((str), VDP_PLANE_XY(PLANE_A_ADDR, x, y, Width64), FONT_TILE)

// Results, readable from an emulator debugger
volatile u16 mode1_status; // 0x0100 | mode1_load_sub result, 0x00FF = no Mega CD
volatile u16 mode1_echo;   // last COMSTAT0 value from the Sub CPU

static void print_value(char * buf, char const * fmt, u32 value, u8 x, u8 y)
{
  snprintf(buf, 16, fmt, value);
  print_at(buf, x, y);
}

void main()
{
  char buf[16];

  disable_interrupts();

  bios_load_vdp_regs_default();
  bios_clear_vram();
  bios_clear_tables();
  bios_clear_vsram();

  vdp_ctrl_32 = to_vdp_addr(0) | CRAM_W;
  vdp_data = 0x0000;
  vdp_data = 0x0EEE;

  vdp_load_1bpp_tiles(font8x8, FONT8X8_CHARS, FONT_TILE, 1, 0);

  // the Boot ROM default registers leave the display off
  bios_vdp_disp_enable();

  *exvec_vblank = (void (*)) BIOS_VBLANK_HANDLER;
  enable_interrupts();

  print_at("MODE 1 EXAMPLE", 2, 2);

  if (!mode1_detect())
  {
    mode1_status = 0x00FF;
    print_at("MEGA CD: NOT FOUND", 2, 4);
    for (;;)
      bios_vblank_wait_default();
  }
  print_at("MEGA CD: FOUND", 2, 4);

  // crash handler: Main CPU exceptions, and Sub CPU crashes reported below
  exvec_install();

  u8 const result = mode1_load_sub(res_sp.data, res_sp.size);
  mode1_status = 0x0100 | result;
  print_at(result == MODE1_OK ? "SUB CPU: RUNNING" : "SUB CPU: ERROR", 2, 5);
  print_value(buf, "%02X", result, 20, 5);

  print_at("FRAME:", 2, 7);
  print_at("COMCMD0:", 2, 8);
  print_at("COMSTAT0:", 2, 9);

  u16 counter = 0;
  for (;;)
  {
    bios_vblank_wait_default();
    exvec_check_sub();
    ++counter;
    ga_main_reg_comcmd0 = counter;
    mode1_echo = ga_main_reg_comstat0;
    print_value(buf, "%02X", *(volatile u8 *) BIOS_VBLANK_COUNTER, 12, 7);
    print_value(buf, "%04X", counter, 12, 8);
    print_value(buf, "%04X", mode1_echo, 12, 9);
  }
}

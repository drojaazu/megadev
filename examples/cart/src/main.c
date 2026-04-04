#include "res.h"
#include <main/io.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <memory.h>
#include <system.h>

volatile u8 p1_hold, p1_single, p1_prev;
volatile u8 p2_hold, p2_single, p2_prev;

void read_inputs()
{
  p1_hold = ~read_input_joypad(io_data1);
  p2_hold = ~read_input_joypad(io_data2);
  if (p1_hold == p1_prev)
    p1_single = 0;
  else
    p1_single = p1_hold;
  if (p2_hold == p2_prev)
    p2_single = 0;
  else
    p2_single = p2_hold;
  p1_prev = p1_hold;
  p2_prev = p2_hold;
}

__attribute__((interrupt)) void INT2_EXT()
{
  return;
}

__attribute__((interrupt)) void INT4_HBLANK()
{
  return;
}

volatile bool vblank_done;
u32           vblank_counter;
u8            vblank_flags;

#define vblank_skip_updates    (1 << 0)
#define vblank_update_cram     (1 << 1)
#define vblank_update_vdp_regs (1 << 2)

__attribute__((interrupt)) void INT6_VBLANK()
{
  read_inputs();
  if (vblank_flags & vblank_skip_updates)
    goto vblank_done;
  if (vblank_flags & vblank_update_cram)
    update_cram();
  if (vblank_flags & vblank_update_vdp_regs)
    update_vdp_regs();

vblank_done:
  ++vblank_counter;
  vblank_done = true;
  return;
}

// copy of VDP registers in memory
// these need to be updated to the VDP whenever they are updated
u16 vdp_regs[18];

void update_vdp_regs()
{
  for (u8 i = 0; i < 18; ++i)
    vdp_ctrl = vdp_regs[i];
}

// copy of color RAM in memory
u16 cram[64];

void update_cram_line(u8 pal_line)
{
  pal_line <<= 4;
  vdp_ctrl_32 = vdp_ptr(pal_line) | CRAM_W;
  u16 * ptr_pal_line = cram + pal_line;
  for (u8 i = 0; i < 16; ++i)
    vdp_data = *ptr_pal_line++;
}

void update_cram()
{
  vdp_ctrl_32 = vdp_ptr(0) | CRAM_W;
  u16 * ptr_pal_line = cram;
  for (u8 i = 0; i < 64; ++i)
    vdp_data = *ptr_pal_line++;
}

void clear_vram()
{
  for (u16 i = 0; i < (0x10000 / 4); ++i)
    vdp_data_32 = 0;
}

#define PLANE_A_ADDR 0x2000
#define PLANE_B_ADDR 0x4000

#if VIDEO == PAL
#define VIDEO_SIGNAL VDP_PAL_VIDEO
#else
#define VIDEO_SIGNAL 0
#endif

u16 const default_vdp_regs[] = {
  VDP_REG_MODE1 | VDP_HICOLOR_ENABLE,
  VDP_REG_MODE2 | VDP_MD_DISPLAY_MODE | VDP_VBLANK_ENABLE | VIDEO_SIGNAL |
    VDP_DISPLAY_ENABLE,
  VDP_REG_PLA_ADDR | (PLANE_A_ADDR / 0x400),
  VDP_REG_WIN_ADDR | (0xa00 / 0x400),
  VDP_REG_PLB_ADDR | (PLANE_B_ADDR / 0x2000),
  VDP_REG_SPR_ADDR | (0xb800 / 0x200),
  VDP_REG_SPR_ADDR2,
  VDP_REG_BGCOLOR,
  VDP_REG_HBLANK_COUNT,
  VDP_REG_MODE3 | VDP_EXTINT_ENABLE,
  VDP_REG_MODE4 | VDP_MASK_WIDTH_40CELL,
  VDP_REG_HS_ADDR | (0xbc00 / 0x400),
  VDP_REG_PL_ADDR2,
  VDP_REG_AUTOINC | 2,
  VDP_REG_PL_SIZE | VDP_PL_32x32,
  VDP_REG_WIN_HPOS,
  VDP_REG_WIN_VPOS};

#define plane_xy(x, y)                                        \
  (vdp_ptr(PLANE_POS(x, y, Width32) + PLANE_A_ADDR) | VRAM_W)

void print(char const * string, vdp_addr pos)
{
  vdp_ctrl_32 = pos;
  while (*string != 0)
    vdp_data = *string++;
}

u8 counter;

void main()
{

  disable_interrupts();
  vblank_done = false;
  vblank_counter = 0;
  memcpy16(default_vdp_regs, vdp_regs, 18);
  update_vdp_regs();

  // note: seems to be important to set up vdp_regs first, then clear vram
  clear_vram();

  cram[0] = 0x0000;
  cram[1] = 0x0eee;
  cram[2] = 0x000e;
  cram[3] = 0x00e0;
  cram[4] = 0x0e00;
  update_cram();

  init_joypads();
  vdp_regs[1] |= VDP_DMA_ENABLE;
  vdp_ctrl = vdp_regs[1];
  vdp_dma_transfer(
    res_basic_font, vdp_ptr(VRAMPTR(0x20)), (res_basic_font_size << 1));
  vdp_regs[1] &= ~VDP_DMA_ENABLE;
  vdp_ctrl = vdp_regs[1];

  enable_interrupts();

  counter = 0;
  char const * test = "TESTING!";
  print(test, plane_xy(1, 1));
  __asm__("main_loop:");
  while (1)
  {
    while (! vblank_done)
      asm("nop");
    vblank_done = false;

    if (p1_single & PAD_A)
      ++counter;
  }
}

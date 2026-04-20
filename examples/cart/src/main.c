#include "res.h"
#include <main/io.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <memory.h>
#include <system.h>

#define PLANE_A_ADDR    0x2000
#define PLANE_B_ADDR    0x4000
#define SPRITE_TBL_ADDR 0xb800

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

// copy of VDP registers in memory
// these need to be updated to the VDP whenever they are updated
u16    vdp_regs[18];
Sprite sprites[80];

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
  vdp_ctrl_32 = to_vdp_addr(pal_line) | CRAM_W;
  u16 * ptr_pal_line = cram + pal_line;
  for (u8 i = 0; i < 16; ++i)
    vdp_data = *ptr_pal_line++;
}

void update_cram()
{
  vdp_ctrl_32 = to_vdp_addr(0) | CRAM_W;
  u16 * ptr_pal_line = cram;
  for (u8 i = 0; i < 64; ++i)
    vdp_data = *ptr_pal_line++;
}

void clear_vram()
{
  for (u16 i = 0; i < (0x10000 / 4); ++i)
    vdp_data_32 = 0;
}

void update_sprites()
{
  vdp_ctrl_32 = to_vdp_addr(SPRITE_TBL_ADDR) | VRAM_W;
  for (u8 i = 0; i < 80; ++i)
  {
    vdp_data_32 = ((SpriteEx) sprites[i]).as_u32[0];
    vdp_data_32 = ((SpriteEx) sprites[i]).as_u32[1];
  }
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
  VDP_REG_SPR_ADDR | (SPRITE_TBL_ADDR / 0x200),
  VDP_REG_SPR_ADDR2,
  VDP_REG_BGCOLOR,
  VDP_REG_HBLANK_COUNT,
  VDP_REG_MODE3 | VDP_EXTINT_ENABLE,
  VDP_REG_MODE4 | VDP_MASK_WIDTH_40CELL,
  VDP_REG_HS_ADDR | (0xbc00 / 0x400),
  VDP_REG_PL_ADDR2,
  VDP_REG_AUTOINC | 2,
  VDP_REG_PL_SIZE | VDP_PL_64x32,
  VDP_REG_WIN_HPOS,
  VDP_REG_WIN_VPOS};

#define plane_xy(x, y)                                                \
  (to_vdp_addr(VDP_PLANE_POS(x, y, Width64) + PLANE_A_ADDR) | VRAM_W)

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
  memcpy16(res_megadev_pal, &cram[16], 16);
  update_cram();

  init_joypads();
  vdp_regs[1] |= VDP_DMA_ENABLE;
  vdp_ctrl = vdp_regs[1];
  vdp_dma_transfer(
    res_basic_font, to_vdp_addr(tile_offset(0x20)), (res_basic_font_size << 1));
  vdp_dma_transfer(
    res_letters, to_vdp_addr(tile_offset(0x60)), (res_letters_size << 1));
  vdp_regs[1] &= ~VDP_DMA_ENABLE;
  vdp_ctrl = vdp_regs[1];

  u16 base_pos_x = 175;
  u16 base_pos_y = 200;

  sprites[0].pos_x = base_pos_x;
  sprites[0].pos_y = base_pos_y;
  sprites[0].tile = 96;
  sprites[0].width = 3;
  sprites[0].height = 2;
  sprites[0].palette = 1;
  sprites[0].next = 1;

  sprites[1].pos_x = sprites[0].pos_x + 32;
  sprites[1].pos_y = base_pos_y;
  sprites[1].tile = 108;
  sprites[1].width = 3;
  sprites[1].height = 2;
  sprites[1].palette = 1;
  sprites[1].next = 2;

  sprites[2].pos_x = sprites[1].pos_x + 32;
  sprites[2].pos_y = base_pos_y;
  sprites[2].tile = 120;
  sprites[2].width = 3;
  sprites[2].height = 2;
  sprites[2].palette = 1;
  sprites[2].next = 3;

  sprites[3].pos_x = sprites[2].pos_x + 32;
  sprites[3].pos_y = base_pos_y;
  sprites[3].tile = 132;
  sprites[3].width = 3;
  sprites[3].height = 2;
  sprites[3].palette = 1;
  sprites[3].next = 4;

  sprites[4].pos_x = sprites[3].pos_x + 32;
  sprites[4].pos_y = base_pos_y;
  sprites[4].tile = 144;
  sprites[4].width = 3;
  sprites[4].height = 2;
  sprites[4].palette = 1;
  sprites[4].next = 5;

  sprites[5].pos_x = sprites[4].pos_x + 32;
  sprites[5].pos_y = base_pos_y;
  sprites[5].tile = 108;
  sprites[5].width = 3;
  sprites[5].height = 2;
  sprites[5].palette = 1;
  sprites[5].next = 6;

  sprites[6].pos_x = sprites[5].pos_x + 32;
  sprites[6].pos_y = base_pos_y;
  sprites[6].tile = 156;
  sprites[6].width = 3;
  sprites[6].height = 2;
  sprites[6].palette = 1;
  sprites[6].next = 0;

  update_sprites();

  enable_interrupts();

  counter = 0;
  char const * subtitle = "MEGA DRIVE AND MEGA CD DEVELOPMENT KIT";
  print(subtitle, plane_xy(1, 16));
  __asm__("main_loop:");
  u8  sin_pos = 0;
  u16 wait = 0;

  while (1)
  {
    while (! vblank_done)
      asm("nop");
    vblank_done = false;

    if (p1_hold & PAD_LEFT)
      --base_pos_x;
    if (p1_hold & PAD_RIGHT)
      ++base_pos_x;
    if (p1_hold & PAD_UP)
      --base_pos_y;
    if (p1_hold & PAD_DOWN)
      ++base_pos_y;

    if (wait >= 1)
    {
      sprites[0].pos_x = base_pos_x;
      sprites[0].pos_y = base_pos_y + (sintab[sin_pos] >> 4);
      sprites[1].pos_x = sprites[0].pos_x + 32;
      sprites[1].pos_y = base_pos_y + (sintab[sin_pos + 8] >> 4);
      sprites[2].pos_x = sprites[1].pos_x + 32;
      sprites[2].pos_y = base_pos_y + (sintab[sin_pos + 16] >> 4);
      sprites[3].pos_x = sprites[2].pos_x + 32;
      sprites[3].pos_y = base_pos_y + (sintab[sin_pos + 24] >> 4);
      sprites[4].pos_x = sprites[3].pos_x + 32;
      sprites[4].pos_y = base_pos_y + (sintab[sin_pos + 32] >> 4);
      sprites[5].pos_x = sprites[4].pos_x + 32;
      sprites[5].pos_y = base_pos_y + (sintab[sin_pos + 40] >> 4);
      sprites[6].pos_x = sprites[5].pos_x + 32;
      sprites[6].pos_y = base_pos_y + (sintab[sin_pos + 48] >> 4);
      sin_pos += 8;
      wait = 0;
    }
    else
    {
      wait++;
    }
    update_sprites();
  }
}

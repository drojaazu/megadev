/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/exvec_report.c
 * @brief Crash report on screen and over the serial port (main/exvec.h)
 *
 * @details
 * Main CPU only. Sets up the VDP itself rather than relying on the crashed
 * program's settings or on Boot ROM state.
 */

#include <font8x8.h>
#include <main/comm.h>
#include <main/exvec.h>
#include <main/gate_arr.h>
#include <main/memmap.def.h>
#include <main/text.h>
#include <main/vdp.h>
#include <stdio.h>

#ifdef EXVEC_FONT_BIOS
#if TARGET == MEGADRIVE
#error "EXVEC_FONT=BIOS needs the Mega CD Boot ROM"
#endif
#include <main/bios.def.h>
#endif

#define EXVEC_PLANE 0xC000
#define EXVEC_FONT_TILE 0x20

static u8 exvec_row;

static char const * exvec_name(u16 id)
{
  switch (id)
  {
    case EXVEC_ID_BUS_ERROR:
      return "BUS ERROR";
    case EXVEC_ID_ADDRESS_ERROR:
      return "ADDRESS ERROR";
    case EXVEC_ID_ILLEGAL:
      return "ILLEGAL INSTRUCTION";
    case EXVEC_ID_ZERO_DIVIDE:
      return "DIVIDE BY ZERO";
    case EXVEC_ID_CHK:
      return "CHK OUT OF RANGE";
    case EXVEC_ID_TRAPV:
      return "TRAPV OVERFLOW";
    case EXVEC_ID_PRIVILEGE:
      return "PRIVILEGE VIOLATION";
    case EXVEC_ID_TRACE:
      return "TRACE";
    case EXVEC_ID_LINE_1010:
      return "LINE 1010 INSTRUCTION";
    case EXVEC_ID_LINE_1111:
      return "LINE 1111 INSTRUCTION";
    case EXVEC_ID_UNINITIALIZED:
      return "UNINITIALIZED INTERRUPT";
    case EXVEC_ID_SPURIOUS:
      return "SPURIOUS INTERRUPT";
    default:
      return "EXCEPTION";
  }
}

#ifdef EXVEC_FONT_BIOS
// The Boot ROM's font, found through its font loading routine: the
// BIOS_LOAD_FONT slot branches to code that loads the font's address with
// lea xxx.l, a1.
static void const * exvec_font(void)
{
  u16 const * slot = (u16 const *) BIOS_LOAD_FONT;
  u16 const * p = (u16 const *) ((u8 const *) slot + 2 + (s16) slot[1]);
  for (u8 i = 0; i < 16; ++i, ++p)
    if (*p == 0x43F9)
      return (void const *) (BOOT_ROM + (((u32) p[1] << 16) | p[2]));
  return font8x8;
}
#else
#define exvec_font() ((void const *) font8x8)
#endif

static void exvec_screen(void)
{
  static u16 const regs[] = {
    0x8004, // mode 1
    0x8144, // mode 2: display on, mode 5, no interrupts, no DMA
    0x8200 | (EXVEC_PLANE / 0x400), // plane A
    0x8407, // plane B at 0xE000
    0x8578, // sprites at 0xF000
    0x8700, // background color 0
    0x8B00, // full-screen scrolling
    0x8C81, // 40 cells wide
    0x8D3F, // horizontal scroll table at 0xFC00
    0x8F02, // autoincrement 2
    0x9001, // planes 64x32
    0x9100, // no window
    0x9200,
  };

  while (vdp_ctrl & 2) // wait for any DMA to finish
    ;
  for (u8 i = 0; i < sizeof regs / sizeof regs[0]; ++i)
    vdp_ctrl = regs[i];

  vdp_ctrl_32 = CRAM_W;
  vdp_data = 0x0400; // background: dark blue
  vdp_data = 0x0EEE; // text: white
  vdp_ctrl_32 = VSRAM_W;
  vdp_data_32 = 0;

  // planes A and B filled with spaces; sprite and scroll tables cleared
  vdp_ctrl_32 = to_vdp_addr(EXVEC_PLANE) | VRAM_W;
  for (u16 n = 0x3000 / 2; n; --n)
    vdp_data = EXVEC_FONT_TILE;
  for (u16 n = 0x1000 / 2; n; --n)
    vdp_data = 0;

  vdp_load_1bpp_tiles(exvec_font(), FONT8X8_CHARS, EXVEC_FONT_TILE, 1, 0);
}

static void exvec_line(char const * text)
{
  text_print(text, VDP_PLANE_XY(EXVEC_PLANE, 1, exvec_row, Width64), EXVEC_FONT_TILE);
  ++exvec_row;
  if (*(u8 volatile *) EXT_SCTRL & SCTRL_SERIAL_OUT_ENABLE_MASK)
  {
    for (; *text; ++text)
      ext_tx((u8) *text);
    ext_tx('\r');
    ext_tx('\n');
  }
}

void exvec_report(ExvecSnapshot const * snapshot, char const * cpu)
{
  char buf[40];

  exvec_screen();
  exvec_row = 1;

  snprintf(buf, sizeof buf, "%s CPU: %s", cpu, exvec_name(snapshot->vector));
  exvec_line(buf);
  exvec_line("");
  snprintf(buf, sizeof buf, "PC %08lX  SR %04X", (unsigned long) snapshot->pc, snapshot->sr);
  exvec_line(buf);
  if (snapshot->vector == EXVEC_ID_BUS_ERROR || snapshot->vector == EXVEC_ID_ADDRESS_ERROR)
  {
    u16 const info = snapshot->access_info;
    // function code: 1 and 5 are data, 2 and 6 are program fetches
    snprintf(buf, sizeof buf, "ACCESS %08lX %s %s FC%u",
      (unsigned long) snapshot->access_addr, (info & 0x10) ? "READ" : "WRITE",
      ((info & 3) == 2) ? "PROGRAM" : "DATA", info & 7);
    exvec_line(buf);
    snprintf(buf, sizeof buf, "IR %04X", snapshot->ir);
    exvec_line(buf);
  }
  exvec_line("");
  for (u8 i = 0; i < 8; ++i)
  {
    snprintf(buf, sizeof buf, "D%u %08lX  A%u %08lX", i, (unsigned long) snapshot->d[i], i,
      (unsigned long) snapshot->a[i]);
    exvec_line(buf);
  }
  exvec_line("");
  exvec_line("STACK");
  for (u8 i = 0; i < EXVEC_STACK_LONGS; i += 4)
  {
    snprintf(buf, sizeof buf, "%08lX %08lX %08lX %08lX", (unsigned long) snapshot->stack[i],
      (unsigned long) snapshot->stack[i + 1], (unsigned long) snapshot->stack[i + 2],
      (unsigned long) snapshot->stack[i + 3]);
    exvec_line(buf);
  }

  for (;;)
    asm volatile("stop #0x2700");
}

void exvec_check_sub(void)
{
  if (ga_main_reg_comstat5 != EXVEC_SUB_SIGNAL)
    return;

  u32 const addr = ((u32) ga_main_reg_comstat6 << 16) | ga_main_reg_comstat7;

  // take the Sub CPU's bus without resetting it, and select the Program RAM
  // bank that holds the snapshot
  ga_main_reg_subcpu = GA_MAIN_SUB_REQ_MASK | GA_MAIN_SUB_RESET_MASK;
  while (!(ga_main_reg_subcpu & GA_MAIN_SUB_REQ_MASK))
    ;
  ga_main_reg_memmode =
    (ga_main_reg_memmode & GA_MAIN_DMNA_MASK) | FIELD_PREP(GA_MAIN_MEMMODE_BANK, addr >> 17);

  ExvecSnapshot snapshot;
  u8 const volatile * src = (u8 const volatile *) (PRG_RAM_1M + (addr & 0x1FFFF));
  u8 * dst = (u8 *) &snapshot;
  for (u8 i = 0; i < sizeof snapshot; ++i)
    dst[i] = src[i];

  exvec_report(&snapshot, "SUB");
}

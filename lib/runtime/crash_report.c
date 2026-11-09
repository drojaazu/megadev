/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/crash_report.c
 * @brief Crash report on screen and over the serial port (main/crash.h)
 *
 * @details
 * Main CPU only. Sets up the VDP itself rather than relying on the crashed
 * program's settings or on Boot ROM state.
 */

#include <sysfont.h>
#include <main/comm.h>
#include <main/crash.h>
#include <main/gate_arr.h>
#include <main/memmap.def.h>
#include <main/text.h>
#include <main/vdp.h>
#include <stdio.h>

#ifdef CRASH_FONT_BIOS
#if TARGET == MEGADRIVE
#error "CRASH_FONT=BIOS needs the Mega CD Boot ROM"
#endif
#include <main/bios.def.h>
#endif

#define CRASH_PLANE     0xC000
#define CRASH_FONT_TILE 0x20

static u8 crash_row;

static char const * crash_name(u16 id)
{
  switch (id) {
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

#ifdef CRASH_FONT_BIOS
// The Boot ROM's font, found through its font loading routine: the
// BIOS_LOAD_FONT slot branches to code that loads the font's address with
// lea xxx.l, a1.
static void const * crash_font(void)
{
  u16 const * slot = (u16 const *) BIOS_LOAD_FONT;
  u16 const * p = (u16 const *) ((u8 const *) slot + 2 + (s16) slot[1]);
  for (u8 i = 0; i < 16; ++i, ++p)
    if (*p == 0x43F9)
      return (void const *) (BOOT_ROM + (((u32) p[1] << 16) | p[2]));
  return sysfont;
}
#else
#define crash_font() ((void const *) sysfont)
#endif

static void crash_screen(void)
{
  static u16 const regs[] = {
    0x8004, // mode 1
    0x8144, // mode 2: display on, mode 5, no interrupts, no DMA
    0x8200 | (CRASH_PLANE / 0x400), // plane A
    0x8407,                         // plane B at 0xE000
    0x8578,                         // sprites at 0xF000
    0x8700,                         // background color 0
    0x8B00,                         // full-screen scrolling
    0x8C81,                         // 40 cells wide
    0x8D3F,                         // horizontal scroll table at 0xFC00
    0x8F02,                         // autoincrement 2
    0x9001,                         // planes 64x32
    0x9100,                         // no window
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
  vdp_ctrl_32 = to_vdp_addr(CRASH_PLANE) | VRAM_W;
  for (u16 n = 0x3000 / 2; n; --n)
    vdp_data = CRASH_FONT_TILE;
  for (u16 n = 0x1000 / 2; n; --n)
    vdp_data = 0;

  vdp_load_1bpp_tiles(crash_font(), SYSFONT_CHARS, CRASH_FONT_TILE, 1, 0);
}

static void crash_line(char const * text)
{
  text_print(
    text, VDP_PLANE_XY(CRASH_PLANE, 1, crash_row, Width64), CRASH_FONT_TILE);
  ++crash_row;
  if (*(u8 volatile *) EXT_SCTRL & SCTRL_SERIAL_OUT_ENABLE_MASK) {
    for (; *text; ++text)
      ext_tx((u8) *text);
    ext_tx('\r');
    ext_tx('\n');
  }
}

void crash_report(CrashState const * snapshot, char const * cpu)
{
  char buf[40];

  crash_screen();
  crash_row = 1;

  snprintf(buf, sizeof buf, "%s CPU: %s", cpu, crash_name(snapshot->vector));
  crash_line(buf);
  crash_line("");
  snprintf(
    buf,
    sizeof buf,
    "PC %08lX  SR %04X",
    (unsigned long) snapshot->pc,
    snapshot->sr);
  crash_line(buf);
  if (
    snapshot->vector == EXVEC_ID_BUS_ERROR ||
    snapshot->vector == EXVEC_ID_ADDRESS_ERROR) {
    u16 const info = snapshot->access_info;
    // function code: 1 and 5 are data, 2 and 6 are program fetches
    snprintf(
      buf,
      sizeof buf,
      "ACCESS %08lX %s %s FC%u",
      (unsigned long) snapshot->access_addr,
      (info & 0x10) ? "READ" : "WRITE",
      ((info & 3) == 2) ? "PROGRAM" : "DATA",
      info & 7);
    crash_line(buf);
    snprintf(buf, sizeof buf, "IR %04X", snapshot->ir);
    crash_line(buf);
  }
  crash_line("");
  for (u8 i = 0; i < 8; ++i) {
    snprintf(
      buf,
      sizeof buf,
      "D%u %08lX  A%u %08lX",
      i,
      (unsigned long) snapshot->d[i],
      i,
      (unsigned long) snapshot->a[i]);
    crash_line(buf);
  }
  crash_line("");
  crash_line("STACK");
  for (u8 i = 0; i < CRASH_STACK_LONGS; i += 4) {
    snprintf(
      buf,
      sizeof buf,
      "%08lX %08lX %08lX %08lX",
      (unsigned long) snapshot->stack[i],
      (unsigned long) snapshot->stack[i + 1],
      (unsigned long) snapshot->stack[i + 2],
      (unsigned long) snapshot->stack[i + 3]);
    crash_line(buf);
  }

  for (;;)
    asm volatile("stop #0x2700");
}

void crash_check_sub(void)
{
  if (ga_main_reg_comstat5 != CRASH_SUB_SIGNAL)
    return;

  u32 const addr = ((u32) ga_main_reg_comstat6 << 16) | ga_main_reg_comstat7;

  // take the Sub CPU's bus without resetting it, and select the Program RAM
  // bank that holds the snapshot
  ga_main_reg_subcpu = GA_MAIN_SUB_REQ_MASK | GA_MAIN_SUB_RESET_MASK;
  while (! (ga_main_reg_subcpu & GA_MAIN_SUB_REQ_MASK))
    ;
  ga_main_reg_memmode = (ga_main_reg_memmode & GA_MAIN_DMNA_MASK) |
                        FIELD_PREP(GA_MAIN_MEMMODE_BANK, addr >> 17);

  CrashState          snapshot;
  u8 const volatile * src =
    (u8 const volatile *) (PRG_RAM_1M + (addr & 0x1FFFF));
  u8 * dst = (u8 *) &snapshot;
  for (u8 i = 0; i < sizeof snapshot; ++i)
    dst[i] = src[i];

  crash_report(&snapshot, "SUB");
}

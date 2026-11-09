/**
 * Pseudo-3D ground plane -- Main CPU side.
 *
 * Owns the camera, sends it to the Sub CPU once per frame, then DMAs the
 * rendered image buffer out of Word RAM into VRAM.
 *
 *   D-pad up/down     move forwards and backwards
 *   D-pad left/right  turn
 *   A + left/right    strafe
 *   C                 reset the camera
 */

#include "ipx.h"
#include "ipx.res.h"
#include "shared.h"
#include "tables.h"
#include <cpu.h>
#include <macro.h>
#include <main/bios.h>
#include <main/crash.h>
#include <main/gate_arr.h>
#include <main/io.h>
#include <main/memmap.def.h>
#include <main/memmap.h>
#include <main/mmd.h>
#include <memory.h>
#include <trace.h>
#include <types.h>

/*
  Module layout.

  ROM begins at the start of Work RAM, overwriting the security code/IP, with
  the stack and then RAM right after it. That gives 60KB of ROM space, a 1KB
  stack and 512 bytes of RAM; the split is arbitrary. MMD_DEST is where the code
  actually executes, i.e. where it is copied to after the Sub places it in Word
  RAM.
*/
GLOBAL_SYM(MODULE_ROM_ORIGIN, WORK_RAM);
GLOBAL_SYM(MODULE_RAM_ORIGIN, 0xFFF000);
GLOBAL_SYM(STACK_TOP, MODULE_RAM_ORIGIN);
GLOBAL_SYM(STACK_SIZE, 0x400);
GLOBAL_SYM(MODULE_ROM_LENGTH, STACK_TOP - STACK_SIZE - MODULE_ROM_ORIGIN);
GLOBAL_SYM(MODULE_RAM_LENGTH, 0x700);
GLOBAL_SYM(MMD_DEST, WORK_RAM);

#define TURN_RATE 2  /* sine table steps per frame */
#define MOVE_RATE 24 /* trace_pos units per frame, i.e. 3 dots */

/**
 * Signed 16x16 into 32 multiply.
 *
 * The 68000 has exactly this instruction, but GCC will not reliably select it
 * from `(s16) * (s16)` at -O1 and falls back to __mulsi3, a called routine, in
 * the innermost loop of the frame. Stating it directly costs one instruction.
 */
static inline s32 mul16(s16 a, s16 b)
{
  s32 r = a;
  asm("muls.w %1,%0" : "+d"(r) : "d"(b));
  return r;
}

void vblank_user()
{
  bios_copy_pal();
  bios_copy_sprlist();
}

vdp_cmd tiles = to_vdp_addr(vram_addr_from_tileidx(0x80)) | VRAM_W;

static s16 cam_x, cam_y, cam_angle;

/* The camera walks in the direction it faces, so movement needs the same sine
 * table the Sub CPU uses to rotate the step vectors. Kept in Q15 and scaled
 * down to trace_pos units at the point of use. */
static s16 const * const sin_tbl = ground_sin;

static void camera_reset(void)
{
  cam_x = to_trace_pos(512.0);
  cam_y = to_trace_pos(512.0);
  cam_angle = 0;
}

__attribute__((noreturn)) void main()
{
  *bios_vblank_user = vblank_user;
  // crash handler for Main CPU exceptions (docs/crash_handler.md)
  crash_install();

  ga_main_grant_2m();

  memset8(0, (u8 *) &bios_sprlist, sizeof(bios_sprlist));
  enable_interrupts();
  bios_clear_vram();
  bios_palette[1] = 0xEEE;
  bios_vdp_update_flags |= BIOS_MASK_COPY_PALETTE;
  bios_load_pal(&res_pal_stamps);

  camera_reset();

  do {
    bios_vblank_wait(0);
    crash_check_sub();

    /* s16 so the 68000's 16x16 multiply is used; s32 operands would call
     * __mulsi3 four times a frame for no benefit. */
    s16 const s = sin_tbl[cam_angle & GROUND_ANGLE_MASK];
    s16 const c = sin_tbl[(cam_angle + GROUND_COS_OFFSET) & GROUND_ANGLE_MASK];

    if (bios_joy1_hold & PAD_UP) {
      cam_x += (s16) (mul16(s, MOVE_RATE) >> 15);
      cam_y += (s16) (mul16(c, MOVE_RATE) >> 15);
    }
    if (bios_joy1_hold & PAD_DOWN) {
      cam_x -= (s16) (mul16(s, MOVE_RATE) >> 15);
      cam_y -= (s16) (mul16(c, MOVE_RATE) >> 15);
    }

    if (bios_joy1_hold & PAD_LEFT) {
      if (bios_joy1_hold & PAD_A) {
        /* Strafe: move along "right", which is (cos, -sin). */
        cam_x -= (s16) (mul16(c, MOVE_RATE) >> 15);
        cam_y += (s16) (mul16(s, MOVE_RATE) >> 15);
      } else
        cam_angle -= TURN_RATE;
    }
    if (bios_joy1_hold & PAD_RIGHT) {
      if (bios_joy1_hold & PAD_A) {
        cam_x += (s16) (mul16(c, MOVE_RATE) >> 15);
        cam_y -= (s16) (mul16(s, MOVE_RATE) >> 15);
      } else
        cam_angle += TURN_RATE;
    }

    if (bios_joy1_hit & PAD_C)
      camera_reset();

    ga_main_grant_2m();
    ga_main_reg_comcmd1 = cam_x;
    ga_main_reg_comcmd2 = cam_y;
    ga_main_reg_comcmd3 = cam_angle & GROUND_ANGLE_MASK;
    ga_main_reg_comcmd0 = CMD_REDRAW;

    do {
      crash_check_sub();
    } while (ga_main_reg_comstat0 == 0);

    ga_main_reg_comcmd0 = 0;

    do {
      crash_check_sub();
    } while (ga_main_reg_comstat0 != 0);

    ga_main_wait_2m();
    bios_vblank_wait_default();
    crash_check_sub();

    bios_dma_xfer_word_ram(
      tiles,
      (void const *) (WORD_RAM + 0x30000),
      (GROUND_SCREEN_W * GROUND_ROWS) / 2);

    bios_load_stamp_tilemap(
      (GROUND_SCREEN_W / 8) - 1,
      (GROUND_ROWS / 8) - 1,
      to_vdp_addr(BIOS_VDP_DEFAULT_PLANEB) | VRAM_W,
      0x80);
  } while (1);
}

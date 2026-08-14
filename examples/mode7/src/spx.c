/**
 * Pseudo-3D ground plane -- Sub CPU side.
 *
 * Builds a trace vector table describing a textured plane seen in perspective,
 * then runs one graphics operation per frame to render it into the image
 * buffer. The Main CPU DMAs the result to VRAM.
 *
 * The derivation of the maths is in docs/rotation_scaling.md section 7. The
 * short version, per output row:
 *
 *   - the row's ground distance z comes from a table, because z = K/d needs a
 *     divide and d depends only on the row
 *   - the per-pixel step is z scaled by the field of view, rotated by the
 *     camera angle
 *   - the row's start point is the camera position, walked z forward, then half
 *     a screen width back along the step direction
 */

#include "shared.h"
#include "tables.h"
#include "mode7.res.h"
#include <memory.h>
#include <sub/cdrom.h>
#include <sub/gate_arr.h>
#include <sub/memmap.h>
#include <system.h>
#include <xform.h>

#define STAMP_DATA_OFFSET  0x00000
#define STAMP_MAP_OFFSET   0x10000
#define TRACE_TABLE_OFFSET 0x20000
#define IMG_BUFFER_OFFSET  0x30000

typedef struct
{
	xform_pos x;
	xform_pos y;
	xform_delta dx;
	xform_delta dy;
} trace_entry;

/* Built here, in Program RAM, and block-copied into Word RAM in one burst.
 * Writing it directly into Word RAM would work, but every one of those writes
 * costs 3 clocks of any graphics operation still running -- see
 * docs/rotation_scaling.md section 6. */
static trace_entry trace_table[MODE7_ROWS];

/**
 * Signed 16x16 into 32 multiply.
 *
 * The 68000 has exactly this instruction, but GCC will not reliably select it
 * from `(s16) * (s16)` at -O1 and falls back to __mulsi3, a called routine, in
 * the innermost loop of the frame. Stating it directly costs one instruction.
 */
static inline s32 mul16 (s16 a, s16 b)
{
	s32 r = a;
	asm("muls.w %1,%0" : "+d"(r) : "d"(b));
	return r;
}

extern void sp_fatal();

char const * const filenames[] = {"IPX.MMD;1"};

static void load_gfx (void);
static void redraw (s16 cam_x, s16 cam_y, s16 angle);

void main()
{
	enable_interrupts();
	wait_2m();
	memset32 (0, (u32 *) WORD_RAM_2M, 0x20000 * 2);

	register s16 command, param1, param2, param3;
	do
	{
		do
		{
			command = ga_reg_comcmd0;
		} while (command == 0);

		if (command != ga_reg_comcmd0)
			continue;

		param1 = ga_reg_comcmd1;
		param2 = ga_reg_comcmd2;
		param3 = ga_reg_comcmd3;

		switch (command)
		{
			case CMD_LOAD_FILE:
				load_file (CDROM_LOAD_CDC, filenames[param1], (u8 *) WORD_RAM_2M);
				grant_2m();
				if (access_op_result != CDROM_RESULT_OK)
					sp_fatal();
				break;

			case CMD_REDRAW:
				redraw (param1, param2, param3);
				break;
		}

		ga_reg_comstat0 = ga_reg_comcmd0;
		do
		{
			asm("nop");
			command = ga_reg_comcmd0;
		} while (command != 0);
		ga_reg_comstat0 = 0;

	} while (1);
}

static void load_gfx (void)
{
	memcpy16 ((u16 const *) &res_stamp01.data,
		(u16 *) (WORD_RAM_2M + STAMP_DATA_OFFSET + (4 * 4 * 32 * 1)),
		res_stamp01.size / 2);
	memcpy16 ((u16 const *) &res_stamp02.data,
		(u16 *) (WORD_RAM_2M + STAMP_DATA_OFFSET + (4 * 4 * 32 * 2)),
		res_stamp02.size / 2);
	memcpy16 ((u16 const *) &res_stamp03.data,
		(u16 *) (WORD_RAM_2M + STAMP_DATA_OFFSET + (4 * 4 * 32 * 3)),
		res_stamp03.size / 2);
	memcpy16 ((u16 const *) &res_stamp04.data,
		(u16 *) (WORD_RAM_2M + STAMP_DATA_OFFSET + (4 * 4 * 32 * 4)),
		res_stamp04.size / 2);
	memcpy16 ((u16 const *) &res_stamp_map.data,
		(u16 *) (WORD_RAM_2M + STAMP_MAP_OFFSET), res_stamp_map.size / 2);
}

/**
 * Build one frame's trace vector table and render it.
 *
 * @param cam_x,cam_y Camera position on the plane, in xform_pos units
 * @param angle Facing, 0 to MODE7_SIN_ENTRIES-1 over a full turn
 */
static void redraw (s16 cam_x, s16 cam_y, s16 angle)
{
	wait_2m();
	load_gfx();

	/* forward = (sin a, cos a), right = (cos a, -sin a). One table serves both:
	 * cosine is sine read a quarter turn along. */
	/* Deliberately s16: the 68000 multiplies 16x16 into 32, and s32 operands
	 * would pull in __mulsi3, a called routine, once per row per frame. */
	s16 const sin_a = mode7_sin[angle & MODE7_ANGLE_MASK];
	s16 const cos_a = mode7_sin[(angle + MODE7_COS_OFFSET) & MODE7_ANGLE_MASK];

	for (u16 r = 0; r < MODE7_ROWS; ++r)
	{
		s16 const zk = mode7_zk[r]; /* step before rotation, Q4.11 */
		s16 const zf = (s16) mode7_zf[r]; /* ground distance, Q13.3 */

		/* Rotate the step. Q4.11 * Q15 gives Q26, so shift back by 15. */
		s16 const dx = (s16) (mul16 (zk, cos_a) >> 15);
		s16 const dy = (s16) (-(mul16 (zk, sin_a) >> 15));

		/* Walk z forward from the camera to reach the middle of the row, then
		 * half a screen width back along the step to reach its left end.
		 *
		 * That half-width term is (SCREEN_W/2) * step. With a 256 dot screen it
		 * reduces to dx >> 1: 128 steps of Q11 expressed in Q3 is 128/2048*8,
		 * which is exactly one half. Any other width needs the full multiply. */
		trace_table[r].x = (xform_pos) (cam_x + (mul16 (zf, sin_a) >> 15) - (dx >> 1));
		trace_table[r].y = (xform_pos) (cam_y + (mul16 (zf, cos_a) >> 15) - (dy >> 1));
		trace_table[r].dx = dx;
		trace_table[r].dy = dy;
	}

	memcpy16 ((u16 const *) trace_table,
		(u16 *) (WORD_RAM_2M + TRACE_TABLE_OFFSET),
		(sizeof (trace_table)) / 2);

	ga_reg_stampmapbase = (u16) (STAMP_MAP_OFFSET / 4);
	ga_reg_imgbufstart = (u16) (IMG_BUFFER_OFFSET / 4);
	ga_reg_stampsize = GA_STAMPSIZE_REPEAT_MASK | GA_STAMPSIZE_32x32_STAMP_MASK;
	ga_reg_imgbufoffset = 0;
	ga_reg_imgbufhdotsize = MODE7_SCREEN_W;
	ga_reg_imgbufvsize = (MODE7_ROWS / 8) - 1;

	/* Rewritten every frame on purpose: the hardware decrements this as it runs
	 * and it reads 0 on completion. Set once and reused, the second operation
	 * would silently have zero height. */
	ga_reg_imgbufvdotsize = MODE7_ROWS;

	/* Writing the trace vector base is what starts the operation, so it goes
	 * last, after every other register holds its final value. */
	ga_reg_tracevectbase = (u16) (TRACE_TABLE_OFFSET / 4);

	while (ga_reg_gfxstat & GA_GRON_MASK)
		asm("nop");

	grant_2m();
}

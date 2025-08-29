/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file vdp.macros.s
 * @brief VDP utility macros
 */

#ifndef MEGADEV__MAIN_VDP_S
#define MEGADEV__MAIN_VDP_S

#include <macros.s>

/**
 * @fn VDP_DMA_TRANSFER
 * @param[in] D0.l Destination address (in vdpcmd format)
 * @param[in] D1.l Source address
 * @param[in] D2.w Length of data (in words)
 * @clobber a6, d0-d3
 * @ingroup vdp
 * @warning Enabling/disabling the DMA Enable bit on VDP Mode Register 2 is the responsibility of the user
 */
SUB VDP_DMA_TRANSFER
  lea      (vdp_ctrl).l, a6
  asr.l    #0x1, d1
  move.l   #0x940000, d3
  move.w   d2, d3
  lsl.l    #0x8, d3
  move.w   #0x9300, d3
  move.b   d2, d3
  move.l   d3, (a6)
  move.l   #0x960000, d3
  move.w   d1, d3
  lsl.l    #0x8, d3
  move.w   #0x9500, d3
  move.b   d1, d3
  move.l   d3, (a6)
  swap     d1
  move.w   #0x9700, d3
  move.b   d1, d3
  move.w   d3, (a6)
  ori.l    #0x40000080, d0
  swap     d0
  move.w   d0, (a6)
  swap     d0
  /* DMA trigger (final write) must come from Work RAM - a quick push/pop from the stack will take care of that */
  move.w   d0, -(SP)
  move.w   (SP)+, (a6)
  rts

/**
 * @fn VDP_DMA_FILL
 * @param[in] D0.l Destination address (in vdpcmd format)
 * @param[in] D1.w Count (in bytes)
 * @param[in] D2.b Value to write
 * @clobber a6, d0, d3
 * @ingroup vdp
 * @warning Enabling/disabling the DMA Enable bit on VDP Mode Register 2 is the responsibility of the user
 */
SUB VDP_DMA_FILL
  lea      (vdp_ctrl).l,a6
  move.l   #0x00940000, d3
  move.w   d1.w, d3.w
  lsl.l    #0x8, d3
  move.w   #0x9300, d3.w
  move.b   d1.b, d3.b
  move.l   d3, (a6)
  move.w   #0x9780, (a6)
  ori.l    #0x40000080, d0
  move.l   d0, (a6)
  move.b   d2.b, (-0x4,a6)
  /* wait for DMA in progress flag to clear */
0:move.w   (a6), d3.w
  btst.l   0x1, d3
  bne.b    0b
  rts

#endif

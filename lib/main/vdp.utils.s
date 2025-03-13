   *******************************************************************************************************
   * Performs a Main CPU -> VRAM DMA transfer                                                            *
   *                                                                                                     *
   * IN:                                                                                                 *
   *  d0 - vram dest                                                                                     *
   *  d1 - source                                                                                        *
   *  d2 - length (see note)                                                                             *
   *                                                                                                     *
   * BREAK: d3, a6                                                                                       *
   *                                                                                                     *
   * Note that length will need to be the size of the data divided by 2! This is taken care of in the... *
   *******************************************************************************************************
                      void _BOOT_DMA_XFER(long vdp
                      Tags: DMA, VDP
         void         <VOID>    <RETURN>
         long         D0:4      vdpaddr_dest
         byte *       D1:4      source
         word         d2.w:2     length
         word         Stack[-0x dma_trigger                 XREF[1]: 00000ac4(*)  
 // _BOOT_DMA_XFER

/**
 * @def vdp_dma_transfer
 * @param[in] D0.l Destination address (in VDP_COMMAND format)
 * @param[in] D1.l Source address
 * @param[in] D2.w Length of data (in words)
 * @clobber a6, d0-d3
 * @ingroup vdp
 * @warning Enabling/disabling the DMA Enable bit on VDP Mode Register 2 is the responsibility of the user
 */
SUB vdp_dma_transfer
  lea      (VDP_CTRL).l, A6
  asr.l    #0x1, d1
  move.l   #0x940000, d3
  move.w   d2, d3
  lsl.l    #0x8, d3
  move.w   #0x9300, d3
  move.b   d2, d3
  move.l   d3, (A6)
  move.l   #0x960000, d3
  move.w   d1, d3
  lsl.l    #0x8, d3
  move.w   #0x9500, d3
  move.b   d1, d3
  move.l   d3, (A6)
  swap     d1
  move.w   #0x9700, d3
  move.b   d1, d3
  move.w   d3, (A6)
  ori.l    #0x40000080, d0
  swap     d0
  move.w   d0, (A6)
  swap     d0
  // DMA trigger (final write) must come from RAM, so we do a quick push/pop from the stack
  move.w   d0, -(SP)
  move.w   (SP)+, (A6)
  rts


   *******************************************************************************************************
   * Fills a VRAM region with zeroes.                                                                    *
   *                                                                                                     *
   * IN:                                                                                                 *
   *  d0 - vdpaddr start                                                                                 *
   *  d1 - count (in bytes)                                                                              *
   *                                                                                                     *
   * BREAK: d0-d3/a6                                                                                     *
   *******************************************************************************************************
                      void _BOOT_DMA_FILL_CLEAR(vo
                      Tags: DMA, VDP
         void         <VOID>    <RETURN>
 _BOOT_DMA_FILL_CLEAR                          XREF[3]: 
moveq    #0x0,D2
   *******************************************************************************************************
   * Performs a DMA fill operation.                                                                      *
   *                                                                                                     *
   * IN:                                                                                                 *
   *  d0.l - vdpaddr start                                                                               *
   *  d1.w - count (in bytes)                                                                            *
   *  d2.b - value to write                                                                              *
   *                                                                                                     *
   * BREAK: d0-d3/a6                                                                                     *
   *******************************************************************************************************
                      undefined _BOOT_DMA_FILL()
                      Tags: DMA, VDP
         undefined    D0b:1     <RETURN>
 _BOOT_DMA_FILL                                XREF[1]: 
lea      (VDP_CTRL).l,A6
           = ??
move.w   #0x8F01,(A6)=>VDP_CTRL
           = ??
move.w   (VDPREG_MIRR_01).w,d3.w
           = ??
bset.l   0x4,D3
move.w   d3.w,(A6)=>VDP_CTRL
           = ??
move.l   #0x940000,D3
move.w   d1.w,d3.w
lsl.l    #0x8,D3
move.w   #0x9300,d3.w
move.b   d1.b,d3.b
move.l   D3,(A6)=>VDP_CTRL
           = ??
move.w   #DAT_00009780,(A6)=>VDP_CTRL
           = D5h
           = ??
ori.l    #0x40000080,D0
move.l   D0,(A6)=>VDP_CTRL
           = ??
move.b   d2.b,(-0x4,A6)=>VDP_DATA
                     = ??
 LAB_00000a66                                  XREF[1]: 
move.w   (A6)=>VDP_CTRL,d3.w
           = ??
btst.l   0x1,D3
bne.b    LAB_00000a66
move.w   (VDPREG_MIRR_01).w,(A6)=>VDP_CTRL
           = ??
move.w   #0x8F02,(A6)=>VDP_CTRL
           = ??
rts

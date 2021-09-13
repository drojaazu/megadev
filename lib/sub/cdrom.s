/**
 * \file
 * CD-ROM File Access API
 * Subdirectories are *not* supported. All files should be in the root.
 */

#ifndef MEGADEV__CD_SUB_CDROM_S
#define MEGADEV__CD_SUB_CDROM_S

#include "macros.s"
#include "sub/cdrom_def.h"
#include "sub/sub_macros.s"
#include "sub/bios_def.h"

.section .text

/**
 * \fn load_file_prg_dma
 * \brief Convenience sub to load a file to PRG RAM via DMA
 * \param[in] A0.l Pointer to filename string
 * \note Be sure to set the destination in the _GA_DMAADDR register
 * beforehand
 */
load_file_prg_dma:
  move.w  #ACC_OP_LOAD_PRG_DMA, access_op
  jbra load_file

/**
 * \fn load_file_sub
 * \brief Convenience sub to get a file loaded
 * \param[in] A0.l Pointer to filename string
 * \param[in] A1.l Pointer to destination buffer
 */
load_file_sub:
  move.w  #ACC_OP_LOAD_CDC, access_op
	move.l  a1, filebuff
load_file:
  move.l  a0, filename
0:jbsr    _WAITVSYNC
  jbsr check_acc_op
  bcs      0b  // wait for completion
  // should have the caller check the status
//  cmpi.w  #RESULT_OK, d0  /*check status*/
//  bne      load_file_sub        /*try again if bad status*/
  rts

/**
 * \fn find_file
 * \brief Get a pointer to the cached file info entry
 * \param[in] A0.l Pointer to file name
 * \param[out] CS File not found
 * \param[out] CC File found
 * \param[out] A0.l Pointer to file list entry (if found)
 * \break d0-d1/a1-a2
 */
.global find_file
find_file:
  // part 1 - get filename length
  // we need this for string comparison
  moveq    #0, d1     // d1 will hold the size
  movea.l  a0, a1     // a1 is the string work register
  moveq    #0xa, d0   // max length of filename (without version info) - 11 bytes
0:tst.b    (a1)       // begin check for end of string
  beq      1f         // Hit \0 - end of filename string
  cmpi.b   #';', (a1)  // Hit ; - end of filename string
  beq      1f
  cmp.b    #' ', (a1)  // Hit space - end of filename string (or invalid)
  beq      1f
  addq.w   #1, d1       // increment size
  addq.l   #1, a1       // increment char pointer
  dbf      d0, 0b

  // part 2 - find the filename in the directory cache
1:move.w   dir_entry_count, d0  // for each file in the dir
  subq.w   #1, d0          // -1 for loop index
  movea.l  a0, a1          // point string work reg to filename
  lea      dir_cache, a2   // directory cache in a2
2:COMPARE_STRING           // compare the filename to this dir entry
  beq      3f              // found the file!
  adda.w   #22, a2       // not found, move to next entry
  dbf      d0, 2b

  move     #1, ccr     // couldn't find the file! report file not found
  rts

3:movea.l  a2, a0      // set a0 to ptr to the directory entry
  move     #0, ccr     // report file found
  rts


/**
 * \fn check_acc_op
 * \brief Check if an access operation is still in proces. If  completed,
 * returns the last result code
 * \param[out] CS Access op in progress
 * \param[out] CC Access op complete (access loop is idle)
 * \param[out] D0.w Result code (if op is complete)
 * \param[out] D1.w Size of file in bytes (if result is OK); number of sectors
 * read (if result not OK)
 */
check_acc_op:
  cmpi.w   #ACC_OP_IDLE, access_op    // if status is anything but idle
  bne      2f                         // then jump down
  move.w   access_op_result, d0
  cmpi.w   #RESULT_OK, d0          // check for good status
  bne      0f                      // not good, jump down
  move.l   filesize, d1   // status ok, get size of the last read record
  bra      1f
0:cmpi.w   #RESULT_LOAD_FAIL, d0   // check for load file failure
  bne      1f
  move.w   sectors_read_count, d1  // if so, return number of sectors read
1:move     #0, ccr  // operation completed
  rts
2:move     #1, ccr  // operation in progress
  rts

/*
  Functions below this point shouldn't be called by the user
*/
/**
 * \fn access_op_idle
 * \brief CD-ROM access operation switch
 * \warning The operation should be requested by setting the access_op value rather
 * than calling any of these functions directly
 */
access_op_idle:
  jbsr      accloop_break
.global op_switch
op_switch:
  move.w   access_op, d0
  add.w    d0, d0
  move.w   op_jmptbl(pc,d0.w), d0
  jbra     op_jmptbl(pc,d0.w)
  // TODO this rts is unnecessary right?
  rts

op_jmptbl:
  .word    access_op_idle - op_jmptbl
  .word    access_op_load_dir - op_jmptbl
  .word    access_op_load_dma_word - op_jmptbl
  .word    access_op_load_sub - op_jmptbl
  .word    access_op_load_dma_prg - op_jmptbl
  .word    access_op_load_dma_pcm - op_jmptbl

/**
 * \fn access_op_load_dma_word
 * \brief Load a file to Word RAM via DMA
 */ 
access_op_load_dma_word:
  move.b   #CDC_DEST_WRAMDMA, cdc_dev_dest
  move.l   #load_data_dma, (load_method_ptr)
  jbra     load_process

/**
 * \fn access_op_load_sub
 * \brief Load a file to a Sub CPU address space
 */
access_op_load_sub:
  move.b   #CDC_DEST_SUBREAD, cdc_dev_dest
  move.l   #load_data_sub, (load_method_ptr)
  jbra     load_process

/**
 * \fn access_op_load_dma_prg
 * \brief Load a file to PRG RAM via DMA
 */
access_op_load_dma_prg:
  move.b   #CDC_DEST_PRAMDMA, cdc_dev_dest
  move.l   #load_data_dma, (load_method_ptr)
  jbra     load_process

/**
 * \fn access_op_load_dma_pcm
 * \brief Load a file to PCM Wave Data memory via DMA
 */
access_op_load_dma_pcm:
  move.b   #CDC_DEST_PCMDMA, cdc_dev_dest
  move.l   #load_data_dma, (load_method_ptr)

/**
 * \fn load_process
 * \brief Shared code for the file load operations
 */
load_process:
  movea.l  (filename), a0
  jbsr     find_file       // get file info from dir cache
  bcs      load_proc_notfound  // jump down if file not found
  move.l   14(a0), cdread_sector_start  // get start sector
  move.l   18(a0), d1                   // get file size (bytes)
  move.l   d1, filesize
  /*get the file size in sectors by 'dividing' by 2048*/
  /*TODO: access_op_load_dir actually uses divu... use that here too?*/
  // or can't we just shift right 11 bits...?
  move.l   #1, cdread_sector_count   // file must be at least 1 sector
0:subi.l   #0x800, d1                // subtract 2K bytes (1 sector)
  ble      1f                        // we've hit the end of the file
  addq.l   #1, cdread_sector_count   // add 1 to the sector count
  bra      0b
test_label:
1:movea.l  load_method_ptr, a0
  jbsr     (a0)                      // actually load some data
3:move.w   #ACC_OP_IDLE, access_op   // set the acc loop back to idle
  bra      access_op_idle            // and jump back into the loop
load_proc_notfound:
  /* we set the not found result here as opposed to the find_file subroutine
   * so we don't tamper with the load process results, in case find_file is
   * called elsewhere
  */
  move.w   #RESULT_NOT_FOUND, access_op_result
  bra      3b

/**
 * \fn access_op_load_dir
 * \brief Load and cache the root directory entries (filename, offset, size)
 */
access_op_load_dir:
  move.b  #3, cdc_dev_dest            // set CDC data destination

  // part 1 - load primary volume descriptor
  move.l   #0x10, cdread_sector_start  // primary VD is at sector 0x10
  move.l   #1, cdread_sector_count     // read one frame (sector)
  lea      sector_buffer, a0           // temporary data buffer in a0
  move.l   a0, filebuff         // make it the data destination
  bsr      load_data_sub               // actually get data
  cmpi.w   #RESULT_LOAD_FAIL, access_op_result  // check for problems
  beq      cdacc_loop_loaddir_err      // read failed, jump down
  lea      sector_buffer, a1           // PVD is in the buffer now
  move.l   0xa2(a1), cdread_sector_start  // start sector of root dir record
  move.l   0xaa(a1), d0                   // size of root dir record (bytes)
  divu.w   #0x800, d0          // get size in sectors
  swap     d0                  // check for modulus
  tst.w    d0
  beq      0f
  addi.l   #0x10000, d0        // if there's a remainder, add a sector
                               // (value is swapped right now; upper word
                               // is sector count)
0:swap     d0                  // back to normal order
  move.w   d0, record_size

  // part 2 - loop over root dir sectors and build directory cache
  clr      dir_entry_count
1:move.l   #1, cdread_sector_count  // read one sector
  lea      sector_buffer, a1
  move.l   a1, filebuff      // setup destination
  bsr      load_data_sub            // actually get data
  cmpi     #RESULT_LOAD_FAIL, access_op_result  // any issues?
  beq      cdacc_loop_loaddir_err   // if so, jump down
  lea      dir_cache, a0    
  move.w   dir_entry_count, d0      // this will be 0 on the sector
  mulu.w   #0x20, d0                // each file entry is 0x20 bytes
  # TODO:  shift left by 5 instead of mulu?
  adda.l   d0, a0    /*move up to the latest file entry offset*/
  lea      sector_buffer, a1
  moveq    #0, d0
2:move.b   0(a1), d0  // no more entries? (size is 0)
  beq      7f         // no more, jump down
  move.l   6(a1), 14(a0)      //file start sector (big endian)
  move.l   0xe(a1), 18(a0)    //file size in bytes (big endian)
  moveq    #0, d1               // d1 will be filename char index
4:move.b   0x21(a1,d1.w), (a0,d1.w)  // filename
  addq.w   #1, d1
  cmp.b    0x20(a1), d1  // length of filename (including version suffix)
  blt      4b            // not done with filename yet
5:cmpi.b   #0xc, d1      /// is filename less than 0xC characters in length?
  bge      6f            // no, jump down
  move.b   #' ', (a0, d1.w)    // yes, fill with spaces until it's 0xC length
  addq.w   #1, d1
  bra      5b
6:addq.w   #1, dir_entry_count  //this file entry is done, add it to the count
  adda.l   d0, a1      //move to next entry in dir record
                       // (d0 holds dir record length)
  adda.l   #22, a0   // move to next entry in the file list
  bra      2b          // and do it all again
7:subq.w   #1, record_size  // any more sectors left in the dir record?
  bne      1b          // yes, jump back and do it all again
  move.w   #RESULT_OK, access_op_result  // we're good here
load_file_list_end:
  move.w   #ACC_OP_IDLE, access_op  // free and return to access loop
  bra      access_op_idle
cdacc_loop_loaddir_err:
  move.w   #RESULT_DIR_FAIL, access_op_result  // indicate bad result
  bra      load_file_list_end


/**
 * \fn load_data_sub
 * \brief Load data using CDCTRN (only available for Sub CPU Read)
 */
load_data_sub:
  // we want to save the call site in order to properly return, since we'll
  // be messing with the stack by calling accloop_break
  POP       return_ptr
  move.w    #0, sectors_read_count
  move.w    #0x1e, read_retry_count

load_data_begin:
  move.b   cdc_dev_dest, (_GA_CDCMODE)
  lea      cdread_sector_start, a0 // point to sector struct for ROMREADN

  /*
    Next we want to partially convert the start sector to MM:SS:FF format and
    get the frame value in particular. This will be used for error checking by
    confirming that the CDC has read the frame (i.e. the sector) we were
    expecting.
  */
  move.l   (a0), d0  // retrieve the start sector
  divu     #75, d0   // get the number of seconds (75 frames / second)
  swap     d0        // put the modulus in the lower half
  HEX2BCD            // convert the value to BCD (because the CDC returns the
                     // value MM:SS:FF format)
  move.b   d0, cdc_frame_check  // cache for later error checking

  CDBIOS #CDCSTOP     // stop any current CDC transfers
  CDBIOS #ROMREADN    // begin the data read

  move.w  #0x258, read_timeout
1:bsr      accloop_break  /*take a break here and come back next VINT*/
2:CDBIOS #CDCSTAT            /*check CDC status since our read call*/
  bcc      3f              /*we have a sector read to be read*/
  subq.w  #1, read_timeout  /*count down read timeout & try again*/
  bge      1b
  subq.w  #1, read_retry_count  /*count down read retry & try again*/
  bge      load_data_begin
  bra      load_data_failure          /*failed completely, jump down*/

3:CDBIOS #CDCREAD    /*read out the data from the CDC*/
  bcs      4f      /*sector not ready, this shouldn't happen since*/
                  /*CDCSTAT said we were good; jump down & try again*/
  move.l  d0, cdc_read_timecode  /*the timecode for the read sector comes back in d0*/
  move.b  cdc_frame_check, d0    /*bring back the frame count we calculated earlier*/
  cmp.b    cdc_read_timecode+2, d0  /*and check it against the frame count from CDC*/
  beq      5f                      /*things looks good, let's keep going*/
4:subq.w  #1, read_retry_count  /*count down read retry & try again*/
  bge      load_data_begin
  bra      load_data_failure
5:move.w  #0x7ff, d0        /*wait for Data Set Ready flag from CDC*/
  btst    #CDCMODE_DSR_BIT-8, (_GA_CDCMODE).l
  dbne    d0, 5b
  bne      6f
  subq.w  #1, read_retry_count  /*no response from CDC in time, retry*/
  bge      load_data_begin
  bra      load_data_failure
6:cmpi.b  #2, cdc_dev_dest  /*is this a main CPU read?*/
  beq      load_data_maincpudest    /*if so, jump down; main cpu can't use CDCTRN*/
  movea.l  (filebuff), a0  /*setup CDCTRN pointers*/
  lea      cdc_read_timecode, a1
  CDBIOS #CDCTRN            /*transfer data from CDC to RAM*/
  bcs      7f
  move.b  cdc_frame_check, d0      /*check against our expected frame count again*/
  cmp.b    cdc_read_timecode+2, d0  
  beq      8f                    /*frame count is good, move on*/
7:subq.w  #1, read_retry_count  /*frame count didn't match, retry*/
  bge      load_data_begin
  bra      load_data_failure
  /*why set ccr here? Calling code expected a status value in RAM, not condition code.
  Probably leftover from example code...?*/
8:move    #0, ccr
  moveq    #1, d1    /*prepare to move to next frame*/
  abcd    d1, d0    /*d0 has cdc_frame_check (as BCD), add one to it*/
  move.b  d0, cdc_frame_check      /*TODO: check if this can be re-arranged*/
  cmpi.b  #0x75, cdc_frame_check  /* check if we're past 75 frames (0 indexed,BCD)*/
  bcs      9f        /*not yet*/
  move.b  #0, cdc_frame_check    /*frame counter rolled past 75, reset our check*/
9:CDBIOS #CDCACK        /*send ack to CDC*/
  move.w  #6, read_timeout    /*reset error counters*/
  move.w  #0x1e, read_retry_count
  addi.l  #0x800, filebuff  /*move the dest buffer up a sector*/
  addq.w  #1, sectors_read_count    /*add to the sectors read count*/
  addq.l  #1, cdread_sector_start  /*move to the next frame*/
  subq.l  #1, cdread_sector_count  /*countdown frames to be read*/
  bgt      2b          /*and loop back if there are still frames pending*/
  move.w  #RESULT_OK, access_op_result  /*indicate we completed successfully*/
load_data_end:
  move.b  cdc_dev_dest, _GA_CDCMODE
  movea.l  return_ptr, a0  /*return to the original call site*/
  jmp      (a0)
load_data_failure:
  move.w  #RESULT_LOAD_FAIL, access_op_result
  bra      load_data_end
  # not entirely certain how this works with Main CPU being the destination,
  # since I'm not 100% clear on the effect of CDC mode
load_data_maincpudest:
  move.w  #6, read_timeout
1:bsr      accloop_break
  btst    #7, _GA_CDCMODE  /*check EDT*/
  bne      9b
  subq.w  #1, read_timeout
  bge      1b
  bra      load_data_failure



/**
 * \fn load_data_dma
 * \brief Load data without CDCTRN (for DMA processes)
 */
load_data_dma:
  // we want to save the call site in order to properly return, since we'll
  // be messing with the stack by calling accloop_break
  POP       return_ptr
  move.w    #0, sectors_read_count
  move.w    #0x1e, read_retry_count

load_data_dma_begin:
  move.b   cdc_dev_dest, (_GA_CDCMODE)
  lea      cdread_sector_start, a0  // point to sector struct for ROMREADN

  /*
    Next we want to partially convert the start sector to MM:SS:FF format and
    get the frame value in particular. This will be used for error checking by
    confirming that the CDC has read the frame (i.e. the sector) we were
    expecting.
  */
  move.l   (a0), d0  // retrieve the start sector
  divu     #75, d0   // get the number of seconds (75 frames / second)
  swap     d0        // put the modulus in the lower half
  HEX2BCD            // convert the value to BCD (because the CDC returns the
                     // value MM:SS:FF format)
  move.b   d0, cdc_frame_check  // cache for later error checking

  CDBIOS #CDCSTOP     // stop any current CDC transfers
  CDBIOS #ROMREADN    // begin the data read

  move.w   #0x258, read_timeout // set up for reading
1:bsr      accloop_break    // take a break here and come back next VINT
2:CDBIOS #CDCSTAT               // check on the CDC on the status of our data
  bcc      3f                // sector is ready! jump down
  subq.w   #1, read_timeout  // not ready yet,count down read timeout
  bge      1b                // and try again
  subq.w   #1, read_retry_count  // decrement read retry
  bge      load_data_dma_begin   // and try again
  bra      load_data_dma_failure // failed after all attempts, return error

3:CDBIOS #CDCREAD     // read out the data from the CDC to the destination
  bcs      4f      // data not ready, this really shouldn't happen since
                   // CDCSTAT said we were good; jump down & try again
  lsr      #8, d0  // the timecode for the loaded sector comes back in d0
                   // shift it over to get the frame offset into the low byte
  move.b   cdc_frame_check, d1   // bring back the expected frame offset...
  cmp.b    d1, d0                // and compare the two to make sure they match
  beq      6f                    // all good, let's keep going
4:subq.w   #1, read_retry_count  // didn't match, decrement read retry...
  bge      load_data_dma_begin   // and try again
  bra      load_data_dma_failure // failed after all attempts, return error

6:move.w   #6, read_timeout  // next we want the signal from the CDC that 
                             // everything is done
7:bsr       accloop_break   // give it some time...
  btst     #CDCMODE_EDT_BIT-8, _GA_CDCMODE  // check that the EDT bit is set
  beq       0f               // not set yet, retry
  move.b   (cdc_frame_check), d0  // CDC is done, let's prepare for next frame
  moveq    #1, d1                 // grab the error check value
  abcd     d1, d0                 // and BCD add 1 to it, because we are going
                                  // to expect the next frame (sector)
  cmp.b    #0x76, d0                // have we exceeded the 75th frame?
  bne      8f                     // no, jump down
  moveq    #0, d0                 // yes, reset expected frame to 0
8:move.b   d0, (cdc_frame_check)  // cache our expected frame
  bra      9f                     // and keep it moving!

0:subq.w   #1, read_timeout
  bge      7b
  bra      load_data_dma_failure

9:CDBIOS #CDCACK          // send ack to CDC (required after every sector/frame)
  move.w   #6, read_timeout          // reset error counters for next sector
  move.w   #0x1e, read_retry_count
  addq.w   #1, sectors_read_count    // add to the sectors loaded count
  subq.l   #1, cdread_sector_count   // decrement remaining sector count
  bgt       2b                 // loop back if there are still frames pending
  move.w   #RESULT_OK, access_op_result  // all data loaded!
load_data_dma_return:
  move.b  cdc_dev_dest, _GA_CDCMODE // write the dest. again to reset DMA
  movea.l  return_ptr, a0          // return to the original call site
  jmp      (a0)
load_data_dma_failure:
  move.w   #RESULT_LOAD_FAIL, access_op_result
  bra      load_data_dma_return

/*
  Pops the last address from the stack and stores
  in the INT2 call ptr
  Should be called as subroutine rather than directly
*/
accloop_break:
  POP      acc_loop_jump
  // TODO shouldn't this be an RTE?
  rts

.section .bss

/**
 * Points to the last location in the access loop
 */
 .global acc_loop_jump
acc_loop_jump: .long 0

load_method_ptr: .long 0

// the two longs are the table used by ROMREADN/ROMREADE, so
//# it is necessary that there are two consecutive long values!
cdread_sector_start: .long 0
cdread_sector_count: .long 0

.global filename
filename: .long 0

.global filebuff
filebuff: .long 0

.global filesize
filesize: .long 0

.global access_op
access_op: .word 0

.global access_op_result
access_op_result: .word 0

sectors_read_count: .word  0

/**
 * The loop count for checking on whether data is ready from CDC.
 */
read_timeout: .word 0

/**
 * The number of attempts to read a file if a read timeout occurs
 */
read_retry_count: .word 0

cdc_read_timecode: .long  0

.align 2
cdc_frame_check: .byte  0

.align 2
return_ptr: .long 0

/**
 * CDC Device Destination
 * Specifies the bus the CDC should use for data output
 * Set on the CDC Mode GA register
 */
cdc_dev_dest: .byte 0
.align 2

dir_entry_count:
  .word 0

// in sectors
record_size:
  .word 0

/**
 * Buffer for a single sector of data during disc read work
 */
sector_buffer: .space 0x800

/**
 * Cache for the file info entries
 * 22 bytes per entry * 128 files = 2.75k buffer
 * This can be adjusted as necessary
 */
dir_cache: .space 2816

#endif

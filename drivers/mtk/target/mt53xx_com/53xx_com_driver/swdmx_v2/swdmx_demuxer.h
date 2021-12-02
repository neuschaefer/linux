/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright(c) 2010, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: swdmx_cmpb.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file swdmx_cmpb.h
 *  This header file declares exported APIs of SWDMX_CMPB.
 */

#ifndef SWDMX_DEMUXER_H
#define SWDMX_DEMUXER_H

#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "x_assert.h"


#include "swdmx_stream.h"
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SD_DMX_ST_FULL    0x1
#define SD_DMX_ST_MID_2   0x3
#define SD_DMX_ST_MID_1   0x7
#define SD_DMX_ST_EMPTY   0xF
#define SD_DMX_ST_IDLE    0x1f


#define SD_DMX_PIC_NS     16

// SEND CODE PICTURE MODE 
#define SD_DMX_SM_VDECES 0  // BY VDECES 

// SEND CODE PICTURE MODE 
#define SD_DMX_SRCH_SC_NONE   0  // BY VDECES 
#define SD_DMX_SRCH_SC_BY_DMX 1  // dmx search start code 


#define SD_MAX_V_STREAMS   2
#define SD_MAX_A_STREAMS   8
#define SD_MAX_S_STREAMS   8
#define SD_MAX_KEYTABLES   10
#if (SD_MAX_KEYTABLES != SD_MAX_V_STREAMS + SD_MAX_A_STREAMS)
    #error "The number of keytable must be euqal to the sum of audio and video streams" 
#endif


#define SD_CTRL_RET_OK         0
#define SD_CTRL_RET_INVAL      1
#define SD_CTRL_RET_NONE       2


#define SD_CTRL_CMD_SWITCH_AUDIO    1
#define SD_CTRL_CMD_SWITCH_VIDEO    2
#define SD_CTRL_CMD_SWITCH_SUBTITLE 3
#define SD_CTRL_CMD_RESYNC_VIDEO    4
#define SD_CTRL_CMD_RESYNC_AUDIO    5
#define SD_CTRL_CMD_RESYNC_SUBTITLE 6
#define SD_CTRL_CMD_IDXTBLPTS_RATIO 7


#define SD_SYNC_FLAG_NONE       0 
#define SD_SYNC_FLAG_POSITION   (1L<<0)
#define SD_SYNC_FLAG_PTS        (1L<<1)


#define SD_TBL_NORAML           0
#define SD_TBL_VIRTUAL          1


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct 
{
    UINT8  packet_num;
    UINT8  num_packets;
    UINT8  broken_up_by_us;
    UINT8  ecc_mask;
    UINT8  tx_packet_nums;
    UINT32 frame_size;
    UINT32 buff_sa;
    UINT32 slice_info;
    UINT32 fref_timestamp;
    UINT32 bref_timestamp;
    UINT32 fref_tr;
    UINT32 bref_tr;
}dmx_rm_video_t;


typedef struct 
{
    UINT32 buff_sa;                      
    UINT16 buff_size;
    UINT16 frame_size;
    UINT8  frames_per_block;
    UINT8  blocks_per_sb;    
    UINT8  block_tx_ns;
    UINT8  block_tx_status;
    UINT8  channel_num;
    UINT16 cp1q_bits;
}dmx_rm_audio_t;

typedef struct 
{
    UINT16 stream_number;          //stream id
    UINT32 timestamp;          //stream id
    union
    {
      dmx_rm_video_t v;
      dmx_rm_audio_t a;
    } info;
}dmx_rm_t;

typedef struct 
{
    UINT16 stream_number;
    UINT32 timestamp;
}dmx_cmpb_t;

typedef struct 
{
    UINT16 stream_number;
    UINT32 timestamp;
}dmx_wmv_t;


typedef struct 
{
    UINT16 stream_number;
    UINT32 timestamp;
    BOOL fgFrameHead;
}dmx_mkv_t;

typedef struct 
{
    UINT32 stream_number;  /*stream id*/
    UINT32 timestamp;
    BOOL   b_is_vbr;
    BOOL   b_not_cal_pts;
    UINT64 remainder;
    UINT64 scale;
    UINT32 rate;
    UINT8  total_a_track;
    UINT8  cur_a_trackidx;
    UINT64 cur_pos;
}dmx_avi_t;

typedef struct
{
    UINT8 stream_number;
    UINT8 sub_stream_id;
    UINT32 extra_info;
} dmx_ps_t;

typedef struct _dmx_stream_t 
{
    UINT64 curr_pos;     
    UINT64 start_pos;     
    UINT64 end_pos;     
    UINT32 last_pts;    
    UINT32 start_pts;    
    UINT32 pack_no;
    UINT32 fifo_size;
    UINT32 fifo_empty_size;
    UINT32 fifo_expect_size;   
    UINT8  prs_on;
    UINT8  dmx_mul_pid;
    UINT8  dec_type;
    UINT8  idx_tbl_type;           //virtual or real index table 
    void * idx_tbl;                //index table 
    UINT32 sync_flag;     
    UINT64 sync_pos;     
    UINT32 sync_pts;     
    union
    {
      dmx_rm_t   rm;
      dmx_cmpb_t cmpb;
      dmx_wmv_t  wmv;
      dmx_avi_t avi;
      dmx_mkv_t mkv;
      dmx_ps_t ps;
    } priv;                //private data for special demxuer
}dmx_stream_t;


#define DMX_ES_COMMON  \
    UINT8  type;       \
    UINT32 addr;       \
    UINT32 size;       \
    UINT32 pts;        \


typedef struct 
{
    DMX_ES_COMMON     
}dmx_es_comm;

typedef struct 
{
    DMX_ES_COMMON     
    UINT32 slice_buf_wp;
    UINT8  slice_cnt;
}dmx_es_rv;

typedef struct 
{
    DMX_ES_COMMON     
    BOOL no_vdec_cal_pts;
}dmx_es_mpeg4;

typedef struct 
{
    DMX_ES_COMMON     
    UINT32 time_offset;
    UINT16 decoder_order;
    UINT16 es_id;
    BOOL fg_seq_header;
    BOOL fg_group;
    BOOL fg_eos;
    BOOL fg_pts;
}dmx_es_ps;

typedef struct 
{
    UINT32  fifo_start;     
    UINT32  fifo_end;     
}dmx_vdec_info;

typedef struct 
{
    dmx_stream_t *tx_ds;   
    UINT32       tx_pts;   
}dmx_switch_ctrl;


struct _sd_demuxer_t;

typedef struct 
{
    UINT32 type;
    UINT32 (*init)  (struct _sd_demuxer_t *);
    UINT32 (*ctrl)  (struct _sd_demuxer_t *, UINT32 cmd, void *arg);
    UINT32 (*demux) (struct _sd_demuxer_t *, dmx_stream_t *);
    UINT32 (*seek)  (struct _sd_demuxer_t *, UINT32 ms);
    UINT32 (*deinit)(struct _sd_demuxer_t *);
}sd_sub_demuxer;


typedef struct _sd_demuxer_t{
    sd_stream_t *stream;

    //dmx stream begin
    dmx_stream_t *audio; 
    dmx_stream_t *video; 
    dmx_stream_t *video2; 
    dmx_stream_t *sub;   
    //dmx stream end


    void *audio_streams[SD_MAX_A_STREAMS]; 
    void *video_streams[SD_MAX_V_STREAMS]; 
    void *sub_streams[SD_MAX_S_STREAMS];   
    void *key_tables[SD_MAX_S_STREAMS];   

    UINT32          tx_ctrl_on;
    dmx_switch_ctrl tx_ctrl;

    dmx_vdec_info vdec_info;
    UINT8         srch_sc_mode;  // search start code by dmx or  not : 1 by dmx, 

    //virtual index table
    UINT8 need_virtual_idx;  // build virtual index table on/off
    UINT8 build_virtual_idx_tbl_on;  // build virtual index table on/off
    
    void* priv;            //  data for file-formate ex. avi, rm
    sd_sub_demuxer  *sub_demuxer;

} sd_demuxer_t;


typedef UINT32 (*PFN_SWDMX_DEMUXER_XX)(sd_demuxer_t *, dmx_stream_t *);

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro functions
//-----------------------------------------------------------------------------
#define swdmx_check_stream_size(_s,_n)  \
    if(sd_stream_capacity(_s) < (_n)) \
    { \
        if (sd_stream_eof(_s)) \
        { \
            sd_stream_skip(_s, sd_stream_capacity(_s));\
        } \
        break; \
    }

#define swdmx_has_idx_tbl(_d) (_d && (_d->idx_tbl_type == SD_TBL_NORAML) && _d->idx_tbl)
#define swdmx_has_virturl_idx_tbl(_d) (_d && (_d->idx_tbl_type == SD_TBL_VIRTUAL) && _d->idx_tbl)
#define swdmx_build_virtual_idx_tbl(_d) (_d->build_virtual_idx_tbl_on)


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void swdmx_demuxer(sd_demuxer_t *pt_demuxer,PFN_SWDMX_DEMUXER_XX fp_demxuer_xx);

dmx_stream_t * new_dmx_stream(void);
UINT32 new_dmx_frame(dmx_stream_t *ds, UINT32 size, void *pv_data);
void decoder_add_vframe(sd_demuxer_t *demuxer, void *pv_data);
sd_sub_demuxer *swdmx_get_sub_demuxer(UINT32 type);

UINT32 swdmx_switch_audio(sd_demuxer_t *pt_demuxer, UINT32 strm_id);
UINT32 swdmx_switch_video(sd_demuxer_t *pt_demuxer, UINT32 strm_id);
UINT32 swdmx_switch_sub(sd_demuxer_t *pt_demuxer, UINT32 strm_id);
UINT32 swdmx_resync_video(sd_demuxer_t *pt_demuxer, UINT32 pts);
UINT32 swdmx_resync_audio(sd_demuxer_t *pt_demuxer, UINT32 pts);
UINT32 swdmx_resync_subtitle(sd_demuxer_t *pt_demuxer, UINT32 pts);
UINT32 swdmx_seek(sd_demuxer_t *pt_demuxer, UINT32 ms);
UINT32 swdmx_get_idx_entry_by_pts(sd_demuxer_t *pt_demuxer, void *pv_data);
//UINT32 swdmx_trans_pts_delta(sd_demuxer_t *pt_demuxer, void *pv_data);
UINT32 swdmx_idxpts_to_pts_ratio(sd_demuxer_t *pt_demuxer, void *pv_data);

#define swdmx_member_offset(type,member) ((UINT32)(&(((type*)0)->member)))

#endif  // SWDMX_DEMUXER_H



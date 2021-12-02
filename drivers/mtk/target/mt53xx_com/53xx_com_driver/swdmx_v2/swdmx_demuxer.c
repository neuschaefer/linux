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
 * Copyright (c) 2010, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: swdmx_demuxer.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_demuxer.c
 *   
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "swdmx_demuxer.h"
#include "vdec_drvif.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"
#include "x_util.h"
#include "x_timer.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
extern sd_sub_demuxer sub_demuxer_rm;
extern sd_sub_demuxer sub_demuxer_avi; 

static sd_sub_demuxer * demuxer_list [] =
{
#if 0	
#ifndef CC_RM_DISABLE
    &sub_demuxer_rm,
#endif
#ifndef CC_AVI_DISABLE
    &sub_demuxer_avi,
#endif
#endif
    NULL    
};


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define ring_addr_add_x(_new, _addr, _s, _e, _adv)  \
    do{ \
        _new = _addr + _adv; \
        if(_new >= _e)   \
        { \
            _new -= (_e - _s);  \
        } \
    }while(0)\


dmx_stream_t * new_dmx_stream(void)
{
    dmx_stream_t * demuxer;
    demuxer = (dmx_stream_t *)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(sizeof(dmx_stream_t), 16));
    x_memset((void*)demuxer, 0, sizeof(dmx_stream_t ));
    return demuxer;
}

UINT32 new_dmx_frame(dmx_stream_t *ds, UINT32 size, void *pv_data)
{
    DMX_PID_T  rPid;
    DMX_GetPid(ds->dmx_mul_pid, DMX_PID_FLAG_DATA_POINTERS, &rPid);
    UNUSED(size);
    UNUSED(pv_data);
    return rPid.u4Wp;
}

void decoder_add_vframe(sd_demuxer_t *demuxer, void *pv_data)
{
    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)(demuxer->stream)->pv_host;
    VDEC_PES_INFO_T rPesInfo;
    UINT32 cur_pts = 0;
    UINT8 type = SD_DMX_SM_VDECES;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    switch(type)
    {
    case SD_DMX_SM_VDECES:
    default:
    {
        dmx_es_comm * es_comm = (dmx_es_comm * )pv_data;
        
        rPesInfo.ucMpvId   = (demuxer->video->dec_type == ENUM_SWDMX_VDEC_MJPEG) ? VLD1:VLD0;
        rPesInfo.ucEsId    = swdmx_inst->u1VdecId;
        rPesInfo.u4PTS     = es_comm->pts; 
        rPesInfo.ucPicType = es_comm->type;

        rPesInfo.u4VldReadPtr = PHYSICAL(es_comm->addr);
        rPesInfo.u4FifoStart  = PHYSICAL(demuxer->vdec_info.fifo_start);
        rPesInfo.u4FifoEnd    = PHYSICAL(demuxer->vdec_info.fifo_end);
        
        switch(demuxer->video->dec_type)
        {
        case ENUM_SWDMX_VDEC_DX311:
        {
            UINT8 b;    
            b = *(UINT8*)VIRTUAL(es_comm->addr);
            rPesInfo.fgDtsValid = TRUE;
            if ((b >> 6) & 0x1)
            {
                rPesInfo.ucPicType  = P_TYPE;
            }
            else
            {
                rPesInfo.ucPicType = I_TYPE;
            }
        }   
            break;
        case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
        {
            dmx_es_ps * ps_es = (dmx_es_ps * )pv_data;
            rPesInfo.fgDtsValid = ps_es->fg_pts;
            rPesInfo.fgSeqHdr = ps_es->fg_seq_header;
            rPesInfo.fgGop = ps_es->fg_group;
            rPesInfo.fgEos = ps_es->fg_eos;
            rPesInfo.u2DecodingOrder = ps_es->decoder_order;
            rPesInfo.u4TotalTimeOffset = ps_es->time_offset;
            if(ps_es->es_id != 0xffff)
            {
                rPesInfo.ucEsId = (UCHAR)(ps_es->es_id);
            }
            switch (es_comm->type)
            {
                case PIC_TYPE_I:
                     rPesInfo.ucPicType = MPV_I_TYPE;
                     rPesInfo.rExtra.u8PTSI = rPesInfo.u4PTS;
                    break;
                
                case PIC_TYPE_P:
                    rPesInfo.ucPicType = MPV_P_TYPE;
                    break;
                
                case PIC_TYPE_B:
                    rPesInfo.ucPicType = MPV_B_TYPE;
                    break;
                
                default:
#ifndef CC_VDEC_FMT_DETECT
                    ASSERT(0);
#endif
                    break;

            }
            break;
        }
        case ENUM_SWDMX_VDEC_H264:
        {
            UINT32 addr = 0;           
            ring_addr_add_x(addr, es_comm->addr, demuxer->vdec_info.fifo_start, demuxer->vdec_info.fifo_start, 3);
            rPesInfo.ucPicType = *(UINT8*)VIRTUAL(addr);
            ring_addr_add_x(addr, es_comm->addr, demuxer->vdec_info.fifo_start, demuxer->vdec_info.fifo_end, 4);   
            rPesInfo.u4VldReadPtr = addr;
            if((rPesInfo.ucPicType & 0x1f) == 5)
            {
                rPesInfo.fgDtsValid = TRUE;
            }
            else
            {
                rPesInfo.fgDtsValid = FALSE;
            }    
            break;
        }
        case ENUM_SWDMX_VDEC_MPEG4:
        {
            UINT32 addr = 0;
            UINT8 b;
            dmx_es_mpeg4 * mpeg4_es = (dmx_es_mpeg4 * )pv_data;
            ring_addr_add_x(addr, es_comm->addr, demuxer->vdec_info.fifo_start, demuxer->vdec_info.fifo_start, 3);
            rPesInfo.u4PTS     = mpeg4_es->pts; 
            rPesInfo.ucPicType = mpeg4_es->type;
            rPesInfo.u4VldReadPtr = PHYSICAL(mpeg4_es->addr);
            b = *(UINT8*)VIRTUAL(addr);
            rPesInfo.fgNoVdecCalPts = mpeg4_es->no_vdec_cal_pts;  
			rPesInfo.fgDtsValid = TRUE;
            if (b == 0xB6)
            {
                ring_addr_add_x(addr, es_comm->addr, demuxer->vdec_info.fifo_start, demuxer->vdec_info.fifo_end, 4);   
                b = *(UINT8*)VIRTUAL(addr);
                switch (b >> 6)
                {
                    case 0:
                        rPesInfo.ucPicType  = I_TYPE;
                        rPesInfo.rExtra.u8PTSI = rPesInfo.u4PTS;
                        rPesInfo.fgDtsValid = TRUE;
                        break;
                    case 1: case 3:
                        rPesInfo.ucPicType  = P_TYPE;
                        rPesInfo.fgDtsValid = FALSE;
                        break;
                    case 2:
                        rPesInfo.ucPicType  = B_TYPE;
                        rPesInfo.fgDtsValid = FALSE;
                        break;
                    default:
                        rPesInfo.ucPicType  = UNKNOWN_TYPE;
                        rPesInfo.fgDtsValid = FALSE;
                }     
            }
            else if ((b & 0x20) == 0x20)            
            {                
                rPesInfo.fgSeqHdr = TRUE;
            }
            break;
        }
        case ENUM_SWDMX_VDEC_MJPEG:
            rPesInfo.ucPicType = I_TYPE;
            break;
        case ENUM_SWDMX_VDEC_RV:
        {
            dmx_es_rv * es_rv = (dmx_es_rv * )pv_data;
            rPesInfo.u4RenderPts  = es_rv->slice_buf_wp;
            rPesInfo.u1SliceCount = es_rv->slice_cnt;
            rPesInfo.fgDtsValid = TRUE;
        }
            break;
        case ENUM_SWDMX_VDEC_VC1:
            rPesInfo.fgDtsValid = TRUE;
            rPesInfo.u4VideoType = DMX_VIDEO_VC1;
            break;
        case ENUM_SWDMX_VDEC_WMV7:
            rPesInfo.fgDtsValid = TRUE;
            rPesInfo.u4VideoType = DMX_VIDEO_WMV7;
            break;
        case ENUM_SWDMX_VDEC_WMV8:
            rPesInfo.fgDtsValid = TRUE;
            rPesInfo.u4VideoType = DMX_VIDEO_WMV8;
            break;
        case ENUM_SWDMX_VDEC_WMV9:
            rPesInfo.fgDtsValid = TRUE;
            rPesInfo.u4VideoType = DMX_VIDEO_WMV9;
            break;
        }
        VDEC_SendEs((void *)&rPesInfo);
        cur_pts = rPesInfo.u4PTS;
    }
        break;
    }

    LOG(6, "decoder_add_vframe:(ns, pts, addr, Fstart, Fend) => (%d, %d, %d, %d, %d)\n", 
        demuxer->video->pack_no, cur_pts, rPesInfo.u4VldReadPtr, rPesInfo.u4FifoStart, rPesInfo.u4FifoEnd);

    //Printf("decoder_add_vframe:(ns, pts, addr, Fstart, Fend) => (%d, %d, %d, %d, %d)\n", 
    //    demuxer->video->pack_no, cur_pts, rPesInfo.u4VldReadPtr, rPesInfo.u4FifoStart, rPesInfo.u4FifoEnd);

    demuxer->video->pack_no++;
    demuxer->video->last_pts  = MAX(demuxer->video->last_pts, cur_pts);
    demuxer->video->start_pts = MIN(demuxer->video->start_pts, cur_pts);

}

void swdmx_demuxer(sd_demuxer_t *pt_demuxer,PFN_SWDMX_DEMUXER_XX fp_demxuer_xx)
{
    dmx_stream_t *ds = NULL;
    UINT8 a_status, tmp = 0;
    UINT8 v_status;
    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)(pt_demuxer->stream)->pv_host;

    #define swdmx_switch_stream(_s) \
        do { \
            ds =  _s;\
            pt_demuxer->tx_ctrl.tx_ds     = ds; \
            pt_demuxer->tx_ctrl.tx_pts    = ds->last_pts; \
        } while (0)

    // 1. IBC process

    // 2. check stream status and switch stream if nessary
    v_status = SD_DMX_ST_IDLE;
    ds = pt_demuxer->video;
    if (ds && ds->prs_on)
    {
        // hard code for test
        UINT16 u2CurQNum, u2MaxQNum;
        UINT32 u4DmxAvailSize;
        //check vfifo first
        // check if the parsed frame size is correct
        u4DmxAvailSize = DMX_MUL_GetEmptySize(swdmx_inst->u1SwdmxId, DMX_PID_TYPE_ES_VIDEO, pt_demuxer->video->dmx_mul_pid);
        ds->fifo_empty_size = u4DmxAvailSize;
        if ((ds->fifo_expect_size > u4DmxAvailSize) || (u4DmxAvailSize < 0x5F000))   // 380k
        {
            tmp = SD_DMX_ST_FULL;
        }
        else if (u4DmxAvailSize < ((ds->fifo_size << 1)/3))   // 2/3 fifo size
        {
            tmp = SD_DMX_ST_MID_2;
        }
        else if (u4DmxAvailSize < ds->fifo_size/*max*/)
        {
            tmp = SD_DMX_ST_MID_1;
        }
        else                                // 1/3 380k
        {
            tmp = SD_DMX_ST_EMPTY;
        }
        v_status &= tmp;
        
        //check es ns
        VDEC_GetQueueInfo(swdmx_inst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        u2MaxQNum = MIN(u2MaxQNum, 120);    // 3s 
        if (u2CurQNum >= u2MaxQNum)  //test
        {
            tmp = SD_DMX_ST_FULL;
        }
        else if (u2CurQNum > ((u2MaxQNum << 1)/3) )
        {
            tmp = SD_DMX_ST_MID_2;
        }
        else if (u2CurQNum > 0)
        {
            tmp = SD_DMX_ST_MID_1;
        }
        else
        {
            tmp = SD_DMX_ST_EMPTY;
        }
        v_status &= tmp;
        
        //check last pts
        
    }
    
    a_status = SD_DMX_ST_IDLE;
    ds = pt_demuxer->audio;
    if (ds && ds->prs_on)
    {
        //check afifo first
        UINT32 u4DmxAvailSize;
        // check if the parsed frame size is correct
        u4DmxAvailSize = DMX_MUL_GetEmptySize(swdmx_inst->u1SwdmxId, DMX_PID_TYPE_ES_AUDIO, pt_demuxer->audio->dmx_mul_pid);
        ds->fifo_empty_size = u4DmxAvailSize;
        if ((ds->fifo_expect_size > u4DmxAvailSize) || (u4DmxAvailSize < 0x8000))   // 32K
        {
            tmp = SD_DMX_ST_FULL;
        }
        else if (u4DmxAvailSize < ((ds->fifo_size << 1)/3))   // 2/3 fifo size
        {
            tmp = SD_DMX_ST_MID_2;
        }
        else if (u4DmxAvailSize < ds->fifo_size/*max*/)
        {
            tmp = SD_DMX_ST_MID_1;
        }
        else                                // 1/3 380k
        {
            tmp = SD_DMX_ST_EMPTY;
        }
        a_status &= tmp;

        //check last pts
        
    }

    if (pt_demuxer->tx_ctrl_on) //SD_FAST_SEEK)
    {
        //swtich stream 
        // for test only
        if (pt_demuxer->tx_ctrl.tx_ds)
        {
            if (pt_demuxer->tx_ctrl.tx_pts + 90000 < pt_demuxer->tx_ctrl.tx_ds->last_pts)
            {
                if (pt_demuxer->tx_ctrl.tx_ds == pt_demuxer->audio)
                {
                    if (v_status != SD_DMX_ST_FULL) swdmx_switch_stream(pt_demuxer->video);
                }
                else
                {
                    if (a_status != SD_DMX_ST_FULL) swdmx_switch_stream(pt_demuxer->audio);
                }
            }
            else if (a_status == SD_DMX_ST_FULL && (pt_demuxer->tx_ctrl.tx_ds == pt_demuxer->audio))
            {
                swdmx_switch_stream(pt_demuxer->video);
            }
            else if (v_status == SD_DMX_ST_FULL && (pt_demuxer->tx_ctrl.tx_ds == pt_demuxer->video))
            {
                swdmx_switch_stream(pt_demuxer->audio);
            }
            
        }
        else
        {
            swdmx_switch_stream(pt_demuxer->video);
        }

        tmp = (pt_demuxer->tx_ctrl.tx_ds == pt_demuxer->video) ? v_status : a_status;
        ds = pt_demuxer->tx_ctrl.tx_ds;
    }
    else
    {
        tmp = a_status & v_status;
        ds = NULL;
    }

    if (tmp == SD_DMX_ST_FULL || tmp == SD_DMX_ST_IDLE)
    {
        x_thread_delay(2);
        return;  
    }
    
    // 3. demux stream 
    if (fp_demxuer_xx)
    {
        fp_demxuer_xx(pt_demuxer, ds);
    }

}


UINT32 swdmx_switch_audio(sd_demuxer_t *pt_demuxer, UINT32 strm_id)
{
    ASSERT(pt_demuxer);
    if (pt_demuxer->sub_demuxer->ctrl)
    {
        pt_demuxer->sub_demuxer->ctrl(pt_demuxer, SD_CTRL_CMD_SWITCH_AUDIO, &strm_id);
    }
    return 0;
}

UINT32 swdmx_switch_video(sd_demuxer_t *pt_demuxer, UINT32 strm_id)
{
    return 0;
}

UINT32 swdmx_switch_sub(sd_demuxer_t *pt_demuxer, UINT32 strm_id)
{
    return 0;
}

UINT32 swdmx_resync_video(sd_demuxer_t *pt_demuxer, UINT32 pts)
{
    ASSERT(pt_demuxer);
    if (pt_demuxer->sub_demuxer->ctrl && 
        (pt_demuxer->sub_demuxer->ctrl(pt_demuxer, SD_CTRL_CMD_RESYNC_AUDIO, &pts) == SD_CTRL_RET_OK))
    {
        pt_demuxer->video->start_pts = 0xFFFFFFFF;
        pt_demuxer->video->last_pts  = 0;
    }
    return 0;
}


UINT32 swdmx_resync_audio(sd_demuxer_t *pt_demuxer, UINT32 pts)
{
    ASSERT(pt_demuxer);
    if (pt_demuxer->sub_demuxer->ctrl && 
        (pt_demuxer->sub_demuxer->ctrl(pt_demuxer, SD_CTRL_CMD_RESYNC_AUDIO, &pts) == SD_CTRL_RET_OK))
    {
        pt_demuxer->audio->start_pts = 0xFFFFFFFF;
        pt_demuxer->audio->last_pts  = 0;
    }
    return 0;
}

UINT32 swdmx_resync_subtitle(sd_demuxer_t *pt_demuxer, UINT32 pts)
{
    ASSERT(pt_demuxer);
    if (pt_demuxer->sub_demuxer->ctrl && 
        (pt_demuxer->sub_demuxer->ctrl(pt_demuxer, SD_CTRL_CMD_RESYNC_SUBTITLE, &pts) == SD_CTRL_RET_OK))
    {
        pt_demuxer->sub->start_pts = 0xFFFFFFFF;
        pt_demuxer->sub->last_pts  = 0;
    }
    return 0;
}


UINT32 swdmx_seek(sd_demuxer_t *pt_demuxer, UINT32 ms)
{
    ASSERT(pt_demuxer);
    if (pt_demuxer->sub_demuxer->seek)
    {
        pt_demuxer->sub_demuxer->seek(pt_demuxer, ms);
    }
    return 0;

}

UINT32 swdmx_get_idx_entry_by_pts(sd_demuxer_t *pt_demuxer, void *pv_data)
{
    #if 0
    ASSERT(pt_demuxer);
    if (pt_demuxer->sub_demuxer->ctrl)
    {
        pt_demuxer->sub_demuxer->ctrl(pt_demuxer, SD_CTRL_CMD_GET_IDX_ENTRY, pv_data);
    }

    #else

    sd_pb_idx_entry_t *pt_pb_idx = (sd_pb_idx_entry_t *)pv_data;
    MM_RANGE_ELMT_IDXTBL_T * pCurBuldIdxTbl = NULL;
    UINT32 pts;
    UINT32 ratio;

    UINT32 idx;
    sd_entry_pos entry_pos;

    if (!pt_demuxer->video)
    {
        return SD_CTRL_RET_NONE;
    }

    pCurBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T * )pt_demuxer->video->idx_tbl;
    ASSERT(pCurBuldIdxTbl);

    swdmx_idxpts_to_pts_ratio(pt_demuxer, (void *)&ratio);

    pts = pt_pb_idx->pts/ratio;
 
    if (!swdmx_find_tbl_entry_by_pts(pCurBuldIdxTbl, pts, &entry_pos))
    {
        return SD_CTRL_RET_NONE;
    }

    pCurBuldIdxTbl = entry_pos.idx_tbl;
    idx            = entry_pos.pos;

    if (pt_pb_idx->sup_flag && (pCurBuldIdxTbl->u.pt_pb_idx_tbl_entry[idx].ui4_pts < pts))
    {
        idx = MIN((idx+1), (pCurBuldIdxTbl->ui4_number_of_entry - 1));
    }
    
    pt_pb_idx->idx_tbl = pCurBuldIdxTbl;
    pt_pb_idx->pos     = idx;
    pt_pb_idx->pts     = (pCurBuldIdxTbl->u.pt_pb_idx_tbl_entry[idx].ui4_pts)*ratio;

    return SD_CTRL_RET_OK;

    #endif
}

inline UINT32 swdmx_idxpts_to_pts_ratio(sd_demuxer_t *pt_demuxer, void *pv_data)
{
    ASSERT(pt_demuxer);
    if (pt_demuxer->sub_demuxer->ctrl)
    {
        if (pt_demuxer->sub_demuxer->ctrl(pt_demuxer, SD_CTRL_CMD_IDXTBLPTS_RATIO, pv_data) != SD_CTRL_RET_OK)
        {
            *((UINT32 *)pv_data) = 1;  //default
        }
    }
    return 0;
}

sd_sub_demuxer *swdmx_get_sub_demuxer(UINT32 type)
{
    UINT32 i;
    for (i = 0; demuxer_list[i]; i++)
    {
        if (demuxer_list[i]->type == type)
        {
            return demuxer_list[i];
        }
    }
    return NULL;
}


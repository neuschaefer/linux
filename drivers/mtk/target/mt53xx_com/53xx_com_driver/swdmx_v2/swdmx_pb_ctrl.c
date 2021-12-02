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
 * $RCSfile: swdmx_pb_ctrl.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_pb_ctrl.c
 *   
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_util.h"
#include "x_timer.h"
#include "swdmx_drvif.h"
#include "vdec_drvif.h"
#include "aud_drvif.h"
#include "swdmx_debug.h"
#include "swdmx_pb_ctrl.h"


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
typedef void (*fp_ctrl_s)(sd_pb_ctrl_t *);

#define swdmx_next_idx_entry(_idx_tbl, _idx) \
    do{\
        if ((++(_idx)) >= (_idx_tbl)->ui4_number_of_entry) \
        { \
            (_idx_tbl) = (_idx_tbl)->pt_next_partial; \
            (_idx)     = 0; \
        }\
    }while (0)


#define swdmx_prev_idx_entry(_base_idx_tbl, _idx_tbl, _idx) \
    do{\
        MM_RANGE_ELMT_IDXTBL_T *__idx_tbl = (_base_idx_tbl);\
        if ((_idx)) \
        { \
            (_idx)--; \
        }\
        else \
        {\
            while (__idx_tbl->pt_next_partial && __idx_tbl->pt_next_partial != (_idx_tbl))\
            { \
                __idx_tbl = __idx_tbl->pt_next_partial; \
            } \
            _idx_tbl = __idx_tbl->pt_next_partial ? __idx_tbl : NULL;\
            _idx     = __idx_tbl->ui4_number_of_entry - 1;\
        }\
    }while (0)
    

#define swdmx_prs_off_streams(_pt_demuxer) \
    do{ \
        UINT32 first,last; \
        dmx_stream_t *pt = NULL; \
        first = swdmx_member_offset(sd_demuxer_t,audio); \
        last  = swdmx_member_offset(sd_demuxer_t,sub); \
        for (; first < last;) \
        {\
            pt = (dmx_stream_t *)(*((UINT32 *)(((UINT32)_pt_demuxer) + first)));\
            if (pt) pt->prs_on = FALSE; \
            first += sizeof(dmx_stream_t *);\
        }\
    }while (0)

//-----------------------------------------------------------------------------
// function 
//-----------------------------------------------------------------------------
static void ctrl_init(sd_pb_ctrl_t*pt_this);
static void ctrl_buffering(sd_pb_ctrl_t *pt_this);
static void ctrl_playing(sd_pb_ctrl_t *pt_this);
static void ctrl_playing_idx_based(sd_pb_ctrl_t *pt_this);
static void ctrl_backup_stream_prs_position(sd_demuxer_t *pt_demuxer);

static void _SWDMX_VideoWakeUpX(SWDMX_INFO_T *prSwdmxInst); 


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
// must be the same order as ctrl_state
static fp_ctrl_s _ctrl_state [] =
{
    ctrl_init,
    ctrl_buffering,
    ctrl_playing,
    NULL,
    NULL,
    ctrl_playing_idx_based,
    ctrl_playing_idx_based,
    NULL,
    NULL
};
//-----------------------------------------------------------------------------
// internal  function
//-----------------------------------------------------------------------------

static void ctrl_add_stream(sd_demuxer_t *pt_demuxer, MM_RANGE_ELMT_IDXTBL_T *pr_elmt)
{
    void **pv = NULL;
    UINT32 i, max = 0;

    ASSERT(pr_elmt);
    switch(pr_elmt->e_elem_type)
    {
    case MM_ELEMT_TYPE_VIDEO:
        pv = pt_demuxer->video_streams;
        max = SD_MAX_V_STREAMS;
        break;
    case MM_ELEMT_TYPE_AUDIO:
        pv = pt_demuxer->audio_streams;
        max = SD_MAX_A_STREAMS;
        break;
    case MM_ELEMT_TYPE_SP:
        pv = pt_demuxer->sub_streams;
        max = SD_MAX_S_STREAMS;
        break;
	default:
        pv = pt_demuxer->video_streams;
        max = SD_MAX_V_STREAMS;
        break;
    }

    i = 0;
    while (pv[i] && i < max) i++;

    if (i >= max) return;

    pv[i] = (void *)pr_elmt;
    
}

static void ctrl_add_keytable(sd_demuxer_t *pt_demuxer, MM_RANGE_ELMT_IDXTBL_T *pr_elmt)
{
    void **pv = NULL;
    UINT32 i, max = 0;

    ASSERT(pr_elmt);
    switch(pr_elmt->e_elem_type)
    {
    case MM_ELEMT_TYPE_KEY:
        pv = pt_demuxer->key_tables;
        max = SD_MAX_KEYTABLES;
        break;
    default:
        return;
    }

    i = 0;
    while (pv[i] && i < max) i++;

    if (i >= max) return;

    pv[i] = (void *)pr_elmt;
    
}



static UINT8 ctrl_get_dmx_resource(SWDMX_INFO_T *swdmx_inst, UINT8 type)
{
    DMX_MM_T rDmxMMInfo;
    UINT8    dmx_pid = 0xFF;
    UINT32   u4Flag  = DMX_MM_FLAG_NONE;

    if (type == eSWDMX_MEDIA_VIDEO)
    {
        UINT32 u4VidFifoAddr, u4VidFifoSize;
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
    
        DMX_SetToDecoder(TRUE);
    
        dmx_pid = DMX_MUL_GetAvailablePidx(swdmx_inst->u1SwdmxId);
    
        _SWDMX_GetVideoFifo(swdmx_inst->u1SwdmxId, &u4VidFifoAddr, &u4VidFifoSize);

        u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_VALID |
                DMX_MM_FLAG_BUF | DMX_MM_FLAG_INSTANCE_TAG);
    
        rDmxMMInfo.fgAllocBuf        = FALSE;
        rDmxMMInfo.u4BufAddr         = u4VidFifoAddr ;
        rDmxMMInfo.u4BufSize         = u4VidFifoSize;
        rDmxMMInfo.u1ChannelId       = swdmx_inst->u1VdecId;
        rDmxMMInfo.pvInstanceTag     = (void*)swdmx_inst;
        
    }
    else if (type == eSWDMX_MEDIA_AUDIO)
    {
        UINT32 u4AudFifoAddr, u4AudFifoSize;
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;

        DMX_SetToDecoder(TRUE);

        dmx_pid = DMX_MUL_GetAvailablePidx(swdmx_inst->u1SwdmxId);
        _SWDMX_GetAudioFifo(swdmx_inst->u1SwdmxId, &u4AudFifoAddr, &u4AudFifoSize);

        u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_VALID |
            DMX_MM_FLAG_BUF | DMX_MM_FLAG_INSTANCE_TAG |
            DMX_MM_FLAG_DEVICE_ID);
    
        rDmxMMInfo.fgAllocBuf = FALSE;
        
        rDmxMMInfo.u4BufAddr     = u4AudFifoAddr;
        rDmxMMInfo.u4BufSize     = u4AudFifoSize;
        rDmxMMInfo.pvInstanceTag = (void*)swdmx_inst;
        rDmxMMInfo.u1ChannelId   = swdmx_inst->u1AdecId;
        rDmxMMInfo.u1DeviceId    = swdmx_inst->u1AdecId;

    }
    else if (type == eSWDMX_MEDIA_SUBTITLE)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_PES;
        
        dmx_pid = DMX_MUL_GetSubtitlePidx(swdmx_inst->u1SwdmxId);
        /*
            if (pfnCbFunc)
            {
                u4Flag = (DMX_MM_FLAG_TYPE | 
                    DMX_MM_FLAG_SEARCH_START_CODE | 
                    DMX_MM_FLAG_BUF | 
                  DMX_MM_FLAG_VALID);
            
                u4Flag |= DMX_MM_FLAG_NOTIFY;
                rDmxMMInfo.pfnCallback = (PFN_DMX_NOTIFY)pfnCbFunc;
                
                rDmxMMInfo.fgSearchStartCode = FALSE;
                rDmxMMInfo.fgAllocBuf = TRUE;
                rDmxMMInfo.u4BufSize = AVI_SUBTITLE_BUFSIZE;
            }
            */
    }

    if (!DMX_MM_Set(dmx_pid, u4Flag, &rDmxMMInfo))
    {
        dmx_pid = 0xFF;
    }

    return dmx_pid;

}

void ctrl_set_dmx_vparam(SWDMX_INFO_T *swdmx_inst, dmx_stream_t *pt_ds)
{
    UINT32 u4Flag = (DMX_MM_FLAG_SEARCH_START_CODE | DMX_MM_FLAG_DEVICE_ID);
    DMX_MM_T rDmxMMInfo;

    switch(pt_ds->dec_type)
    {
    case ENUM_SWDMX_VDEC_MJPEG:
        rDmxMMInfo.u1DeviceId = VLD1;
        rDmxMMInfo.fgSearchStartCode = FALSE;
        break;
    case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
    case ENUM_SWDMX_VDEC_MPEG4:
    case ENUM_SWDMX_VDEC_H264:
    case ENUM_SWDMX_VDEC_VC1:
        rDmxMMInfo.u1DeviceId = VLD0;
        rDmxMMInfo.fgSearchStartCode = TRUE;
        break;
    default:
        rDmxMMInfo.u1DeviceId = VLD0;
        rDmxMMInfo.fgSearchStartCode = FALSE;
        break;
    }
    DMX_MM_Set(pt_ds->dmx_mul_pid, u4Flag, &rDmxMMInfo);
}

dmx_stream_t * ctrl_new_dmx_stream(sd_pb_ctrl_t *pt_this)
{
    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_this->pv_host;
    dmx_stream_t * pt;    
    pt = swdmx_mem_valloc(sizeof(dmx_stream_t ));
    x_memset((void*)pt, 0, sizeof(dmx_stream_t ));
    return pt;
}


UINT32 ctrl_get_dmx_buffer_size(UINT8 u1Idx)
{
    DMX_MM_T rMMStruct;
    DMX_MM_Get(u1Idx, DMX_MM_FLAG_BUF, &rMMStruct);
    return rMMStruct.u4BufSize;
}

static void ctrl_free_dmx_resource(UINT8 u1Idx)
{
    DMX_MM_T rPid;
    rPid.fgEnable = FALSE;
    VERIFY(DMX_MM_Set(u1Idx, DMX_MM_FLAG_VALID, &rPid));
    DMX_MM_Free(u1Idx);
    DMX_MUL_FreePidx(u1Idx);
}

static void ctrl_proc_cmd(sd_pb_ctrl_t *pt_this)
{
    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_this->pv_host;
    sd_demuxer_t *pt_demuxer = &swdmx_inst->demuxer;

    if (pt_this->curr_s.state == INIT_S)
    {
        return;
    }

    switch(pt_this->cmd.cmd)
    {
    default:
        pt_this->cmd.state = SD_PB_CMD_IDLE;
        pt_this->cmd.cmd   = SD_PB_CMD_NONE;
        break;
    case SD_PB_CMD_CHG_AUDIO:
    {
        UINT32 pts;
        pts = VDP_GetPts(swdmx_inst->u1B2rId);
        LOG(3 ,"SD_PB_CMD_CHG_AUDIO, pts(%d), e(%d,%d,%d)\n", pts, swdmx_inst->fgEnVideoDmx, swdmx_inst->fgEnAudioDmx, swdmx_inst->fgEnSubTitleDmx);
        ctrl_backup_stream_prs_position(pt_demuxer);
        if (swdmx_inst->fgEnAudioDmx)
        {
            DMX_MM_FlushBuffer(pt_demuxer->audio->dmx_mul_pid);
            pt_demuxer->audio->dec_type = swdmx_inst->eAudioType;
            swdmx_switch_audio(pt_demuxer, swdmx_inst->u4AudStrmID);
            pt_demuxer->audio->prs_on = TRUE;
            swdmx_resync_audio(pt_demuxer, pts + 30000); // 1/3s
        }
        pt_this->cmd.state = SD_PB_CMD_IDLE;
        pt_this->cmd.cmd   = SD_PB_CMD_NONE;
    }
        break;
    case SD_PB_CMD_SEEK_TIME:
        MPV_FlushEsmQ(swdmx_inst->u1VdecId, TRUE);
        VDEC_ReleaseDispQ(swdmx_inst->u1VdecId);

        LOG(3 ,"SD_PB_CMD_SEEK_TIME, pts(%d) e(%d,%d,%d)\n", pt_this->cmd.arg1, swdmx_inst->fgEnVideoDmx, swdmx_inst->fgEnAudioDmx, swdmx_inst->fgEnSubTitleDmx);

        //video
        if (swdmx_inst->fgEnVideoDmx)
        {
            VDEC_SetRenderFromPts(swdmx_inst->u1VdecId, pt_this->cmd.arg1);
            VDEC_StartPTS(swdmx_inst->u1VdecId, TRUE, pt_this->cmd.arg1, NULL); 
            DMX_MM_FlushBuffer(pt_demuxer->video->dmx_mul_pid);
            pt_demuxer->video->prs_on    = TRUE;
            pt_demuxer->video->start_pts = 0xFFFFFFFF;
            pt_demuxer->video->last_pts  = 0;
        }

        //audio
        if (swdmx_inst->fgEnAudioDmx)
        {
            DMX_MM_FlushBuffer(pt_demuxer->audio->dmx_mul_pid);
            pt_demuxer->audio->prs_on    = TRUE;
            pt_demuxer->audio->start_pts = 0xFFFFFFFF;
            pt_demuxer->audio->last_pts  = 0;
        }

        //subtitle
        if (swdmx_inst->fgEnSubTitleDmx)
        {
            DMX_MM_FlushBuffer(pt_demuxer->sub->dmx_mul_pid);
            pt_demuxer->sub->prs_on    = TRUE;
            pt_demuxer->sub->start_pts = 0xFFFFFFFF;
            pt_demuxer->sub->last_pts  = 0;
        }

        swdmx_seek(pt_demuxer, pt_this->cmd.arg1);

        // set stc
        STC_StopStc(swdmx_inst->u1StcId);
        STC_SetStcValue(swdmx_inst->u1StcId, pt_this->cmd.arg1);
        
        pt_this->cmd.state = SD_PB_CMD_IDLE;
        pt_this->cmd.cmd   = SD_PB_CMD_NONE;
        break;
    case SD_PB_CMD_CHG_SPEED:
    {
        UINT32 cur_pts;
        INT32  speed;
        sd_pb_idx_entry_t idx_entry;
        cur_pts = VDP_GetPts(swdmx_inst->u1B2rId);
        idx_entry.pts      = cur_pts;
        idx_entry.sup_flag = 1;

        speed              = (INT32)pt_this->cmd.arg1;
        
        //if old speed && new speed
        if (/*IS_IDX_BASED_MODE(0) &&*/ IS_IDX_BASED_MODE(speed))
        {
            LOG(3, "change speed, >= 4x, (old speed, new speed, pts)=>(%d, %d, %d)\n", pt_this->curr_s.speed, speed, cur_pts);

            MPV_FlushEsmQ(swdmx_inst->u1VdecId, TRUE);
            VDEC_ReleaseDispQ(swdmx_inst->u1VdecId);

            swdmx_prs_off_streams(pt_demuxer);

            //video
            if (swdmx_inst->fgEnVideoDmx)
            {
                DMX_MM_FlushBuffer(pt_demuxer->video->dmx_mul_pid);
            }

            //find the nearest keyframe in keytable
            swdmx_get_idx_entry_by_pts(pt_demuxer, (void *)&idx_entry);

            pt_this->curr_idx     = idx_entry.pos;
            pt_this->curr_idx_tbl = idx_entry.idx_tbl;
            pt_this->next_idx     = idx_entry.pos;
            pt_this->next_idx_tbl = idx_entry.idx_tbl;
            
            if (IS_FORWARD(speed))
            {
                swdmx_next_idx_entry(pt_this->next_idx_tbl, pt_this->next_idx);
            }
            else
            {
                swdmx_prev_idx_entry(pt_demuxer->video->idx_tbl, pt_this->next_idx_tbl, pt_this->next_idx);
            }

            pt_this->repeat_cnt = 0xFFFFFFFF;
            
            //update playback status
            pt_this->last_s       = pt_this->curr_s;
            pt_this->curr_s.state = IS_FORWARD(speed) ? FORWARD_S : BACKWARD_S;
        }
        else if (IS_IDX_BASED_MODE(pt_this->curr_s.speed) && !IS_IDX_BASED_MODE(speed))
        {
            LOG(3, "change speed, from idx based to non-idx based, (old speed, new speed, pts)=>(%d, %d, %d)\n", pt_this->curr_s.speed, speed, cur_pts);

            MPV_FlushEsmQ(swdmx_inst->u1VdecId, TRUE);
            VDEC_ReleaseDispQ(swdmx_inst->u1VdecId);

            //video
            if (swdmx_inst->fgEnVideoDmx)
            {
                VDEC_SetRenderFromPts(swdmx_inst->u1VdecId, cur_pts);
                VDEC_StartPTS(swdmx_inst->u1VdecId, TRUE, cur_pts, NULL); 
                DMX_MM_FlushBuffer(pt_demuxer->video->dmx_mul_pid);
                pt_demuxer->video->prs_on = TRUE;
                swdmx_resync_video(pt_demuxer, cur_pts);
            }
        
            //audio
            if (swdmx_inst->fgEnAudioDmx)
            {
                DMX_MM_FlushBuffer(pt_demuxer->audio->dmx_mul_pid);
                pt_demuxer->audio->prs_on = TRUE;
                swdmx_resync_audio(pt_demuxer, cur_pts + 30000);
            }

            //subtitle
            if (swdmx_inst->fgEnSubTitleDmx)
            {
                DMX_MM_FlushBuffer(pt_demuxer->sub->dmx_mul_pid);
                pt_demuxer->sub->prs_on = TRUE;
                swdmx_resync_subtitle(pt_demuxer, cur_pts + 30000);
            }

            // set stc
            STC_StopStc(swdmx_inst->u1StcId);
            STC_SetStcValue(swdmx_inst->u1StcId, cur_pts);

            pt_this->last_s = pt_this->curr_s;
            pt_this->curr_s.state = PLAYING_S;
            
        }
        else //if (!IS_IDX_BASED_MODE(0) && IS_IDX_BASED_MODE(1))
        {
            LOG(3, "change speed, from non-idx based to non-idx based, (old speed, new speed, pts)=>(%d, %d, %d)\n", pt_this->curr_s.speed, speed, cur_pts);

            ctrl_backup_stream_prs_position(pt_demuxer);

            //just resync audio if nessary
            if (swdmx_inst->fgEnAudioDmx)
            {
                DMX_MM_FlushBuffer(pt_demuxer->audio->dmx_mul_pid);
                pt_demuxer->audio->prs_on = TRUE;
                swdmx_resync_audio(pt_demuxer, cur_pts + 30000);
            }

            //subtitle
            if (swdmx_inst->fgEnSubTitleDmx)
            {
                DMX_MM_FlushBuffer(pt_demuxer->sub->dmx_mul_pid);
                pt_demuxer->sub->prs_on = TRUE;
                swdmx_resync_subtitle(pt_demuxer, cur_pts + 30000);
            }

            if (IS_STEP(pt_this->curr_s.speed))
            {
                STC_SetStcValue(swdmx_inst->u1StcId, cur_pts);
                STC_StartStc(swdmx_inst->u1StcId);
            }

            pt_this->last_s = pt_this->curr_s;
        }

        pt_this->curr_s.speed = speed;
        pt_this->cmd.state    = SD_PB_CMD_IDLE;
        pt_this->cmd.cmd      = SD_PB_CMD_NONE;
    }
        break;
    case SD_PB_CMD_NONE:
        break;
    }

    
}

static void ctrl_reg_check(sd_pb_ctrl_t *pt_this)
{
    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_this->pv_host;
    sd_demuxer_t *pt_demuxer = &swdmx_inst->demuxer;

    // if demuxing is done
    if (sd_stream_eof(pt_demuxer->stream) && sd_stream_capacity(pt_demuxer->stream) == 0)
    {
        swdmx_inst->rCurRangeReq.eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        if (pt_demuxer->video->prs_on)
        {
            VDEC_PES_INFO_T rPesInfo;
            rPesInfo.fgEos        = TRUE;
            rPesInfo.ucEsId       = swdmx_inst->u1VdecId;
            rPesInfo.ucMpvId      = (pt_demuxer->video->dec_type == ENUM_SWDMX_VDEC_MJPEG)? VLD1: VLD0;
            rPesInfo.u4FifoStart  = (UINT32)NULL;
            rPesInfo.u4FifoEnd    = (UINT32)NULL;
            rPesInfo.u4VldReadPtr = (UINT32)NULL;

            VDEC_SendEs((void*)&rPesInfo);
            pt_demuxer->video->prs_on = FALSE;
            LOG(3,"video demux finished, non-index based mode\n");
        }
        if (pt_demuxer->audio->prs_on)
        {
		    DMX_MM_DATA_T rDmxMMData;
            FEEDER_BUF_INFO_T *prFeederInfo = &swdmx_inst->rFeederInfo;
            FeederGetBufferInfo(swdmx_inst->eFeederSrc, &swdmx_inst->rFeederInfo);
            
            rDmxMMData.u4BufStart = prFeederInfo->u4StartAddr;
            rDmxMMData.u4BufEnd = prFeederInfo->u4EndAddr;
            rDmxMMData.u4StartAddr = prFeederInfo->u4StartAddr;
            rDmxMMData.u4FrameSize = 4;
            rDmxMMData.fgEOS = TRUE;
            rDmxMMData.u1Idx = pt_demuxer->audio->dmx_mul_pid;
            _SWDMX_MoveData(swdmx_inst->u1SwdmxId, &rDmxMMData);
            AUD_MM_Set_Avsync_event(swdmx_inst->u1AdecId);
            pt_demuxer->audio->prs_on = FALSE;
            LOG(3,"video demux finished, non-index based mode\n");
        }
    }
    
}



static void ctrl_init(sd_pb_ctrl_t *pt_this)
{
    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_this->pv_host;
    SWDMX_RANGE_LIST_T * pr_range_list = &swdmx_inst->rCurRangeReq;
    MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl = NULL;
    sd_demuxer_t *pt_demuxer = &swdmx_inst->demuxer;

    if (!(pr_range_list->prDmxRangeInfo))
    {
        return;
    }
    pr_range_list->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;


    // process stream first
    pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pr_range_list->prDmxRangeInfo->pt_idxtbl_lst;
    while (pBuldIdxTbl)
    {
        switch(pBuldIdxTbl->e_elem_type)
        {
        case MM_ELEMT_TYPE_AUDIO:
        case MM_ELEMT_TYPE_VIDEO:
        case MM_ELEMT_TYPE_SP:
            ctrl_add_stream(pt_demuxer, pBuldIdxTbl);
            break;
        case MM_ELEMT_TYPE_KEY:
            ctrl_add_keytable(pt_demuxer, pBuldIdxTbl);
            break;
		default:
            ctrl_add_stream(pt_demuxer, pBuldIdxTbl);
			break;
        };
        pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
    }

    //if video exist, request resource
    if (pt_demuxer->video_streams[0])
    {
        pt_demuxer->video = ctrl_new_dmx_stream(pt_this);
        pt_demuxer->video->dmx_mul_pid = ctrl_get_dmx_resource(swdmx_inst, eSWDMX_MEDIA_VIDEO);
        pt_demuxer->video->prs_on = FALSE;
        pt_demuxer->video->fifo_size = ctrl_get_dmx_buffer_size(pt_demuxer->video->dmx_mul_pid);//DMX_MM_GetBufEmptySize(pt_demuxer->video->dmx_mul_pid);
    }

    //if audio exist, request resource
    if (pt_demuxer->audio_streams[0])
    {
        pt_demuxer->audio = ctrl_new_dmx_stream(pt_this);
        pt_demuxer->audio->dmx_mul_pid = ctrl_get_dmx_resource(swdmx_inst, eSWDMX_MEDIA_AUDIO);
        pt_demuxer->audio->prs_on = FALSE;
        pt_demuxer->audio->fifo_size = ctrl_get_dmx_buffer_size(pt_demuxer->audio->dmx_mul_pid);
    }

    //if audio exist, request resource
    if (pt_demuxer->sub_streams[0])
    {
        pt_demuxer->sub = ctrl_new_dmx_stream(pt_this);
        pt_demuxer->sub->dmx_mul_pid = ctrl_get_dmx_resource(swdmx_inst, eSWDMX_MEDIA_SUBTITLE);
        pt_demuxer->sub->prs_on = FALSE;
        pt_demuxer->sub->fifo_size = ctrl_get_dmx_buffer_size(pt_demuxer->sub->dmx_mul_pid);
    }

    //  stream module
    switch(pr_range_list->prDmxRangeInfo->e_mm_src_type)
    {
    case MM_SRC_TYPE_PUSH:
        pt_demuxer->stream->seekable = SD_NO_SEEK;
        sd_stream_set_input_mode(pt_demuxer->stream, SD_STREAM_PUSH);
        break;
    case MM_SRC_TYPE_HIGH_SPEED_STORAGE:
        pt_demuxer->stream->seekable = SD_FAST_SEEK;
        sd_stream_set_input_mode(pt_demuxer->stream, SD_STREAM_PULL);
        break;
    default:
        pt_demuxer->stream->seekable = SD_SLOW_SEEK;
        sd_stream_set_input_mode(pt_demuxer->stream, SD_STREAM_PULL);
        break;
    }
    if (pt_demuxer->stream->stream_type == SD_STREAM_TYPE_RING)
    {
        FEEDER_BUF_INFO_T type;
        FeederGetBufferInfo(swdmx_inst->eFeederSrc, &type);
        pt_demuxer->stream->priv.u.ring.start_addr = VIRTUAL(type.u4StartAddr);
        pt_demuxer->stream->priv.u.ring.end_addr   = VIRTUAL(type.u4EndAddr);
    }
    
    pt_demuxer->stream->start_fpos = pr_range_list->prDmxRangeInfo->ui8_fileoffset;
    pt_demuxer->stream->end_fpos   = pt_demuxer->stream->start_fpos + pr_range_list->prDmxRangeInfo->z_range_sz;
    pt_demuxer->stream->fpos       = pt_demuxer->stream->start_fpos;
 
    //enable stream
    if (swdmx_inst->fgEnVideoDmx)
    {
        UINT32 u4Addr, u4Size;
        pt_demuxer->video->dec_type  = swdmx_inst->eVideoType;
        pt_demuxer->video->curr_pos  = pt_demuxer->stream->fpos;
        pt_demuxer->video->start_pos = pt_demuxer->stream->start_fpos;
        pt_demuxer->video->end_pos   = pt_demuxer->stream->end_fpos;
        pt_demuxer->video->prs_on    = TRUE;
        ctrl_set_dmx_vparam(swdmx_inst, pt_demuxer->video);
        DMX_MM_FlushBuffer(pt_demuxer->video->dmx_mul_pid);
        pt_demuxer->video->start_pts = 0xFFFFFFFF;

        _SWDMX_GetVideoFifo(swdmx_inst->u1SwdmxId, &u4Addr, &u4Size);
        swdmx_inst->demuxer.vdec_info.fifo_start = u4Addr;
        swdmx_inst->demuxer.vdec_info.fifo_end   = u4Addr + u4Size;

    }

    if (swdmx_inst->fgEnAudioDmx)
    {
        pt_demuxer->audio->dec_type  = swdmx_inst->eAudioType;
        pt_demuxer->audio->curr_pos  = pt_demuxer->stream->fpos;
        pt_demuxer->audio->start_pos = pt_demuxer->stream->start_fpos;
        pt_demuxer->audio->end_pos   = pt_demuxer->stream->end_fpos;
        pt_demuxer->audio->prs_on    = TRUE;
        DMX_MM_FlushBuffer(pt_demuxer->audio->dmx_mul_pid);
        pt_demuxer->audio->start_pts = 0xFFFFFFFF;

    }

    if (swdmx_inst->fgEnSubTitleDmx)
    {
        pt_demuxer->sub->dec_type  = swdmx_inst->eSubVideoType;
        pt_demuxer->sub->curr_pos  = pt_demuxer->stream->fpos;
        pt_demuxer->sub->start_pos = pt_demuxer->stream->start_fpos;
        pt_demuxer->sub->end_pos   = pt_demuxer->stream->end_fpos;
        pt_demuxer->sub->prs_on    = TRUE;
        DMX_MM_FlushBuffer(pt_demuxer->sub->dmx_mul_pid);
        pt_demuxer->sub->start_pts = 0xFFFFFFFF;

    }

    if (swdmx_inst->fgEnVideo2Dmx)
    {
        //request dmx resource first
        pt_demuxer->video2              = ctrl_new_dmx_stream(pt_this);
        pt_demuxer->video2->dmx_mul_pid = ctrl_get_dmx_resource(swdmx_inst, eSWDMX_MEDIA_VIDEO);
        pt_demuxer->video2->fifo_size   = ctrl_get_dmx_buffer_size(pt_demuxer->video2->dmx_mul_pid);

        pt_demuxer->video2->dec_type    = swdmx_inst->eVideo2Type;
        pt_demuxer->video2->curr_pos    = pt_demuxer->stream->fpos;
        pt_demuxer->video2->start_pos   = pt_demuxer->stream->start_fpos;
        pt_demuxer->video2->end_pos     = pt_demuxer->stream->end_fpos;
        pt_demuxer->video2->prs_on      = TRUE;
        DMX_MM_FlushBuffer(pt_demuxer->video2->dmx_mul_pid);
        pt_demuxer->video2->start_pts   = 0xFFFFFFFF;
    }


    //video callback
    swdmx_inst->pfnSwdmxSendVideoPes = NULL;
    swdmx_inst->demuxer.srch_sc_mode = SD_DMX_SRCH_SC_NONE;

    //bad interlave detection mode 
    pt_demuxer->tx_ctrl_on = FALSE;



    //for special container configuration
    if (pt_demuxer->sub_demuxer->init)
    {
        pt_demuxer->sub_demuxer->init(pt_demuxer);
    }


    //cmd
    pt_this->cmd.state    = SD_PB_CMD_IDLE;
    pt_this->cmd.cmd      = SD_PB_CMD_NONE;
    pt_this->cmd.priority = SD_PB_CMD_LV2;

    // start to play
    sd_stream_reset(pt_demuxer->stream);
    pt_demuxer->stream->state      = SD_STREAM_ST_RUN;
    pt_this->last_s.state = INIT_S;
    pt_this->curr_s.state = PLAYING_S;
    pt_this->curr_s.speed = DEFAULT_SPEED;

    if (swdmx_inst->fgEnVideoDmx)
    {
        VDEC_PTS_INFO_T rPTSInfo = {0};
        MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T *)pt_demuxer->video_streams[0];

        rPTSInfo.u8Scale = (UINT64)pBuldIdxTbl->ui4_scale * 90000;
        rPTSInfo.u4Rate  = pBuldIdxTbl->ui4_rate;

        VDP_SetPauseMm(swdmx_inst->u1B2rId, TRUE);
        VDEC_Pause(swdmx_inst->u1VdecId);
        VDEC_StartPTS(swdmx_inst->u1VdecId, TRUE, 0, &rPTSInfo);
        VDEC_SetRenderFromPts(swdmx_inst->u1VdecId, 0);
    }

    // audio only
    if (swdmx_inst->fgEnAudioDmx)
    {
        AUD_SetStartPts(swdmx_inst->u1AdecId, 0);
    }
    
    // set stc for starting playback
    STC_StopStc(swdmx_inst->u1StcId);
    STC_SetStcValue(swdmx_inst->u1StcId, 0);
  
    _SWDMX_VideoWakeUpX(swdmx_inst);

}

static void ctrl_buffering(sd_pb_ctrl_t *pt_this)
{
    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_this->pv_host;
    sd_demuxer_t * pt_demuxer = &swdmx_inst->demuxer;

    UINT32 size_threshold = 0;

    // step 1 check state todo
    if (sd_stream_capacity(pt_demuxer->stream) >= size_threshold ||
        sd_stream_fullness(pt_demuxer->stream))
    {
        //notify status
        pt_this->last_s       = pt_this->curr_s;
        pt_this->curr_s.state = PAUSE_S;
        return;
    }
    
    // step 2 stream module
    sd_stream_state(pt_demuxer->stream);
}

static void ctrl_playing(sd_pb_ctrl_t *pt_this)
{
    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_this->pv_host;
    sd_demuxer_t * pt_demuxer = &swdmx_inst->demuxer;

    // step 1 check state
    ctrl_reg_check(pt_this);
    
    // step 2 stream module
    sd_stream_state(pt_demuxer->stream);

    // step 3 demxuer
    swdmx_demuxer(pt_demuxer, pt_demuxer->sub_demuxer->demux);

}

inline static UINT32 ctrl_calc_repeat_cnt(sd_pb_ctrl_t *pt_this)
{
    UINT32 delta = 0;
    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_this->pv_host;
    sd_demuxer_t * pt_demuxer = &swdmx_inst->demuxer;
    UINT32 ratio;
    if (!(pt_this->curr_idx_tbl) || !(pt_this->next_idx_tbl))
    {
        return delta;
    }

    if (pt_this->curr_idx_tbl->u.pt_pb_idx_tbl_entry[pt_this->curr_idx].ui4_pts < 
        pt_this->next_idx_tbl->u.pt_pb_idx_tbl_entry[pt_this->next_idx].ui4_pts )
    {
        delta = pt_this->next_idx_tbl->u.pt_pb_idx_tbl_entry[pt_this->next_idx].ui4_pts -
            pt_this->curr_idx_tbl->u.pt_pb_idx_tbl_entry[pt_this->curr_idx].ui4_pts;
    }
    else
    {
        delta = pt_this->curr_idx_tbl->u.pt_pb_idx_tbl_entry[pt_this->curr_idx].ui4_pts -
            pt_this->next_idx_tbl->u.pt_pb_idx_tbl_entry[pt_this->next_idx].ui4_pts;
    }

    //swdmx_trans_pts_delta(pt_demuxer, (void *)&delta);
    swdmx_idxpts_to_pts_ratio(pt_demuxer, (void *)&ratio);
    delta *= ratio;
    
    delta = (delta * pt_this->video_fps )/(90 * ABS(pt_this->curr_s.speed));
    
    LOG(5, "ctrl_calc_repeat_cnt (cnt, idx, next_idx, pts, next_pts)=>(%d,%d,%d,%d,%d)\n", 
    delta, pt_this->curr_idx, pt_this->next_idx,
    pt_this->curr_idx_tbl->u.pt_pb_idx_tbl_entry[pt_this->curr_idx].ui4_pts,
    pt_this->next_idx_tbl->u.pt_pb_idx_tbl_entry[pt_this->next_idx].ui4_pts);
    
    return delta;
    
}

static void ctrl_playing_idx_based(sd_pb_ctrl_t *pt_this)
{
    SWDMX_INFO_T * swdmx_inst = (SWDMX_INFO_T *)pt_this->pv_host;
    sd_demuxer_t * pt_demuxer = &swdmx_inst->demuxer;
    UINT64 offset = 0;

    // step 1 check state
    if (!pt_this->curr_idx_tbl)
    {
        swdmx_inst->rCurRangeReq.eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        if (pt_demuxer->video->prs_on)
        {
            VDEC_PES_INFO_T rPesInfo;
            rPesInfo.fgEos        = TRUE;
            rPesInfo.ucEsId       = swdmx_inst->u1VdecId;
            rPesInfo.ucMpvId      = (pt_demuxer->video->dec_type == ENUM_SWDMX_VDEC_MJPEG)? VLD1: VLD0;
            rPesInfo.u4FifoStart  = (UINT32)NULL;
            rPesInfo.u4FifoEnd    = (UINT32)NULL;
            rPesInfo.u4VldReadPtr = (UINT32)NULL;
            VDEC_SendEs((void*)&rPesInfo);
            pt_demuxer->video->prs_on = FALSE;
            LOG(3,"video demux finished, index based mode\n");
        }
        return;
    }

    // step 2 
    //calc repeat count
    if (pt_this->repeat_cnt == 0xFFFFFFFF) 
    {
        pt_this->repeat_cnt = pt_this->next_idx_tbl ? ctrl_calc_repeat_cnt(pt_this):4;
        pt_this->repeat_st  = SD_PB_IDX_BASE_ST_SEND;
    }

    if (pt_this->repeat_st == SD_PB_IDX_BASE_ST_SENDING && 
        pt_demuxer->video->pack_no == pt_this->repeat_last_pack_no) 
    {
    
    }
    else if (pt_this->repeat_cnt)
    {
        offset = pt_this->curr_idx_tbl->ui8_base_offset + 
            pt_this->curr_idx_tbl->u.pt_pb_idx_tbl_entry[pt_this->curr_idx].ui4_relative_offset;
        sd_stream_seek(pt_demuxer->stream, offset);
        pt_this->repeat_cnt--;
        pt_demuxer->video->prs_on = TRUE;

        if (pt_this->repeat_cnt == 0)
        {
            pt_this->curr_idx_tbl = pt_this->next_idx_tbl;
            pt_this->curr_idx     = pt_this->next_idx;
        }
        pt_this->repeat_st  = SD_PB_IDX_BASE_ST_SENDING;
        pt_this->repeat_last_pack_no = pt_demuxer->video->pack_no;
    }
    else 
    {
        if (IS_FORWARD(pt_this->curr_s.speed))
        {
            swdmx_next_idx_entry(pt_this->next_idx_tbl, pt_this->next_idx);
        }
        else
        {
            swdmx_prev_idx_entry(pt_demuxer->video->idx_tbl, pt_this->next_idx_tbl, pt_this->next_idx);
        }
        
        pt_this->repeat_cnt       = 0xFFFFFFFF;
        pt_demuxer->video->prs_on = FALSE;
        return;
    }

    // step 3 stream module
    sd_stream_state(pt_demuxer->stream);

    // step 4 demxuer
    swdmx_demuxer(pt_demuxer, pt_demuxer->sub_demuxer->demux);
}

static void ctrl_playback_state(sd_pb_ctrl_t *pt_this)
{
    ASSERT(pt_this);
    ASSERT(pt_this->curr_s.state <= ERROR_S);

    // process cmd
    ctrl_proc_cmd(pt_this);
    
    if (_ctrl_state[pt_this->curr_s.state])
    {
        _ctrl_state[pt_this->curr_s.state](pt_this); 
    }
}


static void ctrl_backup_stream_prs_position(sd_demuxer_t *pt_demuxer) 
{
    UINT32 first,last;
    UINT64 fpos;
    dmx_stream_t *pt = NULL;
    
    fpos  = sd_stream_tell(pt_demuxer->stream);
    first = swdmx_member_offset(sd_demuxer_t,audio);
    last  = swdmx_member_offset(sd_demuxer_t,sub);
    for (; first < last;)
    {
        pt = (dmx_stream_t *)(*((UINT32 *)(((UINT32)pt_demuxer) + first)));
        if (pt && pt->prs_on)
        {
            pt->sync_flag |= SD_SYNC_FLAG_POSITION;
            pt->sync_pos   = fpos;
        }
        first += sizeof(dmx_stream_t *);
    }
}

SWDMX_HDR_PRS_STATE_T _SWDMX_HdrPrsX(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T* prCmd)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);


    ctrl_playback_state(&prSwdmxInst->pb_ctrl);

    UNUSED(prRangeList);
    UNUSED(prCmd);

    return eSWDMX_HDR_PRS_SUCCEED;
}

BOOL _SWDMX_EnableStrmX(UINT8 u1SrcId, UINT8 u1StrmType, VOID* pfnCbFunc)
{
    BOOL ret = TRUE;
    SWDMX_INFO_T* swdmx_inst = _SWDMX_GetInst(u1SrcId);

    if (swdmx_inst->pb_ctrl.curr_s.state == INIT_S)
    {
        ;
    }
    else if (swdmx_inst->pb_ctrl.cmd.state != SD_PB_CMD_IDLE)
    {
        switch(swdmx_inst->pb_ctrl.cmd.cmd)
        {
        case SD_PB_CMD_SEEK_TIME:
        case SD_PB_CMD_CHG_SPEED:    
            break;
        default:
            ret = FALSE;
            break;
        }
    }
    else if (IS_STEP(swdmx_inst->pb_ctrl.curr_s.speed))
    {
        ;
    }
    else
    {
        swdmx_inst->pb_ctrl.cmd.state     = SD_PB_CMD_DOING;
        switch(u1StrmType)
        {
        case eSWDMX_STRM_TYPE_AUD:
            swdmx_inst->pb_ctrl.cmd.cmd   = SD_PB_CMD_CHG_AUDIO;
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            swdmx_inst->pb_ctrl.cmd.cmd   = SD_PB_CMD_CHG_SUBTITLE;
            swdmx_inst->pb_ctrl.cmd.arg1  = (UINT32)pfnCbFunc;
            break;
        case eSWDMX_STRM_TYPE_VID:
            swdmx_inst->pb_ctrl.cmd.cmd   = SD_PB_CMD_NONE;
            swdmx_inst->pb_ctrl.cmd.state = SD_PB_CMD_IDLE;
            break;
        default:
            swdmx_inst->pb_ctrl.cmd.state = SD_PB_CMD_IDLE;
            ret = FALSE;
            break;
        }
    }

    LOG(3, "_SWDMX_EnableStrmX, (type,ret) => (%d,%d)\n", u1StrmType, ret);


    return ret;
}

BOOL _SWDMX_DisableStrmX(UINT8 u1SrcId, UINT8 u1StrmType)
{
    SWDMX_INFO_T* swdmx_inst = _SWDMX_GetInst(u1SrcId);
    sd_demuxer_t *demuxer    = &swdmx_inst->demuxer;

    if (swdmx_inst->pb_ctrl.curr_s.state == INIT_S)
    {
        return TRUE;
    }

    //if (swdmx_inst->pb_ctrl.cmd.state != SD_PB_CMD_IDLE)
    //{
    //    return FALSE;
    //}
    LOG(3, "_SWDMX_DisableStrmX, type => (%d)\n", u1StrmType);

    switch(u1StrmType)
    {
    case eSWDMX_STRM_TYPE_VID:
        demuxer->video->prs_on = FALSE;
        break;
    case eSWDMX_STRM_TYPE_AUD:
        demuxer->audio->prs_on = FALSE;
        break;
    case eSWDMX_STRM_TYPE_SUBTITLE:
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

static void _SWDMX_VideoWakeUpX(SWDMX_INFO_T *prSwdmxInst)
{
    ENUM_VDEC_FMT_T eFmt = VDEC_FMT_MAX;
    VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
    switch (prSwdmxInst->eVideoType)
    {
    case ENUM_SWDMX_VDEC_RV:
        eFmt = VDEC_FMT_RV;
        break;
    case ENUM_SWDMX_VDEC_VC1:
    case ENUM_SWDMX_VDEC_WMV7:
    case ENUM_SWDMX_VDEC_WMV8:
    case ENUM_SWDMX_VDEC_WMV9:
        eFmt = VDEC_FMT_WMV;
        break;
    case ENUM_SWDMX_VDEC_DX311:
    case ENUM_SWDMX_VDEC_MPEG4:    
        eFmt = VDEC_FMT_MP4;
        break;
    case ENUM_SWDMX_VDEC_MJPEG:
        eFmt = VDEC_FMT_MJPEG;
        break;
    case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
        eFmt = VDEC_FMT_MPV;
        break;
    case ENUM_SWDMX_VDEC_H264:
        eFmt = VDEC_FMT_H264;
        break;
    default:
        LOG(1, "_SWDMX_VideoWakeUp Unknown Video Type\n");
    }
    VDEC_Play(prSwdmxInst->u1VdecId, eFmt);
}


BOOL _SWDMX_StopX(UINT8 u1SrcId)
{
    SWDMX_INFO_T *swdmx_inst  = _SWDMX_GetInst(u1SrcId);
    sd_demuxer_t *pt_demuxer  = &swdmx_inst->demuxer;

    UINT32 first,last;
    dmx_stream_t *ds = NULL;

    // sub deinit
    if (pt_demuxer->sub_demuxer->deinit)
    {
        pt_demuxer->sub_demuxer->deinit(pt_demuxer);
    }

    first = swdmx_member_offset(sd_demuxer_t,audio);
    last  = swdmx_member_offset(sd_demuxer_t,sub);

    for (; first < last;)
    {
        ds = (dmx_stream_t *)(*((UINT32 *)(((UINT32)pt_demuxer) + first)));
        if (ds) ctrl_free_dmx_resource(ds->dmx_mul_pid);
        first += sizeof(dmx_stream_t *);
    }

    swdmx_mem_deinit(swdmx_inst->pr_mem_list);

    return TRUE;
}

BOOL _SWDMX_SetDecoderInfoX(UINT8 u1SrcId, UINT8 u1Type, UINT32 u4Para1, UINT32 u4Para2, UINT32 u4Para3)
{

    UNUSED(u1SrcId);
    UNUSED(u1Type);
    UNUSED(u4Para1);
    UNUSED(u4Para2);
    UNUSED(u4Para3);

    return FALSE;
    
}

BOOL _SWDMX_GetInfoX(UINT8 u1SrcId,UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3)
{
    BOOL ret = FALSE;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    sd_demuxer_t *demuxer     = &prSwdmxInst->demuxer;
  
    //if sub-modual 

    //default
    switch(u1Type)
    {
    case eSWDMX_GET_CURRENT_POSITION:
        break;
    case eSWDMX_GET_IS_TRICK:
        if (pu4Para1) *pu4Para1 = FALSE;
        if (pu4Para2) *pu4Para2 = FALSE;
        if ( prSwdmxInst->fgEnVideoDmx && 
            (demuxer->video->idx_tbl) && 
            (((MM_RANGE_ELMT_IDXTBL_T *)(demuxer->video->idx_tbl))->ui4_number_of_entry >= 3)
           )
        {
            if (pu4Para1) *pu4Para1 = TRUE;
            if (pu4Para2) *pu4Para2 = TRUE;
        }
        ret = TRUE;
        break;
    };


    UNUSED(pu4Para3);
    return ret;
}


BOOL _SWDMX_AudioCbX(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    ASSERT(prSwdmxInst);

    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        //SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
        UINT32 stc, vpts;
        
        stc  = STC_GetStcValue(prSwdmxInst->u1StcId);
        vpts = VDP_GetPts(prSwdmxInst->u1B2rId);

        LOG(3, "audio drv cb x:STC, PTS: %d, %d\n", stc, vpts);
        
        if (prSwdmxInst->fgEnVideoDmx)
        {
            VDP_TriggerAudReceive(prSwdmxInst->demuxer.audio->start_pts);
            LOG(3, "trigger pts: %d, %d(ms)\n",prSwdmxInst->demuxer.audio->start_pts, prSwdmxInst->demuxer.audio->start_pts/90);
        }
        else
        {
            // Audio Only case
            LOG(3, "trigger audio here: %d, %d(ms)\n", prSwdmxInst->demuxer.audio->start_pts, prSwdmxInst->demuxer.audio->start_pts/90);
            LOG(3, "Trigger audio by _SWDMX_AviAudioDrvCb\n");
            AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
            STC_StartStc(prSwdmxInst->u1StcId);
        }
    }


    return TRUE;
}


BOOL _SWDMX_SetSpeedX(UINT8 u1SrcId, INT32 i4Speed)
{
    SWDMX_INFO_T* swdmx_inst = _SWDMX_GetInst(u1SrcId);
//    sd_demuxer_t *demuxer    = &swdmx_inst->demuxer;

    if (swdmx_inst->pb_ctrl.cmd.state != SD_PB_CMD_IDLE)
    {
        switch(swdmx_inst->pb_ctrl.cmd.cmd)
        {
        case SD_PB_CMD_CHG_AUDIO:
            break;
        case SD_PB_CMD_SEEK_TIME:
            //case:do timesearch op when pasuing the video
            swdmx_inst->pb_ctrl.last_s = swdmx_inst->pb_ctrl.curr_s;
            swdmx_inst->pb_ctrl.curr_s.speed = i4Speed;
            return TRUE;
        default:
            return FALSE;
        }
    }

    LOG(3, "_SWDMX_SetSpeedX, (i4Speed) => (%d) \n", i4Speed);

    if (swdmx_inst->pb_ctrl.curr_s.speed == i4Speed)
    {
        return TRUE;
    }

    swdmx_inst->pb_ctrl.cmd.state = SD_PB_CMD_DOING;
    swdmx_inst->pb_ctrl.cmd.cmd   = SD_PB_CMD_CHG_SPEED;
    swdmx_inst->pb_ctrl.cmd.arg1  = (UINT32)i4Speed;

    return TRUE;
}

BOOL _SWDMX_SeekTimeX(UINT8 u1SrcId, UINT32 u4SeekTime, UINT64* pu8FilePos)
{
    SWDMX_INFO_T* swdmx_inst = _SWDMX_GetInst(u1SrcId);
    //sd_demuxer_t *demuxer    = &swdmx_inst->demuxer;

    LOG(3, "_SWDMX_SeekTimeX, (time) => (%d), \n", u4SeekTime);

    if (swdmx_inst->pb_ctrl.cmd.state != SD_PB_CMD_IDLE)
    {
        return FALSE;
    }

    swdmx_inst->pb_ctrl.cmd.state = SD_PB_CMD_DOING;
    swdmx_inst->pb_ctrl.cmd.cmd   = SD_PB_CMD_SEEK_TIME;
    swdmx_inst->pb_ctrl.cmd.arg1  = u4SeekTime;

    UNUSED(pu8FilePos);
    return TRUE;
}

BOOL _SWDMX_SeekPosX(UINT8 u1SrcId, UINT64 u8SeekPos, UINT64 u8SeekPosI)
{
    UNUSED(u1SrcId);
    UNUSED(u8SeekPos);
    UNUSED(u8SeekPosI);
    LOG(3, "_SWDMX_SeekPosX, (pos,posI) => (%lld, %lld)\n", u8SeekPos, u8SeekPosI);

    return FALSE;
}

BOOL _SWDMX_SeekPercentX(UINT8 u1SrcId, UINT32 u4SeekPercent, UINT64* pu8FilePos)
{
    UNUSED(u1SrcId);
    UNUSED(u4SeekPercent);
    UNUSED(pu8FilePos);
    LOG(3, "_SWDMX_SeekPercentX, (percent) => (%d)\n", u4SeekPercent);
    return FALSE;
}

BOOL _SWDMX_SeekOffsetX(UINT8 u1SrcId, VOID *prPosition)
{
    UNUSED(u1SrcId);
    UNUSED(prPosition);
    LOG(3, "_SWDMX_SeekOffsetX, (Position) => (%d)\n", *(UINT32*)prPosition);
    return FALSE;
}



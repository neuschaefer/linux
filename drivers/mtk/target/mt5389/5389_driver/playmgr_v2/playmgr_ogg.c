/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: playmgr_avi.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

#if !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)

#include "playmgr.h"
#include "feeder_if.h"
#include "playmgr_util.h"
#include "swdmx_drvif.h"
#include "playmgr_ogg.h"
#include "playmgr_debug.h"

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _PLAYMGR_SetOggInfo(PLAYMGR_STATUS_T* prMediaStatus, PLAYMGR_INFO_T* prMediaInfo)
{
    UINT16 i; 
    UINT16 wTmp = 0;
    PLAYMGR_OGG_INFO_T* prOggInfo = &prMediaInfo->FmtInfo.rOggInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prOggInfo->rSrcInfo;
    
    prMediaStatus->eMediaType = PLAYMGR_MEDIA_TYPE_OGG;

    prMediaStatus->u8FileSize = prMediaInfo->FmtInfo.rOggInfo.rPlayerGenericInfo.ui8_media_sz;

  /*  prOggInfo->rStmAttribute[prOggInfo->u2VidIdx].u.t_ogg_video_stm_attr.e_enc = MINFO_INFO_VID_ENC_MPEG_4;
    prMediaInfo->u2VidStmNum = 1;
    prOggInfo->u2VidIdx = 0;*/
    if ((prMediaInfo->u2VidStmNum > 0) &&  
        (prOggInfo->u2VidIdx < 17))
    {
        prMediaStatus->u2VideoTracks = prMediaInfo->u2VidStmNum;
        LOG(5, "Number of video tracks in this avi is %ld\n", prMediaStatus->u2VideoTracks);
        switch (prOggInfo->rStmAttribute[prOggInfo->u2VidIdx].u.t_ogg_video_stm_attr.e_enc)
        {
            case MINFO_INFO_VID_ENC_MPEG_1: case MINFO_INFO_VID_ENC_MPEG_2:
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_MPEG1_MPEG2;
                break;
            case MINFO_INFO_VID_ENC_DIVX_311: 
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_DIVX311;
                break;
            case MINFO_INFO_VID_ENC_DIVX_4:
            case MINFO_INFO_VID_ENC_DIVX_5: case MINFO_INFO_VID_ENC_XVID:
            case MINFO_INFO_VID_ENC_MPEG_4:
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_MPEG4;
                break;
            case MINFO_INFO_VID_ENC_H264:
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_H264;
                break;
            case MINFO_INFO_VID_ENC_MJPG:
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_MJPEG;
                break;
            default:
                LOG(3, "Unknown Video %d\n", (prOggInfo->rStmAttribute[prOggInfo->u2VidIdx].u.t_ogg_video_stm_attr.e_enc));
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_UNKNOWN;
                break;
                //return FALSE;
        }
    }
    /*prMediaInfo->u2AudStmNum =0;
    prOggInfo->u2AudIdx = 0;*/
    if ((prMediaInfo->u2AudStmNum > 0) &&
        (prOggInfo->u2AudIdx < 17))
    {
        prMediaStatus->u2AudioTracks = prMediaInfo->u2AudStmNum;
        LOG(5, "Number of audio tracks in this avi is %ld\n", prMediaStatus->u2AudioTracks);
        
        for (i = prOggInfo->u2AudIdx;((i < prMediaInfo->u2AudStmNum + prOggInfo->u2AudIdx)) && (i < 16);i++)
        {
            wTmp = i - prMediaInfo->u2VidStmNum;

            if (wTmp < PLAYMGR_MAX_AUD_TRACK)
            {
                prOggInfo->rStmAttribute[i].u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
                switch (prOggInfo->rStmAttribute[i].u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_enc)
                {
                    case MINFO_INFO_AUD_ENC_MPEG_1:
                        /* if (prOggInfo->rStmAttribute[i].u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_layer == 3)
                         {
                             prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = PLAYMGR_ADEC_MP3;
                         }
                         else
                         {
                             prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = PLAYMGR_ADEC_MPEG;
                         }*/
                        prMediaStatus->aeAudFormat[wTmp] = PLAYMGR_ADEC_MP3;
                        break;
                    case MINFO_INFO_AUD_ENC_MPEG_2:
                        prMediaStatus->aeAudFormat[wTmp] = PLAYMGR_ADEC_MPEG;
                        break;
                    case MINFO_INFO_AUD_ENC_AC3:
                        prMediaStatus->aeAudFormat[wTmp] = PLAYMGR_ADEC_AC3;
                        break;
                    case MINFO_INFO_AUD_ENC_PCM:
                        prMediaStatus->aeAudFormat[wTmp] = PLAYMGR_ADEC_PCM;
                        break;
                    case MINFO_INFO_AUD_ENC_AAC:
                        prMediaStatus->aeAudFormat[wTmp] = PLAYMGR_ADEC_AAC;
                        break;
                    case MINFO_INFO_AUD_ENC_ADPCM:
                        prMediaStatus->aeAudFormat[wTmp] = PLAYMGR_ADEC_ADPCM;
                        break;
                    default:
                        LOG(3, "Unknown Audio %d\n", (prOggInfo->rStmAttribute[i].u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.e_enc));
                        prMediaStatus->aeAudFormat[wTmp] = PLAYMGR_ADEC_UNKNOWN;   //not support
                        //   LOG(1, "Aud type %d not support\n", prMediaInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.e_enc);
                        break;
                }
            }
        }
    }

    for (i = 0; (i < prMediaInfo->u2VidStmNum) && (i < 17); i++)
    {
        prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;

        if (i < PLAYMGR_MAX_VID_TRACK)
        {
            prMediaStatus->au4VideoId[i] = prOggInfo->rStmAttribute[i].t_stm_id.u.t_ogg_stm_id;
        }

        prSrcInfo->t_es_map[i].ui4_strm_id = 
            prOggInfo->rStmAttribute[i].t_stm_id.u.t_ogg_stm_id;
        prSrcInfo->t_es_map[i].ui8_timeunit =
                prOggInfo->rStmAttribute[i].u.t_ogg_video_stm_attr.ui8_timeunit;
        prSrcInfo->t_es_map[i].ui4_avg_packet_size =
            prOggInfo->rStmAttribute[i].u.t_ogg_video_stm_attr.ui4_avg_packet_size;
        prSrcInfo->t_es_map[i].ui4_avg_frame_num =
                prOggInfo->rStmAttribute[i].u.t_ogg_video_stm_attr.ui4_avg_frame_num;
       
         prSrcInfo->t_es_map[i].ui8_granule_pos = 
                        prOggInfo->rStmAttribute[i].u.t_ogg_video_stm_attr.ui8_granule_pos;
    }

    for (;(i < prMediaInfo->u2AudStmNum + prMediaInfo->u2VidStmNum) && (i < 17); i++)
    {
        wTmp = i - prMediaInfo->u2VidStmNum;
        if (wTmp < PLAYMGR_MAX_AUD_TRACK)
        {
            prMediaStatus->au4AudioId[wTmp] =
                prOggInfo->rStmAttribute[i].t_stm_id.u.t_ogg_stm_id;

            prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
            prSrcInfo->t_es_map[i].ui4_strm_id = 
            	prOggInfo->rStmAttribute[i].t_stm_id.u.t_ogg_stm_id;

            prSrcInfo->t_es_map[i].ui4_avg_bytes_per_sec =  
                   prOggInfo->rStmAttribute[i].u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui4_avg_bytes_per_sec;
            prSrcInfo->t_es_map[i].ui4_rate = 
                prOggInfo->rStmAttribute[i].u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui8_sample_per_unit;
            /* prSrcInfo->t_es_map[i].ui8_timeunit = 
                prOggInfo->rStmAttribute[i].u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui8_sample_per_unit;*/
        
           /* prSrcInfo->t_es_map[i].ui8_granule_pos = 
                        prOggInfo->rStmAttribute[i].u.t_ogg_audio_stm_attr.u.t_ogg_strm_audio_info.ui8_granule_pos;*/
        }
        else
        {
            LOG(3, "there r too many audio tracks\n");
            return FALSE;
        }
    }
      
    // Backup the media information
    x_memcpy(&prMediaInfo->rPlayerStatus, prMediaStatus, sizeof(PLAYMGR_STATUS_T));

    return TRUE;
}

static void _PLAYMGR_Minfo_Notify(
                           HANDLE_T                    hMinfo,                 
                           MINFO_NFY_EVENT_TYPE_T      e_event,  
                           const VOID*                 pvData1,
                           const VOID*                 pvData2,
                           const VOID*                 pvTag) 

{
    SIZE_T zRecLength;
    UINT16 u2Temp;
    INT32 i4Ret;  
    MINFO_INFO_T rMinfoRec;
    PLAYMGR_INFO_T *prMediaInfo;    
    PLAYMGR_OGG_INFO_T* prOggInfo;
    MINFO_NFY_EVENT_TYPE_T      eEvent = e_event;
    
    zRecLength = sizeof(MINFO_INFO_T);
    prMediaInfo = (PLAYMGR_INFO_T*)pvTag;
    prOggInfo = &prMediaInfo->FmtInfo.rOggInfo;

    switch(e_event) {
    case MINFO_NFY_EVENT_PARSING_DONE:
        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_GENERIC, 1,&rMinfoRec, &zRecLength);
    	 if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
	 {
	        LOG(3, "get media geberic info error\n");
	        eEvent = MINFO_NFY_EVENT_ERROR;
	        break;
    	 }
	 x_memcpy(&prOggInfo->rPlayerGenericInfo, &(rMinfoRec.u.t_general_info),
	 	          sizeof(MINFO_TYPE_GENERIC_T));
        
        i4Ret = x_minfo_get_info(hMinfo, 
        	                                    MINFO_INFO_TYPE_IDX_INFO, 
        	                                    1,
        	                                    &rMinfoRec, 
        	                                    &zRecLength);
	 if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
        {
            LOG(3, "get media idx info error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
	 }
        x_memcpy(&prOggInfo->rPlayerIdxInfo, &(rMinfoRec.u.t_idx_info),
	 	          sizeof(MINFO_TYPE_IDX_INFO_T));

        i4Ret = x_minfo_get_num_recs(hMinfo, 
        	                                             MINFO_INFO_TYPE_VIDEO_ATTR, 
        	                                             &prMediaInfo->u2VidStmNum);
        if (i4Ret != MINFOR_OK)
        {
            LOG(1, "get media video recs error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }

        prOggInfo->u2AudIdx = prMediaInfo->u2VidStmNum;
        for (u2Temp = 0; u2Temp < prMediaInfo->u2VidStmNum; u2Temp++)
        {
            if (u2Temp < 17)
            {
                i4Ret = x_minfo_get_info(hMinfo,MINFO_INFO_TYPE_VIDEO_ATTR, (MINFO_REC_IDX_T)(u2Temp + 1), &rMinfoRec, &zRecLength);
                if (i4Ret != MINFOR_OK)
                {
                    LOG(1, "get media video info error\n");
                    eEvent = MINFO_NFY_EVENT_ERROR;
                    break;
                }

                x_memcpy(&prOggInfo->rStmAttribute[u2Temp], 
                                 &(rMinfoRec.u.t_stm_attribute),
                                 sizeof(MINFO_TYPE_STM_ATRBT_T));
            }
        }

        i4Ret = x_minfo_get_num_recs(hMinfo, MINFO_INFO_TYPE_AUDIO_ATTR, &prMediaInfo->u2AudStmNum);
        if (i4Ret != MINFOR_OK)
        {
            LOG(3, "get media audio recs error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }
        
        for (; u2Temp < prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum; u2Temp++)
        {
            if (u2Temp < 17)
            {
                i4Ret = x_minfo_get_info(hMinfo,MINFO_INFO_TYPE_AUDIO_ATTR, (MINFO_REC_IDX_T)(u2Temp + 1 - prOggInfo->u2AudIdx), &rMinfoRec, &zRecLength);
                if (i4Ret != MINFOR_OK)
                {
                    LOG(3, "get media audio info error\n");
                    eEvent = MINFO_NFY_EVENT_ERROR;
                    break;
                }

                x_memcpy(&prOggInfo->rStmAttribute[u2Temp], 
                    &(rMinfoRec.u.t_stm_attribute),
                    sizeof(MINFO_TYPE_STM_ATRBT_T));
            }
        }
        // get meta data
        x_memset(&rMinfoRec, 0, zRecLength);
        break;
    case MINFO_NFY_EVENT_HANDLE_CLOSED:
        break;
    case MINFO_NFY_EVENT_ERROR:             
        break;
    default:
        break;
    }

    _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, eEvent);
}

/*static VOID _PLAYMGR_MIdx_Notify(
                           HANDLE_T                    hMinfo,                 
                           MIDXBULD_NFY_EVENT_TYPE_T   e_event,  
                           const VOID*                 pvData1,
                           const VOID*                 pvData2,
                           const VOID*                 pvTag) 

{
    PLAYMGR_INFO_T *prMediaInfo;    
    prMediaInfo = (PLAYMGR_INFO_T*)pvTag;

    switch(e_event) {
    case MIDXBULD_NFY_EVENT_INDEXING_DONE:
        break;
    case MIDXBULD_NFY_EVENT_INDEXING_ABORT:
        break;
    case MIDXBULD_NFY_EVENT_HANDLE_CLOSED:
        break;
    case MIDXBULD_NFY_EVENT_ERROR:             
        break;
    default:
        break;
    }

    _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, e_event);
}*/


BOOL _PLAYMGR_OggStart(VOID* pvMediaInfo)
{
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_OGG_INFO_T* prOggInfo = 
    	(PLAYMGR_OGG_INFO_T*)&prMediaInfo->FmtInfo.rOggInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prOggInfo->rSrcInfo;
   
    if (prMediaInfo == 0)
    {
        LOG(1, "_PLAYMGR_OggStart prMediaInfo == 0\n");
        return FALSE;
    }
  
    if((prMediaInfo->u2VidStmNum > 0) ||
        (prMediaInfo->u2AudStmNum > 0))
    {
        prMediaInfo->rRangeInfo[0].ui8_pb_duration = (UINT64)prSrcInfo->ui4_total_duration;
        SWDMX_AddRange(prMediaInfo->u1SrcId,
                       &prMediaInfo->rRangeInfo[0], 
                       (UINT32)prMediaInfo->rRangeInfo[0].ui4_range_id);
    }
    else
    {
        LOG(3, "range render cnt 0!\n");
    }
    return TRUE;
}


BOOL _PLAYMGR_OggStop(VOID* pvMediaInfo)
{
#if 0
    BOOL fgRet = TRUE;
    //PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    fgRet = _SWDMX_Stop();
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_AviStop Fail in _SWDMX_Stop\n");
        return FALSE;
    }
#endif    
    return TRUE;
}

EXTERN BOOL _PLAYMGR_OggClose(VOID* pvMediaInfo)
{
    BOOL fgRet = TRUE;
    UINT32 u4Msg;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    if (prMediaInfo->FmtInfo.rOggInfo.hMediaInfoHandle)
    {
        x_minfo_close(prMediaInfo->FmtInfo.rOggInfo.hMediaInfoHandle);
        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (VOID*)&u4Msg);
        if (u4Msg != MINFO_NFY_EVENT_HANDLE_CLOSED)
        {
            LOG(1, "x_minfo_close failed\n");
            fgRet = FALSE;
        }
    }
 
    x_memset(&prMediaInfo->FmtInfo.rOggInfo, 0, sizeof(PLAYMGR_OGG_INFO_T));

    return fgRet;
}

EXTERN BOOL _PLAYMGR_OggResume(VOID* pvMediaInfo)
{
#if 0
    BOOL fgRet;
    
    fgRet = _SWDMX_Play();
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_AviStart Fail to play\n");
        return FALSE;
    }
#endif
    return TRUE;
}

BOOL _PLAYMGR_OggPause(VOID)
{
#if 0
    return _SWDMX_Pause();
#endif
    return TRUE;
}


BOOL _PLAYMGR_OpenOggMediaInfo(PLAYMGR_STATUS_T* prMediaStatus, VOID* pvMediaInfo)
{
    INT32 i4Ret;
    UINT32 u4Msg;
   
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    PLAYMGR_OGG_INFO_T* prOggInfo = 
    	(PLAYMGR_OGG_INFO_T*)&prMediaInfo->FmtInfo.rOggInfo;
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt; 

    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prOggInfo->rSrcInfo;
    MIDXBULD_ELMT_TBL_DESCRIB_T* prVDescrib = &prOggInfo->rVDescrib[0];
    MIDXBULD_ELMT_TBL_DESCRIB_T* prADescrib = &prOggInfo->rADescrib[0];

    MM_RANGE_ELMT_IDXTBL_T* prVidElmtStrm = &prOggInfo->rVidIDXTBL[0];
    MM_RANGE_ELMT_IDXTBL_T* prAudElmtStrm = &prOggInfo->rAudIDXTBL[0];


    // Open for media info
   i4Ret = x_minfo_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _PLAYMGR_Minfo_Notify, prMediaInfo,
            		       prMediaFmt, &(prOggInfo->hMediaInfoHandle));
    if(i4Ret != MINFOR_OK)
    {
        return FALSE;
    }

    _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (VOID*)&u4Msg);
    if (u4Msg != MINFO_NFY_EVENT_PARSING_DONE)
    {
        return FALSE;
    }
    
    // Wait for media info's notify
    prSrcInfo->ui8_data_offset = prOggInfo->rPlayerIdxInfo.ui8_data_offset;
    prSrcInfo->ui8_data_size = prOggInfo->rPlayerIdxInfo.ui8_data_sz;
    prSrcInfo->ui8_idx_offset = prOggInfo->rPlayerIdxInfo.ui8_idx_offset;
    prSrcInfo->ui8_idx_size = prOggInfo->rPlayerIdxInfo.ui8_idx_sz;
    prSrcInfo->ui4_idx_type = prOggInfo->rPlayerIdxInfo.ui4_idx_type;
    prSrcInfo->ui1_total_strm_num = (UINT8)(prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum);
    // Temporally multiply SYSTEM_TIME_SCALE
    prSrcInfo->ui4_total_duration = (UINT32)prOggInfo->rPlayerGenericInfo.ui8_pb_duration;// * SYSTEM_TIME_SCALE;
    prMediaStatus->u4TotalDuration = (UINT32)prOggInfo->rPlayerGenericInfo.ui8_pb_duration;
    prMediaInfo->rPlayerStatus.u4TotalDuration = (UINT32)prOggInfo->rPlayerGenericInfo.ui8_pb_duration;

    if (_PLAYMGR_SetOggInfo(prMediaStatus, prMediaInfo) != TRUE)
    {
        return FALSE;
    }

    prMediaInfo->rRangeInfo[0].ui8_fileoffset = prSrcInfo->ui8_data_offset;
    prMediaInfo->rRangeInfo[0].z_range_sz =prSrcInfo->ui8_data_size;


    if ((prMediaInfo->u2VidStmNum > 0) &&
        (prOggInfo->u2VidIdx < 17))
    {
        prMediaInfo->rRangeInfo[0].pt_idxtbl_lst = prVidElmtStrm;

        prVidElmtStrm->e_elem_type = MM_ELEMT_TYPE_VIDEO;
        prVidElmtStrm->t_strm_id.u.t_stm_ogg_id = prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui4_strm_id;
        prVidElmtStrm->ui4_rate = prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui4_rate;
        prVidElmtStrm->ui4_scale = prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui4_scale;
        //prVidElmtStrm->video_page_num = prSrcInfo->t_es_map[prOggInfo->u2VidIdx].video_page_num;
        prVidElmtStrm->ui4_avg_packet_size = prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui4_avg_packet_size;
        prVidElmtStrm->ui4_avg_frame_num = prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui4_avg_frame_num;
        prVidElmtStrm->ui8_timeunit = prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui8_timeunit;
        prVidElmtStrm->ui4_avg_bytes_per_sec = 
            prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui4_avg_bytes_per_sec;
        prVidElmtStrm->ui4_number_of_entry = 
            prOggInfo->rStmAttribute[prOggInfo->u2VidIdx].u.t_avi_video_stm_attr.ui4_length;

        prVDescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        prVDescrib->u_info.t_es.ui4_strm_id = 
            prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui4_strm_id;
        prVDescrib->u_info.t_es.ui4_scale = 
            prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui4_scale;
        prVDescrib->u_info.t_es.ui8_timeunit = 
            prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui8_timeunit;
        prVDescrib->u_info.t_es.ui4_rate = 
            prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui4_rate;
        prVDescrib->u_info.t_es.ui4_sample_sz = 0;
        prVDescrib->u_info.t_es.ui4_avg_bytes_per_sec = 0;

        prVDescrib->u_info.t_es.ui4_indx_offset = 
            prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui4_indx_offset;
        prVDescrib->u_info.t_es.ui4_indx_size = 
            prSrcInfo->t_es_map[prOggInfo->u2VidIdx].ui4_indx_size;
    }
    
    if ((prMediaInfo->u2AudStmNum > 0) &&
        (prOggInfo->u2AudIdx < 17))
    {
        (prMediaInfo->u2VidStmNum > 0) ? (prVidElmtStrm->pv_next_tbl = prAudElmtStrm)
        : (prMediaInfo->rRangeInfo[0].pt_idxtbl_lst = prAudElmtStrm);

        prAudElmtStrm->e_elem_type = MM_ELEMT_TYPE_AUDIO;
        prAudElmtStrm->t_strm_id.u.t_stm_ogg_id = prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_strm_id;
        prAudElmtStrm->ui4_rate =prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_rate;
        prAudElmtStrm->ui4_scale = prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_scale;
        prAudElmtStrm->ui8_timeunit =prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_rate;
        prAudElmtStrm->ui4_avg_bytes_per_sec = 
              prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_avg_bytes_per_sec;
        prAudElmtStrm->ui4_number_of_entry = 
            prOggInfo->rStmAttribute[prOggInfo->u2AudIdx].u.t_avi_video_stm_attr.ui4_length;
   
        prADescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        prADescrib->u_info.t_es.ui4_strm_id = 
                    prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_strm_id;
        prADescrib->u_info.t_es.ui4_scale = 
        	prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_scale;
        prADescrib->u_info.t_es.ui8_timeunit = 
            prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui8_timeunit;
        prADescrib->u_info.t_es.ui4_rate =  
            prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_rate;
        prADescrib->u_info.t_es.ui4_sample_sz = 
        	prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_sample_sz ;
        prADescrib->u_info.t_es.ui4_avg_bytes_per_sec = 
            prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_avg_bytes_per_sec;

        prADescrib->u_info.t_es.ui4_indx_offset = 
            prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_indx_offset;
        prADescrib->u_info.t_es.ui4_indx_size = 
            prSrcInfo->t_es_map[prOggInfo->u2AudIdx].ui4_indx_size;

    }

    return TRUE;
}


BOOL _PLAYMGR_OggFreeRange(VOID* pvMediaInfo, UINT32 u4RangeTag)
{
    return TRUE;
}

#endif // !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)




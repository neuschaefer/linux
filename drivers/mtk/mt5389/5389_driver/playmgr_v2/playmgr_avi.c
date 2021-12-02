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
#include "playmgr_avi.h"
#include "playmgr_debug.h"

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//#define EN_PLAYMGR_AVI_DRM

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

//static PLAYMGR_DRMTYPE _eDRMType = PLAYMGR_DRMTYPE_NONE;
//static UINT32 _u4DRMRentalLimit = 0;
//static UINT32 _u4DRMRentalCount = 0;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _PLAYMGR_SetAVIInfo(PLAYMGR_STATUS_T* prMediaStatus, PLAYMGR_INFO_T* prMediaInfo)
{
#ifndef __MODEL_slt__
    UINT16 i; 
    UINT32 u4Temp;
#endif
    PLAYMGR_AVI_INFO_T* prAviInfo = &prMediaInfo->FmtInfo.rAviInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prAviInfo->rSrcInfo;
#ifndef __MODEL_slt__
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prAviInfo->rKeyInfo;
#endif
    prMediaStatus->eMediaType = PLAYMGR_MEDIA_TYPE_AVI;
#ifndef __MODEL_slt__
    prMediaStatus->u8FileSize = prMediaInfo->FmtInfo.rAviInfo.rPlayerGenericInfo.ui8_media_sz;
    
    if ((prMediaInfo->u2VidStmNum > 0) &&
        (prAviInfo->u2VidIdx < 17))
    {
        prMediaStatus->u2VideoTracks = prMediaInfo->u2VidStmNum;
        LOG(5, "Number of video tracks in this avi is %ld\n", prMediaStatus->u2VideoTracks);
        switch (prAviInfo->rStmAttribute[prAviInfo->u2VidIdx].u.t_avi_video_stm_attr.e_enc)
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
                LOG(3, "Unknown Video %d\n", (prAviInfo->rStmAttribute[prAviInfo->u2VidIdx].u.t_avi_video_stm_attr.e_enc));
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_UNKNOWN;
                break;
                //return FALSE;
        }
    }

    if ((prMediaInfo->u2AudStmNum > 0) &&
        (prAviInfo->u2AudIdx < 17))
    {
        prMediaStatus->u2AudioTracks = prMediaInfo->u2AudStmNum;
        LOG(5, "Number of audio tracks in this avi is %ld\n", prMediaStatus->u2AudioTracks);
        
        for(i = prAviInfo->u2AudIdx; 
        ((i < prMediaInfo->u2AudStmNum + prAviInfo->u2AudIdx) && 
        ((i - prMediaInfo->u2VidStmNum)<PLAYMGR_MAX_AUD_TRACK)) &&
        (i < 16) && (i >= prMediaInfo->u2VidStmNum); 
        i++)
       {
            switch (prAviInfo->rStmAttribute[i].u.t_avi_audio_stm_attr.e_enc)
            {
                case MINFO_INFO_AUD_ENC_MPEG_1: 
                    if (prAviInfo->rStmAttribute[i].u.t_avi_audio_stm_attr.e_layer == 3)
                    {
                        prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = PLAYMGR_ADEC_MP3;
                    }
                    else
                    {
                        prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = PLAYMGR_ADEC_MPEG;
                    }
                    break;
                case MINFO_INFO_AUD_ENC_MPEG_2:
                    prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = PLAYMGR_ADEC_MPEG;
                    break;
                case MINFO_INFO_AUD_ENC_AC3:
                    prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = 
                        PLAYMGR_ADEC_AC3;
                    break;
                case MINFO_INFO_AUD_ENC_PCM:
                    prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = 
                        PLAYMGR_ADEC_PCM;
                    break;
                case MINFO_INFO_AUD_ENC_AAC:
                    prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = 
                        PLAYMGR_ADEC_AAC;
                    break;
                case MINFO_INFO_AUD_ENC_ADPCM:
                    prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = 
                        PLAYMGR_ADEC_ADPCM;
                    break;                    
                default:
                    LOG(3, "Unknown Audio %d\n", (prAviInfo->rStmAttribute[i].u.t_avi_audio_stm_attr.e_enc));
                    prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = 
                        PLAYMGR_ADEC_UNKNOWN;   //not support
                 //   LOG(1, "Aud type %d not support\n", prMediaInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.e_enc);
                	  break;
             }
        }
    }

    if (prMediaInfo->u2SubStmNum > 0)
    {
        prMediaStatus->u2CaptionTracks = prMediaInfo->u2SubStmNum;
        LOG(5, "Number of subtitle tracks in this avi is %ld\n", prMediaStatus->u2CaptionTracks);
    }

    for (i = 0; (i < prMediaInfo->u2VidStmNum) && (i < 17); i++)
    {
        prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;

        if (i < PLAYMGR_MAX_VID_TRACK)
        {
            prMediaStatus->au4VideoId[i] = prAviInfo->rStmAttribute[i].t_stm_id.u.t_avi_stm_id;
        }

        prSrcInfo->t_es_map[i].ui4_strm_id = 
        	prAviInfo->rStmAttribute[i].t_stm_id.u.t_avi_stm_id;
        prSrcInfo->t_es_map[i].ui4_scale = 
        	prAviInfo->rStmAttribute[i].u.t_avi_video_stm_attr.ui4_scale;
        prSrcInfo->t_es_map[i].ui4_rate = 
        	prAviInfo->rStmAttribute[i].u.t_avi_video_stm_attr.ui4_rate;
        prSrcInfo->t_es_map[i].ui4_sample_sz = 
        	prAviInfo->rStmAttribute[i].u.t_avi_video_stm_attr.ui4_sample_size;
        prSrcInfo->t_es_map[i].ui4_avg_bytes_per_sec = 0;

        prMediaInfo->rRangeInfo[0].ui8_vid_duration =
            prAviInfo->rStmAttribute[i].u.t_avi_video_stm_attr.ui8_duration;
        
        // Avi 2.0
        prSrcInfo->t_es_map[i].ui4_indx_offset = 
            prAviInfo->rStmAttribute[i].u.t_avi_video_stm_attr.ui4_indx_offset;
        prSrcInfo->t_es_map[i].ui4_indx_size = 
            prAviInfo->rStmAttribute[i].u.t_avi_video_stm_attr.ui4_indx_size;
    }

    for (;(i < prMediaInfo->u2AudStmNum + prMediaInfo->u2VidStmNum) && (i < 17) && (i >= prMediaInfo->u2VidStmNum); i++)
    {
        if ((i - prMediaInfo->u2VidStmNum) < PLAYMGR_MAX_AUD_TRACK)
        {
            prMediaStatus->au4AudioId[i-prMediaInfo->u2VidStmNum] = 
                prAviInfo->rStmAttribute[i].t_stm_id.u.t_avi_stm_id;

            prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
            prSrcInfo->t_es_map[i].ui4_strm_id = 
            	prAviInfo->rStmAttribute[i].t_stm_id.u.t_avi_stm_id;
            prSrcInfo->t_es_map[i].ui4_scale = 
            	prAviInfo->rStmAttribute[i].u.t_avi_audio_stm_attr.ui4_scale;
            prSrcInfo->t_es_map[i].ui4_rate = 
            	prAviInfo->rStmAttribute[i].u.t_avi_audio_stm_attr.ui4_rate;
            prSrcInfo->t_es_map[i].ui4_sample_sz = 
            	prAviInfo->rStmAttribute[i].u.t_avi_audio_stm_attr.ui4_sample_size;
            prSrcInfo->t_es_map[i].ui4_avg_bytes_per_sec = 
                prAviInfo->rStmAttribute[i].u.t_avi_audio_stm_attr.ui4_avg_bytes_sec;

            // Avi 2.0
            prSrcInfo->t_es_map[i].ui4_indx_offset = 
                prAviInfo->rStmAttribute[i].u.t_avi_audio_stm_attr.ui4_indx_offset;
            prSrcInfo->t_es_map[i].ui4_indx_size = 
                prAviInfo->rStmAttribute[i].u.t_avi_audio_stm_attr.ui4_indx_size;
        }
        else
        {
            LOG(3, "there r too many audio tracks\n");
            return FALSE;
        }
    }

    u4Temp = prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum;
    for (;(i < prMediaInfo->u2AudStmNum + prMediaInfo->u2VidStmNum + prMediaInfo->u2SubStmNum) && (i < 17); i++)
    {
        if ((i >= u4Temp) &&
            ((i - u4Temp) < PLAYMGR_MAX_SUB_TRACK))
        {
            prMediaStatus->au4CapTrackId[(UINT16)(i - (UINT16)u4Temp)] = prAviInfo->rStmAttribute[i].t_stm_id.u.t_avi_stm_id;
            switch (prAviInfo->rStmAttribute[i].u.t_avi_sp_stm_attr.ui4_subtitle_type)
            {
                case MINFO_INFO_AVI_SUBTITLE_TYPE_TEXT:
                    prMediaStatus->aeCapFormat[(UINT16)(i - (UINT16)u4Temp)] = PLAYMGR_CAPTION_AVI_TEXT;
                    break;
                case MINFO_INFO_AVI_SUBTITLE_TYPE_BITMAP:
                    prMediaStatus->aeCapFormat[(UINT16)(i - (UINT16)u4Temp)] = PLAYMGR_CAPTION_AVI_BITMAP;
                    break;
                case MINFO_INFO_AVI_SUBTITLE_TYPE_BITMAP_DXSA: 
                    prMediaStatus->aeCapFormat[(UINT16)(i - (UINT16)u4Temp)] = PLAYMGR_CAPTION_AVI_DXSA;
                    break;
                default:
                    LOG(1, "Unknown Subtitle Type\n");
            }
        }
        else
        {
            LOG(3, "there r too many subtitle tracks\n");
            return FALSE;
        }
    }


    if (prAviInfo->u2VidIdx < 17)
    {
        prKeyInfo->ui4_strm_id = prSrcInfo->t_es_map[prAviInfo->u2VidIdx].ui4_strm_id;
        prKeyInfo->ui4_scale = 
        	prAviInfo->rStmAttribute[prAviInfo->u2VidIdx].u.t_avi_video_stm_attr.ui4_scale;
        prKeyInfo->ui4_rate = 
        	prAviInfo->rStmAttribute[prAviInfo->u2VidIdx].u.t_avi_video_stm_attr.ui4_rate;
        prKeyInfo->ui4_sample_sz = 
        	prAviInfo->rStmAttribute[prAviInfo->u2VidIdx].u.t_avi_video_stm_attr.ui4_sample_size;
        prKeyInfo->ui4_avg_bytes_per_sec = 0;
    }
#else
    prMediaStatus->u8FileSize = 523228422;
    prMediaStatus->u2VideoTracks = 1;
    prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_MPEG4;
    prMediaStatus->u2AudioTracks = 1;
    prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_AC3;
    prMediaStatus->u2CaptionTracks = 0;
    prMediaInfo->u2VidStmNum = 1;
    prMediaInfo->u2AudStmNum = 1;

    prAviInfo->u2VidIdx = 0;
    prAviInfo->u2AudIdx = 1;
        
    prSrcInfo->t_es_map[0].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;

    prMediaStatus->au4VideoId[0] = 0x63643030;

    prSrcInfo->t_es_map[0].ui4_strm_id = 0x63643030;
    prSrcInfo->t_es_map[0].ui4_scale = 1;
    prSrcInfo->t_es_map[0].ui4_rate = 0x1E;
    prSrcInfo->t_es_map[0].ui4_sample_sz = 0;
    prSrcInfo->t_es_map[0].ui4_avg_bytes_per_sec = 0;

    prMediaInfo->rRangeInfo[0].ui8_vid_duration = 19287000;
    
    prMediaStatus->au4AudioId[0] = 0x62773130;

    prSrcInfo->t_es_map[1].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
    prSrcInfo->t_es_map[1].ui4_strm_id = 0x62773130;
    prSrcInfo->t_es_map[1].ui4_scale = 1;
    prSrcInfo->t_es_map[1].ui4_rate = 0x5DC0;
    prSrcInfo->t_es_map[1].ui4_sample_sz = 1;
    prSrcInfo->t_es_map[1].ui4_avg_bytes_per_sec = 1;

#endif

    // Backup the media information
    x_memcpy(&prMediaInfo->rPlayerStatus, prMediaStatus, sizeof(PLAYMGR_STATUS_T));

    return TRUE;
}

#ifndef __MODEL_slt__
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
    PLAYMGR_AVI_INFO_T* prAviInfo;
    MINFO_NFY_EVENT_TYPE_T      eEvent = e_event;
#ifdef EN_PLAYMGR_AVI_DRM
    DRM_COMMAND_T arDrmCmdSets[2];
#endif

    zRecLength = sizeof(MINFO_INFO_T);
    prMediaInfo = (PLAYMGR_INFO_T*)pvTag;
    prAviInfo = &prMediaInfo->FmtInfo.rAviInfo;

    switch(e_event) {
    case MINFO_NFY_EVENT_PARSING_DONE:
        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_GENERIC, 1,&rMinfoRec, &zRecLength);
    	 if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
	 {
	        LOG(3, "get media geberic info error\n");
	        eEvent = MINFO_NFY_EVENT_ERROR;
	        break;
    	 }
	 x_memcpy(&prAviInfo->rPlayerGenericInfo, &(rMinfoRec.u.t_general_info),
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
        x_memcpy(&prAviInfo->rPlayerIdxInfo, &(rMinfoRec.u.t_idx_info),
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

        prAviInfo->u2AudIdx = prMediaInfo->u2VidStmNum;
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

                x_memcpy(&prAviInfo->rStmAttribute[u2Temp], 
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
                i4Ret = x_minfo_get_info(hMinfo,MINFO_INFO_TYPE_AUDIO_ATTR, (MINFO_REC_IDX_T)(u2Temp + 1 - prAviInfo->u2AudIdx), &rMinfoRec, &zRecLength);
                if (i4Ret != MINFOR_OK)
                {
                    LOG(3, "get media audio info error\n");
                    eEvent = MINFO_NFY_EVENT_ERROR;
                    break;
                }

                x_memcpy(&prAviInfo->rStmAttribute[u2Temp], 
                    &(rMinfoRec.u.t_stm_attribute),
                    sizeof(MINFO_TYPE_STM_ATRBT_T));
            }
        }

        i4Ret = x_minfo_get_num_recs(hMinfo, MINFO_INFO_TYPE_SP_ATTR, &prMediaInfo->u2SubStmNum);
        if (i4Ret != MINFOR_OK)
        {
            LOG(3, "get media subtitle recs error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }

        prAviInfo->u2SubIdx = prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum;
        for (; u2Temp < prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum + prMediaInfo->u2SubStmNum; u2Temp++)
        {
            if (u2Temp < 17)
            {
                i4Ret = x_minfo_get_info(hMinfo,MINFO_INFO_TYPE_SP_ATTR, (MINFO_REC_IDX_T)(u2Temp + 1 - prAviInfo->u2SubIdx), &rMinfoRec, &zRecLength);
                if (i4Ret != MINFOR_OK)
                {
                    LOG(1, "get media subtitle info error\n");
                    eEvent = MINFO_NFY_EVENT_ERROR;
                    break;
                }

                x_memcpy(&prAviInfo->rStmAttribute[u2Temp], 
                    &(rMinfoRec.u.t_stm_attribute),
                    sizeof(MINFO_TYPE_STM_ATRBT_T));
            }
        }

        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_DRM_INFO, 0, &rMinfoRec, &zRecLength);
        if (i4Ret != MINFOR_OK)
        {
            LOG(1, "get drm info error\n");
            _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, MINFO_NFY_EVENT_ERROR);
            return;
        }

        //_PLAYMGR_AviGetDivxRegCode((void*)prMediaInfo);

        prAviInfo->rDivXDrmInfo.u4DrmType = rMinfoRec.u.t_drm_info.t_minfo_drm_type;

#ifdef EN_PLAYMGR_AVI_DRM
        if (prAviInfo->rDivXDrmInfo.u4DrmType == DRM_TYPE_DIVX_DRM)
        {
            DIVX_DRM_UNION_INFO_T rDivxDrmInfo;

            prAviInfo->eDRMType = PLAYMGR_DRMTYPE_PURCHASE;
            prAviInfo->u4DRMRentalLimit = 0;
            prAviInfo->u4DRMRentalCount = 0;

            arDrmCmdSets[0].e_code = DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK;
            arDrmCmdSets[0].u.pv_data = rMinfoRec.u.t_drm_info.u.t_minfo_divx_drm_obj.pui1_strd_chunk;
            arDrmCmdSets[1].e_code = DRM_CMD_CODE_END;

            LOG(0, "get divx drm handle\n");
            // Get drm handle.
            i4Ret = x_drm_open(arDrmCmdSets, rMinfoRec.u.t_drm_info.t_minfo_drm_type, &prAviInfo->hDivXDrmHandle);
            if (i4Ret != DRMR_OK)
            {
                LOG(0, "get divx drm info error\n");
                _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, MINFO_NFY_EVENT_ERROR);
                return;
            }

            i4Ret = x_drm_get_info(prAviInfo->hDivXDrmHandle, DRM_GET_TYPE_DIVX_DRM_BASIC_INFO, &rDivxDrmInfo, sizeof(DIVX_DRM_UNION_INFO_T));
            if (i4Ret != DRMR_OK)
            {
                LOG(0, "get divx drm info error\n");
                _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, MINFO_NFY_EVENT_ERROR);
                return;
            }

            if (!rDivxDrmInfo.pt_divx_drm_basic_info)
            {
                LOG(0, "rDivxDrmInfo.pt_divx_drm_basic_info null pointer\n");
                _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, MINFO_NFY_EVENT_ERROR);
                return;
            }

            prAviInfo->rDivXDrmInfo.prDivxDrmBasicInfo = rDivxDrmInfo.pt_divx_drm_basic_info;

            if (prAviInfo->rDivXDrmInfo.prDivxDrmBasicInfo->ui1_flag & DIVX_DRM_AUTH_ERROR)
            {
                prAviInfo->eDRMType = PLAYMGR_DRMTYPE_AUTH_ERR;
                prAviInfo->u4DRMRentalLimit = prAviInfo->rDivXDrmInfo.prDivxDrmBasicInfo->ui1_use_limit;
                prAviInfo->u4DRMRentalCount = prAviInfo->rDivXDrmInfo.prDivxDrmBasicInfo->ui1_use_count;
                LOG(0, "DIVX_DRM_AUTH_ERROR\n");
            }
            else if (prAviInfo->rDivXDrmInfo.prDivxDrmBasicInfo->ui1_flag & DIVX_DRM_RENTAL_EXPIRED)
            {
                prAviInfo->eDRMType = PLAYMGR_DRMTYPE_RENTAL_EXPIRED;
                prAviInfo->u4DRMRentalLimit = prAviInfo->rDivXDrmInfo.prDivxDrmBasicInfo->ui1_use_limit;
                prAviInfo->u4DRMRentalCount = prAviInfo->rDivXDrmInfo.prDivxDrmBasicInfo->ui1_use_count;
                LOG(0, "DIVX_DRM_RENTAL_EXPIRED\n");
            }
            else if (prAviInfo->rDivXDrmInfo.prDivxDrmBasicInfo->ui1_flag & DIVX_DRM_RENTAL)
            {
                prAviInfo->eDRMType = PLAYMGR_DRMTYPE_RENTAL;
                prAviInfo->u4DRMRentalLimit = prAviInfo->rDivXDrmInfo.prDivxDrmBasicInfo->ui1_use_limit;
                prAviInfo->u4DRMRentalCount = prAviInfo->rDivXDrmInfo.prDivxDrmBasicInfo->ui1_use_count;
                LOG(0, "DIVX_DRM_RENTAL\n");
            }

            i4Ret = x_drm_get_info(prAviInfo->hDivXDrmHandle, DRM_GET_TYPE_OUTPUT_SIGNAL_PROTECTION_INFO, &rDivxDrmInfo, sizeof(DIVX_DRM_UNION_INFO_T));
            if (i4Ret != DRMR_OK)
            {
                LOG(0, "x_drm_get_info error\n");
                _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, MINFO_NFY_EVENT_ERROR);
                return;
            }

            if (!rDivxDrmInfo.pt_drm_output_signal_protection_info)
            {
                LOG(0, "rDivxDrmInfo.pt_drm_output_signal_protection_info null pointer\n");
                _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, MINFO_NFY_EVENT_ERROR);
                return;
            }
            
            prAviInfo->rDivXDrmInfo.prDrmOutputSignalProtectionInfo = 
                rDivxDrmInfo.pt_drm_output_signal_protection_info;

            LOG(0, "DRM cgmsa_flag = %d\n", prAviInfo->rDivXDrmInfo.prDrmOutputSignalProtectionInfo->ui1_cgmsa_flag);
            LOG(0, "DRM acptb_flag = %d\n", prAviInfo->rDivXDrmInfo.prDrmOutputSignalProtectionInfo->ui1_acptb_flag);
            LOG(0, "DRM digital_protection_flag = %d\n", prAviInfo->rDivXDrmInfo.prDrmOutputSignalProtectionInfo->ui1_digital_protection_flag);

#if 0  // moved to _PLAYMGR_AviStart below
            i4Ret = x_drm_set_info(prAviInfo->hDivXDrmHandle, DRM_SET_TYPE_DIVX_DRM_PLAYBACK, NULL, NULL);
            if (i4Ret != DRMR_OK)
            {
                LOG(0, "x_drm_set_info error code = %d\n", i4Ret);
                _PLAYMGR_SendNotify(MINFO_NFY_EVENT_ERROR);
                return;
            }    
        
            i4Ret = x_drm_get_info(prAviInfo->hDivXDrmHandle, DRM_GET_TYPE_DIVX_DRM_CRYPTO_INFO, &rDivxDrmInfo, sizeof(DIVX_DRM_UNION_INFO_T));
            if (i4Ret != DRMR_OK)
            {
                LOG(0, "x_drm_set_info error code = %d\n", i4Ret);
                _PLAYMGR_SendNotify(MINFO_NFY_EVENT_ERROR);
                return;
            }

            if (!rDivxDrmInfo.pt_divx_drm_crypto_info)
            {
                LOG(0, "rDivxDrmInfo.pt_divx_drm_crypto_info null pointer\n");
                _PLAYMGR_SendNotify(MINFO_NFY_EVENT_ERROR);
                return;
            }
            
            prAviInfo->rDivXDrmInfo.prDrmCryptoInfo = rDivxDrmInfo.pt_divx_drm_crypto_info;
#endif
        }
        else
#endif
        {
            prAviInfo->eDRMType = PLAYMGR_DRMTYPE_NONE;
        }

        // get meta data
        x_memset(&rMinfoRec, 0, zRecLength);
        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_META_DATA, 1, (VOID*)&rMinfoRec, &zRecLength);
        if (i4Ret != MINFOR_OK)
        {
            LOG(1, "get meta data error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }
        x_memcpy(&prAviInfo->rAviMetaData, &(rMinfoRec.u.t_meta_data), sizeof(MINFO_TYPE_META_DATA_T));

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

static VOID _PLAYMGR_MIdx_Notify(
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
}
#endif

BOOL _PLAYMGR_AviStart(VOID* pvMediaInfo)
{
    INT32 i4Ret;
#ifndef __MODEL_slt__
    UINT8 u1KeyExtCnt;
    UINT8 u1MaxAudCnt = 0;
    UINT32 u4Msg;
    SIZE_T zSize;
#endif
    
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_AVI_INFO_T* prAviInfo = 
    	(PLAYMGR_AVI_INFO_T*)&prMediaInfo->FmtInfo.rAviInfo;
    
#ifndef __MODEL_slt__
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prAviInfo->rSrcInfo;
    MIDXBULD_KEY_TBL_INFO_T* prTableInfo = &prAviInfo->rTableInfo;
    MIDXBULD_KEY_TBL_EXT_T*  prKeyTblList;
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt; 
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prAviInfo->rKeyInfo;
    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTbl = &prAviInfo->rKeyIdxTBL;
    MM_RANGE_ELMT_IDXTBL_T* prAudKeyIdxTbl = &prAviInfo->rAudKeyIdxTBL[0];
    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTblExt = &prAviInfo->rKeyIdxTBLExt[0];
    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTblTmp;
#endif

    if (prMediaInfo == 0)
    {
        LOG(1, "_PLAYMGR_AviStart prMediaInfo == 0\n");
        return FALSE;
    }

    /*if (prMediaInfo->rPlayerStatus.fgEnSubStrm &&
        prMediaInfo->u2SubStmNum > 0)
    {
        fgRet = SWDMX_SetStrmID(prMediaInfo->rPlayerStatus.u4PlaySubId, 
                                                    0, eSWDMX_STRM_TYPE_SUBTITLE); 
        if (fgRet == FALSE)
        {
            LOG(1, "_PLAYMGR_AviStart Fail to set subtitle stream ID\n");
            return FALSE;
        }
        SWDMX_SetInfo(eSWDMX_ENABLE_STREAM, eSWDMX_STRM_TYPE_SUBTITLE, 
            (UINT32)_PLAYMGR_SubtitleCb, 
            0);
    }*/


    /*prMediaInfo->rRangeInfo[0].pv_tag = (void*)prMediaInfo->u4RangeID++;
    fgRet = SWDMX_AddRange((void*)&prMediaInfo->rRangeInfo[0], 
                                                                (UINT32)prMediaInfo->rRangeInfo[0].pv_tag);*/

#ifdef EN_PLAYMGR_AVI_DRM  // moved from _PLAYMGR_Minfo_Notify above
    if (prAviInfo->rDivXDrmInfo.u4DrmType == DRM_TYPE_DIVX_DRM)
    {
        DIVX_DRM_UNION_INFO_T rDivxDrmInfo;
        
        i4Ret = x_drm_get_info(prAviInfo->hDivXDrmHandle, DRM_GET_TYPE_DIVX_DRM_BASIC_INFO, &rDivxDrmInfo, sizeof(DIVX_DRM_UNION_INFO_T));
        if (i4Ret != DRMR_OK)
        {
            LOG(0, "x_drm_get_info error\n");
            _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, MINFO_NFY_EVENT_ERROR);
            return FALSE;
        }

        i4Ret = x_drm_set_info(prAviInfo->hDivXDrmHandle, DRM_SET_TYPE_DIVX_DRM_PLAYBACK, NULL, NULL);
        if (i4Ret != DRMR_OK)
        {
            LOG(0, "x_drm_set_info error code = %d\n", i4Ret);
            _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, MINFO_NFY_EVENT_ERROR);
            return FALSE;
        }    
        
        i4Ret = x_drm_get_info(prAviInfo->hDivXDrmHandle, DRM_GET_TYPE_DIVX_DRM_CRYPTO_INFO, &rDivxDrmInfo, sizeof(DIVX_DRM_UNION_INFO_T));
        if (i4Ret != DRMR_OK)
        {
            LOG(0, "x_drm_set_info error code = %d\n", i4Ret);
            _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, MINFO_NFY_EVENT_ERROR);
            return FALSE;
        }

        if (!rDivxDrmInfo.pt_divx_drm_crypto_info)
        {
            LOG(0, "rDivxDrmInfo.pt_divx_drm_crypto_info null pointer\n");
            _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, MINFO_NFY_EVENT_ERROR);
            return FALSE;
        }
            
        prAviInfo->rDivXDrmInfo.prDrmCryptoInfo = rDivxDrmInfo.pt_divx_drm_crypto_info;
    }
#endif
            
    //SWDMX_DelRange(NULL, (UINT32)prMediaInfo->rRangeInfo[0].ui4_range_id);
    //SWDMX_DelRange(NULL, (UINT32)prMediaInfo->rRangeInfo[1].ui4_range_id);
#ifndef __MODEL_slt__    
    if ((!prAviInfo->hMediaIdxHandle) &&
        !(prAviInfo->rPlayerIdxInfo.ui4_idx_type & MM_MINFO_IDX_TYPE_NONE)) 
    {
        LOG(1, "x_midxbuld_mopen\n");
    i4Ret = x_midxbuld_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _PLAYMGR_MIdx_Notify, prMediaInfo,
                                prMediaFmt, prSrcInfo, prKeyInfo, &prAviInfo->hMediaIdxHandle);
        if(i4Ret != MINFOR_OK)
        {
            return FALSE;
        }

        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (VOID*)&u4Msg);
        if (u4Msg != MIDXBULD_NFY_EVENT_INDEXING_DONE)
        {
            return FALSE;
        }
        else
        {
            zSize = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
            prTableInfo->ui4_key_tbl_id = prKeyInfo->ui4_tbl_id;
            i4Ret = x_midxbuld_get_info(prAviInfo->hMediaIdxHandle, MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST,
            	(VOID*)prTableInfo, &zSize);
            if (i4Ret)
            {
                LOG(1, "get midxbuild elmt tbl error\n");
            }       

            //video key table
            u1KeyExtCnt = 0;
            prKeyTblList = prTableInfo->pt_key_tbl_ext;
            prKeyIdxTbl->e_elem_type = MM_ELEMT_TYPE_KEY;
            prKeyIdxTbl->t_strm_id.u.t_stm_avi_id = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
            prKeyIdxTbl->ui4_number_of_entry = prKeyTblList->t_elmt_tbl.ui4_number_of_entry;
            prKeyIdxTbl->ui8_base_offset = prKeyTblList->t_elmt_tbl.ui8_base_offset;
            prKeyIdxTbl->u.pt_pb_idx_tbl_entry = 
                (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)prKeyTblList->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
            if (prKeyTblList->t_elmt_tbl.pt_next != NULL)
            {
                MIDXBULD_ELMT_TBL_OBJ_T *pt_elmt_tbl = prKeyTblList->t_elmt_tbl.pt_next;
                prKeyIdxTblTmp = prKeyIdxTbl;
                while(pt_elmt_tbl)
                {
                    if(u1KeyExtCnt >= AVI_MAX_KEY_EXT)
                    {
                        LOG(1, "KeyExt Num >= AVI_MAX_KEY_EXT\n");
                        break;
                    }
                    prKeyIdxTblTmp->pt_next_partial = prKeyIdxTblExt;
                    prKeyIdxTblTmp = prKeyIdxTblTmp->pt_next_partial;
                    prKeyIdxTblTmp->e_elem_type = MM_ELEMT_TYPE_KEY;
                    prKeyIdxTblTmp->t_strm_id.u.t_stm_mkv_id = pt_elmt_tbl->u_fmt.t_es.t_info.ui4_strm_id;
                    prKeyIdxTblTmp->ui4_number_of_entry = pt_elmt_tbl->ui4_number_of_entry;
                    prKeyIdxTblTmp->ui8_base_offset = pt_elmt_tbl->ui8_base_offset;
                    prKeyIdxTblTmp->u.pt_pb_idx_tbl_entry = 
                        (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)pt_elmt_tbl->u_fmt.t_es.pt_idx_entry;
                    prKeyIdxTblTmp->ui4_rate = pt_elmt_tbl->u_fmt.t_es.t_info.ui4_rate;
                    prKeyIdxTblTmp->ui4_scale = pt_elmt_tbl->u_fmt.t_es.t_info.ui4_scale;

                    prKeyIdxTblExt++;
                    u1KeyExtCnt++;
                    pt_elmt_tbl = pt_elmt_tbl->pt_next;
                }//--key table next partial
            }
            
            prKeyTblList = prKeyTblList->pt_next;   //key tbl info from mutil
            
            //audio key table
            if (prMediaInfo->u2AudStmNum > 0)
            {
                while ( prKeyTblList!= NULL)           //key tbl info from mutil
                {
                    u1KeyExtCnt = 0;
                    if(u1MaxAudCnt >= AVI_MAX_AUD_TRACK)
                    {
                        LOG(1, "u1MaxAudCnt Num >= AVI_MAX_AUD_TRACK\n");
                        break;
                    }
                    prKeyIdxTblExt = &prAviInfo->rAudKeyIdxTBLExt[u1MaxAudCnt][u1KeyExtCnt];
                    prAudKeyIdxTbl->e_elem_type = MM_ELEMT_TYPE_KEY;
                    prAudKeyIdxTbl->t_strm_id.u.t_stm_avi_id = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
                    prAudKeyIdxTbl->ui4_number_of_entry = prKeyTblList->t_elmt_tbl.ui4_number_of_entry;
                    prAudKeyIdxTbl->ui8_base_offset = prKeyTblList->t_elmt_tbl.ui8_base_offset;
                    prAudKeyIdxTbl->ui4_scale = prKeyTblList->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_scale;
                    prAudKeyIdxTbl->ui4_rate = prKeyTblList->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_rate;
                    prAudKeyIdxTbl->ui4_avg_bytes_per_sec = prKeyTblList->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_avg_bytes_per_sec;
                    prAudKeyIdxTbl->b_is_VBR = 
                        (prKeyTblList->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_sample_sz == 0) ? TRUE : FALSE;
                    prAudKeyIdxTbl->u.pv_idx_tbl_entry = 
                        (void*)prKeyTblList->t_elmt_tbl.u_fmt.t_as_key.pui4_pts;
                    if (prKeyTblList->t_elmt_tbl.pt_next != NULL)
                    {
                        MIDXBULD_ELMT_TBL_OBJ_T *pt_elmt_tbl = prKeyTblList->t_elmt_tbl.pt_next;
                        prKeyIdxTblTmp = prAudKeyIdxTbl;
                        while(pt_elmt_tbl)
                        {
                            if(u1KeyExtCnt >= AVI_MAX_KEY_EXT)
                            {
                                LOG(1, "KeyExt Num >= AVI_MAX_KEY_EXT\n");
                                break;
                            }
                            prKeyIdxTblTmp->pt_next_partial = prKeyIdxTblExt;
                            prKeyIdxTblTmp = prKeyIdxTblTmp->pt_next_partial;
                            prKeyIdxTblTmp->e_elem_type = MM_ELEMT_TYPE_KEY;
                            prKeyIdxTblTmp->t_strm_id.u.t_stm_mkv_id = pt_elmt_tbl->u_fmt.t_es.t_info.ui4_strm_id;
                            prKeyIdxTblTmp->ui4_number_of_entry = pt_elmt_tbl->ui4_number_of_entry;
                            prKeyIdxTblTmp->ui8_base_offset = pt_elmt_tbl->ui8_base_offset;
                            prKeyIdxTblTmp->u.pt_pb_idx_tbl_entry = 
                                (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)pt_elmt_tbl->u_fmt.t_es.pt_idx_entry;
                            prKeyIdxTblTmp->ui4_rate = pt_elmt_tbl->u_fmt.t_es.t_info.ui4_rate;
                            prKeyIdxTblTmp->ui4_scale = pt_elmt_tbl->u_fmt.t_es.t_info.ui4_scale;

                            prKeyIdxTblExt++;
                            u1KeyExtCnt++;
                            pt_elmt_tbl = pt_elmt_tbl->pt_next;
                        }//--key table next partial
                    }
                    prKeyTblList = prKeyTblList->pt_next;
                    prAudKeyIdxTbl++;
                }
            }
         }
        //build two interval of indx table at beginning
        LOG(1, "start build idx\n");
        prMediaInfo->u1CurEmptyRange = 0;
        prMediaInfo->u4CurRangePTS = 0;
        _PLAYMGR_AviUpdateRange(pvMediaInfo);
    }
    else
    {
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
    }
#else

    prMediaInfo->u1CurEmptyRange = 0;
    prMediaInfo->u4CurRangePTS = 0;
    _PLAYMGR_AviUpdateRange(pvMediaInfo);
#endif
#if 0
    fgRet = _SWDMX_Play();
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_AviStart Fail to play\n");
        return FALSE;
    }
#endif

    return TRUE;
}


BOOL _PLAYMGR_AviStop(VOID* pvMediaInfo)
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

EXTERN BOOL _PLAYMGR_AviClose(VOID* pvMediaInfo)
{
    BOOL fgRet = TRUE;
    UINT32 u4Msg;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    if (prMediaInfo->FmtInfo.rAviInfo.hMediaInfoHandle)
    {
        x_minfo_close(prMediaInfo->FmtInfo.rAviInfo.hMediaInfoHandle);
        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (VOID*)&u4Msg);
        if (u4Msg != MINFO_NFY_EVENT_HANDLE_CLOSED)
        {
            LOG(1, "x_minfo_close failed\n");
            fgRet = FALSE;
        }
    }
    if (prMediaInfo->FmtInfo.rAviInfo.hMediaIdxHandle)
    {
        x_midxbuld_close(prMediaInfo->FmtInfo.rAviInfo.hMediaIdxHandle);
        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (VOID*)&u4Msg);
        if (u4Msg != MIDXBULD_NFY_EVENT_HANDLE_CLOSED)
        {
            LOG(1, "x_midxbuld_close failed");
            fgRet = FALSE;
        }
    }
#ifdef EN_PLAYMGR_AVI_DRM
    if (prMediaInfo->FmtInfo.rAviInfo.hDivXDrmHandle)
    {
        INT32 i4Ret;
        i4Ret = x_drm_close (prMediaInfo->FmtInfo.rAviInfo.hDivXDrmHandle);
        if (i4Ret != DRMR_OK)
        {
            LOG(1, "x_drm_close failed, code = %d\r\n", i4Ret);
            fgRet = FALSE;
        }
        prMediaInfo->FmtInfo.rAviInfo.rDivXDrmInfo.u4DrmType = DRM_TYPE_NONE;
        prMediaInfo->FmtInfo.rAviInfo.hDivXDrmHandle = NULL;
    }
#endif

    x_memset(&prMediaInfo->FmtInfo.rAviInfo, 0, sizeof(PLAYMGR_AVI_INFO_T));

    return fgRet;
}

EXTERN BOOL _PLAYMGR_AviResume(VOID* pvMediaInfo)
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

BOOL _PLAYMGR_AviPause(VOID)
{
#if 0
    return _SWDMX_Pause();
#endif
    return TRUE;
}


BOOL _PLAYMGR_OpenAviMediaInfo(PLAYMGR_STATUS_T* prMediaStatus, VOID* pvMediaInfo)
{
#ifndef __MODEL_slt__
    INT32 i4Ret;
    UINT32 u4Msg;
#endif
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    PLAYMGR_AVI_INFO_T* prAviInfo = 
    	(PLAYMGR_AVI_INFO_T*)&prMediaInfo->FmtInfo.rAviInfo;
#ifndef __MODEL_slt__
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt; 
#endif
    //MIDXBULD_KEY_INFO_T* prKeyInfo = &prAviInfo->rKeyInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prAviInfo->rSrcInfo;
    //MIDXBULD_ELMT_TBL_INFO_T* prTableInfo = &prAviInfo->rTableInfo;
    //MIDXBULD_ELMT_TBL_ALLOC_INFO_T* prAllocTblInfo = &prAviInfo->rAllocTblInfo;
    MIDXBULD_ELMT_TBL_DESCRIB_T* prVDescrib = &prAviInfo->rVDescrib[0];
    MIDXBULD_ELMT_TBL_DESCRIB_T* prADescrib = &prAviInfo->rADescrib[0];
    //MIDXBULD_ELMT_TBL_DESCRIB_T* prFirstDescrib = NULL;

    MM_RANGE_ELMT_IDXTBL_T* prVidElmtStrm = &prAviInfo->rVidIDXTBL[0];
    MM_RANGE_ELMT_IDXTBL_T* prAudElmtStrm = &prAviInfo->rAudIDXTBL[0];

    //SIZE_T zSize;

    //prMediaInfo->u1CurEmptyRange = 1;


#ifndef __MODEL_slt__
    // Open for media info
    i4Ret = x_minfo_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _PLAYMGR_Minfo_Notify, prMediaInfo,
            		       prMediaFmt, &(prAviInfo->hMediaInfoHandle));
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
    prSrcInfo->ui8_data_offset = prAviInfo->rPlayerIdxInfo.ui8_data_offset;
    prSrcInfo->ui8_data_size = prAviInfo->rPlayerIdxInfo.ui8_data_sz;
    prSrcInfo->ui8_idx_offset = prAviInfo->rPlayerIdxInfo.ui8_idx_offset;
    prSrcInfo->ui8_idx_size = prAviInfo->rPlayerIdxInfo.ui8_idx_sz;
    prSrcInfo->ui4_idx_type = prAviInfo->rPlayerIdxInfo.ui4_idx_type;
    prSrcInfo->ui1_total_strm_num = (UINT8)(prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum);
    // Temporally multiply SYSTEM_TIME_SCALE
    prSrcInfo->ui4_total_duration = (UINT32)prAviInfo->rPlayerGenericInfo.ui8_pb_duration;// * SYSTEM_TIME_SCALE;
    prMediaStatus->u4TotalDuration = (UINT32)prAviInfo->rPlayerGenericInfo.ui8_pb_duration;
    prMediaInfo->rPlayerStatus.u4TotalDuration = (UINT32)prAviInfo->rPlayerGenericInfo.ui8_pb_duration;
#else
    // Wait for media info's notify
    prSrcInfo->ui8_data_offset = 2056;
    prSrcInfo->ui8_data_size = 523020854;
    prSrcInfo->ui8_idx_offset = 523022910;
    prSrcInfo->ui8_idx_size = 205504;
    prSrcInfo->ui4_idx_type = prAviInfo->rPlayerIdxInfo.ui4_idx_type;
    prSrcInfo->ui1_total_strm_num = 2;
    // Temporally multiply SYSTEM_TIME_SCALE
    prSrcInfo->ui4_total_duration = (UINT32)19287000;
    prMediaStatus->u4TotalDuration = (UINT32)19287000;
    prMediaInfo->rPlayerStatus.u4TotalDuration = (UINT32)19287000;
#endif

    //SWDMX_SetInfo(eSWDMX_TOTAL_DURATION, prSrcInfo->ui4_total_duration, 0, 0);
        
    if (_PLAYMGR_SetAVIInfo(prMediaStatus, prMediaInfo) != TRUE)
    {
        return FALSE;
    }

#ifndef __MODEL_slt__
    prMediaInfo->rRangeInfo[0].ui8_fileoffset = prSrcInfo->ui8_data_offset;
    prMediaInfo->rRangeInfo[0].z_range_sz = prSrcInfo->ui8_data_size;
#else
    prMediaInfo->rRangeInfo[0].ui8_fileoffset = 2056;
    prMediaInfo->rRangeInfo[0].z_range_sz = 523020854;
#endif

    if ((prMediaInfo->u2VidStmNum > 0) &&
        (prAviInfo->u2VidIdx < 17))
    {
        prMediaInfo->rRangeInfo[0].pt_idxtbl_lst = prVidElmtStrm;

        prVidElmtStrm->e_elem_type = MM_ELEMT_TYPE_VIDEO;
        prVidElmtStrm->t_strm_id.u.t_stm_avi_id = prSrcInfo->t_es_map[prAviInfo->u2VidIdx].ui4_strm_id;
        prVidElmtStrm->ui4_rate = prSrcInfo->t_es_map[prAviInfo->u2VidIdx].ui4_rate;
        prVidElmtStrm->ui4_scale = prSrcInfo->t_es_map[prAviInfo->u2VidIdx].ui4_scale;
        prVidElmtStrm->ui4_avg_bytes_per_sec = 
            prSrcInfo->t_es_map[prAviInfo->u2VidIdx].ui4_avg_bytes_per_sec;
        prVidElmtStrm->ui4_number_of_entry = 
            prAviInfo->rStmAttribute[prAviInfo->u2VidIdx].u.t_avi_video_stm_attr.ui4_length;

        prVDescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        prVDescrib->u_info.t_es.ui4_strm_id = 
            prSrcInfo->t_es_map[prAviInfo->u2VidIdx].ui4_strm_id;
        prVDescrib->u_info.t_es.ui4_scale = 
            prSrcInfo->t_es_map[prAviInfo->u2VidIdx].ui4_scale;
        prVDescrib->u_info.t_es.ui4_rate = 
            prSrcInfo->t_es_map[prAviInfo->u2VidIdx].ui4_rate;
        prVDescrib->u_info.t_es.ui4_sample_sz = 0;
        prVDescrib->u_info.t_es.ui4_avg_bytes_per_sec = 0;

        prVDescrib->u_info.t_es.ui4_indx_offset = 
            prSrcInfo->t_es_map[prAviInfo->u2VidIdx].ui4_indx_offset;
        prVDescrib->u_info.t_es.ui4_indx_size = 
            prSrcInfo->t_es_map[prAviInfo->u2VidIdx].ui4_indx_size;
    }
    
    if ((prMediaInfo->u2AudStmNum > 0) &&
        (prAviInfo->u2AudIdx < 17))
    {
        (prMediaInfo->u2VidStmNum > 0) ? (prVidElmtStrm->pv_next_tbl = prAudElmtStrm)
        : (prMediaInfo->rRangeInfo[0].pt_idxtbl_lst = prAudElmtStrm);

        prAudElmtStrm->e_elem_type = MM_ELEMT_TYPE_AUDIO;
        prAudElmtStrm->t_strm_id.u.t_stm_avi_id = prSrcInfo->t_es_map[prAviInfo->u2AudIdx].ui4_strm_id;
        prAudElmtStrm->ui4_rate = prSrcInfo->t_es_map[prAviInfo->u2AudIdx].ui4_rate;
        prAudElmtStrm->ui4_scale = prSrcInfo->t_es_map[prAviInfo->u2AudIdx].ui4_scale;
        prAudElmtStrm->ui4_avg_bytes_per_sec = 
            prSrcInfo->t_es_map[prAviInfo->u2AudIdx].ui4_avg_bytes_per_sec;
        prAudElmtStrm->ui4_number_of_entry = 
            prAviInfo->rStmAttribute[prAviInfo->u2AudIdx].u.t_avi_video_stm_attr.ui4_length;
   
        prADescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        prADescrib->u_info.t_es.ui4_strm_id = 
        	prSrcInfo->t_es_map[prAviInfo->u2AudIdx].ui4_strm_id;
        prADescrib->u_info.t_es.ui4_scale = 
        	prSrcInfo->t_es_map[prAviInfo->u2AudIdx].ui4_scale;
        prADescrib->u_info.t_es.ui4_rate = 
        	prSrcInfo->t_es_map[prAviInfo->u2AudIdx].ui4_rate;
        prADescrib->u_info.t_es.ui4_sample_sz = 
        	prSrcInfo->t_es_map[prAviInfo->u2AudIdx].ui4_sample_sz ;
        prADescrib->u_info.t_es.ui4_avg_bytes_per_sec = 
        	prSrcInfo->t_es_map[prAviInfo->u2AudIdx].ui4_avg_bytes_per_sec;

        prADescrib->u_info.t_es.ui4_indx_offset = 
            prSrcInfo->t_es_map[prAviInfo->u2AudIdx].ui4_indx_offset;
        prADescrib->u_info.t_es.ui4_indx_size = 
            prSrcInfo->t_es_map[prAviInfo->u2AudIdx].ui4_indx_size;

    }

    return TRUE;
}


BOOL _PLAYMGR_AviUpdateRange(VOID* pvMediaInfo)
{
    //INT32 i4RangeIdx;
    //UINT32 u4Msg;
#ifndef __MODEL_slt__
    INT32 i;
    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTbl = NULL;
    MM_RANGE_ELMT_IDXTBL_T* prAudKeyIdxTbl = NULL;
#endif
    MM_RANGE_ELMT_IDXTBL_T* prVidElmtStrm = NULL;
    MM_RANGE_ELMT_IDXTBL_T* prAudElmtStrm = NULL;
#ifndef __MODEL_slt__
    MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
#endif
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_AVI_INFO_T* prAviInfo = 
    	(PLAYMGR_AVI_INFO_T*)&prMediaInfo->FmtInfo.rAviInfo;

    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prAviInfo->rSrcInfo;

    //i4RangeIdx = prMediaInfo->u1CurEmptyRange;

#ifndef __MODEL_slt__
    if (&prAviInfo->rKeyIdxTBL)
    {
        prKeyIdxTbl = &prAviInfo->rKeyIdxTBL;
    }
    else
    {
        return FALSE;
    }

    if (&prAviInfo->rAudKeyIdxTBL[0])
    {
        prAudKeyIdxTbl = &prAviInfo->rAudKeyIdxTBL[0];
    }
    else
    {
        return FALSE;
    }

    if (&prAviInfo->rVidIDXTBL[0])
    {
        prVidElmtStrm = &prAviInfo->rVidIDXTBL[0];
    }
    else
    {
        return FALSE;
    }

    if (&prAviInfo->rAudIDXTBL[0])
    {
        prAudElmtStrm = &prAviInfo->rAudIDXTBL[0];
    }
    else
    {
        return FALSE;
    }
    
    // link key index to the end of the list
    if (prMediaInfo->rRangeInfo[0].pt_idxtbl_lst)
    {
        prIdxTbl = prMediaInfo->rRangeInfo[0].pt_idxtbl_lst;
    }
    else 
    {
        return FALSE;
    }
    
    while (prIdxTbl->pv_next_tbl != NULL)
    {
        prIdxTbl = prIdxTbl->pv_next_tbl;
    }

    // video key table
    if (prKeyIdxTbl != NULL &&
        prKeyIdxTbl->ui4_number_of_entry > 0)
    {
        LOG(5, "The entry of video key table = 0x%08x\n", prKeyIdxTbl->ui4_number_of_entry);
        prKeyIdxTbl->t_strm_id.u.t_stm_avi_id = prVidElmtStrm->t_strm_id.u.t_stm_avi_id;
        prIdxTbl->pv_next_tbl = prKeyIdxTbl;
        prIdxTbl = prIdxTbl->pv_next_tbl;
    }

    if ((prMediaInfo->u2AudStmNum > 0) &&
        (prAudKeyIdxTbl != NULL) &&
        (prAudKeyIdxTbl->ui4_number_of_entry > 0))
    {
        for (i = 0; i < prMediaInfo->u2AudStmNum; i++)
        {
            LOG(5, "Audio key table is not NULL\n");
            prIdxTbl->pv_next_tbl = prAudKeyIdxTbl;
            prIdxTbl = prIdxTbl ->pv_next_tbl;
            prAudKeyIdxTbl++;
        }
    }
#if 0
    // audio key table
    while(prAudKeyIdxTbl != NULL)
    {
        LOG(5, "Audio key table is not NULL\n");
        prIdxTbl->pv_next_tbl = prAudKeyIdxTbl;
        prIdxTbl = prIdxTbl ->pv_next_tbl;
        prAudKeyIdxTbl = prAudKeyIdxTbl->pv_next_tbl;
    }
#endif

#ifdef EN_PLAYMGR_AVI_DRM
    if (prMediaInfo->FmtInfo.rAviInfo.rDivXDrmInfo.u4DrmType == DRM_TYPE_DIVX_DRM)
    {
        prMediaInfo->rRangeInfo[0].b_drm = TRUE;
        prMediaInfo->rRangeInfo[0].t_drm_info_drv.t_drm_type = DRM_TYPE_DIVX_DRM;
        prMediaInfo->rRangeInfo[0].t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info = 
            VIRTUAL(prMediaInfo->FmtInfo.rAviInfo.rDivXDrmInfo.prDrmCryptoInfo);
    }
#endif

#endif

    prMediaInfo->rRangeInfo[0].ui4_range_id = 
        prAviInfo->rAllocTblInfo[0].ui4_range_id;

    LOG(3, "Allocate ui4_range_id = %ld, pv_tag = %ld\n", 
        prAviInfo->rAllocTblInfo[0].ui4_range_id,
        prMediaInfo->rRangeInfo[0].ui4_range_id);

    if((prVidElmtStrm->ui4_number_of_entry > 0) ||
        (prAudElmtStrm->ui4_number_of_entry > 0))
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


BOOL _PLAYMGR_AviFreeRange(VOID* pvMediaInfo, UINT32 u4RangeTag)
{
    /*INT32 i, i4Ret;
    SIZE_T zSize;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_AVI_INFO_T* prAviInfo = 
    	(PLAYMGR_AVI_INFO_T*)&prMediaInfo->FmtInfo.rAviInfo;

    for (i = 0; i < MAX_RANGE_NUM; i++)
    {
        if (prMediaInfo->rRangeInfo[i].ui4_range_id == u4RangeTag)
        {
            zSize = sizeof(UINT32);
            LOG(3, "free range prMediaInfo->rRangeInfo[i].pv_tag = %ld\n", prMediaInfo->rRangeInfo[i].ui4_range_id);
            LOG(3, "free range prAviInfo->rAllocTblInfo[i].ui4_range_id = %ld\n", prAviInfo->rAllocTblInfo[i].ui4_range_id);
            i4Ret = x_midxbuld_set_info(prMediaInfo->FmtInfo.rAviInfo.hMediaIdxHandle, 
                                 MIDXBULD_SET_INFO_TYPE_FREE_RANGE, 
                                 (VOID*)prAviInfo->rAllocTblInfo[i].ui4_range_id, 
                                 &zSize);
            prMediaInfo->u1CurEmptyRange = i;
            x_memset(&prMediaInfo->rRangeInfo[i], 0, sizeof(MM_RANGE_INFO_T));
        }
    }
    UNUSED(i4Ret);*/
    return TRUE;
}


BOOL _PLAYMGR_AviGetDivxDRMStatus(UINT8 u1SrcId, PLAYMGR_DRMTYPE *peDRMType, UINT32 *pu4DRMRentalLimit, UINT32 *pu4DRMRentalCount)
{
#ifndef EN_PLAYMGR_AVI_DRM
    UNUSED(u1SrcId);
    UNUSED(peDRMType);
    UNUSED(pu4DRMRentalLimit);
    UNUSED(pu4DRMRentalCount);
#else
    PLAYMGR_INFO_T *prPlaymgrInst=_PLAYMGR_GetInst(u1SrcId);

    if(!prPlaymgrInst)
    {
        LOG(3, "_PLAYMGR_ChkSubtitle(%d), prPlaymgrInst null\n", u1SrcId);
        return FALSE;
    }

    *peDRMType = prPlaymgrInst->FmtInfo.rAviInfo.eDRMType;
    *pu4DRMRentalLimit = prPlaymgrInst->FmtInfo.rAviInfo.u4DRMRentalLimit;
    *pu4DRMRentalCount = prPlaymgrInst->FmtInfo.rAviInfo.u4DRMRentalCount;
#endif
    return TRUE;
}


BOOL _PLAYMGR_AviGetDivxDRMRegCode(char *pu1Code)
{
#ifndef EN_PLAYMGR_AVI_DRM
    UNUSED(pu1Code);
#else
    INT32 i4Ret;

    i4Ret = x_drm_get_generic_info(DRM_TYPE_DIVX_DRM, DRM_GET_TYPE_DIVX_DRM_GENERIC_REGISTRATION_INFO, 
        VIRTUAL(pu1Code), sizeof(DIVX_DRM_UNION_GENERIC_INFO_T));
    if (i4Ret != DRMR_OK)
    {
        LOG(1, "Get Registration code failed: %d\n", i4Ret);
        return FALSE;
    }

    pu1Code[10] = 0;
    LOG(0, "Registration Code = %s\n", pu1Code);
#endif
    return TRUE;
}


BOOL _PLAYMGR_AviGetDivxDRMUIHelpInfo(UINT32 *pu4Info)
{
#ifndef EN_PLAYMGR_AVI_DRM
    UNUSED(pu4Info);
#else
    INT32 i4Ret;

    i4Ret = x_drm_get_generic_info(DRM_TYPE_DIVX_DRM, DRM_GET_TYPE_DIVX_DRM_GENERIC_UI_HELP_INFO, 
        VIRTUAL(pu4Info), sizeof(DIVX_DRM_UNION_GENERIC_INFO_T));
    if (i4Ret != DRMR_OK)
    {
        LOG(1, "Get UI help info failed: %d\n", i4Ret);
        return FALSE;
    }

    LOG(0, "UI help info = %d\n", *pu4Info);
#endif
    return TRUE;
}


BOOL _PLAYMGR_AviDivxDRMDeactivation(char *pu1Code)
{
#ifndef EN_PLAYMGR_AVI_DRM
    UNUSED(pu1Code);
#else
    INT32 i4Ret;

    //i4Ret = x_drm_get_generic_info(DRM_TYPE_DIVX_DRM, DRM_SET_TYPE_DIVX_DRM_GENERIC_DEACTIVATION, 
    //    VIRTUAL(pu1Code), sizeof(DIVX_DRM_UNION_GENERIC_INFO_T));
    i4Ret = x_drm_set_generic_info(DRM_TYPE_DIVX_DRM, DRM_SET_TYPE_DIVX_DRM_GENERIC_DEACTIVATION, 
        VIRTUAL(pu1Code), sizeof(DIVX_DRM_UNION_GENERIC_INFO_T));
    if (i4Ret != DRMR_OK)
    {
        LOG(1, "Deactivation failed: %d\n", i4Ret);
        return FALSE;
    }

    pu1Code[8] = 0;
    LOG(0, "Deactivation Code = %s\n", pu1Code);
#endif
    return TRUE;
}


#endif // !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)




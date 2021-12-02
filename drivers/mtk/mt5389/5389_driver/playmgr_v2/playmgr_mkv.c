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
 * $RCSfile: playmgr_mkv.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

#if !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)

#include "playmgr.h"
#include "feeder_if.h"
#include "playmgr_util.h"
#include "swdmx_drvif.h"
#include "playmgr_mkv.h"
#include "playmgr_debug.h"


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static BOOL _SetMKVInfo(PLAYMGR_STATUS_T* prMediaStatus, PLAYMGR_INFO_T* prMediaInfo)
{
#ifndef __MODEL_slt__
    UINT16 i; 
#endif
    PLAYMGR_MKV_INFO_T* prMkvInfo = &prMediaInfo->FmtInfo.rMkvInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prMkvInfo->rSrcInfo;
#ifndef __MODEL_slt__
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prMkvInfo->rKeyInfo;
#endif
    prMediaStatus->eMediaType = PLAYMGR_MEDIA_TYPE_MKV;

#ifndef __MODEL_slt__
    prMediaStatus->u8FileSize = prMediaInfo->FmtInfo.rMkvInfo.rPlayerGenericInfo.ui8_media_sz;

    prMediaStatus->u2TotalPrmNum = 1;
    prMediaStatus->u2VideoTracks = prMediaInfo->u2VidStmNum;
    
    for(i = prMkvInfo->u2VidIdx; 
        (i < (prMediaInfo->u2VidStmNum + prMkvInfo->u2VidIdx)) && (i<PLAYMGR_MAX_VID_TRACK); 
        i++)
    {
        switch (prMkvInfo->rStmAttribute[i].u.t_mkv_video_stm_attr.e_enc)
        {
            case MINFO_INFO_VID_ENC_MPEG_1: case MINFO_INFO_VID_ENC_MPEG_2:
                prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_MPEG1_MPEG2;
                break;
            case MINFO_INFO_VID_ENC_DIVX_311: 
                prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_DIVX311;
                break;
            case MINFO_INFO_VID_ENC_DIVX_4:
            case MINFO_INFO_VID_ENC_DIVX_5: case MINFO_INFO_VID_ENC_XVID:
            case MINFO_INFO_VID_ENC_MPEG_4:
                prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_MPEG4;
                break;
            case MINFO_INFO_VID_ENC_H264:
                prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_H264;
                break;
            case MINFO_INFO_VID_ENC_WVC1:
                prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_VC1;
                break;
            case MINFO_INFO_VID_ENC_WMV3:
                prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_WMV9;
                break;
            case MINFO_INFO_VID_ENC_VP8:
                prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_VP8;
                break;
            default:
                return FALSE;
        }
    }

    prMediaStatus->u2AudioTracks = prMediaInfo->u2AudStmNum;
    ASSERT((prMediaInfo->u2AudStmNum + prMkvInfo->u2AudIdx) <= 17);
    for(i = prMkvInfo->u2AudIdx; 
        ((i < prMediaInfo->u2AudStmNum + prMkvInfo->u2AudIdx) && 
        ((i - prMediaInfo->u2VidStmNum)<PLAYMGR_MAX_AUD_TRACK)) &&
        (i < 16); 
        i++)
    {
        if (i >= prMediaInfo->u2VidStmNum)
        {
            switch (prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.e_enc)
            {
            case MINFO_INFO_AUD_ENC_MPEG_1: case MINFO_INFO_AUD_ENC_MPEG_2:
                prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = 
                    PLAYMGR_ADEC_MP3;
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
            default:
                prMediaStatus->aeAudFormat[i - prMediaInfo->u2VidStmNum] = 
                    PLAYMGR_ADEC_UNKNOWN;   //not support
                LOG(1, "Aud type %d not support\n", prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.e_enc);
            	  break;
            }
        }
    }

    // Not implement yet
    prMediaStatus->u2CaptionTracks = 0;

    // Backup the media information
    x_memcpy(&prMediaInfo->rPlayerStatus, prMediaStatus, sizeof(PLAYMGR_STATUS_T));

    ASSERT(prMediaInfo->u2VidStmNum <= 17);
    for (i = 0; (i < prMediaInfo->u2VidStmNum) && (i<17); i++)
    {
        prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;

        if (i < PLAYMGR_MAX_VID_TRACK)
        {
            prMediaStatus->au4VideoId[i] = prMkvInfo->rStmAttribute[i].t_stm_id.u.t_mkv_stm_id;
            prMediaInfo->rPlayerStatus.au4VideoId[i] = 
                prMkvInfo->rStmAttribute[i].t_stm_id.u.t_mkv_stm_id;
        }       
        prSrcInfo->t_es_map[i].ui4_strm_id = 
        	prMkvInfo->rStmAttribute[i].t_stm_id.u.t_mkv_stm_id;
        prSrcInfo->t_es_map[i].ui4_scale = 
        	prMkvInfo->rStmAttribute[i].u.t_mkv_video_stm_attr.ui4_scale;
        prSrcInfo->t_es_map[i].ui4_rate = 
        	prMkvInfo->rStmAttribute[i].u.t_mkv_video_stm_attr.ui4_rate;
        //prSrcInfo->t_es_map[i].ui4_sample_sz = 
        //	prMkvInfo->rStmAttribute[i].u.t_mkv_video_stm_attr.ui4_sample_size;
        prSrcInfo->t_es_map[i].ui4_avg_bytes_per_sec = 0;
    }

    ASSERT((prMediaInfo->u2AudStmNum + prMediaInfo->u2VidStmNum) <= 17);
    for (;(i < (prMediaInfo->u2AudStmNum + prMediaInfo->u2VidStmNum)) && (i<17); i++)
    {
        prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;

        if ((i >= prMediaInfo->u2VidStmNum) && ((i - prMediaInfo->u2VidStmNum) < PLAYMGR_MAX_AUD_TRACK))
        {
            prMediaStatus->au4AudioId[i-prMediaInfo->u2VidStmNum] = 
                prMkvInfo->rStmAttribute[i].t_stm_id.u.t_mkv_stm_id;
            prMediaInfo->rPlayerStatus.au4AudioId[i-prMediaInfo->u2VidStmNum] = 
                prMkvInfo->rStmAttribute[i].t_stm_id.u.t_mkv_stm_id;
        }
        else
        {
            LOG(3, "there r too many audio tracks\n");
        }
        
        prSrcInfo->t_es_map[i].ui4_strm_id = 
        	prMkvInfo->rStmAttribute[i].t_stm_id.u.t_mkv_stm_id;
        prSrcInfo->t_es_map[i].ui4_scale = 
        	prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.ui4_scale;
        prSrcInfo->t_es_map[i].ui4_rate = 
        	prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.ui4_rate;
        //prSrcInfo->t_es_map[i].ui4_sample_sz = 
        //	prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.ui4_sample_size;
        prSrcInfo->t_es_map[i].ui4_avg_bytes_per_sec = 
        	prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.ui4_avg_bytes_sec;    
    }

    ASSERT(prMkvInfo->u2VidIdx < 17);
    if(prMkvInfo->u2VidIdx < 17)
    {
        prKeyInfo->ui4_strm_id = prSrcInfo->t_es_map[prMkvInfo->u2VidIdx].ui4_strm_id;
        prKeyInfo->ui4_scale = 
        	prMkvInfo->rStmAttribute[prMkvInfo->u2VidIdx].u.t_mkv_video_stm_attr.ui4_scale;
        prKeyInfo->ui4_rate = 
        	prMkvInfo->rStmAttribute[prMkvInfo->u2VidIdx].u.t_mkv_video_stm_attr.ui4_rate;
        //prKeyInfo->ui4_sample_sz = 
        //	prMkvInfo->rStmAttribute[prMkvInfo->u2VidIdx].u.t_mkv_video_stm_attr.ui4_sample_size;
        prKeyInfo->ui4_avg_bytes_per_sec = 0;
    }
#else
    prMediaStatus->u8FileSize = 60918;
    prMediaStatus->u4TotalDuration = 90000;
    prMediaInfo->rRangeInfo[0].ui8_vid_duration = 90000;
    prMediaStatus->u2TotalPrmNum = 1;
    
    prMediaStatus->u2CaptionTracks = 0;
    
    prMediaInfo->u2VidStmNum = 1;   
    prMediaStatus->u2VideoTracks = 1;
    prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_VP8;
    prMediaStatus->au4VideoId[0] = 1;
    prMediaInfo->rPlayerStatus.au4VideoId[0] = 1;
    prSrcInfo->t_es_map[0].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
    prSrcInfo->t_es_map[0].ui4_strm_id = 1;
    prSrcInfo->t_es_map[0].ui4_scale = 1000;
    prSrcInfo->t_es_map[0].ui4_rate = 30000;
    prSrcInfo->t_es_map[0].ui4_sample_sz = 0;
    prSrcInfo->t_es_map[0].ui4_avg_bytes_per_sec = 0;
    prSrcInfo->t_es_map[0].ui4_indx_offset = 0;
    prSrcInfo->t_es_map[0].ui4_indx_size = 0;

    prMediaInfo->u2AudStmNum = 0;
    prMediaStatus->u2AudioTracks = 0;
    
    // Backup the media information
    x_memcpy(&prMediaInfo->rPlayerStatus, prMediaStatus, sizeof(PLAYMGR_STATUS_T));
#endif

    return TRUE;
}

#ifndef __MODEL_slt__
static void _MinfoNotify(
                           HANDLE_T                    hMinfo,                 
                           MINFO_NFY_EVENT_TYPE_T      e_event,  
                           const VOID*                 pvData1,
                           const VOID*                 pvData2,
                           const VOID*                 pvTag) 

{
    SIZE_T zRecLength;
    UINT32 u4Temp;
    INT32 i4Ret;
    MINFO_INFO_T rMinfoRec;
    PLAYMGR_INFO_T *prMediaInfo;    
    PLAYMGR_MKV_INFO_T* prMkvInfo;
    
    zRecLength = sizeof(MINFO_INFO_T);
    prMediaInfo = (PLAYMGR_INFO_T*)pvTag;
    prMkvInfo = &prMediaInfo->FmtInfo.rMkvInfo;
    x_memset(&rMinfoRec,0,sizeof(MINFO_INFO_T));

    switch(e_event) {
    case MINFO_NFY_EVENT_PARSING_DONE:
        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_GENERIC, (MINFO_REC_IDX_T)1,&rMinfoRec, &zRecLength);
    	 if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
	 {
	        LOG(1, "get media geberic info error\n");
	        break;//return MINFO_NFY_EVENT_ERROR;
    	 }
	 x_memcpy(&prMkvInfo->rPlayerGenericInfo, &(rMinfoRec.u.t_general_info),
	 	          sizeof(MINFO_TYPE_GENERIC_T));
        x_memset(&rMinfoRec.u,0,sizeof(rMinfoRec.u.t_general_info));
        i4Ret = x_minfo_get_info(hMinfo, 
        	                                    MINFO_INFO_TYPE_IDX_INFO, 
        	                                    (MINFO_REC_IDX_T)1,
        	                                    &rMinfoRec, 
        	                                    &zRecLength);
	 if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
        {
            LOG(1, "get media idx info error\n");
     	     break;//return MINFO_NFY_EVENT_ERROR;
	 }
        x_memcpy(&prMkvInfo->rPlayerIdxInfo, &(rMinfoRec.u.t_idx_info),
	 	          sizeof(MINFO_TYPE_IDX_INFO_T));
		x_memset(&rMinfoRec.u,0,sizeof(rMinfoRec.u.t_idx_info));
        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_META_DATA, (MINFO_REC_IDX_T)1,&rMinfoRec, &zRecLength);
    	 if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
	 {
	        LOG(1, "get metadata error\n");
	        break;//return MINFO_NFY_EVENT_ERROR;
    	 }
        x_memcpy(&prMkvInfo->rMetaData, &(rMinfoRec.u.t_meta_data), sizeof(MINFO_TYPE_META_DATA_T));

		x_memset(&rMinfoRec.u,0,sizeof(rMinfoRec.u.t_meta_data));
        i4Ret = x_minfo_get_num_recs(hMinfo, 
        	                                             MINFO_INFO_TYPE_VIDEO_ATTR, 
        	                                             &prMediaInfo->u2VidStmNum);
        if (i4Ret)
        {
            LOG(1, "get media video recs error\n");
            break;
        }
        
        prMkvInfo->u2AudIdx = prMediaInfo->u2VidStmNum;
        
        ASSERT(prMediaInfo->u2VidStmNum <= 17);
        for (u4Temp = 0; (u4Temp < prMediaInfo->u2VidStmNum) && (u4Temp < 17); u4Temp++)
        {
            i4Ret = x_minfo_get_info(hMinfo,MINFO_INFO_TYPE_VIDEO_ATTR, (MINFO_REC_IDX_T)1, &rMinfoRec, &zRecLength);
            if (i4Ret)
            {
                LOG(1, "get media video info error\n");
                break;
            }
            x_memcpy(&prMkvInfo->rStmAttribute[u4Temp], 
                             &(rMinfoRec.u.t_stm_attribute),
                             sizeof(MINFO_TYPE_STM_ATRBT_T));
			x_memset(&rMinfoRec.u,0,sizeof(rMinfoRec.u.t_stm_attribute));
        }

        x_minfo_get_num_recs(hMinfo, MINFO_INFO_TYPE_AUDIO_ATTR, &prMediaInfo->u2AudStmNum);
        if (i4Ret)
        {
            LOG(1, "get media audio recs error\n");
            break;
        }

        ASSERT((prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum) <= 17);
		x_memset(&rMinfoRec.u,0,sizeof(rMinfoRec.u.t_stm_attribute));
        for (; 
            (u4Temp < prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum) && (u4Temp < 17);
            u4Temp++)
        {
            i4Ret = x_minfo_get_info(hMinfo,MINFO_INFO_TYPE_AUDIO_ATTR, (MINFO_REC_IDX_T)1, &rMinfoRec, &zRecLength);
            if (i4Ret)
            {
                LOG(1, "get media audio info error\n");
                break;
            }

	     x_memcpy(&prMkvInfo->rStmAttribute[u4Temp], 
	                      &(rMinfoRec.u.t_stm_attribute),
	                      sizeof(MINFO_TYPE_STM_ATRBT_T));
		 x_memset(&rMinfoRec.u,0,sizeof(rMinfoRec.u.t_stm_attribute));

        }

        break;
    case MINFO_NFY_EVENT_HANDLE_CLOSED:
        break;
    case MINFO_NFY_EVENT_ERROR:             
        break;
    default:
        break;
    }

    _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, e_event);
}

static VOID _MIdxNotify(
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
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

BOOL _PLAYMGR_MkvStart(VOID* pvMediaInfo)
{
#ifndef __MODEL_slt__
    //BOOL fgRet;
    SIZE_T zSize;
    UINT32 u4Msg;
    UINT16 i=0;
    UINT16 j=0;    
    INT32 i4Ret;
#endif
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
#ifndef __MODEL_slt__
    PLAYMGR_MKV_INFO_T* prMkvInfo = 
    	(PLAYMGR_MKV_INFO_T*)&prMediaInfo->FmtInfo.rMkvInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prMkvInfo->rSrcInfo;
    MIDXBULD_KEY_TBL_INFO_T* prTableInfo = &prMkvInfo->rTableInfo;
    MIDXBULD_KEY_TBL_EXT_T*  prKeyTblList;
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt; 
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prMkvInfo->rKeyInfo;
    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTbl = &prMkvInfo->rKeyIdxTBL;
    //MM_RANGE_ELMT_IDXTBL_T* prAudKeyIdxTbl = &prMkvInfo->rAudKeyIdxTBL[0];
#endif
    if (prMediaInfo == 0)
    {
        LOG(1, "_PLAYMGR_MkvStart prMediaInfo == 0\n");
        return FALSE;
    }

    //move to common part
    /*if (prMediaInfo->u2VidStmNum > 0)
    {
        UINT16 u2VidIdx = prMediaInfo->FmtInfo.rMkvInfo.u2VidIdx;
        
        fgRet = SWDMX_SetStrmID(prMediaInfo->FmtInfo.rMkvInfo.rVDescrib[0].u_info.t_es.ui4_strm_id,
                                                    0, eSWDMX_STRM_TYPE_VID); 
        if (fgRet == FALSE)
        {
            LOG(1, "_PLAYMGR_MkvStart Fail to set video stream ID\n");
            return FALSE;
        }

        //mkv test
        //if (prMediaInfo->FmtInfo.rMkvInfo.rStmAttribute[u2VidIdx].u.t_mkv_video_stm_attr.ui4_codec_info_size != 0)
        {
            ASSERT(u2VidIdx < 17);
            if(u2VidIdx < 17)
            {
                SWDMX_SetInfo(eSWDMX_SET_VIDSEQHEADER, 
                    (UINT32)prMediaInfo->FmtInfo.rMkvInfo.rStmAttribute[u2VidIdx].u.t_mkv_video_stm_attr.pui1_codec_info, 
                    prMediaInfo->FmtInfo.rMkvInfo.rStmAttribute[u2VidIdx].u.t_mkv_video_stm_attr.ui4_codec_info_size, 
                    0);
            }
        }

        SWDMX_SetInfo(eSWDMX_ENABLE_STREAM, eSWDMX_STRM_TYPE_VID, 0, 0);
    }

    if ((prMediaInfo->u2AudStmNum > 0) && 
        (prMediaInfo->rPlayerStatus.aeAudFormat[0] != PLAYMGR_ADEC_UNKNOWN))
    {
        UINT16 u2AudIdx = prMediaInfo->FmtInfo.rMkvInfo.u2AudIdx;
        
        fgRet = SWDMX_SetStrmID(prMediaInfo->FmtInfo.rMkvInfo.rADescrib[0].u_info.t_es.ui4_strm_id, 
                                                    0, eSWDMX_STRM_TYPE_AUD); 
        if (fgRet == FALSE)
        {
            LOG(1, "_PLAYMGR_MkvStart Fail to set audio stream ID\n");
            return FALSE;
        }

        //mkv test
        ASSERT(u2AudIdx < 17);
        if(u2AudIdx < 17)
        {
            SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE, 
                0xFF,
                (UINT32)prMediaInfo->FmtInfo.rMkvInfo.rStmAttribute[u2AudIdx].u.t_mkv_audio_stm_attr.i4_samples_per_sec, 
                (UINT32)prMediaInfo->FmtInfo.rMkvInfo.rStmAttribute[u2AudIdx].u.t_mkv_audio_stm_attr.i2_channels);
        }        
        SWDMX_SetInfo(eSWDMX_ENABLE_STREAM, eSWDMX_STRM_TYPE_AUD, 0, 0);
    }*/

    //SWDMX_DelRange(prMediaInfo->u1SrcId, NULL, (UINT32)prMediaInfo->rRangeInfo[0].ui4_range_id);
    //SWDMX_DelRange(prMediaInfo->u1SrcId, NULL, (UINT32)prMediaInfo->rRangeInfo[1].ui4_range_id);

#ifndef __MODEL_slt__
    LOG(1, "x_midxbuld_mopen\n");
    i4Ret = x_midxbuld_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _MIdxNotify, prMediaInfo,
                            prMediaFmt, prSrcInfo, prKeyInfo, &prMkvInfo->hMediaIdxHandle);
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
        i4Ret = x_midxbuld_get_info(prMkvInfo->hMediaIdxHandle, MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST,
        	(VOID*)prTableInfo, &zSize);
        if (i4Ret)
        {
            LOG(1, "get midxbuild elmt tbl error\n");
        }       

        //video key table
        prKeyTblList = prTableInfo->pt_key_tbl_ext;
        prKeyIdxTbl->e_elem_type = MM_ELEMT_TYPE_KEY;
        prKeyIdxTbl->t_strm_id.u.t_stm_mkv_id = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
        prKeyIdxTbl->ui4_number_of_entry = prKeyTblList->t_elmt_tbl.ui4_number_of_entry;
        prKeyIdxTbl->ui8_base_offset = prKeyTblList->t_elmt_tbl.ui8_base_offset;
        prKeyIdxTbl->u.pt_pb_idx_tbl_entry = 
            (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)prKeyTblList->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
        prKeyIdxTbl->ui4_rate = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_rate;
        prKeyIdxTbl->ui4_scale = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale;
        prKeyTblList = prKeyTblList->pt_next;   //key tbl info from mutil
	if (prMediaInfo->u2AudStmNum > 0)
            {
  
	   prKeyIdxTbl->pv_next_tbl=&prMkvInfo->rAudKeyIdxTBL[0];
	   
   for(i= prMkvInfo->u2AudIdx; 
        ((i < prMediaInfo->u2AudStmNum + prMkvInfo->u2AudIdx) && 
        ((i - prMediaInfo->u2VidStmNum)<PLAYMGR_MAX_AUD_TRACK)) &&
        (i < 16)&&(j<15); 
        i++, j++)
    {
         prMkvInfo->rAudKeyIdxTBL[j].e_elem_type=MM_ELEMT_TYPE_AUDIO;
        

        if ((i - prMediaInfo->u2VidStmNum) < PLAYMGR_MAX_AUD_TRACK)
        {
            prMkvInfo->rAudKeyIdxTBL[j].t_strm_id.u.t_stm_mkv_id = 
                prMkvInfo->rStmAttribute[i].t_stm_id.u.t_mkv_stm_id;
        }
        else
        {
            LOG(3, "there r too many audio tracks\n");
        }

	switch (prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.e_enc)
        {
            case MINFO_INFO_AUD_ENC_MPEG_1: case MINFO_INFO_AUD_ENC_MPEG_2: 
               prMkvInfo->rAudKeyIdxTBL[j].t_aud_spec_info.e_fmt= 
                    ASH_AUDIO_FMT_MP3;
                break;
            case MINFO_INFO_AUD_ENC_AC3:
                prMkvInfo->rAudKeyIdxTBL[j].t_aud_spec_info.e_fmt=  
                    ASH_AUDIO_FMT_AC3;
                break;
            case MINFO_INFO_AUD_ENC_PCM:
                prMkvInfo->rAudKeyIdxTBL[j].t_aud_spec_info.e_fmt=  
                    ASH_AUDIO_FMT_PCM;
                break;
            case MINFO_INFO_AUD_ENC_AAC:
                prMkvInfo->rAudKeyIdxTBL[j].t_aud_spec_info.e_fmt=  
                    ASH_AUDIO_FMT_AAC;
		prMkvInfo->rAudKeyIdxTBL[j].t_aud_spec_info.u.t_aac_info.ui4_sample_rate=
		prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.i4_samples_per_sec;
                break;
            default:
                prMkvInfo->rAudKeyIdxTBL[j].t_aud_spec_info.e_fmt= 
                    ASH_AUDIO_FMT_UNKNOWN;   //not support
                LOG(1, "Aud type %d not support\n", prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.e_enc);
            	  break;
        }
        prMkvInfo->rAudKeyIdxTBL[j].ui4_scale = 
        	prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.ui4_scale;
        prMkvInfo->rAudKeyIdxTBL[j].ui4_rate = 
        	prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.ui4_rate;
        //prSrcInfo->t_es_map[i].ui4_sample_sz = 
        //	prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.ui4_sample_size;
        prMkvInfo->rAudKeyIdxTBL[j].ui4_avg_bytes_per_sec = 
        	prMkvInfo->rStmAttribute[i].u.t_mkv_audio_stm_attr.ui4_avg_bytes_sec;   

	prMkvInfo->rAudKeyIdxTBL[j].pv_next_tbl=(void*)&prMkvInfo->rAudKeyIdxTBL[j+1];
    }
 
         }
        
        //audio key table
        /*if (prMediaInfo->u2AudStmNum > 0)
        {
            while ( prKeyTblList!= NULL)           //key tbl info from mutil
            {
                prAudKeyIdxTbl->e_elem_type = MM_ELEMT_TYPE_KEY;
                prAudKeyIdxTbl->t_strm_id.u.t_stm_mkv_id = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
                prAudKeyIdxTbl->ui4_number_of_entry = prKeyTblList->t_elmt_tbl.ui4_number_of_entry;
                prAudKeyIdxTbl->ui8_base_offset = prKeyTblList->t_elmt_tbl.ui8_base_offset;
                prAudKeyIdxTbl->ui4_scale = prKeyTblList->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_scale;
                prAudKeyIdxTbl->ui4_rate = prKeyTblList->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_rate;
                prAudKeyIdxTbl->ui4_avg_bytes_per_sec = prKeyTblList->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_avg_bytes_per_sec;
                prAudKeyIdxTbl->b_is_VBR = (prAudKeyIdxTbl->ui4_avg_bytes_per_sec == 0) ? TRUE : FALSE;
                prAudKeyIdxTbl->u.pv_idx_tbl_entry = 
                    (void*)prKeyTblList->t_elmt_tbl.u_fmt.t_as_key.pui4_pts;
                prKeyTblList = prKeyTblList->pt_next;
                prAudKeyIdxTbl++;
            }
        }*/
     }
#endif

    //build two interval of indx table at beginning
    LOG(1, "start build idx\n");
    prMediaInfo->u1CurEmptyRange = 0;
    prMediaInfo->u4CurRangePTS = 0;
    _PLAYMGR_MkvUpdateRange(pvMediaInfo);
            
    /*fgRet = _SWDMX_Play();
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_MkvStart Fail to play\n");
        return FALSE;
    }*/

    return TRUE;
}


BOOL _PLAYMGR_MkvStop(VOID* pvMediaInfo)
{
    #if 0
    //BOOL fgRet = TRUE;
    UINT32 u4SwdmxCurStas;

    /*fgRet = _SWDMX_Stop();
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_MkvStop Fail in _SWDMX_Stop\n");
        return FALSE;
    }*/

    u4SwdmxCurStas = SWDMX_QueryStatus();
    while (u4SwdmxCurStas != eSWDMX_STATE_STOP)
    {
        u4SwdmxCurStas = SWDMX_QueryStatus();
        x_thread_delay(1);
    }
    #endif
        
    return TRUE;
}

EXTERN BOOL _PLAYMGR_MkvClose(VOID* pvMediaInfo)
{
    UINT32 u4Msg;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    if (prMediaInfo->FmtInfo.rMkvInfo.hMediaInfoHandle != (HANDLE_T)NULL)
    {
        x_minfo_close(prMediaInfo->FmtInfo.rMkvInfo.hMediaInfoHandle);
        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (VOID*)&u4Msg);
        if (u4Msg != MINFO_NFY_EVENT_HANDLE_CLOSED)
        {
            return FALSE;
        }
    }
    if (prMediaInfo->FmtInfo.rMkvInfo.hMediaIdxHandle != (HANDLE_T)NULL)
    {
        x_midxbuld_close(prMediaInfo->FmtInfo.rMkvInfo.hMediaIdxHandle);
        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (VOID*)&u4Msg);
        if (u4Msg != MIDXBULD_NFY_EVENT_HANDLE_CLOSED)
        {
            return FALSE;
        }
    }

    x_memset(&prMediaInfo->FmtInfo.rMkvInfo, 0, sizeof(PLAYMGR_MKV_INFO_T));

    return TRUE;
}

EXTERN BOOL _PLAYMGR_MkvResume(VOID* pvMediaInfo)
{
    /*BOOL fgRet;
    
    fgRet = _SWDMX_Play();
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_MkvStart Fail to play\n");
        return FALSE;
    }*/
    UNUSED(pvMediaInfo);

    return TRUE;
}

BOOL _PLAYMGR_MkvPause(VOID)
{
    return TRUE;//_SWDMX_Pause();
}


BOOL _PLAYMGR_OpenMkvMediaInfo(PLAYMGR_STATUS_T* prMediaStatus, VOID* pvMediaInfo)
{
#ifndef __MODEL_slt__
    INT32 i4Ret;
    UINT32 u4Msg;
#endif
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    PLAYMGR_MKV_INFO_T* prMkvInfo = 
    	(PLAYMGR_MKV_INFO_T*)&prMediaInfo->FmtInfo.rMkvInfo;
#ifndef __MODEL_slt__
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt; 
#endif
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prMkvInfo->rSrcInfo;
    MIDXBULD_ELMT_TBL_DESCRIB_T* prVDescrib = &prMkvInfo->rVDescrib[0];
    MIDXBULD_ELMT_TBL_DESCRIB_T* prADescrib = &prMkvInfo->rADescrib[0];

    //MM_RANGE_ELMT_IDXTBL_T* prVidElmtStrm = &prMkvInfo->rVidIDXTBL[0];
    //MM_RANGE_ELMT_IDXTBL_T* prAudElmtStrm = &prMkvInfo->rAudIDXTBL[0];

#ifndef __MODEL_slt__
    // Open for media info
    i4Ret = x_minfo_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _MinfoNotify, prMediaInfo,
            		       prMediaFmt, &(prMkvInfo->hMediaInfoHandle));
    
    if(i4Ret != MINFOR_OK)
    {
        return FALSE;
    }

    _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (VOID*)&u4Msg);
    if (u4Msg != MINFO_NFY_EVENT_PARSING_DONE)
    {
        LOG(1, "x_minfo_mopen rcv msg != DONE\n");
        return FALSE;
    }
    
    // Wait for media info's notify
    prSrcInfo->ui8_data_offset = prMkvInfo->rPlayerIdxInfo.ui8_data_offset;
    prSrcInfo->ui8_data_size = prMkvInfo->rPlayerIdxInfo.ui8_data_sz;
    prSrcInfo->ui8_idx_offset = prMkvInfo->rPlayerIdxInfo.ui8_idx_offset;
    prSrcInfo->ui8_idx_size = prMkvInfo->rPlayerIdxInfo.ui8_idx_sz;
    prSrcInfo->ui1_total_strm_num = (UINT8)(prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum);
    prSrcInfo->ui4_total_duration = (UINT32)prMkvInfo->rPlayerGenericInfo.ui8_pb_duration;
    prSrcInfo->ui8_seg_data_offset = prMkvInfo->rPlayerIdxInfo.ui8_segment_data_offset;
    prSrcInfo->ui8_seg_tm_scale = prMkvInfo->rPlayerIdxInfo.ui8_seg_timecode_scale;
#else
    // Wait for media info's notify
    prSrcInfo->ui8_data_offset = 5431;
    prSrcInfo->ui8_data_size = 55487;
    prSrcInfo->ui8_idx_offset = 60899;
    prSrcInfo->ui8_idx_size = 19;
    prSrcInfo->ui1_total_strm_num = 1;
    prSrcInfo->ui4_total_duration = 42030;
    prSrcInfo->ui8_seg_data_offset = 52;
    prSrcInfo->ui8_seg_tm_scale = 1000000;
    
    x_memset(&prMediaInfo->rRangeInfo[0], 0, sizeof(MM_RANGE_INFO_T));
#endif

    if (_SetMKVInfo(prMediaStatus, prMediaInfo) != TRUE)
    {
        return FALSE;
    }

    prMediaInfo->rRangeInfo[0].ui8_fileoffset = prSrcInfo->ui8_data_offset;
    prMediaInfo->rRangeInfo[0].z_range_sz = prSrcInfo->ui8_data_size;
    prMediaInfo->rPlayerStatus.u4TotalDuration = prSrcInfo->ui4_total_duration;
    prMediaStatus->u4TotalDuration = prSrcInfo->ui4_total_duration;

    ASSERT(prMkvInfo->u2VidIdx < MIDXBULD_ELMT_TBL_MAX);
    if ((prMediaInfo->u2VidStmNum) > 0 && (prMkvInfo->u2VidIdx < MIDXBULD_ELMT_TBL_MAX))
    {
        prVDescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        prVDescrib->u_info.t_es.ui4_strm_id = 
            prSrcInfo->t_es_map[prMkvInfo->u2VidIdx].ui4_strm_id;
        prVDescrib->u_info.t_es.ui4_scale = 
            prSrcInfo->t_es_map[prMkvInfo->u2VidIdx].ui4_scale;
        prVDescrib->u_info.t_es.ui4_rate = 
            prSrcInfo->t_es_map[prMkvInfo->u2VidIdx].ui4_rate;
        prVDescrib->u_info.t_es.ui4_sample_sz = 0;
        prVDescrib->u_info.t_es.ui4_avg_bytes_per_sec = 0;
    }
    
    ASSERT(prMkvInfo->u2AudIdx < MIDXBULD_ELMT_TBL_MAX);    
    if ((prMediaInfo->u2AudStmNum > 0) && (prMkvInfo->u2AudIdx < MIDXBULD_ELMT_TBL_MAX))
    {
        prADescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        prADescrib->u_info.t_es.ui4_strm_id = 
        	prSrcInfo->t_es_map[prMkvInfo->u2AudIdx].ui4_strm_id;
        prADescrib->u_info.t_es.ui4_scale = 
        	prSrcInfo->t_es_map[prMkvInfo->u2AudIdx].ui4_scale;
        prADescrib->u_info.t_es.ui4_rate = 
        	prSrcInfo->t_es_map[prMkvInfo->u2AudIdx].ui4_rate;
        prADescrib->u_info.t_es.ui4_sample_sz = 
        	prSrcInfo->t_es_map[prMkvInfo->u2AudIdx].ui4_sample_sz ;
        prADescrib->u_info.t_es.ui4_avg_bytes_per_sec = 
        	prSrcInfo->t_es_map[prMkvInfo->u2AudIdx].ui4_avg_bytes_per_sec;
    }
    return TRUE;
}


BOOL _PLAYMGR_MkvUpdateRange(VOID* pvMediaInfo)
{
#ifndef __MODEL_slt__
    INT32 i4RangeIdx;
#endif
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
#ifndef __MODEL_slt__
    PLAYMGR_MKV_INFO_T* prMkvInfo = 
    	(PLAYMGR_MKV_INFO_T*)&prMediaInfo->FmtInfo.rMkvInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prMkvInfo->rSrcInfo;

    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTbl = &prMkvInfo->rKeyIdxTBL;
#endif
    //MM_RANGE_ELMT_IDXTBL_T* prAudKeyIdxTbl = &prMkvInfo->rAudKeyIdxTBL[0];
    
    //MM_RANGE_ELMT_IDXTBL_T* prVidElmtStrm = &prMkvInfo->rVidIDXTBL[i4RangeIdx];
    //MM_RANGE_ELMT_IDXTBL_T* prAudElmtStrm = &prMkvInfo->rAudIDXTBL[i4RangeIdx];
    	
    // link key index to the end of the list
    //mkv test
    //MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = prMediaInfo->rRangeInfo[i4RangeIdx].pt_idxtbl_lst;

#ifndef __MODEL_slt__
    i4RangeIdx = prMediaInfo->u1CurEmptyRange;

    // video key table
    ASSERT(i4RangeIdx < MAX_RANGE_NUM);
    if ((prKeyIdxTbl->ui4_number_of_entry > 0) && (i4RangeIdx < MAX_RANGE_NUM))
    {
        prMediaInfo->rRangeInfo[i4RangeIdx].pt_idxtbl_lst = prKeyIdxTbl;
    
        prMediaInfo->rRangeInfo[i4RangeIdx].ui4_range_id = 
            prMkvInfo->rAllocTblInfo[i4RangeIdx].ui4_range_id;

        LOG(3, "Allocate prMkvInfo->rAllocTblInfo[%d].ui4_range_id = %ld, pv_tag = %ld\n", 
            i4RangeIdx, prMkvInfo->rAllocTblInfo[i4RangeIdx].ui4_range_id,
            prMediaInfo->rRangeInfo[i4RangeIdx].ui4_range_id);

        //if((prVidElmtStrm->ui4_number_of_entry > 0) ||
        //    (prAudElmtStrm->ui4_number_of_entry > 0))
        {
            prMediaInfo->rRangeInfo[i4RangeIdx].ui8_pb_duration = (UINT64)prSrcInfo->ui4_total_duration;
            //prMediaInfo->rRangeInfo[i4RangeIdx].ui8_timecode_scale = 
            //    (UINT32)prMediaInfo->FmtInfo.rMkvInfo.rPlayerGenericInfo.ui8_timecode_scale;
            SWDMX_AddRange(prMediaInfo->u1SrcId, &prMediaInfo->rRangeInfo[i4RangeIdx], 
                                    (UINT32)prMediaInfo->rRangeInfo[i4RangeIdx].ui4_range_id);
        }
        //else
        {
        //   LOG(3, "range render cnt 0!\n");
        }
    }
#else
    SWDMX_AddRange(prMediaInfo->u1SrcId, &prMediaInfo->rRangeInfo[0], 
                            (UINT32)prMediaInfo->rRangeInfo[0].ui4_range_id);
#endif

    return TRUE;
}


BOOL _PLAYMGR_MkvFreeRange(VOID* pvMediaInfo, UINT32 u4RangeTag)
{
    INT32 i, i4Ret;
    SIZE_T zSize;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_MKV_INFO_T* prMkvInfo = 
    	(PLAYMGR_MKV_INFO_T*)&prMediaInfo->FmtInfo.rMkvInfo;

    for (i = 0; i < MAX_RANGE_NUM; i++)
    {
        if (prMediaInfo->rRangeInfo[i].ui4_range_id == u4RangeTag)
        {
            zSize = sizeof(UINT32);
            LOG(3, "free range prMediaInfo->rRangeInfo[i].pv_tag = %ld\n", prMediaInfo->rRangeInfo[i].ui4_range_id);
            LOG(3, "free range prMkvInfo->rAllocTblInfo[i].ui4_range_id = %ld\n", prMkvInfo->rAllocTblInfo[i].ui4_range_id);
            i4Ret = x_midxbuld_set_info(prMediaInfo->FmtInfo.rMkvInfo.hMediaIdxHandle, 
                                 MIDXBULD_SET_INFO_TYPE_FREE_RANGE, 
                                 (VOID*)prMkvInfo->rAllocTblInfo[i].ui4_range_id, 
                                 &zSize);
            prMediaInfo->u1CurEmptyRange = (UINT8)i;
            x_memset(&prMediaInfo->rRangeInfo[i], 0, sizeof(MM_RANGE_INFO_T));
        }
    }
    UNUSED(i4Ret);
    return TRUE;
}


#endif // !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)



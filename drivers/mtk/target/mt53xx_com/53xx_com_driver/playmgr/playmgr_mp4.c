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
 * $RCSfile: playmgr_mp4.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
#include "playmgr.h"
#include "feeder_if.h"
#include "playmgr_util.h"
#include "swdmx_drvif.h"
#include "playmgr_mp4.h"
#include "playmgr_debug.h"

static BOOL _PLAYMGR_SetMP4Info(PLAYMGR_STATUS_T* prMediaStatus, PLAYMGR_INFO_T* prMediaInfo)
{
    UINT16 i; 
    PLAYMGR_MP4_INFO_T* prMp4Info = &prMediaInfo->FmtInfo.rMp4Info;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prMp4Info->rSrcInfo;
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prMp4Info->rKeyInfo;
    
    prMediaStatus->eMediaType = PLAYMGR_MEDIA_TYPE_MP4;
    prMediaStatus->u8FileSize = prMediaInfo->FmtInfo.rAviInfo.rPlayerGenericInfo.ui8_media_sz;
    
    prMediaStatus->u2TotalPrmNum = 1;
    if ((prMediaInfo->u2VidStmNum > 0) &&
        (prMp4Info->u2VidIdx < 3))
    {
        prMediaStatus->u2VideoTracks = prMediaInfo->u2VidStmNum;
        switch (prMp4Info->rStmAttribute[prMp4Info->u2VidIdx].u.t_mp4i_video_stm_attr.e_enc)
        {
            case MINFO_INFO_VID_ENC_MPEG_4:
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_MPEG4;
                break;
            case MINFO_INFO_VID_ENC_H264:
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_H264;
                break;
            case MINFO_INFO_VID_ENC_H263:
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_H263;
                break;
            case MINFO_INFO_VID_ENC_MJPG:                
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_MJPEG;                
                break;
            default:
                return FALSE;
        }
    }

    if ((prMediaInfo->u2AudStmNum > 0) &&
        (prMp4Info->u2AudIdx < 3))
    {
        prMediaStatus->u2AudioTracks = prMediaInfo->u2AudStmNum;
        switch (prMp4Info->rStmAttribute[prMp4Info->u2AudIdx].u.t_mp4i_audio_stm_attr.e_enc)
        {
            case MINFO_INFO_AUD_ENC_AAC: 
                prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_AAC;
                break;
            case MINFO_INFO_AUD_ENC_MPEG_1: case MINFO_INFO_AUD_ENC_MPEG_2:
                prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_MP3;
                break;
            default:
            	return FALSE;
        }
    }

    // Not implement yet
    prMediaStatus->u2CaptionTracks = 0;

    for (i = 0; (i < prMediaInfo->u2VidStmNum) && (i < 3); i++)
    {
        prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
       
        prSrcInfo->t_es_map[i].ui4_strm_id = 
        	prMp4Info->rStmAttribute[i].t_stm_id.u.t_mp4_stm_id;
        prSrcInfo->t_es_map[i].ui4_scale = 
        	prMp4Info->rStmAttribute[i].u.t_mp4i_video_stm_attr.ui4_timescale;
        prSrcInfo->t_es_map[i].ui4_rate = 0;
        prSrcInfo->t_es_map[i].ui4_sample_sz = 0;
        prSrcInfo->t_es_map[i].ui4_avg_bytes_per_sec = 0;
    }

    for (;(i < prMediaInfo->u2AudStmNum + prMediaInfo->u2VidStmNum) && (i < 3); i++)
    {
        if ((i - prMediaInfo->u2VidStmNum) < 3)
        {
            prMediaStatus->au4AudioId[i-prMediaInfo->u2VidStmNum] = 
                    prMp4Info->rStmAttribute[i].t_stm_id.u.t_mp4_stm_id;

            prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
            prSrcInfo->t_es_map[i].ui4_strm_id = 
            	prMp4Info->rStmAttribute[i].t_stm_id.u.t_mp4_stm_id;
            prSrcInfo->t_es_map[i].ui4_scale = 
            	prMp4Info->rStmAttribute[i].u.t_mp4i_audio_stm_attr.ui4_timescale;
            prSrcInfo->t_es_map[i].ui4_rate = 0;
            prSrcInfo->t_es_map[i].ui4_sample_sz = 0;
            prSrcInfo->t_es_map[i].ui4_avg_bytes_per_sec = 0;    
        }
    }

    if (prMp4Info->u2VidIdx < 3)
    {
        prKeyInfo->ui4_strm_id = prSrcInfo->t_es_map[prMp4Info->u2VidIdx].ui4_strm_id;
        prKeyInfo->ui4_scale = 
        	prMp4Info->rStmAttribute[prMp4Info->u2VidIdx].u.t_mp4i_video_stm_attr.ui4_timescale;
        prKeyInfo->ui4_rate = 0;    	
        prKeyInfo->ui4_sample_sz = 0;
        prKeyInfo->ui4_avg_bytes_per_sec = 0;
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
    INT32 i4Ret;  
    UINT32 u4Temp;
    MINFO_INFO_T rMinfoRec;
    PLAYMGR_INFO_T *prMediaInfo;    
    PLAYMGR_MP4_INFO_T* prMp4Info;
    MINFO_NFY_EVENT_TYPE_T      eEvent = e_event;
    
    zRecLength = sizeof(MINFO_INFO_T);
    prMediaInfo = (PLAYMGR_INFO_T*)pvTag;
    prMp4Info = &prMediaInfo->FmtInfo.rMp4Info;
    
    switch(e_event) {
    case MINFO_NFY_EVENT_PARSING_DONE:
        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_GENERIC, 1,&rMinfoRec, &zRecLength);
    	 if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
	 {
	        LOG(1, "get media geberic info error\n");
	        eEvent = MINFO_NFY_EVENT_ERROR;
	        break;
    	 }
	 x_memcpy(&prMp4Info->rPlayerGenericInfo, &(rMinfoRec.u.t_general_info),
	 	          sizeof(MINFO_TYPE_GENERIC_T));
        
        i4Ret = x_minfo_get_info(hMinfo, 
        	                                    MINFO_INFO_TYPE_IDX_INFO, 
        	                                    1,
        	                                    &rMinfoRec, 
        	                                    (SIZE_T*)&zRecLength);
	 if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
        {
            LOG(1, "get media idx info error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
	 }
        x_memcpy(&prMp4Info->rPlayerIdxInfo, &(rMinfoRec.u.t_idx_info),
	 	          sizeof(MINFO_TYPE_IDX_INFO_T));

        i4Ret = x_minfo_get_num_recs(hMinfo, 
        	                                             MINFO_INFO_TYPE_VIDEO_ATTR, 
        	                                             &prMediaInfo->u2VidStmNum);
        if (i4Ret)
        {
            LOG(1, "get media video recs error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }

        prMp4Info->u2AudIdx = prMediaInfo->u2VidStmNum;
        for (u4Temp = 0; (u4Temp < prMediaInfo->u2VidStmNum) && (u4Temp < 3); u4Temp++)
        {
            i4Ret = x_minfo_get_info(hMinfo,MINFO_INFO_TYPE_VIDEO_ATTR, 1, &rMinfoRec, &zRecLength);
            if (i4Ret)
            {
                LOG(1, "get media video info error\n");
                eEvent = MINFO_NFY_EVENT_ERROR;
                break;
            }
            x_memcpy(&prMp4Info->rStmAttribute[u4Temp], 
                             &(rMinfoRec.u.t_stm_attribute),
                             sizeof(MINFO_TYPE_STM_ATRBT_T));
        }

        x_minfo_get_num_recs(hMinfo, MINFO_INFO_TYPE_AUDIO_ATTR, &prMediaInfo->u2AudStmNum);
        if (i4Ret)
        {
            LOG(1, "get media audio recs error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }
        
        for (; (u4Temp < prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum) && (u4Temp < 3); u4Temp++)
        {
            i4Ret = x_minfo_get_info(hMinfo,MINFO_INFO_TYPE_AUDIO_ATTR, 1, &rMinfoRec, &zRecLength);
            if (i4Ret)
            {
                LOG(1, "get media audio info error\n");
                eEvent = MINFO_NFY_EVENT_ERROR;
                break;
            }

	     x_memcpy(&prMp4Info->rStmAttribute[u4Temp], 
	                      &(rMinfoRec.u.t_stm_attribute),
	                      sizeof(MINFO_TYPE_STM_ATRBT_T));

        }

        // get meta data
        x_memset(&rMinfoRec, 0, zRecLength);
        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_META_DATA, 1, (VOID*)&rMinfoRec, &zRecLength);
        if (i4Ret)
        {
            LOG(1, "get meta data error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }
        x_memcpy(&prMp4Info->rMp4MetaData, &(rMinfoRec.u.t_meta_data), sizeof(MINFO_TYPE_META_DATA_T));

        break;
    case MINFO_NFY_EVENT_HANDLE_CLOSED:
        break;
    case MINFO_NFY_EVENT_ERROR:             
        break;
    default:
        break;
    }

    _PLAYMGR_SendNotify(eEvent);
}


static void _PLAYMGR_MIdx_Notify(
                           HANDLE_T                    hMinfo,                 
                           MIDXBULD_NFY_EVENT_TYPE_T   e_event,  
                           const VOID*                 pvData1,
                           const VOID*                 pvData2,
                           const VOID*                 pvTag) 

{

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

    _PLAYMGR_SendNotify(e_event);
}


BOOL _PLAYMGR_Mp4Start(VOID* pvMediaInfo)
{
    INT32 i4Ret;
    UINT32 u4Msg;
   
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_MP4_INFO_T* prMp4Info = 
    	(PLAYMGR_MP4_INFO_T*)&prMediaInfo->FmtInfo.rMp4Info;
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt; 

    MIDXBULD_KEY_INFO_T* prKeyInfo = &prMp4Info->rKeyInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prMp4Info->rSrcInfo;
    MIDXBULD_ELMT_TBL_INFO_T* prTableInfo = &prMp4Info->rTableInfo;
    MIDXBULD_KEY_TBL_INFO_T* prKeyTableInfo = &prMp4Info->rKeyTableInfo;

    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTbl = &prMp4Info->rKeyIdxTBL;
    MIDXBULD_KEY_TBL_EXT_T*  prKeyTblList;
    
    MIDXBULD_ELMT_TBL_ALLOC_INFO_T* prAllocTblInfo = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T* prVDescrib = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T* prADescrib = &prMp4Info->rADescrib[0];
    MIDXBULD_ELMT_TBL_DESCRIB_T* prFirstDescrib = NULL;

    MM_RANGE_ELMT_IDXTBL_T* prVidElmtStrm = &prMp4Info->rVidIDXTBL[0];
    MM_RANGE_ELMT_IDXTBL_T* prAudElmtStrm = &prMp4Info->rAudIDXTBL[0];

    MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
    SIZE_T zSize;


    if (prMediaInfo == 0)
    {
        LOG(1, "_PLAYMGR_Mp4Start prMediaInfo == 0\n");
        return FALSE;
    }
    prMediaInfo->u1CurEmptyRange = 1;

    if (&prMp4Info->rAllocTblInfo[0])
    {
        prAllocTblInfo = &prMp4Info->rAllocTblInfo[0];
    }
    else
    {
        return FALSE;
    }

    if (&prMp4Info->rVDescrib[0])
    {
        prVDescrib = &prMp4Info->rVDescrib[0];
    }
    else
    {
        return FALSE;
    }

    prMediaInfo->rRangeInfo[0].ui8_fileoffset = prSrcInfo->ui8_data_offset;
    prMediaInfo->rRangeInfo[0].z_range_sz = prSrcInfo->ui8_data_size;

    if ((prMediaInfo->u2VidStmNum > 0) &&
        (prMp4Info->u2VidIdx < 3))
    {
        prVDescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        prVDescrib->u_info.t_es.ui4_strm_id = 
            prSrcInfo->t_es_map[prMp4Info->u2VidIdx].ui4_strm_id;
        prVDescrib->u_info.t_es.ui4_scale = 
            prSrcInfo->t_es_map[prMp4Info->u2VidIdx].ui4_scale;
        prVDescrib->u_info.t_es.ui4_rate = 
            prSrcInfo->t_es_map[prMp4Info->u2VidIdx].ui4_rate;
        prVDescrib->u_info.t_es.ui4_sample_sz = 0;
        prVDescrib->u_info.t_es.ui4_avg_bytes_per_sec = 0;
    }
    
    if ((prMediaInfo->u2AudStmNum > 0) &&
        (prMp4Info->u2AudIdx < 3))
    {
        prADescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        prADescrib->u_info.t_es.ui4_strm_id = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_strm_id;
        prADescrib->u_info.t_es.ui4_scale = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_scale;
        prADescrib->u_info.t_es.ui4_rate = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_rate;
        prADescrib->u_info.t_es.ui4_sample_sz = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_sample_sz ;
        prADescrib->u_info.t_es.ui4_avg_bytes_per_sec = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_avg_bytes_per_sec;
    }

    i4Ret = x_midxbuld_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _PLAYMGR_MIdx_Notify, 0,
                            prMediaFmt, prSrcInfo, prKeyInfo, &prMp4Info->hMediaIdxHandle);
    if(i4Ret != MINFOR_OK)
    {
        LOG(3, "x_midxbuld_mopen Fail\n");
        return FALSE;
    }

    if (!_PLAYMGR_ReceiveNotify((VOID*)&u4Msg))
    {
        return FALSE;
    }
    
    if (u4Msg != MIDXBULD_NFY_EVENT_INDEXING_DONE)
    {
        return FALSE;
    }
    else
    {
        //save key table
        zSize = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
        prKeyTableInfo->ui4_key_tbl_id = prKeyInfo->ui4_tbl_id;
        i4Ret = x_midxbuld_get_info(prMp4Info->hMediaIdxHandle, MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST,
            (VOID*)prKeyTableInfo, &zSize);
        if (i4Ret)
        {
            LOG(1, "get midxbuild elmt tbl error\n");
        }       

        //video key table
        prKeyTblList = prKeyTableInfo->pt_key_tbl_ext;
        prKeyIdxTbl->e_elem_type = MM_ELEMT_TYPE_KEY;
        prKeyIdxTbl->t_strm_id.u.t_stm_mp4_id = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
        prKeyIdxTbl->ui4_number_of_entry = prKeyTblList->t_elmt_tbl.ui4_number_of_entry;
        prKeyIdxTbl->ui8_base_offset = prKeyTblList->t_elmt_tbl.ui8_base_offset;
        prKeyIdxTbl->u.pt_pb_idx_tbl_entry = 
            (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)prKeyTblList->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
        prKeyTblList = prKeyTblList->pt_next;   //key tbl info from mutil
        //--- finish key table saving

        zSize = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
        prTableInfo->ui4_tbl_id = prKeyInfo->ui4_tbl_id;
        i4Ret = x_midxbuld_get_info(prMp4Info->hMediaIdxHandle, MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL,
        	(VOID*)prTableInfo, &zSize);
        if (i4Ret)
        {
            LOG(1, "get midxbuild elmt tbl error\n");
        }        
    }

    if ((prMediaInfo->u2AudStmNum > 0) &&
        (prMp4Info->u2AudIdx < 3))
    {
        prFirstDescrib = prADescrib;
        prADescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        prADescrib->u_info.t_es.ui4_strm_id = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_strm_id;
        prADescrib->u_info.t_es.ui4_scale = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_scale;
        prADescrib->u_info.t_es.ui4_rate = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_rate;
        prADescrib->u_info.t_es.ui4_sample_sz = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_sample_sz ;
        prADescrib->u_info.t_es.ui4_avg_bytes_per_sec = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_avg_bytes_per_sec;
        prVDescrib->u_info.t_es.b_is_cur_strm_codec_spt = 
                  prMp4Info->rStmAttribute[prMp4Info->u2AudIdx].b_is_codec_support;
        prADescrib->pt_next = NULL;
    }


    if (prMediaInfo->u2VidStmNum > 0)
    {
        (prFirstDescrib == NULL) ? (prFirstDescrib = prVDescrib) : 
                                       (prFirstDescrib->pt_next = prVDescrib);

        prVDescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        prVDescrib->u_info.t_es.ui4_strm_id = prSrcInfo->t_es_map[0].ui4_strm_id;
        prVDescrib->u_info.t_es.ui4_scale = prSrcInfo->t_es_map[0].ui4_scale;
        prVDescrib->u_info.t_es.ui4_rate = prSrcInfo->t_es_map[0].ui4_rate;
        prVDescrib->u_info.t_es.ui4_sample_sz = 0;
        prVDescrib->u_info.t_es.ui4_avg_bytes_per_sec = 0;
        prVDescrib->u_info.t_es.b_is_cur_strm_codec_spt = 
            prMp4Info->rStmAttribute[0].b_is_codec_support;
        prVDescrib->pt_next = NULL;
    }
    
    prAllocTblInfo->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;

    // Set build range
    prAllocTblInfo->u_range.t_es.ui4_start_pts = 0;
    prAllocTblInfo->u_range.t_es.ui4_end_pts = (prSrcInfo->ui4_total_duration > (UINT32)TWENTY_SEC_DURATION) ?
        (UINT32)(TWENTY_SEC_DURATION) : (prSrcInfo->ui4_total_duration);

    prMediaInfo->u4CurRangePTS = prAllocTblInfo->u_range.t_es.ui4_end_pts;

    if (prAllocTblInfo->u_range.t_es.ui4_end_pts == prSrcInfo->ui4_total_duration)
    {
        prMp4Info->fgLastRange = TRUE;
        prAllocTblInfo->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
    }


    LOG(5, "prMediaInfo->u4CurRangePTS = %ld\n", prMediaInfo->u4CurRangePTS);
    LOG(5, "prSrcInfo->ui4_total_duration = %ld\n", prSrcInfo->ui4_total_duration);

    prAllocTblInfo->pt_tbl_describ = prFirstDescrib;

    zSize = sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T);
    i4Ret = x_midxbuld_set_info(prMp4Info->hMediaIdxHandle, 
    	MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE, (VOID*)prAllocTblInfo, &zSize);
    if (i4Ret != MIDXBULDR_OK)
    {
         return FALSE;
    }

    while(prFirstDescrib != NULL)
    {
        x_midxbuld_start_indexing(prMp4Info->hMediaIdxHandle);
        if (!_PLAYMGR_ReceiveNotify((VOID*)&u4Msg))
        {
            return FALSE;
        }
        
        if (u4Msg != MIDXBULD_NFY_EVENT_INDEXING_DONE)
        {
            return FALSE;
        }
        else
        {
            zSize = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
             prTableInfo->ui4_tbl_id = prFirstDescrib->ui4_tbl_id;
             
             i4Ret = x_midxbuld_get_info(prMp4Info->hMediaIdxHandle, 
            	MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL, prTableInfo, &zSize);
             if (prTableInfo->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type == 
             	MIDXBULD_ELEM_STRM_TYPE_VIDEO)
             {
                  x_memset(prVidElmtStrm, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
                  prMediaInfo->rRangeInfo[0].t_vid_start_pts = 0;
                  prMediaInfo->rRangeInfo[0].t_vid_start_render_pts = 0;
                  prMediaInfo->rRangeInfo[0].ui4_vid_render_counts = prTableInfo->t_elmt_tbl.ui4_number_of_entry;
                  prVidElmtStrm->ui4_number_of_entry = prTableInfo->t_elmt_tbl.ui4_number_of_entry;
                  prVidElmtStrm->e_elem_type = MM_ELEMT_TYPE_VIDEO;
                  prVidElmtStrm->t_strm_id.u.t_stm_mp4_id = prTableInfo->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
                  prVidElmtStrm->u.pt_pb_idx_tbl_entry =  
                    (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T *)prTableInfo->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
                  prVidElmtStrm->pv_next_tbl = NULL;
             }
             else if (prTableInfo->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type == 
             	MIDXBULD_ELEM_STRM_TYPE_AUDIO)
             {
                  x_memset(prAudElmtStrm, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
                  prMediaInfo->rRangeInfo[0].t_aud_start_pts = 0;
                  prMediaInfo->rRangeInfo[0].t_aud_start_render_pts = 0;
                  prMediaInfo->rRangeInfo[0].ui4_aud_render_counts = prTableInfo->t_elmt_tbl.ui4_number_of_entry;
                  prAudElmtStrm->ui4_number_of_entry = prTableInfo->t_elmt_tbl.ui4_number_of_entry;
                  prAudElmtStrm->e_elem_type = MM_ELEMT_TYPE_AUDIO;
                  prAudElmtStrm->t_strm_id.u.t_stm_mp4_id = prTableInfo->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
                  prAudElmtStrm->u.pt_pb_idx_tbl_entry =  
                    (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T *)prTableInfo->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
                  prAudElmtStrm->pv_next_tbl = NULL;
             }
             prFirstDescrib = prFirstDescrib->pt_next;
        }
    }

    if (prMediaInfo->u2VidStmNum > 0)
    {
        prMediaInfo->rRangeInfo[0].pt_idxtbl_lst = prVidElmtStrm;
    }

    if (prMediaInfo->u2AudStmNum > 0)
    {
        (prMediaInfo->u2VidStmNum > 0) ? (prVidElmtStrm->pv_next_tbl = prAudElmtStrm)
        	: (prMediaInfo->rRangeInfo[0].pt_idxtbl_lst = prAudElmtStrm);
    }

    if (prMediaInfo->u2VidStmNum > 0)
    {
        UINT32 i = (prVidElmtStrm->ui4_number_of_entry >= 5) ? (i = 5) : (i = prVidElmtStrm->ui4_number_of_entry);
        UINT32 u4VidPts = prVidElmtStrm->u.pt_pb_idx_tbl_entry[prVidElmtStrm->ui4_number_of_entry-i].ui4_pts;

        for (; i > 0; i--)
        {
            if (u4VidPts < prVidElmtStrm->u.pt_pb_idx_tbl_entry[prVidElmtStrm->ui4_number_of_entry-i].ui4_pts)
            {
                u4VidPts = prVidElmtStrm->u.pt_pb_idx_tbl_entry[prVidElmtStrm->ui4_number_of_entry-i].ui4_pts;
            }
        }
    
        prMediaInfo->u4CurRangePTS = u4VidPts + 1;
    }
    else
    {
        prMediaInfo->u4CurRangePTS = 
            prAudElmtStrm->u.pt_pb_idx_tbl_entry[prAudElmtStrm->ui4_number_of_entry-1].ui4_pts + 1;
    }

    // link key index to the end of the list
    prIdxTbl = prMediaInfo->rRangeInfo[0].pt_idxtbl_lst;
    while (prIdxTbl->pv_next_tbl != NULL)
    {
        prIdxTbl = prIdxTbl->pv_next_tbl;
    }

    // video key table
    if (prKeyIdxTbl->ui4_number_of_entry > 0)
    {
        prKeyIdxTbl->t_strm_id.u.t_stm_avi_id = prVidElmtStrm->t_strm_id.u.t_stm_avi_id;
        prKeyIdxTbl->pv_next_tbl = NULL;
        prIdxTbl->pv_next_tbl = prKeyIdxTbl;
        //prIdxTbl = prIdxTbl->pv_next_tbl;
    }

    prMediaInfo->u1LeftRangeNum++;

    //set end of stream info
    if(prMediaInfo->u4CurRangePTS >= prSrcInfo->ui4_total_duration)
    {
        prMediaInfo->rRangeInfo[0].b_endofstream = TRUE;
    }
    else
    {
        prMediaInfo->rRangeInfo[0].ui4_vid_render_counts = 0;
        prMediaInfo->rRangeInfo[0].b_endofstream = FALSE;
    }

    prMediaInfo->rRangeInfo[0].ui4_range_id = prMp4Info->rAllocTblInfo[0].ui4_range_id;
    prMediaInfo->rRangeInfo[0].ui8_pb_duration = prSrcInfo->ui4_total_duration;

    prMediaInfo->rRangeInfo[0].z_range_sz = prSrcInfo->ui8_data_size;

    SWDMX_AddRange(&prMediaInfo->rRangeInfo[0], 
        prMediaInfo->rRangeInfo[0].ui4_range_id);

    
    if (prMediaInfo->u4CurRangePTS < prSrcInfo->ui4_total_duration)
    {
        _PLAYMGR_Mp4UpdateRange(pvMediaInfo, PLAYMGR_UNKNOWN_SEEKPTS);
    }

    return SWDMX_Play();
}


BOOL _PLAYMGR_Mp4Stop(VOID* pvMediaInfo)
{
    //BOOL fgRet = TRUE;
    //UINT32 u4SwdmxCurStas;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    //return SWDMX_Stop();

    /*u4SwdmxCurStas = SWDMX_QueryStatus();
    while (u4SwdmxCurStas != eSWDMX_STATE_STOP)
    {
        u4SwdmxCurStas = SWDMX_QueryStatus();
        x_thread_delay(1);
    }

    fgRet = _SWDMX_Add_or_Del_Range(&prMediaInfo->rRangeInfo[prMediaInfo->u1CurInUseRange], 
                                                                SWDMX_DEL_RANGE, 
                                                                prMediaInfo->rRangeInfo[prMediaInfo->u1CurInUseRange].pv_tag);
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_Mp4Stop Fail in _SWDMX_Stop\n");
        return FALSE;
    }*/
    
    prMediaInfo->FmtInfo.rMp4Info.fgLastRange = FALSE;
    return TRUE;
}

EXTERN BOOL _PLAYMGR_Mp4Close(VOID* pvMediaInfo)
{
    UINT32 u4Msg;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    if (prMediaInfo->FmtInfo.rMp4Info.hMediaInfoHandle != NULL)
    {
        x_minfo_close(prMediaInfo->FmtInfo.rMp4Info.hMediaInfoHandle);
        if (!_PLAYMGR_ReceiveNotify((VOID*)&u4Msg))
        {
            return FALSE;
        }
        
        if (u4Msg != MINFO_NFY_EVENT_HANDLE_CLOSED)
        {
            return FALSE;
        }
    }
    if (prMediaInfo->FmtInfo.rMp4Info.hMediaIdxHandle)
    {
        x_midxbuld_close(prMediaInfo->FmtInfo.rMp4Info.hMediaIdxHandle);
        if (!_PLAYMGR_ReceiveNotify((VOID*)&u4Msg))
        {
            return FALSE;
        }
        
        if (u4Msg != MIDXBULD_NFY_EVENT_HANDLE_CLOSED)
        {
            return FALSE;
        }
    }

    x_memset(&prMediaInfo->FmtInfo.rMp4Info, 0, sizeof(PLAYMGR_MP4_INFO_T));

    return TRUE;
}

EXTERN BOOL _PLAYMGR_Mp4Resume(VOID* pvMediaInfo)
{
    SWDMX_Play();
    return TRUE;
}

BOOL _PLAYMGR_Mp4Pause(VOID)
{
    SWDMX_Pause();
    return TRUE;
}


BOOL _PLAYMGR_OpenMp4MediaInfo(PLAYMGR_STATUS_T* prMediaStatus, VOID* pvMediaInfo)
{
    INT32 i4Ret;
    UINT32 u4Msg;
   
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_MP4_INFO_T* prMp4Info = 
    	(PLAYMGR_MP4_INFO_T*)&prMediaInfo->FmtInfo.rMp4Info;
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt; 

    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prMp4Info->rSrcInfo;

    prMediaInfo->u1CurEmptyRange = 1;

    // Open for media info
    i4Ret = x_minfo_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _PLAYMGR_Minfo_Notify, prMediaInfo,
            		       prMediaFmt, &(prMp4Info->hMediaInfoHandle));
    if(i4Ret != MINFOR_OK)
    {
        return FALSE;
    }

    if (!_PLAYMGR_ReceiveNotify((VOID*)&u4Msg))
    {
        return FALSE;
    }
    
    if (u4Msg != MINFO_NFY_EVENT_PARSING_DONE)
    {
        return FALSE;
    }

    // Wait for media info's notify
    prSrcInfo->ui8_data_offset = prMp4Info->rPlayerIdxInfo.ui8_data_offset;
    prSrcInfo->ui8_data_size = prMp4Info->rPlayerIdxInfo.ui8_data_sz;
    prSrcInfo->ui8_idx_offset = prMp4Info->rPlayerIdxInfo.ui8_idx_offset;
    prSrcInfo->ui8_idx_size = prMp4Info->rPlayerIdxInfo.ui8_idx_sz;
    prSrcInfo->pt_mp4_cmov_info = prMp4Info->rPlayerIdxInfo.pt_mp4_cmov_info;
    prSrcInfo->ui1_total_strm_num = (UINT8)(prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum);
    // Temporally multiply SYSTEM_TIME_SCALE
    prSrcInfo->ui4_total_duration = (UINT32)prMp4Info->rPlayerGenericInfo.ui8_pb_duration;
    //SWDMX_SetInfo(eSWDMX_TOTAL_DURATION, prSrcInfo->ui4_total_duration, 0, 0);
    prMediaStatus->u4TotalDuration = (UINT32)prMp4Info->rPlayerGenericInfo.ui8_pb_duration;
    prMediaInfo->rPlayerStatus.u4TotalDuration = (UINT32)prMp4Info->rPlayerGenericInfo.ui8_pb_duration;

    if (_PLAYMGR_SetMP4Info(prMediaStatus, prMediaInfo) != TRUE)
    {
        return FALSE;
    }

    return TRUE;
}


BOOL _PLAYMGR_Mp4UpdateRange(VOID* pvMediaInfo, UINT32 u4Param1)
{
    INT32 i4Ret, i4RangeIdx = 0;
    UINT32 u4Msg;
    SIZE_T zSize;

    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_MP4_INFO_T* prMp4Info = 
    	(PLAYMGR_MP4_INFO_T*)&prMediaInfo->FmtInfo.rMp4Info;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prMp4Info->rSrcInfo;
    MIDXBULD_ELMT_TBL_INFO_T* prTableInfo = &prMp4Info->rTableInfo;

    MIDXBULD_ELMT_TBL_ALLOC_INFO_T* prAllocTblInfo = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T* prVDescrib = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T* prADescrib = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T* prFirstDescrib = NULL;

    MM_RANGE_ELMT_IDXTBL_T* prVidElmtStrm = NULL;
    MM_RANGE_ELMT_IDXTBL_T* prAudElmtStrm = NULL;
    
    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTbl = NULL;
    MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;

    i4RangeIdx = prMediaInfo->u1CurEmptyRange;
    LOG(5, "Current update range = %d\n", i4RangeIdx);

    if (i4RangeIdx < MAX_RANGE_NUM)
    {
        prAllocTblInfo = &prMp4Info->rAllocTblInfo[i4RangeIdx];
        prVDescrib = &prMp4Info->rVDescrib[i4RangeIdx];
        prADescrib = &prMp4Info->rADescrib[i4RangeIdx];

        prVidElmtStrm = &prMp4Info->rVidIDXTBL[i4RangeIdx];
        prAudElmtStrm = &prMp4Info->rAudIDXTBL[i4RangeIdx];

        x_memset(prVidElmtStrm, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
        x_memset(prAudElmtStrm, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
    }
    else
    {
        return FALSE;
    }
    
    prKeyIdxTbl = &prMp4Info->rKeyIdxTBL;

    if ((prMediaInfo->u2AudStmNum > 0) &&
        (prMp4Info->u2AudIdx < MIDXBULD_ELMT_TBL_MAX))
    {
        prFirstDescrib = prADescrib;
                                               
        prADescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        prADescrib->u_info.t_es.ui4_strm_id = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_strm_id;
        prADescrib->u_info.t_es.ui4_scale = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_scale;
        prADescrib->u_info.t_es.ui4_rate = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_rate;
        prADescrib->u_info.t_es.ui4_sample_sz = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_sample_sz ;
        prADescrib->u_info.t_es.ui4_avg_bytes_per_sec = 
        	prSrcInfo->t_es_map[prMp4Info->u2AudIdx].ui4_avg_bytes_per_sec;
        
        if(prMp4Info->u2AudIdx < 3)
        {
             prVDescrib->u_info.t_es.b_is_cur_strm_codec_spt = 
                  prMp4Info->rStmAttribute[prMp4Info->u2AudIdx].b_is_codec_support;
        }
        prADescrib->pt_next = NULL;
    }

    if (prMediaInfo->u2VidStmNum > 0)
    {
        (prFirstDescrib == NULL) ? (prFirstDescrib = prVDescrib) : 
                                               (prFirstDescrib->pt_next = prVDescrib);
                                               
        prVDescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        prVDescrib->u_info.t_es.ui4_strm_id = prSrcInfo->t_es_map[0].ui4_strm_id;
        prVDescrib->u_info.t_es.ui4_scale = prSrcInfo->t_es_map[0].ui4_scale;
        prVDescrib->u_info.t_es.ui4_rate = prSrcInfo->t_es_map[0].ui4_rate;
        prVDescrib->u_info.t_es.ui4_sample_sz = 0;
        prVDescrib->u_info.t_es.ui4_avg_bytes_per_sec = 0;
         prVDescrib->u_info.t_es.b_is_cur_strm_codec_spt = 
            prMp4Info->rStmAttribute[0].b_is_codec_support;
        prVDescrib->pt_next = NULL;
    }
    
    prAllocTblInfo->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;

    if (u4Param1 < PLAYMGR_UNKNOWN_SEEKPTS)
    {
        prMediaInfo->u4CurRangePTS = u4Param1;
        prMp4Info->fgLastRange = FALSE;
    }

    LOG(5, "u4CurRangePTS = 0x%08x, Total Duration = 0x%08x\n", 
        prMediaInfo->u4CurRangePTS,
        prSrcInfo->ui4_total_duration);


    if ((prMediaInfo->u4CurRangePTS >= prSrcInfo->ui4_total_duration) ||
        (prMp4Info->fgLastRange))
    {
        return FALSE;
    }

#if 0
    // Set build range
    if(prMediaInfo->u4SeekPts < (UINT32)0xFFFFFFFF)
    {
        prMediaInfo->u4CurRangePTS = prMediaInfo->u4SeekPts;
        prMediaInfo->u4SeekPts = 0xFFFFFFFF;
    }
#endif 

    prAllocTblInfo->u_range.t_es.ui4_start_pts = prMediaInfo->u4CurRangePTS;
    prAllocTblInfo->u_range.t_es.ui4_end_pts = prMediaInfo->u4CurRangePTS;
    prAllocTblInfo->u_range.t_es.ui4_end_pts += ((prSrcInfo->ui4_total_duration - prMediaInfo->u4CurRangePTS) > TWENTY_SEC_DURATION) ?
        (TWENTY_SEC_DURATION) : (prSrcInfo->ui4_total_duration- prMediaInfo->u4CurRangePTS);

    if (prAllocTblInfo->u_range.t_es.ui4_end_pts == prSrcInfo->ui4_total_duration)
    {
        prMp4Info->fgLastRange = TRUE;
        prAllocTblInfo->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
    }

    //prMediaInfo->u4CurRangePTS = prAllocTblInfo->u_range.t_es.ui4_end_pts;
    prAllocTblInfo->pt_tbl_describ = prFirstDescrib;

    zSize = sizeof(MIDXBULD_ELMT_TBL_ALLOC_INFO_T);
    i4Ret = x_midxbuld_set_info(prMp4Info->hMediaIdxHandle, 
    	MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE, (VOID*)prAllocTblInfo, &zSize);
    if (i4Ret != MIDXBULDR_OK)
    {
        LOG(3, "_PLAYMGR_Mp4UpdateRange Fail to allocate range\n");
         return FALSE;
    }

    while(prFirstDescrib != NULL)
    {
        x_midxbuld_start_indexing(prMp4Info->hMediaIdxHandle);
        if (!_PLAYMGR_ReceiveNotify((VOID*)&u4Msg))
        {
            return FALSE;
        }
        
        if (u4Msg != MIDXBULD_NFY_EVENT_INDEXING_DONE)
        {
            LOG(5, "_PLAYMGR_Mp4UpdateRange !MIDXBULD_NFY_EVENT_INDEXING_DONE\n");
            return FALSE;
        }
        else
        {
             zSize = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
             prTableInfo->ui4_tbl_id = prFirstDescrib->ui4_tbl_id;
             
             i4Ret = x_midxbuld_get_info(prMp4Info->hMediaIdxHandle, 
            	MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL, prTableInfo, &zSize);
            if ((prTableInfo->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO) &&
                (i4RangeIdx < MAX_RANGE_NUM))
            {
                x_memset(prVidElmtStrm, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
                prMediaInfo->rRangeInfo[i4RangeIdx].t_vid_start_pts = 0;
                prMediaInfo->rRangeInfo[i4RangeIdx].t_vid_start_render_pts = 0;
                prMediaInfo->rRangeInfo[i4RangeIdx].ui4_vid_render_counts = prTableInfo->t_elmt_tbl.ui4_number_of_entry;
                prVidElmtStrm->ui4_number_of_entry = prTableInfo->t_elmt_tbl.ui4_number_of_entry;
                prVidElmtStrm->e_elem_type = MM_ELEMT_TYPE_VIDEO;
                prVidElmtStrm->t_strm_id.u.t_stm_mp4_id = prTableInfo->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
                prVidElmtStrm->u.pt_pb_idx_tbl_entry =  
                    (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T *)prTableInfo->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
                prVidElmtStrm->pv_next_tbl = NULL;
            }
            else if ((prTableInfo->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO) &&
                (i4RangeIdx < MAX_RANGE_NUM))
            {
                x_memset(prAudElmtStrm, 0, sizeof(MM_RANGE_ELMT_IDXTBL_T));
                prMediaInfo->rRangeInfo[i4RangeIdx].t_aud_start_pts = 0;
                prMediaInfo->rRangeInfo[i4RangeIdx].t_aud_start_render_pts = 0;
                prMediaInfo->rRangeInfo[i4RangeIdx].ui4_aud_render_counts = prTableInfo->t_elmt_tbl.ui4_number_of_entry;
                prAudElmtStrm->ui4_number_of_entry = prTableInfo->t_elmt_tbl.ui4_number_of_entry;
                prAudElmtStrm->e_elem_type = MM_ELEMT_TYPE_AUDIO;
                prAudElmtStrm->t_strm_id.u.t_stm_mp4_id = prTableInfo->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
                prAudElmtStrm->u.pt_pb_idx_tbl_entry =  
                    (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T *)prTableInfo->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
                prAudElmtStrm->pv_next_tbl = NULL;
            }
            prFirstDescrib = prFirstDescrib->pt_next;
        }
    }

    if ((prMediaInfo->u2VidStmNum > 0) &&
        (prVidElmtStrm->ui4_number_of_entry > 0))
    {
        prMediaInfo->rRangeInfo[i4RangeIdx].pt_idxtbl_lst = prVidElmtStrm;
    }

    if ((prMediaInfo->u2AudStmNum > 0) &&
        (prAudElmtStrm->ui4_number_of_entry > 0))
    {
        (prMediaInfo->u2VidStmNum > 0) ? (prVidElmtStrm->pv_next_tbl = prAudElmtStrm)
        	: (prMediaInfo->rRangeInfo[i4RangeIdx].pt_idxtbl_lst = prAudElmtStrm);
    }

    if ((prMediaInfo->u2VidStmNum > 0) &&
        (prVidElmtStrm->ui4_number_of_entry > 0))
    {
        UINT32 i = (prVidElmtStrm->ui4_number_of_entry >= 5) ? (i = 5) : (i = prVidElmtStrm->ui4_number_of_entry);
        UINT32 u4VidPts = prVidElmtStrm->u.pt_pb_idx_tbl_entry[prVidElmtStrm->ui4_number_of_entry-i].ui4_pts;

        for (; i > 0; i--)
        {
            if (u4VidPts < prVidElmtStrm->u.pt_pb_idx_tbl_entry[prVidElmtStrm->ui4_number_of_entry-i].ui4_pts)
            {
                u4VidPts = prVidElmtStrm->u.pt_pb_idx_tbl_entry[prVidElmtStrm->ui4_number_of_entry-i].ui4_pts;
            }
        }
    
        prMediaInfo->u4CurRangePTS = u4VidPts + 1;
    }
    else
    {
        prMediaInfo->u4CurRangePTS = 
            prAudElmtStrm->u.pt_pb_idx_tbl_entry[prAudElmtStrm->ui4_number_of_entry-1].ui4_pts + 1;
    }


    // link key index to the end of the list
    if (i4RangeIdx < MAX_RANGE_NUM)
    {
        prIdxTbl = prMediaInfo->rRangeInfo[i4RangeIdx].pt_idxtbl_lst;
        while (prIdxTbl->pv_next_tbl != NULL)
        {
            prIdxTbl = prIdxTbl->pv_next_tbl;
        }
    }

    // video key table
    if (prKeyIdxTbl->ui4_number_of_entry > 0)
    {
        prKeyIdxTbl->t_strm_id.u.t_stm_avi_id = prVidElmtStrm->t_strm_id.u.t_stm_avi_id;
        prKeyIdxTbl->pv_next_tbl = NULL;
        prIdxTbl->pv_next_tbl = prKeyIdxTbl;
        //prIdxTbl = prIdxTbl->pv_next_tbl;
    }
    
    //LOG(0, "Allocate prMp4Info->rAllocTblInfo[i].ui4_range_id = %ld\n", prMp4Info->rAllocTblInfo[i].ui4_range_id);

    prMediaInfo->u1LeftRangeNum++;

    if (i4RangeIdx < MAX_RANGE_NUM)
    {
        prMediaInfo->rRangeInfo[i4RangeIdx].ui4_range_id = prMp4Info->rAllocTblInfo[i4RangeIdx].ui4_range_id;
        prMediaInfo->rRangeInfo[i4RangeIdx].ui8_pb_duration = prSrcInfo->ui4_total_duration;

        //set end of stream info
        //LOG(0, "PTS 0x%x, total 0x%x\n", prMediaInfo->u4CurRangePTS ,prSrcInfo->ui4_total_duration);
        if(prMp4Info->fgLastRange)
        {
            //LOG(0, "Last Range\n");
            prMediaInfo->rRangeInfo[i4RangeIdx].b_endofstream = TRUE;
        }
        else
        {
            prMediaInfo->rRangeInfo[i4RangeIdx].ui4_vid_render_counts = 0;
            prMediaInfo->rRangeInfo[i4RangeIdx].b_endofstream = FALSE;
        }

        prMediaInfo->rRangeInfo[i4RangeIdx].z_range_sz = prSrcInfo->ui8_data_size;
        SWDMX_AddRange(&prMediaInfo->rRangeInfo[i4RangeIdx], 
            prMediaInfo->rRangeInfo[i4RangeIdx].ui4_range_id);
    }
    else
    {
        LOG(1, "Out of Max Range Number\n");
        return FALSE;
    }
        
    return TRUE;
}


BOOL _PLAYMGR_Mp4FreeRange(VOID* pvMediaInfo, UINT32 u4RangeTag)
{
    UINT8 i;
    INT32 i4Ret;
    SIZE_T zSize;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_MP4_INFO_T* prMp4Info = 
    	(PLAYMGR_MP4_INFO_T*)&prMediaInfo->FmtInfo.rMp4Info;

    LOG(5, "_PLAYMGR_Mp4FreeRange left range number = 0x%08x \n", prMediaInfo->u1LeftRangeNum);
    for (i = 0; i < MAX_RANGE_NUM; i++)
    {
        if (prMediaInfo->rRangeInfo[i].ui4_range_id == u4RangeTag)
        {
            zSize = sizeof(UINT32);
            LOG(5, "free range prMp4Info->rAllocTblInfo[%d].ui4_range_id = 0x%08x\n", 
                i,
                prMp4Info->rAllocTblInfo[i].ui4_range_id);

            i4Ret = x_midxbuld_set_info(prMediaInfo->FmtInfo.rMp4Info.hMediaIdxHandle, 
                                 MIDXBULD_SET_INFO_TYPE_FREE_RANGE, 
                                 (VOID*)prMp4Info->rAllocTblInfo[i].ui4_range_id, 
                                 &zSize);
            if (i4Ret == MIDXBULDR_INV_HANDLE)
            {
                LOG(5, "free same range id twice!!!!\n");
            }

            if (i4Ret != MIDXBULDR_OK)
            {
                LOG(5, "Fail to free range\n");
                return FALSE;
            }

            prMediaInfo->u1CurEmptyRange = i;
            prMediaInfo->u1LeftRangeNum--;
            x_memset(&prMediaInfo->rRangeInfo[i], 0, sizeof(MM_RANGE_INFO_T));
        }
    }
    UNUSED(i4Ret);
    return TRUE;
}


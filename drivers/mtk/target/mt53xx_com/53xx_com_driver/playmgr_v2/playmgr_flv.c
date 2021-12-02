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
 * $RCSfile: playmgr_flv.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

#if !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)

#include "playmgr.h"
#include "feeder_if.h"
#include "playmgr_util.h"
#include "swdmx_drvif.h"
#include "playmgr_flv.h"
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
//#define FLV_DONOT_BUILDINDEX
#ifdef USE_TEST_CASE

//#define USE_TEST_CASE
static UINT32 u4CaseIndex=0;
#define flvmax(x, y) ((x > y) ? x : y)
#include "flv_case.h"

static BOOL _PLAYMGR_SetCaseInfo(PLAYMGR_INFO_T *prMediaInfo,UINT32 u4CaseNumber)
{
  PLAYMGR_FLV_INFO_T* prFlvInfo = &prMediaInfo->FmtInfo.rFlvInfo;
  p_flvfile_info prFlvFileInfo=&rFlvFileInfo[u4CaseNumber];
  prMediaInfo->u2VidStmNum=1;
  prMediaInfo->u2AudStmNum=1;
  prFlvInfo->u2VidIdx=0;
  prFlvInfo->u2AudIdx=1;
  prFlvInfo->rStmAttribute[prFlvInfo->u2VidIdx].t_stm_id.u.t_flv_stm_id=0;
  prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].t_stm_id.u.t_flv_stm_id=1;
  prFlvInfo->rStmAttribute[prFlvInfo->u2VidIdx].b_is_codec_support=TRUE;
  prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].b_is_codec_support=TRUE;
  prFlvInfo->rPlayerGenericInfo.ui8_timecode_scale=1;
  prFlvInfo->rPlayerGenericInfo.ui8_media_sz=prFlvFileInfo->u4FileSize;
  x_memcpy(&(prFlvInfo->rStmAttribute[prFlvInfo->u2VidIdx].u.t_flv_video_stm_attr),&prFlvFileInfo->rVidAtr,sizeof(MINFO_FLV_VIDEO_STM_ATRBT_T));
  x_memcpy(&(prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].u.t_flv_audio_stm_attr),&prFlvFileInfo->rAudAtr,sizeof(MINFO_FLV_AUDIO_STM_ATRBT_T));
  prFlvInfo->rPlayerGenericInfo.ui8_pb_duration=flvmax(prFlvInfo->rStmAttribute[prFlvInfo->u2VidIdx].u.t_flv_video_stm_attr.ui8_duration,prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].u.t_flv_audio_stm_attr.ui8_duration);
  prFlvInfo->rKeyIdxTBL.ui4_scale=prFlvFileInfo->rVidAtr.ui4_scale;
  prFlvInfo->rKeyIdxTBL.ui4_rate=prFlvFileInfo->rVidAtr.ui4_rate;
  return TRUE;
}

static INT32 _PLAYMGR_BuildCaseIdx(MIDXBULD_KEY_TBL_INFO_T *prTableInfo,UINT32 u4CaseNumber)
{
   MIDXBULD_KEY_TBL_EXT_T * pt_key_tbl_ext=NULL;
   MIDXBULD_ELMT_TBL_OBJ_T *p_elm_obj=NULL;
   MIDXBULD_ELMT_STRM_ENTRY_T *prIdxEntry=NULL;
   p_flvfile_info prFlvFileInfo=&rFlvFileInfo[u4CaseNumber];
   UINT32 u4IdxNum=0;
   u4IdxNum=prFlvFileInfo->u4IndexNum;
   prIdxEntry=prFlvFileInfo->prIdxEntry;
   
   if(u4IdxNum>0 && prIdxEntry)
   {
	 pt_key_tbl_ext=(MIDXBULD_KEY_TBL_EXT_T *)x_mem_alloc(sizeof(MIDXBULD_KEY_TBL_EXT_T));
	 if(!pt_key_tbl_ext)
	   return 1;
	 pt_key_tbl_ext->pt_next=NULL;
	 p_elm_obj=&pt_key_tbl_ext->t_elmt_tbl;
     
	 p_elm_obj->ui8_base_offset=0;
	 p_elm_obj->ui4_number_of_entry=u4IdxNum;
	 p_elm_obj->e_tbl_type=MIDXBULD_TBL_TYPE_ELMT_STRM;
	 p_elm_obj->u_fmt.t_es.t_info.e_es_type=MIDXBULD_ELEM_STRM_TYPE_KEY;
	 p_elm_obj->u_fmt.t_es.t_info.ui4_strm_id=0;
	 p_elm_obj->u_fmt.t_es.t_info.ui4_scale=1;
	 p_elm_obj->u_fmt.t_es.pt_idx_entry=prIdxEntry;
	 p_elm_obj->pt_next=NULL;
	 prTableInfo->pt_key_tbl_ext=pt_key_tbl_ext;
   }
   else
   {
     return 1;
   }
   return 0;
}

#endif

BOOL PLAYMGR_FlvSetTestCase(INT32 i4Index)
{
#ifdef USE_TEST_CASE
   if(i4Index>=FLV_FILE_CASE_NUM)
    return FALSE;
   LOG(0,"Index=%d,Name=%s\n",i4Index,rFlvFileInfo[i4Index].szName);
   u4CaseIndex=i4Index;
#endif
   return TRUE;
}


static BOOL _PLAYMGR_SetFlvInfo(PLAYMGR_INFO_T* prMediaInfo)
{
    PLAYMGR_FLV_INFO_T* prFlvInfo = &prMediaInfo->FmtInfo.rFlvInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prFlvInfo->rSrcInfo;
    PLAYMGR_STATUS_T* prMediaStatus=&prMediaInfo->rPlayerStatus;
#ifndef __MODEL_slt__
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prFlvInfo->rKeyInfo;
    MIDXBULD_ELMT_TBL_DESCRIB_T* prVDescrib = &prFlvInfo->rVDescrib[0];
    MIDXBULD_ELMT_TBL_DESCRIB_T* prADescrib = &prFlvInfo->rADescrib[0];
#endif
	
    prMediaStatus->eMediaType = PLAYMGR_MEDIA_TYPE_FLV;

#ifndef __MODEL_slt__
    prMediaStatus->u8FileSize = prFlvInfo->rPlayerGenericInfo.ui8_media_sz;
    prMediaStatus->u4TotalDuration = (UINT32)prFlvInfo->rPlayerGenericInfo.ui8_pb_duration;
     // Wait for media info's notify

/*
	prSrcInfo->ui4_total_duration=prMediaStatus->u4TotalDuration;
	prSrcInfo->ui8_data_size =   prFlvInfo->rPlayerGenericInfo.ui8_media_sz;
	prSrcInfo->ui8_data_offset=0;
    prSrcInfo->ui1_total_strm_num = (UINT8)(prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum);
*/
            
    if(prMediaInfo->u2VidStmNum >0 && prFlvInfo->u2VidIdx<2)
    {
        prMediaStatus->u2VideoTracks = prMediaInfo->u2VidStmNum;
        LOG(5, "Number of video tracks in this flv is %ld\n", prMediaStatus->u2VideoTracks);
        switch (prFlvInfo->rStmAttribute[prFlvInfo->u2VidIdx].u.t_flv_video_stm_attr.e_enc)
        {
            case MINFO_INFO_VID_ENC_MPEG_4: 
		    case MINFO_INFO_VID_ENC_SORENSON:
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_MPEG4;
                break;
            case MINFO_INFO_VID_ENC_H264:
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_H264;
                break;
            case MINFO_INFO_VID_ENC_MJPG:
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_MJPEG;
				break;
			case MINFO_INFO_VID_ENC_VP6:
                prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_VP6;
				break;
            default:
                return FALSE;
        }
    }

    if (prMediaInfo->u2AudStmNum > 0 && prFlvInfo->u2AudIdx<2)
    {
        prMediaStatus->u2AudioTracks = prMediaInfo->u2AudStmNum;
        LOG(5, "Number of audio tracks in this flv is %ld\n", prMediaStatus->u2AudioTracks);
        switch (prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].u.t_flv_audio_stm_attr.e_enc)
        {
			case MINFO_INFO_AUD_ENC_MPEG_1:
                prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_MP3;
                break;
            case MINFO_INFO_AUD_ENC_AAC:
                prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_AAC;
                break;
            case MINFO_INFO_AUD_ENC_PCM:
			case MINFO_INFO_AUD_ENC_ADPCM:
			case MINFO_INFO_AUD_ENC_LPCM:
                prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_PCM;
                break;
            default:
            	return FALSE;
        }
    }

    if(prMediaInfo->u2VidStmNum>0 && prFlvInfo->u2VidIdx<2)
    {
	    prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
		prMediaStatus->au4VideoId[0] = prFlvInfo->rStmAttribute[prFlvInfo->u2VidIdx].t_stm_id.u.t_flv_stm_id;
		prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_strm_id =prFlvInfo->rStmAttribute[prFlvInfo->u2VidIdx].t_stm_id.u.t_flv_stm_id;
		prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_scale = 1;
		prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_rate = prFlvInfo->rStmAttribute[prFlvInfo->u2VidIdx].u.t_flv_video_stm_attr.ui4_rate;
		prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_sample_sz = 0;
		prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_avg_bytes_per_sec = 0;
		prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_indx_offset=0;
		prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_indx_size=0;
    }
    
    if(prMediaInfo->u2AudStmNum>0 && prFlvInfo->u2AudIdx<2)
    {
		 prMediaStatus->au4AudioId[0]= prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].t_stm_id.u.t_flv_stm_id;
		 prSrcInfo->t_es_map[prFlvInfo->u2AudIdx].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
		 prSrcInfo->t_es_map[prFlvInfo->u2AudIdx].ui4_strm_id = prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].t_stm_id.u.t_flv_stm_id;
		 prSrcInfo->t_es_map[prFlvInfo->u2AudIdx].ui4_scale = 1;
		 prSrcInfo->t_es_map[prFlvInfo->u2AudIdx].ui4_rate = prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].u.t_flv_audio_stm_attr.i4_samples_per_sec;
		 prSrcInfo->t_es_map[prFlvInfo->u2AudIdx].ui4_sample_sz = prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].u.t_flv_audio_stm_attr.i2_bits_per_sample;
		 prSrcInfo->t_es_map[prFlvInfo->u2AudIdx].ui4_avg_bytes_per_sec = prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].u.t_flv_audio_stm_attr.ui4_avg_bytes_per_sec;
   }
 
    if (prMediaInfo->u2VidStmNum>0 && prFlvInfo->u2VidIdx<2)
    {
        prKeyInfo->ui4_strm_id = prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_strm_id;
        prKeyInfo->ui4_rate = prFlvInfo->rStmAttribute[prFlvInfo->u2VidIdx].u.t_flv_video_stm_attr.ui4_rate;
        prKeyInfo->ui4_avg_bytes_per_sec = 0;
		prKeyInfo->ui4_scale=prFlvInfo->rStmAttribute[prFlvInfo->u2VidIdx].u.t_flv_video_stm_attr.ui4_scale;
	}

    if (prMediaInfo->u2VidStmNum >0 && prFlvInfo->u2VidIdx<2)
    {		
        prVDescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        prVDescrib->u_info.t_es.ui4_strm_id = prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_strm_id;
        prVDescrib->u_info.t_es.ui4_rate = prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_rate;
        prVDescrib->u_info.t_es.ui4_sample_sz = 0;
        prVDescrib->u_info.t_es.ui4_avg_bytes_per_sec = 0;
        prVDescrib->u_info.t_es.ui4_indx_offset= prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_indx_offset;
        prVDescrib->u_info.t_es.ui4_indx_size  = prSrcInfo->t_es_map[prFlvInfo->u2VidIdx].ui4_indx_size;
    }

    if (prMediaInfo->u2AudStmNum > 0 && prFlvInfo->u2AudIdx<2)
    {
        prADescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        prADescrib->u_info.t_es.ui4_strm_id = prSrcInfo->t_es_map[prFlvInfo->u2AudIdx].ui4_strm_id;
        prADescrib->u_info.t_es.ui4_scale = prSrcInfo->t_es_map[prFlvInfo->u2AudIdx].ui4_scale;
        prADescrib->u_info.t_es.ui4_rate = prSrcInfo->t_es_map[prFlvInfo->u2AudIdx].ui4_rate;
        prADescrib->u_info.t_es.ui4_sample_sz = prSrcInfo->t_es_map[prFlvInfo->u2AudIdx].ui4_sample_sz ;
        prADescrib->u_info.t_es.ui4_avg_bytes_per_sec = prSrcInfo->t_es_map[prFlvInfo->u2AudIdx].ui4_avg_bytes_per_sec;
    }
#else
    prMediaStatus->u8FileSize = 54218;
    prMediaStatus->u4TotalDuration = 90000;
    prMediaInfo->rRangeInfo[0].ui8_vid_duration = 90000;
    
    prMediaStatus->u2CaptionTracks = 0;
    
    prMediaInfo->u2VidStmNum = 1;   
    prMediaStatus->u2VideoTracks = 1;
    prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_VP6;
    prMediaStatus->au4VideoId[0] = 1;        
    prSrcInfo->t_es_map[0].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
    prSrcInfo->t_es_map[0].ui4_strm_id = 1;
    prSrcInfo->t_es_map[0].ui4_scale = 1;
    prSrcInfo->t_es_map[0].ui4_rate = 30;
    prSrcInfo->t_es_map[0].ui4_sample_sz = 0;
    prSrcInfo->t_es_map[0].ui4_avg_bytes_per_sec = 0;
    prSrcInfo->t_es_map[0].ui4_indx_offset = 0;
    prSrcInfo->t_es_map[0].ui4_indx_size = 0;

    prMediaInfo->u2AudStmNum = 0;
    prMediaStatus->u2AudioTracks = 0;
/*
    prMediaStatus->u2AudioTracks = 1;
    prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_MP3;
    prMediaStatus->au4AudioId[0] = 0;
    prSrcInfo->t_es_map[1].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
    prSrcInfo->t_es_map[1].ui4_strm_id = 0;
    prSrcInfo->t_es_map[1].ui4_scale = 1;
    prSrcInfo->t_es_map[1].ui4_rate = 0x5DC0;
    prSrcInfo->t_es_map[1].ui4_sample_sz = 1;
    prSrcInfo->t_es_map[1].ui4_avg_bytes_per_sec = 0;
*/
#endif

    // Backup the media information
    x_memcpy(&prMediaInfo->rPlayerStatus, prMediaStatus, sizeof(PLAYMGR_STATUS_T));
    
    return TRUE;
}

#ifndef USE_TEST_CASE
    #ifndef __MODEL_slt__
static void _PLAYMGR_FlvMinfo_Notify(
                           HANDLE_T                    hMinfo,                 
                           MINFO_NFY_EVENT_TYPE_T      e_event,  
                           const VOID*                 pvData1,
                           const VOID*                 pvData2,
                           const VOID*                 pvTag) 

{
    SIZE_T zRecLength;
    INT32 i4Ret;  
    MINFO_INFO_T rMinfoRec;
    PLAYMGR_INFO_T *prMediaInfo;    
    PLAYMGR_FLV_INFO_T* prFlvInfo;
    MINFO_NFY_EVENT_TYPE_T      eEvent = e_event;
    
    zRecLength  = sizeof(MINFO_INFO_T);
    prMediaInfo =(PLAYMGR_INFO_T*)pvTag;
    prFlvInfo   = &prMediaInfo->FmtInfo.rFlvInfo;

    switch(e_event) {
    case MINFO_NFY_EVENT_PARSING_DONE:
        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_GENERIC, 1,&rMinfoRec, &zRecLength);
    	if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
	    {
	        LOG(3, "get media geberic info error\n");
	        eEvent = MINFO_NFY_EVENT_ERROR;
	        break;
    	}
	    x_memcpy(&prFlvInfo->rPlayerGenericInfo, &(rMinfoRec.u.t_general_info),sizeof(MINFO_TYPE_GENERIC_T));

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
        x_memcpy(&prFlvInfo->rPlayerIdxInfo, &(rMinfoRec.u.t_idx_info),
	 	          sizeof(MINFO_TYPE_IDX_INFO_T));
        
        i4Ret = x_minfo_get_num_recs(hMinfo, MINFO_INFO_TYPE_VIDEO_ATTR,&prMediaInfo->u2VidStmNum);

		if (i4Ret != MINFOR_OK)
        {
            LOG(1, "can't get media video recs\n");
        }

		if(prMediaInfo->u2VidStmNum>0)
		{
			prFlvInfo->u2VidIdx=0;
			i4Ret = x_minfo_get_info(hMinfo,MINFO_INFO_TYPE_VIDEO_ATTR, (MINFO_REC_IDX_T)(1), &rMinfoRec, &zRecLength);
			if (i4Ret != MINFOR_OK)
			{
			   LOG(1, "get media video info error\n");
			   eEvent = MINFO_NFY_EVENT_ERROR;
			   break;
			}
			
			x_memcpy(&prFlvInfo->rStmAttribute[0], &(rMinfoRec.u.t_stm_attribute),sizeof(MINFO_TYPE_STM_ATRBT_T));
		}


		i4Ret = x_minfo_get_num_recs(hMinfo, MINFO_INFO_TYPE_AUDIO_ATTR, &prMediaInfo->u2AudStmNum);
		if (i4Ret != MINFOR_OK)
        {
            LOG(3, "Can't get media audio recs\n");
        }
        
	    if(prMediaInfo->u2AudStmNum>0)
	    {
	        prFlvInfo->u2AudIdx=1;
			i4Ret = x_minfo_get_info(hMinfo,MINFO_INFO_TYPE_AUDIO_ATTR, (MINFO_REC_IDX_T)(1), &rMinfoRec, &zRecLength);
			if (i4Ret != MINFOR_OK)
			{
			  LOG(3, "get media audio info error\n");
			  eEvent = MINFO_NFY_EVENT_ERROR;
			  break;
			}
			x_memcpy(&prFlvInfo->rStmAttribute[1], &(rMinfoRec.u.t_stm_attribute),sizeof(MINFO_TYPE_STM_ATRBT_T));
	    }
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

static VOID _PLAYMGR_FlvMIdx_Notify(
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
#endif

BOOL _PLAYMGR_FlvStart(VOID* pvMediaInfo)
{
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_FLV_INFO_T* prFlvInfo = (PLAYMGR_FLV_INFO_T*)(&prMediaInfo->FmtInfo.rFlvInfo);
#ifndef  USE_TEST_CASE
    UINT32 u4Msg;
    SIZE_T zSize;
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt; 
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prFlvInfo->rSrcInfo;
#endif
#ifndef __MODEL_slt__
    MIDXBULD_KEY_TBL_INFO_T* prTableInfo = &prFlvInfo->rTableInfo;
    MIDXBULD_KEY_TBL_EXT_T*  prKeyTblList;
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prFlvInfo->rKeyInfo;
    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTbl = &prFlvInfo->rKeyIdxTBL;
    INT32 i4Ret;
#endif
    
    if (prMediaInfo == 0)
    {
        LOG(1, "_PLAYMGR_FlvStart prMediaInfo == 0\n");
        return FALSE;
    }
            
#ifndef  USE_TEST_CASE
    UNUSED(u4Msg);
    UNUSED(zSize);
    UNUSED(prMediaFmt);
    UNUSED(prSrcInfo);
#endif

//    SWDMX_DelRange(prMediaInfo->u1SrcId, NULL, (UINT32)prMediaInfo->rRangeInfo[0].ui4_range_id);
//    SWDMX_DelRange(prMediaInfo->u1SrcId, NULL, (UINT32)prMediaInfo->rRangeInfo[1].ui4_range_id);

#ifndef __MODEL_slt__
    LOG(1, "x_midxbuld_mopen\n");
	if((!prFlvInfo->hMediaIdxHandle) && 
        !(prFlvInfo->rPlayerIdxInfo.ui4_idx_type & MM_MINFO_IDX_TYPE_NONE)) 
	{

        UNUSED(_PLAYMGR_FlvMIdx_Notify);
#ifndef  USE_TEST_CASE
#ifndef FLV_DONOT_BUILDINDEX
		i4Ret = x_midxbuld_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _PLAYMGR_FlvMIdx_Notify, prMediaInfo,
								prMediaFmt, prSrcInfo, prKeyInfo, &prFlvInfo->hMediaIdxHandle);
		if(i4Ret != MINFOR_OK)
		{
			return FALSE;
		}

        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId,(VOID*)&u4Msg);
        if (u4Msg != MIDXBULD_NFY_EVENT_INDEXING_DONE)
        {
           return FALSE;
        }
        else
#endif
#else
        prFlvInfo->hMediaIdxHandle=0xffffff;
#endif
        {
        
		    LOG(1, "start build idx\n");
            prTableInfo->ui4_key_tbl_id = prKeyInfo->ui4_tbl_id;
			prFlvInfo->b_HasKeyframes=TRUE;
#ifndef  USE_TEST_CASE
#ifndef FLV_DONOT_BUILDINDEX
            zSize = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
            i4Ret = x_midxbuld_get_info(prFlvInfo->hMediaIdxHandle, MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST,(VOID*)prTableInfo, &zSize);
#else
             i4Ret=1;
#endif
#else
            i4Ret=_PLAYMGR_BuildCaseIdx(prTableInfo,u4CaseIndex);
#endif
            if (i4Ret)
            {
               prFlvInfo->b_HasKeyframes=FALSE;
               LOG(1, "get midxbuild elmt tbl error\n");
            }

			if(prFlvInfo->b_HasKeyframes)
			{
				prKeyTblList = prTableInfo->pt_key_tbl_ext;
				prKeyIdxTbl->e_elem_type = MM_ELEMT_TYPE_KEY;
				prKeyIdxTbl->t_strm_id.u.t_stm_flv_id = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
				prKeyIdxTbl->ui4_number_of_entry = prKeyTblList->t_elmt_tbl.ui4_number_of_entry;
				prKeyIdxTbl->ui8_base_offset = prKeyTblList->t_elmt_tbl.ui8_base_offset;
				prKeyIdxTbl->u.pt_pb_idx_tbl_entry = (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)prKeyTblList->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;		
            }
            else
            {
                prKeyIdxTbl->u.pt_pb_idx_tbl_entry=NULL;
                prKeyIdxTbl->ui4_number_of_entry=0;
            }
	     }
		    //build two interval of indx table at beginning
		    prMediaInfo->u1CurEmptyRange = 0;
		    prMediaInfo->u4CurRangePTS = 0;
			if(prMediaInfo->u2VidStmNum > 0||prMediaInfo->u2AudStmNum > 0)
			{
				_PLAYMGR_FlvUpdateRange(pvMediaInfo);
			}
	}
	else
	{
        if((prMediaInfo->u2VidStmNum > 0)||(prMediaInfo->u2AudStmNum > 0))
        {
          _PLAYMGR_FlvUpdateRange(pvMediaInfo);
        }
        else
        {
            LOG(3, "range render cnt 0!\n");
        }
	}
#else
    prMediaInfo->u1CurEmptyRange = 0;
    prMediaInfo->u4CurRangePTS = 0;
    _PLAYMGR_FlvUpdateRange(pvMediaInfo);
#endif

    if(prFlvInfo->u2AudIdx<2)
    {
        SWDMX_SetInfo(prMediaInfo->u1SrcId,eSWDMX_SET_SYSTEMINFO,prFlvInfo->rVDescrib[0].u_info.t_es.ui4_rate,
          prFlvInfo->rADescrib[0].u_info.t_es.ui4_sample_sz,prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].u.t_flv_audio_stm_attr.ui4_type);
    }
    return TRUE;
}


BOOL _PLAYMGR_FlvStop(VOID* pvMediaInfo)
{  
    return TRUE;
}

EXTERN BOOL _PLAYMGR_FlvClose(VOID* pvMediaInfo)
{
    BOOL fgRet = TRUE;
    UINT32 u4Msg;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    if (prMediaInfo->FmtInfo.rFlvInfo.hMediaInfoHandle)
    {
        x_minfo_close(prMediaInfo->FmtInfo.rFlvInfo.hMediaInfoHandle);
        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId,(VOID*)&u4Msg);
        if (u4Msg != MINFO_NFY_EVENT_HANDLE_CLOSED)
        {
            LOG(1, "x_minfo_close failed\n");
            fgRet = FALSE;
        }
    }
	
    if (prMediaInfo->FmtInfo.rFlvInfo.hMediaIdxHandle)
    {
#ifndef USE_TEST_CASE
        x_midxbuld_close(prMediaInfo->FmtInfo.rFlvInfo.hMediaIdxHandle);
        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId,(VOID*)&u4Msg);
        if (u4Msg != MIDXBULD_NFY_EVENT_HANDLE_CLOSED)
        {
            LOG(1, "x_midxbuld_close failed");
            fgRet = FALSE;
        }
#else
       x_mem_free(prMediaInfo->FmtInfo.rFlvInfo.rTableInfo.pt_key_tbl_ext);
#endif
    }
	
    x_memset(&prMediaInfo->FmtInfo.rFlvInfo, 0, sizeof(PLAYMGR_FLV_INFO_T));
    return fgRet;
}

EXTERN BOOL _PLAYMGR_FlvResume(VOID* pvMediaInfo)
{
    return TRUE;
}

BOOL _PLAYMGR_FlvPause(VOID)
{

    return TRUE;
}

BOOL _PLAYMGR_OpenFlvMediaInfo(PLAYMGR_STATUS_T* prMediaStatus, VOID* pvMediaInfo)
{
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
#ifndef USE_TEST_CASE
    PLAYMGR_FLV_INFO_T* prFlvInfo = (PLAYMGR_FLV_INFO_T*)&prMediaInfo->FmtInfo.rFlvInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prFlvInfo->rSrcInfo;

    #ifndef __MODEL_slt__
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt; 
    INT32 i4Ret;
    UINT32 u4Msg;

    // Open for media info
    i4Ret = x_minfo_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _PLAYMGR_FlvMinfo_Notify, prMediaInfo,
            		       prMediaFmt, &(prFlvInfo->hMediaInfoHandle));
    if(i4Ret != MINFOR_OK)
    {
        return FALSE;
    }
    
    _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId,(VOID*)&u4Msg);
    if (u4Msg != MINFO_NFY_EVENT_PARSING_DONE)
    {
        return FALSE;
    }
    
    // Wait for media info's notify
    prSrcInfo->ui8_data_offset = prFlvInfo->rPlayerIdxInfo.ui8_data_offset;
    prSrcInfo->ui8_data_size = prFlvInfo->rPlayerIdxInfo.ui8_data_sz;
    prSrcInfo->ui8_idx_offset = prFlvInfo->rPlayerIdxInfo.ui8_idx_offset;
    prSrcInfo->ui8_idx_size = prFlvInfo->rPlayerIdxInfo.ui8_idx_sz;
    prSrcInfo->ui4_idx_type = prFlvInfo->rPlayerIdxInfo.ui4_idx_type;
    prSrcInfo->ui1_total_strm_num = (UINT8)(prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum);
    // Temporally multiply SYSTEM_TIME_SCALE
    prSrcInfo->ui4_total_duration = (UINT32)prFlvInfo->rPlayerGenericInfo.ui8_pb_duration;// * SYSTEM_TIME_SCALE;
    prMediaStatus->u4TotalDuration = (UINT32)prFlvInfo->rPlayerGenericInfo.ui8_pb_duration;
    prMediaInfo->rPlayerStatus.u4TotalDuration = (UINT32)prFlvInfo->rPlayerGenericInfo.ui8_pb_duration;
    #else
    // Wait for media info's notify
    prSrcInfo->ui8_data_offset = 9;
    prSrcInfo->ui8_data_size = 54209;
    prSrcInfo->ui8_idx_offset = 24;
    prSrcInfo->ui8_idx_size = 417;
    prSrcInfo->ui4_idx_type = 18;
    prSrcInfo->ui1_total_strm_num = 1;
    // Temporally multiply SYSTEM_TIME_SCALE
    prSrcInfo->ui4_total_duration = 90000;
    prMediaStatus->u4TotalDuration = 90000;
    prMediaInfo->rPlayerStatus.u4TotalDuration = 90000;
    #endif
#else
    _PLAYMGR_SetCaseInfo(prMediaInfo,u4CaseIndex);

#endif

    if (_PLAYMGR_SetFlvInfo(prMediaInfo) != TRUE)
    {
        return FALSE;
    }

    prMediaInfo->rRangeInfo[0].ui8_fileoffset = prSrcInfo->ui8_data_offset;
    prMediaInfo->rRangeInfo[0].z_range_sz = prSrcInfo->ui8_data_size;

    return TRUE;
}


BOOL _PLAYMGR_FlvUpdateRange(VOID* pvMediaInfo)
{
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    prMediaInfo->rRangeInfo[0].ui4_range_id = 0;
    
#ifndef __MODEL_slt__
    {
    PLAYMGR_FLV_INFO_T* prFlvInfo = (PLAYMGR_FLV_INFO_T*)&prMediaInfo->FmtInfo.rFlvInfo;
    
    prMediaInfo->rRangeInfo[0].ui8_pb_duration = (UINT64)prFlvInfo->rSrcInfo.ui4_total_duration;
    prMediaInfo->rRangeInfo[0].ui8_fileoffset =  prFlvInfo->rSrcInfo.ui8_data_offset;
    prMediaInfo->rRangeInfo[0].z_range_sz = prFlvInfo->rSrcInfo.ui8_data_size;
	prMediaInfo->rRangeInfo[0].b_key_tbl_exist=FALSE;
    prMediaInfo->rRangeInfo[0].e_mm_src_type=MM_SRC_TYPE_HIGH_SPEED_STORAGE;
    if(prFlvInfo->u2VidIdx<2)
    {
        prMediaInfo->rRangeInfo[0].t_vid_start_pts=prFlvInfo->rStmAttribute[prFlvInfo->u2VidIdx].u.t_flv_video_stm_attr.ui4_start_pts;
    }
    if(prFlvInfo->u2AudIdx<2)
    {
        prMediaInfo->rRangeInfo[0].t_aud_start_pts=prFlvInfo->rStmAttribute[prFlvInfo->u2AudIdx].u.t_flv_audio_stm_attr.ui4_start_pts;
    }
    prMediaInfo->rRangeInfo[0].ui8_vid_duration=prMediaInfo->rRangeInfo[0].ui8_pb_duration;
    prMediaInfo->rRangeInfo[0].pt_idxtbl_lst=&prFlvInfo->rKeyIdxTBL;
    prFlvInfo->rKeyIdxTBL.t_strm_id.u.t_stm_flv_id = prFlvInfo->rKeyInfo.ui4_strm_id;
    if(prFlvInfo->b_HasKeyframes)
    { 
      prMediaInfo->rRangeInfo[0].b_key_tbl_exist=TRUE;
    }
    }
#endif
	SWDMX_AddRange(prMediaInfo->u1SrcId,&prMediaInfo->rRangeInfo[0],(UINT32)prMediaInfo->rRangeInfo[0].ui4_range_id);
    return TRUE;
}


#endif // !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)



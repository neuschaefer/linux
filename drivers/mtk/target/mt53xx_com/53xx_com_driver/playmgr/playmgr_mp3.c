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
 * $RCSfile: playmgr_mp3.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
#include "playmgr.h"
#include "feeder_if.h"
#include "playmgr_util.h"
#include "swdmx_drvif.h"
#include "playmgr_mp3.h"
#include "playmgr_debug.h"


static VOID _MIdxNotify(
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


static VOID _SetMp3Info(MINFO_INFO_T* pt_minfo_rec,PLAYMGR_INFO_T* prMediaInfo)
{
    MINFO_INFO_TYPE_T               e_info_type;
    MINFO_TYPE_GENERIC_T*           pt_general_info;  
    MINFO_TYPE_ID3_TAG_T*           prMp3Id3Data;
    VOID*     prAudioAttr;
    MINFO_TYPE_IDX_INFO_T*          prIdxInfo;
    PLAYMGR_MP3_INFO_T* prMp3Info;
    

    //UINT32 u4VideoIdx = 1;
    //UINT32 u4AudioIdx = 1;

    if (NULL == pt_minfo_rec)
    {
        LOG(0,("_minfo_dump_minfo_rec:NULL \r\n"));
        return ;
    }
    
    e_info_type = pt_minfo_rec->e_info_type;
    prMp3Info = &(prMediaInfo->FmtInfo.rMp3Info);
    
    switch(e_info_type) 
    {
        case MINFO_INFO_TYPE_GENERIC:
            pt_general_info = &(pt_minfo_rec->u.t_general_info);
            LOG(0,"Gneric:duration=%llu; "
                      " media_sz=%llu; strm_ns=%u ;pts_offset : %x\n ",
                      pt_general_info->ui8_pb_duration,
                      pt_general_info->ui8_media_sz,
                      pt_general_info->ui1_strm_ns);
                      
            x_memcpy((VOID*)VIRTUAL((UINT32)(&(prMp3Info->rPlayerGenericInfo))),
                (VOID*)VIRTUAL((UINT32)(&(pt_minfo_rec->u.t_general_info))),
                sizeof(MINFO_TYPE_GENERIC_T));
            break;
        case MINFO_INFO_TYPE_ID3_DATA:
            
            //ui4_strm_id = pt_minfo_rec->u.t_id3_data
            prMp3Id3Data = &(pt_minfo_rec->u.t_id3_data);
            
            x_memcpy((VOID*)VIRTUAL((UINT32)(&(prMp3Info->rMp3Id3Data))), 
                 (VOID*)VIRTUAL((UINT32)prMp3Id3Data),
                 sizeof(MINFO_TYPE_ID3_TAG_T));
            break;
        case MINFO_INFO_TYPE_AUDIO_ATTR:
            prAudioAttr = &(pt_minfo_rec->u.t_stm_attribute);
            x_memcpy((VOID*)VIRTUAL((UINT32)(&(prMp3Info->rStreamAttr))), 
                 (VOID*)VIRTUAL((UINT32)prAudioAttr),
                 sizeof(MINFO_TYPE_STM_ATRBT_T));
            /*
            if ((prMediaInfo->rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
                (prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3))
            {
                prAudioAttr = &(pt_minfo_rec->u.t_stm_attribute.u.t_mp3_audio_stm_attr);
                x_memcpy((VOID*)VIRTUAL((UINT32)(&(prMp3Info->AudAttr.rMp3AudioAttr))), 
                     (VOID*)VIRTUAL((UINT32)prAudioAttr),
                     sizeof(MINFO_MP3_AUDIO_STM_ATRBT_T));
            }
            else if ((prMediaInfo->rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
                (prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AIF))
            {
                prAudioAttr = &(pt_minfo_rec->u.t_stm_attribute.u.t_aif_audio_stm_attr);
                x_memcpy((VOID*)VIRTUAL((UINT32)(&(prMp3Info->AudAttr.rAifAudioAttr))), 
                     (VOID*)VIRTUAL((UINT32)prAudioAttr),
                     sizeof(MINFO_AIF_AUDIO_STM_ATRBT_T));
            }
            else if ((prMediaInfo->rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
                ((prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WAVE) ||
                 (prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AC3) ||
                 (prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AAC)))
            {
                prAudioAttr = &(pt_minfo_rec->u.t_stm_attribute.u.t_es_audio_stm_attr);
                x_memcpy((VOID*)VIRTUAL((UINT32)(&(prMp3Info->AudAttr.rEsAudioAttr))), 
                     (VOID*)VIRTUAL((UINT32)prAudioAttr),
                     sizeof(MINFO_ES_AUDIO_STM_ATRBT_T));
            }
            */
            
            break;
        case MINFO_INFO_TYPE_IDX_INFO:
            prIdxInfo = &(pt_minfo_rec->u.t_idx_info);
            x_memcpy(&prMp3Info->rIdxInfo, (VOID*)VIRTUAL((UINT32)prIdxInfo),
                        sizeof(MINFO_TYPE_IDX_INFO_T));
        
            break;
        default:
            LOG(0,"_minfo_dump_minfo_rec:unknown\r\n");
            break;
    }

}

#if 0
BOOL _PLAYMGR_Mp3UpdateRange(VOID* pvMediaInfo)
{
    INT32 i4RangeIdx;
   
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_MP3_INFO_T* prMp3Info = 
    	(PLAYMGR_MP3_INFO_T*)&prMediaInfo->FmtInfo.rMp3Info;

    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prMp3Info->rSrcInfo;

    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTbl = &prMp3Info->rKeyIdxTBL;

    i4RangeIdx = prMediaInfo->u1CurEmptyRange;

    ASSERT(i4RangeIdx < MAX_RANGE_NUM);
    if ((prKeyIdxTbl->ui4_number_of_entry > 0) && (i4RangeIdx < MAX_RANGE_NUM))
    {
        prMediaInfo->rRangeInfo[i4RangeIdx].pt_idxtbl_lst = prKeyIdxTbl;
    
        prMediaInfo->rRangeInfo[i4RangeIdx].ui4_range_id = 
            prMp3Info->rAllocTblInfo[i4RangeIdx].ui4_range_id;

        LOG(3, "Allocate prMp3Info->rAllocTblInfo[%d].ui4_range_id = %ld, pv_tag = %ld\n", 
            i4RangeIdx, prMp3Info->rAllocTblInfo[i4RangeIdx].ui4_range_id,
            prMediaInfo->rRangeInfo[i4RangeIdx].ui4_range_id);

        prMediaInfo->rRangeInfo[i4RangeIdx].ui8_pb_duration = (UINT64)prSrcInfo->ui4_total_duration;
        SWDMX_AddRange(&prMediaInfo->rRangeInfo[i4RangeIdx], 
                            (UINT32)prMediaInfo->rRangeInfo[i4RangeIdx].ui4_range_id);
    }

    return TRUE;
}
#endif

VOID _PLAYMGR_Mp3MediaInfoNotify(
                           HANDLE_T                    hMinfo,                 
                           MINFO_NFY_EVENT_TYPE_T      e_event,  
                           const VOID*                 pvData1,
                           const VOID*                 pvData2,
                           const VOID*                 pvTag) 
{    
    CHAR *apsz_parse_Info[] = { 
                                "PARSING DONE", 
                                "HANDLE CLOSED",   
                                "ERROR"
                            };
    INT32 i4Ret;
    MINFO_INFO_T rMinfoRec;
    SIZE_T  t_size;
    SIZE_T zRecLength;
    //UINT32 u4RecLength;
    UINT16 ui2_number;
    PLAYMGR_INFO_T *prMediaInfo;    
    PLAYMGR_MP3_INFO_T* prMp3Info;
    
    
    
    //u4RecLength = sizeof(MINFO_INFO_T);
    zRecLength = sizeof(MINFO_INFO_T);
    prMediaInfo = (PLAYMGR_INFO_T*)pvTag;
    prMp3Info = &prMediaInfo->FmtInfo.rMp3Info;

    LOG(0,"_PLAYMGR_Mp3MediaInfoNotify : Event type = %s\r\n",apsz_parse_Info[e_event]);
    
    switch(e_event) {
    case MINFO_NFY_EVENT_PARSING_DONE:
        LOG(0,"event : MINFO_NFY_EVENT_PARSING_DONE\n");
        // 1) get generic num should be 1
        i4Ret = x_minfo_get_num_recs((prMp3Info->hMediaInfoHandle),MINFO_INFO_TYPE_GENERIC,&ui2_number);
        
        if (MINFOR_OK == i4Ret)
        {
            if (1 != ui2_number)
            {
                LOG(0,"mp3 getnum Generic unsuccessfully num=%u\r\n", 
                          ui2_number);
            }
            else
            {
                LOG(0,"mp3 getnum Generic successfully\r\n");
            }
        }
        else
        {
            LOG(0,"getnum Generic unsuccessfully(%d)\r\n", i4Ret);
        }
        
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&rMinfoRec, 0, t_size);
        i4Ret = x_minfo_get_info( (prMp3Info->hMediaInfoHandle),MINFO_INFO_TYPE_GENERIC,1,(VOID*)&rMinfoRec,&t_size);
        
        if (MINFOR_OK != i4Ret)
        {
            LOG(0,"x_minfo_get_info return %d for generic \r\n",i4Ret);
            return;
        }

        _SetMp3Info(&rMinfoRec,prMediaInfo);//set generic information

        /*2) get ID3 info*/
        if ((prMediaInfo->rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
            (prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3))
        {
            t_size = sizeof(MINFO_INFO_T);
            x_memset(&rMinfoRec, 0, t_size);
            i4Ret = x_minfo_get_info( (prMp3Info->hMediaInfoHandle),MINFO_INFO_TYPE_ID3_DATA,1,(VOID*)&rMinfoRec,&t_size);
            if (MINFOR_OK != i4Ret)
            {
                LOG(0,"x_minfo_get_info return %d for audio (ID3)\r\n",i4Ret);
                //return;
            }
            else
            {
                /*show obtainted info*/
                _SetMp3Info(&rMinfoRec,prMediaInfo);
            }
        }

        /*3) get audio attribute*/
        i4Ret = x_minfo_get_num_recs((prMp3Info->hMediaInfoHandle),MINFO_INFO_TYPE_GENERIC,&ui2_number);
        if (MINFOR_OK == i4Ret)
        {
            LOG(0,"getnum Audio successfully num=%u\r\n",ui2_number);
        }
        else
        {
            LOG(0,"getnum Audio unsuccessfully(%d)\r\n",i4Ret);
        }
        
        /*get audio info*/
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&rMinfoRec, 0, t_size);
        i4Ret = x_minfo_get_info((prMp3Info->hMediaInfoHandle),MINFO_INFO_TYPE_AUDIO_ATTR,1,(VOID*)&rMinfoRec,&t_size);

        if (MINFOR_OK != i4Ret)
        {
            LOG(0,"x_minfo_get_info return %d for audio\r\n",i4Ret);
            return;
        }
        /*show obtainted info*/
        _SetMp3Info(&rMinfoRec,prMediaInfo);

        
        /*4) get index info*/
        x_memset(&rMinfoRec, 0, t_size);
        i4Ret = x_minfo_get_info(hMinfo, 
                    MINFO_INFO_TYPE_IDX_INFO, 
                    1,
                    &rMinfoRec, 
                    &zRecLength);
        if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
        {
            LOG(0, "get media idx info error\n");
            break;
        }
        /*show obtainted info*/
        _SetMp3Info(&rMinfoRec,prMediaInfo);
        
        
        break;
    case MINFO_NFY_EVENT_HANDLE_CLOSED:
        break;
    case MINFO_NFY_EVENT_ERROR:             
        break;
    default:
        break;
    }
    _PLAYMGR_SendNotify(e_event);
    return;
}

BOOL _PLAYMGR_OpenMp3MediaInfo(PLAYMGR_STATUS_T* prMediaStatus, VOID* pvMediaInfo)
{
    //BOOL fgRet;
    SIZE_T zSize;
    UINT32 u4Msg;
    INT32 i4Ret;
	PLAYMGR_SourceType_T ePlayMgrSrcType;
    
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_MP3_INFO_T* prMp3Info = (PLAYMGR_MP3_INFO_T*)&prMediaInfo->FmtInfo.rMp3Info;
    MIDXBULD_KEY_TBL_INFO_T* prTableInfo = &prMp3Info->rTableInfo;
    MIDXBULD_KEY_TBL_EXT_T*  prKeyTblList;
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt; 
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prMp3Info->rKeyInfo;
    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTbl = &prMp3Info->rKeyIdxTBL;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prMp3Info->rSrcInfo;    

    ePlayMgrSrcType = _PLAYMGR_GetSrcType();
    if(ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        UINT64 u8FileSize;
        UINT64 u8Duration;
        prMediaInfo->rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_LPCM;
        //prMediaInfo->rPlayerStatus.aeAudFormat[0] = PLAYMGR_ADEC_PCM;

        // set it as LPCM info
        u8FileSize = prMediaInfo->rRangeInfo[0].z_range_sz;
        u8Duration = prMediaInfo->rRangeInfo[0].ui8_pb_duration;
        
        x_memset(&prMediaInfo->rRangeInfo[0],0,sizeof(MM_RANGE_INFO_T));  
        
        prMediaInfo->rRangeInfo[0].z_range_sz = u8FileSize;
        prMediaInfo->rRangeInfo[0].ui8_pb_duration = u8Duration;        
    }
    // Rhapsody, do not use minfo to parse duration
    // it wastes too much times
    else if(ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_RHAPSODY)
    {
        prMediaInfo->rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MP3;
        
        x_memset(&prMediaInfo->rRangeInfo[0],0,sizeof(MM_RANGE_INFO_T));  
        
        prMediaInfo->rRangeInfo[0].z_range_sz = prMediaInfo->u8FileSize;
        prMediaInfo->rRangeInfo[0].ui8_pb_duration = 0;        
    }
    else
    {
        x_memset(&prMediaInfo->rRangeInfo[0],0,sizeof(MM_RANGE_INFO_T));  
        
        LOG(5,"x_minfo_mopen\n");
        i4Ret = x_minfo_mopen((VOID *)&(prMediaInfo->rMediaFeeder), 0, _PLAYMGR_Mp3MediaInfoNotify, (VOID*)prMediaInfo,
                               prMediaFmt, &(prMp3Info->hMediaInfoHandle));

        if (MINFOR_OK != i4Ret )
        {
            return FALSE;
        }
        
        //waiting for media info notify
        _PLAYMGR_ReceiveNotify((VOID*)&u4Msg);
        
        LOG(5,"x_minfo_mopen notify finish\n");
        
        if (u4Msg != MINFO_NFY_EVENT_PARSING_DONE)
        {
            LOG(1,"x_minfo_mopen error notify message : %d\n",u4Msg);
            return FALSE;
        }

        // Wait for media info's notify
        prSrcInfo->ui8_data_offset = prMp3Info->rIdxInfo.ui8_data_offset;
        prSrcInfo->ui8_data_size = prMp3Info->rIdxInfo.ui8_data_sz;
        prSrcInfo->ui8_idx_offset = prMp3Info->rIdxInfo.ui8_idx_offset;
        prSrcInfo->ui8_idx_size = prMp3Info->rIdxInfo.ui8_idx_sz;
        prSrcInfo->ui1_total_strm_num = (UINT8)(1);
        prSrcInfo->ui4_total_duration = (UINT32)prMp3Info->rPlayerGenericInfo.ui8_pb_duration;
        prSrcInfo->ui8_seg_data_offset = prMp3Info->rIdxInfo.ui8_segment_data_offset;
        prSrcInfo->ui8_seg_tm_scale = prMp3Info->rIdxInfo.ui8_seg_timecode_scale;
            
        prMediaInfo->rRangeInfo[0].ui8_fileoffset = prMediaInfo->FmtInfo.rMp3Info.rIdxInfo.ui8_data_offset;
        //ui8_media_sz is file size.
        //prMediaInfo->rRangeInfo[0].z_range_sz = (SIZE_T)prMediaInfo->FmtInfo.rMp3Info.rPlayerGenericInfo.ui8_media_sz;
        prMediaInfo->rRangeInfo[0].z_range_sz = (SIZE_T)prMediaInfo->FmtInfo.rMp3Info.rIdxInfo.ui8_data_sz;
        prMediaInfo->rRangeInfo[0].ui8_pb_duration = prMediaInfo->FmtInfo.rMp3Info.rPlayerGenericInfo.ui8_pb_duration;
        prMediaInfo->rRangeInfo[0].t_pts_offset = 0;
    }

    prMediaInfo->rPlayerStatus.u2AudioTracks = 1;
    
    if(prMediaInfo->rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MP2)
    {
        prMediaInfo->rPlayerStatus.aeAudFormat[0] = PLAYMGR_ADEC_MPEG;
    }
    else if(prMediaInfo->rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MP3)
    {
        prMediaInfo->rPlayerStatus.aeAudFormat[0] = PLAYMGR_ADEC_MP3;
    }
    else if(prMediaInfo->rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_WAVE)
    {
        prMediaInfo->rPlayerStatus.aeAudFormat[0] = PLAYMGR_ADEC_PCM;
    }
    else if(prMediaInfo->rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AIF)
    {
        prMediaInfo->rPlayerStatus.aeAudFormat[0] = PLAYMGR_ADEC_PCM;
    }
    else if(prMediaInfo->rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AC3)
    {
        prMediaInfo->rPlayerStatus.aeAudFormat[0] = PLAYMGR_ADEC_AC3;
    }
    else if(prMediaInfo->rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AAC)
    {
        prMediaInfo->rPlayerStatus.aeAudFormat[0] = PLAYMGR_ADEC_AAC;
    }
    else if(prMediaInfo->rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_LPCM)
    {
        prMediaInfo->rPlayerStatus.aeAudFormat[0] = PLAYMGR_ADEC_PCM;
    }
    else
    {
        LOG(1, "Unknown format %d\n", (UINT32)prMediaInfo->rPlayerStatus.eMediaType);
        prMediaInfo->rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_UNKNOWN;
        prMediaInfo->rPlayerStatus.aeAudFormat[0] = PLAYMGR_ADEC_UNKNOWN;
        return FALSE;
    }
    prMediaInfo->rPlayerStatus.u2VideoTracks = 0;
    prMediaInfo->rPlayerStatus.aeVidFormat[0] = PLAYMGR_VDEC_UNKNOWN;
    prMediaInfo->rPlayerStatus.u4TotalDuration = (UINT32)prMediaInfo->FmtInfo.rMp3Info.rPlayerGenericInfo.ui8_pb_duration;
    
    if(ePlayMgrSrcType==PLAYMGR_SRC_TYPE_NETWORK_FM)
    {
		LOG(5,"SrcType is PLAYMGR_SRC_TYPE_NETWORK_FM no need to build index\n");
		return TRUE;
    }
    else if(ePlayMgrSrcType==PLAYMGR_SRC_TYPE_NETWORK_RHAPSODY)
    {
		LOG(5,"SrcType is PLAYMGR_SRC_TYPE_NETWORK_RHAPSODY no need to build index\n");
		return TRUE;
    }
    else if(ePlayMgrSrcType != PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        LOG(5,"x_midxbuld_mopen\n");
        i4Ret = x_midxbuld_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _MIdxNotify, 0,
                                prMediaFmt, prSrcInfo, prKeyInfo, &prMp3Info->hMediaIdxHandle);
        if(i4Ret == MINFOR_OK)
        {
            _PLAYMGR_ReceiveNotify((VOID*)&u4Msg);
            
            LOG(5,"x_midxbuld_mopen notify finish\n");
            
            if (u4Msg != MIDXBULD_NFY_EVENT_INDEXING_DONE)
            {
                LOG(1, "_PLAYMGR_Mp3Start build index fail, ret = %d\n", u4Msg);
                return FALSE;
            }
            else
            {
                zSize = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
                prTableInfo->ui4_key_tbl_id = prKeyInfo->ui4_tbl_id;
                i4Ret = x_midxbuld_get_info(prMp3Info->hMediaIdxHandle, MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST,
                	(VOID*)prTableInfo, &zSize);
                if (i4Ret)
                {
                    LOG(3, "get midxbuild elmt tbl error\n");
                    return FALSE;
                }
                else
                {
                    //key table
                    prKeyTblList = prTableInfo->pt_key_tbl_ext;
                    
                    prMediaInfo->rRangeInfo[0].b_key_tbl_exist = TRUE;
                    prMediaInfo->rRangeInfo[0].pt_idxtbl_lst = prKeyIdxTbl;
                    
                    prKeyIdxTbl->e_elem_type = MM_ELEMT_TYPE_KEY;
                    prKeyIdxTbl->t_strm_id.u.t_stm_es_id = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
                    prKeyIdxTbl->ui4_number_of_entry = prKeyTblList->t_elmt_tbl.ui4_number_of_entry;
                    prKeyIdxTbl->ui8_base_offset = prKeyTblList->t_elmt_tbl.ui8_base_offset;
                    prKeyIdxTbl->u.pt_pb_idx_tbl_entry = 
                        (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)prKeyTblList->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
                    prKeyIdxTbl->ui4_rate = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_rate;
                    prKeyIdxTbl->ui4_scale = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale;
                    prKeyTblList = prKeyTblList->pt_next;   //key tbl info from mutil
                    
                    if ((prKeyIdxTbl->ui4_number_of_entry > 0) &&
                        (prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prKeyIdxTbl->ui4_number_of_entry-1].ui4_pts > 0))
                    {
                        UINT32 u4Time =
                            prKeyIdxTbl->u.pt_pb_idx_tbl_entry[prKeyIdxTbl->ui4_number_of_entry-1].ui4_pts;

                        if(prMediaInfo->FmtInfo.rMp3Info.rPlayerGenericInfo.ui8_pb_duration != u4Time)
                        {
                            LOG(5, "Change total time %lld -> %d\n",
                                prMediaInfo->FmtInfo.rMp3Info.rPlayerGenericInfo.ui8_pb_duration,
                                u4Time);
                            prMediaInfo->FmtInfo.rMp3Info.rPlayerGenericInfo.ui8_pb_duration = u4Time;
                            prMediaInfo->rPlayerStatus.u4TotalDuration = u4Time;
                            prMediaInfo->rRangeInfo[0].ui8_pb_duration = u4Time;
                        }
                    }
                }
            }
        }
    }


    //LOG(1, "start build idx\n");
    //prMediaInfo->u1CurEmptyRange = 0;
    //prMediaInfo->u4CurRangePTS = 0;
    //_PLAYMGR_Mp3UpdateRange(pvMediaInfo);
    
    // mp3's ui8_pb_duration unit is second
    // u4TotalDuration unit is 90K
    /* now is 90K unit.
    prMediaInfo->FmtInfo.rMp3Info.rPlayerGenericInfo.ui8_pb_duration *= 90000;
    prMediaInfo->rPlayerStatus.u4TotalDuration *= 90000;
    prMediaInfo->rRangeInfo[0].ui8_pb_duration *= 90000;//sunman
    */

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

BOOL _PLAYMGR_Mp3Start(VOID* pvMediaInfo)
{
    BOOL fgRet;
    PLAYMGR_SourceType_T ePlayMgrSrcType;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    
    if((prMediaInfo == NULL) || (pvMediaInfo == NULL))
    {
        LOG(1, "_PLAYMGR_Mp3Start prMediaInfo pvMediaInfo NULL\n");
        return FALSE;
    }

    ePlayMgrSrcType = _PLAYMGR_GetSrcType();
    switch(ePlayMgrSrcType)
    {
        case PLAYMGR_SRC_TYPE_NETWORK_DLNA:
            prMediaInfo->rRangeInfo[0].e_mm_src_type = MM_SRC_TYPE_NETWORK_DLNA;
        break;
        case PLAYMGR_SRC_TYPE_NETWORK_VUDU:
            prMediaInfo->rRangeInfo[0].e_mm_src_type = MM_SRC_TYPE_NETWORK_VUDU;
        break;
        case PLAYMGR_SRC_TYPE_NETWORK_RHAPSODY:
            prMediaInfo->rRangeInfo[0].e_mm_src_type = MM_SRC_TYPE_NETWORK_RHAPSODY;
        break;
        case PLAYMGR_SRC_TYPE_HIGH_SPEED_STORAGE:
        default:
            prMediaInfo->rRangeInfo[0].e_mm_src_type = MM_SRC_TYPE_HIGH_SPEED_STORAGE;
        break;
    }
    
    fgRet = SWDMX_AddRange((void*)&prMediaInfo->rRangeInfo[0], prMediaInfo->rRangeInfo[0].ui4_range_id);
    if(!fgRet)
    {
        LOG(1, "_PLAYMGR_MpgStart SWDMX_AddRange Fail\n");
        return FALSE;
    }

    return TRUE;
}


BOOL _PLAYMGR_Mp3Stop(VOID* pvMediaInfo)
{
    INT32 i4Ret = TRUE;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    if((prMediaInfo == NULL) || (pvMediaInfo == NULL))
    {
        LOG(5, "_PLAYMGR_MpgStop prMediaInfo NULL\n");
        return FALSE;
    }
    
    LOG(5, "_PLAYMGR_MpgStop\n");
    
    i4Ret = SWDMX_DelRange((VOID*)&(prMediaInfo->rRangeInfo[0]), 0, FALSE);
    if (!i4Ret)
    {
        LOG(1, "_PLAYMGR_MpgStop Fail in _SWDMX_DelRange\n");
        return FALSE;
    }

    return TRUE;
}

BOOL _PLAYMGR_Mp3Close(VOID* pvMediaInfo)
{
    UINT32 u4Msg;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_SourceType_T ePlayMgrSrcType;

    ePlayMgrSrcType = _PLAYMGR_GetSrcType();
    if(ePlayMgrSrcType==PLAYMGR_SRC_TYPE_NETWORK_FM)
    {
    }
    else if(ePlayMgrSrcType==PLAYMGR_SRC_TYPE_NETWORK_RHAPSODY)
    {
    }
    else if(ePlayMgrSrcType != PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        if (prMediaInfo->FmtInfo.rMp3Info.hMediaInfoHandle != (HANDLE_T)NULL)
        {
            x_minfo_close(prMediaInfo->FmtInfo.rMp3Info.hMediaInfoHandle);
            _PLAYMGR_ReceiveNotify((VOID*)&u4Msg);
            if (u4Msg != MINFO_NFY_EVENT_HANDLE_CLOSED)
            {
                return FALSE;
            }
        }
        if (prMediaInfo->FmtInfo.rMp3Info.hMediaIdxHandle != (HANDLE_T)NULL)
        {
            x_midxbuld_close(prMediaInfo->FmtInfo.rMp3Info.hMediaIdxHandle);
            _PLAYMGR_ReceiveNotify((VOID*)&u4Msg);
            if (u4Msg != MIDXBULD_NFY_EVENT_HANDLE_CLOSED)
            {
                return FALSE;
            }
        }
    }

    x_memset(&prMediaInfo->FmtInfo.rMp3Info, 0, sizeof(PLAYMGR_MP3_INFO_T));

    return TRUE;
}

BOOL _PLAYMGR_Mp3Resume(VOID* pvMediaInfo)
{
    UNUSED(pvMediaInfo);

    return TRUE;
}

BOOL _PLAYMGR_Mp3Pause(VOID)
{
    return TRUE;//_SWDMX_Pause();
}


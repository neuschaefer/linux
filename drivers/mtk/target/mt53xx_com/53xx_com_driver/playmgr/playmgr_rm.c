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
 * $RCSfile: playmgr_rm.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
#include "playmgr.h"
#include "feeder_if.h"
#include "playmgr_util.h"
#include "swdmx_drvif.h"
#include "playmgr_rm.h"
#include "playmgr_debug.h"

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _PLAYMGR_SetRMInfo(PLAYMGR_STATUS_T* prMediaStatus, PLAYMGR_INFO_T* prMediaInfo)
{
    UINT16 i;
    //UINT32 u4Temp;
    PLAYMGR_RM_INFO_T* prRmInfo = &prMediaInfo->FmtInfo.rRmInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prRmInfo->rSrcInfo;
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prRmInfo->rKeyInfo;

    prMediaStatus->eMediaType = PLAYMGR_MEDIA_TYPE_RM;
#ifndef __MODEL_slt__
    prMediaStatus->u8FileSize = prMediaInfo->FmtInfo.rRmInfo.rPlayerGenericInfo.ui8_media_sz;

    if (prMediaInfo->u2VidStmNum > 0 && prRmInfo->u2VidIdx < 17)
    {
        MINFO_RM_VIDEO_STM_ATRBT_T *pt_rm_video_stm_attr
            = &(prRmInfo->rStmAttribute[prRmInfo->u2VidIdx].u.t_rm_video_stm_attr);
        prMediaStatus->u2VideoTracks = prMediaInfo->u2VidStmNum;
        LOG(5, "Number of video tracks in this RM is %ld\n", prMediaStatus->u2VideoTracks);
        switch (pt_rm_video_stm_attr->e_enc)
        {
        case MINFO_INFO_VID_ENC_RV8:
            pt_rm_video_stm_attr->ui1_ecc_mask = 0x20;  // megaa 20090601: tmp solution
            if (prRmInfo->u2VidIdx < 8)
                prMediaStatus->aeVidFormat[prRmInfo->u2VidIdx] = PLAYMGR_VDEC_RV8;
            break;
        case MINFO_INFO_VID_ENC_RV9:
            pt_rm_video_stm_attr->ui1_ecc_mask = 0x80;  // megaa 20090601: tmp solution
            if (prRmInfo->u2VidIdx < 8)
                prMediaStatus->aeVidFormat[prRmInfo->u2VidIdx] = PLAYMGR_VDEC_RV9;
            break;
        default:
            LOG(3, "Unknown Video %d\n", (pt_rm_video_stm_attr->e_enc));
            pt_rm_video_stm_attr->ui1_ecc_mask = 0x0;
            if (prRmInfo->u2VidIdx < 8)
                prMediaStatus->aeVidFormat[prRmInfo->u2VidIdx] = PLAYMGR_VDEC_UNKNOWN;
            break;
        }
#if 1 //sync from function _mm_hdlr_set_strm_codec_spec_info
        {
            UINT32 ui4_specific_size = 0;
            XX_RM_SPEC_INFO_T* pt_rm_vid_spec_info
                = (XX_RM_SPEC_INFO_T*)&prRmInfo->rRmSpecInfo;
            if(MINFO_INFO_VID_ENC_RV8 == pt_rm_video_stm_attr->e_enc)
            {
                pt_rm_vid_spec_info->b_is_rm8 = TRUE;
            }
            else
            {
                pt_rm_vid_spec_info->b_is_rm8 = FALSE;
            }
            pt_rm_vid_spec_info->ui2_frm_height = pt_rm_video_stm_attr->ui2_frm_height;
            pt_rm_vid_spec_info->ui2_frm_width = pt_rm_video_stm_attr->ui2_frm_width;
            pt_rm_vid_spec_info->ui4_spo_extra_flags = pt_rm_video_stm_attr->ui4_spo_extra_flags;

            ui4_specific_size = (((pt_rm_vid_spec_info->ui4_spo_extra_flags & 0x70000) >> 16) + 1) * 2;
            ui4_specific_size *= sizeof(UINT32);

            if (ui4_specific_size > sizeof(pt_rm_vid_spec_info->aui4_specific))
            {
                LOG(0, "_PLAYMGR_SetRMInfo: ui4_specific_size = %d NG\n", ui4_specific_size);
                ui4_specific_size = sizeof(pt_rm_vid_spec_info->aui4_specific);
            }

            if (pt_rm_video_stm_attr->ui4_specific != 0 && ui4_specific_size > 0)
            {
                x_memcpy((void*)pt_rm_vid_spec_info->aui4_specific,
                    (void*)pt_rm_video_stm_attr->ui4_specific,
                    ui4_specific_size);
            }
        }
#endif
    }

    if (prMediaInfo->u2AudStmNum > 0 && prRmInfo->u2AudIdx < 17)
    {
        prMediaStatus->u2AudioTracks = prMediaInfo->u2AudStmNum;
        LOG(5, "Number of audio tracks in this RM is %ld\n", prMediaStatus->u2AudioTracks);
        switch (prRmInfo->rStmAttribute[prRmInfo->u2AudIdx].u.t_rm_audio_stm_attr.e_enc)
        {
        case MINFO_INFO_AUD_ENC_AAC:
            prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_AAC;
            break;
        case MINFO_INFO_AUD_ENC_COOK:
#if 1  //def COOK_OK
            prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_RA;  // !!!
#else
            prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_UNKNOWN;
//            prMediaStatus->u2AudioTracks = 0;
#endif
            break;
        default:
            LOG(3, "Unknown Audio %d\n", (prRmInfo->rStmAttribute[prRmInfo->u2AudIdx].u.t_rm_audio_stm_attr.e_enc));
            prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_UNKNOWN;
            break;
            //return FALSE;
        }
    }

    for (i = 0; i < prMediaInfo->u2VidStmNum && i < 17; i++)
    {
        prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;

        if (i < PLAYMGR_MAX_VID_TRACK)
        {
            prMediaStatus->au4VideoId[i] = prRmInfo->rStmAttribute[i].u.t_rm_video_stm_attr.ui2_strm_num;
        }

        prSrcInfo->t_es_map[i].ui4_strm_id =
            prRmInfo->rStmAttribute[i].u.t_rm_video_stm_attr.ui2_strm_num;
        prSrcInfo->t_es_map[i].ui4_avg_bytes_per_sec = 0;
        prMediaInfo->rRangeInfo[0].ui8_vid_duration =
            prRmInfo->rStmAttribute[i].u.t_rm_video_stm_attr.ui4_duration;
    }

    for ( ; (i < (prMediaInfo->u2AudStmNum + prMediaInfo->u2VidStmNum)) && (i < 17); i++)
    {
        if ((i - prMediaInfo->u2VidStmNum) < PLAYMGR_MAX_AUD_TRACK)
        {
            prMediaStatus->au4AudioId[i-prMediaInfo->u2VidStmNum] =
                prRmInfo->rStmAttribute[i].u.t_rm_audio_stm_attr.ui2_strm_num;

            prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
            prSrcInfo->t_es_map[i].ui4_strm_id =
                prRmInfo->rStmAttribute[i].u.t_rm_audio_stm_attr.ui2_strm_num;
        }
        else
        {
            LOG(3, "there r too many audio tracks\n");
            return FALSE;
        }
    }

    if (prRmInfo->u2VidIdx < 17)
    {
        prKeyInfo->ui4_strm_id = prSrcInfo->t_es_map[prRmInfo->u2VidIdx].ui4_strm_id;
    }
    prKeyInfo->ui4_avg_bytes_per_sec = 0;
#else  // __MODEL_slt__
    {
        XX_RM_SPEC_INFO_T* pt_rm_vid_spec_info
            = (XX_RM_SPEC_INFO_T*)&prRmInfo->rRmSpecInfo;
        pt_rm_vid_spec_info->b_is_rm8 = FALSE;
        pt_rm_vid_spec_info->ui2_frm_width = 640;
        pt_rm_vid_spec_info->ui2_frm_height = 480;
        pt_rm_vid_spec_info->ui4_spo_extra_flags = 0x481020;
    }
    prMediaStatus->u8FileSize = 1313929;
    prMediaStatus->u2VideoTracks = 1;
    prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_RV9;
    prMediaStatus->u2AudioTracks = 1;
    prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_RA;
    prMediaStatus->u2CaptionTracks = 0;

    prMediaStatus->au4VideoId[0] = 1;

    prSrcInfo->t_es_map[0].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
    prSrcInfo->t_es_map[0].ui4_strm_id = 1;
    //prSrcInfo->t_es_map[0].ui4_scale = 1;
    //prSrcInfo->t_es_map[0].ui4_rate = 0x1E;
    //prSrcInfo->t_es_map[0].ui4_sample_sz = 0;
    //prSrcInfo->t_es_map[0].ui4_avg_bytes_per_sec = 0;

    prMediaInfo->rRangeInfo[0].ui8_vid_duration = 21921;

    prMediaStatus->au4AudioId[0] = 0;

    prSrcInfo->t_es_map[1].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
    prSrcInfo->t_es_map[1].ui4_strm_id = 0;
    //prSrcInfo->t_es_map[1].ui4_scale = 1;
    //prSrcInfo->t_es_map[1].ui4_rate = 0x5DC0;
    //prSrcInfo->t_es_map[1].ui4_sample_sz = 1;
    //prSrcInfo->t_es_map[1].ui4_avg_bytes_per_sec = 0;
#endif  // __MODEL_slt__

    // Backup the media information
    x_memcpy(&prMediaInfo->rPlayerStatus, prMediaStatus, sizeof(PLAYMGR_STATUS_T));

    return TRUE;
}

static void _PLAYMGR_Minfo_Notify_Rm(
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
    PLAYMGR_RM_INFO_T* prRmInfo;
    MINFO_NFY_EVENT_TYPE_T      eEvent = e_event;

    zRecLength = sizeof(MINFO_INFO_T);
    prMediaInfo = (PLAYMGR_INFO_T*)pvTag;
    prRmInfo = &prMediaInfo->FmtInfo.rRmInfo;

    switch (e_event)
    {
    case MINFO_NFY_EVENT_PARSING_DONE:
        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_GENERIC, 1, &rMinfoRec, &zRecLength);
        if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
        {
            LOG(3, "get media geberic info error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }
        x_memcpy(&prRmInfo->rPlayerGenericInfo, &(rMinfoRec.u.t_general_info),
                 sizeof(MINFO_TYPE_GENERIC_T));

        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_IDX_INFO, 1, &rMinfoRec, &zRecLength);
        if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
        {
            LOG(3, "get media idx info error\n");
        }
        else
        {
            x_memcpy(&prRmInfo->rPlayerIdxInfo, &(rMinfoRec.u.t_idx_info),
                     sizeof(MINFO_TYPE_IDX_INFO_T));
        }

        i4Ret = x_minfo_get_num_recs(hMinfo, MINFO_INFO_TYPE_VIDEO_ATTR, &prMediaInfo->u2VidStmNum);
        if (i4Ret != MINFOR_OK)
        {
            LOG(1, "get media video recs error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }

        prRmInfo->u2AudIdx = prMediaInfo->u2VidStmNum;
        for (u2Temp = 0; u2Temp < prMediaInfo->u2VidStmNum && u2Temp < 17; u2Temp++)
        {
            i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_VIDEO_ATTR, (MINFO_REC_IDX_T)(u2Temp + 1), &rMinfoRec, &zRecLength);
            if (i4Ret != MINFOR_OK)
            {
                LOG(1, "get media video info error\n");
                eEvent = MINFO_NFY_EVENT_ERROR;
                break;
            }
            //printf("RM video stream num: %d\n", rMinfoRec.u.t_stm_attribute.u.t_rm_video_stm_attr.ui2_strm_num);
            x_memcpy(&prRmInfo->rStmAttribute[u2Temp],
                     &(rMinfoRec.u.t_stm_attribute),
                     sizeof(MINFO_TYPE_STM_ATRBT_T));

        }

        i4Ret = x_minfo_get_num_recs(hMinfo, MINFO_INFO_TYPE_AUDIO_ATTR, &prMediaInfo->u2AudStmNum);
        if (i4Ret != MINFOR_OK)
        {
            LOG(3, "get media audio recs error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }

        for ( ; (u2Temp < (prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum)) && (u2Temp < 17); u2Temp++)
        {
            i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_AUDIO_ATTR, (MINFO_REC_IDX_T)(u2Temp + 1 - prRmInfo->u2AudIdx), &rMinfoRec, &zRecLength);
            if (i4Ret != MINFOR_OK)
            {
                LOG(3, "get media audio info error\n");
                eEvent = MINFO_NFY_EVENT_ERROR;
                break;
            }
            //printf("RM audio stream num: %d\n", rMinfoRec.u.t_stm_attribute.u.t_rm_audio_stm_attr.ui2_strm_num);
            x_memcpy(&prRmInfo->rStmAttribute[u2Temp],
                &(rMinfoRec.u.t_stm_attribute),
                sizeof(MINFO_TYPE_STM_ATRBT_T));
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
        x_memcpy(&prRmInfo->rRmMetaData, &(rMinfoRec.u.t_meta_data), sizeof(MINFO_TYPE_META_DATA_T));

        break;

    case MINFO_NFY_EVENT_HANDLE_CLOSED:
        LOG(5, "_PLAYMGR_Minfo_Notify_Rm get MINFO_NFY_EVENT_HANDLE_CLOSED\n");
        break;

    case MINFO_NFY_EVENT_ERROR:
        LOG(5, "_PLAYMGR_Minfo_Notify_Rm get MINFO_NFY_EVENT_ERROR\n");
        break;

    default:
        LOG(5, "_PLAYMGR_Minfo_Notify_Rm get unknow ntf: %d\n", e_event);
        break;
    }

    _PLAYMGR_SendNotify(eEvent);
}

static VOID _PLAYMGR_MIdx_Notify_Rm(
                           HANDLE_T                    hMinfo,
                           MIDXBULD_NFY_EVENT_TYPE_T   e_event,
                           const VOID*                 pvData1,
                           const VOID*                 pvData2,
                           const VOID*                 pvTag)
{
    switch (e_event)
    {
    case MIDXBULD_NFY_EVENT_INDEXING_DONE:
        LOG(5, "_PLAYMGR_MIdx_Notify_Rm get MIDXBULD_NFY_EVENT_INDEXING_DONE\n");
        break;
    case MIDXBULD_NFY_EVENT_INDEXING_ABORT:
        LOG(5, "_PLAYMGR_MIdx_Notify_Rm get MIDXBULD_NFY_EVENT_INDEXING_ABORT\n");
        break;
    case MIDXBULD_NFY_EVENT_HANDLE_CLOSED:
        LOG(5, "_PLAYMGR_MIdx_Notify_Rm get MIDXBULD_NFY_EVENT_HANDLE_CLOSED\n");
        break;
    case MIDXBULD_NFY_EVENT_ERROR:
        LOG(5, "_PLAYMGR_MIdx_Notify_Rm get MIDXBULD_NFY_EVENT_ERROR\n");
        break;
    default:
        LOG(5, "_PLAYMGR_MIdx_Notify_Rm get unknow ntf: %d\n", e_event);
        break;
    }

    _PLAYMGR_SendNotify(e_event);
}


BOOL _PLAYMGR_RmStart(VOID* pvMediaInfo)
{
    BOOL fgRet;
    INT32 i4Ret;
    UINT32 u4Msg;
    SIZE_T zSize;

    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_RM_INFO_T* prRmInfo =
        (PLAYMGR_RM_INFO_T*)&prMediaInfo->FmtInfo.rRmInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prRmInfo->rSrcInfo;
    MIDXBULD_KEY_TBL_INFO_T* prTableInfo = &prRmInfo->rTableInfo;
    MIDXBULD_KEY_TBL_EXT_T*  prKeyTblList;
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt;
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prRmInfo->rKeyInfo;
    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTbl = &prRmInfo->rKeyIdxTBL;
//    MM_RANGE_ELMT_IDXTBL_T* prAudKeyIdxTbl = &prRmInfo->rAudKeyIdxTBL[0];

    LOG(5, "_PLAYMGR_RmStart\n");

    if (prMediaInfo == 0)
    {
        LOG(1, "_PLAYMGR_RmStart prMediaInfo == 0\n");
        return FALSE;
    }

    SWDMX_DelRange(NULL, (UINT32)prMediaInfo->rRangeInfo[0].ui4_range_id, FALSE);
    SWDMX_DelRange(NULL, (UINT32)prMediaInfo->rRangeInfo[1].ui4_range_id, FALSE);

#ifndef __MODEL_slt__
    if (prRmInfo->hMediaIdxHandle == 0)
    {
        LOG(1, "x_midxbuld_mopen\n");
        i4Ret = x_midxbuld_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _PLAYMGR_MIdx_Notify_Rm, 0,
                                prMediaFmt, prSrcInfo, prKeyInfo, &prRmInfo->hMediaIdxHandle);
        if(i4Ret != MIDXBULDR_OK)
        {
            LOG(1, "x_midxbuld_mopen NG: %d\n", i4Ret);
            return FALSE;
        }

        _PLAYMGR_ReceiveNotify((VOID*)&u4Msg);
        if (u4Msg != MIDXBULD_NFY_EVENT_INDEXING_DONE)
        {
            LOG(1, "_PLAYMGR_ReceiveNotify !MIDXBULD_NFY_EVENT_INDEXING_DONE: %d\n", u4Msg);
            return FALSE;
        }
    }

    zSize = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
    prTableInfo->ui4_key_tbl_id = prKeyInfo->ui4_tbl_id;
    i4Ret = x_midxbuld_get_info(prRmInfo->hMediaIdxHandle, MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST,
        (VOID*)prTableInfo, &zSize);
    if (i4Ret)
    {
        LOG(1, "get midxbuild elmt tbl error\n");
    }

    //video key table
    prKeyTblList = prTableInfo->pt_key_tbl_ext;
    if (NULL == prKeyTblList)
    {
        LOG(3, "_PLAYMGR_RmStart: No video key table!!!\n");
    }
    else
    {
        prKeyIdxTbl->e_elem_type = MM_ELEMT_TYPE_KEY;
        prKeyIdxTbl->t_strm_id.u.t_stm_rm_id = prKeyTblList->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id;
        prKeyIdxTbl->ui4_number_of_entry = prKeyTblList->t_elmt_tbl.ui4_number_of_entry;
        prKeyIdxTbl->ui8_base_offset = prKeyTblList->t_elmt_tbl.ui8_base_offset;
        prKeyIdxTbl->u.pt_pb_idx_tbl_entry =
            (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)prKeyTblList->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
//            prKeyTblList = prKeyTblList->pt_next;   //key tbl info from mutil
    }
#endif  // !__MODEL_slt__

    prMediaInfo->u1CurEmptyRange = 0;
    prMediaInfo->u4CurRangePTS = 0;
    _PLAYMGR_RmUpdateRange(pvMediaInfo);

    fgRet = _SWDMX_Play();
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_RmStart Fail to play\n");
        return FALSE;
    }

    return TRUE;
}


BOOL _PLAYMGR_RmStop(VOID* pvMediaInfo)
{
    BOOL fgRet = TRUE;
    //PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    fgRet = _SWDMX_Stop();
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_RmStop Fail in _SWDMX_Stop\n");
        return FALSE;
    }

    return TRUE;
}

BOOL _PLAYMGR_RmClose(VOID* pvMediaInfo)
{
    UINT32 u4Msg;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    if (prMediaInfo->FmtInfo.rRmInfo.hMediaInfoHandle != 0)
    {
        x_minfo_close(prMediaInfo->FmtInfo.rRmInfo.hMediaInfoHandle);
        _PLAYMGR_ReceiveNotify((VOID*)&u4Msg);
        if (u4Msg != MINFO_NFY_EVENT_HANDLE_CLOSED)
        {
            return FALSE;
        }
    }
    if (prMediaInfo->FmtInfo.rRmInfo.hMediaIdxHandle != 0)
    {
        x_midxbuld_close(prMediaInfo->FmtInfo.rRmInfo.hMediaIdxHandle);
        _PLAYMGR_ReceiveNotify((VOID*)&u4Msg);
        if (u4Msg != MIDXBULD_NFY_EVENT_HANDLE_CLOSED)
        {
            return FALSE;
        }
    }

    x_memset(&prMediaInfo->FmtInfo.rRmInfo, 0, sizeof(PLAYMGR_RM_INFO_T));

    return TRUE;
}

EXTERN BOOL _PLAYMGR_RmResume(VOID* pvMediaInfo)
{
    BOOL fgRet;

    fgRet = _SWDMX_Play();
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_RmResume Fail to play\n");
        return FALSE;
    }

    return TRUE;
}

BOOL _PLAYMGR_RmPause(VOID)
{
    return _SWDMX_Pause();
}


BOOL _PLAYMGR_OpenRmMediaInfo(PLAYMGR_STATUS_T* prMediaStatus, VOID* pvMediaInfo)
{
    INT32 i4Ret;
    UINT32 u4Msg;

    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    PLAYMGR_RM_INFO_T* prRmInfo =
    (PLAYMGR_RM_INFO_T*)&prMediaInfo->FmtInfo.rRmInfo;
    MEDIA_FORMAT_T* prMediaFmt = &prMediaInfo->rMediaFmt;

    //MIDXBULD_KEY_INFO_T* prKeyInfo = &prRmInfo->rKeyInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prRmInfo->rSrcInfo;
    //MIDXBULD_ELMT_TBL_INFO_T* prTableInfo = &prRmInfo->rTableInfo;
    //MIDXBULD_ELMT_TBL_ALLOC_INFO_T* prAllocTblInfo = &prRmInfo->rAllocTblInfo;
    MIDXBULD_ELMT_TBL_DESCRIB_T* prVDescrib = &prRmInfo->rVDescrib[0];
    MIDXBULD_ELMT_TBL_DESCRIB_T* prADescrib = &prRmInfo->rADescrib[0];
    //MIDXBULD_ELMT_TBL_DESCRIB_T* prFirstDescrib = NULL;

    MM_RANGE_ELMT_IDXTBL_T* prVidElmtStrm = &prRmInfo->rVidIDXTBL[0];
    MM_RANGE_ELMT_IDXTBL_T* prAudElmtStrm = &prRmInfo->rAudIDXTBL[0];

    //SIZE_T zSize;

    //prMediaInfo->u1CurEmptyRange = 1;

    // Open for media info
    i4Ret = x_minfo_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _PLAYMGR_Minfo_Notify_Rm, prMediaInfo,
                          prMediaFmt, &(prRmInfo->hMediaInfoHandle));
    if(i4Ret != MINFOR_OK)
    {
        return FALSE;
    }

    _PLAYMGR_ReceiveNotify((VOID*)&u4Msg);
    if (u4Msg != MINFO_NFY_EVENT_PARSING_DONE)
    {
        return FALSE;
    }

#ifndef __MODEL_slt__
    // Wait for media info's notify
    prSrcInfo->ui8_data_offset = prRmInfo->rPlayerIdxInfo.ui8_data_offset;
    prSrcInfo->ui8_data_size = prRmInfo->rPlayerIdxInfo.ui8_data_sz;
    prSrcInfo->ui8_idx_offset = prRmInfo->rPlayerIdxInfo.ui8_idx_offset;
    prSrcInfo->ui8_idx_size = prRmInfo->rPlayerIdxInfo.ui8_idx_sz;
    prSrcInfo->ui4_idx_type = prRmInfo->rPlayerIdxInfo.ui4_idx_type;
    prSrcInfo->ui1_total_strm_num = (UINT8)(prMediaInfo->u2VidStmNum + prMediaInfo->u2AudStmNum);
    // Temporally multiply SYSTEM_TIME_SCALE
    prSrcInfo->ui4_total_duration = (UINT32)prRmInfo->rPlayerGenericInfo.ui8_pb_duration;// * SYSTEM_TIME_SCALE;
    prMediaStatus->u4TotalDuration = (UINT32)prRmInfo->rPlayerGenericInfo.ui8_pb_duration;
    prMediaInfo->rPlayerStatus.u4TotalDuration = (UINT32)prRmInfo->rPlayerGenericInfo.ui8_pb_duration;
#else
    // Wait for media info's notify
    prSrcInfo->ui8_data_offset = 850;
    prSrcInfo->ui8_data_size = 1312627;
    prSrcInfo->ui8_idx_offset = 1313685;
    prSrcInfo->ui8_idx_size = 20;
    prSrcInfo->ui4_idx_type = prRmInfo->rPlayerIdxInfo.ui4_idx_type;  // 2?
    prSrcInfo->ui1_total_strm_num = 2;
    // Temporally multiply SYSTEM_TIME_SCALE
    prSrcInfo->ui4_total_duration = 1993950;// * SYSTEM_TIME_SCALE;
    prMediaStatus->u4TotalDuration = 1993950;
    prMediaInfo->rPlayerStatus.u4TotalDuration = 1993950;
#endif

    //SWDMX_SetInfo(eSWDMX_TOTAL_DURATION, prSrcInfo->ui4_total_duration, 0, 0);

    if (_PLAYMGR_SetRMInfo(prMediaStatus, prMediaInfo) != TRUE)
    {
        return FALSE;
    }

#ifndef __MODEL_slt__
    prMediaInfo->rRangeInfo[0].ui8_fileoffset = prSrcInfo->ui8_data_offset;
    prMediaInfo->rRangeInfo[0].z_range_sz = prSrcInfo->ui8_data_size;
#else
    prMediaInfo->rRangeInfo[0].ui8_fileoffset = 850;
    prMediaInfo->rRangeInfo[0].z_range_sz = 1312627;
#endif

    if (prMediaInfo->u2VidStmNum > 0 && prRmInfo->u2VidIdx < 17)
    {
        prMediaInfo->rRangeInfo[0].pt_idxtbl_lst = prVidElmtStrm;

        prVidElmtStrm->e_elem_type = MM_ELEMT_TYPE_VIDEO;
        prVidElmtStrm->t_strm_id.u.t_stm_rm_id = prSrcInfo->t_es_map[prRmInfo->u2VidIdx].ui4_strm_id;
        prVidElmtStrm->ui4_rate = prSrcInfo->t_es_map[prRmInfo->u2VidIdx].ui4_rate;
        prVidElmtStrm->ui4_scale = prSrcInfo->t_es_map[prRmInfo->u2VidIdx].ui4_scale;
        prVidElmtStrm->ui4_avg_bytes_per_sec =
            prSrcInfo->t_es_map[prRmInfo->u2VidIdx].ui4_avg_bytes_per_sec;
        prVidElmtStrm->ui4_number_of_entry = 1;

        prVDescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;
        prVDescrib->u_info.t_es.ui4_strm_id =
            prSrcInfo->t_es_map[prRmInfo->u2VidIdx].ui4_strm_id;
        prVDescrib->u_info.t_es.ui4_scale =
            prSrcInfo->t_es_map[prRmInfo->u2VidIdx].ui4_scale;
        prVDescrib->u_info.t_es.ui4_rate =
            prSrcInfo->t_es_map[prRmInfo->u2VidIdx].ui4_rate;
        prVDescrib->u_info.t_es.ui4_sample_sz = 0;
        prVDescrib->u_info.t_es.ui4_avg_bytes_per_sec = 0;

        prVDescrib->u_info.t_es.ui4_indx_offset =
            prSrcInfo->t_es_map[prRmInfo->u2VidIdx].ui4_indx_offset;
        prVDescrib->u_info.t_es.ui4_indx_size =
            prSrcInfo->t_es_map[prRmInfo->u2VidIdx].ui4_indx_size;
    }

    if (prMediaInfo->u2AudStmNum > 0 && prRmInfo->u2AudIdx < 17)
    {
        (prMediaInfo->u2VidStmNum > 0) ? (prVidElmtStrm->pv_next_tbl = prAudElmtStrm)
        : (prMediaInfo->rRangeInfo[0].pt_idxtbl_lst = prAudElmtStrm);

        prAudElmtStrm->e_elem_type = MM_ELEMT_TYPE_AUDIO;
        prAudElmtStrm->t_strm_id.u.t_stm_rm_id = prSrcInfo->t_es_map[prRmInfo->u2AudIdx].ui4_strm_id;
        prAudElmtStrm->ui4_rate = prSrcInfo->t_es_map[prRmInfo->u2AudIdx].ui4_rate;
        prAudElmtStrm->ui4_scale = prSrcInfo->t_es_map[prRmInfo->u2AudIdx].ui4_scale;
        prAudElmtStrm->ui4_avg_bytes_per_sec =
            prSrcInfo->t_es_map[prRmInfo->u2AudIdx].ui4_avg_bytes_per_sec;
        prAudElmtStrm->ui4_number_of_entry = 1;

        prADescrib->u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
        prADescrib->u_info.t_es.ui4_strm_id =
          prSrcInfo->t_es_map[prRmInfo->u2AudIdx].ui4_strm_id;
        prADescrib->u_info.t_es.ui4_scale =
          prSrcInfo->t_es_map[prRmInfo->u2AudIdx].ui4_scale;
        prADescrib->u_info.t_es.ui4_rate =
          prSrcInfo->t_es_map[prRmInfo->u2AudIdx].ui4_rate;
        prADescrib->u_info.t_es.ui4_sample_sz =
          prSrcInfo->t_es_map[prRmInfo->u2AudIdx].ui4_sample_sz ;
        prADescrib->u_info.t_es.ui4_avg_bytes_per_sec =
          prSrcInfo->t_es_map[prRmInfo->u2AudIdx].ui4_avg_bytes_per_sec;

        prADescrib->u_info.t_es.ui4_indx_offset =
            prSrcInfo->t_es_map[prRmInfo->u2AudIdx].ui4_indx_offset;
        prADescrib->u_info.t_es.ui4_indx_size =
            prSrcInfo->t_es_map[prRmInfo->u2AudIdx].ui4_indx_size;

    }

    return TRUE;
}


BOOL _PLAYMGR_RmUpdateRange(VOID* pvMediaInfo)
{
    INT32 i4RangeIdx;
    //UINT32 u4Msg;
    MM_RANGE_ELMT_IDXTBL_T* prKeyIdxTbl = NULL;
//    MM_RANGE_ELMT_IDXTBL_T* prAudKeyIdxTbl = NULL;
    MM_RANGE_ELMT_IDXTBL_T* prVidElmtStrm = NULL;
    MM_RANGE_ELMT_IDXTBL_T* prAudElmtStrm = NULL;
    MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;

    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_RM_INFO_T* prRmInfo =
        (PLAYMGR_RM_INFO_T*)&prMediaInfo->FmtInfo.rRmInfo;

    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prRmInfo->rSrcInfo;

    i4RangeIdx = prMediaInfo->u1CurEmptyRange;

#ifndef __MODEL_slt__
    if (NULL == pvMediaInfo)
    {
        return FALSE;
    }
    prKeyIdxTbl = &prRmInfo->rKeyIdxTBL;
//    prAudKeyIdxTbl = &prRmInfo->rAudKeyIdxTBL[0];
    prVidElmtStrm = &prRmInfo->rVidIDXTBL[i4RangeIdx];
    prAudElmtStrm = &prRmInfo->rAudIDXTBL[i4RangeIdx];

    // link key index to the end of the list
    if (i4RangeIdx < MAX_RANGE_NUM)
    {
        if (prMediaInfo->rRangeInfo[i4RangeIdx].pt_idxtbl_lst)
        {
            prIdxTbl = prMediaInfo->rRangeInfo[i4RangeIdx].pt_idxtbl_lst;
        }
        else
        {
            return FALSE;
        }
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
        prKeyIdxTbl->t_strm_id.u.t_stm_rm_id = prVidElmtStrm->t_strm_id.u.t_stm_rm_id;
        prIdxTbl->pv_next_tbl = prKeyIdxTbl;
        prIdxTbl = prIdxTbl->pv_next_tbl;
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

#endif  // __MODEL_slt__

    if (i4RangeIdx < MAX_RANGE_NUM)
    {
        prMediaInfo->rRangeInfo[i4RangeIdx].ui4_range_id =
            prRmInfo->rAllocTblInfo[i4RangeIdx].ui4_range_id;

        LOG(3, "Allocate prRmInfo->rAllocTblInfo[%d].ui4_range_id = %ld, pv_tag = %ld\n",
            i4RangeIdx, prRmInfo->rAllocTblInfo[i4RangeIdx].ui4_range_id,
            prMediaInfo->rRangeInfo[i4RangeIdx].ui4_range_id);

        if((prVidElmtStrm->ui4_number_of_entry > 0) ||
           (prAudElmtStrm->ui4_number_of_entry > 0))
        {
            prMediaInfo->rRangeInfo[i4RangeIdx].ui8_pb_duration = (UINT64)prSrcInfo->ui4_total_duration;
            SWDMX_AddRange(&prMediaInfo->rRangeInfo[i4RangeIdx],
                                    (UINT32)prMediaInfo->rRangeInfo[i4RangeIdx].ui4_range_id);
        }
        else
        {
            LOG(3, "range render cnt 0!\n");
        }
    }
    return TRUE;
}



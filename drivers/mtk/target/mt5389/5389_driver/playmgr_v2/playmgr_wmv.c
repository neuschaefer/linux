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
 * $RCSfile: playmgr_wmv.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

#if !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)
#include "playmgr.h"
#include "feeder_if.h"
#include "playmgr_util.h"
#include "swdmx_drvif.h"
#include "playmgr_wmv.h"
#include "playmgr_debug.h"

extern void _AUD_DspWMAHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo);

static UINT32 _u4IdxOfPraingBitstream = 0;
static UINT32 _u4NumberOfBitstream = 1;
static UINT8 _u1NetflixVideoId = 0xff;
static UINT8 _u1NetflixAudioId = 0xff;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


static BOOL _PLAYMGR_SetWmvInfo(PLAYMGR_STATUS_T* prMediaStatus, PLAYMGR_INFO_T* prMediaInfo)
{
#ifndef __MODEL_slt__
    UINT16 i;
#endif
    //UINT32 u4Temp;
    PLAYMGR_WMV_INFO_T* prWmvInfo = &prMediaInfo->FmtInfo.rWmvInfo;
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prWmvInfo->rWmvSrcInfo;
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prWmvInfo->rWmvKeyInfo;
    //netflix
    //PLAYMGR_SourceType_T ePlayMgrSrcType = PLAYMGR_SRC_TYPE_HIGH_SPEED_STORAGE;

    //ePlayMgrSrcType = _PLAYMGR_GetSrcType(prMediaInfo->u1SrcId);
    prMediaStatus->eMediaType = PLAYMGR_MEDIA_TYPE_VC1_WMV;
#ifndef __MODEL_slt__
    prMediaStatus->u8FileSize = prMediaInfo->FmtInfo.rWmvInfo.rWmvGenericInfo[0].ui8_media_sz;

    if ((prMediaInfo->u2VidStmNum > 0) &&
            (prWmvInfo->u2VidIdx < 17) &&
            (prWmvInfo->u1VideoStreamIdx < 10))
    {
        MINFO_INFO_VIDEO_ENC_T eVideoFormat;
        prMediaStatus->u2VideoTracks = prMediaInfo->u2VidStmNum;
        LOG(5, "Number of video tracks in this wmv is %ld\n", prMediaStatus->u2VideoTracks);
        for(i=0;i<prMediaStatus->u2VideoTracks;i++)
        {   
            if(i<PLAYMGR_MAX_VID_TRACK)
            {
               eVideoFormat= prWmvInfo->rStmAttribute[_u4IdxOfPraingBitstream][i].u.t_asf_video_stm_attr.e_enc;
               switch (eVideoFormat)
               {
                  case MINFO_INFO_VID_ENC_MPEG_1:
                  case MINFO_INFO_VID_ENC_MPEG_2:
                    prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_MPEG1_MPEG2;
                    break;
                  case MINFO_INFO_VID_ENC_DIVX_311:
                    prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_DIVX311;
                    break;
                  case MINFO_INFO_VID_ENC_DIVX_4:
                  case MINFO_INFO_VID_ENC_DIVX_5:
                  case MINFO_INFO_VID_ENC_XVID:
                  case MINFO_INFO_VID_ENC_MPEG_4:
                    prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_MPEG4;
                  break;
                  case MINFO_INFO_VID_ENC_H264:
                    prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_H264;
                    break;
                  case MINFO_INFO_VID_ENC_MJPG:
                    prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_MJPEG;
                   break;
                  case MINFO_INFO_VID_ENC_WMV1:
                    prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_WMV7;
                    break;
                  case MINFO_INFO_VID_ENC_WMV2:
                    prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_WMV8;
                    break;
                  case MINFO_INFO_VID_ENC_WMV3:
                    prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_WMV9;
                    break;
                  case MINFO_INFO_VID_ENC_WVC1:
                    prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_VC1;
                    break;
                  default:
                    LOG(3, "Unknown Video %d\n", (eVideoFormat));
                    prMediaStatus->aeVidFormat[i] = PLAYMGR_VDEC_UNKNOWN;
                    break;             
                }
               
#ifdef SUPPORT_3D_MEDIA
                if(_u4IdxOfPraingBitstream<10 && prMediaStatus->u2VideoTracks<17)
                {
                    if(prWmvInfo->rWmvGenericInfo[_u4IdxOfPraingBitstream].b_stereo_scopic && (prWmvInfo->rWmvGenericInfo[_u4IdxOfPraingBitstream].e_scopic_type==MM_SCOPIC_DS))
                    {
                       prMediaStatus->aeSubVidFormat[i]=PLAYMGR_VDEC_MULTITRACK;
                       prMediaStatus->au4SubVideoId[i]=prWmvInfo->rStmAttribute[_u4IdxOfPraingBitstream][prMediaStatus->u2VideoTracks-1].t_stm_id.u.t_asf_stm_id;
                    }
                    else
                    {
                       prMediaStatus->aeSubVidFormat[i]=PLAYMGR_VDEC_UNKNOWN;
                       prMediaStatus->au4SubVideoId[i]=0;
                    }
                }
#endif
            }
            //return FALSE;
        }

#if !defined(CC_MT5392B) && !defined(CC_MT5365) && !defined(CC_MT5395) && !defined(CC_MT5396) && !defined(CC_MT5368)&& !defined(CC_MT5389)
        if(eVideoFormat == MINFO_INFO_VID_ENC_WMV1 || eVideoFormat == MINFO_INFO_VID_ENC_WMV2
            || eVideoFormat == MINFO_INFO_VID_ENC_WMV3 || eVideoFormat == MINFO_INFO_VID_ENC_WVC1)
        {
            prMediaStatus->u2VideoTracks = 0;
        }
#endif
    }

    if ((prMediaInfo->u2AudStmNum > 0) &&
            (prWmvInfo->u2AudIdx < 17) &&
            (prWmvInfo->u1AudioStreamIdx <10))
    {
        prMediaStatus->u2AudioTracks = prMediaInfo->u2AudStmNum;
        LOG(5, "Number of audio tracks in this wmv is %ld\n", prMediaStatus->u2AudioTracks);

        switch (prWmvInfo->rStmAttribute[prWmvInfo->u1AudioStreamIdx][prWmvInfo->u2AudIdx].u.t_asf_audio_stm_attr.e_enc)
        {
        case MINFO_INFO_AUD_ENC_MPEG_1:
        case MINFO_INFO_AUD_ENC_MPEG_2:
            prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_MP3;
            break;
        case MINFO_INFO_AUD_ENC_AC3:
            prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_AC3;
            break;
        case MINFO_INFO_AUD_ENC_PCM:
            prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_PCM;
            break;
        case MINFO_INFO_AUD_ENC_WMA_V1:
        case MINFO_INFO_AUD_ENC_WMA_V2:
        case MINFO_INFO_AUD_ENC_WMA_V3:
            prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_WMA;
            break;
        default:
            LOG(3, "Unknown Audio %d\n", (prWmvInfo->rStmAttribute[0][prWmvInfo->u2AudIdx].u.t_asf_audio_stm_attr.e_enc));
            prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_UNKNOWN;
            break;
        }
    }

    //only set index key table info in usb mode
    //if(ePlayMgrSrcType == PLAYMGR_SRC_TYPE_HIGH_SPEED_STORAGE)
    {
        for (i = 0; (i < prMediaInfo->u2VidStmNum) && (i < 17); i++)
        {
            prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;

            if (i < PLAYMGR_MAX_VID_TRACK)
            {
                prMediaStatus->au4VideoId[i] = prWmvInfo->rStmAttribute[0][i].t_stm_id.u.t_asf_stm_id;
            }

            prSrcInfo->t_es_map[i].ui4_strm_id =
                prWmvInfo->rStmAttribute[0][i].t_stm_id.u.t_asf_stm_id;
            prSrcInfo->t_es_map[i].ui4_scale =
                prWmvInfo->rStmAttribute[0][i].u.t_asf_video_stm_attr.ui4_scale;
            prSrcInfo->t_es_map[i].ui4_rate =
                prWmvInfo->rStmAttribute[0][i].u.t_asf_video_stm_attr.ui4_rate;
            prSrcInfo->t_es_map[i].ui4_avg_bytes_per_sec = 0;
            prSrcInfo->t_es_map[i].ui4_sample_sz = 0;
        }

        for (; (i < prMediaInfo->u2AudStmNum + prMediaInfo->u2VidStmNum) && (i < 17); i++)
        {
            if (((i - prMediaInfo->u2VidStmNum) >= 0) && ((i - prMediaInfo->u2VidStmNum) < PLAYMGR_MAX_AUD_TRACK))
            {
                prMediaStatus->au4AudioId[i-prMediaInfo->u2VidStmNum] =
                    prWmvInfo->rStmAttribute[0][i].t_stm_id.u.t_asf_stm_id;

                prSrcInfo->t_es_map[i].e_es_type = MIDXBULD_ELEM_STRM_TYPE_AUDIO;
                prSrcInfo->t_es_map[i].ui4_strm_id =
                    prWmvInfo->rStmAttribute[0][i].t_stm_id.u.t_asf_stm_id;

                prSrcInfo->t_es_map[i].ui4_scale = 0;
                prSrcInfo->t_es_map[i].ui4_rate = 0;
                prSrcInfo->t_es_map[i].ui4_sample_sz = 0;
                prSrcInfo->t_es_map[i].ui4_avg_bytes_per_sec = 0;

            }
            else
            {
                LOG(3, "there r too many audio tracks\n");
                return FALSE;
            }
        }

        if (prWmvInfo->u2VidIdx < 17)
        {
            prKeyInfo->ui4_strm_id = prSrcInfo->t_es_map[prWmvInfo->u2VidIdx].ui4_strm_id;
            prKeyInfo->ui4_scale =
                prWmvInfo->rStmAttribute[0][prWmvInfo->u2VidIdx].u.t_asf_video_stm_attr.ui4_scale;
            prKeyInfo->ui4_rate =
                prWmvInfo->rStmAttribute[0][prWmvInfo->u2VidIdx].u.t_asf_video_stm_attr.ui4_rate;
            prKeyInfo->ui4_sample_sz = 0;
            prKeyInfo->ui4_avg_bytes_per_sec = 0;
			x_memcpy( (VOID*)prKeyInfo->aui1_idx_strm_map, 
					  (const VOID*)prWmvInfo->rWmvIdxInfo.aui1_idx_strm_map, 
					  sizeof(prWmvInfo->rWmvIdxInfo.aui1_idx_strm_map) );
            //prKeyInfo->ui4_range_id = 0xff;
            //prKeyInfo->ui4_tbl_id = 0xff;
        }
    }
#else   // __MODEL_slt__
    prMediaStatus->u8FileSize = 6861515;
    prMediaStatus->u2VideoTracks = 1;
    prMediaStatus->aeVidFormat[0] = PLAYMGR_VDEC_VC1;
    prMediaStatus->u2AudioTracks = 0;
    prMediaStatus->aeAudFormat[0] = PLAYMGR_ADEC_UNKNOWN;
    prMediaStatus->u2CaptionTracks = 0;

    prMediaStatus->au4VideoId[0] = 2;
    
    prSrcInfo->t_es_map[0].ui4_strm_id = 2;
    prSrcInfo->t_es_map[0].ui4_scale = 1000;
    prSrcInfo->t_es_map[0].ui4_rate = 18215;
    prSrcInfo->t_es_map[0].ui4_avg_bytes_per_sec = 0;
    prSrcInfo->t_es_map[0].ui4_sample_sz = 0;
    prSrcInfo->t_es_map[0].e_es_type = MIDXBULD_ELEM_STRM_TYPE_VIDEO;

    prKeyInfo->ui4_strm_id = 2;
    prKeyInfo->ui4_scale = 1000;
    prKeyInfo->ui4_rate = 18215;
    prKeyInfo->ui4_sample_sz = 0;
    prKeyInfo->ui4_avg_bytes_per_sec = 0;
#endif  // __MODEL_slt__

    // Backup the media information
    x_memcpy(&prMediaInfo->rPlayerStatus, prMediaStatus, sizeof(PLAYMGR_STATUS_T));

    return TRUE;
}


static VOID _PLAYMGR_WmvMediaInfoNotify(
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
    PLAYMGR_INFO_T *prMediaInfo;
    PLAYMGR_WMV_INFO_T* prWmvInfo;
    MINFO_NFY_EVENT_TYPE_T eEvent = e_event;
    UINT16 u2Temp;

    //u4RecLength = sizeof(MINFO_INFO_T);
    zRecLength = sizeof(MINFO_INFO_T);
    prMediaInfo = (PLAYMGR_INFO_T*)pvTag;
    prWmvInfo = &prMediaInfo->FmtInfo.rWmvInfo;

    LOG(0,"_PLAYMGR_WmvMediaInfoNotify : Event type = %s\r\n",apsz_parse_Info[e_event]);

    switch (e_event)
    {
    case MINFO_NFY_EVENT_PARSING_DONE:
        LOG(0,"event : MINFO_NFY_EVENT_PARSING_DONE\n");

        //1. get generic info
        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_GENERIC, 1,&rMinfoRec, &zRecLength);
        if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
        {
            LOG(3, "get media geberic info error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }
        x_memcpy(&(prWmvInfo->rWmvGenericInfo[_u4IdxOfPraingBitstream]), &(rMinfoRec.u.t_general_info),sizeof(MINFO_TYPE_GENERIC_T));

        //2. get idx info
        i4Ret = x_minfo_get_info(hMinfo, MINFO_INFO_TYPE_IDX_INFO, 1,&rMinfoRec, &zRecLength);
        if ((zRecLength != sizeof(MINFO_INFO_T)) || i4Ret)
        {
            LOG(3, "get media idx info error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }
        x_memcpy(&(prWmvInfo->rWmvIdxInfo), &(rMinfoRec.u.t_idx_info),sizeof(MINFO_TYPE_IDX_INFO_T));

        //3. get video info
        i4Ret = x_minfo_get_num_recs( (prWmvInfo->hWmvMediaInfoHandle),MINFO_INFO_TYPE_VIDEO_ATTR,&prMediaInfo->u2VidStmNum);
        if (i4Ret != MINFOR_OK)
        {
            LOG(1, "get media video recs error\n");
            eEvent = MINFO_NFY_EVENT_ERROR;
            break;
        }
        prWmvInfo->u2AudIdx = prMediaInfo->u2VidStmNum;

        for (u2Temp = 0 ; u2Temp < prMediaInfo->u2VidStmNum; ++u2Temp)
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

                x_memcpy(&prWmvInfo->rStmAttribute[_u4IdxOfPraingBitstream][u2Temp],
                         &(rMinfoRec.u.t_stm_attribute),
                         sizeof(MINFO_TYPE_STM_ATRBT_T));
                _u1NetflixVideoId = prWmvInfo->rStmAttribute[_u4IdxOfPraingBitstream][u2Temp].t_stm_id.u.t_asf_stm_id;
                prWmvInfo->u1VideoStreamIdx = _u4IdxOfPraingBitstream;
            }

        }

        //4. get audio info
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
                i4Ret = x_minfo_get_info(hMinfo,MINFO_INFO_TYPE_AUDIO_ATTR, (MINFO_REC_IDX_T)(u2Temp + 1 - prWmvInfo->u2AudIdx), &rMinfoRec, &zRecLength);
                if (i4Ret != MINFOR_OK)
                {
                    LOG(3, "get media audio info error\n");
                    eEvent = MINFO_NFY_EVENT_ERROR;
                    break;
                }

                x_memcpy(&prWmvInfo->rStmAttribute[_u4IdxOfPraingBitstream][u2Temp],
                         &(rMinfoRec.u.t_stm_attribute),
                         sizeof(MINFO_TYPE_STM_ATRBT_T));
                _u1NetflixAudioId = prWmvInfo->rStmAttribute[_u4IdxOfPraingBitstream][u2Temp].t_stm_id.u.t_asf_stm_id;
                prWmvInfo->u1AudioStreamIdx = _u4IdxOfPraingBitstream;
            }
        }


        //5. get meta info
        t_size = sizeof(MINFO_INFO_T);
        x_memset(&rMinfoRec, 0, t_size);
        i4Ret = x_minfo_get_info((prWmvInfo->hWmvMediaInfoHandle),MINFO_INFO_TYPE_META_DATA, 1, (VOID*)&rMinfoRec, &t_size);

        if (MINFOR_OK != i4Ret)
        {
            LOG(0,"playmgr wmv get meta error\n",i4Ret);
            eEvent = MINFO_NFY_EVENT_ERROR;
            return;
        }
        x_memcpy(&(prWmvInfo->rWmvMetaData),&(rMinfoRec.u.t_meta_data),sizeof(MINFO_TYPE_META_DATA_T));

        break;
    case MINFO_NFY_EVENT_HANDLE_CLOSED:
        break;
    case MINFO_NFY_EVENT_ERROR:
        break;
    default:
        break;
    }
    _PLAYMGR_SendNotify(prMediaInfo->u1SrcId, eEvent);
    return;
}

#ifndef __MODEL_slt__
static VOID _PLAYMGR_WmvMediaIdxNotify(
    HANDLE_T                    hMinfo,
    MIDXBULD_NFY_EVENT_TYPE_T   e_event,
    const VOID*                 pvData1,
    const VOID*                 pvData2,
    const VOID*                 pvTag)

{
    //UINT32 i4Ret;
    //UINT32 u4RecLength;
    PLAYMGR_INFO_T *prMediaInfo = (PLAYMGR_INFO_T*)pvTag;

    switch (e_event) {
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
    return;
}

#endif
BOOL _PLAYMGR_GetWMVSupport(PLAYMGR_VDEC_T rDecoderType,SeqHeaderInfo* prHeaderInfo)
{
#if 1//efined(CC_MT5392B) || defined(CC_MT5365) || defined(CC_MT5395) ||(CC_MT5396) 
    INT32  i4SeqHdrData1;
    BOOL    fgXintra8Switch;

    if(rDecoderType == PLAYMGR_VDEC_WMV9 || rDecoderType == PLAYMGR_VDEC_WMV8)
    {
        if(prHeaderInfo->u1Addr != NULL)
        {
            x_memcpy(&i4SeqHdrData1,(prHeaderInfo->u1Addr),4);

            i4SeqHdrData1 = ((i4SeqHdrData1&0x000000FF)<<24) |
                            ((i4SeqHdrData1&0x0000FF00)<<8) |
                            ((i4SeqHdrData1&0x00FF0000)>>8) |
                            ((i4SeqHdrData1&0xFF000000)>>24);

            if((prHeaderInfo->u4Len) <4 )
                return FALSE;

            if(rDecoderType == PLAYMGR_VDEC_WMV8)
            {
                if((prHeaderInfo->u4Len) >=4)
                {

                    fgXintra8Switch  = (i4SeqHdrData1& 0x00001000) >> 12;

                    return (!fgXintra8Switch);
                }
                else
                {
                    return TRUE;
                }
            }
            else if(rDecoderType == PLAYMGR_VDEC_WMV9)
            {
                BOOL  fgYUV411;
                INT32 i4Profile;
                i4Profile = (i4SeqHdrData1& 0xc0000000) >> 30;
                fgYUV411 = (i4SeqHdrData1 & 0x20000000) >> 29;

                if(i4Profile ==WMV3_PC_PROFILE || fgYUV411)
                {
                    return FALSE;
                }
                else if(i4Profile == WMV3_SIMPLE_PROFILE || i4Profile == WMV3_MAIN_PROFILE)
                {
                    if(!(i4SeqHdrData1&0x1))
                    {
                        return FALSE;
                    }
                    else
                    {
                        return TRUE;
                    }
                }
                else
                    return TRUE;
            }
            else
            {
                return TRUE;
            }
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        return TRUE;
    }
#else
    return FALSE;
#endif
}

BOOL _PLAYMGR_WmvStart(VOID* pvMediaInfo)
{
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    //PLAYMGR_WMV_INFO_T* prWmvInfo = (PLAYMGR_WMV_INFO_T*)&prMediaInfo->FmtInfo.rWmvInfo;
    INT32 i4Ret;
    BOOL fgAddRange = FALSE;

    LOG(5, "_PLAYMGR_WmvStart\n");

    if (prMediaInfo->u2AudStmNum > 0 || prMediaInfo->u2VidStmNum > 0 )
    {
        fgAddRange = TRUE;
    }
    if (fgAddRange)
    {
        i4Ret = SWDMX_AddRange(prMediaInfo->u1SrcId, &prMediaInfo->rRangeInfo[0], prMediaInfo->rRangeInfo[0].ui4_range_id);
        if (i4Ret == FALSE)
        {
            LOG(1, "_PLAYMGR_WmvStop Fail in _SWDMX_AddRange \n");
            return FALSE;
        }
    }
    return TRUE;
}


BOOL _PLAYMGR_WmvStop(VOID* pvMediaInfo)
{
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    BOOL fgRet = TRUE;

    fgRet = _SWDMX_Stop(prMediaInfo->u1SrcId);
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_WmvStop Fail in _SWDMX_Stop\n");
        return FALSE;
    }

    return TRUE;
}

EXTERN BOOL _PLAYMGR_WmvClose(VOID* pvMediaInfo)
{
    UINT32 u4Msg;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    if (prMediaInfo->FmtInfo.rWmvInfo.hWmvMediaInfoHandle != 0)
    {
        x_minfo_close(prMediaInfo->FmtInfo.rWmvInfo.hWmvMediaInfoHandle);
        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (void*)&u4Msg);
        if (u4Msg != MINFO_NFY_EVENT_HANDLE_CLOSED)
        {
            return FALSE;
        }
    }
    if (prMediaInfo->FmtInfo.rWmvInfo.hMediaIdxHandle != 0)
    {
        x_midxbuld_close(prMediaInfo->FmtInfo.rWmvInfo.hMediaIdxHandle);
        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (VOID*)&u4Msg);
        if (u4Msg != MIDXBULD_NFY_EVENT_HANDLE_CLOSED)
        {
            return FALSE;
        }
    }

    x_memset(&(prMediaInfo->FmtInfo.rWmvInfo), 0, sizeof(PLAYMGR_WMV_INFO_T));

    return TRUE;
}

EXTERN BOOL _PLAYMGR_WmvResume(VOID* pvMediaInfo)
{
    BOOL fgRet;
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;

    fgRet = _SWDMX_Play(prMediaInfo->u1SrcId);
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_WmvResume Fail to play\n");
        return FALSE;
    }
    return TRUE;
}

BOOL _PLAYMGR_WmvPause(VOID)
{
    return TRUE;
}

BOOL _PLAYMGR_WmvSetStreamIndex(PLAYMGR_INFO_T* prMediaInfo,UINT8 u1VideoStreamIdx, UINT8 u1AudioStreamIdx)
{
    PLAYMGR_WMV_INFO_T* prWmvInfo = &prMediaInfo->FmtInfo.rWmvInfo;
    prWmvInfo->u1AudioStreamIdx = u1AudioStreamIdx;
    prWmvInfo->u1VideoStreamIdx = u1VideoStreamIdx;
    return TRUE;
}

BOOL _PLAYMGR_OpenWmvMediaInfo(PLAYMGR_STATUS_T* prMediaStatus, VOID* pvMediaInfo)
{
    PLAYMGR_INFO_T* prMediaInfo = (PLAYMGR_INFO_T*)pvMediaInfo;
    PLAYMGR_WMV_INFO_T* prWmvInfo = (PLAYMGR_WMV_INFO_T*)&prMediaInfo->FmtInfo.rWmvInfo;

    UINT32 u4Msg;
    INT32 i4Ret;
    //UINT32 u4PacketSize =0;
    #ifndef __MODEL_slt__
    SIZE_T  zSize;
    #endif
    //for netflix
    PLAYMGR_SourceType_T ePlayMgrSrcType = PLAYMGR_SRC_TYPE_HIGH_SPEED_STORAGE;
    //for index builder
    #ifndef __MODEL_slt__
    MIDXBULD_KEY_INFO_T* prKeyInfo = &prWmvInfo->rWmvKeyInfo;
	#endif
    MIDXBULD_SOURCE_INFO_T* prSrcInfo = &prWmvInfo->rWmvSrcInfo;
	#ifndef __MODEL_slt__
    MIDXBULD_ELMT_TBL_INFO_T* prTableInfo = &prWmvInfo->rWmvTableInfo;

    MM_RANGE_ELMT_IDXTBL_T* prKeyElmtStrm = &prWmvInfo->rWmvKeyIDXTBL;
	#endif
    MEDIA_FORMAT_T* prformat = &(prMediaInfo->rMediaFmt);


    //UNUSED(prKeyElmtStrm);
    x_memset(&prMediaInfo->rRangeInfo[0],0,sizeof(MM_RANGE_INFO_T));

    ePlayMgrSrcType = _PLAYMGR_GetSrcType(prMediaInfo->u1SrcId);

    //use mutil to parsing ASF file
    if(ePlayMgrSrcType != PLAYMGR_SRC_TYPE_NETWORK_NETFLIX)
    {
        _u4IdxOfPraingBitstream = 0;
#ifndef __MODEL_slt__
        i4Ret = x_minfo_mopen((VOID *)&(prMediaInfo->rMediaFeeder), 0, _PLAYMGR_WmvMediaInfoNotify, (VOID*)prMediaInfo,
                              prformat, &(prWmvInfo->hWmvMediaInfoHandle));
        if (MINFOR_OK != i4Ret )
        {
            LOG(0,"playmgrc wmv open fail,i4Ret = %d\n",i4Ret);
            return FALSE;
        }

        //waiting for asf media info notify
        _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (VOID*)&u4Msg);

        if (u4Msg != MINFO_NFY_EVENT_PARSING_DONE)
        {
            LOG(0,"error notify message : %d\n",u4Msg);
            return FALSE;
        }
        prSrcInfo->ui8_data_offset = prWmvInfo->rWmvIdxInfo.ui8_data_offset;
        prSrcInfo->ui8_data_size = prWmvInfo->rWmvIdxInfo.ui8_data_sz;
        prSrcInfo->ui8_idx_offset = prWmvInfo->rWmvIdxInfo.ui8_idx_offset;
        prSrcInfo->ui8_idx_size = prWmvInfo->rWmvIdxInfo.ui8_idx_sz;
        prSrcInfo->ui4_idx_type = prWmvInfo->rWmvIdxInfo.ui4_idx_type;

        prSrcInfo->ui1_total_strm_num = prWmvInfo->rWmvGenericInfo[0].ui1_strm_ns;
        prSrcInfo->ui4_total_duration = (UINT32)prWmvInfo->rWmvGenericInfo[0].ui8_pb_duration;
        prSrcInfo->ui4_packet_sz = prWmvInfo->rWmvGenericInfo[0].ui4_sz_max_acces_unt;
        if(!SWDMX_SetInfo(prMediaInfo->u1SrcId, (UINT8)eSWDMX_TOTAL_DURATION,
        prSrcInfo->ui4_total_duration, 0, 0))
        {
            LOG(1, "SWDMX_SetInfo eSWDMX_TOTAL_DURATION fail\n");
        }
#else   // __MODEL_slt__
        prSrcInfo->ui8_data_offset = 1139;
        prSrcInfo->ui8_data_size = 6860050;
        prSrcInfo->ui8_idx_offset = 6861189;
        prSrcInfo->ui8_idx_size = 326;
        prSrcInfo->ui4_idx_type = 2;
        prSrcInfo->ui1_total_strm_num = 1;
        prSrcInfo->ui4_total_duration = 2789640;
        prSrcInfo->ui4_packet_sz = 10000;
        prMediaInfo->u2VidStmNum = 1;
        prMediaInfo->u2AudStmNum = 0;
#endif  // __MODEL_slt__
    }
    else //netflix mode
    {
        //netflix mode , parsing all asf/wmv headers
        UINT32 u4Idx;
        FeederSetInfo(FEEDER_PARSING_SOURCE,FEEDER_SWDMX_SRC_TYPE,SWDMX_SRC_TYPE_NETWORK_NETFLIX);
        _u4NumberOfBitstream = _PLAYMGR_GetNumberOfBitstream(prMediaInfo->u1SrcId);

        for(u4Idx = 0 ; u4Idx < _u4NumberOfBitstream; u4Idx++)
        {
            FEEDER_TRANSMIT_COND rFeederCond;
            _u4IdxOfPraingBitstream = u4Idx;
            i4Ret = FeederSetRequest(FEEDER_PARSING_SOURCE,u4Idx,(UINT64)(0),&rFeederCond,0);
            if(i4Ret != FEEDER_E_OK)
            {
                LOG(5, "_PLAYMGR_OpenWmvMediaInfo FeederSetRequest Fail in netflix mode.\n");
                return FALSE;
            }
            i4Ret = x_minfo_mopen((VOID*)(rFeederCond.u4BufAddr), rFeederCond.u4TransmitSize, _PLAYMGR_WmvMediaInfoNotify, (VOID*)prMediaInfo,
                      prformat, &(prWmvInfo->hWmvMediaInfoHandle));
            if (MINFOR_OK != i4Ret )
            {
                LOG(0,"playmgr wmv open fail,i4Ret = %d in netflix mode\n",i4Ret);
                return FALSE;
            }
            //waiting for asf media info notify
            _PLAYMGR_ReceiveNotify(prMediaInfo->u1SrcId, (VOID*)&u4Msg);
        }

        prMediaInfo->u2VidStmNum = 1;
        prMediaInfo->u2AudStmNum = 1;
        prWmvInfo->u2VidIdx =0;
        prWmvInfo->u2AudIdx =0;

    }

    if (_PLAYMGR_SetWmvInfo(prMediaStatus, prMediaInfo) != TRUE)
    {
        return FALSE;
    }

    if(ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_NETFLIX)
    {
    SWDMX_SetInfo(prMediaInfo->u1SrcId, eSWDMX_SET_SYSTEMINFO,(UINT32)prWmvInfo->rWmvGenericInfo,_u4NumberOfBitstream,0);
        //SWDMX_SetInfo(prMediaInfo->u1SrcId, eSWDMX_SET_NETFLIX_STREAM_ID,(UINT32)prMediaStatus->au4VideoId[0],(UINT32)prMediaStatus->au4AudioId[0],0);
        SWDMX_SetInfo(prMediaInfo->u1SrcId, eSWDMX_SET_NETFLIX_STREAM_ID,(UINT32)_u1NetflixVideoId,(UINT32)_u1NetflixAudioId,0);
    }


    switch(ePlayMgrSrcType)
    {
        case PLAYMGR_SRC_TYPE_NETWORK_NETFLIX:
            prMediaInfo->rRangeInfo[0].e_mm_src_type = MM_SRC_TYPE_NETWORK_NETFLIX;
        break;
        case PLAYMGR_SRC_TYPE_NETWORK_DLNA:
            prMediaInfo->rRangeInfo[0].e_mm_src_type = MM_SRC_TYPE_NETWORK_DLNA;
        break;
        case PLAYMGR_SRC_TYPE_HIGH_SPEED_STORAGE:
        default:
            prMediaInfo->rRangeInfo[0].e_mm_src_type = MM_SRC_TYPE_HIGH_SPEED_STORAGE;
        break;
    }

    prMediaStatus->u2TotalPrmNum = 1; //for TS only, other format only contains one program in file
#ifndef __MODEL_slt__
    prMediaStatus->u4TotalDuration = (UINT32)prWmvInfo->rWmvGenericInfo[0].ui8_pb_duration;
    prMediaInfo->rPlayerStatus.u4TotalDuration = (UINT32)prWmvInfo->rWmvGenericInfo[0].ui8_pb_duration;

    //pts offset
    prMediaInfo->rRangeInfo[0].t_pts_offset = prWmvInfo->rWmvGenericInfo[0].ui8_preroll;

    prMediaInfo->rRangeInfo[0].ui4_sz_max_acces_unt = prWmvInfo->rWmvGenericInfo[0].ui4_sz_max_acces_unt;
    prMediaInfo->rRangeInfo[0].ui4_sz_min_acces_unt = prWmvInfo->rWmvGenericInfo[0].ui4_sz_min_acces_unt;
    if(ePlayMgrSrcType != PLAYMGR_SRC_TYPE_NETWORK_NETFLIX)
    {
        prMediaInfo->rRangeInfo[0].z_range_sz = prWmvInfo->rWmvIdxInfo.ui8_data_sz;
    }
    else
    {
        prMediaInfo->rRangeInfo[0].z_range_sz = 0xFFFFFFFF;
    }
    prMediaInfo->rRangeInfo[0].ui8_fileoffset = prWmvInfo->rWmvIdxInfo.ui8_data_offset;
#ifdef SUPPORT_3D_MEDIA
	prMediaInfo->rRangeInfo[0].b_stereo_scopic=prWmvInfo->rWmvGenericInfo[0].b_stereo_scopic;
	prMediaInfo->rRangeInfo[0].e_scopic_type=prWmvInfo->rWmvGenericInfo[0].e_scopic_type;
	prMediaInfo->rRangeInfo[0].b_half_height=prWmvInfo->rWmvGenericInfo[0].b_half_height;
	prMediaInfo->rRangeInfo[0].b_half_width=prWmvInfo->rWmvGenericInfo[0].b_half_width;
	prMediaInfo->rRangeInfo[0].ui8_skip_dur=prWmvInfo->rWmvGenericInfo[0].ui8_skip_dur;
#endif
    //building index
    if (prSrcInfo->ui4_idx_type != MM_MINFO_IDX_TYPE_NONE && ePlayMgrSrcType != PLAYMGR_SRC_TYPE_NETWORK_NETFLIX)
    {
        //prformat.e_media_type = MEDIA_TYPE_CONTAINER;
        //prformat.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_ASF;

        i4Ret = x_midxbuld_mopen((VOID *)&prMediaInfo->rMediaFeeder, 0, _PLAYMGR_WmvMediaIdxNotify, (VOID*)prMediaInfo,
                                 prformat, prSrcInfo, prKeyInfo, &prWmvInfo->hMediaIdxHandle);

        if (i4Ret != MINFOR_OK)
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
            LOG(0,"key index range id: %d(0x%.8X)\r\n",
                (UINT32)prKeyInfo->ui4_range_id,
                (UINT32)prKeyInfo->ui4_range_id);
            LOG(0,"key index element table 00 id: %d(0x%.8X)\r\n",
                (UINT32)prKeyInfo->ui4_tbl_id,
                (UINT32)prKeyInfo->ui4_tbl_id);

            zSize = sizeof(MIDXBULD_ELMT_TBL_INFO_T);

            prTableInfo->ui4_tbl_id = prKeyInfo->ui4_tbl_id;

            i4Ret = x_midxbuld_get_info(prWmvInfo->hMediaIdxHandle,
                                        MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL,
                                        (VOID*)prTableInfo, &zSize);
            if (MIDXBULDR_OK != i4Ret)
            {
                LOG(0,"midxbuld_cli_asf_get_tbl ERROR!\r\n");
                return FALSE;
            }
            //_PLAYMGR_SetWmvIdxInfo(prTableInfo);
            //set key table to range info structure

            prKeyElmtStrm->e_elem_type = MM_ELEMT_TYPE_KEY;
            prKeyElmtStrm->ui4_number_of_entry = prTableInfo->t_elmt_tbl.ui4_number_of_entry;
            prKeyElmtStrm->ui8_base_offset = prTableInfo->t_elmt_tbl.ui8_base_offset;
            prKeyElmtStrm->t_strm_id.u.t_stm_asf_id = prKeyInfo->ui4_strm_id;

            prKeyElmtStrm->u.pt_pb_idx_tbl_entry = (MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T*)prTableInfo->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;

            // link key index to the end of the list
            if (prKeyElmtStrm->ui4_number_of_entry > 0)
            {
                prMediaInfo->rRangeInfo[0].pt_idxtbl_lst = prKeyElmtStrm;
            }

        }
    }
#else   // __MODEL_slt__
    prMediaStatus->u4TotalDuration = 2789640;
    prMediaInfo->rPlayerStatus.u4TotalDuration = 2789640;

    //pts offset
    prMediaInfo->rRangeInfo[0].t_pts_offset = 450000;
    
    prMediaInfo->rRangeInfo[0].ui4_sz_max_acces_unt = 10000;
    prMediaInfo->rRangeInfo[0].ui4_sz_min_acces_unt = 10000;
    prMediaInfo->rRangeInfo[0].z_range_sz = 6860050;
    prMediaInfo->rRangeInfo[0].ui8_fileoffset = 1139;
#endif  // __MODEL_slt__

    if (prMediaInfo->u2AudStmNum > 0 && prWmvInfo->u2AudIdx < 17 )
    {
        AUD_WMA_CFG_T rAudWmaCfg;
        // for push mode to play wma file
        if (prMediaInfo->rPlayerStatus.aeAudFormat[0] == PLAYMGR_ADEC_WMA)
        {
            rAudWmaCfg.u2HdrSrc = 0;
            _AUD_DspWMAHeader(AUD_DEC_MAIN,&rAudWmaCfg);
        }

    }
    return TRUE;
}


#endif // !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)



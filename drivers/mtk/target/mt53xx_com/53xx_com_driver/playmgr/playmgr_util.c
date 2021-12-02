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
 * $RCSfile: playmgr_util.c,v $
 *---------------------------------------------------------------------------*/

/** @file playmgr_util.c
 *  This file contains implementation of exported APIs of play manager's utility.
 */
#include "playmgr.h"
#include "feeder_if.h"
#include "playmgr_util.h"
#include "playmgr_if.h"
#include "playmgr_mpg.h"
#include "swdmx_drvif.h"
#include "stc_drvif.h"
#include "x_debug.h"

VOID _PLAYMGR_RecgFileByExt(PLAYMGR_INFO_T* prMediaInfo, CHAR *szFileName);

BOOL _PLAYMGR_InitMediaInfo(VOID)
{
#ifndef CC_ES_DISABLE
   
#ifndef CC_ES_VIDEO_DISABLE
    if (x_mfmtrecg_vid_es_init() != MFMTRECGR_OK)
    {
          return FALSE;
    }
#endif

#ifndef CC_ES_AUDIO_DISABLE
    if (x_minfo_aud_es_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_minfo_aif_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }
    
    if (x_mfmtrecg_aif_init() != MFMTRECGR_OK)
    {
          return FALSE;
    }
    
    if (x_mfmtrecg_aud_es_init() != MFMTRECGR_OK)
    {
          return FALSE;
    }
#endif



#ifndef CC_ES_MP3_DISABLE
    // Add for DRM
    //DIVX_DRM_INIT_CONFIG_T rDivxDrmCfg;
    // MP3 should not reconized before container just as the same reason
    // of video only file.
    if (x_mfmtrecg_mp3_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_minfo_mp3_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_midxbuld_mp3_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }
#endif
#endif

#ifndef CC_PS_DISABLE
    // PS format recognizer also recognize VIDEO ES file
    // we should use that function only after other format can't be recognized.
    if (x_mfmtrecg_ps_init() != MFMTRECGR_OK)
    {
          return FALSE;
    }
    
    if (x_minfo_ps_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }
#endif

#ifndef CC_TS_DISABLE
    if (x_mfmtrecg_ts_init() != MFMTRECGR_OK)
    {
        return FALSE;
    }
#endif


#ifndef CC_MP4_DISABLE
    if (x_mfmtrecg_mp4_init() != MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_minfo_mp4_init() != MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_midxbuld_mp4_init() != MFMTRECGR_OK)
    {
        return FALSE;
    }
#endif

#ifndef CC_ASF_DISABLE
    if (x_mfmtrecg_asf_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_minfo_asf_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_midxbuld_asf_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }
#endif

#ifndef CC_AVI_DISABLE

    if (x_mfmtrecg_avi_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_minfo_avi_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_midxbuld_avi_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }
#endif

#ifndef CC_MKV_DISABLE
    if (x_minfo_mkv_init() != MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_mfmtrecg_mkv_init() != MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_midxbuld_mkv_init() != MFMTRECGR_OK)
    {
        return FALSE;
    }
#endif

#ifndef CC_RM_DISABLE
    if (x_minfo_rm_init() != MINFOR_OK)
    {
        return FALSE;
    }

    if (x_mfmtrecg_rm_init() != MFMTRECGR_OK)
    {
        return FALSE;
    }

    if (x_midxbuld_rm_init() != MIDXBULDR_OK)
    {
        return FALSE;
    }
#endif

    /*rDivxDrmCfg.ui2_divx_drm_memory_offset = DIVX_DRM_MEMORY_OFFSET;
    rDivxDrmCfg.ui2_divx_drm_memory_size_byte = DIVX_DRM_MEMORY_SIZE_BYTE;
    rDivxDrmCfg.ui1_divx_drm_model_id_low_byte = DIVX_DRM_MODEL_ID_LOW_BYTE;
    rDivxDrmCfg.ui1_divx_drm_model_id_high_byte = DIVX_DRM_MODEL_ID_HIGH_BYTE;

    if (DRMR_OK != x_divx_drm_init(&rDivxDrmCfg))
    {
        return FALSE;
    }*/

    return TRUE;
}

// Assign the e_media_type, then MW utility will base on this format
// to start recogazine file
VOID _PLAYMGR_RecgFileByExt(PLAYMGR_INFO_T* prMediaInfo, CHAR *szFileName)
{
    CHAR* szExt;
    if((szFileName == NULL) || (prMediaInfo == NULL))
    {
        return;
    }

    szExt = x_strrchr(szFileName, '.');
    if ((!szExt) || (x_strlen((const CHAR*)szExt) < 2))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_UNKNOWN;
        return;
    }
    szExt += 1;

    if((x_strcmp(szExt, "mp3") == 0) ||
       (x_strcmp(szExt, "MP3") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_AUDIO_ES;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype =
            MEDIA_AUD_SUBTYPE_MP3;
    }
    else if((x_strcmp(szExt, "mpeg") == 0) ||
       	(x_strcmp(szExt, "MPEG") == 0) ||
       	(x_strcmp(szExt, "mpg") == 0) ||
       	(x_strcmp(szExt, "MPG") == 0) ||
       	(x_strcmp(szExt, "vob") == 0) ||
       	(x_strcmp(szExt, "VOB") == 0) ||
       	(x_strcmp(szExt, "dat") == 0) ||
       	(x_strcmp(szExt, "DAT") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_PS;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_ps_subtype =
            MEDIA_PS_SUBTYPE_MPG;
    }
    else if((x_strcmp(szExt, "trp") == 0) ||
       	(x_strcmp(szExt, "TRP") == 0) ||
       	(x_strcmp(szExt, "ts") == 0) ||
       	(x_strcmp(szExt, "TS") == 0) ||
       	(x_strcmp(szExt, "tp") == 0) ||
       	(x_strcmp(szExt, "TP") == 0) ||
       	(x_strcmp(szExt, "m2t") == 0) ||
       	(x_strcmp(szExt, "M2T") == 0) ||
       	(x_strcmp(szExt, "m2ts") == 0) ||
       	(x_strcmp(szExt, "M2TS") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_TS;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_ts_subtype =
            MEDIA_TS_SUBTYPE_TS_188;
    }
    else if((x_strcmp(szExt, "pvr") == 0) ||
       	(x_strcmp(szExt, "PVR") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_TS;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_ts_subtype =
            MEDIA_TS_SUBTYPE_TS_TIME_SHIFT;
    }
    else if((x_strcmp(szExt, "m2v") == 0) ||
       	(x_strcmp(szExt, "M2V") == 0) ||
       	(x_strcmp(szExt, "mpv") == 0) ||
       	(x_strcmp(szExt, "MPV") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_VIDEO_ES;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_vid_subtype =
            MEDIA_VID_SUBTYPE_M2V;
    }
    else if((x_strcmp(szExt, "m2a") == 0) ||
       	(x_strcmp(szExt, "M2A") == 0) ||
       	(x_strcmp(szExt, "mpa") == 0) ||
       	(x_strcmp(szExt, "MPA") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_AUDIO_ES;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype =
            MEDIA_AUD_SUBTYPE_MP2;
    }
    else if((x_strcmp(szExt, "h264") == 0) ||
       	(x_strcmp(szExt, "H264") == 0) ||
       	(x_strcmp(szExt, "264") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_VIDEO_ES;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_vid_subtype =
            MEDIA_VID_SUBTYPE_H264;
    }
    else if((x_strcmp(szExt, "m4v") == 0) ||
       	(x_strcmp(szExt, "M4V") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_VIDEO_ES;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_vid_subtype =
            MEDIA_VID_SUBTYPE_M4V;
    }
    else if((x_strcmp(szExt, "vc1") == 0) ||
       	(x_strcmp(szExt, "VC1") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_VIDEO_ES;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_vid_subtype =
            MEDIA_VID_SUBTYPE_VC1;
    }
    else if((x_strcmp(szExt, "avs") == 0) ||
       	(x_strcmp(szExt, "AVS") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_VIDEO_ES;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_vid_subtype =
            MEDIA_VID_SUBTYPE_AVS;
    }
    else if((x_strcmp(szExt, "avi") == 0) ||
       	(x_strcmp(szExt, "AVI") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_contnr_subtype =
            MEDIA_CONTNR_SUBTYPE_AVI;
    }
    else if((x_strcmp(szExt, "mp4") == 0) ||
       	(x_strcmp(szExt, "MP4") == 0) ||
       	(x_strcmp(szExt, "mpg4") == 0) ||
       	(x_strcmp(szExt, "MPG4") == 0) ||
       	(x_strcmp(szExt, "mpeg4") == 0) ||
       	(x_strcmp(szExt, "MPEG4") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_contnr_subtype =
            MEDIA_CONTNR_SUBTYPE_MP4;
    }
    else if((x_strcmp(szExt, "wmv") == 0) ||
       	(x_strcmp(szExt, "WMV") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_contnr_subtype =
            MEDIA_CONTNR_SUBTYPE_ASF;
    }
    else if((x_strcmp(szExt, "wma") == 0) ||
       	(x_strcmp(szExt, "WMA") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_contnr_subtype =
            MEDIA_CONTNR_SUBTYPE_ASF;
    }
    else if((x_strcmp(szExt, "mkv") == 0) ||
       	(x_strcmp(szExt, "MKV") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_contnr_subtype =
            MEDIA_CONTNR_SUBTYPE_MATROSKA;
    }
    else if((x_strcmp(szExt, "rm") == 0) ||
       	(x_strcmp(szExt, "RM") == 0) ||
       	(x_strcmp(szExt, "rmvb") == 0) ||
       	(x_strcmp(szExt, "RMVB") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_contnr_subtype =
            MEDIA_CONTNR_SUBTYPE_RM;
    }
    else if((x_strcmp(szExt, "flv") == 0) ||
       	(x_strcmp(szExt, "FLV") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_contnr_subtype =
            MEDIA_CONTNR_SUBTYPE_FLV;
    }
    else if((x_strcmp(szExt, "wav") == 0) ||
       	(x_strcmp(szExt, "WAV") == 0) ||
       	(x_strcmp(szExt, "wave") == 0) ||
       	(x_strcmp(szExt, "WAVE") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_AUDIO_ES;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype =
            MEDIA_AUD_SUBTYPE_WAVE;
    }
    else if((x_strcmp(szExt, "ac3") == 0) ||
       	(x_strcmp(szExt, "AC3") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_AUDIO_ES;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype =
            MEDIA_AUD_SUBTYPE_AC3;
    }
    else if((x_strcmp(szExt, "aac") == 0) ||
       	(x_strcmp(szExt, "AAC") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_AUDIO_ES;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype =
            MEDIA_AUD_SUBTYPE_AAC;
    }
    else if((x_strcmp(szExt, "aif") == 0) ||
       	(x_strcmp(szExt, "AIF") == 0) ||
       	(x_strcmp(szExt, "aiff") == 0) ||
       	(x_strcmp(szExt, "AIFF") == 0))
    {
        prMediaInfo->rMediaFmt.e_media_type = MEDIA_TYPE_AUDIO_ES;
        prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype =
            MEDIA_AUD_SUBTYPE_AIF;
    }
    UNUSED(szFileName);
    UNUSED(prMediaInfo);
    return;
}

BOOL _PLAYMGR_RecgFile(PLAYMGR_INFO_T* prMediaInfo, CHAR *szFileName)
{
    INT32 i4Ret;
    MEDIA_FORMAT_T* prMediaFmt;
	PLAYMGR_SourceType_T ePlayMgrSrcType;

    if((szFileName == NULL) || (prMediaInfo == NULL))
    {
        return FALSE;
    }

    prMediaFmt = &prMediaInfo->rMediaFmt;
    ePlayMgrSrcType = _PLAYMGR_GetSrcType();

    _PLAYMGR_RecgFileByExt(prMediaInfo, szFileName);

#ifdef TIME_SHIFT_SUPPORT
    if((prMediaInfo->rMediaFmt.e_media_type == MEDIA_TYPE_TS) &&
       (prMediaInfo->rMediaFmt.t_media_subtype.u.e_ts_subtype ==
        MEDIA_TS_SUBTYPE_TS_TIME_SHIFT))
    {
        prMediaInfo->rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT;

        return TRUE;
    }
    else
#endif
    if(ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_RHAPSODY)
    {
        // base on .aac or .mp3 to decide the format.
        if((prMediaInfo->rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
           (prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype ==
            MEDIA_AUD_SUBTYPE_AAC))
        {
            prMediaInfo->rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_AAC;
        }
        else if((prMediaInfo->rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
           (prMediaInfo->rMediaFmt.t_media_subtype.u.e_aud_subtype ==
            MEDIA_AUD_SUBTYPE_MP3))
        {
            prMediaInfo->rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MP3;
        }
        return TRUE;
    }

    if ((prMediaInfo->rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
        (prMediaInfo->rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV))
    {
        return TRUE;
    }
    // Get Format type
    i4Ret = x_mfmtrecg_get_mem_fmt((VOID *)&prMediaInfo->rMediaFeeder, 0, prMediaFmt);


#if 0 // disable mpeg recoginzer
    if(i4Ret != MFMTRECGR_OK)
    {
#if !defined(CC_TS_DISABLE) || !defined(CC_ES_DISABLE) || !defined(CC_PS_DISABLE)
        if(_PLAYMGR_IsMPG(prMediaInfo, szFileName))
#endif
        {
            return TRUE;
        }
        return FALSE;
    }
#endif
    UNUSED(i4Ret);
    return TRUE;
}


BOOL _PLAYMGR_SetSTC(UINT32 u4StartPTS)
{
    UINT32 u4Temp;

    STC_StopStc();
    u4Temp = STC_GetStcValue();
    STC_SetStcValue(u4StartPTS);
    u4Temp = STC_GetStcValue();

    if (u4Temp != u4StartPTS)
    {
        return FALSE;
    }

    STC_StartStc();

    return TRUE;
}


#if 1  // megaa SPU

typedef enum
{
    SPU_NONE = 0,
    SPU_WAIT_DEC,
    SPU_DEC_OK,
    SPU_WAIT_DRAW_ACK,
    SPU_DRAWN
} SPU_STATE_T;

typedef struct
{
    SPU_STATE_T eState;
    BOOL fgOutBufEmpty;
    UCHAR *pucSpuBuf;
    UINT32 u4FrameAddr[5];
    UINT32 u4CopyLen[5];
    UINT8 u1SerialNumber[5];
    UINT32 u4StartPts[5];
    UINT32 u4EndPts[5];
    UINT32 u4SpuRp;
    UINT32 u4SpuWp;
    BOOL fgXsubPlus;
    UINT32 u4WindowWidth;
    UINT32 u4WindowHeight;
} SPU_STATUS_T;

SPU_STATUS_T _rSpuStatus;

extern UCHAR *_pucSubOutBuf;


VOID _PLAYMGR_InitSubtitle(void)
{
  extern PLAYMGR_INFO_T _rPlayerInfo;
  PLAYMGR_AVI_INFO_T* prAviInfo = &_rPlayerInfo.FmtInfo.rAviInfo;
  INT32 i4Height = prAviInfo->rStmAttribute[0].u.t_avi_video_stm_attr.i4_height;
  PLAYMGR_STATUS_T rPlaymgrStatus;

  _rSpuStatus.eState = SPU_NONE;
  _rSpuStatus.fgOutBufEmpty = TRUE;
  _rSpuStatus.u4SpuRp = 0;
  _rSpuStatus.u4SpuWp = 0;

  if (i4Height <= 576)
  {
    _rSpuStatus.u4WindowWidth = 640;
    _rSpuStatus.u4WindowHeight = 480;
  }
  else if (i4Height <= 720)
  {
    _rSpuStatus.u4WindowWidth = 960;
    _rSpuStatus.u4WindowHeight = 720;
  }
  else
  {
    _rSpuStatus.u4WindowWidth = 1440;
    _rSpuStatus.u4WindowHeight = 1080;
  }

  if (PLAYMGR_GetInfo(&rPlaymgrStatus))
  {
    if (rPlaymgrStatus.aeCapFormat[0] == PLAYMGR_CAPTION_AVI_BITMAP)
      _rSpuStatus.fgXsubPlus = FALSE;
    else if (rPlaymgrStatus.aeCapFormat[0] == PLAYMGR_CAPTION_AVI_DXSA)
      _rSpuStatus.fgXsubPlus = TRUE;
    else
      Printf("Unsupported subtitle format!!!\n");
  }
}


static UINT32 COUNT0(UCHAR *p, UINT32 u4BytePos)
{
  UINT32 u4Run = 0;
  UCHAR u1P;
  UINT32 i;

  if (u4BytePos == 0)
  {
    u1P = *p;
    for (i = 0; i < 8; i++)
    {
      if ((u1P & 0x80) == 0)
        u4Run++;
      else
        break;
      u1P <<= 1;
    }

    if (u4Run < 8)
      return u4Run;

    u1P = *(p + 1);
    for (i = 0; i < 8; i++)
    {
      if ((u1P & 0x80) == 0)
        u4Run++;
      else
        break;
      u1P <<= 1;
    }

    return u4Run;
  }
  else if (u4BytePos == 4)
  {
    u1P = *p;
    for (i = 0; i < 4; i++)
    {
      if ((u1P & 0x08) == 0)
        u4Run++;
      else
        break;
      u1P <<= 1;
    }

    if (u4Run < 4)
      return u4Run;

    u1P = *(p + 1);
    for (i = 0; i < 8; i++)
    {
      if ((u1P & 0x80) == 0)
        u4Run++;
      else
        break;
      u1P <<= 1;
    }

    if (u4Run < 12)
      return u4Run;

    u1P = *(p + 2);
    for (i = 0; i < 4; i++)
    {
      if ((u1P & 0x80) == 0)
        u4Run++;
      else
        break;
      u1P <<= 1;
    }

    return u4Run;
  }
  return 0;
}

static void vSpuSWDec(UCHAR *pbSrc, UCHAR *pbDst, UINT32 u4Width, UINT32 u4Height, UINT32 u4Pitch, UINT32 u4ColorKey/*UINT32 u4Bpp*/)
{
  UINT32 u4SrcBytePos;
  //UINT32 u4DstBytePos;  2, 4bpp output only
  UINT32 u4Line;
  UINT32 u4Pixel;
  UINT32 u4Cnt0;
  UINT8  u1Run;
  UINT8  u1Pxl;
  UCHAR *pbDstBak = pbDst;

  // because it is interlaced!!!
  u4Height >>= 1;
  u4Pitch <<= 1;

#if 0
  if (RISCRead(0x04044) & 0x1)  // color key switch
    u4ColorKey = RISCRead(0x04048);
  else
    u4ColorKey = 0xFFFFFFFF;
#endif

  u4SrcBytePos = 0;

  switch (8/*u4Bpp*/)
  {
  case 8:
    for (u4Line = 0; u4Line < u4Height; u4Line++)
    {
      pbDst = pbDstBak;
      pbDst += u4Line * u4Pitch;

      // run-length decoding a line
      for (u4Pixel = 0; u4Pixel < u4Width; )
      {
        u4Cnt0 = COUNT0(pbSrc, u4SrcBytePos);

        if (u4Cnt0 <= 1)  // 1 to 3
        {
          u1Run = (UINT8)((*pbSrc >> (6 - u4SrcBytePos)) & 0x03);
          u1Pxl = (UINT8)((*pbSrc >> (4 - u4SrcBytePos)) & 0x03);
          u4Pixel += u1Run;
          if (u4ColorKey != 0xFFFFFFFFU && (UINT8)u4ColorKey == u1Pxl)
          {
            pbDst += u1Run;
          }
          else
          {
            for (; u1Run > 0; u1Run--)
              *pbDst++ = (UCHAR)(u1Pxl + 1);
          }
          if (u4SrcBytePos == 4)
            pbSrc++;
          u4SrcBytePos = (u4SrcBytePos + 4) % 8;
        }
        else if (u4Cnt0 <= 3)  // 4 to 15
        {
          u1Run = (UINT8)(((*pbSrc << 8 | *(pbSrc+1)) >> (10 - u4SrcBytePos)) & 0x0F);
          u1Pxl = (UINT8)(((*pbSrc << 8 | *(pbSrc+1)) >> (8 - u4SrcBytePos)) & 0x03);
          u4Pixel += u1Run;
          if (u4ColorKey != 0xFFFFFFFFU && (UINT8)u4ColorKey == u1Pxl)
          {
            pbDst += u1Run;
          }
          else
          {
            for (; u1Run > 0; u1Run--)
              *pbDst++ = (UCHAR)(u1Pxl + 1);
          }
          pbSrc++;
        }
        else if (u4Cnt0 <= 5)  // 16 to 63
        {
          u1Run = (UINT8)(((*pbSrc << 8 | *(pbSrc+1)) >> (6 - u4SrcBytePos)) & 0x3F);
          u1Pxl = (UINT8)(((*pbSrc << 8 | *(pbSrc+1)) >> (4 - u4SrcBytePos)) & 0x03);
          u4Pixel += u1Run;
          if (u4ColorKey != 0xFFFFFFFFU && (UINT8)u4ColorKey == u1Pxl)
          {
            pbDst += u1Run;
          }
          else
          {
            for (; u1Run > 0; u1Run--)
              *pbDst++ = (UCHAR)(u1Pxl + 1);
          }
          pbSrc++;
          if (u4SrcBytePos == 4)
            pbSrc++;
          u4SrcBytePos = (u4SrcBytePos + 12) % 8;
        }
        else if (u4Cnt0 <= 7)  // 64 to 255
        {
          u1Run = (UINT8)(((*pbSrc << 16 | *(pbSrc+1) << 8 | *(pbSrc+2)) >> (10 - u4SrcBytePos)) & 0xFF);
          u1Pxl = (UINT8)(((*pbSrc << 16 | *(pbSrc+1) << 8 | *(pbSrc+2)) >> (8 - u4SrcBytePos)) & 0x03);
          u4Pixel += u1Run;
          if (u4ColorKey != 0xFFFFFFFFU && (UINT8)u4ColorKey == u1Pxl)
          {
            pbDst += u1Run;
          }
          else
          {
            for (; u1Run > 0; u1Run--)
              *pbDst++ = (UCHAR)(u1Pxl + 1);
          }
          pbSrc += 2;
        }
        else  // goes to the end of line
        {
          u1Pxl = (UINT8)(((*pbSrc << 16 | *(pbSrc+1) << 8 | *(pbSrc+2)) >> (8 - u4SrcBytePos)) & 0x03);
          while (u4Pixel < u4Width)
          {
            if (u4ColorKey != 0xFFFFFFFFU && (UINT8)u4ColorKey == u1Pxl)
              pbDst++;
            else
              *pbDst++ = (UCHAR)(u1Pxl + 1);
            u4Pixel++;
          }
          pbSrc += 2;
        }
      }

      if (u4SrcBytePos != 0)
      {
        pbSrc++;
        u4SrcBytePos  = 0;
      }
    }
    break;
  }
}

#define GET_ASCII_NUM(a)      (pucSrc[a] - '0')
#define LOAD_RGB(a)           (pucSrc[a] * 65536 + pucSrc[a+1] * 256 + pucSrc[a+2])
#define SPU_LOADL_WORD(a)     (pucSrc[a+1] * 256 + pucSrc[a])
#define SPU_LOAD_BYTE(a)      (pucSrc[a])

VOID vSpuDec(BOOL fgRealDec, UCHAR *pucSrc, UCHAR *pucDst, PLAYMGR_SUBTITLE_INFO_T *prSubtitleInfo)
{
  UINT32 u4Width;
  UINT32 u4Height;
  UINT32 u4Left;
  UINT32 u4Top;
  UINT32 u4BotOfst;
  UINT32 u4HH, u4MM, u4SS, u4MS;
  UINT32 u4Col0, u4Col1, u4Col2, u4Col3;
  UINT8  u1Alpha0, u1Alpha1, u1Alpha2, u1Alpha3;

  prSubtitleInfo->u4WindowWidth = _rSpuStatus.u4WindowWidth;
  prSubtitleInfo->u4WindowHeight = _rSpuStatus.u4WindowHeight;

  u4HH = (UINT32)(GET_ASCII_NUM(1) * 10 + GET_ASCII_NUM(2));
  u4MM = (UINT32)(GET_ASCII_NUM(4) * 10 + GET_ASCII_NUM(5));
  u4SS = (UINT32)(GET_ASCII_NUM(7) * 10 + GET_ASCII_NUM(8));
  u4MS = (UINT32)(GET_ASCII_NUM(10) * 100 + GET_ASCII_NUM(11) * 10 + GET_ASCII_NUM(12));
  prSubtitleInfo->u4StartPts = u4HH * 3600 + u4MM * 60 + u4SS;
  prSubtitleInfo->u4StartPts *= 1000;
  prSubtitleInfo->u4StartPts += u4MS;
  if (prSubtitleInfo->u4StartPts >= 500)
    prSubtitleInfo->u4StartPts -= 500;

  u4HH = (UINT32)(GET_ASCII_NUM(14) * 10 + GET_ASCII_NUM(15));
  u4MM = (UINT32)(GET_ASCII_NUM(17) * 10 + GET_ASCII_NUM(18));
  u4SS = (UINT32)(GET_ASCII_NUM(20) * 10 + GET_ASCII_NUM(21));
  u4MS = (UINT32)(GET_ASCII_NUM(23) * 100 + GET_ASCII_NUM(24) * 10 + GET_ASCII_NUM(25));
  prSubtitleInfo->u4EndPts = u4HH * 3600 + u4MM * 60 + u4SS;
  prSubtitleInfo->u4EndPts *= 1000;
  prSubtitleInfo->u4EndPts += u4MS;
  if (prSubtitleInfo->u4EndPts >= 500)
    prSubtitleInfo->u4EndPts -= 500;

  u4Width = (UINT32)SPU_LOADL_WORD(27);
  u4Height = (UINT32)SPU_LOADL_WORD(29);
  u4Left = (UINT32)SPU_LOADL_WORD(31);
  u4Top = (UINT32)SPU_LOADL_WORD(33);
  u4BotOfst = (UINT32)SPU_LOADL_WORD(39);
  u4Col0 = (UINT32)LOAD_RGB(41);
  u4Col1 = (UINT32)LOAD_RGB(44);
  u4Col2 = (UINT32)LOAD_RGB(47);
  u4Col3 = (UINT32)LOAD_RGB(50);
  if (_rSpuStatus.fgXsubPlus)
  {
    u1Alpha0 = SPU_LOAD_BYTE(53);
    u1Alpha1 = SPU_LOAD_BYTE(54);
    u1Alpha2 = SPU_LOAD_BYTE(55);
    u1Alpha3 = SPU_LOAD_BYTE(56);
  }
  else
  {
    u1Alpha0 = 0;
    u1Alpha1 = 0;
    u1Alpha2 = 0;
    u1Alpha3 = 0;
  }

  prSubtitleInfo->u4Width = u4Width;
  prSubtitleInfo->u4Height = u4Height;
  prSubtitleInfo->u4XPos = u4Left;
  prSubtitleInfo->u4YPos = u4Top;
  if (_rSpuStatus.fgXsubPlus)
  {
    prSubtitleInfo->au4ARGB[0] = u4Col0 | (u1Alpha0 << 24);
    prSubtitleInfo->au4ARGB[1] = u4Col1 | (u1Alpha1 << 24);
    prSubtitleInfo->au4ARGB[2] = u4Col2 | (u1Alpha2 << 24);
    prSubtitleInfo->au4ARGB[3] = u4Col3 | (u1Alpha3 << 24);
  }
  else
  {
    //*au4Color++ = u4Col0 | 0x00000000;
    prSubtitleInfo->au4ARGB[0] = 0x00000000;  // force it be black
    prSubtitleInfo->au4ARGB[1] = u4Col1 | 0xFF000000;
    prSubtitleInfo->au4ARGB[2] = u4Col2 | 0xFF000000;
    prSubtitleInfo->au4ARGB[3] = u4Col3 | 0xFF000000;
  }

  if (fgRealDec)
  {
    x_memset(pucDst, 0, u4Width*u4Height);  // clear output buffer

    vSpuSWDec(pucSrc + (_rSpuStatus.fgXsubPlus ? 57 : 53), pucDst, u4Width, u4Height, u4Width, 0xFFFFFFFF);
    vSpuSWDec(pucSrc + (_rSpuStatus.fgXsubPlus ? 57 : 53) + u4BotOfst, pucDst + u4Width, u4Width, u4Height, u4Width, 0xFFFFFFFF);
  }

  //Printf("d.save.binary d:/out.bin 0x%08X--0x%08X\n", (UINT32)pucDst, (UINT32)pucDst + u4Width * u4Height - 1);
  //Printf("SPU decoding done!\n");
}

#endif  // megaa SPU

PLAYMGR_SUBTITLE_INFO_T _rTmpSubtInfo;

BOOL _PLAYMGR_DrawSubtitle(PLAYMGR_STATUS_CMD_T *prStatusCmd,
    PLAYMGR_INFO_T* prMediaInfo)
{
    UINT32 u4Param;
    PLAYMGR_SUBTITLE_INFO_T *prSubtitleInfo;
    SWDMX_READ_INFO_T rSwdmxReadInfo = {0};
    BOOL fgRealDec;

    if((prMediaInfo == NULL) || (prStatusCmd == NULL))
    {
        return FALSE;
    }

    if (prMediaInfo->rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AVI)
    {
        rSwdmxReadInfo.u4FrameAddr = prStatusCmd->u4Param1;
        rSwdmxReadInfo.pucDest = (UCHAR*)prStatusCmd->u4Param2;
        rSwdmxReadInfo.u4CopyLen = prStatusCmd->u4Param3;
        rSwdmxReadInfo.u1SerialNumber = (UINT8)prStatusCmd->u4Param4;

        // Get subtitle buffer for decoding.
        SWDMX_ReadBuffer(eSWDMX_STRM_TYPE_SUBTITLE, &rSwdmxReadInfo);

        if (_rSpuStatus.eState == SPU_NONE)
        {
          prSubtitleInfo = (PLAYMGR_SUBTITLE_INFO_T *)_pucSubOutBuf;
          fgRealDec = TRUE;
        }
        else
        {
          prSubtitleInfo = &_rTmpSubtInfo;
          fgRealDec = FALSE;
        }

        // decode subtitle
        vSpuDec(fgRealDec,
                prMediaInfo->pucSubBuf,
                _pucSubOutBuf + 64,   // first 64 bytes are reserved for SPU parameter structure
                prSubtitleInfo);

        if (fgRealDec && prSubtitleInfo->u4StartPts <= PLAYMGR_GetPos())
        {
          Printf("SPU W x H = %d x %d, X : Y = %d : %d, S.PTS = %d, E.PTS = %d drawn at once\n", prSubtitleInfo->u4Width, prSubtitleInfo->u4Height, prSubtitleInfo->u4XPos, prSubtitleInfo->u4YPos, prSubtitleInfo->u4StartPts, prSubtitleInfo->u4EndPts);
          u4Param = ((prSubtitleInfo->u4Width<<16) + prSubtitleInfo->u4Height);
          // draw subtitle
          //spu_draw(_pucSubOutBuf, u4Width, u4Height, u4StartPts, u4EndPts);
          //call back function
          //Printf("notify info : %d\n",u4Param);
          FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DRAWSUBTITLE, u4Param);
          //_rSpuStatus.eState = SPU_WAIT_DRAW_ACK;  // ToDo: ACK mechanism to water proof
          _rSpuStatus.eState = SPU_NONE;  // this assumes the user space drawing code is fast enough
          _rSpuStatus.fgOutBufEmpty = TRUE;

          // Unlock subtitle buffer
          SWDMX_ReleaseBuffer(eSWDMX_STRM_TYPE_SUBTITLE, &rSwdmxReadInfo);
        }
        else
        {
          Printf("SPU W x H = %d x %d, X : Y = %d : %d, S.PTS = %d, E.PTS = %d pending\n", prSubtitleInfo->u4Width, prSubtitleInfo->u4Height, prSubtitleInfo->u4XPos, prSubtitleInfo->u4YPos, prSubtitleInfo->u4StartPts, prSubtitleInfo->u4EndPts);
          if (fgRealDec)
          {
            _rSpuStatus.eState = SPU_DEC_OK;
            _rSpuStatus.fgOutBufEmpty = FALSE;
          }
          else
          {
            _rSpuStatus.eState = SPU_WAIT_DEC;
          }
          _rSpuStatus.pucSpuBuf = rSwdmxReadInfo.pucDest;
          _rSpuStatus.u4FrameAddr[_rSpuStatus.u4SpuWp] = rSwdmxReadInfo.u4FrameAddr;
          _rSpuStatus.u4CopyLen[_rSpuStatus.u4SpuWp] = rSwdmxReadInfo.u4CopyLen;
          _rSpuStatus.u1SerialNumber[_rSpuStatus.u4SpuWp] = rSwdmxReadInfo.u1SerialNumber;
          _rSpuStatus.u4StartPts[_rSpuStatus.u4SpuWp] = prSubtitleInfo->u4StartPts;
          _rSpuStatus.u4EndPts[_rSpuStatus.u4SpuWp++] = prSubtitleInfo->u4EndPts;
          _rSpuStatus.u4SpuWp %= 5;
        }

        return TRUE;
    }

    UNUSED(prMediaInfo);
    UNUSED(prStatusCmd);
    return FALSE;
}


BOOL _PLAYMGR_ChkSubtitle(BOOL fgDrawAck)
{
    //BOOL fgRet;
    extern PLAYMGR_INFO_T _rPlayerInfo;
    UINT32 u4Param;
    PLAYMGR_SUBTITLE_INFO_T *prSubtitleInfo = (PLAYMGR_SUBTITLE_INFO_T *)_pucSubOutBuf;
    SWDMX_READ_INFO_T rSwdmxReadInfo = {0};
    BOOL fgRealDec = FALSE;

    if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AVI)
    {
        if (_rSpuStatus.eState == SPU_WAIT_DRAW_ACK && fgDrawAck)
        {
          _rSpuStatus.fgOutBufEmpty = TRUE;

          // Unlock subtitle buffer
          rSwdmxReadInfo.u4FrameAddr = _rSpuStatus.u4FrameAddr[_rSpuStatus.u4SpuRp];
          rSwdmxReadInfo.u4CopyLen = _rSpuStatus.u4CopyLen[_rSpuStatus.u4SpuRp];
          rSwdmxReadInfo.u1SerialNumber = (UINT8)_rSpuStatus.u1SerialNumber[_rSpuStatus.u4SpuRp++];
          _rSpuStatus.u4SpuRp %= 5;
          SWDMX_ReleaseBuffer(eSWDMX_STRM_TYPE_SUBTITLE, &rSwdmxReadInfo);

          if (_rSpuStatus.u4SpuRp == _rSpuStatus.u4SpuWp)
            _rSpuStatus.eState = SPU_NONE;
          else
            _rSpuStatus.eState = SPU_WAIT_DEC;

          return FALSE;
        }

        if (fgDrawAck)
        {
          Printf("!!!!! fatal error !!!!!\n");
        }

        if (_rSpuStatus.eState != SPU_DEC_OK && _rSpuStatus.eState != SPU_WAIT_DEC)
          return FALSE;

        if (_rSpuStatus.u4StartPts[_rSpuStatus.u4SpuRp] > PLAYMGR_GetPos())
          return FALSE;

        if (_rSpuStatus.eState == SPU_DEC_OK)
        {
          fgRealDec = FALSE;
        }
        if (_rSpuStatus.eState == SPU_WAIT_DEC && _rSpuStatus.fgOutBufEmpty)
        {
          fgRealDec = TRUE;
          _rSpuStatus.eState = SPU_DEC_OK;
          _rSpuStatus.fgOutBufEmpty = FALSE;
        }

        // Get subtitle buffer for decoding.
        rSwdmxReadInfo.u4FrameAddr = _rSpuStatus.u4FrameAddr[_rSpuStatus.u4SpuRp];
        rSwdmxReadInfo.pucDest = _rSpuStatus.pucSpuBuf;
        rSwdmxReadInfo.u4CopyLen = _rSpuStatus.u4CopyLen[_rSpuStatus.u4SpuRp];
        rSwdmxReadInfo.u1SerialNumber = _rSpuStatus.u1SerialNumber[_rSpuStatus.u4SpuRp];
        SWDMX_ReadBuffer(eSWDMX_STRM_TYPE_SUBTITLE, &rSwdmxReadInfo);

        // decode subtitle
        vSpuDec(fgRealDec,
                _rSpuStatus.pucSpuBuf,
                _pucSubOutBuf + 64,   // first 64 bytes are reserved for SPU parameter structure
                prSubtitleInfo);

        Printf("SPU W x H = %d x %d, X : Y = %d : %d, S.PTS = %d, E.PTS = %d  ==> drawn!!!\n", prSubtitleInfo->u4Width, prSubtitleInfo->u4Height, prSubtitleInfo->u4XPos, prSubtitleInfo->u4YPos, prSubtitleInfo->u4StartPts, prSubtitleInfo->u4EndPts);

        if (_rSpuStatus.fgOutBufEmpty)
          Printf("!!! fatal error !!!\n");

        u4Param = ((prSubtitleInfo->u4Width<<16) + prSubtitleInfo->u4Height);
        // draw subtitle
        //spu_draw(_pucSubOutBuf, u4Width, u4Height, u4StartPts, u4EndPts);
        //call back function
        //Printf("notify info : %d\n",u4Param);
        FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DRAWSUBTITLE, u4Param);
        _rSpuStatus.eState = SPU_WAIT_DRAW_ACK;

        return TRUE;
    }

    return FALSE;
}


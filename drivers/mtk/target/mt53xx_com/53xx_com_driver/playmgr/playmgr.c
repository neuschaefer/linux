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
 * $RCSfile: playmgr.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mtmplayer.c
 *  This file contains implementation of exported APIs of play manager's utility.
 */
#include "playmgr.h"
#include "feeder_if.h"
#include "vdec_if.h"
#include "vdp_if.h"
#include "playmgr_util.h"
#include "swdmx_drvif.h"
#include "swdmx_if.h"
#ifdef TIME_SHIFT_SUPPORT
#include "swdmx_pvr_if.h"
#endif
#include "vdec_drvif.h"
#include "aud_drvif.h"
#include "playmgr_avi.h"
#include "playmgr_mpg.h"
#include "playmgr_wmv.h"
#include "playmgr_mp4.h"
#include "playmgr_mkv.h"
#include "playmgr_mp3.h"
#include "playmgr_rm.h"
#include "playmgr_flv.h"
#include "playmgr_extsubtitle.h"
#include "vdp_if.h"
#include "srm_drvif.h"
#include "playmgr_debug.h"
#include "x_util.h"
#ifndef __KERNEL__
#include "../fm/x_fm.h"
#else
#include <linux/mt53xx_cb.h>    // for Linux driver using callback function
#endif
#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif

// as reviewer request
//#if !defined(CC_MT5387) && !defined(CC_MT5392B) && !defined(CC_MT5391) && !defined(CC_MT5360) && !defined(CC_MT5360B)
#if defined(CC_MT5363)
#define CC_53XX_AUDIO_V2
#endif

/********************************************************************
 MACRO CONSTANT DEFINITIONS
********************************************************************/
//#ifdef __KERNEL__
//#else
//#define CONTROL_AVDEC
//#endif

/********************************************************************
  MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
  TYPE DEFINITION
********************************************************************/
#ifdef __KERNEL__
typedef struct
{
    UINT32 u4ReadSize;
    UINT64 u8FilePos;
} PLAYMGR_RECEIVE_DATA_T;

extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

#endif

//#define PLAYMGR_ASYNC_MODE

#ifdef PLAYMGR_ASYNC_MODE
static VOID _PLAYMGR_MainLoop(VOID* pvArg);
#endif

#define FPS_PREC 1000
#define FILENAME_LENGTH 1024

extern void _AUD_DspWMAHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo);

/********************************************************************
  VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
PLAYMGR_INFO_T _rPlayerInfo;  // change it to non-static, tmp solution
static FeederSourceType eFeederSrc = FEEDER_PROGRAM_SOURCE;
static PLAYMGR_CALLBACK _arNotifyTbl[PLAYMGR_CALLBACK_MAX];
static HANDLE_T _hPlayMgrCmdQ = 0;
static HANDLE_T _hPlayMgrStatusQ = 0;
static HANDLE_T _hPlayMgrDmxCtrlSema = 0;
static HANDLE_T _hPlayMgrVdecCtrlSema = 0;
static HANDLE_T _hPlayMgrAdecCtrlSema = 0;
static HANDLE_T _hPlayMgrVdpCtrlSema = 0;
static HANDLE_T _hPlayMgrEosMutex = 0;
#ifdef PLAYMGR_ASYNC_MODE
static HANDLE_T _hPlayMgrThread = 0;
#endif
static HANDLE_T _hPlayMgrRangeCmdQ = 0;
static HANDLE_T _hPlayMgrPlaybackStatusCmdQ = 0;

static HANDLE_T _hPlaymgrUpdateRangeThread = 0;
static HANDLE_T _hPlayMgrUpdateStatusThread = 0;

static HANDLE_T _hPlaymgrStatusNotifyTimer = 0;
static BOOL _fgPlaymgrStatusNotifyTimerStarted = FALSE;
//static HANDLE_T _hPlayMgrRangeSema = 0;
static HANDLE_T _hPlayMgrSema = 0;
static HANDLE_T _hPlayMgrSemaFinSeek = 0;
static UINT32 _u4AudHandle = 0;

#ifdef __KERNEL__
static HANDLE_T _hPlayMgrDataQ = 0;
#else
static HANDLE_T _hFileHandle = 0;
static HANDLE_T _hHdrHandle = 0;
#endif
static CHAR _szFileName[FILENAME_LENGTH];

static PLAYMGR_STATE_T _ePlayMgrCurStatus = PLAYMGR_STATE_CLOSE;
static PLAYMGR_SourceType_T _ePlayMgrSrcType = PLAYMGR_SRC_TYPE_HIGH_SPEED_STORAGE;
static UINT32 _u4NumberOfBitstream = 1;

#ifdef __MODEL_slt__
static UINT32 _u4SltContainerFormat = MEDIA_CONTNR_SUBTYPE_AVI;
#endif

extern const UINT32 _au4CookSampPerFrame[30];

static COOK_INFO_T _rCookInfo;

static BOOL _fgIsSeeking = FALSE;

static SAMPLE_FREQ_T _GetAudSamplingRate(UINT32 u4Rate)
{
    SAMPLE_FREQ_T eSampleFreq;

    switch (u4Rate)
    {
    case 48000:
        eSampleFreq = FS_48K;
        break;
    case 32000:
        eSampleFreq = FS_32K;
        break;
    case 44100:
        eSampleFreq = FS_44K;
        break;
    case 8000:
        eSampleFreq = FS_8K;
        break;
    case 11025:
    case 11024:
        eSampleFreq = FS_11K;
        break;
    case 12000:
        eSampleFreq = FS_12K;
        break;
   case 16000:
        eSampleFreq = FS_16K;
        break;
    case 22050:
    case 22255:
        eSampleFreq = FS_22K;
        break;
    case 24000:
        eSampleFreq = FS_24K;
        break;
    default:
        eSampleFreq = FS_48K;
        break;
    }

    return eSampleFreq;
}


static void _PLAYMGR_FRC(UINT64 u8Fps, VDEC_SEQUENCE_DATA_T *pVdecSeq)
{
    if (u8Fps < 23976)
    {
        pVdecSeq->e_frame_rate = VDEC_SRC_FRAME_RATE_23_976;
    }
    else if ((u8Fps > 23976) &&
        (u8Fps < 24000))
    {
        pVdecSeq->e_frame_rate = VDEC_SRC_FRAME_RATE_24;
    }
    else if ((u8Fps > 24000) &&
        (u8Fps < 25000))
    {
        pVdecSeq->e_frame_rate = VDEC_SRC_FRAME_RATE_25;
    }
    else if ((u8Fps > 25000) &&
        (u8Fps < 29970))
    {
        pVdecSeq->e_frame_rate = VDEC_SRC_FRAME_RATE_29_97;
    }
    else if ((u8Fps > 29970) &&
        (u8Fps < 30000))
    {
        pVdecSeq->e_frame_rate = VDEC_SRC_FRAME_RATE_30;
    }
    else if ((u8Fps > 30000) &&
        (u8Fps < 50000))
    {
        pVdecSeq->e_frame_rate = VDEC_SRC_FRAME_RATE_50;
    }
    else if ((u8Fps > 50000) &&
        (u8Fps < 59940))
    {
        pVdecSeq->e_frame_rate = VDEC_SRC_FRAME_RATE_59_94;
    }
    else if ((u8Fps > 59940) &&
        (u8Fps < 60000))
    {
        pVdecSeq->e_frame_rate = VDEC_SRC_FRAME_RATE_60;
    }
    else
    {
        pVdecSeq->e_frame_rate = VDEC_SRC_FRAME_RATE_UNKNOWN;
    }
}

BOOL _PLAYMGR_SendUpdStatusNotify(PLAYMGR_STATUS_CMD_T* prStatus);
void _PLAYMGR_FeederCb(MM_NOTIFY_INFO_T *prNfyInfo, UINT32 u4Param);
BOOL _PLAYMGR_SetLPCMInfo(PLAYMGR_LPCM_INFO_T rPlayLPCMInfo);
BOOL _PLAYMGR_SeekPosition(UINT64 u8SeekPos);

static ENUM_SWDMX_VDEC_T _PLAYMGR_PlaymgrVdec2Swdmx(PLAYMGR_VDEC_T ePVdec)
{
    BOOL fgNotSupport;

    fgNotSupport = FALSE;

    if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
    {
        if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
        {
            #if defined(CC_WMV_MPEG1_DISABLE) && defined(CC_WMV_MPEG2_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG1_MPEG2)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_WMV_MPEG4_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG4)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_WMV_DX311_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_DIVX311)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_WMV_H264_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_H264)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_WMV_VC1_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_VC1)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_WMV_MJPEG_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MJPEG)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            {
            }
        }
        else
        if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
        {
            #if defined(CC_AVI_MPEG1_DISABLE) && defined(CC_AVI_MPEG2_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG1_MPEG2)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_AVI_MPEG4_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG4)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_AVI_DX311_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_DIVX311)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_AVI_H264_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_H264)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_AVI_VC1_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_VC1)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_AVI_MJPEG_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MJPEG)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            {
            }
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
        {
            #if defined(CC_MKV_MPEG1_DISABLE) && defined(CC_MKV_MPEG2_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG1_MPEG2)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_MKV_MPEG4_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG4)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_MKV_DX311_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_DIVX311)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_MKV_H264_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_H264)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_MKV_VC1_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_VC1)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_MKV_MJPEG_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MJPEG)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            {
            }
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM)
        {
            #if defined(CC_RM_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_RV8 || ePVdec == PLAYMGR_VDEC_RV9)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            {
            }
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4)
        {
            #if defined(CC_MP4_MPEG1_DISABLE) && defined(CC_MP4_MPEG2_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG1_MPEG2)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_MP4_MPEG4_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG4)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_MP4_DX311_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_DIVX311)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_MP4_H264_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_H264)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_MP4_VC1_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_VC1)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_MP4_MJPEG_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MJPEG)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            {
            }
        }
        else if(_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV)
        {
            #if defined(CC_FLV_MJPG_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MJPEG)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_FLV_SORENSON_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_SORENSON)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_FLV_H264_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_H264)
            {
                fgNotSupport = TRUE;
            }
            #endif
        }
    }
    else if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
    {
        if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_ES)
        {
            #if defined(CC_ES_MPEG1_DISABLE) && defined(CC_ES_MPEG2_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG1_MPEG2)
            {
                fgNotSupport = TRUE;
            }
            #endif
        }
        else if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_H264_ES)
        {
            #if defined(CC_ES_H264_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_H264)
            {
                fgNotSupport = TRUE;
            }
            #endif
        }
        else if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VC1_ES)
        {
            #if defined(CC_ES_VC1_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_VC1)
            {
                fgNotSupport = TRUE;
            }
            #endif
        }
        else if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG4_ES)
        {
            #if defined(CC_ES_MPEG4_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG4)
            {
                fgNotSupport = TRUE;
            }
            #endif
        }
    }
    else if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_PS)
    {
        #if defined(CC_PS_MPEG1_DISABLE) && defined(CC_PS_MPEG2_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_MPEG1_MPEG2)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_PS_MPEG4_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_MPEG4)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_PS_DX311_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_DIVX311)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_PS_H264_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_H264)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_PS_VC1_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_VC1)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_PS_MJPEG_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_MJPEG)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        {
        }
    }
    else if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_TS)
    {
        #if defined(CC_TS_MPEG1_DISABLE) && defined(CC_TS_MPEG2_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_MPEG1_MPEG2)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_TS_MPEG4_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_MPEG4)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_TS_DX311_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_DIVX311)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_TS_H264_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_H264)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_TS_VC1_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_VC1)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_TS_MJPEG_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_MJPEG)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        {
        }
    }
    else
    {
    }

    if(fgNotSupport)
    {
        LOG(0, "PlaymgrVdec Not Support %d\n", (UINT32)ePVdec);
        return ENUM_SWDMX_VDEC_UNKNOWN;
    }

    switch (ePVdec)
    {
    case PLAYMGR_VDEC_MPEG1_MPEG2:
        return ENUM_SWDMX_VDEC_MPEG1_MPEG2;
    case PLAYMGR_VDEC_DIVX311:
        return ENUM_SWDMX_VDEC_DX311;
    case PLAYMGR_VDEC_MPEG4:
        return ENUM_SWDMX_VDEC_MPEG4;
    case PLAYMGR_VDEC_H264:
        return ENUM_SWDMX_VDEC_H264;
    case PLAYMGR_VDEC_WMV7:
        return ENUM_SWDMX_VDEC_WMV7;
    case PLAYMGR_VDEC_WMV8 :
        return ENUM_SWDMX_VDEC_WMV8;
    case PLAYMGR_VDEC_WMV9:
        return ENUM_SWDMX_VDEC_WMV9;
    case PLAYMGR_VDEC_VC1:
        return ENUM_SWDMX_VDEC_VC1;
    case PLAYMGR_VDEC_H263:
        return ENUM_SWDMX_VDEC_DX311;
        case PLAYMGR_VDEC_RV8:
        case PLAYMGR_VDEC_RV9:
            return ENUM_SWDMX_VDEC_RV;
    case PLAYMGR_VDEC_SORENSON:
        return ENUM_SWDMX_VDEC_MPEG4;
    case PLAYMGR_VDEC_MJPEG:
        return ENUM_SWDMX_VDEC_MJPEG;
    default:
        break;
    }

    return ENUM_SWDMX_VDEC_UNKNOWN;
}

static ENUM_SWDMX_ADEC_T _PLAYMGR_PlaymgrAdec2Swdmx(PLAYMGR_ADEC_T ePAdec)
{
    BOOL fgNotSupport;

    fgNotSupport = FALSE;

    if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
    {
        #if defined(CC_ES_MP3_DISABLE)
        if (ePAdec == PLAYMGR_ADEC_MP3)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_ES_MP2_DISABLE)
        if (ePAdec == PLAYMGR_ADEC_MPEG)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_ES_LPCM_DISABLE)
        if (ePAdec == PLAYMGR_ADEC_PCM)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_ES_WMA_DISABLE)
        if (ePAdec == PLAYMGR_ADEC_WMA)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        {
        }
    }

    if(fgNotSupport)
    {
        LOG(0, "PlaymgrAdec Not Support %d\n", (UINT32)ePAdec);
        return ENUM_SWDMX_ADEC_UNKNOWN;
    }

    switch (ePAdec)
    {
    case PLAYMGR_ADEC_MPEG:
        return ENUM_SWDMX_ADEC_MPEG;
    case PLAYMGR_ADEC_AC3:
        return ENUM_SWDMX_ADEC_AC3;
    case PLAYMGR_ADEC_PCM:
        return ENUM_SWDMX_ADEC_PCM;
    case PLAYMGR_ADEC_MP3:
        return ENUM_SWDMX_ADEC_MP3;
    case PLAYMGR_ADEC_AAC:
        return ENUM_SWDMX_ADEC_AAC;
    case PLAYMGR_ADEC_DTS:
        return ENUM_SWDMX_ADEC_DTS;
    case PLAYMGR_ADEC_WMA:
        return ENUM_SWDMX_ADEC_WMA;
    case PLAYMGR_ADEC_RA:
        return ENUM_SWDMX_ADEC_RA;
    case PLAYMGR_ADEC_HDCD:
        return ENUM_SWDMX_ADEC_HDCD;
    case PLAYMGR_ADEC_MLP:
        return ENUM_SWDMX_ADEC_MLP;
    case PLAYMGR_ADEC_MTS:
        return ENUM_SWDMX_ADEC_MTS;
    case PLAYMGR_ADEC_A2:
        return ENUM_SWDMX_ADEC_A2;
    case PLAYMGR_ADEC_PAL:
        return ENUM_SWDMX_ADEC_PAL;
    case PLAYMGR_ADEC_FMFM:
        return ENUM_SWDMX_ADEC_FMFM;
    case PLAYMGR_ADEC_NICAM:
        return ENUM_SWDMX_ADEC_NICAM;
    case PLAYMGR_ADEC_TTXAAC:
        return ENUM_SWDMX_ADEC_TTXAAC;
    case PLAYMGR_ADEC_DETECTOR:
        return ENUM_SWDMX_ADEC_DETECTOR;
    case PLAYMGR_ADEC_MINER:
        return ENUM_SWDMX_ADEC_MINER;
    default:
        break;
    }

    return ENUM_SWDMX_ADEC_UNKNOWN;
}

#if 1
static void _PLAYMGR_StatusNfyTimer(HANDLE_T  pt_tm_handle,
                                    VOID*     pv_tag)
{
    PLAYMGR_STATUS_CMD_T rPlayStatus;

    rPlayStatus.eCmd = PLAYMGR_SETCMD_UPDATEPLAYTIME;
    UNUSED(_PLAYMGR_SendUpdStatusNotify(&rPlayStatus));
    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);
}
#endif

static BOOL _PLAYMGR_ReceiveUpdRangeNotify(VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    zMsgSize = sizeof(UINT32);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                            &(_hPlayMgrRangeCmdQ), 1, X_MSGQ_OPTION_WAIT);

    ASSERT(zMsgSize == sizeof(UINT32));

    if (i4Ret != OSR_OK)
    {
        return FALSE;
    }

    return TRUE;
}


static BOOL _PLAYMGR_SendUpdRangeNotify(UINT32 u4State)
{
    INT32 i4Ret;
    UINT32 u4CurState = u4State;
    SIZE_T zMsgSize = sizeof(UINT32);

    i4Ret = x_msg_q_send(_hPlayMgrRangeCmdQ, &u4CurState, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        i4Ret = x_msg_q_send(_hPlayMgrRangeCmdQ, &u4CurState, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);

    return TRUE;

}

static BOOL _PLAYMGR_FlushRangeNotify(VOID)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    UINT32 u4Temp;

    zMsgSize = sizeof(UINT32);

    while(x_msg_q_receive(&u2MsgQIdx, (VOID*)&u4Temp, &zMsgSize,
                            &(_hPlayMgrRangeCmdQ), 1, X_MSGQ_OPTION_WAIT) == OSR_OK)
    {
    }

    VERIFY(x_msg_q_num_msgs(_hPlayMgrRangeCmdQ, &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt == 0);

    return TRUE;
}

static VOID _PLAYMGR_UpdateRangeLoop(VOID *pvArg)
{
    BOOL fgRet;
    UINT32 u4SeekPts;

    UNUSED(pvArg);
    while (1)
    {
        LOG(5, "_PLAYMGR_UpdateRangeLoop Seek pts = 0x%08x\n",
            u4SeekPts);
        fgRet = _PLAYMGR_ReceiveUpdRangeNotify(&u4SeekPts);
        if(!fgRet)
        {
            LOG(0, "_PLAYMGR_ReceiveUpdRangeNotify fail\n");
        }
        if (fgRet && (_rPlayerInfo.pfnPlaymgrUpdateRange != 0))
        {
            LOG(5, "_PLAYMGR_UpdateRangeLoop seek pts = 0x%08x\n", u4SeekPts);
            if(!_rPlayerInfo.pfnPlaymgrUpdateRange(&_rPlayerInfo, u4SeekPts))
            {
                LOG(3, "pfnPlaymgrUpdateRange fail\n");
            }
        }
    }
}

void _PLAYMGR_FeederCb(MM_NOTIFY_INFO_T *prNfyInfo, UINT32 u4Param)
{
    LOG(5, "_PLAYMGR_FeederCb\n");
    if (!prNfyInfo)
    {
        return;
    }
    if (prNfyInfo->u4Condition < (UINT32)PLAYMGR_CALLBACK_MAX)
    {
        if (_arNotifyTbl[prNfyInfo->u4Condition])
        {
            _arNotifyTbl[prNfyInfo->u4Condition](
                (PLAYMGR_CALLBACK_TYPE_T)prNfyInfo->u4Condition,
                0, 0);
        }
    }
    else
    {
        LOG(5, "_PLAYMGR_RegCb unknow eCallbackType %d\n", prNfyInfo->u4Condition);
    }
    UNUSED(prNfyInfo);
    UNUSED(u4Param);
    return;
}

static BOOL _PLAYMGR_UpdateRangeCb(VOID* pv_nfy_tag,
                                   MM_RANGE_COND_T  e_nfy_cond,
                                   UINT32           ui4_data_1,
                                   UINT32           ui4_data_2)
{
    if (e_nfy_cond == MM_RANGE_DONE)
    {
        LOG(5, "Update RangeCb MM_RANGE_DONE next pts = 0x%08x\n",
            ui4_data_2);

        if (_rPlayerInfo.pfnPlaymgrFreeRange != 0)
        {
            if (!_rPlayerInfo.pfnPlaymgrFreeRange((VOID*)&_rPlayerInfo, (UINT32)ui4_data_1))
            {
                LOG(1, "pfnPlaymgrFreeRange fail\n");
            }
        }
        //_rPlayerInfo.u4SeekPts = ui4_data_2;
        UNUSED(_PLAYMGR_FlushRangeNotify());
        UNUSED(_PLAYMGR_SendUpdRangeNotify(ui4_data_2));
    }
    else if (e_nfy_cond == MM_RANGE_ABORT)
    {
        if (_rPlayerInfo.pfnPlaymgrFreeRange != 0)
        {
            if (!_rPlayerInfo.pfnPlaymgrFreeRange((VOID*)&_rPlayerInfo, (UINT32)ui4_data_1))
            {
                LOG(1, "Free Range Error\n");
            }
        }
    }
#ifdef TIME_SHIFT_SUPPORT
    else if((e_nfy_cond == MM_OUT_OF_SAFE_RANGE_START) ||
            (e_nfy_cond == MM_OUT_OF_SAFE_RANGE_END) ||
            (e_nfy_cond == MM_REACH_VALID_RANGE_START) ||
            (e_nfy_cond == MM_REACH_VALID_RANGE_END))
    {
        UNUSED(FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_PVR_RANGE_NOTIFY,
                ((UINT32)(e_nfy_cond) - (UINT32)(MM_OUT_OF_SAFE_RANGE_START))));
    }
#endif
    return TRUE;
}


static void _PLAYMGR_DmxNfy(void* pvNfyTag,
                            SWDMX_COND_T        eNfyCond,
                            UINT32              u4Data1,
                            UINT32              u4Data2)
{
    if (eNfyCond == SWDMX_COND_CTRL_DONE)
    {
        VERIFY (x_sema_unlock(_hPlayMgrDmxCtrlSema) == OSR_OK);
    }
    else
    {
        LOG(3, "_PLAYMGR_DmxNfy unknow ctrl %d\n", (UINT32)eNfyCond);
    }
    UNUSED(pvNfyTag);
    UNUSED(u4Data1);
    UNUSED(u4Data2);
}


static void _PLAYMGR_DmxFlushCtrl(void)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock(_hPlayMgrDmxCtrlSema, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}


static void _PLAYMGR_DmxWaitCtrlDone(void)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock_timeout(_hPlayMgrDmxCtrlSema, 5000);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "_PLAYMGR_DmxWaitCtrlDone timeout\n");
    }
}


static void _PLAYMGR_VdecNfy(void* pvNfyTag,
                             VDEC_DEC_COND_T     eNfyCond,
                             UINT32              u4Data1,
                             UINT32              u4Data2)
{
    PLAYMGR_STATUS_T* prMediaStatus = &_rPlayerInfo.rPlayerStatus;

    if (eNfyCond == VDEC_COND_CTRL_DONE)
    {
        VERIFY (x_sema_unlock(_hPlayMgrVdecCtrlSema) == OSR_OK);
    }
    else if ((eNfyCond == VDEC_COND_DECODE_STATUS_CHG) &&
             (u4Data1  == (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT))
    {
        LOG(3, "_PLAYMGR_VdecNfy codec not support\n");
        if (!FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_ERROR, 1))
        {
            // 1: MT_MPLAYER_VIDEO_ERROR
            LOG(3, "_PLAYMGR_VdecNfy codec not support fail\n");
        }
        prMediaStatus->fgGotVidEos = TRUE;
        if((!prMediaStatus->fgSendEos) &&
            ((prMediaStatus->fgEnVidStrm && prMediaStatus->fgGotVidEos) ||
             (!prMediaStatus->fgEnVidStrm)) &&
            ((prMediaStatus->fgEnAudStrm && prMediaStatus->fgGotAudEos) ||
             (!prMediaStatus->fgEnAudStrm)))
        {
            prMediaStatus->fgSendEos = TRUE;
            VERIFY(x_sema_unlock(_hPlayMgrEosMutex) == OSR_OK);
            LOG(3, "_PLAYMGR_VdecNfy not support FeederNotifyHandle FileEnd\n");
            if(!FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_FILEEND, 0))
            {
                LOG(3, "_PLAYMGR_VdecNfy not support FeederNotifyHandle fail\n");
            }
        }
    }
    else
    {
        LOG(3, "_PLAYMGR_VdecNfy unknow ctrl %d\n", (UINT32)eNfyCond);
    }
    UNUSED(pvNfyTag);
    UNUSED(u4Data1);
    UNUSED(u4Data2);
}


static void _PLAYMGR_VdecFlushCtrl(void)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock(_hPlayMgrVdecCtrlSema, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}


static void _PLAYMGR_VdecWaitCtrlDone(void)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock_timeout(_hPlayMgrVdecCtrlSema, 5000);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "_PLAYMGR_VdecWaitCtrlDone timeout\n");
    }
}

static VOID _PLAYMGR_AdecNfy (void *          pvNfyTag,
                              UINT8           u1DecId,
                              AUD_COND_NFY_T  eNfyCond,
                              UINT32          u4Data1,
                              UINT32          u4Data2)
{
    PLAYMGR_STATUS_T* prMediaStatus = &_rPlayerInfo.rPlayerStatus;
    if (eNfyCond == AUD_COND_MM_EOS)
    {
        LOG(0, "_PLAYMGR_AdecNfy AUD_COND_MM_EOS\n");
        if (prMediaStatus == NULL)
        {
            LOG(1, "_PLAYMGR_AdecNfy prMediaStatus null\n");
            return;
        }
        VERIFY(x_sema_lock(_hPlayMgrEosMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        prMediaStatus->fgGotAudEos = TRUE;
        if ((!prMediaStatus->fgSendEos) &&
                ((prMediaStatus->fgEnVidStrm && prMediaStatus->fgGotVidEos) ||
                 (!prMediaStatus->fgEnVidStrm)) &&
                ((prMediaStatus->fgEnAudStrm && prMediaStatus->fgGotAudEos) ||
                 (!prMediaStatus->fgEnAudStrm)))
        {
            prMediaStatus->fgSendEos = TRUE;
            VERIFY(x_sema_unlock(_hPlayMgrEosMutex) == OSR_OK);
            LOG(3, "_PLAYMGR_AdecNfy FeederNotifyHandle FileEnd\n");
            if (!FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_FILEEND, 0))
            {
                LOG(3, "_PLAYMGR_AdecNfy FeederNotifyHandle fail\n");
            }
        }
        else
        {
            VERIFY(x_sema_unlock(_hPlayMgrEosMutex) == OSR_OK);
        }
    }
    else if (eNfyCond == AUD_COND_CTRL_DONE)
    {
        LOG(3, "_PLAYMGR_AdecNfy _PLAYMGR_AUD_CTRL_DONE\n");
        VERIFY (x_sema_unlock(_hPlayMgrAdecCtrlSema) == OSR_OK);
    }
    else if ((eNfyCond == AUD_COND_AUD_DECODE_STATUS_CHG)
             && (u4Data1 == AUD_DECODE_NOT_SUPPORT))
    {
        LOG(3, "_PLAYMGR_AdecNfy _PLAYMGR_AUD_CTRL_DONE AUD_DECODE_NOT_SUPPORT\n");
        if (!FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_ERROR, 2))
        {
            // 2: MT_MPLAYER_AUDIO_ERROR
            LOG(3, "_PLAYMGR_AdecNfy codec not support fail\n");
        }
        prMediaStatus->fgGotAudEos = TRUE;
        if((!prMediaStatus->fgSendEos) &&
            ((prMediaStatus->fgEnVidStrm && prMediaStatus->fgGotVidEos) ||
             (!prMediaStatus->fgEnVidStrm)) &&
            ((prMediaStatus->fgEnAudStrm && prMediaStatus->fgGotAudEos) ||
             (!prMediaStatus->fgEnAudStrm)))
        {
            prMediaStatus->fgSendEos = TRUE;
            VERIFY(x_sema_unlock(_hPlayMgrEosMutex) == OSR_OK);
            LOG(3, "_PLAYMGR_AdecNfy not support FeederNotifyHandle FileEnd\n");
            if(!FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_FILEEND, 0))
            {
                LOG(3, "_PLAYMGR_AdecNfy not support FeederNotifyHandle fail\n");
            }
        }
        VERIFY (x_sema_unlock(_hPlayMgrAdecCtrlSema) == OSR_OK);
    }
    else
    {
        LOG(3, "_PLAYMGR_AdecNfy unknow ctrl %d, u4Data1 %d\n",
            (UINT32)eNfyCond, u4Data1);
    }
    UNUSED(pvNfyTag);
    UNUSED(u1DecId);
    UNUSED(u4Data2);
}

static void _PLAYMGR_AdecFlushCtrl(void)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock(_hPlayMgrAdecCtrlSema, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}

static void _PLAYMGR_AdecWaitCtrlDone(void)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock_timeout(_hPlayMgrAdecCtrlSema, 5000);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "_PLAYMGR_AdecWaitCtrlDone timeout\n");
    }
}

static VOID _PLAYMGR_VdpNfy(UINT32 u4VdpId, VDP_COND_T eCond)
{
    PLAYMGR_STATUS_T* prMediaStatus = &_rPlayerInfo.rPlayerStatus;

    LOG(0, "_PLAYMGR_VdpNfy\n");
    if (eCond == VDP_COND_EOS)
    {
        LOG(3, "_PLAYMGR_VdpNfy VDP_COND_EOS\n");

        if (prMediaStatus == NULL)
        {
            LOG(1, "_PLAYMGR_VdpNfy prMediaStatus null\n");
            return;
        }
        VERIFY(x_sema_lock(_hPlayMgrEosMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        prMediaStatus->fgGotVidEos = TRUE;
        if ((!prMediaStatus->fgSendEos) &&
                ((prMediaStatus->fgEnVidStrm && prMediaStatus->fgGotVidEos) ||
                 (!prMediaStatus->fgEnVidStrm)) &&
                ((prMediaStatus->fgEnAudStrm && prMediaStatus->fgGotAudEos) ||
                 (!prMediaStatus->fgEnAudStrm)))
        {
            prMediaStatus->fgSendEos = TRUE;
            VERIFY(x_sema_unlock(_hPlayMgrEosMutex) == OSR_OK);
            LOG(3, "_PLAYMGR_VdpNfy FeederNotifyHandle FileEnd\n");
            FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_FILEEND, 0);
        }
        else
        {
            VERIFY(x_sema_unlock(_hPlayMgrEosMutex) == OSR_OK);
        }
    }
    else
    {
        LOG(3, "_PLAYMGR_VdpNfy unknow condition %d\n", (UINT32)eCond);
    }
}

static VOID _PLAYMGR_VdpStepDoneNfy(UINT32 u4VdpId, UINT32 u4PTS)
{
    UNUSED(u4VdpId);
    UNUSED(u4PTS);
    LOG(3, "_PLAYMGR_VdpStepDoneNfy\n");

    STC_StopStc();
    VERIFY (x_sema_unlock(_hPlayMgrVdpCtrlSema) == OSR_OK);

    // notify timer
    _PLAYMGR_StatusNfyTimer(0, 0);
}

static void _PLAYMGR_VdpFlushCtrl(void)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock(_hPlayMgrVdpCtrlSema, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}

static void _PLAYMGR_VdpWaitCtrlDone(void)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock_timeout(_hPlayMgrVdpCtrlSema, 50000);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "_PLAYMGR_VdpWaitCtrlDone timeout\n");
    }
}

static BOOL _PLAYMGR_ReceiveUpdStatusNotify(VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    zMsgSize = sizeof(PLAYMGR_STATUS_CMD_T);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                            &(_hPlayMgrPlaybackStatusCmdQ), 1, X_MSGQ_OPTION_WAIT);

    ASSERT(zMsgSize == sizeof(PLAYMGR_STATUS_CMD_T));

    if (i4Ret != OSR_OK)
    {
        return FALSE;
    }

    return TRUE;
}


BOOL _PLAYMGR_SendUpdStatusNotify(PLAYMGR_STATUS_CMD_T* prStatus)
{
    INT32 i4Ret;
    SIZE_T zMsgSize = sizeof(PLAYMGR_STATUS_CMD_T);

    i4Ret = x_msg_q_send(_hPlayMgrPlaybackStatusCmdQ, prStatus, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        PLAYMGR_STATUS_CMD_T rNotify;
        SIZE_T zMsgSize;
        UINT16 u2MsgQIdx;
        zMsgSize = sizeof(PLAYMGR_STATUS_CMD_T);

        // when seek done or step done, (step or seek done)
        // we will send a message to this message queue.
        // when our message is more then timer receive,
        // we should remove older message
        i4Ret = x_msg_q_receive(&u2MsgQIdx, &rNotify, &zMsgSize,
                                &(_hPlayMgrPlaybackStatusCmdQ), 1, X_MSGQ_OPTION_WAIT);
        UNUSED(i4Ret);
        x_thread_delay(10);
        i4Ret = x_msg_q_send(_hPlayMgrPlaybackStatusCmdQ, prStatus, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);

    return TRUE;

}


BOOL _PLAYMGR_SubtitleCb(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
                         UINT32 u4Data, const void* pvTag)
{
    BOOL fgRet = TRUE;
    UINT32 u4DataAddr;
    DMX_NOTIFY_INFO_PES_T* prNotifyInfo;
    PLAYMGR_STATUS_CMD_T rStatus;

    switch (eCode)
    {
    case DMX_NOTIFY_CODE_PES:
        prNotifyInfo = (DMX_NOTIFY_INFO_PES_T*)u4Data;
        //ASSERT(prNotifyInfo != NULL);
        u4DataAddr = VIRTUAL(prNotifyInfo->u4DataAddr);
        //ASSERT(u4DataAddr < 0x10000000);
        rStatus.eCmd = PLAYMGR_SETCMD_UPDATESUBTITLE;
        rStatus.u4Param1 = u4DataAddr;
        rStatus.u4Param2 = (UINT32)_rPlayerInfo.pucSubBuf;
        rStatus.u4Param3 = prNotifyInfo->u4DataSize;
        rStatus.u4Param4 = (UINT32)prNotifyInfo->u1SerialNumber;
        UNUSED(_PLAYMGR_SendUpdStatusNotify(&rStatus));
        break;
    case DMX_NOTIFY_CODE_OVERFLOW:
        break;

    default:
        // Unknown notification code
        LOG(1, "Pidx %u: Unknown notification code %u\n", u1Pidx,
            (UINT32)eCode);
        ASSERT(0);
        break;
    }

    return fgRet;
}


static VOID _PLAYMGR_UpdateStatusLoop(VOID *pvArg)
{
    PLAYMGR_STATUS_CMD_T rNotify;
    //UINT32 u4CurDisplayTime = 0;

    while (1)
    {
        _PLAYMGR_ReceiveUpdStatusNotify(&rNotify);

        // Do subtitle decoding and callback to AP.
        switch (rNotify.eCmd)
        {
        case PLAYMGR_SETCMD_UPDATEPLAYTIME:
            if (_arNotifyTbl[PLAYMGR_CALLBACK_ELAPSEDTIME])
            {
                _arNotifyTbl[PLAYMGR_CALLBACK_ELAPSEDTIME](
                    PLAYMGR_CALLBACK_ELAPSEDTIME,
                    0, 0);
            }
            break;
            // Draw Subtitle
        case PLAYMGR_SETCMD_UPDATESUBTITLE:
            _PLAYMGR_DrawSubtitle(&rNotify, &_rPlayerInfo);
            break;
        default:
            break;
        }
    }
}


static UINT32 _PLAYMGR_DataQuery(FeederSourceType eSrcType, FEEDER_BUF_INFO_T *pQryBufInfo,
                                 FEEDER_REQ_DATA_T* pt_ReqData)
{
    UINT32 u4Read;

#ifdef __KERNEL__
    MM_CALLBACK_INFO_T rCallbackInfo;
    PLAYMGR_RECEIVE_DATA_T rReceive;
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize = sizeof(PLAYMGR_RECEIVE_DATA_T);

    if(!pt_ReqData)
    {
        LOG(1, "pt_ReqData null\n");
        return 0;
    }

    rCallbackInfo.rMMNotify.u4SrcType = (UINT32)eSrcType;
    rCallbackInfo.rMMNotify.u4Condition = FEEDER_NOTIFY_NEEDDATA;
    rCallbackInfo.rFileInfo.u8Offset = pt_ReqData->u8FilePos;
    rCallbackInfo.rFileInfo.u1Whence = 0;
    rCallbackInfo.u4QuerySize = (UINT32)pt_ReqData->u4ReadSize;
    rCallbackInfo.u4WritePtr = pt_ReqData->u4WriteAddr - pQryBufInfo->u4StartAddr;

    VERIFY (x_sema_lock(_hPlayMgrSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _CB_PutEvent(CB_MTMPLAYER_TRIGGER, sizeof(MM_CALLBACK_INFO_T), &rCallbackInfo);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, (VOID *)&rReceive, &zMsgSize,
                            &(_hPlayMgrDataQ), 1, X_MSGQ_OPTION_WAIT);

    VERIFY (x_sema_unlock(_hPlayMgrSema) == OSR_OK);

    if (rReceive.u4ReadSize == 0 && (rReceive.u8FilePos + 1) == 0)  // EOS notification for network application (size == 0 && pos == 0xFFFFFFFFFFFFFFFF)
    {
        pt_ReqData->fgEof = TRUE;
    }
    else
    {
        pt_ReqData->fgEof = FALSE;
    }
	
    if (rReceive.u4ReadSize == 0 && (rReceive.u8FilePos + 2) == 0)  // ERROR notification for network application (size == 0 && pos == 0xFFFFFFFFFFFFFFFE)
    {
        pt_ReqData->u4WriteAddr= 0;
		LOG(0,"_PLAYMGR_DataQuery get Error\n");
    }

    u4Read = rReceive.u4ReadSize;

    ASSERT(zMsgSize == sizeof(PLAYMGR_RECEIVE_DATA_T));

    if (i4Ret != OSR_OK)
    {
        return 0;
    }

    if(pt_ReqData->u4ReadSize != u4Read)
    {
        LOG(1, "pt_ReqData->u4ReadSize(%d) != rReceive.u4ReadSize(%d)\n", pt_ReqData->u4ReadSize, rReceive.u4ReadSize);
    }

    // ToDo: enable this on linux
    //HalFlushInvalidateDCache();

#else
    UINT64 u8CurPos;
    UINT32 u4FeedingSize1, u4FeedingSize2, u4Cnt;
    HANDLE_T hFile;

    hFile = (eSrcType == FEEDER_PARSING_SOURCE)? _hHdrHandle : _hFileHandle;
    u4Read = 0;

    if(!pt_ReqData)
    {
        LOG(1, "pt_ReqData null\n");
        return 0;
    }

    pt_ReqData->fgEof = FALSE;

    if ((eSrcType == FEEDER_PARSING_SOURCE) ||
            ((pt_ReqData->u4WriteAddr + pt_ReqData->u4ReadSize) <= pQryBufInfo->u4EndAddr))
        //header parsing, use external buf, or ring buf needn't back to head
    {
        u4FeedingSize1 = pt_ReqData->u4ReadSize;
        u4FeedingSize2 = 0;
    }
    else    //ring buf, back to buffer head
    {
        u4FeedingSize1 = pQryBufInfo->u4EndAddr - pt_ReqData->u4WriteAddr;
        u4FeedingSize2 = pt_ReqData->u4ReadSize - u4FeedingSize1;
    }

    if (eSrcType != FEEDER_PARSING_SOURCE)
    {                                       //read AV data, set offset here
        x_fm_lseek(hFile, (INT64)pt_ReqData->u8FilePos, FM_SEEK_BGN, &u8CurPos);
    }
    x_fm_read(hFile, (VOID *)pt_ReqData->u4WriteAddr, u4FeedingSize1, &u4Cnt);
    #ifdef CC_MT5395
    HalFlushDCacheMultipleLine(pt_ReqData->u4WriteAddr, u4FeedingSize1);
    #else
    HalFlushInvalidateDCache();
    #endif
    if (u4FeedingSize2)
    {
        x_fm_read(hFile, (VOID *)pQryBufInfo->u4StartAddr, u4FeedingSize2, &u4Read);
        #ifdef CC_MT5395
        HalFlushDCacheMultipleLine(pQryBufInfo->u4StartAddr, u4FeedingSize2);
        #else
        HalFlushInvalidateDCache();
        #endif
    }
    u4Read += u4Cnt;
#endif

    return (UINT32)u4Read;
}

static INT64 _PLAYMGR_SeekPos(FeederSourceType eSrcType, MM_FILE_OPS_T *prFileInfo)
{
    UINT64 u8CurPos;
    UINT64 u8Offset = prFileInfo->u8Offset;
    UINT8 u1Whence = prFileInfo->u1Whence;

//read header data, set offset here
#ifdef __KERNEL__
    MM_CALLBACK_INFO_T rCallbackInfo;
    PLAYMGR_RECEIVE_DATA_T rReceive;
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize = sizeof(PLAYMGR_RECEIVE_DATA_T);

    x_memset(&rCallbackInfo, 0, sizeof(rCallbackInfo));
    rCallbackInfo.rMMNotify.u4SrcType = (UINT32)eSrcType;
    rCallbackInfo.rMMNotify.u4Condition = FEEDER_NOTIFY_NEEDDATA;
    rCallbackInfo.rFileInfo.u8Offset = u8Offset;
    rCallbackInfo.rFileInfo.u1Whence = u1Whence;

    VERIFY (x_sema_lock(_hPlayMgrSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _CB_PutEvent(CB_MTMPLAYER_TRIGGER, sizeof(MM_CALLBACK_INFO_T), &rCallbackInfo);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, (VOID *)&rReceive, &zMsgSize,
                            &(_hPlayMgrDataQ), 1, X_MSGQ_OPTION_WAIT);

    VERIFY (x_sema_unlock(_hPlayMgrSema) == OSR_OK);

    u8CurPos = rReceive.u8FilePos;

    ASSERT(zMsgSize == sizeof(PLAYMGR_RECEIVE_DATA_T));

    if (i4Ret != OSR_OK)
    {
        return 0;
    }
#else
    HANDLE_T hFile;

    hFile = (eSrcType == FEEDER_PARSING_SOURCE)? _hHdrHandle : _hFileHandle;

    x_fm_lseek(hFile, (INT64)u8Offset, u1Whence, &u8CurPos);
#endif

    return (INT64)u8CurPos;
}

BOOL _PLAYMGR_ReceiveNotify(VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    zMsgSize = sizeof(UINT32);

    /*i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                            &(_hPlayMgrCmdQ), 1, X_MSGQ_OPTION_WAIT);*/

    i4Ret = x_msg_q_receive_timeout (&u2MsgQIdx, pvData, &zMsgSize,
                            &(_hPlayMgrCmdQ), 1, PLAYMGR_NOTIFICATION_TIMEOUT);

    ASSERT(zMsgSize == sizeof(UINT32));

    if (i4Ret == OSR_TIMEOUT)
    {
        LOG(1, "_PLAYMGR_ReceiveNotify TimeOut!!!\n");
        return FALSE;
    }
    else if (i4Ret != OSR_OK)
    {
        LOG(1, "_PLAYMGR_ReceiveNotify is not OSR_OK!!!\n");
        return FALSE;
    }

    return TRUE;
}


BOOL _PLAYMGR_SendNotify(UINT32 u4State)
{
    INT32 i4Ret;
    UINT32 u4CurState = u4State;
    SIZE_T zMsgSize = sizeof(UINT32);

    i4Ret = x_msg_q_send(_hPlayMgrCmdQ, &u4CurState, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        i4Ret = x_msg_q_send(_hPlayMgrCmdQ, &u4CurState, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);

    return TRUE;

}


static VOID _PLAYMGR_SeekFinNotify(UINT32 u4PTS)
{
    LOG(3, "_PLAYMGR_SeekFinNotify u4PTS %d\n", (u4PTS/90));
    VERIFY (x_sema_unlock(_hPlayMgrSemaFinSeek) == OSR_OK);
    FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_SEEK_END, (u4PTS/90));

    _rPlayerInfo.u4SeekPts = u4PTS;
    VDEC_RegRenderPtsCb(NULL);

    // notify timer
    _PLAYMGR_StatusNfyTimer(0, 0);

    _fgIsSeeking = FALSE;
}

#if 1  // megaa SPU
UCHAR *_pucSubOutBuf = NULL;
#endif


BOOL _PLAYMGR_Init(VOID)
{
    FEEDER_QUERY_DATA rSetQuery;
    INT32 i4Ret = OSR_OK;

    if (_rPlayerInfo.fgInit == FALSE)
    {
        LOG(5, "_PLAYMGR_Init\n");

        SWDMX_Init();
        if (!_PLAYMGR_InitMediaInfo())
        {
            return FALSE;
        }

        if (!_PLAYMGR_InitExtSub())
        {
            return FALSE;
        }

        x_memset(&_rPlayerInfo, 0, sizeof(PLAYMGR_INFO_T));

        // create msg queue
        VERIFY(x_msg_q_create(&_hPlayMgrCmdQ, "PLAYMGR_CMDQ", sizeof(UINT32),
                              PLAYMGR_CMD_Q_SIZE) == OSR_OK);

        VERIFY(x_msg_q_create(&_hPlayMgrStatusQ, "PLAYMGR_StatusQ", sizeof(PLAYMGR_STATUS_CMD_T),
                              PLAYMGR_CMD_Q_SIZE) == OSR_OK);

#ifdef __KERNEL__
        VERIFY(x_msg_q_create(&_hPlayMgrDataQ, "PLAYMGR_DataQ", sizeof(PLAYMGR_RECEIVE_DATA_T),
                              PLAYMGR_DATA_Q_SIZE) == OSR_OK);
#endif

#ifdef PLAYMGR_ASYNC_MODE
        i4Ret = x_thread_create(&_hPlayMgrThread, "PlayQueryTask",
                                PLAYMGR_STACK_SIZE, PLAYMGR_THREAD_PRIORITY,
                                _PLAYMGR_MainLoop, 0, NULL);
#endif
        if (i4Ret != OSR_OK)
        {
            LOG(5, "_FeederOpen: Create the query thread failed \n");
            return FALSE;
        }

        VERIFY(x_msg_q_create(&_hPlayMgrRangeCmdQ, "PLAYMGR_RANGECMDQ", sizeof(UINT32),
                              PLAYMGR_CMD_Q_SIZE) == OSR_OK);

        // Create for playback status update and drawing subtitle.
        VERIFY(x_msg_q_create(&_hPlayMgrPlaybackStatusCmdQ, "PLAYMGR_STATUSCMDQ", sizeof(PLAYMGR_STATUS_CMD_T),
                              PLAYMGR_CMD_Q_SIZE) == OSR_OK);

        VERIFY (x_sema_create(&_hPlayMgrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

        VERIFY (x_sema_create(&_hPlayMgrSemaFinSeek, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY(x_sema_create(&_hPlayMgrDmxCtrlSema,
                             X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY(x_sema_create(&_hPlayMgrVdecCtrlSema,
                             X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY(x_sema_create(&_hPlayMgrAdecCtrlSema,
                             X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY(x_sema_create(&_hPlayMgrVdpCtrlSema,
                             X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY(x_sema_create(&_hPlayMgrEosMutex, X_SEMA_TYPE_MUTEX,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);

        // according to thread_lib.h
        // #define THREAD_NAME_LEN         16
        // thread name should not over this length. or we will leak '\0' character
        // create update range thread
        VERIFY(x_thread_create(&_hPlaymgrUpdateRangeThread,
                               "PlayUpdRgeThd", PLAYMGR_STACK_SIZE, PLAYMGR_THREAD_PRIORITY,
                               (x_os_thread_main_fct)_PLAYMGR_UpdateRangeLoop, 0, NULL) == OSR_OK);

        // create status update and subtitle thread
        VERIFY(x_thread_create(&_hPlayMgrUpdateStatusThread,
                               "PlayUpdStsThd", PLAYMGR_STACK_SIZE, PLAYMGR_THREAD_PRIORITY,
                               (x_os_thread_main_fct)_PLAYMGR_UpdateStatusLoop, 0, NULL) == OSR_OK);

        VERIFY(x_timer_create(&_hPlaymgrStatusNotifyTimer) == OSR_OK);

#if 0   // megaa SPU
        _pucSubOutBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(512*384, 16));
#endif

        _rPlayerInfo.pucSubBuf =
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(PLAYMGR_SUBTITLE_BUFSIZE, 16));

        _rPlayerInfo.rMediaFeeder.h_feeder = (UINT32)FEEDER_PARSING_SOURCE;
        _rPlayerInfo.rMediaFeeder.t_feeder_be_fct_tbl.pf_copybytes = FeederCopyBytesBE;
        _rPlayerInfo.rMediaFeeder.t_feeder_be_fct_tbl.pf_input4 = FeederInput4BytesBE;
        _rPlayerInfo.rMediaFeeder.t_feeder_be_fct_tbl.pf_input3 = FeederInput3BytesBE;
        _rPlayerInfo.rMediaFeeder.t_feeder_be_fct_tbl.pf_input2 = FeederInput2BytesBE;
        _rPlayerInfo.rMediaFeeder.t_feeder_be_fct_tbl.pf_input1 = FeederInput1Bytes;
        _rPlayerInfo.rMediaFeeder.t_feeder_be_fct_tbl.pf_set_pos = FeederSetPos;
        _rPlayerInfo.rMediaFeeder.t_feeder_be_fct_tbl.pf_get_pos = FeederGetPos;
        _rPlayerInfo.rMediaFeeder.t_feeder_le_fct_tbl.pf_copybytes = FeederCopyBytesLE;
        _rPlayerInfo.rMediaFeeder.t_feeder_le_fct_tbl.pf_input4 = FeederInput4BytesLE;
        _rPlayerInfo.rMediaFeeder.t_feeder_le_fct_tbl.pf_input3 = FeederInput3BytesLE;
        _rPlayerInfo.rMediaFeeder.t_feeder_le_fct_tbl.pf_input2 = FeederInput2BytesLE;
        _rPlayerInfo.rMediaFeeder.t_feeder_le_fct_tbl.pf_input1 = FeederInput1Bytes;
        _rPlayerInfo.rMediaFeeder.t_feeder_le_fct_tbl.pf_set_pos = FeederSetPos;
        _rPlayerInfo.rMediaFeeder.t_feeder_le_fct_tbl.pf_get_pos = FeederGetPos;

        x_memset(&_arNotifyTbl, 0, sizeof(_arNotifyTbl));
        _rPlayerInfo.fgInit = TRUE;
        _rPlayerInfo.rPlayerStatus.i4Speed = 1000;
        _rPlayerInfo.rPlayerStatus.i4PreviousSpeed = 1000;
        _rPlayerInfo.u4SeekPts = 0xFFFFFFFF;
    }

    FeederOpen(eFeederSrc);
    FeederOpen(FEEDER_PARSING_SOURCE);
    FeederSetAppQueryMode(eFeederSrc, FEEDER_PULL_MODE);
    FeederSetAppQueryMode(FEEDER_PARSING_SOURCE, FEEDER_PULL_MODE);
    rSetQuery.eDataType = eFeederSrc;
    rSetQuery.pfnSource = _PLAYMGR_DataQuery;
    rSetQuery.pfnSeekPos = _PLAYMGR_SeekPos;
    rSetQuery.pfnConsume = NULL;
    rSetQuery.u4QuerySize = 0;
    FeederRegisterPullDataFunc(eFeederSrc, &rSetQuery);
    FeederRegisterPullDataFunc(FEEDER_PARSING_SOURCE, &rSetQuery);
    FeederStart(eFeederSrc);
	FeederSetRing(eFeederSrc,TRUE);
    FeederStart(FEEDER_PARSING_SOURCE);
	FeederSetRing(FEEDER_PARSING_SOURCE,FALSE);
    FeederSetInfo(FEEDER_MM_USE, FALSE);
    // should not change it here.
    //_ePlayMgrSrcType = PLAYMGR_SRC_TYPE_HIGH_SPEED_STORAGE;
    _u4NumberOfBitstream = 1;

    return TRUE;
}


/**Initialize MPlayer
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Open(CHAR* pcFileName)
{
    UINT32 u4FileNameLength;

    if (pcFileName == NULL)
    {
        LOG(3, "_PLAYMGR_Open pcFileName NULL\n");
    }

    LOG(5, "_PLAYMGR_Open\n");

    STC_SetStcValue((UINT32)-3000);

#ifndef __KERNEL__
    if(pcFileName)
    {
        if (FMR_OK != x_fm_open(FM_ROOT_HANDLE, pcFileName, FM_READ_ONLY, 0777, FALSE, &_hFileHandle))
        {
            return FALSE;
        }
        if (FMR_OK != x_fm_open(FM_ROOT_HANDLE, pcFileName, FM_READ_ONLY, 0777, FALSE, &_hHdrHandle))
        {
            return FALSE;
        }
    }
#endif

    _szFileName[0] = '\0';
    if(pcFileName)
    {
        u4FileNameLength = x_strlen(pcFileName);
        if (u4FileNameLength < FILENAME_LENGTH)
        {
            x_strcpy(_szFileName, pcFileName);
        }
    }

    return TRUE;
}


BOOL _PLAYMGR_Cfg(VOID)
{
    SWDMX_NFY_INFO_T rSwdmxNfyInfo;
    VDEC_DEC_NFY_INFO_T rVDecNfyInfo;

    _rPlayerInfo.pfnPlayMgrStart = NULL;
    _rPlayerInfo.pfnPlayMgrStop = NULL;
    _rPlayerInfo.pfnPlayMgrClose = NULL;
    _rPlayerInfo.pfnPlayMgrPause = NULL;
    _rPlayerInfo.pfnPlayMgrResume = NULL;
    _rPlayerInfo.pfnPlayMgrOpenMediaInfo = NULL;
    _rPlayerInfo.pfnPlaymgrUpdateRange = NULL;
    _rPlayerInfo.pfnPlaymgrFreeRange = NULL;

    // Set swdmx's notify function.
    rSwdmxNfyInfo.pvTag = 0;
    rSwdmxNfyInfo.pfSwdmxNfy = _PLAYMGR_DmxNfy;
    SWDMX_SetDmxNfy(&rSwdmxNfyInfo);
    // swdmx_cfg will reset registed range cb, call it after SWDMX_SetContainerType
    //SWDMX_SetRangeCb((VOID*)_PLAYMGR_UpdateRangeCb);

    rVDecNfyInfo.pvTag = 0;
    rVDecNfyInfo.pfDecNfy = _PLAYMGR_VdecNfy;
    VDEC_SetDecNfy(ES0, &rVDecNfyInfo);

    VDP_RegCbFunc(VDP_CB_FUNC_MM_COND_IND, (UINT32)_PLAYMGR_VdpNfy, 0, 0);
    if (_u4AudHandle == 0)
    {
        AUD_RegNotifyFunc(&_u4AudHandle, _PLAYMGR_AdecNfy);
    }

#ifndef CC_PS_DISABLE
    if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_PS)
    {
        if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_ps_subtype
                == MEDIA_PS_SUBTYPE_DAT)
        {
            SWDMX_SetContainerType(SWDMX_FMT_MPEG1_DAT);
            _rPlayerInfo.rPlayerStatus.eMediaType =
                PLAYMGR_MEDIA_TYPE_VCD;
        }
        else
        {
            SWDMX_SetContainerType(SWDMX_FMT_MPEG2_PS);
            _rPlayerInfo.rPlayerStatus.eMediaType =
                PLAYMGR_MEDIA_TYPE_MPEG2_PS;
        }
        _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_MpgStart;
        _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_MpgStop;
        _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
    }
    else
#endif
#ifndef CC_TS_DISABLE
    if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_TS)
    {
        if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_ts_subtype
                == MEDIA_TS_SUBTYPE_TS_188)
        {
            _rPlayerInfo.rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG2_TS;
            SWDMX_SetContainerType(SWDMX_FMT_MPEG2_TS);
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_ts_subtype
                 == MEDIA_TS_SUBTYPE_TS_192)
        {
            _rPlayerInfo.rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG2_TS_192;
            SWDMX_SetContainerType(SWDMX_FMT_MPEG2_TS_192);
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_ts_subtype
                 == MEDIA_TS_SUBTYPE_TS_192_ZERO)
        {
            _rPlayerInfo.rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG2_TS_ZERO_192;
            SWDMX_SetContainerType(SWDMX_FMT_MPEG2_TS_ZERO_192);
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_ts_subtype
                 == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT)
        {
            _rPlayerInfo.rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT;
            SWDMX_SetContainerType(SWDMX_FMT_MPEG2_TS_TIME_SHIFT);
        }
        else
        {
            LOG(5,"_PLAYMGR_Cfg MEDIA_TYPE_TS unknown\n");
            return FALSE;
        }
        _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_MpgStart;
        _rPlayerInfo.pfnPlayMgrPause = _PLAYMGR_MpgPause;
        _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_MpgStop;
        _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
    }
    else
#endif
#ifndef CC_ES_DISABLE
    if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
    {
        if ((_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_M1V) ||
                (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_M2V))
        {
            SWDMX_SetContainerType(SWDMX_FMT_MPEG2_VIDEO_ES);
            _rPlayerInfo.rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG2_ES;
            _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_MpgStart;
            _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_MpgStop;
            _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_VC1)
        {
            SWDMX_SetContainerType(SWDMX_FMT_VC1_ES);
            _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_MpgStart;
            _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_MpgStop;
            _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
            _rPlayerInfo.rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_VC1_ES;
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_H264)
        {
            SWDMX_SetContainerType(SWDMX_FMT_H264_VIDEO_ES);
            _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_MpgStart;
            _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_MpgStop;
            _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
            _rPlayerInfo.rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_H264_ES;
        }
        else if(_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_M4V)
        {
            SWDMX_SetContainerType(SWDMX_FMT_MPEG4_VIDEO_ES);
            _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_MpgStart;
            _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_MpgStop;
            _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
            _rPlayerInfo.rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG4_ES;
        }
    }
    else if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
    {
        if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3)
        {
            SWDMX_SetContainerType(SWDMX_FMT_RAW_AUDIO_ES);
            _rPlayerInfo.rPlayerStatus.eMediaType =
                PLAYMGR_MEDIA_TYPE_MP3;
            _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_Mp3Start;
            _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_Mp3Stop;
            _rPlayerInfo.pfnPlayMgrClose = _PLAYMGR_Mp3Close;
            _rPlayerInfo.pfnPlayMgrPause = _PLAYMGR_Mp3Pause;
            _rPlayerInfo.pfnPlayMgrResume = _PLAYMGR_Mp3Resume;
            _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMp3MediaInfo;
        }
#ifndef CC_WMV_DISABLE
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WMA)
        {
            SWDMX_SetContainerType(SWDMX_FMT_VC1_WMV);
            _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_WmvStart;
            _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_WmvStop;
            _rPlayerInfo.pfnPlayMgrClose = _PLAYMGR_WmvClose;
            _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenWmvMediaInfo;
        }
#endif
        else
        {
            SWDMX_SetContainerType(SWDMX_FMT_RAW_AUDIO_ES);
            if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_MP2)
            {
                _rPlayerInfo.rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_MP2;
            }
            else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_WAVE)
            {
                _rPlayerInfo.rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_WAVE;
            }
            else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_AIF)
            {
                _rPlayerInfo.rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_AIF;
            }
            else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_AC3)
            {
                _rPlayerInfo.rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_AC3;
            }
            else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_AAC)
            {
                _rPlayerInfo.rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_AAC;
            }
            else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_LPCM)
            {
                _rPlayerInfo.rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_LPCM;
            }
            else
            {
                LOG(5,"_PLAYMGR_Cfg MEDIA_TYPE_TS unknown\n");
                return FALSE;
            }
            _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_MpgStart;
            _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_MpgStop;
            _rPlayerInfo.pfnPlayMgrClose = _PLAYMGR_Mp3Close;
            _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMp3MediaInfo;
        }
    }
    else
#endif
    if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
    {
        switch (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype)
        {
#ifndef CC_AVI_DISABLE
        case MEDIA_CONTNR_SUBTYPE_AVI:
            SWDMX_SetContainerType(SWDMX_FMT_AVI);
            // Avi just uses one range now.
            _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_AviStart;
            _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_AviStop;
            _rPlayerInfo.pfnPlayMgrClose = _PLAYMGR_AviClose;
            _rPlayerInfo.pfnPlayMgrPause = _PLAYMGR_AviPause;
            _rPlayerInfo.pfnPlayMgrResume = _PLAYMGR_AviResume;
            _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenAviMediaInfo;
            //_rPlayerInfo.pfnPlaymgrUpdateRange = _PLAYMGR_AviUpdateRange;
            //_rPlayerInfo.pfnPlaymgrFreeRange = _PLAYMGR_AviFreeRange;
            break;
#endif
#ifndef CC_WMV_DISABLE
        case MEDIA_CONTNR_SUBTYPE_ASF:
            SWDMX_SetContainerType(SWDMX_FMT_VC1_WMV);
            _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_WmvStart;
            _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_WmvStop;
            _rPlayerInfo.pfnPlayMgrClose = _PLAYMGR_WmvClose;
            //_rPlayerInfo.pfnPlayMgrPause = _PLAYMGR_WmvPause;
            //_rPlayerInfo.pfnPlayMgrResume = _PLAYMGR_WmvResume;
            _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenWmvMediaInfo;
            break;
#endif
#ifndef CC_MP4_DISABLE
        case MEDIA_CONTNR_SUBTYPE_MP4:
            SWDMX_SetContainerType(SWDMX_FMT_MP4);
            _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_Mp4Start;
            _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_Mp4Stop;
            _rPlayerInfo.pfnPlayMgrClose = _PLAYMGR_Mp4Close;
            _rPlayerInfo.pfnPlayMgrPause = _PLAYMGR_Mp4Pause;
            _rPlayerInfo.pfnPlayMgrResume = _PLAYMGR_Mp4Resume;
            _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMp4MediaInfo;
            _rPlayerInfo.pfnPlaymgrUpdateRange = _PLAYMGR_Mp4UpdateRange;
            _rPlayerInfo.pfnPlaymgrFreeRange = _PLAYMGR_Mp4FreeRange;
            break;
#endif
#ifndef CC_FLV_DISABLE
        case MEDIA_CONTNR_SUBTYPE_FLV:
           SWDMX_SetContainerType(SWDMX_FMT_FLV);
           // Flv just uses one range now.
           _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_FlvStart;
           _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_FlvStop;
           _rPlayerInfo.pfnPlayMgrClose = _PLAYMGR_FlvClose;
           _rPlayerInfo.pfnPlayMgrPause = _PLAYMGR_FlvPause;
           _rPlayerInfo.pfnPlayMgrResume = _PLAYMGR_FlvResume;
           _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenFlvMediaInfo;
           break;
#endif
#ifndef CC_MKV_DISABLE
        case MEDIA_CONTNR_SUBTYPE_MATROSKA:
            SWDMX_SetContainerType(SWDMX_FMT_MKV);
            _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_MkvStart;
            _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_MkvStop;
            _rPlayerInfo.pfnPlayMgrClose = _PLAYMGR_MkvClose;
            _rPlayerInfo.pfnPlayMgrPause = _PLAYMGR_MkvPause;
            _rPlayerInfo.pfnPlayMgrResume = _PLAYMGR_MkvResume;
            _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMkvMediaInfo;
            break;
#endif
#ifndef CC_RM_DISABLE
            case MEDIA_CONTNR_SUBTYPE_RM:
                SWDMX_SetContainerType(SWDMX_FMT_RM);
                _rPlayerInfo.pfnPlayMgrStart = _PLAYMGR_RmStart;
                _rPlayerInfo.pfnPlayMgrStop = _PLAYMGR_RmStop;
                _rPlayerInfo.pfnPlayMgrClose = _PLAYMGR_RmClose;
                _rPlayerInfo.pfnPlayMgrPause = _PLAYMGR_RmPause;
                _rPlayerInfo.pfnPlayMgrResume = _PLAYMGR_RmResume;
                _rPlayerInfo.pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenRmMediaInfo;
                break;
#endif
        default:
            LOG(0, "_PLAYMGR_Cfg false\n");
            return FALSE;
        }
    }
    SWDMX_SetRangeCb((VOID*)_PLAYMGR_UpdateRangeCb);
    return TRUE;
}

/**Terminate Mplayer
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Close(VOID)
{
    BOOL fgRet;
    LOG(5, "_PLAYMGR_Close\n");

    if (_rPlayerInfo.pfnPlayMgrClose)
    {
        fgRet = _rPlayerInfo.pfnPlayMgrClose((VOID*)&_rPlayerInfo);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Close pfnPlayMgrClose Fail\n");
            return FALSE;
        }
    }


    if (_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
    {
        _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_VIDEO, FALSE);
    }
    if (_rPlayerInfo.rPlayerStatus.fgEnAudStrm)
    {
        _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_AUDIO, FALSE);
    }
    if (_rPlayerInfo.rPlayerStatus.fgEnSubStrm)
    {
        _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_CAPTRACK, FALSE);
    }
    if (_rPlayerInfo.rPlayerStatus.fgEnPcrStrm)
    {
        _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_PCR, FALSE);
    }
    if (_u4AudHandle)
    {
        AUD_DeRegNotifyFunc(_u4AudHandle);
        _u4AudHandle = 0;
    }

    _rPlayerInfo.rPlayerStatus.u2EnPgmNum = 0;

   // FeederClose(eFeederSrc);
   // FeederClose(FEEDER_PARSING_SOURCE);

    x_memset(&_rPlayerInfo.FmtInfo, 0, sizeof(_rPlayerInfo.FmtInfo));

    x_memset(_rPlayerInfo.rRangeInfo, 0, MAX_RANGE_NUM * sizeof(MM_RANGE_INFO_T));

    _ePlayMgrSrcType = PLAYMGR_SRC_TYPE_HIGH_SPEED_STORAGE;
    _rPlayerInfo.rMediaFmt.e_media_type = MEDIA_TYPE_UNKNOWN;

#ifndef __KERNEL__
    x_fm_close(_hFileHandle);
    x_fm_close(_hHdrHandle);
#endif
    return TRUE;
}

/**Reset Mplayer
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Reset(VOID)
{
    return FALSE;
}

/**Set the stream settings to MPlayer
*@param MPlayer Handle
*@param pSettings
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Set(PLAYMGR_SETTING_T* prSettings)
{
    BOOL fgRet;

    if((_fgIsSeeking) &&
       (_ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }

    if (prSettings == NULL)
    {
        LOG(1, "_PLAYMGR_Set prSettings = NULL\n");
        return FALSE;
    }

    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PROGRAM)
    {
        if ((prSettings->u2PlayPgm < PLAYMGR_MAX_PROGRAM) &&
                (prSettings->u2PlayPgm < _rPlayerInfo.rPlayerStatus.u2TotalPrmNum))
        {
            _PLAYMGR_GetInfoByPgm(&_rPlayerInfo.rPlayerStatus, prSettings->u2PlayPgm);
        }
        else
        {
            LOG(1, "_PLAYMGR_Set fail, u2PlayPgm %d\n", prSettings->u2PlayPgm);
            return FALSE;
        }
    }

 #ifndef CC_WMV_DISABLE
    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_NETFLIX_STREAM)
    {
        if (_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_NETFLIX)
        {
            _PLAYMGR_WmvSetStreamIndex(&_rPlayerInfo,prSettings->u1NetflixVideoIdx,prSettings->u1NetflixAudioIdx);
        }
    }
 #endif

    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_VIDEO)
    {
        _rPlayerInfo.rPlayerStatus.fgPlayVidDecrypt = prSettings->fgVidDecrypt;
        _rPlayerInfo.rPlayerStatus.u4PlayVidKeyIdx = prSettings->u1VideoKeyIdx;
        _rPlayerInfo.rPlayerStatus.u4PlayVidIdx = prSettings->u2VideoPidIdx;
        _rPlayerInfo.rPlayerStatus.u4PlayVidStrmId = prSettings->u2VideoPid;
        _rPlayerInfo.rPlayerStatus.au4VideoId[0] = prSettings->u2VideoPid;
        _rPlayerInfo.rPlayerStatus.u2EnVTrackNum = 0;
        _rPlayerInfo.rPlayerStatus.u2VideoTracks = 1;
        _rPlayerInfo.rPlayerStatus.aeVidFormat[0] = prSettings->eVdecFmt;
        fgRet = _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_VIDEO, prSettings->fgVidEnable);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Set Fail to set video format info\n");
            return FALSE;
        }
    }

    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_AUDIO)
    {
        _rPlayerInfo.rPlayerStatus.fgPlayAudDecrypt = prSettings->fgAudDecrypt;
        _rPlayerInfo.rPlayerStatus.u4PlayAudKeyIdx = prSettings->u1AudioKeyIdx;
        _rPlayerInfo.rPlayerStatus.u4PlayAudIdx = prSettings->u2AudioPidIdx;
        _rPlayerInfo.rPlayerStatus.u4PlayAudStrmId = prSettings->u2AudioPid;
        _rPlayerInfo.rPlayerStatus.au4AudioId[0] = prSettings->u2AudioPid;
        _rPlayerInfo.rPlayerStatus.u2EnATrackNum = 0;
        _rPlayerInfo.rPlayerStatus.u2AudioTracks = 1;
        _rPlayerInfo.rPlayerStatus.aeAudFormat[0] = prSettings->eAdecFmt;
        fgRet = _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_AUDIO, prSettings->fgAudEnable);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Set Fail to set audio format info\n");
            return FALSE;
        }
    }

    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_PCR)
    {
        _rPlayerInfo.rPlayerStatus.u4PlayPcrIdx = prSettings->u2PcrPidIdx;
        _rPlayerInfo.rPlayerStatus.u4PlayPcrStrmId = prSettings->u2PcrPid;
        _rPlayerInfo.rPlayerStatus.au4PcrId[0] = prSettings->u2AudioPid;
        _rPlayerInfo.rPlayerStatus.u2EnPcrTrackNum = 0;
        _rPlayerInfo.rPlayerStatus.u2PcrTracks = 1;
        fgRet = _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_PVR_PCR, prSettings->fgPcrEnable);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Set Fail to set audio format info\n");
            return FALSE;
        }
    }

#ifdef TIME_SHIFT_SUPPORT
    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_CTRL_BLK)
    {
        x_memset(&_rPlayerInfo.FmtInfo.rPsInfo.rTickCtl, 0, sizeof(PVR_TICK_CTRL_BLK_T));
        _rPlayerInfo.FmtInfo.rPsInfo.rTickCtl.pv_start_address = prSettings->rTickCtl.pv_start_address;
        _rPlayerInfo.FmtInfo.rPsInfo.rTickCtl.pv_end_address = prSettings->rTickCtl.pv_end_address;
        _rPlayerInfo.FmtInfo.rPsInfo.rTickCtl.ui4_entry_num = prSettings->rTickCtl.ui4_entry_num;
        _rPlayerInfo.FmtInfo.rPsInfo.rTickCtl.ui4_entry_size = prSettings->rTickCtl.ui4_entry_size;
        _rPlayerInfo.FmtInfo.rPsInfo.rTickCtl.ui4_tick_period = prSettings->rTickCtl.ui4_tick_period;
        _rPlayerInfo.FmtInfo.rPsInfo.rTickCtl.ui4_lba_init = prSettings->rTickCtl.ui4_lba_init;
        _rPlayerInfo.FmtInfo.rPsInfo.rTickCtl.ui8_fifo_offset = prSettings->rTickCtl.ui8_fifo_offset;
        _rPlayerInfo.FmtInfo.rPsInfo.rTickCtl.ui4_fifo_pkt_num = prSettings->rTickCtl.ui4_fifo_pkt_num;
    }

    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_REC_RANGE)
    {
        x_memset(&_rPlayerInfo.FmtInfo.rPsInfo.rRangeInfo, 0, sizeof(MM_REC_BUF_RANGE_INFO_T));
        _rPlayerInfo.FmtInfo.rPsInfo.rRangeInfo.t_valid.ui4_start = prSettings->rRangeInfo.t_valid.ui4_start;
        _rPlayerInfo.FmtInfo.rPsInfo.rRangeInfo.t_valid.ui4_end = prSettings->rRangeInfo.t_valid.ui4_end;
        _rPlayerInfo.FmtInfo.rPsInfo.rRangeInfo.t_safe.ui4_start = prSettings->rRangeInfo.t_safe.ui4_start;
        _rPlayerInfo.FmtInfo.rPsInfo.rRangeInfo.t_safe.ui4_end = prSettings->rRangeInfo.t_safe.ui4_end;

        if(!SWDMX_SetInfo(eSWDMX_SET_REC_BUF_RANGE, (UINT32)&_rPlayerInfo.FmtInfo.rPsInfo.rRangeInfo, 0, 0))
        {
            LOG(3, "_PLAYMGR_Set PLAYMGR_INFO_MASK_PVR_REC_RANGE fail\n");
        }
    }
#endif

    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_LPCM_INFO)
    {
        if(!_PLAYMGR_SetLPCMInfo(prSettings->rLPCMInfo))
        {
            LOG(1, "_PLAYMGR_Set fail, _PLAYMGR_SetLPCMInfo\n");
            return FALSE;
        }
    }

    if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_PS) ||
            (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_TS) ||
            (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) ||
            (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES) ||
            (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER))
    {
        if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_VIDEO)
        {
            if ((prSettings->u2PlayVidTrack < PLAYMGR_MAX_VID_TRACK) &&
                    (prSettings->u2PlayVidTrack < _rPlayerInfo.rPlayerStatus.u2VideoTracks))
            {
                _rPlayerInfo.rPlayerStatus.u4PlayVidStrmId =
                    _rPlayerInfo.rPlayerStatus.au4VideoId[prSettings->u2PlayVidTrack];
                _rPlayerInfo.rPlayerStatus.u2EnVTrackNum = prSettings->u2PlayVidTrack;

                fgRet = _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_VIDEO, prSettings->fgVidEnable);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_Set Fail to set video format info\n");
                    return FALSE;
                }
            }
            else
            {
                LOG(1, "_PLAYMGR_Set fail, u2PlayVidTrack %d\n", prSettings->u2PlayVidTrack);
                return FALSE;
            }
        }
#ifndef __MODEL_slt__
        if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_AUDIO)
#else
        if (FALSE)
#endif
        {
            if ((prSettings->u2PlayAudTrack < PLAYMGR_MAX_AUD_TRACK) &&
                (prSettings->u2PlayAudTrack < _rPlayerInfo.rPlayerStatus.u2AudioTracks) &&
                (_rPlayerInfo.rPlayerStatus.aeAudFormat[prSettings->u2PlayAudTrack] != PLAYMGR_ADEC_UNKNOWN))
            {
                _rPlayerInfo.rPlayerStatus.u4PlayAudStrmId =
                    _rPlayerInfo.rPlayerStatus.au4AudioId[prSettings->u2PlayAudTrack];
                _rPlayerInfo.rPlayerStatus.u4PlayAudSubStrmId =
                    _rPlayerInfo.rPlayerStatus.au4AudioSubId[prSettings->u2PlayAudTrack];
                _rPlayerInfo.rPlayerStatus.u2EnATrackNum = prSettings->u2PlayAudTrack;
                fgRet = _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_AUDIO, prSettings->fgAudEnable);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_Set Fail to set audio format info\n");
                    return FALSE;
                }
            }
            else
            {
                LOG(1, "_PLAYMGR_Set fail, u2PlayAudTrack %d\n", prSettings->u2PlayAudTrack);
                return FALSE;
            }
        }
        if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_CAPTRACK)
        {
            if (prSettings->pu1SubtitleBuffer != NULL)
            {
                _pucSubOutBuf = prSettings->pu1SubtitleBuffer;
            }
            else
            {
                ASSERT(0);
            }
            if ((prSettings->u2PlaySubTrack < PLAYMGR_MAX_SUB_TRACK) &&
                    (prSettings->u2PlaySubTrack < _rPlayerInfo.rPlayerStatus.u2CaptionTracks))
            {
                _rPlayerInfo.rPlayerStatus.u4PlaySubId =
                    _rPlayerInfo.rPlayerStatus.au4CapTrackId[prSettings->u2PlaySubTrack];
                _rPlayerInfo.rPlayerStatus.u2EnCapTrackNum = prSettings->fgSubEnable;

                fgRet = _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_CAPTRACK, prSettings->fgSubEnable);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_Set Fail to set capture format info\n");
                    return FALSE;
                }
            }
            else
            {
                LOG(1, "_PLAYMGR_Set fail, u2PlaySubTrack %\n", prSettings->u2PlaySubTrack);
                return FALSE;
            }
        }
        if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PCR)
        {
            if ((prSettings->u2PlayPcrTrack < PLAYMGR_MAX_SUB_TRACK) &&
                    (prSettings->u2PlayPcrTrack < _rPlayerInfo.rPlayerStatus.u2PcrTracks))
            {
                _rPlayerInfo.rPlayerStatus.u4PlayPcrStrmId =
                    _rPlayerInfo.rPlayerStatus.au4PcrId[prSettings->u2PlayPcrTrack];
                _rPlayerInfo.rPlayerStatus.u2EnPcrTrackNum = prSettings->fgPcrEnable;

                fgRet = _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_PCR, prSettings->fgPcrEnable);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_Set Fail to set pcr format info\n");
                    return FALSE;
                }
            }
            else
            {
                LOG(1, "_PLAYMGR_Set fail, u2PlayPcrTrack %d\n", prSettings->u2PlayPcrTrack);
                return FALSE;
            }
        }
    }

    return TRUE;
}


/**Set the stream settings to MPlayer
*@param MPlayer Handle
*@param pSettings
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Get(PLAYMGR_SETTING_T* prSettings)
{
    if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_PS) ||
            (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_TS) ||
            (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER))
    {
        if (_rPlayerInfo.u2VidStmNum > 0)
        {
            prSettings->u2NumVideoTrack = _rPlayerInfo.u2VidStmNum;
        }
        if (_rPlayerInfo.u2AudStmNum > 0)
        {
            prSettings->u2NumAudioTrack = _rPlayerInfo.u2AudStmNum;
        }
        if (_rPlayerInfo.u2SubStmNum > 0)
        {
            prSettings->u2NumSubTrack = _rPlayerInfo.u2SubStmNum;
        }
    }

    return TRUE;
}


/**Start Play.
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
extern INT64 _FeederSetOffset(FeederSourceType eDataType, UINT64 u8Offset, UINT8 u1Whence, UINT32 u4ReqID);

BOOL _PLAYMGR_Start(VOID)
{
    BOOL fgRet;
    UINT32 u4Width,u4Height;
    UINT32 u4HeaderLen,u4HeaderData1;
    UINT32 u4CodecVersion;
    AUD_PCM_SETTING_T rPCMInfo;
    AUD_LPCM_INFO_T rLPCMInfo;            //for LPCM setting
    AUD_WMA_CFG_T rAudWmaCfg;
    UINT16* pu2AudioOption = NULL;
#if 0//ndef CC_RM_DISABLE
    UINT32 u4NumRprSize;  // for RV only
#endif
    BOOL fgCodecSupport = FALSE;
    //UINT8 u1index=0;
    VDEC_SEQUENCE_DATA_T rVDecSeqData;
    MINFO_TYPE_STM_ATRBT_T *pVidStrmAttr = NULL;
    MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;


    PLAYMGR_STATUS_T* prMediaStatus = &_rPlayerInfo.rPlayerStatus;
    //PLAYMGR_WMV_INFO_T* prWmvInfo = &(_rPlayerInfo.FmtInfo.rWmvInfo);
    UINT64 u8Numerator=0, u8Denominator=0, u8Fps = 0, u8Remainder=0;

    if((_fgIsSeeking) &&
       (_ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))

    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }

#ifndef PLAYMGR_ASYNC_MODE
    if(_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
    {
        LOG(3, "_PLAYMGR_Start already started\n");
        return TRUE;
    }
#endif

    LOG(5, "_PLAYMGR_Start\n");

    /*
    if(_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        _rPlayerInfo.rPlayerStatus.u8FileSize = _rPlayerInfo.u8FileSize
                    = (UINT64)_FeederSetOffset(FEEDER_PROGRAM_SOURCE,0,PLAYMGR_SEEK_END,0);
        _FeederSetOffset(FEEDER_PROGRAM_SOURCE,0,PLAYMGR_SEEK_BGN,0);
    }
    */

    if (prMediaStatus->u2EnVTrackNum >= (UINT16)PLAYMGR_MAX_VID_TRACK)
    {
        LOG(3, "PLAYMGR_MAX_VID_TRACK\n");
        prMediaStatus->u2EnVTrackNum = (UINT16)(PLAYMGR_MAX_VID_TRACK-1);
    }

    if (prMediaStatus->u2EnATrackNum >= (UINT16)PLAYMGR_MAX_AUD_TRACK)
    {
        LOG(3, "PLAYMGR_MAX_AUD_TRACK\n");
        prMediaStatus->u2EnATrackNum = (UINT16)(PLAYMGR_MAX_AUD_TRACK-1);
    }

    if (prMediaStatus->u2EnCapTrackNum >= (UINT16)PLAYMGR_MAX_SUB_TRACK)
    {
        LOG(3, "PLAYMGR_MAX_SUB_TRACK\n");
        prMediaStatus->u2EnCapTrackNum = (UINT16)(PLAYMGR_MAX_SUB_TRACK-1);
    }

    if (prMediaStatus->u2EnPcrTrackNum >= (UINT16)PLAYMGR_MAX_PCR_TRACK)
    {
        LOG(3, "PLAYMGR_MAX_PCR_TRACK\n");
        prMediaStatus->u2EnPcrTrackNum = (UINT16)(PLAYMGR_MAX_PCR_TRACK-1);
    }

    if (_ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
    {
        return _PLAYMGR_Resume();
    }

    if ((_arNotifyTbl[PLAYMGR_CALLBACK_ELAPSEDTIME]) &&
            (!_fgPlaymgrStatusNotifyTimerStarted))
    {
        VERIFY(x_timer_start (_hPlaymgrStatusNotifyTimer,
                              PLAYMGR_STATUS_UPDATE_TIMER,
                              X_TIMER_FLAG_REPEAT,
                              _PLAYMGR_StatusNfyTimer,
                              NULL) == OSR_OK);
        _fgPlaymgrStatusNotifyTimerStarted = TRUE;
    }

    SRM_SetMmMode(SRM_MM_MODE_TYPE_VIDEO);

    FeederStart(eFeederSrc);
    FeederSetRing(eFeederSrc, TRUE);

    if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
            (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
             == MEDIA_CONTNR_SUBTYPE_AVI))
    {
        if (_rPlayerInfo.FmtInfo.rAviInfo.u2VidIdx < 17)
        {
            pVidStrmAttr =
                &_rPlayerInfo.FmtInfo.rAviInfo.rStmAttribute[_rPlayerInfo.FmtInfo.rAviInfo.u2VidIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rAviInfo.u2VidIdx >= 17\n");
        }
        if (_rPlayerInfo.FmtInfo.rAviInfo.u2AudIdx < 17)
        {
            prAudStrmAttr =
                &_rPlayerInfo.FmtInfo.rAviInfo.rStmAttribute[_rPlayerInfo.FmtInfo.rAviInfo.u2AudIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rAviInfo.u2AudIdx >= 17\n");
        }
    }
    else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
            (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
             == MEDIA_CONTNR_SUBTYPE_FLV))
    {
        if (_rPlayerInfo.FmtInfo.rFlvInfo.u2VidIdx == 0)
        {
            pVidStrmAttr =&_rPlayerInfo.FmtInfo.rFlvInfo.rStmAttribute[_rPlayerInfo.FmtInfo.rFlvInfo.u2VidIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rFlvInfo.u2VidIdx!=1\n");
        }

        if (_rPlayerInfo.FmtInfo.rFlvInfo.u2AudIdx == 1)
        {
            prAudStrmAttr =&_rPlayerInfo.FmtInfo.rFlvInfo.rStmAttribute[_rPlayerInfo.FmtInfo.rFlvInfo.u2AudIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rFlvInfo.u2AudIdx !=1\n");
        }

    }
    else if((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
                _rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WMA)
    {
        if (_rPlayerInfo.FmtInfo.rWmvInfo.u2AudIdx < 17)
        {
            prAudStrmAttr =
                &_rPlayerInfo.FmtInfo.rWmvInfo.rStmAttribute[0][_rPlayerInfo.FmtInfo.rWmvInfo.u2AudIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rWmvInfo.u2AudIdx >= 17\n");
        }
    }
    else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
             ((_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_LPCM) ||
              (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WAVE) ||
              (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AIF) ||
              (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AAC) ||
              (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AC3)))

    {
        prAudStrmAttr =&_rPlayerInfo.FmtInfo.rMp3Info.rStreamAttr;
    }
    else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
             (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
              == MEDIA_CONTNR_SUBTYPE_ASF))
    {
        if ((_rPlayerInfo.FmtInfo.rWmvInfo.u2VidIdx < 17) &&
            (_rPlayerInfo.FmtInfo.rWmvInfo.u1VideoStreamIdx <10))
        {
            pVidStrmAttr =
                &_rPlayerInfo.FmtInfo.rWmvInfo.rStmAttribute[_rPlayerInfo.FmtInfo.rWmvInfo.u1VideoStreamIdx][_rPlayerInfo.FmtInfo.rWmvInfo.u2VidIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rWmvInfo.u2VidIdx >= 17\n");
        }
        if ((_rPlayerInfo.FmtInfo.rWmvInfo.u2AudIdx < 17)&&
            (_rPlayerInfo.FmtInfo.rWmvInfo.u1AudioStreamIdx<10))
        {
            prAudStrmAttr =
                &_rPlayerInfo.FmtInfo.rWmvInfo.rStmAttribute[_rPlayerInfo.FmtInfo.rWmvInfo.u1AudioStreamIdx][_rPlayerInfo.FmtInfo.rWmvInfo.u2AudIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rWmvInfo.u2AudIdx >= 17\n");
        }

    }
    else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
             (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
              == MEDIA_CONTNR_SUBTYPE_MATROSKA))
    {
        if (_rPlayerInfo.FmtInfo.rMkvInfo.u2VidIdx < 17)
        {
            pVidStrmAttr =
                &_rPlayerInfo.FmtInfo.rMkvInfo.rStmAttribute[_rPlayerInfo.FmtInfo.rMkvInfo.u2VidIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rMkvInfo.u2VidIdx >= 17\n");
        }
        if (_rPlayerInfo.FmtInfo.rMkvInfo.u2AudIdx < 17)
        {
            prAudStrmAttr =
                &_rPlayerInfo.FmtInfo.rMkvInfo.rStmAttribute[_rPlayerInfo.FmtInfo.rMkvInfo.u2AudIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rMkvInfo.u2AudIdx >= 17\n");
        }
    }
    else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
             (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
              == MEDIA_CONTNR_SUBTYPE_MP4))
    {
        if (prMediaStatus->u2VideoTracks > 0)
        {
            if (_rPlayerInfo.FmtInfo.rMp4Info.u2VidIdx < 3)
            {
                pVidStrmAttr =
                    &_rPlayerInfo.FmtInfo.rMp4Info.rStmAttribute[_rPlayerInfo.FmtInfo.rMp4Info.u2VidIdx];
            }
            else
            {
                LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rMkvInfo.u2VidIdx >= 17\n");
            }
        }

        if (prMediaStatus->u2AudioTracks > 0)
        {
            if (_rPlayerInfo.FmtInfo.rMp4Info.u2AudIdx < 3)
            {
                prAudStrmAttr =
                    &_rPlayerInfo.FmtInfo.rMp4Info.rStmAttribute[_rPlayerInfo.FmtInfo.rMp4Info.u2AudIdx];
            }
            else
            {
                LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rMp4Info.u2AudIdx >= 3\n");
            }

        }

        UNUSED(SWDMX_SetInfo(eSWDMX_SET_RANGECALLBACK, (UINT32)_PLAYMGR_UpdateRangeCb, 0, 0));
    }
    else if((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
        (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
        == MEDIA_CONTNR_SUBTYPE_RM))
    {
        if (prMediaStatus->u2VideoTracks > 0)
        {
            if(_rPlayerInfo.FmtInfo.rRmInfo.u2VidIdx < 3)
            {
                pVidStrmAttr =
                    &_rPlayerInfo.FmtInfo.rRmInfo.rStmAttribute[_rPlayerInfo.FmtInfo.rRmInfo.u2VidIdx];
            }
            else
            {
                LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rRmInfo.u2VidIdx >= 3\n");
            }
        }

        if (prMediaStatus->u2AudioTracks > 0)
        {
            if(_rPlayerInfo.FmtInfo.rRmInfo.u2AudIdx < 3)
            {
                prAudStrmAttr =
                    &_rPlayerInfo.FmtInfo.rRmInfo.rStmAttribute[_rPlayerInfo.FmtInfo.rRmInfo.u2AudIdx];
            }
            else
            {
                LOG(1, "_PLAYMGR_Start _rPlayerInfo.FmtInfo.rRmInfo.u2AudIdx >= 3\n");
            }

        }

        UNUSED(SWDMX_SetInfo(eSWDMX_SET_RANGECALLBACK, (UINT32)_PLAYMGR_UpdateRangeCb, 0, 0));
    }

    if (_rPlayerInfo.pfnPlayMgrStart == 0)
    {
        LOG(1, "_PLAYMGR_Start _rPlayerInfo.pfnPlayMgrStart == 0\n");
        return FALSE;
    }

    if ((_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AVI ||
            _rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AVI) &&
            _rPlayerInfo.rPlayerStatus.u2CaptionTracks > 0)
        _PLAYMGR_InitSubtitle();  // megaa 20090118: tmp solution

    if ((prMediaStatus->u2VideoTracks > 0) &&
            (prMediaStatus->fgEnVidStrm) &&
            (prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum] != PLAYMGR_VDEC_UNKNOWN))
    {
        if (VDP_GetStepForward(VDP_1) != 0)
        {
            VDP_SetFBNoTimeout(VDP_1, FALSE);
            VDP_SetStepForward(VDP_1, VDP_PLAY_NORMAL);
        }

        if(VDP_GetSlowForward(VDP_1) != 0)
        {
            VDP_SetFBNoTimeout(VDP_1, FALSE);
            VDP_SetSlowForward(VDP_1, VDP_PLAY_NORMAL);
        }

        VDP_SetPauseMm(VDP_1, FALSE);
        VDP_SetSpeed(VDP_1, STC_SPEED_TYPE_FORWARD_1X);

        VDP_SetTrickMode(VDP_PLAY_NORMAL);

        LOG(5, "aeVidFormat %d\n", prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum]);
        VDEC_PlayMM(ES0 , TRUE);

        switch (prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum])
        {
        case PLAYMGR_VDEC_DIVX311:
            fgRet = (BOOL)VDEC_Play(ES0, VDEC_FMT_MP4);
            DMX_SetVideoType(DMX_VIDEO_MPEG4);
            if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    ((_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_AVI) ||
                     (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_MATROSKA)))
            {
                ASSERT(pVidStrmAttr!=NULL);
                if (pVidStrmAttr!=NULL)
                {
                    if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                            == MEDIA_CONTNR_SUBTYPE_MATROSKA)
                    {
                        rVDecSeqData.u2_width = (UINT16)pVidStrmAttr->u.t_mkv_video_stm_attr.i4_width;
                        rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_mkv_video_stm_attr.i4_height;
                        u8Numerator =
                            (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator =
                            (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_scale;
                    }
                    else    // avi
                    {
                        rVDecSeqData.u2_width = (UINT16)pVidStrmAttr->u.t_avi_video_stm_attr.i4_width;
                        rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_avi_video_stm_attr.i4_height;

                        u8Numerator =
                            (UINT64)pVidStrmAttr->u.t_avi_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator =
                            (UINT64)pVidStrmAttr->u.t_avi_video_stm_attr.ui4_scale;
                    }
                }
                if (u8Numerator == 0 ||
                        u8Denominator == 0)
                {
                    rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_UNKNOWN;
                }
                else
                {
                    u8Fps = u8Div6432(u8Numerator, u8Denominator, &u8Remainder);
                    switch (u8Fps)
                    {
                    case 23970:
                    case 23976:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_23_976;
                        break;
                    case 24000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_24;
                        break;
                    case 25000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_25;
                        break;
                    case 29970:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_29_97;
                        break;
                    case 30000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_30;
                        break;
                    case 50000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_50;
                        break;
                    case 59940:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_59_94;
                        break;
                    case 60000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_60;
                        break;
                    default:
                        //rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_UNKNOWN;
                        _PLAYMGR_FRC(u8Fps, &rVDecSeqData);
                        break;
                    }
                }
                VDEC_SetMMParam(ES0, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            }
            SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_DX311, 0, 0);

            break;
        case PLAYMGR_VDEC_MPEG4:
        case PLAYMGR_VDEC_SORENSON:
            fgRet = (BOOL)VDEC_Play(ES0, VDEC_FMT_MP4);
            DMX_SetVideoType(DMX_VIDEO_MPEG4);
            if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    ((_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_AVI) ||
                     (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_MATROSKA)||
                     (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_FLV)))
            {
                ASSERT(pVidStrmAttr!=NULL);
                //rVDecSeqData.u2_width = (UINT16)pVidStrmAttr->u.t_avi_video_stm_attr.i4_width;
                //rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_avi_video_stm_attr.i4_height;
                if (pVidStrmAttr!=NULL)
                {
                    if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                            == MEDIA_CONTNR_SUBTYPE_MATROSKA)
                    {
                        rVDecSeqData.u2_width = (UINT16)pVidStrmAttr->u.t_mkv_video_stm_attr.i4_width;
                        rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_mkv_video_stm_attr.i4_height;
                        u8Numerator =
                            (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator =
                            (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_scale;
                    }
                    else if(_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                            == MEDIA_CONTNR_SUBTYPE_AVI)   // avi
                    {
                        u8Numerator =
                            (UINT64)pVidStrmAttr->u.t_avi_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator =
                            (UINT64)pVidStrmAttr->u.t_avi_video_stm_attr.ui4_scale;
                    }
                    else if(_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                            == MEDIA_CONTNR_SUBTYPE_FLV)   // flv
                    {
                        rVDecSeqData.u2_width  = (UINT16)pVidStrmAttr->u.t_flv_video_stm_attr.i4_width;
                        rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_flv_video_stm_attr.i4_height;

                        u8Numerator =(UINT64)pVidStrmAttr->u.t_flv_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator =1;

                    }
                }
                if (u8Numerator == 0 || u8Denominator == 0)
                {
                    rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_UNKNOWN;
                }
                else
                {
                    u8Fps = u8Div6432(u8Numerator, u8Denominator, &u8Remainder);
                    switch (u8Fps)
                    {
                    case 15000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_15;
                        break;
                    case 23000:
                    case 23970:
                    case 23976:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_23_976;
                        break;
                    case 24000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_24;
                        break;
                    case 25000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_25;
                        break;
                    case 29970:
                    case 29000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_29_97;
                        break;
                    case 30000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_30;
                        break;
                    case 50000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_50;
                        break;
                    case 59940:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_59_94;
                        break;
                    case 60000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_60;
                        break;
                    default:
                        //rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_UNKNOWN;
                        _PLAYMGR_FRC(u8Fps, &rVDecSeqData);
                        break;
                    }
                }

                VDEC_SetMMParam(ES0, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            }
            else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                     (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4))
            {
                ASSERT(pVidStrmAttr!=NULL);
                if ((pVidStrmAttr!=NULL) && (pVidStrmAttr->u.t_mp4i_video_stm_attr.ui4_codec_info_size != 0))
                {
                    SWDMX_SetInfo(eSWDMX_SET_VIDSEQHEADER,
                                  (UINT32)pVidStrmAttr->u.t_mp4i_video_stm_attr.pui1_codec_info,
                                  pVidStrmAttr->u.t_mp4i_video_stm_attr.ui4_codec_info_size,
                                  0);
                }
            }
            SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_MPEG4, 0, 0);
            break;
        case PLAYMGR_VDEC_MPEG1_MPEG2:
            fgRet = (BOOL)VDEC_Play(ES0, VDEC_FMT_MPV);
            DMX_SetVideoType(DMX_VIDEO_MPEG);
            SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_MPEG1_MPEG2, 0, 0);
            break;
        case PLAYMGR_VDEC_H264:
            fgRet = (BOOL)VDEC_Play(ES0, VDEC_FMT_H264);
            DMX_SetVideoType(DMX_VIDEO_H264);
            if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4))
            {
                ASSERT(pVidStrmAttr!=NULL);
                if ((pVidStrmAttr!=NULL) && (pVidStrmAttr->u.t_mp4i_video_stm_attr.ui4_codec_info_size != 0))
                {
                    SWDMX_SetInfo(eSWDMX_SET_VIDSEQHEADER,
                                  (UINT32)pVidStrmAttr->u.t_mp4i_video_stm_attr.pui1_codec_info,
                                  pVidStrmAttr->u.t_mp4i_video_stm_attr.ui4_codec_info_size,
                                  0);
                }
            }
            if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA))
            {
                ASSERT(pVidStrmAttr!=NULL);
                if ((pVidStrmAttr!=NULL) && (pVidStrmAttr->u.t_mp4i_video_stm_attr.ui4_codec_info_size != 0))
                {
                    SWDMX_SetInfo(eSWDMX_SET_VIDSEQHEADER,
                    (UINT32)pVidStrmAttr->u.t_mkv_video_stm_attr.pui1_codec_info,
                    pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_codec_info_size,
                    0);
                }
            }
            if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV))
            {
                ASSERT(pVidStrmAttr!=NULL);
                if ((pVidStrmAttr!=NULL) && (pVidStrmAttr->u.t_flv_video_stm_attr.ui4_codec_info_size != 0))
                {
                    SWDMX_SetInfo(eSWDMX_SET_VIDSEQHEADER,
                    (UINT32)pVidStrmAttr->u.t_flv_video_stm_attr.pui1_codec_info,
                    pVidStrmAttr->u.t_flv_video_stm_attr.ui4_codec_info_size,
                    0);
                }
            }
            SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_H264, 0, 0);
            break;
#ifndef CC_RM_DISABLE
        case PLAYMGR_VDEC_RV8:
        case PLAYMGR_VDEC_RV9:
            fgRet = (BOOL)VDEC_Play(ES0, VDEC_FMT_RV);
            DMX_SetVideoType(DMX_VIDEO_RV);
#if 0
            u4NumRprSize = 0;
#endif
            if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER &&
                _rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM)
            {
                if (pVidStrmAttr != NULL)
                {
#if 0 // do in _SetRmSpecInfo
                    rVDecSeqData.u2_width = (UINT16)pVidStrmAttr->u.t_rm_video_stm_attr.ui2_frm_width;
                    rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_rm_video_stm_attr.ui2_frm_height;
                    rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_VARIABLE;
                    u4NumRprSize = (pVidStrmAttr->u.t_rm_video_stm_attr.ui4_spo_extra_flags & 0x70000) >> 16;
                    if (u4NumRprSize > 0)  // It will be 0 for RV9.  For RV8, add the count of native size.
                        u4NumRprSize++;
                    //printf("Num of RPR size is %u\n", u4NumRprSize);
#endif

                SWDMX_SetInfo(
                    eSWDMX_SET_VIDSEQHEADER,
                    (UINT32)(&(_rPlayerInfo.FmtInfo.rRmInfo.rRmSpecInfo)),
                    0,
                    0);

                }
                else
                {
                    ASSERT(0);
                    return FALSE;
                }
            }
            else
            {
                ASSERT(0);
                return FALSE;
            }
#if 0 // do in _SetRmSpecInfo
            VDEC_SetMMParam(ES0, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            VDEC_SetMMParam(ES0, VDEC_MM_RV_RPR_INFO, u4NumRprSize, pVidStrmAttr->u.t_rm_video_stm_attr.ui4_specific, PLAYMGR_VDEC_RV9 == prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum]);
            SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_RV, 0, 0);
#endif
            break;
#endif
        case PLAYMGR_VDEC_WMV7:
        case PLAYMGR_VDEC_WMV8:
        case PLAYMGR_VDEC_WMV9:
        case PLAYMGR_VDEC_VC1:

            if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF))
            {
                ASSERT(pVidStrmAttr != NULL);
                if ( pVidStrmAttr != NULL)
                {
                    SeqHeaderInfo rSeqHeaderInfo;

                    u4CodecVersion = pVidStrmAttr->u.t_asf_video_stm_attr.e_enc;
                    u4Width = pVidStrmAttr->u.t_asf_video_stm_attr.i4_width;
                    u4Height = pVidStrmAttr->u.t_asf_video_stm_attr.i4_height;
                    u4HeaderLen = pVidStrmAttr->u.t_asf_video_stm_attr.ui4_codec_info_size;
                    u8Numerator = (UINT64)pVidStrmAttr->u.t_asf_video_stm_attr.ui4_rate * FPS_PREC;
                    u8Denominator = (UINT64)pVidStrmAttr->u.t_asf_video_stm_attr.ui4_scale;

                    rSeqHeaderInfo.u1Addr = pVidStrmAttr->u.t_asf_video_stm_attr.pui1_codec_info;
                    rSeqHeaderInfo.u4Len = u4HeaderLen;

#ifndef CC_WMV_DISABLE
                    fgCodecSupport = _PLAYMGR_GetWMVSupport(prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum],
                            &rSeqHeaderInfo);

                    if(fgCodecSupport == FALSE)
                    {
                        _PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_VIDEO,FALSE);
                        break;
                    }
#endif
                    UNUSED(fgCodecSupport);
                    UNUSED(rSeqHeaderInfo);


                    if (u8Numerator == 0 || u8Denominator == 0)
                    {
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_UNKNOWN;
                    }
                    else
                    {
                        u8Fps = u8Div6432(u8Numerator, u8Denominator, &u8Remainder);
                        switch (u8Fps)
                        {
                        case 23970:
                        case 23975:
                        case 23976:
                            rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_23_976;
                            break;
                        case 24000:
                            rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_24;
                            break;
                        case 25000:
                            rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_25;
                            break;
                        case 29970:
                            rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_29_97;
                            break;
                        case 30000:
                            rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_30;
                            break;
                        case 50000:
                            rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_50;
                            break;
                        case 59940:
                            rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_59_94;
                            break;
                        case 60000:
                            rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_60;
                            break;
                        default:
                            rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_UNKNOWN;
                            break;
                        }
                    }
                    if (u4HeaderLen <=5)
                    {
                        if (u4HeaderLen!=0)
                        {
                            x_memcpy(&u4HeaderData1,pVidStrmAttr->u.t_asf_video_stm_attr.pui1_codec_info,4);
                            u4HeaderData1 = ((u4HeaderData1&0x000000FF)<<24) | ((u4HeaderData1&0x0000FF00)<<8) | ((u4HeaderData1&0x00FF0000)>>8) | ((u4HeaderData1&0xFF000000)>>24);
                        }
                        else
                        {
                            u4HeaderData1 =0;
                        }
                    }
                    else
                    {
                        x_memcpy(&u4HeaderData1,pVidStrmAttr->u.t_asf_video_stm_attr.pui1_codec_info,4);
                        //x_memcpy(&u4HeaderData2,prWmvInfo->rWmvVideoStmAttribute[1].pui1_codec_info+4,4);
                    }

                    rVDecSeqData.u2_width = (UINT16)u4Width;
                    rVDecSeqData.u2_height = (UINT16)u4Height;
                    rVDecSeqData.u4SequenceHeader = u4HeaderData1;
                    rVDecSeqData.u2SequenceHeaderSize = u4HeaderLen;
                    VDEC_SetMMParam(ES0, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);

                    if (u4CodecVersion == MINFO_INFO_VID_ENC_WMV1)
                    {
                        SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_WMV7,0,0);
                    }
                    else if (u4CodecVersion == MINFO_INFO_VID_ENC_WMV2)
                    {
                        SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_WMV8,0,0);
                    }
                    else if (u4CodecVersion == MINFO_INFO_VID_ENC_WMV3)
                    {
                        SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_WMV9,0,0);
                    }
                    else if (u4CodecVersion == MINFO_INFO_VID_ENC_WVC1)
                    {
                        SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_VC1,0,0);
                        SWDMX_SetInfo(eSWDMX_SET_VIDSEQHEADER,(UINT32)(pVidStrmAttr->u.t_asf_video_stm_attr.pui1_codec_info),(UINT32)(pVidStrmAttr->u.t_asf_video_stm_attr.ui4_codec_info_size),0);
                    }
                    else
                    {
                        LOG(0,"error wmv type\n");
                    }
                }
            }

            fgRet = (BOOL)VDEC_Play(ES0, VDEC_FMT_WMV);
            DMX_SetVideoType(DMX_VIDEO_VC1);
            LOG(0,"PLAYMGR_VDEC_VC1\n");

            break;
        case PLAYMGR_VDEC_MJPEG:
            fgRet = (BOOL)VDEC_Play(ES0, VDEC_FMT_MJPEG);
            SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_MJPEG, 0, 0);

            if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                    == MEDIA_CONTNR_SUBTYPE_AVI))
            {
                ASSERT(pVidStrmAttr!=NULL);
                if (pVidStrmAttr!=NULL)
                {
                    rVDecSeqData.u2_width = (UINT16)pVidStrmAttr->u.t_avi_video_stm_attr.i4_width;
                    rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_avi_video_stm_attr.i4_height;

                    u8Numerator =
                        (UINT64)pVidStrmAttr->u.t_avi_video_stm_attr.ui4_rate * FPS_PREC;
                    u8Denominator =
                        (UINT64)pVidStrmAttr->u.t_avi_video_stm_attr.ui4_scale;
                }
                if (u8Numerator == 0 || u8Denominator == 0)
                {
                    rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_UNKNOWN;
                }
                else
                {
                    u8Fps = u8Div6432(u8Numerator, u8Denominator, &u8Remainder);
                    switch (u8Fps)
                    {
                    case 23970:
                    case 23976:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_23_976;
                        break;
                    case 24000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_24;
                        break;
                    case 25000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_25;
                        break;
                    case 29970:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_29_97;
                        break;
                    case 30000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_30;
                        break;
                    case 50000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_50;
                        break;
                    case 59940:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_59_94;
                        break;
                    case 60000:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_60;
                        break;
                    default:
                        rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_UNKNOWN;
                        break;
                    }
                }

                VDEC_SetMMParam(ES0, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            }
            break;
        default:
            return FALSE;
        }
    }

    // AV sync mode must be set before audio play
    _PLAYMGR_SetSyncMode();

    if ((prMediaStatus->u2AudioTracks > 0) &&
            (prMediaStatus->fgEnAudStrm) &&
            (prMediaStatus->aeAudFormat[prMediaStatus->u2EnATrackNum] != PLAYMGR_ADEC_UNKNOWN))
    {
        LOG(5, "aeAudFormat %d\n", prMediaStatus->aeAudFormat[prMediaStatus->u2EnATrackNum]);

        switch (prMediaStatus->aeAudFormat[prMediaStatus->u2EnATrackNum])
        {
        case PLAYMGR_ADEC_MP3:
#ifdef CC_53XX_AUDIO_V2
                AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#else
            AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#endif
            SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_MP3,0,0);
            break;
        case PLAYMGR_ADEC_AAC:
#ifdef CC_53XX_AUDIO_V2
                AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#else
            AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#endif
            if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4) &&
                    (prAudStrmAttr))
            {
                SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_AAC,
                              prAudStrmAttr->u.t_mp4i_audio_stm_attr.i4_sample_rate,
                              prAudStrmAttr->u.t_mp4i_audio_stm_attr.i2_channels);
            }
                else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                         (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM) && prAudStrmAttr)
                {
                    SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_AAC,
                        prAudStrmAttr->u.t_rm_audio_stm_attr.ui4_sample_rate,
                        prAudStrmAttr->u.t_rm_audio_stm_attr.ui2_num_channel);
                }
           else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                         (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV) && prAudStrmAttr)
            {
                    SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_AAC,
                        prAudStrmAttr->u.t_flv_audio_stm_attr.i4_samples_per_sec,
                        prAudStrmAttr->u.t_flv_audio_stm_attr.i2_channels);
            }
            else
            {
                SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_AAC,0,0);
            }
            break;
        case PLAYMGR_ADEC_AC3:
#ifdef CC_53XX_AUDIO_V2
                AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#else
            AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#endif
            SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_AC3,0,0);
            break;
        case PLAYMGR_ADEC_MPEG:
#ifdef CC_53XX_AUDIO_V2
                AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#else
            AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#endif
            SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_MPEG,0,0);
            break;
        case PLAYMGR_ADEC_PCM:
        case PLAYMGR_ADEC_ADPCM:
            x_memset((void*)&rLPCMInfo, 0, sizeof(rLPCMInfo));

            if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                      (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                       == MEDIA_CONTNR_SUBTYPE_FLV))
            {
                UINT32 u4SampleRate=0;
                UINT8  uPcmType=0;
                if(prAudStrmAttr==NULL)
                {
                    LOG(0,"prAudStrmAttr NULL\n");
                    break;
                }

                u4SampleRate=prAudStrmAttr->u.t_flv_audio_stm_attr.i4_samples_per_sec;
                uPcmType=prAudStrmAttr->u.t_flv_audio_stm_attr.e_enc;
                if(u4SampleRate==11000)
                   rPCMInfo.eSampleFreq=FS_11K;
                else if(u4SampleRate==22000)
                   rPCMInfo.eSampleFreq=FS_22K;
                else if(u4SampleRate==44100)
                   rPCMInfo.eSampleFreq=FS_44K;

                if(prAudStrmAttr->u.t_flv_audio_stm_attr.ui4_type)
                {
                    rLPCMInfo.ui2_num_ch = 2;
                }
                else
                {
                    rLPCMInfo.ui2_num_ch = 1;
                }

                rLPCMInfo.b_info_not_specific = 0;

                if(uPcmType==MINFO_INFO_AUD_ENC_PCM)
                {
                    rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                    rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                    rLPCMInfo.b_big_endian =0;
                }
                else if(uPcmType==MINFO_INFO_AUD_ENC_LPCM)
                {
                    rLPCMInfo.b_big_endian =0;
                    rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                    rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                }
                else if(uPcmType==MINFO_INFO_AUD_ENC_ADPCM)
                {
                    rLPCMInfo.b_big_endian =0;
                    rLPCMInfo.eLPcmType = LPCM_TYPE_MS_ADPCM;
                    rPCMInfo.ePcmDataInfo = PCM_MS_ADPCM_LITTLE_ENDIAN;
                }
            }
            else if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
            {
                if(prAudStrmAttr==NULL)
                {
                    LOG(0,"prAudStrmAttr NULL\n");
                    break;
                }

                if(_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
                {
                    rPCMInfo.eSampleFreq = _GetAudSamplingRate(prAudStrmAttr->u.t_avi_audio_stm_attr.i4_samples_per_sec);
                    rLPCMInfo.ui2_num_ch = prAudStrmAttr->u.t_avi_audio_stm_attr.i2_channels;
                    rLPCMInfo.ui2_bits_per_sample = prAudStrmAttr->u.t_avi_audio_stm_attr.i2_bits_per_sample;
                    rLPCMInfo.ui2_block_align = prAudStrmAttr->u.t_avi_audio_stm_attr.i2_block_align;
                }
                else if(_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
                {
                    rPCMInfo.eSampleFreq = _GetAudSamplingRate(prAudStrmAttr->u.t_asf_audio_stm_attr.i4_samples_per_sec);
                    rLPCMInfo.ui2_num_ch = prAudStrmAttr->u.t_asf_audio_stm_attr.i2_channels;
                    rLPCMInfo.ui2_bits_per_sample = prAudStrmAttr->u.t_asf_audio_stm_attr.i2_bits_per_sample;
                    rLPCMInfo.ui2_block_align = prAudStrmAttr->u.t_asf_audio_stm_attr.i2_block_align;
                }
                else if(_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
                {
                    rPCMInfo.eSampleFreq = _GetAudSamplingRate(prAudStrmAttr->u.t_mkv_audio_stm_attr.i4_samples_per_sec);
                    rLPCMInfo.ui2_num_ch = prAudStrmAttr->u.t_mkv_audio_stm_attr.i2_channels;
                    rLPCMInfo.ui2_bits_per_sample = prAudStrmAttr->u.t_mkv_audio_stm_attr.i2_bits_per_sample;
                    rLPCMInfo.ui2_block_align = prAudStrmAttr->u.t_mkv_audio_stm_attr.i2_block_align;
                }
                else
                {
                    LOG(3,"unknown e_contnr_subtype(%d)\n",
                        (UINT32)_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype);
                    break;
                }

                rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                rLPCMInfo.b_big_endian = 0;
                rLPCMInfo.b_info_not_specific = 0;
                rLPCMInfo.eSampleFreq = rPCMInfo.eSampleFreq;
                if(prMediaStatus->aeAudFormat[prMediaStatus->u2EnATrackNum] == PLAYMGR_ADEC_PCM)
                {
                    rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                }
                else
                {
                    rLPCMInfo.eLPcmType = LPCM_TYPE_MS_ADPCM;
                }
            }
            else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
                     (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype
                       == MEDIA_AUD_SUBTYPE_LPCM))
            {
                rPCMInfo.eSampleFreq = _GetAudSamplingRate(
                    prAudStrmAttr->u.t_es_audio_stm_attr.ui4_sampling_frequency);
                rLPCMInfo.ui2_num_ch =
                    prAudStrmAttr->u.t_es_audio_stm_attr.ui2_channels;
                rLPCMInfo.ui2_bits_per_sample =
                    prAudStrmAttr->u.t_es_audio_stm_attr.ui2_bits_per_sample;
                rLPCMInfo.ui2_block_align =
                    (rLPCMInfo.ui2_num_ch * rLPCMInfo.ui2_bits_per_sample);
                if(rLPCMInfo.ui2_block_align > 8)
                {
                    rLPCMInfo.ui2_block_align /= 8;
                }
                else
                {
                    rLPCMInfo.ui2_block_align = 1;
                }

                rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                rLPCMInfo.b_big_endian =
                    prAudStrmAttr->u.t_es_audio_stm_attr.b_big_endian;
                rLPCMInfo.b_info_not_specific = 0;
                rLPCMInfo.eSampleFreq = rPCMInfo.eSampleFreq;
                if((prAudStrmAttr->u.t_es_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_PCM) ||
                   (prAudStrmAttr->u.t_es_audio_stm_attr.e_enc == MINFO_INFO_AUD_ENC_LPCM))
                {
                    rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                }
                else
                {
                    rLPCMInfo.eLPcmType = LPCM_TYPE_MS_ADPCM;
                }
            }
            else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
                      ((_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype
                        == MEDIA_AUD_SUBTYPE_WAVE) ||
                        (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype
                        == MEDIA_AUD_SUBTYPE_AIF)))
            {
                UINT8  uPcmType=0;
                BOOL b_big_endian=FALSE;
                if(prAudStrmAttr==NULL)
                {
                    LOG(0,"prAudStrmAttr NULL\n");
                    break;
                }

                x_memset(&rLPCMInfo, 0, sizeof(rLPCMInfo));
                if(_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype
                        == MEDIA_AUD_SUBTYPE_WAVE)
                {
                    uPcmType=prAudStrmAttr->u.t_avi_audio_stm_attr.e_enc;

                    rPCMInfo.eSampleFreq = _GetAudSamplingRate(
                        prAudStrmAttr->u.t_avi_audio_stm_attr.i4_samples_per_sec);

                    rLPCMInfo.eSampleFreq = rPCMInfo.eSampleFreq;
                    rLPCMInfo.ui2_num_ch = prAudStrmAttr->u.t_avi_audio_stm_attr.i2_channels;
                    rLPCMInfo.ui2_block_align = prAudStrmAttr->u.t_avi_audio_stm_attr.i2_block_align;
                    rLPCMInfo.ui2_bits_per_sample = prAudStrmAttr->u.t_avi_audio_stm_attr.i2_bits_per_sample;
                }
                else if(_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype
                        == MEDIA_AUD_SUBTYPE_AIF)
                {
                    uPcmType=prAudStrmAttr->u.t_es_audio_stm_attr.e_enc;

                    rPCMInfo.eSampleFreq = _GetAudSamplingRate(
                        prAudStrmAttr->u.t_es_audio_stm_attr.ui4_sampling_frequency);

                    rLPCMInfo.eSampleFreq = rPCMInfo.eSampleFreq;
                    rLPCMInfo.ui2_num_ch = prAudStrmAttr->u.t_es_audio_stm_attr.ui2_channels;
                    rLPCMInfo.ui2_bits_per_sample = prAudStrmAttr->u.t_es_audio_stm_attr.ui2_bits_per_sample;
                    rLPCMInfo.ui2_block_align = (UINT16)(rLPCMInfo.ui2_num_ch * rLPCMInfo.ui2_bits_per_sample);
                    if(rLPCMInfo.ui2_block_align > 8)
                    {
                        rLPCMInfo.ui2_block_align /= 8;
                    }
                    else
                    {
                        rLPCMInfo.ui2_block_align = 1;
                    }
                    b_big_endian = prAudStrmAttr->u.t_es_audio_stm_attr.b_big_endian;
                }

                rLPCMInfo.b_info_not_specific = 0;
                rLPCMInfo.b_big_endian = b_big_endian;

                if(uPcmType==MINFO_INFO_AUD_ENC_PCM)
                {
                    rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                    rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                }
                else if(uPcmType==MINFO_INFO_AUD_ENC_LPCM)
                {
                    rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                    rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                }
                else if(uPcmType==MINFO_INFO_AUD_ENC_ADPCM)
                {
                    rLPCMInfo.eLPcmType = LPCM_TYPE_MS_ADPCM;
                    rPCMInfo.ePcmDataInfo = PCM_MS_ADPCM_LITTLE_ENDIAN;
                }
                else
                {
                    rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                    rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                }
            }
            else // PS file
            {
                // will change sample frequence in audio driver
                rPCMInfo.eSampleFreq = FS_48K;
                rPCMInfo.ePcmDataInfo = PCM_16_BITS_BIG_ENDIAN;
                // LPCM
                rLPCMInfo.b_big_endian = 1;
                rLPCMInfo.b_info_not_specific = 1;
                rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                rLPCMInfo.eSampleFreq = FS_48K;
                rLPCMInfo.ui2_num_ch = 2;

            }
#ifdef CC_53XX_AUDIO_V2
                AUD_PcmSetting(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, &rPCMInfo);
                AUD_PcmSetting(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, &rPCMInfo);
                AUD_LPcmSetting(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, &rLPCMInfo);
                AUD_LPcmSetting(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, &rLPCMInfo);
                AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_PCM);
#else
                AUD_PcmSetting(INTERNAL_DAC_ID_MAIN, &rPCMInfo);
                AUD_LPcmSetting(INTERNAL_DAC_ID_MAIN, &rLPCMInfo);
                AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_PCM);
#endif
                SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_PCM,0,0);
            break;
        case PLAYMGR_ADEC_WMA:
            //set audio option
            ASSERT(prAudStrmAttr != NULL);
            if(prAudStrmAttr != NULL)
            {
                pu2AudioOption = (UINT16*)(prAudStrmAttr->u.t_asf_audio_stm_attr.pui1_codec_info);
                rAudWmaCfg.u2HdrSrc = 1;
                rAudWmaCfg.u4Pktsz = (UINT32)prAudStrmAttr->u.t_asf_audio_stm_attr.i2_block_align;
                rAudWmaCfg.u2Numch = (UINT16)prAudStrmAttr->u.t_asf_audio_stm_attr.i2_channels;
                rAudWmaCfg.u4Sampersec = (UINT32)prAudStrmAttr->u.t_asf_audio_stm_attr.i4_samples_per_sec;
                //rAudWmaCfg.u4Bpersec = prWmvInfo->rWmvAudioStmAttribute[1].i2_bits_per_sample;
                rAudWmaCfg.u4Bpersec = prAudStrmAttr->u.t_asf_audio_stm_attr.ui4_avg_bytes_per_sec;
                rAudWmaCfg.u4Blocksz = (UINT32)prAudStrmAttr->u.t_asf_audio_stm_attr.i2_block_align;
                rAudWmaCfg.u2Encoderopt = pu2AudioOption[2];
                _AUD_DspWMAHeader(AUD_DEC_MAIN,&rAudWmaCfg);
#ifdef CC_53XX_AUDIO_V2
                AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#else
                AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#endif
                SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_WMA,0,0);
            }
            break;
#ifdef CC_MT5363
            case PLAYMGR_ADEC_RA:
#ifdef CC_53XX_AUDIO_V2
                AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#else
                AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#endif
                if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                         (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM) && prAudStrmAttr)
                {
                    UINT8 *pu1Ptr = (UINT8 *)prAudStrmAttr->u.t_rm_audio_stm_attr.pv_codec_opaque_data;
                    UINT8 u1RalbrMajorVer, u1RalbrMinorVer;
                    AUD_COOK_CFG_T rCookCfg;

                    _rCookInfo.u1FlavorID = prAudStrmAttr->u.t_rm_audio_stm_attr.ui2_flavor_idx;
                    ASSERT(_rCookInfo.u1FlavorID <= 29);
                    if (_rCookInfo.u1FlavorID <= 29)
                    {
                        _rCookInfo.u4SamplePerFrame = _au4CookSampPerFrame[_rCookInfo.u1FlavorID];
                    }
                    _rCookInfo.u1ChanNum = prAudStrmAttr->u.t_rm_audio_stm_attr.ui2_num_channel;
                    _rCookInfo.u4CookSampRate = prAudStrmAttr->u.t_rm_audio_stm_attr.ui4_sample_rate >> 16;
                    _rCookInfo.u2FrameSzInBytes = prAudStrmAttr->u.t_rm_audio_stm_attr.ui2_codec_frm_sz;

                    u1RalbrMajorVer = *pu1Ptr;
                    u1RalbrMinorVer = *(pu1Ptr + 3);
                    _rCookInfo.u2RegnNum = (*(pu1Ptr + 6) << 8) | (*(pu1Ptr + 7));
                    if (u1RalbrMajorVer >= 1 && u1RalbrMinorVer >= 3)
                    {
                        _rCookInfo.u2StartRegn = (*(pu1Ptr + 12) << 8) | (*(pu1Ptr + 13));
                        _rCookInfo.u2CplQBits= (*(pu1Ptr + 14) << 8) | (*(pu1Ptr + 15));
                    }
                    else
                    {
                        _rCookInfo.u2StartRegn = _rCookInfo.u2CplQBits = 0;
                    }

                    _rCookInfo.u2CookBlockPerSB = prAudStrmAttr->u.t_rm_audio_stm_attr.ui2_intlv_factor;
                    _rCookInfo.u2CookBlockSz = prAudStrmAttr->u.t_rm_audio_stm_attr.ui2_intlv_blk_sz;
                    _rCookInfo.u2CookFramePerBlock = _rCookInfo.u2CookBlockSz / _rCookInfo.u2FrameSzInBytes;
                    _rCookInfo.u2CookFramePerSB = _rCookInfo.u2CookFramePerBlock * _rCookInfo.u2CookBlockPerSB;
                    _rCookInfo.u2CookSBSz = _rCookInfo.u2CookBlockSz * _rCookInfo.u2CookBlockPerSB;

                    rCookCfg.u2ChanNum = _rCookInfo.u1ChanNum;
                    rCookCfg.u2CplQBits = _rCookInfo.u2CplQBits;
                    rCookCfg.u2FrameSzInBytes = _rCookInfo.u2FrameSzInBytes;
                    rCookCfg.u2RegnNum = _rCookInfo.u2RegnNum;
                    rCookCfg.u2SampPerFrame = _rCookInfo.u4SamplePerFrame;
                    rCookCfg.u2SampRate = _rCookInfo.u4CookSampRate;
                    rCookCfg.u2StartRegn = _rCookInfo.u2StartRegn;
                    AUD_DspCOOKParameter(AUD_DEC_MAIN, &rCookCfg);

                    SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE, ENUM_SWDMX_ADEC_RA, (UINT32)&_rCookInfo, 0);
                }
                else
                {
                    ASSERT(0);
                    SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_RA,0,0);
                }
                break;
#endif  // CC_MT5363
        default:
            return FALSE;
        }

        _PLAYMGR_AdecFlushCtrl();
        #ifdef CC_53XX_AUDIO_V2
        AUD_DSPCmdPlay(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
        AUD_DSPCmdPlay(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
        #else
        AUD_DSPCmdPlay(INTERNAL_DAC_ID_MAIN);
        #endif
        _PLAYMGR_AdecWaitCtrlDone();
        //AUD_DSPCmdPlayAsyn(INTERNAL_DAC_ID_MAIN);
        _rPlayerInfo.rPlayerStatus.fgAudPlayed = TRUE;
    }

	//FeederSetRing((FeederSourceType)_rPlayerInfo.rMediaFeeder.h_feeder, FALSE);
    fgRet = _rPlayerInfo.pfnPlayMgrStart((VOID*)&_rPlayerInfo);
    if (fgRet != TRUE)
    {
        LOG(1, "Fail to Start playback\n");
        return FALSE;
    }

    //settting av sync mode before playing audio
    //_PLAYMGR_SetSyncMode();

    prMediaStatus->fgGotVidEos = FALSE;
    prMediaStatus->fgGotAudEos = FALSE;
    prMediaStatus->fgSendEos = FALSE;
    fgRet = SWDMX_Play();
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_Start SWDMX_Play fail\n");
        return fgRet;
    }
    // do not set this, set start stc value on swdmx and vdec
    // start stc on vdec
    //_PLAYMGR_SetSTC(_rPlayerInfo.u4StartPts);

    return TRUE;
}

/**Stop MPlayer.
*@param handle
*@retval TRUE                      Success.
*@retval FALSE                     Fail.
*/
BOOL _PLAYMGR_Stop(VOID)
{
    //BOOL fgRet = TRUE;

#ifndef PLAYMGR_ASYNC_MODE
    if(_ePlayMgrCurStatus == PLAYMGR_STATE_STOP)
    {
        LOG(3, "_PLAYMGR_Stop already stopped\n");
        return TRUE;
    }
#endif

    LOG(5, "_PLAYMGR_Stop\n");

    if ((_arNotifyTbl[PLAYMGR_CALLBACK_ELAPSEDTIME]) &&
            (_fgPlaymgrStatusNotifyTimerStarted))
    {
        VERIFY(x_timer_stop(_hPlaymgrStatusNotifyTimer) == OSR_OK);
        _fgPlaymgrStatusNotifyTimerStarted = FALSE;
    }

    SRM_SetMmMode(SRM_MM_MODE_TYPE_OFF);

    if (_rPlayerInfo.pfnPlayMgrStop == 0)
    {
        LOG(1, "_PLAYMGR_Stop _rPlayerInfo.pfnPlayMgrStop == 0\n");
        return FALSE;
    }

    STC_StopStc();
    STC_SetStcValue((UINT32)-3000);

    _rPlayerInfo.rPlayerStatus.i4Speed = 1000;
    _rPlayerInfo.rPlayerStatus.i4PreviousSpeed = 1000;

    // Because AUD_DSPCmdStopAsyn will change audio fifo read/write pointer.
    // call SWDMX_Stop before that.
    // Or, maybe we could use other API but not AUD_DSPCmdStopAsyn.

    // Vdec also have the same reason as audio.
    _PLAYMGR_DmxFlushCtrl();
    if (!SWDMX_Stop())
    {
        LOG(1,"_PLAYMGR_Stop SWDMX_Pause fail\n");
    }
    _PLAYMGR_DmxWaitCtrlDone();

    if (_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
    {
        if (VDP_GetStepForward(VDP_1) != 0)
        {
            VDP_SetFBNoTimeout(VDP_1, FALSE);
            VDP_SetStepForward(VDP_1, VDP_PLAY_NORMAL);
        }

        if(VDP_GetSlowForward(VDP_1) != 0)
        {
            VDP_SetFBNoTimeout(VDP_1, FALSE);
            VDP_SetSlowForward(VDP_1, VDP_PLAY_NORMAL);
        }

        // Start -> Pause -> Stop: reset Pause status.
        VDP_SetPauseMm(B2R_1, FALSE);
        VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1X);
        VDP_SetTrickMode(VDP_PLAY_NORMAL);

        _PLAYMGR_VdecFlushCtrl();
        VDEC_Stop(ES0);
        _PLAYMGR_VdecWaitCtrlDone();
    }

    if (_rPlayerInfo.rPlayerStatus.fgEnAudStrm)
    {
        LOG(5, "_PLAYMGR_Stop AUD_DSPCmdStop\n");
        _PLAYMGR_AdecFlushCtrl();
#ifdef CC_53XX_AUDIO_V2
        AUD_DSPCmdStop(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
        AUD_DSPCmdStop(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
#else
        AUD_DSPCmdStop(INTERNAL_DAC_ID_MAIN);
#endif
        _PLAYMGR_AdecWaitCtrlDone();
        _rPlayerInfo.rPlayerStatus.fgAudPlayed = FALSE;
    }

    _rPlayerInfo.pfnPlayMgrStop((VOID*)&_rPlayerInfo);

    //FeederStop(eFeederSrc);

    _fgIsSeeking = FALSE;

    return TRUE;
}

/**Pause MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Pause(VOID)
{
    BOOL fgRet;
    LOG(5, "_PLAYMGR_Pause\n");

    if((_fgIsSeeking) &&
       (_ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }

    STC_StopStc();

    /*
    if((_arNotifyTbl[PLAYMGR_CALLBACK_ELAPSEDTIME]) &&
        (_fgPlaymgrStatusNotifyTimerStarted))
    {
        VERIFY(x_timer_stop(_hPlaymgrStatusNotifyTimer) == OSR_OK);
        _fgPlaymgrStatusNotifyTimerStarted = FALSE;
    }
    */

    if (_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
    {
        VDP_SetPauseMm(B2R_1, TRUE);
        // Vdec may block by VDP, we should not wait for Pause cmd.
        //_PLAYMGR_VdecFlushCtrl();
        VDEC_Pause(ES0);
        x_thread_delay(1);
        //_PLAYMGR_VdecWaitCtrlDone();
    }
    if (_rPlayerInfo.rPlayerStatus.fgEnAudStrm)
    {
        _PLAYMGR_AdecFlushCtrl();
#ifdef CC_53XX_AUDIO_V2
        AUD_DSPCmdPause(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
        AUD_DSPCmdPause(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
#else
        AUD_DSPCmdPause(INTERNAL_DAC_ID_MAIN);
#endif
        _PLAYMGR_AdecWaitCtrlDone();
    }

    _PLAYMGR_DmxFlushCtrl();
    if (!SWDMX_Pause())
    {
        LOG(1,"_PLAYMGR_Pause SWDMX_Pause fail\n");
    }
    _PLAYMGR_DmxWaitCtrlDone();

    if (_rPlayerInfo.pfnPlayMgrPause)
    {
        fgRet = _rPlayerInfo.pfnPlayMgrPause();
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Pause _rPlayerInfo.pfnPlayMgrPause == 0\n");
            return FALSE;
        }
    }

    return TRUE;
}

/**Resume MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Resume(VOID)
{
    PLAYMGR_STATUS_T* prMediaStatus = &_rPlayerInfo.rPlayerStatus;
    BOOL fgRet;

    LOG(5, "_PLAYMGR_Resume\n");

    if((_fgIsSeeking) &&
       (_ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }
    // We might in step, so swdmx are still running.
    _PLAYMGR_DmxFlushCtrl();
    if(!SWDMX_Pause())
    {
        LOG(1,"_PLAYMGR_Resume SWDMX_Pause fail\n");
    }
    _PLAYMGR_DmxWaitCtrlDone();

    /*
    if((_arNotifyTbl[PLAYMGR_CALLBACK_ELAPSEDTIME]) &&
        (!_fgPlaymgrStatusNotifyTimerStarted))
    {
        VERIFY(x_timer_start (_hPlaymgrStatusNotifyTimer,
                            PLAYMGR_STATUS_UPDATE_TIMER,
                            X_TIMER_FLAG_REPEAT,
                            _PLAYMGR_StatusNfyTimer,
                            NULL) == OSR_OK);
        _fgPlaymgrStatusNotifyTimerStarted = TRUE;
    }
    */

    if (prMediaStatus->u2EnVTrackNum >= (UINT16)PLAYMGR_MAX_VID_TRACK)
    {
        LOG(3, "PLAYMGR_MAX_VID_TRACK\n");
        prMediaStatus->u2EnVTrackNum = (UINT16)(PLAYMGR_MAX_VID_TRACK-1);
    }

    if (_rPlayerInfo.pfnPlayMgrResume)
    {
        fgRet = _rPlayerInfo.pfnPlayMgrResume((VOID*)&_rPlayerInfo);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Resume pfnPlayMgrResume Fail\n");
            return FALSE;
        }
    }

    fgRet = TRUE;
    if (prMediaStatus->fgEnVidStrm)
    {
        VDEC_PlayMM(ES0 , TRUE);
        switch (prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum])
        {
        case PLAYMGR_VDEC_MPEG4:
        case PLAYMGR_VDEC_DIVX311:
        case PLAYMGR_VDEC_SORENSON:
            fgRet = VDEC_Play(ES0, VDEC_FMT_MP4);
            break;
        case PLAYMGR_VDEC_MPEG1_MPEG2:
            fgRet = VDEC_Play(ES0, VDEC_FMT_MPV);
            break;
        case PLAYMGR_VDEC_H264:
            fgRet = VDEC_Play(ES0, VDEC_FMT_H264);
            break;
        case PLAYMGR_VDEC_WMV7:
        case PLAYMGR_VDEC_WMV8:
        case PLAYMGR_VDEC_WMV9:
        case PLAYMGR_VDEC_VC1:
            fgRet = VDEC_Play(ES0, VDEC_FMT_WMV);
            break;
        case PLAYMGR_VDEC_RV8:
        case PLAYMGR_VDEC_RV9:
            fgRet = VDEC_Play(ES0, VDEC_FMT_RV);
            break;
        case PLAYMGR_VDEC_MJPEG:
            fgRet = VDEC_Play(ES0, VDEC_FMT_MJPEG);
            break;
        default:
            return FALSE;
        }
    }
    if (!fgRet)
    {
        LOG(1, "_PLAYMGR_Resume VDEC_Play Fail\n");
        return FALSE;
    }

    if ((prMediaStatus->fgDisAudStrm) && (!prMediaStatus->fgEnAudStrm))
    {
        if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_AUDIO, TRUE))
        {
            LOG(1,"_PLAYMGR_CtlStm enable audio fail\n");
        }
    }

    // AV sync mode must be set before audio play
    _PLAYMGR_SetSyncMode();


    if ((prMediaStatus->fgEnAudStrm) || (prMediaStatus->fgDisAudStrm))
    {
#if 0
        if ((prMediaStatus->fgDisAudStrm) && (!prMediaStatus->fgEnAudStrm))
        {
            if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_AUDIO, TRUE))
            {
                LOG(1,"_PLAYMGR_CtlStm enable audio fail\n");
            }
        }
#endif

        if (!_rPlayerInfo.rPlayerStatus.fgAudPlayed)
        {
            _PLAYMGR_AdecFlushCtrl();
            // wait for audio API.
            //AUD_DrvSetDelay();
#ifdef CC_53XX_AUDIO_V2
            AUD_DSPCmdPlay(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
            AUD_DSPCmdPlay(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
#else
            AUD_DSPCmdPlay(INTERNAL_DAC_ID_MAIN);
#endif
            _PLAYMGR_AdecWaitCtrlDone();
            _rPlayerInfo.rPlayerStatus.fgAudPlayed = TRUE;
        }
        else
        {
#ifdef CC_53XX_AUDIO_V2
            AUD_DSPCmdResume(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
            AUD_DSPCmdResume(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
#else
            AUD_DSPCmdResume(INTERNAL_DAC_ID_MAIN);
#endif
        }
        prMediaStatus->fgDisAudStrm = FALSE;
        prMediaStatus->fgEnAudStrm = TRUE;
    }
    if ((prMediaStatus->fgEnPcrStrm) || (prMediaStatus->fgDisPcrStrm))
    {
        if ((prMediaStatus->fgDisPcrStrm) && (!prMediaStatus->fgEnPcrStrm))
        {
            if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_PCR, TRUE))
            {
                LOG(1,"_PLAYMGR_CtlStm enable Pcr fail\n");
            }
        }

        if (!_rPlayerInfo.rPlayerStatus.fgPcrPlayed)
        {
            _rPlayerInfo.rPlayerStatus.fgPcrPlayed = TRUE;
        }
        prMediaStatus->fgDisPcrStrm = FALSE;
        prMediaStatus->fgEnPcrStrm = TRUE;
    }

    //setting av sync mode before audio playing
    //_PLAYMGR_SetSyncMode();

    SWDMX_Play();

    if (prMediaStatus->fgEnVidStrm)
    {
        if (VDP_GetStepForward(VDP_1) != 0)
        {
            VDP_SetFBNoTimeout(VDP_1, FALSE);
            VDP_SetStepForward(VDP_1, VDP_PLAY_NORMAL);
        }

        if(VDP_GetSlowForward(VDP_1) != 0)
        {
            VDP_SetFBNoTimeout(VDP_1, FALSE);
            VDP_SetSlowForward(VDP_1, VDP_PLAY_NORMAL);
        }

        VDP_SetPauseMm(B2R_1, FALSE);
        VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1X);
        VDP_SetTrickMode(VDP_PLAY_NORMAL);
    }

    STC_StartStc();

    return TRUE;
}


/**Get the stream settings and status from MPlayer
*@param handle
*@param prSettings
*@param prStatus
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/

BOOL _PLAYMGR_GetInfo(PLAYMGR_STATUS_T *prStatus)
{
    BOOL fgRet = FALSE;
    LOG(5, "_PLAYMGR_GetInfo\n");

    // ToDo: why can clean info?
    _rPlayerInfo.u2VidStmNum = 0;
    _rPlayerInfo.u2AudStmNum = 0;
    _rPlayerInfo.u2SubStmNum = 0;
    _rPlayerInfo.rPlayerStatus.aeAudFormat[0] = PLAYMGR_ADEC_UNKNOWN;
    _rPlayerInfo.rPlayerStatus.aeVidFormat[0] = PLAYMGR_VDEC_UNKNOWN;
    _rPlayerInfo.rPlayerStatus.aeCapFormat[0] = PLAYMGR_CAPTION_UNKNOWN;
    _rPlayerInfo.rPlayerStatus.u2VideoTracks = 0;
    _rPlayerInfo.rPlayerStatus.u2AudioTracks = 0;
    _rPlayerInfo.rPlayerStatus.u2CaptionTracks = 0;
    _rPlayerInfo.rPlayerStatus.u4TotalDuration = 0;
    _rPlayerInfo.rPlayerStatus.u4PlayCap = 0;
    _rPlayerInfo.rPlayerStatus.u4CurPos = 0;

    if (_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_NETFLIX)
    {
        _rPlayerInfo.rPlayerStatus.u8FileSize = _rPlayerInfo.u8FileSize = (UINT64)(-1);
    }
    else if(_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_TIME_SHIFT)
    {
        _rPlayerInfo.rPlayerStatus.u8FileSize = _rPlayerInfo.u8FileSize = (UINT64)(-1);
    }
    else if(_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        _rPlayerInfo.rPlayerStatus.u8FileSize = _rPlayerInfo.u8FileSize = (UINT64)(-1);
    }
    else if(_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_VUDU)
    {
        _rPlayerInfo.rPlayerStatus.u8FileSize = _rPlayerInfo.u8FileSize = (UINT64)(-1);
    }
    else
    {
        _rPlayerInfo.rPlayerStatus.u8FileSize = _rPlayerInfo.u8FileSize
                                            = (UINT64)_FeederSetOffset(FEEDER_PROGRAM_SOURCE,0,PLAYMGR_SEEK_END,0);
        _FeederSetOffset(FEEDER_PROGRAM_SOURCE,0,PLAYMGR_SEEK_BGN,0);

    }

	FeederSetRing((FeederSourceType)_rPlayerInfo.rMediaFeeder.h_feeder, FALSE);
    FeederSetOffset((FeederSourceType)_rPlayerInfo.rMediaFeeder.h_feeder, (UINT64)0, PLAYMGR_SEEK_BGN, 0);

    x_memcpy((VOID *)prStatus, (VOID *)&(_rPlayerInfo.rPlayerStatus), sizeof(PLAYMGR_STATUS_T));

#ifndef __MODEL_slt__
    if (_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_NETFLIX)
    {
        _rPlayerInfo.rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        _rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_ASF;
    }
    else if (_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_TIME_SHIFT)
    {
        _rPlayerInfo.rMediaFmt.e_media_type = MEDIA_TYPE_TS;
        _rPlayerInfo.rMediaFmt.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_TIME_SHIFT;
    }
    else if(_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        _rPlayerInfo.rMediaFmt.e_media_type = MEDIA_TYPE_AUDIO_ES;
        _rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_LPCM;
    }
    else if(_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_VUDU)
    {
        _rPlayerInfo.rMediaFmt.e_media_type = MEDIA_TYPE_TS;
        _rPlayerInfo.rMediaFmt.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_188;
    }
    else if (_PLAYMGR_RecgFile(&_rPlayerInfo, _szFileName) != TRUE)
    {
        return FALSE;
    }
#else
    if (_u4SltContainerFormat == MEDIA_CONTNR_SUBTYPE_AVI)
    {
        _rPlayerInfo.rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        _rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_AVI;
    }
    else if (_u4SltContainerFormat == MEDIA_CONTNR_SUBTYPE_RM)
    {
        _rPlayerInfo.rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        _rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_RM;
    }
    else if (_u4SltContainerFormat == MEDIA_CONTNR_SUBTYPE_ASF)
    {
        _rPlayerInfo.rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        _rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_ASF;
    }
#endif

    if (!_PLAYMGR_Cfg())
    {
        return FALSE;
    }

    if(!_rPlayerInfo.pfnPlayMgrOpenMediaInfo)
    {
        return FALSE;
    }

	//FeederSetRing((FeederSourceType)_rPlayerInfo.rMediaFeeder.h_feeder, FALSE);
    fgRet = _rPlayerInfo.pfnPlayMgrOpenMediaInfo(prStatus, (VOID*)&_rPlayerInfo);
    if (!fgRet)
    {
        LOG(3, "_PLAYMGR_GetInfo pfnPlayMgrOpenMediaInfo Fail\n");
    }

    x_memcpy((VOID *)prStatus, (VOID *)&(_rPlayerInfo.rPlayerStatus), sizeof(PLAYMGR_STATUS_T));

    //FeederSetRing(FEEDER_PROGRAM_SOURCE, TRUE);
    return fgRet;
}


/**Get the stream settings and status from MPlayer
*@param handle
*@param prStatus
*@param u4Pgm
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_GetInfoByPgm(PLAYMGR_STATUS_T *prStatus, UINT32 u4Pgm)
{
    BOOL fgRet = FALSE;
    LOG(5, "_PLAYMGR_GetInfoByPgm\n");

    if (u4Pgm >= _rPlayerInfo.rPlayerStatus.u2TotalPrmNum)
    {
        LOG(3, "_PLAYMGR_GetInfoByPgm %d > %d\n", u4Pgm, _rPlayerInfo.rPlayerStatus.u2TotalPrmNum);
        return fgRet;
    }

    fgRet = _PLAYMGR_GetMpgMediaInfo((VOID*)&_rPlayerInfo, u4Pgm);
    if (!fgRet)
    {
        LOG(3, "_PLAYMGR_GetMpgMediaInfo Fail\n");
    }

    x_memcpy((VOID *)prStatus, (VOID *)&(_rPlayerInfo.rPlayerStatus), sizeof(PLAYMGR_STATUS_T));

    return fgRet;
}

/**Register the callback function of MPlayer
 Player can uses the registered callback function to notify the upper layer.
*@param handle
*@param eCallbackType
*@param callback funtion pointer
*@param reserved
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_RegCb(PLAYMGR_CALLBACK_TYPE_T eCallbackType,
                    PLAYMGR_CALLBACK pfnCallback,
                    UINT32 u4Param)
{
    LOG(5, "_PLAYMGR_RegCb\n");
    FeederInstallCallback(eFeederSrc, (FEEDER_NOTIFY_COND)eCallbackType,
                          (FEEDER_CALBACK)_PLAYMGR_FeederCb, u4Param);
    if (eCallbackType < PLAYMGR_CALLBACK_MAX)
    {
        _arNotifyTbl[eCallbackType]= pfnCallback;
    }
    else
    {
        LOG(5, "_PLAYMGR_RegCb unknow eCallbackType %d\n", eCallbackType);
    }
    return TRUE;
}

/**Set the seek position by time.
*@param handle
*@param seek time(millisecond)
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Speed(INT32 i4Speed)
{
    BOOL fgSetOK = TRUE;
    BOOL fgRet = TRUE;
    AUD_PCM_SETTING_T rPCMInfo;
    BOOL fgResetSwDmx = TRUE;
    PLAYMGR_STATUS_T* prMediaStatus = &_rPlayerInfo.rPlayerStatus;

    LOG(5, "_PLAYMGR_Speed\n");

    if (i4Speed == _rPlayerInfo.rPlayerStatus.i4Speed)
    {
        LOG(5, "same speed\n");
        return TRUE;
    }

    if(_fgIsSeeking)
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }

    if(!_PLAYMGR_GetCap(PLAYMGR_CAP_TRICKMODE, i4Speed, 0))
    {
        LOG(5,"_PLAYMGR_Speed fgTrick(0)\n");
        return FALSE;
    }

    if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
    {
        _PLAYMGR_DmxFlushCtrl();
        if (!SWDMX_Pause())
        {
            LOG(1,"_PLAYMGR_Speed SWDMX_Pause fail\n");
        }
        _PLAYMGR_DmxWaitCtrlDone();
    }

    //reset EOS flag if play to the end and than BF or
    //BF to the beginning and normal play
    //if(((i4Speed > 0) && (_rPlayerInfo.rPlayerStatus.i4Speed < 0)) ||
    //    ((i4Speed < 0) && (_rPlayerInfo.rPlayerStatus.i4Speed > 0)))
    {
        prMediaStatus->fgSendEos = FALSE;
    }

    if (i4Speed != 1000)
    {
        // ToDo: correct this.
        /*
        if (!(((_rPlayerInfo.rPlayerStatus.i4PreviousSpeed == 1000) ||
            (_rPlayerInfo.rPlayerStatus.i4PreviousSpeed == MM_SPEED_TYPE_PAUSE) ||
            (_rPlayerInfo.rPlayerStatus.i4PreviousSpeed == 1)) &&
            (_rPlayerInfo.rPlayerStatus.i4Speed == 2000)))
        {

            UNUSED(SWDMX_Flush());
        }*/

        if (_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
        {
            switch (i4Speed)
            {
            case 1024000:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1024X);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1024X, 0, 0);

            case 512000:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_512X);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_512X, 0, 0);

            case 256000:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_256X);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_256X, 0, 0);

            case 128000:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_128X);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_128X, 0, 0);

            case 64000:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_64X);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_64X, 0, 0);

            case 32000:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_32X);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_32X, 0, 0);

                break;
            case 16000:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_16X);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_16X, 0, 0);

                break;
            case 8000:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_8X);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_8X, 0, 0);

                break;
            case 4000:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_4X);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_4X, 0, 0);

                break;
            case 2000:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_2X);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_2X, 0, 0);

                break;
                /* Slow motion case */
            case 500:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1_DIV_2X);
                VDP_SetFBNoTimeout(VDP_1, TRUE);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_2X, 0, 0);
                VDP_SetSlowForward(VDP_1, VDP_PLAY_SLOW_FORWARD);
                break;
            case 250:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1_DIV_4X);
                VDP_SetFBNoTimeout(VDP_1, TRUE);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_4X, 0, 0);
                VDP_SetSlowForward(VDP_1, VDP_PLAY_SLOW_FORWARD);
                break;
            case 125:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1_DIV_8X);
                VDP_SetFBNoTimeout(VDP_1, TRUE);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_8X, 0, 0);
                VDP_SetSlowForward(VDP_1, VDP_PLAY_SLOW_FORWARD);
                break;
            case 62:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1_DIV_16X);
                VDP_SetFBNoTimeout(VDP_1, TRUE);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_16X, 0, 0);
                VDP_SetSlowForward(VDP_1, VDP_PLAY_SLOW_FORWARD);
                break;

            case -1024000:
            case -512000:
            case -256000:
            case -128000:
            case -64000:
            case -32000:
            case -16000:
            case -8000:
            case -4000:
            case -2000:
            case -1000:
            case -500:
            case -1:
                VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1X);
                VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_REWIND_1X, 0, 0);
                break;

            default:
                fgSetOK = FALSE;
                ASSERT(0);
                return FALSE;
            }

            VDP_SetTrickMode(VDP_PLAY_TRICK);

            if (_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
            {
                if ((_rPlayerInfo.rPlayerStatus.i4Speed == 1000) &&
                        (i4Speed == 2000))
                {
                    /* 1x -> 2x */
                    fgResetSwDmx = FALSE;
                }
                if ((_rPlayerInfo.rPlayerStatus.i4Speed == 1000) &&
                        ((i4Speed < 1000) && (i4Speed >= 62)))
                {
                    /* 1x -> Slow motion */
                    fgResetSwDmx = FALSE;
                }
                if (((_rPlayerInfo.rPlayerStatus.i4Speed < 1000) && (_rPlayerInfo.rPlayerStatus.i4Speed >= 62)) &&
                        ((i4Speed < 1000) && (i4Speed >= 62)))
                {
                    /* Slow motion -> Slow motion */
                    fgResetSwDmx = FALSE;
                }
                if (fgResetSwDmx)
                {
                    /* 1x -> 2x or Slow motion not to SWDMX ?! */
                    UNUSED(SWDMX_Flush());
                }
            }
        }
        else if (_rPlayerInfo.rPlayerStatus.fgEnAudStrm)
        {
            if(i4Speed > 0)
            {
                AUD_DspSpeed(INTERNAL_DAC_ID_MAIN, (i4Speed/1000));
            }
            else
            {
                UNUSED(SWDMX_Flush());

                if(_rPlayerInfo.rPlayerStatus.fgEnAudStrm)
                {
                    _PLAYMGR_AdecFlushCtrl();
                    // wait for audio API.
                    //AUD_DrvSetDelay();
#ifdef CC_53XX_AUDIO_V2
                     AUD_DSPCmdPlay(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
                     AUD_DSPCmdPlay(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
#else
                    AUD_DSPCmdPlay(INTERNAL_DAC_ID_MAIN);
#endif
                    _PLAYMGR_AdecWaitCtrlDone();
                }
                else
                {
                    LOG(1,"_PLAYMGR_Speed fgEnAudStrm false\n");
                }

                // Use 1000 to play -x speed audio
                AUD_DspSpeed(INTERNAL_DAC_ID_MAIN, 0);
            }
        }

        if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            if ((_rPlayerInfo.rPlayerStatus.fgEnAudStrm) &&
                    (_rPlayerInfo.rPlayerStatus.fgEnVidStrm))
            {
                if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_AUDIO, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
                }
                _rPlayerInfo.rPlayerStatus.fgEnAudStrm = FALSE;
                _rPlayerInfo.rPlayerStatus.fgDisAudStrm = TRUE;
                /*
                _PLAYMGR_AdecFlushCtrl();
#ifdef CC_53XX_AUDIO_V2
                AUD_DSPCmdStop(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
                AUD_DSPCmdStop(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
#else
                AUD_DSPCmdStop(INTERNAL_DAC_ID_MAIN);
#endif
                _PLAYMGR_AdecWaitCtrlDone();
                */
                _rPlayerInfo.rPlayerStatus.fgAudPlayed = FALSE;
            }
        }
        if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            if (_rPlayerInfo.rPlayerStatus.fgEnPcrStrm)
            {
                if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_PCR, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable pcr fail\n");
                }
                _rPlayerInfo.rPlayerStatus.fgEnPcrStrm = FALSE;
                _rPlayerInfo.rPlayerStatus.fgDisPcrStrm = TRUE;
                _rPlayerInfo.rPlayerStatus.fgPcrPlayed = FALSE;
            }
        }
    }
    else
    {
        // ToDo: correct this.
        /*
        if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
            (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype ==
             MEDIA_CONTNR_SUBTYPE_AVI) ||
             (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype ==
             MEDIA_CONTNR_SUBTYPE_MP4))
        {
            if (_rPlayerInfo.rPlayerStatus.i4PreviousSpeed != MM_SPEED_TYPE_PAUSE &&
                _rPlayerInfo.rPlayerStatus.i4PreviousSpeed != MM_SPEED_TYPE_FORWARD_2X &&
                _rPlayerInfo.rPlayerStatus.i4PreviousSpeed != MM_SPEED_TYPE_STEP)
            {
                UNUSED(SWDMX_Flush());
            }
        }
        else
        {
            if (_rPlayerInfo.rPlayerStatus.i4PreviousSpeed != MM_SPEED_TYPE_PAUSE)
            {
                UNUSED(SWDMX_Flush());
            }
        }*/

        if (_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
        {
            VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1X, 0, 0);

            VDP_SetTrickMode(VDP_PLAY_NORMAL);
            if ((_rPlayerInfo.rPlayerStatus.i4Speed < 1000) && (_rPlayerInfo.rPlayerStatus.i4Speed >= 62))
            {
                /* Slow Forward to Normal */
                if(VDP_GetSlowForward(VDP_1) != 0)
                {
                    VDP_SetFBNoTimeout(VDP_1, FALSE);
                    VDP_SetSlowForward(VDP_1, VDP_PLAY_NORMAL);
                }
            }

            if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
            {
                if ((_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI) ||
                        (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4) ||
                        (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA))
                {
                    if ((_rPlayerInfo.rPlayerStatus.i4Speed != MM_SPEED_TYPE_PAUSE) &&
                        (_rPlayerInfo.rPlayerStatus.i4Speed != 2000/*MM_SPEED_TYPE_FORWARD_2X*/) &&
                        (_rPlayerInfo.rPlayerStatus.i4Speed != 500) &&
                        (_rPlayerInfo.rPlayerStatus.i4Speed != 250) &&
                        (_rPlayerInfo.rPlayerStatus.i4Speed != 125) &&
                        (_rPlayerInfo.rPlayerStatus.i4Speed != 62))
                    {
                        UNUSED(SWDMX_Flush());
                    }
                }
                else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM)
                {
                    if ((_rPlayerInfo.rPlayerStatus.i4Speed != MM_SPEED_TYPE_PAUSE) &&
                            (_rPlayerInfo.rPlayerStatus.i4Speed != 2000/*MM_SPEED_TYPE_FORWARD_2X*/) &&
                            (_rPlayerInfo.rPlayerStatus.i4Speed != 500) &&
                            (_rPlayerInfo.rPlayerStatus.i4Speed != 250) &&
                            (_rPlayerInfo.rPlayerStatus.i4Speed != 125) &&
                            (_rPlayerInfo.rPlayerStatus.i4Speed != 62))
                    {
                        UNUSED(SWDMX_Flush());
                    }
                }
                else
                {
                    if ((_ePlayMgrCurStatus != PLAYMGR_STATE_PAUSE) ||
                        // previous is step
                        (_rPlayerInfo.rPlayerStatus.i4Speed != 1000))
                    {
                        UNUSED(SWDMX_Flush());
                    }
                }
            }
            else
            {
                if ((_ePlayMgrCurStatus != PLAYMGR_STATE_PAUSE) ||
                    // previous is step
                    (_rPlayerInfo.rPlayerStatus.i4Speed != 1000))
                {
                    UNUSED(SWDMX_Flush());
                }
            }
        }
        else if (_rPlayerInfo.rPlayerStatus.fgEnAudStrm)
        {
            AUD_DspSpeed(INTERNAL_DAC_ID_MAIN, 0);
        }

        if (fgSetOK)
        {
            _rPlayerInfo.rPlayerStatus.i4PreviousSpeed =
                _rPlayerInfo.rPlayerStatus.i4Speed;
            _rPlayerInfo.rPlayerStatus.i4Speed = i4Speed;
        }
        else
        {
            LOG(1, "_PLAYMGR_Speed SWDMX_SetSpeed fgSetOK Fail\n");
        }

        if (_rPlayerInfo.rPlayerStatus.fgDisAudStrm)
        {
            if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_AUDIO, TRUE))
            {
                LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
            }
            _rPlayerInfo.rPlayerStatus.fgEnAudStrm = TRUE;
        }

        // AV sync mode must be set before audio play
        _PLAYMGR_SetSyncMode();
        // speed should not start stc
        //STC_StartStc();

        if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            if (_rPlayerInfo.rPlayerStatus.fgDisAudStrm)
            {
#if 0
                if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_AUDIO, TRUE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
                }
                _rPlayerInfo.rPlayerStatus.fgEnAudStrm = TRUE;
#endif
                _rPlayerInfo.rPlayerStatus.fgDisAudStrm = FALSE;

                if (_rPlayerInfo.rPlayerStatus.u2EnATrackNum >= PLAYMGR_MAX_AUD_TRACK)
                {
                    LOG(1,"_PLAYMGR_CtlStm u2EnATrackNum >= PLAYMGR_MAX_AUD_TRACK\n");
                    _rPlayerInfo.rPlayerStatus.u2EnATrackNum = (UINT16)(PLAYMGR_MAX_AUD_TRACK-1);
                }

                LOG(5, "aeAudFormat %d\n", _rPlayerInfo.rPlayerStatus.aeAudFormat[_rPlayerInfo.rPlayerStatus.u2EnATrackNum]);

                switch (_rPlayerInfo.rPlayerStatus.aeAudFormat[_rPlayerInfo.rPlayerStatus.u2EnATrackNum])
                {
                case PLAYMGR_ADEC_MP3:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#else
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#endif
                    break;
                case PLAYMGR_ADEC_AC3:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#else
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#endif
                    break;
                case PLAYMGR_ADEC_AAC:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#else
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#endif
                    break;
                case PLAYMGR_ADEC_MPEG:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#else
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#endif
                    break;
                case PLAYMGR_ADEC_PCM:
                    if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                            (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                             == MEDIA_CONTNR_SUBTYPE_AVI))
                    {
                        // ToDo: set correct sample frequence
                        rPCMInfo.eSampleFreq = FS_48K;
                        rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                    }
                    else // PS file
                    {
                        // will change sample frequence in audio driver
                        rPCMInfo.eSampleFreq = FS_48K;
                        rPCMInfo.ePcmDataInfo = PCM_16_BITS_BIG_ENDIAN;
                    }
#ifdef CC_53XX_AUDIO_V2
                        AUD_PcmSetting(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, &rPCMInfo);
                        AUD_PcmSetting(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, &rPCMInfo);
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_PCM);
#else
                    AUD_PcmSetting(INTERNAL_DAC_ID_MAIN, &rPCMInfo);
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_PCM);
#endif
                    break;
                case PLAYMGR_ADEC_WMA:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#else
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#endif
                    break;
#ifdef CC_MT5363
                    case PLAYMGR_ADEC_RA:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#else
                        AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#endif
                        break;
#endif
                default:
                    return FALSE;
                }
                _PLAYMGR_AdecFlushCtrl();
                // wait for audio API.
                //AUD_DrvSetDelay();
#ifdef CC_53XX_AUDIO_V2
                AUD_DSPCmdPlay(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
                AUD_DSPCmdPlay(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
#else
                AUD_DSPCmdPlay(INTERNAL_DAC_ID_MAIN);
#endif
                _PLAYMGR_AdecWaitCtrlDone();

                AUD_DspSpeed(INTERNAL_DAC_ID_MAIN, 0);
                _rPlayerInfo.rPlayerStatus.fgAudPlayed = TRUE;
            }
            if (_rPlayerInfo.rPlayerStatus.fgDisPcrStrm)
            {
                if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_PCR, TRUE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable pcr fail\n");
                }
                _rPlayerInfo.rPlayerStatus.fgEnPcrStrm = TRUE;
                _rPlayerInfo.rPlayerStatus.fgDisPcrStrm = FALSE;
                _rPlayerInfo.rPlayerStatus.fgPcrPlayed = TRUE;
            }
        }
    }

    if (fgSetOK)
    {
        _rPlayerInfo.rPlayerStatus.i4PreviousSpeed =
            _rPlayerInfo.rPlayerStatus.i4Speed;
        _rPlayerInfo.rPlayerStatus.i4Speed = i4Speed;
    }
    else
    {
        LOG(1, "_PLAYMGR_Speed SWDMX_SetSpeed fgSetOK Fail\n");
    }

    //setting av sync mode before playing audio
    //_PLAYMGR_SetSyncMode();
    // speed should not start stc
    //STC_StartStc();

    if(i4Speed != 1000)
    {
        _PLAYMGR_SetSyncMode();
    }

    if (!SWDMX_SetSpeed(i4Speed))
    {
        LOG(1, "_PLAYMGR_Speed SWDMX_SetSpeed Fail\n");
    }

    if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
    {
        fgRet = SWDMX_Play();
    }

    return fgRet;
}


/**Get current speed
*@param
*@retval current speed
*/
INT32 _PLAYMGR_GetSpeed(VOID)
{
    return _rPlayerInfo.rPlayerStatus.i4Speed;
}

BOOL _PLAYMGR_GetDataExhausted(VOID)
{
    UINT16 u2VdecQueueSize =0;
    UINT16 u2VdecMaxQueueSize=0;
    static UINT8 u1DataExhaustedCount = 0;

    VDEC_GetQueueInfo(0,&u2VdecQueueSize,&u2VdecMaxQueueSize);
    if(u2VdecQueueSize < 30 && u1DataExhaustedCount !=0 )
    {
        u1DataExhaustedCount = 0;
        return TRUE;
    }
    else if(u2VdecQueueSize < 30 && u1DataExhaustedCount ==0)
    {
        u1DataExhaustedCount++;
        return FALSE;
    }
    return FALSE;
}

/**Flush a/v/swdmx.
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Flush(VOID)
{
    // Please make sure you call this function with swdmx/vdec/adec paused.
    if((_fgIsSeeking) &&
       (_ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }

    SWDMX_Flush();
    // audio is stopped while calling AUD_WaitDspFlush(INTERNAL_DAC_ID_MAIN)
    _rPlayerInfo.rPlayerStatus.fgAudPlayed = FALSE;

    return TRUE;
}

/**Set the seek position by time.
*@param handle
*@param seek time(millisecond)
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_SeekTime(UINT32 u4SeekTime)
{
    BOOL fgRet;
    UINT64 u8FilePos;

    _PLAYMGR_Flush();

    if (_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
    {
        if((_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_PS) ||
           (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS) ||
           (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_192) ||
           (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_ZERO_192) ||
           (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT) ||
           (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_ES) ||
           (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_H264_ES) ||
           (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG4_ES) ||
           (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VC1_ES) ||
           (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VCD) ||
           (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VOB) ||
           (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VRO))
        {
            VDEC_SetRenderFromFirstPic(ES0, TRUE, _rPlayerInfo.rPlayerStatus.u8FileSize);
        }
        /*else  //set in each container part
        {
            VDEC_SetRenderFromPts(ES0, u4SeekTime);
        }*/
        VDEC_RegRenderPtsCb(_PLAYMGR_SeekFinNotify);
        VDP_SetPauseMm(B2R_1, FALSE);
    }
    else
    {   // audio only or no a/v case.
        STC_StopStc();
        STC_SetStcValue((UINT32)(u4SeekTime*90));
        _PLAYMGR_SeekFinNotify(u4SeekTime*90);
        #if 1
        FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_SEEK_END, (u4SeekTime));
        #endif
    }

    if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT)
    {
        // SWDMX_SeekTime eat tick in time-shift
        fgRet = SWDMX_SeekTime(u4SeekTime, &u8FilePos);
    }
    else if (_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        UINT64 u8Remainder;
        MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;

        u8FilePos = (UINT64)u4SeekTime;
        prAudStrmAttr = &_rPlayerInfo.FmtInfo.rMp3Info.rStreamAttr;

        u8FilePos *= (UINT64)prAudStrmAttr->u.t_es_audio_stm_attr.ui4_sampling_frequency;
        u8FilePos *= (UINT64)prAudStrmAttr->u.t_es_audio_stm_attr.ui2_channels;

        u8FilePos *= 8;
        if(prAudStrmAttr->u.t_es_audio_stm_attr.ui2_bits_per_sample > 0)
        {
            //u8FilePos /= prAudStrmAttr->u.t_es_audio_stm_attr.ui2_bits_per_sample;
            u8FilePos = u8Div6432(u8FilePos,
                (UINT64)prAudStrmAttr->u.t_es_audio_stm_attr.ui2_bits_per_sample,
                &u8Remainder);

            fgRet = _PLAYMGR_SeekPosition(u8FilePos);
        }
        else
        {
            fgRet = FALSE;
        }
    }
    else
    {
        // SWDMX_SeekTime eat 90K time unit
        fgRet = SWDMX_SeekTime(u4SeekTime*90, &u8FilePos);
    }
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_SeekTime SWDMX_SeekTime fail\n");
        return fgRet;
    }

    return TRUE;
}


/**Set the seek position by bytes.
*@param handle
*@param seek bytes
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_SeekPosition(UINT64 u8SeekPos)
{
    BOOL fgRet;
    UINT32 u4Time;
    UINT64 u8Time;
    UINT64 u8Remainder;

    LOG(5, "_PLAYMGR_SeekPosition\n");
    _PLAYMGR_Flush();

    if (_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
    {
        VDEC_SetRenderFromFirstPic(ES0, TRUE, _rPlayerInfo.rPlayerStatus.u8FileSize);
        VDEC_RegRenderPtsCb(_PLAYMGR_SeekFinNotify);
        VDP_SetPauseMm(B2R_1, FALSE);
    }
    else if (_rPlayerInfo.rPlayerStatus.fgEnAudStrm)
    {   // audio only
        u4Time = 0;
        if (_rPlayerInfo.rPlayerStatus.u8FileSize)
        {
            if (_rPlayerInfo.rPlayerStatus.u8FileSize > (UINT64)0xFFFFFFFF)
            {
                LOG(0, "_PLAYMGR_SeekPosition FixMe\n");
            }
            u8Time = u8SeekPos;
            u8Time *= _rPlayerInfo.rPlayerStatus.u4TotalDuration;
            u4Time = u8Div6432(u8Time, _rPlayerInfo.rPlayerStatus.u8FileSize, &u8Remainder);
        }
        STC_StopStc();
        STC_SetStcValue((UINT32)(u4Time*90));
        _PLAYMGR_SeekFinNotify(u4Time*90);
        #if 1
        FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_SEEK_END, (u4Time));
        #endif

    }

    fgRet = SWDMX_SeekPos(u8SeekPos, u8SeekPos);
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_SeekPosition SWDMX_SeekPos fail\n");
        return fgRet;
    }

    return TRUE;
}

/**Set the seek position by time.
*@param handle
*@param seek time(millisecond)
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_SeekPercent(UINT32 u4SeekPercent)
{
    BOOL fgRet;
    UINT32 u4Time;
    UINT64 u8FilePos;
    UINT64 u8Time;
    UINT64 u8Remainder;

    LOG(5, "_PLAYMGR_SeekPercent\n");
    _PLAYMGR_Flush();

    if (_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
    {
        LOG(5, "_PLAYMGR_SeekPercent fgEnVidStrm\n");
        VDEC_SetRenderFromFirstPic(ES0, TRUE, _rPlayerInfo.rPlayerStatus.u8FileSize);
        VDEC_RegRenderPtsCb(_PLAYMGR_SeekFinNotify);
        VDP_SetPauseMm(B2R_1, FALSE);
    }
    else
    {   // audio only
        u4Time = 0;
        if (_rPlayerInfo.rPlayerStatus.u4TotalDuration)
        {
            u8Time = _rPlayerInfo.rPlayerStatus.u4TotalDuration;
            u8Time *= u4SeekPercent;
            u4Time = u8Div6432(u8Time, (UINT64)100, &u8Remainder);
        }
        STC_StopStc();
        STC_SetStcValue((UINT32)(u4Time*90));
        _PLAYMGR_SeekFinNotify(u4Time*90);
        #if 1
        FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_SEEK_END, (u4Time));
        #endif

    }

    LOG(5, "_PLAYMGR_SeekPercent SWDMX_SeekPercent\n");
    fgRet = SWDMX_SeekPercent(u4SeekPercent, &u8FilePos);
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_SeekPercent SWDMX_SeekPercent fail\n");
        return fgRet;
    }

    return TRUE;
}

BOOL _PLAYMGR_GetMetaData(PLAYMGR_AVMETA_T *prMeta)
{
    UINT64 u8TempRemainder;
    UINT16 u2I;
    prMeta->u8MediaSize = _rPlayerInfo.u8FileSize;
    if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
    {
        if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3)
        {
            prMeta->pu2Title = _rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.pwc_str_id3_title;
            prMeta->pu2Album = _rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.pwc_str_id3_album;
            prMeta->pu2Artist = _rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.pwc_str_id3_artist;
            prMeta->pu2Genre = _rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.pwc_str_id3_genre;
            prMeta->u4TitleLen = _rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.z_id3_title_len;
            prMeta->u4AlbumLen = _rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.z_id3_album_len;
            prMeta->u4ArtistLen = _rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.z_id3_artist_len;
            prMeta->u4GenreLen = _rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.z_id3_genre_len;
            prMeta->u4SampleFreq = _rPlayerInfo.FmtInfo.rMp3Info.rStreamAttr.u.t_mp3_audio_stm_attr.ui4_sample_rate;
            prMeta->u4BitRate = _rPlayerInfo.FmtInfo.rMp3Info.rStreamAttr.u.t_mp3_audio_stm_attr.ui4_bitrate;
            prMeta->u4FrameSize = _rPlayerInfo.FmtInfo.rMp3Info.rStreamAttr.u.t_mp3_audio_stm_attr.ui4_frame_length;
            prMeta->u8PlayTime = _rPlayerInfo.FmtInfo.rMp3Info.rPlayerGenericInfo.ui8_pb_duration;
            prMeta->u4ImgPos = prMeta->u4ImgSize = 0;
            for (u2I = 0; u2I < MINFO_MAX_ID3_PIC_NUM; u2I++)
            {
                if ((_rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.at_id3_attatched_pic[u2I].t_format.e_media_type
                        == MEDIA_TYPE_IMAGE) &&
                        (_rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.at_id3_attatched_pic[u2I].t_format.t_media_subtype.u.e_img_subtype
                         == MEDIA_IMG_SUBTYPE_JPG))
                {
                    prMeta->u4ImgPos =
                        _rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.at_id3_attatched_pic[u2I].ui4_starting_offset;
                    prMeta->u4ImgSize =
                        _rPlayerInfo.FmtInfo.rMp3Info.rMp3Id3Data.at_id3_attatched_pic[u2I].ui4_length;
                    break;
                }
            }
            //return TRUE;
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP2)
        {
            prMeta->u8PlayTime = _rPlayerInfo.rPlayerStatus.u4TotalDuration;
            //return TRUE;
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WMA)
        {
            //ToDo: wma add meta data in this scope
            prMeta->u8PlayTime = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvGenericInfo[0].ui8_pb_duration;
            prMeta->pu2Title = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_title;
            prMeta->u4TitleLen  = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.z_title_len;
            prMeta->pu2Artist = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_director;
            prMeta->u4ArtistLen = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.z_director_len;
            prMeta->pu2Genre = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_genre;
            prMeta->u4GenreLen = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.z_genre_len;
            //return TRUE;
        }
    }
    else if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
    {
        if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
        {
            prMeta->u8PlayTime = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvGenericInfo[0].ui8_pb_duration;
            prMeta->pu2Title = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_title;
            prMeta->u4TitleLen  = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.z_title_len;
            prMeta->pu2Artist = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_director;
            prMeta->u4ArtistLen = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.z_director_len;
            prMeta->pu2Genre = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_genre;
            prMeta->u4GenreLen = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.z_genre_len;
            prMeta->u4BitRate = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.ui4_bitrate;
            prMeta->u4FrameRate = _rPlayerInfo.FmtInfo.rWmvInfo.rWmvMetaData.t_frame_rate.ui4_rate;
            prMeta->u4Width = (UINT32)_rPlayerInfo.FmtInfo.rWmvInfo.rStmAttribute[0][0].u.t_asf_video_stm_attr.i4_width;
            prMeta->u4Height = (UINT32)_rPlayerInfo.FmtInfo.rWmvInfo.rStmAttribute[0][0].u.t_asf_video_stm_attr.i4_height;
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
        {
            PLAYMGR_AVI_INFO_T* prAviInfo = &_rPlayerInfo.FmtInfo.rAviInfo;

            prMeta->u4Width = (UINT32)prAviInfo->rStmAttribute[0].u.t_avi_video_stm_attr.i4_width;
            prMeta->u4Height = (UINT32)prAviInfo->rStmAttribute[0].u.t_avi_video_stm_attr.i4_height;

            prMeta->u8PlayTime = _rPlayerInfo.FmtInfo.rAviInfo.rAviMetaData.ui8_pb_duration;
            prMeta->pu2Title = _rPlayerInfo.FmtInfo.rAviInfo.rAviMetaData.pwc_str_title;
            prMeta->u4TitleLen  = _rPlayerInfo.FmtInfo.rAviInfo.rAviMetaData.z_title_len;
            prMeta->pu2Artist = _rPlayerInfo.FmtInfo.rAviInfo.rAviMetaData.pwc_str_director;
            prMeta->u4ArtistLen = _rPlayerInfo.FmtInfo.rAviInfo.rAviMetaData.z_director_len;
            prMeta->pu2Genre = _rPlayerInfo.FmtInfo.rAviInfo.rAviMetaData.pwc_str_genre;
            prMeta->u4GenreLen = _rPlayerInfo.FmtInfo.rAviInfo.rAviMetaData.z_genre_len;
            prMeta->u4BitRate = _rPlayerInfo.FmtInfo.rAviInfo.rAviMetaData.ui4_bitrate;
            prMeta->u4FrameRate = _rPlayerInfo.FmtInfo.rAviInfo.rAviMetaData.t_frame_rate.ui4_rate;
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
        {
            prMeta->u8PlayTime = _rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.ui8_pb_duration;
            prMeta->u4TitleLen = _rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.z_title_len;
            prMeta->pu2Title = _rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.pwc_str_title;
            prMeta->u4ArtistLen = _rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.z_director_len;
            prMeta->pu2Artist = _rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.pwc_str_director;
            prMeta->u4GenreLen = _rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.z_genre_len;
            prMeta->pu2Genre = _rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.pwc_str_genre;
            prMeta->u4BitRate = _rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.ui4_bitrate;
            prMeta->u4FrameRate = _rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.t_frame_rate.ui4_rate;
            prMeta->u4Width = (UINT32)_rPlayerInfo.FmtInfo.rMkvInfo.rStmAttribute[0].u.t_mkv_video_stm_attr.i4_width;
            prMeta->u4Height = (UINT32)_rPlayerInfo.FmtInfo.rMkvInfo.rStmAttribute[0].u.t_mkv_video_stm_attr.i4_height;

            //_rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.t_frame_rate.ui4_scale
            //_rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.z_copyright_len
            //_rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.pwc_str_copyright
            //_rPlayerInfo.FmtInfo.rMkvInfo.rMetaData.b_cr_date_exist
        }
        else if (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4)
        {
            PLAYMGR_MP4_INFO_T* prMp4Info = &_rPlayerInfo.FmtInfo.rMp4Info;

            prMeta->u4Width = (UINT32)prMp4Info->rStmAttribute[0].u.t_mp4i_video_stm_attr.i4_width;
            prMeta->u4Height = (UINT32)prMp4Info->rStmAttribute[0].u.t_mp4i_video_stm_attr.i4_height;

            prMeta->u8PlayTime = _rPlayerInfo.FmtInfo.rMp4Info.rMp4MetaData.ui8_pb_duration;
            prMeta->pu2Title = _rPlayerInfo.FmtInfo.rMp4Info.rMp4MetaData.pwc_str_title;
            prMeta->u4TitleLen  = _rPlayerInfo.FmtInfo.rMp4Info.rMp4MetaData.z_title_len;
            prMeta->pu2Artist = _rPlayerInfo.FmtInfo.rMp4Info.rMp4MetaData.pwc_str_director;
            prMeta->u4ArtistLen = _rPlayerInfo.FmtInfo.rMp4Info.rMp4MetaData.z_director_len;
            prMeta->pu2Genre = _rPlayerInfo.FmtInfo.rMp4Info.rMp4MetaData.pwc_str_genre;
            prMeta->u4GenreLen = _rPlayerInfo.FmtInfo.rMp4Info.rMp4MetaData.z_genre_len;
            prMeta->u4BitRate = _rPlayerInfo.FmtInfo.rMp4Info.rMp4MetaData.ui4_bitrate;
            prMeta->u4FrameRate = _rPlayerInfo.FmtInfo.rMp4Info.rMp4MetaData.t_frame_rate.ui4_rate;
        }
    }
    else if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
    {
        if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_ES)
        {
            prMeta->u8PlayTime = _rPlayerInfo.rPlayerStatus.u4TotalDuration;
            //return TRUE;
        }
        else if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_H264_ES)
        {
            prMeta->u8PlayTime = _rPlayerInfo.rPlayerStatus.u4TotalDuration;
            //return TRUE;
        }
        else if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VC1_ES)
        {
            prMeta->u8PlayTime = _rPlayerInfo.rPlayerStatus.u4TotalDuration;
            //return TRUE;
        }
        else if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG4_ES)
        {
            prMeta->u8PlayTime = _rPlayerInfo.rPlayerStatus.u4TotalDuration;
            //return TRUE;
        }
    }
    else if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_PS)
    {
        PLAYMGR_PS_INFO_T *prPsInfo = &_rPlayerInfo.FmtInfo.rPsInfo;
        prMeta->u8PlayTime = _rPlayerInfo.FmtInfo.rPsInfo.rPlayerGenericInfo.ui8_pb_duration;
        prMeta->u4Width = (UINT32)prPsInfo->rStmAttribute[0].u.t_ps_video_stm_attr.i4_width;
        prMeta->u4Height = (UINT32)prPsInfo->rStmAttribute[0].u.t_ps_video_stm_attr.i4_height;
        //return TRUE;
    }
    else if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_TS)
    {
        prMeta->u8PlayTime = _rPlayerInfo.rPlayerStatus.u4TotalDuration;
        //return TRUE;
    }
    else
    {
        LOG(3, "_PLAYMGR_GetMetaData unknown format\n");
        //ToDo: other media type add playtime , framerate, and bitrate at this scope
    }
    //pts -> millisecond
    prMeta->u8PlayTime = u8Div6432(prMeta->u8PlayTime, (UINT64)90, &u8TempRemainder);

    return TRUE;
}


BOOL _PLAYMGR_SetFifo(PLAYMGR_FIFO_T *prFifo)
{
    return TRUE;
}

UINT64 _PLAYMGR_GetBytePos(VOID)
{
    UINT64 u8CurPos;
    if (SWDMX_GetInfo(eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8CurPos, 0, 0))
    {
        return u8CurPos;
    }
    return 0;
}

UINT32 _PLAYMGR_GetPos(VOID)
{
    UINT32 u4Time;
    UINT64 u8CurPos;
    UINT64 u8Tmp;
    UINT64 u8Remainder;
    VDP_PIC_INFO_T rPicInfo;
    u4Time = 0;
    if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
    {
        u8CurPos = 0;
        if (VDP_GetPicInfo(B2R_1, &rPicInfo) == VDP_SET_OK)
        {
            u8CurPos = rPicInfo.u8Offset;
        }
        else if (SWDMX_GetInfo(eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8CurPos, 0, 0))
        {
        }
        //u4Time = (u8CurPos / _rPlayerInfo.u8FileSize) *
        //    _rPlayerInfo.u4TotalTime;
        if ((_rPlayerInfo.rPlayerStatus.u4TotalDuration > 0) &&
                (_rPlayerInfo.rPlayerStatus.u8FileSize > 0))
        {
            // 90000 -> 1000
            u8Tmp = u8Div6432(_rPlayerInfo.rPlayerStatus.u8FileSize * 90,
                              _rPlayerInfo.rPlayerStatus.u4TotalDuration,
                              &u8Remainder);
            if(u8Tmp > 0)
            {
                u4Time = (UINT32)u8Div6432(u8CurPos,
                                           (UINT32)u8Tmp, &u8Remainder);
            }
        }
    }
    else if(_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
    {
        #if 0
        UINT64 u8FileOffset;
        u8CurPos = 0;
        UNUSED(SWDMX_GetInfo(eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8CurPos, 0, 0));
        u8FileOffset = 0;
        UNUSED(SWDMX_GetInfo(eSWDMX_GET_FILE_OFFSET, (UINT32*)&u8FileOffset, 0, 0));
        if ((_rPlayerInfo.rPlayerStatus.u4TotalDuration > 0) &&
                (_rPlayerInfo.rPlayerStatus.u8FileSize > 0))
        {
            // 90000 -> 1000
            u8Tmp = u8Div6432((_rPlayerInfo.rPlayerStatus.u8FileSize - u8FileOffset) * 90000,
                              _rPlayerInfo.rPlayerStatus.u4TotalDuration,
                              &u8Remainder);
            if(u8Tmp > 0)
            {
                u4Time = 0;
                if(u8CurPos > u8FileOffset)
                {
                    u4Time = (UINT32)u8Div6432((u8CurPos - u8FileOffset) * 1000,
                                               (UINT32)u8Tmp, &u8Remainder);
                }
            }
        }
        #endif
        u4Time = 0;
        UNUSED(SWDMX_GetInfo(eSWDMX_GET_CURRENT_TIME, (UINT32*)&u4Time, 0, 0));
        if (u4Time > (0xFFFFFFFF - 90000)) // -1 sec
        {
            u4Time = 0;
        }
        else
        {
            u4Time /= 90;
        }
        if(_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
        {
            UINT64 u8Remainder;
            MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;
            prAudStrmAttr =&_rPlayerInfo.FmtInfo.rMp3Info.rStreamAttr;
            UNUSED(SWDMX_GetInfo(eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8CurPos, 0, 0));

            if((prAudStrmAttr->u.t_es_audio_stm_attr.ui4_sampling_frequency > 0) &&
               (prAudStrmAttr->u.t_es_audio_stm_attr.ui2_channels))
            {
                u8CurPos *= prAudStrmAttr->u.t_es_audio_stm_attr.ui2_bits_per_sample;

                //u8CurPos /= 8;
                //u8CurPos /= prAudStrmAttr->u.t_es_audio_stm_attr.ui4_sampling_frequency;
                //u8CurPos /= prAudStrmAttr->u.t_es_audio_stm_attr.ui2_channels;
                u8CurPos = u8Div6432(u8CurPos,
                    (UINT64)8,
                    &u8Remainder);
                u8CurPos = u8Div6432(u8CurPos,
                    (UINT64)prAudStrmAttr->u.t_es_audio_stm_attr.ui4_sampling_frequency,
                    &u8Remainder);
                u8CurPos = u8Div6432(u8CurPos,
                    (UINT64)prAudStrmAttr->u.t_es_audio_stm_attr.ui2_channels,
                    &u8Remainder);
                u4Time = (UINT32)u8CurPos;
            }
        }
        else if(_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_RHAPSODY)
        {
            u4Time = STC_GetStcValue();
            if (u4Time > (0xFFFFFFFF - 90000)) // -1 sec
            {
                u4Time = 0;
            }
            else
            {
                u4Time /= 90;
            }
        }
    }
    else
    {
        if (((_rPlayerInfo.rPlayerStatus.fgEnVidStrm) && (MPV_IsDisplay(ES0))) ||
                (!_rPlayerInfo.rPlayerStatus.fgEnVidStrm)) // audio only
        {
            // Getting Pts from VDP for 4X/8X/16X/32X or -2X/-4X/-8X/-16X/-32X
            if ((_rPlayerInfo.rPlayerStatus.i4Speed > 2000 ||_rPlayerInfo.rPlayerStatus.i4Speed < 0) &&
                    (_rPlayerInfo.rPlayerStatus.fgEnVidStrm))
            {
                VDP_PIC_INFO_T rVdpPicInfo = {0};

                if (VDP_GetPicInfo(B2R_1, &rVdpPicInfo) != VDP_SET_OK)
                {
                    return 0;
                }

                u4Time = (rVdpPicInfo.u4Pts/90);
            }
            else
            {
                u4Time = STC_GetStcValue();
                if (u4Time > (0xFFFFFFFF - 90000)) // -1 sec
                {
                    u4Time = 0;
                }
                else
                {
                    u4Time /= 90;
                }
            }
        }
        else
        {
            u4Time = 0;
        }
    }
    LOG(6, "_PLAYMGR_GetPos %d\n", u4Time);
    return u4Time;
}

UINT32 _PLAYMGR_GetTick(VOID)
{
    UINT32 u4Tick;
    VDP_PIC_INFO_T rPicInfo;

    u4Tick = 0;
    if (VDP_GetPicInfo(B2R_1, &rPicInfo) == VDP_SET_OK)
    {
        u4Tick = rPicInfo.u4TickNum;
    }
    else if (SWDMX_GetInfo(eSWDMX_GET_CURRENT_TICK, (UINT32*)&u4Tick, 0, 0))
    {
        //u4Tick
    }
    LOG(6, "_PLAYMGR_GetTick %d\n", u4Tick);
    return u4Tick;
}

BOOL _PLAYMGR_CtlStm(UINT32 u4InfoValue, BOOL fgEnable)
{
    BOOL fgRet = TRUE;
    AUD_PCM_SETTING_T rPCMInfo;
    AUD_LPCM_INFO_T rLPCMInfo;            //for LPCM setting
    PLAYMGR_STATUS_T* prMediaStatus = &_rPlayerInfo.rPlayerStatus;

    if((_fgIsSeeking) &&
       (_ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }

    x_memset(&rPCMInfo,0,sizeof(AUD_PCM_SETTING_T));
    if (u4InfoValue == PLAYMGR_INFO_MASK_VIDEO)
    {
        if (fgEnable)
        {
            if (_rPlayerInfo.rPlayerStatus.u2EnVTrackNum >= (UINT16)PLAYMGR_MAX_VID_TRACK)
            {
                LOG(1,"_PLAYMGR_CtlStm u2EnVTrackNum >= PLAYMGR_MAX_VID_TRACK\n");
                _rPlayerInfo.rPlayerStatus.u2EnVTrackNum = (UINT16)(PLAYMGR_MAX_VID_TRACK-1);
            }

            // handle video track change
            if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                VDEC_PlayMM(ES0 , TRUE);
                switch(_rPlayerInfo.rPlayerStatus.i4Speed)
                {
                    case 1024000:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1024X, 0, 0);
                        break;
                    case 512000:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_512X, 0, 0);
                        break;
                    case 256000:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_256X, 0, 0);
                        break;
                    case 128000:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_128X, 0, 0);
                        break;
                    case 64000:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_64X, 0, 0);
                        break;
                    case 32000:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_32X, 0, 0);
                        break;
                    case 16000:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_16X, 0, 0);
                        break;
                    case 8000:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_8X, 0, 0);
                        break;
                    case 4000:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_4X, 0, 0);
                        break;
                    case 2000:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_2X, 0, 0);
                        break;
                        /* Slow motion case */
                    case 500:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_2X, 0, 0);
                        break;
                    case 250:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_4X, 0, 0);
                        break;
                    case 125:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_8X, 0, 0);
                        break;
                    case 62:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_16X, 0, 0);
                        break;

                    case -1024000:
                    case -512000:
                    case -256000:
                    case -128000:
                    case -64000:
                    case -32000:
                    case -16000:
                    case -8000:
                    case -4000:
                    case -2000:
                    case -1000:
                    case -500:
                    case -1:
                        VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_REWIND_1X, 0, 0);
                        break;
                    default:
                        break;
                }
                _PLAYMGR_SetSyncMode();
                switch (prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum])
                {
                case PLAYMGR_VDEC_MPEG4:
                case PLAYMGR_VDEC_DIVX311:
                    fgRet = VDEC_Play(ES0, VDEC_FMT_MP4);
                    DMX_SetVideoType(DMX_VIDEO_MPEG4);
                    break;
                case PLAYMGR_VDEC_MPEG1_MPEG2:
                    fgRet = VDEC_Play(ES0, VDEC_FMT_MPV);
                    DMX_SetVideoType(DMX_VIDEO_MPEG);
                    break;
                case PLAYMGR_VDEC_H264:
                    fgRet = VDEC_Play(ES0, VDEC_FMT_H264);
                    DMX_SetVideoType(DMX_VIDEO_H264);
                    break;
                case PLAYMGR_VDEC_WMV7:
                case PLAYMGR_VDEC_WMV8:
                case PLAYMGR_VDEC_WMV9:
                case PLAYMGR_VDEC_VC1:
                    fgRet = VDEC_Play(ES0, VDEC_FMT_WMV);
                    DMX_SetVideoType(DMX_VIDEO_VC1);
                    break;
                case PLAYMGR_VDEC_RV8:
                case PLAYMGR_VDEC_RV9:
                    fgRet = VDEC_Play(ES0, VDEC_FMT_RV);
                    break;
                case PLAYMGR_VDEC_MJPEG:
                    fgRet = VDEC_Play(ES0, VDEC_FMT_MJPEG);
                    break;
                default:
                    LOG(1,"_PLAYMGR_CtlStm unknown aeVidFormat(%d)\n",
                        (prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum]));
                    return FALSE;
                }
            }

            fgRet = SWDMX_SetInfo(eSWDMX_SET_VIDEOTYPE,
                                  _PLAYMGR_PlaymgrVdec2Swdmx(
                                      _rPlayerInfo.rPlayerStatus.aeVidFormat[_rPlayerInfo.rPlayerStatus.u2EnVTrackNum]),
                                  0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set video format info\n");
                return FALSE;
            }

#ifdef TIME_SHIFT_SUPPORT
            if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT)
            {
                if(_rPlayerInfo.rPlayerStatus.fgPlayVidDecrypt)
                {
                    fgRet = SWDMX_SetInfo(eSWDMX_SET_VID_PID_IDX,
                                            _rPlayerInfo.rPlayerStatus.u4PlayVidIdx,
                                            _rPlayerInfo.rPlayerStatus.u4PlayVidKeyIdx,
#ifdef CC_MT5387
                                            (UINT32)DMX_DESC_MODE_AES_ECB);
#else
                                            (UINT32)DMX_DESC_MODE_3DES_ECB);
#endif

                }
                else
                {
                    fgRet = SWDMX_SetInfo(eSWDMX_SET_VID_PID_IDX,
                                            _rPlayerInfo.rPlayerStatus.u4PlayVidIdx,
                                            0, 0);
                }
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_CtlStm Fail to set video pid idx\n");
                    return FALSE;
                }
            }
#endif

            fgRet = SWDMX_SetStrmID(_rPlayerInfo.rPlayerStatus.u4PlayVidStrmId,
                                    0, eSWDMX_STRM_TYPE_VID);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set video stream ID\n");
                return FALSE;
            }

            fgRet = SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to enable video stream\n");
                return FALSE;
            }
        }
        else
        {
            fgRet = SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_Set Fail to disable video stream\n");
                return FALSE;
            }
            // handle video track change
            if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                if (_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
                {
                    _PLAYMGR_VdecFlushCtrl();
                    VDEC_Stop(INTERNAL_DAC_ID_MAIN);
                    _PLAYMGR_VdecWaitCtrlDone();
                }
            }
        }
        _rPlayerInfo.rPlayerStatus.fgEnVidStrm = fgEnable;
    }
    if (u4InfoValue == PLAYMGR_INFO_MASK_AUDIO)
    {
        if (fgEnable)
        {
            if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                #ifdef eSWDMX_GET_CHG_AUDIO_CAP
                UINT32 u4Capable2ChgAud = 0;

                // True is for playmgr can get the eSWDMX_GET_CHG_AUDIO_CAP successfully
                // False is for nothing
                if (SWDMX_GetInfo(eSWDMX_GET_CHG_AUDIO_CAP, &u4Capable2ChgAud, 0, 0))
                {
                    if (u4Capable2ChgAud == 0)
                    {
                        return FALSE;
                    }
                }
                #endif
            }

            if (_rPlayerInfo.rPlayerStatus.u2EnATrackNum >= (UINT16)PLAYMGR_MAX_AUD_TRACK)
            {
                LOG(1,"_PLAYMGR_CtlStm u2EnATrackNum >= PLAYMGR_MAX_AUD_TRACK\n");
                _rPlayerInfo.rPlayerStatus.u2EnATrackNum = (UINT16)(PLAYMGR_MAX_AUD_TRACK-1);
            }

            // handle audio track change
            if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                LOG(5, "aeAudFormat %d\n", _rPlayerInfo.rPlayerStatus.aeAudFormat[_rPlayerInfo.rPlayerStatus.u2EnATrackNum]);

                switch (_rPlayerInfo.rPlayerStatus.aeAudFormat[_rPlayerInfo.rPlayerStatus.u2EnATrackNum])
                {
                case PLAYMGR_ADEC_MP3:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#else
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#endif
                    break;
                case PLAYMGR_ADEC_AC3:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#else
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#endif
                    break;
                case PLAYMGR_ADEC_AAC:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#else
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#endif
                    break;
                case PLAYMGR_ADEC_MPEG:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#else
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#endif
                    break;
                   case PLAYMGR_ADEC_PCM:
                   if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                     (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                                                 == MEDIA_CONTNR_SUBTYPE_AVI))
                   {
                        // ToDo: set correct sample frequence
                        rPCMInfo.eSampleFreq = FS_48K;
                        rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                        // LPCM
                        rLPCMInfo.b_big_endian = 0;
                        rLPCMInfo.b_info_not_specific = 0;
                        rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                        rLPCMInfo.eSampleFreq = FS_48K;
                        rLPCMInfo.ui2_num_ch = 2;
                    }
                    else if((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                     == MEDIA_CONTNR_SUBTYPE_FLV))
                    {
                        UINT32 u4SampleRate=0;
                        UINT8    uPcmType=0;
                        MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;
                        if(_rPlayerInfo.FmtInfo.rFlvInfo.u2AudIdx<2)
                        {
                            prAudStrmAttr=&_rPlayerInfo.FmtInfo.rFlvInfo.rStmAttribute[_rPlayerInfo.FmtInfo.rFlvInfo.u2AudIdx];
                        }
                        else
                        {
                            LOG(5, "Flv Error,AudioIndex=%d\n",_rPlayerInfo.FmtInfo.rFlvInfo.u2AudIdx);
                            break;
                        }
                        u4SampleRate=prAudStrmAttr->u.t_flv_audio_stm_attr.i4_samples_per_sec;
                        uPcmType=prAudStrmAttr->u.t_flv_audio_stm_attr.e_enc;
                        if(u4SampleRate==11000)
                        rPCMInfo.eSampleFreq=FS_11K;
                        else if(u4SampleRate==22000)
                        rPCMInfo.eSampleFreq=FS_22K;
                        else if(u4SampleRate==44100)
                        rPCMInfo.eSampleFreq=FS_44K;

                        if(prAudStrmAttr->u.t_flv_audio_stm_attr.ui4_type)
                        {
                             rLPCMInfo.ui2_num_ch = 2;
                        }
                        else
                        {
                            rLPCMInfo.ui2_num_ch = 1;
                        }
                        rLPCMInfo.b_info_not_specific = 0;
                        if(uPcmType==MINFO_INFO_AUD_ENC_PCM)
                        {
                            rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                            rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                            rLPCMInfo.b_big_endian =0;
                        }
                        else if(uPcmType==MINFO_INFO_AUD_ENC_LPCM)
                        {
                             rLPCMInfo.b_big_endian =0;
                             rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                             rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
                        }
                        else if(uPcmType==MINFO_INFO_AUD_ENC_ADPCM)
                        {
                             rLPCMInfo.b_big_endian =0;
                             rLPCMInfo.eLPcmType = LPCM_TYPE_MS_ADPCM;
                             rPCMInfo.ePcmDataInfo = PCM_MS_ADPCM_LITTLE_ENDIAN;
                        }
                    }
                    else // PS file
                    {
                            // will change sample frequence in audio driver
                            rPCMInfo.eSampleFreq = FS_48K;
                            rPCMInfo.ePcmDataInfo = PCM_16_BITS_BIG_ENDIAN;
                            // LPCM
                            rLPCMInfo.b_big_endian = 1;
                            rLPCMInfo.b_info_not_specific = 1;
                            rLPCMInfo.eLPcmType = LPCM_TYPE_NORAML;
                            rLPCMInfo.eSampleFreq = FS_48K;
                            rLPCMInfo.ui2_num_ch = 2;

                    }

#ifdef CC_MT5363
                    AUD_PcmSetting(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, &rPCMInfo);
                    AUD_PcmSetting(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, &rPCMInfo);
                    AUD_LPcmSetting(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, &rLPCMInfo);
                    AUD_LPcmSetting(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, &rLPCMInfo);
                    AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                    AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,    AUD_FMT_PCM);
#else
                    AUD_PcmSetting(INTERNAL_DAC_ID_MAIN, &rPCMInfo);
                    AUD_LPcmSetting(INTERNAL_DAC_ID_MAIN, &rLPCMInfo);
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_PCM);
#endif

                    break;

                case PLAYMGR_ADEC_WMA:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#else
                    AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#endif
                        break;
#ifdef CC_MT5363
                    case PLAYMGR_ADEC_RA:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#else
                        AUD_SetDecType(INTERNAL_DAC_ID_MAIN, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#endif
                    break;
#endif
                default:
                    return FALSE;
                }
                //_PLAYMGR_AdecFlushCtrl();
                //AUD_DSPCmdPlay(INTERNAL_DAC_ID_MAIN);
                //_PLAYMGR_AdecWaitCtrlDone();
                // wait for audio API.
                //AUD_DrvSetDelay();
#ifdef CC_53XX_AUDIO_V2
                _PLAYMGR_AdecFlushCtrl();
                AUD_DSPCmdPlay(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
                AUD_DspSpeed(INTERNAL_DAC_ID_MAIN, 0);
                AUD_DSPCmdPlay(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
                _PLAYMGR_AdecWaitCtrlDone();
                //AUD_DSPCmdPlayAsyn(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
                //AUD_DspSpeed(INTERNAL_DAC_ID_MAIN, 0);
                //AUD_DSPCmdPlayAsyn(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
#else
                _PLAYMGR_AdecFlushCtrl();
                AUD_DSPCmdPlay(INTERNAL_DAC_ID_MAIN);
                _PLAYMGR_AdecWaitCtrlDone();
                //AUD_DSPCmdPlayAsyn(INTERNAL_DAC_ID_MAIN);
                AUD_DspSpeed(INTERNAL_DAC_ID_MAIN, 0);
#endif
                _rPlayerInfo.rPlayerStatus.fgAudPlayed = TRUE;
            }

            if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                     == MEDIA_CONTNR_SUBTYPE_MP4))
            {
                if ((_rPlayerInfo.rPlayerStatus.u2VideoTracks + _rPlayerInfo.rPlayerStatus.u2EnATrackNum) < 3)
                {
                    MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;

                    prAudStrmAttr =
                        &_rPlayerInfo.FmtInfo.rMp4Info.rStmAttribute[_rPlayerInfo.rPlayerStatus.u2VideoTracks + _rPlayerInfo.rPlayerStatus.u2EnATrackNum];

                    fgRet = SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,
                                          _PLAYMGR_PlaymgrAdec2Swdmx(
                                              _rPlayerInfo.rPlayerStatus.aeAudFormat[_rPlayerInfo.rPlayerStatus.u2EnATrackNum]),
                                          prAudStrmAttr->u.t_mp4i_audio_stm_attr.i4_sample_rate,
                                          prAudStrmAttr->u.t_mp4i_audio_stm_attr.i2_channels);
                }
                else
                {
                    LOG(1, "Out of total stream count\n");
                    fgRet = FALSE;
                    return FALSE;
                }
            }
            else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                     (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_FLV))
             {
                 if ((_rPlayerInfo.rPlayerStatus.u2VideoTracks + _rPlayerInfo.rPlayerStatus.u2EnATrackNum) < 2)
                 {
                     MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;

                     prAudStrmAttr =&_rPlayerInfo.FmtInfo.rFlvInfo.rStmAttribute[_rPlayerInfo.rPlayerStatus.u2VideoTracks + _rPlayerInfo.rPlayerStatus.u2EnATrackNum];

                    fgRet = SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,
                                          _PLAYMGR_PlaymgrAdec2Swdmx(
                                              _rPlayerInfo.rPlayerStatus.aeAudFormat[_rPlayerInfo.rPlayerStatus.u2EnATrackNum]),
                                          prAudStrmAttr->u.t_flv_audio_stm_attr.i4_samples_per_sec,
                                          prAudStrmAttr->u.t_flv_audio_stm_attr.i2_channels);
                 }
                 else
                 {
                     LOG(1, "Out of total stream count\n");
                     fgRet = FALSE;
                     return FALSE;
                 }
             }
#if 1
            else if ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                     (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_contnr_subtype
                     == MEDIA_CONTNR_SUBTYPE_RM))
            {
                MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;
                ENUM_SWDMX_ADEC_T eAud;

                if (_rPlayerInfo.rPlayerStatus.u2VideoTracks + _rPlayerInfo.rPlayerStatus.u2EnATrackNum < 3)
                {
                    prAudStrmAttr =
                        &_rPlayerInfo.FmtInfo.rRmInfo.rStmAttribute[_rPlayerInfo.rPlayerStatus.u2VideoTracks + _rPlayerInfo.rPlayerStatus.u2EnATrackNum];
                }
                else
                {
                    LOG(1, "Out of total stream count\n");
                    fgRet = FALSE;
                    return FALSE;
                }

                eAud = _PLAYMGR_PlaymgrAdec2Swdmx(_rPlayerInfo.rPlayerStatus.aeAudFormat[_rPlayerInfo.rPlayerStatus.u2EnATrackNum]);
                if (eAud == ENUM_SWDMX_ADEC_AAC)
                {
                    fgRet = SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE, ENUM_SWDMX_ADEC_AAC,
                        prAudStrmAttr->u.t_rm_audio_stm_attr.ui4_sample_rate,
                        prAudStrmAttr->u.t_rm_audio_stm_attr.ui2_num_channel);
                }
                else if (eAud == ENUM_SWDMX_ADEC_RA)
                {
                    fgRet = SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE, ENUM_SWDMX_ADEC_RA,
                        (UINT32)&_rCookInfo, 0);
                }
                else
                {
                    ASSERT(0);
                    fgRet = FALSE;
                }
            }
#endif
            else
            {
                fgRet = SWDMX_SetInfo(eSWDMX_SET_AUDIOTYPE,
                                      _PLAYMGR_PlaymgrAdec2Swdmx(
                                          _rPlayerInfo.rPlayerStatus.aeAudFormat[_rPlayerInfo.rPlayerStatus.u2EnATrackNum]),
                                      0, 0);
            }

            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set audio format info\n");
                return FALSE;
            }

            fgRet = SWDMX_SetStrmID(_rPlayerInfo.rPlayerStatus.u4PlayAudStrmId,
                                    _rPlayerInfo.rPlayerStatus.u4PlayAudSubStrmId,
                                    eSWDMX_STRM_TYPE_AUD);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set audio stream ID\n");
                return FALSE;
            }

#ifdef TIME_SHIFT_SUPPORT
            if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT)
            {
                if(_rPlayerInfo.rPlayerStatus.fgPlayAudDecrypt)
                {
                    fgRet = SWDMX_SetInfo(eSWDMX_SET_AUD_PID_IDX,
                                            _rPlayerInfo.rPlayerStatus.u4PlayAudIdx,
                                            _rPlayerInfo.rPlayerStatus.u4PlayAudKeyIdx,
#ifdef CC_MT5387
                                            (UINT32)DMX_DESC_MODE_AES_ECB);
#else
                                            (UINT32)DMX_DESC_MODE_3DES_ECB);
#endif
                }
                else
                {
                    fgRet = SWDMX_SetInfo(eSWDMX_SET_AUD_PID_IDX,
                                            _rPlayerInfo.rPlayerStatus.u4PlayAudIdx,
                                            0, 0);
                }
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_CtlStm Fail to set audio pid idx\n");
                    return FALSE;
                }
            }
#endif
            fgRet = SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_AUD, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to enable audio stream\n");
                return FALSE;
            }
        }
        else
        {
            fgRet = SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_AUD, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_Set Fail to disable audio stream\n");
                return FALSE;
            }
            _rPlayerInfo.rPlayerStatus.fgDisAudStrm = FALSE;
            // handle audio track change
            if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                if (_rPlayerInfo.rPlayerStatus.fgEnAudStrm)
                {
                    _PLAYMGR_AdecFlushCtrl();
#ifdef CC_53XX_AUDIO_V2
                    AUD_DSPCmdStop(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
                    AUD_DSPCmdStop(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
#else
                    AUD_DSPCmdStop(INTERNAL_DAC_ID_MAIN);
#endif
                    _PLAYMGR_AdecWaitCtrlDone();
                    _rPlayerInfo.rPlayerStatus.fgAudPlayed = FALSE;
                }
            }
        }
        _rPlayerInfo.rPlayerStatus.fgEnAudStrm = fgEnable;
    }
    if (u4InfoValue == PLAYMGR_INFO_MASK_PCR)
    {
        if (fgEnable)
        {
            if (_rPlayerInfo.rPlayerStatus.u2EnPcrTrackNum >= (UINT16)PLAYMGR_MAX_PCR_TRACK)
            {
                LOG(1,"_PLAYMGR_CtlStm u2EnPcrTrackNum >= PLAYMGR_MAX_PCR_TRACK\n");
                _rPlayerInfo.rPlayerStatus.u2EnPcrTrackNum = (UINT16)(PLAYMGR_MAX_PCR_TRACK-1);
            }

#ifdef TIME_SHIFT_SUPPORT
            if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT)
            {
                fgRet = SWDMX_SetInfo(eSWDMX_SET_PCR_PID_IDX,
                                        _rPlayerInfo.rPlayerStatus.u4PlayPcrIdx,
                                        0, 0);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_CtlStm Fail to set pcr pid idx\n");
                    return FALSE;
                }
            }
#endif

            fgRet = SWDMX_SetStrmID(_rPlayerInfo.rPlayerStatus.u4PlayPcrStrmId,
                                    0, eSWDMX_STRM_TYPE_PCR);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set pcr stream ID\n");
                return FALSE;
            }

            fgRet = SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to enable pcr stream\n");
                return FALSE;
            }
        }
        else
        {
            fgRet = SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_Set Fail to disable pcr stream\n");
                return FALSE;
            }
        }
        _rPlayerInfo.rPlayerStatus.fgEnPcrStrm = fgEnable;
    }
    if (u4InfoValue == PLAYMGR_INFO_MASK_CAPTRACK)
    {
        if (fgEnable)
        {
            if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                #ifdef eSWDMX_GET_CHG_SUBTITLE_CAP
                UINT32 u4Capable2ChgSub = 0;

                // True is for playmgr can get the eSWDMX_GET_CHG_AUDIO_CAP successfully
                // False is for nothing
                if (SWDMX_GetInfo(eSWDMX_GET_CHG_SUBTITLE_CAP, &u4Capable2ChgSub, 0, 0))
                {
                    if (u4Capable2ChgSub == 0)
                    {
                        return FALSE;
                    }
                }
                #endif
            }

            fgRet = SWDMX_SetStrmID(_rPlayerInfo.rPlayerStatus.u4PlaySubId,
                                    0, eSWDMX_STRM_TYPE_SUBTITLE);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set subpic stream ID\n");
                return FALSE;
            }

            fgRet = SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM,
                                  (UINT32)eSWDMX_STRM_TYPE_SUBTITLE,
                                  (UINT32)_PLAYMGR_SubtitleCb,
                                  0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to enable subpic stream\n");
                return FALSE;
            }
        }
        else
        {
            fgRet = SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_SUBTITLE, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to disable subpic stream\n");
                return FALSE;
            }
        }
        _rPlayerInfo.rPlayerStatus.fgEnSubStrm = fgEnable;
    }
    if (u4InfoValue == PLAYMGR_INFO_MASK_PCR)
    {
        if (fgEnable)
        {
            fgRet = SWDMX_SetStrmID(_rPlayerInfo.rPlayerStatus.u4PlayPcrStrmId,
                                    0, eSWDMX_STRM_TYPE_PCR);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set pcr stream ID\n");
                return FALSE;
            }

            fgRet = SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM,
                                  (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to enable pcr stream\n");
                return FALSE;
            }
        }
        else
        {
            fgRet = SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to disable pcr stream\n");
                return FALSE;
            }
            _rPlayerInfo.rPlayerStatus.fgDisPcrStrm = FALSE;
            // handle pcr track change
            if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                if (_rPlayerInfo.rPlayerStatus.fgEnPcrStrm)
                {
                    _rPlayerInfo.rPlayerStatus.fgPcrPlayed = FALSE;
                }
            }
        }
        _rPlayerInfo.rPlayerStatus.fgEnPcrStrm = fgEnable;
    }
    return TRUE;
}


BOOL _PLAYMGR_Step(INT32 i4StepAmount)
{
    PLAYMGR_STATUS_T* prMediaStatus = &_rPlayerInfo.rPlayerStatus;
    ENUM_VDEC_FMT_T eFmt;
    BOOL fgRet;

    LOG(5, "_PLAYMGR_Step %d\n", i4StepAmount);

    if((_fgIsSeeking) &&
       (_ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }

    if (_ePlayMgrCurStatus != PLAYMGR_STATE_PAUSE)
    {
        LOG(3, "_PLAYMGR_Step must in PLAYMGR_STATE_PAUSE\n");
        ASSERT(0);
        return TRUE;
    }

    if (prMediaStatus->u2EnVTrackNum >= PLAYMGR_MAX_VID_TRACK)
    {
        LOG(3, "PLAYMGR_MAX_VID_TRACK\n");
        prMediaStatus->u2EnVTrackNum = (PLAYMGR_MAX_VID_TRACK-1);
    }

    if ((_rPlayerInfo.rPlayerStatus.fgEnAudStrm) &&
            (_rPlayerInfo.rPlayerStatus.fgEnVidStrm))
    {
        if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_AUDIO, FALSE))
        {
            LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
        }
        _rPlayerInfo.rPlayerStatus.fgEnAudStrm = FALSE;
        _rPlayerInfo.rPlayerStatus.fgDisAudStrm = TRUE;
        _PLAYMGR_AdecFlushCtrl();
#ifdef CC_53XX_AUDIO_V2
        AUD_DSPCmdStop(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
        AUD_DSPCmdStop(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
#else
        AUD_DSPCmdStop(INTERNAL_DAC_ID_MAIN);
#endif
        _PLAYMGR_AdecWaitCtrlDone();
        _rPlayerInfo.rPlayerStatus.fgAudPlayed = FALSE;
    }
    if (_rPlayerInfo.rPlayerStatus.fgEnPcrStrm)
    {
        if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_PCR, FALSE))
        {
            LOG(1,"_PLAYMGR_CtlStm disable pcr fail\n");
        }
        _rPlayerInfo.rPlayerStatus.fgEnPcrStrm = FALSE;
        _rPlayerInfo.rPlayerStatus.fgDisPcrStrm = TRUE;
        _rPlayerInfo.rPlayerStatus.fgAudPlayed = FALSE;
    }

    if(((_rPlayerInfo.rPlayerStatus.i4Speed <= 0) ||
        (_rPlayerInfo.rPlayerStatus.i4Speed >= 2000))
       ||
       (i4StepAmount < 0))
    {
        _PLAYMGR_Flush();
    }
    _rPlayerInfo.rPlayerStatus.i4Speed = 1;
    if (i4StepAmount < 0)
    {
        // ToDo: consider amount
        _rPlayerInfo.rPlayerStatus.i4Speed = -1;
    }

    fgRet = SWDMX_SetSpeed(_rPlayerInfo.rPlayerStatus.i4Speed);
    if (!fgRet)
    {
        LOG(1,"_PLAYMGR_Step SWDMX_SetSpeed fail\n");
    }
    fgRet = SWDMX_Play();
    if (!fgRet)
    {
        LOG(1,"_PLAYMGR_Step SWDMX_Play fail\n");
        return FALSE;
    }

    switch (prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum])
    {
    case PLAYMGR_VDEC_MPEG4:
    case PLAYMGR_VDEC_DIVX311:
        eFmt = VDEC_FMT_MP4;
        break;
    case PLAYMGR_VDEC_MPEG1_MPEG2:
        eFmt = VDEC_FMT_MPV;
        break;
    case PLAYMGR_VDEC_H264:
        eFmt = VDEC_FMT_H264;
        break;
    case PLAYMGR_VDEC_WMV7:
    case PLAYMGR_VDEC_WMV8:
    case PLAYMGR_VDEC_WMV9:
    case PLAYMGR_VDEC_VC1:
        eFmt = VDEC_FMT_WMV;
        break;
        case PLAYMGR_VDEC_RV8:
        case PLAYMGR_VDEC_RV9:
            eFmt = VDEC_FMT_RV;
            break;
    default:
        LOG(1, "Video format unknown\n");
        return FALSE;
    }

    /* VDEC play for new frame decoding */
    VDEC_PlayMM(ES0 , TRUE);
    VDEC_Play(ES0, eFmt);
    VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1X, 0, 0);
    VDP_SetSpeed(B2R_1, STC_SPEED_TYPE_FORWARD_1X);

#if 1
    _PLAYMGR_VdpFlushCtrl();
#endif

    /* Pause VDP and just display 1 frame */
    VDP_SetPauseMm(B2R_1, TRUE);
    VDP_SetFBNoTimeout(VDP_1, TRUE);
    VDP_SetStepForward(VDP_1, VDP_PLAY_STEP_FORWARD);

    _PLAYMGR_SetSyncMode();
    VDP_RegCbFunc(VDP_CB_FUNC_MM_STEP_FIN_IND, (UINT32)_PLAYMGR_VdpStepDoneNfy, 0, 0);

    /* VDP will play one frame and then Pause if VDP_PLAY_STEP_FORWARD is set */
#if 0
    _PLAYMGR_VdpFlushCtrl();
#endif
    VDP_SetPauseMm(B2R_1, FALSE);
    _PLAYMGR_VdpWaitCtrlDone();

    // step done.
    FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_SEEK_END, (_rPlayerInfo.u4SeekPts/90));

    // ToDo: we don't need pause?
    VDEC_Pause(ES0);

    // we don't need pause
    //fgRet = SWDMX_Pause();
    //if(!fgRet)
    //{
    //    LOG(1,"_PLAYMGR_Step SWDMX_Pause fail\n");
    //    return FALSE;
    //}

    return TRUE;
}


BOOL _PLAYMGR_SetSyncMode(VOID)
{
    if((_fgIsSeeking) &&
       (_ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }

    // DTV AUD_MASTER, AV_SYNC_MODE_SYSTEM_MASTER
#ifdef TIME_SHIFT_SUPPORT
    if ((_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT) &&
            (_rPlayerInfo.rPlayerStatus.i4Speed == 1000))
    {
        STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER);
        return TRUE;
    }
#endif

    if (!_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
    {
        if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
        {
            // not have container, not have PTS case.
            //STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER);
            STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER);
        }
        else
        {
            // have container, have PTS case.
            // audio driver could udpate stc value
            STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER);
        }
    }
    else // have video
    {
        if (_rPlayerInfo.rPlayerStatus.i4Speed == 1000)
        {
            if (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
            {
                STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER);
            }
            if (_rPlayerInfo.rPlayerStatus.fgEnAudStrm)
            {
                STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER);
            }
            else
            {
                STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER);
            }
        }
        else if (_rPlayerInfo.rPlayerStatus.i4Speed == 2000)
        {
#ifdef TIME_SHIFT_SUPPORT
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
            if (_rPlayerInfo.rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT)
            {
                STC_SetSyncMode(AV_SYNC_MODE_NONE);
            }
            else
#endif
#endif
            {
                STC_SetSyncMode(AV_SYNC_MODE_VIDEO_MASTER);
            }
        }
        else if ((_rPlayerInfo.rPlayerStatus.i4Speed < 1000) &&
                 (_rPlayerInfo.rPlayerStatus.i4Speed > 10))
        {
            /* Slow motion */
            STC_SetSyncMode(AV_SYNC_MODE_VIDEO_MASTER);
        }
        else
        {
            STC_SetSyncMode(AV_SYNC_MODE_NONE);
        }
    }

    return TRUE;
}

#ifdef __KERNEL__
/**Nofify playmgr to get data
*@param u4Size                Received data size
*@retval TRUE                 Success.
*@retval FALSE                Fail.
*/
BOOL _PLAYMGR_ReceiveData(UINT32 u4Size,  UINT64 u8Pos)
{
    PLAYMGR_RECEIVE_DATA_T rReceive;
    SIZE_T zMsgSize = sizeof(PLAYMGR_RECEIVE_DATA_T);
    INT32 i4Ret;

    rReceive.u4ReadSize = u4Size;
    rReceive.u8FilePos = u8Pos;

    i4Ret = x_msg_q_send(_hPlayMgrDataQ, &rReceive, zMsgSize, 255);
    if (i4Ret == OSR_TOO_MANY)
    {
        return FALSE;
    }

    VERIFY(i4Ret == OSR_OK);

    return TRUE;
}
#endif


/**Set playback command to playmgr
*@param rStatus  command and parameters
*@retval
*/
BOOL _PLAYMGR_SetCMD(PLAYMGR_STATUS_CMD_T rStatus)
{
#ifdef PLAYMGR_ASYNC_MODE
    BOOL i4Ret = FALSE;
    SIZE_T zMsgSize = sizeof(PLAYMGR_STATUS_CMD_T);

    i4Ret = x_msg_q_send(_hPlayMgrStatusQ, (VOID *)&rStatus, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        i4Ret = x_msg_q_send(_hPlayMgrStatusQ, (VOID *)&rStatus, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);
#else
    BOOL fgRet = FALSE;
    UINT64 u8Tmp;

    if(((_fgIsSeeking) &&
       (_ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT)) &&
       (rStatus.eCmd != PLAYMGR_SETCMD_STOP))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }

    switch (rStatus.eCmd)
    {
    case PLAYMGR_SETCMD_INIT:
        _ePlayMgrCurStatus = PLAYMGR_STATE_CLOSE;
        fgRet = TRUE;
        break;
    case PLAYMGR_SETCMD_OPEN:
        if (_ePlayMgrCurStatus == PLAYMGR_STATE_CLOSE)
        {
            _ePlayMgrCurStatus = PLAYMGR_STATE_OPEN;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_CLOSE:
        if ( (_ePlayMgrCurStatus == PLAYMGR_STATE_STOP) || (_ePlayMgrCurStatus == PLAYMGR_STATE_CLOSE) ||
            (_ePlayMgrCurStatus == PLAYMGR_STATE_OPEN))
        {
            _ePlayMgrCurStatus = PLAYMGR_STATE_CLOSE;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_START:
        if ((_ePlayMgrCurStatus == PLAYMGR_STATE_STOP) ||
                (_ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE))
        {
            _ePlayMgrCurStatus = PLAYMGR_STATE_PLAY;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_STOP:
        if ( (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY) || (_ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
                || (_ePlayMgrCurStatus == PLAYMGR_STATE_OPEN))
        {
            _ePlayMgrCurStatus = PLAYMGR_STATE_STOP;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_PAUSE:
        if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            _ePlayMgrCurStatus = PLAYMGR_STATE_PAUSE;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_RESUME:
        if (_ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
        {
            _ePlayMgrCurStatus = PLAYMGR_STATE_PLAY;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_TRICK:
        if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            fgRet = TRUE;
        }
        else if (_ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
        {
            fgRet = _PLAYMGR_Resume();
            if (fgRet)
            {
                _ePlayMgrCurStatus = PLAYMGR_STATE_PLAY;
            }
        }
        break;
    case PLAYMGR_SETCMD_ABORT:
        // ToDo: what should be done here
        /*
        if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            _ePlayMgrCurStatus = PLAYMGR_STATE_STOP;
            fgRet = TRUE;
        }
        else if (_ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
        {
            _ePlayMgrCurStatus = PLAYMGR_STATE_STOP;
            fgRet = TRUE;
        }
        */
        // call x_midxbuld_stop_indexing
        FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_ABORT_DONE, 0);
        break;
    case PLAYMGR_SETCMD_SEEKTIME:
    case PLAYMGR_SETCMD_SEEKPOSITION:
    case PLAYMGR_SETCMD_SEEKPERCENT:
        _fgIsSeeking = TRUE;
        if(!_PLAYMGR_GetCap(PLAYMGR_CAP_SEEK, 0, 0))
        {
            fgRet = FALSE;
            LOG(5,"_PLAYMGR_Seek fgTrick(0)\n");
        }
        else if ((_ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE) ||
                (_ePlayMgrCurStatus == PLAYMGR_STATE_STOP))
        {
            if (_rPlayerInfo.rPlayerStatus.fgEnAudStrm)
            {
                if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_AUDIO, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
                }
                _rPlayerInfo.rPlayerStatus.fgEnAudStrm = FALSE;
                _rPlayerInfo.rPlayerStatus.fgDisAudStrm = TRUE;
                _PLAYMGR_AdecFlushCtrl();
#ifdef CC_53XX_AUDIO_V2
                AUD_DSPCmdStop(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
                AUD_DSPCmdStop(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
#else
                AUD_DSPCmdStop(INTERNAL_DAC_ID_MAIN);
#endif
                _PLAYMGR_AdecWaitCtrlDone();
                _rPlayerInfo.rPlayerStatus.fgAudPlayed = FALSE;
            }
            if (_rPlayerInfo.rPlayerStatus.fgEnPcrStrm)
            {
                if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_PCR, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable pcr fail\n");
                }
                _rPlayerInfo.rPlayerStatus.fgEnPcrStrm = FALSE;
                _rPlayerInfo.rPlayerStatus.fgDisPcrStrm = TRUE;
                _rPlayerInfo.rPlayerStatus.fgAudPlayed = FALSE;
            }
            if (rStatus.eCmd == PLAYMGR_SETCMD_SEEKTIME)
            {
                fgRet = _PLAYMGR_SeekTime(rStatus.u4Param1);
            }
            else if (rStatus.eCmd == PLAYMGR_SETCMD_SEEKPOSITION)
            {
                u8Tmp = rStatus.u4Param1;
                u8Tmp <<= 32;
                u8Tmp += rStatus.u4Param2;
                fgRet = _PLAYMGR_SeekPosition(u8Tmp);
            }
            else
            {
                fgRet = _PLAYMGR_SeekPercent(rStatus.u4Param1);
            }
            if (!fgRet)
            {
                LOG(3, "!PLAYMGR_STATE_PLAY _PLAYMGR_Seekxxx fail\n");
            }
        }
        else if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            LOG(5, "_PLAYMGR_SetCMD _PLAYMGR_Pause\n");
            fgRet = _PLAYMGR_Pause();
            if (!fgRet)
            {
                LOG(3, "PLAYMGR_STATE_PLAY _PLAYMGR_Pause fail\n");
            }
            _ePlayMgrCurStatus = PLAYMGR_STATE_PAUSE;
            if (_rPlayerInfo.rPlayerStatus.fgEnAudStrm)
            {
                if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_AUDIO, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
                }
                _rPlayerInfo.rPlayerStatus.fgEnAudStrm = FALSE;
                _rPlayerInfo.rPlayerStatus.fgDisAudStrm = TRUE;
                _PLAYMGR_AdecFlushCtrl();
#ifdef CC_53XX_AUDIO_V2
                AUD_DSPCmdStop(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
                AUD_DSPCmdStop(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
#else
                AUD_DSPCmdStop(INTERNAL_DAC_ID_MAIN);
#endif
                _PLAYMGR_AdecWaitCtrlDone();
                _rPlayerInfo.rPlayerStatus.fgAudPlayed = FALSE;
            }

            if (_rPlayerInfo.rPlayerStatus.fgEnPcrStrm)
            {
                if (!_PLAYMGR_CtlStm(PLAYMGR_INFO_MASK_PCR, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable pcr fail\n");
                }
                _rPlayerInfo.rPlayerStatus.fgEnPcrStrm = FALSE;
                _rPlayerInfo.rPlayerStatus.fgDisPcrStrm = TRUE;
                _rPlayerInfo.rPlayerStatus.fgPcrPlayed = FALSE;
            }

            if (rStatus.eCmd == PLAYMGR_SETCMD_SEEKTIME)
            {
                LOG(5, "_PLAYMGR_SetCMD _PLAYMGR_SeekTime\n");
                fgRet = _PLAYMGR_SeekTime(rStatus.u4Param1);
            }
            else if (rStatus.eCmd == PLAYMGR_SETCMD_SEEKPOSITION)
            {
                LOG(5, "_PLAYMGR_SetCMD _PLAYMGR_SeekPosition\n");
                u8Tmp = rStatus.u4Param1;
                u8Tmp <<= 32;
                u8Tmp += rStatus.u4Param2;
                fgRet = _PLAYMGR_SeekPosition(u8Tmp);
            }
            else
            {
                LOG(5, "_PLAYMGR_SetCMD _PLAYMGR_SeekPercent\n");
                fgRet = _PLAYMGR_SeekPercent(rStatus.u4Param1);
            }
            if (!fgRet)
            {
                LOG(3, "PLAYMGR_STATE_PLAY _PLAYMGR_Seekxxx fail\n");
            }

            LOG(5, "_PLAYMGR_SetCMD _PLAYMGR_Start\n");
            fgRet = _PLAYMGR_Start();
            if (!fgRet)
            {
                LOG(3, "PLAYMGR_SETCMD_SEEKTIME _PLAYMGR_Start fail\n");
            }
            _ePlayMgrCurStatus = PLAYMGR_STATE_PLAY;
        }
        if (!fgRet)
        {
            _fgIsSeeking = FALSE;
        }
        break;
    case PLAYMGR_SETCMD_STEP:
        if (!_rPlayerInfo.rPlayerStatus.fgEnVidStrm)
        {
            LOG(3, "PLAYMGR_SETCMD_STEP no video stream\n");
            return TRUE;
        }
        if (_ePlayMgrCurStatus == PLAYMGR_STATE_STOP)
        {
            LOG(3, "PLAYMGR_SETCMD_STEP PLAYMGR_STATE_STOP\n");
            return TRUE;
        }
        else if (_ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
        {
            fgRet = _PLAYMGR_Step((INT32)rStatus.u4Param1);
            if (!fgRet)
            {
                LOG(3, "!PLAYMGR_STATE_PLAY _PLAYMGR_Step fail\n");
            }
        }
        else if (_ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            LOG(5, "PLAYMGR_STATE_PLAY _PLAYMGR_Pause\n");
            fgRet = _PLAYMGR_Pause();
            if (!fgRet)
            {
                LOG(3, "PLAYMGR_STATE_PLAY _PLAYMGR_Pause fail\n");
            }
            fgRet = _PLAYMGR_Step((INT32)rStatus.u4Param1);
            if (!fgRet)
            {
                LOG(3, "!PLAYMGR_STATE_PLAY _PLAYMGR_Step fail\n");
            }
        }
        break;
    default:
        break;
    }
    return fgRet;
#endif
}

#ifdef PLAYMGR_ASYNC_MODE
/**A Thread for state changing of playmgr
*@param pvArg
*@retval
*/
static VOID _PLAYMGR_MainLoop(VOID* pvArg)
{
    PLAYMGR_STATUS_CMD_T rStatus;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize = sizeof(PLAYMGR_STATUS_CMD_T);

    while (1)
    {
        x_msg_q_receive(&u2MsgQIdx, (VOID *)&rStatus, &zMsgSize,
                        &(_hPlayMgrStatusQ), 1, X_MSGQ_OPTION_WAIT);

        switch (rStatus.eCmd)
        {
        case PLAYMGR_SETCMD_OPEN:
            if (_ePlayMgrCurStatus == PLAYMGR_STATE_CLOSE)
            {
                if (_PLAYMGR_GetInfo(&rStatus.u4Param1))
                {
                    _ePlayMgrCurStatus = PLAYMGR_STATE_GETINFO;
                }
            }
            break;
        case PLAYMGR_SETCMD_CLOSE:

            break;
        case PLAYMGR_SETCMD_START:

            break;
        case PLAYMGR_SETCMD_STOP:

            break;
        case PLAYMGR_SETCMD_PAUSE:

            break;
        case PLAYMGR_SETCMD_RESUME:

            break;
        case PLAYMGR_SETCMD_TRICK:

            break;
        case PLAYMGR_SETCMD_SEEKTIME:
        case PLAYMGR_SETCMD_SEEKPERCENT:

            break;
        default:
            break;
        }
    }
}
#endif


BOOL _PLAYMGR_GetDivxDRMStatus(PLAYMGR_DRMTYPE *peDRMType, UINT32 *pu4DRMRentalLimit, UINT32 *pu4DRMRentalCount)
{
#ifndef CC_AVI_DISABLE
    return _PLAYMGR_AviGetDivxDRMStatus(peDRMType, pu4DRMRentalLimit, pu4DRMRentalCount);
#else
    return TRUE;
#endif
}


BOOL _PLAYMGR_GetDivxDRMRegCode(char *pu1Code)
{
#ifndef CC_AVI_DISABLE
    return _PLAYMGR_AviGetDivxDRMRegCode(pu1Code);
#else
    return TRUE;
#endif
}


BOOL _PLAYMGR_GetDivxDRMUIHelpInfo(UINT32 *pu4Info)
{
#ifndef CC_AVI_DISABLE
    return _PLAYMGR_AviGetDivxDRMUIHelpInfo(pu4Info);
#else
    return TRUE;
#endif
}


BOOL _PLAYMGR_DivxDRMDeactivation(char *pu1Code)
{
#ifndef CC_AVI_DISABLE
    return _PLAYMGR_AviDivxDRMDeactivation(pu1Code);
#else
    return TRUE;
#endif
}


VOID _PLAYMGR_SetSrcType(PLAYMGR_SourceType_T eSrcType)
{
    if (!_rPlayerInfo.fgInit)
    {
        _PLAYMGR_Init();
    }
    _ePlayMgrSrcType = eSrcType;
}

PLAYMGR_SourceType_T _PLAYMGR_GetSrcType(VOID)
{
    return _ePlayMgrSrcType;
}

VOID _PLAYMGR_SetNumberOfBitstream(UINT32 u4Size)
{
    _u4NumberOfBitstream = u4Size;
}

UINT32 _PLAYMGR_GetNumberOfBitstream(VOID)
{
    return _u4NumberOfBitstream;
}

BOOL _PLAYMGR_GetCap(UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2)
{
    BOOL fgRet = FALSE;
    UINT32 u4Trick = TRUE, u4Dummy1, u4Dummy2;
    UINT32 u4Forward = 0, u4Speed;

    switch(u4Type)
    {
        case PLAYMGR_CAP_SEEK:
            if(_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
            {
                SWDMX_GetInfo(eSWDMX_GET_IS_TRICK, &u4Trick, &u4Dummy1, &u4Dummy2);
            }
            fgRet = (BOOL)u4Trick;
            break;
        case PLAYMGR_CAP_TRICKMODE:
            if(_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
            {
                SWDMX_GetInfo(eSWDMX_GET_IS_TRICK, &u4Trick, &u4Dummy1, &u4Dummy2);
            }
            else if((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_PS) ||
                    (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_TS) ||
                    (_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES) ||
                    ((_rPlayerInfo.rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
                     (_rPlayerInfo.rMediaFmt.t_media_subtype.u.e_aud_subtype != MEDIA_AUD_SUBTYPE_WMA)))
            {
                u4Speed = (u4Type == PLAYMGR_CAP_TRICKMODE) ? u4Param1 : _rPlayerInfo.rPlayerStatus.i4Speed;
                if(u4Speed == 1000)
                {
                    fgRet = TRUE;
                    break;
                }
                if(u4Speed > 1000)    //speed
                {
                    u4Forward = 1;
                }
                SWDMX_GetInfo(eSWDMX_GET_IS_TRICK, &u4Trick, &u4Forward, &u4Dummy2);
            }
            fgRet = (BOOL)u4Trick;
            break;
        default:
            break;
    }

    UNUSED(u4Param2);
    UNUSED(u4Dummy1);
    UNUSED(u4Dummy2);
    return fgRet;
}

BOOL _PLAYMGR_SetLPCMInfo(PLAYMGR_LPCM_INFO_T rPlayLPCMInfo)
{
    MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;

    if (_ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        prAudStrmAttr =&_rPlayerInfo.FmtInfo.rMp3Info.rStreamAttr;
        if(prAudStrmAttr==NULL)
        {
            LOG(0,"prAudStrmAttr NULL\n");
            return FALSE;
        }

        LOG(5,"_PLAYMGR_SetLPCMInfo\n");
        LOG(5,"eLPCMType(%d), fgBigEndian(%d), u4SampleRate(%d), u2Channel(%d)\n",
            rPlayLPCMInfo.eLPCMType,
            rPlayLPCMInfo.fgBigEndian,
            rPlayLPCMInfo.u4SampleRate,
            rPlayLPCMInfo.u2Channel);
        //LOG(5,"u2BitsPerSample(%d), u8Duration(%lld), u8FileSize(%lld)\n",
        //    rPlayLPCMInfo.u2BitsPerSample,
        //    rPlayLPCMInfo.u8Duration,
        //    rPlayLPCMInfo.u8FileSize);
        LOG(5,"u2BitsPerSample(%d)\n",
            rPlayLPCMInfo.u2BitsPerSample);
        x_memset(&prAudStrmAttr->u.t_es_audio_stm_attr, 0,
            sizeof(prAudStrmAttr->u.t_es_audio_stm_attr));
        prAudStrmAttr->u.t_es_audio_stm_attr.e_enc =
            (rPlayLPCMInfo.eLPCMType == PLAYMGR_LPCM_TYPE_NORMAL) ?
            MINFO_INFO_AUD_ENC_PCM :
            MINFO_INFO_AUD_ENC_ADPCM;
        prAudStrmAttr->u.t_es_audio_stm_attr.b_big_endian =
            rPlayLPCMInfo.fgBigEndian;
        prAudStrmAttr->u.t_es_audio_stm_attr.ui4_sampling_frequency =
            rPlayLPCMInfo.u4SampleRate;
        prAudStrmAttr->u.t_es_audio_stm_attr.ui2_channels =
            rPlayLPCMInfo.u2Channel;
        prAudStrmAttr->u.t_es_audio_stm_attr.ui2_bits_per_sample =
            rPlayLPCMInfo.u2BitsPerSample;

        //_rPlayerInfo.rRangeInfo[0].z_range_sz =
        //    rPlayLPCMInfo.u8FileSize;
        //_rPlayerInfo.rRangeInfo[0].ui8_pb_duration =
        //    (rPlayLPCMInfo.u8Duration * (UINT64)90000);
        _rPlayerInfo.rRangeInfo[0].z_range_sz = (UINT64)(-1);
        _rPlayerInfo.rRangeInfo[0].ui8_pb_duration = 0;

        _rPlayerInfo.rPlayerStatus.u4TotalDuration =
            (UINT32)_rPlayerInfo.rRangeInfo[0].ui8_pb_duration;
        _rPlayerInfo.u8FileSize =
            (UINT32)_rPlayerInfo.rRangeInfo[0].z_range_sz;
        _rPlayerInfo.rPlayerStatus.u8FileSize =
            (UINT32)_rPlayerInfo.rRangeInfo[0].z_range_sz;

        return TRUE;
    }
    LOG(5,"_PLAYMGR_SetLPCMInfo unknow type\n");
    return FALSE;
}



#ifdef __MODEL_slt__
VOID _PLAYMGR_SetContainerType(UINT32 u4ContainerType)
{
    _u4SltContainerFormat = u4ContainerType;
}

VOID _PLAYMGR_SLTVdecFlushCtrl(VOID)
{
    VDEC_DEC_NFY_INFO_T rVDecNfyInfo;

    rVDecNfyInfo.pvTag = 0;
    rVDecNfyInfo.pfDecNfy = _PLAYMGR_VdecNfy;
    VDEC_SetDecNfy(ES0, &rVDecNfyInfo);

    _PLAYMGR_VdecFlushCtrl();
}

VOID _PLAYMGR_SLTVdecWaitCtrlDone(VOID)
{
    _PLAYMGR_VdecWaitCtrlDone();
}

#endif


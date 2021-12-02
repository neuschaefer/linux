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

#if !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)

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
#include "playmgr_ogg.h"
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

#define PLAYMGR_VERIFY_ID(id)                                    \
          do {                                              \
            if (id >= PLAYMGR_MAX_INPUT_SRC) { \
                LOG(3, "%s: Wrong u1SrcId(%d), oops....\n", __FUNCTION__, __LINE__); \
                return; }      \
            else{ \
                LOG(5, "%s: u1SrcId(%d)\n", __FUNCTION__, id); \
            }      \
          } while (0)

#define PLAYMGR_VERIFY_ID_ERROR(id)                                    \
          do {                                              \
            if (id >= PLAYMGR_MAX_INPUT_SRC) { \
                LOG(3, "%s: Wrong u1SrcId(%d), oops....\n", __FUNCTION__, __LINE__); \
                return FALSE; }      \
            else{ \
                LOG(5, "%s u1SrcId(%d)\n", __FUNCTION__, id); \
            }      \
          } while (0)

#define _rCookInfo (_arPlayerInfo[u1SrcId].rCookInfo)

#ifdef FEEDER_FIVE_INSTANCE
#define IsFeederParsingSource(x) ((x == FEEDER_PARSING_SOURCE) || \
                                  (x == FEEDER_PARSING_SOURCE_2) || \
                                  (x == FEEDER_PARSING_AUDIO_3) || \
                                  (x == FEEDER_PARSING_AUDIO_2) || \
                                  (x == FEEDER_PARSING_AUDIO))
#define IsFeederSource(x)        ((x == FEEDER_PROGRAM_SOURCE) || \
                                  (x == FEEDER_PROGRAM_SOURCE_2) || \
                                  (x == FEEDER_AUDIO_SOURCE_3) || \
                                  (x == FEEDER_AUDIO_SOURCE_2) || \
                                  (x == FEEDER_AUDIO_SOURCE))
#else
#define IsFeederParsingSource(x) ((x == FEEDER_PARSING_SOURCE) || \
                                  (x == FEEDER_PARSING_AUDIO))
#define IsFeederSource(x)        ((x == FEEDER_PROGRAM_SOURCE) || \
                                  (x == FEEDER_AUDIO_SOURCE))
#endif

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

extern void _AUD_DspWMAHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo);
static void _PLAYMGR_VdpWaitCtrlDone(UINT8 u1SrcId);
/********************************************************************
  VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
static BOOL _fgPlaymgrInit = FALSE;
PLAYMGR_INFO_T _arPlayerInfo[PLAYMGR_MAX_INPUT_SRC];  // change it to non-static, tmp solution
static PLAYMGR_CALLBACK _arNotifyTbl[PLAYMGR_MAX_INPUT_SRC][PLAYMGR_CALLBACK_MAX];

#ifdef __MODEL_slt__
static UINT32 _u4SltContainerFormat = MEDIA_TYPE_CONTAINER;
static UINT32 _u4SltSubType = MEDIA_CONTNR_SUBTYPE_AVI;
#endif


#if 1//def CC_MT5363
extern const UINT32 _au4CookSampPerFrame[30];
#endif

//static COOK_INFO_T _rCookInfo;

static BOOL _afgIsSeeking[PLAYMGR_MAX_INPUT_SRC] = {FALSE};

BOOL _PLAYMGR_SendUpdStatusNotify(UINT8 u1SrcId, PLAYMGR_STATUS_CMD_T* prStatus);
void _PLAYMGR_FeederCb(MM_NOTIFY_INFO_T *prNfyInfo, UINT32 u4Param);
BOOL _PLAYMGR_SeekPosition(UINT8 u1SrcId, UINT64 u8SeekPos);

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

static ENUM_SWDMX_VDEC_T _PLAYMGR_PlaymgrVdec2Swdmx(UINT8 u1SrcId, PLAYMGR_VDEC_T ePVdec)
{
    BOOL fgNotSupport;

    fgNotSupport = FALSE;

    if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
    {
        if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
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
            #if defined(CC_WMV_MULTITRACK_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MULTITRACK)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            {
            
            }
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
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
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
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
            #if defined(CC_MKV_VP8_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_VP8)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            {
            }
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM)
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
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4)
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
        else if(_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV)
        {
            #if defined(CC_FLV_MJPG_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MJPEG)
            {
                fgNotSupport = TRUE;
            }
            else
            #endif
            #if defined(CC_FLV_MPEG4_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG4)
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
			else
			#endif
            #if defined(CC_FLV_VP6_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_VP6)
            {
                fgNotSupport = TRUE;
            }
			else
            #endif
		    {
            }
        }
    }
    else if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
    {
        if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_ES)
        {
            #if defined(CC_ES_MPEG1_DISABLE) && defined(CC_ES_MPEG2_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG1_MPEG2)
            {
                fgNotSupport = TRUE;
            }
            #endif
        }
        else if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_H264_ES)
        {
            #if defined(CC_ES_H264_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_H264)
            {
                fgNotSupport = TRUE;
            }
            #endif
        }
        else if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VC1_ES)
        {
            #if defined(CC_ES_VC1_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_VC1)
            {
                fgNotSupport = TRUE;
            }
            #endif
        }
        else if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG4_ES)
        {
            #if defined(CC_ES_MPEG4_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_MPEG4)
            {
                fgNotSupport = TRUE;
            }
            #endif
        }
        else if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AVS_ES)
        {
            #if defined(CC_ES_AVS_DISABLE)
            if (ePVdec == PLAYMGR_VDEC_AVS)
            {
                fgNotSupport = TRUE;
            }
            #endif
        }
    }
    else if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_PS)
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
    else if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_TS)
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
        #if defined(CC_TS_MVC_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_MVC)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_TS_MULTITRACK_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_MULTITRACK)
        {
            fgNotSupport = TRUE;
        }
        else
        #endif
        #if defined(CC_TS_AVS_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_AVS)
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
        #if defined(CC_TS_AVS_DISABLE)
        if (ePVdec == PLAYMGR_VDEC_AVS)
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
    case PLAYMGR_VDEC_MVC:
        return ENUM_SWDMX_VDEC_MVC;
    case PLAYMGR_VDEC_MULTITRACK:
        return ENUM_SWDMX_VDEC_MULTITRACK;
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
    case PLAYMGR_VDEC_AVS:
        return ENUM_SWDMX_VDEC_AVS;
    case PLAYMGR_VDEC_VP6:
        return ENUM_SWDMX_VDEC_VP6;
    case PLAYMGR_VDEC_VP8:
        return ENUM_SWDMX_VDEC_VP8;
    default:
        break;
    }

    return ENUM_SWDMX_VDEC_UNKNOWN;
}

static ENUM_SWDMX_ADEC_T _PLAYMGR_PlaymgrAdec2Swdmx(UINT8 u1SrcId, PLAYMGR_ADEC_T ePAdec)
{
    BOOL fgNotSupport;

    fgNotSupport = FALSE;

    if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
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
    UINT8 u1SrcId = 0;
    PLAYMGR_STATUS_CMD_T rPlayStatus;

    if(pv_tag)
    {
        u1SrcId = *(UINT8*)pv_tag;
    }

    rPlayStatus.eCmd = PLAYMGR_SETCMD_UPDATEPLAYTIME;
    UNUSED(_PLAYMGR_SendUpdStatusNotify(u1SrcId, &rPlayStatus));
    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);
}
#endif

static BOOL _PLAYMGR_ReceiveUpdRangeNotify(UINT8 u1SrcId, VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    zMsgSize = sizeof(UINT32);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                            &(_arPlayerInfo[u1SrcId].hPlayMgrRangeCmdQ), 1, X_MSGQ_OPTION_WAIT);

    ASSERT(zMsgSize == sizeof(UINT32));

    if (i4Ret != OSR_OK)
    {
        return FALSE;
    }

    return TRUE;
}


static BOOL _PLAYMGR_SendUpdRangeNotify(UINT8 u1SrcId, UINT32 u4State)
{
    INT32 i4Ret;
    UINT32 u4CurState = u4State;
    SIZE_T zMsgSize = sizeof(UINT32);

    i4Ret = x_msg_q_send(_arPlayerInfo[u1SrcId].hPlayMgrRangeCmdQ, &u4CurState, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        i4Ret = x_msg_q_send(_arPlayerInfo[u1SrcId].hPlayMgrRangeCmdQ, &u4CurState, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);

    return TRUE;

}


static BOOL _PLAYMGR_FlushRangeNotify(UINT8 u1SrcId, UINT32 u4State)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    UINT32 u4Temp;
    
    UNUSED(u4State);
    zMsgSize = sizeof(UINT32);

    while(x_msg_q_receive(&u2MsgQIdx, (VOID*)&u4Temp, &zMsgSize,
                            &(_arPlayerInfo[u1SrcId].hPlayMgrRangeCmdQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    VERIFY(x_msg_q_num_msgs(_arPlayerInfo[u1SrcId].hPlayMgrRangeCmdQ, &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt == 0);

    return TRUE;
}


static VOID _PLAYMGR_UpdateRangeLoop(VOID *pvArg)
{
    BOOL fgRet;
    UINT8 u1SrcId;
    UINT32 u4SeekPts = 0;

    u1SrcId = *((UINT8*)(pvArg));    
    PLAYMGR_VERIFY_ID(u1SrcId);
    while (1)
    {
        fgRet = _PLAYMGR_ReceiveUpdRangeNotify(u1SrcId, &u4SeekPts);
        if(!fgRet)
        {
            LOG(0, "_PLAYMGR_ReceiveUpdRangeNotify fail\n");
        }
        LOG(5, "_PLAYMGR_UpdateRangeLoop Seek pts = 0x%08x\n",
            u4SeekPts);
        if (fgRet && (_arPlayerInfo[u1SrcId].pfnPlaymgrUpdateRange != 0))
        {
            LOG(5, "_PLAYMGR_UpdateRangeLoop(%d) seek pts = 0x%08x\n", u1SrcId, u4SeekPts);
            if(!_arPlayerInfo[u1SrcId].pfnPlaymgrUpdateRange(&_arPlayerInfo[u1SrcId], u4SeekPts))
            {
                LOG(3, "pfnPlaymgrUpdateRange fail\n");
            }
        }
    }
}

static VOID _PLAYMGR_ABDoneThread(VOID *pvArg)
{
    UINT8 u1SrcId;

    u1SrcId = *((UINT8*)(pvArg));    
    PLAYMGR_VERIFY_ID(u1SrcId);
    
    while (1)
    {
        PLAYMGR_AB_POS_INFO_T* prA  = &_arPlayerInfo[u1SrcId].rABInfo.rA;
        
        VERIFY(x_sema_lock(_arPlayerInfo[u1SrcId].hPlaymgrABDoneSema, X_SEMA_OPTION_WAIT) == OSR_OK);       
        
        LOG(3, "_PLAYMGR_ABDoneThread %d %d\n", u1SrcId, (((UINT32)prA->ui8_pts_info)/90));

        PLAYMGR_SeekTime(u1SrcId, (((UINT32)prA->ui8_pts_info)/90));
    }
}

void _PLAYMGR_FeederCb(MM_NOTIFY_INFO_T *prNfyInfo, UINT32 u4Param)
{
    UINT8 u1SrcId = 0;
    LOG(5, "_PLAYMGR_FeederCb\n");
    if (!prNfyInfo)
    {
        return;
    }
    if (prNfyInfo->u4Condition < (UINT32)PLAYMGR_CALLBACK_MAX)
    {
        for(;u1SrcId < PLAYMGR_MAX_INPUT_SRC;u1SrcId++)
        {
            if((_arPlayerInfo[u1SrcId].fgOpen) &&
               (prNfyInfo->u4SrcType == (UINT32)_arPlayerInfo[u1SrcId].eFeederSrc))
            {
                break;               
            }
        }

        if(u1SrcId < PLAYMGR_MAX_INPUT_SRC)
        {
            if (_arNotifyTbl[u1SrcId][prNfyInfo->u4Condition])
            {
                _arNotifyTbl[u1SrcId][prNfyInfo->u4Condition](
                    u1SrcId,
                    (PLAYMGR_CALLBACK_TYPE_T)prNfyInfo->u4Condition,
                    0, 0);
            }
        }
    }
    else
    {
        LOG(5, "_PLAYMGR_FeederCb unknow eCallbackType %d\n", prNfyInfo->u4Condition);
    }
    UNUSED(prNfyInfo);
    UNUSED(u4Param);
    return;
}

static BOOL _PLAYMGR_UpdateRangeCb(VOID* pv_nfy_tag,
                                   MM_RANGE_COND_T  e_nfy_cond,
                                   UINT32           ui4_data_1,
                                   UINT32           ui4_data_2,
                                   UINT32           ui4_data_3)
{
    UINT8 u1SrcId = ui4_data_3;
    
    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    
    if (e_nfy_cond == MM_RANGE_DONE)
    {
        LOG(5, "_PLAYMGR_UpdateRangeCb Src Id = %d\n", u1SrcId);
        LOG(5, "Update RangeCb MM_RANGE_DONE next pts = 0x%08x\n",
            ui4_data_2);        
        if (_arPlayerInfo[u1SrcId].pfnPlaymgrFreeRange != 0)
        {
            if (!_arPlayerInfo[u1SrcId].pfnPlaymgrFreeRange((VOID*)&_arPlayerInfo[u1SrcId], (UINT32)ui4_data_1))
            {
                LOG(1, "pfnPlaymgrFreeRange fail\n");
            }
        }
        //_arPlayerInfo[u1SrcId].u4SeekPts = ui4_data_2;
        UNUSED(_PLAYMGR_FlushRangeNotify(u1SrcId, ui4_data_2));
        UNUSED(_PLAYMGR_SendUpdRangeNotify(u1SrcId, ui4_data_2));
    }
    else if (e_nfy_cond == MM_RANGE_ABORT)
    {
        if (_arPlayerInfo[u1SrcId].pfnPlaymgrFreeRange != 0)
        {
            if (!_arPlayerInfo[u1SrcId].pfnPlaymgrFreeRange((VOID*)&_arPlayerInfo[u1SrcId], (UINT32)ui4_data_1))
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
        UNUSED(FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_PVR_RANGE_NOTIFY,
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
    UINT8 u1SrcId = (UINT8)u4Data1;
    PLAYMGR_VERIFY_ID(u1SrcId);

    if (eNfyCond == SWDMX_COND_CTRL_DONE)
    {
        VERIFY (x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrDmxCtrlSema) == OSR_OK);
    }
    else
    {
        LOG(3, "_PLAYMGR_DmxNfy unknow ctrl %d\n", (UINT32)eNfyCond);
    }
    UNUSED(pvNfyTag);
    UNUSED(u4Data1);
    UNUSED(u4Data2);
}


static void _PLAYMGR_DmxFlushCtrl(UINT8 u1SrcId)
{
    INT32 i4Ret;
    PLAYMGR_VERIFY_ID(u1SrcId);
    i4Ret = x_sema_lock(_arPlayerInfo[u1SrcId].hPlayMgrDmxCtrlSema, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}


static void _PLAYMGR_DmxWaitCtrlDone(UINT8 u1SrcId)
{
    INT32 i4Ret;
    PLAYMGR_VERIFY_ID(u1SrcId);
    i4Ret = x_sema_lock_timeout(_arPlayerInfo[u1SrcId].hPlayMgrDmxCtrlSema, 5000);
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
#ifndef __MODEL_slt__
    UINT8 u1SrcId = *((UINT8*)pvNfyTag);
#else
    UINT8 u1SrcId = 0;
#endif
    PLAYMGR_STATUS_T* prMediaStatus;

    PLAYMGR_VERIFY_ID(u1SrcId);

    prMediaStatus = &_arPlayerInfo[u1SrcId].rPlayerStatus;

    if (eNfyCond == VDEC_COND_CTRL_DONE)
    {
        VERIFY (x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrVdecCtrlSema) == OSR_OK);
        //VERIFY (x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrVdec2CtrlSema) == OSR_OK);
    }
    else if ((eNfyCond == VDEC_COND_DECODE_STATUS_CHG) &&
             (u4Data1  == (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT))
    {
        LOG(3, "_PLAYMGR_VdecNfy codec not support\n");
        if (!FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_ERROR, 1))
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
            VERIFY(x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrEosMutex) == OSR_OK);
            LOG(3, "_PLAYMGR_VdecNfy not support FeederNotifyHandle FileEnd\n");
            if(!FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_FILEEND, 0))
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


static void _PLAYMGR_VdecFlushCtrl(UINT8 u1SrcId)
{
    INT32 i4Ret;
    PLAYMGR_VERIFY_ID(u1SrcId);
    i4Ret = x_sema_lock(_arPlayerInfo[u1SrcId].hPlayMgrVdecCtrlSema, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}


static void _PLAYMGR_VdecWaitCtrlDone(UINT8 u1SrcId)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock_timeout(_arPlayerInfo[u1SrcId].hPlayMgrVdecCtrlSema, 5000);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "_PLAYMGR_VdecWaitCtrlDone timeout\n");
    }
}
/*
static void _PLAYMGR_Vdec2FlushCtrl(UINT8 u1SrcId)
{
    INT32 i4Ret;
    PLAYMGR_VERIFY_ID(u1SrcId);
    i4Ret = x_sema_lock(_arPlayerInfo[u1SrcId].hPlayMgrVdec2CtrlSema, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}


static void _PLAYMGR_Vdec2WaitCtrlDone(UINT8 u1SrcId)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock_timeout(_arPlayerInfo[u1SrcId].hPlayMgrVdecCtrlSema, 5000);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "_PLAYMGR_Vdec2WaitCtrlDone timeout\n");
    }
}
*/
static VOID _PLAYMGR_AdecNfy (void *          pvNfyTag,
                              UINT8           u1DecId,
                              AUD_COND_NFY_T  eNfyCond,
                              UINT32          u4Data1,
                              UINT32          u4Data2)
{
    UINT32 i;
    UINT8 u1SrcId = 0xFF;
    PLAYMGR_STATUS_T* prMediaStatus;
    
    for( i=0;i< PLAYMGR_MAX_INPUT_SRC; i++)
    {
        if((_arPlayerInfo[i].fgOpen) &&
           ((_arPlayerInfo[i].rPlayerStatus.fgEnAudStrm) ||
            (_arPlayerInfo[i].rPlayerStatus.fgDisAudStrm))&&
           (_arPlayerInfo[i].u1AdecId == u1DecId))
        {
            u1SrcId = i;
            break;
        }
    }
    if(u1SrcId >= PLAYMGR_MAX_INPUT_SRC)
    {
        LOG(3, "_PLAYMGR_AdecNfy No audio\n");
        return;
    }

    prMediaStatus = &_arPlayerInfo[u1SrcId].rPlayerStatus;
    if (eNfyCond == AUD_COND_MM_EOS)
    {
        LOG(0, "_PLAYMGR_AdecNfy(%d) AUD_COND_MM_EOS\n", u1SrcId);
        if (prMediaStatus == NULL)
        {
            LOG(1, "_PLAYMGR_AdecNfy prMediaStatus null\n");
            return;
        }
        VERIFY(x_sema_lock(_arPlayerInfo[u1SrcId].hPlayMgrEosMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        prMediaStatus->fgGotAudEos = TRUE;
        if ((!prMediaStatus->fgSendEos) &&
                ((prMediaStatus->fgEnVidStrm && prMediaStatus->fgGotVidEos) ||
                 (!prMediaStatus->fgEnVidStrm)) &&
                ((prMediaStatus->fgEnAudStrm && prMediaStatus->fgGotAudEos) ||
                 (!prMediaStatus->fgEnAudStrm)))
        {
            prMediaStatus->fgSendEos = TRUE;
            VERIFY(x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrEosMutex) == OSR_OK);
            LOG(3, "_PLAYMGR_AdecNfy FeederNotifyHandle FileEnd\n");
            if (!FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_FILEEND, 0))
            {
                LOG(3, "_PLAYMGR_AdecNfy FeederNotifyHandle fail\n");
            }
        }
        else
        {
            VERIFY(x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrEosMutex) == OSR_OK);
        }
    }
    else if (eNfyCond == AUD_COND_CTRL_DONE)
    {
        LOG(3, "_PLAYMGR_AdecNfy(%d) _PLAYMGR_AUD_CTRL_DONE\n", u1SrcId);
        VERIFY (x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrAdecCtrlSema) == OSR_OK);
    }
    else if ((eNfyCond == AUD_COND_AUD_DECODE_STATUS_CHG)
             && (u4Data1 == AUD_DECODE_NOT_SUPPORT))
    {
        LOG(3, "_PLAYMGR_AdecNfy(%d) _PLAYMGR_AUD_CTRL_DONE AUD_DECODE_NOT_SUPPORT\n", u1SrcId);
        if (!FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_ERROR, 2))
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
            VERIFY(x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrEosMutex) == OSR_OK);
            LOG(3, "_PLAYMGR_AdecNfy not support FeederNotifyHandle FileEnd\n");
            if(!FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_FILEEND, 0))
            {
                LOG(3, "_PLAYMGR_AdecNfy not support FeederNotifyHandle fail\n");
            }
        }
        VERIFY (x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrAdecCtrlSema) == OSR_OK);
    }
    else
    {
        LOG(3, "_PLAYMGR_AdecNfy(%d) unknow ctrl %d, u4Data1 %d\n",
            u1SrcId, (UINT32)eNfyCond, u4Data1);
    }
    UNUSED(pvNfyTag);
    UNUSED(u1DecId);
    UNUSED(u4Data2);
}

static void _PLAYMGR_AdecFlushCtrl(UINT8 u1SrcId)
{
    INT32 i4Ret;
    PLAYMGR_VERIFY_ID(u1SrcId);
    i4Ret = x_sema_lock(_arPlayerInfo[u1SrcId].hPlayMgrAdecCtrlSema, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}

static void _PLAYMGR_AdecWaitCtrlDone(UINT8 u1SrcId)
{
    INT32 i4Ret;
    PLAYMGR_VERIFY_ID(u1SrcId);
    i4Ret = x_sema_lock_timeout(_arPlayerInfo[u1SrcId].hPlayMgrAdecCtrlSema, 5000);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "_PLAYMGR_AdecWaitCtrlDone timeout\n");
    }
}

static VOID _PLAYMGR_VdpNfy(UINT32 u4VdpId,
    VDP_COND_T eCond, BOOL fgFastForward, UINT32 u4AttachedSrcId)
{
    PLAYMGR_STATUS_T* prMediaStatus;
    PLAYMGR_VERIFY_ID(u4VdpId);

    UNUSED(fgFastForward);
    prMediaStatus = &_arPlayerInfo[u4AttachedSrcId].rPlayerStatus;

    LOG(0, "_PLAYMGR_VdpNfy\n");
    if (eCond == VDP_COND_EOS)
    {
        LOG(3, "_PLAYMGR_VdpNfy VDP_COND_EOS\n");

        if (prMediaStatus == NULL)
        {
            LOG(1, "_PLAYMGR_VdpNfy prMediaStatus null\n");
            return;
        }
        VERIFY(x_sema_lock(_arPlayerInfo[u4AttachedSrcId].hPlayMgrEosMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        prMediaStatus->fgGotVidEos = TRUE;
        if ((!prMediaStatus->fgSendEos) &&
                ((prMediaStatus->fgEnVidStrm && prMediaStatus->fgGotVidEos) ||
                 (!prMediaStatus->fgEnVidStrm)) &&
                ((prMediaStatus->fgEnAudStrm && prMediaStatus->fgGotAudEos) ||
                 (!prMediaStatus->fgEnAudStrm)))
        {
            prMediaStatus->fgSendEos = TRUE;
            VERIFY(x_sema_unlock(_arPlayerInfo[u4AttachedSrcId].hPlayMgrEosMutex) == OSR_OK);
            LOG(3, "_PLAYMGR_VdpNfy FeederNotifyHandle FileEnd\n");
            //FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_FILEEND, 0);
            FeederNotifyHandle(_arPlayerInfo[u4AttachedSrcId].eFeederSrc, FEEDER_NOTIFY_FILEEND, 0);
        }
        else
        {
            VERIFY(x_sema_unlock(_arPlayerInfo[u4AttachedSrcId].hPlayMgrEosMutex) == OSR_OK);
        }
    }
    else if (eCond == VDP_COND_RANGE_AB_DONE)
    {
        VERIFY (x_sema_unlock(_arPlayerInfo[u4VdpId].hPlaymgrABDoneSema) == OSR_OK);
    }
    else
    {
        LOG(3, "_PLAYMGR_VdpNfy unknow condition %d\n", (UINT32)eCond);
    }
}

static VOID _PLAYMGR_VdpStepDoneNfy(UINT32 u4VdpId, UINT32 u4PTS,
    UINT32 u4Ret, UINT32 u4AttachedSrcId)
{
    UNUSED(u4VdpId);
    UNUSED(u4PTS);
    UNUSED(u4Ret);
    LOG(3, "_PLAYMGR_VdpStepDoneNfy\n");

    PLAYMGR_VERIFY_ID(u4AttachedSrcId);

    STC_StopStc(_arPlayerInfo[u4AttachedSrcId].u1StcId);
    
    VERIFY (x_sema_unlock(_arPlayerInfo[u4AttachedSrcId].hPlayMgrVdpCtrlSema) == OSR_OK);

    // notify timer
    _PLAYMGR_StatusNfyTimer(0, &u4AttachedSrcId);
    
    
    // ASYNC FOR XUNLEI
    if (_arPlayerInfo[u4AttachedSrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_XUNLEI)
    {
        _PLAYMGR_VdpWaitCtrlDone(u4AttachedSrcId);

        // step done.
        FeederNotifyHandle(_arPlayerInfo[u4AttachedSrcId].eFeederSrc, FEEDER_NOTIFY_SEEK_END, (_arPlayerInfo[u4AttachedSrcId].u4SeekPts/90));

        // ToDo: we don't need pause?
        VDEC_Pause(_arPlayerInfo[u4AttachedSrcId].u1VdecId);
        if ((_arPlayerInfo[u4AttachedSrcId].rPlayerStatus.u2EnVTrackNum < (UINT16)PLAYMGR_MAX_VID_TRACK) &&
            ((_arPlayerInfo[u4AttachedSrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u4AttachedSrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
             (_arPlayerInfo[u4AttachedSrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u4AttachedSrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK)))
        {
            VDEC_Pause(_arPlayerInfo[u4AttachedSrcId].u1Vdec2Id);
        }
    }

}

static void _PLAYMGR_VdpFlushCtrl(UINT8 u1SrcId)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock(_arPlayerInfo[u1SrcId].hPlayMgrVdpCtrlSema, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}

static void _PLAYMGR_VdpWaitCtrlDone(UINT8 u1SrcId)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock_timeout(_arPlayerInfo[u1SrcId].hPlayMgrVdpCtrlSema, 50000);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "_PLAYMGR_VdpWaitCtrlDone timeout\n");
    }
}

static BOOL _PLAYMGR_ReceiveUpdStatusNotify(UINT8 u1SrcId, VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    zMsgSize = sizeof(PLAYMGR_STATUS_CMD_T);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                            &(_arPlayerInfo[u1SrcId].hPlayMgrPlaybackStatusCmdQ), 1, X_MSGQ_OPTION_WAIT);

    ASSERT(zMsgSize == sizeof(PLAYMGR_STATUS_CMD_T));

    if (i4Ret != OSR_OK)
    {
        return FALSE;
    }

    return TRUE;
}


BOOL _PLAYMGR_SendUpdStatusNotify(UINT8 u1SrcId, PLAYMGR_STATUS_CMD_T* prStatus)
{
    INT32 i4Ret;
    SIZE_T zMsgSize = sizeof(PLAYMGR_STATUS_CMD_T);

    if((u1SrcId >= PLAYMGR_MAX_INPUT_SRC) ||
       (!_arPlayerInfo[u1SrcId].fgOpen))
    {
        //ASSERT(0);
        return FALSE;
    }

    i4Ret = x_msg_q_send(_arPlayerInfo[u1SrcId].hPlayMgrPlaybackStatusCmdQ, prStatus, zMsgSize, 255);
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
                                &(_arPlayerInfo[u1SrcId].hPlayMgrPlaybackStatusCmdQ), 1, X_MSGQ_OPTION_WAIT);
        UNUSED(i4Ret);
        //x_thread_delay(10);
        i4Ret = x_msg_q_send(_arPlayerInfo[u1SrcId].hPlayMgrPlaybackStatusCmdQ, prStatus, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);

    return TRUE;

}


BOOL _PLAYMGR_SubtitleCb(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
                         UINT32 u4Data, const void* pvTag)
{
    UINT8 u1SrcId = 0;
    BOOL fgRet = TRUE;
    UINT32 u4DataAddr;
    DMX_NOTIFY_INFO_PES_T* prNotifyInfo;
    PLAYMGR_STATUS_CMD_T rStatus;

    if(pvTag)
    {
        u1SrcId = *(UINT8*)pvTag;
    }
    
    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    
    switch (eCode)
    {
    case DMX_NOTIFY_CODE_PES:
        prNotifyInfo = (DMX_NOTIFY_INFO_PES_T*)u4Data;
        //ASSERT(prNotifyInfo != NULL);
        u4DataAddr = VIRTUAL(prNotifyInfo->u4DataAddr);
        //ASSERT(u4DataAddr < 0x10000000);
        rStatus.eCmd = PLAYMGR_SETCMD_UPDATESUBTITLE;
        rStatus.u4Param1 = u4DataAddr;
        rStatus.u4Param2 = (UINT32)_arPlayerInfo[u1SrcId].pucSubBuf;
        rStatus.u4Param3 = prNotifyInfo->u4DataSize;
        rStatus.u4Param4 = (UINT32)prNotifyInfo->u1SerialNumber;
        UNUSED(_PLAYMGR_SendUpdStatusNotify(u1SrcId, &rStatus));
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
    UINT8 u1SrcId;
    PLAYMGR_STATUS_CMD_T rNotify;
    //UINT32 u4CurDisplayTime = 0;

    u1SrcId = *((UINT8*)(pvArg));
    
    PLAYMGR_VERIFY_ID(u1SrcId);
    
    while (1)
    {
        _PLAYMGR_ReceiveUpdStatusNotify(u1SrcId, &rNotify);

        // Do subtitle decoding and callback to LG's AP.
        switch (rNotify.eCmd)
        {
        case PLAYMGR_SETCMD_UPDATEPLAYTIME:
            if (_arNotifyTbl[u1SrcId][PLAYMGR_CALLBACK_ELAPSEDTIME])
            {
                _arNotifyTbl[u1SrcId][PLAYMGR_CALLBACK_ELAPSEDTIME](
                    u1SrcId,
                    PLAYMGR_CALLBACK_ELAPSEDTIME,
                    0, 0);
            }
            break;
            // Draw Subtitle
        case PLAYMGR_SETCMD_UPDATESUBTITLE:
            _PLAYMGR_DrawSubtitle(u1SrcId, &rNotify, &_arPlayerInfo[u1SrcId]);
            break;
        default:
            break;
        }
    }
}


static UINT32 _PLAYMGR_DataQuery(UINT8 u1SrcId, FeederSourceType eSrcType, FEEDER_BUF_INFO_T *pQryBufInfo,
                                 FEEDER_REQ_DATA_T* pt_ReqData)
{
    UINT32 u4Read;
    
#ifdef __KERNEL__
    MM_CALLBACK_INFO_T rCallbackInfo;
    PLAYMGR_RECEIVE_DATA_T rReceive;
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize = sizeof(PLAYMGR_RECEIVE_DATA_T);

    if ((u1SrcId >= PLAYMGR_MAX_INPUT_SRC) ||
       (!_arPlayerInfo[u1SrcId].fgOpen))
    {
        LOG(3, "%s: Wrong u1SrcId(%d), oops....\n", __FUNCTION__, __LINE__);
        return 0;
    }
    
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

    VERIFY (x_sema_lock(_arPlayerInfo[u1SrcId].hPlayMgrSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _CB_PutEvent(CB_MTMPLAYER_TRIGGER, sizeof(MM_CALLBACK_INFO_T), &rCallbackInfo);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, (VOID *)&rReceive, &zMsgSize,
                            &(_arPlayerInfo[u1SrcId].hPlayMgrDataQ), 1, X_MSGQ_OPTION_WAIT);

    VERIFY (x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrSema) == OSR_OK);

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

    if ((u1SrcId >= PLAYMGR_MAX_INPUT_SRC) ||
       (!_arPlayerInfo[u1SrcId].fgOpen))
    {
        LOG(3, "%s: Wrong u1SrcId(%d), oops....\n", __FUNCTION__, __LINE__);
        return 0;
    }

    if (IsFeederParsingSource(eSrcType))
    {
        hFile = _arPlayerInfo[u1SrcId].hHdrHandle;
    }
    else
    {
        hFile = _arPlayerInfo[u1SrcId].hFileHandle;
    }

    u4Read = 0;

    if(!pt_ReqData)
    {
        LOG(1, "pt_ReqData null\n");
        return 0;
    }

    pt_ReqData->fgEof = FALSE;

    if ((IsFeederParsingSource(eSrcType)) ||
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

    if (IsFeederSource(eSrcType))
    /*
    if ((eSrcType != FEEDER_PARSING_SOURCE) &&
        (eSrcType != FEEDER_PARSING_AUDIO))
    */
    {                                       //read AV data, set offset here
        x_fm_lseek(hFile, (INT64)pt_ReqData->u8FilePos, FM_SEEK_BGN, &u8CurPos);
    }
    x_fm_read(hFile, (VOID *)pt_ReqData->u4WriteAddr, u4FeedingSize1, &u4Cnt);
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    HalFlushDCacheMultipleLine(pt_ReqData->u4WriteAddr, u4FeedingSize1);
    #elif defined(CC_MT5365) || defined(CC_MT5395)
    HalFlushInvalidateDCacheMultipleLine(pt_ReqData->u4WriteAddr, u4FeedingSize1);
    #else
    HalFlushInvalidateDCache();
    #endif
    if (u4FeedingSize2)
    {
        x_fm_read(hFile, (VOID *)pQryBufInfo->u4StartAddr, u4FeedingSize2, &u4Read);
        #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        HalFlushDCacheMultipleLine(pQryBufInfo->u4StartAddr, u4FeedingSize2);
        #elif defined(CC_MT5365) || defined(CC_MT5395)
        HalFlushInvalidateDCacheMultipleLine(pQryBufInfo->u4StartAddr, u4FeedingSize2);
        #else
        HalFlushInvalidateDCache();
        #endif
    }
    u4Read += u4Cnt;
#endif

    return (UINT32)u4Read;
}

static INT64 _PLAYMGR_SeekPos(UINT8 u1SrcId, FeederSourceType eSrcType, MM_FILE_OPS_T *prFileInfo)
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

    if ((u1SrcId >= PLAYMGR_MAX_INPUT_SRC) ||
        (!_arPlayerInfo[u1SrcId].fgOpen))
    {
        LOG(3, "%s: Wrong u1SrcId(%d), oops....\n", __FUNCTION__, __LINE__);
        return 0;
    }
    
    x_memset(&rCallbackInfo, 0, sizeof(rCallbackInfo));
    rCallbackInfo.rMMNotify.u4SrcType = (UINT32)eSrcType;
    rCallbackInfo.rMMNotify.u4Condition = FEEDER_NOTIFY_NEEDDATA;
    rCallbackInfo.rFileInfo.u8Offset = u8Offset;
    rCallbackInfo.rFileInfo.u1Whence = u1Whence;

    VERIFY (x_sema_lock(_arPlayerInfo[u1SrcId].hPlayMgrSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _CB_PutEvent(CB_MTMPLAYER_TRIGGER, sizeof(MM_CALLBACK_INFO_T), &rCallbackInfo);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, (VOID *)&rReceive, &zMsgSize,
                            &(_arPlayerInfo[u1SrcId].hPlayMgrDataQ), 1, X_MSGQ_OPTION_WAIT);

    VERIFY (x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrSema) == OSR_OK);

    u8CurPos = rReceive.u8FilePos;

    ASSERT(zMsgSize == sizeof(PLAYMGR_RECEIVE_DATA_T));

    if (i4Ret != OSR_OK)
    {
        return 0;
    }
#else
    HANDLE_T hFile;

    if ((u1SrcId >= PLAYMGR_MAX_INPUT_SRC) ||
        (!_arPlayerInfo[u1SrcId].fgOpen))
    {
        LOG(3, "%s: Wrong u1SrcId(%d), oops....\n", __FUNCTION__, __LINE__);
        return 0;
    }
    
    if (IsFeederParsingSource(eSrcType))
    {
        hFile = _arPlayerInfo[u1SrcId].hHdrHandle;
    }
    else
    {
        hFile = _arPlayerInfo[u1SrcId].hFileHandle;
    }

    x_fm_lseek(hFile, (INT64)u8Offset, u1Whence, &u8CurPos);
#endif

    return (INT64)u8CurPos;
}

BOOL _PLAYMGR_ReceiveNotify(UINT8 u1SrcId, VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    
    zMsgSize = sizeof(UINT32);

    /*i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                            &(_arPlayerInfo[u1SrcId].hPlayMgrCmdQ), 1, X_MSGQ_OPTION_WAIT);*/

    i4Ret = x_msg_q_receive_timeout (&u2MsgQIdx, pvData, &zMsgSize,
                            &(_arPlayerInfo[u1SrcId].hPlayMgrCmdQ), 1, PLAYMGR_NOTIFICATION_TIMEOUT);

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


BOOL _PLAYMGR_SendNotify(UINT8 u1SrcId, UINT32 u4State)
{
    INT32 i4Ret;
    UINT32 u4CurState = u4State;
    SIZE_T zMsgSize = sizeof(UINT32);

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    
    i4Ret = x_msg_q_send(_arPlayerInfo[u1SrcId].hPlayMgrCmdQ, &u4CurState, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        i4Ret = x_msg_q_send(_arPlayerInfo[u1SrcId].hPlayMgrCmdQ, &u4CurState, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);

    return TRUE;

}

VOID _PLAYMGR_FeederReset(UINT8 u1SrcId)
{
    FeederSourceType eSourceType=FEEDER_SOURCE_INVALID;
    FEEDER_QUERY_DATA rSetQuery;

    x_memset(&rSetQuery,0,sizeof(FEEDER_QUERY_DATA));
	rSetQuery.pfnSource = _PLAYMGR_DataQuery;
	rSetQuery.pfnSeekPos = _PLAYMGR_SeekPos;
	rSetQuery.pfnConsume = NULL;
	rSetQuery.u4QuerySize = 0;

	eSourceType=(FeederSourceType)_arPlayerInfo[u1SrcId].rMediaFeeder.h_feeder;	
	FeederOpen(eSourceType);
	FeederStart(eSourceType);
	FeederSetAppQueryMode(eSourceType, FEEDER_PULL_MODE);
    rSetQuery.u1PlaymgrAttachedId = u1SrcId;
	rSetQuery.eDataType = eSourceType;
	FeederRegisterPullDataFunc(eSourceType, &rSetQuery);
	FeederSetRing(eSourceType,FALSE);
	FeederSetInfo(eSourceType, FEEDER_MM_USE, FALSE);

	eSourceType=(FeederSourceType)_arPlayerInfo[u1SrcId].eFeederSrc;
	FeederOpen(eSourceType);
	FeederStart(eSourceType);
	FeederSetAppQueryMode(eSourceType, FEEDER_PULL_MODE);
	rSetQuery.u1PlaymgrAttachedId = u1SrcId;
	rSetQuery.eDataType = eSourceType;
	FeederRegisterPullDataFunc(eSourceType, &rSetQuery);
	FeederSetRing(eSourceType,TRUE);
	FeederSetInfo(eSourceType, FEEDER_MM_USE, FALSE);
	return ;
}

static VOID _PLAYMGR_SeekFinNotify(UINT32 u4PTS, UINT8 u1SrcId)
{
    LOG(3, "_PLAYMGR_SeekFinNotify u4PTS %d\n", (u4PTS/90));
    
    PLAYMGR_VERIFY_ID(u1SrcId);
    
    VERIFY (x_sema_unlock(_arPlayerInfo[u1SrcId].hPlayMgrSemaFinSeek) == OSR_OK);
    FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_SEEK_END, (u4PTS/90));

    _arPlayerInfo[u1SrcId].u4SeekPts = u4PTS;
    VDEC_RegRenderPtsCb(_arPlayerInfo[u1SrcId].u1VdecId, NULL, u1SrcId);

    // notify timer
    _PLAYMGR_StatusNfyTimer(0, &u1SrcId);
    _afgIsSeeking[u1SrcId] = FALSE;
}

#if 0  // megaa SPU
UCHAR *_pucSubOutBuf;
#endif


BOOL _PLAYMGR_Init(VOID)
{
    FEEDER_QUERY_DATA rSetQuery;
    INT32 i4Ret = OSR_OK;
    PLAYMGR_INFO_T *prPlayerInfo;

    if (!_fgPlaymgrInit)
    {
        char acBuf[32];
        UINT8 i = 0;
        _fgPlaymgrInit = TRUE;
        
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

        for (i = 0; i < PLAYMGR_MAX_INPUT_SRC; i++)
        {
            prPlayerInfo = _arPlayerInfo;
            prPlayerInfo += (UINT32)i;
            x_memset((void*)prPlayerInfo, 0, sizeof(PLAYMGR_INFO_T));
            _arPlayerInfo[i].u1SrcId = i;
            _arPlayerInfo[i].u1StcId = i;
            _arPlayerInfo[i].u1VdecId = i;
            // todo, to get available vdec resource form decoder
            _arPlayerInfo[i].u1Vdec2Id = i+1;
            // todo, define audio id
            _arPlayerInfo[i].u1AdecId = i;
            _arPlayerInfo[i].u1B2rId = i;
            //_arPlayerInfo[i].u1VdpId = VDP_1;

            if(_arPlayerInfo[i].u1VdecId >= VDEC_MAX_ES)
            {
                // need to define vdec id
                _arPlayerInfo[i].u1VdecId = 0;
            }

            x_memset(acBuf, 0, 32 * sizeof(char));
            x_snprintf(acBuf, sizeof(acBuf), "PLAYMGRCMDQ%d", i);

        // create msg queue
            VERIFY(x_msg_q_create(&_arPlayerInfo[i].hPlayMgrCmdQ, acBuf, sizeof(UINT32),
                              PLAYMGR_CMD_Q_SIZE) == OSR_OK);

            x_memset(acBuf, 0, 32 * sizeof(char));
            x_snprintf(acBuf, sizeof(acBuf), "PLAYMGRStatQ%d", i);

            VERIFY(x_msg_q_create(&_arPlayerInfo[i].hPlayMgrStatusQ, acBuf, sizeof(PLAYMGR_STATUS_CMD_T),
                              PLAYMGR_CMD_Q_SIZE) == OSR_OK);

#ifdef __KERNEL__
            x_memset(acBuf, 0, 32 * sizeof(char));
            x_snprintf(acBuf, sizeof(acBuf), "PMGRDataQ%d", i);

            VERIFY(x_msg_q_create(&_arPlayerInfo[i].hPlayMgrDataQ, acBuf, sizeof(PLAYMGR_RECEIVE_DATA_T),
                              PLAYMGR_DATA_Q_SIZE) == OSR_OK);
#endif

#ifdef PLAYMGR_ASYNC_MODE
            x_memset(acBuf, 0, 32 * sizeof(char));
            x_snprintf(acBuf, sizeof(acBuf), "PMGRQTask %d", i);

            i4Ret = x_thread_create(&_arPlayerInfo[i].hPlayMgrThread, acBuf,
                                PLAYMGR_STACK_SIZE, PLAYMGR_THREAD_PRIORITY,
                                _PLAYMGR_MainLoop, 0, NULL);
#endif
            if (i4Ret != OSR_OK)
            {
                LOG(5, "_FeederOpen: Create the query thread failed \n");
                return FALSE;
            }

            x_memset(acBuf, 0, 32 * sizeof(char));
            x_snprintf(acBuf, sizeof(acBuf), "PLAYMGRRCMDQ %d", i);

            VERIFY(x_msg_q_create(&_arPlayerInfo[i].hPlayMgrRangeCmdQ, acBuf, sizeof(UINT32),
                              PLAYMGR_CMD_Q_SIZE) == OSR_OK);

            x_memset(acBuf, 0, 32 * sizeof(char));
            x_snprintf(acBuf, sizeof(acBuf), "PLAYMGRSCMDQ %d", i);

        // Create for playback status update and drawing subtitle.
            VERIFY(x_msg_q_create(&_arPlayerInfo[i].hPlayMgrPlaybackStatusCmdQ, acBuf, sizeof(PLAYMGR_STATUS_CMD_T),
                              PLAYMGR_CMD_Q_SIZE) == OSR_OK);

            VERIFY (x_sema_create(&_arPlayerInfo[i].hPlayMgrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

            VERIFY (x_sema_create(&_arPlayerInfo[i].hPlayMgrSemaFinSeek, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

            VERIFY(x_sema_create(&_arPlayerInfo[i].hPlayMgrDmxCtrlSema,
                             X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

            VERIFY(x_sema_create(&_arPlayerInfo[i].hPlayMgrVdecCtrlSema,
                             X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

//            VERIFY(x_sema_create(&_arPlayerInfo[i].hPlayMgrVdec2CtrlSema,
//                             X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
            
            VERIFY(x_sema_create(&_arPlayerInfo[i].hPlayMgrAdecCtrlSema,
                             X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

            VERIFY(x_sema_create(&_arPlayerInfo[i].hPlayMgrVdpCtrlSema,
                             X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

            VERIFY(x_sema_create(&_arPlayerInfo[i].hPlayMgrEosMutex, X_SEMA_TYPE_MUTEX,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);

        // according to thread_lib.h
        // #define THREAD_NAME_LEN         16
        // thread name should not over this length. or we will leak '\0' character
        // create update range thread
            x_memset(acBuf, 0, 32 * sizeof(char));
            x_snprintf(acBuf, sizeof(acBuf), "PmgrRge%d", i);

            VERIFY(x_thread_create(&_arPlayerInfo[i].hPlaymgrUpdateRangeThread,
                                   acBuf, PLAYMGR_STACK_SIZE, PLAYMGR_THREAD_PRIORITY,
                                   (x_os_thread_main_fct)_PLAYMGR_UpdateRangeLoop, 
                                   sizeof(UINT8), 
                                   (void*)&_arPlayerInfo[i].u1SrcId) == OSR_OK);

            x_memset(acBuf, 0, 32 * sizeof(char));
            x_snprintf(acBuf, sizeof(acBuf), "PmgrUpdSts%d", i);
            
            // create status update and subtitle thread
            VERIFY(x_thread_create(&_arPlayerInfo[i].hPlayMgrUpdateStatusThread,
                acBuf, PLAYMGR_STACK_SIZE, PLAYMGR_THREAD_PRIORITY,
                (x_os_thread_main_fct)_PLAYMGR_UpdateStatusLoop,
                sizeof(UINT8), 
                (void*)&_arPlayerInfo[i].u1SrcId) == OSR_OK);

            VERIFY(x_timer_create(&_arPlayerInfo[i].hPlaymgrStatusNotifyTimer) == OSR_OK);



            VERIFY(x_sema_create(&_arPlayerInfo[i].hPlaymgrABDoneSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
            x_memset(acBuf, 0, 32 * sizeof(char));
            x_snprintf(acBuf, sizeof(acBuf), "ABDoneThd%d", i);
            VERIFY(x_thread_create(&_arPlayerInfo[i].hPlaymgrABDoneThread,
                                   acBuf, PLAYMGR_STACK_SIZE, PLAYMGR_THREAD_PRIORITY,
                                   (x_os_thread_main_fct)_PLAYMGR_ABDoneThread, 
                                   sizeof(UINT8), 
                                   (void*)&_arPlayerInfo[i].u1SrcId) == OSR_OK);
#if 0   // megaa SPU
        _arPlayerInfo[i].pucSubOutBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(512*384, 16));
#endif

            _arPlayerInfo[i].pucSubBuf =
                (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(PLAYMGR_SUBTITLE_BUFSIZE, 16));
            ASSERT(_arPlayerInfo[i].pucSubBuf);

            _arPlayerInfo[i].rMediaFeeder.h_feeder = (UINT32)FEEDER_PARSING_SOURCE;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_be_fct_tbl.pf_copybytes = FeederCopyBytesBE;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_be_fct_tbl.pf_input4 = FeederInput4BytesBE;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_be_fct_tbl.pf_input3 = FeederInput3BytesBE;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_be_fct_tbl.pf_input2 = FeederInput2BytesBE;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_be_fct_tbl.pf_input1 = FeederInput1Bytes;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_be_fct_tbl.pf_set_pos = FeederSetPos;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_be_fct_tbl.pf_get_pos = FeederGetPos;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_le_fct_tbl.pf_copybytes = FeederCopyBytesLE;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_le_fct_tbl.pf_input4 = FeederInput4BytesLE;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_le_fct_tbl.pf_input3 = FeederInput3BytesLE;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_le_fct_tbl.pf_input2 = FeederInput2BytesLE;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_le_fct_tbl.pf_input1 = FeederInput1Bytes;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_le_fct_tbl.pf_set_pos = FeederSetPos;
            _arPlayerInfo[i].rMediaFeeder.t_feeder_le_fct_tbl.pf_get_pos = FeederGetPos;

            x_memset(&_arNotifyTbl[i], 0, sizeof(_arNotifyTbl[i]));
            x_memset(&rSetQuery, 0, sizeof(FEEDER_QUERY_DATA));
            //_arPlayerInfo[i].fgInit = TRUE;
            _arPlayerInfo[i].rPlayerStatus.i4Speed = 1000;
            _arPlayerInfo[i].rPlayerStatus.i4PreviousSpeed = 1000;
            _arPlayerInfo[i].u4SeekPts = 0xFFFFFFFF;

            
            if (i == PLAYMGR0)
            {
                _arPlayerInfo[i].rMediaFeeder.h_feeder =
                    (UINT32)FEEDER_PARSING_SOURCE;                

                FeederSetInfo(FEEDER_PARSING_SOURCE, FEEDER_MM_USE, FALSE);
                FeederOpen(FEEDER_PARSING_SOURCE);
                FeederSetAppQueryMode(FEEDER_PARSING_SOURCE, FEEDER_PULL_MODE);
                            rSetQuery.u1PlaymgrAttachedId = i;
                            rSetQuery.eDataType = FEEDER_PARSING_SOURCE;
                rSetQuery.pfnSource = _PLAYMGR_DataQuery;
                rSetQuery.pfnSeekPos = _PLAYMGR_SeekPos;
                rSetQuery.pfnConsume = NULL;
                rSetQuery.u4QuerySize = 0;
                FeederRegisterPullDataFunc(FEEDER_PARSING_SOURCE, &rSetQuery);
                FeederStart(FEEDER_PARSING_SOURCE);
                FeederSetRing(FEEDER_PARSING_SOURCE,FALSE);
                // move from _PLAYMGR_SetFeederSrcType
                _arPlayerInfo[i].eFeederSrc = FEEDER_PROGRAM_SOURCE;                    
                FeederOpen(_arPlayerInfo[i].eFeederSrc);
                FeederSetAppQueryMode(_arPlayerInfo[i].eFeederSrc, FEEDER_PULL_MODE);
                rSetQuery.u1PlaymgrAttachedId = i;
                rSetQuery.eDataType = _arPlayerInfo[i].eFeederSrc;
                rSetQuery.pfnSource = _PLAYMGR_DataQuery;
                rSetQuery.pfnSeekPos = _PLAYMGR_SeekPos;
                rSetQuery.pfnConsume = NULL;
                rSetQuery.u4QuerySize = 0;
                FeederRegisterPullDataFunc(_arPlayerInfo[i].eFeederSrc, &rSetQuery);
                FeederStart(_arPlayerInfo[i].eFeederSrc);
                FeederSetRing(_arPlayerInfo[i].eFeederSrc,TRUE);
                // not necessary
                //FeederSetRing(_arPlayerInfo[i].eFeederSrc, TRUE);
                //FeederSetOffset(_arPlayerInfo[i].eFeederSrc, (UINT64)0, PLAYMGR_SEEK_BGN, 0);
                
            }
#ifdef CC_FBM_TWO_FBP
            else if (i == PLAYMGR1)
            {
                _arPlayerInfo[i].rMediaFeeder.h_feeder =
                    (UINT32)FEEDER_PARSING_AUDIO;

                FeederSetInfo(FEEDER_PARSING_AUDIO, FEEDER_MM_USE, FALSE);
                FeederOpen(FEEDER_PARSING_AUDIO);
                FeederSetAppQueryMode(FEEDER_PARSING_AUDIO, FEEDER_PULL_MODE);
                rSetQuery.u1PlaymgrAttachedId = i;
                rSetQuery.eDataType = FEEDER_PARSING_AUDIO;
                rSetQuery.pfnSource = _PLAYMGR_DataQuery;
                rSetQuery.pfnSeekPos = _PLAYMGR_SeekPos;
                rSetQuery.pfnConsume = NULL;
                rSetQuery.u4QuerySize = 0;
                FeederRegisterPullDataFunc(FEEDER_PARSING_AUDIO, &rSetQuery);
                FeederStart(FEEDER_PARSING_AUDIO);
                FeederSetRing(FEEDER_PARSING_AUDIO,FALSE);
                
                // move from _PLAYMGR_SetFeederSrcType
                _arPlayerInfo[i].eFeederSrc = FEEDER_AUDIO_SOURCE;                    
                FeederOpen(_arPlayerInfo[i].eFeederSrc);
                FeederSetAppQueryMode(_arPlayerInfo[i].eFeederSrc, FEEDER_PULL_MODE);
                rSetQuery.u1PlaymgrAttachedId = i;
                rSetQuery.eDataType = _arPlayerInfo[i].eFeederSrc;
                rSetQuery.pfnSource = _PLAYMGR_DataQuery;
                rSetQuery.pfnSeekPos = _PLAYMGR_SeekPos;
                rSetQuery.pfnConsume = NULL;
                rSetQuery.u4QuerySize = 0;
                FeederRegisterPullDataFunc(_arPlayerInfo[i].eFeederSrc, &rSetQuery);
                FeederStart(_arPlayerInfo[i].eFeederSrc);
                FeederSetRing(_arPlayerInfo[i].eFeederSrc,TRUE);
                // not necessary
                //FeederSetRing(_arPlayerInfo[i].eFeederSrc, TRUE);
                //FeederSetOffset(_arPlayerInfo[i].eFeederSrc, (UINT64)0, PLAYMGR_SEEK_BGN, 0);

            }
#endif
#ifdef CC_SKYPE_FINE_INSTANCE
            else if ((i >= PLAYMGR2) || (i <= PLAYMGR4))
            {
                FeederSourceType eParsingType;

                if(i == PLAYMGR2)
                {
                    eParsingType = FEEDER_PARSING_SOURCE_2;
                    _arPlayerInfo[i].eFeederSrc = FEEDER_PROGRAM_SOURCE_2;
                }
                else if(i == PLAYMGR3)
                {
                    eParsingType = FEEDER_PARSING_AUDIO_2;
                    _arPlayerInfo[i].eFeederSrc = FEEDER_AUDIO_SOURCE_2;
                }
                else
                {
                    eParsingType = FEEDER_PARSING_AUDIO_3;
                    _arPlayerInfo[i].eFeederSrc = FEEDER_AUDIO_SOURCE_3;
                }
                
                _arPlayerInfo[i].rMediaFeeder.h_feeder =
                    (UINT32)eParsingType;

                FeederSetInfo(eParsingType, FEEDER_MM_USE, FALSE);
                FeederOpen(eParsingType);
                FeederSetAppQueryMode(eParsingType, FEEDER_PULL_MODE);
                rSetQuery.u1PlaymgrAttachedId = i;
                rSetQuery.eDataType = eParsingType;
                rSetQuery.pfnSource = _PLAYMGR_DataQuery;
                rSetQuery.pfnSeekPos = _PLAYMGR_SeekPos;
                rSetQuery.pfnConsume = NULL;
                rSetQuery.u4QuerySize = 0;
                FeederRegisterPullDataFunc(eParsingType, &rSetQuery);
                FeederStart(eParsingType);
                FeederSetRing(eParsingType,FALSE);
                
                // move from _PLAYMGR_SetFeederSrcType                
                FeederOpen(_arPlayerInfo[i].eFeederSrc);
                FeederSetAppQueryMode(_arPlayerInfo[i].eFeederSrc, FEEDER_PULL_MODE);
                rSetQuery.u1PlaymgrAttachedId = i;
                rSetQuery.eDataType = _arPlayerInfo[i].eFeederSrc;
                rSetQuery.pfnSource = _PLAYMGR_DataQuery;
                rSetQuery.pfnSeekPos = _PLAYMGR_SeekPos;
                rSetQuery.pfnConsume = NULL;
                rSetQuery.u4QuerySize = 0;
                FeederRegisterPullDataFunc(_arPlayerInfo[i].eFeederSrc, &rSetQuery);
                FeederStart(_arPlayerInfo[i].eFeederSrc);
                FeederSetRing(_arPlayerInfo[i].eFeederSrc,TRUE);
                // not necessary
                //FeederSetRing(_arPlayerInfo[i].eFeederSrc, TRUE);
                //FeederSetOffset(_arPlayerInfo[i].eFeederSrc, (UINT64)0, PLAYMGR_SEEK_BGN, 0);

            }
#endif
            
            // to close
            _arPlayerInfo[i].ePlayMgrSrcType = PLAYMGR_SRC_TYPE_HIGH_SPEED_STORAGE;
            _arPlayerInfo[i].u4NumberOfBitstream = 1;
        }
    }

    return TRUE;
}


/**Initialize MPlayer
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Open(UINT8 u1SrcId, CHAR* pcFileName)
{
    UINT32 u4FileNameLength;

    if (pcFileName == NULL)
    {
        LOG(3, "_PLAYMGR_Open pcFileName NULL\n");
    }

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    
    STC_SetStcValue(_arPlayerInfo[u1SrcId].u1StcId, (UINT32)-3000);
    
#ifndef __KERNEL__
    if(pcFileName)
    {
	    if (FMR_OK != x_fm_open(FM_ROOT_HANDLE, pcFileName, FM_READ_ONLY, 0777,
	        FALSE, &_arPlayerInfo[u1SrcId].hFileHandle))
	    {
	        return FALSE;
	    }
	    if (FMR_OK != x_fm_open(FM_ROOT_HANDLE, pcFileName, FM_READ_ONLY, 0777,
	        FALSE, &_arPlayerInfo[u1SrcId].hHdrHandle))
	    {
	        return FALSE;
	    }
    }
#endif

    _arPlayerInfo[u1SrcId].szFileName[0] = '\0';
    if(pcFileName)
    {
        u4FileNameLength = x_strlen(pcFileName);
        if (u4FileNameLength < PLAYMGR_FILENAME_LENGTH)
        {
            //x_strcpy(_arPlayerInfo[u1SrcId].szFileName, pcFileName);
            x_memcpy((VOID*)_arPlayerInfo[u1SrcId].szFileName,
                (VOID*)pcFileName, u4FileNameLength);
        }
    }

    _arPlayerInfo[u1SrcId].fgOpen = TRUE;
    return TRUE;
}


BOOL _PLAYMGR_Cfg(UINT8 u1SrcId)
{
    BOOL fgEnable;
    UINT8 u1SrcIdOther;
    SWDMX_NFY_INFO_T rSwdmxNfyInfo;
    VDEC_DEC_NFY_INFO_T rVDecNfyInfo;

    _arPlayerInfo[u1SrcId].pfnPlayMgrStart = NULL;
    _arPlayerInfo[u1SrcId].pfnPlayMgrStop = NULL;
    _arPlayerInfo[u1SrcId].pfnPlayMgrClose = NULL;
    _arPlayerInfo[u1SrcId].pfnPlayMgrPause = NULL;
    _arPlayerInfo[u1SrcId].pfnPlayMgrResume = NULL;
    _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = NULL;
    _arPlayerInfo[u1SrcId].pfnPlaymgrUpdateRange = NULL;
    _arPlayerInfo[u1SrcId].pfnPlaymgrFreeRange = NULL;

    // Set swdmx's notify function.
#ifndef __MODEL_slt__
    rSwdmxNfyInfo.pvTag = &(_arPlayerInfo[u1SrcId].u1SrcId);
#else
    rSwdmxNfyInfo.pvTag = &(_arPlayerInfo[PLAYMGR0].u1SrcId);
#endif

    rSwdmxNfyInfo.pfSwdmxNfy = _PLAYMGR_DmxNfy;
    SWDMX_SetDmxNfy(u1SrcId, &rSwdmxNfyInfo);
    // swdmx_cfg will reset registed range cb, call it after SWDMX_SetContainerType
    //SWDMX_SetRangeCb((VOID*)_PLAYMGR_UpdateRangeCb);

#ifndef __MODEL_slt__
    rVDecNfyInfo.pvTag = &(_arPlayerInfo[u1SrcId].u1SrcId);
    rVDecNfyInfo.pfDecNfy = _PLAYMGR_VdecNfy;
    VDEC_SetDecNfy(_arPlayerInfo[u1SrcId].u1VdecId, &rVDecNfyInfo);
    //VDEC_SetDecNfy(1, &rVDecNfyInfo);
    VDEC_SetSrcId(_arPlayerInfo[u1SrcId].u1VdecId, u1SrcId);
#else
    rVDecNfyInfo.pvTag = &(_arPlayerInfo[PLAYMGR0].u1SrcId);
    rVDecNfyInfo.pfDecNfy = _PLAYMGR_VdecNfy;
    VDEC_SetDecNfy(_arPlayerInfo[PLAYMGR0].u1VdecId, &rVDecNfyInfo);

    VDEC_SetSrcId(_arPlayerInfo[PLAYMGR0].u1VdecId, u1SrcId);
#endif
        
    // current, vdp only have one callback function.
    fgEnable = FALSE;
    for(u1SrcIdOther=0;u1SrcIdOther<PLAYMGR_MAX_INPUT_SRC;u1SrcIdOther++)
    {
        if((_arPlayerInfo[u1SrcId].fgOpen) &&
           (_arPlayerInfo[u1SrcIdOther].rPlayerStatus.fgEnVidStrm))
        {
            fgEnable = TRUE;
        }
    }
    // current B2r is 2
    if(u1SrcId < B2R_NS)
    {
        // change to ->
        //VDP_SetCbAttachedSrcId(u1B2rId, u1SrcId);
        VDP_SetCbAttachedSrcId(u1SrcId, u1SrcId);
    }
    if(!fgEnable)
    {
        //VDP_SetCbAttachedSrcId(_arPlayerInfo[u1SrcId].u1B2rId, u1SrcId);
        VDP_RegCbFunc(VDP_CB_FUNC_MM_COND_IND, (UINT32)_PLAYMGR_VdpNfy, 0, 0);
    }

    // to prevent audio stream is open after config, we should always set srcid.
    //if(_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
    {
        AUD_MMSetSwdmxSrcId(u1SrcId, _arPlayerInfo[u1SrcId].u1AdecId);
    }
    
    if (_arPlayerInfo[u1SrcId].u4AudHandle == 0)
    {
        AUD_RegNotifyFunc(&_arPlayerInfo[u1SrcId].u4AudHandle, _PLAYMGR_AdecNfy);
    }

#ifndef CC_PS_DISABLE
    if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_PS)
    {
        if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_ps_subtype
                == MEDIA_PS_SUBTYPE_DAT)
        {
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_MPEG1_DAT);
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType =
                PLAYMGR_MEDIA_TYPE_VCD;
        }
        else
        {
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_MPEG2_PS);
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType =
                PLAYMGR_MEDIA_TYPE_MPEG2_PS;
        }
        _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_MpgStart;
        _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_MpgStop;
        _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
        _arPlayerInfo[u1SrcId].pfnPlayMgrClose = _PLAYMGR_MpgClose;
    }
    else
#endif
#ifndef CC_TS_DISABLE
    if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_TS)
    {
        if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_ts_subtype
                == MEDIA_TS_SUBTYPE_TS_188)
        {
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG2_TS;
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_MPEG2_TS);
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_ts_subtype
                 == MEDIA_TS_SUBTYPE_TS_192)
        {
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG2_TS_192;
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_MPEG2_TS_192);
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_ts_subtype
                 == MEDIA_TS_SUBTYPE_TS_192_ZERO)
        {
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG2_TS_ZERO_192;
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_MPEG2_TS_ZERO_192);
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_ts_subtype
                 == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT)
        {
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT;
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_MPEG2_TS_TIME_SHIFT);
        }
        else
        {
            LOG(5,"_PLAYMGR_Cfg MEDIA_TYPE_TS unknown\n");
            return FALSE;
        }
        _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_MpgStart;
        _arPlayerInfo[u1SrcId].pfnPlayMgrPause = _PLAYMGR_MpgPause;
        _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_MpgStop;
        _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
    }
    else
#endif
#ifndef CC_ES_DISABLE
    if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
    {
        if ((_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_M1V) ||
                (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_M2V))
        {
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_MPEG2_VIDEO_ES);
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG2_ES;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_MpgStart;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_MpgStop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_VC1)
        {
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_VC1_ES);
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_MpgStart;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_MpgStop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_VC1_ES;
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_H264)
        {
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_H264_VIDEO_ES);
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_MpgStart;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_MpgStop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_H264_ES;
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_AVS)
        {
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_AVS_VIDEO_ES);
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_MpgStart;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_MpgStop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_AVS_ES;
        }
        else if(_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_vid_subtype == MEDIA_VID_SUBTYPE_M4V)
        {
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_MPEG4_VIDEO_ES);
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_MpgStart;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_MpgStop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMpgMediaInfo;
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType = PLAYMGR_MEDIA_TYPE_MPEG4_ES;
        }
    }
    else if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
    {
        if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3)
        {
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_RAW_AUDIO_ES);
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType =
                PLAYMGR_MEDIA_TYPE_MP3;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_Mp3Start;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_Mp3Stop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrClose = _PLAYMGR_Mp3Close;
            _arPlayerInfo[u1SrcId].pfnPlayMgrPause = _PLAYMGR_Mp3Pause;
            _arPlayerInfo[u1SrcId].pfnPlayMgrResume = _PLAYMGR_Mp3Resume;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMp3MediaInfo;
        }
#ifndef CC_WMV_DISABLE
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WMA)
        {
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_WMA_AUDIO_ES);
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_WmvStart;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_WmvStop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrClose = _PLAYMGR_WmvClose;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenWmvMediaInfo;
        }
#endif
        else
        {
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_RAW_AUDIO_ES);
            if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_MP2)
            {
                _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_MP2;
            }
            else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_WAVE)
            {
                _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_WAVE;
            }
            else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_AIF)
            {
                _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_AIF;
            }
            else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_AC3)
            {
                _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_AC3;
            }
            else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_AAC)
            {
                _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_AAC;
            }
            else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype
                == MEDIA_AUD_SUBTYPE_LPCM)
            {
                _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType =
                    PLAYMGR_MEDIA_TYPE_LPCM;
            }
            else
            {
                LOG(5,"_PLAYMGR_Cfg MEDIA_TYPE_TS unknown\n");
                return FALSE;
            }
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_MpgStart;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_MpgStop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrClose = _PLAYMGR_Mp3Close;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMp3MediaInfo;
        }
    }
    else
#endif
    if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
    {
        switch (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype)
        {
#ifndef CC_AVI_DISABLE
        case MEDIA_CONTNR_SUBTYPE_AVI:
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_AVI);
            // Avi just uses one range now.
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_AviStart;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_AviStop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrClose = _PLAYMGR_AviClose;
            _arPlayerInfo[u1SrcId].pfnPlayMgrPause = _PLAYMGR_AviPause;
            _arPlayerInfo[u1SrcId].pfnPlayMgrResume = _PLAYMGR_AviResume;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenAviMediaInfo;
            //_arPlayerInfo[u1SrcId].pfnPlaymgrUpdateRange = _PLAYMGR_AviUpdateRange;
            //_arPlayerInfo[u1SrcId].pfnPlaymgrFreeRange = _PLAYMGR_AviFreeRange;
            break;
#endif

#ifndef CC_OGG_DISABLE
        case MEDIA_CONTNR_SUBTYPE_OGG:
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_OGG);
            // Avi just uses one range now.
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_OggStart;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_OggStop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrClose = _PLAYMGR_OggClose;
            _arPlayerInfo[u1SrcId].pfnPlayMgrPause = _PLAYMGR_OggPause;
            _arPlayerInfo[u1SrcId].pfnPlayMgrResume = _PLAYMGR_OggResume;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenOggMediaInfo;
            //_arPlayerInfo[u1SrcId].pfnPlaymgrUpdateRange = _PLAYMGR_AviUpdateRange;
            //_arPlayerInfo[u1SrcId].pfnPlaymgrFreeRange = _PLAYMGR_AviFreeRange;
            break;
#endif


#ifndef CC_WMV_DISABLE
        case MEDIA_CONTNR_SUBTYPE_ASF:
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_VC1_WMV);
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_WmvStart;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_WmvStop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrClose = _PLAYMGR_WmvClose;
            _arPlayerInfo[u1SrcId].pfnPlayMgrPause = _PLAYMGR_WmvPause;
            _arPlayerInfo[u1SrcId].pfnPlayMgrResume = _PLAYMGR_WmvResume;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenWmvMediaInfo;
            break;
#endif
#ifndef CC_MP4_DISABLE
        case MEDIA_CONTNR_SUBTYPE_MP4:
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_MP4);
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_Mp4Start;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_Mp4Stop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrClose = _PLAYMGR_Mp4Close;
            _arPlayerInfo[u1SrcId].pfnPlayMgrPause = _PLAYMGR_Mp4Pause;
            _arPlayerInfo[u1SrcId].pfnPlayMgrResume = _PLAYMGR_Mp4Resume;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMp4MediaInfo;
            _arPlayerInfo[u1SrcId].pfnPlaymgrUpdateRange = _PLAYMGR_Mp4UpdateRange;
            _arPlayerInfo[u1SrcId].pfnPlaymgrFreeRange = _PLAYMGR_Mp4FreeRange;
            break;
#endif
#ifndef CC_FLV_DISABLE
        case MEDIA_CONTNR_SUBTYPE_FLV:
           SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_FLV);
           // Flv just uses one range now.
           _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_FlvStart;
           _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_FlvStop;
           _arPlayerInfo[u1SrcId].pfnPlayMgrClose = _PLAYMGR_FlvClose;
           _arPlayerInfo[u1SrcId].pfnPlayMgrPause = _PLAYMGR_FlvPause;
           _arPlayerInfo[u1SrcId].pfnPlayMgrResume = _PLAYMGR_FlvResume;
           _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenFlvMediaInfo;
		   return TRUE;
#endif
#ifndef CC_MKV_DISABLE
        case MEDIA_CONTNR_SUBTYPE_MATROSKA:
            SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_MKV);
            _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_MkvStart;
            _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_MkvStop;
            _arPlayerInfo[u1SrcId].pfnPlayMgrClose = _PLAYMGR_MkvClose;
            _arPlayerInfo[u1SrcId].pfnPlayMgrPause = _PLAYMGR_MkvPause;
            _arPlayerInfo[u1SrcId].pfnPlayMgrResume = _PLAYMGR_MkvResume;
            _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenMkvMediaInfo;
            break;
#endif
#ifndef CC_RM_DISABLE
            case MEDIA_CONTNR_SUBTYPE_RM:
                SWDMX_SetContainerType(u1SrcId, SWDMX_FMT_RM);
                _arPlayerInfo[u1SrcId].pfnPlayMgrStart = _PLAYMGR_RmStart;
                _arPlayerInfo[u1SrcId].pfnPlayMgrStop = _PLAYMGR_RmStop;
                _arPlayerInfo[u1SrcId].pfnPlayMgrClose = _PLAYMGR_RmClose;
                _arPlayerInfo[u1SrcId].pfnPlayMgrPause = _PLAYMGR_RmPause;
                _arPlayerInfo[u1SrcId].pfnPlayMgrResume = _PLAYMGR_RmResume;
                _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo = _PLAYMGR_OpenRmMediaInfo;
                break;
#endif
        default:
            LOG(0, "_PLAYMGR_Cfg false\n");
            return FALSE;
        }
    }
    //SWDMX_SetRangeCb(u1SrcId, (VOID*)_PLAYMGR_UpdateRangeCb);
    SWDMX_SetRangeCbEx(u1SrcId, (VOID*)_PLAYMGR_UpdateRangeCb);
    return TRUE;
}

/**Terminate Mplayer
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Close(UINT8 u1SrcId)
{
    BOOL fgRet;
    LOG(5, "_PLAYMGR_Close\n");

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    
    if (_arPlayerInfo[u1SrcId].pfnPlayMgrClose)
    {
        fgRet = _arPlayerInfo[u1SrcId].pfnPlayMgrClose((VOID*)&_arPlayerInfo[u1SrcId]);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Close pfnPlayMgrClose Fail\n");
            return FALSE;
        }
    }


    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
    {
        _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_VIDEO, FALSE);
    }
    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
    {
        _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_AUDIO, FALSE);
    }
    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnSubStrm)
    {
        _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_CAPTRACK, FALSE);
    }
    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm)
    {
        _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_PCR, FALSE);
    }
    if (_arPlayerInfo[u1SrcId].u4AudHandle)
    {
        AUD_DeRegNotifyFunc(_arPlayerInfo[u1SrcId].u4AudHandle);
        _arPlayerInfo[u1SrcId].u4AudHandle = 0;
    }

    _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnPgmNum = 0;

   // FeederClose(eFeederSrc);
   // FeederClose(FEEDER_PARSING_SOURCE);

    x_memset(&_arPlayerInfo[u1SrcId].FmtInfo, 0, sizeof(_arPlayerInfo[u1SrcId].FmtInfo));
    x_memset(_arPlayerInfo[u1SrcId].rRangeInfo, 0, MAX_RANGE_NUM * sizeof(MM_RANGE_INFO_T));
    
    _arPlayerInfo[u1SrcId].ePlayMgrSrcType = PLAYMGR_SRC_TYPE_HIGH_SPEED_STORAGE;
    
    _arPlayerInfo[u1SrcId].fgOpen = FALSE;

    _arPlayerInfo[u1SrcId].rMediaFmt.e_media_type = MEDIA_TYPE_UNKNOWN;
    
#ifndef __KERNEL__
    x_fm_close(_arPlayerInfo[u1SrcId].hFileHandle);
    x_fm_close(_arPlayerInfo[u1SrcId].hHdrHandle);
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
BOOL _PLAYMGR_Set(UINT8 u1SrcId, PLAYMGR_SETTING_T* prSettings)
{
    BOOL fgRet;
    if (prSettings == NULL)
    {
        LOG(1, "_PLAYMGR_Set prSettings = NULL\n");
        return FALSE;
    }

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);

    if((_afgIsSeeking[u1SrcId]) &&
       (_arPlayerInfo[u1SrcId].ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }
    
    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PROGRAM)
    {
        if ((prSettings->u2PlayPgm < PLAYMGR_MAX_PROGRAM) &&
                (prSettings->u2PlayPgm < _arPlayerInfo[u1SrcId].rPlayerStatus.u2TotalPrmNum))
        {
            _PLAYMGR_GetInfoByPgm(u1SrcId, &_arPlayerInfo[u1SrcId].rPlayerStatus, prSettings->u2PlayPgm);
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
        if (_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_NETFLIX)
        {
            _PLAYMGR_WmvSetStreamIndex(&_arPlayerInfo[u1SrcId],prSettings->u1NetflixVideoIdx,prSettings->u1NetflixAudioIdx);
        }
    }
 #endif

    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_VIDEO)
    {
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgPlayVidDecrypt = prSettings->fgVidDecrypt;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayVidKeyIdx = prSettings->u1VideoKeyIdx;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayVidIdx = prSettings->u2VideoPidIdx;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayVidStrmId = prSettings->u2VideoPid;
        _arPlayerInfo[u1SrcId].rPlayerStatus.au4VideoId[0] = prSettings->u2VideoPid;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum = 0;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u2VideoTracks = 1;
        _arPlayerInfo[u1SrcId].rPlayerStatus.aeVidFormat[0] = prSettings->eVdecFmt;
        fgRet = _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_VIDEO, prSettings->fgVidEnable);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Set Fail to set video format info\n");
            return FALSE;
        }
    }

    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_AUDIO)
    {
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgPlayAudDecrypt = prSettings->fgAudDecrypt;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayAudKeyIdx = prSettings->u1AudioKeyIdx;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayAudIdx = prSettings->u2AudioPidIdx;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayAudStrmId = prSettings->u2AudioPid;
        _arPlayerInfo[u1SrcId].rPlayerStatus.au4AudioId[0] = prSettings->u2AudioPid;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum = 0;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u2AudioTracks = 1;
        _arPlayerInfo[u1SrcId].rPlayerStatus.aeAudFormat[0] = prSettings->eAdecFmt;
        fgRet = _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_AUDIO, prSettings->fgAudEnable);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Set Fail to set audio format info\n");
            return FALSE;
        }
    }

    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_PCR)
    {
        _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayPcrIdx = prSettings->u2PcrPidIdx;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayPcrStrmId = prSettings->u2PcrPid;
        _arPlayerInfo[u1SrcId].rPlayerStatus.au4PcrId[0] = prSettings->u2AudioPid;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnPcrTrackNum = 0;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u2PcrTracks = 1;
        fgRet = _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_PVR_PCR, prSettings->fgPcrEnable);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Set Fail to set audio format info\n");
            return FALSE;
        }
    }

#ifdef TIME_SHIFT_SUPPORT
    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_CTRL_BLK)
    {
        x_memset(&_arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rTickCtl, 0, sizeof(PVR_TICK_CTRL_BLK_T));
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rTickCtl.pv_start_address = prSettings->rTickCtl.pv_start_address;
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rTickCtl.pv_end_address = prSettings->rTickCtl.pv_end_address;
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rTickCtl.ui4_entry_num = prSettings->rTickCtl.ui4_entry_num;
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rTickCtl.ui4_entry_size = prSettings->rTickCtl.ui4_entry_size;
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rTickCtl.ui4_tick_period = prSettings->rTickCtl.ui4_tick_period;
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rTickCtl.ui4_lba_init = prSettings->rTickCtl.ui4_lba_init;
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rTickCtl.ui8_fifo_offset = prSettings->rTickCtl.ui8_fifo_offset;
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rTickCtl.ui4_fifo_pkt_num = prSettings->rTickCtl.ui4_fifo_pkt_num;
    }

    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_PVR_REC_RANGE)
    {
        x_memset(&_arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rRangeInfo, 0, sizeof(MM_REC_BUF_RANGE_INFO_T));
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rRangeInfo.t_valid.ui4_start = prSettings->rRangeInfo.t_valid.ui4_start;
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rRangeInfo.t_valid.ui4_end = prSettings->rRangeInfo.t_valid.ui4_end;
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rRangeInfo.t_safe.ui4_start = prSettings->rRangeInfo.t_safe.ui4_start;
        _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rRangeInfo.t_safe.ui4_end = prSettings->rRangeInfo.t_safe.ui4_end;

        if(!SWDMX_SetInfo(u1SrcId, eSWDMX_SET_REC_BUF_RANGE, (UINT32)&_arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rRangeInfo, 0, 0))
        {
            LOG(3, "_PLAYMGR_Set PLAYMGR_INFO_MASK_PVR_REC_RANGE fail\n");
        }
    }
#endif

    if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_LPCM_INFO)
    {
        if(!_PLAYMGR_SetLPCMInfo(u1SrcId, prSettings->rLPCMInfo))
        {
            LOG(1, "_PLAYMGR_Set fail, _PLAYMGR_SetLPCMInfo\n");
            return FALSE;
        }
    }
    
    if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_PS) ||
            (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_TS) ||
            (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) ||
            (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES) ||
            (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER))
    {
        if((prSettings->u4InfoMask&PLAYMGR_INFO_MASK_VIDEO)==PLAYMGR_INFO_MASK_VIDEO)
        {
            if ((prSettings->u2PlayVidTrack < PLAYMGR_MAX_VID_TRACK) &&
                    (prSettings->u2PlayVidTrack < _arPlayerInfo[u1SrcId].rPlayerStatus.u2VideoTracks))
            {
                _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayVidStrmId =
                    _arPlayerInfo[u1SrcId].rPlayerStatus.au4VideoId[prSettings->u2PlayVidTrack];
                _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlaySubVidStrmId =
                    _arPlayerInfo[u1SrcId].rPlayerStatus.au4SubVideoId[prSettings->u2PlayVidTrack];
                _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum = prSettings->u2PlayVidTrack;

                fgRet = _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_VIDEO, prSettings->fgVidEnable);
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
        if ((prSettings->u4InfoMask&PLAYMGR_INFO_MASK_AUDIO)==PLAYMGR_INFO_MASK_AUDIO)
#else
        if (FALSE)
#endif
        {
            if ((prSettings->u2PlayAudTrack < PLAYMGR_MAX_AUD_TRACK) &&
                (prSettings->u2PlayAudTrack < _arPlayerInfo[u1SrcId].rPlayerStatus.u2AudioTracks) &&
                (_arPlayerInfo[u1SrcId].rPlayerStatus.aeAudFormat[prSettings->u2PlayAudTrack] != PLAYMGR_ADEC_UNKNOWN))
            {
                _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayAudStrmId =
                    _arPlayerInfo[u1SrcId].rPlayerStatus.au4AudioId[prSettings->u2PlayAudTrack];
                _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayAudSubStrmId =
                    _arPlayerInfo[u1SrcId].rPlayerStatus.au4AudioSubId[prSettings->u2PlayAudTrack];
                _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum = prSettings->u2PlayAudTrack;
                fgRet = _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_AUDIO, prSettings->fgAudEnable);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_Set Fail to set audio format info\n");
                    return FALSE;
                }
            }
            else
            {
                LOG(1, "_PLAYMGR_Set fail, u2PlayAudTrack %d\n", prSettings->u2PlayAudTrack);
                //return FALSE;
            }
        }
        if (prSettings->u4InfoMask & PLAYMGR_INFO_MASK_CAPTRACK)
        {
            if (prSettings->pu1SubtitleBuffer != NULL)
            {
                _arPlayerInfo[u1SrcId].pucSubOutBuf = prSettings->pu1SubtitleBuffer;
            }
            else
            {
                ASSERT(0);
            }
            if ((prSettings->u2PlaySubTrack < PLAYMGR_MAX_SUB_TRACK) &&
                    (prSettings->u2PlaySubTrack < _arPlayerInfo[u1SrcId].rPlayerStatus.u2CaptionTracks))
            {
                _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlaySubId =
                    _arPlayerInfo[u1SrcId].rPlayerStatus.au4CapTrackId[prSettings->u2PlaySubTrack];
                _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnCapTrackNum = prSettings->fgSubEnable;

                fgRet = _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_CAPTRACK, prSettings->fgSubEnable);
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
                    (prSettings->u2PlayPcrTrack < _arPlayerInfo[u1SrcId].rPlayerStatus.u2PcrTracks))
            {
                _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayPcrStrmId =
                    _arPlayerInfo[u1SrcId].rPlayerStatus.au4PcrId[prSettings->u2PlayPcrTrack];
                _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnPcrTrackNum = prSettings->fgPcrEnable;

                fgRet = _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_PCR, prSettings->fgPcrEnable);
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
BOOL _PLAYMGR_Get(UINT8 u1SrcId, PLAYMGR_SETTING_T* prSettings)
{
    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);

    if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_PS) ||
            (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_TS) ||
            (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER))
    {
        if (_arPlayerInfo[u1SrcId].u2VidStmNum > 0)
        {
            prSettings->u2NumVideoTrack = _arPlayerInfo[u1SrcId].u2VidStmNum;
        }
        if (_arPlayerInfo[u1SrcId].u2AudStmNum > 0)
        {
            prSettings->u2NumAudioTrack = _arPlayerInfo[u1SrcId].u2AudStmNum;
        }
        if (_arPlayerInfo[u1SrcId].u2SubStmNum > 0)
        {
            prSettings->u2NumSubTrack = _arPlayerInfo[u1SrcId].u2SubStmNum;
        }
    }

    return TRUE;
}

extern INT64 _FeederSetOffset(FeederSourceType eDataType, UINT64 u8Offset, UINT8 u1Whence, UINT32 u4ReqID);

/**Start Play.
*@param MPlayer Handle
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Start(UINT8 u1SrcId)
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
    ENUM_VDEC_FMT_T eVdecFmt = VDEC_FMT_MPV;
    //UINT8 u1index=0;

    VDEC_SEQUENCE_DATA_T rVDecSeqData;
    MINFO_TYPE_STM_ATRBT_T *pVidStrmAttr = NULL;
    MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;


    PLAYMGR_STATUS_T* prMediaStatus = &_arPlayerInfo[u1SrcId].rPlayerStatus;
    //PLAYMGR_WMV_INFO_T* prWmvInfo = &(_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo);
    UINT64 u8Numerator=0, u8Denominator=0, u8Fps = 0, u8Remainder=0;

    LOG(5, "_PLAYMGR_Start\n");
    
    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);

    if((_afgIsSeeking[u1SrcId]) &&
       (_arPlayerInfo[u1SrcId].ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }
    
#ifndef PLAYMGR_ASYNC_MODE
    if(_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
    {
        LOG(3, "_PLAYMGR_Start already started\n");
        return TRUE;
    }
#endif

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
    
    fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_SET_STC_ID,
                (UINT32)_arPlayerInfo[u1SrcId].u1StcId, 0, 0);
    if (!fgRet)
    {
        LOG(3, "SWDMX_SetInfo eSWDMX_SET_STC_ID fail\n");
    }

    if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
    {
        return _PLAYMGR_Resume(u1SrcId);
    }

    if ((_arNotifyTbl[u1SrcId][PLAYMGR_CALLBACK_ELAPSEDTIME]) &&
            (!_arPlayerInfo[u1SrcId].fgPlaymgrStatusNotifyTimerStarted))
    {
        VERIFY(x_timer_start (_arPlayerInfo[u1SrcId].hPlaymgrStatusNotifyTimer,
                              PLAYMGR_STATUS_UPDATE_TIMER,
                              X_TIMER_FLAG_REPEAT,
                              _PLAYMGR_StatusNfyTimer,
                              &_arPlayerInfo[u1SrcId].u1SrcId) == OSR_OK);
        _arPlayerInfo[u1SrcId].fgPlaymgrStatusNotifyTimerStarted = TRUE;
    }

    if((_arPlayerInfo[u1SrcId].ePlayMgrCurStatus != PLAYMGR_STATE_PLAY) &&
       (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus != PLAYMGR_STATE_PAUSE))
    {
        SRM_SetMmMode(SRM_MM_MODE_TYPE_VIDEO);
    }

    FeederStart(_arPlayerInfo[u1SrcId].eFeederSrc);
    FeederSetRing(_arPlayerInfo[u1SrcId].eFeederSrc, TRUE);
    if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
            (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
             == MEDIA_CONTNR_SUBTYPE_AVI))
    {
        if (_arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.u2VidIdx < 17)
        {
            pVidStrmAttr =
                &_arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.u2VidIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.u2VidIdx >= 17\n");
        }

        if (_arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.u2AudIdx < 17)
        {
            prAudStrmAttr =
                &_arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.u2AudIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.u2AudIdx >= 17\n");
        }
    }
    else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
            (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
             == MEDIA_CONTNR_SUBTYPE_FLV))
    {
        if (_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.u2VidIdx == 0)
        {
            pVidStrmAttr =&_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.u2VidIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.u2VidIdx!=1\n");
        }

	    if (_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.u2AudIdx == 1)
	    {
		    prAudStrmAttr =&_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.u2AudIdx];
	    }
	    else
	    {
		    LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.u2AudIdx !=1\n");
	    }

    }
    else if((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
                _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WMA)
    {
        if (_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u2AudIdx < 17)
        {
            prAudStrmAttr =
                &_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rStmAttribute[0][_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u2AudIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u2AudIdx >= 17\n");
        }
    }
    else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
             ((_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_LPCM) ||
              (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WAVE) ||
              (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AIF) ||
              (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AAC) ||
              (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AC3)))

    {
        prAudStrmAttr =&_arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rStreamAttr;
    }
    else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
             (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
              == MEDIA_CONTNR_SUBTYPE_ASF))
    {
        if ((_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u2VidIdx < 17) &&
            (_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u1VideoStreamIdx <10))
        {
            pVidStrmAttr =
                &_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u1VideoStreamIdx][_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u2VidIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u2VidIdx >= 17\n");
        }
        if ((_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u2AudIdx < 17)&&
            (_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u1AudioStreamIdx<10))
        {
            prAudStrmAttr =
                &_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u1AudioStreamIdx][_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u2AudIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.u2AudIdx >= 17\n");
        }

    }
    else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
             (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
              == MEDIA_CONTNR_SUBTYPE_MATROSKA))
    {
        if (_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.u2VidIdx < 17)
        {
            pVidStrmAttr =
                &_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.u2VidIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.u2VidIdx >= 17\n");
        }
        if (_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.u2AudIdx < 17)
        {
            prAudStrmAttr =
                &_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.u2AudIdx];
        }
        else
        {
            LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.u2AudIdx >= 17\n");
        }
    }
    else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
             (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
              == MEDIA_CONTNR_SUBTYPE_MP4))
    {
        if (prMediaStatus->u2VideoTracks > 0)
        {
            if (_arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.u2VidIdx < 3)
            {
                pVidStrmAttr =
                    &_arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.u2VidIdx];
            }
            else
            {
                LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.u2VidIdx >= 17\n");
            }
        }

        if (prMediaStatus->u2AudioTracks > 0)
        {
            if (_arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.u2AudIdx < 3)
            {
                prAudStrmAttr =
                    &_arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.u2AudIdx];
            }
            else
            {
                LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.u2AudIdx >= 3\n");
            }

        }

        UNUSED(SWDMX_SetInfo(u1SrcId, eSWDMX_SET_RANGECALLBACKEX, (UINT32)_PLAYMGR_UpdateRangeCb, 0, 0));
    }
    else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
             (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
              == MEDIA_CONTNR_SUBTYPE_RM))
    {
        if (prMediaStatus->u2VideoTracks > 0)
        {
            if(_arPlayerInfo[u1SrcId].FmtInfo.rRmInfo.u2VidIdx < 17)
            {
                pVidStrmAttr =
                    &_arPlayerInfo[u1SrcId].FmtInfo.rRmInfo.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rRmInfo.u2VidIdx];
            }
            else
            {
                LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rRmInfo.u2VidIdx >= 17\n");
            }
        }

        if (prMediaStatus->u2AudioTracks > 0)
        {
            if(_arPlayerInfo[u1SrcId].FmtInfo.rRmInfo.u2AudIdx < 17)
            {
                prAudStrmAttr =
                    &_arPlayerInfo[u1SrcId].FmtInfo.rRmInfo.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rRmInfo.u2AudIdx];
            }
            else
            {
                LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].FmtInfo.rRmInfo.u2AudIdx >= 17\n");
            }

        }

        UNUSED(SWDMX_SetInfo(u1SrcId, eSWDMX_SET_RANGECALLBACKEX, (UINT32)_PLAYMGR_UpdateRangeCb, 0, 0));
    }

    if (_arPlayerInfo[u1SrcId].pfnPlayMgrStart == 0)
    {
        LOG(1, "_PLAYMGR_Start _arPlayerInfo[u1SrcId].pfnPlayMgrStart == 0\n");
        return FALSE;
    }

    if ((_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AVI ||
            _arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AVI) &&
            _arPlayerInfo[u1SrcId].rPlayerStatus.u2CaptionTracks > 0)
    {
        _PLAYMGR_InitSubtitle(u1SrcId);  // megaa 20090118: tmp solution
    }

    if ((prMediaStatus->u2VideoTracks > 0) &&
            (prMediaStatus->fgEnVidStrm) &&
            (prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum] != PLAYMGR_VDEC_UNKNOWN))
    {
        if (VDP_GetStepForward(_arPlayerInfo[u1SrcId].u1B2rId) != 0)
        {
            VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
            VDP_SetStepForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_NORMAL);
        }

        if(VDP_GetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId) != 0)
        {
            VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
            VDP_SetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_NORMAL);
        }

        VDP_SetPauseMm(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
        VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_1X);

        VDP_SetTrickMode(_arPlayerInfo[u1SrcId].u1B2rId,VDP_PLAY_NORMAL);

        LOG(5, "aeVidFormat %d\n", prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum]);
        VDEC_PlayMM(_arPlayerInfo[u1SrcId].u1VdecId , TRUE);
        VDEC_SetSrcId(_arPlayerInfo[u1SrcId].u1VdecId, u1SrcId);
        if((prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
           (prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK))
        {
            VDEC_PlayMM(_arPlayerInfo[u1SrcId].u1Vdec2Id , TRUE);
            VDEC_SetSrcId(_arPlayerInfo[u1SrcId].u1Vdec2Id, u1SrcId);
        }

        fgRet = SWDMX_SetInfo(u1SrcId, 
            (UINT8)eSWDMX_SET_VDEC_ID,
            (UINT32)_arPlayerInfo[u1SrcId].u1VdecId, 
            0, 
            0);
        fgRet = SWDMX_SetInfo(u1SrcId, 
            (UINT8)eSWDMX_SET_B2R_ID,
            (UINT32)_arPlayerInfo[u1SrcId].u1B2rId, 
            0, 
            0);    
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Start Fail to set vdec id\n");
            return FALSE;
        }
        if((prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
           (prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK))
        {
            fgRet = SWDMX_SetInfo(u1SrcId, 
                (UINT8)eSWDMX_SET_VDEC2_ID,
                (UINT32)_arPlayerInfo[u1SrcId].u1Vdec2Id, 
                0, 
                0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_Start Fail to set vdec 2 id\n");
                return FALSE;
            }
        }
        
        switch (prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum])
        {
        case PLAYMGR_VDEC_DIVX311:
            eVdecFmt = VDEC_FMT_MP4;
            fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_MP4);
            // Done in swdmx_drvif.c
            //DMX_SetVideoType(DMX_VIDEO_MPEG4);
            if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    ((_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_AVI) ||
                     (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_ASF) ||
                     (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_MATROSKA)))
            {
                ASSERT(pVidStrmAttr!=NULL);
                if (pVidStrmAttr!=NULL)
                {
                    if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                            == MEDIA_CONTNR_SUBTYPE_MATROSKA)
                    {
                        rVDecSeqData.u2_width = (UINT16)pVidStrmAttr->u.t_mkv_video_stm_attr.i4_width;
                        rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_mkv_video_stm_attr.i4_height;
                        u8Numerator =
                            (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator =
                            (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_scale;
                    }
                    else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                                 == MEDIA_CONTNR_SUBTYPE_AVI)
                    {
                        rVDecSeqData.u2_width = (UINT16)pVidStrmAttr->u.t_avi_video_stm_attr.i4_width;
                        rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_avi_video_stm_attr.i4_height;

                        u8Numerator =
                            (UINT64)pVidStrmAttr->u.t_avi_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator =
                            (UINT64)pVidStrmAttr->u.t_avi_video_stm_attr.ui4_scale;
                    }
                    else // asf
                    {
                        rVDecSeqData.u2_width = (UINT16)pVidStrmAttr->u.t_asf_video_stm_attr.i4_width;
                        rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_asf_video_stm_attr.i4_height;

                        u8Numerator =
                            (UINT64)pVidStrmAttr->u.t_asf_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator =
                            (UINT64)pVidStrmAttr->u.t_asf_video_stm_attr.ui4_scale;
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
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            }
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_DX311, 0, 0);

            break;
        case PLAYMGR_VDEC_MPEG4:
        case PLAYMGR_VDEC_SORENSON:
		case PLAYMGR_VDEC_H263:
            eVdecFmt = VDEC_FMT_MP4;
            fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_MP4);
            // Done in swdmx_drvif.c
            //DMX_SetVideoType(DMX_VIDEO_MPEG4);
            if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    ((_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_AVI) ||
                     (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_MATROSKA)||
                     (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_FLV)))
            {
                ASSERT(pVidStrmAttr!=NULL);
                //rVDecSeqData.u2_width = (UINT16)pVidStrmAttr->u.t_avi_video_stm_attr.i4_width;
                //rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_avi_video_stm_attr.i4_height;
                if (pVidStrmAttr!=NULL)
                {
                    if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                            == MEDIA_CONTNR_SUBTYPE_MATROSKA)
                    {
                        rVDecSeqData.u2_width = (UINT16)pVidStrmAttr->u.t_mkv_video_stm_attr.i4_width;
                        rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_mkv_video_stm_attr.i4_height;
                        u8Numerator =
                            (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator =
                            (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_scale;
                    }
                    else if(_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                            == MEDIA_CONTNR_SUBTYPE_AVI)   // avi
                    {
                        u8Numerator =
                            (UINT64)pVidStrmAttr->u.t_avi_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator =
                            (UINT64)pVidStrmAttr->u.t_avi_video_stm_attr.ui4_scale;
                    }
                    else if(_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
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

                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            }
            else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                     (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4))
            {
                ASSERT(pVidStrmAttr!=NULL);
                if ((pVidStrmAttr!=NULL) && (pVidStrmAttr->u.t_mp4i_video_stm_attr.ui4_codec_info_size != 0))
                {
                    SWDMX_SetInfo(u1SrcId, 
                                  eSWDMX_SET_VIDSEQHEADER,
                                  (UINT32)pVidStrmAttr->u.t_mp4i_video_stm_attr.pui1_codec_info,
                                  pVidStrmAttr->u.t_mp4i_video_stm_attr.ui4_codec_info_size,
                                  0);
                }
            }
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_MPEG4, 0, 0);
            break;
        case PLAYMGR_VDEC_MPEG1_MPEG2:
            eVdecFmt = VDEC_FMT_MPV;
            fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_MPV);
            // Done in swdmx_drvif.c
            //DMX_SetVideoType(DMX_VIDEO_MPEG);
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_MPEG1_MPEG2, 0, 0);
            break;
		case PLAYMGR_VDEC_MJPEG:
            eVdecFmt = VDEC_FMT_MJPEG;
		    fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_MJPEG);
            // Done in swdmx_drvif.c
            //DMX_SetVideoType(DMX_VIDEO_MPEG4);
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_MJPEG, 0, 0);

            if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
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

                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            }
            break;
#ifndef CC_FLV_DISABLE
             case PLAYMGR_VDEC_VP6:
            eVdecFmt = VDEC_FMT_VP6;
            if(pVidStrmAttr==NULL)
            {
                LOG(0,"pVidStrmAttr NULL\n");
                break;
            }
            fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_VP6);
            // Done in swdmx_drvif.c
            //DMX_SetVideoType(DMX_VIDEO_VP6);
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_VP6, 0, 0);
            rVDecSeqData.u2_width  = (UINT16)pVidStrmAttr->u.t_flv_video_stm_attr.i4_width;
            rVDecSeqData.u2_height = (UINT16)pVidStrmAttr->u.t_flv_video_stm_attr.i4_height;
            rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_25;
            rVDecSeqData.e_src_asp = VDEC_ASPECT_RATIO_4_3;
            rVDecSeqData.u4SequenceHeader=0;
            rVDecSeqData.u2SequenceHeaderSize=0;
            VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            break;
#endif

        case PLAYMGR_VDEC_VP8:
            eVdecFmt = VDEC_FMT_VP8;
            if(pVidStrmAttr == NULL)
            {
                LOG(0, "pVidStrmAttr NULL\n");
                break;
            }
            fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_VP8);
            // Done in swdmx_drvif.c
            //DMX_SetVideoType(DMX_VIDEO_VP8);
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_VP8, 0, 0);

            rVDecSeqData.u2_width   = (UINT16)pVidStrmAttr->u.t_mkv_video_stm_attr.i4_width;
            rVDecSeqData.u2_height  = (UINT16)pVidStrmAttr->u.t_mkv_video_stm_attr.i4_height;
            u8Numerator =
                (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_rate * FPS_PREC;
            u8Denominator =
                (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_scale;

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
                    _PLAYMGR_FRC(u8Fps, &rVDecSeqData);
                    break;
                }
            }
            
            VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            break;

        case PLAYMGR_VDEC_H264:
            eVdecFmt = VDEC_FMT_H264;
            fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_H264);
            // Done in swdmx_drvif.c
            //DMX_SetVideoType(DMX_VIDEO_H264);
            if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4))
            {
                ASSERT(pVidStrmAttr!=NULL);
                if ((pVidStrmAttr!=NULL) && (pVidStrmAttr->u.t_mp4i_video_stm_attr.ui4_codec_info_size != 0))
                {
                    SWDMX_SetInfo(u1SrcId, 
                                  eSWDMX_SET_VIDSEQHEADER,
                                  (UINT32)pVidStrmAttr->u.t_mp4i_video_stm_attr.pui1_codec_info,
                                  pVidStrmAttr->u.t_mp4i_video_stm_attr.ui4_codec_info_size,
                                  0);
                }
            }
            if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA))
            {
                ASSERT(pVidStrmAttr!=NULL);
                if ((pVidStrmAttr!=NULL) && (pVidStrmAttr->u.t_mp4i_video_stm_attr.ui4_codec_info_size != 0))
                {
                    SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDSEQHEADER,
                    (UINT32)pVidStrmAttr->u.t_mkv_video_stm_attr.pui1_codec_info,
                    pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_codec_info_size,
                    0);
                }
            }
            if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV))
            {
                ASSERT(pVidStrmAttr!=NULL);
                if ((pVidStrmAttr!=NULL) && (pVidStrmAttr->u.t_flv_video_stm_attr.ui4_codec_info_size != 0))
                {
                    SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDSEQHEADER,
                    (UINT32)pVidStrmAttr->u.t_flv_video_stm_attr.pui1_codec_info,
                    pVidStrmAttr->u.t_flv_video_stm_attr.ui4_codec_info_size,
                    0);
                }
            }
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_H264, 0, 0);
            break;
        case PLAYMGR_VDEC_AVS:
            eVdecFmt = VDEC_FMT_AVS;
            fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_AVS);
            // Done in swdmx_drvif.c
            //DMX_SetVideoType(DMX_VIDEO_AVS);
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_AVS, 0, 0);
            break;
#ifndef CC_RM_DISABLE
        case PLAYMGR_VDEC_RV8:
        case PLAYMGR_VDEC_RV9:
            eVdecFmt = VDEC_FMT_RV;
            fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_RV);
            // Done in swdmx_drvif.c
            //DMX_SetVideoType(DMX_VIDEO_RV);
//            u4NumRprSize = 0;
            if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER &&
                _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM)
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
                        u1SrcId, 
                        eSWDMX_SET_VIDSEQHEADER, 
                        (UINT32)(&(_arPlayerInfo[u1SrcId].FmtInfo.rRmInfo.rRmSpecInfo)), 
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
            VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_RV_RPR_INFO, u4NumRprSize, pVidStrmAttr->u.t_rm_video_stm_attr.ui4_specific, PLAYMGR_VDEC_RV9 == prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum]);
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_RV, 0, 0);
#endif
            break;
#endif
        case PLAYMGR_VDEC_WMV7:
        case PLAYMGR_VDEC_WMV8:
        case PLAYMGR_VDEC_WMV9:
        case PLAYMGR_VDEC_VC1:
            eVdecFmt = VDEC_FMT_WMV;
            if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF ||
                 _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA))
            {
                ASSERT(pVidStrmAttr != NULL);
                if ( pVidStrmAttr != NULL)
                {
                    SeqHeaderInfo rSeqHeaderInfo;
                    UINT8 *pu1CodecInfo;
                    UINT32 u4CodecInfoSize;

                    if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
                    {
                        u4CodecVersion = pVidStrmAttr->u.t_asf_video_stm_attr.e_enc;
                        u4Width = pVidStrmAttr->u.t_asf_video_stm_attr.i4_width;
                        u4Height = pVidStrmAttr->u.t_asf_video_stm_attr.i4_height;
                        u4HeaderLen = pVidStrmAttr->u.t_asf_video_stm_attr.ui4_codec_info_size;
                        u8Numerator = (UINT64)pVidStrmAttr->u.t_asf_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator = (UINT64)pVidStrmAttr->u.t_asf_video_stm_attr.ui4_scale;
                        rSeqHeaderInfo.u1Addr = pVidStrmAttr->u.t_asf_video_stm_attr.pui1_codec_info;
                        pu1CodecInfo = pVidStrmAttr->u.t_asf_video_stm_attr.pui1_codec_info;
                        u4CodecInfoSize = pVidStrmAttr->u.t_asf_video_stm_attr.ui4_codec_info_size;
                    }
                    else  // mkv
                    {
                        u4CodecVersion = pVidStrmAttr->u.t_mkv_video_stm_attr.e_enc;
                        u4Width = pVidStrmAttr->u.t_mkv_video_stm_attr.i4_width;
                        u4Height = pVidStrmAttr->u.t_mkv_video_stm_attr.i4_height;
                        u4HeaderLen = pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_codec_info_size;
                        u8Numerator = (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_rate * FPS_PREC;
                        u8Denominator = (UINT64)pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_scale;
                        rSeqHeaderInfo.u1Addr = pVidStrmAttr->u.t_mkv_video_stm_attr.pui1_codec_info;
                        pu1CodecInfo = pVidStrmAttr->u.t_mkv_video_stm_attr.pui1_codec_info;
                        u4CodecInfoSize = pVidStrmAttr->u.t_mkv_video_stm_attr.ui4_codec_info_size;
                    }
                    rSeqHeaderInfo.u4Len = u4HeaderLen;

#ifndef CC_WMV_DISABLE
                    fgCodecSupport = _PLAYMGR_GetWMVSupport(prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum],
                            &rSeqHeaderInfo);

                    if(fgCodecSupport == FALSE)
                    {
                        _PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_VIDEO,FALSE);
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
                            //rVDecSeqData.e_frame_rate = VDEC_SRC_FRAME_RATE_UNKNOWN;
                            _PLAYMGR_FRC(u8Fps, &rVDecSeqData);
                            break;
                        }
                    }
                    if (u4HeaderLen <=5)
                    {
                        if (u4HeaderLen!=0)
                        {
                            x_memcpy(&u4HeaderData1,pu1CodecInfo,4);
                            u4HeaderData1 = ((u4HeaderData1&0x000000FF)<<24) | ((u4HeaderData1&0x0000FF00)<<8) | ((u4HeaderData1&0x00FF0000)>>8) | ((u4HeaderData1&0xFF000000)>>24);
                        }
                        else
                        {
                            u4HeaderData1 =0;
                        }
                    }
                    else
                    {
                        x_memcpy(&u4HeaderData1,pu1CodecInfo,4);
                        u4HeaderData1 = ((u4HeaderData1&0x000000FF)<<24) | ((u4HeaderData1&0x0000FF00)<<8) | ((u4HeaderData1&0x00FF0000)>>8) | ((u4HeaderData1&0xFF000000)>>24);
                        //x_memcpy(&u4HeaderData2,prWmvInfo->rWmvVideoStmAttribute[1].pui1_codec_info+4,4);
                    }
                    
                    rVDecSeqData.u2_width = (UINT16)u4Width;
                    rVDecSeqData.u2_height = (UINT16)u4Height;
                    rVDecSeqData.u4SequenceHeader = u4HeaderData1;
                    rVDecSeqData.u2SequenceHeaderSize = u4HeaderLen;
					
                    VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
					if(prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK)
					{
						VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1Vdec2Id, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
					}
					
                    if (u4CodecVersion == MINFO_INFO_VID_ENC_WMV1)
                    {
                        SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_WMV7,0,0);
                    }
                    else if (u4CodecVersion == MINFO_INFO_VID_ENC_WMV2)
                    {
                        SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_WMV8,0,0);
                    }
                    else if (u4CodecVersion == MINFO_INFO_VID_ENC_WMV3)
                    {
                        SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_WMV9,0,0);
                    }
                    else if (u4CodecVersion == MINFO_INFO_VID_ENC_WVC1)
                    {
                        SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_VC1,0,0);
                        SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VIDSEQHEADER,(UINT32)(pu1CodecInfo),u4CodecInfoSize,0);
                    }
                    else
                    {
                        LOG(0,"error wmv type\n");
                    }
                }
            }

            fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_WMV);
            // Done in swdmx_drvif.c
            //DMX_SetVideoType(DMX_VIDEO_VC1);
            LOG(0,"PLAYMGR_VDEC_VC1\n");

            break;
        default:
            return FALSE;
        }
        
        if((prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
           (prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK))
        {
            fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1Vdec2Id, eVdecFmt);
            if(prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MVC)
            {
                SWDMX_SetInfo(u1SrcId, eSWDMX_SET_SUBVIDEOTYPE, ENUM_SWDMX_VDEC_MVC, 0, 0);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_3D_INFO, VDEC_3D_MVC,
                    (UINT32)&prMediaStatus->aau1VideoHdr[prMediaStatus->u2EnVTrackNum][0], 0);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1Vdec2Id, VDEC_MM_3D_INFO, VDEC_3D_MVC,
                    (UINT32)&prMediaStatus->aau1VideoHdr[prMediaStatus->u2EnVTrackNum][0], 0);
            }
            else
            {
                SWDMX_SetInfo(u1SrcId, eSWDMX_SET_SUBVIDEOTYPE, ENUM_SWDMX_VDEC_MULTITRACK, 0, 0);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_3D_INFO, VDEC_3D_REALD_DUAL_STREAM,
                    0, 0);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1Vdec2Id, VDEC_MM_3D_INFO, VDEC_3D_REALD_DUAL_STREAM,
                    0, 0);
            }        
        }
    }

    // AV sync mode must be set before audio play
    _PLAYMGR_SetSyncMode(u1SrcId);

    if ((prMediaStatus->u2AudioTracks > 0) &&
            (prMediaStatus->fgEnAudStrm) &&
            (prMediaStatus->aeAudFormat[prMediaStatus->u2EnATrackNum] != PLAYMGR_ADEC_UNKNOWN))
    {
        LOG(5, "aeAudFormat %d\n", prMediaStatus->aeAudFormat[prMediaStatus->u2EnATrackNum]);

        fgRet = SWDMX_SetInfo(u1SrcId, 
            (UINT8)eSWDMX_SET_ADEC_ID,
            (UINT32)_arPlayerInfo[u1SrcId].u1AdecId, 
            0, 
            0);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Start Fail to set vdec id\n");
            return FALSE;
        }
        
        switch (prMediaStatus->aeAudFormat[prMediaStatus->u2EnATrackNum])
        {
        case PLAYMGR_ADEC_MP3:
#ifdef CC_53XX_AUDIO_V2
                AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#else
            AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#endif
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_MP3,0,0);
            break;
        case PLAYMGR_ADEC_AAC:
#ifdef CC_53XX_AUDIO_V2
                AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#else
            AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#endif
            if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4) &&
                    (prAudStrmAttr))
            {
                SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_AAC,
                              prAudStrmAttr->u.t_mp4i_audio_stm_attr.i4_sample_rate,
                              prAudStrmAttr->u.t_mp4i_audio_stm_attr.i2_channels);
            }
            else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                     (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM) && prAudStrmAttr)
            {
                SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_AAC,
                    prAudStrmAttr->u.t_rm_audio_stm_attr.ui4_sample_rate,
                    prAudStrmAttr->u.t_rm_audio_stm_attr.ui2_num_channel);
            }
           else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                         (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV) && prAudStrmAttr)
            {
                    SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_AAC,
                        prAudStrmAttr->u.t_flv_audio_stm_attr.i4_samples_per_sec,
                        prAudStrmAttr->u.t_flv_audio_stm_attr.i2_channels);
            }
            else
            {
                SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_AAC,0,0);
            }
            break;
        case PLAYMGR_ADEC_AC3:
#ifdef CC_53XX_AUDIO_V2
                AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#else
            AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#endif
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_AC3,0,0);
            break;
        case PLAYMGR_ADEC_MPEG:
#ifdef CC_53XX_AUDIO_V2
                AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#else
            AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#endif
            SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_MPEG,0,0);
            break;
        case PLAYMGR_ADEC_PCM:
        case PLAYMGR_ADEC_ADPCM:
            x_memset((void*)&rLPCMInfo, 0, sizeof(rLPCMInfo));
            x_memset((void*)&rPCMInfo, 0, sizeof(rPCMInfo));
                        
            if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                      (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
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
            else if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
            {
                if(prAudStrmAttr==NULL)
                {
                    LOG(0,"prAudStrmAttr NULL\n");
                    break;
                }
                if(_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
                {
                    rPCMInfo.eSampleFreq = _GetAudSamplingRate(prAudStrmAttr->u.t_avi_audio_stm_attr.i4_samples_per_sec);
                    rLPCMInfo.ui2_num_ch = prAudStrmAttr->u.t_avi_audio_stm_attr.i2_channels;
                    rLPCMInfo.ui2_bits_per_sample = prAudStrmAttr->u.t_avi_audio_stm_attr.i2_bits_per_sample;	
                    rLPCMInfo.ui2_block_align = prAudStrmAttr->u.t_avi_audio_stm_attr.i2_block_align;	
                }
                else if(_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
                {
                    rPCMInfo.eSampleFreq = _GetAudSamplingRate(prAudStrmAttr->u.t_asf_audio_stm_attr.i4_samples_per_sec);
                    rLPCMInfo.ui2_num_ch = prAudStrmAttr->u.t_asf_audio_stm_attr.i2_channels;
                    rLPCMInfo.ui2_bits_per_sample = prAudStrmAttr->u.t_asf_audio_stm_attr.i2_bits_per_sample;	
                    rLPCMInfo.ui2_block_align = prAudStrmAttr->u.t_asf_audio_stm_attr.i2_block_align;	
                }                
                else if(_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
                {
                    rPCMInfo.eSampleFreq = _GetAudSamplingRate(prAudStrmAttr->u.t_mkv_audio_stm_attr.i4_samples_per_sec);
                    rLPCMInfo.ui2_num_ch = prAudStrmAttr->u.t_mkv_audio_stm_attr.i2_channels;
                    rLPCMInfo.ui2_bits_per_sample = prAudStrmAttr->u.t_mkv_audio_stm_attr.i2_bits_per_sample;	
                    rLPCMInfo.ui2_block_align = prAudStrmAttr->u.t_mkv_audio_stm_attr.i2_block_align;	
                }
                else
                {
                    LOG(3,"unknown e_contnr_subtype(%d)\n",
                        (UINT32)_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype);
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
            else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
                     (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype
                       == MEDIA_AUD_SUBTYPE_LPCM))
            {
                if(prAudStrmAttr==NULL)
                {
                    LOG(0,"prAudStrmAttr NULL\n");
                    break;
                }
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
            else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
                      ((_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype
                        == MEDIA_AUD_SUBTYPE_WAVE) ||
                        (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype
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
                uPcmType=prAudStrmAttr->u.t_avi_audio_stm_attr.e_enc;

                if(_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype
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
                else if(_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype
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
                AUD_PcmSetting(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, &rPCMInfo);
                AUD_PcmSetting(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, &rPCMInfo);
                AUD_LPcmSetting(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, &rLPCMInfo);
                AUD_LPcmSetting(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, &rLPCMInfo);
                AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_PCM);
#else
                AUD_PcmSetting(_arPlayerInfo[u1SrcId].u1AdecId, &rPCMInfo);
                AUD_LPcmSetting(_arPlayerInfo[u1SrcId].u1AdecId, &rLPCMInfo);
                AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_PCM);
#endif
                SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_PCM,0,0);
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
                _AUD_DspWMAHeader(_arPlayerInfo[u1SrcId].u1AdecId,&rAudWmaCfg);
#ifdef CC_53XX_AUDIO_V2
                AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#else
                AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#endif
                SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_WMA,0,0);
            }
            break;
#if 1//def CC_MT5363
        case PLAYMGR_ADEC_RA:
#ifdef CC_53XX_AUDIO_V2
            AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
            AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#else
            AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#endif
            if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                 (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM) && prAudStrmAttr)
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
                AUD_DspCOOKParameter(_arPlayerInfo[u1SrcId].u1AdecId, &rCookCfg);

                SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE, ENUM_SWDMX_ADEC_RA, (UINT32)&_rCookInfo, 0);
            }
            else
            {
                ASSERT(0);
                SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,ENUM_SWDMX_ADEC_RA,0,0);
            }
            break;
#endif
        default:
            return FALSE;
        }

        _PLAYMGR_AdecFlushCtrl(u1SrcId);
        // wait for audio API.
        //AUD_DrvSetDelay();
        #ifdef CC_53XX_AUDIO_V2
        AUD_DSPCmdPlay(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
        AUD_DSPCmdPlay(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
        #else
        AUD_DSPCmdPlay(_arPlayerInfo[u1SrcId].u1AdecId);
        #endif
        _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
        //AUD_DSPCmdPlayAsyn(_arPlayerInfo[u1SrcId].u1AdecId);
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = TRUE;
    }
    //FeederSetRing((FeederSourceType)_arPlayerInfo[u1SrcId].rMediaFeeder.h_feeder, FALSE);
    fgRet = _arPlayerInfo[u1SrcId].pfnPlayMgrStart((VOID*)&_arPlayerInfo[u1SrcId]);
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
    fgRet = SWDMX_Play(u1SrcId);
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_Start SWDMX_Play fail\n");
        return fgRet;
    }
    // do not set this, set start stc value on swdmx and vdec
    // start stc on vdec
    //_PLAYMGR_SetSTC(_arPlayerInfo[u1SrcId].u4StartPts);

    return TRUE;
}

/**Stop MPlayer.
*@param handle
*@retval TRUE                      Success.
*@retval FALSE                     Fail.
*/
BOOL _PLAYMGR_Stop(UINT8 u1SrcId)
{
    LOG(5, "_PLAYMGR_Stop\n");

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);

#ifndef PLAYMGR_ASYNC_MODE
    if(_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_STOP)
    {
        LOG(3, "_PLAYMGR_Stop already stopped\n");
        return TRUE;
    }
#endif

    if ((_arNotifyTbl[u1SrcId][PLAYMGR_CALLBACK_ELAPSEDTIME]) &&
            (_arPlayerInfo[u1SrcId].fgPlaymgrStatusNotifyTimerStarted))
    {
        VERIFY(x_timer_stop(_arPlayerInfo[u1SrcId].hPlaymgrStatusNotifyTimer) == OSR_OK);
        _arPlayerInfo[u1SrcId].fgPlaymgrStatusNotifyTimerStarted = FALSE;
    }

    if((_arPlayerInfo[u1SrcId].ePlayMgrCurStatus != PLAYMGR_STATE_PLAY) &&
       (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus != PLAYMGR_STATE_PAUSE))
    {
        SRM_SetMmMode(SRM_MM_MODE_TYPE_OFF);
    }

    if (_arPlayerInfo[u1SrcId].pfnPlayMgrStop == 0)
    {
        LOG(1, "_PLAYMGR_Stop _arPlayerInfo[u1SrcId].pfnPlayMgrStop == 0\n");
        return FALSE;
    }

    STC_StopStc(_arPlayerInfo[u1SrcId].u1StcId);
    STC_SetStcValue(_arPlayerInfo[u1SrcId].u1StcId, (UINT32)-3000);

    _arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed = 1000;
    _arPlayerInfo[u1SrcId].rPlayerStatus.i4PreviousSpeed = 1000;

    // Because AUD_DSPCmdStopAsyn will change audio fifo read/write pointer.
    // call SWDMX_Stop before that.
    // Or, maybe we could use other API but not AUD_DSPCmdStopAsyn.

    // Vdec also have the same reason as audio.
    _PLAYMGR_DmxFlushCtrl(u1SrcId);
    if (!SWDMX_Stop(u1SrcId))
    {
        LOG(1,"_PLAYMGR_Stop SWDMX_Pause fail\n");
    }
    _PLAYMGR_DmxWaitCtrlDone(u1SrcId);

    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
    {
        if (VDP_GetStepForward(_arPlayerInfo[u1SrcId].u1B2rId) != 0)
        {
            VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
            VDP_SetStepForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_NORMAL);
        }

        if(VDP_GetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId) != 0)
        {
            VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
            VDP_SetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_NORMAL);
        }

        // Start -> Pause -> Stop: reset Pause status.
        VDP_SetPauseMm(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
        VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_1X);
        VDP_SetTrickMode(_arPlayerInfo[u1SrcId].u1B2rId,VDP_PLAY_NORMAL);

        _PLAYMGR_VdecFlushCtrl(u1SrcId);
        VDEC_Stop(_arPlayerInfo[u1SrcId].u1VdecId);
        if ((_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum < (UINT16)PLAYMGR_MAX_VID_TRACK) &&
            ((_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
             (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK)))
        {
            VDEC_Stop(_arPlayerInfo[u1SrcId].u1Vdec2Id);
        }
        _PLAYMGR_VdecWaitCtrlDone(u1SrcId);
    }

    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
    {
        LOG(5, "_PLAYMGR_Stop AUD_DSPCmdStop(%d)\n", _arPlayerInfo[u1SrcId].u1AdecId);
        _PLAYMGR_AdecFlushCtrl(u1SrcId);
#ifdef CC_53XX_AUDIO_V2
        AUD_DSPCmdStop(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
        AUD_DSPCmdStop(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
        AUD_DSPCmdStop(_arPlayerInfo[u1SrcId].u1AdecId);
#endif
        _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = FALSE;
    }

    _arPlayerInfo[u1SrcId].pfnPlayMgrStop((VOID*)&_arPlayerInfo[u1SrcId]);

    //FeederStop(eFeederSrc);
    _afgIsSeeking[u1SrcId] = FALSE;
    
    return TRUE;
}

/**Pause MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Pause(UINT8 u1SrcId)
{
    BOOL fgRet;
    LOG(5, "_PLAYMGR_Pause\n");

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);

    if((_afgIsSeeking[u1SrcId]) &&
       (_arPlayerInfo[u1SrcId].ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }
    
    STC_StopStc(_arPlayerInfo[u1SrcId].u1StcId);

    /*
    if((_arNotifyTbl[u1SrcId][PLAYMGR_CALLBACK_ELAPSEDTIME]) &&
        (_arPlayerInfo[u1SrcId].fgPlaymgrStatusNotifyTimerStarted))
    {
        VERIFY(x_timer_stop(_arPlayerInfo[u1SrcId].hPlaymgrStatusNotifyTimer) == OSR_OK);
        _arPlayerInfo[u1SrcId].fgPlaymgrStatusNotifyTimerStarted = FALSE;
    }
    */

    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
    {
        VDP_SetPauseMm(_arPlayerInfo[u1SrcId].u1B2rId, TRUE);
        // Vdec may block by VDP, we should not wait for Pause cmd.
        //_PLAYMGR_VdecFlushCtrl();
        VDEC_Pause(_arPlayerInfo[u1SrcId].u1VdecId);
        if ((_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum < (UINT16)PLAYMGR_MAX_VID_TRACK) &&
            ((_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
             (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK)))
        {
            VDEC_Pause(_arPlayerInfo[u1SrcId].u1Vdec2Id);
        }
        x_thread_delay(1);
        //_PLAYMGR_VdecWaitCtrlDone();
    }
    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
    {
        _PLAYMGR_AdecFlushCtrl(u1SrcId);
#ifdef CC_53XX_AUDIO_V2
        AUD_DSPCmdPause(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
        AUD_DSPCmdPause(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
        AUD_DSPCmdPause(_arPlayerInfo[u1SrcId].u1AdecId);
#endif
        _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
    }

    _PLAYMGR_DmxFlushCtrl(u1SrcId);
    if (!SWDMX_Pause(u1SrcId))
    {
        LOG(1,"_PLAYMGR_Pause SWDMX_Pause fail\n");
    }
    _PLAYMGR_DmxWaitCtrlDone(u1SrcId);

    if (_arPlayerInfo[u1SrcId].pfnPlayMgrPause)
    {
        fgRet = _arPlayerInfo[u1SrcId].pfnPlayMgrPause();//(VOID*)&_arPlayerInfo[u1SrcId]);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Pause _arPlayerInfo[u1SrcId].pfnPlayMgrPause == 0\n");
            return FALSE;
        }
    }

    return TRUE;
}

BOOL _PLAYMGR_ABRepeat(UINT8 u1SrcId, PLAYMGR_ABREPEAT_TYPE_T eMode)
{   
    VDP_AB_INTO_T rVdpPosInfo;
    VDEC_AB_INTO_T rVdecPosInfo;
    VDP_POS_INTO_T  rVdpInfo;
    PLAYMGR_AB_INFO_T* prABInfo = &_arPlayerInfo[u1SrcId].rABInfo;
    PLAYMGR_AB_POS_INFO_T* prPos= NULL;

    LOG(0, "_PLAYMGR_ABRepeat %d\n", eMode);
    
    if (!PLAYMGR_GetCap(u1SrcId, PLAYMGR_CAP_SEEK, 0, 0))
    {
        return FALSE;
    }
    
    if (eMode == PLAYMGR_ABREPEAT_SET_A)
    {
        prPos = &prABInfo->rA;
    }
    else if (eMode == PLAYMGR_ABREPEAT_SET_B)
    {
        prPos = &prABInfo->rB;
    }
    else if (eMode == PLAYMGR_ABREPEAT_CANCEL)
    {
        x_memset(&rVdpPosInfo, 0, sizeof(VDP_AB_INTO_T));
        x_memset(&rVdecPosInfo, 0, sizeof(VDEC_AB_INTO_T));
        
        VDP_SetABInfo(B2R_1, &rVdpPosInfo);
        VDEC_SetABInfo(B2R_1, &rVdecPosInfo);
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
    if(VDP_GetPosInfo(B2R_1, &rVdpInfo))
    {
        x_memset(prPos, 0, sizeof(PLAYMGR_AB_POS_INFO_T));
        prPos->ui8_aud_pts_info         = rVdpInfo.u8AudPts;
        prPos->ui8_aud_frame_position   = rVdpInfo.u8AudOffset;
        prPos->ui8_i_pts_info           = rVdpInfo.u8PtsI;
        prPos->ui8_pts_info             = rVdpInfo.u8Pts;
        prPos->ui2_decoding_order       = rVdpInfo.u2DecodingOrder;
        prPos->i4_temporal_reference    = rVdpInfo.i4TemporalRef;
        prPos->ui8_i_frame_position     = rVdpInfo.u8OffsetI;
        prPos->ui8_frame_position       = rVdpInfo.u8Offset;
        prPos->ui8_frame_position_disp  = rVdpInfo.u8OffsetDisp;
        prPos->ui8_stc                  = rVdpInfo.u8Stc;
    }
    else
    {
        return FALSE;
    }
    
    if (eMode == PLAYMGR_ABREPEAT_SET_B)
    {   
        MEDIA_FORMAT_T* prMediaFmt  = &_arPlayerInfo[u1SrcId].rMediaFmt;
        //PLAYMGR_STATUS_T* prStatus  = &_arPlayerInfo[u1SrcId].rPlayerStatus;
        PLAYMGR_AB_POS_INFO_T* prA  = &prABInfo->rA;
        PLAYMGR_AB_POS_INFO_T* prB  = &prABInfo->rB;
        PLAYMGR_AB_POS_INFO_T  rTmp;

        if (prA->ui8_pts_info > prB->ui8_pts_info)
        {
            rTmp = *prA;
            *prA = *prB;
            *prB = rTmp;            
        }
        else
        {
            // no need swap AB, seek to A
            PLAYMGR_SeekTime(u1SrcId, (((UINT32)prA->ui8_pts_info)/90));
        }
        
        // set AB Info to VDP & VDEC
        x_memset(&rVdpPosInfo, 0, sizeof(VDP_AB_INTO_T));
        x_memset(&rVdecPosInfo, 0, sizeof(VDEC_AB_INTO_T));
   
        rVdpPosInfo.fgValid     = TRUE;
        rVdecPosInfo.fgValid    = TRUE;
        
        if ((prMediaFmt->e_media_type == MEDIA_TYPE_PS) ||
            (prMediaFmt->e_media_type == MEDIA_TYPE_TS) ||
            (prMediaFmt->e_media_type == MEDIA_TYPE_VIDEO_ES) ||
            (prMediaFmt->e_media_type == MEDIA_TYPE_AUDIO_ES)
           )
        {
            rVdpPosInfo.fgPosition  = TRUE;
            rVdecPosInfo.fgPosition = TRUE;
        }
        
        rVdpPosInfo.rA.u8AudPts            = prA->ui8_aud_pts_info;
        rVdpPosInfo.rA.u8AudOffset         = prA->ui8_aud_frame_position;
        rVdpPosInfo.rA.u8PtsI              = prA->ui8_i_pts_info;
        rVdpPosInfo.rA.u8Pts               = prA->ui8_pts_info;
        rVdpPosInfo.rA.u8Stc               = prA->ui8_stc;
        rVdpPosInfo.rA.u8OffsetI           = prA->ui8_i_frame_position;
        rVdpPosInfo.rA.u8Offset            = prA->ui8_frame_position;
        rVdpPosInfo.rA.u8OffsetDisp        = prA->ui8_frame_position_disp;
        rVdpPosInfo.rA.i4TemporalRef       = prA->i4_temporal_reference;
        rVdpPosInfo.rA.u2DecodingOrder     = prA->ui2_decoding_order;

        rVdecPosInfo.rA.u8AudPts            = prA->ui8_aud_pts_info;
        rVdecPosInfo.rA.u8AudOffset         = prA->ui8_aud_frame_position;
        rVdecPosInfo.rA.u8PtsI              = prA->ui8_i_pts_info;
        rVdecPosInfo.rA.u8Pts               = prA->ui8_pts_info;
        rVdecPosInfo.rA.u8OffsetI           = prA->ui8_i_frame_position;
        rVdecPosInfo.rA.u8Offset            = prA->ui8_frame_position;
        rVdecPosInfo.rA.u8OffsetDisp        = prA->ui8_frame_position_disp;
        rVdecPosInfo.rA.i4TemporalRef       = prA->i4_temporal_reference;
        rVdecPosInfo.rA.u2DecodingOrder     = prA->ui2_decoding_order;
                
        rVdpPosInfo.rB.u8AudPts            = prB->ui8_aud_pts_info;
        rVdpPosInfo.rB.u8AudOffset         = prB->ui8_aud_frame_position;
        rVdpPosInfo.rB.u8PtsI              = prB->ui8_i_pts_info;
        rVdpPosInfo.rB.u8Pts               = prB->ui8_pts_info;
        rVdpPosInfo.rB.u8Stc               = prB->ui8_stc;
        rVdpPosInfo.rB.u8OffsetI           = prB->ui8_i_frame_position;
        rVdpPosInfo.rB.u8Offset            = prB->ui8_frame_position;
        rVdpPosInfo.rB.u8OffsetDisp        = prB->ui8_frame_position_disp;
        rVdpPosInfo.rB.i4TemporalRef       = prB->i4_temporal_reference;
        rVdpPosInfo.rB.u2DecodingOrder     = prB->ui2_decoding_order;

        rVdecPosInfo.rB.u8AudPts            = prB->ui8_aud_pts_info;
        rVdecPosInfo.rB.u8AudOffset         = prB->ui8_aud_frame_position;
        rVdecPosInfo.rB.u8PtsI              = prB->ui8_i_pts_info;
        rVdecPosInfo.rB.u8Pts               = prB->ui8_pts_info;
        rVdecPosInfo.rB.u8OffsetI           = prB->ui8_i_frame_position;
        rVdecPosInfo.rB.u8Offset            = prB->ui8_frame_position;
        rVdecPosInfo.rB.u8OffsetDisp        = prB->ui8_frame_position_disp;
        rVdecPosInfo.rB.i4TemporalRef       = prB->i4_temporal_reference;
        rVdecPosInfo.rB.u2DecodingOrder     = prB->ui2_decoding_order;
        
        VDP_SetABInfo(B2R_1, &rVdpPosInfo);
        VDEC_SetABInfo(B2R_1, &rVdecPosInfo);
    }

    return TRUE;
}

/**Resume MPlayer.
*@param handle
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Resume(UINT8 u1SrcId)
{
    PLAYMGR_STATUS_T* prMediaStatus = &_arPlayerInfo[u1SrcId].rPlayerStatus;
    BOOL fgRet;
    ENUM_VDEC_FMT_T eVdecFmt = VDEC_FMT_MPV;

    LOG(5, "_PLAYMGR_Resume\n");

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    
    if((_afgIsSeeking[u1SrcId]) &&
       (_arPlayerInfo[u1SrcId].ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }
    
    // We might in step, so swdmx are still running.
    _PLAYMGR_DmxFlushCtrl(u1SrcId);
    if(!SWDMX_Pause(u1SrcId))
    {
        LOG(1,"_PLAYMGR_Resume SWDMX_Pause fail\n");
    }
    _PLAYMGR_DmxWaitCtrlDone(u1SrcId);
    
    /*
    if((_arNotifyTbl[u1SrcId][PLAYMGR_CALLBACK_ELAPSEDTIME]) &&
        (!_arPlayerInfo[u1SrcId].fgPlaymgrStatusNotifyTimerStarted))
    {
        VERIFY(x_timer_start (_arPlayerInfo[u1SrcId].hPlaymgrStatusNotifyTimer,
                            PLAYMGR_STATUS_UPDATE_TIMER,
                            X_TIMER_FLAG_REPEAT,
                            _PLAYMGR_StatusNfyTimer,
                            NULL) == OSR_OK);
        _arPlayerInfo[u1SrcId].fgPlaymgrStatusNotifyTimerStarted = TRUE;
    }
    */

    if (prMediaStatus->u2EnVTrackNum >= (UINT16)PLAYMGR_MAX_VID_TRACK)
    {
        LOG(3, "PLAYMGR_MAX_VID_TRACK\n");
        prMediaStatus->u2EnVTrackNum = (UINT16)(PLAYMGR_MAX_VID_TRACK-1);
    }

    if (_arPlayerInfo[u1SrcId].pfnPlayMgrResume)
    {
        fgRet = _arPlayerInfo[u1SrcId].pfnPlayMgrResume((VOID*)&_arPlayerInfo[u1SrcId]);
        if (!fgRet)
        {
            LOG(1, "_PLAYMGR_Resume pfnPlayMgrResume Fail\n");
            return FALSE;
        }
    }

    fgRet = TRUE;
    if (prMediaStatus->fgEnVidStrm)
    {
        VDEC_PlayMM(_arPlayerInfo[u1SrcId].u1VdecId , TRUE);
        if ((prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
            (prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK))
        {
            VDEC_PlayMM(_arPlayerInfo[u1SrcId].u1Vdec2Id , TRUE);
        }
        switch (prMediaStatus->aeVidFormat[prMediaStatus->u2EnVTrackNum])
        {
        case PLAYMGR_VDEC_MPEG4:
        case PLAYMGR_VDEC_DIVX311:
        case PLAYMGR_VDEC_SORENSON:
            eVdecFmt = VDEC_FMT_MP4;
            fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_MP4);
            break;
        case PLAYMGR_VDEC_MPEG1_MPEG2:
            eVdecFmt = VDEC_FMT_MPV;
            fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_MPV);
            break;
        case PLAYMGR_VDEC_H264:
            eVdecFmt = VDEC_FMT_H264;
            fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_H264);
            break;
        case PLAYMGR_VDEC_AVS:
            eVdecFmt = VDEC_FMT_AVS;
            fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_AVS);
            break;
		case PLAYMGR_VDEC_MJPEG:
            eVdecFmt = VDEC_FMT_MJPEG;
            fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_MJPEG);
			break;
        case PLAYMGR_VDEC_WMV7:
        case PLAYMGR_VDEC_WMV8:
        case PLAYMGR_VDEC_WMV9:
        case PLAYMGR_VDEC_VC1:
            eVdecFmt = VDEC_FMT_WMV;
            fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_WMV);
            break;
        case PLAYMGR_VDEC_RV8:
        case PLAYMGR_VDEC_RV9:
            eVdecFmt = VDEC_FMT_RV;
            fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_RV);
            break;
         case PLAYMGR_VDEC_VP6:
            eVdecFmt = VDEC_FMT_VP6;
            fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_VP6);
            break;

        case PLAYMGR_VDEC_VP8:
            eVdecFmt = VDEC_FMT_VP8;
            fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_VP8);
            break;
        default:
            return FALSE;
        }

        if((prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
           (prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK))
        {
            fgRet = (BOOL)VDEC_Play(_arPlayerInfo[u1SrcId].u1Vdec2Id, eVdecFmt);
            if(prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MVC)
            {
                SWDMX_SetInfo(u1SrcId, eSWDMX_SET_SUBVIDEOTYPE, ENUM_SWDMX_VDEC_MVC, 0, 0);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_3D_INFO, VDEC_3D_MVC,
                    (UINT32)&prMediaStatus->aau1VideoHdr[prMediaStatus->u2EnVTrackNum][0], 0);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1Vdec2Id, VDEC_MM_3D_INFO, VDEC_3D_MVC,
                    (UINT32)&prMediaStatus->aau1VideoHdr[prMediaStatus->u2EnVTrackNum][0], 0);
            }
            else
            {
                SWDMX_SetInfo(u1SrcId, eSWDMX_SET_SUBVIDEOTYPE, ENUM_SWDMX_VDEC_MULTITRACK, 0, 0);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_3D_INFO, VDEC_3D_REALD_DUAL_STREAM,
                    0, 0);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1Vdec2Id, VDEC_MM_3D_INFO, VDEC_3D_REALD_DUAL_STREAM,
                    0, 0);
            }        
        }
    }
    if (!fgRet)
    {
        LOG(1, "_PLAYMGR_Resume VDEC_Play Fail\n");
        return FALSE;
    }


    if ((prMediaStatus->fgDisAudStrm) && (!prMediaStatus->fgEnAudStrm))
    {
        if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_AUDIO, TRUE))
        {
            LOG(1,"_PLAYMGR_CtlStm enable audio fail\n");
        }
    }
    
    // AV sync mode must be set before audio play
    _PLAYMGR_SetSyncMode(u1SrcId);


    if ((prMediaStatus->fgEnAudStrm) || (prMediaStatus->fgDisAudStrm))
    {
#if 0
        if ((prMediaStatus->fgDisAudStrm) && (!prMediaStatus->fgEnAudStrm))
        {
            if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_AUDIO, TRUE))
            {
                LOG(1,"_PLAYMGR_CtlStm enable audio fail\n");
            }
        }
#endif

        if (!_arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed)
        {
            _PLAYMGR_AdecFlushCtrl(u1SrcId);
            // wait for audio API.
            //AUD_DrvSetDelay();
#ifdef CC_53XX_AUDIO_V2
            AUD_DSPCmdPlay(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
            AUD_DSPCmdPlay(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
            AUD_DSPCmdPlay(_arPlayerInfo[u1SrcId].u1AdecId);
#endif
            _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
            _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = TRUE;
        }
        else
        {
#ifdef CC_53XX_AUDIO_V2
            AUD_DSPCmdResume(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
            AUD_DSPCmdResume(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
            AUD_DSPCmdResume(_arPlayerInfo[u1SrcId].u1AdecId);
#endif
        }
        prMediaStatus->fgDisAudStrm = FALSE;
        prMediaStatus->fgEnAudStrm = TRUE;
    }
    if ((prMediaStatus->fgEnPcrStrm) || (prMediaStatus->fgDisPcrStrm))
    {
        if ((prMediaStatus->fgDisPcrStrm) && (!prMediaStatus->fgEnPcrStrm))
        {
            if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_PCR, TRUE))
            {
                LOG(1,"_PLAYMGR_CtlStm enable Pcr fail\n");
            }
        }

        if (!_arPlayerInfo[u1SrcId].rPlayerStatus.fgPcrPlayed)
        {
            _arPlayerInfo[u1SrcId].rPlayerStatus.fgPcrPlayed = TRUE;
        }
        prMediaStatus->fgDisPcrStrm = FALSE;
        prMediaStatus->fgEnPcrStrm = TRUE;
    }

    //setting av sync mode before audio playing
    //_PLAYMGR_SetSyncMode();

    SWDMX_Play(u1SrcId);

    if (prMediaStatus->fgEnVidStrm)
    {
        if (VDP_GetStepForward(_arPlayerInfo[u1SrcId].u1B2rId) != 0)
        {
            VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
            VDP_SetStepForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_NORMAL);
        }

        if(VDP_GetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId) != 0)
        {
            VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
            VDP_SetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_NORMAL);
        }

        VDP_SetPauseMm(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
        VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_1X);
        VDP_SetTrickMode(_arPlayerInfo[u1SrcId].u1B2rId,VDP_PLAY_NORMAL);
    }

    STC_StartStc(_arPlayerInfo[u1SrcId].u1StcId);

    return TRUE;
}


/**Get the stream settings and status from MPlayer
*@param handle
*@param prSettings
*@param prStatus
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_GetInfo(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus)
{
    BOOL fgRet = FALSE;

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);

    LOG(5, "_PLAYMGR_GetInfo\n");
    
    _arPlayerInfo[u1SrcId].u2VidStmNum = 0;
    _arPlayerInfo[u1SrcId].u2AudStmNum = 0;
    _arPlayerInfo[u1SrcId].u2SubStmNum = 0;
    _arPlayerInfo[u1SrcId].rPlayerStatus.aeAudFormat[0] = PLAYMGR_ADEC_UNKNOWN;
    _arPlayerInfo[u1SrcId].rPlayerStatus.aeVidFormat[0] = PLAYMGR_VDEC_UNKNOWN;
    _arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[0] = PLAYMGR_VDEC_UNKNOWN;
    _arPlayerInfo[u1SrcId].rPlayerStatus.aeCapFormat[0] = PLAYMGR_CAPTION_UNKNOWN;
    _arPlayerInfo[u1SrcId].rPlayerStatus.u2VideoTracks = 0;
    _arPlayerInfo[u1SrcId].rPlayerStatus.u2AudioTracks = 0;
    _arPlayerInfo[u1SrcId].rPlayerStatus.u2CaptionTracks = 0;
    _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration = 0;
    _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayCap = 0;
    _arPlayerInfo[u1SrcId].rPlayerStatus.u4CurPos = 0;

    if (_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_NETFLIX)
    {
        _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize = _arPlayerInfo[u1SrcId].u8FileSize = (UINT64)(-1);
    }
    else if(_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_TIME_SHIFT)
    {
        _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize = _arPlayerInfo[u1SrcId].u8FileSize = (UINT64)(-1);
    }
    else if(_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize = _arPlayerInfo[u1SrcId].u8FileSize = (UINT64)(-1);
    }
    else if(_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_VUDU)
    {
        _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize = _arPlayerInfo[u1SrcId].u8FileSize = (UINT64)(-1);
    }
    else
    {
        _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize =
            _arPlayerInfo[u1SrcId].u8FileSize =
            (UINT64)_FeederSetOffset(
            _arPlayerInfo[u1SrcId].eFeederSrc,
            0,PLAYMGR_SEEK_END,0);
        _FeederSetOffset(
            _arPlayerInfo[u1SrcId].eFeederSrc,
            0,PLAYMGR_SEEK_BGN,0);

    }

    FeederSetRing((FeederSourceType)_arPlayerInfo[u1SrcId].rMediaFeeder.h_feeder, FALSE);
    FeederSetOffset((FeederSourceType)_arPlayerInfo[u1SrcId].rMediaFeeder.h_feeder, (UINT64)0, PLAYMGR_SEEK_BGN, 0);
    x_memcpy((VOID *)prStatus, (VOID *)&(_arPlayerInfo[u1SrcId].rPlayerStatus), sizeof(PLAYMGR_STATUS_T));
#ifndef __MODEL_slt__
    if (_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_NETFLIX)
    {
        _arPlayerInfo[u1SrcId].rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
        _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_ASF;
    }
    else if (_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_TIME_SHIFT)
    {
        _arPlayerInfo[u1SrcId].rMediaFmt.e_media_type = MEDIA_TYPE_TS;
        _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_TIME_SHIFT;
    }
    else if(_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        _arPlayerInfo[u1SrcId].rMediaFmt.e_media_type = MEDIA_TYPE_AUDIO_ES;
        _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_LPCM;
    }
    else if(_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_VUDU)
    {
        _arPlayerInfo[u1SrcId].rMediaFmt.e_media_type = MEDIA_TYPE_TS;
        _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_188;
    }
    else if (_PLAYMGR_RecgFile(&_arPlayerInfo[u1SrcId], _arPlayerInfo[u1SrcId].szFileName) != TRUE)
    {
        return FALSE;
    }
#else
    if (_u4SltContainerFormat == MEDIA_TYPE_CONTAINER)
    {
        if (_u4SltSubType == MEDIA_CONTNR_SUBTYPE_AVI)
        {
            _arPlayerInfo[u1SrcId].rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
            _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_AVI;
        }
            else if (_u4SltSubType == MEDIA_CONTNR_SUBTYPE_RM)
        {
            _arPlayerInfo[u1SrcId].rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
            _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_RM;
        }
            else if (_u4SltSubType == MEDIA_CONTNR_SUBTYPE_ASF)
        {
            _arPlayerInfo[u1SrcId].rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
            _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_ASF;
        }
        else if (_u4SltContainerFormat == MEDIA_CONTNR_SUBTYPE_FLV)
        {
            _arPlayerInfo[u1SrcId].rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
            _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_FLV;
        }
        else if (_u4SltContainerFormat == MEDIA_CONTNR_SUBTYPE_MATROSKA)
        {
            _arPlayerInfo[u1SrcId].rMediaFmt.e_media_type = MEDIA_TYPE_CONTAINER;
            _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MATROSKA;
        }
    }
    else if (_u4SltContainerFormat == MEDIA_TYPE_VIDEO_ES)
    {
        _arPlayerInfo[u1SrcId].rMediaFmt.e_media_type = MEDIA_TYPE_VIDEO_ES;
        _arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_vid_subtype = MEDIA_VID_SUBTYPE_AVS;
    }
#endif

    _PLAYMGR_SetFeederSrcType(u1SrcId);

    if (!_PLAYMGR_Cfg(u1SrcId))
    {
        return FALSE;
    }

    if(!_arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo)
    {
        return FALSE;
    }
	fgRet = _arPlayerInfo[u1SrcId].pfnPlayMgrOpenMediaInfo(prStatus, (VOID*)&_arPlayerInfo[u1SrcId]);
    if (!fgRet)
    {
        LOG(3, "_PLAYMGR_GetInfo pfnPlayMgrOpenMediaInfo Fail\n");
    }

    x_memcpy((VOID *)prStatus, (VOID *)&(_arPlayerInfo[u1SrcId].rPlayerStatus), sizeof(PLAYMGR_STATUS_T));
    return fgRet;
}


/**Get the stream settings and status from MPlayer
*@param handle
*@param prStatus
*@param u4Pgm
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_GetInfoByPgm(UINT8 u1SrcId, PLAYMGR_STATUS_T *prStatus, UINT32 u4Pgm)
{
    BOOL fgRet = FALSE;
    LOG(5, "_PLAYMGR_GetInfoByPgm\n");

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);

    if (u4Pgm >= _arPlayerInfo[u1SrcId].rPlayerStatus.u2TotalPrmNum)
    {
        LOG(3, "_PLAYMGR_GetInfoByPgm %d > %d\n", u4Pgm, _arPlayerInfo[u1SrcId].rPlayerStatus.u2TotalPrmNum);
        return fgRet;
    }

    fgRet = _PLAYMGR_GetMpgMediaInfo((VOID*)&_arPlayerInfo[u1SrcId], u4Pgm);
    if (!fgRet)
    {
        LOG(3, "_PLAYMGR_GetMpgMediaInfo Fail\n");
    }

    x_memcpy((VOID *)prStatus, (VOID *)&(_arPlayerInfo[u1SrcId].rPlayerStatus), sizeof(PLAYMGR_STATUS_T));

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
BOOL _PLAYMGR_RegCb(UINT8 u1SrcId,
                    PLAYMGR_CALLBACK_TYPE_T eCallbackType,
                    PLAYMGR_CALLBACK pfnCallback,
                    UINT32 u4Param)
{
    LOG(5, "_PLAYMGR_RegCb(%d)\n", u1SrcId);
    FeederInstallCallback(_arPlayerInfo[u1SrcId].eFeederSrc, (FEEDER_NOTIFY_COND)eCallbackType,
                          (FEEDER_CALBACK)_PLAYMGR_FeederCb, u4Param);
    if (eCallbackType < PLAYMGR_CALLBACK_MAX)
    {
        _arNotifyTbl[u1SrcId][eCallbackType]= pfnCallback;
    }
    else
    {
        LOG(5, "_PLAYMGR_RegCb(%d) unknow eCallbackType %d\n", u1SrcId, eCallbackType);
    }
    return TRUE;
}

/**Set the seek position by time.
*@param handle
*@param seek time(millisecond)
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_Speed(UINT8 u1SrcId, INT32 i4Speed)
{
    BOOL fgSetOK = TRUE;
    BOOL fgRet = TRUE;
    AUD_PCM_SETTING_T rPCMInfo;
    BOOL fgResetSwDmx = TRUE;
    STC_SPEED_TYPE_T eStcSpeed = STC_SPEED_TYPE_FORWARD_1X;
    PLAYMGR_STATUS_T* prMediaStatus;
    
    if ((u1SrcId >= PLAYMGR_MAX_INPUT_SRC) ||
        (!_arPlayerInfo[u1SrcId].fgOpen))
    {
        LOG(3, "%s: Wrong u1SrcId(%d), oops....\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    
    prMediaStatus = &_arPlayerInfo[u1SrcId].rPlayerStatus;
    
    LOG(5, "_PLAYMGR_Speed(%d)\n", u1SrcId);    

    if (i4Speed == _arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed)
    {
        LOG(5, "same speed\n");
        return TRUE;
    }

    if(_afgIsSeeking[u1SrcId])
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }

    if(!_PLAYMGR_GetCap(u1SrcId, PLAYMGR_CAP_TRICKMODE, i4Speed, 0))
    {
        LOG(5,"_PLAYMGR_Speed fgTrick(0)\n");
        return FALSE;
    }
    
    if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
    {
        _PLAYMGR_DmxFlushCtrl(u1SrcId);
        if (!SWDMX_Pause(u1SrcId))
        {
            LOG(1,"_PLAYMGR_Speed SWDMX_Pause fail\n");
        }
        _PLAYMGR_DmxWaitCtrlDone(u1SrcId);
    }
    
    //reset EOS flag if play to the end and than BF or
    //BF to the beginning and normal play
    //if(((i4Speed > 0) && (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed < 0)) ||
    //    ((i4Speed < 0) && (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed > 0)))
    {
        prMediaStatus->fgSendEos = FALSE;
    }
    
    if (i4Speed != 1000)
    {
        // ToDo: correct this.
        /*
        if (!(((_arPlayerInfo[u1SrcId].rPlayerStatus.i4PreviousSpeed == 1000) ||
            (_arPlayerInfo[u1SrcId].rPlayerStatus.i4PreviousSpeed == MM_SPEED_TYPE_PAUSE) ||
            (_arPlayerInfo[u1SrcId].rPlayerStatus.i4PreviousSpeed == 1)) &&
            (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed == 2000)))
        {

            UNUSED(SWDMX_Flush());
        }*/

        if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
        {
            switch (i4Speed)
            {
            case 1024000:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_1024X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1024X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_1024X;
    
            case 512000:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_512X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_512X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_512X;
                
            case 256000:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_256X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_256X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_256X;
                    
            case 128000:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_128X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_128X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_128X;
                
            case 64000:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_64X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_64X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_64X;
                
            case 32000:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_32X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_32X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_32X;
                
                break;
            case 16000:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_16X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_16X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_16X;
                
                break;
            case 8000:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_8X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_8X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_8X;
                
                break;
            case 4000:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_4X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_4X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_4X;
                
                break;
            case 3000:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_3X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_3X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_3X;
                
                break;
            case 2000:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_2X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_2X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_2X;
                
                break;
                /* Slow motion case */
            case 500:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_1_DIV_2X);
                VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, TRUE);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_2X, 0, 0);
                VDP_SetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_SLOW_FORWARD);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_1_DIV_2X;
                break;
            case 250:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_1_DIV_4X);
                VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, TRUE);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_4X, 0, 0);
                VDP_SetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_SLOW_FORWARD);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_1_DIV_4X;
                break;
            case 125:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_1_DIV_8X);
                VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, TRUE);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_8X, 0, 0);
                VDP_SetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_SLOW_FORWARD);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_1_DIV_8X;
                break;
            case 62:
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_1_DIV_16X);
                VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, TRUE);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_16X, 0, 0);
                VDP_SetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_SLOW_FORWARD);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_1_DIV_16X;
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
                VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_REWIND_1X);
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_REWIND_1X, 0, 0);
                eStcSpeed = STC_SPEED_TYPE_FORWARD_1X;
                break;

            default:
                fgSetOK = FALSE;
                ASSERT(0);
                return FALSE;
            }
            
            if ((_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum < (UINT16)PLAYMGR_MAX_VID_TRACK) &&
                ((_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
                 (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK)))
            {
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1Vdec2Id, VDEC_MM_PLAY_SPEED, eStcSpeed, 0, 0);
            }
            
            VDP_SetTrickMode(_arPlayerInfo[u1SrcId].u1B2rId,VDP_PLAY_TRICK);

            if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
            {
                if ((_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed == 1000) &&
                        (i4Speed == 2000))
                {
                    /* 1x -> 2x */
                    fgResetSwDmx = FALSE;
                }
                if ((_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed == 1000) &&
                        ((i4Speed < 1000) && (i4Speed >= 62)))
                {
                    /* 1x -> Slow motion */
                    fgResetSwDmx = FALSE;
                }
                if (((_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed < 1000) && (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed >= 62)) &&
                        ((i4Speed < 1000) && (i4Speed >= 62)))
                {
                    /* Slow motion -> Slow motion */
                    fgResetSwDmx = FALSE;
                }
                if (fgResetSwDmx)
                {
                    /* 1x -> 2x or Slow motion not to SWDMX ?! */
                    UNUSED(SWDMX_Flush(u1SrcId));
                }
            }
        }
        else if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
        {
            if(i4Speed > 0)
            {
                AUD_DspSpeed(_arPlayerInfo[u1SrcId].u1AdecId, (i4Speed/1000));
            }
            else
            {
                UNUSED(SWDMX_Flush(u1SrcId));

                if(_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
                {
                    _PLAYMGR_AdecFlushCtrl(u1SrcId);
                    // wait for audio API.
                    //AUD_DrvSetDelay();
#ifdef CC_53XX_AUDIO_V2
                     AUD_DSPCmdPlay(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
                     AUD_DSPCmdPlay(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
                    AUD_DSPCmdPlay(_arPlayerInfo[u1SrcId].u1AdecId);
#endif
                    _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
                }
                else
                {
                    LOG(1,"_PLAYMGR_Speed fgEnAudStrm false\n");
                }

                // Use 1000 to play -x speed audio
                AUD_DspSpeed(_arPlayerInfo[u1SrcId].u1AdecId, 0);
            }
        }

        if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            if ((_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm) &&
                    (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm))
            {
                if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_AUDIO, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
                }
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm = FALSE;
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisAudStrm = TRUE;
                /*
                _PLAYMGR_AdecFlushCtrl(u1SrcId);
#ifdef CC_53XX_AUDIO_V2
                AUD_DSPCmdStop(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
                AUD_DSPCmdStop(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
                AUD_DSPCmdStop(_arPlayerInfo[u1SrcId].u1AdecId);
#endif
                _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
                */
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = FALSE;
            }
        }
        if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm)
            {
                if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_PCR, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable pcr fail\n");
                }
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm = FALSE;
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisPcrStrm = TRUE;
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgPcrPlayed = FALSE;
            }
        }
    }
    else
    {
        // ToDo: correct this.
        /*
        if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
            (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype ==
             MEDIA_CONTNR_SUBTYPE_AVI) ||
             (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype ==
             MEDIA_CONTNR_SUBTYPE_MP4))
        {
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.i4PreviousSpeed != MM_SPEED_TYPE_PAUSE &&
                _arPlayerInfo[u1SrcId].rPlayerStatus.i4PreviousSpeed != MM_SPEED_TYPE_FORWARD_2X &&
                _arPlayerInfo[u1SrcId].rPlayerStatus.i4PreviousSpeed != MM_SPEED_TYPE_STEP)
            {
                UNUSED(SWDMX_Flush());
            }
        }
        else
        {
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.i4PreviousSpeed != MM_SPEED_TYPE_PAUSE)
            {
                UNUSED(SWDMX_Flush());
            }
        }*/

        if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
        {
            VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1X, 0, 0);

            if ((_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum < (UINT16)PLAYMGR_MAX_VID_TRACK) &&
                ((_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
                 (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK)))
            {
                VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1Vdec2Id, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1X, 0, 0);
            }
            
            VDP_SetTrickMode(_arPlayerInfo[u1SrcId].u1B2rId,VDP_PLAY_NORMAL);
            if ((_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed < 1000) && (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed >= 62))
            {
                /* Slow Forward to Normal */
                if(VDP_GetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId) != 0)
                {
                    VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
                    VDP_SetSlowForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_NORMAL);
                }
            }

            if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
            {
                if ((_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
                 || (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4)
                 || (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
                 || (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM)
                 || (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV))
                {
                    if ((_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed != MM_SPEED_TYPE_PAUSE) &&
                        (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed != 2000/*MM_SPEED_TYPE_FORWARD_2X*/) &&
                        (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed != 500) &&
                        (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed != 333) &&
                        (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed != 250) &&
                        (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed != 125) &&
                        (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed != 62) && 
                        (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed != 1))
                    {
                        UNUSED(SWDMX_Flush(u1SrcId));
                    }
                }
                else
                {
                    if ((_arPlayerInfo[u1SrcId].ePlayMgrCurStatus != PLAYMGR_STATE_PAUSE) ||
                        // previous is step
                        (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed != 1000))
                    {
                        UNUSED(SWDMX_Flush(u1SrcId));
                    }
                }
            }
            else
            {
                if ((_arPlayerInfo[u1SrcId].ePlayMgrCurStatus != PLAYMGR_STATE_PAUSE) ||
                    // previous is step
                    (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed != 1000))
                {
                    UNUSED(SWDMX_Flush(u1SrcId));
                }
            }
        }
        else if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
        {
            AUD_DspSpeed(_arPlayerInfo[u1SrcId].u1AdecId, 0);
        }

        if (fgSetOK)
        {
            _arPlayerInfo[u1SrcId].rPlayerStatus.i4PreviousSpeed =
                _arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed;
            _arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed = i4Speed;
        }
        else
        {
            LOG(1, "_PLAYMGR_Speed SWDMX_SetSpeed fgSetOK Fail\n");
        }

        if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgDisAudStrm)
        {
            if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_AUDIO, TRUE))
            {
                LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
            }
            _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm = TRUE;
        }

        // AV sync mode must be set before audio play
        _PLAYMGR_SetSyncMode(u1SrcId);
        // speed should not start stc
        //STC_StartStc();

        if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgDisAudStrm)
            {
#if 0
                if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_AUDIO, TRUE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
                }
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm = TRUE;
#endif
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisAudStrm = FALSE;

                if (_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum >= PLAYMGR_MAX_AUD_TRACK)
                {
                    LOG(1,"_PLAYMGR_CtlStm u2EnATrackNum >= PLAYMGR_MAX_AUD_TRACK\n");
                    _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum = (UINT16)(PLAYMGR_MAX_AUD_TRACK-1);
                }

                LOG(5, "aeAudFormat %d\n", _arPlayerInfo[u1SrcId].rPlayerStatus.aeAudFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum]);

                switch (_arPlayerInfo[u1SrcId].rPlayerStatus.aeAudFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum])
                {
                case PLAYMGR_ADEC_MP3:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#else
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#endif
                    break;
                case PLAYMGR_ADEC_AC3:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#else
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#endif
                    break;
                case PLAYMGR_ADEC_AAC:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#else
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#endif
                    break;
                case PLAYMGR_ADEC_MPEG:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#else
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#endif
                    break;
                case PLAYMGR_ADEC_PCM:
                    if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                            (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
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
                        AUD_PcmSetting(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, &rPCMInfo);
                        AUD_PcmSetting(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, &rPCMInfo);
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_PCM);
#else
                    AUD_PcmSetting(_arPlayerInfo[u1SrcId].u1AdecId, &rPCMInfo);
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_PCM);
#endif
                    break;
                case PLAYMGR_ADEC_WMA:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#else
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#endif
                    break;
#if 1//def CC_MT5363
                case PLAYMGR_ADEC_RA:
#ifdef CC_53XX_AUDIO_V2
                    AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                    AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#else
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#endif
                    break;
#endif
                default:
                    return FALSE;
                }
                _PLAYMGR_AdecFlushCtrl(u1SrcId);
                // wait for audio API.
                //AUD_DrvSetDelay();
#ifdef CC_53XX_AUDIO_V2
                AUD_DSPCmdPlay(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
                AUD_DSPCmdPlay(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
                AUD_DSPCmdPlay(_arPlayerInfo[u1SrcId].u1AdecId);
#endif
                _PLAYMGR_AdecWaitCtrlDone(u1SrcId);

                AUD_DspSpeed(_arPlayerInfo[u1SrcId].u1AdecId, 0);
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = TRUE;
            }
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgDisPcrStrm)
            {
                if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_PCR, TRUE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable pcr fail\n");
                }
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm = TRUE;
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisPcrStrm = FALSE;
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgPcrPlayed = TRUE;
            }
        }
    }

    if (fgSetOK)
    {
        _arPlayerInfo[u1SrcId].rPlayerStatus.i4PreviousSpeed =
            _arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed;
        _arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed = i4Speed;
    }
    else
    {
        LOG(1, "_PLAYMGR_Speed SWDMX_SetSpeed fgSetOK Fail\n");
    }

    //setting av sync mode before playing audio
    //_PLAYMGR_SetSyncMode(u1SrcId);
    // speed should not start stc
    //STC_StartStc();

    if(i4Speed != 1000)
    {
        _PLAYMGR_SetSyncMode(u1SrcId);
    }

    if(i4Speed>2000 || i4Speed<0)
    {
		SWDMX_SetPlayMode(u1SrcId,SWDMX_PLAY_MODE_IFRAME);
    }
	else
	{
		SWDMX_SetPlayMode(u1SrcId,SWDMX_PLAY_MODE_NORMAL);
	}
	
    if (!SWDMX_SetSpeed(u1SrcId, i4Speed))
    {
        LOG(1, "_PLAYMGR_Speed SWDMX_SetSpeed Fail\n");
    }

    if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
    {
        fgRet = SWDMX_Play(u1SrcId);
    }

    return fgRet;
}


/**Get current speed
*@param
*@retval current speed
*/
INT32 _PLAYMGR_GetSpeed(UINT8 u1SrcId)
{
    if ((u1SrcId >= PLAYMGR_MAX_INPUT_SRC) ||
        (!_arPlayerInfo[u1SrcId].fgOpen))
    {
        LOG(3, "Wrong playmgr source id %s,%d, oops....\n", __FUNCTION__, __LINE__);
        return -1;
    }

    LOG(6, "_PLAYMGR_GetSpeed src id = %d\n", u1SrcId);
    
    return _arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed;
}

BOOL _PLAYMGR_GetDataExhausted(UINT8 u1SrcId)
{
    UINT16 u2VdecQueueSize =0;
    UINT16 u2VdecMaxQueueSize=0;
    static UINT8 u1DataExhaustedCount = 0;

    VDEC_GetQueueInfo(_arPlayerInfo[u1SrcId].u1VdecId, &u2VdecQueueSize,&u2VdecMaxQueueSize);
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
BOOL _PLAYMGR_Flush(UINT8 u1SrcId)
{
    // Please make sure you call this function with swdmx/vdec/adec paused.
    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    
    if((_afgIsSeeking[u1SrcId]) &&
       (_arPlayerInfo[u1SrcId].ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }
    
    SWDMX_Flush(u1SrcId);
    // audio is stopped while calling AUD_WaitDspFlush(_arPlayerInfo[u1SrcId].u1AdecId)
    _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = FALSE;

    return TRUE;
}

/**Set the seek position by time.
*@param handle
*@param seek time(millisecond)
*@retval TRUE                    Success.
*@retval FALSE                   Fail.
*/
BOOL _PLAYMGR_SeekTime(UINT8 u1SrcId, UINT32 u4SeekTime)
{
    BOOL fgRet;
    UINT64 u8FilePos;

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);

    LOG(5, "Seektime = %d\n", u4SeekTime);
    
    _PLAYMGR_Flush(u1SrcId);

    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
    {
        if((_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_PS) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_192) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_ZERO_192) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_ES) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_H264_ES) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AVS_ES) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG4_ES) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VC1_ES) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VCD) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VOB) ||
           (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VRO))
        {
            VDEC_SetRenderFromFirstPic(_arPlayerInfo[u1SrcId].u1VdecId, TRUE, _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize);
            if ((_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum < (UINT16)PLAYMGR_MAX_VID_TRACK) &&
                ((_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
                 (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK)))
            {
                VDEC_SetRenderFromFirstPic(_arPlayerInfo[u1SrcId].u1Vdec2Id, TRUE, _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize);
            }
        }
         /*else  //set in each container part
        {
            VDEC_SetRenderFromPts(u1SrcId, u4SeekTime);
        }*/
        VDEC_RegRenderPtsCb(_arPlayerInfo[u1SrcId].u1VdecId, _PLAYMGR_SeekFinNotify, u1SrcId);
        VDP_SetPauseMm(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
    }
    else
    {   // audio only or no a/v case.
        STC_StopStc(_arPlayerInfo[u1SrcId].u1StcId);
        STC_SetStcValue(_arPlayerInfo[u1SrcId].u1StcId, (UINT32)(u4SeekTime*90));
        
        _PLAYMGR_SeekFinNotify(u4SeekTime*90, u1SrcId);
        #if 1
        FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_SEEK_END, (u4SeekTime));
        #endif
    }

    if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT)
    {
        // SWDMX_SeekTime eat tick in time-shift
        fgRet = SWDMX_SeekTime(u1SrcId, u4SeekTime, &u8FilePos);
    }
    else if (_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        UINT64 u8Remainder;
        MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;   
        
        u8FilePos = (UINT64)u4SeekTime;             
        prAudStrmAttr = &_arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rStreamAttr;
        
        u8FilePos *= (UINT64)prAudStrmAttr->u.t_es_audio_stm_attr.ui4_sampling_frequency;
        u8FilePos *= (UINT64)prAudStrmAttr->u.t_es_audio_stm_attr.ui2_channels;
        
        u8FilePos *= 8;
        if(prAudStrmAttr->u.t_es_audio_stm_attr.ui2_bits_per_sample > 0)
        {
            //u8FilePos /= prAudStrmAttr->u.t_es_audio_stm_attr.ui2_bits_per_sample;
            u8FilePos = u8Div6432(u8FilePos,
                (UINT64)prAudStrmAttr->u.t_es_audio_stm_attr.ui2_bits_per_sample,
                &u8Remainder);
                
            fgRet = _PLAYMGR_SeekPosition(u1SrcId, u8FilePos);
        }
        else
        {
            fgRet = FALSE;
        }
    }
    else
    {
        // SWDMX_SeekTime eat 90K time unit
        fgRet = SWDMX_SeekTime(u1SrcId, u4SeekTime*90, &u8FilePos);
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
BOOL _PLAYMGR_SeekPosition(UINT8 u1SrcId, UINT64 u8SeekPos)
{
    BOOL fgRet;
    UINT32 u4Time;
    UINT64 u8Time;
    UINT64 u8Remainder;

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);

    LOG(5, "Pos = %lld\n", u8SeekPos);
    _PLAYMGR_Flush(u1SrcId);

    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
    {
        VDEC_SetRenderFromFirstPic(_arPlayerInfo[u1SrcId].u1VdecId, TRUE, _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize);
        if ((_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum < (UINT16)PLAYMGR_MAX_VID_TRACK) &&
            ((_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
             (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK)))
        {
            VDEC_SetRenderFromFirstPic(_arPlayerInfo[u1SrcId].u1Vdec2Id, TRUE, _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize);
        }
        VDEC_RegRenderPtsCb(_arPlayerInfo[u1SrcId].u1VdecId, _PLAYMGR_SeekFinNotify, u1SrcId);
        VDP_SetPauseMm(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
    }
    else if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
    {   // audio only
        u4Time = 0;
        if (_arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize)
        {
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize > (UINT64)0xFFFFFFFF)
            {
                LOG(0, "_PLAYMGR_SeekPosition FixMe\n");
            }
            u8Time = u8SeekPos;
            u8Time *= _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration;
            u4Time = u8Div6432(u8Time, _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize, &u8Remainder);
        }
        STC_StopStc(_arPlayerInfo[u1SrcId].u1StcId);
        STC_SetStcValue(_arPlayerInfo[u1SrcId].u1StcId, (UINT32)(u4Time*90));
        _PLAYMGR_SeekFinNotify(u4Time*90, u1SrcId);
        #if 1
        FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_SEEK_END, (u4Time));
        #endif

    }

    fgRet = SWDMX_SeekPos(u1SrcId, u8SeekPos, u8SeekPos);
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
BOOL _PLAYMGR_SeekPercent(UINT8 u1SrcId, UINT32 u4SeekPercent)
{
    BOOL fgRet;
    UINT32 u4Time;
    UINT64 u8FilePos;
    UINT64 u8Time;
    UINT64 u8Remainder;

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);

    LOG(5, "seek percent = %d\n", u4SeekPercent);
    _PLAYMGR_Flush(u1SrcId);

    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
    {
        LOG(5, "_PLAYMGR_SeekPercent fgEnVidStrm\n");
        VDEC_SetRenderFromFirstPic(_arPlayerInfo[u1SrcId].u1VdecId, TRUE, _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize);
        if ((_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum < (UINT16)PLAYMGR_MAX_VID_TRACK) &&
            ((_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
             (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK)))
        {
            VDEC_SetRenderFromFirstPic(_arPlayerInfo[u1SrcId].u1Vdec2Id, TRUE, _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize);
        }
        VDEC_RegRenderPtsCb(_arPlayerInfo[u1SrcId].u1VdecId, _PLAYMGR_SeekFinNotify, u1SrcId);
        VDP_SetPauseMm(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);
    }
    else
    {   // audio only
        u4Time = 0;
        if (_arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration)
        {
            u8Time = _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration;
            u8Time *= u4SeekPercent;
            u4Time = u8Div6432(u8Time, (UINT64)100, &u8Remainder);
        }
        STC_StopStc(_arPlayerInfo[u1SrcId].u1StcId);
        STC_SetStcValue(_arPlayerInfo[u1SrcId].u1StcId, (UINT32)(u4Time*90));
        _PLAYMGR_SeekFinNotify(u4Time*90, u1SrcId);
        #if 1
        FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_SEEK_END, (u4Time));
        #endif

    }

    LOG(5, "_PLAYMGR_SeekPercent SWDMX_SeekPercent\n");
    fgRet = SWDMX_SeekPercent(u1SrcId, u4SeekPercent, &u8FilePos);
    if (fgRet == FALSE)
    {
        LOG(1, "_PLAYMGR_SeekPercent SWDMX_SeekPercent fail\n");
        return fgRet;
    }

    return TRUE;
}

BOOL _PLAYMGR_GetMetaData(UINT8 u1SrcId, PLAYMGR_AVMETA_T *prMeta)
{
    UINT64 u8TempRemainder;
    UINT16 u2I;

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    LOG(5, "_PLAYMGR_GetMetaData SrcId = %d\n", u1SrcId);

    prMeta->u8MediaSize = _arPlayerInfo[u1SrcId].u8FileSize;
    if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
    {
        if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3)
        {
            prMeta->pu2Title = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.pwc_str_id3_title;
            prMeta->pu2Album = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.pwc_str_id3_album;
            prMeta->pu2Artist = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.pwc_str_id3_artist;
            prMeta->pu2Genre = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.pwc_str_id3_genre;
            prMeta->u4TitleLen = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.z_id3_title_len;
            prMeta->u4AlbumLen = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.z_id3_album_len;
            prMeta->u4ArtistLen = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.z_id3_artist_len;
            prMeta->u4GenreLen = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.z_id3_genre_len;
            prMeta->u4SampleFreq = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rStreamAttr.u.t_mp3_audio_stm_attr.ui4_sample_rate;
            prMeta->u4BitRate = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rStreamAttr.u.t_mp3_audio_stm_attr.ui4_bitrate;
            prMeta->u4FrameSize = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rStreamAttr.u.t_mp3_audio_stm_attr.ui4_frame_length;
            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rPlayerGenericInfo.ui8_pb_duration;
            prMeta->u4ImgPos = prMeta->u4ImgSize = 0;
            for (u2I = 0; u2I < MINFO_MAX_ID3_PIC_NUM; u2I++)
            {
                if ((_arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.at_id3_attatched_pic[u2I].t_format.e_media_type
                        == MEDIA_TYPE_IMAGE) &&
                        (_arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.at_id3_attatched_pic[u2I].t_format.t_media_subtype.u.e_img_subtype
                         == MEDIA_IMG_SUBTYPE_JPG))
                {
                    prMeta->u4ImgPos =
                        _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.at_id3_attatched_pic[u2I].ui4_starting_offset;
                    prMeta->u4ImgSize =
                        _arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rMp3Id3Data.at_id3_attatched_pic[u2I].ui4_length;
                    break;
                }
            }
            //return TRUE;
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP2)
        {
            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration;
            //return TRUE;
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WMA)
        {
            //ToDo: wma add meta data in this scope
            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvGenericInfo[0].ui8_pb_duration;
            prMeta->pu2Title = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_title;
            prMeta->u4TitleLen  = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.z_title_len;
            prMeta->pu2Artist = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_director;
            prMeta->u4ArtistLen = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.z_director_len;
            prMeta->pu2Genre = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_genre;
            prMeta->u4GenreLen = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.z_genre_len;
            //return TRUE;
        }
    }
    else if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
    {
        if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
        {
            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvGenericInfo[0].ui8_pb_duration;
            prMeta->pu2Title = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_title;
            prMeta->u4TitleLen  = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.z_title_len;
            prMeta->pu2Artist = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_director;
            prMeta->u4ArtistLen = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.z_director_len;
            prMeta->pu2Genre = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.pwc_str_genre;
            prMeta->u4GenreLen = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.z_genre_len;
            prMeta->u4BitRate = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.ui4_bitrate;
            prMeta->u4FrameRate = _arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rWmvMetaData.t_frame_rate.ui4_rate;
            prMeta->u4Width = (UINT32)_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rStmAttribute[0][0].u.t_asf_video_stm_attr.i4_width;
            prMeta->u4Height = (UINT32)_arPlayerInfo[u1SrcId].FmtInfo.rWmvInfo.rStmAttribute[0][0].u.t_asf_video_stm_attr.i4_height;
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
        {
            PLAYMGR_AVI_INFO_T* prAviInfo = &_arPlayerInfo[u1SrcId].FmtInfo.rAviInfo;

            prMeta->u4Width = (UINT32)prAviInfo->rStmAttribute[0].u.t_avi_video_stm_attr.i4_width;
            prMeta->u4Height = (UINT32)prAviInfo->rStmAttribute[0].u.t_avi_video_stm_attr.i4_height;

            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.rAviMetaData.ui8_pb_duration;
            prMeta->pu2Title = _arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.rAviMetaData.pwc_str_title;
            prMeta->u4TitleLen  = _arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.rAviMetaData.z_title_len;
            prMeta->pu2Artist = _arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.rAviMetaData.pwc_str_director;
            prMeta->u4ArtistLen = _arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.rAviMetaData.z_director_len;
            prMeta->pu2Genre = _arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.rAviMetaData.pwc_str_genre;
            prMeta->u4GenreLen = _arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.rAviMetaData.z_genre_len;
            prMeta->u4BitRate = _arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.rAviMetaData.ui4_bitrate;
            prMeta->u4FrameRate = _arPlayerInfo[u1SrcId].FmtInfo.rAviInfo.rAviMetaData.t_frame_rate.ui4_rate;
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
        {
            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.ui8_pb_duration;
            prMeta->u4TitleLen = _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.z_title_len;
            prMeta->pu2Title = _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.pwc_str_title;
            prMeta->u4ArtistLen = _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.z_director_len;
            prMeta->pu2Artist = _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.pwc_str_director;
            prMeta->u4GenreLen = _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.z_genre_len;
            prMeta->pu2Genre = _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.pwc_str_genre;
            prMeta->u4BitRate = _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.ui4_bitrate;
            prMeta->u4FrameRate = _arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.t_frame_rate.ui4_rate;
            prMeta->u4Width = (UINT32)_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rStmAttribute[0].u.t_mkv_video_stm_attr.i4_width;
            prMeta->u4Height = (UINT32)_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rStmAttribute[0].u.t_mkv_video_stm_attr.i4_height;

            //_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.t_frame_rate.ui4_scale
            //_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.z_copyright_len
            //_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.pwc_str_copyright
            //_arPlayerInfo[u1SrcId].FmtInfo.rMkvInfo.rMetaData.b_cr_date_exist
        }
        else if (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4)
        {
            PLAYMGR_MP4_INFO_T* prMp4Info = &_arPlayerInfo[u1SrcId].FmtInfo.rMp4Info;

            prMeta->u4Width = (UINT32)prMp4Info->rStmAttribute[0].u.t_mp4i_video_stm_attr.i4_width;
            prMeta->u4Height = (UINT32)prMp4Info->rStmAttribute[0].u.t_mp4i_video_stm_attr.i4_height;

            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rMp4MetaData.ui8_pb_duration;
            prMeta->pu2Title = _arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rMp4MetaData.pwc_str_title;
            prMeta->u4TitleLen  = _arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rMp4MetaData.z_title_len;
            prMeta->pu2Artist = _arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rMp4MetaData.pwc_str_director;
            prMeta->u4ArtistLen = _arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rMp4MetaData.z_director_len;
            prMeta->pu2Genre = _arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rMp4MetaData.pwc_str_genre;
            prMeta->u4GenreLen = _arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rMp4MetaData.z_genre_len;
            prMeta->u4BitRate = _arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rMp4MetaData.ui4_bitrate;
            prMeta->u4FrameRate = _arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rMp4MetaData.t_frame_rate.ui4_rate;
        }
		else if(_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV)
		{
			PLAYMGR_FLV_INFO_T* prFlvInfo = &_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo;
			
			prMeta->u4Width = (UINT32)prFlvInfo->rStmAttribute[0].u.t_flv_video_stm_attr.i4_width;
			prMeta->u4Height = (UINT32)prFlvInfo->rStmAttribute[0].u.t_flv_video_stm_attr.i4_height;
			prMeta->u8PlayTime = prFlvInfo->rStmAttribute[0].u.t_flv_video_stm_attr.ui8_duration;
			prMeta->u4FrameRate = prFlvInfo->rStmAttribute[0].u.t_flv_video_stm_attr.ui4_rate;
		}
        else if(_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM)
        {
            PLAYMGR_RM_INFO_T* prRmInfo = &_arPlayerInfo[u1SrcId].FmtInfo.rRmInfo;

            prMeta->u4Width = prRmInfo->rRmMetaData.t_video_es_info.i4_video_inf_width;
            prMeta->u4Height = prRmInfo->rRmMetaData.t_video_es_info.i4_video_inf_height;
            prMeta->u4FrameRate = prRmInfo->rRmMetaData.t_frame_rate.ui4_rate;
            prMeta->u8PlayTime = prRmInfo->rRmMetaData.ui8_pb_duration;
            prMeta->pu2Title = prRmInfo->rRmMetaData.pwc_str_title;
            prMeta->u4TitleLen  = prRmInfo->rRmMetaData.z_title_len;
            prMeta->pu2Artist = prRmInfo->rRmMetaData.pwc_str_artist;
            prMeta->u4ArtistLen = prRmInfo->rRmMetaData.z_artist_len;
            prMeta->pu2Genre = prRmInfo->rRmMetaData.pwc_str_genre;
            prMeta->u4GenreLen = prRmInfo->rRmMetaData.z_genre_len;
            prMeta->u4BitRate = prRmInfo->rRmMetaData.ui4_bitrate;
        }
    }
    else if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
    {
        if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_ES)
        {
            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration;
            //return TRUE;
        }
        else if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_H264_ES)
        {
            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration;
            //return TRUE;
        }
        else if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_AVS_ES)
        {
            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration;
            //return TRUE;
        }
        else if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_VC1_ES)
        {
            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration;
            //return TRUE;
        }
        else if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG4_ES)
        {
            prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration;
            //return TRUE;
        }
    }
    else if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_PS)
    {
        PLAYMGR_PS_INFO_T *prPsInfo = &_arPlayerInfo[u1SrcId].FmtInfo.rPsInfo;
        prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].FmtInfo.rPsInfo.rPlayerGenericInfo.ui8_pb_duration;
        prMeta->u4Width = (UINT32)prPsInfo->rStmAttribute[0].u.t_ps_video_stm_attr.i4_width;
        prMeta->u4Height = (UINT32)prPsInfo->rStmAttribute[0].u.t_ps_video_stm_attr.i4_height;
        //return TRUE;
    }
    else if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_TS)
    {
        prMeta->u8PlayTime = _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration;
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


BOOL _PLAYMGR_SetFifo(UINT8 u1SrcId, PLAYMGR_FIFO_T *prFifo)
{
    return TRUE;
}

UINT64 _PLAYMGR_GetBytePos(UINT8 u1SrcId)
{
    UINT64 u8CurPos;
    if (SWDMX_GetInfo(u1SrcId, eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8CurPos, 0, 0))
    {
        return u8CurPos;
    }
    return 0;
}

UINT32 _PLAYMGR_GetPos(UINT8 u1SrcId)
{
    UINT32 u4Time;
    UINT64 u8CurPos;
    UINT64 u8Tmp;
    UINT64 u8Remainder;
    VDP_PIC_INFO_T rPicInfo;
    u4Time = 0;
    
    if ((u1SrcId >= PLAYMGR_MAX_INPUT_SRC) ||
        (!_arPlayerInfo[u1SrcId].fgOpen))
    {
        LOG(3, "Wrong u1SrcId %s,%d, oops....\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    x_memset(&rPicInfo, 0, sizeof(VDP_PIC_INFO_T));
    if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
    {
        u8CurPos = 0;
        if (VDP_GetPicInfo(_arPlayerInfo[u1SrcId].u1B2rId, &rPicInfo) == VDP_SET_OK)
        {
            u8CurPos = rPicInfo.u8Offset;
        }
        else if (SWDMX_GetInfo(u1SrcId, eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8CurPos, 0, 0))
        {
        }
        //u4Time = (u8CurPos / _arPlayerInfo[u1SrcId].u8FileSize) *
        //    _arPlayerInfo[u1SrcId].u4TotalTime;
        if ((_arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration > 0) &&
                (_arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize > 0))
        {
            // 90000 -> 1000
            u8Tmp = u8Div6432(_arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize * 90,
                              _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration,
                              &u8Remainder);
            if(u8Tmp > 0)
            {
                u4Time = (UINT32)u8Div6432(u8CurPos,
                                           (UINT32)u8Tmp, &u8Remainder);
            }
        }
    }
    else if(_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
    {
        #if 0
        UINT64 u8FileOffset;
        u8CurPos = 0;
        UNUSED(SWDMX_GetInfo(u1SrcId, eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8CurPos, 0, 0));
        u8FileOffset = 0;
        UNUSED(SWDMX_GetInfo(u1SrcId, eSWDMX_GET_FILE_OFFSET, (UINT32*)&u8FileOffset, 0, 0));
        if ((_arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration > 0) &&
                (_arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize > 0))
        {
            // 90000 -> 1000
            u8Tmp = u8Div6432((_arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize - u8FileOffset) * 90000,
                              _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration,
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
        UNUSED(SWDMX_GetInfo(u1SrcId, eSWDMX_GET_CURRENT_TIME, (UINT32*)&u4Time, 0, 0));
        if (u4Time > (0xFFFFFFFF - 90000)) // -1 sec
        {
            u4Time = 0;
        }
        else
        {
            u4Time /= 90;
        }
        if(_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
        {
            UINT64 u8Remainder;
            MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;
            prAudStrmAttr =&_arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rStreamAttr;
            UNUSED(SWDMX_GetInfo(u1SrcId, eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8CurPos, 0, 0));

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
        else if(_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_RHAPSODY)
        {
            u4Time = STC_GetStcValue(_arPlayerInfo[u1SrcId].u1StcId);
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
        if (((_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm) && (MPV_IsDisplay(u1SrcId))) ||
                (!_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)) // audio only
        {
            // Getting Pts from VDP for 4X/8X/16X/32X or -2X/-4X/-8X/-16X/-32X
            if ((_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed > 2000 ||_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed < 0) &&
                    (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm))
            {
                VDP_PIC_INFO_T rVdpPicInfo = {0};

                if (VDP_GetPicInfo(_arPlayerInfo[u1SrcId].u1B2rId, &rVdpPicInfo) != VDP_SET_OK)
                {
                    return 0;
                }

                u4Time = (rVdpPicInfo.u4Pts/90);
            }
            else
            {
                u4Time = STC_GetStcValue(_arPlayerInfo[u1SrcId].u1StcId);
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

UINT32 _PLAYMGR_GetTick(UINT8 u1SrcId)
{
    UINT32 u4Tick;
    VDP_PIC_INFO_T rPicInfo;

    u4Tick = 0;
    x_memset(&rPicInfo, 0, sizeof(VDP_PIC_INFO_T));
    if (VDP_GetPicInfo(_arPlayerInfo[u1SrcId].u1B2rId, &rPicInfo) == VDP_SET_OK)
    {
        u4Tick = rPicInfo.u4TickNum;
    }
    else if (SWDMX_GetInfo(u1SrcId, eSWDMX_GET_CURRENT_TICK, (UINT32*)&u4Tick, 0, 0))
    {
        //u4Tick
    }
    LOG(6, "_PLAYMGR_GetTick %d\n", u4Tick);
    return u4Tick;
}

BOOL _PLAYMGR_CtlStm(UINT8 u1SrcId, UINT32 u4InfoValue, BOOL fgEnable)
{
    BOOL fgRet = TRUE;
    PLAYMGR_VDEC_T  ePlaymgrVdec = PLAYMGR_VDEC_UNKNOWN;
    AUD_PCM_SETTING_T rPCMInfo;
    AUD_LPCM_INFO_T rLPCMInfo;            //for LPCM setting
    STC_SPEED_TYPE_T eStcSpeed = STC_SPEED_TYPE_FORWARD_1X;
    ENUM_VDEC_FMT_T eVdecFmt = VDEC_FMT_MPV;
    //PLAYMGR_STATUS_T* prMediaStatus;;

    if((_afgIsSeeking[u1SrcId]) &&
       (_arPlayerInfo[u1SrcId].ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    
    //prMediaStatus = &_arPlayerInfo[u1SrcId].rPlayerStatus;

    x_memset(&rPCMInfo,0,sizeof(AUD_PCM_SETTING_T));
    if (u4InfoValue == PLAYMGR_INFO_MASK_VIDEO)
    {
        if (_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum >= (UINT16)PLAYMGR_MAX_VID_TRACK)
        {
            LOG(1,"_PLAYMGR_CtlStm u2EnVTrackNum >= PLAYMGR_MAX_VID_TRACK\n");
            _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum = (UINT16)(PLAYMGR_MAX_VID_TRACK-1);
        }

        ePlaymgrVdec = (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum]);

        if (fgEnable)
        {
            // handle video track change
            if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                VDEC_PlayMM(_arPlayerInfo[u1SrcId].u1VdecId , TRUE);
                if((ePlaymgrVdec == PLAYMGR_VDEC_MVC) ||
                   (ePlaymgrVdec == PLAYMGR_VDEC_MULTITRACK))
                {
                    VDEC_PlayMM(_arPlayerInfo[u1SrcId].u1Vdec2Id , TRUE);
                }
                switch(_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed)
                {
                    case 1024000:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_1024X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1024X, 0, 0);
                        break;
                    case 512000:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_512X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_512X, 0, 0);
                        break;
                    case 256000:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_256X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_256X, 0, 0);
                        break;
                    case 128000:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_128X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_128X, 0, 0);
                        break;
                    case 64000:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_64X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_64X, 0, 0);
                        break;
                    case 32000:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_32X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_32X, 0, 0);
                        break;
                    case 16000:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_16X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_16X, 0, 0);
                        break;
                    case 8000:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_8X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_8X, 0, 0);
                        break;
                    case 4000:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_4X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_4X, 0, 0);
                        break;
                    case 3000:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_3X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_3X, 0, 0);
                        break;
                    case 2000:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_2X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_2X, 0, 0);
                        break;
                        /* Slow motion case */
                    case 500:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_1_DIV_2X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_2X, 0, 0);
                        break;
                    case 333:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_1_DIV_3X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_3X, 0, 0);
                        break;
                    case 250:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_1_DIV_4X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_4X, 0, 0);
                        break;
                    case 125:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_1_DIV_8X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_8X, 0, 0);
                        break;
                    case 62:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_1_DIV_16X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1_DIV_16X, 0, 0);
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
                    case -3000:
                    case -2000:
                    case -1000:
                    case -500:
                    case -1:
                        eStcSpeed = STC_SPEED_TYPE_FORWARD_1X;
                        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_REWIND_1X, 0, 0);
                        break;
                    default:
                        break;
                }
                if((_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
                   (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK))
                {
                    VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1Vdec2Id, VDEC_MM_PLAY_SPEED, eStcSpeed, 0, 0);
                }
                _PLAYMGR_SetSyncMode(u1SrcId);
                switch (_arPlayerInfo[u1SrcId].rPlayerStatus.aeVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum])
                {
                case PLAYMGR_VDEC_MPEG4:
                case PLAYMGR_VDEC_DIVX311:
                    eVdecFmt = VDEC_FMT_MP4;
                    fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_MP4);
                    //DMX_SetVideoType(DMX_VIDEO_MPEG4);
                    break;
                case PLAYMGR_VDEC_MPEG1_MPEG2:
                    eVdecFmt = VDEC_FMT_MPV;
                    fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_MPV);
                    //DMX_SetVideoType(DMX_VIDEO_MPEG);
                    break;
                case PLAYMGR_VDEC_H264:
                    eVdecFmt = VDEC_FMT_H264;
                    fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_H264);
                    //DMX_SetVideoType(DMX_VIDEO_H264);
                case PLAYMGR_VDEC_AVS:
                    eVdecFmt = VDEC_FMT_AVS;
                    fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_AVS);
                    //DMX_SetVideoType(DMX_VIDEO_AVS);
                    break;
                case PLAYMGR_VDEC_WMV7:
                case PLAYMGR_VDEC_WMV8:
                case PLAYMGR_VDEC_WMV9:
                case PLAYMGR_VDEC_VC1:
                    eVdecFmt = VDEC_FMT_WMV;
                    fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_WMV);
                    //DMX_SetVideoType(DMX_VIDEO_VC1);
                    break;
                case PLAYMGR_VDEC_RV8:
                case PLAYMGR_VDEC_RV9:
                    eVdecFmt = VDEC_FMT_RV;
                    fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_RV);
                    break;
                case PLAYMGR_VDEC_MJPEG:
                    eVdecFmt = VDEC_FMT_MJPEG;
                    fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_MJPEG);
                    break;
                case PLAYMGR_VDEC_VP6:
                    eVdecFmt = VDEC_FMT_VP6;
                    fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_VP6);
                    break;
                case PLAYMGR_VDEC_VP8:
                    eVdecFmt = VDEC_FMT_VP8;
                    fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_FMT_VP8);
                    break;
                default:
                    LOG(1,"_PLAYMGR_CtlStm unknown aeVidFormat(%d)\n",
                        (_arPlayerInfo[u1SrcId].rPlayerStatus.aeVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum]));
                    return FALSE;
                }
                if((_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
                   (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK))
                {
                    fgRet = VDEC_Play(_arPlayerInfo[u1SrcId].u1Vdec2Id, eVdecFmt);
                }
            }

            fgRet = SWDMX_SetInfo(u1SrcId, 
                                  eSWDMX_SET_VIDEOTYPE,
                                  _PLAYMGR_PlaymgrVdec2Swdmx(u1SrcId, 
                                      _arPlayerInfo[u1SrcId].rPlayerStatus.aeVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum]),
                                  0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set video format info\n");
                return FALSE;
            }
            
            if((ePlaymgrVdec == PLAYMGR_VDEC_MVC) ||
               (ePlaymgrVdec == PLAYMGR_VDEC_MULTITRACK))
            {
                fgRet = SWDMX_SetInfo(u1SrcId, 
                                      eSWDMX_SET_SUBVIDEOTYPE,
                                      _PLAYMGR_PlaymgrVdec2Swdmx(u1SrcId, 
                                          _arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum]),
                                      0, 0);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_CtlStm Fail to set video 2 format info\n");
                    return FALSE;
                }
            }
            
            fgRet = SWDMX_SetInfo(u1SrcId, 
                (UINT8)eSWDMX_SET_VDEC_ID,
                (UINT32)_arPlayerInfo[u1SrcId].u1VdecId, 
                0, 
                0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set vdec id\n");
                return FALSE;
            }

            if((ePlaymgrVdec == PLAYMGR_VDEC_MVC) ||
               (ePlaymgrVdec == PLAYMGR_VDEC_MULTITRACK))
            {
                fgRet = SWDMX_SetInfo(u1SrcId, 
                    (UINT8)eSWDMX_SET_VDEC2_ID,
                    (UINT32)_arPlayerInfo[u1SrcId].u1Vdec2Id, 
                    0, 
                    0);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_CtlStm Fail to set vdec 2 id\n");
                    return FALSE;
                }
            }
            
#ifdef TIME_SHIFT_SUPPORT
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT)
            {
                if(_arPlayerInfo[u1SrcId].rPlayerStatus.fgPlayVidDecrypt)
                {
                    fgRet = SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VID_PID_IDX,
                                            _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayVidIdx,
                                            _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayVidKeyIdx,
#ifdef CC_MT5387
                                            (UINT32)DMX_DESC_MODE_AES_ECB);
#else
                                            (UINT32)DMX_DESC_MODE_3DES_ECB);
#endif

                }
                else
                {
                    fgRet = SWDMX_SetInfo(u1SrcId, eSWDMX_SET_VID_PID_IDX,
                                            _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayVidIdx,
                                            0, 0);
                }
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_CtlStm Fail to set video pid idx\n");
                    return FALSE;
                }
            }
#endif

            fgRet = SWDMX_SetStrmID(u1SrcId, _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayVidStrmId,
                                    0, eSWDMX_STRM_TYPE_VID);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set video stream ID\n");
                return FALSE;
            }

            if((ePlaymgrVdec == PLAYMGR_VDEC_MVC) ||
               (ePlaymgrVdec == PLAYMGR_VDEC_MULTITRACK))
            {
                fgRet = SWDMX_SetStrmID(u1SrcId, _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlaySubVidStrmId,
                                        0, eSWDMX_STRM_TYPE_VID2);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_CtlStm Fail to set video 2 stream ID\n");
                    return FALSE;
                }
            }
            
            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to enable video stream\n");
                return FALSE;
            }
            if((ePlaymgrVdec == PLAYMGR_VDEC_MVC) ||
               (ePlaymgrVdec == PLAYMGR_VDEC_MULTITRACK))
            {
                fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID2, 0, 0);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_CtlStm Fail to enable video 2 stream\n");
                    return FALSE;
                }
            }
        }
        else
        {
            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_Set Fail to disable video stream\n");
                return FALSE;
            }
            
            if((ePlaymgrVdec == PLAYMGR_VDEC_MVC) ||
               (ePlaymgrVdec == PLAYMGR_VDEC_MULTITRACK))
            {
                fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID2, 0, 0);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_Set Fail to disable video 2 stream\n");
                    return FALSE;
                }
            }
            
            // handle video track change
            if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
                {
                    _PLAYMGR_VdecFlushCtrl(u1SrcId);
                    VDEC_Stop(_arPlayerInfo[u1SrcId].u1VdecId);                    
                    if((ePlaymgrVdec == PLAYMGR_VDEC_MVC) ||
                       (ePlaymgrVdec == PLAYMGR_VDEC_MULTITRACK))
                    {
                        VDEC_Stop(_arPlayerInfo[u1SrcId].u1Vdec2Id);
                    }
                    _PLAYMGR_VdecWaitCtrlDone(u1SrcId);
                }
            }
        }
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm = fgEnable;
    }
    if (u4InfoValue == PLAYMGR_INFO_MASK_AUDIO)
    {
        if (fgEnable)
        {
            if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                #ifdef eSWDMX_GET_CHG_AUDIO_CAP
                UINT32 u4Capable2ChgAud = 0;
                
                // True is for playmgr can get the eSWDMX_GET_CHG_AUDIO_CAP successfully
                // False is for nothing
                if (SWDMX_GetInfo(u1SrcId, eSWDMX_GET_CHG_AUDIO_CAP, &u4Capable2ChgAud, 0, 0))
                {
                    if (u4Capable2ChgAud == 0)
                    {
                        return FALSE;
                    }
                }
                #endif
            }
            
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum >= (UINT16)PLAYMGR_MAX_VID_TRACK)
            {
                LOG(1,"_PLAYMGR_CtlStm u2EnVTrackNum >= PLAYMGR_MAX_VID_TRACK\n");
                _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum = (UINT16)(PLAYMGR_MAX_VID_TRACK-1);
            }
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum >= (UINT16)PLAYMGR_MAX_AUD_TRACK)
            {
                LOG(1,"_PLAYMGR_CtlStm u2EnATrackNum >= PLAYMGR_MAX_AUD_TRACK\n");
                _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum = (UINT16)(PLAYMGR_MAX_AUD_TRACK-1);
            }
            
            // handle audio track change
            if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                LOG(5, "aeAudFormat %d\n", _arPlayerInfo[u1SrcId].rPlayerStatus.aeAudFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum]);

                switch (_arPlayerInfo[u1SrcId].rPlayerStatus.aeAudFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum])
                {
                case PLAYMGR_ADEC_MP3:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#else
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MP3);
#endif
                    break;
                case PLAYMGR_ADEC_AC3:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#else
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AC3);
#endif
                    break;
                case PLAYMGR_ADEC_AAC:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#else
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_AAC);
#endif
                    break;
                case PLAYMGR_ADEC_MPEG:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#else
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_MPEG);
#endif
                    break;
                case PLAYMGR_ADEC_PCM:
			    case PLAYMGR_ADEC_ADPCM:					
                    if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                            (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
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
                    else if((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                            (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                             == MEDIA_CONTNR_SUBTYPE_FLV))
                    {
                        UINT32 u4SampleRate=0;
						UINT8  uPcmType=0;						
                        MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;
                        if(_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.u2AudIdx<2)
                        {
                            prAudStrmAttr=&_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.rStmAttribute[_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.u2AudIdx];
                        }
                        else
                        {
                            LOG(5, "Flv Error,AudioIndex=%d\n",_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.u2AudIdx);
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
					
#ifdef CC_53XX_AUDIO_V2
					AUD_PcmSetting(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, &rPCMInfo);
					AUD_PcmSetting(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, &rPCMInfo);
					AUD_LPcmSetting(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, &rLPCMInfo);
					AUD_LPcmSetting(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, &rLPCMInfo);
					AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
					AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_PCM);
#else
					AUD_PcmSetting(_arPlayerInfo[u1SrcId].u1AdecId, &rPCMInfo);
					AUD_LPcmSetting(_arPlayerInfo[u1SrcId].u1AdecId, &rLPCMInfo);
					AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_PCM);
#endif

                    break;
                case PLAYMGR_ADEC_WMA:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#else
                    AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_WMA);
#endif
                        break;
#if 1//def CC_MT5363
                    case PLAYMGR_ADEC_RA:
#ifdef CC_53XX_AUDIO_V2
                        AUD_SetDecType(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_INTERNAL_BUF,  AUD_FMT_PCM);
                        AUD_SetDecType(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#else
                        AUD_SetDecType(_arPlayerInfo[u1SrcId].u1AdecId, AUD_STREAM_FROM_MULTI_MEDIA,  AUD_FMT_COOK);
#endif
                    break;
#endif
                default:
                    return FALSE;
                }
				_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm = TRUE; //  DTV00083755 In _PLAYMGR_AdecNty() will check fgEnAudStrm, so we should set this flag
				                                                         // otherwise _PLAYMGR_AdecWaitCtrlDone() will timeout
                //_PLAYMGR_AdecFlushCtrl();
                //AUD_DSPCmdPlay(_arPlayerInfo[u1SrcId].u1AdecId);
                //_PLAYMGR_AdecWaitCtrlDone();
#ifdef CC_53XX_AUDIO_V2
                _PLAYMGR_AdecFlushCtrl(u1SrcId);
                AUD_DSPCmdPlay(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
                AUD_DspSpeed(_arPlayerInfo[u1SrcId].u1AdecId, 0);
                AUD_DSPCmdPlay(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);              
                _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
                //AUD_DSPCmdPlayAsyn(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
                //AUD_DspSpeed(_arPlayerInfo[u1SrcId].u1AdecId, 0);
                //AUD_DSPCmdPlayAsyn(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
                _PLAYMGR_AdecFlushCtrl(u1SrcId);
                AUD_DSPCmdPlay(_arPlayerInfo[u1SrcId].u1AdecId);                
                _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
                //AUD_DSPCmdPlayAsyn(_arPlayerInfo[u1SrcId].u1AdecId);
                AUD_DspSpeed(_arPlayerInfo[u1SrcId].u1AdecId, 0);
#endif
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = TRUE;
            }

            if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                    (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                     == MEDIA_CONTNR_SUBTYPE_MP4))
            {
                UINT32 u4StrmId = ((UINT32)_arPlayerInfo[u1SrcId].rPlayerStatus.u2VideoTracks + (UINT32)_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum);
                if (u4StrmId < 3)
                {
                    MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;

                    prAudStrmAttr =
                        &_arPlayerInfo[u1SrcId].FmtInfo.rMp4Info.rStmAttribute[u4StrmId];

                    fgRet = SWDMX_SetInfo(u1SrcId, 
                                          eSWDMX_SET_AUDIOTYPE,
                                          _PLAYMGR_PlaymgrAdec2Swdmx(u1SrcId, 
                                              _arPlayerInfo[u1SrcId].rPlayerStatus.aeAudFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum]),
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
            else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                     (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                     == MEDIA_CONTNR_SUBTYPE_RM))
            {
                MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;
                ENUM_SWDMX_ADEC_T eAud;
                UINT32 u4StrmId = ((UINT32)_arPlayerInfo[u1SrcId].rPlayerStatus.u2VideoTracks + (UINT32)_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum);

                if (u4StrmId < 3)
                {
                    prAudStrmAttr =
                        &_arPlayerInfo[u1SrcId].FmtInfo.rRmInfo.rStmAttribute[u4StrmId];
                }
                else
                {
                    LOG(1, "Out of total stream count\n");
                    fgRet = FALSE;
                    return FALSE;
                }

                eAud = _PLAYMGR_PlaymgrAdec2Swdmx(u1SrcId, _arPlayerInfo[u1SrcId].rPlayerStatus.aeAudFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum]);
                if (eAud == ENUM_SWDMX_ADEC_AAC)
                {
                    fgRet = SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE, ENUM_SWDMX_ADEC_AAC,
                        prAudStrmAttr->u.t_rm_audio_stm_attr.ui4_sample_rate,
                        prAudStrmAttr->u.t_rm_audio_stm_attr.ui2_num_channel);
                }
                else if (eAud == ENUM_SWDMX_ADEC_RA)
                {
                    fgRet = SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE, ENUM_SWDMX_ADEC_RA,
                        (UINT32)&_rCookInfo, 0);
                }
                else
                {
                    ASSERT(0);
                    fgRet = FALSE;
                }
            }
            else if ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER) &&
                     (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_contnr_subtype
                      == MEDIA_CONTNR_SUBTYPE_FLV))
             {
                 UINT32 u4StrmId = ((UINT32)_arPlayerInfo[u1SrcId].rPlayerStatus.u2VideoTracks + (UINT32)_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum);

                 if (u4StrmId < 2)
                 {
                     MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;
            
                     prAudStrmAttr =&_arPlayerInfo[u1SrcId].FmtInfo.rFlvInfo.rStmAttribute[u4StrmId];
            
                    fgRet = SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,
                                          _PLAYMGR_PlaymgrAdec2Swdmx(u1SrcId, 
                                              _arPlayerInfo[u1SrcId].rPlayerStatus.aeAudFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum]),
                                          prAudStrmAttr->u.t_flv_audio_stm_attr.i4_samples_per_sec,
                                          prAudStrmAttr->u.t_flv_audio_stm_attr.i2_channels);
                 }
            }
            else
            {
                fgRet = SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUDIOTYPE,
                                      _PLAYMGR_PlaymgrAdec2Swdmx(u1SrcId, 
                                          _arPlayerInfo[u1SrcId].rPlayerStatus.aeAudFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnATrackNum]),
                                      0, 0);
            }

            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set audio format info\n");
                return FALSE;
            }

            fgRet = SWDMX_SetInfo(u1SrcId, 
                (UINT8)eSWDMX_SET_ADEC_ID,
                (UINT32)_arPlayerInfo[u1SrcId].u1AdecId, 
                0, 
                0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set adec id\n");
                return FALSE;
            }
            
            fgRet = SWDMX_SetStrmID(u1SrcId, _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayAudStrmId,
                                    _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayAudSubStrmId,
                                    eSWDMX_STRM_TYPE_AUD);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set audio stream ID\n");
                return FALSE;
            }

#ifdef TIME_SHIFT_SUPPORT
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT)
            {
                if(_arPlayerInfo[u1SrcId].rPlayerStatus.fgPlayAudDecrypt)
                {
                    fgRet = SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUD_PID_IDX,
                                            _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayAudIdx,
                                            _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayAudKeyIdx,
#ifdef CC_MT5387
                                            (UINT32)DMX_DESC_MODE_AES_ECB);
#else
                                            (UINT32)DMX_DESC_MODE_3DES_ECB);
#endif
                }
                else
                {
                    fgRet = SWDMX_SetInfo(u1SrcId, eSWDMX_SET_AUD_PID_IDX,
                                            _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayAudIdx,
                                            0, 0);
                }
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_CtlStm Fail to set audio pid idx\n");
                    return FALSE;
                }
            }
#endif
            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_AUD, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to enable audio stream\n");
                return FALSE;
            }
        }
        else
        {
            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_AUD, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_Set Fail to disable audio stream\n");
                return FALSE;
            }
            _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisAudStrm = FALSE;
            // handle audio track change
            if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
                {
                    _PLAYMGR_AdecFlushCtrl(u1SrcId);
#ifdef CC_53XX_AUDIO_V2
                    AUD_DSPCmdStop(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
                    AUD_DSPCmdStop(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
                    AUD_DSPCmdStop(_arPlayerInfo[u1SrcId].u1AdecId);
#endif
                    _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
                    _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = FALSE;
                }
            }
        }
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm = fgEnable;
    }
    if (u4InfoValue == PLAYMGR_INFO_MASK_PCR)
    {
        if (fgEnable)
        {
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnPcrTrackNum >= (UINT16)PLAYMGR_MAX_PCR_TRACK)
            {
                LOG(1,"_PLAYMGR_CtlStm u2EnPcrTrackNum >= PLAYMGR_MAX_PCR_TRACK\n");
                _arPlayerInfo[u1SrcId].rPlayerStatus.u2EnPcrTrackNum = (UINT16)(PLAYMGR_MAX_PCR_TRACK-1);
            }

#ifdef TIME_SHIFT_SUPPORT
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT)
            {
                fgRet = SWDMX_SetInfo(u1SrcId, eSWDMX_SET_PCR_PID_IDX,
                                        _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayPcrIdx,
                                        0, 0);
                if (!fgRet)
                {
                    LOG(1, "_PLAYMGR_CtlStm Fail to set pcr pid idx\n");
                    return FALSE;
                }
            }
#endif

            fgRet = SWDMX_SetStrmID(u1SrcId, _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayPcrStrmId,
                                    0, eSWDMX_STRM_TYPE_PCR);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set pcr stream ID\n");
                return FALSE;
            }

            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to enable pcr stream\n");
                return FALSE;
            }
        }
        else
        {
            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_Set Fail to disable pcr stream\n");
                return FALSE;
            }
        }
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm = fgEnable;
    }
    if (u4InfoValue == PLAYMGR_INFO_MASK_CAPTRACK)
    {
        if (fgEnable)
        {
            if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                #ifdef eSWDMX_GET_CHG_SUBTITLE_CAP
                UINT32 u4Capable2ChgSub = 0;
                
                // True is for playmgr can get the eSWDMX_GET_CHG_AUDIO_CAP successfully
                // False is for nothing
                if (SWDMX_GetInfo(u1SrcId, eSWDMX_GET_CHG_SUBTITLE_CAP, &u4Capable2ChgSub, 0, 0))
                {
                    if (u4Capable2ChgSub == 0)
                    {
                        return FALSE;
                    }
                }
                #endif
            }
            
            fgRet = SWDMX_SetStrmID(u1SrcId, _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlaySubId,
                                    0, eSWDMX_STRM_TYPE_SUBTITLE);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set subpic stream ID\n");
                return FALSE;
            }

            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_ENABLE_STREAM,
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
            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_SUBTITLE, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to disable subpic stream\n");
                return FALSE;
            }
        }
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnSubStrm = fgEnable;
    }
    if (u4InfoValue == PLAYMGR_INFO_MASK_PCR)
    {
        if (fgEnable)
        {
            fgRet = SWDMX_SetStrmID(u1SrcId, _arPlayerInfo[u1SrcId].rPlayerStatus.u4PlayPcrStrmId,
                                    0, eSWDMX_STRM_TYPE_PCR);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to set pcr stream ID\n");
                return FALSE;
            }

            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_ENABLE_STREAM,
                                  (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to enable pcr stream\n");
                return FALSE;
            }
        }
        else
        {
            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0);
            if (!fgRet)
            {
                LOG(1, "_PLAYMGR_CtlStm Fail to disable pcr stream\n");
                return FALSE;
            }
            _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisPcrStrm = FALSE;
            // handle pcr track change
            if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
            {
                if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm)
                {
                    _arPlayerInfo[u1SrcId].rPlayerStatus.fgPcrPlayed = FALSE;
                }
            }
        }
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm = fgEnable;
    }
    return TRUE;
}


BOOL _PLAYMGR_Step(UINT8 u1SrcId, INT32 i4StepAmount)
{
    PLAYMGR_STATUS_T* prMediaStatus = &_arPlayerInfo[u1SrcId].rPlayerStatus;
    ENUM_VDEC_FMT_T eFmt;
    BOOL fgRet;

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    
    LOG(5, "_PLAYMGR_Step %d\n", i4StepAmount);
    
    if((_afgIsSeeking[u1SrcId]) &&
       (_arPlayerInfo[u1SrcId].ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }    

    if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus != PLAYMGR_STATE_PAUSE)
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

    if ((_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm) &&
            (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm))
    {
        if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_AUDIO, FALSE))
        {
            LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
        }
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm = FALSE;
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisAudStrm = TRUE;
        _PLAYMGR_AdecFlushCtrl(u1SrcId);
#ifdef CC_53XX_AUDIO_V2
        AUD_DSPCmdStop(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
        AUD_DSPCmdStop(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
        AUD_DSPCmdStop(_arPlayerInfo[u1SrcId].u1AdecId);
#endif
        _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = FALSE;
    }
    if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm)
    {
        if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_PCR, FALSE))
        {
            LOG(1,"_PLAYMGR_CtlStm disable pcr fail\n");
        }
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm = FALSE;
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisPcrStrm = TRUE;
        _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = FALSE;
    }

    if(((_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed <= 0) ||
        (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed >= 2000))
       ||
       (i4StepAmount < 0))
    {
        _PLAYMGR_Flush(u1SrcId);
    }
    _arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed = 1;
    if (i4StepAmount < 0)
    {
        // ToDo: consider amount
        _arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed = -1;
    }

	SWDMX_SetPlayMode(u1SrcId,SWDMX_PLAY_MODE_NORMAL);
    fgRet = SWDMX_SetSpeed(u1SrcId, _arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed);
    if (!fgRet)
    {
        LOG(1,"_PLAYMGR_Step SWDMX_SetSpeed fail\n");
    }
    fgRet = SWDMX_Play(u1SrcId);
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
    case PLAYMGR_VDEC_AVS:
        eFmt = VDEC_FMT_AVS;
        break;
    case PLAYMGR_VDEC_WMV7:
    case PLAYMGR_VDEC_WMV8:
    case PLAYMGR_VDEC_WMV9:
    case PLAYMGR_VDEC_VC1:
        eFmt = VDEC_FMT_WMV;
        break;
    case PLAYMGR_VDEC_VP6:
        eFmt = VDEC_FMT_VP6;
        break;
    case PLAYMGR_VDEC_VP8:
        eFmt = VDEC_FMT_VP8;
        break;
    case PLAYMGR_VDEC_MJPEG:
        eFmt = VDEC_FMT_MJPEG;
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
    VDEC_PlayMM(_arPlayerInfo[u1SrcId].u1VdecId , TRUE);
    VDEC_Play(_arPlayerInfo[u1SrcId].u1VdecId, eFmt);
    VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1X, 0, 0);
    if((prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
       (prMediaStatus->aeSubVidFormat[prMediaStatus->u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK))
    {
        VDEC_PlayMM(_arPlayerInfo[u1SrcId].u1Vdec2Id , TRUE);
        VDEC_Play(_arPlayerInfo[u1SrcId].u1Vdec2Id, eFmt);
        VDEC_SetMMParam(_arPlayerInfo[u1SrcId].u1Vdec2Id, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1X, 0, 0);
    }
    
    VDP_SetSpeed(_arPlayerInfo[u1SrcId].u1B2rId, STC_SPEED_TYPE_FORWARD_1X);
    
#if 1
    _PLAYMGR_VdpFlushCtrl(u1SrcId);
#endif

    /* Pause VDP and just display 1 frame */
    VDP_SetFBNoTimeout(_arPlayerInfo[u1SrcId].u1B2rId, TRUE);
    VDP_SetPauseMm(_arPlayerInfo[u1SrcId].u1B2rId, TRUE);
    VDP_SetStepForward(_arPlayerInfo[u1SrcId].u1B2rId, VDP_PLAY_STEP_FORWARD);

    _PLAYMGR_SetSyncMode(u1SrcId);
    VDP_RegCbFunc(VDP_CB_FUNC_MM_STEP_FIN_IND, (UINT32)_PLAYMGR_VdpStepDoneNfy, 0, 0);

    /* VDP will play one frame and then Pause if VDP_PLAY_STEP_FORWARD is set */
#if 0
    _PLAYMGR_VdpFlushCtrl(u1SrcId);
#endif
    VDP_SetPauseMm(_arPlayerInfo[u1SrcId].u1B2rId, FALSE);

    // ASYNC FOR XUNLEI
    if (_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_XUNLEI)
    {
        return TRUE;
    }
    _PLAYMGR_VdpWaitCtrlDone(u1SrcId);

    // step done.
    FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_SEEK_END, (_arPlayerInfo[u1SrcId].u4SeekPts/90));

    // ToDo: we don't need pause?
    VDEC_Pause(_arPlayerInfo[u1SrcId].u1VdecId);
    if ((_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum < (UINT16)PLAYMGR_MAX_VID_TRACK) &&
        ((_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
         (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK)))
    {
        VDEC_Pause(_arPlayerInfo[u1SrcId].u1Vdec2Id);
    }

    // we don't need pause
    //fgRet = SWDMX_Pause();
    //if(!fgRet)
    //{
    //    LOG(1,"_PLAYMGR_Step SWDMX_Pause fail\n");
    //    return FALSE;
    //}

    return TRUE;
}


BOOL _PLAYMGR_SetSyncMode(UINT8 u1SrcId)
{
    UINT32 u4SyncMode=AV_SYNC_MODE_NONE;
    if((_afgIsSeeking[u1SrcId]) &&
       (_arPlayerInfo[u1SrcId].ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }
    
    // DTV AUD_MASTER, AV_SYNC_MODE_SYSTEM_MASTER
#ifdef TIME_SHIFT_SUPPORT
    if ((_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT) &&
            (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed == 1000))
    {
        STC_SetSyncMode(_arPlayerInfo[u1SrcId].u1StcId,
            _arPlayerInfo[u1SrcId].u1AdecId,
            _arPlayerInfo[u1SrcId].u1VdecId, AV_SYNC_MODE_SYSTEM_MASTER);
        return TRUE;
    }
#endif

    if (!_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
    {
        if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
        {
            // not have container, not have PTS case.
            //STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER);
			u4SyncMode=AV_SYNC_MODE_AUDIO_MASTER;
        }
        else
        {
            // have container, have PTS case.
            // audio driver could udpate stc value
			u4SyncMode=AV_SYNC_MODE_AUDIO_MASTER;
        }
    }
    else // have video
    {
        if (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed == 1000)
        {
            if (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES)
            {
				u4SyncMode=AV_SYNC_MODE_SYSTEM_MASTER;
            }
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
            {
				u4SyncMode=AV_SYNC_MODE_AUDIO_MASTER;
            }
            else
            {
				u4SyncMode=AV_SYNC_MODE_SYSTEM_MASTER;
            }
        }
        else if (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed == 2000)
        {
#ifdef TIME_SHIFT_SUPPORT
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.eMediaType == PLAYMGR_MEDIA_TYPE_MPEG2_TS_TIME_SHIFT)
            {
				u4SyncMode=AV_SYNC_MODE_NONE;
            }
            else
#endif
#endif
            {
				u4SyncMode=AV_SYNC_MODE_VIDEO_MASTER;
            }
        }
        else if ((_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed < 1000) &&
                 (_arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed > 10))
        {
            /* Slow motion */
			u4SyncMode=AV_SYNC_MODE_VIDEO_MASTER;
        }
        else
        {

			u4SyncMode=AV_SYNC_MODE_NONE;
        }
    }



	if ((_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum < (UINT16)PLAYMGR_MAX_VID_TRACK) &&
		((_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MVC) ||
		 (_arPlayerInfo[u1SrcId].rPlayerStatus.aeSubVidFormat[_arPlayerInfo[u1SrcId].rPlayerStatus.u2EnVTrackNum] == PLAYMGR_VDEC_MULTITRACK)))
	{
		STC_SetSyncMode(_arPlayerInfo[u1SrcId].u1StcId,
			_arPlayerInfo[u1SrcId].u1AdecId,
			_arPlayerInfo[u1SrcId].u1Vdec2Id, u4SyncMode);
	}

    STC_SetSyncMode(_arPlayerInfo[u1SrcId].u1StcId,
        _arPlayerInfo[u1SrcId].u1AdecId,
        _arPlayerInfo[u1SrcId].u1VdecId, u4SyncMode);
    return TRUE;
}

#ifdef __KERNEL__
/**Nofify playmgr to get data
*@param u4Size                Received data size
*@retval TRUE                 Success.
*@retval FALSE                Fail.
*/
BOOL _PLAYMGR_ReceiveData(UINT8 u1SrcId, UINT32 u4Size,  UINT64 u8Pos)
{
    PLAYMGR_RECEIVE_DATA_T rReceive;
    SIZE_T zMsgSize = sizeof(PLAYMGR_RECEIVE_DATA_T);
    INT32 i4Ret;

    rReceive.u4ReadSize = u4Size;
    rReceive.u8FilePos = u8Pos;

    i4Ret = x_msg_q_send(_arPlayerInfo[u1SrcId].hPlayMgrDataQ, &rReceive, zMsgSize, 255);
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
BOOL _PLAYMGR_SetCMD(UINT8 u1SrcId, PLAYMGR_STATUS_CMD_T rStatus)
{
#ifdef PLAYMGR_ASYNC_MODE
    BOOL i4Ret = FALSE;
    SIZE_T zMsgSize = sizeof(PLAYMGR_STATUS_CMD_T);

    i4Ret = x_msg_q_send(_arPlayerInfo[u1SrcId].hPlayMgrStatusQ, (VOID *)&rStatus, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        i4Ret = x_msg_q_send(_arPlayerInfo[u1SrcId].hPlayMgrStatusQ, (VOID *)&rStatus, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);
#else
    BOOL fgRet = FALSE;
    UINT64 u8Tmp;
    
    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);
    
    if(((_afgIsSeeking[u1SrcId]) &&
       (_arPlayerInfo[u1SrcId].ePlayMgrSrcType != PLAYMGR_SRC_TYPE_TIME_SHIFT)) &&
       (rStatus.eCmd != PLAYMGR_SETCMD_STOP))
    {
        LOG(0, "!!Seek not finished\n");
        return FALSE;
    }
    
    switch (rStatus.eCmd)
    {
    case PLAYMGR_SETCMD_INIT:
        _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_CLOSE;
        fgRet = TRUE;
        break;
    case PLAYMGR_SETCMD_OPEN:
        if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_CLOSE)
        {
            _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_OPEN;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_CLOSE:
        if ( (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_STOP) || (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_CLOSE) ||
            (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_OPEN))
        {
            _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_CLOSE;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_START:
        if ((_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_STOP) ||
                (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE))
        {
            _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_PLAY;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_STOP:
        if ( (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY) || (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
                || (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_OPEN))
        {
            _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_STOP;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_PAUSE:
        if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_PAUSE;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_RESUME:
        if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
        {
            _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_PLAY;
            fgRet = TRUE;
        }
        break;
    case PLAYMGR_SETCMD_ABREPEAT:
        fgRet = TRUE;
        break;
    case PLAYMGR_SETCMD_TRICK:
        if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            fgRet = TRUE;
        }
        else if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
        {
            fgRet = _PLAYMGR_Resume(u1SrcId);
            if (fgRet)
            {
                _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_PLAY;
            }
        }
        break;
    case PLAYMGR_SETCMD_ABORT:
        // ToDo: what should be done here
        /*
        if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_STOP;
            fgRet = TRUE;
        }
        else if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
        {
            _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_STOP;
            fgRet = TRUE;
        }
        */
        // call x_midxbuld_stop_indexing
        FeederNotifyHandle(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_NOTIFY_ABORT_DONE, 0);
        break;
    case PLAYMGR_SETCMD_SEEKTIME:
    case PLAYMGR_SETCMD_SEEKPOSITION:
    case PLAYMGR_SETCMD_SEEKPERCENT:
        if(!_PLAYMGR_GetCap(u1SrcId, PLAYMGR_CAP_SEEK, 0, 0))
        {
            fgRet = FALSE;
            LOG(5,"_PLAYMGR_Seek fgTrick(0)\n");
        }        
        else if ((_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE) ||
                 (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_STOP))
        {
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
            {
                if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_AUDIO, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
                }
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm = FALSE;
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisAudStrm = TRUE;
                _PLAYMGR_AdecFlushCtrl(u1SrcId);
#ifdef CC_53XX_AUDIO_V2
                AUD_DSPCmdStop(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
                AUD_DSPCmdStop(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
                AUD_DSPCmdStop(_arPlayerInfo[u1SrcId].u1AdecId);
#endif
                _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = FALSE;
            }
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm)
            {
                if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_PCR, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable pcr fail\n");
                }
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm = FALSE;
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisPcrStrm = TRUE;
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = FALSE;
            }
            if (rStatus.eCmd == PLAYMGR_SETCMD_SEEKTIME)
            {
                fgRet = _PLAYMGR_SeekTime(u1SrcId, rStatus.u4Param1);
            }
            else if (rStatus.eCmd == PLAYMGR_SETCMD_SEEKPOSITION)
            {
                u8Tmp = rStatus.u4Param1;
                u8Tmp <<= 32;
                u8Tmp += rStatus.u4Param2;
                fgRet = _PLAYMGR_SeekPosition(u1SrcId, u8Tmp);
            }
            else
            {
                fgRet = _PLAYMGR_SeekPercent(u1SrcId, rStatus.u4Param1);
            }
            if (!fgRet)
            {
                LOG(3, "!PLAYMGR_STATE_PLAY _PLAYMGR_Seekxxx fail\n");
            }
        }
        else if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            LOG(5, "_PLAYMGR_SetCMD _PLAYMGR_Pause\n");
            fgRet = _PLAYMGR_Pause(u1SrcId);
            if (!fgRet)
            {
                LOG(3, "PLAYMGR_STATE_PLAY _PLAYMGR_Pause fail\n");
            }
            _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_PAUSE;
            if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm)
            {
                if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_AUDIO, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable audio fail\n");
                }
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnAudStrm = FALSE;
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisAudStrm = TRUE;
                _PLAYMGR_AdecFlushCtrl(u1SrcId);
#ifdef CC_53XX_AUDIO_V2
                AUD_DSPCmdStop(AUD_A_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
                AUD_DSPCmdStop(AUD_D_DSP, _arPlayerInfo[u1SrcId].u1AdecId);
#else
                AUD_DSPCmdStop(_arPlayerInfo[u1SrcId].u1AdecId);
#endif
                _PLAYMGR_AdecWaitCtrlDone(u1SrcId);
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgAudPlayed = FALSE;
            }

            if (_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm)
            {
                if (!_PLAYMGR_CtlStm(u1SrcId, PLAYMGR_INFO_MASK_PCR, FALSE))
                {
                    LOG(1,"_PLAYMGR_CtlStm disable pcr fail\n");
                }
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgEnPcrStrm = FALSE;
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgDisPcrStrm = TRUE;
                _arPlayerInfo[u1SrcId].rPlayerStatus.fgPcrPlayed = FALSE;
            }

            if (rStatus.eCmd == PLAYMGR_SETCMD_SEEKTIME)
            {
                LOG(5, "_PLAYMGR_SetCMD _PLAYMGR_SeekTime\n");
                fgRet = _PLAYMGR_SeekTime(u1SrcId, rStatus.u4Param1);
            }
            else if (rStatus.eCmd == PLAYMGR_SETCMD_SEEKPOSITION)
            {
                LOG(5, "_PLAYMGR_SetCMD _PLAYMGR_SeekPosition\n");
                u8Tmp = rStatus.u4Param1;
                u8Tmp <<= 32;
                u8Tmp += rStatus.u4Param2;
                fgRet = _PLAYMGR_SeekPosition(u1SrcId, u8Tmp);
            }
            else
            {
                LOG(5, "_PLAYMGR_SetCMD _PLAYMGR_SeekPercent\n");
                fgRet = _PLAYMGR_SeekPercent(u1SrcId, rStatus.u4Param1);
            }
            if (!fgRet)
            {
                LOG(3, "PLAYMGR_STATE_PLAY _PLAYMGR_Seekxxx fail\n");
            }

            LOG(5, "_PLAYMGR_SetCMD _PLAYMGR_Start\n");
            fgRet = _PLAYMGR_Start(u1SrcId);
            if (!fgRet)
            {
                LOG(3, "PLAYMGR_SETCMD_SEEKTIME _PLAYMGR_Start fail\n");
            }
            _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_PLAY;
            if (!_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
            {
                fgRet = FALSE;
            }
            if (fgRet)
            {
                _afgIsSeeking[u1SrcId] = TRUE;
            }
        }
        break;
    case PLAYMGR_SETCMD_STEP:
        if (!_arPlayerInfo[u1SrcId].rPlayerStatus.fgEnVidStrm)
        {
            LOG(3, "PLAYMGR_SETCMD_STEP no video stream\n");
            return TRUE;
        }
        if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_STOP)
        {
            LOG(3, "PLAYMGR_SETCMD_STEP PLAYMGR_STATE_STOP\n");
            return TRUE;
        }
        else if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PAUSE)
        {
            fgRet = _PLAYMGR_Step(u1SrcId, (INT32)rStatus.u4Param1);
            if (!fgRet)
            {
                LOG(3, "!PLAYMGR_STATE_PLAY _PLAYMGR_Step fail\n");
            }
        }
        else if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_PLAY)
        {
            LOG(5, "PLAYMGR_STATE_PLAY _PLAYMGR_Pause\n");
            fgRet = _PLAYMGR_Pause(u1SrcId);
            if (!fgRet)
            {
                LOG(3, "PLAYMGR_STATE_PLAY _PLAYMGR_Pause fail\n");
            }
            fgRet = _PLAYMGR_Step(u1SrcId, (INT32)rStatus.u4Param1);
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
                        &(_arPlayerInfo[u1SrcId].hPlayMgrStatusQ), 1, X_MSGQ_OPTION_WAIT);

        switch (rStatus.eCmd)
        {
        case PLAYMGR_SETCMD_OPEN:
            if (_arPlayerInfo[u1SrcId].ePlayMgrCurStatus == PLAYMGR_STATE_CLOSE)
            {
                if (_PLAYMGR_GetInfo(&rStatus.u4Param1))
                {
                    _arPlayerInfo[u1SrcId].ePlayMgrCurStatus = PLAYMGR_STATE_GETINFO;
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


BOOL _PLAYMGR_GetDivxDRMStatus(UINT8 u1SrcId, PLAYMGR_DRMTYPE *peDRMType, UINT32 *pu4DRMRentalLimit, UINT32 *pu4DRMRentalCount)
{
#ifndef CC_AVI_DISABLE
    return _PLAYMGR_AviGetDivxDRMStatus(u1SrcId, peDRMType, pu4DRMRentalLimit, pu4DRMRentalCount);
#else
    return TRUE;
#endif
}


BOOL _PLAYMGR_GetDivxDRMRegCode(UINT8 u1SrcId, char *pu1Code)
{
#ifndef CC_AVI_DISABLE
    return _PLAYMGR_AviGetDivxDRMRegCode(pu1Code);
#else
    return TRUE;
#endif
}


BOOL _PLAYMGR_GetDivxDRMUIHelpInfo(UINT8 u1SrcId, UINT32 *pu4Info)
{
#ifndef CC_AVI_DISABLE
    return _PLAYMGR_AviGetDivxDRMUIHelpInfo(pu4Info);
#else
    return TRUE;
#endif
}


BOOL _PLAYMGR_DivxDRMDeactivation(UINT8 u1SrcId, char *pu1Code)
{
#ifndef CC_AVI_DISABLE
    return _PLAYMGR_AviDivxDRMDeactivation(pu1Code);
#else
    return TRUE;
#endif
}


VOID _PLAYMGR_SetSrcType(UINT8 u1SrcId, PLAYMGR_SourceType_T eSrcType)
{
    PLAYMGR_VERIFY_ID(u1SrcId);
    
    if (!_fgPlaymgrInit)
    {
        _PLAYMGR_Init();
    }

    _arPlayerInfo[u1SrcId].ePlayMgrSrcType = eSrcType;
}

PLAYMGR_SourceType_T _PLAYMGR_GetSrcType(UINT8 u1SrcId)
{
    if (u1SrcId >= PLAYMGR_MAX_INPUT_SRC)
    {
        return PLAYMGR_SRC_TYPE_UNKNOWN;
    }

    return _arPlayerInfo[u1SrcId].ePlayMgrSrcType;
}

VOID _PLAYMGR_SetNumberOfBitstream(UINT8 u1SrcId, UINT32 u4Size)
{
    PLAYMGR_VERIFY_ID(u1SrcId);

    _arPlayerInfo[u1SrcId].u4NumberOfBitstream = u4Size;
}

UINT32 _PLAYMGR_GetNumberOfBitstream(UINT8 u1SrcId)
{
    if (u1SrcId >= PLAYMGR_MAX_INPUT_SRC)
    {
        return 0;
    }

    return _arPlayerInfo[u1SrcId].u4NumberOfBitstream;
}

BOOL _PLAYMGR_SetFeederSrcType(UINT8 u1SrcId)
{
    //FeederSourceType eFeederSrc;
    //FEEDER_QUERY_DATA rSetQuery;

    PLAYMGR_VERIFY_ID_ERROR(u1SrcId);

    /* move to init, keep original behavior with v1 version.
    if( u1SrcId == PLAYMGR0)
    {
        _arPlayerInfo[u1SrcId].eFeederSrc = FEEDER_PROGRAM_SOURCE;
    }
    else if (u1SrcId == PLAYMGR1)
    {
        _arPlayerInfo[u1SrcId].eFeederSrc = FEEDER_AUDIO_SOURCE;
    }
        
    FeederOpen(_arPlayerInfo[u1SrcId].eFeederSrc);
    FeederSetAppQueryMode(_arPlayerInfo[u1SrcId].eFeederSrc, FEEDER_PULL_MODE);
    rSetQuery.u1PlaymgrAttachedId = u1SrcId;
    rSetQuery.eDataType = _arPlayerInfo[u1SrcId].eFeederSrc;
    rSetQuery.pfnSource = _PLAYMGR_DataQuery;
    rSetQuery.pfnSeekPos = _PLAYMGR_SeekPos;
    rSetQuery.pfnConsume = NULL;
    rSetQuery.u4QuerySize = 0;
    FeederRegisterPullDataFunc(_arPlayerInfo[u1SrcId].eFeederSrc, &rSetQuery);
    FeederStart(_arPlayerInfo[u1SrcId].eFeederSrc);
    FeederSetRing(_arPlayerInfo[u1SrcId].eFeederSrc, TRUE);
    FeederSetOffset(_arPlayerInfo[u1SrcId].eFeederSrc, (UINT64)0, PLAYMGR_SEEK_BGN, 0);
    */

    SWDMX_SetInfo(u1SrcId, eSWDMX_SET_SRC_TYPE, _arPlayerInfo[u1SrcId].eFeederSrc, 0, 0);

    return TRUE;
}

PLAYMGR_INFO_T* _PLAYMGR_GetInst(UINT8 u1SrcId)
{
    if (u1SrcId >= PLAYMGR_MAX_INPUT_SRC)
    {
        LOG(1, "_PLAYMGR_GetInst, Error u1SrcId %d\n", u1SrcId);
        u1SrcId = 0;
    }
    
    return &_arPlayerInfo[u1SrcId];
}


BOOL _PLAYMGR_GetCap(UINT8 u1SrcId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2)
{
    BOOL fgRet = FALSE;
    UINT32 u4Trick = TRUE, u4Dummy1, u4Dummy2;
    UINT32 u4Forward = 0, u4Speed;
    
    switch(u4Type)
    {
        case PLAYMGR_CAP_SEEK:
            if(_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
            {
                SWDMX_GetInfo(u1SrcId, eSWDMX_GET_IS_TRICK, &u4Trick, &u4Dummy1, &u4Dummy2);
            }
            fgRet = (BOOL)u4Trick;
            break;
        case PLAYMGR_CAP_TRICKMODE:
            if(_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_CONTAINER)
            {
                SWDMX_GetInfo(u1SrcId, eSWDMX_GET_IS_TRICK, &u4Trick, &u4Dummy1, &u4Dummy2);
            }
            else if((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_PS) ||
                    (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_TS) ||
                    (_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_VIDEO_ES) ||
                    ((_arPlayerInfo[u1SrcId].rMediaFmt.e_media_type == MEDIA_TYPE_AUDIO_ES) &&
                     (_arPlayerInfo[u1SrcId].rMediaFmt.t_media_subtype.u.e_aud_subtype != MEDIA_AUD_SUBTYPE_WMA)))                
            {
                u4Speed = (u4Type == PLAYMGR_CAP_TRICKMODE) ? u4Param1 : _arPlayerInfo[u1SrcId].rPlayerStatus.i4Speed;
                if(u4Speed == 1000)
                {
                    fgRet = TRUE;
                    break;
                }
                if(u4Speed > 1000)    //speed
                {
                    u4Forward = 1;
                }
                SWDMX_GetInfo(u1SrcId, eSWDMX_GET_IS_TRICK, &u4Trick, &u4Forward, &u4Dummy2);
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

BOOL _PLAYMGR_SetLPCMInfo(UINT8 u1SrcId, PLAYMGR_LPCM_INFO_T rPlayLPCMInfo)
{
    MINFO_TYPE_STM_ATRBT_T *prAudStrmAttr = NULL;

    if (_arPlayerInfo[u1SrcId].ePlayMgrSrcType == PLAYMGR_SRC_TYPE_NETWORK_DLNA_LPCM)
    {
        prAudStrmAttr =&_arPlayerInfo[u1SrcId].FmtInfo.rMp3Info.rStreamAttr;
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
        
        //_arPlayerInfo[u1SrcId].rRangeInfo[0].z_range_sz =
        //    rPlayLPCMInfo.u8FileSize;
        //_arPlayerInfo[u1SrcId].rRangeInfo[0].ui8_pb_duration =
        //    (rPlayLPCMInfo.u8Duration * (UINT64)90000);
        _arPlayerInfo[u1SrcId].rRangeInfo[0].z_range_sz = (UINT64)(-1);
        _arPlayerInfo[u1SrcId].rRangeInfo[0].ui8_pb_duration = 0;
        
        _arPlayerInfo[u1SrcId].rPlayerStatus.u4TotalDuration =
            (UINT32)_arPlayerInfo[u1SrcId].rRangeInfo[0].ui8_pb_duration;
        _arPlayerInfo[u1SrcId].u8FileSize = 
            (UINT32)_arPlayerInfo[u1SrcId].rRangeInfo[0].z_range_sz;
        _arPlayerInfo[u1SrcId].rPlayerStatus.u8FileSize =
            (UINT32)_arPlayerInfo[u1SrcId].rRangeInfo[0].z_range_sz;
        
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

VOID _PLAYMGR_SetSubType(UINT32 u4SubType)
{    
    _u4SltSubType = u4SubType;
}

VOID _PLAYMGR_SLTVdecFlushCtrl(VOID)
{
    VDEC_DEC_NFY_INFO_T rVDecNfyInfo;

    rVDecNfyInfo.pvTag = 0;
    rVDecNfyInfo.pfDecNfy = _PLAYMGR_VdecNfy;
    VDEC_SetDecNfy(ES0, &rVDecNfyInfo);

    _PLAYMGR_VdecFlushCtrl(PLAYMGR0);
}

VOID _PLAYMGR_SLTVdecWaitCtrlDone(VOID)
{
    _PLAYMGR_VdecWaitCtrlDone(PLAYMGR0);
}

#endif

#endif // !defined(LINUX_TURNKEY_SOLUTION) || defined(CC_ENABLE_PLAYMGR)



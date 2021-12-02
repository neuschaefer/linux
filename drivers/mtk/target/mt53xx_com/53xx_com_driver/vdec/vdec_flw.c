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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_flw.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_flw.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "vdec_if.h"
#include "vdec_debug.h"
#include "vdec_esm.h"
#include "vdec_type.h"
#if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
#ifndef CC_VDEC_H264_DISABLE
#include "vdec_h264api.h"
#endif
#endif
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_VC1_DISABLE
#include "vdec_wmvapi.h"
#endif
#ifndef CC_VDEC_MPEG4_DISABLE
#include "vdec_mpeg4api.h"
#endif
#ifndef CC_VDEC_MJPEG_DISABLE
#include "vdec_img.h"
#endif
#ifndef CC_VDEC_RV_DISABLE
#include "vdec_rvapi.h"
#endif
#endif
#ifndef CC_VDEC_AVS_DISABLE
#include "vdec_avsapi.h"
#endif
#include "mpv_if.h"
#include "stc_drvif.h"
#include "fbm_drvif.h"
#include "x_timer.h"
#include "x_mid.h"
#include "imgrz_if.h"
#include "gfx_if.h"
#include "dmx_if.h"
#ifdef CC_MT5392B
#include "vdp_drvif.h"
#endif
LINT_EXT_HEADER_END

#ifdef __MODEL_slt__
EXTERN BOOL fgSltWatchPts;
EXTERN UINT32 u4SltPts;
EXTERN BOOL fgSltNotSendPic;
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#ifdef VDEC_TIME_PROFILE
#define DRV_PREV 0
#define HDR_PARSE 1
#define HW_DEC 2
#define MAX_SET 3
#endif
#define ON      1
#define OFF    0
//#define VDEC_THUMBNAIL_ADDR_CHECK
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgWaitFbgChg = FALSE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _VdecGetThumbnail(UCHAR ucEsId);
static void _VdecFlushDPB(UCHAR ucEsId);

UCHAR _VDecReceiveEs(UCHAR ucEsId);
void _VDecReceiveCmd(UCHAR ucEsId, BOOL bIsBlock);
void _VDEC_DeliverEos(UCHAR ucEsId);
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef VDEC_TIME_PROFILE
//static HAL_TIME_T _rVdecStart[MAX_SET], _rVdecMaxDt[MAX_SET], _rVdecCurDt[MAX_SET];
// 0:get msg q, 1:hdr parsing, 2:decoding time

static HAL_TIME_T _rTimeEsS, _rTimeEsE, _rTimeEsDt, _rTimeFrmS, _rTimeFrmE, _rTimeFrmDt, _rEsAcc;

/*static void _VdecPrintTime(UINT8 u1Idx)
{
    HAL_TIME_T rTime;

    HAL_GetTime(&rTime);
    LOG(1, "Time%d %u.%06us\n", u1Idx, rTime.u4Seconds, rTime.u4Micros);
}

static void _VdecTimeS(UINT8 u1ProfileIdx)
{
    if((_rVdecStart[u1ProfileIdx].u4Seconds == 0) && (_rVdecStart[u1ProfileIdx].u4Micros == 0))
    {
        HAL_GetTime(&_rVdecStart[u1ProfileIdx]);
    }
}

static void _VdecTimeE(UINT8 u1ProfileIdx, BOOL bShowLog)
{
    HAL_TIME_T rTimeEnd;
    HAL_GetTime(&rTimeEnd);
    HAL_GetDeltaTime(&_rVdecCurDt[u1ProfileIdx], &_rVdecStart[u1ProfileIdx], &rTimeEnd);

    if(_rVdecCurDt[u1ProfileIdx].u4Seconds > _rVdecMaxDt[u1ProfileIdx].u4Seconds)
    {
        _rVdecMaxDt[u1ProfileIdx].u4Seconds = _rVdecCurDt[u1ProfileIdx].u4Seconds;
        _rVdecMaxDt[u1ProfileIdx].u4Micros = _rVdecCurDt[u1ProfileIdx].u4Micros;
    }
    else if ((_rVdecCurDt[u1ProfileIdx].u4Seconds == _rVdecMaxDt[u1ProfileIdx].u4Seconds) &&
        (_rVdecCurDt[u1ProfileIdx].u4Micros > _rVdecMaxDt[u1ProfileIdx].u4Micros))
    {
        _rVdecMaxDt[u1ProfileIdx].u4Micros = _rVdecCurDt[u1ProfileIdx].u4Micros;
    }
    _rVdecStart[u1ProfileIdx].u4Seconds = 0;
    _rVdecStart[u1ProfileIdx].u4Micros = 0;

    if(bShowLog)
    {
        LOG(6, "Cur profileIdx%d %u.%06us\n", u1ProfileIdx,
            _rVdecCurDt[u1ProfileIdx].u4Seconds, _rVdecCurDt[u1ProfileIdx].u4Micros);
        LOG(6, "Max profileIdx%d %u.%06us\n", u1ProfileIdx,
            _rVdecMaxDt[u1ProfileIdx].u4Seconds, _rVdecMaxDt[u1ProfileIdx].u4Micros);
    }
}*/
#endif

void _VdecCheckStatus(UCHAR ucEsId, BOOL fgDecodeOK)
{
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    PARAM_MPV_T* prMpvCounter;
    static HAL_TIME_T _rErrStartTime;
    HAL_TIME_T rErrCurTime;
    HAL_TIME_T rDeltaTime;
    VDEC_DECERR_CB_T rDecErr;
    VDEC_PICTYPE_CB_T rPicType;
    UINT32 u4FailCntThreshold;


    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prMpvCounter = prVdecEsInfo->prMpvCounter;

    if(fgDecodeOK)
    {
        //notify decoder status
        prVdecEsInfo->u1PVRWaitSps = 0;
        prVdecEsInfo->u4FailCnt = 0; // reset fail count
        prVdecEsInfo->u4FbgFailCnt = 0; // reset fail count
        if(!prVdecEsInfo->fgFmtDectFinish)
        {
            prVdecEsInfo->fgFmtDectFinish = TRUE;
        }
        if((prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_NORMAL) &&
            (prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT))
        {
            if(VDEC_ChkCodecCap(ucEsId, prVdecInfo->eCurFMT))
            {
                LOG(3, "Notify Status change, decode normal\n");
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_NORMAL;
            }
            else
            {
                LOG(3, "Notify Status change, codec not support\n");
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
                prVdecInfo->eCurState = VDEC_ST_IDLE;
            }
            //callback, correct mpv data
            if (prVdecEsInfoKeep->pfDecNfyFct)
            {
                prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                VDEC_COND_DECODE_STATUS_CHG, (UINT32)prVdecEsInfo->eNotifyStatus, 0);
            }
        }

        if (prVdecInfo->pfPicTypeCb)
        {
            rPicType.u1EsId = ucEsId;
            if(prVdecEsInfo->fgSeqHdr)
            {
                LOG(6, "CB, PicType = Seq\n");
                rPicType.u1PicType = (UINT8)VDEC_PIC_TYPE_SEQ;
                prVdecInfo->pfPicTypeCb(&rPicType);
            }
            // map to VDEC_PIC_TYPE_T
            LOG(6, "CB, PicType = %d\n", (1 << prVdecEsInfo->ucPicType));
            rPicType.u1PicType = (UINT8)(1 << prVdecEsInfo->ucPicType);
            prVdecInfo->pfPicTypeCb(&rPicType);
        }
    }
    else
    {
        if(prVdecEsInfo->fgMMPlayback)
        {
            u4FailCntThreshold = VDEC_FRM_ERR_THRSD_PLAYBACK;
        }
        else
        {
            u4FailCntThreshold = VDEC_FRM_ERR_THRSD_DTV;
        }

        prVdecEsInfo->u4FailCnt++;

        if (prVdecInfo->pfnDecErrCb)
        {
            if (prVdecEsInfo->u4FailCnt == 1)
            {
                HAL_GetTime(&_rErrStartTime);
            }
            else
            {
                HAL_GetTime(&rErrCurTime);
                HAL_GetDeltaTime(&rDeltaTime, &_rErrStartTime, &rErrCurTime);

                if (((rDeltaTime.u4Seconds * 1000) + (rDeltaTime.u4Micros / 1000)) >= prVdecInfo->u4ErrDuration)
                {
                    rDecErr.u1EsId = ucEsId;
                    rDecErr.u4DecErrNs = prVdecEsInfo->u4FailCnt;
                    prVdecInfo->pfnDecErrCb(&rDecErr);
                    x_memcpy(&_rErrStartTime, &rErrCurTime, sizeof(HAL_TIME_T));
                }
            }
        }

        if (prVdecInfo->eCurState == VDEC_ST_PLAY)
        {
            //notify decoder status
            if((prMpvCounter->u2HSize > 720) &&
                (prMpvCounter->u2VSize > 576) &&
                (prVdecEsInfo->ucFbgId == FBM_FBG_ID_UNKNOWN) &&
                (prVdecEsInfo->u4FbgFailCnt > 0) &&
                (prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_HD_NOT_SUPPORT))
            {
                prVdecEsInfo->u4FailCnt = 0; // reset fail count
                prVdecEsInfo->u4FbgFailCnt = 0; // reset fail count

                //callback, non this format data
                if (prVdecEsInfoKeep->pfDecNfyFct)
                {
                    LOG(3, "Notify Status change, HD size\n");
                    prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                      VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_HD_NOT_SUPPORT, 0);
                }
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_HD_NOT_SUPPORT;
                prVdecEsInfo->fgFmtDectFinish = TRUE;
            }
            else if(prVdecEsInfo->u4FbgFailCnt > 0)
            {
                prVdecEsInfo->u4FailCnt = 0; // reset fail count
                prVdecEsInfo->u4FbgFailCnt = 0; // reset fail count
            }
            else if(prVdecEsInfo->eNotifyStatus == VDEC_DEC_DECODE_CODEC_NOT_SUPPORT)
            {
                prVdecEsInfo->u4FailCnt = 0; // reset fail count
                prVdecEsInfo->u4FbgFailCnt = 0; // reset fail count
                prVdecEsInfo->fgFmtDectFinish = TRUE;
            }
            else if((prVdecEsInfo->u4FailCnt >= u4FailCntThreshold) &&
              (prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_ERROR))
            {
                //callback, non mpv data
                if (prVdecEsInfoKeep->pfDecNfyFct)
                {  
                   if(prVdecEsInfo->u1PVRWaitSps != 2)
                    {
                     LOG(3, "Notify Status change, decode error\n");
                     prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                      VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_ERROR, 0);
                    }
                }
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_ERROR;
                prVdecEsInfo->fgFmtDectFinish = TRUE;
            }
        }
    }
}

static void _VdecTimeProfile(UCHAR ucEsId)
{
    UINT32 u4HwDecMs = 0;
    #ifndef VDEC_TIME_PROFILE
        #ifdef CC_MT5392B
        static UINT32 _u4MoniterCnt = 0;
        static BOOL _fgMJCOn = TRUE;
        #endif
        VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
        #ifdef CC_H264_SUPPORT
        #ifndef CC_H264_DISABLE
        VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
        UINT32 u4FbgId = 0, u4WaitFb = 0;
        #endif
        #endif

        #ifdef CC_H264_SUPPORT
        #ifndef CC_H264_DISABLE
        if(prVdecInfo->pfnVdecGetParam)
        {
            prVdecInfo->pfnVdecGetParam(ucEsId, (UINT32)TIME_PROFILE, &u4FbgId, &u4HwDecMs, &u4WaitFb);
        }
        #endif
        #endif
        #ifdef CC_MT5392B
        u4HwDecMs *= 1000;
        //dynamic switch mechanism
        if (u4HwDecMs > prVdecEsInfo->u4BigBLevel1)
        {
            if (u4HwDecMs > prVdecEsInfo->u4BigBLevel3)
            {
                _u4MoniterCnt = (3*prVdecEsInfo->u4BigBBaseCnt);
            }
            else
            if (u4HwDecMs > prVdecEsInfo->u4BigBLevel2)
            {
                _u4MoniterCnt = (2*prVdecEsInfo->u4BigBBaseCnt);
            }
            else
            {
                if (_u4MoniterCnt < prVdecEsInfo->u4BigBBaseCnt)
                {
                    _u4MoniterCnt = prVdecEsInfo->u4BigBBaseCnt;
                }
            }
        }
        else
        {
            if (_u4MoniterCnt != 0)
            {
                _u4MoniterCnt--;
            }
        }

        if((_u4MoniterCnt == 0) && !_fgMJCOn)
        {
            LOG(5, "Enable MJC\n");
            vDrvMJCDynOnOff(TRUE);
            _fgMJCOn = TRUE;
        }
        else if((_u4MoniterCnt > 0) && _fgMJCOn)
        {
            LOG(5, "Disable MJC\n");
            vDrvMJCDynOnOff(FALSE);
            _fgMJCOn = FALSE;
        }
        #else
        UNUSED(prVdecEsInfo);
        #endif
    #else
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    UINT32 u4FrmRate = 0, u4FbgId = 0, u4WaitFb = 0;
    UINT32 u4RstMs = 0, u4SliceType = 0, u4IsFrmReady = 0;
    UINT32 u4SwMicro = 0, u4HwDecCycle = 0, u4DramUseCycle = 0;
    UCHAR ucSliceType = '-';
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    static DRV_DBASE_T* _prDbase = NULL;
    CRIT_STATE_T _rCritState;


    if(prVdecInfo->pfnVdecGetParam)
    {
        prVdecInfo->pfnVdecGetParam(ucEsId, TIME_PROFILE, &u4FbgId, &u4HwDecMs, &u4WaitFb);
        prVdecInfo->pfnVdecGetParam(ucEsId, TIME_PROFILE2, &u4RstMs, &u4SliceType, &u4IsFrmReady);
        prVdecInfo->pfnVdecGetParam(ucEsId, TIME_PROFILE3, &u4SwMicro, &u4HwDecCycle, &u4DramUseCycle);
    }

    if((u4FbgId < 0xFF) && (u4IsFrmReady > 0))
    {
        HAL_GetTime(&_rTimeFrmE);
        HAL_GetDeltaTime(&_rTimeFrmDt, &_rTimeFrmS, &_rTimeFrmE);
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(u4FbgId);
        switch (prFbmSeqHdr->ucFrmRatCod)
        {
          case MPEG_FRAME_RATE_24_:
              u4FrmRate = 2397;
              break;
          case MPEG_FRAME_RATE_24:
              u4FrmRate = 2400;
              break;
          case MPEG_FRAME_RATE_25:
              u4FrmRate = 2500;
              break;
          case MPEG_FRAME_RATE_30_:
              u4FrmRate = 2997;
              break;
          case MPEG_FRAME_RATE_30:
              u4FrmRate = 3000;
              break;
          case MPEG_FRAME_RATE_50:
              u4FrmRate = 5000;
              break;
          case MPEG_FRAME_RATE_60_:
              u4FrmRate = 5994;
              break;
          case MPEG_FRAME_RATE_60:
              u4FrmRate = 6000;
              break;
          default:
              u4FrmRate = 3000;
              break;
        }
        if(u4SliceType >= 5)
        {
            u4SliceType -= 5;
        }
        switch(u4SliceType)
        {
            case 0:
                ucSliceType = 'P';
                break;
            case 1:
                ucSliceType = 'B';
                break;
            case 2:
                ucSliceType = 'I';
                break;
            case 3:
                ucSliceType = 'S';  //SP
                break;
            case 4:
                ucSliceType = 'S';  //SI
                break;
        }
        //dec time and wait es input time > 9/10 vsync time
        if((u4HwDecMs/* + (_rEsAcc.u4Micros/1000)*/) >= (90000 / u4FrmRate))
        {
            /*LOG(1, "Frm %u.%03us/HwDec 0.%03us/WEs %u.%03us/WFb 0.%03us/Rst 0.%03us/%c\n",
            _rTimeFrmDt.u4Seconds, _rTimeFrmDt.u4Micros/1000,
            u4HwDecMs,
            _rEsAcc.u4Seconds, _rEsAcc.u4Micros/1000,
            u4WaitFb,
            u4RstMs,
            ucSliceType
            );*/
            //if(_rEsAcc.u4Micros < 1000)
            {
                prVdecEsInfo->au4DecTime[u4SliceType][4]++;
            }
        }

        _prDbase = DBS_Lock(&_rCritState);
        ASSERT(_prDbase);
        VERIFY(DBS_Unlock(_prDbase, _rCritState));
        //LOG(1, "PicType %c, HwDec 0.%03us, SwDec 0.%03us, DramCycle %d, F(%d), Q(%d)\n",
        //    ucSliceType, u4HwDecMs, u4SwMicro/1000, u4DramUseCycle, _prDbase->rVdp.arFbCounter[0][0].u4DispFail,
        //    FBM_CheckFrameBufferDispQ(u4FbgId));
        _rEsAcc.u4Seconds = 0;
        _rEsAcc.u4Micros = 0;
        if(u4HwDecMs > prVdecEsInfo->au4DecTime[u4SliceType][0])
        {
            prVdecEsInfo->au4DecTime[u4SliceType][0] = u4HwDecMs;
        }
        else if(u4HwDecMs < prVdecEsInfo->au4DecTime[u4SliceType][1])
        {
            prVdecEsInfo->au4DecTime[u4SliceType][1] = u4HwDecMs;
        }

        if(u4SwMicro > prVdecEsInfo->au4DecTime[u4SliceType][5])
        {
            prVdecEsInfo->au4DecTime[u4SliceType][5] = u4SwMicro;
        }
        if(u4HwDecCycle > prVdecEsInfo->au4DecTime[u4SliceType][7])
        {
            prVdecEsInfo->au4DecTime[u4SliceType][7] = u4HwDecCycle;
        }
        if(u4DramUseCycle > prVdecEsInfo->au4DecTime[u4SliceType][8])
        {
            prVdecEsInfo->au4DecTime[u4SliceType][8] = u4DramUseCycle;
        }
        prVdecEsInfo->au4DecTime[u4SliceType][2] += u4HwDecMs;
        prVdecEsInfo->au4DecTime[u4SliceType][6] += (u4HwDecCycle/10000);
        prVdecEsInfo->au4DecTime[u4SliceType][3]++;
        HAL_GetTime(&_rTimeFrmS);
    }
    UNUSED(ucSliceType);    //only for log
    #endif
    UNUSED(u4HwDecMs);
}


static void _VdecSetSyncStc(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
    #ifndef CC_VDEC_H264_DISABLE
    VDEC_INFO_T* prVdecInfo;
    prVdecInfo = _VDEC_GetInfo();
    #endif
    #endif

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if (prVdecEsInfo->ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        FBM_SetSyncStc(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);
    }

    if (prVdecEsInfo->ucSyncMode== AV_SYNC_MODE_NONE)
    {
          //DTV00213023
          //if (STC_SetSyncTarget(AV_SYNC_TARGET_VDO,  0xFF, ucEsId) != STC_VALID)
          if (STC_SetSyncTarget(AV_SYNC_TARGET_VDO, prVdecEsInfo->ucStcId, ucEsId) != STC_VALID)
          {
              LOG(3, "_VdecSetSyncStc Invalid\n");
          }
          prVdecEsInfo->u2UdfNs = MPV_UDF_PES_NS;
          prVdecEsInfo->u4WaitDispTime = VDEC_WAIT_DISP_TIME;
    }
    else // need to SYNC
    {
          if (STC_SetSyncTarget(AV_SYNC_TARGET_VDO, prVdecEsInfo->ucStcId, ucEsId) != STC_VALID)
          {
              LOG(3, "_VdecSetSyncStc Invalid\n");
          }
          prVdecEsInfo->u2UdfNs = 0;
          if(prVdecEsInfo->fgMMPlayback)
          {
              prVdecEsInfo->u4WaitDispTime = VDEC_AVSYNC_WAIT_DISP_TIME;
          }
          else
          {
              prVdecEsInfo->u4WaitDispTime = VDEC_WAIT_DISP_TIME;
          }
    }
    #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
    #ifndef CC_VDEC_H264_DISABLE
    if(prVdecInfo->pfnVdecSetParam)
    {
         prVdecInfo->pfnVdecSetParam(ucEsId, (UINT32)WAIT_FB_TIME, prVdecEsInfo->u4WaitDispTime, 0, 0);
         prVdecInfo->pfnVdecSetParam(ucEsId, (UINT32)AV_SYNC_INFO, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId, 0);
    }
    #endif
    #endif
    STC_SetVdoDeviceId(prVdecEsInfo->ucStcId, ucEsId);
    //set VFIFO initial delay, will modify PTS value of audio and video by dmx
    if(!prVdecEsInfo->fgMMPlayback)
    {
        #if (defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)) && !defined(CC_VDEC_H264_DISABLE)
        if(prVdecInfo->eCurFMT == VDEC_FMT_H264)    // format will be mpv by default before _VDEC_DecCfg
        {
            VERIFY(STC_SetPtsDrift(prVdecEsInfo->ucStcId, H264_FIFO_CTRL_INIT) == STC_VALID);
        }
        else
        #endif
        {
            VERIFY(STC_SetPtsDrift(prVdecEsInfo->ucStcId, MPV_FIFO_CTRL_INIT) == STC_VALID);
        }
    }
}


static void _VdecGetThumbnail(UCHAR ucEsId)
{
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
	UINT32 u4WorkBuf = 0, u4AlignSz = 0,u4FbNum = 0, u4Idx,u4TmpBuf;	
#if 0	
	UINT32 u4Line=0,u4Hpos=0,u4Offset=0;
	UCHAR *pDstBuffer;
#endif 


    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    ASSERT(prVdecInfo != NULL);
    ASSERT(prVdecEsInfo != NULL);
	
#ifdef VDEC_THUMBNAIL_ADDR_CHECK
		{
		   UINT32 u4OsdFbmAddr=0,u4OsdPoolIndex=0;
		   UCHAR *pDstBuffer;
		   BOOL fgFindOSD=FALSE;
		   FBM_POOL_T *prFbmPool1;
		   pDstBuffer=VIRTUAL(prVdecEsInfo->rCapDstInfo.pucCanvasBuffer);
		   for(u4OsdPoolIndex=FBM_POOL_TYPE_JPEG_OSD1;u4OsdPoolIndex<=FBM_POOL_TYPE_JPEG_OSD4;u4OsdPoolIndex++)
		   {
			  prFbmPool1 = FBM_GetPoolInfo((UINT8)u4OsdPoolIndex);
			  u4OsdFbmAddr=VIRTUAL(prFbmPool1->u4Addr);
			  if(pDstBuffer>=u4OsdFbmAddr&& pDstBuffer<(u4OsdFbmAddr+prFbmPool1->u4Size))
			  {
				 fgFindOSD=TRUE;
				 break;
			  }
		   }
		   if(!fgFindOSD)
		   {
			  ASSERT(0);
		   }
		}
#endif



#if 0           //remove it for kernnel crash 
    u4Offset=prVdecEsInfo->rCapDstInfo.u4ThumbnailX*4;
    pDstBuffer=prVdecEsInfo->rCapDstInfo.pucCanvasBuffer+prVdecEsInfo->rCapDstInfo.u4ThumbnailY*prVdecEsInfo->rCapDstInfo.u4CanvasPitch;
    pDstBuffer=VIRTUAL(pDstBuffer);
	for(u4Line=0;u4Line<prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight;u4Line++)
    {
       for(u4Hpos=0;u4Hpos<prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth;u4Hpos++)
       {
         if(prVdecEsInfo->bIsOnePass)
         {
			 *(((UINT32 *)(VIRTUAL(pDstBuffer)+u4Offset))+u4Hpos)=0xff008080;
         }
		 else
		 {
			 *(((UINT32 *)(VIRTUAL(pDstBuffer)+u4Offset))+u4Hpos)=0xff000000;
		 }
       }
       pDstBuffer+=prVdecEsInfo->rCapDstInfo.u4CanvasPitch;
    }
#endif 

    if(!prVdecEsInfo->bIsOnePass)       //allocate working buffer for 2 pass
    {
        u4FbNum = (UINT32)FBM_GetFrameBufferNs(prVdecEsInfo->ucFbgId);
        for(u4Idx = 0; u4Idx < u4FbNum; u4Idx++)
        {
            if(FBM_GetFrameBufferStatus(prVdecEsInfo->ucFbgId, u4Idx)
                != FBM_FB_STATUS_LOCK)  //already freeze at mpv_mw.c
            {
                FBM_GetFrameBufferAddr(prVdecEsInfo->ucFbgId, u4Idx, &u4WorkBuf, &u4TmpBuf);
                UNUSED(u4TmpBuf);
                break;
            }
        }
        if((u4WorkBuf == 0) || (u4WorkBuf == prVdecEsInfo->rCapSrcInfo.u4AddrY))
        {
            LOG(3, "WorkingBuf err in GetThumbnail\n");
            return;
        }

        //check alignment, gfx convert can't set src pitch, should be equal to width
        //set active region in the middle of Thumbnail rectangle
        u4AlignSz = prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth & 0xF;

        if(u4AlignSz >0)
        {
            prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth -= u4AlignSz;
            prVdecEsInfo->rCapDstInfo.u4ThumbnailX+=(u4AlignSz>>1);
        }

        if(prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight&0x1)
        {
           prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight--;
        }

    }
    IMGRZ_SetDigest((void*)&prVdecEsInfo->rCapSrcInfo, (void*)&prVdecEsInfo->rCapDstInfo,
                    prVdecEsInfo->bIsOnePass, u4WorkBuf);
    if(!prVdecEsInfo->bIsOnePass)
    {
        UNUSED(GFX_ColorConvert((void*)&prVdecEsInfo->rCapDstInfo, u4WorkBuf, 0xFF));
    }
    if (prVdecInfo->pfErrNotify)
    {
        LOG(3, "Notify get thumbnail done, no data\n");
        prVdecInfo->pfErrNotify((UINT32)prVdecEsInfo->u2CompId,
            (UINT32)VDEC_COND_THUMBNAIL_DONE, 0, 0);
    }
}


static void _VdecFlushDPB(UCHAR ucEsId)
{
    UCHAR ucFRefFbId, ucBRefFbId;
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    FBM_PIC_HDR_T *prFbmPicHdr;

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if(!prVdecInfo)
    {
        LOG(3, "Flush buffer handle null!\n");
        return;
    }

    if ((prVdecEsInfo->ucFbgId != FBM_FBG_TYPE_UNKNOWN) &&
        (prVdecInfo->eCurState == VDEC_ST_IDLE))
    {
        if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
        {
            //flush dpb
            #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
            #ifndef CC_VDEC_H264_DISABLE
            _VDEC_H264Flush(ucEsId, TRUE);
            #endif
            #endif
        }
        else
        {
            //Put Backward Reference pic to DISPQ
            FBM_GetRefFrameBuffer(prVdecEsInfo->ucFbgId, &ucFRefFbId, &ucBRefFbId);
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
            if(prFbmPicHdr)
            {
                if(FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_READY))
                {
                    if(prVdecInfo->eCurFMT == VDEC_FMT_MPV)
                    {
                        UNUSED(_MPV_CalcPts(ucEsId, ucBRefFbId));
                    }
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_MPEG4_DISABLE
                    else if (prVdecInfo->eCurFMT == VDEC_FMT_MP4)
                    {
                        UNUSED(_VDEC_MPEG4CalPts(ucEsId, ucBRefFbId));
                    }
#endif
#endif

                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);

                    prVdecEsInfo->u4DisplayQPicCnt++;
                }
                if(FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_READY))
                {
                    if(prVdecInfo->eCurFMT == VDEC_FMT_MPV)
                    {
                        UNUSED(_MPV_CalcPts(ucEsId, ucFRefFbId));
                    }
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_MPEG4_DISABLE
                    else if (prVdecInfo->eCurFMT == VDEC_FMT_MP4)
                    {
                        UNUSED(_VDEC_MPEG4CalPts(ucEsId, ucBRefFbId));
                    }
#endif
#endif
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_DISPLAYQ);
                    prVdecEsInfo->u4DisplayQPicCnt++;
                }
            }
        }
    }
    else
    {
        LOG(3, "Flush buffer state err!\n");
    }
}


void _VDEC_DeliverEos(UCHAR ucEsId)
{
    //UCHAR ucFbgId;
    UCHAR ucFbId;
    UCHAR ucFbgType;
    UCHAR ucFbStatus;
    //UCHAR ucDecFbId;
    UCHAR ucFRefFbId, ucBRefFbId;
    FBM_PIC_HDR_T *prFbmPicHdr;
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if((!prVdecInfo) || (!prVdecEsInfo))
    {
        LOG(3, "DeEos handle null\n");
        return;
    }

    if((prVdecInfo->rPesInfo.fgEos) &&
        (!prVdecEsInfo->fgSendedEos))
    {
        prVdecEsInfo->fgSendedEos = TRUE;
        LOG(3, "_VDEC_DeliverEos\n");

        // if this is the last video frame,
        // do the same thing in VDEC_IsNotDisplay and VDEC_CheckStopStatus
        if (prVdecEsInfo->fgRenderBackStep)
        {
            prVdecEsInfo->fgRenderBackStep = FALSE;
            prVdecEsInfo->u8RenderFromPos = 0;
            prVdecEsInfo->i4RenderTempRef = 0;
            prVdecEsInfo->u2RenderDecodingOrder = 0;

            if(prVdecEsInfo->u2Disp2EmptyCnt > 0)
            {
                #if 0
                // That means we have previous display frame to empty queue.
                ucFbId = FBM_GetLatestEmptyFrameBuffer(prVdecEsInfo->ucFbgId,
                                        prVdecEsInfo->u4WaitDispTime);
                if(ucFbId != FBM_FB_ID_UNKNOWN)
                {
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);
                    LOG(5, "_VDEC_DeliverEos fgRenderBackStep ucFbId %d u8Offset 0x%llx u8RenderFromPos 0x%llx\n",
                        ucFbId, prVdecEsInfo->u8Offset, prVdecEsInfo->u8RenderFromPos);
                }
                else
                {
                    LOG(3, "_VDEC_DeliverEos FBM_GetLatestEmptyFrameBuffer FBM_FB_ID_UNKNOWN\n");
                }
                #endif
            }
            else
            {
                LOG(5, "_VDEC_DeliverEos fgRenderBackStep have no previous frame\n");
            }

            if(prVdecInfo->pfnTrickPtsCb)
            {
                // We have put two to empty that originaly should put to display.
                // That means we have previous display frame to empty queue.
                if(prVdecEsInfo->u2Disp2EmptyCnt > 1)
                {
                    prVdecInfo->pfnTrickPtsCb(0, FALSE);
                    LOG(5, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderBackStep finish\n");
                }
                else
                {
                    // Send again with far position
                    prVdecInfo->pfnTrickPtsCb(0, TRUE);
                    LOG(5, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderBackStep again\n");
                }
                //x_thread_delay(33);
            }
            if(prVdecInfo->pfnRenderPtsCb)
            {
                prVdecInfo->pfnRenderPtsCb(prVdecEsInfo->u4PTSLast);
                LOG(3, "_VDEC_DeliverEos pfnRenderPtsCb fgRenderBackStep\n");
            }

            VDEC_Pause(ucEsId);
            // switch to cmd queue (send cmd)
            x_thread_delay(1);

            LOG(3, "_VDEC_DeliverEos fgRenderBackStep\n");
            return;
        }

        if (prVdecEsInfo->ucFbgId != FBM_FBG_TYPE_UNKNOWN)
        {
            if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
            {
                //flush dpb
                #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
                #ifndef CC_VDEC_H264_DISABLE
                if(!prVdecEsInfo->fgKeepPes)
                {
                    _VDEC_H264Flush(ucEsId, FALSE);
                }
                else
                {
                    _VDEC_H264Flush(ucEsId, TRUE);
                }
                #endif
                #endif
            }
            else
            {
                // error handle for not finished frame. Ex. the last one is field.
                ucFbId = FBM_GetDecFrameBuffer(prVdecEsInfo->ucFbgId);
                ucFbStatus = FBM_GetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId);
                if ((ucFbStatus == FBM_FB_STATUS_DECODE) || (ucFbStatus == FBM_FB_STATUS_FIELD_READY) ||
                    (ucFbStatus == FBM_FB_STATUS_READY))
                {
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
                }
                // ~error

                //Put Backward Reference pic to DISPQ
                FBM_GetRefFrameBuffer(prVdecEsInfo->ucFbgId, &ucFRefFbId, &ucBRefFbId);
                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
                if(prFbmPicHdr)
                {
                    if(FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_READY))
                    {
                        //UNUSED(_MPV_CalcPts(ucEsId, ucBRefFbId));
                        if(!prVdecEsInfo->fgKeepPes)
                        {
                            FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_EMPTY);
                            LOG(3, "DeEos ucBRefFbId %d -> empty\n", ucBRefFbId);
                        }
                        else
                        {
                            if(prVdecInfo->eCurFMT == VDEC_FMT_MPV)
                            {
                                UNUSED(_MPV_CalcPts(ucEsId, ucBRefFbId));
                            }
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_MPEG4_DISABLE
                            else if (prVdecInfo->eCurFMT == VDEC_FMT_MP4)
                            {
                                UNUSED(_VDEC_MPEG4CalPts(ucEsId, ucBRefFbId));
                            }
#endif
#endif

                            FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);

                            if(prVdecEsInfo->u4DisplayQPicCnt == 0)
                            {
                                STC_StopStc();
                                STC_SetStcValue((UINT32)(prFbmPicHdr->u4PTS));
                                STC_StartStc();
                            }

                            prVdecEsInfo->u4DisplayQPicCnt++;
                            LOG(3, "DeEos ucBRefFbId %d -> display\n", ucBRefFbId);
                        }
                    }
                    if(FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_READY))
                    {
                        //UNUSED(_MPV_CalcPts(ucEsId, ucFRefFbId));
                        if(!prVdecEsInfo->fgKeepPes)
                        {
                            FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_EMPTY);
                            LOG(3, "DeEos ucFRefFbId %d -> empty\n", ucFRefFbId);
                        }
                        else
                        {
                            if(prVdecInfo->eCurFMT == VDEC_FMT_MPV)
                            {
                                UNUSED(_MPV_CalcPts(ucEsId, ucFRefFbId));
                            }
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_MPEG4_DISABLE
                            else if (prVdecInfo->eCurFMT == VDEC_FMT_MP4)
                            {
                                UNUSED(_VDEC_MPEG4CalPts(ucEsId, ucBRefFbId));
                            }
#endif
#endif
                            FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_DISPLAYQ);

                            if(prVdecEsInfo->u4DisplayQPicCnt == 0)
                            {
                                STC_StopStc();
                                STC_SetStcValue((UINT32)(prFbmPicHdr->u4PTS));
                                STC_StartStc();
                            }

                            prVdecEsInfo->u4DisplayQPicCnt++;
                            LOG(3, "DeEos ucFRefFbId %d -> display\n", ucFRefFbId);
                        }
                    }
                }
            }

            //Deliver a black picture if we don't get any video frame
            // Also deliver EOS.
            if(prVdecEsInfo->u4DisplayQPicCnt == 0)
            {
                if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
                {
                    ucFbId = FBM_GetEmptyFrameBuffer(prVdecEsInfo->ucFbgId, prVdecEsInfo->u4WaitDispTime);
                }
                else
                {
                    ucFbId = FBM_GetEmptyBFrameBuffer(prVdecEsInfo->ucFbgId,
                                        prVdecEsInfo->u4WaitDispTime);
                }

                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucFbId);
                if(prFbmPicHdr)
                {
                    UINT32 u4AddrY;
                    UINT32 u4AddrC;
                    STC_StopStc();
                    // DTV00139938, [MM]Flash wrong progress bar after excute FR2X and FF2X several times during playback
                    // should base on direct to send dummy black frame
                    if(prVdecEsInfo->u4MMSpeed <= STC_SPEED_TYPE_FORWARD_1_DOT_5X)
                    {
                        prFbmPicHdr->u4PTS = prVdecEsInfo->u4TotalTime;
                        prFbmPicHdr->u4TotlaTimeOffset = prVdecEsInfo->u4TotlaTimeOffset;
                        prFbmPicHdr->u8Offset = prVdecEsInfo->u8FileSize;
                        prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->u8FileSize;
                        prFbmPicHdr->u8OffsetI = prVdecEsInfo->u8FileSize;
                        STC_SetStcValue((UINT32)(prFbmPicHdr->u4PTS));
                    }
                    else
                    {
                        prFbmPicHdr->u4PTS = 0;
                        prFbmPicHdr->u4TotlaTimeOffset = 0;
                        prFbmPicHdr->u8Offset = 0;
                        prFbmPicHdr->u8OffsetDisp = 0;
                        prFbmPicHdr->u8OffsetI = 0;
                        STC_SetStcValue((UINT32)(0));
                    }
                    prFbmPicHdr->u2DecodingOrder = 0;
                    STC_StartStc();

                    u4AddrY = 0;
                    u4AddrC = 0;
                    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdecEsInfo->ucFbgId);
                    FBM_GetFrameBufferAddr(prVdecEsInfo->ucFbgId, ucFbId, &u4AddrY, &u4AddrC);
                    if((u4AddrY > 0) && (u4AddrC > 0) && prFbmSeqHdr)
                    {
                        x_memset((void*)VIRTUAL(u4AddrY), 0, (prFbmSeqHdr->u2LineSize*prFbmSeqHdr->u2VSize));
                        x_memset((void*)VIRTUAL(u4AddrC), 128, (prFbmSeqHdr->u2LineSize*prFbmSeqHdr->u2VSize)/2);
                        #if defined(CC_MT5365) || defined(CC_MT5395)
                        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4AddrY), (prFbmSeqHdr->u2LineSize*prFbmSeqHdr->u2VSize));
                        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4AddrC), (prFbmSeqHdr->u2LineSize*prFbmSeqHdr->u2VSize)>>1);
                        #else
                        HalFlushInvalidateDCache();
                        #endif
                        FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_PSEUDO_EOS_FLAG);
                        FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);
                        LOG(1, "DeEos Black Image ucFbId %d\n", ucFbId);
                    }
                    else
                    {
                        FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_EOS_FLAG);
                        FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
                        LOG(1, "DeEos Black Image null\n");
                    }
                }
                else
                {
                    FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_EOS_FLAG);
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
                    LOG(1, "DeEos Black Image prFbmPicHdr NULL");
                }
            }
            else
            {
                if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
                {
                    ucFbId = FBM_GetEmptyFrameBuffer(prVdecEsInfo->ucFbgId, prVdecEsInfo->u4WaitDispTime);
                }
                else
                {
                    ucFbId = FBM_GetEmptyBFrameBuffer(prVdecEsInfo->ucFbgId,
                                        prVdecEsInfo->u4WaitDispTime);
                }
                //Deliver EOS
                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucFbId);
                if(prFbmPicHdr)
                {
                    FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_EOS_FLAG);
                    FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_NOT_DISPLAY_FLAG);
                    //UNUSED(_MPV_CalcPts(ucEsId, ucFbId));
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);
                    LOG(1, "DeEos ucFbId %d\n", ucFbId);
                }
                else
                {
                    FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_EOS_FLAG);
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
                    LOG(1, "DeEos prFbmPicHdr NULL");
                }
            }
        }
        else
        {
            if(prVdecInfo->eCurFMT == VDEC_FMT_MPV  || prVdecInfo->eCurFMT == VDEC_FMT_MP4 )
            {
                LOG(5, "Create FBG\n");

                if(prVdecInfo->eCurFMT == VDEC_FMT_MPV  )
                {
                    ucFbgType = FBM_SelectGroupType(720, 480);
                    prVdecEsInfo->ucFbgId = FBM_CreateGroup(ucFbgType, FBM_VDEC_MPEG2, 0, 0);
                }
                else if(prVdecInfo->eCurFMT == VDEC_FMT_MP4)
                {
                    ucFbgType = FBM_SelectGroupType(720, 480);
                    prVdecEsInfo->ucFbgId = FBM_CreateGroup(ucFbgType, FBM_VDEC_MPEG4, 0, 0);
                    FBM_SetPlayMode(prVdecEsInfo->ucFbgId,FBM_FBG_MM_MODE);
                }
               
                if(prVdecEsInfo->ucFbgId != FBM_FBG_ID_UNKNOWN)
                {
                    FBM_FbgChgNotify(prVdecEsInfo->ucFbgId, ucEsId);
                    FBM_SetSyncStc(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId);

                    // Check if reset is required
                    if (FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_RESET))
                    {
                        FBM_ResetGroup(prVdecEsInfo->ucFbgId);
                        FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_RESET);
                        FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_NOT_READY);
                    }

                    /*
                    if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
                    {
                        ucFbId = FBM_GetEmptyFrameBuffer(prVdecEsInfo->ucFbgId, prVdecEsInfo->u4WaitDispTime);
                    }
                    else*/
                    {
                        ucFbId = FBM_GetEmptyBFrameBuffer(prVdecEsInfo->ucFbgId,
                                            prVdecEsInfo->u4WaitDispTime);
                    }

                    if(ucFbId != FBM_FB_ID_UNKNOWN)
                    {
                        FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_EOS_FLAG);
                        FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_NOT_DISPLAY_FLAG);
                        FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);
                        LOG(5, "DeEos ucFbId %d\n", ucFbId);
                    }
                    else
                    {
                        LOG(1, "Get empty frame buffer fail\n");
                    }
                }
                else
                {
                    LOG(3, "Create FBG fail\n");
                }
            }


            if ((prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_NO_DATA) &&
                (prVdecEsInfoKeep->pfDecNfyFct))    //enable after mw add this handler
            {
                LOG(3, "Notify Status change, decode no data\n");
                prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                    VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_NO_DATA, 0);
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_NO_DATA;
            }
        }
        // notify decode finish, in this case, we didn't get any video data, but eos.
        if (prVdecEsInfo->fgRenderFromFirstPic)
        {
            prVdecEsInfo->fgRenderFromFirstPic = FALSE;
            // DTV00210874
            // We will deliver two EOSs in trick mode
            // We should not let swdmx jump again in eos.
            /*
            if (prVdecEsInfo->fgPlayOneFrm)
            {
                if(prVdecInfo->pfnTrickPtsCb)
                {
                    prVdecInfo->pfnTrickPtsCb(0, FALSE);
                    LOG(3, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderFromFirstPic\n");
                }
            }
            */
            if(prVdecInfo->pfnRenderPtsCb)
            {
                prVdecInfo->pfnRenderPtsCb(
                    (prVdecEsInfo->u4TotalTime + prVdecEsInfo->u4TotlaTimeOffset));
                LOG(3, "_VDEC_DeliverEos pfnRenderPtsCb fgRenderFromFirstPic\n");
            }
        }
        else if ((prVdecEsInfo->fgRenderPts) &&
            (prVdecEsInfo->u4PTS == prVdecEsInfo->u4RenderPts))
        {
            prVdecEsInfo->fgRenderPts = FALSE;
            prVdecEsInfo->u4RenderPts = 0;
            // DTV00210874
            // We will deliver two EOSs in trick mode
            // We should not let swdmx jump again in eos.
            /*
            if (prVdecEsInfo->fgPlayOneFrm)
            {
                if(prVdecInfo->pfnTrickPtsCb)
                {
                    prVdecInfo->pfnTrickPtsCb(0, FALSE);
                    LOG(3, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderPts\n");
                }
            }
            */
            if(prVdecInfo->pfnRenderPtsCb)
            {
                prVdecInfo->pfnRenderPtsCb(
                    (prVdecEsInfo->u4TotalTime + prVdecEsInfo->u4TotlaTimeOffset));
                LOG(3, "_VDEC_DeliverEos pfnRenderPtsCb fgRenderPts\n");
            }
        }

        if ((prVdecEsInfo->fgRenderFromPos) &&
            (prVdecEsInfo->u8Offset > prVdecEsInfo->u8RenderFromPos))
        {
            prVdecEsInfo->fgRenderFromPos = FALSE;
            prVdecEsInfo->u8RenderFromPos = 0;
            prVdecEsInfo->u2RenderDecodingOrder = 0;
            prVdecEsInfo->i4RenderTempRef = 0;
            // DTV00210874
            // We will deliver two EOSs in trick mode
            // We should not let swdmx jump again in eos.
            /*
            if(prVdecInfo->pfnTrickPtsCb)
            {
                prVdecInfo->pfnTrickPtsCb(0, FALSE);
                LOG(3, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderFromPos\n");
            }
            */
            if(prVdecInfo->pfnRenderPtsCb)
            {
                prVdecInfo->pfnRenderPtsCb(
                    (prVdecEsInfo->u4TotalTime + prVdecEsInfo->u4TotlaTimeOffset));
                LOG(3, "_VDEC_DeliverEos pfnRenderPtsCb fgRenderFromPos\n");
            }
        }
        else if (prVdecEsInfo->fgRenderBackStep)
        {
            prVdecEsInfo->fgRenderBackStep = FALSE;
            prVdecEsInfo->u8RenderFromPos = 0;
            prVdecEsInfo->u2RenderDecodingOrder = 0;
            prVdecEsInfo->i4RenderTempRef = 0;
            // DTV00210874
            // We will deliver two EOSs in trick mode
            // We should not let swdmx jump again in eos.
            /*
            if (prVdecEsInfo->fgPlayOneFrm)
            {
                if(prVdecInfo->pfnTrickPtsCb)
                {
                    prVdecInfo->pfnTrickPtsCb(0, FALSE);
                    LOG(3, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderBackStep\n");
                }
            }
            */
            if(prVdecInfo->pfnRenderPtsCb)
            {
                prVdecInfo->pfnRenderPtsCb(
                    (prVdecEsInfo->u4TotalTime + prVdecEsInfo->u4TotlaTimeOffset));
                LOG(3, "_VDEC_DeliverEos pfnRenderPtsCb fgRenderBackStep\n");
            }
        }
        LOG(3, "_VDEC_DeliverEos Done\n");
    }
}


void _VDEC_SendCmd(UCHAR ucEsId, UINT32 u4Cmd)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    UINT32 u4CmdDump;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    LOG(3, "Send Cmd(%d) 0x%x\n", (UINT32)ucEsId, (UINT32)u4Cmd);
    
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    i4Ret = x_msg_q_send(prVdecEsInfoKeep->hCmdQ,(void*)&u4Cmd, VDEC_CMD_SIZE, 255);
    if (i4Ret == OSR_TOO_MANY)
    {
        zMsgSize = VDEC_CMD_SIZE;
        VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4CmdDump, &zMsgSize,
                &(prVdecEsInfoKeep->hCmdQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK);
        i4Ret = x_msg_q_send(prVdecEsInfoKeep->hCmdQ, (void *)&u4Cmd, VDEC_CMD_SIZE, 255);
    }
    else
    {
        VERIFY(i4Ret == OSR_OK);
    }

    //SYSLOG(60, ucEsId, u4Cmd, 0);

}

void _VDecReceiveCmd(UCHAR ucEsId, BOOL bIsBlock)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    UINT32 u4PreStatus;
    UINT32 u4Cmd;
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    zMsgSize = VDEC_CMD_SIZE;

    if(bIsBlock)
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, &u4Cmd, &zMsgSize,
                &(prVdecEsInfoKeep->hCmdQ), 1, X_MSGQ_OPTION_WAIT);
        ASSERT(i4Ret == OSR_OK);
    }
    else
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, &u4Cmd, &zMsgSize,
                &(prVdecEsInfoKeep->hCmdQ), 1, X_MSGQ_OPTION_NOWAIT);
        if (i4Ret == OSR_NO_MSG)
        {
            return;
        }
    }

    if (i4Ret == OSR_NO_MSG)
    {
        return;
    }
    else if (i4Ret == OSR_OK)
    {
        ASSERT(zMsgSize == VDEC_CMD_SIZE);
    }

    LOG(3, "Rsv Es%d u4Cmd %x\n", ucEsId, u4Cmd);

    u4PreStatus = (UINT32)prVdecInfo->eCurState;

    //filter illegal command
    switch (u4Cmd)
    {
        case VDEC_CMD_STOP:
            prVdecInfo->ePreviousState = prVdecInfo->eCurState;
            prVdecInfo->eCurState = VDEC_ST_STOP;
            prVdecInfo->u4PreviousStopId = prVdecInfo->u4CurStopId;
            prVdecInfo->u4CurStopId ++;
            prVdecInfo->fgWaitIFrame = TRUE;
            break;
        case VDEC_CMD_PLAY:
        case VDEC_CMD_PLAY_I_FRAME:
            if(prVdecInfo->eCurState == VDEC_ST_STOP)
            {
                prVdecInfo->ePreviousState = prVdecInfo->eCurState;
                prVdecInfo->eCurState = (u4Cmd == (UINT32)VDEC_CMD_PLAY) ? VDEC_ST_PLAY : VDEC_ST_PLAY_I_FRAME;
            }
            else if(prVdecInfo->eCurState == VDEC_ST_IDLE)
            {
                if(VDEC_ChkCodecCap(ucEsId, prVdecInfo->eCurFMT))
                {
                    prVdecInfo->ePreviousState = prVdecInfo->eCurState;
                    prVdecInfo->eCurState = (u4Cmd == (UINT32)VDEC_CMD_PLAY) ? VDEC_ST_PLAY : VDEC_ST_PLAY_I_FRAME;
                    prVdecEsInfo->ucDropBNs = 0;
                    if(prVdecEsInfo->fgRenderPts)   //for seek case
                    {
                        prVdecInfo->eDecStatus = VDEC_DEC_HEADER_PARSE;
                    }
                }
            }
            break;
        case VDEC_CMD_PAUSE:
            if (prVdecInfo->eCurState == VDEC_ST_PLAY)
            {
                prVdecInfo->ePreviousState = prVdecInfo->eCurState;
                prVdecInfo->eCurState = VDEC_ST_IDLE;
            }
            break;
        case VDEC_CMD_AVSYNC:
            _VdecSetSyncStc(ucEsId);
            break;
        case VDEC_CMD_DEBLOCK:
            _MPV_SetDbk(ucEsId);
            break;
        case VDEC_CMD_RR:
            _MPV_SetRR(ucEsId);
            break;
        case VDEC_CMD_CAPTURE:
            _VdecGetThumbnail(ucEsId);
            break;
        case VDEC_CMD_FLUSH:
            _VDEC_FlushEs(ucEsId);
            //clear decoder internal buffer, fbg release
            if(prVdecInfo->pfnVdecStop)
            {
                prVdecInfo->pfnVdecStop(ucEsId);
            }
            prVdecEsInfo->ucFbgId = 0xFF;
            prVdecInfo->eDecStatus = VDEC_DEC_HEADER_PARSE;
            break;
        case VDEC_CMD_FLUSH_DPB:
        	  _VdecFlushDPB(ucEsId);
        	  break;
        default:
            break;
    }

    if ((UINT32)prVdecInfo->eCurState != u4PreStatus)
    {
        LOG(3, "Es%d Status Change %x to %x\n", ucEsId, u4PreStatus, prVdecInfo->eCurState);
    }
}


UCHAR _VDecReceiveEs(UCHAR ucEsId)
{
    UCHAR ucMpvId;
    UINT16 u2MsgQIdx;
    INT8 i1EsBufStatus = 0;
    VDEC_PES_INFO_T* prPesInfo = NULL;
    SIZE_T zMsgSize;
    VDEC_INFO_T* prVdecInfo;
    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    PARAM_MPV_T* prMpvCounter;
    UINT32 u4Pts = 0, u4Dts = 0;
    UCHAR ucSkipMode = IPBMode;
    STC_CLOCK rStcClk;
    VDEC_STATUS_CB_T rStatusCB;
    static UINT32 u4VldReadPtr = 0;
    #ifdef CC_VDEC_FMT_DETECT
    UINT32 u4Type = 0, u4DmxType = (UINT32)VDEC_FMT_MAX, u4NtfyCodecType, u4Thrd;
    VDEC_DEC_DECODE_STATUS_T eNfyStatus;
    #endif

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prMpvCounter = prVdecEsInfo->prMpvCounter;

    if (!prVdecEsInfo->ucStarted)
    {
        VERIFY(x_sema_lock(prVdecEsInfoKeep->hUnderFlowSema, X_SEMA_OPTION_WAIT)
        == OSR_OK);
    }

    //get msg from dmx
    if(prVdecInfo->pfnVdecGetPesStruct)    //h264
    {
        prVdecInfo->pfnVdecGetPesStruct(ucEsId, (void**)&prPesInfo);
        if(prPesInfo == NULL)
        {
            return VLD0;
        }
    }


    zMsgSize = sizeof(VDEC_PES_INFO_T);
    VERIFY(x_sema_lock(prVdecEsInfoKeep->hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

#ifdef VDEC_TIME_PROFILE
    HAL_TIME_T rTimeRstS, rTimeRstE, rTimeRstDt;
    HAL_GetTime(&rTimeRstS);
#endif

    VERIFY(x_msg_q_receive(&u2MsgQIdx, &prVdecInfo->rPesInfo, &zMsgSize,
        &(prVdecEsInfoKeep->hMsgQ), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);

#ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeRstE);
    HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
    LOG(6, "VDec Receieve ES Delta T = (%ld), %d.%6d \n", rTimeRstDt.u4Micros,
        rTimeRstE.u4Seconds, rTimeRstE.u4Micros);
#endif


    if(prPesInfo)    //h264
    {
        if(!prVdecInfo->rPesInfo.fgEos)
        {
            ASSERT((prVdecInfo->rPesInfo.u4VldReadPtr <= prVdecInfo->rPesInfo.u4FifoEnd) &&
                (prVdecInfo->rPesInfo.u4VldReadPtr >= prVdecInfo->rPesInfo.u4FifoStart));
        }

        u4Pts = prPesInfo->u4PTS;
        u4Dts = prPesInfo->u4DTS;
        x_memcpy(prPesInfo, &prVdecInfo->rPesInfo, sizeof(prVdecInfo->rPesInfo));

        if(!prVdecInfo->rPesInfo.fgDtsValid)
        {
            prPesInfo->u4PTS = u4Pts;
            prPesInfo->u4DTS = u4Dts;
        }
        //error check
        if((prVdecInfo->eCurFMT == VDEC_FMT_H264VER) &&
            ((prPesInfo->ucPicType > 0) && (u4VldReadPtr == prPesInfo->u4VldReadPtr)))
        {
            LOG(5, "Rsv Dup Rptr 0x%x\n", prPesInfo->u4VldReadPtr);
            return VDEC_MAX_VLD;
        }
        u4VldReadPtr = prPesInfo->u4VldReadPtr;

        //log receive pesInfo timing
        if (STC_GetSrc(0, &rStcClk) == STC_VALID)
        {
            LOG(6, "Rsv PTS(0x%x, %d), DTS(0x%x), STC(0x%x)\n",
                prPesInfo->u4PTS, prPesInfo->fgDtsValid,
                prPesInfo->u4DTS, rStcClk.u4Base);
        }
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
        if(prVdecInfo->pfnVdecSetParam)
        {
            prVdecInfo->pfnVdecSetParam(ucEsId, (UINT32)MM_PARAM, (UINT32)(prVdecEsInfo->fgMMPlayback?1:0), 0, 0);
        }
        if((prVdecEsInfo->fgMMPlayback && prPesInfo->fgEos)
            && (!IS_H264_PIC_NALU(prPesInfo->ucPicType)))
        {
            return VDEC_MAX_VLD;
        }
        #endif
        #endif
    }
    VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
    VERIFY(x_sema_unlock(prVdecEsInfoKeep->hMutex) == OSR_OK);

    ASSERT(zMsgSize == sizeof(VDEC_PES_INFO_T));
    prMpvCounter->u4EsmQCnt = prVdecEsInfo->u2Count;

    //check if flush FB
    if(prVdecEsInfo->fgFlushFB)
    {
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
        if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
        {
            _VDEC_H264Flush(ucEsId, FALSE);
        }
        #endif
        #endif
        prVdecEsInfo->fgFlushFB = FALSE;
    }
    //dmx flush Q
    _VDEC_LockFlushMutex(ucEsId);
    if (prVdecEsInfo->fgFlushEsmQ)
    {
        LOG(3, "ES(%d) _VDecReceiveEs Flush\n", prVdecEsInfo->u4EsId);
        _VDEC_FlushEs(ucEsId);
        prVdecEsInfo->fgFlushEsmQ = FALSE;
        _VDEC_UnlockFlushSema(ucEsId);
        _VDEC_UnlockFlushMutex(ucEsId);
        return VDEC_MAX_VLD;
    }
    _VDEC_UnlockFlushMutex(ucEsId);

    ucMpvId = prVdecInfo->rPesInfo.ucMpvId;

#ifdef __MODEL_slt__
    if (fgSltWatchPts)
    {
        if ( (u4SltPts == prVdecInfo->rPesInfo.u4PTS) && (VDEC_IsLock(prVdecInfo->rPesInfo.ucEsId)))
        {
            fgSltNotSendPic = TRUE;
        }

        if(fgSltNotSendPic)
        {
            return VDEC_MAX_VLD;
        }
    }
#endif
    // MM
    #if 0
    if(prVdecEsInfo->fgTricking)
    {
        if((prMpvCounter->u4DecOkNs - prVdecEsInfo->u4PicCntJump) == 1)
        {
            return VDEC_MAX_VLD;
        }
    }
    #endif

    ucMpvId = prVdecInfo->rPesInfo.ucMpvId;
    #ifdef CC_VDEC_FMT_DETECT
    u4Type = prVdecInfo->rPesInfo.u4DmxFrameType;
    #endif

    // Handle Virtual Picture Header for Event Trigger
    if (ucMpvId == VDEC_MAX_VLD)
    {
        return VDEC_MAX_VLD;
    }
    else if(prVdecInfo->fgWaitIFrame)
    {
        if (prVdecEsInfoKeep->pfDecNfyFct)
           {
               LOG(3, "Notify Status change, wait i frame\n");
               prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                 VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_RECEIVE_DATA, 0);
           }
        prVdecInfo->fgWaitIFrame = FALSE;
    }
    #ifdef CC_VDEC_FMT_DETECT
    // Handle start code / codec type mismatch
    else if(u4Type > 0)
    {
        if(H264_PIC_VALID_TYPE(u4Type))
        {
            u4DmxType = (UINT32)VDEC_FMT_H264;
            if((u4Type != H264_PIC_TYPE_SEQ) &&
                (prVdecEsInfo->arRcvPesCnt[u4DmxType] == 0) &&
                (u4DmxType != (UINT32)prVdecInfo->eCurFMT))
            {
                return VDEC_MAX_VLD;    //start counting from seq header
            }
        }
        else if(MPV_PIC_VALID_TYPE(u4Type))
        {
            u4DmxType = (UINT32)VDEC_FMT_MPV;
        }
        else if(VC1_PIC_VALID_TYPE(u4Type))
        {
            u4DmxType = (UINT32)VDEC_FMT_WMV;
        }
        else if(MPEG4_PIC_VALID_TYPE(u4Type))
        {
            u4DmxType = (UINT32)VDEC_FMT_MP4;
        }
        else
        {
            return VDEC_MAX_VLD;
        }

        if((u4DmxType < (UINT32)VDEC_FMT_MAX) && (prVdecInfo->eCurFMT < VDEC_FMT_MAX))
        {
            prVdecEsInfo->arRcvPesCnt[u4DmxType]++;
            if(prVdecEsInfo->arRcvPesCnt[u4DmxType] == 0xFF)
            {
                x_memset(prVdecEsInfo->arRcvPesCnt, 0, sizeof(prVdecEsInfo->arRcvPesCnt));
            }

            if(u4DmxType != (UINT32)prVdecInfo->eCurFMT)
            {
                LOG(6, "detec (type,num)=(%d,%d), cur(%d,%d)\n",
                    u4DmxType, prVdecEsInfo->arRcvPesCnt[u4DmxType],
                    prVdecInfo->eCurFMT, prVdecEsInfo->arRcvPesCnt[prVdecInfo->eCurFMT]);
                //if(!prVdecEsInfo->fgFmtDectFinish)
                {
                    u4Thrd = (prMpvCounter->u4DecOkNs > CODEC_DETECT_THRD) ? prMpvCounter->u4DecOkNs : CODEC_DETECT_THRD;
                    if((prVdecEsInfo->arRcvPesCnt[u4DmxType] > prVdecEsInfo->arRcvPesCnt[(UINT32)prVdecInfo->eCurFMT]) &&
                        ((prVdecEsInfo->arRcvPesCnt[u4DmxType] - prVdecEsInfo->arRcvPesCnt[(UINT32)prVdecInfo->eCurFMT])
                        >= u4Thrd) /*&&
                        (prMpvCounter->u4DecOkNs <= CODEC_DETECT_THRD)*/)
                    {
                        switch(u4DmxType)
                        {
                            case VDEC_FMT_MPV:
                                u4NtfyCodecType = VDEC_CODEC_MPEG_2;
                                break;
                            case VDEC_FMT_MP4:
                                u4NtfyCodecType = VDEC_CODEC_MPEG_4;
                                break;
                            case VDEC_FMT_H264:
                                u4NtfyCodecType = VDEC_CODEC_H264;
                                break;
                            case VDEC_FMT_WMV:
                                u4NtfyCodecType = VDEC_CODEC_WVC1;
                                break;
                            case VDEC_FMT_MJPEG:
                                u4NtfyCodecType = VDEC_CODEC_MJPEG;
                                break;
                            default:
                                u4NtfyCodecType = VDEC_CODEC_UNKNOWN;
                                break;
                        }
                        prVdecEsInfo->fgFmtDectFinish = TRUE;
                        //notify MW
                        if(prVdecEsInfoKeep->pfDecNfyFct && (!prVdecEsInfo->fgMMPlayback))
                        {
                            if(VDEC_ChkCodecCap(ucEsId, (ENUM_VDEC_FMT_T)u4DmxType))
                            {
                                LOG(3, "Notify Status change, codec type change, type %d\n", u4DmxType);
                                eNfyStatus = VDEC_DEC_DECODE_CODEC_TYPE_CHG;
                            }
                            else
                            {
                                LOG(3, "Notify Status change, codec not support\n");
                                eNfyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
                                prVdecInfo->eCurState = VDEC_ST_IDLE;
                            }
                            if(prVdecEsInfo->eNotifyStatus != eNfyStatus)
                            {
                                prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                                    VDEC_COND_DECODE_STATUS_CHG, (UINT32)eNfyStatus, u4NtfyCodecType);
                                prVdecEsInfo->eNotifyStatus = eNfyStatus;
                            }
                            x_memset(prVdecEsInfo->arRcvPesCnt, 0, sizeof(prVdecEsInfo->arRcvPesCnt));
                        }
                    }
                }
                return VDEC_MAX_VLD;
            }
        }
    }
    #endif

    // [LOG] Get message with ESM Q, Start Decode
    //SYSLOG(prVdecEsInfo->u4EsId, 2, prVdecEsInfo->u2Count, prVdecInfo->rPesInfo.ucPicType);

    ASSERT(ucMpvId < VDEC_MAX_VLD);
    //VERIFY(prVdecInfo->rPesInfo.ucPicType > 0);

    if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
    {
        if((prVdecInfo->rPesInfo.ucPicType & 0x1f) <= 0x5)
        {
            prMpvCounter->u4RetrieveNs++;
        }
    }
    else
    {
        prMpvCounter->u4RetrieveNs++;
    }

    prVdecEsInfo->u4FifoStart = prVdecInfo->rPesInfo.u4FifoStart;
    prVdecEsInfo->u4FifoEnd = prVdecInfo->rPesInfo.u4FifoEnd;
    prVdecEsInfo->u4VldReadPtr = prVdecInfo->rPesInfo.u4VldReadPtr;
    prVdecEsInfo->u4PTS = prVdecInfo->rPesInfo.u4PTS;
    prVdecEsInfo->u4DTS = prVdecInfo->rPesInfo.u4DTS;
    prVdecEsInfo->ucPicType = prVdecInfo->rPesInfo.ucPicType;
    prVdecEsInfo->fgDtsValid = prVdecInfo->rPesInfo.fgDtsValid;
    prVdecEsInfo->fgSeqHdr = prVdecInfo->rPesInfo.fgSeqHdr;
    prVdecEsInfo->u8Offset = prVdecInfo->rPesInfo.u8Offset;
    if(prVdecEsInfo->fgMMPlayback)
    {
        if((prVdecEsInfo->u8OffsetLast > 0)
           &&
           // forward
           (prVdecEsInfo->u4MMSpeed <= STC_SPEED_TYPE_FORWARD_1_DOT_5X))
        {
            if(prVdecEsInfo->u8OffsetLast < prVdecEsInfo->u8Offset)
            {
                prVdecEsInfo->u8OffsetLast = prVdecEsInfo->u8Offset;
            }
        }
        else
        {
            prVdecEsInfo->u8OffsetLast = prVdecEsInfo->u8Offset;
        }
    }
    prVdecEsInfo->u8OffsetI = prVdecInfo->rPesInfo.u8OffsetI;
    prVdecEsInfo->u4TickNum = prVdecInfo->rPesInfo.u4TickNum;
    prVdecEsInfo->u2DecodingOrder = prVdecInfo->rPesInfo.u2DecodingOrder;
    prVdecEsInfo->u4TotlaTimeOffset = prVdecInfo->rPesInfo.u4TotalTimeOffset;

    if((prVdecEsInfo->fgMMPlayback) &&
        (prVdecInfo->rPesInfo.fgEos) &&
        (prVdecEsInfo->u4FifoStart == 0) &&
        (prVdecEsInfo->u4FifoEnd == 0) &&
        (prVdecEsInfo->u4VldReadPtr == 0))
    {
        // EOS and without any accessable video frame
        _VDEC_DeliverEos(ucEsId);
        return VDEC_MAX_VLD;
    }

    if ((prVdecEsInfoKeep->fgDmxPartialPes) && (prVdecEsInfo->fgChgThrsd)
        && (prVdecInfo->eCurState != VDEC_ST_PLAY_I_FRAME))
    {
        if (prVdecEsInfo->u4LastVldRptr == 0)
        {
            prVdecEsInfo->fgSetDmxIntThrsd = TRUE;
            prVdecEsInfo->fgCalDmxIntThrsd = TRUE;
            prVdecEsInfo->u4LastVldRptr = prVdecEsInfo->u4VldReadPtr;
            prVdecEsInfo->u4DmxIntThrsd = 0 ;
        }
        else if (prVdecEsInfo->fgCalDmxIntThrsd)
        {
            if (prVdecEsInfo->u4VldReadPtr > prVdecEsInfo->u4LastVldRptr)
            {
                prVdecEsInfo->u4DmxIntThrsd = prVdecEsInfo->u4VldReadPtr - prVdecEsInfo->u4LastVldRptr;
            }
            else
            {
                prVdecEsInfo->u4DmxIntThrsd = (prVdecEsInfo->u4FifoEnd - prVdecEsInfo->u4FifoStart) -
                                                        (prVdecEsInfo->u4LastVldRptr - prVdecEsInfo->u4VldReadPtr);
            }

            prVdecEsInfo->u4TotalPicSize += prVdecEsInfo->u4DmxIntThrsd;
            prVdecEsInfo->u4PicCnt ++;

            if (prVdecEsInfo->u4PicCnt == 10)
            {
                prVdecEsInfo->fgCalDmxIntThrsd = FALSE;
                prVdecEsInfo->fgSetDmxIntThrsd = TRUE;
                prVdecEsInfo->u4DmxIntThrsd = prVdecEsInfo->u4TotalPicSize /prVdecEsInfo->u4PicCnt;
                prVdecEsInfo->fgChgThrsd = FALSE;
            }
            else
            {
                prVdecEsInfo->u4LastVldRptr = prVdecEsInfo->u4VldReadPtr;
            }
        }

        if ((prVdecEsInfo->fgSetDmxIntThrsd) && (prVdecEsInfo->u4DmxIntThrsd !=0))
        {
            VERIFY( DMX_SetInterruptThreshold(DMX_PID_TYPE_ES_VIDEO , prVdecEsInfo->u4DmxIntThrsd));
            prVdecEsInfo->fgSetDmxIntThrsd = FALSE;
            LOG(5, "DMX_SetInterruptThreshold 0x%x\n", prVdecEsInfo->u4DmxIntThrsd);
        }
    }

    #ifdef __MODEL_slt__
    if ((prVdecEsInfo->u4PTS == prVdecEsInfo->u4StartPts) &&
        (prVdecEsInfo->u4PTS != 0) &&
        (prVdecEsInfo->fgCountPicFinish == FALSE))
    {
        prVdecEsInfo->fgCountPicNs = TRUE;
        LOG(1, "SLT: start counting pictures\n");
    }
    #endif

    if(prVdecEsInfo->fgMMPlayback)
    {

        switch (prVdecEsInfo->u4MMSpeed)
        {
        case STC_SPEED_TYPE_FORWARD_2X:
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
            if(prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                LOG(5, "Skip mode 2x -> I only\n");
                ucSkipMode = IMode;
            }
            else
#endif
            {
                LOG(5, "Skip mode -> I/P\n");
                ucSkipMode = IPMode;
            }
            break;
        case STC_SPEED_TYPE_FORWARD_4X:
        case STC_SPEED_TYPE_FORWARD_8X:
        case STC_SPEED_TYPE_FORWARD_16X:
        case STC_SPEED_TYPE_FORWARD_32X:
        case STC_SPEED_TYPE_FORWARD_64X:
        case STC_SPEED_TYPE_FORWARD_128X:
        case STC_SPEED_TYPE_FORWARD_256X:
        case STC_SPEED_TYPE_FORWARD_512X:
        case STC_SPEED_TYPE_FORWARD_1024X:
        case STC_SPEED_TYPE_REWIND_1X:
        case STC_SPEED_TYPE_REWIND_2X:
        case STC_SPEED_TYPE_REWIND_4X:
        case STC_SPEED_TYPE_REWIND_8X:
        case STC_SPEED_TYPE_REWIND_16X:
        case STC_SPEED_TYPE_REWIND_32X:
        case STC_SPEED_TYPE_REWIND_64X:
        case STC_SPEED_TYPE_REWIND_128X:
        case STC_SPEED_TYPE_REWIND_256X:
        case STC_SPEED_TYPE_REWIND_512X:
        case STC_SPEED_TYPE_REWIND_1024X:
            LOG(5, "Skip mode -> I only\n");
            ucSkipMode = IMode;
            break;
        default:
            LOG(6, "Skip mode -> I/P/B\n");
            ucSkipMode = IPBMode;
            break;
        }
    }

    //underflow callback
    if (prVdecEsInfo->u2Count <= prVdecEsInfo->u2UdfNs)
    {
        if(prVdecInfo->pfnStatusCb)
        {
            rStatusCB.u1EsId = ucEsId;
            rStatusCB.u4Status = (UINT32)VDEC_BUF_UNDERFLOW;
            prVdecInfo->pfnStatusCb(&rStatusCB);
        }
    }
    //skip es
    if(prVdecInfo->pfnVdecEsFlwCtrl)
    {
        if (prVdecEsInfo->u2Count <= prVdecEsInfo->u2UdfNs)
        {
            i1EsBufStatus = -1;
        }
        else if (prVdecEsInfo->ucDropBNs > 0)
        {
            i1EsBufStatus = 1;
        }
        if(prVdecInfo->pfnVdecEsFlwCtrl(ucEsId, &ucMpvId, i1EsBufStatus, ucSkipMode))
        {
            if((i1EsBufStatus > 0) && (prVdecEsInfo->ucDropBNs > 0))
            {
                prVdecEsInfo->ucDropBNs--;
            }
            return VDEC_MAX_VLD;//_VDecReceiveEs(prVdecEsInfo->u4EsId);
        }
    }

    return (ucMpvId);
}

UCHAR _VDEC_HandleOverFlw(UCHAR ucVldId, UCHAR ucEsmQId, HANDLE_T hMutex,
    HANDLE_T hMsgQueue, const void* pvPesInfo)
{
    ASSERT(pvPesInfo!=NULL);
    //temp code
    do{
        x_thread_delay(1);
    }while(x_msg_q_send(hMsgQueue, pvPesInfo, sizeof(VDEC_PES_INFO_T), 255) != OSR_OK);

    UNUSED(ucVldId);
    UNUSED(ucEsmQId);
    UNUSED(hMutex);
    return 1;
}

INT32 _VDEC_DecCfg(UCHAR ucEsId)
{
    INT32 i4Ret = (INT32)E_VDEC_OK;
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    switch(prVdecInfo->eCurFMT)
    {
        case VDEC_FMT_H264VER:
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
            prVdecInfo->pfnVdecSWRst = _VDEC_H264SWRst;
            prVdecInfo->pfnVdecDecode = NULL;
            prVdecInfo->pfnVdecHdrParse = _VDEC_H264HdrPrsNalu;//_VDEC_H264HdrPrsPic;
            prVdecInfo->pfnVdecDecStart = _VDEC_H264DecStart;
            prVdecInfo->pfnVdecDecFin = _VDEC_H264DecFin;
            prVdecInfo->pfnVdecDecDrop = NULL;
            prVdecInfo->pfnVdecStop = _VDEC_H264Stop;
            prVdecInfo->pfnVdecGetPesStruct = _VDEC_H264GetPesStruct; // should not need this?
            prVdecInfo->pfnVdecGetDispPic = _VDEC_H264GetDispPic;
            prVdecInfo->pfnVdecSetParam = _VDEC_H264SetParam;
            prVdecInfo->pfnVdecGetParam = _VDEC_H264GetParam;
            prVdecInfo->pfnVdecUpdRptr = NULL;
            prVdecInfo->pfnVdecSetWptr = NULL;
            prVdecInfo->pfnVdecEsFlwCtrl = NULL;
            prVdecInfo->pfnVdecWaitRandomAP = NULL;
            prVdecInfo->pfnVdecHandleOverFlw = _VDEC_H264HandleOverFlw;
            prVdecInfo->pfnVdecStartPts = NULL;
            //init function
            i4Ret = _VDEC_H264Init();
            if(prVdecInfo->pfnVdecSetParam)
            {
                prVdecInfo->pfnVdecSetParam(ucEsId, (UINT32)WAIT_FB_TIME, VDEC_WAIT_DISP_TIME, 0, 0);
            }
            //set VFIFO initial delay, will modify PTS value of audio and video by dmx
            if(!prVdecEsInfo->fgMMPlayback)
            {
                VERIFY(STC_SetPtsDrift(prVdecEsInfo->ucStcId, H264_FIFO_CTRL_INIT) == STC_VALID);
            }
        #endif
        #endif
            break;
        case VDEC_FMT_H264:
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
            prVdecInfo->pfnVdecSWRst = _VDEC_H264SWRst;
            prVdecInfo->pfnVdecDecode = NULL;
            prVdecInfo->pfnVdecHdrParse = _VDEC_H264HdrPrsNalu;//_VDEC_H264HdrPrsPic;
            prVdecInfo->pfnVdecDecStart = _VDEC_H264DecStart;
            prVdecInfo->pfnVdecDecFin = _VDEC_H264DecFin;
            prVdecInfo->pfnVdecDecDrop = NULL;
            prVdecInfo->pfnVdecStop = _VDEC_H264Stop;
            prVdecInfo->pfnVdecGetPesStruct = _VDEC_H264GetPesStruct; // should not need this?
            prVdecInfo->pfnVdecGetDispPic = _VDEC_H264GetDispPic;
            prVdecInfo->pfnVdecSetParam = _VDEC_H264SetParam;
            prVdecInfo->pfnVdecGetParam = _VDEC_H264GetParam;
            prVdecInfo->pfnVdecUpdRptr = _VDEC_H264UpdDmxRptr;
            prVdecInfo->pfnVdecSetWptr = _VDEC_H264SetDmxWptr;
            prVdecInfo->pfnVdecEsFlwCtrl = _VDEC_H264EsFlwCtrl;
            prVdecInfo->pfnVdecWaitRandomAP = NULL;
            prVdecInfo->pfnVdecHandleOverFlw = _VDEC_H264HandleOverFlw;
            prVdecInfo->pfnVdecStartPts = _VDEC_H264StartPTS;
            //init function
            i4Ret = _VDEC_H264Init();
            if(prVdecInfo->pfnVdecSetParam)
            {
                prVdecInfo->pfnVdecSetParam(ucEsId, (UINT32)WAIT_FB_TIME, prVdecEsInfo->u4WaitDispTime, 0, 0);
                prVdecInfo->pfnVdecSetParam(ucEsId, (UINT32)AV_SYNC_INFO, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId, 0);
            }
        #endif
        #endif
            break;
        case VDEC_FMT_MPV:
            prVdecInfo->pfnVdecSWRst = _MPV_DecReset;
            prVdecInfo->pfnVdecDecode = _MPV_Decode;
            prVdecInfo->pfnVdecHdrParse = _MPV_VParser;
            prVdecInfo->pfnVdecDecStart = _MPV_DecodeProc;
            prVdecInfo->pfnVdecDecFin = _MPV_WaitDecodeProc;
            prVdecInfo->pfnVdecDecDrop = _MPV_DropPic;
            prVdecInfo->pfnVdecStop = _MPV_StopDecES;
            prVdecInfo->pfnVdecGetPesStruct = NULL;
            prVdecInfo->pfnVdecGetDispPic = NULL;
            prVdecInfo->pfnVdecSetParam = NULL;
            prVdecInfo->pfnVdecGetParam = NULL;
            prVdecInfo->pfnVdecEsFlwCtrl = MPV_EsFlwCtrl;
            prVdecInfo->pfnVdecWaitRandomAP = _MPV_WaitRandomAP;
            #ifndef MPV_VIRTUAL_PRS
            prVdecInfo->pfnVdecHandleOverFlw = _MPV_HandleOverFlow;
            prVdecInfo->pfnVdecUpdRptr = MPV_UpdateVldRptr;
            prVdecInfo->pfnVdecSetWptr = MPV_UpdateVldWptr;
            #else
            prVdecInfo->pfnVdecHandleOverFlw = NULL;
            prVdecInfo->pfnVdecUpdRptr = NULL;
            prVdecInfo->pfnVdecSetWptr = NULL;
            prVdecInfo->pfnVdecStartPts = NULL;
            #endif
            //init function
            MPV_Init();
            break;
        case VDEC_FMT_WMV:
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_VC1_DISABLE
            prVdecInfo->pfnVdecSWRst = _VDEC_WMVSWRst;
            prVdecInfo->pfnVdecDecode = NULL;
            prVdecInfo->pfnVdecHdrParse = _VDEC_WMVHdrPrs;//_VDEC_H264HdrPrsPic;
            prVdecInfo->pfnVdecDecStart = _VDEC_WMVDecStart;
            prVdecInfo->pfnVdecDecFin = _VDEC_WMVDecFin;
            prVdecInfo->pfnVdecDecDrop = NULL;
            prVdecInfo->pfnVdecStop = _VDEC_WMVStop;
            prVdecInfo->pfnVdecGetPesStruct = _VDEC_WMVGetPesStruct; // should not need this?
            prVdecInfo->pfnVdecGetDispPic = _VDEC_WMVGetDispPic;
            prVdecInfo->pfnVdecSetParam = _VDEC_WMVSetParam;
            prVdecInfo->pfnVdecGetParam = NULL;
            prVdecInfo->pfnVdecEsFlwCtrl = _VDEC_WMVEsFlwCtrl;
            prVdecInfo->pfnVdecWaitRandomAP = NULL;
            prVdecInfo->pfnVdecHandleOverFlw = _VDEC_HandleOverFlw;
            prVdecInfo->pfnVdecUpdRptr = _VDEC_WMVUpdDmxRptr;
            prVdecInfo->pfnVdecSetWptr = _VDEC_WMVSetDmxWptr;
            prVdecInfo->pfnVdecStartPts = _VDEC_WMVStartPTS;
            //init function
            i4Ret = _VDEC_WMVInit();
        #endif
        #endif
            break;
        case VDEC_FMT_RV:
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_RV_DISABLE
            prVdecInfo->pfnVdecSWRst = _VDEC_RVSWRst;
            prVdecInfo->pfnVdecDecode = NULL;
            prVdecInfo->pfnVdecHdrParse = _VDEC_RVHdrPrs;//_VDEC_H264HdrPrsPic;
            prVdecInfo->pfnVdecDecStart = _VDEC_RVDecStart;
            prVdecInfo->pfnVdecDecFin = _VDEC_RVDecFin;
            prVdecInfo->pfnVdecDecDrop = NULL;
            prVdecInfo->pfnVdecStop = _VDEC_RVStop;
            prVdecInfo->pfnVdecGetPesStruct = _VDEC_RVGetPesStruct; // should not need this?
            prVdecInfo->pfnVdecGetDispPic = _VDEC_RVGetDispPic;
            prVdecInfo->pfnVdecSetParam = _VDEC_RVSetParam;
            prVdecInfo->pfnVdecGetParam = NULL;
            prVdecInfo->pfnVdecEsFlwCtrl = _VDEC_RVEsFlwCtrl;
            prVdecInfo->pfnVdecWaitRandomAP = NULL;
            prVdecInfo->pfnVdecHandleOverFlw = _VDEC_HandleOverFlw;
            prVdecInfo->pfnVdecUpdRptr = _VDEC_RVUpdDmxRptr;
            prVdecInfo->pfnVdecSetWptr = _VDEC_RVSetDmxWptr;
            prVdecInfo->pfnVdecStartPts = NULL;
            //init function
            i4Ret = _VDEC_RVInit();
        #endif
        #endif
            break;
        case VDEC_FMT_MP4:
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_MPEG4_DISABLE
            //prVdecEsInfo->hVdec = _VDEC_Mpeg4GetHdlr();
            prVdecInfo->pfnVdecSWRst = _VDEC_Mpeg4SWRst;
            prVdecInfo->pfnVdecDecode = NULL;
            prVdecInfo->pfnVdecHdrParse = _VDEC_Mpeg4PrsHdr;
            prVdecInfo->pfnVdecDecStart = _VDEC_Mpeg4DecStart;
            prVdecInfo->pfnVdecDecFin = _VDEC_Mpeg4DecFin;
            prVdecInfo->pfnVdecDecDrop = _VDEC_MPEG4DropFrame;
            prVdecInfo->pfnVdecStop = _VDEC_Mpeg4Stop;
            prVdecInfo->pfnVdecGetPesStruct = _VDEC_GetMpeg4PesStruct; // should not need this?
            prVdecInfo->pfnVdecGetDispPic = _VDEC_GetMpeg4DispPic;
            prVdecInfo->pfnVdecSetParam = _VDEC_Mpeg4SetParam;
            prVdecInfo->pfnVdecGetParam = NULL;
            prVdecInfo->pfnVdecEsFlwCtrl = _VDEC_Mpeg4EsFlwCtrl;
            /* prVdecInfo->pfnVdecGetParam = _VDEC_H264GetParam;*/
            prVdecInfo->pfnVdecWaitRandomAP = NULL;
            prVdecInfo->pfnVdecUpdRptr = _VDEC_MPEG4UpdDmxRptr;
            prVdecInfo->pfnVdecSetWptr = _VDEC_MPEG4SetDmxWptr;
            prVdecInfo->pfnVdecHandleOverFlw = _VDEC_HandleOverFlw;
            prVdecInfo->pfnVdecStartPts = _VDEC_MPEG4StartPTS;
            //init function
            i4Ret = _VDEC_Mpeg4Init();
        #endif
        #endif
            break;
        case VDEC_FMT_MJPEG:
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_MJPEG_DISABLE
            prVdecInfo->pfnVdecSWRst = _VDEC_MJPEGSWRst;
            prVdecInfo->pfnVdecDecode = _VDEC_MJPEGDecode;
            prVdecInfo->pfnVdecHdrParse = NULL;
            prVdecInfo->pfnVdecDecStart = NULL;
            prVdecInfo->pfnVdecDecFin = NULL;
            prVdecInfo->pfnVdecDecDrop = NULL;
            prVdecInfo->pfnVdecStop = _VDEC_MJPEGStop;
            prVdecInfo->pfnVdecGetPesStruct = _VDEC_MJPEGGetPesStruct;
            prVdecInfo->pfnVdecGetDispPic = NULL;
            prVdecInfo->pfnVdecSetParam = NULL;
            prVdecInfo->pfnVdecGetParam = NULL;
            prVdecInfo->pfnVdecEsFlwCtrl = _VDEC_MJPEGEsFlwCtrl;
            prVdecInfo->pfnVdecWaitRandomAP = NULL;
            prVdecInfo->pfnVdecHandleOverFlw = _VDEC_HandleOverFlw;
            prVdecInfo->pfnVdecUpdRptr = _VDEC_MJPEGUpdDmxRptr;
            prVdecInfo->pfnVdecSetWptr = _VDEC_MJPEGSetDmxWptr;
            //init function
            i4Ret = _VDEC_MJPEGInit();
        #endif
        #endif
            break;
#ifndef CC_VDEC_AVS_DISABLE
        case VDEC_FMT_AVS: 
            prVdecInfo->pfnVdecSWRst = _VDEC_AVSSWRst;                        
            prVdecInfo->pfnVdecDecode = NULL;                        
            prVdecInfo->pfnVdecHdrParse = _VDEC_AVSHdrPrsHdr;                        
            prVdecInfo->pfnVdecDecStart = _VDEC_AVSDecStart;                        
            prVdecInfo->pfnVdecDecFin = _VDEC_AVSDecFin;                        
            prVdecInfo->pfnVdecDecDrop = _VDEC_AVSDropFrame;                        
            prVdecInfo->pfnVdecStop = _VDEC_AVSStop;                        
            prVdecInfo->pfnVdecGetPesStruct = _VDEC_AVSGetPesStruct;            
            prVdecInfo->pfnVdecGetDispPic = _VDEC_AVSGetDispPic;                        
            prVdecInfo->pfnVdecSetParam = NULL;                        
            prVdecInfo->pfnVdecGetParam = NULL;                        
            prVdecInfo->pfnVdecEsFlwCtrl = NULL;                        
            prVdecInfo->pfnVdecWaitRandomAP = NULL;
            prVdecInfo->pfnVdecHandleOverFlw = _VDEC_HandleOverFlw;
            prVdecInfo->pfnVdecUpdRptr = _VDEC_AVSUpdDmxRptr;            
            prVdecInfo->pfnVdecSetWptr = _VDEC_AVSSetDmxWptr;            
            //init function            
            i4Ret = _VDEC_AVSInit(); 
            break;
#endif
        default:
            return (INT32)E_VDEC_FAIL;
            //break;
    }

    if(prVdecInfo->pfnVdecStartPts)
    {
        prVdecInfo->pfnVdecStartPts(ucEsId , prVdecEsInfo->fgEnCalPTS, prVdecEsInfo->u4StartPTS, (UINT32)&prVdecEsInfo->rPTSInfo);
    }
    return i4Ret;
}


BOOL VDEC_IsNotDisplay(UCHAR ucEsId, UINT32 u4PTS, INT32 i4TempRef,
        UINT64 u8Offset)
{
    BOOL fgNotDisplay = FALSE;
    #if 0
    UCHAR ucFbId;
    #endif
    VDEC_ES_INFO_T* prVdecEsInfo;

    ASSERT(ucEsId < MPV_MAX_ES);


    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(!prVdecEsInfo)
    {
        return TRUE;
    }

    if(!prVdecEsInfo->fgMMPlayback)
    {
        // do nothing for DTV
        return FALSE;
    }
    else if(prVdecEsInfo->fgMMPlayback && prVdecEsInfo->fgSendedEos)
    {
        return TRUE;
    }

    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I))
    {
        // when this flag is set, we still keep one frame in decoder
        // and VDEC_CheckStopStatus is called because of this keepped frame.
        // we should not check status while putting this frame to display queue.
        return TRUE;
    }

    if ((prVdecEsInfo->fgRenderFromFirstPic) &&
        (((prVdecEsInfo->u8Offset >= prVdecEsInfo->u8RenderFromPos) &&
          ((prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_1X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_2X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_4X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_8X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_16X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_32X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_64X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_128X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_256X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_512X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_1024X)))
         ||
         ((prVdecEsInfo->u8RenderFromPos != prVdecEsInfo->u8FileSize) &&
          (u8Offset <= prVdecEsInfo->u8RenderFromPos) &&
          ((prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_4X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_8X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_16X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_32X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_64X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_128X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_256X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_512X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_1024X)))
        )
       )
    {
        fgNotDisplay = TRUE;
    }
    else if ((prVdecEsInfo->fgRenderPts) &&
        (u4PTS < prVdecEsInfo->u4RenderPts))
    {
        fgNotDisplay = TRUE;
    }
    else if (prVdecEsInfo->fgRenderFromPos)
    {
        fgNotDisplay = TRUE;
    }
    else if (prVdecEsInfo->fgRenderBackStep)
    {
        // Even this frame is target frame, we don't want to display it.
        fgNotDisplay = TRUE;
        if((prVdecEsInfo->u8Offset >= prVdecEsInfo->u8RenderFromPos) &&
            (i4TempRef == prVdecEsInfo->i4RenderTempRef))
        {
            if(prVdecEsInfo->u2Disp2EmptyCnt > 0)
            {
                #if 0
                // That means we have previous display frame to empty queue.
                ucFbId = FBM_GetLatestEmptyFrameBuffer(prVdecEsInfo->ucFbgId,
                                        prVdecEsInfo->u4WaitDispTime);
                if(ucFbId != FBM_FB_ID_UNKNOWN)
                {
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);
                    LOG(5, "VDEC_IsNotDisplay fgRenderBackStep ucFbId %d Org i4TempRef %d u8Offset 0x%llx u8RenderFromPos 0x%llx\n",
                        ucFbId, i4TempRef, prVdecEsInfo->u8Offset, prVdecEsInfo->u8RenderFromPos);
                }
                else
                {
                    LOG(3, "VDEC_IsNotDisplay FBM_GetLatestEmptyFrameBuffer FBM_FB_ID_UNKNOWN\n");
                }
                #endif
            }
            else
            {
                LOG(5, "VDEC_IsNotDisplay fgRenderBackStep have no previous frame\n");
            }
        }
    }
    else if ((prVdecEsInfo->fgRenderFromPts) &&
        (u4PTS < prVdecEsInfo->u4RenderFromPts))
    {
        fgNotDisplay = TRUE;
    }
    else
    {
#ifdef ENABLE_MULTIMEDIA
        if(prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if(!prVdecEsInfo->fgSetStartPts)
            {
                prVdecEsInfo->fgSetStartPts = TRUE;
                if (STC_SetStartPts(AV_SYNC_TARGET_VDO, prVdecEsInfo->ucStcId, u4PTS) != STC_VALID)
                {
                    LOG(3, "STC_SetStartPts Invalid\n");
                }
                LOG(5, "STC_SetStartPts 0x%x\n", u4PTS);                    
                STC_StartStc();
            }
        }
        else
#endif
        {
            if(!prVdecEsInfo->fgSetStartPts)
            {
                if(u4PTS)
                {
                    LOG(5, "VDEC_IsNotDisplay fgSetStartPts, pts 0x%x\n",
                        (u4PTS - 3000));
                    //VDP_TriggerAudReceive(u4PTS);
                    STC_StopStc();
                    STC_SetStcValue((UINT32)(u4PTS - 3000));
                    STC_StartStc();
                }
                else
                {
                    LOG(3, "VDEC_IsNotDisplay fgSetStartPts no pts or zero\n");
                    //VDP_TriggerAudReceive(0);
                    //STC_SetStcValue((UINT32)(0 - 3000));
                    STC_StartStc();
                }
                prVdecEsInfo->fgSetStartPts = TRUE;
            }
        }
    }

    if(fgNotDisplay)
    {
        prVdecEsInfo->u2Disp2EmptyCnt++;
    }
    else
    {
        if(prVdecEsInfo->u4FirstDispPTS == 0)
        {
            prVdecEsInfo->u4FirstDispPTS = u4PTS;
        }
    }

    return fgNotDisplay;
}

BOOL VDEC_CheckStopStatus(UCHAR ucEsId, UINT32 u4PTS, INT32 i4TempRef)
{
    BOOL fgSkip = FALSE;
    UCHAR ucFbId;
    //UINT32 u4Temp;

    FBM_PIC_HDR_T *prFbmPicHdr = NULL;

    VDEC_INFO_T* prVdecInfo;
    VDEC_ES_INFO_T* prVdecEsInfo;

    ASSERT(ucEsId < MPV_MAX_ES);

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if((!prVdecEsInfo) || (!prVdecInfo))
    {
        return FALSE;
    }

    if(!prVdecEsInfo->fgMMPlayback)
    {
        // do nothing for DTV
        return FALSE;
    }

    if (IS_DECFLAG_SET(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I))
    {
        // when this flag is set, we still keep one frame in decoder
        // and VDEC_CheckStopStatus is called because of this keepped frame.
        // we should not check status while putting this frame to display queue.
        return FALSE;
    }


   if (prVdecEsInfo->fgRenderFromFirstPic)
    {
        prVdecEsInfo->fgRenderFromFirstPic = FALSE;
        if(prVdecInfo->pfnRenderPtsCb)
        {
            prVdecInfo->pfnRenderPtsCb(u4PTS);
            LOG(3, "VDEC_CheckStopStatus pfnRenderPtsCb fgRenderFromFirstPic\n");
        }
        if (prVdecEsInfo->fgPlayOneFrm)
        {
            prVdecEsInfo->fgPlayOneFrm = FALSE;
            if(prVdecInfo->pfnTrickPtsCb)
            {
                // We didn't found any frame before u8RenderFromPos.
                if(prVdecEsInfo->u2Disp2EmptyCnt > 0)
                {
                    // Send again with far position
                    prVdecInfo->pfnTrickPtsCb(0, TRUE);
                    LOG(5, "VDEC_CheckStopStatus pfnTrickPtsCb fgRenderFromFirstPic again\n");
                }
                else
                {
                    prVdecInfo->pfnTrickPtsCb(0, FALSE);
                    LOG(5, "VDEC_CheckStopStatus pfnTrickPtsCb fgRenderFromFirstPic finish\n");
                }
                //x_thread_delay(33);
            }
            VDEC_Pause(ucEsId);
            // switch to cmd queue (send cmd)
            x_thread_delay(1);
            fgSkip = TRUE;
        }
    }
    else if ((prVdecEsInfo->fgRenderPts) &&
        (u4PTS >= prVdecEsInfo->u4RenderPts))
    {
        prVdecEsInfo->fgRenderPts = FALSE;
        prVdecEsInfo->u4RenderPts = 0;
        if(prVdecInfo->pfnRenderPtsCb)
        {
            prVdecInfo->pfnRenderPtsCb(u4PTS);
            LOG(3, "VDEC_CheckStopStatus pfnRenderPtsCb fgRenderPts\n");
        }
        if (prVdecEsInfo->fgPlayOneFrm)
        {
            prVdecEsInfo->fgPlayOneFrm = FALSE;
            if(prVdecInfo->pfnTrickPtsCb)
            {
                prVdecInfo->pfnTrickPtsCb(0, FALSE);
                //x_thread_delay(33);
                LOG(3, "VDEC_CheckStopStatus pfnTrickPtsCb fgRenderPts\n");
            }
            VDEC_Pause(ucEsId);
            // switch to cmd queue (send cmd)
            x_thread_delay(1);
            fgSkip = TRUE;
        }
    }
    else if ((prVdecEsInfo->fgRenderFromPos) &&
        (prVdecEsInfo->u8Offset >= prVdecEsInfo->u8RenderFromPos) &&
        (i4TempRef == prVdecEsInfo->i4RenderTempRef))
    {
        prVdecEsInfo->fgRenderFromPos = FALSE;
        prVdecEsInfo->u8RenderFromPos = 0;
        prVdecEsInfo->i4RenderTempRef = 0;
        prVdecEsInfo->u2RenderDecodingOrder = 0;

        if(prVdecInfo->pfnRenderPtsCb)
        {
            prVdecInfo->pfnRenderPtsCb(u4PTS);
            LOG(3, "VDEC_CheckStopStatus fgRenderFromPos pfnRenderPtsCb u4PTS %d\n", u4PTS);
        }

        LOG(3, "VDEC_CheckStopStatus fgRenderFromPos i4TempRef %d\n", i4TempRef);
    }
    else if ((prVdecEsInfo->fgRenderBackStep) &&
        (prVdecEsInfo->u8Offset >= prVdecEsInfo->u8RenderFromPos) &&
        (i4TempRef == prVdecEsInfo->i4RenderTempRef))
    {
        prVdecEsInfo->fgRenderBackStep = FALSE;
        prVdecEsInfo->u8RenderFromPos = 0;
        prVdecEsInfo->i4RenderTempRef = 0;
        prVdecEsInfo->u2RenderDecodingOrder = 0;

        // if only decode one frame, we have to seek again (far)
        // notify swdmx to do it again
        // FixMe

        if(prVdecInfo->pfnTrickPtsCb)
        {
            // We have put two to empty that originaly should put to display.
            // That means we have previous display frame to empty queue.
            if(prVdecEsInfo->u2Disp2EmptyCnt > 1)
            {
                prVdecInfo->pfnTrickPtsCb(0, FALSE);
                LOG(5, "VDEC_CheckStopStatus pfnTrickPtsCb fgRenderBackStep finish\n");
            }
            else
            {
                // Send again with far position
                prVdecInfo->pfnTrickPtsCb(0, TRUE);
                LOG(5, "VDEC_CheckStopStatus pfnTrickPtsCb fgRenderBackStep again\n");
            }
            //x_thread_delay(33);
        }
        if(prVdecInfo->pfnRenderPtsCb)
        {
            prVdecInfo->pfnRenderPtsCb(u4PTS);
            LOG(3, "VDEC_CheckStopStatus pfnRenderPtsCb fgRenderBackStep\n");
        }
        VDEC_Pause(ucEsId);
        // switch to cmd queue (send cmd)
        x_thread_delay(1);

        LOG(3, "VDEC_CheckStopStatus fgRenderBackStep i4TempRef %d\n", i4TempRef);
    }
    else if ((prVdecEsInfo->fgRenderFromPts) &&
        (u4PTS >= prVdecEsInfo->u4RenderFromPts))
    {
        // Pull stc back to 0.5 sec before rendering.
        //STC_StopStc();
        //STC_SetStcValue(prVdecEsInfo->u4RenderFromPts - 45000);
        //STC_StartStc();

        prVdecEsInfo->fgRenderFromPts = FALSE;
        prVdecEsInfo->u4RenderFromPts = 0;

        if(prVdecInfo->pfnRenderPtsCb)
        {
            prVdecInfo->pfnRenderPtsCb(u4PTS);
            LOG(3, "VDEC_CheckStopStatus fgRenderFromPts pfnRenderPtsCb u4PTS %d\n", u4PTS);
        }

        if (prVdecEsInfo->fgAutoPause)
        {
            prVdecEsInfo->fgAutoPause = FALSE;
            VDEC_Pause(ucEsId);
        }
    }
    else if ((prVdecEsInfo->fgRenderPicCnt) &&
        (prVdecEsInfo->u4DisplayQPicCnt >= prVdecEsInfo->u4RenderPicCnt) &&
        (!prVdecEsInfo->fgSendedEos))
    {
        UCHAR ucFbStatus;
        prVdecEsInfo->fgRenderPicCnt = FALSE;
        prVdecEsInfo->u4RenderPicCnt = 0;

        prVdecEsInfo->fgSendedEos = TRUE;

        //Deliver EOS
        // error handle for not finished frame. Ex. the last one is field.
        ucFbId = FBM_GetDecFrameBuffer(prVdecEsInfo->ucFbgId);
        ucFbStatus = FBM_GetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId);
        if ((ucFbStatus == FBM_FB_STATUS_DECODE) || (ucFbStatus == FBM_FB_STATUS_FIELD_READY) ||
            (ucFbStatus == FBM_FB_STATUS_READY))
        {
            FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
        }
        // ~error
        if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
        {
            ucFbId = FBM_GetEmptyFrameBuffer(prVdecEsInfo->ucFbgId, prVdecEsInfo->u4WaitDispTime);
        }
        else
        {

            ucFbId = FBM_GetEmptyBFrameBuffer(prVdecEsInfo->ucFbgId,
                            prVdecEsInfo->u4WaitDispTime);
        }
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucFbId);
        if(prFbmPicHdr)
        {
            ucFbStatus = FBM_GetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId);
            FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_EOS_FLAG);
            FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_NOT_DISPLAY_FLAG);
            //UNUSED(_MPV_CalcPts(ucEsId, ucFbId));
            if ((ucFbStatus == FBM_FB_STATUS_DECODE) || (ucFbStatus == FBM_FB_STATUS_FIELD_READY) ||
			         (ucFbStatus == FBM_FB_STATUS_READY))
			  {
                FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);
		    }
            LOG(1, "DeEos ucFbId %d\n", ucFbId);
        }
        else
        {
            FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
            LOG(1, "DeEos prFbmPicHdr NULL");
        }

        VDEC_Pause(ucEsId);
        // switch to cmd queue (send cmd)
        x_thread_delay(1);
        //fgSkip = TRUE;
        LOG(3, "VDEC_CheckStopStatus fgRenderPicCnt\n");
    }
    else
    {
        //LOG(7, "VDEC_CheckStopStatus do nothing\n");
    }
    return fgSkip;
}


BOOL VDEC_PrsSeqHdr(ENUM_VDEC_FMT_T eFmt,
    VDEC_PES_INFO_T* prPesInfo, VDEC_SEQUENCE_DATA_T* prSeqInfo)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_PES_INFO_T* prPes;
    #ifdef CC_H264_SUPPORT
    #ifndef CC_VDEC_H264_DISABLE
    UINT32 u4Dummy;
    UINT32 u4W, u4H;
    #endif
    #endif
    //BOOL fgPowerOn;

    prVdecEsInfo = _VDEC_GetEsInfo(ES0);

    if((prPesInfo == NULL) || (prSeqInfo == NULL) || (prVdecEsInfo == NULL))
    {
        LOG(1, "PrsSeqHdr input arg error\n");
        return FALSE;
    }

    #ifdef CC_H264_SUPPORT
    #ifndef CC_VDEC_H264_DISABLE
    if(eFmt == VDEC_FMT_H264)
    {
        if((prPesInfo->ucPicType & 0xF) != SPS_NALU)
        {
            LOG(1, "PrsSeqHdr pes type %d error\n", prPesInfo->ucPicType);
            return FALSE;
        }
    }
    #endif
    #endif

    #if 0 // move to VDEC_SetGetSeqHdrOnlyBegin
    fgPowerOn = prVdecInfo->arVdecVldInfo[VLD0].fgPowerOn;
    if(!prVdecInfo->arVdecVldInfo[VLD0].fgPowerOn)
    {
        UNUSED(VDEC_PowerOn(VLD0));
    }

    if((eFmt != prVdecInfo->eCurFMT) ||
       (prVdecInfo->eCurState == VDEC_ST_STOP))
    {
        prVdecInfo->eCurFMT = eFmt;
        UNUSED(_VDEC_DecCfg(ES0));
    }
    #endif
    
    #ifdef CC_MT5395
    HalFlushDCacheMultipleLine(prPesInfo->u4FifoStart, (prPesInfo->u4FifoEnd - prPesInfo->u4FifoStart));
    #else
    HalFlushInvalidateDCache();
    #endif
    prVdecEsInfo->u4FifoStart = prPesInfo->u4FifoStart;
    prVdecEsInfo->u4FifoEnd = prPesInfo->u4FifoEnd;
    prVdecEsInfo->u4VldReadPtr = prPesInfo->u4VldReadPtr;
    prVdecEsInfo->u4PTS = prPesInfo->u4PTS;
    prVdecEsInfo->u4DTS = prPesInfo->u4DTS;
    prVdecEsInfo->ucPicType = prPesInfo->ucPicType;
    prVdecEsInfo->fgDtsValid = prPesInfo->fgDtsValid;
    prVdecEsInfo->fgSeqHdr = prPesInfo->fgSeqHdr;
    prVdecEsInfo->u8Offset = prPesInfo->u8Offset;
    prVdecEsInfo->u8OffsetLast = prPesInfo->u8Offset;
    prVdecEsInfo->u8OffsetI = prPesInfo->u8OffsetI;
    prVdecEsInfo->u2DecodingOrder = prPesInfo->u2DecodingOrder;
    prVdecEsInfo->u4TotlaTimeOffset = prPesInfo->u4TotalTimeOffset;

    //get pointer from internal decoder struct, or save in common part
    if(prVdecInfo->pfnVdecGetPesStruct)
    {
        prVdecInfo->pfnVdecGetPesStruct(ES0, (void**)&prPes);
        if(prPes == NULL)
        {
            #if 0 // move to VDEC_SetGetSeqHdrOnlyEnd
            if(!fgPowerOn)
            {
                UNUSED(VDEC_PowerOff(VLD0));
            }
            #endif
            return FALSE;
        }
        x_memcpy(prPes, prPesInfo, sizeof(VDEC_PES_INFO_T));
        if(prVdecInfo->pfnVdecHdrParse)
        {
            UNUSED(prVdecInfo->pfnVdecHdrParse(prPesInfo->ucMpvId, ES0));
        }
        prSeqInfo->u2_width = 0;
        prSeqInfo->u2_height = 0;
        #ifdef CC_H264_SUPPORT
        #ifndef CC_VDEC_H264_DISABLE
        if (prVdecInfo->pfnVdecGetParam)
        {
            prVdecInfo->pfnVdecGetParam(ES0, (UINT32)SEQ_HDR_INFO, &u4W, &u4H, &u4Dummy);
            prSeqInfo->u2_width = (UINT16)u4W;
            prSeqInfo->u2_height = (UINT16)u4H;
        }
        #endif
        #endif
    }
    else if(eFmt == VDEC_FMT_MPV)
    {
        UINT32 u4HResolution;
        UINT32 u4VResolution;
        INT32 i4PrgScan;
        MPV_ALL_ASPECT_RATIO_T e4Ratio;
        UINT32 u4FrmRat;

        //VDEC_PlayMM(ES0, TRUE);

        // lock vld
        VERIFY(x_sema_lock(prVdecInfo->arVdecVldInfo[VLD0].hVldSemaphore,
                X_SEMA_OPTION_WAIT) == OSR_OK);

        if(prVdecInfo->pfnVdecSWRst)
        {
            prVdecInfo->pfnVdecSWRst(prPesInfo->ucMpvId, ES0);
        }
        if(prVdecInfo->pfnVdecHdrParse)
        {
            UNUSED(prVdecInfo->pfnVdecHdrParse(prPesInfo->ucMpvId, ES0));
        }

        MPV_GetVideoInfo(ES0, &u4HResolution, &u4VResolution,
                            &i4PrgScan, &e4Ratio, &u4FrmRat);
        prSeqInfo->u2_width = (UINT16)u4HResolution;
        prSeqInfo->u2_height = (UINT16)u4VResolution;

        VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[VLD0].hVldSemaphore) == OSR_OK);
    }
    else if(eFmt == VDEC_FMT_WMV)
    {
        if(prVdecInfo->pfnVdecHdrParse)
        {
            UNUSED(prVdecInfo->pfnVdecHdrParse(prPesInfo->ucMpvId, ES0));
        }
        prSeqInfo->u2_width = 0;
        prSeqInfo->u2_height = 0;
    }

    #if 0 // move to VDEC_SetGetSeqHdrOnlyEnd
    if((prVdecInfo->pfnVdecStop) && (prVdecInfo->eCurState == VDEC_ST_STOP))
    {
        prVdecInfo->pfnVdecStop(ES0);
    }
    // I am not sure if we need this.
    //MPV_FlushEsmQ(ES0, FALSE);

    if(!fgPowerOn)
    {
        UNUSED(VDEC_PowerOff(VLD0));
    }
    #endif
    UNUSED(prPesInfo);
    return TRUE;
}


void _VDEC_MainLoop(void* pvArg)
{
    INT32 i4RetValue = 0;
    VDEC_DISP_PIC_INFO_T rDispInfo;
    UCHAR ucEsId;
    UCHAR ucMpvId = 0;
    //UINT32 u4DecCount = 0;
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    //PARAM_MPV_T* prMpvCounter;

    if(pvArg == NULL)
    {
        ASSERT(!pvArg);
        ucEsId = ES0;
    }
    else
    {
        ucEsId = *(UCHAR*)pvArg;
    }

    if (ucEsId >= VDEC_MAX_ES)
    {
        ASSERT(ucEsId < VDEC_MAX_ES);
        ucEsId = ES0;
    }
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    prVdecEsInfoKeep->fgThreadActive = TRUE;
    prVdecEsInfoKeep->fgThreadDestroy = FALSE;
    #ifdef VDEC_TIME_PROFILE
    _rEsAcc.u4Seconds = 0;
    _rEsAcc.u4Micros = 0;
    x_memset((void*)&prVdecEsInfo->au4DecTime, 0, sizeof(prVdecEsInfo->au4DecTime));
    prVdecEsInfo->au4DecTime[0][1] = 0xFF;
    prVdecEsInfo->au4DecTime[1][1] = 0xFF;
    prVdecEsInfo->au4DecTime[2][1] = 0xFF;
    #endif

    while (prVdecInfo->fgVDecInitiated)
    {
        if(prVdecInfo->eCurState == VDEC_ST_STOP)
        {
            _VDecReceiveCmd(ucEsId, TRUE);
            if((prVdecInfo->eCurState == VDEC_ST_PLAY) || (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME))
            {
                LOG(6, "Config\n");
                UNUSED(VDEC_PowerOn(ucEsId));

                prVdecInfo->eDecStatus = VDEC_DEC_HEADER_PARSE;
                //config, create es queue at first time
                if(_VDEC_DecCfg(ucEsId) != (INT32)E_VDEC_OK)
                {
                    prVdecInfo->ePreviousState = prVdecInfo->eCurState;
                    prVdecInfo->eCurState = VDEC_ST_STOP;
                    continue;
                }
                prVdecEsInfo->u4MaxEsCnt = 0;
                //reset
                prVdecInfo->pfnVdecSWRst(VLD0, ucEsId);
                #ifdef VDEC_TIME_PROFILE
                HAL_GetTime(&_rTimeFrmS);
                #endif
            }
        }
        else if(prVdecInfo->eCurState == VDEC_ST_IDLE)
        {
            if(prVdecEsInfo->fgMMPlayback)
            {
                // just pause, MM case
                _VDecReceiveCmd(ucEsId, TRUE);

                //clear queue, clear stored info, do notification
                _VDEC_LockFlushMutex(ucEsId);
                if (prVdecEsInfo->fgFlushEsmQ)
                {
                    LOG(1, "ES(%d) VDEC_ST_IDLE Flush\n", prVdecEsInfo->u4EsId);
                    _VDEC_FlushEs(ucEsId);
                    prVdecEsInfo->fgFlushEsmQ = FALSE;
                    _VDEC_UnlockFlushSema(ucEsId);
                }
                _VDEC_UnlockFlushMutex(ucEsId);

                if(prVdecInfo->ePreviousState != prVdecInfo->eCurState)
                {
                    //callback to playmgr after pause process finished
                    if (prVdecEsInfoKeep->pfDecNfyFct)
                    {
                        prVdecEsInfoKeep->pfDecNfyFct(
                            prVdecEsInfoKeep->pvDecNfyTag,
                            VDEC_COND_CTRL_DONE,
                            1, 0);
                    }
                }
            }
            else
            {
                // dtv ts, eat es data
                ucMpvId = _VDecReceiveEs(ucEsId);
                _VDecReceiveCmd(ucEsId, FALSE);
            }
        }
        else
        {
            _VDecReceiveCmd(ucEsId, FALSE);
        }

        switch(prVdecInfo->eCurState)
        {
            case VDEC_ST_PLAY:
            case VDEC_ST_PLAY_I_FRAME:
            {
                if(prVdecInfo->pfnVdecDecode)
                {
                    LOG(6, "Decode\n");
                    #ifdef VDEC_TIME_PROFILE
                    HAL_GetTime(&_rTimeEsS);
                    #endif

                    ucMpvId = _VDecReceiveEs(ucEsId);

                    #ifdef VDEC_TIME_PROFILE
                    HAL_GetTime(&_rTimeEsE);
                    HAL_GetDeltaTime(&_rTimeEsDt, &_rTimeEsS, &_rTimeEsE);
                    //LOG(1, "WEs %u.%06us\n", _rTimeEsDt.u4Seconds, _rTimeEsDt.u4Micros);
                    #endif

                    if(ucMpvId >= VDEC_MAX_VLD)
                    {
                        _VDEC_DeliverEos(ucEsId);
                        break;
                    }
                    // lock vld
                    VERIFY(x_sema_lock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore,
                    X_SEMA_OPTION_WAIT) == OSR_OK);

                    //reset on each pic, we do it in decode.
                    //prVdecInfo->pfnVdecSWRst(ucMpvId, ucEsId);

                    i4RetValue = prVdecInfo->pfnVdecDecode(ucMpvId, ucEsId);

                    // TODO:
                    // check notify status if OK and Drop

                    #ifdef __MODEL_slt__
                    /*
                        if (prEsInfo->u4PTS == prEsInfo->u4EndPts)
                        {
                            prEsInfo->fgCountPicNs = FALSE;
                            prEsInfo->fgCountPicFinish = TRUE;
                            LOG(1, "SLT: stop counting pictures\n");
                        }
                        */
                    #endif

                     //update rptr
                    if(prVdecInfo->pfnVdecUpdRptr)
                    {
                        prVdecInfo->pfnVdecUpdRptr(ucEsId);
                    }

                    _VDEC_DeliverEos(ucEsId);

                    VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore) == OSR_OK);

                    if((i4RetValue == (INT32)E_VDEC_NOT_SUPPORT) &&
                                (prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT))
                    {
                        if (prVdecEsInfoKeep->pfDecNfyFct)    //enable after mw add this handler
                        {
                            LOG(3, "Notify Status change, codec not support\n");
                            prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                                VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                            prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
                            prVdecInfo->eCurState = VDEC_ST_IDLE;
                        }
                    }
                }
                else
                {
                    if(prVdecInfo->eDecStatus == VDEC_DEC_HEADER_PARSE)
                    {
                        LOG(9, "VDEC_DEC_HEADER_PARSE\n");
                        #ifdef VDEC_TIME_PROFILE
                        HAL_GetTime(&_rTimeEsS);
                        #endif
                        #ifdef VDP_RACE_LOG
                          VDP_FrcRacingLog('E');
                        #endif

                        ucMpvId = _VDecReceiveEs(ucEsId);
                        #ifdef VDP_RACE_LOG
                          VDP_FrcRacingLog('O');
                        #endif

                        #ifdef VDEC_TIME_PROFILE
                        HAL_GetTime(&_rTimeEsE);
                        HAL_GetDeltaTime(&_rTimeEsDt, &_rTimeEsS, &_rTimeEsE);
                        _rEsAcc.u4Seconds += _rTimeEsDt.u4Seconds;
                        _rEsAcc.u4Micros  += _rTimeEsDt.u4Micros;
                        if (_rEsAcc.u4Micros >= 1000000)
                        {
                            _rEsAcc.u4Seconds += _rEsAcc.u4Micros / 1000000;
                            _rEsAcc.u4Micros  %= 1000000;
                        }
                        #endif

                        if(ucMpvId >= VDEC_MAX_VLD)
                        {
                            _VDEC_DeliverEos(ucEsId);
                            break;
                        }
                        // lock vld
                        //VERIFY(x_sema_lock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore,
                        //X_SEMA_OPTION_WAIT) == OSR_OK);

                        //check if need re-config according to id in PesInfo?
                        i4RetValue = prVdecInfo->pfnVdecHdrParse(ucMpvId, ucEsId);

                        if((i4RetValue == (INT32)E_VDEC_OK) ||
                            (i4RetValue == (INT32)E_VDEC_OK_RES_CHG))            //parse done
                        {
                            if(prVdecEsInfo->fgFmtDectFinish)
                            {
                                prVdecInfo->eDecStatus = VDEC_DEC_TRIGGER;
                            }
                            else
                            {
                                if(VDEC_ChkCodecCap(ucEsId, prVdecInfo->eCurFMT))
                                {
                                    prVdecInfo->eDecStatus = VDEC_DEC_TRIGGER;
                                }
                                else
                                {
                                    prVdecInfo->eDecStatus = VDEC_DEC_HEADER_PARSE;
                                    if(prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT)
                                    {
                                        LOG(3, "Notify Status change, codec not support\n");
                                        prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                                            VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                                        prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
                                        prVdecInfo->eCurState = VDEC_ST_IDLE;
                                    }
                                }
                                prVdecEsInfo->fgFmtDectFinish = TRUE;
                            }
                        }
                        #if 0  //request by scart out feature, but cause DTV aspect ratio change problem, callback timing earlier than vdp prepared
                        else if(i4RetValue == (INT32)E_VDEC_OK_RES_CHG)
                        {
                            prVdecInfo->eDecStatus = VDEC_DEC_TRIGGER;
                            prVdecEsInfo->fgFmtDectFinish = TRUE;
                            //DTV00036439
                            if (prVdecEsInfoKeep->pfDecNfyFct)
                            {
                                prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_RESOLUTION_CHG, 0, 0);
                            }
                        }
                        #endif
                        else if(i4RetValue == (INT32)E_VDEC_FAIL)    //error
                        {
                            prVdecInfo->eDecStatus = VDEC_DEC_DROP;
                        }
                        else if(i4RetValue == (INT32)E_VDEC_BS_END)
                        {
                            _VDEC_DeliverEos(ucEsId);
                            if(prVdecInfo->pfErrNotify)
                            {
                                prVdecInfo->pfErrNotify((UINT32)VDEC_EVT_DEC_BS_END, (UINT32)&rDispInfo, 0, 0);
                            }
                            if(prVdecInfo->pfnVdecStop)
                            {
                                prVdecInfo->pfnVdecStop(ucEsId);
                            }
                            prVdecInfo->ePreviousState = prVdecInfo->eCurState;
                            prVdecInfo->eCurState = VDEC_ST_STOP;
                            _VDEC_FlushEs(ucEsId);
                            prVdecEsInfo->fgFmtDectFinish = TRUE;
                            //VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore) == OSR_OK);
                        }
                        else
                        {
                            _VDEC_DeliverEos(ucEsId);
                            if((i4RetValue == (INT32)E_VDEC_NOT_SUPPORT) &&
                                (prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT))
                            {
                                if (prVdecEsInfoKeep->pfDecNfyFct)    //enable after mw add this handler
                                {
                                    LOG(3, "Notify Status change, codec not support\n");
                                    prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                                        VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                                    prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
                                    prVdecInfo->eCurState = VDEC_ST_IDLE;
                                }
                                prVdecEsInfo->fgFmtDectFinish = TRUE;
                            }
                            prVdecInfo->eDecStatus = VDEC_DEC_HEADER_PARSE;
                        }
                        //update rptr
                        // may need another one for H264
                        //if(prVdecInfo->pfnVdecUpdRptr)
                        //{
                            //prVdecInfo->pfnVdecUpdRptr(ucEsId);
                        //}
                    }
                    else if(prVdecInfo->eDecStatus == VDEC_DEC_TRIGGER)
                    {
                        //LOG(6, "Pic %d Decoding\n", u4DecCount++);
                        if(prVdecEsInfoKeep->fgVdecMidEnable && prVdecInfo->pfnVdecGetDispPic)
                        {
                            if((prVdecInfo->eCurFMT == VDEC_FMT_H264) ||
                                (prVdecInfo->eCurFMT == VDEC_FMT_H264VER))
                            {
                                prVdecInfo->pfnVdecGetDispPic(ucEsId, (void*)&rDispInfo);
                                if(!_VDEC_MidSetting(ucEsId, &rDispInfo))
                                {
                                    LOG(3, "VdecMid Fail~\n");
                                }
                            }
                        }
                        i4RetValue = prVdecInfo->pfnVdecDecStart(ucMpvId, ucEsId);
                        if(i4RetValue == (INT32)E_VDEC_OK)
                        {
                            prVdecInfo->eDecStatus = VDEC_DEC_WAIT_DEC_FINISH;
                        }
                        else    //error
                        {
                            prVdecInfo->eDecStatus = VDEC_DEC_DROP;
                        }
                    }
                    //combine trigger decode and wait finish in one stage
                    /*else */if(prVdecInfo->eDecStatus == VDEC_DEC_WAIT_DEC_FINISH)
                    {
                        i4RetValue = prVdecInfo->pfnVdecDecFin(ucMpvId, ucEsId);

                        _VdecTimeProfile(ucEsId);

                        if(i4RetValue == (INT32)E_VDEC_OK)
                        {
                            //for emulation, get output picture and do notification
                            if(prVdecInfo->pfnVdecGetDispPic)
                            {
                                prVdecInfo->pfnVdecGetDispPic(ucEsId, (void*)&rDispInfo);
                                if(rDispInfo.pvYAddr != NULL)
                                {
                                    //callback, compare golden
                                    if(prVdecInfo->pfErrNotify)
                                    {
                                        prVdecInfo->pfErrNotify((UINT32)VDEC_EVT_DISP_READY, (UINT32)&rDispInfo, 0, 0);
                                    }
                                }
                            }

                            _VdecCheckStatus(ucEsId, TRUE);

                             //update rptr
                            if(prVdecInfo->pfnVdecUpdRptr)
                            {
                                prVdecInfo->pfnVdecUpdRptr(ucEsId);
                            }

                            prVdecInfo->eDecStatus = VDEC_DEC_HEADER_PARSE;
                            //reset on each pic
                            prVdecInfo->pfnVdecSWRst(ucMpvId, ucEsId);

                            _VDEC_DeliverEos(ucEsId);

                            //VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore) == OSR_OK);
                        }
                        else if(i4RetValue == (INT32) E_VDEC_FAIL)
                        {
                            prVdecInfo->eDecStatus = VDEC_DEC_DROP;
                        }
                        else if (i4RetValue == E_VDEC_RETRIGGER)
                        {
                            prVdecInfo->eDecStatus = VDEC_DEC_TRIGGER;
                        }
                        else
                        {
                            prVdecInfo->eDecStatus = VDEC_DEC_HEADER_PARSE;
                            _VDEC_DeliverEos(ucEsId);
                        }
                    }
                    else if(prVdecInfo->eDecStatus == VDEC_DEC_DROP)
                    {
                        if(prVdecInfo->pfnVdecDecDrop)
                        {
                            prVdecInfo->pfnVdecDecDrop(ucEsId);
                        }

                        _VdecCheckStatus(ucEsId, FALSE);

                        //change state
                        prVdecInfo->eDecStatus = VDEC_DEC_HEADER_PARSE;
                        //reset on each pic
                        prVdecInfo->pfnVdecSWRst(ucMpvId, ucEsId);

                        _VDEC_DeliverEos(ucEsId);

                        //VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore) == OSR_OK);
                    }

                }
                break;
            }

            case VDEC_ST_STOP:
                //clear queue, clear stored info, do notification
                //dmx flush Q
                _VDEC_LockFlushMutex(ucEsId);
                if (prVdecEsInfo->fgFlushEsmQ)
                {
                    LOG(1, "ES(%d) _VDECStop Flush\n", prVdecEsInfo->u4EsId);
                    _VDEC_FlushEs(ucEsId);
                    prVdecEsInfo->fgFlushEsmQ = FALSE;
                    _VDEC_UnlockFlushSema(ucEsId);
                }
                _VDEC_UnlockFlushMutex(ucEsId);

                if(prVdecInfo->ePreviousState != prVdecInfo->eCurState)
                {   // to prevent clean esinfo twice. we may clean the setting
                    // from mw.
                    if(prVdecInfo->pfnVdecStop)
                    {
                        prVdecInfo->pfnVdecStop(ucEsId);
                    }
                    _VDEC_FlushEs(ucEsId);
                    //callback to mw after stop process finished, cr: DTV00125552
                    // move down,
                    // DTV00212525,[Pre-Test]Always show 'Retrieving Data' after change channel from specail TS
                    // MW will call stop even play is fail
                    /*
                    if (prVdecEsInfoKeep->pfDecNfyFct)
                    {
                        prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_CTRL_DONE,
                            //(UINT32)VID_DEC_CTRL_STOP, 0);
                            1, 0); //cr: DTV00126433
                    }
                    */
                    //h.264 will set EsInfo->fbgId as well, should clear this or
                    //mpeg2 won't create new fbg, cause display not ready
                    _VDEC_LockFlushMutex(ucEsId);

                    if (prVdecEsInfo->fgFlushEsmQ)
                    {
                        _VDEC_FlushEs(ucEsId);
                        prVdecEsInfo->fgFlushEsmQ = FALSE;
                        _VDEC_UnlockFlushSema(ucEsId);
                    }
                    _VDEC_ClearEsInfo(ucEsId);

                    _VDEC_UnlockFlushMutex(ucEsId);
                    UNUSED(VDEC_PowerOff(ucEsId));
                }

                // DTV00212525,[Pre-Test]Always show 'Retrieving Data' after change channel from specail TS
                // MW will call stop even play is fail. even that we have to notify control done
                if(prVdecInfo->u4PreviousStopId != prVdecInfo->u4CurStopId)
                {
                    //callback to mw after stop process finished, cr: DTV00125552
                    if (prVdecEsInfoKeep->pfDecNfyFct)
                    {
                        prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_CTRL_DONE,
                            //(UINT32)VID_DEC_CTRL_STOP, 0);
                            1, 0); //cr: DTV00126433
                    }
                }

                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_UNKNOWN;
                prVdecInfo->ePreviousState = prVdecInfo->eCurState;
                break;
            case VDEC_ST_IDLE:
                // will eat pes data
                break;
            default:
                break;
        }
    }

    {// finish thread
        prVdecInfo->arVdecEsInfoKeep[ucEsId].fgThreadActive = FALSE;

        // if all threads are destroy, delete vdec semaphore
        {
            UCHAR ucIdx;
            UCHAR ucAllDestory;

            ucAllDestory = 1;
            for (ucIdx = 0; ucIdx < VDEC_MAX_ES; ucIdx++)
            {
                if (prVdecInfo->arVdecEsInfoKeep[ucEsId].fgThreadActive)
                {
                    ucAllDestory = 0;
                    ucIdx = VDEC_MAX_ES;
                }
            }

            if (ucAllDestory)
            {
                for (ucIdx = 0; ucIdx < VDEC_MAX_VLD; ucIdx++)
                {
                    _MPV_VldPower(ucIdx, OFF);

                    _MPV_IsrStop();

                    _MPV_DeleteDecSema(ucIdx);

                    VERIFY(x_timer_stop(prVdecInfo->arVdecEsInfoKeep[ucIdx].hDataTimer) == OSR_OK);

                    VERIFY(x_timer_stop(prVdecInfo->arVdecEsInfoKeep[ucIdx].hCodecChkTimer) == OSR_OK);

                    VERIFY (x_sema_delete(prVdecInfo->arVdecVldInfo[ucIdx].hVldSemaphore) == OSR_OK);
                }
                prVdecInfo->fgVDecInitiated = FALSE;
            }
        }

        _VDEC_DeleteEsmSema(ucEsId);
    }
    UNUSED(pvArg);
    x_thread_exit();
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL _VDEC_MidSetting(UCHAR ucEsId, VDEC_DISP_PIC_INFO_T* prDispInfo)
{
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    UINT32 u4WorkingBuffer, u4WorkingSize, u4YSize;
    MID_AGENT_ID_T ePPAgentId;
    BOOL fgRet = TRUE;

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if((prVdecInfo == NULL) || (prVdecEsInfo == NULL) ||
        (prVdecEsInfoKeep == NULL) ||
        (prDispInfo->pvYAddr == NULL) || (prDispInfo->pvCAddr == NULL))
    {
        return FALSE;
    }
    if(!prVdecEsInfoKeep->fgVdecMidEnable)
    {
        return TRUE;
    }

    #if defined(CC_MT5360)
    ePPAgentId = MID_AGENT_VDEC_MISC;
    #else
    ePPAgentId = MID_AGENT_JPEG;
    #endif

    switch(prVdecInfo->eCurFMT)
    {
        case VDEC_FMT_H264VER:
        case VDEC_FMT_H264:
            if(prDispInfo->pvMVAddr == NULL)
            {
                return FALSE;
            }
            u4YSize = (UINT32)prDispInfo->pvCAddr - (UINT32)prDispInfo->pvYAddr;
            fgRet &= MID_SetRegionEx(0, ePPAgentId,
                (UINT32)prDispInfo->pvYAddr,
                (UINT32)prDispInfo->pvMVAddr + (UINT32)(u4YSize >> 2),
                MID_FLAG_INVERSE);
            fgRet &= MID_EnableRegionProtect(0);
            fgRet &= MID_SetRegion(1, MID_AGENT_CPU,
                (UINT32)prDispInfo->pvCAddr + (UINT32)(u4YSize>>1),
                (UINT32)prDispInfo->pvMVAddr);
            fgRet &= MID_EnableRegionProtect(1);
            break;

        case VDEC_FMT_MPV:
            fgRet &= MID_SetRegionEx(0, MID_AGENT_MPEG1,
                (UINT32)prDispInfo->pvYAddr,
                (UINT32)prDispInfo->pvCAddr + (UINT32)(prDispInfo->u4YSize >> 1),
                MID_FLAG_INVERSE);
            fgRet &= MID_EnableRegionProtect(0);
            fgRet &= MID_SetRegionEx(1, ePPAgentId,
                (UINT32)prDispInfo->pvYDbkAddr,
                (UINT32)prDispInfo->pvCDbkAddr + (UINT32)(prDispInfo->u4YSize >> 1),
                MID_FLAG_INVERSE);
            fgRet &= MID_EnableRegionProtect(1);
            break;

        case VDEC_FMT_WMV:
        case VDEC_FMT_MP4:
        case VDEC_FMT_RV:  // !!! megaa 20090516
        case VDEC_FMT_AVS:
            //frame buffer
            fgRet &= MID_SetRegionEx(0, MID_AGENT_MPEG1,
                (UINT32)prDispInfo->pvYAddr,
                (UINT32)prDispInfo->pvCAddr + (UINT32)(prDispInfo->u4YSize >> 1),
                MID_FLAG_INVERSE);
            fgRet &= MID_EnableRegionProtect(0);
            //ac, dc buffer
            FBM_GetWorkingBuffer(prVdecEsInfo->ucFbgId, &u4WorkingBuffer, &u4WorkingSize);
            fgRet &= MID_SetRegionEx(1, ePPAgentId, u4WorkingBuffer,
                (u4WorkingBuffer + u4WorkingSize), MID_FLAG_INVERSE);
            fgRet &= MID_EnableRegionProtect(1);
            break;

        default:
            return FALSE;
            //break;
    }

    return fgRet;
}


void VDEC_FbmMidTest(UCHAR ucEsId, UINT8 u1Mode, BOOL fgDBKDisable)
{
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(prVdecEsInfoKeep)
    {
        prVdecEsInfoKeep->u1FbmMidMode = u1Mode;
        prVdecEsInfoKeep->fgMidDBKDisable = fgDBKDisable;
    }
}


void VDEC_AVSyncTest(UCHAR ucEsId, UINT8 u1Mode)
{
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(prVdecEsInfoKeep)
    {
        prVdecEsInfoKeep->u1AvSyncTestMode = u1Mode;
        _fgWaitFbgChg = TRUE;
    }
}


void VDEC_FbgChgHdlr(UCHAR ucEsId, UCHAR ucFbgId, UINT32 u4FBStart, UINT32 u4FBEnd)
{
    UINT8 i;
    DMX_PID_T rPid;
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    MID_AGENT_ID_T eAgentId, ePPAgentId, eDmxAgentId = MID_AGENT_DEMUX;
    FBM_POOL_T* prFbmPoolDMX;
    UINT32 u4WorkingBuffer, u4WorkingSize;
    ENUM_VDEC_FMT_T eVDecFmt;
    BOOL fgRet = TRUE;

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if((prVdecInfo == NULL) || (prVdecEsInfo == NULL) ||
        (prVdecEsInfoKeep == NULL))
    {
        return;
    }

    if(ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        if(prVdecEsInfoKeep->u1FbmMidMode > 0)
        {
            #if defined(CC_MT5360)
            ePPAgentId = MID_AGENT_VDEC_MISC;
            #else
            ePPAgentId = MID_AGENT_JPEG;
            #endif

            eVDecFmt = prVdecInfo->eCurFMT;
            FBM_GetWorkingBuffer(ucFbgId, &u4WorkingBuffer, &u4WorkingSize);

            if((eVDecFmt == VDEC_FMT_H264) ||
                (eVDecFmt == VDEC_FMT_H264VER))
            {
                eAgentId = ePPAgentId;
            }
            else
            {
                eAgentId = MID_AGENT_MPEG1;
                if((eVDecFmt == VDEC_FMT_WMV) || (eVDecFmt == VDEC_FMT_MP4) || (eVDecFmt == VDEC_FMT_RV))  // !!! megaa 20090516
                {
                    u4FBEnd = u4WorkingBuffer;
                }
            }

            //protect mode
            if((prVdecEsInfoKeep->u1FbmMidMode == 2) ||
                (prVdecEsInfoKeep->u1FbmMidMode == 3))    //fbm corrupt prevention
            {
                //region 0, for dmx
                if(prVdecEsInfoKeep->u1FbmMidMode == 2)
                {
                    if(prVdecEsInfo->fgMMPlayback)
                    {
                        eDmxAgentId = MID_AGENT_DDI;
                    }
                    prFbmPoolDMX = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
                    if((prFbmPoolDMX == NULL) || (prFbmPoolDMX->u4Addr == 0))
                    {
                        return;
                    }
                    fgRet &= MID_SetRegion(0, eDmxAgentId, prFbmPoolDMX->u4Addr,
                        prFbmPoolDMX->u4Addr + prFbmPoolDMX->u4Size);
                    fgRet &= MID_EnableRegionProtect(0);
                }
                //region 1, fbm
                if((eVDecFmt != VDEC_FMT_MPV) || prVdecEsInfoKeep->fgMidDBKDisable)
                {
                    fgRet &= MID_SetRegion(1, eAgentId, u4FBStart, u4FBEnd);
                    fgRet &= MID_EnableRegionProtect(1);
                }
                //region 2, ac, dc buffer
                if((eVDecFmt == VDEC_FMT_WMV) || (eVDecFmt == VDEC_FMT_MP4) || (eVDecFmt == VDEC_FMT_MP4))  // !!! megaa 20090516
                {
                    fgRet &= MID_SetRegion(2, ePPAgentId, u4WorkingBuffer,
                        u4WorkingBuffer + u4WorkingSize);
                    fgRet &= MID_EnableRegionProtect(2);
                }
            }
            if(fgRet)
            {
                LOG(2, "Vdec Memory Protect in FBM done~\n");
            }
            else
            {
                LOG(2, "Vdec Memory Protect in FBM fail !\n");
            }
        }
    }

    //=== av sync free pid mode
    if(prVdecEsInfoKeep)
    {
        // free pid
        x_memset((void*)&rPid, 0, sizeof(rPid));
        if((prVdecEsInfoKeep->u1AvSyncTestMode & 0x4) > 0)
        {
            for (i = 0; i < DMX_NUM_PID_INDEX; i++)
            {
                if (!DMX_GetPid(i, (UINT32)(DMX_PID_FLAG_PCR | DMX_PID_FLAG_VALID), &rPid))
                {
                    ASSERT(0);
                }

                if (rPid.fgEnable)
                {
                    if ((rPid.ePcrMode == DMX_PCR_MODE_OLD) || (rPid.ePcrMode == DMX_PCR_MODE_NEW))
                    {
                        DMX_FreePid(i);
                        break;
                    }
                }
            }
        }
        prVdecEsInfoKeep->u4AvSyncCnt = 0x1064;
        _fgWaitFbgChg = FALSE;
    }
}


void VDEC_FbgReleaseHdlr()
{
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ES0);

    if((prVdecEsInfoKeep != NULL) && (prVdecEsInfoKeep->u1FbmMidMode > 0))
    {
        UNUSED(MID_Reset());
    }
}


void VDEC_GenPTS(UCHAR ucFbgId, UCHAR ucFbId)
{
    FBM_PIC_HDR_T* prFbmPicHdr;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    BOOL fgChgPTS = FALSE;
    UINT32 u4PrevCnt = 0;

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ES0);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);

    if((prVdecEsInfoKeep != NULL) && (prFbmPicHdr != NULL))
    {
        if((prVdecEsInfoKeep->u1AvSyncTestMode & 0x3) == 1)   //only apply fake pts on 1st frame
        {
            if(!_fgWaitFbgChg)
            {
                fgChgPTS = TRUE;
                _fgWaitFbgChg = TRUE;
            }
        }
        else if((prVdecEsInfoKeep->u1AvSyncTestMode & 0x3) > 0)   // 2 or 3, apply fake pts randomly
        {
            fgChgPTS = TRUE;
        }
        //Cnt == bit[15:12], 0:normal/1:fake PTS, bit[11:8], 0:pts+/1:pts-, bit[7:0] counter
        if(fgChgPTS)
        {
            if((prVdecEsInfoKeep->u4AvSyncCnt & 0xF000) > 0)    // fake pts, else original pts
            {
                if((prVdecEsInfoKeep->u4AvSyncCnt & 0xF00) > 0)
                {
                    prFbmPicHdr->u4PTS -= (90000 * 5);  // set 5 sec precede
                }
                else
                {
                    prFbmPicHdr->u4PTS += (90000 * 5);  // set 5 sec behind
                }
            }
            prVdecEsInfoKeep->u4AvSyncCnt --;
            if((prVdecEsInfoKeep->u4AvSyncCnt & 0xFF) == 0) //switch, 0x1000->0x0100->0x1100->0x0000->0x1000
            {
                u4PrevCnt = prVdecEsInfoKeep->u4AvSyncCnt;
                prVdecEsInfoKeep->u4AvSyncCnt = 0;
                if((u4PrevCnt & 0xF000) == 0)
                {
                    prVdecEsInfoKeep->u4AvSyncCnt |= 0x1000;
                    prVdecEsInfoKeep->u4AvSyncCnt |= (u4PrevCnt & 0xF00);
                }
                else if((u4PrevCnt & 0xF00) == 0)
                {
                    prVdecEsInfoKeep->u4AvSyncCnt |= 0x100;
                }
                prVdecEsInfoKeep->u4AvSyncCnt |= 0x64;
            }
        }
    }

}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



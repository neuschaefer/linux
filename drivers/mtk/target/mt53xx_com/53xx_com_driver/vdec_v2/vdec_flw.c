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
 * $Author: xiaolei.li $
 * $Date: 2012/10/18 $
 * $RCSfile: vdec_flw.c,v $
 * $Revision: #9 $
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
#ifndef CC_VDEC_RAW_DISABLE
#include "vdec_raw.h"
#endif
#ifndef CC_VDEC_MPV_DISABLE
#include "mpv_decode.h"
#endif
#ifndef CC_VDEC_VP6_DISABLE
#include "vdec_vp6api.h"
#endif
#ifndef CC_VDEC_VP8_DISABLE
#include "vdec_vp8api.h"
#endif
#endif
#include "mpv_if.h"
#include "stc_drvif.h"
#include "fbm_drvif.h"
#include "x_timer.h"
#include "x_mid.h"
#include "x_bim.h"
#include "imgrz_if.h"
#include "gfx_if.h"
#ifdef VDEC_VENC_PATH
#include "gfx_drvif.h"
#include "imgrz_drvif.h"
#include "venc_drvif.h"
#include "venc_util_drvif.h"
#endif
#include "dmx_if.h"
#ifdef CC_MT5392B
#include "vdp_drvif.h"
#endif
#ifndef CC_VDEC_AVS_DISABLE
#include "vdec_avsapi.h"
#endif
#ifdef CC_REALD_3D_SUPPORT
#include "gfx_drvif.h"
#endif
#include "x_chip_id.h"
LINT_EXT_HEADER_END

#ifdef VDEC_TIME_PROFILE
#include <string.h>
#endif

#ifdef __MODEL_slt__
EXTERN BOOL fgSltWatchPts;
EXTERN UINT32 u4SltPts;
EXTERN BOOL fgSltNotSendPic;
#endif


#ifndef LINUX_TURNKEY_SOLUTION
//#define VDEC_THUMBNAIL_ADDR_CHECK
#endif

EXTERN UCHAR VDP_FrcGetLockFrameBuffer(UCHAR ucVdpId);
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
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgWaitFbgChg = FALSE;

#ifdef VDEC_VENC_PATH
typedef struct
{
    BOOL         fgOn;
    VENC_HANDLE  hVEnc;
    VENC_BUFMGR_BASE_T * ptFrmBuffMgr;
    #ifdef VENC_FRAME_CTRL_BY_DRV
    VENC_FILTER_T *ptFilter;
    #endif
    //venc input source fmt
    UINT32 u4Pitch;
    UINT32 u4Width;
    UINT32 u4Height;
    
} VDEC_2_VENC_T; 

static VDEC_2_VENC_T _arVdec2VencFlow[VDEC_MAX_ES];
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _VdecGetThumbnail(UCHAR ucEsId);
static void _VdecFlushDPB(UCHAR ucEsId);
#ifdef VDEC_TIME_PROFILE
static void _VDEC_CalPicSize(UCHAR ucEsId);

extern INT32 T32_HostExec(INT32 i4TargetFlag, const CHAR *szFormat, ...);

enum
{
    E_T32_NOP,
    E_T32_EXEC,
    E_T32_RESTART,
    E_T32_EXIT,
    E_T32_EXEC_RELOAD_SYM,
    E_T32_DETECT_HOST,
    E_T32_END
};

#endif
UCHAR _VDecReceiveEs(UCHAR ucEsId);
void _VDecReceiveCmd(UCHAR ucEsId, BOOL bIsBlock);
void _VDEC_DeliverEos(UCHAR ucEsId);
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef VDEC_TIME_PROFILE
//static HAL_TIME_T _rVdecStart[MAX_SET], _rVdecMaxDt[MAX_SET], _rVdecCurDt[MAX_SET];
// 0:get msg q, 1:hdr parsing, 2:decoding time

#define INFO_BUF_SIZE       (7*1024*1024)
#define TMP_BUF_SIZE        (256)
#define WORDS_PER_LINE   (32)
#define MAX_LINE               (UINT32)(INFO_BUF_SIZE/WORDS_PER_LINE)

#define NEW_A_LINE      \
    { \
        u4CurTotalLines++;\
        if ( u4CurTotalLines >  MAX_LINE) \
        {\
            _VdecTimeProfileSaveToFile(ucEsId);\
            u4CurSaveFileCnt++;\
            u4CurTotalLines = 0x1;\
            x_memset(acProfieInfo, 0, INFO_BUF_SIZE);\
        }\
        else \
        {\
            strcat(acProfieInfo, "\n"); \
        }\
    }

static HAL_TIME_T _rTimeEsS, _rTimeEsE, _rTimeEsDt, _rTimeFrmS, _rTimeFrmE, _rTimeFrmDt, _rEsAcc;

char acProfieInfo[INFO_BUF_SIZE];
char acProfileTmp[TMP_BUF_SIZE];
char acProfileSaveDir[512];
UINT32 u4CurTotalLines;
UINT32 u4CurSaveFileCnt;


static void _VdecTimeProfileSaveToFile(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    CHAR acData[1024];
    INT32 i = 0, iLen = 0;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    x_memset(&(acData[0]), 0x0, 1024);
    x_memset(prVdecEsInfo->ucAutoPlayCurFileDir, 0x0, sizeof(prVdecEsInfo->ucAutoPlayCurFileDir));
    x_memset(prVdecEsInfo->ucAutoPlayCurFileName, 0x0, sizeof(prVdecEsInfo->ucAutoPlayCurFileName));

#if 0
#if defined(CC_MT5395)
    HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvYAddr, (prDispInfo->pvYAddr + prDispInfo->u4YSize));
#endif
#if defined(CC_MT5365)
    HalFlushInvalidateDCache();
#endif
#else
    HalFlushInvalidateDCache();
#endif
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\model.name\" 0x%08x /ny", &(acData[0])));
#if 0
#if defined(CC_MT5395)
    HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
    HalFlushInvalidateDCache();
#endif
#else
    HalFlushInvalidateDCache();
#endif

    iLen = strlen((CHAR *)acData);

    for ( i = iLen; i > 0; i-- )
    {
        if ( acData[i] == '/' )
        {
            break;
        }
    }

    x_strncpy(prVdecEsInfo->ucAutoPlayCurFileDir, acData + 16, i - 16); //Ignore the pre string "mnt/usb/Mass-000/"
    x_strncpy(prVdecEsInfo->ucAutoPlayCurFileName, acData + i + 1, iLen - i - 1);

    for ( i = 0; i < strlen((CHAR *)prVdecEsInfo->ucAutoPlayCurFileDir); i++ )
    {
        if ( prVdecEsInfo->ucAutoPlayCurFileDir[i] == '/' )
        {
            prVdecEsInfo->ucAutoPlayCurFileDir[i] = '\\';
        }
    }

#ifdef VDEC_TIME_PROFILE_SAVE_TO_CURRET
    {
        HalFlushInvalidateDCache();
        if ( prVdecEsInfo->ucAutoPlayCurFileName[0] == 0x0 )
        {
            UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%sTEST_Data_%d_%d.model\" 0x%08x--0x%08x",
                                "D:\\", prVdecEsInfo->u4FrmFldCnt, u4CurSaveFileCnt,
                                (UINT32)&(acProfieInfo[0]), (UINT32)((UINT32)(&(acProfieInfo[0])) + strlen((CHAR *)acProfieInfo) - 1)));
        }
        else
        {
            sprintf(acProfileSaveDir, "%s%s\\", "D:\\Model_Design_Bitstreams\\", (CHAR *)prVdecEsInfo->ucAutoPlayCurFileDir);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_%d.model\" 0x%08x--0x%08x",
                                (CHAR *)acProfileSaveDir, (CHAR *)prVdecEsInfo->ucAutoPlayCurFileName, prVdecEsInfo->u4FrmFldCnt, u4CurSaveFileCnt,
                                (UINT32)&(acProfieInfo[0]), (UINT32)((UINT32)(&(acProfieInfo[0])) + strlen((CHAR *)acProfieInfo) - 1)));
        }
    }
#else
    {
        HalFlushInvalidateDCache();
        if ( prVdecEsInfo->ucAutoPlayCurFileName[0] == 0x0 )
        {
            UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%sTEST_Data_%d_%d.model\" 0x%08x--0x%08x",
                                "D:\\", prVdecEsInfo->u4FrmFldCnt, u4CurSaveFileCnt,
                                (UINT32)&(acProfieInfo[0]), (UINT32)((UINT32)(&(acProfieInfo[0])) + strlen((CHAR *)acProfieInfo) - 1)));
        }
        else
        {
            UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s.model\" 0x%08x--0x%08x",
                                "D:\\Model_Design_Bitstreams\\", (CHAR *)prVdecEsInfo->ucAutoPlayCurFileName,
                                (UINT32)&(acProfieInfo[0]), (UINT32)((UINT32)(&(acProfieInfo[0])) + strlen((CHAR *)acProfieInfo) - 1)));
        }
    }
#endif

    return;
}

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
    
		// send AFD
		VDEC_SendAFDInfo(ucEsId);
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
            if(VDEC_ChkCodecCap(ucEsId, prVdecEsInfoKeep->eCurFMT))
            {
                LOG(1, "ES(%d) Notify Status change, decode normal\n", ucEsId);
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_NORMAL;
            }
            else
            {
                LOG(1, "ES(%d) Notify Status change, codec not support\n", ucEsId);
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
                prVdecEsInfoKeep->eCurState = VDEC_ST_IDLE;
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
            UINT32 u4Param1 = 0, u4Param2 = 0, u4Param3 = 0;
            x_memset(&rPicType, 0, sizeof(VDEC_PICTYPE_CB_T));
            rPicType.u1EsId = ucEsId;

            #if (defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)) && !defined(CC_VDEC_H264_DISABLE)
            if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
            {
                if((prVdecEsInfo->ucPicType & 0x1F) == 0x7)
                {
                    LOG(6, "CB, PicType = Seq\n");
                    rPicType.u1PicType = (UINT8)VDEC_PIC_TYPE_SEQ;
                    prVdecInfo->pfPicTypeCb(&rPicType);
                }
                _VDEC_H264GetParam(ucEsId, TIME_PROFILE2,
                    &u4Param1,
                    &u4Param2,
                    &u4Param3);
                if(u4Param2 == 2)
                {
                    rPicType.u1PicType = VDEC_PIC_TYPE_I;
                    LOG(6, "CB, PicType = VDEC_PIC_TYPE_I\n");
                }
                else if(u4Param2 == 0)
                {
                    rPicType.u1PicType = VDEC_PIC_TYPE_P;
                    LOG(6, "CB, PicType = VDEC_PIC_TYPE_P\n");
                }
                else if(u4Param2 == 1)
                {
                    rPicType.u1PicType = VDEC_PIC_TYPE_B;
                    LOG(6, "CB, PicType = VDEC_PIC_TYPE_B\n");
                }
                else
                {
                    rPicType.u1PicType = VDEC_PIC_TYPE_I;
                    LOG(6, "CB, PicType = unknown(%d)\n", u4Param2);
                }
            }
            else                
            #endif
            {
                if(prVdecEsInfo->fgSeqHdr)
                {
                    LOG(6, "CB, PicType = Seq\n");
                    rPicType.u1PicType = (UINT8)VDEC_PIC_TYPE_SEQ;
                    prVdecInfo->pfPicTypeCb(&rPicType);
                }
                // map to VDEC_PIC_TYPE_T
                LOG(6, "CB, PicType = %d\n", (1 << prVdecEsInfo->ucPicType));
                rPicType.u1PicType = (UINT8)(1 << prVdecEsInfo->ucPicType);
            }
            prVdecInfo->pfPicTypeCb(&rPicType);
        }
    }
    else
    {
        if(prVdecEsInfo->fgMMPlayback)
        {
        	if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
        	{
        		//some 264 format need more frame to find the sequence header for start decoding.
				u4FailCntThreshold = VDEC_FRM_ERR_THRSD_PLAYBACK*5;
			}
			else
			{
				u4FailCntThreshold = VDEC_FRM_ERR_THRSD_PLAYBACK;
			}
            
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
                x_memset(&rErrCurTime, 0, sizeof(HAL_TIME_T));
                x_memset(&rDeltaTime, 0, sizeof(HAL_TIME_T));

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

        if (prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY)
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
                    LOG(1, "ES(%d) Notify Status change, HD size\n", ucEsId);
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
                     LOG(1, "ES(%d) Notify Status change, decode error\n", ucEsId);
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
        VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
        #ifdef CC_H264_SUPPORT
        #ifndef CC_H264_DISABLE
        VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
        UINT32 u4FbgId = 0, u4WaitFb = 0;
        UNUSED(prVdecInfo);
        #endif
        #endif

        UNUSED(prVdecEsInfoKeep);
        UNUSED(u4HwDecMs);
        #ifdef CC_H264_SUPPORT
        #ifndef CC_H264_DISABLE
        if(prVdecEsInfoKeep->pfnVdecGetParam)
        {
            prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, (UINT32)TIME_PROFILE, &u4FbgId, &u4HwDecMs, &u4WaitFb);
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

#else //#ifndef VDEC_TIME_PROFILE

    FBM_SEQ_HDR_T *prFbmSeqHdr;
    UINT32 u4FrmRate = 0, u4FbgId = 0, u4WaitFb = 0;
    UINT32 u4RstMs = 0, u4SliceType = 0, u4IsFrmReady = 0;
    UINT32 u4SliceID = 0;
    UINT32 u4SwMicro = 0, u4HwDecCycle = 0, u4DramUseCycle = 0;
    UINT8 i = 0;
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    static DRV_DBASE_T* _prDbase = NULL;
    CRIT_STATE_T _rCritState;

    char *acInfoTitle = "Dec Type,Frm Idx,Pic Type Idx,Pic Type,HW Dec MS,DramCycle,Pic Size,";

    UNUSED(i);

    if(prVdecEsInfoKeep->pfnVdecGetParam)
    {
        prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, TIME_PROFILE, &u4FbgId, &u4HwDecMs, &u4WaitFb);
        prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, TIME_PROFILE2, &u4RstMs, &u4SliceType, &u4IsFrmReady);
        prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, TIME_PROFILE3, &u4SwMicro, &u4HwDecCycle, &u4DramUseCycle);
    }

    if ( prVdecEsInfo->u4FrmFldCnt == 0 )
    {
        x_memset(acProfieInfo, 0, INFO_BUF_SIZE);
        x_memset(acProfileTmp, 0, TMP_BUF_SIZE);
        x_memset(acProfileSaveDir, 0, sizeof(acProfileSaveDir));
        u4CurTotalLines = 0x0;
        u4CurSaveFileCnt = 0x0;

        prVdecEsInfo->u4CurrPicSize = 0x0;
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
        //LOG(0, "Slice Type \t = %ld \n", u4SliceType);
        u4SliceID = u4SliceType;
        if(u4SliceType >= 5)
        {
            u4SliceType -= 5;
        }
        ASSERT(u4SliceType < 5);
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
        if (u4HwDecMs < prVdecEsInfo->au4DecTime[u4SliceType][1])
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
        if (  prVdecEsInfo->u4CurrPicSize > prVdecEsInfo->au4DecTimeMaxPicSize[u4SliceType][0] )
        {
            prVdecEsInfo->au4DecTimeMaxPicSize[u4SliceType][0] = prVdecEsInfo->u4CurrPicSize;
        }
        prVdecEsInfo->au4DecTime[u4SliceType][2] += u4HwDecMs;
        prVdecEsInfo->au4DecTime[u4SliceType][6] += (u4HwDecCycle/10000);
        prVdecEsInfo->au4DecTime[u4SliceType][3]++;
        HAL_GetTime(&_rTimeFrmS);

        //if (prVdecEsInfo->u4FrmFldCnt < 31)
    {
            if ( prVdecEsInfo->u4FrmFldCnt == 0 )
            {
                sprintf(acProfieInfo, "%s", acInfoTitle);
                NEW_A_LINE;
            }
            sprintf(acProfileTmp, "%ld,", prVdecEsInfoKeep->eCurFMT);
            strcat(acProfieInfo, acProfileTmp);
            sprintf(acProfileTmp, "%ld,", prVdecEsInfo->u4FrmFldCnt);
            strcat(acProfieInfo, acProfileTmp);
            sprintf(acProfileTmp, "%ld,", u4SliceType);
            strcat(acProfieInfo, acProfileTmp);
            switch (u4SliceID)
            {
                case 0:
                    sprintf(acProfileTmp, "P,");
                    break;
                case 1:
                    sprintf(acProfileTmp, "B,");
                    break;
                case 2:
                    sprintf(acProfileTmp, "I,");
                    break;
                case 3:
                    sprintf(acProfileTmp, "SP,");
                    break;
                case 4:
                    sprintf(acProfileTmp, "SI,");
                    break;
                case 5:
                    sprintf(acProfileTmp, "P_ALL,");
                    break;
                case 6:
                    sprintf(acProfileTmp, "B_ALL,");
                    break;
                case 7:
                    sprintf(acProfileTmp, "I_ALL,");
                    break;
                case 8:
                    sprintf(acProfileTmp, "SP_ALL,");
                    break;
                case 9:
                    sprintf(acProfileTmp, "SI_ALL,");
                    break;
                default:
                    sprintf(acProfileTmp, "ERROR,"); //Error
                    break;
    }
            strcat(acProfieInfo, acProfileTmp);
            sprintf(acProfileTmp, "%ld,", u4HwDecMs);
            strcat(acProfieInfo, acProfileTmp);
            sprintf(acProfileTmp, "%ld,", u4DramUseCycle);
            strcat(acProfieInfo, acProfileTmp);
            sprintf(acProfileTmp, "%ld,", prVdecEsInfo->u4CurrPicSize);
            strcat(acProfieInfo, acProfileTmp);

            NEW_A_LINE;
        }

        prVdecEsInfo->u4FrmFldCnt++;
    }

    UNUSED(u4HwDecMs);

#endif

}


static void _VdecSetSyncStc(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
    #ifndef CC_VDEC_H264_DISABLE
    //VDEC_INFO_T* prVdecInfo;
    //prVdecInfo = _VDEC_GetInfo();
    #endif
    #endif

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    UNUSED(prVdecEsInfoKeep);

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
    if(prVdecEsInfoKeep->pfnVdecSetParam)
    {
         prVdecEsInfoKeep->pfnVdecSetParam(ucEsId, (UINT32)WAIT_FB_TIME, prVdecEsInfo->u4WaitDispTime, 0, 0);
         prVdecEsInfoKeep->pfnVdecSetParam(ucEsId, (UINT32)AV_SYNC_INFO, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId, 0);
    }
    #endif
    #endif
    STC_SetVdoDeviceId(prVdecEsInfo->ucStcId, ucEsId);
    //set VFIFO initial delay, will modify PTS value of audio and video by dmx
    if(!prVdecEsInfo->fgMMPlayback)
    {
        #if (defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)) && !defined(CC_VDEC_H264_DISABLE)
        if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)    // format will be mpv by default before _VDEC_DecCfg
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

INT32 _VdecGetAspectRatioValue(FBM_SEQ_HDR_T *prFbmSeqHdr,UINT32 *u4HRatio,UINT32 *u4VRatio)
{
  UINT8 ucAspectRatio=prFbmSeqHdr->ucAspRatInf;
  if(!prFbmSeqHdr->fgPixelAspectRatio)
  {
      switch (ucAspectRatio)
      {
		 case MPEG_ASPECT_RATIO_4_3:
		   *u4HRatio=4;
		   *u4VRatio=3;
		   break;
		 case MPEG_ASPECT_RATIO_16_9:
		   *u4HRatio=16;
		   *u4VRatio=9;
		   break;
		 case MPEG_ASPECT_RATIO_221_1:
		   *u4HRatio=221;
		   *u4VRatio=100;
		   break;
		 case MPEG_ASPECT_RATIO_TRANSMIT:
		   *u4HRatio=prFbmSeqHdr->u4AspectRatioWidth;
		   *u4VRatio=prFbmSeqHdr->u4AspectRatioHeight;
		   break;
         default:
		   *u4HRatio=1;
		   *u4VRatio=1;
		   break;
      }
  }
  else
  {

	  switch (ucAspectRatio)
	  {
		  case MPEG_ASPECT_RATIO_1_1:
			*u4HRatio=1;
			*u4VRatio=1;
			break;
		  case MPEG_ASPECT_RATIO_4_3:
			*u4HRatio=4;
			*u4VRatio=3;
			break;
		  case MPEG_ASPECT_RATIO_16_9:
			*u4HRatio=16;
			*u4VRatio=9;
			break;
		 case MPEG_ASPECT_RATIO_221_1:
			*u4HRatio=221;
			*u4VRatio=100;
			break;
		case MPEG_ASPECT_RATIO_10_11:
			*u4HRatio=10;
			*u4VRatio=11;
			break;
		case MPEG_ASPECT_RATIO_40_33:
			*u4HRatio=40;
			*u4VRatio=33;
			break;
		case MPEG_ASPECT_RATIO_16_11:
			*u4HRatio=16;
			*u4VRatio=11;
			break;
		case MPEG_ASPECT_RATIO_12_11:
			*u4HRatio=12;
			*u4VRatio=11;
			break;
		case MPEG_ASPECT_RATIO_3_2:
			*u4HRatio=3;
			*u4VRatio=2;
			break;
		case MPEG_ASPECT_RATIO_TRANSMIT:
			*u4HRatio=prFbmSeqHdr->u4AspectRatioWidth;
			*u4VRatio=prFbmSeqHdr->u4AspectRatioHeight;
			break;
		default:
			*u4HRatio=1;
			*u4VRatio=1;
			break;
	    }
  }
  return 0;
}


VDEC_ASPECT_RATIO_T _VdecDecideAspectRatio(ENUM_VDEC_FMT_T eFmt,UCHAR ucAspectRatio, UINT32 u4Width, UINT32 u4Height, BOOL fgMMPlay)
{
#if 0
    if(eFmt==VDEC_FMT_H264)
    {
		switch (ucAspectRatio)
		{			 
			case MPEG_ASPECT_RATIO_16_9:
				return VDEC_ASPECT_RATIO_16_9;
				
			case MPEG_ASPECT_RATIO_221_1:
				return VDEC_ASPECT_RATIO_221_1;

			default:
				break;
		} 
 		return VDEC_ASPECT_RATIO_4_3;
    }
#endif
	
	//if (ucAspectRatio != MPEG_ASPECT_RATIO_1_1)
	{
		switch (ucAspectRatio)
		{
		    case MPEG_ASPECT_RATIO_1_1:
		        return VDEC_ASPECT_RATIO_1_1;
		        
			case MPEG_ASPECT_RATIO_4_3:
				return VDEC_ASPECT_RATIO_4_3;

			case MPEG_ASPECT_RATIO_16_9:
				return VDEC_ASPECT_RATIO_16_9;

			case MPEG_ASPECT_RATIO_221_1:
			    return VDEC_ASPECT_RATIO_221_1;

			case MPEG_ASPECT_RATIO_10_11:
			    return VDEC_ASPECT_RATIO_10_11;

			case MPEG_ASPECT_RATIO_40_33:
			    return VDEC_ASPECT_RATIO_40_33;

			case MPEG_ASPECT_RATIO_16_11:
			    return VDEC_ASPECT_RATIO_16_11;

            case MPEG_ASPECT_RATIO_12_11:
			    return VDEC_ASPECT_RATIO_12_11;
			    
            case MPEG_ASPECT_RATIO_3_2:
			    return VDEC_ASPECT_RATIO_3_2;

            case MPEG_ASPECT_RATIO_24_11:
                return VDEC_ASPECT_RATIO_24_11;

            case MPEG_ASPECT_RATIO_20_11:
                return VDEC_ASPECT_RATIO_20_11;
                
            case MPEG_ASPECT_RATIO_32_11:
                return VDEC_ASPECT_RATIO_32_11;
                
            case MPEG_ASPECT_RATIO_80_33:
                return VDEC_ASPECT_RATIO_80_33;
                
            case MPEG_ASPECT_RATIO_18_11:
                return VDEC_ASPECT_RATIO_18_11;

            case MPEG_ASPECT_RATIO_15_11:
                return VDEC_ASPECT_RATIO_15_11;

            case MPEG_ASPECT_RATIO_64_33:
                return VDEC_ASPECT_RATIO_64_33;
                
            case MPEG_ASPECT_RATIO_160_99:
                return VDEC_ASPECT_RATIO_160_99;
                
            case MPEG_ASPECT_RATIO_TRANSMIT:
                return VDEC_ASPECT_RATIO_TRANSMIT;
                
			default:
				return VDEC_ASPECT_RATIO_1_1;
		}
	}
	/*else
	{
		UINT32 u4Temp;

		// 4:3 = 1.33
		// 16:9 = 1.78
		// 2.21:1 = 2.21

		// if width / height < (1.78 + 1.33) / 2 >> 4:3
		// else if width / height < (2.21 + 1.78) / 2 >> 16:9
		// else 2.21 : 1
		// (1.78 + 1.33) / 2 = 14 / 9
		// (2.21 + 1.78) / 2 ~= 2

		u4Temp = (((u4Width * 9) / 14) / u4Height);

		if (u4Temp < 1)
		{
			return VDEC_ASPECT_RATIO_4_3;
		}
		else
		{
			u4Temp = ((u4Width / 2) / u4Height);

			if (u4Temp < 1)
			{
				return VDEC_ASPECT_RATIO_16_9;
			}
			else
			{
				return VDEC_ASPECT_RATIO_221_1;
			}
		}
	}
	*/
}

static void _VdecFillThumbnailRegion(VDP_THUMBNAIL_INFO_T *prCapDstInfo)
{
   //UINT32 u4Color=0xff000000;
	UINT32 u4ColorMode= 0xff000000;
	switch(prCapDstInfo->eCanvasColormode)
	{
	case VDP_COLORMODE_ARGB_D8888:
	    u4ColorMode=0xff000000;
	 break;
	  case VDP_COLORMODE_AYUV_D8888:
	    u4ColorMode=0xff008080;
	 break;
	  case VDP_COLORMODE_ARGB_D4444:
	    u4ColorMode=0xf000;
	 break;
	  // todo :other format.......
	  default:
	     LOG(3,"unknow color format\n");
	 break;
	}

   GFX_Lock();
   GFX_SetDst(prCapDstInfo->pucCanvasBuffer,prCapDstInfo->eCanvasColormode+2, \
		prCapDstInfo->u4CanvasPitch);
   GFX_SetColor(u4ColorMode);
   GFX_Fill(prCapDstInfo->u4ThumbnailX,prCapDstInfo->u4ThumbnailY, \
		prCapDstInfo->u4ThumbnailWidth,prCapDstInfo->u4ThumbnailHeight);
   GFX_Flush();
   GFX_Wait();
   GFX_Unlock();
}


static void _VdecGetThumbnail(UCHAR ucEsId)
{
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
	UINT32 u4WorkBuf = 0, u4AlignSz = 0,u4FbNum = 0, u4Idx, u4TmpBuf;
    UINT32 u4AdjThumbWidth = 0, u4AdjThumbHeight = 0;
    UINT32 u4ThumbRatio = 0, u4VideoRatio = 0;

#if 0
	UINT32 u4Line=0,u4Hpos=0,u4Offset=0;
	UCHAR *pDstBuffer;
#endif

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    ASSERT(prVdecInfo != NULL);
    ASSERT(prVdecEsInfo != NULL);
    ASSERT(prVdecEsInfoKeep != NULL);

    if((prVdecEsInfo->rCapSrcInfo.u4VSize == 0) || (prVdecEsInfo->rCapSrcInfo.u4HSize == 0))
    {
        return;
    }

#ifdef VDEC_THUMBNAIL_ADDR_CHECK
    {
       UINT32 u4OsdFbmAddr=0,u4OsdPoolIndex=0;
	   UCHAR *pDstBuffer;
	   BOOL fgFindOSD=FALSE;
       FBM_POOL_T *prFbmPool1;
	   pDstBuffer=(UCHAR*)VIRTUAL(prVdecEsInfo->rCapDstInfo.pucCanvasBuffer);
	   for(u4OsdPoolIndex=FBM_POOL_TYPE_JPEG_OSD1;u4OsdPoolIndex<=FBM_POOL_TYPE_JPEG_OSD4;u4OsdPoolIndex++)
	   {
		  prFbmPool1 = FBM_GetPoolInfo((UINT8)u4OsdPoolIndex);
		  u4OsdFbmAddr=VIRTUAL(prFbmPool1->u4Addr);
		  if(((UINT32)pDstBuffer)>=u4OsdFbmAddr&& ((UINT32)pDstBuffer)<(u4OsdFbmAddr+prFbmPool1->u4Size))
		  {
             fgFindOSD=TRUE;
			 break;
		  }
	   }

	   if(!fgFindOSD)
	   {
	       if (prVdecInfo->pfErrNotify)
	       {
		      LOG(0, "Notify get thumbnail fail, address error\n");
		      prVdecInfo->pfErrNotify((UINT32)prVdecEsInfo->u2CompId,
			   (UINT32)VDEC_COND_ERROR, 0, 0);
	       }
		   return;
	   }
    }
#endif

#if 0                //remove it for kernnel crash
    u4Offset=prVdecEsInfo->rCapDstInfo.u4ThumbnailX*4;
    pDstBuffer=VIRTUAL((UINT32)prVdecEsInfo->rCapDstInfo.pucCanvasBuffer+(prVdecEsInfo->rCapDstInfo.u4ThumbnailY*prVdecEsInfo->rCapDstInfo.u4CanvasPitch));
	for(u4Line=0;u4Line<prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight;u4Line++)
    {
       for(u4Hpos=0;u4Hpos<prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth;u4Hpos++)
       {
         if(prVdecEsInfo->bIsOnePass)
         {
			 *(((UINT32 *)(VIRTUAL((UINT32)pDstBuffer)+u4Offset))+u4Hpos)=0xff008080;
         }
		 else
		 {
			 *(((UINT32 *)(VIRTUAL((UINT32)pDstBuffer)+u4Offset))+u4Hpos)=0xff000000;
		 }
       }
       pDstBuffer+=prVdecEsInfo->rCapDstInfo.u4CanvasPitch;
    }
#endif
	_VdecFillThumbnailRegion(&prVdecEsInfo->rCapDstInfo);

    // keep ratio, calculate image area, if request to ARGB --> 2pass needed
 if(prVdecEsInfo->fgKeepRation)  //DTV000311949,
 {
    u4ThumbRatio = ((prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth * 1000) / prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight);
    u4VideoRatio = ((prVdecEsInfo->rCapSrcInfo.u4HSize * 1000)/ prVdecEsInfo->rCapSrcInfo.u4VSize);
    if(u4ThumbRatio < u4VideoRatio)
    {
        u4AdjThumbHeight = prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth *
                            prVdecEsInfo->rCapSrcInfo.u4VSize /
                            prVdecEsInfo->rCapSrcInfo.u4HSize ;
        prVdecEsInfo->rCapDstInfo.u4ThumbnailY += (prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight - u4AdjThumbHeight) / 2;
        prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight = u4AdjThumbHeight;
    }
    else
    {
        u4AdjThumbWidth  = prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight *
                            prVdecEsInfo->rCapSrcInfo.u4HSize /
                            prVdecEsInfo->rCapSrcInfo.u4VSize ;
        prVdecEsInfo->rCapDstInfo.u4ThumbnailX += (prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth - u4AdjThumbWidth) / 2;
        prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth = u4AdjThumbWidth;
    }
        
    if(!prVdecEsInfo->bIsOnePass)       //allocate working buffer for 2 pass
    {
        u4FbNum = (UINT32)FBM_GetFrameBufferNs(prVdecEsInfo->ucFbgId);
        for(u4Idx = 0; u4Idx < u4FbNum; u4Idx++)
        {
            if(FBM_GetFrameBufferStatus(prVdecEsInfo->ucFbgId, u4Idx)
                != FBM_FB_STATUS_LOCK)  //already freeze at mpv_mw.c
            {
                if((prVdecEsInfo->fgPlaybackNotDisplay) || // MHP I frame case
                   (!prVdecEsInfo->fgKeepRation))          // show logo case
                {
                    if(FBM_GetFrameBufferStatus(prVdecEsInfo->ucFbgId, u4Idx)
                       == FBM_FB_STATUS_DISPLAYQ)
                    {
                        // displaying, get other frame
                        continue;
                    }
                }
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
    }

	if(prVdecEsInfo->rCapSrcInfo.u4VSize&0x1)
		   {
        prVdecEsInfo->rCapSrcInfo.u4VSize--;
        }

    if(prVdecEsInfo->bIsOnePass)
    {
    IMGRZ_SetDigest((void*)&prVdecEsInfo->rCapSrcInfo, (void*)&prVdecEsInfo->rCapDstInfo,
                    prVdecEsInfo->bIsOnePass, u4WorkBuf);
    }
    else if(prVdecEsInfo->fgKeepRation) // YCbCr->YUV->ARGB8888
    {
        IMGRZ_SetDigest((void*)&prVdecEsInfo->rCapSrcInfo, (void*)&prVdecEsInfo->rCapDstInfo,
                       prVdecEsInfo->bIsOnePass, u4WorkBuf);
        UNUSED(GFX_ColorConvert((void*)&prVdecEsInfo->rCapDstInfo, u4WorkBuf, 14, 0xFF));
    }
    else // YCbCr(64*32) -> ARGB8888 
    {
         // we clean dst buffer by cpu, flush cache aviod the memory being changed by cpu automatically flush later
         // be care of virtual address translation
         #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
            HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)prVdecEsInfo->rCapDstInfo.pucCanvasBuffer),
                ( prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight*prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth*sizeof(UINT32)));
        #else
            HalFlushInvalidateDCache();
        #endif
        UNUSED(GFX_ColorConvert_YC((void*)&prVdecEsInfo->rCapDstInfo, prVdecEsInfo->rCapSrcInfo.u4AddrY,
                    prVdecEsInfo->rCapSrcInfo.u4AddrC, 14, 0xFF));  
        
        // we clean dst buffer by cpu, flush cache aviod the memory being changed by cpu automatically flush later
        #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
            HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)prVdecEsInfo->rCapDstInfo.pucCanvasBuffer),
                ( prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight*prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth*sizeof(UINT32)));
        #else
            HalFlushInvalidateDCache();
        #endif
    }
    

    if (prVdecInfo->pfErrNotify)
    {
        LOG(3, "Notify get thumbnail done\n");
        prVdecInfo->pfErrNotify((UINT32)prVdecEsInfo->u2CompId,
            (UINT32)VDEC_COND_THUMBNAIL_DONE, 0, 0);
    }
    // TODO, modify mpv_mw.c to use this callback function.
    UNUSED(prVdecEsInfoKeep);
    #if 0
    if (prVdecEsInfoKeep->pfDecNfyFct)
    {
        LOG(3, "Notify get thumbnail done\n");
        prVdecEsInfoKeep->pfDecNfyFct(
            prVdecEsInfoKeep->pvDecNfyTag,
            VDEC_COND_THUMBNAIL_DONE, 0, 0);
    }
    #endif
}

static void _VdecGetThumbnail2(UCHAR ucEsId, UCHAR ucFbId)
{
    //VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_PIC_HDR_T* prFbmPicHdr;
	UINT32 u4WorkBuf = 0, u4AlignSz = 0;
    UINT32 u4AdjThumbWidth = 0, u4AdjThumbHeight = 0;
    UINT32 u4ThumbRatio = 0, u4VideoRatio = 0,u4OriginalBottom=0;

    //prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    //ASSERT(prVdecInfo != NULL);
    ASSERT(prVdecEsInfo != NULL);

    FBM_GetFrameBufferAddr(prVdecEsInfo->ucFbgId, ucFbId,
        &(prVdecEsInfo->rCapSrcInfo.u4AddrY),
        &(prVdecEsInfo->rCapSrcInfo.u4AddrC));
    FBM_GetFrameBufferSize(prVdecEsInfo->ucFbgId,
        &(prVdecEsInfo->rCapSrcInfo.u4MemHSize),
        &(prVdecEsInfo->rCapSrcInfo.u4MemVSize));
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdecEsInfo->ucFbgId);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucFbId);

    if (prFbmSeqHdr == NULL)
    {
        LOG(3, "ES(%d)(L:%d) %s fail\n", ucEsId, __LINE__, __FUNCTION__);

        if (prVdecEsInfoKeep->pfDecNfyFct)
        {
            prVdecEsInfoKeep->pfDecNfyFct(
                prVdecEsInfoKeep->pvDecNfyTag,
                VDEC_COND_THUMBNAIL_FAIL, 0, 0);
        }
        return;
    }
    else
    {
        /* Thumbnail usage: Image resizer will use this as pitch. */
        if (prFbmPicHdr && prFbmSeqHdr->fgResizeSmallPic)
        {
            prVdecEsInfo->rCapSrcInfo.u4MemHSize =
                (UINT32)prFbmPicHdr->u4PicWidthPitch;

            prVdecEsInfo->rCapSrcInfo.u4HSize = (UINT32)prFbmPicHdr->u4PicWidth;
            prVdecEsInfo->rCapSrcInfo.u4VSize = (UINT32)prFbmPicHdr->u4PicHeight;
        }
        else
        {
            prVdecEsInfo->rCapSrcInfo.u4MemHSize = (UINT32)prFbmSeqHdr->u2LineSize;

            prVdecEsInfo->rCapSrcInfo.u4HSize = (UINT32)prFbmSeqHdr->u2OrgHSize;
            prVdecEsInfo->rCapSrcInfo.u4VSize = (UINT32)prFbmSeqHdr->u2OrgVSize;
        }

        prVdecEsInfo->rCapSrcInfo.u4RRMode = (UINT32)prFbmSeqHdr->fgRRYEn;
    }

	if(prVdecEsInfoKeep->eCurFMT==VDEC_FMT_MJPEG)
	{
                #if  defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)// Force raster out for VENC
		prVdecEsInfo->rCapSrcInfo.u4IsRsIn = 1; //MJPEG for 5395 encoder input
		prVdecEsInfo->rCapSrcInfo.u4SrcSwap = 0;
		#else
		prVdecEsInfo->rCapSrcInfo.u4IsRsIn = 0;
		prVdecEsInfo->rCapSrcInfo.u4SrcSwap = 6;//block swap mode;
		#endif
	}
	else
	{
		prVdecEsInfo->rCapSrcInfo.u4IsRsIn = 0;
		prVdecEsInfo->rCapSrcInfo.u4SrcSwap = 6;//block swap mode;
	}

    if((prVdecEsInfo->rCapSrcInfo.u4VSize == 0) ||
       (prVdecEsInfo->rCapSrcInfo.u4HSize == 0))
    {
        LOG(3, "ES(%d)(L:%d) %s fail\n", ucEsId, __LINE__, __FUNCTION__);

        if (prVdecEsInfoKeep->pfDecNfyFct)
        {
            prVdecEsInfoKeep->pfDecNfyFct(
                prVdecEsInfoKeep->pvDecNfyTag,
                VDEC_COND_THUMBNAIL_FAIL, 0, 0);
        }
        return;
    }
	_VdecFillThumbnailRegion(&prVdecEsInfo->rCapDstInfo);

    if(prVdecEsInfo->fgKeepRation)
    {
        u4OriginalBottom = prVdecEsInfo->rCapDstInfo.u4ThumbnailY
            + prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight;
        u4ThumbRatio = ((prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth * 1000) /
            prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight);
        u4VideoRatio = ((prVdecEsInfo->rCapSrcInfo.u4HSize * 1000) /
            prVdecEsInfo->rCapSrcInfo.u4VSize);
        if(u4ThumbRatio < u4VideoRatio)
        {
            u4AdjThumbHeight = prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth *
                prVdecEsInfo->rCapSrcInfo.u4VSize /
                prVdecEsInfo->rCapSrcInfo.u4HSize ;
            prVdecEsInfo->rCapDstInfo.u4ThumbnailY +=
                (prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight - u4AdjThumbHeight) / 2;
            prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight = u4AdjThumbHeight;
        }
        else
        {
            u4AdjThumbWidth  = prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight *
                prVdecEsInfo->rCapSrcInfo.u4HSize /
                prVdecEsInfo->rCapSrcInfo.u4VSize ;
            prVdecEsInfo->rCapDstInfo.u4ThumbnailX +=
                (prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth - u4AdjThumbWidth) / 2;
            prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth = u4AdjThumbWidth;
        }
    }

    //allocate working buffer for 2 pass
    {
        //FBM_POOL_T *prFbmPool;
    	//prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_FEEDER);
        u4WorkBuf = prVdecEsInfo->u4WorkingBufAddr;
        if((u4WorkBuf == 0) ||
           (prVdecEsInfo->u4WorkingBufSize <
           (prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth *
            prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight * 4)))
        {
            LOG(3, "ES(%d)(L:%d) %s fail\n", ucEsId, __LINE__, __FUNCTION__);

            if (prVdecEsInfoKeep->pfDecNfyFct)
            {
                prVdecEsInfoKeep->pfDecNfyFct(
                    prVdecEsInfoKeep->pvDecNfyTag,
                    VDEC_COND_THUMBNAIL_FAIL, 0, 0);
            }
            return;
        }

        //check alignment, gfx convert can't set src pitch, should be equal to width
        //set active region in the middle of Thumbnail rectangle
        u4AlignSz = prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth & 0xF;

        if(u4AlignSz > 0)
        {
            prVdecEsInfo->rCapDstInfo.u4ThumbnailWidth -= u4AlignSz;
            prVdecEsInfo->rCapDstInfo.u4ThumbnailX += (u4AlignSz>>1);
        }

        if(prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight&0x1)
        {
            if((prVdecEsInfo->rCapDstInfo.u4ThumbnailY +
                prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight + 1) >
                u4OriginalBottom)
            {
                prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight--;
            }
            else
            {
                prVdecEsInfo->rCapDstInfo.u4ThumbnailHeight++;
            }
        }
    }

    IMGRZ_SetDigest((void*)&prVdecEsInfo->rCapSrcInfo,
        (void*)&prVdecEsInfo->rCapDstInfo,
                    FALSE, u4WorkBuf);
    // second pass
    {
        UNUSED(GFX_ColorConvert((void*)&prVdecEsInfo->rCapDstInfo,
            u4WorkBuf, 14, 0xFF));
    }

    // notify finish
    if (prVdecEsInfoKeep->pfDecNfyFct)
    {
        UINT32 u4DeltaMs = 0;
        HAL_TIME_T rTime;
        HAL_TIME_T rDeltaTime;
        HAL_GetTime(&rTime);

        HAL_GetDeltaTime(&rDeltaTime, &prVdecEsInfo->rThumbStartTime, &rTime);
        u4DeltaMs = (rDeltaTime.u4Seconds * 1000) +
                    (rDeltaTime.u4Micros / 1000);
        LOG(5, "Es(%d) Thumbnail Delat Time(%d.%06d)\n", ucEsId,
            rDeltaTime.u4Seconds, rDeltaTime.u4Micros);

        LOG(3, "ES(%d)(L:%d) %s done\n", ucEsId, __LINE__, __FUNCTION__);
        prVdecEsInfoKeep->pfDecNfyFct(
            prVdecEsInfoKeep->pvDecNfyTag,
            VDEC_COND_THUMBNAIL_DONE, u4DeltaMs, 0);
    }
}

VOID _VDEC_JobBeforePutDispQ(UCHAR ucEsId, UCHAR ucFbId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    ASSERT(prVdecEsInfo != NULL);
    if(!prVdecEsInfo)
    {
        LOG(3, "_VDEC_JobBeforePutDispQ, prVdecEsInfo null\n");
        return;
    }
    if((prVdecEsInfo->fgThumbMethod2) && (prVdecEsInfo->fgThumbStartTime))
    {
        _VdecGetThumbnail2(ucEsId, ucFbId);
        prVdecEsInfo->fgThumbMethod2 = FALSE;
    }
}

static void _VdecFlushDPB(UCHAR ucEsId)
{
    UCHAR ucFRefFbId, ucBRefFbId;
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    FBM_PIC_HDR_T *prFbmPicHdr;

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    if(!prVdecInfo)
    {
        LOG(3, "Flush buffer handle null!\n");
        return;
    }

    if ((prVdecEsInfo->ucFbgId != FBM_FBG_TYPE_UNKNOWN) &&
        (prVdecEsInfoKeep->eCurState == VDEC_ST_IDLE))
    {
        if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
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
                    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV)
                    {
                        UNUSED(_MPV_CalcPts(ucEsId, ucBRefFbId));
                    }
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_MPEG4_DISABLE
                    else if (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MP4)
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
                    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV)
                    {
                        UNUSED(_MPV_CalcPts(ucEsId, ucFRefFbId));
                    }
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_MPEG4_DISABLE
                    else if (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MP4)
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

    if((prVdecEsInfoKeep->rPesInfo.fgEos) &&
        (!prVdecEsInfo->fgSendedEos))
    {
        prVdecEsInfo->fgSendedEos = TRUE;
        LOG(3, "_VDEC_DeliverEos(%d)\n", ucEsId);

#ifdef MULTIMEDIA_AUTOTEST_SUPPORT
        //UINT32 u4CrcCkRlt;

		if (prVdecEsInfoKeep->pfDecAutoTestNfyFct)
		{
			LOG(0, "VDEC EOS, notify Auto test Crc result\n");
			prVdecEsInfoKeep->pfDecAutoTestNfyFct(prVdecEsInfoKeep->pvDecAutoTestNfyTag,
			VDEC_COND_CRC_CHECK_DONE, 2, prVdecEsInfo->u4CrcResult);
		}

		if(prVdecEsInfo->fgAutoCrcCk)
		{
			prVdecEsInfo->fgAutoCrcCk = FALSE;
		    if(prVdecEsInfo->u4CrcGolden != prVdecEsInfo->u4CrcResult)
		    {
				LOG(0," !!!crc mismatch, u4CrcResult =0x%x, u4CrcGolden =0x%x\n",
				prVdecEsInfo->u4CrcResult, prVdecEsInfo->u4CrcGolden);
		    }
		}

#endif

#if defined(VDEC_CRC_TEST) && !defined(MULTIMEDIA_AUTOTEST_SUPPORT)

    if(VDEC_IsTestCRC(ucEsId))
    {
        LOG(0, "The CRC value of the file is %u\n", prVdecEsInfo->u4CrcResult);
        prVdecEsInfo->u4CrcResult = 0;
    }
#endif

#ifdef VDEC_TIME_PROFILE

        {
            {
                UINT8 i = 0;

                NEW_A_LINE;
                strcat(acProfieInfo, "Pic Type Idx,Max HW Decode MS,Min HW Decode MS,Total HW Decode MS,Total Frm/Fld Cnt,Max Dram Cyle,Max Pic Size,");
                NEW_A_LINE;
                for ( i = 0; i < 5; i++ )
                {
                    sprintf(acProfileTmp, "%ld,", i);
                    strcat(acProfieInfo, acProfileTmp);
                    sprintf(acProfileTmp, "%ld,", prVdecEsInfo->au4DecTime[i][0]);
                    strcat(acProfieInfo, acProfileTmp);
                    sprintf(acProfileTmp, "%ld,", prVdecEsInfo->au4DecTime[i][1]);
                    strcat(acProfieInfo, acProfileTmp);
                    sprintf(acProfileTmp, "%ld,", prVdecEsInfo->au4DecTime[i][2]);
                    strcat(acProfieInfo, acProfileTmp);
                    sprintf(acProfileTmp, "%ld,", prVdecEsInfo->au4DecTime[i][3]);
                    strcat(acProfieInfo, acProfileTmp);
                    sprintf(acProfileTmp, "%ld,", prVdecEsInfo->au4DecTime[i][8]);
                    strcat(acProfieInfo, acProfileTmp);
                    sprintf(acProfileTmp, "%ld,", prVdecEsInfo->au4DecTimeMaxPicSize[i][0]);
                    strcat(acProfieInfo, acProfileTmp);
                    NEW_A_LINE;
                }
            }

            _VdecTimeProfileSaveToFile(ucEsId);

            //LOG(0, "The current dir is \n %s \n", (char *)prVdecEsInfo->ucAutoPlayCurFileDir);

            x_memset(&(prVdecEsInfo->ucAutoPlayCurFileDir[0]), 0x0, sizeof(prVdecEsInfo->ucAutoPlayCurFileDir));
            x_memset(&(prVdecEsInfo->ucAutoPlayCurFileName[0]), 0x0, sizeof(prVdecEsInfo->ucAutoPlayCurFileName));
        }

#endif

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

            if(prVdecEsInfoKeep->pfnTrickPtsCb)
            {
                // We have put two to empty that originaly should put to display.
                // That means we have previous display frame to empty queue.
                if(prVdecEsInfo->u2Disp2EmptyCnt > 1)
                {
                    prVdecEsInfoKeep->pfnTrickPtsCb(prVdecEsInfo->u1SwdmxSrcId, 0, FALSE);
                    LOG(5, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderBackStep finish\n");
                }
                else
                {
                    // Send again with far position
                    prVdecEsInfoKeep->pfnTrickPtsCb(prVdecEsInfo->u1SwdmxSrcId, 0, TRUE);
                    LOG(5, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderBackStep again\n");
                }
                //x_thread_delay(33);
            }
            if(prVdecEsInfoKeep->pfnRenderPtsCb)
            {
                prVdecEsInfoKeep->pfnRenderPtsCb(prVdecEsInfo->u4PTSLast, prVdecEsInfoKeep->u1AttachedSrcId);
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
            if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
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
            else if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_AVS)
            {
                //flush dpb
                #if defined(CC_AVS_SUPPORT)
                #ifndef CC_VDEC_AVS_DISABLE
                if(!prVdecEsInfo->fgKeepPes)
                {
                    _VDEC_AVSFlush(ucEsId, FALSE);
                }
                else
                {
                    _VDEC_AVSFlush(ucEsId, TRUE);
                }
                #endif
                #endif
            }
            else if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_VP6)
            {
    #if defined(CC_VP6_SUPPORT)
    #ifndef CC_VDEC_VP6_DISABLE
                if(!prVdecEsInfo->fgKeepPes)
                {
                    _VDEC_VP6Flush(ucEsId, FALSE);
                }
            else
            {
                    _VDEC_VP6Flush(ucEsId, TRUE);
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
                            if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV)
                            {
                                UNUSED(_MPV_CalcPts(ucEsId, ucBRefFbId));
                            }
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_MPEG4_DISABLE
                            else if (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MP4)
                            {
                                UNUSED(_VDEC_MPEG4CalPts(ucEsId, ucBRefFbId));
                            }
#endif
#endif

                            #ifdef CC_REALD_3D_SUPPORT
                            _VDEC_Check2D3DSwitch(ucEsId, ucBRefFbId);
                            #endif
                            _VDEC_JobBeforePutDispQ(ucEsId, ucBRefFbId);

                            FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);

                            if(prVdecEsInfo->u4DisplayQPicCnt == 0)
                            {
                                STC_StopStc(prVdecEsInfo->ucStcId);
                                STC_SetStcValue(prVdecEsInfo->ucStcId, (UINT32)(prFbmPicHdr->u4PTS));
                                STC_StartStc(prVdecEsInfo->ucStcId);
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
                            if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV)
                            {
                                UNUSED(_MPV_CalcPts(ucEsId, ucFRefFbId));
                            }
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_MPEG4_DISABLE
                            else if (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MP4)
                            {
                                UNUSED(_VDEC_MPEG4CalPts(ucEsId, ucBRefFbId));
                            }
#endif
#endif
                            #ifdef CC_REALD_3D_SUPPORT
                            _VDEC_Check2D3DSwitch(ucEsId, ucFRefFbId);
                            #endif
                            _VDEC_JobBeforePutDispQ(ucEsId, ucFRefFbId);
                            FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_DISPLAYQ);

                            if(prVdecEsInfo->u4DisplayQPicCnt == 0)
                            {
                                STC_StopStc(prVdecEsInfo->ucStcId);
                                STC_SetStcValue(prVdecEsInfo->ucStcId, (UINT32)(prFbmPicHdr->u4PTS));
                                STC_StartStc(prVdecEsInfo->ucStcId);
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
                if((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
                    ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MJPEG)
                    ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_AVS)
                    ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_RAW)
                    ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_VP8)
                    ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_VP6))
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

                    STC_StopStc(prVdecEsInfo->ucStcId);
                    prFbmPicHdr->i4TemporalRef = 0;
                    prFbmPicHdr->u2DecodingOrder = 0;
                    // DTV00139938, [MM]Flash wrong progress bar after excute FR2X and FF2X several times during playback
                    // should base on direct to send dummy black frame
                    if(prVdecEsInfo->u4MMSpeed <= STC_SPEED_TYPE_FORWARD_1_DOT_5X)
                    {
                        if(prVdecEsInfo->rABInfo.fgValid)
                        {
                            prFbmPicHdr->rExtra.u8PTSI = prVdecEsInfo->rABInfo.rB.u8PtsI;
                            prFbmPicHdr->rExtra.u8AudPTS = prVdecEsInfo->rABInfo.rB.u8AudPts;
                            prFbmPicHdr->rExtra.u8AudOffset = prVdecEsInfo->rABInfo.rB.u8AudOffset;
                            prFbmPicHdr->u4PTS = (UINT32)prVdecEsInfo->rABInfo.rB.u8Pts;
                            prFbmPicHdr->u4TotlaTimeOffset = prVdecEsInfo->u4TotlaTimeOffset;
                            prFbmPicHdr->u8Offset = prVdecEsInfo->rABInfo.rB.u8Offset;
                            prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->rABInfo.rB.u8OffsetDisp;
                            prFbmPicHdr->u8OffsetI = prVdecEsInfo->rABInfo.rB.u8OffsetI;
                            prFbmPicHdr->i4TemporalRef =
                                prVdecEsInfo->rABInfo.rB.i4TemporalRef;
                            prFbmPicHdr->u2DecodingOrder =
                                prVdecEsInfo->rABInfo.rB.u2DecodingOrder;
                            STC_SetStcValue(prVdecEsInfo->ucStcId,
                                (UINT32)(prFbmPicHdr->u4PTS));
                        }
                        else
                        {
                            prFbmPicHdr->rExtra.u8PTSI = prVdecEsInfo->u4TotalTime;
                            prFbmPicHdr->rExtra.u8AudPTS = prVdecEsInfo->u4TotalTime;
                            prFbmPicHdr->rExtra.u8AudOffset = prVdecEsInfo->u8FileSize;
                        prFbmPicHdr->u4PTS = prVdecEsInfo->u4TotalTime;
                        prFbmPicHdr->u4TotlaTimeOffset = prVdecEsInfo->u4TotlaTimeOffset;
                        prFbmPicHdr->u8Offset = prVdecEsInfo->u8FileSize;
                        prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->u8FileSize;
                        prFbmPicHdr->u8OffsetI = prVdecEsInfo->u8FileSize;
                            STC_SetStcValue(prVdecEsInfo->ucStcId,
                                (UINT32)(prFbmPicHdr->u4PTS));
                        }
                    }
                    else
                    {
                        if(prVdecEsInfo->rABInfo.fgValid)
                        {
                            prFbmPicHdr->rExtra.u8PTSI = prVdecEsInfo->rABInfo.rA.u8PtsI;
                            prFbmPicHdr->rExtra.u8AudPTS = prVdecEsInfo->rABInfo.rA.u8AudPts;
                            prFbmPicHdr->rExtra.u8AudOffset = prVdecEsInfo->rABInfo.rA.u8AudOffset;
                            prFbmPicHdr->u4PTS = (UINT32)prVdecEsInfo->rABInfo.rA.u8Pts;
                            prFbmPicHdr->u4TotlaTimeOffset = prVdecEsInfo->u4TotlaTimeOffset;
                            prFbmPicHdr->u8Offset = prVdecEsInfo->rABInfo.rA.u8Offset;
                            prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->rABInfo.rA.u8OffsetDisp;
                            prFbmPicHdr->u8OffsetI = prVdecEsInfo->rABInfo.rA.u8OffsetI;
                            prFbmPicHdr->i4TemporalRef =
                                prVdecEsInfo->rABInfo.rA.i4TemporalRef;
                            prFbmPicHdr->u2DecodingOrder =
                                prVdecEsInfo->rABInfo.rA.u2DecodingOrder;
                            STC_SetStcValue(prVdecEsInfo->ucStcId,
                                (UINT32)(prFbmPicHdr->u4PTS));
                        }
                        else
                        {
                            prFbmPicHdr->rExtra.u8PTSI = 0;
                            prFbmPicHdr->rExtra.u8AudPTS = 0;
                            prFbmPicHdr->rExtra.u8AudOffset = 0;
                            prFbmPicHdr->u4PTS = 0;
                            prFbmPicHdr->u4TotlaTimeOffset = 0;
                            prFbmPicHdr->u8Offset = 0;
                            prFbmPicHdr->u8OffsetDisp = 0;
                            prFbmPicHdr->u8OffsetI = 0;
                            STC_SetStcValue(prVdecEsInfo->ucStcId, (UINT32)(0));
                        }
                    }
                    STC_StartStc(prVdecEsInfo->ucStcId);

                    u4AddrY = 0;
                    u4AddrC = 0;
                    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdecEsInfo->ucFbgId);
                    FBM_GetFrameBufferAddr(prVdecEsInfo->ucFbgId, ucFbId, &u4AddrY, &u4AddrC);
                    if((u4AddrY > 0) && (u4AddrC > 0) && prFbmSeqHdr)
                    {
                        UINT32 u4AlignVSize; // we use 64x32 block
                        u4AlignVSize = ((prFbmSeqHdr->u2VSize + 0x3F) & (~0x3F));
                        x_memset((void*)VIRTUAL(u4AddrY), 0, (prFbmSeqHdr->u2LineSize*u4AlignVSize));
                        x_memset((void*)VIRTUAL(u4AddrC), 128, (prFbmSeqHdr->u2LineSize*u4AlignVSize)/2);
                        #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
                        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4AddrY), (prFbmSeqHdr->u2LineSize*u4AlignVSize));
                        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4AddrC), (prFbmSeqHdr->u2LineSize*u4AlignVSize)>>1);
                        #else
                        HalFlushInvalidateDCache();
                        #endif
                        if(prVdecEsInfoKeep->rPesInfo.fgBackward)
                        {
                            FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_BACKWARD_FLAG);
                        }
                        FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_PSEUDO_EOS_FLAG);
                        #ifdef CC_REALD_3D_SUPPORT
                        _VDEC_Check2D3DSwitch(ucEsId, ucFbId);
                        #endif
                        _VDEC_JobBeforePutDispQ(ucEsId, ucFbId);
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
                if((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
                    ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MJPEG)
                    ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_AVS)
                    ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_RAW)
                    ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_VP8)
                    ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_VP6))
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
                    if(prVdecEsInfoKeep->rPesInfo.fgBackward)
                    {
                        FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_BACKWARD_FLAG);
                    }
                    FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_EOS_FLAG);
                    FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_NOT_DISPLAY_FLAG);
                    //UNUSED(_MPV_CalcPts(ucEsId, ucFbId));
                    #ifdef CC_REALD_3D_SUPPORT
                    _VDEC_Check2D3DSwitch(ucEsId, ucFbId);
                    #endif
                    _VDEC_JobBeforePutDispQ(ucEsId, ucFbId);
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
            if((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV ) ||
                (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MP4)||
                (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264))
	          {
                FBM_CREATE_FBG_PAR_T rPar;
                x_memset(&rPar, 0, sizeof(FBM_CREATE_FBG_PAR_T));
                rPar.u1VdecId = (UINT8)ucEsId;

                if((prVdecEsInfoKeep->fgVPush) && (prVdecEsInfoKeep->fgVPushDecodeOnly))
                {
                    rPar.u1AppMode = FBM_FBG_APP_OMX;
                }
                else if(prVdecEsInfoKeep->fgVPush)
                {
                    rPar.u1AppMode = FBM_FBG_APP_OMX_DISP;
                }
                else
                {
                    rPar.u1AppMode = FBM_FBG_APP_NORMAL;
                }

	              LOG(5, "Create FBG\n");
                  ucFbgType = FBM_SelectGroupType(720, 480);
	              if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV )
	              {
	                    prVdecEsInfo->ucFbgId =
                            FBM_CreateGroupExt(ucFbgType, FBM_VDEC_MPEG2,
                                0, 0, &rPar);
	              }
	              else if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MP4)
	              {
	                    prVdecEsInfo->ucFbgId =
                            FBM_CreateGroupExt(ucFbgType, FBM_VDEC_MPEG4,
                                0, 0, &rPar);
	              }
                  else if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
	              {
	                    prVdecEsInfo->ucFbgId =
                            FBM_CreateGroupExt(ucFbgType, FBM_VDEC_H264,
                                720, 480, &rPar);
	              }
                  if(prVdecEsInfo->fgMMPlayback)
                  {
	                  FBM_SetPlayMode(prVdecEsInfo->ucFbgId,FBM_FBG_MM_MODE);
	              }
                  if(prVdecEsInfoKeep->fgVPush)
                  {
                      if(prVdecEsInfoKeep->fgVPushDecodeOnly)
                      {
                          FBM_SetPlayMode(prVdecEsInfo->ucFbgId,FBM_FBG_NOTDISPLAY_MODE);
                      }
                      FBM_RegFbgCbFunc(prVdecEsInfo->ucFbgId,
                        FBM_CB_FUNC_FB_READY_EX_IND, (UINT32)_VPUSH_DecodeDone, ucEsId);
                  }
                  if(prVdecEsInfo->fgPlaybackNotDisplay)
                  {
                      FBM_SetPlayMode(prVdecEsInfo->ucFbgId,FBM_FBG_NOTDISPLAY_MODE);
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


                    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
                    {
                        ucFbId = FBM_GetEmptyFrameBuffer(prVdecEsInfo->ucFbgId, prVdecEsInfo->u4WaitDispTime);
                    }
                    else
                    {
                        ucFbId = FBM_GetEmptyBFrameBuffer(prVdecEsInfo->ucFbgId,
                                            prVdecEsInfo->u4WaitDispTime);
                    }

                    if(ucFbId != FBM_FB_ID_UNKNOWN)
                    {
                        if(prVdecEsInfoKeep->rPesInfo.fgBackward)
                        {
                            FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_BACKWARD_FLAG);
                        }
                        FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_EOS_FLAG);
                        FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_NOT_DISPLAY_FLAG);
                        #ifdef CC_REALD_3D_SUPPORT
                        _VDEC_Check2D3DSwitch(ucEsId, ucFbId);
                        #endif
                        _VDEC_JobBeforePutDispQ(ucEsId, ucFbId);
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
                LOG(3, "ES(%d) Notify Status change, decode no data\n", ucEsId);
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
                if(prVdecEsInfoKeep->pfnTrickPtsCb)
                {
                    prVdecEsInfoKeep->pfnTrickPtsCb(0, FALSE);
                    LOG(3, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderFromFirstPic\n");
                }
            }
            */
            if(prVdecEsInfoKeep->pfnRenderPtsCb)
            {
                prVdecEsInfoKeep->pfnRenderPtsCb(
                    (prVdecEsInfo->u4TotalTime + prVdecEsInfo->u4TotlaTimeOffset),
                    prVdecEsInfoKeep->u1AttachedSrcId);
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
                if(prVdecEsInfoKeep->pfnTrickPtsCb)
                {
                    prVdecEsInfoKeep->pfnTrickPtsCb(0, FALSE);
                    LOG(3, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderPts\n");
                }
            }
            */
            if(prVdecEsInfoKeep->pfnRenderPtsCb)
            {
                prVdecEsInfoKeep->pfnRenderPtsCb(
                    (prVdecEsInfo->u4TotalTime + prVdecEsInfo->u4TotlaTimeOffset),
                    prVdecEsInfoKeep->u1AttachedSrcId);
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
            if(prVdecEsInfoKeep->pfnTrickPtsCb)
            {
                prVdecEsInfoKeep->pfnTrickPtsCb(0, FALSE);
                LOG(3, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderFromPos\n");
            }
            */
            if(prVdecEsInfoKeep->pfnRenderPtsCb)
            {
                prVdecEsInfoKeep->pfnRenderPtsCb(
                    (prVdecEsInfo->u4TotalTime + prVdecEsInfo->u4TotlaTimeOffset),
                    prVdecEsInfoKeep->u1AttachedSrcId);
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
                if(prVdecEsInfoKeep->pfnTrickPtsCb)
                {
                    prVdecEsInfoKeep->pfnTrickPtsCb(0, FALSE);
                    LOG(3, "_VDEC_DeliverEos pfnTrickPtsCb fgRenderBackStep\n");
                }
            }
            */
            if(prVdecEsInfoKeep->pfnRenderPtsCb)
            {
                prVdecEsInfoKeep->pfnRenderPtsCb(
                    (prVdecEsInfo->u4TotalTime + prVdecEsInfo->u4TotlaTimeOffset),
                    prVdecEsInfoKeep->u1AttachedSrcId);
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
    #ifdef CC_REALD_3D_SUPPORT
    UCHAR ucLockFbId;
    #endif

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

    LOG(1, "Rsv Es%d u4Cmd %x\n", ucEsId, u4Cmd);

    u4PreStatus = (UINT32)prVdecEsInfoKeep->eCurState;

    //filter illegal command
    switch (u4Cmd)
    {
        case VDEC_CMD_STOP:
            prVdecEsInfoKeep->ePreviousState = prVdecEsInfoKeep->eCurState;
            prVdecEsInfoKeep->eCurState = VDEC_ST_STOP;
            prVdecEsInfoKeep->u4PreviousStopId = prVdecEsInfoKeep->u4CurStopId;
            prVdecEsInfoKeep->u4CurStopId ++;
            prVdecEsInfoKeep->fgWaitIFrame = TRUE;
			prVdecEsInfo->fgNoFrameBuffer = FALSE;
            prVdecEsInfo->fgMvcMismatch = FALSE;
            break;
        case VDEC_CMD_PLAY:
        case VDEC_CMD_PLAY_I_FRAME:
            if(prVdecEsInfoKeep->eCurState == VDEC_ST_STOP)
            {
                prVdecEsInfoKeep->ePreviousState = prVdecEsInfoKeep->eCurState;
                prVdecEsInfoKeep->eCurState = (u4Cmd == (UINT32)VDEC_CMD_PLAY) ? VDEC_ST_PLAY : VDEC_ST_PLAY_I_FRAME;
            }
            else if(prVdecEsInfoKeep->eCurState == VDEC_ST_IDLE)
            {
                if(VDEC_ChkCodecCap(ucEsId, prVdecEsInfoKeep->eCurFMT))
                {
                    prVdecEsInfoKeep->ePreviousState = prVdecEsInfoKeep->eCurState;
                    prVdecEsInfoKeep->eCurState = (u4Cmd == (UINT32)VDEC_CMD_PLAY) ? VDEC_ST_PLAY : VDEC_ST_PLAY_I_FRAME;
                    prVdecEsInfo->ucDropBNs = 0;
                    if(prVdecEsInfo->fgRenderPts)   //for seek case
                    {
                        if(prVdecEsInfoKeep->eDecStatus == VDEC_DEC_TRIGGER)
                        {
                            if((prVdecInfo->arVdecVldInfo[prVdecEsInfo->u4VldId].u4LockCount > 0) &&
                               (prVdecInfo->arVdecVldInfo[prVdecEsInfo->u4VldId].ucLockEsId == ucEsId))
                            {
                                // Unlock VLD to keep lock/unlock in pairs
                                _VDEC_UnlockVld(ucEsId, (UCHAR)prVdecEsInfo->u4VldId);
                            }
                        }
                        prVdecEsInfoKeep->eDecStatus = VDEC_DEC_HEADER_PARSE;
                    }
                }
            }
            break;
        case VDEC_CMD_PAUSE:
            if (prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY)
            {
                prVdecEsInfoKeep->ePreviousState = prVdecEsInfoKeep->eCurState;
                prVdecEsInfoKeep->eCurState = VDEC_ST_IDLE;
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
            if(prVdecEsInfoKeep->pfnVdecStop)
            {
                prVdecEsInfoKeep->pfnVdecStop(ucEsId);
            }
            prVdecEsInfo->ucFbgId = 0xFF;
            prVdecEsInfoKeep->eDecStatus = VDEC_DEC_HEADER_PARSE;
            break;
        case VDEC_CMD_FLUSH_DPB:
        	  _VdecFlushDPB(ucEsId);
        	  break;
        case VDEC_CMD_FLUSH_ESMQ:
            if (prVdecEsInfo->fgMMPlayback)
            {
                //clear queue, clear stored info, do notification
                _VDEC_LockFlushMutex(ucEsId);
                if (prVdecEsInfo->fgFlushEsmQ)
                {
                    LOG(1, "ES(%d) VDEC_CMD_FLUSH_ESMQ Flush\n", ucEsId);
                    _VDEC_FlushEs(ucEsId);
                    prVdecEsInfo->fgFlushEsmQ = FALSE;
                    _VDEC_UnlockFlushSema(ucEsId);
                }
                _VDEC_UnlockFlushMutex(ucEsId);
            }
            break;
        #ifdef CC_REALD_3D_SUPPORT
        case VDEC_CMD_3D_MODE_CHANGE:
            prVdecEsInfo->u13DUsrCtrlMode = prVdecEsInfo->u13DUsrCtrlModeNtfy;
            prVdecEsInfo->u1IsForceLView = prVdecEsInfo->u1IsForceLViewNtfy;

            if(prVdecEsInfoKeep->eCurState == VDEC_ST_IDLE)
            {
                ucLockFbId = VDP_FrcGetLockFrameBuffer(ucEsId);
                if(ucLockFbId == 0xFF)
                {
                    #ifdef CC_B2R_SKYPE_SUPPORT
                    ucLockFbId = VDP_GetPendingFB(0);
                    #else
                    ucLockFbId = VDP_GetPendingFB();
                    #endif
                }
                if(ucLockFbId == 0xFF)
                {
                    //ASSERT(ucLockFbId != 0xFF);
                    LOG(3, "ucLockFbId == 0xFF\n");
                }
                else
                {
                    _VDEC_Check2D3DSwitch(ucEsId, ucLockFbId);
                }
            }
            break;
        #endif
        default:
            break;
    }

    if ((UINT32)prVdecEsInfoKeep->eCurState != u4PreStatus)
    {
        LOG(1, "Es%d Status Change %x to %x\n", ucEsId, u4PreStatus, prVdecEsInfoKeep->eCurState);
    }
}

VOID _VDEC_SourceStateNty(UCHAR ucEsId, UINT32 u4State)
{
    VDEC_ES_INFO_T* prVdecEsInfo;
    ENUM_VDEC_SCRAMBLE_STATE_T eTmpState = VDEC_SCRAMBLE_STATE_UNKNOWN;
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if (NULL == prVdecEsInfo)
    {
        LOG(1, "Get ES info failed... ...\n");
        return ;
    }

    switch (u4State)
    {
    case DMX_SCRAMBLE_STATE_CLEAR:
        eTmpState = VDEC_SCRAMBLE_STATE_CLEAR;
        LOG(5, "Source will enter into Clear state...\n");
        break;
    case DMX_SCRAMBLE_STATE_SCRAMBLED:
        eTmpState = VDEC_SCRAMBLE_STATE_SCRAMBLED;
        LOG(5, "Source will enter into Scramble state...\n");
        break;
    default:
        eTmpState = VDEC_SCRAMBLE_STATE_UNKNOWN;
        LOG(5, "Source will enter into Unknown state...\n");
    }

    prVdecEsInfo->eCurSourceState = eTmpState;
 
    return ;
}

VOID _VDEC_HandleSourceState(UCHAR ucEsId)
{
    VDEC_ES_INFO_T* prVdecEsInfo;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if (NULL == prVdecEsInfo)
    {
        LOG(1, "Get ES info failed... ...\n");
        return ;
    }

    if (prVdecEsInfo->ePerSourceState != prVdecEsInfo->eCurSourceState)
    {
        if (VDEC_SCRAMBLE_STATE_SCRAMBLED == prVdecEsInfo->eCurSourceState)
        {
            // FBM_SetFrameBufferFlag will ignore unknown id, so we do not judge it
            LOG(5, "VDEC enter into source scramble state... ...\n");
            FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SOURCE_SCRAMBLED);
            FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);
        }
        else if (VDEC_SCRAMBLE_STATE_CLEAR == prVdecEsInfo->eCurSourceState)
        {
            // FBM_SetFrameBufferFlag will ignore unknown id, so we do not judge it
            LOG(5, "VDEC enter into source clear state... ...\n");
            FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SOURCE_SCRAMBLED);
            // FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, ucEsId);    // Notify will report when vdec finds the seq hdr
        }
        else
        {
            LOG(2, "UnHandle state(%d)\n", prVdecEsInfo->eCurSourceState);
        }

        prVdecEsInfo->ePerSourceState = prVdecEsInfo->eCurSourceState;
    }
    
    return ;
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
    VDEC_ES_INFO_T* prVdecEsInfoSub;
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

    HANDLE_T hMsgQ;

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
    if(prVdecEsInfoKeep->pfnVdecGetPesStruct)    //h264
    {
        prVdecEsInfoKeep->pfnVdecGetPesStruct(ucEsId, (void**)&prPesInfo);
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

    // es case will not have EsId > 0, Sub Q is useless
    if(((prVdecEsInfo->e3DType == VDEC_3D_MVC) ||
        (prVdecEsInfo->e3DType == VDEC_3D_REALD_DUAL_STREAM))
        &&
#ifdef ENABLE_MULTIMEDIA
        (prVdecEsInfo->eContainerType != SWDMX_FMT_H264_VIDEO_ES) &&
#endif
        prVdecEsInfo->fgIsDependView)
    {
        hMsgQ = prVdecEsInfoKeep->hMsgQSub;
    }
    else
    {
        hMsgQ = prVdecEsInfoKeep->hMsgQ;
    }

    // For DTV, Source Status should report to FBM
    #if 0   // Vestel will get state by VDEC_GetSrcState, VDEC will not send state to FBM
    if ((FALSE == prVdecEsInfo->fgMMPlayback) &&
        (prVdecEsInfo->ePerSourceState != prVdecEsInfo->eCurSourceState) )
    {
        UINT32 u4QCount = 0;
        VERIFY(x_msg_q_num_msgs(hMsgQ, &u4QCount) == OSR_OK);

        if (((0 == u4QCount) && (VDEC_SCRAMBLE_STATE_SCRAMBLED == prVdecEsInfo->eCurSourceState)) ||    // Clear to scramble, report after processed all the frame
            (VDEC_SCRAMBLE_STATE_CLEAR == prVdecEsInfo->eCurSourceState))                               // scramble to Clear, report immediately
        {
            _VDEC_HandleSourceState(ucEsId);
        }       
    }
    #endif
    
    if(prVdecEsInfoKeep->eCurFMT != VDEC_FMT_H264 || prVdecEsInfo->fgNoFrameBuffer == FALSE)
    {
        VERIFY(x_msg_q_receive(&u2MsgQIdx, &prVdecEsInfoKeep->rPesInfo, &zMsgSize,
            &(hMsgQ), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
    }
    else
    {
       prVdecEsInfo->fgNoFrameBuffer = FALSE;
       LOG(3,"previous es not decoded yet.\n");
    }

#ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeRstE);
    HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
    LOG(6, "VDec Receieve ES Delta T = (%ld), %d.%6d \n", rTimeRstDt.u4Micros,
        rTimeRstE.u4Seconds, rTimeRstE.u4Micros);
#endif

#ifdef SWDMX_DBG_USB
    do
    {
        static UINT32 u4Addr = 0;
        if (USB_DUMP_ON(DUMP_DATA_DMX_SWDMX_VDEC))
        {
            static UINT32 u4EsCount = 0;
            static CHAR ucFileName[NAME_MAX_LEN] = {0};
            
            if (0!=u4Addr && u4Addr <= prVdecEsInfoKeep->rPesInfo.u4VldReadPtr)
            {
                x_sprintf(ucFileName, "%s/VES_%010u.raw", FILE_LOCAL, u4EsCount);
                Dbg_SaveBlock(ucFileName, VIRTUAL(u4Addr), prVdecEsInfoKeep->rPesInfo.u4VldReadPtr-u4Addr);
                if (DUMP_LOG)
                {
                    LOG(1, "VEs   KAddr:0x%08x VAddr:0x%08x Size=%u\n", u4Addr, VIRTUAL(u4Addr), 
                        prVdecEsInfoKeep->rPesInfo.u4VldReadPtr-u4Addr);
                }
            }
            else if (0 != u4Addr)
            {
                x_sprintf(ucFileName, "%s/VEs_%010u_1.raw", FILE_LOCAL, u4EsCount);
                Dbg_SaveBlock(ucFileName, VIRTUAL(u4Addr), prVdecEsInfoKeep->rPesInfo.u4FifoEnd-u4Addr);
                if (DUMP_LOG)
                {
                    LOG(1, "VES_1 KAddr:0x%08x VAddr:0x%08x Size=%u\n", u4Addr, VIRTUAL(u4Addr), 
                        prVdecEsInfoKeep->rPesInfo.u4FifoEnd-u4Addr);
                }

                x_sprintf(ucFileName, "%s/VEs_%010u_2.raw", FILE_LOCAL, u4EsCount);
                Dbg_SaveBlock(ucFileName, VIRTUAL(prVdecEsInfoKeep->rPesInfo.u4FifoStart), 
                    prVdecEsInfoKeep->rPesInfo.u4VldReadPtr-prVdecEsInfoKeep->rPesInfo.u4FifoStart);
                if (DUMP_LOG)
                {
                    LOG(1, "VEs_2 KAddr:0x%08x VAddr:0x%08x Size=%u\n", u4Addr, VIRTUAL(prVdecEsInfoKeep->rPesInfo.u4FifoStart), 
                        prVdecEsInfoKeep->rPesInfo.u4VldReadPtr-prVdecEsInfoKeep->rPesInfo.u4FifoStart);    
                }
            }

            u4EsCount ++;
        }

        u4Addr = prVdecEsInfoKeep->rPesInfo.u4VldReadPtr;
    }while(FALSE);
#endif

    if(prPesInfo)    //h264
    {
        if(!prVdecEsInfoKeep->rPesInfo.fgEos)
        {
            ASSERT((prVdecEsInfoKeep->rPesInfo.u4VldReadPtr <= prVdecEsInfoKeep->rPesInfo.u4FifoEnd) &&
                (prVdecEsInfoKeep->rPesInfo.u4VldReadPtr >= prVdecEsInfoKeep->rPesInfo.u4FifoStart));
        }

        u4Pts = prPesInfo->u4PTS;
        u4Dts = prPesInfo->u4DTS;
        x_memcpy(prPesInfo, &prVdecEsInfoKeep->rPesInfo, sizeof(prVdecEsInfoKeep->rPesInfo));

        if(!prVdecEsInfoKeep->rPesInfo.fgDtsValid)
        {
            prPesInfo->u4PTS = u4Pts;
            prPesInfo->u4DTS = u4Dts;
        }
        //error check
        if((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264VER) &&
            ((prPesInfo->ucPicType > 0) && (u4VldReadPtr == prPesInfo->u4VldReadPtr)))
        {
            LOG(5, "Rsv Dup Rptr 0x%x\n", prPesInfo->u4VldReadPtr);
            return VDEC_MAX_VLD;
        }
        u4VldReadPtr = prPesInfo->u4VldReadPtr;

        //log receive pesInfo timing
        if(((prPesInfo->ucPicType & 0x1F) == 0x5)||((prPesInfo->ucPicType & 0x1F) == 0x1))
        	{
        if (STC_GetSrc(prVdecEsInfo->ucStcId, &rStcClk) == STC_VALID)
        {
            LOG(6, "R%x, S%x, P%x\n",prPesInfo->u4PTS, 
                rStcClk.u4Base, prPesInfo->ucPicType & 0x1F);
        }
		else
		{
			LOG(6, "R%x, P%x\n",prPesInfo->u4PTS,prPesInfo->ucPicType & 0x1F);
		}
        	}
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
        if(prVdecEsInfoKeep->pfnVdecSetParam)
        {
            prVdecEsInfoKeep->pfnVdecSetParam(ucEsId, (UINT32)MM_PARAM, (UINT32)(prVdecEsInfo->fgMMPlayback?1:0), 0, 0);
        }
        if((prVdecEsInfo->fgMMPlayback && prPesInfo->fgEos)
            && (!IS_H264_PIC_NALU(prPesInfo->ucPicType))
            && (!IS_MVC_DEPEND_PIC_NALU(prPesInfo->ucPicType)))
        {
            return VDEC_MAX_VLD;
        }
        #endif
        #endif
    }
    VERIFY(x_msg_q_num_msgs(hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
    VERIFY(x_sema_unlock(prVdecEsInfoKeep->hMutex) == OSR_OK);

    ASSERT(zMsgSize == sizeof(VDEC_PES_INFO_T));
    prMpvCounter->u4EsmQCnt = prVdecEsInfo->u2Count;

    //check if flush FB
    if(prVdecEsInfo->fgFlushFB)
    {
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
        if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
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
        LOG(3, "ES(%d) _VDecReceiveEs Flush\n", ucEsId);
        _VDEC_FlushEs(ucEsId);
        prVdecEsInfo->fgFlushEsmQ = FALSE;
        _VDEC_UnlockFlushSema(ucEsId);
        _VDEC_UnlockFlushMutex(ucEsId);
        prVdecEsInfo->fgIsDependView = FALSE;
        return VDEC_MAX_VLD;
    }
    _VDEC_UnlockFlushMutex(ucEsId);

    ucMpvId = prVdecEsInfoKeep->rPesInfo.ucMpvId;

#ifdef __MODEL_slt__
    if (fgSltWatchPts)
    {
        if ( (u4SltPts == prVdecEsInfoKeep->rPesInfo.u4PTS) && (VDEC_IsLock(prVdecEsInfoKeep->rPesInfo.ucEsId)))
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

    ucMpvId = prVdecEsInfoKeep->rPesInfo.ucMpvId;
    #ifdef CC_VDEC_FMT_DETECT
    u4Type = prVdecEsInfoKeep->rPesInfo.u4DmxFrameType;
    #endif

    // Handle Virtual Picture Header for Event Trigger
    if (ucMpvId >= VDEC_MAX_VLD)
    {
        return VDEC_MAX_VLD;
    }
    else if(prVdecEsInfoKeep->fgWaitIFrame)
    {
        if (prVdecEsInfoKeep->pfDecNfyFct)
           {
               LOG(3, "Notify Status change, wait i frame\n");
               prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                 VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_RECEIVE_DATA, 0);
           }
        prVdecEsInfoKeep->fgWaitIFrame = FALSE;
    }

    // set this in VDEC_Play
    //prVdecEsInfo->u4VldId = ucMpvId;

    #ifdef CC_VDEC_FMT_DETECT
    // Handle start code / codec type mismatch
    else if(u4Type > 0)
    {
        if(H264_PIC_VALID_TYPE(u4Type))
        {
            u4DmxType = (UINT32)VDEC_FMT_H264;
            if((u4Type != H264_PIC_TYPE_SEQ) &&
                (prVdecEsInfo->arRcvPesCnt[u4DmxType] == 0) &&
                (u4DmxType != (UINT32)prVdecEsInfoKeep->eCurFMT))
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

        if((u4DmxType < (UINT32)VDEC_FMT_MAX) && (prVdecEsInfoKeep->eCurFMT < VDEC_FMT_MAX))
        {
            prVdecEsInfo->arRcvPesCnt[u4DmxType]++;
            if(prVdecEsInfo->arRcvPesCnt[u4DmxType] == 0xFF)
            {
                x_memset(prVdecEsInfo->arRcvPesCnt, 0, sizeof(prVdecEsInfo->arRcvPesCnt));
            }

            if(u4DmxType != (UINT32)prVdecEsInfoKeep->eCurFMT)
            {
                LOG(6, "detec (type,num)=(%d,%d), cur(%d,%d)\n",
                    u4DmxType, prVdecEsInfo->arRcvPesCnt[u4DmxType],
                    prVdecEsInfoKeep->eCurFMT, prVdecEsInfo->arRcvPesCnt[prVdecEsInfoKeep->eCurFMT]);
                //if(!prVdecEsInfo->fgFmtDectFinish)
                {
                    u4Thrd = (prMpvCounter->u4DecOkNs > CODEC_DETECT_THRD) ? prMpvCounter->u4DecOkNs : CODEC_DETECT_THRD;
                    if((prVdecEsInfo->arRcvPesCnt[u4DmxType] > prVdecEsInfo->arRcvPesCnt[(UINT32)prVdecEsInfoKeep->eCurFMT]) &&
                        ((prVdecEsInfo->arRcvPesCnt[u4DmxType] - prVdecEsInfo->arRcvPesCnt[(UINT32)prVdecEsInfoKeep->eCurFMT])
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
                            case VDEC_FMT_VP6:
                                u4NtfyCodecType = VDEC_CODEC_VP6;
                                break;
                            case VDEC_FMT_VP8:
                                u4NtfyCodecType = VDEC_CODEC_VP8;
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
                                LOG(3, "ES(%d) Notify Status change, codec type change, type %d\n", ucEsId, u4DmxType);
                                eNfyStatus = VDEC_DEC_DECODE_CODEC_TYPE_CHG;
                            }
                            else
                            {
                                LOG(3, "ES(%d) Notify Status change, codec not support\n", ucEsId);
                                eNfyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
                                prVdecEsInfoKeep->eCurState = VDEC_ST_IDLE;
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
    //SYSLOG(ucEsId, 2, prVdecEsInfo->u2Count, prVdecEsInfoKeep->rPesInfo.ucPicType);

    ASSERT(ucMpvId < VDEC_MAX_VLD);
    //VERIFY(prVdecEsInfoKeep->rPesInfo.ucPicType > 0);

    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
    {
        #ifdef VDEC_TIME_PROFILE
        if (prVdecEsInfoKeep->pfnVdecIsPic)
        {
            if (prVdecEsInfoKeep->pfnVdecIsPic(ucEsId, &prVdecEsInfoKeep->rPesInfo))
            {
                _VDEC_CalPicSize(ucEsId);
            }
        }
        #endif
        if((prVdecEsInfoKeep->rPesInfo.ucPicType & 0x1f) <= 0x5)
        {
            prMpvCounter->u4RetrieveNs++;

        }
        //check dependent view
        if((prVdecEsInfo->e3DType == VDEC_3D_MVC) ||
            (prVdecEsInfo->e3DType == VDEC_3D_REALD_DUAL_STREAM)
#if 0//def ENABLE_MULTIMEDIA
        	&& (prVdecEsInfo->eContainerType != SWDMX_FMT_H264_VIDEO_ES)
#endif
        	)
        {
            prVdecEsInfoSub = _VDEC_GetEsInfo(ES1);    //dependent view
            if((prVdecEsInfoSub->e3DType == VDEC_3D_MVC) ||
                (prVdecEsInfo->e3DType == VDEC_3D_REALD_DUAL_STREAM))
            {
                if(IS_AVC_PIC_NALU(prVdecEsInfoKeep->rPesInfo.ucPicType) &&
                    (prVdecEsInfoKeep->rPesInfo.ucEsId == 0))
                {
                    prVdecEsInfo->fgIsDependView = TRUE;
                    if(prVdecEsInfo->fgMvcMismatch)
                    {
                       return VDEC_MAX_VLD;
                    }
                    prVdecEsInfo->u4BasePTS = prVdecEsInfoKeep->rPesInfo.u4PTS;
                }
                else if(IS_MVC_DEPEND_PIC_NALU(prVdecEsInfoKeep->rPesInfo.ucPicType) ||
                    (IS_AVC_PIC_NALU(prVdecEsInfoKeep->rPesInfo.ucPicType) &&
                    (prVdecEsInfoKeep->rPesInfo.ucEsId > 0)))
                {
                    if(prVdecEsInfo->fgMMPlayback &&
                        (prVdecEsInfo->u4MMSpeed == STC_SPEED_TYPE_FORWARD_1X) &&
                        (prVdecEsInfoSub->e3DType == VDEC_3D_MVC))
                    {
                        if((prVdecEsInfo->u4BasePTS > 0) &&
                            (prVdecEsInfoKeep->rPesInfo.u4PTS != prVdecEsInfo->u4BasePTS))
                        {
                            LOG(3, "PTS mismatch, L 0x%x/R 0x%x\n", prVdecEsInfo->u4BasePTS, prVdecEsInfoKeep->rPesInfo.u4PTS);
                            prVdecEsInfo->fgMvcMismatch = TRUE;
                            if(_VDEC_MVCFindLRPair(ucEsId))
            {
                                LOG(3, "Auto correction done.\n");
                                prVdecEsInfo->fgIsDependView = FALSE;
                                prVdecEsInfo->fgMvcMismatch = FALSE;
                                return VDEC_MAX_VLD;
            }
                            if(prVdecEsInfoKeep->rPesInfo.u4PTS != prVdecEsInfo->u4BasePTS) //possible to fix in _VDEC_MVCFindLRPair
            {
                                LOG(3, "Skip\n");
                                prVdecEsInfo->fgIsDependView = FALSE;
                                return VDEC_MAX_VLD;
                            }
                            LOG(3, "over pts mismatch\n");
                        }
                    }
                    prVdecEsInfo->fgIsDependView = FALSE;
                    //prVdecEsInfoSub = _VDEC_GetEsInfo(ES1);    //dependent view
                prVdecEsInfoSub->u8Offset = prVdecEsInfoKeep->rPesInfo.u8Offset;
            }
                if(prVdecEsInfo->e3DType == VDEC_3D_REALD_DUAL_STREAM)
                {
                    UCHAR ucBaseDispQNum = 0, ucDepDispQNum = 0;
                    ucBaseDispQNum = FBM_CheckFrameBufferDispQ(prVdecEsInfo->ucFbgId);
                    ucDepDispQNum = FBM_CheckFrameBufferDispQ(prVdecEsInfo->ucSubFbgId);

                    if(ucBaseDispQNum > ucDepDispQNum)
                    {
                        prVdecEsInfo->fgIsDependView = TRUE;
                        //LOG(2, "Force Depend View. Base DispQ %d > %d\n", ucBaseDispQNum, ucDepDispQNum);
        }
                    else if(ucBaseDispQNum < ucDepDispQNum)
                    {
                        prVdecEsInfo->fgIsDependView = FALSE;
                        //LOG(2, "Force Base View. Dep DispQ %d > %d\n", ucDepDispQNum, ucBaseDispQNum);
    }
    }
            }
        }
    }
    else
    {
#ifdef VDEC_TIME_PROFILE
        if (prVdecEsInfoKeep->pfnVdecIsPic)
        {
            if (prVdecEsInfoKeep->pfnVdecIsPic(ucEsId, &prVdecEsInfoKeep->rPesInfo))
            {
                _VDEC_CalPicSize(ucEsId);
            }
        }
#endif
        prMpvCounter->u4RetrieveNs++;
    }

    prVdecEsInfo->u4FifoStart = prVdecEsInfoKeep->rPesInfo.u4FifoStart;
    prVdecEsInfo->u4FifoEnd = prVdecEsInfoKeep->rPesInfo.u4FifoEnd;
    prVdecEsInfo->u4VldReadPtr = prVdecEsInfoKeep->rPesInfo.u4VldReadPtr;
    prVdecEsInfo->u4VldReadPtrEnd = prVdecEsInfoKeep->rPesInfo.u4VldReadPtrEnd;
    prVdecEsInfo->u4PTS = prVdecEsInfoKeep->rPesInfo.u4PTS;
    prVdecEsInfo->u4DTS = prVdecEsInfoKeep->rPesInfo.u4DTS;
    prVdecEsInfo->ucPicType = prVdecEsInfoKeep->rPesInfo.ucPicType;
    prVdecEsInfo->fgDtsValid = prVdecEsInfoKeep->rPesInfo.fgDtsValid;
    prVdecEsInfo->fgSeqHdr = prVdecEsInfoKeep->rPesInfo.fgSeqHdr;
    prVdecEsInfo->u8Offset = prVdecEsInfoKeep->rPesInfo.u8Offset;
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
    prVdecEsInfo->rExtra = prVdecEsInfoKeep->rPesInfo.rExtra;
    prVdecEsInfo->u8OffsetI = prVdecEsInfoKeep->rPesInfo.u8OffsetI;
    prVdecEsInfo->u4TickNum = prVdecEsInfoKeep->rPesInfo.u4TickNum;
    prVdecEsInfo->u2DecodingOrder = prVdecEsInfoKeep->rPesInfo.u2DecodingOrder;
    prVdecEsInfo->u4TotlaTimeOffset = prVdecEsInfoKeep->rPesInfo.u4TotalTimeOffset;
    prVdecEsInfo->u4Timestap = prVdecEsInfoKeep->rPesInfo.u4Timestap;

    if((prVdecEsInfo->fgMMPlayback) &&
        (prVdecEsInfoKeep->rPesInfo.fgEos) &&
        (prVdecEsInfo->u4FifoStart == 0) &&
        (prVdecEsInfo->u4FifoEnd == 0) &&
        (prVdecEsInfo->u4VldReadPtr == 0))
    {
        // EOS and without any accessable video frame
        _VDEC_DeliverEos(ucEsId);
        return VDEC_MAX_VLD;
    }

    if ((prVdecEsInfoKeep->fgDmxPartialPes) && (prVdecEsInfo->fgChgThrsd)
        && (prVdecEsInfoKeep->eCurState != VDEC_ST_PLAY_I_FRAME))
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
                LOG(7, "Skip mode 2x -> I only\n");
                ucSkipMode = IMode;
            }
            else
#endif
            {
                LOG(7, "Skip mode -> I/P\n");
                ucSkipMode = IPMode;
            }
            break;
        case STC_SPEED_TYPE_FORWARD_3X:
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
        case STC_SPEED_TYPE_REWIND_3X:
        case STC_SPEED_TYPE_REWIND_4X:
        case STC_SPEED_TYPE_REWIND_8X:
        case STC_SPEED_TYPE_REWIND_16X:
        case STC_SPEED_TYPE_REWIND_32X:
        case STC_SPEED_TYPE_REWIND_64X:
        case STC_SPEED_TYPE_REWIND_128X:
        case STC_SPEED_TYPE_REWIND_256X:
        case STC_SPEED_TYPE_REWIND_512X:
        case STC_SPEED_TYPE_REWIND_1024X:
            LOG(7, "(%d) Skip mode -> I only\n", ucEsId);
            ucSkipMode = IMode;
            break;
        default:
            LOG(7, "(%d) Skip mode -> I/P/B\n", ucEsId);
            ucSkipMode = IPBMode;
            break;
        }
    }
#if defined(CC_USE_DDI)
    else
    {
        switch (prVdecEsInfo->u4MMSpeed)
        {
        case STC_SPEED_TYPE_FORWARD_2X:
            LOG(7, "(%d) Skip mode -> I/P\n", ucEsId);
            ucSkipMode = IPMode;
            break;
        case STC_SPEED_TYPE_FORWARD_4X:
            LOG(7, "(%d) Skip mode -> I only\n", ucEsId);
            ucSkipMode = IMode;
            break;
        default:
            LOG(7, "(%d) Skip mode -> I/P/B\n", ucEsId);
            ucSkipMode = IPBMode;
            break;
        }
    }
#endif

    #ifdef CC_B2R_3D_ERROR_SUPPORT
    if(ucSkipMode != IPBMode)
    {
        if(!FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_3D_ERROR_HANDLE_DISABLE))
        {
            FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_3D_ERROR_HANDLE_DISABLE);
        }
    }
    #endif

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
    if(prVdecEsInfoKeep->pfnVdecEsFlwCtrl)
    {
        if (prVdecEsInfo->u2Count <= prVdecEsInfo->u2UdfNs)
        {
            i1EsBufStatus = -1;
        }
        else if (prVdecEsInfo->ucDropBNs > 0)
        {
            i1EsBufStatus = 1;
        }

		if(prVdecEsInfo->u1LastSkipType==IMode&&ucSkipMode!=IMode)
		{
			SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
		}
		
		prVdecEsInfo->u1LastSkipType=ucSkipMode;
        if(prVdecEsInfoKeep->pfnVdecEsFlwCtrl(ucEsId, &ucMpvId, i1EsBufStatus, ucSkipMode))
        {
            if((i1EsBufStatus > 0) && (prVdecEsInfo->ucDropBNs > 0))
            {
                prVdecEsInfo->ucDropBNs--;
            }
            return VDEC_MAX_VLD;//_VDecReceiveEs(ucEsId);
        }
		else
		{
            if(ucSkipMode==IMode
				&&prVdecEsInfo->u4MMSpeed<STC_SPEED_TYPE_REWIND_1X
				&&prVdecEsInfo->eStreamType==VDEC_STREAM_TYPE_IPB)
            {
               UINT32 u4DeltaPts=0;
			   u4DeltaPts=(90000/VDP_GetFrameRate(ucEsId))*VDP_TrickModeSkipFrameNo(ucEsId);
			   if((prVdecEsInfo->u4PTSLast!=prVdecEsInfoKeep->rPesInfo.u4PTS)
			   	  &&(prVdecEsInfo->u4PTSLast+u4DeltaPts>prVdecEsInfoKeep->rPesInfo.u4PTS)&&
			   	  (!((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)&&(prVdecEsInfo->eContainerType>= SWDMX_FMT_MPEG2_TS) && (prVdecEsInfo->eContainerType< SWDMX_FMT_VC1_ES))))
			   {
				   if((i1EsBufStatus > 0) && (prVdecEsInfo->ucDropBNs > 0))
				   {
					   prVdecEsInfo->ucDropBNs--;
    }
				   return VDEC_MAX_VLD;//_VDecReceiveEs(ucEsId);
			   }
			   else
			   {
                  prVdecEsInfo->u4PTSLast=prVdecEsInfoKeep->rPesInfo.u4PTS;
			   }
            }
		}
    }

    return (ucMpvId);
}



UCHAR _VDEC_HandleOverFlw(UCHAR ucVldId, UCHAR ucEsmQId, HANDLE_T hMutex,
    HANDLE_T hMsgQueue, const void* pvPesInfo)
{
    ASSERT(pvPesInfo!=NULL);
    //temp code
    do
    {
        x_thread_delay(1);
    }
    while (x_msg_q_send(hMsgQueue, pvPesInfo, sizeof(VDEC_PES_INFO_T), 255) != OSR_OK);

    UNUSED(ucVldId);
    UNUSED(ucEsmQId);
    UNUSED(hMutex);
    return 1;
}

INT32 _VDEC_DecCfg(UCHAR ucEsId)
{
    INT32 i4Ret = (INT32)E_VDEC_OK;
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    _VDEC_RegIsr(VECTOR_VDEC, NULL);
    switch(prVdecEsInfoKeep->eCurFMT)
    {
        case VDEC_FMT_H264VER:
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
            prVdecEsInfoKeep->pfnVdecSWRst = _VDEC_H264SWRst;
            prVdecEsInfoKeep->pfnVdecDecode = NULL;
            prVdecEsInfoKeep->pfnVdecHdrParse = _VDEC_H264HdrPrsNalu;//_VDEC_H264HdrPrsPic;
            prVdecEsInfoKeep->pfnVdecDecStart = _VDEC_H264DecStart;
            prVdecEsInfoKeep->pfnVdecDecFin = _VDEC_H264DecFin;
            prVdecEsInfoKeep->pfnVdecDecDrop = NULL;
            prVdecEsInfoKeep->pfnVdecStop = _VDEC_H264Stop;
            prVdecEsInfoKeep->pfnVdecGetPesStruct = _VDEC_H264GetPesStruct; // should not need this?
            prVdecEsInfoKeep->pfnVdecGetDispPic = _VDEC_H264GetDispPic;
            prVdecEsInfoKeep->pfnVdecSetParam = _VDEC_H264SetParam;
            prVdecEsInfoKeep->pfnVdecGetParam = _VDEC_H264GetParam;
            prVdecEsInfoKeep->pfnVdecUpdRptr = NULL;
            prVdecEsInfoKeep->pfnVdecSetWptr = NULL;
            prVdecEsInfoKeep->pfnVdecEsFlwCtrl = NULL;
            prVdecEsInfoKeep->pfnVdecWaitRandomAP = NULL;
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = _VDEC_H264HandleOverFlw;
            prVdecEsInfoKeep->pfnVdecStartPts = NULL;
            prVdecEsInfoKeep->pfnVdecIsPic = NULL;
            //init function
            i4Ret = _VDEC_H264Init(ucEsId);
            if(prVdecEsInfoKeep->pfnVdecSetParam)
            {
                prVdecEsInfoKeep->pfnVdecSetParam(ucEsId, (UINT32)WAIT_FB_TIME, VDEC_WAIT_DISP_TIME, 0, 0);
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
            prVdecEsInfoKeep->pfnVdecSWRst = _VDEC_H264SWRst;
            prVdecEsInfoKeep->pfnVdecDecode = NULL;
            prVdecEsInfoKeep->pfnVdecHdrParse = _VDEC_H264HdrPrsNalu;//_VDEC_H264HdrPrsPic;
            prVdecEsInfoKeep->pfnVdecDecStart = _VDEC_H264DecStart;
            prVdecEsInfoKeep->pfnVdecDecFin = _VDEC_H264DecFin;
            prVdecEsInfoKeep->pfnVdecDecDrop = NULL;
            prVdecEsInfoKeep->pfnVdecStop = _VDEC_H264Stop;
            prVdecEsInfoKeep->pfnVdecGetPesStruct = _VDEC_H264GetPesStruct; // should not need this?
            prVdecEsInfoKeep->pfnVdecGetDispPic = _VDEC_H264GetDispPic;
            prVdecEsInfoKeep->pfnVdecSetParam = _VDEC_H264SetParam;
            prVdecEsInfoKeep->pfnVdecGetParam = _VDEC_H264GetParam;
            prVdecEsInfoKeep->pfnVdecUpdRptr = NULL;
            prVdecEsInfoKeep->pfnVdecSetWptr = _VDEC_H264SetDmxWptr;
            prVdecEsInfoKeep->pfnVdecEsFlwCtrl = _VDEC_H264EsFlwCtrl;
            prVdecEsInfoKeep->pfnVdecWaitRandomAP = NULL;
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = _VDEC_H264HandleOverFlw;
            prVdecEsInfoKeep->pfnVdecStartPts = _VDEC_H264StartPTS;
            #ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecIsPic = _VDEC_H264IsPic;
            #else
		        prVdecEsInfoKeep->pfnVdecIsPic = NULL;
            #endif
            //init function
            i4Ret = _VDEC_H264Init(ucEsId);
            if(prVdecEsInfoKeep->pfnVdecSetParam)
            {
                prVdecEsInfoKeep->pfnVdecSetParam(ucEsId, (UINT32)WAIT_FB_TIME, prVdecEsInfo->u4WaitDispTime, 0, 0);
                prVdecEsInfoKeep->pfnVdecSetParam(ucEsId, (UINT32)AV_SYNC_INFO, prVdecEsInfo->ucSyncMode, prVdecEsInfo->ucStcId, 0);
            }
			prVdecEsInfo->fgNoFrameBuffer = FALSE;
        #endif
        #endif
            break;
        case VDEC_FMT_MPV:
            prVdecEsInfoKeep->pfnVdecSWRst = _MPV_DecReset;
            prVdecEsInfoKeep->pfnVdecDecode = _MPV_Decode;
            prVdecEsInfoKeep->pfnVdecHdrParse = _MPV_VParser;
            prVdecEsInfoKeep->pfnVdecDecStart = _MPV_DecodeProc;
            prVdecEsInfoKeep->pfnVdecDecFin = _MPV_WaitDecodeProc;
            prVdecEsInfoKeep->pfnVdecDecDrop = _MPV_DropPic;
            prVdecEsInfoKeep->pfnVdecStop = _MPV_StopDecES;
            prVdecEsInfoKeep->pfnVdecGetPesStruct = NULL;
            prVdecEsInfoKeep->pfnVdecGetDispPic = NULL;
            prVdecEsInfoKeep->pfnVdecSetParam = NULL;
#ifdef VDEC_TIME_PROFILE
			prVdecEsInfoKeep->pfnVdecGetParam = _MPV_GetParam;
#else
		    prVdecEsInfoKeep->pfnVdecGetParam = NULL;
#endif

            prVdecEsInfoKeep->pfnVdecEsFlwCtrl = MPV_EsFlwCtrl;
            prVdecEsInfoKeep->pfnVdecWaitRandomAP = _MPV_WaitRandomAP;
            #ifndef MPV_VIRTUAL_PRS
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = _MPV_HandleOverFlow;
            prVdecEsInfoKeep->pfnVdecUpdRptr = MPV_UpdateVldRptr;
            prVdecEsInfoKeep->pfnVdecSetWptr = MPV_UpdateVldWptr;
            #else
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = NULL;
            prVdecEsInfoKeep->pfnVdecUpdRptr = NULL;
            prVdecEsInfoKeep->pfnVdecSetWptr = NULL;
            prVdecEsInfoKeep->pfnVdecStartPts = NULL;
            #endif
#ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecIsPic = MPV_MpegIsPic;
#else
            prVdecEsInfoKeep->pfnVdecIsPic = NULL;
#endif
            //init function
            MPV_Init();
            break;
        case VDEC_FMT_WMV:
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_VC1_DISABLE
            prVdecEsInfoKeep->pfnVdecSWRst = _VDEC_WMVSWRst;
            prVdecEsInfoKeep->pfnVdecDecode = NULL;
            prVdecEsInfoKeep->pfnVdecHdrParse = _VDEC_WMVHdrPrs;//_VDEC_H264HdrPrsPic;
            prVdecEsInfoKeep->pfnVdecDecStart = _VDEC_WMVDecStart;
            prVdecEsInfoKeep->pfnVdecDecFin = _VDEC_WMVDecFin;
            prVdecEsInfoKeep->pfnVdecDecDrop = NULL;
            prVdecEsInfoKeep->pfnVdecStop = _VDEC_WMVStop;
            prVdecEsInfoKeep->pfnVdecGetPesStruct = _VDEC_WMVGetPesStruct; // should not need this?
            prVdecEsInfoKeep->pfnVdecGetDispPic = _VDEC_WMVGetDispPic;
            prVdecEsInfoKeep->pfnVdecSetParam = _VDEC_WMVSetParam;
            #ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecGetParam = _VDEC_WMVGetParam;
            #else
            prVdecEsInfoKeep->pfnVdecGetParam = NULL;
            #endif
            prVdecEsInfoKeep->pfnVdecEsFlwCtrl = _VDEC_WMVEsFlwCtrl;
            prVdecEsInfoKeep->pfnVdecWaitRandomAP = NULL;
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = _VDEC_HandleOverFlw;
            prVdecEsInfoKeep->pfnVdecUpdRptr = _VDEC_WMVUpdDmxRptr;
            prVdecEsInfoKeep->pfnVdecSetWptr = _VDEC_WMVSetDmxWptr;
            prVdecEsInfoKeep->pfnVdecStartPts = _VDEC_WMVStartPTS;
#ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecIsPic = _VDEC_WMVIsPic;
#else
            prVdecEsInfoKeep->pfnVdecIsPic = NULL;
#endif
            //init function
            i4Ret = _VDEC_WMVInit(ucEsId);
        #endif
        #endif
            break;
        case VDEC_FMT_RV:
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_RV_DISABLE
            prVdecEsInfoKeep->pfnVdecSWRst = _VDEC_RVSWRst;
            prVdecEsInfoKeep->pfnVdecDecode = NULL;
            prVdecEsInfoKeep->pfnVdecHdrParse = _VDEC_RVHdrPrs;//_VDEC_H264HdrPrsPic;
            prVdecEsInfoKeep->pfnVdecDecStart = _VDEC_RVDecStart;
            prVdecEsInfoKeep->pfnVdecDecFin = _VDEC_RVDecFin;
            prVdecEsInfoKeep->pfnVdecDecDrop = NULL;
            prVdecEsInfoKeep->pfnVdecStop = _VDEC_RVStop;
            prVdecEsInfoKeep->pfnVdecGetPesStruct = _VDEC_RVGetPesStruct; // should not need this?
            prVdecEsInfoKeep->pfnVdecGetDispPic = _VDEC_RVGetDispPic;
#ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecSetParam = _VDEC_RVSetParam;
            prVdecEsInfoKeep->pfnVdecGetParam = _VDEC_RVGetParam;
#else
            prVdecEsInfoKeep->pfnVdecSetParam = NULL;
            prVdecEsInfoKeep->pfnVdecGetParam = NULL;
#endif
            prVdecEsInfoKeep->pfnVdecEsFlwCtrl = _VDEC_RVEsFlwCtrl;
            prVdecEsInfoKeep->pfnVdecWaitRandomAP = NULL;
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = _VDEC_HandleOverFlw;
            prVdecEsInfoKeep->pfnVdecUpdRptr = _VDEC_RVUpdDmxRptr;
            prVdecEsInfoKeep->pfnVdecSetWptr = _VDEC_RVSetDmxWptr;
            prVdecEsInfoKeep->pfnVdecStartPts = NULL;
#ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecIsPic = _VDEC_RvIsPic;
#else
            prVdecEsInfoKeep->pfnVdecIsPic = NULL;
#endif
            //init function
            i4Ret = _VDEC_RVInit(ucEsId);
        #endif
        #endif
            break;
        case VDEC_FMT_MP4:
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_MPEG4_DISABLE
            //prVdecEsInfo->hVdec = _VDEC_Mpeg4GetHdlr();
            prVdecEsInfoKeep->pfnVdecSWRst = _VDEC_Mpeg4SWRst;
            prVdecEsInfoKeep->pfnVdecDecode = NULL;
            prVdecEsInfoKeep->pfnVdecHdrParse = _VDEC_Mpeg4PrsHdr;
            prVdecEsInfoKeep->pfnVdecDecStart = _VDEC_Mpeg4DecStart;
            prVdecEsInfoKeep->pfnVdecDecFin = _VDEC_Mpeg4DecFin;
            prVdecEsInfoKeep->pfnVdecDecDrop = _VDEC_MPEG4DropFrame;
            prVdecEsInfoKeep->pfnVdecStop = _VDEC_Mpeg4Stop;
            prVdecEsInfoKeep->pfnVdecGetPesStruct = _VDEC_GetMpeg4PesStruct; // should not need this?
            prVdecEsInfoKeep->pfnVdecGetDispPic = _VDEC_GetMpeg4DispPic;
            prVdecEsInfoKeep->pfnVdecSetParam = _VDEC_Mpeg4SetParam;
            prVdecEsInfoKeep->pfnVdecGetParam = NULL;
            prVdecEsInfoKeep->pfnVdecEsFlwCtrl = _VDEC_Mpeg4EsFlwCtrl;
#ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecGetParam = _VDEC_MPEG4GetParam;
#else
            prVdecEsInfoKeep->pfnVdecGetParam = NULL;
#endif
            prVdecEsInfoKeep->pfnVdecWaitRandomAP = NULL;
            prVdecEsInfoKeep->pfnVdecUpdRptr = _VDEC_MPEG4UpdDmxRptr;
            prVdecEsInfoKeep->pfnVdecSetWptr = _VDEC_MPEG4SetDmxWptr;
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = _VDEC_HandleOverFlw;
            prVdecEsInfoKeep->pfnVdecStartPts = _VDEC_MPEG4StartPTS;
#ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecIsPic = _VDEC_MPEG4IsPic;
#else
            prVdecEsInfoKeep->pfnVdecIsPic = NULL;
#endif
            //init function
            i4Ret = _VDEC_Mpeg4Init(ucEsId);
        #endif
        #endif
            break;
        case VDEC_FMT_MJPEG:
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_MJPEG_DISABLE
            prVdecEsInfoKeep->pfnVdecSWRst = NULL;//_VDEC_MJPEGSWRst;
            prVdecEsInfoKeep->pfnVdecDecode = _VDEC_MJPEGDecode;
            prVdecEsInfoKeep->pfnVdecHdrParse = NULL;
            prVdecEsInfoKeep->pfnVdecDecStart = NULL;
            prVdecEsInfoKeep->pfnVdecDecFin = NULL;
            prVdecEsInfoKeep->pfnVdecDecDrop = NULL;
            prVdecEsInfoKeep->pfnVdecStop = _VDEC_MJPEGStop;
            prVdecEsInfoKeep->pfnVdecGetPesStruct = _VDEC_MJPEGGetPesStruct;
            prVdecEsInfoKeep->pfnVdecGetDispPic = NULL;
            prVdecEsInfoKeep->pfnVdecSetParam = NULL;
#ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecGetParam = _VDEC_MJPEGGetParam;
#else
            prVdecEsInfoKeep->pfnVdecGetParam = NULL;
#endif
            prVdecEsInfoKeep->pfnVdecEsFlwCtrl = _VDEC_MJPEGEsFlwCtrl;
            prVdecEsInfoKeep->pfnVdecWaitRandomAP = NULL;
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = _VDEC_HandleOverFlw;
            prVdecEsInfoKeep->pfnVdecUpdRptr = _VDEC_MJPEGUpdDmxRptr;
            prVdecEsInfoKeep->pfnVdecSetWptr = _VDEC_MJPEGSetDmxWptr;
#ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecIsPic = _VDEC_MJPEGIsPic;
#else
            prVdecEsInfoKeep->pfnVdecIsPic = NULL;
#endif
            //init function
            i4Ret = _VDEC_MJPEGInit(ucEsId);
        #endif
        #endif
            break;
#ifndef CC_VDEC_AVS_DISABLE
        case VDEC_FMT_AVS:
            prVdecEsInfoKeep->pfnVdecSWRst = _VDEC_AVSSWRst;
            prVdecEsInfoKeep->pfnVdecDecode = NULL;
            prVdecEsInfoKeep->pfnVdecHdrParse = _VDEC_AVSHdrPrsHdr;
            prVdecEsInfoKeep->pfnVdecDecStart = _VDEC_AVSDecStart;
            prVdecEsInfoKeep->pfnVdecDecFin = _VDEC_AVSDecFin;
            prVdecEsInfoKeep->pfnVdecDecDrop = _VDEC_AVSDropFrame;
            prVdecEsInfoKeep->pfnVdecStop = _VDEC_AVSStop;
            prVdecEsInfoKeep->pfnVdecGetPesStruct = _VDEC_AVSGetPesStruct;
            prVdecEsInfoKeep->pfnVdecGetDispPic = _VDEC_AVSGetDispPic;
            prVdecEsInfoKeep->pfnVdecSetParam = _VDEC_AVSSetParam;
            prVdecEsInfoKeep->pfnVdecGetParam = _VDEC_AVSGetParam;
            prVdecEsInfoKeep->pfnVdecEsFlwCtrl = _VDEC_AVSEsFlwCtrl;
            prVdecEsInfoKeep->pfnVdecWaitRandomAP = NULL;
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = _VDEC_HandleOverFlw;
            prVdecEsInfoKeep->pfnVdecUpdRptr = _VDEC_AVSUpdDmxRptr;
            prVdecEsInfoKeep->pfnVdecSetWptr = _VDEC_AVSSetDmxWptr;
#ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecIsPic = _VDEC_AVSIsPic;
#else
            prVdecEsInfoKeep->pfnVdecIsPic = NULL;
#endif
            //init function
            i4Ret = _VDEC_AVSInit(ucEsId);
            break;
#endif
        case VDEC_FMT_RAW:
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_RAW_DISABLE
            prVdecEsInfoKeep->pfnVdecSWRst = NULL;
            prVdecEsInfoKeep->pfnVdecDecode = _VDEC_RAWDecode;
            prVdecEsInfoKeep->pfnVdecHdrParse = NULL;
            prVdecEsInfoKeep->pfnVdecDecStart = NULL;
            prVdecEsInfoKeep->pfnVdecDecFin = NULL;
            prVdecEsInfoKeep->pfnVdecDecDrop = NULL;
            prVdecEsInfoKeep->pfnVdecStop = _VDEC_RAWStop;
            prVdecEsInfoKeep->pfnVdecGetPesStruct = _VDEC_RAWGetPesStruct;
            prVdecEsInfoKeep->pfnVdecGetDispPic = NULL;
            prVdecEsInfoKeep->pfnVdecSetParam = NULL;
            prVdecEsInfoKeep->pfnVdecGetParam = NULL;
            prVdecEsInfoKeep->pfnVdecUpdRptr = _VDEC_RAWUpdDmxRptr;
            prVdecEsInfoKeep->pfnVdecSetWptr = NULL;
            prVdecEsInfoKeep->pfnVdecEsFlwCtrl = _VDEC_RAWEsFlwCtrl;
            prVdecEsInfoKeep->pfnVdecWaitRandomAP = NULL;
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = _VDEC_HandleOverFlw;
            prVdecEsInfoKeep->pfnVdecStartPts = NULL;
            prVdecEsInfoKeep->pfnVdecIsPic = NULL;
            //init function
            i4Ret = _VDEC_RAWInit(ucEsId);
        #endif
        #endif
            break;
        case VDEC_FMT_VP6:
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_VP6_DISABLE
            //prVdecEsInfo->hVdec = _VDEC_Mpeg4GetHdlr();
            prVdecEsInfoKeep->pfnVdecSWRst = _VDEC_Vp6SWRst;
            prVdecEsInfoKeep->pfnVdecDecode = NULL;
            prVdecEsInfoKeep->pfnVdecHdrParse = _VDEC_Vp6HdrPrs;
            prVdecEsInfoKeep->pfnVdecDecStart = _VDEC_Vp6DecStart;
            prVdecEsInfoKeep->pfnVdecDecFin = _VDEC_Vp6DecFin;
            prVdecEsInfoKeep->pfnVdecDecDrop = _VDEC_Vp6DropFrame;
            prVdecEsInfoKeep->pfnVdecStop = _VDEC_Vp6Stop;
            prVdecEsInfoKeep->pfnVdecGetPesStruct = _VDEC_Vp6GetPesStruct; // should not need this?
            prVdecEsInfoKeep->pfnVdecGetDispPic = _VDEC_Vp6GetDispPic;
            prVdecEsInfoKeep->pfnVdecSetParam = _VDEC_Vp6SetParam;
            prVdecEsInfoKeep->pfnVdecGetParam = _VDEC_Vp6GetParam;
            prVdecEsInfoKeep->pfnVdecEsFlwCtrl = _VDEC_Vp6EsFlwCtrl;
            prVdecEsInfoKeep->pfnVdecWaitRandomAP = NULL;
            prVdecEsInfoKeep->pfnVdecUpdRptr = _VDEC_Vp6UpdDmxRptr;
            prVdecEsInfoKeep->pfnVdecSetWptr = _VDEC_VP6SetDmxWptr;
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = NULL;
            prVdecEsInfoKeep->pfnVdecStartPts = NULL;
#ifdef VDEC_TIME_PROFILE
            prVdecEsInfoKeep->pfnVdecIsPic = _VDEC_VP6IsPic;
#else
            prVdecEsInfoKeep->pfnVdecIsPic = NULL;
#endif

            //init function
            i4Ret = _VDEC_Vp6Init(ucEsId);
          #endif
          #endif
            break;

        case VDEC_FMT_VP8:
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_VP8_DISABLE
            prVdecEsInfoKeep->pfnVdecSWRst = _VDEC_Vp8SWRst;
            prVdecEsInfoKeep->pfnVdecDecode = NULL;
            prVdecEsInfoKeep->pfnVdecHdrParse = _VDEC_Vp8HdrPrs;
            prVdecEsInfoKeep->pfnVdecDecStart = _VDEC_Vp8DecStart;
            prVdecEsInfoKeep->pfnVdecDecFin = _VDEC_Vp8DecFin;
            prVdecEsInfoKeep->pfnVdecDecDrop = _VDEC_Vp8DropFrame;
            prVdecEsInfoKeep->pfnVdecStop = _VDEC_Vp8Stop;
            prVdecEsInfoKeep->pfnVdecGetPesStruct = _VDEC_Vp8GetPesStruct; // should not need this?
            prVdecEsInfoKeep->pfnVdecGetDispPic = _VDEC_Vp8GetDispPic;
            prVdecEsInfoKeep->pfnVdecSetParam = _VDEC_Vp8SetParam;
            prVdecEsInfoKeep->pfnVdecEsFlwCtrl = _VDEC_Vp8EsFlwCtrl;
            prVdecEsInfoKeep->pfnVdecWaitRandomAP = NULL;
            prVdecEsInfoKeep->pfnVdecUpdRptr = _VDEC_Vp8UpdDmxRptr;
            prVdecEsInfoKeep->pfnVdecSetWptr = _VDEC_VP8SetDmxWptr;
            prVdecEsInfoKeep->pfnVdecFlush   = _VDEC_Vp8Flush;
            prVdecEsInfoKeep->pfnVdecHandleOverFlw = NULL;
            prVdecEsInfoKeep->pfnVdecStartPts = NULL;
#ifdef VDEC_TIME_PROFILE
			prVdecEsInfoKeep->pfnVdecIsPic = _VDEC_Vp8IsPic;
            prVdecEsInfoKeep->pfnVdecGetParam = _VDEC_Vp8GetParam;
#else
			prVdecEsInfoKeep->pfnVdecIsPic = NULL;
            prVdecEsInfoKeep->pfnVdecGetParam = NULL;
#endif
            //init function
            i4Ret = _VDEC_Vp8Init(ucEsId);
  #endif
  #endif
            break;


        default:
            return (INT32)E_VDEC_FAIL;
            //break;
    }

    if(prVdecEsInfoKeep->pfnVdecStartPts)
    {
        prVdecEsInfoKeep->pfnVdecStartPts(ucEsId , prVdecEsInfo->fgEnCalPTS, prVdecEsInfo->u4StartPTS, (UINT32)&prVdecEsInfo->rPTSInfo);
    }
    return i4Ret;
}

static void _VDEC_VldIsr(UINT16 u2Vector)
{
    VDEC_INFO_T *prVdecInfo;
    UCHAR ucVldId = VLD0;

    if(VECTOR_VDEC == u2Vector)
    {
        ucVldId = VLD0;
    }
    else
    {
        // may changed later
        ucVldId = VLD0;
    }
    if(ucVldId >= VDEC_MAX_VLD)
    {
        LOG(3, "%s(%d) ucVldId err\n", __FUNCTION__, __LINE__);
        return;
    }
    prVdecInfo = _VDEC_GetInfo();    
    if(!prVdecInfo)
    {
        LOG(3, "%s(%d) prVdecInfo err\n", __FUNCTION__, __LINE__);
        return;
    }
    if(prVdecInfo->arVdecVldInfo[ucVldId].pfnVdecIsr)
    {
        prVdecInfo->arVdecVldInfo[ucVldId].pfnVdecIsr(u2Vector);
    }
}

VOID _VDEC_IsrInit(VOID)
{
    x_os_isr_fct pfnOldIsr;
    if (x_reg_isr(VECTOR_VDEC, _VDEC_VldIsr, &pfnOldIsr) != OSR_OK)
    {
        ASSERT(0);
    }
}

VOID _VDEC_RegIsr(UINT16 u2Vector, PFN_VDEC_ISR pfnIsr)
{
    VDEC_INFO_T *prVdecInfo;
    UCHAR ucVldId = VLD0;

    if(VECTOR_VDEC == u2Vector)
    {
        ucVldId = VLD0;
    }
    else
    {
        // may changed later
        ucVldId = VLD0;
    }
    if(ucVldId >= VDEC_MAX_VLD)
    {
        LOG(3, "%s(%d) ucVldId err\n", __FUNCTION__, __LINE__);
        return;
    }
    prVdecInfo = _VDEC_GetInfo();    
    if(!prVdecInfo)
    {
        LOG(3, "%s(%d) prVdecInfo err\n", __FUNCTION__, __LINE__);
        return;
    }
    prVdecInfo->arVdecVldInfo[ucVldId].pfnVdecIsr = pfnIsr;
}
    

VOID _VDEC_SetIsr(UCHAR ucEsId)
{
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    switch(prVdecEsInfoKeep->eCurFMT)
    {
        case VDEC_FMT_MPV:
            _MPV_SetIsr(ucEsId);
            break;
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_MPEG4_DISABLE
        case VDEC_FMT_MP4:
            _VDEC_MPEG4SetIsr(ucEsId);
            break;
#endif
#endif
#if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
#ifndef CC_VDEC_H264_DISABLE
        case VDEC_FMT_H264:
        case VDEC_FMT_H264VER:
            _VDEC_H264SetIsr(ucEsId);
            break;
#endif
#endif
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_VC1_DISABLE
        case VDEC_FMT_WMV:
            _VDEC_WMVSetIsr(ucEsId);
            break;
#endif
#endif
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_RV_DISABLE
        case VDEC_FMT_RV:
            _VDEC_RVSetIsr(ucEsId);
            break;
#endif
#endif
        case VDEC_FMT_MJPEG:
            break;
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_VP6_DISABLE
        case VDEC_FMT_VP6:
          _VDEC_Vp6SetIsr(ucEsId);
          break;
#endif
#endif

#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_VP8_DISABLE
        case VDEC_FMT_VP8:
          _VDEC_Vp8SetIsr(ucEsId);
          break;
#endif
#endif

#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_AVS_DISABLE
        case VDEC_FMT_AVS:
            _VDEC_AVSSetIsr(ucEsId);
            break;
#endif
#endif
        default:
            LOG(3, "format unknown %d\n", (UINT32)prVdecEsInfoKeep->eCurFMT);
            break;
    }
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
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_REWIND_3X) ||
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
          ((prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_3X) ||
           (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_4X) ||
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
    //for TS trick to normal, 1st frame should duplicate with current display
    //avoid b2r display 1st frame withou AV sync judgement
    else if ((prVdecEsInfo->fgRenderFromPos) &&
        ((i4TempRef != prVdecEsInfo->i4RenderTempRef) || 
         (u8Offset < prVdecEsInfo->u8RenderFromPos)))
    //else if (prVdecEsInfo->fgRenderFromPos)
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
                STC_StartStc(prVdecEsInfo->ucStcId);
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
                    STC_StopStc(prVdecEsInfo->ucStcId);
                    STC_SetStcValue(prVdecEsInfo->ucStcId, (UINT32)(u4PTS - 3000));
                    STC_StartStc(prVdecEsInfo->ucStcId);
                }
                else
                {
                    LOG(3, "VDEC_IsNotDisplay fgSetStartPts no pts or zero\n");
                    //VDP_TriggerAudReceive(0);
                    //STC_SetStcValue((UINT32)(0 - 3000));
                    STC_StartStc(prVdecEsInfo->ucStcId);
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
    #ifdef CC_B2R_3D_ERROR_SUPPORT
    BOOL fgRenderFinish = FALSE;
    #endif
    UCHAR ucFbId;
    //UINT32 u4Temp;

    FBM_PIC_HDR_T *prFbmPicHdr = NULL;

    VDEC_INFO_T* prVdecInfo;
    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;

    ASSERT(ucEsId < MPV_MAX_ES);

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

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
        #ifdef CC_B2R_3D_ERROR_SUPPORT
        fgRenderFinish = TRUE;
        #endif
        if(prVdecEsInfoKeep->pfnRenderPtsCb)
        {
            prVdecEsInfoKeep->pfnRenderPtsCb(u4PTS, prVdecEsInfoKeep->u1AttachedSrcId);
            LOG(3, "VDEC_CheckStopStatus pfnRenderPtsCb fgRenderFromFirstPic\n");
        }
        if (prVdecEsInfo->fgPlayOneFrm)
        {
            prVdecEsInfo->fgPlayOneFrm = FALSE;
            if(prVdecEsInfoKeep->pfnTrickPtsCb)
            {
                // We didn't found any frame before u8RenderFromPos.
                if(prVdecEsInfo->u2Disp2EmptyCnt > 0)
                {
                    // Send again with far position
                    prVdecEsInfoKeep->pfnTrickPtsCb(prVdecEsInfo->u1SwdmxSrcId, 0, TRUE);
                    LOG(5, "VDEC_CheckStopStatus pfnTrickPtsCb fgRenderFromFirstPic again\n");
                }
                else
                {
                    prVdecEsInfoKeep->pfnTrickPtsCb(prVdecEsInfo->u1SwdmxSrcId, 0, FALSE);
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
        #ifdef CC_B2R_3D_ERROR_SUPPORT
        fgRenderFinish = TRUE;
        #endif
        prVdecEsInfo->u4RenderPts = 0;
        if(prVdecEsInfoKeep->pfnRenderPtsCb)
        {
            prVdecEsInfoKeep->pfnRenderPtsCb(u4PTS, prVdecEsInfoKeep->u1AttachedSrcId);
            LOG(3, "VDEC_CheckStopStatus pfnRenderPtsCb fgRenderPts\n");
        }
        if (prVdecEsInfo->fgPlayOneFrm)
        {
            prVdecEsInfo->fgPlayOneFrm = FALSE;
            if(prVdecEsInfoKeep->pfnTrickPtsCb)
            {
                prVdecEsInfoKeep->pfnTrickPtsCb(prVdecEsInfo->u1SwdmxSrcId, 0, FALSE);
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
        ((prVdecEsInfo->u8Offset > prVdecEsInfo->u8RenderFromPos) ||
        ((prVdecEsInfo->u8Offset >= prVdecEsInfo->u8RenderFromPos) &&
        ((i4TempRef >= prVdecEsInfo->i4RenderTempRef) ||
         (prVdecEsInfo->u4MMSpeed != (UINT32)STC_SPEED_TYPE_FORWARD_1X)
#ifdef ENABLE_MULTIMEDIA         
         || (((prVdecEsInfo->eContainerType >= SWDMX_FMT_MPEG1_DAT) &&
         (prVdecEsInfo->eContainerType <= SWDMX_FMT_AVS_VIDEO_ES))&&
         (i4TempRef == 0))
#endif
         ))))
    {
        prVdecEsInfo->fgRenderFromPos = FALSE;
        #ifdef CC_B2R_3D_ERROR_SUPPORT
        fgRenderFinish = TRUE;
        #endif
        prVdecEsInfo->u8RenderFromPos = 0;
        prVdecEsInfo->i4RenderTempRef = 0;
        prVdecEsInfo->u2RenderDecodingOrder = 0;

        if(prVdecEsInfoKeep->pfnRenderPtsCb)
        {
            prVdecEsInfoKeep->pfnRenderPtsCb(u4PTS, prVdecEsInfoKeep->u1AttachedSrcId);
            LOG(3, "VDEC_CheckStopStatus fgRenderFromPos pfnRenderPtsCb u4PTS %d\n", u4PTS);
        }

        LOG(3, "VDEC_CheckStopStatus fgRenderFromPos i4TempRef %d\n", i4TempRef);
    }
    else if ((prVdecEsInfo->fgRenderBackStep) &&
        (prVdecEsInfo->u8Offset >= prVdecEsInfo->u8RenderFromPos) &&
        ((i4TempRef == prVdecEsInfo->i4RenderTempRef) ||
         (prVdecEsInfo->u4MMSpeed != (UINT32)STC_SPEED_TYPE_FORWARD_1X)))
    {
        prVdecEsInfo->fgRenderBackStep = FALSE;
        #ifdef CC_B2R_3D_ERROR_SUPPORT
        fgRenderFinish = TRUE;
        #endif
        prVdecEsInfo->u8RenderFromPos = 0;
        prVdecEsInfo->i4RenderTempRef = 0;
        prVdecEsInfo->u2RenderDecodingOrder = 0;

        // if only decode one frame, we have to seek again (far)
        // notify swdmx to do it again
        // FixMe

        if(prVdecEsInfoKeep->pfnTrickPtsCb)
        {
            // We have put two to empty that originaly should put to display.
            // That means we have previous display frame to empty queue.
            if(prVdecEsInfo->u2Disp2EmptyCnt > 1)
            {
                prVdecEsInfoKeep->pfnTrickPtsCb(prVdecEsInfo->u1SwdmxSrcId, 0, FALSE);
                LOG(5, "VDEC_CheckStopStatus pfnTrickPtsCb fgRenderBackStep finish\n");
            }
            else
            {
                // Send again with far position
                prVdecEsInfoKeep->pfnTrickPtsCb(prVdecEsInfo->u1SwdmxSrcId, 0, TRUE);
                LOG(5, "VDEC_CheckStopStatus pfnTrickPtsCb fgRenderBackStep again\n");
            }
            //x_thread_delay(33);
        }
        if(prVdecEsInfoKeep->pfnRenderPtsCb)
        {
            prVdecEsInfoKeep->pfnRenderPtsCb(u4PTS, prVdecEsInfoKeep->u1AttachedSrcId);
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
        #ifdef CC_B2R_3D_ERROR_SUPPORT
        fgRenderFinish = TRUE;
        #endif
        prVdecEsInfo->u4RenderFromPts = 0;

        if(prVdecEsInfoKeep->pfnRenderPtsCb)
        {
            prVdecEsInfoKeep->pfnRenderPtsCb(u4PTS, prVdecEsInfoKeep->u1AttachedSrcId);
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
        #ifdef CC_B2R_3D_ERROR_SUPPORT
        fgRenderFinish = TRUE;
        #endif
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
        if((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
            ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MJPEG)
            ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_AVS)
            ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_RAW)
            ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_VP8)
            ||(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_VP6))
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
            // forward speed EOS
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

    #ifdef CC_B2R_3D_ERROR_SUPPORT
    if(fgRenderFinish)
    {
        if(FBM_ChkFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_3D_ERROR_HANDLE_DISABLE))
        {
            FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_3D_ERROR_HANDLE_DISABLE);
        }
    }
    #endif

    return fgSkip;
}


BOOL VDEC_PrsSeqHdr(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt,
    VDEC_PES_INFO_T* prPesInfo, VDEC_SEQUENCE_DATA_T* prSeqInfo)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    VDEC_PES_INFO_T* prPes;
    BOOL fgHdrParseOk = FALSE;
    #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
    #ifndef CC_VDEC_H264_DISABLE
    UINT32 u4Dummy;
    UINT32 u4W, u4H;
    #endif
    #endif
    //BOOL fgPowerOn;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if((prPesInfo == NULL) || (prSeqInfo == NULL) || (prVdecEsInfo == NULL))
    {
        LOG(1, "PrsSeqHdr input arg error\n");
        return FALSE;
    }

    #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
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
        UNUSED(VDEC_PowerOn(ucEsId, VLD0));
    }

    if((eFmt != prVdecEsInfoKeep->eCurFMT) ||
       (prVdecEsInfoKeep->eCurState == VDEC_ST_STOP))
    {
        prVdecEsInfoKeep->eCurFMT = eFmt;
        UNUSED(_VDEC_DecCfg(ucEsId));
    }
    #endif

    #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
    HalFlushInvalidateDCacheMultipleLine(VIRTUAL(prPesInfo->u4FifoStart), (prPesInfo->u4FifoEnd - prPesInfo->u4FifoStart));
    #else
    HalFlushInvalidateDCache();
    #endif
    // video es only.
    prVdecEsInfo->u4VldId = VLD0;
    prVdecEsInfo->u4FifoStart = prPesInfo->u4FifoStart;
    prVdecEsInfo->u4FifoEnd = prPesInfo->u4FifoEnd;
    prVdecEsInfo->u4VldReadPtr = prPesInfo->u4VldReadPtr;
    prVdecEsInfo->u4PTS = prPesInfo->u4PTS;
    prVdecEsInfo->u4DTS = prPesInfo->u4DTS;
    prVdecEsInfo->ucPicType = prPesInfo->ucPicType;
    prVdecEsInfo->fgDtsValid = prPesInfo->fgDtsValid;
    prVdecEsInfo->fgSeqHdr = prPesInfo->fgSeqHdr;
    prVdecEsInfo->rExtra = prPesInfo->rExtra;
    prVdecEsInfo->u8Offset = prPesInfo->u8Offset;
    prVdecEsInfo->u8OffsetLast = prPesInfo->u8Offset;
    prVdecEsInfo->u8OffsetI = prPesInfo->u8OffsetI;
    prVdecEsInfo->u2DecodingOrder = prPesInfo->u2DecodingOrder;
    prVdecEsInfo->u4TotlaTimeOffset = prPesInfo->u4TotalTimeOffset;

    // lock vld
    _VDEC_LockVld(ucEsId, VLD0);

    //get pointer from internal decoder struct, or save in common part
    if(prVdecEsInfoKeep->pfnVdecGetPesStruct)
    {
        prVdecEsInfoKeep->pfnVdecGetPesStruct(ucEsId, (void**)&prPes);
        if(prPes == NULL)
        {
            #if 0 // move to VDEC_SetGetSeqHdrOnlyEnd
            if(!fgPowerOn)
            {
                UNUSED(VDEC_PowerOff(ucEsId, VLD0));
            }
            #endif
            _VDEC_UnlockVld(ucEsId, VLD0);
            return FALSE;
        }
        x_memcpy(prPes, prPesInfo, sizeof(VDEC_PES_INFO_T));

		if(eFmt == VDEC_FMT_WMV)
		{   
			if(prVdecEsInfoKeep->pfnVdecEsFlwCtrl)
			{
				prVdecEsInfoKeep->pfnVdecEsFlwCtrl(ucEsId, &prPesInfo->ucMpvId,0, IPBMode);
			}
		}
		
        if(prVdecEsInfoKeep->pfnVdecHdrParse)
        {
            fgHdrParseOk = prVdecEsInfoKeep->pfnVdecHdrParse(prPesInfo->ucMpvId, ucEsId);

            if((fgHdrParseOk == E_VDEC_FAIL)&&(eFmt == VDEC_FMT_MP4))
            {
                  _VDEC_UnlockVld(ucEsId, VLD0);
            }
        }
        prSeqInfo->u2_width = 0;
        prSeqInfo->u2_height = 0;
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
        if (prVdecEsInfoKeep->pfnVdecGetParam)
        {
            prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, (UINT32)SEQ_HDR_INFO, &u4W, &u4H, &u4Dummy);
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

        //VDEC_PlayMM(ucEsId, TRUE);

        //VERIFY(x_sema_lock(prVdecInfo->arVdecVldInfo[VLD0].hVldSemaphore,
        //        X_SEMA_OPTION_WAIT) == OSR_OK);

        if(prVdecEsInfoKeep->pfnVdecSWRst)
        {
            prVdecEsInfoKeep->pfnVdecSWRst(prPesInfo->ucMpvId, ucEsId);
        }
        if(prVdecEsInfoKeep->pfnVdecHdrParse)
        {
            UNUSED(prVdecEsInfoKeep->pfnVdecHdrParse(prPesInfo->ucMpvId, ucEsId));
        }

        MPV_GetVideoInfo(ucEsId, &u4HResolution, &u4VResolution,
                            &i4PrgScan, &e4Ratio, &u4FrmRat);
        prSeqInfo->u2_width = (UINT16)u4HResolution;
        prSeqInfo->u2_height = (UINT16)u4VResolution;

        //VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[VLD0].hVldSemaphore) == OSR_OK);
    }
    else if(eFmt == VDEC_FMT_WMV)
    {
        if(prVdecEsInfoKeep->pfnVdecHdrParse)
        {
            UNUSED(prVdecEsInfoKeep->pfnVdecHdrParse(prPesInfo->ucMpvId, ucEsId));
        }
        prSeqInfo->u2_width = 0;
        prSeqInfo->u2_height = 0;
    }

    #if 0 // move to VDEC_SetGetSeqHdrOnlyEnd
    if((prVdecEsInfoKeep->pfnVdecStop) && (prVdecEsInfoKeep->eCurState == VDEC_ST_STOP))
    {
        prVdecEsInfoKeep->pfnVdecStop(ucEsId);
    }
    // I am not sure if we need this.
    //MPV_FlushEsmQ(ucEsId, FALSE);

    if(!fgPowerOn)
    {
        UNUSED(VDEC_PowerOff(ucEsId, VLD0));
    }
    #endif
    _VDEC_UnlockVld(ucEsId, VLD0);
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
        LOG(1, "_VDEC_MainLoop(): (pvArg == NULL)\n");
    }
    else
    {
        ucEsId = *(UCHAR*)pvArg;
    }

    if (ucEsId >= VDEC_MAX_ES)
    {
        ASSERT(ucEsId < VDEC_MAX_ES);
        ucEsId = ES0;
        LOG(1, "_VDEC_MainLoop(): (ucEsId >= VDEC_MAX_ES)\n");
    }
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    prVdecEsInfoKeep->fgThreadActive = TRUE;
    prVdecEsInfoKeep->fgThreadDestroy = FALSE;
    #ifdef VDEC_TIME_PROFILE
    _rEsAcc.u4Seconds = 0;
    _rEsAcc.u4Micros = 0;
#if 0
    x_memset((void*)&prVdecEsInfo->au4DecTime, 0, sizeof(prVdecEsInfo->au4DecTime));
    prVdecEsInfo->au4DecTime[0][1] = 0xFF;
    prVdecEsInfo->au4DecTime[1][1] = 0xFF;
    prVdecEsInfo->au4DecTime[2][1] = 0xFF;
    #endif
#endif

    x_memset(&rDispInfo, 0,sizeof(VDEC_DISP_PIC_INFO_T));

    while (prVdecInfo->fgVDecInitiated)
    {
        if(prVdecEsInfoKeep->eCurState == VDEC_ST_STOP)
        {
            _VDecReceiveCmd(ucEsId, TRUE);
            if((prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY) || (prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY_I_FRAME))
            {
                LOG(1, "Config\n");
                if(prVdecEsInfo->u4VldId >= VDEC_MAX_VLD)
                {
                    LOG(1, "u4VldId(%d) >= VDEC_MAX_VLD\n", prVdecEsInfo->u4VldId);
                    prVdecEsInfo->u4VldId = VLD0;
                }
                UNUSED(VDEC_PowerOn(ucEsId, prVdecEsInfo->u4VldId));

                prVdecEsInfoKeep->eDecStatus = VDEC_DEC_HEADER_PARSE;
                prVdecEsInfo->u1LastSkipType = IPBMode;
                prVdecEsInfo->eStreamType = VDEC_STREAM_TYPE_UNKNOW;

                // Add for source state process
                prVdecEsInfo->ePerSourceState = VDEC_SCRAMBLE_STATE_CLEAR;
                prVdecEsInfo->eCurSourceState = VDEC_SCRAMBLE_STATE_CLEAR;
                if (prVdecEsInfo->fgMMPlayback)
                {
                    UNUSED(DMX_RegSourceStateCbForVdec(ucEsId, NULL));
                }
                else
                {
                    // DTV case
                    UNUSED(DMX_RegSourceStateCbForVdec(ucEsId, _VDEC_SourceStateNty));
                }
                
                //config, create es queue at first time
                if(_VDEC_DecCfg(ucEsId) != (INT32)E_VDEC_OK)
                {
                    prVdecEsInfoKeep->ePreviousState = prVdecEsInfoKeep->eCurState;
                    prVdecEsInfoKeep->eCurState = VDEC_ST_STOP;
                    continue;
                }
                prVdecEsInfo->u4MaxEsCnt = 0;
                //reset
                if(prVdecEsInfoKeep->pfnVdecSWRst)
                {
                    prVdecEsInfoKeep->pfnVdecSWRst(prVdecEsInfo->u4VldId, ucEsId);
                }
                #ifdef VDEC_TIME_PROFILE
                HAL_GetTime(&_rTimeFrmS);
                #endif
            }
        }
        else if(prVdecEsInfoKeep->eCurState == VDEC_ST_IDLE)
        {
            if(prVdecEsInfo->fgMMPlayback)
            {
                // just pause, MM case
                _VDecReceiveCmd(ucEsId, TRUE);

                //clear queue, clear stored info, do notification
                _VDEC_LockFlushMutex(ucEsId);
                if (prVdecEsInfo->fgFlushEsmQ)
                {
                    LOG(1, "ES(%d) VDEC_ST_IDLE Flush\n", ucEsId);
                    _VDEC_FlushEs(ucEsId);
                    prVdecEsInfo->fgFlushEsmQ = FALSE;
                    _VDEC_UnlockFlushSema(ucEsId);
                }
                _VDEC_UnlockFlushMutex(ucEsId);

                if(prVdecEsInfoKeep->ePreviousState != prVdecEsInfoKeep->eCurState)
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

        switch(prVdecEsInfoKeep->eCurState)
        {
            case VDEC_ST_PLAY:
            case VDEC_ST_PLAY_I_FRAME:
            {
                if(prVdecEsInfoKeep->pfnVdecDecode)
                {
                    LOG(1, "Decode\n");
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
                    
                    if (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV)
                        _VDEC_LockVld(ucEsId, ucMpvId);
                    //VERIFY(x_sema_lock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore,
                    //X_SEMA_OPTION_WAIT) == OSR_OK);

                    //reset on each pic, we do it in decode.
                    //prVdecEsInfoKeep->pfnVdecSWRst(ucMpvId, ucEsId);

                    i4RetValue = prVdecEsInfoKeep->pfnVdecDecode(ucMpvId, ucEsId);
                    #ifdef VDEC_TIME_PROFILE
                    _VdecTimeProfile(ucEsId);
                    #endif
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
                    if(prVdecEsInfoKeep->pfnVdecUpdRptr)
                    {
                        prVdecEsInfoKeep->pfnVdecUpdRptr(ucEsId);
                    }

                    _VDEC_DeliverEos(ucEsId);

                    if (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV)
                        _VDEC_UnlockVld(ucEsId, ucMpvId);
                    //VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore) == OSR_OK);

                if ((i4RetValue == (INT32)E_VDEC_NOT_SUPPORT) &&
                                (prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT))
                    {
                        if (prVdecEsInfoKeep->pfDecNfyFct)    //enable after mw add this handler
                        {
                            LOG(1, "ES(%d) Notify Status change, codec not support\n", ucEsId);
                            prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                                VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                            prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
                        if (prVdecEsInfoKeep->pfVdecNotSupporuNfyFct)
                        {
                            prVdecEsInfoKeep->pfVdecNotSupporuNfyFct(ucEsId, TRUE);
                        }
                        prVdecEsInfoKeep->eCurState = VDEC_ST_IDLE;
                    }
                }
                else if ((i4RetValue == (INT32)E_VDEC_RES_NOT_SUPPORT) &&
                         ((prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_RES_NOT_SUPPORT)))
                {
                    if (prVdecEsInfoKeep->pfDecNfyFct)    //enable after mw add this handler
                    {
                        LOG(1, "ES(%d) Notify Status change, resolution not support\n", ucEsId);
                        prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                                                      VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_RES_NOT_SUPPORT, 0);
                        prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_RES_NOT_SUPPORT;
                        if (prVdecEsInfoKeep->pfVdecNotSupporuNfyFct)
                            {
                                prVdecEsInfoKeep->pfVdecNotSupporuNfyFct(ucEsId, TRUE);
                            }
                            prVdecEsInfoKeep->eCurState = VDEC_ST_IDLE;
                        }
                    }
#ifdef CC_VDEC_PRIORITY_ENABLE
					VDEC_ThreadSchedule(ucEsId);
#endif
                }
                else
                {
                if (prVdecEsInfoKeep->eDecStatus == VDEC_DEC_HEADER_PARSE)
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
                        // lock vld, TODO
                        //_VDEC_LockVld(ucEsId, ucMpvId);
                        //VERIFY(x_sema_lock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore,
                        //X_SEMA_OPTION_WAIT) == OSR_OK);

                        //check if need re-config according to id in PesInfo?
                        i4RetValue = prVdecEsInfoKeep->pfnVdecHdrParse(ucMpvId, ucEsId);

                        if((i4RetValue == (INT32)E_VDEC_OK) ||
                            (i4RetValue == (INT32)E_VDEC_OK_RES_CHG))            //parse done
                        {
                            if(prVdecEsInfo->fgFmtDectFinish)
                            {
                                prVdecEsInfoKeep->eDecStatus = VDEC_DEC_TRIGGER;
                            }
                            else
                            {
                                if(VDEC_ChkCodecCap(ucEsId, prVdecEsInfoKeep->eCurFMT))
                                {
                                    prVdecEsInfoKeep->eDecStatus = VDEC_DEC_TRIGGER;
                                }
                                else
                                {
                                    prVdecEsInfoKeep->eDecStatus = VDEC_DEC_HEADER_PARSE;
                                    if(prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT)
                                    {
                                        if(prVdecEsInfoKeep->pfDecNfyFct)
                                        {
                                            LOG(3, "ES(%d) Notify Status change, codec not support\n", ucEsId);
                                            prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                                                VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);


                                            prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
                                            if(prVdecEsInfoKeep->pfVdecNotSupporuNfyFct)
                                            {
                                                prVdecEsInfoKeep->pfVdecNotSupporuNfyFct(ucEsId, TRUE);
                                            }
                                            prVdecEsInfoKeep->eCurState = VDEC_ST_IDLE;
                                        }
                                    }
                                }
                                prVdecEsInfo->fgFmtDectFinish = TRUE;
                            }
                        }
                        #if 0  //request by scart out feature, but cause DTV aspect ratio change problem, callback timing earlier than vdp prepared
                        else if(i4RetValue == (INT32)E_VDEC_OK_RES_CHG)
                        {
                            prVdecEsInfoKeep->eDecStatus = VDEC_DEC_TRIGGER;
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
                            prVdecEsInfoKeep->eDecStatus = VDEC_DEC_DROP;
                        }
                        else if(i4RetValue == (INT32)E_VDEC_BS_END)
                        {
                            _VDEC_DeliverEos(ucEsId);
                            if(prVdecInfo->pfErrNotify)
                            {
                                prVdecInfo->pfErrNotify((UINT32)VDEC_EVT_DEC_BS_END, (UINT32)&rDispInfo, 0, 0);
                            }
                            if(prVdecEsInfoKeep->pfnVdecStop)
                            {
                                prVdecEsInfoKeep->pfnVdecStop(ucEsId);
                            }
                            prVdecEsInfoKeep->ePreviousState = prVdecEsInfoKeep->eCurState;
                            prVdecEsInfoKeep->eCurState = VDEC_ST_STOP;
                            _VDEC_FlushEs(ucEsId);
                            prVdecEsInfo->fgFmtDectFinish = TRUE;
                            // TODO
                            //_VDEC_UnlockVld(ucEsId, ucMpvId);
                            //VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore) == OSR_OK);
                        }
#if 0//def CC_AVS_EMULATION
                        else if (i4RetValue == E_VDEC_SKIPFRAME)
                        {
                            if (prVdecInfo->pfErrNotify)
                            {
                                prVdecInfo->pfErrNotify((UINT32)VDEC_EVT_DEC_ERROR, 0, 0, 0);
                            }
                        }
#endif
                        else
                        {
                            _VDEC_DeliverEos(ucEsId);
                            if((i4RetValue == (INT32)E_VDEC_NOT_SUPPORT) &&
                                (prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT))
                            {
                                if (prVdecEsInfoKeep->pfDecNfyFct)    //enable after mw add this handler
                                {
                                    LOG(3, "ES(%d) Notify Status change, codec not support\n", ucEsId);
                                    prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                                        VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                                    prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;

                                    if(prVdecEsInfoKeep->pfVdecNotSupporuNfyFct)
                                    {
                                        prVdecEsInfoKeep->pfVdecNotSupporuNfyFct(ucEsId, TRUE);
                                    }
                                    prVdecEsInfoKeep->eCurState = VDEC_ST_IDLE;
                                }
                                prVdecEsInfo->fgFmtDectFinish = TRUE;
                            }
                            prVdecEsInfoKeep->eDecStatus = VDEC_DEC_HEADER_PARSE;
                        }
                        //update rptr
                        // may need another one for H264
                        //if(prVdecEsInfoKeep->pfnVdecUpdRptr)
                        //{
                            //prVdecEsInfoKeep->pfnVdecUpdRptr(ucEsId);
                        //}
                    }
                    //state combine(hdr parse+ trigger+ wait dec done)
                    /*else */
                    if (prVdecEsInfoKeep->eDecStatus == VDEC_DEC_TRIGGER)
                    {
                        //LOG(6, "Pic %d Decoding\n", u4DecCount++);
                        if(prVdecEsInfoKeep->fgVdecMidEnable && prVdecEsInfoKeep->pfnVdecGetDispPic)
                        {
                            if((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264) ||
                                (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264VER))
                            {
                                prVdecEsInfoKeep->pfnVdecGetDispPic(ucEsId, (void*)&rDispInfo);
                                if(!_VDEC_MidSetting(ucEsId, &rDispInfo))
                                {
                                    LOG(3, "VdecMid Fail~\n");
                                }
                            }
                        }
                        i4RetValue = prVdecEsInfoKeep->pfnVdecDecStart(ucMpvId, ucEsId);
                        if(i4RetValue == (INT32)E_VDEC_OK)
                        {
                            prVdecEsInfoKeep->eDecStatus = VDEC_DEC_WAIT_DEC_FINISH;
                        }
                        else    //error
                        {
                            prVdecEsInfoKeep->eDecStatus = VDEC_DEC_DROP;
                        }
                    }
                    //combine trigger decode and wait finish in one stage
                    /*else */
                    if (prVdecEsInfoKeep->eDecStatus == VDEC_DEC_WAIT_DEC_FINISH)
                    {
                        i4RetValue = prVdecEsInfoKeep->pfnVdecDecFin(ucMpvId, ucEsId);

                        _VdecTimeProfile(ucEsId);

                        if(i4RetValue == (INT32)E_VDEC_OK)
                        {
                            //for emulation, get output picture and do notification
                            if(prVdecEsInfoKeep->pfnVdecGetDispPic)
                            {
                                prVdecEsInfoKeep->pfnVdecGetDispPic(ucEsId, (void*)&rDispInfo);
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
                            if(prVdecEsInfoKeep->pfnVdecUpdRptr)
                            {
                                prVdecEsInfoKeep->pfnVdecUpdRptr(ucEsId);
                            }

                            prVdecEsInfoKeep->eDecStatus = VDEC_DEC_HEADER_PARSE;
                            //reset on each pic
                            if(prVdecEsInfoKeep->pfnVdecSWRst)
                            {
								prVdecEsInfoKeep->pfnVdecSWRst(ucMpvId, ucEsId);
                            }

                            _VDEC_DeliverEos(ucEsId);
                            // TODO
                            //_VDEC_UnlockVld(ucEsId, ucMpvId);
                            //VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore) == OSR_OK);
                        }
                        else if(i4RetValue == (INT32) E_VDEC_FAIL)
                        {
                            prVdecEsInfoKeep->eDecStatus = VDEC_DEC_DROP;
                        }
                        else if (i4RetValue == E_VDEC_RETRIGGER)
                        {
                            prVdecEsInfoKeep->eDecStatus = VDEC_DEC_TRIGGER;
                        }
                        else
                        {
                            prVdecEsInfoKeep->eDecStatus = VDEC_DEC_HEADER_PARSE;
                            _VDEC_DeliverEos(ucEsId);
                        }
                    }
                    else if(prVdecEsInfoKeep->eDecStatus == VDEC_DEC_DROP)
                    {
                        if(prVdecEsInfoKeep->pfnVdecDecDrop)
                        {
                            prVdecEsInfoKeep->pfnVdecDecDrop(ucEsId);
                        }

                        _VdecCheckStatus(ucEsId, FALSE);

                        //change state
                        prVdecEsInfoKeep->eDecStatus = VDEC_DEC_HEADER_PARSE;
                        //reset on each pic
                        if(prVdecEsInfoKeep->pfnVdecSWRst)
                        {
							prVdecEsInfoKeep->pfnVdecSWRst(ucMpvId, ucEsId);
                        }

                        _VDEC_DeliverEos(ucEsId);

                        // TODO
                        //_VDEC_UnlockVld(ucEsId, ucMpvId);
                        //VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore) == OSR_OK);
                    }
		         #ifdef CC_VDEC_PRIORITY_ENABLE
					VDEC_ThreadSchedule(ucEsId);
			     #endif
                }
                break;
            }

            case VDEC_ST_STOP:
                //clear queue, clear stored info, do notification
                //dmx flush Q
                _VDEC_LockFlushMutex(ucEsId);
                if (prVdecEsInfo->fgFlushEsmQ)
                {
                    LOG(1, "ES(%d) _VDECStop Flush\n", ucEsId);
                    _VDEC_FlushEs(ucEsId);
                    prVdecEsInfo->fgFlushEsmQ = FALSE;
                    _VDEC_UnlockFlushSema(ucEsId);
                }
                _VDEC_UnlockFlushMutex(ucEsId);

                if(prVdecEsInfoKeep->ePreviousState != prVdecEsInfoKeep->eCurState)
                {   // to prevent clean esinfo twice. we may clean the setting
                    // from mw.
                    if(prVdecEsInfoKeep->pfnVdecStop)
                    {
                        prVdecEsInfoKeep->pfnVdecStop(ucEsId);
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

                    if((prVdecInfo->arVdecVldInfo[prVdecEsInfo->u4VldId].u4LockCount > 0) &&
                       (prVdecInfo->arVdecVldInfo[prVdecEsInfo->u4VldId].ucLockEsId == ucEsId))
                    {
                        // Unlock VLD due to stop video decoder.
                        _VDEC_UnlockVld(ucEsId, (UCHAR)prVdecEsInfo->u4VldId);
                    }

                    UNUSED(VDEC_PowerOff(ucEsId, prVdecEsInfo->u4VldId));

                    _VDEC_ClearEsInfo(ucEsId);

                    _VDEC_UnlockFlushMutex(ucEsId);

                }

                // DTV00212525,[Pre-Test]Always show 'Retrieving Data' after change channel from specail TS
                // MW will call stop even play is fail. even that we have to notify control done
                if(prVdecEsInfoKeep->u4PreviousStopId != prVdecEsInfoKeep->u4CurStopId)
                {
                    //callback to mw after stop process finished, cr: DTV00125552
                    if (prVdecEsInfoKeep->pfDecNfyFct)
                    {
                        prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, VDEC_COND_CTRL_DONE,
                            //(UINT32)VID_DEC_CTRL_STOP, 0);
                            1, 0); //cr: DTV00126433
                    }
                }

				prVdecEsInfoKeep->pfnVdecStartPts=NULL;
                prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_UNKNOWN;
                prVdecEsInfoKeep->ePreviousState = prVdecEsInfoKeep->eCurState;
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

static void _VDEC_IntrudeHandler(UINT32 u4Region, MID_AGENT_ID_T eAgentId, UINT32 u4Addr)
{
    // Handling the intrusion event
    Printf("System halted\n");
    BSP_HaltSystem();
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

    switch(prVdecEsInfoKeep->eCurFMT)
    {
        case VDEC_FMT_H264VER:
        case VDEC_FMT_H264:
            if(prDispInfo->pvMVAddr == NULL)
            {
                return FALSE;
            }
            u4YSize = (UINT32)prDispInfo->pvCAddr - (UINT32)prDispInfo->pvYAddr;
            #if defined(VDEC_IS_POST_MT5365)
            MID_ResetRegion(0);
            MID_ResetRegion(1);
            fgRet &= MID_SetRegionEx2(0, MID_AGENT_CPU,
                (UINT32)prDispInfo->pvYAddr,
                (UINT32)prDispInfo->pvMVAddr + (UINT32)(u4YSize >> 2),
                MID_FLAG_INVERSE,
                ~(1<<ePPAgentId));
            #else
            fgRet &= MID_SetRegionEx(0, ePPAgentId,
                (UINT32)prDispInfo->pvYAddr,
                (UINT32)prDispInfo->pvMVAddr + (UINT32)(u4YSize >> 2),
                MID_FLAG_INVERSE);
            #endif
            fgRet &= MID_EnableRegionProtect(0);
            fgRet &= MID_SetRegion(1, MID_AGENT_CPU,
                (UINT32)prDispInfo->pvCAddr + (UINT32)(u4YSize>>1),
                (UINT32)prDispInfo->pvMVAddr);
            fgRet &= MID_EnableRegionProtect(1);
            break;

        case VDEC_FMT_MPV:
            #if defined(VDEC_IS_POST_MT5365)
            MID_ResetRegion(0);
            MID_ResetRegion(1);
            fgRet &= MID_SetRegionEx2(0, MID_AGENT_CPU,
                (UINT32)prDispInfo->pvYAddr,
                (UINT32)prDispInfo->pvCAddr + (UINT32)(prDispInfo->u4YSize >> 1),
                MID_FLAG_INVERSE,
                ~(1<<MID_AGENT_MPEG1));
            fgRet &= MID_EnableRegionProtect(0);
            fgRet &= MID_SetRegionEx2(1, MID_AGENT_CPU,
                (UINT32)prDispInfo->pvYDbkAddr,
                (UINT32)prDispInfo->pvCDbkAddr + (UINT32)(prDispInfo->u4YSize >> 1),
                MID_FLAG_INVERSE,
                ~(1<<ePPAgentId));
            fgRet &= MID_EnableRegionProtect(1);
            #else
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
            #endif
            break;

        case VDEC_FMT_WMV:
        case VDEC_FMT_MP4:
        case VDEC_FMT_RV:  // !!! megaa 20090516
        case VDEC_FMT_AVS:
            #if defined(VDEC_IS_POST_MT5365)
            MID_ResetRegion(0);
            MID_ResetRegion(1);
            //frame buffer
            fgRet &= MID_SetRegionEx2(0, MID_AGENT_CPU,
                (UINT32)prDispInfo->pvYAddr,
                (UINT32)prDispInfo->pvCAddr + (UINT32)(prDispInfo->u4YSize >> 1),
                MID_FLAG_INVERSE,
                ~(1<<MID_AGENT_MPEG1));
            fgRet &= MID_EnableRegionProtect(0);
            //ac, dc buffer
            FBM_GetWorkingBuffer(prVdecEsInfo->ucFbgId, &u4WorkingBuffer, &u4WorkingSize);
            fgRet &= MID_SetRegionEx2(1, MID_AGENT_CPU, u4WorkingBuffer,
                (u4WorkingBuffer + u4WorkingSize), MID_FLAG_INVERSE,
                ~(1<<ePPAgentId));
            fgRet &= MID_EnableRegionProtect(1);
            #else
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
            #endif
            break;

        case VDEC_FMT_VP6:
        case VDEC_FMT_VP8:
        {
            UINT32 u4Region;
            UINT32 u4AgentList;

            MID_RegisterHandler(_VDEC_IntrudeHandler);
            
            u4Region = 0;
            MID_ResetRegion(u4Region);
            u4AgentList = ~(((UINT32)1 << MID_AGENT_MPEG1)|((UINT32)1 << MID_AGENT_JPEG));
            MID_ResetRegion(u4Region);
            fgRet &= MID_SetRegionEx2(
                u4Region,
                MID_AGENT_CPU,
                (UINT32)prDispInfo->pvYAddr,
                (UINT32)prDispInfo->pvCAddr + (UINT32)(prDispInfo->u4YSize >> 1),
                MID_FLAG_INVERSE,
                u4AgentList);
            fgRet &= MID_EnableRegionProtect(u4Region);

            if ((prDispInfo->pvYDbkAddr != 0) && (prDispInfo->pvCDbkAddr!= 0))
            {
                u4Region = 1;
                MID_ResetRegion(u4Region);
                u4AgentList = ~((UINT32)1 << ePPAgentId);
                MID_ResetRegion(u4Region);
                fgRet &= MID_SetRegionEx2(
                    u4Region,
                    MID_AGENT_CPU,
                    (UINT32)prDispInfo->pvYDbkAddr,
                    (UINT32)prDispInfo->pvCDbkAddr + (UINT32)(prDispInfo->u4YSize >> 1),
                    MID_FLAG_INVERSE,
                    u4AgentList);
                fgRet &= MID_EnableRegionProtect(u4Region);
            }
        }
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

            eVDecFmt = prVdecEsInfoKeep->eCurFMT;
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
#ifdef ENABLE_MULTIMEDIA
                    if(prVdecEsInfo->fgMMPlayback)
                    {
                        #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
                        if((prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS) ||
                           (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_192) ||
                           (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_ZERO_192))
                           // dmx use demux_pvr to write when source is pvr or time-shift.
                           //(prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                           //(prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                        {
                            // 5365 dmx use DDI to move in TS file
                            // for other format, use MID_AGENT_DMX
                            eDmxAgentId = MID_AGENT_DDI;
                        }
                        #else
                        eDmxAgentId = MID_AGENT_DDI;
                        #endif
                    }
#endif
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


void VDEC_FbgReleaseHdlr(UCHAR ucEsId)
{
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if((prVdecEsInfoKeep != NULL) && (prVdecEsInfoKeep->u1FbmMidMode > 0))
    {
        UNUSED(MID_Reset());
    }
}


void VDEC_GenPTS(UCHAR ucEsId, UCHAR ucFbgId, UCHAR ucFbId)
{
    FBM_PIC_HDR_T* prFbmPicHdr;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    BOOL fgChgPTS = FALSE;
    UINT32 u4PrevCnt = 0;

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
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


#ifdef VDEC_TIME_PROFILE
static void _VDEC_CalPicSize(UCHAR ucEsId)
{
    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if (!prVdecEsInfo->fgCalEachPicSize)
    {
        return;
    }

    if (prVdecEsInfo->u4VldReadPtr)
    {
        if (prVdecEsInfoKeep->rPesInfo.u4VldReadPtr > prVdecEsInfo->u4VldReadPtr)
        {
            prVdecEsInfo->u4CurrPicSize = (prVdecEsInfoKeep->rPesInfo.u4VldReadPtr - prVdecEsInfo->u4VldReadPtr);
        }
        else
        {
            prVdecEsInfo->u4CurrPicSize = ((prVdecEsInfo->u4FifoEnd - prVdecEsInfo->u4VldReadPtr) +
                                           (prVdecEsInfoKeep->rPesInfo.u4VldReadPtr - prVdecEsInfoKeep->rPesInfo.u4FifoStart));;
        }
        prVdecEsInfo->u4StatisticPicSize += prVdecEsInfo->u4CurrPicSize;
    }

    if (prVdecEsInfoKeep->rPesInfo.fgEos)
    {
        prVdecEsInfo->fgCalEachPicSize = FALSE;
    }

}
#endif

#ifdef VDEC_VENC_PATH

VOID _VDEC_SetVencFrmSz(UCHAR ucEsId, UINT32 u4Width, UINT32 u4Height, UINT32 u4Pitch)
{
    if (VDEC_MAX_ES > ucEsId)
    {
        _arVdec2VencFlow[ucEsId].u4Width  = u4Width;
        _arVdec2VencFlow[ucEsId].u4Pitch  = u4Pitch;
        _arVdec2VencFlow[ucEsId].u4Height = u4Height;
        LOG(3, "%s: %dx%d\n", __FUNCTION__, u4Width, u4Height);
    }
    else
    {
        LOG(0, "%s: invalid id %d\n", __FUNCTION__, ucEsId);
    }
}

VOID _VDEC_Set2VencFlow(UCHAR ucEsId, BOOL fgOn)
{
    if (VDEC_MAX_ES > ucEsId)
    {
        _arVdec2VencFlow[ucEsId].fgOn  = fgOn;
#ifdef VENC_FRAME_CTRL_BY_DRV
        if (_arVdec2VencFlow[ucEsId].ptFilter)
        {
            _VENC_Filter_Reset(_arVdec2VencFlow[ucEsId].ptFilter);
        }
#endif
        
    }
    else
        {
        LOG(0, "%s: invalid id %d\n", __FUNCTION__, ucEsId);
        }
}

VOID _VDEC_SetVencHandle(UCHAR ucEsId, VOID *pvArg1, VOID *pvArg2, VOID *pvArg3)
{
    if (VDEC_MAX_ES > ucEsId)
    {
        _arVdec2VencFlow[ucEsId].hVEnc = (VENC_HANDLE)pvArg1;
        _arVdec2VencFlow[ucEsId].ptFrmBuffMgr = (VENC_BUFMGR_BASE_T *)pvArg2;
#ifdef VENC_FRAME_CTRL_BY_DRV
        _arVdec2VencFlow[ucEsId].ptFilter = (VENC_FILTER_T *)pvArg3;
#endif
    }
    else
    {
        LOG(0, "%s: invalid id %d\n", __FUNCTION__, ucEsId);
    }

    UNUSED(pvArg3);
}


BOOL _VDEC_fg2VencFlowOn(UCHAR ucEsId)
{
    return (VDEC_MAX_ES > ucEsId)? _arVdec2VencFlow[ucEsId].fgOn : FALSE;
}

BOOL _VDEC_PrepareVEncSourse(UCHAR ucEsId, VDEC_VENC_SRC_INFO_T *prSrcInfo)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VENC_FRM_T    rFrm;
    VENC_IN_PIC_T rIn;
    UINT32 u4Width, u4Height, u4HalfHeight, u4Pitch, u4Pts;

    if (ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "%s: invalid esid - %d\n", __FUNCTION__, ucEsId);
        return FALSE;
    }

    if (prSrcInfo == NULL)
    {
        LOG(1, "%s: prSrcInfo is NULL\n", __FUNCTION__);
        return FALSE;
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    if(!prVdecEsInfoKeep)
    {
        LOG(3, "_VDEC_PrepareVEncSourse prVdecEsInfoKeep null\n");
        return FALSE;
    }

    // Prepare inform VENC data
    u4Width = prSrcInfo->u4Width;
    u4Height = prSrcInfo->u4Height;
    u4HalfHeight = prSrcInfo->u4Height >> 1;
    u4Pitch = prSrcInfo->u4Pitch;
    u4Pts        = prSrcInfo->u4Pts;
    ASSERT((prSrcInfo->u4YAddr & 3) == 0);
    ASSERT((prSrcInfo->u4CAddr & 3) == 0);
    ASSERT((u4Width & 15) == 0);
    ASSERT((u4Pitch & 15) == 0);

#ifdef VENC_FRAME_CTRL_BY_DRV
    //LOG(3, "receive fram, and pts is %d\n", u4Pts);
    if (_arVdec2VencFlow[ucEsId].ptFilter && _VENC_Filter_Do(_arVdec2VencFlow[ucEsId].ptFilter, u4Pts) == FALSE)
    {
        LOG(3, "drop frame by filter, pts is %d\n", u4Pts);
        return FALSE;
    }
#endif

    // copy from VDEC frame buffer to VENC frame buffer
    if (VENC_BUFMGR_OK != _BUFMGR_OBJ_Get(_arVdec2VencFlow[ucEsId].ptFrmBuffMgr, (VOID *)&rFrm, VENC_BUFMGR_WAIT))
    {
        LOG(3, "drop frame because of no frame buffer\n");
        return FALSE;
    }

    ASSERT(_arVdec2VencFlow[ucEsId].u4Width);
    ASSERT(_arVdec2VencFlow[ucEsId].u4Height);
    ASSERT(_arVdec2VencFlow[ucEsId].u4Pitch);

#if 0
    if ((_arVdec2VencFlow[ucEsId].u4Width != u4Width)
        || (_arVdec2VencFlow[ucEsId].u4Height != u4Height)
        || (_arVdec2VencFlow[ucEsId].u4Pitch != u4Pitch))
#endif
    {
        RZ_VDO_SCL_PARAM_SET_T rImgrzScaleParam;
		x_memset(&rImgrzScaleParam,0,sizeof(RZ_VDO_SCL_PARAM_SET_T));
        if((prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MJPEG) ||
           (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_RAW))
        {
            rImgrzScaleParam.u4IsRsIn = 1;
            rImgrzScaleParam.u4SrcSwap = 0;
            rImgrzScaleParam.u4OutSwap = 0;
        }
        else
        {
            rImgrzScaleParam.u4SrcSwap = 6;
            rImgrzScaleParam.u4OutSwap = 0;
        }
        rImgrzScaleParam.u4InMode = E_RZ_VDO_OUTMD_420;
        rImgrzScaleParam.u4YSrcBase = prSrcInfo->u4YAddr;
        rImgrzScaleParam.u4YSrcBufLen = u4Pitch;
        rImgrzScaleParam.u4YSrcHOffset = 0;
        rImgrzScaleParam.u4YSrcVOffset = 0;
        rImgrzScaleParam.u4YSrcW = u4Width;
        rImgrzScaleParam.u4YSrcH = u4Height;
        rImgrzScaleParam.u4CSrcBase = prSrcInfo->u4CAddr;
        rImgrzScaleParam.u4CSrcHOffset = 0;
        rImgrzScaleParam.u4CSrcVOffset = 0;
        rImgrzScaleParam.u4CSrcW       = u4Width >> 1;
        rImgrzScaleParam.u4CSrcH = u4HalfHeight;

        rImgrzScaleParam.u4IsRsOut = 1;
        rImgrzScaleParam.u4OutMode = E_RZ_VDO_OUTMD_420;       // 420|422|444 output
        rImgrzScaleParam.u4IsVdo2Osd = 0;     // output in osd format ?
        rImgrzScaleParam.u4YTgBase     = rFrm.u4YAddr;
        rImgrzScaleParam.u4YTgCM = E_RZ_VDO_OUTMD_420;
        rImgrzScaleParam.u4YTgBufLen   = _arVdec2VencFlow[ucEsId].u4Pitch;
        rImgrzScaleParam.u4YTgHOffset = 0;
        rImgrzScaleParam.u4YTgVOffset = 0;
        rImgrzScaleParam.u4YTgW        = _arVdec2VencFlow[ucEsId].u4Width;
        rImgrzScaleParam.u4YTgH        = _arVdec2VencFlow[ucEsId].u4Height;
        rImgrzScaleParam.u4CTgBase     = rFrm.u4UVAddr;
        rImgrzScaleParam.u4CTgW        = rImgrzScaleParam.u4YTgW >> 1;
        rImgrzScaleParam.u4CTgH        = rImgrzScaleParam.u4YTgH >> 1;

        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rImgrzScaleParam));
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
    }
#if 0
    else
    {
        GFX_Lock();
        GFX_SetSrc((UINT8 *)prSrcInfo->u4YAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);
        GFX_SetDst((UINT8 *)rFrm.u4YAddr, (UINT32)CM_RGB_CLUT8, u4Width);
        GFX_BitBlt(0, 0, 0, 0, u4Width, u4Height);
        GFX_SetSrc((UINT8 *)prSrcInfo->u4CAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);
        GFX_SetDst((UINT8 *)rFrm.u4UVAddr, (UINT32)CM_RGB_CLUT8, u4Width);
        GFX_BitBlt(0, 0, 0, 0, u4Width, u4HalfHeight);
        GFX_Flush();
        GFX_Wait();
        GFX_Unlock();
    }
#endif

    x_memset(&rIn, 0, sizeof(rIn));
    rIn.u4YAddr = rFrm.u4YAddr;
    rIn.u4CAddr = rFrm.u4UVAddr;
    rIn.u4Pts   = u4Pts;
    
    if (VENC_OK != VENC_EncFrameAsync(_arVdec2VencFlow[ucEsId].hVEnc, &rIn, NULL))
    {
        LOG(1, "%s: enc frame fail\n", __FUNCTION__);
        return FALSE;
    }
    else
    {
#ifdef VENC_FRAME_CTRL_BY_DRV
        if (_arVdec2VencFlow[ucEsId].ptFilter)
    {
            _VENC_Filter_Update(_arVdec2VencFlow[ucEsId].ptFilter);
    }
#endif
    }

    return TRUE;
}
#endif // VDEC_VENC_PATH


#ifdef CC_REALD_3D_SUPPORT
VOID _VDEC_TDTVModeChangeCB(void *pvArgv)
{
    UCHAR ucEsId = ES0;//(UCHAR)*(UINT32*)pvArgv;
    UINT8 u1Force2D;
    UINT16 u2InFmt, u2OutFmt;
    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_PES_INFO_T rPesInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if(!prVdecEsInfo)
    {
        return;
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    VERIFY (x_sema_lock(prVdecEsInfoKeep->hVdec3DModeChange, X_SEMA_OPTION_WAIT) == OSR_OK);

    prVdecEsInfo->u43DInOut = u4DrvTDTVMMModeQuery(prVdecEsInfo->eDetectedMode);
    switch(prVdecEsInfo->e3DForce2DType)
    {
        case VDEC_3D_FORCE2D_AUTO:
            vDrvTDTVSetMMFix322Region(E_TDTV_UI_3D_2_2D_OFF);
            break;
        case VDEC_3D_FORCE2D_L_VIEW:
            vDrvTDTVSetMMFix322Region(E_TDTV_UI_3D_2_2D_L);
            break;
        case VDEC_3D_FORCE2D_R_VIEW:
            vDrvTDTVSetMMFix322Region(E_TDTV_UI_3D_2_2D_R);
            break;
    }
        
    u2InFmt = prVdecEsInfo->u43DInOut & 0xFFFF;
    u2OutFmt = (prVdecEsInfo->u43DInOut >> 16) & 0xFFFF;
    switch(u2OutFmt)
    {
        case E_TDTV_DECODER_OUTPUT_NATIVE:
            prVdecEsInfo->u13DUsrCtrlModeNtfy = VDEC_3D_CTRL_BYPASS;
            break;
        case E_TDTV_DECODER_OUTPUT_3D_FS:
            switch(u2InFmt)
            {
                 case E_TDTV_DECODER_INPUT_TB_I:
                 case E_TDTV_DECODER_INPUT_TB_P:
                    prVdecEsInfo->u13DUsrCtrlModeNtfy = VDEC_3D_CTRL_FORCE_3D_TB;
                    break;
                 case E_TDTV_DECODER_INPUT_RD_I:
                 case E_TDTV_DECODER_INPUT_RD_P:
                    //prVdecEsInfo->fgIsRealD3DFmt = TRUE;
                 case E_TDTV_DECODER_INPUT_SBS_I:
                 case E_TDTV_DECODER_INPUT_SBS_P:
                 case E_TDTV_DECODER_INPUT_SS_I:
                 case E_TDTV_DECODER_INPUT_SS_P:
                    prVdecEsInfo->u13DUsrCtrlModeNtfy = VDEC_3D_CTRL_FORCE_3D_LR;
                    break;
                default:
                    prVdecEsInfo->u13DUsrCtrlModeNtfy = VDEC_3D_CTRL_OFF;
                    break;
            }
            break;
        case E_TDTV_DECODER_OUTPUT_3D_2_2D:
            switch(u2InFmt)
            {
                 case E_TDTV_DECODER_INPUT_TB_I:
                 case E_TDTV_DECODER_INPUT_TB_P:
                    prVdecEsInfo->u13DUsrCtrlModeNtfy = VDEC_3D_CTRL_FORCE_2D_TB;
                    break;
                 case E_TDTV_DECODER_INPUT_RD_I:
                 case E_TDTV_DECODER_INPUT_RD_P:
                    //prVdecEsInfo->fgIsRealD3DFmt = TRUE;
                 case E_TDTV_DECODER_INPUT_SBS_I:
                 case E_TDTV_DECODER_INPUT_SBS_P:
                 case E_TDTV_DECODER_INPUT_SS_I:
                 case E_TDTV_DECODER_INPUT_SS_P:
                    prVdecEsInfo->u13DUsrCtrlModeNtfy = VDEC_3D_CTRL_FORCE_2D_LR;
                    break;
                default:
                    prVdecEsInfo->u13DUsrCtrlModeNtfy = VDEC_3D_CTRL_FORCE_2D;
                    break;
            }
            vTV3DGetForce2D(0, &u1Force2D, &prVdecEsInfo->u1IsForceLViewNtfy);
            break;
        default:
            prVdecEsInfo->u13DUsrCtrlModeNtfy = VDEC_3D_CTRL_OFF;   //follow original content
            break;
    }

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_3D_MODE_CHANGE); //ucEsId should be 0

    x_memset((void*)&rPesInfo, 0, sizeof(rPesInfo));
    rPesInfo.ucEsId = ucEsId;
    rPesInfo.ucMpvId = (UCHAR)VDEC_MAX_VLD;
    VERIFY(VDEC_SendEs(&rPesInfo) == 1);

    UNUSED(u1Force2D);

    VERIFY (x_sema_unlock(prVdecEsInfoKeep->hVdec3DModeChange) == OSR_OK);
}


//support RealD 3D tag detection
VOID _VDEC_Check2D3DSwitch(UCHAR ucEsId, UCHAR ucFbId)
{
    UINT32* pu4DecData;
    UINT32 u4Offset;
    UINT32 u4WIdx, u4BlkSz;
    static UINT32* pu4TmpBufY = NULL;
    static UINT32* pu4TmpBufC = NULL;
    static UINT8* pu1ARGBBuf = NULL;
    GFX_COLOR_CONVERT_T rGfxColorConvert;
    UINT32* pu4Dst;
    UINT8* pu1Dst;
    UINT32 u4UnqualifiedCnt = 0;
    UINT8 u1Exp, u1S1, u1S2, u1BlockIdx, u1PixIdx, u1QuincunxCnt = 0, u13DType = 0;
    BOOL bIsCur2D, bIs3DQualify = FALSE, bIsFHD = TRUE, bChkFilterType = FALSE;
    VDEC_DISP_PIC_INFO_T rDispInfo;

    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep;
    FBM_SEQ_HDR_T * prFbmSeqHdr;
    E_TDTV_DECODER_INPUT eDetectedMode = E_TDTV_DECODER_INPUT_2D_P;

    //HAL_TIME_T rTimeS, rTimeE, rTimeDt;
    //HAL_GetTime(&rTimeS);

    ASSERT(ucEsId < MPV_MAX_ES);

    if(ucEsId > 0)  //b2r only triggered by main path setting
    {
        return;
    }
    if(ucFbId == 0xFF)
    {
        LOG(2, "3D check FbId = 0xFF\n");
        return;
    }
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    if((!prVdecEsInfo) || (!prVdecEsInfoKeep))
    {
        return;
    }
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdecEsInfo->ucFbgId);
    if(!prFbmSeqHdr)
    {
        return;
    }

    x_memset((void*)&rDispInfo, 0, sizeof(rDispInfo));
    bIsCur2D = !prVdecEsInfo->fgIsRealD3DFmt;

    if(pu4TmpBufY == NULL)
    {
        pu4TmpBufY = (UINT32*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(1920, 1024));
        ASSERT(pu4TmpBufY);
    }
    if(pu4TmpBufC == NULL)
    {
        pu4TmpBufC = (UINT32*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(1920, 1024));
        ASSERT(pu4TmpBufC);
    }
    if(pu1ARGBBuf == NULL)
    {
        pu1ARGBBuf = (UINT8*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(1920 << 2, 1024));
        ASSERT(pu1ARGBBuf);
    }

    FBM_GetFrameBufferAddr(prVdecEsInfo->ucFbgId, ucFbId, (UINT32*)&rDispInfo.pvYAddr, (UINT32*)&rDispInfo.pvCAddr);
    rDispInfo.u4W = (UINT32)prFbmSeqHdr->u2LineSize;
    rDispInfo.u4W_Cmp = (UINT32)prFbmSeqHdr->u2HSize;
    rDispInfo.u4H = ((UINT32)prFbmSeqHdr->u2VSize + 0xF) & (~0xF);
    rDispInfo.u4H_Cmp = ((UINT32)prFbmSeqHdr->u2VSize + 0xF) & (~0xF);

    //file recognize from MM
    switch(prVdecEsInfo->e3DType)
    {
        case VDEC_3D_NONE:
            if(!IS_Support3D()) //bonding check
                break;

            // VOMX decode to OSD output
            if((prVdecEsInfoKeep->fgVPush) && (prVdecEsInfoKeep->fgVPushDecodeOnly))
            {
                break;
            }

            if((rDispInfo.u4W_Cmp != 1920) && (rDispInfo.u4W_Cmp != 1280))
                break;	//RealD only support these two resolution

            //Y start addr of the last line, block 2 raster
            u4Offset = rDispInfo.u4H % 32;
            u4Offset = (u4Offset > 0) ? u4Offset : 32;
            u4BlkSz = 1 << 9; //(64x32) / 4, uint8 to uint32
            pu4Dst = pu4TmpBufY;

            pu4DecData = (UINT32*)((UINT8*)VIRTUAL((UINT32)rDispInfo.pvYAddr)
                + rDispInfo.u4W * (rDispInfo.u4H - u4Offset)
                + 64 * (u4Offset - 2));

            for(u4WIdx = 0; u4WIdx < (rDispInfo.u4W_Cmp - 64); u4WIdx += 64)
            {
                x_memcpy((void*)pu4Dst, (void*)pu4DecData, 64);   // 16 x4 byte
                pu4DecData += u4BlkSz;
                pu4Dst += 16;
            }
            x_memcpy((void*)pu4Dst, (void*)pu4DecData, rDispInfo.u4W_Cmp % 64);

            //C start addr of the last line, block 2 raster
            u4Offset = (rDispInfo.u4H>>1) % 16;
            u4Offset = (u4Offset > 0) ? u4Offset : 16;
            u4BlkSz = 1 << 8; //64x16 / 4
            pu4Dst = pu4TmpBufC;

            pu4DecData = (UINT32*)((UINT8*)VIRTUAL((UINT32)rDispInfo.pvCAddr)
                + rDispInfo.u4W * ((rDispInfo.u4H>>1) - u4Offset)
                + 64 * (u4Offset - 1));

            for(u4WIdx = 0; u4WIdx < (rDispInfo.u4W_Cmp - 64); u4WIdx += 64)
            {
                x_memcpy((void*)pu4Dst, (void*)pu4DecData, 64);
                pu4DecData += u4BlkSz;
                pu4Dst += 16;
            }
            x_memcpy((void*)pu4Dst, (void*)pu4DecData, rDispInfo.u4W_Cmp % 64);

            HalFlushInvalidateDCacheMultipleLine((UINT32)pu4TmpBufY, rDispInfo.u4W);
            HalFlushInvalidateDCacheMultipleLine((UINT32)pu4TmpBufC, rDispInfo.u4W);

            //color convert
            x_memset((void*)&rGfxColorConvert, 0, sizeof(GFX_COLOR_CONVERT_T));

            rGfxColorConvert.pu1SrcAddr = (UINT8*)PHYSICAL((UINT32)pu4TmpBufY);
            rGfxColorConvert.pu1CbCr = (UINT8*)PHYSICAL((UINT32)pu4TmpBufC);
            rGfxColorConvert.pu1DstAddr = (UINT8*)PHYSICAL((UINT32)pu1ARGBBuf);
            rGfxColorConvert.u1Alpha = 0xFF;
            rGfxColorConvert.u4DstCM = CM_ARGB8888_DIRECT32;
            rGfxColorConvert.u4DstPitch = rDispInfo.u4W;
            rGfxColorConvert.u4DstX = 0;
            rGfxColorConvert.u4DstY = 0;
            rGfxColorConvert.u4Height = 1;
            rGfxColorConvert.u4Width = rDispInfo.u4W;
            _GFX_ColorConvert(&rGfxColorConvert);

            HalFlushInvalidateDCacheMultipleLine((UINT32)pu1ARGBBuf, rDispInfo.u4W<<2);

            //check 3D tag
            bIsFHD = (rDispInfo.u4W_Cmp == 1920) ? TRUE : FALSE;
            pu1Dst = (UINT8*)VIRTUAL((UINT32)pu1ARGBBuf);
            u1BlockIdx = (bIsFHD) ? 0 : 1; //720p start from 1, 1080p start from 0
            u1PixIdx = 0; //start from 0
            for(u4WIdx = 0; u4WIdx < rDispInfo.u4W_Cmp; u4WIdx++)
            {
                if(bIsFHD)
                {
                    u1PixIdx = u4WIdx % 32;
                    if(u1PixIdx == 0)
                    {
                        u1BlockIdx++;
                    }
                }
                else //1280
                {
                    u1PixIdx = (u4WIdx ==0) ? 0 : (u1PixIdx + 1);
                    if((((u1BlockIdx & 0x3) == 3) && (u1PixIdx == 22)) ||
                        (((u1BlockIdx & 0x3) < 3) && (u1PixIdx == 21)))
                    {
                        u1PixIdx = 0;
                        u1BlockIdx++;
                    }
                }
                if(u4WIdx == (rDispInfo.u4W_Cmp >> 1)) //left blocks are use to distinguish filter type
                {
                    u1BlockIdx--;
                }
                if((bIsFHD && ((u4WIdx >> 5) >= 54)) ||
                    ((!bIsFHD) && (u4WIdx >= 1152)))    //start checking filter type at 54 block, u1BlockIdx type=3
                {
                    if(!bChkFilterType) //change point
                    {
                        u1BlockIdx = 2; //default: simple horizontal squeeze type
                    }
                    bChkFilterType = TRUE;
                }
                u1BlockIdx = (u1BlockIdx == 4) ? 1 : u1BlockIdx;

                if((u1PixIdx < 5) ||
                    (bIsFHD && (u1PixIdx > 26)) ||
                    ((!bIsFHD) && (u1PixIdx > 16)))
                {
                    pu1Dst+=4;// skip
                    continue;
                }

                if(u1BlockIdx == 1)
                {
                    u1S2 = *pu1Dst++;
                    u1S1 = *pu1Dst++;
                    u1Exp = *pu1Dst++;
                }
                else if(u1BlockIdx == 2)
                {
                    u1S2 = *pu1Dst++;
                    u1Exp = *pu1Dst++;
                    u1S1 = *pu1Dst++;
                }
                else
                {
                    u1Exp = *pu1Dst++;
                    u1S2 = *pu1Dst++;
                    u1S1 = *pu1Dst++;
                }

                pu1Dst++; //alpha
                if(bIsCur2D)
                {
                    if((u1Exp < (u1S1 + 75)) || (u1Exp < (u1S2 + 75)))
                    {
                        if(!bChkFilterType)
                        {
                        u4UnqualifiedCnt++;
                    }
                        else
                        {
                            //quincunx
                            u1QuincunxCnt++;
                        }
                    }
                }
                else
                {
                    if((u1Exp < (u1S1 + 55)) && (u1Exp < (u1S2 + 55)))
                    {
                        if(!bChkFilterType)
                        {
                        u4UnqualifiedCnt++;
                        }
                        else
                        {
                            //quincunx
                            u1QuincunxCnt++;
                        }
                    }
                }
            }

            if((bIsCur2D && (u4UnqualifiedCnt < 125)) ||
                (!bIsCur2D && (u4UnqualifiedCnt < 190)))
            {
                bIs3DQualify = TRUE;
                u13DType = B2R_3D_SIDE_BY_SIDE_REAL_D;
                prFbmSeqHdr->u4B2R3DWidthOffset = rDispInfo.u4W_Cmp >> 1;
                prFbmSeqHdr->u4B2R3DHeightOffset = 0;
                //unqualify threshold is set by ratio 54blk: 6blk ~= 190: 21
                if((u1QuincunxCnt < 42) && (!prVdecEsInfo->fgIsSimpleFilterType))
                {
                    prVdecEsInfo->fgIsSimpleFilterType = TRUE;
                    LOG(3, "DetectFilterType SHS\n");
                }
                eDetectedMode = E_TDTV_DECODER_INPUT_RD_P;
            }
            prVdecEsInfo->fgIsRealD3DFmt = bIs3DQualify;
            break;
        case VDEC_3D_SBS_LF:
            bIs3DQualify = TRUE;
            u13DType = B2R_3D_SIDE_BY_SIDE;
            prFbmSeqHdr->u4B2R3DWidthOffset = rDispInfo.u4W_Cmp >> 1;
            prFbmSeqHdr->u4B2R3DHeightOffset = 0;
            eDetectedMode = E_TDTV_DECODER_INPUT_SBS_P;
            break;
        case VDEC_3D_SBS_RF:
            bIs3DQualify = TRUE;
            u13DType = B2R_3D_SIDE_BY_SIDE;
            prFbmSeqHdr->u4B2R3DWidthOffset = rDispInfo.u4W_Cmp >> 1;
            prFbmSeqHdr->u4B2R3DHeightOffset = 0;
            eDetectedMode = E_TDTV_DECODER_INPUT_SBS_INV_P;
            break;
        case VDEC_3D_TB_LF:
            bIs3DQualify = TRUE;
            u13DType = B2R_3D_TOP_N_BOTTOM;
            prFbmSeqHdr->u4B2R3DWidthOffset = 0;
            prFbmSeqHdr->u4B2R3DHeightOffset = prFbmSeqHdr->u2VSize >> 1;
            eDetectedMode = E_TDTV_DECODER_INPUT_TB_P;
            break;
        case VDEC_3D_TB_RF:
            bIs3DQualify = TRUE;
            u13DType = B2R_3D_TOP_N_BOTTOM;
            prFbmSeqHdr->u4B2R3DWidthOffset = 0;
            prFbmSeqHdr->u4B2R3DHeightOffset = prFbmSeqHdr->u2VSize >> 1;
            eDetectedMode = E_TDTV_DECODER_INPUT_TB_INV_P;
            break;
        case VDEC_3D_DUAL_STREAM:
        case VDEC_3D_MVC:
            bIs3DQualify = TRUE;
            u13DType = B2R_3D_FRAME_SEQUENTIAL;
            eDetectedMode = E_TDTV_DECODER_INPUT_FS_P;
            break;
        case VDEC_3D_REALD_DUAL_STREAM:
            bIs3DQualify = TRUE;
            u13DType = B2R_3D_FRAME_SEQUENTIAL;
            eDetectedMode = E_TDTV_DECODER_INPUT_FS_P;
            prFbmSeqHdr->fgRealDEnhance = TRUE;
            break;
        default:
            break;
    }
    prFbmSeqHdr->u1B2R3DType = u13DType;

    //interlace
    if(!prFbmSeqHdr->fgProgressiveSeq)
    {
        eDetectedMode -= 1;
    }
    if(eDetectedMode != prVdecEsInfo->eDetectedMode)
    {
        prVdecEsInfo->eDetectedMode = eDetectedMode;
        _VDEC_TDTVModeChangeCB((void*)&ucEsId);

        //content format change -> use new mode directly, without waiting vdec command
        prVdecEsInfo->u13DUsrCtrlMode = prVdecEsInfo->u13DUsrCtrlModeNtfy;
        prVdecEsInfo->u1IsForceLView = prVdecEsInfo->u1IsForceLViewNtfy;
    }

    //HAL_GetTime(&rTimeE);
    //HAL_GetDeltaTime(&rTimeDt, &rTimeS, &rTimeE);
    //LOG(5, "bIs3DQualify %d, u4UnqualifiedCnt %d, %u.%06us\n", bIs3DQualify, u4UnqualifiedCnt,
    //    rTimeDt.u4Seconds, rTimeDt.u4Micros);

    // 3d display setting by central control module
    if(prVdecEsInfo->u13DUsrCtrlMode == VDEC_3D_CTRL_FORCE_2D)
    {
        if((prFbmSeqHdr->fgB2R3DEnable != bIs3DQualify) ||
            (!prFbmSeqHdr->fgB2RForce2D) ||
            (prFbmSeqHdr->u1B2RForce2DType != prVdecEsInfo->u1IsForceLView))
        {
            prFbmSeqHdr->fgB2R3DEnable = bIs3DQualify;
            prFbmSeqHdr->u1B2R3DType = u13DType;
            prFbmSeqHdr->fgB2RForce2D = TRUE;
            prFbmSeqHdr->u1B2RForce2DType = prVdecEsInfo->u1IsForceLView;    // 0:L, 1:R
            LOG(3, "[VDEC_3D]Force 2D, view %d\n",  prFbmSeqHdr->u1B2RForce2DType);
            FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_3D_CHG_FLAG);
        }
    }
    else if(prVdecEsInfo->u13DUsrCtrlMode == VDEC_3D_CTRL_FORCE_2D_LR)
    {
        if((!prFbmSeqHdr->fgB2R3DEnable) ||
            (!prFbmSeqHdr->fgB2RForce2D) ||
            ((prFbmSeqHdr->u1B2R3DType != B2R_3D_SIDE_BY_SIDE_REAL_D) &&
            (prFbmSeqHdr->u1B2R3DType != B2R_3D_SIDE_BY_SIDE)) ||
            (prVdecEsInfo->fgIsRealD3DFmt && (prFbmSeqHdr->u1B2R3DType != B2R_3D_SIDE_BY_SIDE_REAL_D)) ||
            (!prVdecEsInfo->fgIsRealD3DFmt && (prFbmSeqHdr->u1B2R3DType == B2R_3D_SIDE_BY_SIDE_REAL_D)) ||
            (prFbmSeqHdr->u1B2RForce2DType != prVdecEsInfo->u1IsForceLView))
        {
            prFbmSeqHdr->fgB2R3DEnable = TRUE;
            prFbmSeqHdr->fgB2RForce2D = TRUE;
            prFbmSeqHdr->u1B2RForce2DType = prVdecEsInfo->u1IsForceLView;    // 0:L, 1:R
            prFbmSeqHdr->u4B2R3DWidthOffset = rDispInfo.u4W_Cmp >> 1;
            prFbmSeqHdr->u4B2R3DHeightOffset = 0;
            prFbmSeqHdr->u1B2R3DType =
                prVdecEsInfo->fgIsRealD3DFmt ? B2R_3D_SIDE_BY_SIDE_REAL_D : B2R_3D_SIDE_BY_SIDE;
            LOG(3, "[VDEC_3D]Force 2D LF, IsRealD %d, view %d\n",
                prVdecEsInfo->fgIsRealD3DFmt, prFbmSeqHdr->u1B2RForce2DType);
            FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_3D_CHG_FLAG);
        }
    }
    else if(prVdecEsInfo->u13DUsrCtrlMode == VDEC_3D_CTRL_FORCE_2D_TB)
    {
        if(!prFbmSeqHdr->fgB2RForce2D ||
            (!prFbmSeqHdr->fgB2R3DEnable) ||
            (prFbmSeqHdr->u1B2R3DType != B2R_3D_TOP_N_BOTTOM) ||
            (prFbmSeqHdr->u1B2RForce2DType != prVdecEsInfo->u1IsForceLView))
        {
            prFbmSeqHdr->fgB2R3DEnable = TRUE;
            prFbmSeqHdr->fgB2RForce2D = TRUE;
            prFbmSeqHdr->u1B2RForce2DType = prVdecEsInfo->u1IsForceLView;    // 0:L, 1:R
            prFbmSeqHdr->u4B2R3DWidthOffset = 0;
            prFbmSeqHdr->u4B2R3DHeightOffset = prFbmSeqHdr->u2VSize >> 1;
            prFbmSeqHdr->u1B2R3DType = B2R_3D_TOP_N_BOTTOM;
            LOG(3, "[VDEC_3D]Force 2D TB, view %d\n", prFbmSeqHdr->u1B2RForce2DType);
            FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_3D_CHG_FLAG);
        }
    }
    else if(prVdecEsInfo->u13DUsrCtrlMode == VDEC_3D_CTRL_FORCE_3D_LR)
    {
        if(prFbmSeqHdr->fgB2RForce2D ||
            (!prFbmSeqHdr->fgB2R3DEnable) ||
            ((prFbmSeqHdr->u1B2R3DType != B2R_3D_SIDE_BY_SIDE_REAL_D) &&
            (prFbmSeqHdr->u1B2R3DType != B2R_3D_SIDE_BY_SIDE)) ||
            (prVdecEsInfo->fgIsRealD3DFmt && (prFbmSeqHdr->u1B2R3DType != B2R_3D_SIDE_BY_SIDE_REAL_D)) ||
            (!prVdecEsInfo->fgIsRealD3DFmt && (prFbmSeqHdr->u1B2R3DType == B2R_3D_SIDE_BY_SIDE_REAL_D)))
        {
            prFbmSeqHdr->fgB2R3DEnable = TRUE;
            prFbmSeqHdr->fgB2RForce2D = FALSE;
            prFbmSeqHdr->u4B2R3DWidthOffset = rDispInfo.u4W_Cmp >> 1;
            prFbmSeqHdr->u4B2R3DHeightOffset = 0;
            prFbmSeqHdr->u1B2R3DType =
                prVdecEsInfo->fgIsRealD3DFmt ? B2R_3D_SIDE_BY_SIDE_REAL_D : B2R_3D_SIDE_BY_SIDE;
            LOG(3, "[VDEC_3D]Force 3D LR, IsRealD %d\n", prVdecEsInfo->fgIsRealD3DFmt);
            FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_3D_CHG_FLAG);
        }
    }
    else if(prVdecEsInfo->u13DUsrCtrlMode == VDEC_3D_CTRL_FORCE_3D_TB)
    {
        if(prFbmSeqHdr->fgB2RForce2D ||
            (!prFbmSeqHdr->fgB2R3DEnable) ||
            (prFbmSeqHdr->u1B2R3DType != B2R_3D_TOP_N_BOTTOM))
        {
            prFbmSeqHdr->fgB2R3DEnable = TRUE;
            prFbmSeqHdr->fgB2RForce2D = FALSE;
            prFbmSeqHdr->u4B2R3DWidthOffset = 0;
            prFbmSeqHdr->u4B2R3DHeightOffset = prFbmSeqHdr->u2VSize >> 1;
            prFbmSeqHdr->u1B2R3DType = B2R_3D_TOP_N_BOTTOM;
            LOG(3, "[VDEC_3D]Force 3D TB\n");
            FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_3D_CHG_FLAG);
        }
    }
    else if(prVdecEsInfo->u13DUsrCtrlMode == VDEC_3D_CTRL_BYPASS)
    {
        if(prFbmSeqHdr->fgB2R3DEnable)
        {
            prFbmSeqHdr->fgB2R3DEnable = FALSE;
            prFbmSeqHdr->fgB2RForce2D = FALSE;
            prFbmSeqHdr->u4B2R3DWidthOffset = 0;
            prFbmSeqHdr->u4B2R3DHeightOffset = 0;
            LOG(3, "[VDEC_3D]Bypass 3D handling\n");
            FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_3D_CHG_FLAG);
        }
    }
    else
    {
        if(prFbmSeqHdr->fgB2RForce2D ||
            (bIs3DQualify !=prFbmSeqHdr->fgB2R3DEnable) ||
            (bIs3DQualify && (prFbmSeqHdr->u1B2R3DType != u13DType)))
        {
            prFbmSeqHdr->fgB2R3DEnable = bIs3DQualify;
            if(bIs3DQualify)
            {
                prFbmSeqHdr->u1B2R3DType = u13DType;
            }
            if(!(prFbmSeqHdr->fgB2RForce2D && (!bIs3DQualify))) // 2D to 2D
            {
                LOG(3, "[VDEC_3D]SetSeqChange Is3D=%d, e3DType %d\n", bIs3DQualify, prVdecEsInfo->e3DType);
                FBM_SetFrameBufferPicFlag(prVdecEsInfo->ucFbgId, ucFbId, FBM_MM_3D_CHG_FLAG);
            }
            prFbmSeqHdr->fgB2RForce2D = FALSE;
        }
    }
}
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



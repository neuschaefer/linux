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
 * $RCSfile: vdec_esm.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_esm.c
 *  This file contains ES manager of MPV.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "mpv_if.h"
#include "vdec_if.h"
#include "vdec_drvif.h"
#include "mpv_drvif.h"
#include "vdec_flw.h"
#include "vdec_type.h"
#include "vdec_debug.h"
#if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
#ifndef CC_VDEC_H264_DISABLE
#include "vdec_h264api.h"
#endif
#endif
LINT_EXT_HEADER_BEGIN
#include "fbm_drvif.h"
#include "dmx_if.h"
#include "drv_dbase.h"
#include "drv_common.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_syslog.h"
#include "x_timer.h"
#include "x_hal_5381.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//static UCHAR _ucEsInfoInit = 0;
//static HANDLE_T _ahFlushSema[VDEC_MAX_ES];
//static HANDLE_T _ahFlushMutex[VDEC_MAX_ES];
//static HANDLE_T _ahCcSema[VDEC_MAX_ES];
static DRV_DBASE_T* _prDbase = NULL;
static VDEC_INFO_T _rVdecInfo;
static VDEC_PES_INFO_T _rPesInfoEsm;
//static ESMQ_T _arEsmQ[VDEC_MAX_ES];
//static MPV_ES_INFO_T _arEsInfo[VDEC_MAX_ES];
//static MPV_INFO_T _rMpvInfo;
//static MPV_CC_T _arMpvCc[VDEC_MAX_ES];
//static MPV_SDAL_INFO_T _rSdalInfo;

#ifdef MPV_VIRTUAL_PRS
static BOOL _fgMpvVirtualPrsStart = FALSE;
static BOOL _fgMpvVirtualPrsDestroy = FALSE;
static UINT32 _ucMpvVirtualPrsDelay = 0;
static UCHAR _ucMpvVPEsNs = 0;
static UCHAR _ucMpvVpEsId[VDEC_MAX_ES];
static UCHAR _ucMpvVpMpvId[VDEC_MAX_ES];
static UINT32 _u4MpvVpPicNs[VDEC_MAX_ES];
static UINT32 _u4MpvVpPicCount[VDEC_MAX_ES];
static UINT32 _u4MpvVpStartAddr[VDEC_MAX_ES];
static UINT32* _u4MpvVpAddr[VDEC_MAX_ES];
static UCHAR* _u4MpvVpPicTp[VDEC_MAX_ES];
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

VDEC_INFO_T* _VDEC_GetInfo(void)
{
    return (VDEC_INFO_T*)&(_rVdecInfo);
}

VDEC_ES_INFO_T* _VDEC_GetEsInfo(UCHAR ucEsId)
{
    return &(_rVdecInfo.arVdecEsInfo[ucEsId]);
}

VDEC_ES_INFO_KEEP_T* _VDEC_GetEsInfoKeep(UCHAR ucEsId)
{
    return &(_rVdecInfo.arVdecEsInfoKeep[ucEsId]);
}

VDEC_PES_INFO_T* _VDEC_GetPesInfo()
{
    return &_rVdecInfo.rPesInfo;
}

void _VDEC_CreateEsmSema(UCHAR ucEsId)
{
    CHAR szBuf[16];
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);    
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    #ifdef MPV_VIRTUAL_PRS
    prVdecEsInfo->u2UdfNs = 0;
    #else
    prVdecEsInfo->u2UdfNs = MPV_UDF_PES_NS;
    #endif

    //ilii test
    //VDEC_PES_INFO_T rPesInfo;
    
    // create semaphore
    prVdecEsInfo->ucStarted = 0;
    VERIFY(x_sema_create(&prVdecEsInfoKeep->hUnderFlowSema, X_SEMA_TYPE_BINARY,
        X_SEMA_STATE_LOCK) == OSR_OK);
            
    // create message queue
    SPrintf(szBuf, "ESM-Q%d\n", ucEsId);
    VERIFY(x_msg_q_create(&prVdecEsInfoKeep->hMsgQ, szBuf,
        sizeof(VDEC_PES_INFO_T), VDEC_MSG_Q_SIZE) == OSR_OK);
            
    VERIFY(x_sema_create(&prVdecEsInfoKeep->hMutex, X_SEMA_TYPE_MUTEX,
        X_SEMA_STATE_UNLOCK) == OSR_OK);

    VERIFY(x_sema_create(&prVdecEsInfoKeep->hFlushSema, X_SEMA_TYPE_BINARY,
        X_SEMA_STATE_LOCK) == OSR_OK);


    VERIFY(x_sema_create(&prVdecEsInfoKeep->hFlushMutex, X_SEMA_TYPE_MUTEX,
        X_SEMA_STATE_UNLOCK) == OSR_OK);

    VERIFY(x_sema_create(&prVdecEsInfoKeep->hCcSema, X_SEMA_TYPE_MUTEX,
        X_SEMA_STATE_UNLOCK) == OSR_OK);

    // create command queue
    SPrintf(szBuf, "CMDQ-Q%d\n", ucEsId);
    VERIFY(x_msg_q_create(&prVdecEsInfoKeep->hCmdQ, szBuf, VDEC_CMD_SIZE,
        VDEC_CMD_Q_SIZE) == OSR_OK);

    VERIFY(x_timer_create(&prVdecEsInfoKeep->hDataTimer) == OSR_OK);
    VERIFY(x_timer_create(&prVdecEsInfoKeep->hCodecChkTimer) == OSR_OK);
}


void _VDEC_DeleteEsmSema(UCHAR ucEsId)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    
    // delete semaphore
    VERIFY(x_sema_delete(prVdecEsInfoKeep->hUnderFlowSema)== OSR_OK);

    // delete message queue
    VERIFY(x_msg_q_delete(prVdecEsInfoKeep->hMsgQ)== OSR_OK);

    VERIFY(x_sema_delete(prVdecEsInfoKeep->hMutex) == OSR_OK);

    VERIFY (x_sema_delete(prVdecEsInfoKeep->hFlushSema) == OSR_OK);

    VERIFY(x_sema_delete(prVdecEsInfoKeep->hFlushMutex) == OSR_OK);

    VERIFY(x_sema_delete(prVdecEsInfoKeep->hCcSema) == OSR_OK);

    // delete command queue
    VERIFY(x_msg_q_delete(prVdecEsInfoKeep->hCmdQ)== OSR_OK);
    
    VERIFY(x_timer_delete(prVdecEsInfoKeep->hDataTimer)== OSR_OK);
    
    VERIFY(x_timer_delete(prVdecEsInfoKeep->hCodecChkTimer)== OSR_OK);
}


void _VDEC_LockFlushSema(UCHAR ucEsId)
{
    VERIFY(x_sema_lock(_rVdecInfo.arVdecEsInfoKeep[ucEsId].hFlushSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}


void _VDEC_UnlockFlushSema(UCHAR ucEsId)
{
    VERIFY(x_sema_unlock(_rVdecInfo.arVdecEsInfoKeep[ucEsId].hFlushSema) == OSR_OK);
}


void _VDEC_LockFlushMutex(UCHAR ucEsId)
{
    VERIFY(x_sema_lock(_rVdecInfo.arVdecEsInfoKeep[ucEsId].hFlushMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
}


void _VDEC_UnlockFlushMutex(UCHAR ucEsId)
{
    VERIFY(x_sema_unlock(_rVdecInfo.arVdecEsInfoKeep[ucEsId].hFlushMutex) == OSR_OK);
}


void _VDEC_LockCcSema(UCHAR ucEsId)
{
    VERIFY(x_sema_lock(_rVdecInfo.arVdecEsInfoKeep[ucEsId].hCcSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}


void _VDEC_UnlockCcSema(UCHAR ucEsId)
{
    VERIFY(x_sema_unlock(_rVdecInfo.arVdecEsInfoKeep[ucEsId].hCcSema) == OSR_OK);
}


void _VDEC_ClearEsInfo(UCHAR ucEsId)
{
    CRIT_STATE_T _rCritState;
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);   
    VDEC_INFO_T *prVdecInfo = (VDEC_INFO_T *)&_rVdecInfo;
    
    x_memset((void*)prVdecEsInfo, 0, sizeof(VDEC_ES_INFO_T));
        
    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));

    // if other format want to use prMpvCounter
    // please move it out of this score.
    prVdecEsInfo->prMpvCounter = (PARAM_MPV_T*)&(_prDbase->rMpv[ucEsId]);
    x_memset((void*)prVdecEsInfo->prMpvCounter, 0, sizeof(PARAM_MPV_T));
    x_memset((void*)&prVdecInfo->rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    x_memset((void*)&_rPesInfoEsm, 0, sizeof(VDEC_PES_INFO_T));
        
    prVdecEsInfo->ucFbgId = FBM_FBG_ID_UNKNOWN;
    prVdecEsInfo->ucFbId = FBM_FB_ID_UNKNOWN;
#ifdef CC_MPV_DEBLOCKING
    prVdecEsInfo->ucDbkFbId = FBM_FB_ID_UNKNOWN;
    prVdecEsInfo->ucDbkRefId = FBM_FB_ID_UNKNOWN;    
#endif
    //default No AV Sync
    prVdecEsInfo->u4WaitDispTime = VDEC_WAIT_DISP_TIME;

    prVdecEsInfo->u4AvgPicSize = VDEC_AVG_PIC_SIZE;
    
    prVdecEsInfo->ucVideoFmt = VDEC_DEFAULT_VIDEO_FORMAT;    
    
    prVdecEsInfo->u4BigBLevel1 = VDEC_BIG_B_LEVEL1;
    prVdecEsInfo->u4BigBLevel2 = VDEC_BIG_B_LEVEL2;
    prVdecEsInfo->u4BigBLevel3 = VDEC_BIG_B_LEVEL3;
    prVdecEsInfo->u4BigBBaseCnt = VDEC_BIG_B_BASE_COUNT;
    
    // clean Vdec thumbnail count
    prVdecEsInfo->fgRenderPicCnt = 0;
    prVdecEsInfo->u4RenderPicCnt = 0;
        
    if(prVdecInfo->eCurFMT == VDEC_FMT_MPV)
    {
        _MPV_ClearEsInfo(ucEsId);
    }
}



void _VDEC_ClearInfo(void)
{
    UCHAR ucIdx;
    for (ucIdx = 0; ucIdx < VDEC_MAX_ES; ucIdx++)
    {
    }
}


void _VDEC_InitInfo(void)
{
    UCHAR ucIdx;

    for (ucIdx = 0; ucIdx < VDEC_MAX_ES; ucIdx++)
    {
        _VDEC_ClearEsInfo(ucIdx);
        _MPV_ClearMpvCc(ucIdx);
        _MPV_VparserReloadEs(ucIdx);
        
    }
    _VDEC_ClearInfo();        
}


void _VDEC_FlushEs(UCHAR ucEsId)
{
    UCHAR ucMpvId;
    UINT16 u2MsgQIdx;
    UINT32 u4Cnt;
    SIZE_T zMsgSize;
    UINT16 u2Count;

    VDEC_PES_INFO_T rPesInfo;

    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    
    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = &_rVdecInfo.arVdecEsInfo[ucEsId];
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);

    u4Cnt = 0;
    
    zMsgSize = sizeof(VDEC_PES_INFO_T);
    while (x_msg_q_receive(&u2MsgQIdx, &rPesInfo, &zMsgSize,
        &(prVdecEsInfoKeep->hMsgQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
        ucMpvId = rPesInfo.ucMpvId;

        // Handle Virtual Picture Header for Event Trigger
        if (ucMpvId == VDEC_MAX_VLD)
        {   
            u4Cnt++;
        }
    }

    // DTV00136850 Video display discontinuously after  plug supported CAM module at TV in Menu
    // video frame are not correct release.
    if(prVdecEsInfo->fgMMPlayback)
    {
        if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
        {
            //flush dpb
            #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
            #ifndef CC_VDEC_H264_DISABLE
            _VDEC_H264Flush(ucEsId, FALSE);
            #endif
            #endif
        }
        else
        {
            FBM_PIC_HDR_T *prFbmPicHdr;
            UCHAR ucFRefFbId, ucBRefFbId;
            FBM_GetRefFrameBuffer(prVdecEsInfo->ucFbgId, &ucFRefFbId, &ucBRefFbId);
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
            if(prFbmPicHdr)
            {
                if((FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_READY))
                   ||
                   (FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_DECODE)))
                {
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_EMPTY);
                    LOG(3, "Flush ucBRefFbId %d -> empty\n", ucBRefFbId);
                }
                if((FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_READY))
                   ||
                   (FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_DECODE)))
                {
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_EMPTY);
                    LOG(3, "Flush ucFRefFbId %d -> empty\n", ucFRefFbId);
                }
            }
            
            // because we invalidate reference frame, we should restart to get a sync.
            prVdecEsInfo->fgDetectIntra = TRUE;
            if(prVdecEsInfo->prMpvCounter)
            {
                prVdecEsInfo->prMpvCounter->u4RetrieveNs = 0;
            }
            
            SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        }
    }

    prVdecEsInfo->fgKeepPes = FALSE;
    prVdecEsInfo->fgSendedEos = FALSE;
    prVdecEsInfo->fgSetStartPts = FALSE;
    prVdecEsInfo->u4PTSLast = 0;
    prVdecEsInfo->u8OffsetLast = 0;
    prVdecEsInfo->u4DisplayQPicCnt = 0;
    prVdecEsInfo->u4FirstDispPTS = 0;
    prVdecInfo->rPesInfo.fgEos = FALSE;

#ifdef __MODEL_slt__
    if(prVdecInfo->eCurFMT == VDEC_FMT_MPV)
    {
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    }
#endif

    VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
    ASSERT(prVdecEsInfo->u2Count == 0);
    LOG(5, "Es%d Flush\n",ucEsId);
    SYSLOG(50, ucEsId, 0, 0);

    VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hCmdQ, &(u2Count)) == OSR_OK);
    if(u4Cnt > u2Count)
    {
        u4Cnt = u2Count;
    }

    // rPesInfo should reset or we will have the last one's information
    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    while (u4Cnt)
    {
        rPesInfo.ucEsId = ucEsId;
        rPesInfo.ucMpvId = VDEC_MAX_VLD;
        VERIFY(VDEC_SendEs(&rPesInfo) == 1);
        u4Cnt--;
    }
}


#ifdef MPV_VIRTUAL_PRS

static void _MpvVirutualPrsRoutine(void* pvArg)
{
    VDEC_PES_INFO_T rPesInfo;
    BOOL fgStop[VDEC_MAX_ES] = {FALSE};
    UINT32 u4OnRepeatCount = 0;
    UINT32 u4RoundCount = 0;
    //HANDLE_T hThread;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_INFO_T *prVdecInfo;

    prVdecInfo = _VDEC_GetInfo();

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    
    while (!_fgMpvVirtualPrsDestroy)
    {
        UCHAR ucEsIdx;

        for (ucEsIdx = 0; ucEsIdx < _ucMpvVPEsNs; ucEsIdx++)
        {
            prVdecEsInfo = &_rVdecInfo.arVdecEsInfo[ucEsIdx];
            if (!fgStop[ucEsIdx])
            {
                if (prVdecEsInfo->u2Count > (MPV_MAX_PES_NS_PER_QUEUE / 2))
                {
                    x_thread_delay(_ucMpvVirtualPrsDelay);
                }
                else
                {
                    rPesInfo.fgNonBuffer = TRUE;
                    rPesInfo.u4FifoStart = _u4MpvVpStartAddr[ucEsIdx];
                    rPesInfo.u4FifoEnd = ~0;
                    rPesInfo.u4VldReadPtr =
                        _u4MpvVpAddr[ucEsIdx][_u4MpvVpPicCount[ucEsIdx]];
                    rPesInfo.ucEsId = _ucMpvVpEsId[ucEsIdx];
                    rPesInfo.ucMpvId = _ucMpvVpMpvId[ucEsIdx];
                    rPesInfo.ucPicType =
                        _u4MpvVpPicTp[ucEsIdx][_u4MpvVpPicCount[ucEsIdx]];

                    VDEC_SendEs(&rPesInfo);

                    #ifdef MPV_SEMI_HOST
                    {
                        if (prVdecInfo->fgSave)
                        {
                            if ((_u4MpvVpPicCount[ucEsIdx] + 1) == _u4MpvVpPicNs[ucEsIdx])
                            {
                                #if 0
                                fgStop[ucEsIdx] = TRUE;
                                _fgMpvVirtualPrsDestroy = TRUE;
                                #else
                                while(1)
                                {
                                    x_thread_delay(10000);
                                    LOG(1, "Wait for decode finish\n");
                                }
                                break;
                                ASSERT(0);
                                #endif
                            }
                        }
                    }
                    #endif

                    if (!fgStop[ucEsIdx])
                    {
                        _u4MpvVpPicCount[ucEsIdx] = (_u4MpvVpPicCount[ucEsIdx] + 1) % _u4MpvVpPicNs[ucEsIdx];
                        if(_u4MpvVpPicCount[ucEsIdx] == 0)
                        {
                            LOG(1, "Parse Round %d\n", u4RoundCount++);
                        }
                        
                        #ifdef MPV_REPEAT_TEST
                        if(_u4MpvVpPicCount[ucEsIdx] > MPV_PARSE_REPEAT_END)
                        {
                            _u4MpvVpPicCount[ucEsIdx] = MPV_PARSE_REPEAT_START;
                            prVdecEsInfo->fgParseOnRepeat = 1;
                        }
                        if(prVdecEsInfo->fgParseOnRepeat)
                        {
                            LOG(1, "Parse On Repeat %d\n", u4OnRepeatCount++);
                        }
                        #endif
                    }
                }
            }
        }
    }

    _fgMpvVirtualPrsStart = FALSE;
    LOG(1, "VirtualPrs Exit\n");

    // destroy thread
    x_thread_exit();

}

#endif


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

UCHAR VDEC_SendEs(const VDEC_PES_INFO_T*prPesInfo)
{
    UCHAR ucValid;
    UCHAR ucEsId;
    UCHAR ucMpvId;
    INT32 i4Ret, u4OverflwThrd;
    //ESMQ_T* prEsmQ;
    //MPV_ES_INFO_T* prEsInfo;   
    VDEC_INFO_T* prVdecInfo;
    VDEC_ES_INFO_T* prVdecEsInfo;
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_STATUS_CB_T rStatusCB;

    ASSERT(prPesInfo != NULL);
    ucMpvId = prPesInfo->ucMpvId;
    ucEsId = prPesInfo->ucEsId;

    ASSERT(ucEsId < VDEC_MAX_ES);

    
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecInfo = &_rVdecInfo;

    // Virtual Picture Header for Event Trigger
    if (ucMpvId == VDEC_MAX_VLD)
    {
        VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hMsgQ, &(prVdecEsInfo->u2Count))
            == OSR_OK);

        //if (prEsmQ->u2Count <= prEsmQ->u2UdfNs)
        {
            i4Ret = x_msg_q_send(prVdecEsInfoKeep->hMsgQ, (void *)prPesInfo,
                sizeof(VDEC_PES_INFO_T), 254);
                
            if (i4Ret != OSR_OK)
            {
                if (i4Ret == OSR_TOO_MANY)
                {
                    LOG(3, "EsQ full\n");
                }
                else
                {
                    ASSERT(0);
                }
            }

            VERIFY(x_sema_unlock(prVdecEsInfoKeep->hUnderFlowSema) == OSR_OK);                                                                               
        }
                
        return 1;
    }

    ASSERT(ucMpvId < VDEC_MAX_VLD);

    // [LOG] Parser send message into ESM Q
    SYSLOG(ucEsId, 1, prVdecEsInfo->u2Count, prPesInfo->u4PTS);
    //SYSLOG(ucEsmQId, 1, prPesInfo->u4PTS, prPesInfo->u4DTS);

#ifdef MPV_VIRTUAL_PRS
    VERIFY(x_msg_q_send(prVdecEsInfoKeep->hMsgQ, (void *)prPesInfo,
        sizeof(VDEC_PES_INFO_T), 255) == OSR_OK);    
    VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
#else
    if(prVdecInfo->pfnVdecWaitRandomAP)
    {
        if(prVdecInfo->pfnVdecWaitRandomAP((void *)prPesInfo))
        {
            return 1;   //waiting first random access point
        }
    }
    
    if(prVdecEsInfo->fgMMPlayback && prVdecEsInfo->ucSequenceHdrFound)
    {
        // MM case, not allow drop
        INT32 i4Count = 0;
        while(1)
        {
            VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
            if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
            {
                u4OverflwThrd = H264_MAX_PES_NS_PER_QUEUE-1;
            }
            else
            {
                u4OverflwThrd = MPV_MAX_PES_NS_PER_QUEUE-1;
            }
            if(prVdecEsInfo->u2Count >= (UINT16)u4OverflwThrd)
            {
                LOG(0,"VDEC Queue Size if full!!!!!!!!!\n");
        	    //x_thread_delay(33);
        	    i4Count++;
        	    if(i4Count > 30)
    	        {
    	            UCHAR ucRet = 0;
                    LOG(7, "MM x_thread_delay(33)\n");
                    
                    if(prVdecInfo->pfnVdecHandleOverFlw)
                    {
                        LOG(5, "fgMMPlayback MM pfnVdecHandleOverFlw\n");
                        ucRet =  prVdecInfo->pfnVdecHandleOverFlw(ucMpvId, ucEsId, 
                            prVdecEsInfoKeep->hMutex, prVdecEsInfoKeep->hMsgQ, (void*)prPesInfo);
                    }
                    return ucRet;
    	        }
            }
            else
            {
                break;
            }
        }
    }

    if(prPesInfo->fgNonBuffer)
    {
        if (prVdecEsInfo->fgKeepPes)
        {
            LOG(1, "fgNonBuffer and fgKeepPes, should not happen\n");
        }
        // direct send
        i4Ret = x_msg_q_send(prVdecEsInfoKeep->hMsgQ, (void *)prPesInfo,
            sizeof(VDEC_PES_INFO_T), 255);
        if (i4Ret != OSR_OK)
        {
            UCHAR ucRet = 0;

            if(prVdecInfo->pfnVdecHandleOverFlw)
            {
                ucRet =  prVdecInfo->pfnVdecHandleOverFlw(ucMpvId, ucEsId, 
                    prVdecEsInfoKeep->hMutex, prVdecEsInfoKeep->hMsgQ, (void*)&_rPesInfoEsm);
                x_memcpy(&_rPesInfoEsm, prPesInfo, sizeof(VDEC_PES_INFO_T));
            }
            return ucRet;
        }
    }
    else if((!prVdecEsInfo->fgKeepPes) && (prVdecInfo->eCurState != VDEC_ST_PLAY_I_FRAME))
    {
        if (prVdecEsInfo->fgMMPlayback && prPesInfo->fgEos)
        {
            // we don't get any frame before Eos
            x_memcpy((void *)&_rPesInfoEsm, (void *)prPesInfo, sizeof(VDEC_PES_INFO_T));
            _rPesInfoEsm.ucMpvId = VDEC_MAX_VLD;
            i4Ret = x_msg_q_send(prVdecEsInfoKeep->hMsgQ, (void *)&_rPesInfoEsm,
                sizeof(VDEC_PES_INFO_T), 255);
        }
        else
        {
            x_memcpy(&_rPesInfoEsm, prPesInfo, sizeof(VDEC_PES_INFO_T));
            prVdecEsInfo->fgKeepPes = TRUE;
        }
    }
    else
    {
        i4Ret = OSR_OK;
        if(prVdecInfo->eCurState != VDEC_ST_PLAY_I_FRAME)
        {
            if (prVdecEsInfo->fgMMPlayback && _rPesInfoEsm.fgSeqEnd)
            {
                if(prPesInfo->fgEos)
                {
                    // we don't get any frame before Eos
                    x_memcpy((void *)&_rPesInfoEsm, (void *)prPesInfo, sizeof(VDEC_PES_INFO_T));
                    _rPesInfoEsm.ucMpvId = VDEC_MAX_VLD;
                    i4Ret = x_msg_q_send(prVdecEsInfoKeep->hMsgQ, (void *)&_rPesInfoEsm,
                        sizeof(VDEC_PES_INFO_T), 255);
                }
                else
                {
                    x_memcpy(&_rPesInfoEsm, prPesInfo, sizeof(VDEC_PES_INFO_T));
                    prVdecEsInfo->fgKeepPes = TRUE;
                }
            }
            else
            {            
                if(prPesInfo->fgEos)
                {
                    LOG(5, "EOS in PesInfo\n");
                    _rPesInfoEsm.fgEos = TRUE;
                }
				
                //DTV00139473
               if(prPesInfo->fgSeqEnd)
               {
                   LOG(5, "fgSeqEnd in PesInfo\n");                    
		           _rPesInfoEsm.fgSeqEnd = TRUE;
               }      
			   
                if(prPesInfo->u8Offset > _rPesInfoEsm.u8Offset)
                {
                    if((prPesInfo->u8Offset - _rPesInfoEsm.u8Offset) <
                       (1024*1024))
                    {
                        prVdecEsInfo->u4AvgPicSize = 
                            ((prVdecEsInfo->u4AvgPicSize*
                            (VDEC_AVG_PIC_COUNT-1))/VDEC_AVG_PIC_COUNT);
                        prVdecEsInfo->u4AvgPicSize +=
                            ((UINT32)(prPesInfo->u8Offset - _rPesInfoEsm.u8Offset)/
                            VDEC_AVG_PIC_COUNT);
                    }
                }
                
                //LOG(1, "_rPesInfoEsm.u4VldReadPtr 0x%x\n", _rPesInfoEsm.u4VldReadPtr);
                i4Ret = x_msg_q_send(prVdecEsInfoKeep->hMsgQ, (void *)&_rPesInfoEsm,
                    sizeof(VDEC_PES_INFO_T), 255);
                if(i4Ret == OSR_OK)
                {
                    x_memcpy(&_rPesInfoEsm, prPesInfo, sizeof(VDEC_PES_INFO_T));
                }
                // When we have a sequence end flag, this pes info
                // does not contain video data. We should skip it.
                if (prVdecEsInfo->fgMMPlayback && prPesInfo->fgSeqEnd)
                {
                    LOG(3, "fgSeqEnd in PesInfo\n");
                    prVdecEsInfo->fgKeepPes = FALSE;
                }
            }
        }
        else
        {
            i4Ret = x_msg_q_send(prVdecEsInfoKeep->hMsgQ, (void *)prPesInfo,
                sizeof(VDEC_PES_INFO_T), 255);
        }
        if (i4Ret != OSR_OK)
        {
            UCHAR ucRet = 0;

            if(prVdecInfo->pfnVdecHandleOverFlw)
            {
                ucRet =  prVdecInfo->pfnVdecHandleOverFlw(ucMpvId, ucEsId, 
                    prVdecEsInfoKeep->hMutex, prVdecEsInfoKeep->hMsgQ, (void*)&_rPesInfoEsm);
                x_memcpy(&_rPesInfoEsm, prPesInfo, sizeof(VDEC_PES_INFO_T));
            }
            return ucRet;
        }
    }
    VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
    if((UINT32)prVdecEsInfo->u2Count > prVdecEsInfo->u4MaxEsCnt)
    {
        prVdecEsInfo->u4MaxEsCnt = (UINT32)prVdecEsInfo->u2Count;
    }
#endif

#if 0
    {
        STC_CLOCK rStcClk;
        
        STC_GetSrc(0, &rStcClk);
        LOG(3, "Time(0x%x) DTS(0x%x)\n", rStcClk.u4Base, prPesInfo->u4DTS);
    }
#endif


    if (prVdecEsInfo->ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdecEsInfo->ucFbgId);
        if (prFbmSeqHdr != NULL)
        {
            prFbmSeqHdr->u4LastPTS = prPesInfo->u4PTS;
        }
    }
    
    ucValid = 1;
    if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
    {
        u4OverflwThrd = H264_MAX_PES_NS_PER_QUEUE;
    }
    else
    {
        u4OverflwThrd = MPV_MAX_PES_NS_PER_QUEUE;
    }
    
    if (prVdecEsInfo->u2Count >= (UINT16)u4OverflwThrd)
    {
        LOG(1, "EsFifoCnt OverFlw\n");
        if(prVdecInfo->pfnStatusCb)
        {
            rStatusCB.u1EsId = ucEsId;
            rStatusCB.u4Status = (UINT32)VDEC_BUF_OVERFLOW;
            prVdecInfo->pfnStatusCb(&rStatusCB);
        }
        prVdecEsInfo->ucDropBNs++;
        ucValid = 0;
    }

    if (!prVdecEsInfo->ucStarted)
    {
        if ((prVdecEsInfo->u2Count >= prVdecEsInfo->u2UdfNs) || 
            (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME) ||
            (prPesInfo->fgNonBuffer) ||
            _rPesInfoEsm.fgEos)
        {
            prVdecEsInfo->ucStarted = 1;

            VERIFY(x_sema_unlock(prVdecEsInfoKeep->hUnderFlowSema) == OSR_OK);
        }
    }

    return ucValid;
}


#ifdef MPV_VIRTUAL_PRS
void MPV_VirtualPrsParse(UCHAR ucMvpId, UINT32 u4Addr, UINT32 u4PicNs, UINT32 u4FrmNs)
{
    ASSERT(ucMvpId < VDEC_MAX_VLD);

    _rVdecInfo.arVdecEsInfo[_ucMpvVPEsNs].u4CompareNs = u4FrmNs;

    if (_ucMpvVPEsNs < VDEC_MAX_ES)
    {
        UCHAR ucPicType;
        UCHAR *pucPicTp;
        UINT32 u4Temp;
        UINT32* pu4Hdr;
        UINT32 u4StartCode;
        UINT32 u4Idx;

        _ucMpvVpEsId[_ucMpvVPEsNs] = _ucMpvVPEsNs;
        _u4MpvVpStartAddr[_ucMpvVPEsNs] = u4Addr;
        _ucMpvVpMpvId[_ucMpvVPEsNs] = ucMvpId;
        _u4MpvVpPicNs[_ucMpvVPEsNs] = u4PicNs;

        pu4Hdr = (UINT32*) x_mem_alloc(u4PicNs * sizeof(UINT32));
        ASSERT(pu4Hdr != NULL);
        _u4MpvVpAddr[_ucMpvVPEsNs] = pu4Hdr;

        pucPicTp = (UCHAR*) x_mem_alloc(u4PicNs * sizeof(UINT32));
        ASSERT(pucPicTp != NULL);
        _u4MpvVpPicTp[_ucMpvVPEsNs] = pucPicTp;

        _MPV_DecReset(ucMvpId, _ucMpvVPEsNs);
        _MPV_VldSetVFifo(ucMvpId, 0, ~0);
        _MPV_VldInitBarrelShifter(ucMvpId, u4Addr);

        for (u4Idx = 0; u4Idx < u4PicNs; u4Idx++)
        {
            u4StartCode = _MPV_VParserGetHdr(ucMvpId, _ucMpvVPEsNs, &u4Temp, &ucPicType);
            pu4Hdr[u4Idx] = u4Temp;
            pucPicTp[u4Idx] = ucPicType;
            LOG(5, "Pic %d, Type %d, Addr 0x%x\n", u4Idx, ucPicType, u4Temp);

            while (u4StartCode != 0x100)        // Picture Header Start Code
            {
                u4StartCode = _MPV_VParserGetHdr(ucMvpId, _ucMpvVPEsNs, &u4Temp, &ucPicType);
            }
        }

        _ucMpvVPEsNs++;
    }
}


void MPV_VirtualPrsStart(UINT32 u4Delay)
{
    if (_fgMpvVirtualPrsStart == 0)
    {
        HANDLE_T hThread1;

        _fgMpvVirtualPrsDestroy = FALSE;
        _fgMpvVirtualPrsStart = TRUE;

        _ucMpvVirtualPrsDelay = u4Delay;

        // create thread
        VERIFY(x_thread_create(&hThread1, "VPrs Thread", MPV_VPRS_STACK_SIZE,
            MPV_VPRS_THREAD_PRIORITY,
        _MpvVirutualPrsRoutine, 0, NULL) == OSR_OK);
    }
}


void MPV_VirtualPrsStop(void)
{
    if (_fgMpvVirtualPrsStart)
    {
        _fgMpvVirtualPrsDestroy = TRUE;
    }
}
#endif



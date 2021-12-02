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
 * $Author: jifeng.fan $
 * $Date: 2013/01/10 $
 * $RCSfile: vdec_if.c,v $
 * $Revision: #14 $
 *---------------------------------------------------------------------------*/

/** @file vdec_if.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "vdec_debug.h"
#include "vdec_if.h"
#include "vdec_drvif.h"
#include "vdec_flw.h"
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
#ifndef CC_VDEC_VP6_DISABLE
#include "vdec_vp6api.h"
#endif
#ifndef CC_VDEC_VP8_DISABLE
#include "vdec_vp8api.h"
#endif

#endif

#ifndef CC_VDEC_RV_DISABLE
#include "vdec_avsapi.h"
#endif

#ifdef ENABLE_MULTIMEDIA
#include "swdmx_if.h"
#endif

#include "vdp_if.h"
#include "drv_dbase.h"
#include "fbm_drvif.h"
#include "imgrz_if.h"
#include "drvcust_if.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_hal_5381.h"
#include "x_chip_id.h"
#include "x_mm_common.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#ifndef REPEAT_FRM
#define REPEAT_FRM 5
#endif
#define DROP_FRM (6)
#define CODEC_CHK_DATASZ (0x3000 * ((UINT32)VDEC_CODEC_CHK_TIMER_THRSD/1000))    // 12kbps
#define VDEC_SCHEDULE_TIMEOUT 5000
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgVDecInitiated = FALSE;
static VDEC_PES_INFO_T _arPesInfo[VDEC_MAX_ES]; // this is for cli
static BOOL _fgIsSupportMPEG4AndH264 = TRUE;

#ifdef VDEC_CRC_TEST
static BOOL _fgCRCTest = FALSE;
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------
void _VDEC_LockLoadCyclesMutex(UCHAR ucEsId)
{

    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
	prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

	VERIFY (x_sema_lock(prVdecEsInfoKeep->hVdecSetPriSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}

void _VDEC_UnlockLoadCyclesMutex(UCHAR ucEsId)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
	prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    VERIFY (x_sema_unlock(prVdecEsInfoKeep->hVdecSetPriSema) == OSR_OK);
}

#if !defined(CC_EMULATION) && !defined(CC_USE_DDI)
static VOID _DataTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UCHAR ucEsId;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    PARAM_MPV_T *prMpvCounter;

    ucEsId = (UCHAR)(UINT32)pv_tag;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prMpvCounter = prVdecEsInfo->prMpvCounter;

    if (ucEsId >= VDEC_MAX_ES)
    {
        return;
    }

    if((prVdecEsInfo->u4NsAtTimeout == prMpvCounter->u4RetrieveNs) &&
        (prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_NO_DATA))
    {
        //callback, no data receive
        if ((prVdecEsInfoKeep->pfDecNfyFct) &&
            (!prVdecEsInfo->fgMMPlayback))
        {
            _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_FLUSH);

            _arPesInfo[ucEsId].ucEsId = ucEsId;
            _arPesInfo[ucEsId].ucMpvId = (UCHAR)VDEC_MAX_VLD;
            VERIFY(VDEC_SendEs(&_arPesInfo[ucEsId]) == 1);

            LOG(3, "ES(%d) Notify Status change, no data\n", ucEsId);
            prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_NO_DATA, 0);

            prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_NO_DATA;
        }
    }

    prVdecEsInfo->u4NsAtTimeout = prMpvCounter->u4RetrieveNs;

    UNUSED(pt_tm_handle);
}
#endif

#if 0
static VOID _ThumbTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UCHAR ucEsId;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    PARAM_MPV_T *prMpvCounter;

    ucEsId = (UCHAR)(UINT32)pv_tag;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prMpvCounter = prVdecEsInfo->prMpvCounter;

    if (ucEsId >= VDEC_MAX_ES)
    {
        return;
    }

    if((prVdecEsInfo->u4NsAtThumbTimeout == prMpvCounter->u4RetrieveNs) &&
       (prVdecEsInfo->fgThumbMethod2))
    {
        //callback, no data receive
        if (prVdecEsInfoKeep->pfDecNfyFct)
        {
            LOG(3, "Notify thumbnail timeout\n");
            prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                VDEC_COND_THUMBNAIL_FAIL,  0, 0); 
        }
        prVdecEsInfo->fgThumbMethod2 = FALSE;
    }

    prVdecEsInfo->u4NsAtThumbTimeout = prMpvCounter->u4RetrieveNs;

    UNUSED(pt_tm_handle);
}
#endif

#if 0
static VOID _CodecChkTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UCHAR ucEsId;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    PARAM_MPV_T *prMpvCounter;
    BOOL fgErrNotify = TRUE;

    ucEsId = (UCHAR)(UINT32)pv_tag;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prMpvCounter = prVdecEsInfo->prMpvCounter;

    if (ucEsId >= VDEC_MAX_ES)
    {
        return;
    }

    if((prMpvCounter->u4RetrieveNs > 0) && (prMpvCounter->u4DecOkNs == 0))
    {
        LOG(1, "RecvPesInfoCnt %d but can't dec any frm\n", prMpvCounter->u4RetrieveNs);
    }
    else if((prMpvCounter->u4RetrieveNs == 0) && (prVdecEsInfo->u4VFifoDataSz > CODEC_CHK_DATASZ))
    {
        LOG(1, "RecvDataSz 0x%x but can't find start code\n", prVdecEsInfo->u4VFifoDataSz);
    }
    else
    {
        fgErrNotify = FALSE;
        LOG(1, "RecvDataSz 0x%x, PesInfoCnt %d\n",
            prVdecEsInfo->u4VFifoDataSz, prMpvCounter->u4RetrieveNs);
    }

    if(fgErrNotify)
    {
        //callback, no data receive
        if (prVdecEsInfoKeep->pfDecNfyFct)
        {
            LOG(3, "ES(%d) Notify Status change, data error\n", ucEsId);
            prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_ERROR, 0);
        }
    }
    prVdecEsInfoKeep->fgCodecTypeChecking = FALSE;
    VERIFY(x_timer_stop(prVdecEsInfoKeep->hCodecChkTimer) == OSR_OK);

    UNUSED(pt_tm_handle);
}
#endif
static VDEC_ASPECT_RATIO_T _AVCDecideAspectRatio(UCHAR ucAspectRatio, UINT32 u4Width, UINT32 u4Height)
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
    UNUSED(u4Width);
    UNUSED(u4Height);
    return VDEC_ASPECT_RATIO_4_3;
}

static void _VDPReapeatDropCB(UINT32 u4VdpId, UINT32 u4RepeatDrop)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_STATUS_CB_T rStatusCB;

    // TODO
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ES0);

    if(prVdecInfo->pfnStatusCb)
    {
        rStatusCB.u1EsId = prVdecEsInfoKeep->rPesInfo.ucEsId;
        if(u4RepeatDrop == (UINT32)REPEAT_FRM)
        {
            rStatusCB.u4Status = (UINT32)VDEC_FRM_REPEAT;
        }
        else if(u4RepeatDrop == (UINT32)DROP_FRM)
        {
            rStatusCB.u4Status = (UINT32)VDEC_FRM_DROP;
        }
        else
        {
            return;
        }
        prVdecInfo->pfnStatusCb(&rStatusCB);
    }
    UNUSED(u4VdpId);
}


static VDEC_ASPECT_RATIO_T _DecideAspectRatio(UCHAR ucAspectRatio, UINT32 u4Width, UINT32 u4Height)
{
    #ifndef VDEC_NEW_ASPECT_RATIO
    if (ucAspectRatio != MPEG_ASPECT_RATIO_1_1)
    #endif
    {
        switch (ucAspectRatio)
        {
        #ifdef VDEC_NEW_ASPECT_RATIO
        case MPEG_ASPECT_RATIO_1_1:
            return VDEC_ASPECT_RATIO_1_1;
        #endif

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

        default:
            return VDEC_ASPECT_RATIO_221_1;
        }
    }
    #ifndef VDEC_NEW_ASPECT_RATIO
    else
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
    #endif
}


static void _VDEC_SetClock(void)
{
        //vdec clock src selection
        #if defined(CC_MT5360) || defined(CC_MT5363)
        UINT32 u4Clk_MC, u4Clk_SYS;

        u4Clk_MC = IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_MC) & 0xFFFFFFF0;
        u4Clk_SYS = IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_SYS) & 0xFFFFFFF0;

        #if defined(CC_MT5360)
        if (BSP_GetIcVersion() < IC_VER_5360_AD)
        {
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, (u4Clk_MC | VDEC_MC_MC_CLK));
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, (u4Clk_SYS | VDEC_MC_SYS_CLK));
        }
        else
        #endif
        {
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, (u4Clk_MC | VDEC_MC_MC_CLK_ECO_B));
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, (u4Clk_SYS | VDEC_MC_SYS_CLK_ECO_B));
        }
            #ifdef CC_DYNAMIC_POWER_ONOFF   //power off
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_MC) | VDEC_PWR_OFF);
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_SYS) | VDEC_PWR_OFF);
            #endif
        #elif defined(CC_MT5387)
        IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, VDEC_MC_SYS_CLK);
            #ifdef CC_DYNAMIC_POWER_ONOFF  //power off
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) | VDEC_PWR_OFF);
            #endif
        #elif defined(CC_MT5365)
        IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, VDEC_MC_MC_CLK);
        IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, VDEC_MC_SYS_CLK);
            #ifdef CC_DYNAMIC_POWER_ONOFF  //power off
            IO_WRITE32(VDEC_CTRL_BASE, VDEC_PWR_CTRL, VDEC_PWR_OFF);
            #endif
        #elif defined(CC_MT5395)
        IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, VDEC_MC_MC_CLK);
        IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, VDEC_MC_SYS_CLK);
            #ifdef CC_DYNAMIC_POWER_ONOFF  //power off
            IO_WRITE32(VDEC_CTRL_BASE, VDEC_PWR_CTRL, VDEC_PWR_OFF);
            #endif
        #elif defined(CC_MT5396)
        // FIXME
        #elif defined(CC_MT5368)
        // FIXME
        #elif defined(CC_MT5389)
        // FIXME
        #else //if defined(CC_MT5391)
        IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, VDEC_MC_SYS_CLK);
            #ifdef CC_DYNAMIC_POWER_ONOFF  //power off
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) | VDEC_PWR_OFF);
            #endif
        #endif
}

void VDEC_EableCrop(UCHAR ucEsId,BOOL fgEnable)
{
    if(ucEsId >= VDEC_MAX_ES)
    {
        return;
    }
    VDEC_ES_INFO_T *prVdecEsInfo;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgEnableCrop = fgEnable;
    LOG(1, "prVdecEsInfo->fgEnableCrop = %d!!\n",prVdecEsInfo->fgEnableCrop);
    return;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

VOID VDEC_GetQueueInfo(UCHAR ucEsId, UINT16* pu2QueueSize,UINT16* pu2MaxQueueSize)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_INFO_T *prVdecInfo;

    if (!_fgVDecInitiated)
    {
        return;
    }

    if((!pu2MaxQueueSize) || (!pu2QueueSize))
    {
        LOG(3, "VDEC_GetQueueInfo Invalid Parameter \n");
        return;
    }

    *pu2QueueSize = 0;
    *pu2MaxQueueSize = 0;

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecInfo = _VDEC_GetInfo();
    if(prVdecEsInfoKeep && prVdecInfo)
    {
        *pu2MaxQueueSize = VDEC_MSG_Q_SIZE;
        if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
        {
            *pu2MaxQueueSize = H264_MAX_PES_NS_PER_QUEUE;
        }
        else
        {
            *pu2MaxQueueSize = MPV_MAX_PES_NS_PER_QUEUE;
        }

        VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hMsgQ, pu2QueueSize) == OSR_OK);
    }
}


#ifdef CC_FAST_INIT
void VDEC_pm_suspend(void)
{
}


void VDEC_pm_resume(void)
{
    _VDEC_SetClock();
}
#endif


//Synchronous command
void VDEC_Init(void)
{
    HANDLE_T hThread;
    VDEC_INFO_T *prVdecInfo;
    UCHAR ucMpvId, ucEsId;
    CHAR szBuf[16];
    DRV_DBASE_T* _prDbase;
    CRIT_STATE_T _rCritState;

    prVdecInfo = _VDEC_GetInfo();
    _prDbase = NULL;

    if (!_fgVDecInitiated)
    {
        _fgVDecInitiated = TRUE;

        _VDEC_SetClock();
        
        x_memset((void *)prVdecInfo, 0, sizeof(VDEC_INFO_T));    //init state = VDEC_ST_STOP

        _VDEC_InitInfo();

        prVdecInfo->fgVDecInitiated = TRUE;

        // create thread semephore
        for (ucMpvId = 0; ucMpvId < VDEC_MAX_VLD; ucMpvId++)
        {
            VERIFY (x_sema_create(&prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore, X_SEMA_TYPE_MUTEX,
                X_SEMA_STATE_UNLOCK) == OSR_OK);
            prVdecInfo->arVdecVldInfo[ucMpvId].u4PowerOnRef = 0;
        }

        for (ucEsId = 0; ucEsId < VDEC_MAX_ES; ucEsId++)
        {
            x_snprintf(szBuf, sizeof(szBuf), "VDEC%3d", ucEsId);

            prVdecInfo->arVdecEsInfoKeep[ucEsId].fgWaitIFrame = TRUE;

            prVdecInfo->arVdecEsInfo[ucEsId].eNotifyStatus
                = VDEC_DEC_DECODE_UNKNOWN;
            prVdecInfo->arVdecEsInfo[ucEsId].fgMMSeqInfoValid = FALSE;
            _prDbase = DBS_Lock(&_rCritState);
            ASSERT(_prDbase);
            VERIFY(DBS_Unlock(_prDbase, _rCritState));
            prVdecInfo->arVdecEsInfo[ucEsId].prMpvCounter = &(_prDbase->rMpv[ucEsId]);

            // create thread
            _VDEC_CreateEsmSema(ucEsId);
            VERIFY(x_thread_create(&hThread, szBuf, VDEC_STACK_SIZE,
                VDEC_THREAD_PRIORITY,  _VDEC_MainLoop, sizeof(ucEsId), (void*)&ucEsId) == OSR_OK);
        }

        // Query Op code
        #ifndef CC_VDEC_MPEG2_DISABLE
        if(!DRVCUST_OpCodeQuery(eIsSupportMP2))
        #endif
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_MPEG2;
        }
        #ifndef CC_VDEC_VC1_DISABLE
        if(!DRVCUST_OpCodeQuery(eIsSupportVC1))
        #endif
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_VC1;
        }

        #ifdef CC_VDEC_MPEG4_DISABLE
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_MPEG4;
        }
        #endif
        #ifdef CC_VDEC_H264_DISABLE
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_H264;
        }
        #endif
        #ifdef CC_VDEC_MJPEG_DISABLE
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_MJPEG;
        }
        #endif
        #ifdef CC_VDEC_RV_DISABLE
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_RV;
        }
        #endif
        #ifdef CC_VDEC_VP6_DISABLE
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_VP6;
        }
        #endif
        #ifdef CC_VDEC_VP8_DISABLE
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_VP8;
        }
        #endif

        /*
        if(!DRVCUST_OpCodeQuery(eIsSupportMPEG4))
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_MPEG4;
        }
        if(!DRVCUST_OpCodeQuery(eIsSupportH264))
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_H264;
        }*/

#ifdef TEST_ONOFF_RUNTIME
        _VDEC_LockVld(ES0, VLD0);
#endif
        MPV_Init();
        _VDEC_IsrInit();
#ifdef TEST_ONOFF_RUNTIME
        _VDEC_UnlockVld(ES0, VLD0);
#endif

#ifdef CC_REALD_3D_SUPPORT
        fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_MM, _VDEC_TDTVModeChangeCB);
#endif
    }
}

//Synchronous command
void VDEC_Termint(void)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    if (prVdecInfo->fgVDecInitiated)
    {
        UCHAR ucEsId;

        for (ucEsId = 0; ucEsId < VDEC_MAX_ES; ucEsId++)
        {
            prVdecInfo->arVdecEsInfoKeep[ucEsId].fgThreadDestroy = TRUE;
        }
    }
}

//Synchronous command
void VDEC_Reset(void)
{
    _VDEC_InitInfo();
}

//Asynchronous command
BOOL VDEC_Play(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt)
{
	UINT32 u4Tmp;
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    UCHAR ucFRefFbId, ucBRefFbId;
    // should assign later
    //prVdecEsInfoKeep->eCurFMT = eFmt;

    ASSERT(ucEsId < VDEC_MAX_ES);

    //power on VDEC module
    //move to main loop, need sync with VDEC thread

    //prVdecEsInfo->fgPlayOneFrm = FALSE;
    /*
    if(FBM_ChkFrameBufferFlagIsr(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE))
    {
        FBM_ReleaseDispQ(prVdecEsInfo->ucFbgId);
        FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    }
    */

    if((eFmt != prVdecEsInfoKeep->eCurFMT) && (prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY))
    {
        return FALSE;
    }

    if((prVdecEsInfo->fgMMPlayback) &&
       (prVdecEsInfo->fgSendedEos) &&
       (prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY))
    {
        return FALSE;
    }

    prVdecEsInfoKeep->eCurFMT = eFmt;
	if(!prVdecEsInfo->fgMMPlayback)
	{
		#if (defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)) && !defined(CC_VDEC_H264_DISABLE)
		if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264) // format will be mpv by default before _VDEC_DecCfg
		{
			VERIFY(STC_SetPtsDrift(prVdecEsInfo->ucStcId, H264_FIFO_CTRL_INIT) == STC_VALID);
		}
		else
		#endif
		{
			VERIFY(STC_SetPtsDrift(prVdecEsInfo->ucStcId, MPV_FIFO_CTRL_INIT) == STC_VALID);
		}
	}

    if(eFmt == VDEC_FMT_MJPEG)
    {
        prVdecEsInfo->u4VldId = VLD1;
    }
    else
    {
        prVdecEsInfo->u4VldId = VLD0;
    }
    #ifdef CC_VDEC_PRIORITY_ENABLE
    if((prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId]== VDEC_VLD_NOUSE)||
		(prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId]== VDEC_VLD_PAUSE)
	  )
    {
	   prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId] = VDEC_VLD_ACTIVE;
	   VDEC_SetPriority(ucEsId,VDEC_PRIORITY_DEFAULT);

	   //LOG(3,"VDEC_Play set Es(%d) priority =%d",ucEsId,10);
    }
    #endif
    if(eFmt == VDEC_FMT_H264)
    {
        #ifndef CC_VDEC_FMT_DETECT
            #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
            #ifndef CC_VDEC_H264_DISABLE
            if(!_VDEC_IsSupportH264())
            #endif
            {
                LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
                return FALSE;
            }
            #else
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
            #endif
        #endif
        if ( FALSE ==_fgIsSupportMPEG4AndH264 )
        {
            LOG(1, "%d,Video codec format H.264 not support!!",__LINE__);
            return FALSE;
        }
    }
    else if(eFmt == VDEC_FMT_WMV)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_VC1_DISABLE
        if(!_VDEC_IsSupportVc1())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_RV)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_RV_DISABLE
        if(!_VDEC_IsSupportRV())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MP4)
    {
        #ifdef CC_VDEC_MPEG4_DISABLE
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
        if (FALSE ==_fgIsSupportMPEG4AndH264 )
        {
            LOG(1, "%d,Video codec format MP4 not support!!",__LINE__);
            return FALSE;
        }
    }
    else if(eFmt == VDEC_FMT_MJPEG)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_MJPEG_DISABLE
        if(!_VDEC_IsSupportMJPEG())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MPV)
    {
        #if defined(CC_MT5387) && !defined(CC_MT5363)
        if((!prVdecEsInfo->fgMMPlayback) && (!IS_SupportDTV()))
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #endif
    }
    else if(eFmt==VDEC_FMT_VP6)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_VP6_DISABLE   
        if(!_VDEC_IsSupportVP6())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt==VDEC_FMT_VP8)
    {
    #ifdef ENABLE_MULTIMEDIA
    #ifndef CC_VDEC_VP8_DISABLE   
        if(!_VDEC_Vp8IsSupport())
    #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
    #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
    #endif
    }

    else if(eFmt >= VDEC_FMT_MAX)
    {
        LOG(1, "Video codec type %d err!!\n", (UINT32)eFmt);
        return FALSE;
    }

	//DTV00094254 for network case, when buffer underflow--->ready, mw will send vdec_pause--->vdec_play
	// for PS/TS/ES in FR or FF 4x, we use VDEC_PlayOneFrm() but not vdec_play, so we should add VDEC_PlayOneFrm() 's function
	//
	
	UNUSED(u4Tmp);
#ifdef ENABLE_MULTIMEDIA
	SWDMX_GetInfo(SWDMX_PRIMARY_SOURCE, eSWDMX_CONTAINER_TYPE, &u4Tmp, 0, 0);
	if(((u4Tmp == (UINT32)SWDMX_FMT_H264_VIDEO_ES) ||
	   (u4Tmp == (UINT32)SWDMX_FMT_MPEG2_VIDEO_ES) ||
	   (u4Tmp == (UINT32)SWDMX_FMT_MPEG4_VIDEO_ES) ||
	   (u4Tmp == (UINT32)SWDMX_FMT_VC1_ES)||
	   (u4Tmp == (UINT32)SWDMX_FMT_MPEG1_DAT)||
       (u4Tmp == (UINT32)SWDMX_FMT_MPEG2_TS)||
       (u4Tmp == (UINT32)SWDMX_FMT_MPEG2_PS)||
       (u4Tmp == (UINT32)SWDMX_FMT_MPEG2_TS_192)||
       (u4Tmp == (UINT32)SWDMX_FMT_MPEG2_TS_ZERO_192)||
       (u4Tmp == (UINT32)SWDMX_FMT_MPEG2_TS_ENCRYPT_192))&&
       (((prVdecEsInfo->u4MMSpeed >= STC_SPEED_TYPE_FORWARD_4X)&&(prVdecEsInfo->u4MMSpeed <= STC_SPEED_TYPE_FORWARD_1024X))||
        (prVdecEsInfo->u4MMSpeed >= STC_SPEED_TYPE_REWIND_1X)))
	   {
	   
          LOG(5, "VDEC_PlayOneFrm\n");
          prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
          prVdecEsInfo->fgPlayOneFrm = TRUE;

          //Put Backward Reference pic to EMPTY
          FBM_GetRefFrameBuffer(prVdecEsInfo->ucFbgId, &ucFRefFbId, &ucBRefFbId);
          //prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
          //if(prFbmPicHdr)
          {
              if(FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_READY))
              {
                 FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_EMPTY);
                 LOG(3, "VDEC_PlayOneFrm ucBRefFbId %d\n", ucBRefFbId);
              }
              if(FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_READY))
              {
                  FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_EMPTY);
                  LOG(3, "VDEC_PlayOneFrm ucFRefFbId %d\n", ucFRefFbId);
               }
           }

           prVdecEsInfo->fgKeepPes = FALSE;
           prVdecEsInfo->fgSendedEos = FALSE;
           prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;

	    }      
       else
#endif	   	
       {
          prVdecEsInfo->fgPlayOneFrm = FALSE;
       }
	   
    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_PLAY);

    // Virtual Picture Header for Event Trigger
    _arPesInfo[ucEsId].ucEsId = ucEsId;
    _arPesInfo[ucEsId].ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_arPesInfo[ucEsId]) == 1);

#if !defined(CC_EMULATION) && !defined(CC_USE_DDI)
    if(prVdecEsInfoKeep->hDataTimer && !prVdecEsInfoKeep->fgTimerStarted
        && !prVdecEsInfo->fgMMPlayback && !prVdecEsInfoKeep->fgVPush)
    {
        VERIFY(x_timer_start(prVdecEsInfoKeep->hDataTimer, VDEC_DATA_TIMER_THRSD, X_TIMER_FLAG_REPEAT,
            _DataTimeout, (void*)(UINT32)ucEsId) == OSR_OK);
        prVdecEsInfoKeep->fgTimerStarted = TRUE;
    }
#endif
    #if 0
    //check if codec type setting error
    if((!prVdecEsInfoKeep->fgCodecTypeChecking) && (!prVdecEsInfo->fgMMPlayback))
    {
        VERIFY(x_timer_start(prVdecEsInfoKeep->hCodecChkTimer, VDEC_CODEC_CHK_TIMER_THRSD,
            X_TIMER_FLAG_ONCE, _CodecChkTimeout, (void*)(UINT32)ucEsId) == OSR_OK);
        prVdecEsInfoKeep->fgCodecTypeChecking = TRUE;
        prVdecEsInfo->u4VFifoDataSz = 0;
    }
    #endif
    return TRUE;
}


void VDEC_Play_I_Frame(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    prVdecEsInfoKeep->eCurFMT = eFmt;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo->fgPlayOneFrm = FALSE;

    if(eFmt == VDEC_FMT_H264)
    {
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
        if(!_VDEC_IsSupportH264())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return;
        #endif
    }
    else if(eFmt >= VDEC_FMT_MAX)
    {
        LOG(1, "Video codec type %d err!!\n", (UINT32)eFmt);
        return;
    }
    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_PLAY_I_FRAME);

    // Virtual Picture Header for Event Trigger
    _arPesInfo[ucEsId].ucEsId = ucEsId;
    _arPesInfo[ucEsId].ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_arPesInfo[ucEsId]) == 1);
}

void VDEC_PlayVideoClip(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfoKeep->eCurFMT = eFmt;

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_PLAY_VIDEO_CLIP);

    // Virtual Picture Header for Event Trigger
    _arPesInfo[ucEsId].ucEsId = ucEsId;
    _arPesInfo[ucEsId].ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_arPesInfo[ucEsId]) == 1);
}


//Asynchronous command
void VDEC_Stop(UCHAR ucEsId)
{
#if 0
    VDEC_INFO_T* prVdecInfo;
#endif
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "ucEsId >= VDEC_MAX_ES\n");
        ASSERT(ucEsId < VDEC_MAX_ES);
        ucEsId = (VDEC_MAX_ES-1);
    }

#if 0
    prVdecInfo = _VDEC_GetInfo();
#endif
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    //power off VDEC module
    //move to main loop, need sync with VDEC thread

    VERIFY(STC_SetPtsDrift(prVdecEsInfo->ucStcId, MPV_FIFO_CTRL_INIT) == STC_VALID);
#ifdef CC_VDEC_PRIORITY_ENABLE
    // should wakeup  vdec pending thread schedule
    //
	UINT32 i4Ret;
    SIZE_T zMsgSize = sizeof(VDEC_VLD_STATUS_T);
    VDEC_VLD_STATUS_T u1SendMsg;
	u1SendMsg = VDEC_VLD_NOUSE;
	if(prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId]== VDEC_VLD_PENDING)
	{

	    i4Ret = x_msg_q_send(prVdecEsInfoKeep->hVdecScheMsg,
			               &u1SendMsg, zMsgSize, 255);
		if ((i4Ret != OSR_OK))
		{
		  ASSERT(0);
		}
		LOG(3,"%s  send ScheMsg %d to Es(%d)\n", __FUNCTION__,u1SendMsg,ucEsId);
	}
	else
	{
		prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId]= VDEC_VLD_NOUSE;
	}
	VDEC_WakeUpPendingThread(ucEsId);
    LOG(3,"VDEC_Stop Esid(%d)\n",ucEsId);
#endif

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_STOP);

#ifndef CC_EMULATION
    FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    FBM_ReleaseDispQ(prVdecEsInfo->ucFbgId);
    FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
#endif

#if 0
    // release 1 frame to empty Q to avoid waiting too long at get_frame_buffer
    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
    {
        if(prVdecEsInfoKeep->eDecStatus == VDEC_DEC_HEADER_PARSE)
        {
            FBM_ReleaseOneFB(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId);
            prVdecEsInfo->ucFbId = FBM_FB_ID_UNKNOWN;
        }
    }
    else
    {
        FBM_ReleaseOneFB(prVdecEsInfo->ucFbgId, FBM_FB_ID_UNKNOWN);
    }
#endif
    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);

    MPV_StopCc(ucEsId);

     // Virtual Picture Header for Event Trigger
    _arPesInfo[ucEsId].ucEsId = ucEsId;
    _arPesInfo[ucEsId].ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_arPesInfo[ucEsId]) == 1);

    if(prVdecEsInfoKeep->hDataTimer && prVdecEsInfoKeep->fgTimerStarted
        && !prVdecEsInfo->fgMMPlayback)
    {
        VERIFY(x_timer_stop(prVdecEsInfoKeep->hDataTimer) == OSR_OK);
        prVdecEsInfoKeep->fgTimerStarted = FALSE;
    }
}

//Asynchronous command
void VDEC_Pause(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
#ifdef CC_VDEC_PRIORITY_ENABLE
    // should wake up  vdec  pending thread schedule
	UINT32 i4Ret;
    SIZE_T zMsgSize = sizeof(VDEC_VLD_STATUS_T);
    VDEC_VLD_STATUS_T u1SendMsg;
	u1SendMsg = VDEC_VLD_PAUSE;
	if(prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId]== VDEC_VLD_PENDING)
	{

	    i4Ret = x_msg_q_send(prVdecEsInfoKeep->hVdecScheMsg,&u1SendMsg, zMsgSize, 255);
		if ((i4Ret != OSR_OK))
		{
		  ASSERT(0);
		}
		LOG(3,"%s  send ScheMsg %d to Es(%d)\n", __FUNCTION__,u1SendMsg,ucEsId);


	}
	else
	{
		prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId] = VDEC_VLD_PAUSE;
	}
	VDEC_WakeUpPendingThread(ucEsId);

    LOG(3,"VDEC_Pause Esid(%d)\n",ucEsId);
#endif

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_PAUSE);
    if(!prVdecEsInfo->fgMMPlayback)
    {
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    }

    // Virtual Picture Header for Event Trigger
    _arPesInfo[ucEsId].ucEsId = ucEsId;
    _arPesInfo[ucEsId].ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_arPesInfo[ucEsId]) == 1);

    // for time-shift, change from DTV to MM-time-shfit
    if(prVdecEsInfoKeep->hDataTimer && prVdecEsInfoKeep->fgTimerStarted
        && prVdecEsInfo->fgMMPlayback)
    {
        VERIFY(x_timer_stop(prVdecEsInfoKeep->hDataTimer) == OSR_OK);
        prVdecEsInfoKeep->fgTimerStarted = FALSE;
    }
}

//Asynchronous command
void VDEC_Flush_DPB(UCHAR ucEsId)
{
    ASSERT(ucEsId < VDEC_MAX_ES);

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_FLUSH_DPB);

    // Virtual Picture Header for Event Trigger
    _arPesInfo[ucEsId].ucEsId = ucEsId;
    _arPesInfo[ucEsId].ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_arPesInfo[ucEsId]) == 1);
}

void VDEC_SyncStc(UCHAR ucEsId, UCHAR ucMode, UCHAR ucStcId)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    ASSERT(ucEsId < VDEC_MAX_ES);
    ASSERT(prVdecEsInfo!=NULL);

    prVdecEsInfo->ucSyncMode = ucMode;
    prVdecEsInfo->ucStcId = ucStcId;

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_AVSYNC);

    // Virtual Picture Header for Event Trigger
    _arPesInfo[ucEsId].ucEsId = ucEsId;
    _arPesInfo[ucEsId].ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_arPesInfo[ucEsId]) == 1);
}


//Synchronous command
void VDEC_RegEventNotifyFunc(PFN_VDEC_EVENT_NOTIFY pfFunc)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    prVdecInfo->pfErrNotify = pfFunc;
}

INT32 VDEC_SetDecNfy(UCHAR ucEsId, const VDEC_DEC_NFY_INFO_T* prDecNfyInfo)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    if (prDecNfyInfo == NULL)
    {
        return (MPV_DRV_INV_SET_INFO);
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfoKeep->pvDecNfyTag = prDecNfyInfo->pvTag;
    prVdecEsInfoKeep->pfDecNfyFct = prDecNfyInfo->pfDecNfy;

    return (VDEC_DRV_OK);
}

INT32 VDEC_SetVdecNotSupportNfy(UCHAR ucEsId, const PFN_VDEC_NOT_SUPPORT_NFY_FCT prVdecNtyNotSupport)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
	
    if (prVdecNtyNotSupport == NULL)
    {
        return (MPV_DRV_INV_SET_INFO);
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfoKeep->pfVdecNotSupporuNfyFct = prVdecNtyNotSupport;

    return (VDEC_DRV_OK);
}

BOOL VDEC_GetGnlInfo(UCHAR ucEsId, VDEC_GNL_INFO_T* prGnlInfo)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    if(prGnlInfo == NULL)
    {
        return FALSE;
    }
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "ucEsId >= VDEC_MAX_ES\n");
        ASSERT(ucEsId < VDEC_MAX_ES);
        ucEsId = (VDEC_MAX_ES-1);
    }
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if(prVdecEsInfo == NULL)
    {
        LOG(1, "VDEC_GetGnlInfo prVdecEsInfo null\n");
        return FALSE;
    }

    prGnlInfo->u2UdfNs = prVdecEsInfo->u2UdfNs;
    if(prVdecEsInfo->u4VldReadPtr >= prVdecEsInfo->u4VFifoPrevWptr)
    {
        prGnlInfo->u4ESBufDataSize = 
            ((prVdecEsInfo->u4FifoEnd - prVdecEsInfo->u4VldReadPtr) +
             (prVdecEsInfo->u4VFifoPrevWptr - prVdecEsInfo->u4FifoStart));
    }
    else
    {
        prGnlInfo->u4ESBufDataSize = 
            (prVdecEsInfo->u4VFifoPrevWptr - prVdecEsInfo->u4VldReadPtr);
    }
    prGnlInfo->u4DisplayQNum = FBM_CheckFrameBufferDispQ(prVdecEsInfo->ucFbgId);
    prGnlInfo->u4DecodingPTS = prVdecEsInfo->u4PTS;
    prGnlInfo->u4DisplayPTS = VDP_GetPts(VDP_Es2Vdp(ucEsId));
    return TRUE;
}

VOID VDEC_SetAFDNfy(const AFD_VDEC_STATUS_CB AFDNfy)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    INT32 i = 0;
    LOG(0, "AFD, set callback function, 0x%x\n", AFDNfy);
    for(i = 0; i< VDEC_MAX_ES; i++)
    {
        if(i < 2)
        {
            prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(i);
            if(!prVdecEsInfoKeep) 
                return;
            LOG(0, "AFD, instance(%d), AFDNfy(0x%x)\n", i, (UINT32)AFDNfy);
            prVdecEsInfoKeep->pfAFDNfyFct = AFDNfy;
        }
    }
}

BOOL VDEC_SendAFDInfo(UCHAR ucEsId)
{
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;    
    UINT32 u4FbgId, u4FbId, u4VideoFmt = 5; //default value
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    static UCHAR eAspectRatio = VDEC_ASPECT_RATIO_NONE;
    VDEC_HDR_AFD_INFO_T rHdrInfo;   

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if (!VDEC_IsLock(ucEsId))
    {        
        LOG(1, "AFD, vdec is unlock\n");
        return FALSE;
    }

    if(prVdecEsInfoKeep->pfnVdecGetParam)
    {
        prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, 0, &u4FbgId, &u4FbId, &u4VideoFmt);
        prVdecEsInfo->ucVideoFmt = (UCHAR)u4VideoFmt;
    }
    else
    {
        u4FbgId = (UINT32)prVdecEsInfo->ucFbgId;
        u4FbId = (UINT32)prVdecEsInfo->ucFbId;
    }

    if(u4FbgId == FBM_FBG_ID_UNKNOWN)
    {
        LOG(1, "AFD, u4FbgId unknown\n");
        return FALSE;
    }
    
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr((UCHAR)u4FbgId);
    
    x_memset((VOID*)(&rHdrInfo), 0, sizeof(VDEC_HDR_AFD_INFO_T));
    
    if(prVdecEsInfoKeep->pfnVdecIsAFD)
    {
        rHdrInfo.fgAfdPrs = prVdecEsInfoKeep->pfnVdecIsAFD(ucEsId);
    }
    else
    {
        rHdrInfo.fgAfdPrs = FALSE;
    }
    {
        rHdrInfo.fgHdrInvalid = FALSE;
        if(prVdecEsInfo->ucVideoFmt == VDEC_FMT_H264)
        {
            rHdrInfo.eAspectRatio = _AVCDecideAspectRatio(prFbmSeqHdr->ucAspRatInf,
                                                prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize);
        }
        else
        {
            rHdrInfo.eAspectRatio = _DecideAspectRatio(prFbmSeqHdr->ucAspRatInf,
                                                prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize);
        }
        rHdrInfo.ucAfd = prFbmSeqHdr->ucActFmt;
    }
    if(rHdrInfo.eAspectRatio != eAspectRatio)
    {
        LOG(0 , "eAspectRatio(%d -> %d), resolution(H:0x%x, V:0x%x)\n", 
            eAspectRatio, rHdrInfo.eAspectRatio, prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize);
        if(prVdecEsInfoKeep->pfAFDNfyFct)
        {   
            prVdecEsInfoKeep->fgAFD_ASPECT = FALSE;
            //prVdecEsInfoKeep->pfAFDNfyFct(ucEsId, (UINT32)(&rHdrInfo));
            prVdecEsInfoKeep->pfAFDNfyFct(ucEsId, rHdrInfo.eAspectRatio);
            LOG(0 , "AFD send change!\n");
        }
        else
        {
            LOG(0 , "AFD callback is null!\n");
        }
        eAspectRatio = rHdrInfo.eAspectRatio;
    }
    else
    {
        //LOG(1, "AFD, no need update!\n");
    }
    return TRUE;
}

BOOL VDEC_QueryAFDInfo(UCHAR ucEsId, VDEC_HDR_AFD_INFO_T* prHdrInfo)
{
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    UINT32 u4FbgId, u4FbId, u4VideoFmt = 5; //default value
    UINT32 u4DAR = 0;
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    if(prHdrInfo == NULL)
    {
        return FALSE;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if (!VDEC_IsLock(ucEsId))
    {
        return FALSE;
    }

    if(prVdecEsInfoKeep->pfnVdecGetParam)
    {
        prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, 0, &u4FbgId, &u4FbId, &u4VideoFmt);
        prVdecEsInfo->ucVideoFmt = (UCHAR)u4VideoFmt;
    }
    else
    {
        u4FbgId = (UINT32)prVdecEsInfo->ucFbgId;
        u4FbId = (UINT32)prVdecEsInfo->ucFbId;
    }

    if(prVdecEsInfoKeep->pfnVdecIsAFD)
    {
        prHdrInfo->fgAfdPrs = prVdecEsInfoKeep->pfnVdecIsAFD(ucEsId);
    }
    else
    {
        prHdrInfo->fgAfdPrs = FALSE;
    }

    if (u4FbgId == FBM_FBG_ID_UNKNOWN)
    {
        prHdrInfo->fgAfdPrs = FALSE;
        prHdrInfo->fgHdrInvalid = TRUE;
        return FALSE;
    }

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr((UCHAR)u4FbgId);
    if (prFbmSeqHdr == NULL)
    {
        x_memset(prHdrInfo, 0, sizeof(VDEC_HDR_INFO_T));
        prHdrInfo->fgHdrInvalid = TRUE;
        return FALSE;
    }
    else
    {
        prHdrInfo->fgHdrInvalid = FALSE;
        LOG(1,"\n %s %d   prVdecEsInfoKeep->eCurFMT  = %d\n",__FUNCTION__,__LINE__, prVdecEsInfoKeep->eCurFMT);
        if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
        {
            LOG(1,"\n %s %d   prFbmSeqHdr->ucAspRatInf  = %d\n",__FUNCTION__,__LINE__, prFbmSeqHdr->ucAspRatInf);
            if((MPEG_ASPECT_RATIO_TRANSMIT == prFbmSeqHdr->ucAspRatInf)||(MPEG_ASPECT_RATIO_1_1== prFbmSeqHdr->ucAspRatInf))
            {
                u4DAR = prFbmSeqHdr->u2OrgHSize * prFbmSeqHdr->u4AspectRatioWidth * 100 / (prFbmSeqHdr->u2OrgVSize * prFbmSeqHdr->u4AspectRatioHeight);
                if (((u4DAR *3) >> 2) < 110)
                {
                    prHdrInfo->eAspectRatio = VDEC_ASPECT_RATIO_4_3;
                }
                else if (((u4DAR *9) >> 4) < 110)
                {
                    prHdrInfo->eAspectRatio = VDEC_ASPECT_RATIO_16_9;
                }
                else if (((u4DAR *100) / 221) < 110)
                {
                    prHdrInfo->eAspectRatio = VDEC_ASPECT_RATIO_221_1;
                }
                else
                {
                    prHdrInfo->eAspectRatio = VDEC_ASPECT_RATIO_16_9;
                }
                LOG(10, "DAR(%d)= ResW[%u] *AspW[%u] *100 / (ResH[%u] *AspH[%u]) Asp(%d).\n", u4DAR, 
                    prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u4AspectRatioWidth, 
                    prFbmSeqHdr->u2OrgVSize, prFbmSeqHdr->u4AspectRatioHeight,
                    prHdrInfo->eAspectRatio);
            }
            else
            {
                prHdrInfo->eAspectRatio = _AVCDecideAspectRatio(prFbmSeqHdr->ucAspRatInf,
                                                prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize);
                LOG(1,"\n %s %d   prVdecEsInfoKeep->eCurFMT  = %d, prHdrInfo->eAspectRatio = %d\n",__FUNCTION__,__LINE__, prVdecEsInfoKeep->eCurFMT,prHdrInfo->eAspectRatio);
            }
        }
        else
        {
            prHdrInfo->eAspectRatio = _DecideAspectRatio(prFbmSeqHdr->ucAspRatInf,
                                                prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize);
            LOG(1,"\n %s %d   prVdecEsInfoKeep->eCurFMT  = %d, prHdrInfo->eAspectRatio = %d\n",__FUNCTION__,__LINE__, prVdecEsInfoKeep->eCurFMT,prHdrInfo->eAspectRatio);
        }
        prHdrInfo->ucAfd = prFbmSeqHdr->ucActFmt;
    }
    return TRUE;
}

void VDEC_ResetAFD(UCHAR ucEsId)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    if(ucEsId >= VDEC_MAX_ES)
    {
        return;
    }
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);


    if(prVdecEsInfoKeep->pfnVdecResetAFD)
    {
       prVdecEsInfoKeep->pfnVdecResetAFD(ucEsId);
    }
    return;
}

BOOL VDEC_GetSrcState(UCHAR ucEsId, BOOL *pbScramble)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    if (ucEsId>=VDEC_MAX_ES|| (NULL==pbScramble))
    {
        return FALSE;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    *pbScramble = (VDEC_SCRAMBLE_STATE_SCRAMBLED==prVdecEsInfo->eCurSourceState) ? TRUE : FALSE;
    
    return TRUE;
}

BOOL VDEC_GetAFDStatue(UCHAR ucEsId ,BOOL *AFDPara)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    if(ucEsId >= VDEC_MAX_ES)
    {
        return;
    }
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);



    if(prVdecEsInfoKeep)
    {
       *AFDPara = (prVdecEsInfoKeep->fgAFD_ASPECT)?TRUE:FALSE;
       return TRUE;
    }
    else
    {
       return FALSE;
    }

}

//Synchronous command
BOOL VDEC_QueryInfo(UCHAR ucEsId, VDEC_HDR_INFO_T* prHdrInfo)
{
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    FBM_PIC_HDR_T *prFbmPicHdr;
    FBM_PIC_HDR_T *prFbmPicHdr0;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    UINT32 u4FbgId, u4FbId, u4VideoFmt = 5; //default value
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    if(prHdrInfo == NULL)
    {
        return FALSE;
    }

    prHdrInfo->fgHdrInvalid = TRUE;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    prHdrInfo->eNotifyStatus = prVdecEsInfo->eNotifyStatus;
    prHdrInfo->u4FailCnt = prVdecEsInfo->u4FailCnt;

    prHdrInfo->fgSeqValid = prVdecEsInfo->fgSeqValid;
    prHdrInfo->u2SeqFrmRate = prVdecEsInfo->u2SeqFrmRate;

    if (!VDEC_IsLock(ucEsId))
    {
        return FALSE;
    }

    if(prVdecEsInfoKeep->pfnVdecGetParam)
    {
        prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, 0, &u4FbgId, &u4FbId, &u4VideoFmt);
        prVdecEsInfo->ucVideoFmt = (UCHAR)u4VideoFmt;
    }
    else
    {
        u4FbgId = (UINT32)prVdecEsInfo->ucFbgId;
        u4FbId = (UINT32)prVdecEsInfo->ucFbId;
    }

    if (u4FbgId == FBM_FBG_ID_UNKNOWN)
    {
        return FALSE;
    }

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr((UCHAR)u4FbgId);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr((UCHAR)u4FbgId, (UCHAR)u4FbId);
    prFbmPicHdr0 = FBM_GetFrameBufferPicHdr((UCHAR)u4FbgId, (UCHAR)0);
	#ifdef CC_REALD_3D_SUPPORT
	if(prFbmSeqHdr != NULL)
	{
	    prHdrInfo->fgB2R3DEnable = prFbmSeqHdr->fgB2R3DEnable;
	    prHdrInfo->fgB2RForce2D = prFbmSeqHdr->fgB2RForce2D;
	    prHdrInfo->u1B2R3DType = prFbmSeqHdr->u1B2R3DType;	    
	}
	prHdrInfo->e3DType = prVdecEsInfo->e3DType;
	prHdrInfo->u13DUsrCtrlMode = prVdecEsInfo->u13DUsrCtrlMode;
	#endif
    if (prFbmSeqHdr == NULL)
    {
        x_memset(prHdrInfo, 0, sizeof(VDEC_HDR_INFO_T));
        prHdrInfo->fgHdrInvalid = TRUE;
        return FALSE;
    }
    else
    {
        prHdrInfo->fgHdrInvalid = FALSE;
        prHdrInfo->fgCropping = prFbmSeqHdr->fgCropping;
        prHdrInfo->u4CropX = prFbmSeqHdr->u4CropX;
        prHdrInfo->u4CropY = prFbmSeqHdr->u4CropY;
        prHdrInfo->u4CropWidth = prFbmSeqHdr->u4CropWidth;
        prHdrInfo->u4CropHeight = prFbmSeqHdr->u4CropHeight;

        if((prFbmSeqHdr->fgResizeSmallPic) && (prFbmPicHdr) && (prFbmPicHdr0))
        {
            if((prFbmPicHdr->u4PicWidth != 0) && (prFbmPicHdr->u4PicHeight != 0))
            {
                prHdrInfo->u2Width = prFbmPicHdr->u4PicWidth;
                prHdrInfo->u2Height = prFbmPicHdr->u4PicHeight;
                prHdrInfo->u2DHS = prFbmPicHdr->u4PicWidth;
                prHdrInfo->u2DVS = prFbmPicHdr->u4PicHeight;
                prHdrInfo->u2OrgWidth = prFbmPicHdr->u4PicWidth;
                prHdrInfo->u2OrgHeight = prFbmPicHdr->u4PicHeight;
            }
            else if((prFbmPicHdr0->u4PicWidth != 0) && (prFbmPicHdr0->u4PicHeight != 0))
            {
                prHdrInfo->u2Width = prFbmPicHdr0->u4PicWidth;
                prHdrInfo->u2Height = prFbmPicHdr0->u4PicHeight;
                prHdrInfo->u2DHS = prFbmPicHdr0->u4PicWidth;
                prHdrInfo->u2DVS = prFbmPicHdr0->u4PicHeight;
                prHdrInfo->u2OrgWidth = prFbmPicHdr0->u4PicWidth;
                prHdrInfo->u2OrgHeight = prFbmPicHdr0->u4PicHeight;
            }
            else
            {
                prHdrInfo->u2Width = prFbmSeqHdr->u2HSize;
                prHdrInfo->u2Height = prFbmSeqHdr->u2VSize;
                prHdrInfo->u2DHS = prFbmSeqHdr->u2DHS;
                prHdrInfo->u2DVS = prFbmSeqHdr->u2DVS;
                prHdrInfo->u2OrgWidth = prFbmSeqHdr->u2OrgHSize;
                prHdrInfo->u2OrgHeight = prFbmSeqHdr->u2OrgVSize;
            }
        }
        else
        {
            prHdrInfo->u2Width = prFbmSeqHdr->u2HSize;
            prHdrInfo->u2Height = prFbmSeqHdr->u2VSize;
            prHdrInfo->u2DHS = prFbmSeqHdr->u2DHS;
            prHdrInfo->u2DVS = prFbmSeqHdr->u2DVS;
            if(prFbmPicHdr)
            {
                prHdrInfo->u2OrgWidth = prFbmPicHdr->u4PicWidth;
                prHdrInfo->u2OrgHeight = prFbmPicHdr->u4PicHeight;
            }
            else
            {
                prHdrInfo->u2OrgWidth = prFbmSeqHdr->u2OrgHSize;
                prHdrInfo->u2OrgHeight = prFbmSeqHdr->u2OrgVSize;
            }                
        }
       

#if 0        
        if(prHdrInfo->eCodecType == VDEC_FMT_H264)
        {
            prHdrInfo->eAspectRatio = _AVCDecideAspectRatio(prFbmSeqHdr->ucAspRatInf,
                                                prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize);
        }
        else
        {
            prHdrInfo->eAspectRatio = _DecideAspectRatio(prFbmSeqHdr->ucAspRatInf,
                                                prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize);
        }
#endif
		prHdrInfo->ucVideoFmt = prVdecEsInfo->ucVideoFmt;
        prHdrInfo->u4BitRate = prFbmSeqHdr->u4BitRatVal;
        prHdrInfo->fgProgressiveSeq = prFbmSeqHdr->fgProgressiveSeq;
        prHdrInfo->ucAfd = prFbmSeqHdr->ucActFmt;
        prHdrInfo->fgNoOverScan = prFbmSeqHdr->fgNoOverScan;

        prHdrInfo->u4PixelAspectRatioWidth = prFbmSeqHdr->u4AspectRatioWidth;
        prHdrInfo->u4PixelAspectRatioHeight = prFbmSeqHdr->u4AspectRatioHeight;
        prHdrInfo->fgPixelAspectRatio = prFbmSeqHdr->fgPixelAspectRatio;
        
        prHdrInfo->fgContrained = prVdecEsInfo->fgContrained;
        prHdrInfo->fgMPEG2 = prVdecEsInfo->fgMPEG2;
        prHdrInfo->fgSeqDispExt = prVdecEsInfo->fgSeqDispExt;
        prHdrInfo->fgLowDelay = prVdecEsInfo->fgLowDelay;
        prHdrInfo->ucProfileId = prVdecEsInfo->ucProfileId;
        prHdrInfo->u4VbvSizeExt = prVdecEsInfo->u4VbvSizeExt;

        prHdrInfo->ucPicType = prVdecEsInfo->ucPicType;

        if(prFbmPicHdr != NULL)
        {
            prHdrInfo->u2TempRef = (UINT16)prFbmPicHdr->i4TemporalRef;
            prHdrInfo->ucPicStruct = prFbmPicHdr->ucPicStruct;
            prHdrInfo->fgTFF = prFbmPicHdr->fgTopFldFirst;
            prHdrInfo->fgRFF = prFbmPicHdr->fgRepFirstFld;
            prHdrInfo->fgProgressiveFrm = prFbmPicHdr->fgProgressiveFrm;
        }
        prHdrInfo->fgColorDescrip = prFbmSeqHdr->fgColorDescrip;
        prHdrInfo->ucMatrixCoefficients = prFbmSeqHdr->ucMatrixCoeff;
        prHdrInfo->ucTransferCharacteristics = prFbmSeqHdr->ucTransCharacter;
        prHdrInfo->ucColorPrimaries = prFbmSeqHdr->ucColourPrimaries;
        prHdrInfo->u4ChromaFmt = prFbmSeqHdr->ucCrmaFmt;

        switch (prFbmSeqHdr->ucFrmRatCod)
        {
            case MPEG_FRAME_RATE_24_:
                prHdrInfo->u2FrmRate = 2397;
                break;
            case MPEG_FRAME_RATE_24:
                prHdrInfo->u2FrmRate = 2400;
                break;
            case MPEG_FRAME_RATE_25:
                prHdrInfo->u2FrmRate = 2500;
                break;
            case MPEG_FRAME_RATE_30_:
                prHdrInfo->u2FrmRate = 2997;
                break;
            case MPEG_FRAME_RATE_30:
                prHdrInfo->u2FrmRate = 3000;
                break;
            case MPEG_FRAME_RATE_50:
                prHdrInfo->u2FrmRate = 5000;
                break;
            case MPEG_FRAME_RATE_60_:
                prHdrInfo->u2FrmRate = 5994;
                break;
            case MPEG_FRAME_RATE_60:
                prHdrInfo->u2FrmRate = 6000;
                break;
            default:
                prHdrInfo->u2FrmRate = 0;
                break;
        }
        //frame buffer number
        prHdrInfo->eCodecType = prVdecEsInfoKeep->eCurFMT;
        if(prHdrInfo->eCodecType == VDEC_FMT_H264)
        {
            if(prVdecEsInfoKeep->pfnVdecGetParam)
            {
                prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, 1,
                    &prHdrInfo->u4Profile, &prHdrInfo->u4Level, &prHdrInfo->u4IsCABAC);
            }
        }
		prHdrInfo->eAspectRatio=_VdecDecideAspectRatio(prHdrInfo->eCodecType,prFbmSeqHdr->ucAspRatInf,
		                          prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize,prVdecEsInfo->fgMMPlayback); 

        prHdrInfo->u1CurState = (UINT8)prVdecEsInfoKeep->eCurState;
        prHdrInfo->ucFbNum = FBM_GetFrameBufferNs((UCHAR)u4FbgId);
        prHdrInfo->u4MaxEsCnt = prVdecEsInfo->u4MaxEsCnt;
        prHdrInfo->u4CurEsCnt = (UINT32)prVdecEsInfo->u2Count;
        prHdrInfo->u4DisplayQPicCnt = prVdecEsInfo->u4DisplayQPicCnt;
        prHdrInfo->u4FirstDispPTS = prVdecEsInfo->u4FirstDispPTS;

        prHdrInfo->fgDeblocking = prVdecEsInfo->fgDeblocking;
        prHdrInfo->u4Dbk1PixelCnt = prVdecEsInfo->u4Dbk1PixelCnt;
        prHdrInfo->u4Dbk2PixelCnt = prVdecEsInfo->u4Dbk2PixelCnt;
        prHdrInfo->u4Dbk3PixelCnt = prVdecEsInfo->u4Dbk3PixelCnt;

#ifdef VDEC_TIME_PROFILE        
        x_memcpy((void*)&prHdrInfo->au4DecTime[0], (void*)&prVdecEsInfo->au4DecTime[0], sizeof(prVdecEsInfo->au4DecTime));
#endif
    }
    return TRUE;
}

//Set parameter
void VDEC_SetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

#if defined(VDEC_CRC_TEST) && !defined(CC_STRESS_TEST_CRC_CHK)
    /**RECORD_CRC**/
    if(u4Type == 16)
    {
        VDEC_SetTestCRC(ucEsId, (BOOL)u4Param1);
        if((BOOL)u4Param1)
        {
            LOG(0, "the crc record is enable!!\n");
        }
        else
        {
            LOG(0, "the crc record is disable!!\n");
        }
        return;
    }
#endif

#ifndef VDEC_TIME_PROFILE
    if(u4Type == 11)    //vdec_h264api.h
    {
        VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
        x_memset((void*)(prVdecEsInfo->au4DecTime), 0, 5*9*sizeof(UINT32));
    }
    else 
#endif        
    {
        if(prVdecEsInfoKeep->pfnVdecSetParam)
        {
            prVdecEsInfoKeep->pfnVdecSetParam(ucEsId , u4Type, u4Param1, u4Param2, u4Param3);
        }
    }
}

//Get parameter
void VDEC_GetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(prVdecEsInfoKeep->pfnVdecGetParam)
    {
        prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, u4Type, pu4Param1, pu4Param2, pu4Param3);
    }
}

//Set parameter
void VDEC_SetWptr(UINT8 u1VdecId, UINT32 u4Wptr)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo((UCHAR)u1VdecId);
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep((UCHAR)u1VdecId);
    FBM_POOL_T* prFbmPool;

    // get video fifo size from FBM
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != 0);

    if((prVdecEsInfo->u4VFifoPrevWptr > 0) && (prVdecEsInfo->u4VFifoDataSz <= CODEC_CHK_DATASZ))
    {
        if(u4Wptr > prVdecEsInfo->u4VFifoPrevWptr)
        {
            prVdecEsInfo->u4VFifoDataSz += (u4Wptr - prVdecEsInfo->u4VFifoPrevWptr);
        }
        else
        {
            prVdecEsInfo->u4VFifoDataSz +=
                ((u4Wptr + prFbmPool->u4Size) - prVdecEsInfo->u4VFifoPrevWptr);
        }
    }
    prVdecEsInfo->u4VFifoPrevWptr = u4Wptr;

    if(prVdecEsInfoKeep->pfnVdecSetWptr)
    {
        prVdecEsInfoKeep->pfnVdecSetWptr(u1VdecId, u4Wptr);
    }
}

BOOL VDEC_IsLock(UCHAR ucEsId)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    UINT32 u4FbgId, u4FbId;
    FBM_SEQ_HDR_T *prFbmSeqHdr;

    if(ucEsId >= VDEC_MAX_ES)
    {
        return FALSE;
    }

    if((prVdecInfo == NULL) || (prVdecEsInfo == NULL))
    {
        return FALSE;
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    /* MPV_IsLock is designed to check if mpv driver receive any video frame
       in a period. Original design is used for no signal check.
       Now we have decode status notify. If customer need that, we could give a
       notification to them.
    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV)
    {
        return MPV_IsLock(ucEsId);
    }
    else
    */
    {
        if(prVdecEsInfoKeep->pfnVdecGetParam)
        {
            prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, 0, &u4FbgId, &u4FbId, &u4FbId);
        }
        else
        {
            u4FbgId = (UINT32)prVdecEsInfo->ucFbgId;
        }

        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr((UCHAR)u4FbgId);
        if(!prFbmSeqHdr)
        {
            return FALSE;
        }
        if((prFbmSeqHdr->u2HSize > 0) &&
            (prFbmSeqHdr->u2VSize > 0) &&
            (prFbmSeqHdr->u2OrgHSize > 0) &&
            (prFbmSeqHdr->u2OrgVSize > 0))
        {
            return TRUE;
        }
    }

    // ToDo: add other format
    return FALSE;
}


//Set parameter
void VDEC_PlayMM(UCHAR ucEsId, BOOL fgPlayMM)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= VDEC_MAX_ES)
    {
        ucEsId = ES0;
        LOG(1, "VDEC_PlayMM: (ucEsId >= VDEC_MAX_ES)\n");
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(prVdecEsInfo)
    {
        prVdecEsInfo->fgMMPlayback = fgPlayMM;
    }
}

void VDEC_RePlayVdec(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
	VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    if(ucEsId >= VDEC_MAX_ES)
    {
        ucEsId = ES0;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
	prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
	if(prVdecEsInfoKeep)
	{
		if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
		{
			VDEC_SetRenderFromPos(ucEsId, TRUE,(UINT64)0, (INT32)0, (UINT16)0);
			if(prVdecEsInfo)
		    {
		        VDEC_SetWaitPvrSps(ucEsId, (UINT8)1);
		    }
		}
		else
		{
			LOG(0, "VDEC_RePlayVdec: the codec(%d) not implement the function \n",prVdecEsInfoKeep->eCurFMT);
		}
		
	}
	else
	{
		LOG(0, "VDEC_RePlayVdec: (prVdecEsInfoKeep == NULL)\n");
	}
    
}

//Get parameter
BOOL VDEC_IsPlayMM(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= VDEC_MAX_ES)
    {
        ucEsId = ES0;
        LOG(1, "VDEC_IsPlayMM: (ucEsId >= VDEC_MAX_ES)\n");
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(prVdecEsInfo)
    {
        return prVdecEsInfo->fgMMPlayback;
    }
    return FALSE;
}


//Set parameter
void VDEC_PlayNotDisplay(UCHAR ucEsId, BOOL fgNotDisplay)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= VDEC_MAX_ES)
    {
        ucEsId = ES0;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(prVdecEsInfo)
    {
        prVdecEsInfo->fgPlaybackNotDisplay = fgNotDisplay;
    }
}


//Get parameter
BOOL VDEC_IsPlayNotDisplay(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= VDEC_MAX_ES)
    {
        ucEsId = ES0;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(prVdecEsInfo)
    {
        return prVdecEsInfo->fgPlaybackNotDisplay;
    }
    return FALSE;
}


void VDEC_SetMMParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(u4Type == (UINT32)VDEC_MM_PLAY_SPEED)
    {
    	if(prVdecEsInfo->u4MMSpeed != u4Param1)
    	{
			prVdecEsInfo->fgSpeedChg = TRUE;
		}
        prVdecEsInfo->u4MMSpeed = u4Param1;
        LOG(5, "ES(%d) VDEC_MM_PLAY_SPEED(%d)\n", ucEsId, prVdecEsInfo->u4MMSpeed);
    }
	else if(u4Type==VDEC_MM_STREAM_TYPE)
	{
       prVdecEsInfo->eStreamType=(VDEC_STREAM_TYPE_T)u4Param1;
	}
    else if(u4Type == (UINT32)VDEC_MM_SEQ_INFO)
    {
        x_memcpy((void*)&prVdecEsInfo->rMMSeqInfo, (void*)u4Param1, sizeof(VDEC_SEQUENCE_DATA_T));
        prVdecEsInfo->fgMMSeqInfoValid = TRUE;
    }
    else if(u4Type == (UINT32)VDEC_MM_TPT_SEQ_INFO)
    {
        x_memcpy((void*)&prVdecEsInfo->rMMTPTSeqInfo, (void*)u4Param1, sizeof(VDEC_SEQUENCE_DATA_T));
        prVdecEsInfo->fgMMTPTSeqInfoValid = TRUE;
    }
    else if(u4Type == (UINT32)VDEC_MM_FRAME_RATE)
    {
        prVdecEsInfo->rMMSeqInfo.e_frame_rate = (ENUM_VDEC_FRAME_RATE_T)(u4Param1);
    }
#ifdef ENABLE_MULTIMEDIA
    else if(u4Type == (UINT32)VDEC_MM_CONTAINER_TYPE)
    {
        prVdecEsInfo->eContainerType = (ENUM_SWDMX_FMT_T)(u4Param1);
    }
    else if(u4Type == (UINT32)VDEC_MM_SRC_TYPE)
    {
        prVdecEsInfo->eMMSrcType = (SWDMX_SOURCE_TYPE_T)(u4Param1);
    }
#ifndef CC_VDEC_RV_DISABLE
    else if (u4Type == (UINT32)VDEC_MM_RV_RPR_INFO)
    {
        UINT32 au4Size[9] = {0,1,1,2,2,3,3,3,3};
        INT32 i;
        ASSERT(u4Param1 < 9);

        prVdecEsInfo->u4SzOfPicSz = au4Size[u4Param1];
        for (i = 0; i < 2 * u4Param1; i++)
        {
            prVdecEsInfo->au4RPRSizes[i] = *(UINT32 *)u4Param2;
            u4Param2 += 4;
        }
        prVdecEsInfo->fgRV9 = (BOOL)u4Param3;
    }
#endif
#endif
    else if(u4Type == (UINT32)VDEC_MM_3D_INFO)
    {
        prVdecEsInfo->e3DType = (VDEC_3D_TYPE_T)(u4Param1);
    }
#ifdef CC_3D_MM_DS_SUPPORT 
    else if(u4Type == (UINT32)VDEC_MM_3D_TRICK_TYPE)
    {
         prVdecEsInfo->u13DTrickModeType = (UINT8)u4Param1;
    }
#endif
	else if (u4Type == (UINT32)VDEC_MM_DIVX4_SIGNAL)
	{
		  prVdecEsInfo->fgSetDivX4 = TRUE;
	}
    else if(u4Type == (UINT32)VDEC_MM_PARSING_INFO)
    {
        prVdecEsInfo->fgParsingInfo = (u4Param1 > 0) ? TRUE : FALSE;
    }    
    else
    {
        LOG(3, "VDEC_SetMMParam: unknown type: %d\n", u4Type);
    }

    UNUSED(u4Param2);
    UNUSED(u4Param3);
}

//Set parameter
void VDEC_SetBigBParameter(UCHAR ucEsId, BOOL fgForceEnable, UINT32 u4BigBLevel1,
    UINT32 u4BigBLevel2, UINT32 u4BigBLevel3, UINT32 u4BigBBaseCnt)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    prVdecEsInfo->fgBigBForceEnable = fgForceEnable;
    prVdecEsInfo->u4BigBLevel1 = u4BigBLevel1;
    prVdecEsInfo->u4BigBLevel2 = u4BigBLevel2;
    prVdecEsInfo->u4BigBLevel3 = u4BigBLevel3;
    prVdecEsInfo->u4BigBBaseCnt = u4BigBBaseCnt;
}

//Set parameter
void VDEC_WaitSyncPoint(UCHAR ucEsId, BOOL fgTricking)
{
    //VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    #if 0
    PARAM_MPV_T *prMpvCounter;
    #endif

    //prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    #if 0
    prMpvCounter = (PARAM_MPV_T *)prVdecEsInfo->prMpvCounter;
    #endif

    MPV_FlushEsmQ(ucEsId, TRUE); // abort decoder? FixMe

    #if 0
    prVdecEsInfo->u4PicCntJump = prMpvCounter->u4DecOkNs;
    #endif
    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    prVdecEsInfo->ucSequenceHdrFound = FALSE;
    prVdecEsInfo->fgKeepPes = FALSE;
    prVdecEsInfo->fgSendedEos = FALSE;
    prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;
    prVdecEsInfo->fgTricking = fgTricking;
}

//Get parameter
#if 0
UINT32 VDEC_GetDecPicCount(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    PARAM_MPV_T *prMpvCounter;

    prMpvCounter = (PARAM_MPV_T *)prVdecEsInfo->prMpvCounter;

    return prMpvCounter->u4DecOkNs - prVdecEsInfo->u4PicCntJump;
}
#endif
//clear the seqinfo flags
void VDEC_ClrMMParam(UCHAR ucEsId, UINT32 u4Type)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgMMSeqInfoValid = FALSE;

}

//clear the seqinfo flags
void VDEC_SetTPTFlag(UCHAR ucEsId, BOOL fgIsTPT)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgIsTPT = fgIsTPT;
}

//set fgMpeg4 flag
void VDEC_SetMpeg4Flag(BOOL fgFlag)
{
    _fgIsSupportMPEG4AndH264 = fgFlag;
}

//Get parameter
UINT32 VDEC_GetAvgPicSize(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(prVdecEsInfo == NULL)
    {
        LOG(3, "VDEC_GetAvgPicSize prVdecEsInfo null");
        return 0;
    }

    return prVdecEsInfo->u4AvgPicSize;
}



void VDEC_GetDecStatus(UCHAR ucEsId, BOOL* pfgLock
    , BOOL* pfgTopFldFirst, BOOL* pfgRepFirstFld, BOOL* pfgProgressiveFrm
    , UCHAR* pucPicType, UCHAR* pucPicStruture, UCHAR* pucTemporalRef
    , UINT32* pu4ReceiveNs, UINT32* pu4DecOkNs)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    //VDEC_INFO_T *prVdecInfo;
    PARAM_MPV_T* prMpvCounter;
    FBM_PIC_HDR_T *prFbmPicHdr = NULL;
    UINT32 u4FbgId;
    UINT32 u4FbId;
    UINT32 u4VideoFmt;

    if((pfgLock == NULL) ||
       (pfgTopFldFirst == NULL) ||
       (pfgRepFirstFld == NULL) ||
       (pfgProgressiveFrm == NULL) ||
       (pucPicType == NULL) ||
       (pucPicStruture == NULL) ||
       (pucTemporalRef == NULL) ||
       (pu4ReceiveNs == NULL) ||
       (pu4DecOkNs == NULL))
    {
        LOG(1, "VDEC_GetDecStatus pointer null");
        return;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    //prVdecInfo = _VDEC_GetInfo();
    prMpvCounter = prVdecEsInfo->prMpvCounter;

    if (prVdecEsInfo->eNotifyStatus == VDEC_DEC_DECODE_NORMAL)
    {
        *pfgLock = TRUE;
    }
    else
    {
        *pfgLock = FALSE;
    }

    *pu4ReceiveNs = prMpvCounter->u4RetrieveNs;
    *pu4DecOkNs = prMpvCounter->u4DecOkNs;
    *pucPicType = prVdecEsInfo->ucPicType;

    if(prVdecEsInfoKeep->pfnVdecGetParam)
    {
        prVdecEsInfoKeep->pfnVdecGetParam(ucEsId, 0, &u4FbgId, &u4FbId, &u4VideoFmt);
    }
    else
    {
        u4FbgId = (UINT32)prVdecEsInfo->ucFbgId;
        u4FbId = (UINT32)prVdecEsInfo->ucFbId;
    }

    if (u4FbgId != FBM_FBG_ID_UNKNOWN)
    {
        prFbmPicHdr = FBM_GetFrameBufferPicHdr((UCHAR)u4FbgId, (UCHAR)u4FbId);
    }

    if(prFbmPicHdr)
    {
        *pucPicStruture = prFbmPicHdr->ucPicStruct;
        *pfgTopFldFirst = prFbmPicHdr->fgTopFldFirst;
        *pfgRepFirstFld = prFbmPicHdr->fgRepFirstFld;
        *pfgProgressiveFrm = prFbmPicHdr->fgProgressiveFrm;
        // ToDo: pucTemporalRef -> pi4TemporalRef
        *pucTemporalRef = (UCHAR)prFbmPicHdr->i4TemporalRef;
        *pucPicStruture = prFbmPicHdr->ucPicStruct;
    }
    else
    {
        *pucPicStruture = 0;
        *pfgTopFldFirst = 0;
        *pfgRepFirstFld = 0;
        *pfgProgressiveFrm = 0;
        *pucTemporalRef = 0;
        *pucPicStruture = 0;
    }
    return;
}


BOOL  VDEC_RegDecErrCb(PFN_VDEC_DECERR_CB pfnDecErrCb, UINT32 u4ErrDuration)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    if (pfnDecErrCb == NULL)
    {
        return FALSE;
    }

    prVdecInfo->pfnDecErrCb = pfnDecErrCb;
    prVdecInfo->u4ErrDuration= u4ErrDuration;

    return TRUE;
}


BOOL VDEC_RegPicTypeCb(PFN_VDEC_PIC_TYPE_CB pfnPicTypeCb)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    if (!prVdecInfo)
    {
        return FALSE;
    }

    prVdecInfo->pfPicTypeCb = pfnPicTypeCb;

    return TRUE;
}


BOOL VDEC_RegStatusCb(PFN_VDEC_STATUS_CB pfnStatusCb)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    if (!prVdecInfo)
    {
        return FALSE;
    }

    prVdecInfo->pfnStatusCb = pfnStatusCb;
    VDP_RegCbFunc(VDP_CB_FUNC_REPEAT_DROP_IND, (UINT32)_VDPReapeatDropCB, 0, 0);

    return TRUE;
}


//Asynchronous command
BOOL VDEC_PlayOneFrm(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    UCHAR ucFRefFbId, ucBRefFbId;

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    ASSERT(ucEsId < VDEC_MAX_ES);

    if(!prVdecEsInfoKeep)
	{
		return FALSE;
	}

    if((prVdecEsInfoKeep) &&
       ((eFmt != prVdecEsInfoKeep->eCurFMT) ||
        ((prVdecEsInfoKeep->eCurState != VDEC_ST_STOP) &&
         (prVdecEsInfoKeep->eCurState != VDEC_ST_IDLE) &&
         (prVdecEsInfoKeep->eCurState != VDEC_ST_PLAY)))) // trick mode in play state
    {
        return FALSE;
    }

    if(eFmt == VDEC_FMT_H264)
    {
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
        if(!_VDEC_IsSupportH264())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_WMV)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_VC1_DISABLE
        if(!_VDEC_IsSupportVc1())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MP4)
    {
        #ifdef CC_VDEC_MPEG4_DISABLE
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MJPEG)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_MJPEG_DISABLE
        if(!_VDEC_IsSupportMJPEG())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_RV)
    {
#ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_RV_DISABLE
        if(!_VDEC_IsSupportRV())
        #endif
    {
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
    }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
#endif
    }
    else if(eFmt == VDEC_FMT_VP6)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_VP6_DISABLE   
                if(!_VDEC_IsSupportVP6())
        #endif
            {
                LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
                return FALSE;
            }
        #else
                LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
                return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_VP8)
    {
    #ifdef ENABLE_MULTIMEDIA
    #ifndef CC_VDEC_VP8_DISABLE   
                if(!_VDEC_Vp8IsSupport())
    #endif
            {
                LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
                return FALSE;
            }
    #else
                LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
                return FALSE;
    #endif
    }

    {
        LOG(5, "VDEC_PlayOneFrm\n");
        prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
        prVdecEsInfo->fgPlayOneFrm = TRUE;

        //Put Backward Reference pic to EMPTY
        FBM_GetRefFrameBuffer(prVdecEsInfo->ucFbgId, &ucFRefFbId, &ucBRefFbId);
        //prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
        //if(prFbmPicHdr)
        {
            if(FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_READY))
            {
                FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_EMPTY);
                LOG(3, "VDEC_PlayOneFrm ucBRefFbId %d\n", ucBRefFbId);
            }
            if(FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_READY))
            {
                FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_EMPTY);
                LOG(3, "VDEC_PlayOneFrm ucFRefFbId %d\n", ucFRefFbId);
            }
        }

        /*
        FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
        FBM_ReleaseDispQ(prVdecEsInfo->ucFbgId);
        */

        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;

        _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_PLAY);
    }
    // Virtual Picture Header for Event Trigger
    _arPesInfo[ucEsId].ucEsId = ucEsId;
    _arPesInfo[ucEsId].ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_arPesInfo[ucEsId]) == 1);

    return TRUE;
}


BOOL  VDEC_RegRenderPtsCb(UCHAR ucEsId, PFN_VDEC_RENDER_PTS_CB pfnRenderPtsCb, UINT8 u1AttachedSrcId)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if (!prVdecEsInfoKeep)
    {
        return FALSE;
    }

    prVdecEsInfoKeep->pfnRenderPtsCb = pfnRenderPtsCb;
    prVdecEsInfoKeep->u1AttachedSrcId = u1AttachedSrcId;
    return TRUE;
}


BOOL  VDEC_RegTrickPtsCb(UCHAR ucEsId, PFN_VDEC_TRICK_PTS_CB pfnTrickPtsCb, UINT8 u1AttachedSrcId)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if (!prVdecEsInfoKeep)
    {
        return FALSE;
    }

    // trick -> normal, will register VDEC_RegTrickPtsCb = NULL;
    prVdecEsInfoKeep->pfnTrickPtsCb = pfnTrickPtsCb;
    prVdecEsInfoKeep->u1AttachedSrcId = u1AttachedSrcId;

    return TRUE;
}


BOOL  VDEC_SetRenderPts(UCHAR ucEsId, UINT32 u4Pts)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo->fgRenderFromPts = FALSE;
    prVdecEsInfo->fgRenderPts = TRUE;
    prVdecEsInfo->u4RenderPts = u4Pts;
    prVdecEsInfo->u4PTSLast = 0;
    prVdecEsInfo->fgSetStartPts = FALSE;

    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    //prVdecEsInfo->ucSequenceHdrFound = FALSE;
    prVdecEsInfo->fgKeepPes = FALSE;
    prVdecEsInfo->fgSendedEos = FALSE;
    prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;

    return TRUE;
}

BOOL  VDEC_SetTotalTime(UCHAR ucEsId, UINT32 u4TotalTime)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->u4TotalTime = u4TotalTime;

    return TRUE;
}

BOOL  VDEC_SetFileSize(UCHAR ucEsId, UINT64 u8FileSize)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->u8FileSize = u8FileSize;

    return TRUE;
}

BOOL  VDEC_SetLastTime(UCHAR ucEsId, UINT32 u4LastTime)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->u4PTSLast = u4LastTime;

    return TRUE;
}

BOOL  VDEC_SetWaitPvrSps(UCHAR ucEsId, UINT8 u1PVRWaitSps)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->u1PVRWaitSps = u1PVRWaitSps;

    return TRUE;
}

BOOL  VDEC_SetRenderBackStep(UCHAR ucEsId, BOOL fgRenderBackStep, UINT64 u8Pos,
    INT32 i4TempRef, UINT16 u2DecodingOrder)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_ES_INFO_T *prVdecEsInfo;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo->fgRenderBackStep = fgRenderBackStep;

    if(fgRenderBackStep)
    {
        prVdecEsInfo->u8RenderFromPos = u8Pos;
        prVdecEsInfo->i4RenderTempRef = i4TempRef;
        prVdecEsInfo->u2RenderDecodingOrder = u2DecodingOrder;
        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;

        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;
    }

    return TRUE;
}

BOOL  VDEC_SetRenderFromPos(UCHAR ucEsId, BOOL fgRenderFromPos,
    UINT64 u8Pos, INT32 i4TempRef, UINT16 u2DecodingOrder)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo->fgRenderFromPos = fgRenderFromPos;

    if(prVdecEsInfo->fgRenderFromPos)
    {
        prVdecEsInfo->u8RenderFromPos = u8Pos;
        prVdecEsInfo->i4RenderTempRef = i4TempRef;
        prVdecEsInfo->u2RenderDecodingOrder = u2DecodingOrder;
        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;

        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;
    }

    return TRUE;
}

BOOL  VDEC_SetRenderFromPts(UCHAR ucEsId, UINT32 u4Pts)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo->fgRenderPts = FALSE;
    prVdecEsInfo->fgRenderFromPts = TRUE;
    prVdecEsInfo->u4RenderFromPts = u4Pts;
    prVdecEsInfo->u2Disp2EmptyCnt = 0;
    prVdecEsInfo->u4PTSLast = 0;
    prVdecEsInfo->fgSetStartPts = FALSE;
    prVdecEsInfo->fgAutoPause = FALSE;
    prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;

    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    //prVdecEsInfo->ucSequenceHdrFound = FALSE;

    return TRUE;
}


BOOL  VDEC_SetRenderFromPtsAutoPause(UCHAR ucEsId, UINT32 u4Pts, BOOL fgAutoPause)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo->fgRenderFromPts = TRUE;
    prVdecEsInfo->u4RenderFromPts = u4Pts;
    prVdecEsInfo->u2Disp2EmptyCnt = 0;
    prVdecEsInfo->u4PTSLast = 0;
    prVdecEsInfo->fgSetStartPts = FALSE;
    prVdecEsInfo->fgAutoPause = fgAutoPause;
    prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;

    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    //prVdecEsInfo->ucSequenceHdrFound = FALSE;

    return TRUE;
}

BOOL  VDEC_SetRenderFromPtsAutoPauseStartPts(UCHAR ucEsId, UINT32 u4Pts, BOOL fgAutoPause, BOOL fgStartPts)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo->fgRenderFromPts = TRUE;
    prVdecEsInfo->u4RenderFromPts = u4Pts;
    prVdecEsInfo->u2Disp2EmptyCnt = 0;
    prVdecEsInfo->u4PTSLast = 0;
    prVdecEsInfo->fgSetStartPts = fgStartPts;
    prVdecEsInfo->fgAutoPause = fgAutoPause;
    prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;

    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    //prVdecEsInfo->ucSequenceHdrFound = FALSE;

    return TRUE;
}

BOOL  VDEC_SetRenderLastFrame(UCHAR ucEsId, BOOL fgRenderLastFrame)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo->fgRenderLastFrame = fgRenderLastFrame;

    if(prVdecEsInfo->fgRenderLastFrame)
    {
        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;

        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;
    }

    return TRUE;
}

BOOL VDEC_SetRenderFromFirstPic(UCHAR ucEsId,
    BOOL fgRenderFromFirstPic, UINT64 u8LastRenderPos)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo->fgRenderFromFirstPic = fgRenderFromFirstPic;

    if(fgRenderFromFirstPic)
    {
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;
        prVdecEsInfo->u8RenderFromPos = u8LastRenderPos;
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;
    }

    return TRUE;
}

BOOL VDEC_SetRenderPicCount(UCHAR ucEsId, BOOL fgRenderPicCnt, UINT32 u4Count)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo->fgRenderPicCnt = fgRenderPicCnt;
    prVdecEsInfo->u4RenderPicCnt = u4Count;
    if(fgRenderPicCnt)
    {
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;

        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;
    }

    return TRUE;
}

BOOL VDEC_SetGetSeqHdrOnlyBegin(UCHAR ucEsId,
    ENUM_VDEC_FMT_T eFmt, BOOL *pfgPowerOff)
{
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo->fgOnlyGotSeqHeader = TRUE;

    *pfgPowerOff = (!prVdecInfo->arVdecVldInfo[VLD0].fgPowerOn);
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

    return TRUE;
}

BOOL VDEC_SetGetSeqHdrOnlyEnd(UCHAR ucEsId, BOOL fgPowerOff)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(prVdecEsInfo->fgOnlyGotSeqHeader)
    {
        if((prVdecEsInfoKeep->pfnVdecStop) && (prVdecEsInfoKeep->eCurState == VDEC_ST_STOP))
        {
            prVdecEsInfoKeep->pfnVdecStop(ucEsId);
        }
        // I am not sure if we need this.
        //MPV_FlushEsmQ(ES0, FALSE);

        // for resume stop, we should assign this variable
        if(!prVdecEsInfo->ucSequenceHdrFound)
        {
            // copy from mpv_if.c _MPV_WaitRandomAP(...)
            prVdecEsInfo->ucSequenceHdrFound = 1;
            prVdecEsInfo->fgChgThrsd = TRUE;
            prVdecEsInfo->u4TotalPicSize = 0;
            prVdecEsInfo->u4PicCnt = 0;
            prVdecEsInfo->u4LastVldRptr = 0;
        }

        if(fgPowerOff)
        {
            UNUSED(VDEC_PowerOff(ucEsId, VLD0));
        }
    }
    prVdecEsInfo->fgOnlyGotSeqHeader = FALSE;

    return TRUE;
}

BOOL  VDEC_SetSuperFreeze(UCHAR ucEsId, BOOL fgSuperFreeze)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_ES_INFO_T *prVdecEsInfo;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(fgSuperFreeze)
    {
        LOG(5, "VDEC_SetSuperFreeze!!\n");

        MPV_FlushEsmQ(ucEsId, FALSE);

        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;

        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;

        FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SUPER_FREEZE);
        prVdecEsInfo->fgTricking = FALSE;
        prVdecEsInfo->fgMMPlayback = TRUE;
        VDEC_Pause(ucEsId);
        LOG(5, "VDEC_SetSuperFreeze exit!!\n");
    }

    return TRUE;
}


BOOL VDEC_GetDisplayFrameBufferAddr(UCHAR ucEsId, VDP_CAPTURE_INTO_T* prCapInfo)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    UINT32 u4FbNum;
    UINT32 u4Idx;
    UCHAR ucFbgId;
    UCHAR ucFbId;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_PIC_HDR_T* prFbmPicHdr;

    if(ucEsId >= VDEC_MAX_ES)
    {
        ucEsId = ES0;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    ucFbId = 0xFF;
    ucFbgId = prVdecEsInfo->ucFbgId;
    u4FbNum = (UINT32)FBM_GetFrameBufferNs(ucFbgId);
    for(u4Idx = 0; u4Idx < u4FbNum; u4Idx++)
    {
        if(FBM_GetFrameBufferStatus(ucFbgId, u4Idx)
            == FBM_FB_STATUS_DISPLAYQ)  //already freeze at mpv_mw.c
        {
            ucFbId = (UCHAR)u4Idx;
            break;
        }
    }
    if(ucFbId < 0xFF)
    {    
        FBM_GetFrameBufferAddr(ucFbgId, ucFbId, &(prCapInfo->u4AddrY), &(prCapInfo->u4AddrC));

        FBM_GetFrameBufferSize(ucFbgId, &(prCapInfo->u4MemHSize), &(prCapInfo->u4MemVSize));

        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);

        if (prFbmSeqHdr == NULL)
        {
            return FALSE;
        }
        else
        {
            /* Thumbnail usage: Image resizer will use this as pitch. */
            if (prFbmPicHdr && prFbmSeqHdr->fgResizeSmallPic)
            {
                prCapInfo->u4MemHSize = (UINT32)prFbmPicHdr->u4PicWidthPitch;

                prCapInfo->u4HSize = (UINT32)prFbmPicHdr->u4PicWidth;
                prCapInfo->u4VSize = (UINT32)prFbmPicHdr->u4PicHeight;
            }
            else
            {
                prCapInfo->u4MemHSize = (UINT32)prFbmSeqHdr->u2LineSize;

                prCapInfo->u4HSize = (UINT32)prFbmSeqHdr->u2OrgHSize;
                prCapInfo->u4VSize = (UINT32)prFbmSeqHdr->u2OrgVSize;
            }

            prCapInfo->u4RRMode = (UINT32)prFbmSeqHdr->fgRRYEn;
        }
    }

    return TRUE;

}

BOOL VDEC_GetIFrameBufferAddr(UCHAR ucEsId, VDP_CAPTURE_INTO_T* prCapInfo)
{
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_PIC_HDR_T* prFbmPicHdr;

    VDEC_ES_INFO_T *prVdecEsInfo;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(!prVdecEsInfo)
    {
        Printf("prVdecEsInfo = 0!\n");
        return FALSE;
    }

    prCapInfo->u4AddrY = 0;
    prCapInfo->u4AddrC = 0;
    FBM_GetFrameBufferAddr(prVdecEsInfo->ucFbgId,
        prVdecEsInfo->ucFbId, &(prCapInfo->u4AddrY), &(prCapInfo->u4AddrC));

    FBM_GetFrameBufferSize(prVdecEsInfo->ucFbgId,
        &(prCapInfo->u4MemHSize), &(prCapInfo->u4MemVSize));

    if((prCapInfo->u4AddrY == 0) ||
       (prCapInfo->u4AddrC == 0) ||
       (prCapInfo->u4MemHSize == 0) ||
       (prCapInfo->u4MemVSize == 0))
    {
        Printf("size = 0!\n");
        return FALSE;
    }

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdecEsInfo->ucFbgId);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId);

    if (prFbmSeqHdr == NULL)
    {
        Printf("prFbmSeqHdr = 0!\n");
        return FALSE;
    }
    else
    {
        /* Thumbnail usage: Image resizer will use this as pitch. */
        // because we do not enter b2r, we don't need to check resize case
        // but because we already assign origina information in picture header
        // and target information to sequence header, we have to use picture header in this case
        if (prFbmPicHdr && prFbmSeqHdr->fgResizeSmallPic)
        {
            prCapInfo->u4MemHSize = (UINT32)prFbmPicHdr->u4PicWidthPitch;

            prCapInfo->u4HSize = (UINT32)prFbmPicHdr->u4PicWidth;
            prCapInfo->u4VSize = (UINT32)prFbmPicHdr->u4PicHeight;
        }
        else
        {
            prCapInfo->u4MemHSize = (UINT32)prFbmSeqHdr->u2LineSize;

            prCapInfo->u4HSize = (UINT32)prFbmSeqHdr->u2OrgHSize;
            prCapInfo->u4VSize = (UINT32)prFbmSeqHdr->u2OrgVSize;
        }

        prCapInfo->u4RRMode = (UINT32)prFbmSeqHdr->fgRRYEn;
    }

    return TRUE;
}

BOOL  VDEC_GetVideoThumbnail(UCHAR ucEsId, UINT16 u2CompId, BOOL bIsOnePass,
    VDP_THUMBNAIL_INFO_T* prDstInfo, VDP_CAPTURE_INTO_T* prSrcInfo, BOOL fgKeepRation)
{
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
	VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "VDEC_GetVideoThumbnail ucEsId %d!!", (UINT32)ucEsId);
        return FALSE;
    }
    if((prDstInfo == NULL) || (prSrcInfo == NULL))
    {
        LOG(0, "VDEC_GetVideoThumbnail pointer null!!");
        return FALSE;
    }


	if(prVdecEsInfoKeep->eCurFMT==VDEC_FMT_MJPEG)
	{
		 #if  defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)// Force raster out for VENC
		prSrcInfo->u4IsRsIn = 1; //MJPEG for 5395 encoder input
		prSrcInfo->u4SrcSwap = 0;
		#else
		prSrcInfo->u4IsRsIn = 0;
		prSrcInfo->u4SrcSwap = 6;//block swap mode;		
		#endif
	}
	else	
	{
		prSrcInfo->u4IsRsIn = 0;
		prSrcInfo->u4SrcSwap = 6;//block swap mode;
	}

    prVdecEsInfo->u2CompId = u2CompId;
    x_memcpy((void*)&prVdecEsInfo->rCapDstInfo, (void*)prDstInfo, sizeof(VDP_THUMBNAIL_INFO_T));
    x_memcpy((void*)&prVdecEsInfo->rCapSrcInfo, (void*)prSrcInfo, sizeof(VDP_CAPTURE_INTO_T));
    prVdecEsInfo->bIsOnePass = bIsOnePass;
	prVdecEsInfo->fgKeepRation = fgKeepRation;

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_CAPTURE);

    // Virtual Picture Header for Event Trigger
    _arPesInfo[ucEsId].ucEsId = ucEsId;
    _arPesInfo[ucEsId].ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_arPesInfo[ucEsId]) == 1);

    UNUSED(prSrcInfo);
    UNUSED(prDstInfo);
    return TRUE;
}

BOOL  VDEC_GetVideoThumbnail2(UCHAR ucEsId,
    VDP_THUMBNAIL_INFO_T* prDstInfo,
    UINT32 u4WorkingBufAddr, UINT32 u4WorkingBufSize,
    BOOL fgKeepRation)
{
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    PARAM_MPV_T *prMpvCounter;
	//VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "ES(%d)(L:%d) %s fail\n", ucEsId, __LINE__, __FUNCTION__);
        return FALSE;
    }
    if((prDstInfo == NULL) || (!u4WorkingBufAddr) || (!u4WorkingBufSize))
    {
        LOG(0, "ES(%d)(L:%d) %s fail\n", ucEsId, __LINE__, __FUNCTION__);
        return FALSE;
    }
    if((prVdecEsInfo == NULL) || (prVdecEsInfoKeep == NULL))
    {
        LOG(0, "ES(%d)(L:%d) %s fail\n", ucEsId, __LINE__, __FUNCTION__);
        return FALSE;
    }

    prMpvCounter = (PARAM_MPV_T*)prVdecEsInfo->prMpvCounter;
    if(prMpvCounter == NULL)
    {
        LOG(0, "ES(%d)(L:%d) %s fail\n", ucEsId, __LINE__, __FUNCTION__);
        return FALSE;
    }

    #if 0
    if(prVdecEsInfoKeep->hThumbTimer && !prVdecEsInfoKeep->fgThumbTimerStarted)
    {
        VERIFY(x_timer_start(prVdecEsInfoKeep->hThumbTimer, VDEC_DATA_TIMER_THRSD, X_TIMER_FLAG_REPEAT,
            _ThumbTimeout, (void*)(UINT32)ucEsId) == OSR_OK);
        prVdecEsInfoKeep->fgThumbTimerStarted = TRUE;
    }
    #endif

    LOG(5, "ES(%d) %s\n", ucEsId, __FUNCTION__);

    x_memcpy((void*)&prVdecEsInfo->rCapDstInfo, (void*)prDstInfo, sizeof(VDP_THUMBNAIL_INFO_T));
	prVdecEsInfo->fgKeepRation = fgKeepRation;
	prVdecEsInfo->fgThumbStartTime = FALSE;
	prVdecEsInfo->u4ThumbRetrieveNs = prMpvCounter->u4RetrieveNs;
	prVdecEsInfo->u4WorkingBufAddr = u4WorkingBufAddr;
	prVdecEsInfo->u4WorkingBufSize = u4WorkingBufSize;
	prVdecEsInfo->fgThumbMethod2 = TRUE;

    UNUSED(prDstInfo);
    return TRUE;
}

BOOL  VDEC_StopVideoThumbnail2(UCHAR ucEsId)
{
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_ES_INFO_KEEP_T* prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "ES(%d)(L:%d) %s fail\n", ucEsId, __LINE__, __FUNCTION__);
        return FALSE;
    }
    if((prVdecEsInfo == NULL) || (prVdecEsInfoKeep == NULL))
    {
        LOG(0, "ES(%d)(L:%d) %s fail\n", ucEsId, __LINE__, __FUNCTION__);
        return FALSE;
    }

	prVdecEsInfo->fgThumbMethod2 = FALSE;
    LOG(5, "ES(%d) %s\n", ucEsId, __FUNCTION__);

    #if 0
    if(prVdecEsInfoKeep->hThumbTimer && prVdecEsInfoKeep->fgThumbTimerStarted)
    {
        VERIFY(x_timer_stop(prVdecEsInfoKeep->hThumbTimer) == OSR_OK);
        prVdecEsInfoKeep->fgThumbTimerStarted = FALSE;
    }
    #endif

    return TRUE;
}

// This function get I-Frame Y,C address, and use it to transform YC-Block --> ARGB8888(one pass if no keep ratio)
BOOL  VDEC_GetVideoThumbnail3(UCHAR ucEsId, UINT16 u2CompId, BOOL bIsOnePass,
    VDP_THUMBNAIL_INFO_T* prDstInfo,  BOOL fgKeepRation)
{
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
	VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    VDP_CAPTURE_INTO_T rSrcInfo;

    x_memset(&rSrcInfo, 0, sizeof(rSrcInfo));

    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "VDEC_GetVideoThumbnail ucEsId %d!!", (UINT32)ucEsId);
        return FALSE;
    }
    
    if((prDstInfo == NULL))
    {
        LOG(0, "VDEC_GetVideoThumbnail pointer null!!\n");
        return FALSE;
    }

    if(!VDEC_GetIFrameBufferAddr(ucEsId, &rSrcInfo))
    {
        LOG(1, " VDEC_GetIFrameBufferAddr fail\n");
        return FALSE;
    }
    
    if((rSrcInfo.u4AddrY == 0) || (rSrcInfo.u4AddrC== 0) || (rSrcInfo.u4RRMode > 1))
    {
        LOG(1, " addr fail\n");
        return FALSE;
    }
    else
    {
        // get pitch from FB, io_mtvdec can not get line size, we fill here
        prDstInfo->u4CanvasPitch = rSrcInfo.u4MemHSize*4;
        LOG(5, " u4AddrY 0x%x, u4 AddrC 0x%x \n",rSrcInfo.u4AddrY,rSrcInfo.u4AddrC);
    }

	if(prVdecEsInfoKeep->eCurFMT==VDEC_FMT_MJPEG)
	{
		#if  defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)// Force raster out for VENC
		rSrcInfo.u4IsRsIn = 1; //MJPEG for 5395 encoder input
		rSrcInfo.u4SrcSwap = 0;
		#else
		rSrcInfo.u4IsRsIn = 0;
		rSrcInfo.u4SrcSwap = 6;//block swap mode;
		#endif
	}
	else
	{
		rSrcInfo.u4IsRsIn = 0;
		rSrcInfo.u4SrcSwap = 6;//block swap mode;
	}

    prVdecEsInfo->u2CompId = u2CompId;
    x_memcpy((void*)&prVdecEsInfo->rCapDstInfo, (void*)prDstInfo, sizeof(VDP_THUMBNAIL_INFO_T));
    x_memcpy((void*)&prVdecEsInfo->rCapSrcInfo, (void*)&rSrcInfo, sizeof(VDP_CAPTURE_INTO_T));
    prVdecEsInfo->bIsOnePass = bIsOnePass;
	prVdecEsInfo->fgKeepRation = fgKeepRation;

    // trigger get thumbnail
    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_CAPTURE);

    // Virtual Picture Header for Event Trigger
    _arPesInfo[ucEsId].ucEsId = ucEsId;
    _arPesInfo[ucEsId].ucMpvId = (UCHAR)VDEC_MAX_VLD;

    // send virtual es
    VERIFY(VDEC_SendEs(&_arPesInfo[ucEsId]) == 1);

    UNUSED(rSrcInfo);
    UNUSED(prDstInfo);
    return TRUE;
}

//Synchronous command, only called when pause/idle/stop
BOOL VDEC_ReleaseDispQ(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    FBM_ReleaseDispQ(prVdecEsInfo->ucFbgId);
    FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);

    prVdecEsInfo->fgFlushFB = TRUE; //for h.264
    return TRUE;
}


BOOL VDEC_CkgenOnOff(BOOL fgEnable)
{
#ifdef VDEC_IS_POST_MT5368
#define BLOCK_RST_CFG0 0x270
#define VDEC_RST ((UINT32)1 << 18)
#define BLOCK_CKEN_CFG0 0x278
#define VDEC_CKEN ((UINT32)1 << 18)
    if (fgEnable)
    {
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~VDEC_RST);
        IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) | VDEC_CKEN);
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) | VDEC_RST);
    }
    else
    {
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~VDEC_RST);
        IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) & ~VDEC_CKEN);
    }
#endif

    UNUSED(fgEnable);
    return TRUE;
}


BOOL VDEC_PowerOn(UCHAR ucEsId, UCHAR ucVldId)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    if(ucVldId >= VDEC_MAX_VLD)
    {
        ASSERT(ucVldId < VDEC_MAX_VLD);
        return FALSE;
    }

    _VDEC_LockVld(ucEsId, ucVldId);
    prVdecInfo->arVdecVldInfo[ucVldId].u4PowerOnRef++;
    LOG(5, "PowerUp(vld%d)(ref%d)!!\n", ucVldId,
        prVdecInfo->arVdecVldInfo[ucVldId].u4PowerOnRef);
    if(prVdecInfo->arVdecVldInfo[ucVldId].fgPowerOn)
    {
        _VDEC_UnlockVld(ucEsId, ucVldId);
        return TRUE;
    }

    //power on VDEC module
    #ifdef CC_DYNAMIC_POWER_ONOFF
    #if defined(CC_MT5360) || defined(CC_MT5363)
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_MC) & VDEC_PWR_ON);
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_SYS) & VDEC_PWR_ON);
    #elif defined(CC_MT5387)
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) & VDEC_PWR_ON);
    #elif defined(CC_MT5365)
    // jpeg use the other h/w
    if(ucVldId == VLD0)
    {
        IO_WRITE32(VDEC_CTRL_BASE, VDEC_PWR_CTRL, VDEC_PWR_ON);
    }
    #elif defined(CC_MT5395)
    // jpeg use the other h/w
    if(ucVldId == VLD0)
    {
        IO_WRITE32(VDEC_CTRL_BASE, VDEC_PWR_CTRL, VDEC_PWR_ON);
    }
    #elif defined(CC_MT5396)
    // FIXME
    #elif defined(CC_MT5368)
    // FIXME
    #elif defined(CC_MT5389)
    // FIXME
    #else //if defined(CC_MT5391)
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) & VDEC_PWR_ON);
    #endif
    #endif
    prVdecInfo->arVdecVldInfo[ucVldId].fgPowerOn = TRUE;
    _VDEC_UnlockVld(ucEsId, ucVldId);   
    return TRUE;
}

BOOL VDEC_PowerOff(UCHAR ucEsId, UCHAR ucVldId)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    if(ucVldId >= VDEC_MAX_VLD)
    {
        ASSERT(ucVldId < VDEC_MAX_VLD);
        return FALSE;
    }

    if(!prVdecInfo->arVdecVldInfo[ucVldId].fgPowerOn)
    {
        return TRUE;
    }

    _VDEC_LockVld(ucEsId, ucVldId);
    prVdecInfo->arVdecVldInfo[ucVldId].u4PowerOnRef--;
    LOG(5, "PowerDown(vld%d)(ref%d)!!\n", ucVldId,
        prVdecInfo->arVdecVldInfo[ucVldId].u4PowerOnRef);
    if(prVdecInfo->arVdecVldInfo[ucVldId].u4PowerOnRef > 0)
    {
        _VDEC_UnlockVld(ucEsId, ucVldId);
        return TRUE;
    }

    //power off VDEC module
    #ifdef CC_DYNAMIC_POWER_ONOFF
    #if defined(CC_MT5360) || defined(CC_MT5363)
    IMGRZ_Lock();
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_MC) | VDEC_PWR_OFF);
    IMGRZ_Unlock();
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_SYS) | VDEC_PWR_OFF);
    #elif defined(CC_MT5387)
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) | VDEC_PWR_OFF);
    #elif defined(CC_MT5365)
    // jpeg use the other h/w
    if(ucVldId == VLD0)
    {
        IO_WRITE32(VDEC_CTRL_BASE, VDEC_PWR_CTRL, VDEC_PWR_OFF);
    }
    #elif defined(CC_MT5395)
    // jpeg use the other h/w
    if(ucVldId == VLD0)
    {
        IO_WRITE32(VDEC_CTRL_BASE, VDEC_PWR_CTRL, VDEC_PWR_OFF);
    }
    #elif defined(CC_MT5396)
    // jpeg use the other h/w
    if(ucVldId == VLD0)
    {
        IO_WRITE32(VDEC_MISC_BASE, VDEC_PDN_CON_SPEC, VDEC_PDN_CON_OFF);
        IO_WRITE32(VDEC_MISC_BASE, VDEC_PDN_CON_MODULE, VDEC_PDN_CON_OFF);
    }
    #elif defined(CC_MT5368)
    // jpeg use the other h/w
    if(ucVldId == VLD0)
    {
        IO_WRITE32(VDEC_MISC_BASE, VDEC_PDN_CON_SPEC, VDEC_PDN_CON_OFF);
        IO_WRITE32(VDEC_MISC_BASE, VDEC_PDN_CON_MODULE, VDEC_PDN_CON_OFF);
    }
    #elif defined(CC_MT5389)
    // jpeg use the other h/w
    if(ucVldId == VLD0)
    {
        IO_WRITE32(VDEC_MISC_BASE, VDEC_PDN_CON_SPEC, VDEC_PDN_CON_OFF);
        IO_WRITE32(VDEC_MISC_BASE, VDEC_PDN_CON_MODULE, VDEC_PDN_CON_OFF);
    }
    #else //if defined(CC_MT5391)
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) | VDEC_PWR_OFF);
    #endif
    #endif

    prVdecInfo->arVdecVldInfo[ucVldId].fgPowerOn = FALSE;
    _VDEC_UnlockVld(ucEsId, ucVldId);  
    return TRUE;
}

void VDEC_StartPTS(UCHAR ucEsId, BOOL fgEnCalPTS, UINT32 u4Pts, VDEC_PTS_INFO_T* prPTSInfo)
{
    //VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    prVdecEsInfo->fgEnCalPTS = fgEnCalPTS;
    prVdecEsInfo->u4StartPTS = u4Pts;
    if(prPTSInfo)
    {
        x_memcpy((void*)&prVdecEsInfo->rPTSInfo, (void*)prPTSInfo, sizeof(VDEC_PTS_INFO_T));
    }
    if(prVdecEsInfoKeep->pfnVdecStartPts)
    {
        _VDEC_FlushEs(ucEsId);
        UNUSED(VDEC_ReleaseDispQ(ucEsId));
        if(prVdecEsInfoKeep->pfnVdecStartPts)
        {
			prVdecEsInfoKeep->pfnVdecStartPts(ucEsId , fgEnCalPTS, u4Pts, (UINT32)prPTSInfo);
        }
    }
}

BOOL VDEC_ChkCodecCap(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt)
{
    UNUSED(ucEsId);

    if(eFmt == VDEC_FMT_H264)
    {
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
        if(!_VDEC_IsSupportH264())
        #endif
        {
            LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
        #endif

        if ( FALSE ==_fgIsSupportMPEG4AndH264 )
        {
            printf("%d,According to the profile information,Disable the h.264 format.\r\n",__LINE__);
            return FALSE;
        }
    }
    else if(eFmt == VDEC_FMT_WMV)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_VC1_DISABLE
        if(!_VDEC_IsSupportVc1())
        #endif
        {
            LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MP4)
    {
        #ifdef CC_VDEC_MPEG4_DISABLE
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
        #endif

        if ( FALSE ==_fgIsSupportMPEG4AndH264 )
        {
            return FALSE;
        }
    }
    else if(eFmt == VDEC_FMT_MJPEG)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_MJPEG_DISABLE
        if(!_VDEC_IsSupportMJPEG())
        #endif
        {
            LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_RV)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_RV_DISABLE
        if(!_VDEC_IsSupportRV())
        #endif
        {
            LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_AVS)
    {
        #ifndef CC_VDEC_AVS_DISABLE
        if(!_VDEC_IsSupportAVS())
        {
            LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_VP6)
    {
    #ifdef ENABLE_MULTIMEDIA
    #ifndef CC_VDEC_VP6_DISABLE   
        if(!_VDEC_IsSupportVP6())
    #endif
        {
            LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
            return FALSE;
        }
    #else
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
    #endif
    }
    else if(eFmt == VDEC_FMT_VP8)
    {
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_VP8_DISABLE   
        if(!_VDEC_Vp8IsSupport())
#endif
        {
            LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
            return FALSE;
        }
#else
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
#endif
    }

    return TRUE;

}

BOOL VDEC_ChkDivXBonding(VOID)
{
    #if defined(CC_MT5391)
    return IS_DivxSupport();
    #elif defined(VDEC_IS_POST_MT5368)
    return IS_SupportDivXHD();
    #elif defined(CC_MT5360) || defined(VDEC_IS_POST_MT5363)
    return IS_SupportDivX();
    #else
    return FALSE;
    #endif
}

BOOL VDEC_SetMid(UCHAR ucEsId, BOOL fgVdecMidEnable)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(prVdecEsInfoKeep)
    {
        prVdecEsInfoKeep->fgVdecMidEnable = fgVdecMidEnable;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL VDEC_Notify(UCHAR ucEsId,VDEC_DEC_DECODE_STATUS_T notifyStatus)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    if(notifyStatus==VDEC_DEC_DECODE_CODEC_NOT_SUPPORT)
    {
        if(prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT)
        {
            //callback, codec not support mpv data
            if (prVdecEsInfoKeep->pfDecNfyFct)
            {
                LOG(3, "ES(%d) Notify Status change, mpeg2 codec not support\n", ucEsId);
                prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
            }
            prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
        }
        return TRUE;
    }
    if(notifyStatus==VDEC_DEC_DECODE_NO_DATA)
    {
        if(prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_NO_DATA)
        {
            //callback, codec not support mpv data
            if (prVdecEsInfoKeep->pfDecNfyFct)
            {
                LOG(3, "ES(%d) Notify Status change, mpeg2 codec no data \n", ucEsId);
                prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag,
                VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_NO_DATA, 0);
            }
            prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_NO_DATA;
        }
        return TRUE;
    }
    else
    {
        LOG(3, "undefine notify status--%dt\n",notifyStatus);
        return FALSE;
    }
}

void VDEC_SetSrcId(UCHAR ucEsId, UINT8 u1SwdmxInstId )
{

    VDEC_ES_INFO_T *prVdecEsInfo;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->u1SwdmxSrcId=u1SwdmxInstId;

}

VOID VDEC_SetABInfo(UCHAR ucEsId, VDEC_AB_INTO_T *prABInfo)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->rABInfo=*prABInfo;
}

void VDEC_WakeUpPendingThread(UCHAR ucEsId)
{
	UINT8 u1temp=0;
	INT32 i4Ret=0;
    SIZE_T zMsgSize = sizeof(VDEC_VLD_STATUS_T);
	VDEC_VLD_STATUS_T u1SendMsg = VDEC_VLD_ACTIVE;
	VDEC_INFO_T *prVdecInfo;
	//VDEC_ES_INFO_T *prVdecEsInfo;
	//VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    //prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
	prVdecInfo = _VDEC_GetInfo();
	//prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    for(u1temp=0;u1temp<VDEC_MAX_ES;u1temp++)
    {
      if(prVdecInfo->arVdecEsInfoKeep[u1temp].arThreadStatus[prVdecInfo->arVdecEsInfo[u1temp].u4VldId]
	   == VDEC_VLD_PENDING)
      {

	      _VDEC_LockLoadCyclesMutex(ucEsId);
	      if(u1temp != ucEsId)
	      {
		    //_arVdecThreadWorkingCycles[u1temp] = _arVdec_Thread_Priority[u1temp];
            prVdecInfo->arVdecEsInfo[u1temp].i2VdecThreadWorkingCycles=
                                   prVdecInfo->arVdecEsInfo[u1temp].i2VdecThreadPriority;

	      }
	      _VDEC_UnlockLoadCyclesMutex(ucEsId);

	      i4Ret = x_msg_q_send(prVdecInfo->arVdecEsInfoKeep[u1temp].hVdecScheMsg,
		  	                   &u1SendMsg, zMsgSize, 255);
	      if ((i4Ret != OSR_OK))
	      {
		    ASSERT(0);
	      }
	      LOG(3,"%s  send ScheMsg to Es(%d)\n", __FUNCTION__ ,u1temp);
      }
    }
}

BOOL VDEC_SetPriority(UCHAR ucEsId,INT16 Priority)
{
	UINT8 u1temp=0;
	INT32 i4Ret=0;
    SIZE_T zMsgSize = sizeof(VDEC_VLD_STATUS_T);
    VDEC_VLD_STATUS_T u1SendMsg;
	VDEC_INFO_T *prVdecInfo;
	VDEC_ES_INFO_T *prVdecEsInfo;
	VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;


	prVdecInfo = _VDEC_GetInfo();
	prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
	prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    ASSERT(ucEsId < VDEC_MAX_ES);

	if((Priority<1)||
		(Priority>100)||
		(prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId] == VDEC_VLD_NOUSE)
	  )
	{
	   LOG(0,"VDEC_SetPriority fail\n");
	   return FALSE;
	}

	LOG(3,"VDEC_Play set Es(%d) priority =%d",ucEsId,Priority);

    prVdecEsInfo->i2VdecThreadPriority= Priority;

	_VDEC_LockLoadCyclesMutex(ucEsId);

    for(u1temp=0;u1temp<VDEC_MAX_ES;u1temp++)
    {
       if(prVdecInfo->arVdecEsInfoKeep[u1temp].arThreadStatus[prVdecInfo->arVdecEsInfo[u1temp].u4VldId]
	   	!= VDEC_VLD_NOUSE)
       {
	     //_arVdecThreadWorkingCycles[u1temp]= _arVdec_Thread_Priority[u1temp];
	     prVdecInfo->arVdecEsInfo[u1temp].i2VdecThreadWorkingCycles=
                                   prVdecInfo->arVdecEsInfo[u1temp].i2VdecThreadPriority;
       }
    }

   _VDEC_UnlockLoadCyclesMutex(ucEsId);

	u1SendMsg = VDEC_VLD_ACTIVE;
	if(prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId]== VDEC_VLD_PENDING)
	{

		i4Ret = x_msg_q_send(prVdecEsInfoKeep->hVdecScheMsg,&u1SendMsg, zMsgSize, 255);
		if ((i4Ret != OSR_OK))
		{
		  ASSERT(0);
		}
		LOG(3,"%s  send ScheMsg to Es(%d)\n", __FUNCTION__ ,u1temp);

	}
    VDEC_WakeUpPendingThread(ucEsId);

   return TRUE;
}
static UINT32 u4SedFd=0;
static UINT32 u4RecFd=0;

BOOL VDEC_ThreadSchedule(UCHAR ucEsId)
{
   INT32 i4Ret=0;
   UINT16 u2MsgQIdx;
   SIZE_T zMsgSize = sizeof(VDEC_VLD_STATUS_T);
   VDEC_VLD_STATUS_T u1SendMsg = VDEC_VLD_NOUSE;
   VDEC_VLD_STATUS_T u1RecMsg = VDEC_VLD_NOUSE;
   UINT8 u1temp=0;
   UINT8 u1temp2=0;

   VDEC_INFO_T *prVdecInfo;
   VDEC_ES_INFO_T *prVdecEsInfo;
   VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

   prVdecInfo = _VDEC_GetInfo();
   prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
   prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);


   //prVdecInfo->arVdecEsInfo[ucEsId]->hThreadScheMsg
   LOG(3,"%s Esid(%d) ++++++++ Decoded one frame \n", __FUNCTION__,ucEsId);


   if((prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId]== VDEC_VLD_FREE_RUN)||
   	  (prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId]== VDEC_VLD_FINISH)||
   	  (prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId]== VDEC_VLD_PAUSE)
   	 )
   {

	 LOG(5," VDEC_ThreadSchedule Esid(%d) free run \n",ucEsId);
     return TRUE;
   }

   for(u1temp=0; u1temp<VDEC_MAX_ES; u1temp++)
   {
      if(u1temp != ucEsId)
      {
		 u1temp2 |= prVdecInfo->arVdecEsInfoKeep[u1temp].arThreadStatus[prVdecInfo->arVdecEsInfo[u1temp].u4VldId];
      }
   }

   if(u1temp2 == VDEC_VLD_NOUSE)                //just one vdec thread is active, so no need to schedule
   {

	 return TRUE;

   }

   if(prVdecEsInfo->i2VdecThreadWorkingCycles>0)
   {
      if(prVdecEsInfo->fgSendedEos)       ////??? maybe should consider do seek after vdec send EOS
      {
         prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId] = VDEC_VLD_FINISH;
		 _VDEC_LockLoadCyclesMutex(ucEsId);
	     prVdecEsInfo->i2VdecThreadWorkingCycles=0;
		_VDEC_UnlockLoadCyclesMutex(ucEsId);

		 return TRUE;
      }
	  else
	  {
		 _VDEC_LockLoadCyclesMutex(ucEsId);
	     prVdecEsInfo->i2VdecThreadWorkingCycles--;
		 _VDEC_UnlockLoadCyclesMutex(ucEsId);

	  }

	  i4Ret=1;
	  for(u1temp=0; u1temp<VDEC_MAX_ES; u1temp++)
	  {
		 if(u1temp != ucEsId)
		 {
			if(prVdecInfo->arVdecEsInfoKeep[u1temp].arThreadStatus[prVdecInfo->arVdecEsInfo[u1temp].u4VldId]
			   == VDEC_VLD_ACTIVE)
			 {
			   i4Ret=0;
               break;
			 }
		 }
	  }

	  if((i4Ret)&&(prVdecEsInfo->i2VdecThreadWorkingCycles ==0))
	  {

	  	  // reload Thread working cycles;
		  _VDEC_LockLoadCyclesMutex(ucEsId);
		  for(u1temp=0;u1temp<VDEC_MAX_ES;u1temp++)
		  {
			  if(prVdecInfo->arVdecEsInfoKeep[u1temp].arThreadStatus[prVdecInfo->arVdecEsInfo[u1temp].u4VldId]
			  	== VDEC_VLD_PENDING)
			  {
				  //_arVdecThreadWorkingCycles[u1temp]=_arVdec_Thread_Priority[u1temp];
                  prVdecInfo->arVdecEsInfo[u1temp].i2VdecThreadWorkingCycles=
                                   prVdecInfo->arVdecEsInfo[u1temp].i2VdecThreadPriority;
			  }
		  }
		  prVdecEsInfo->i2VdecThreadWorkingCycles=prVdecEsInfo->i2VdecThreadPriority;
		  _VDEC_UnlockLoadCyclesMutex(ucEsId);

		  LOG(3,"%s Esid(%d) Reload working cycles \n", __FUNCTION__,ucEsId);

		 u1SendMsg= VDEC_VLD_ACTIVE;
		 for(u1temp=0;u1temp<VDEC_MAX_ES;u1temp++)
		 {
			 if((prVdecInfo->arVdecEsInfoKeep[u1temp].arThreadStatus[prVdecInfo->arVdecEsInfo[u1temp].u4VldId]
				 == VDEC_VLD_PENDING)&&
			     (u1temp != ucEsId))
			{
			   i4Ret = x_msg_q_send(prVdecInfo->arVdecEsInfoKeep[u1temp].hVdecScheMsg,
			   	                   &u1SendMsg, zMsgSize, 255);
				if ((i4Ret != OSR_OK))
				{
				  ASSERT(0);
				}
				LOG(3,"%s Esid(%d) send ScheMsg to Es thread(%d)u4SedFd(%d)\n",
					__FUNCTION__ ,ucEsId,u1temp,u4SedFd);
				u4SedFd ++;
			}

		 }
      }
      else if(prVdecEsInfo->i2VdecThreadWorkingCycles == 0)
      {

         LOG(3,"%s Esid(%d) waiting to received ScheMsg u4RecFd(%d) \n", __FUNCTION__,ucEsId,u4RecFd);
		  prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId] = VDEC_VLD_PENDING;

		 #if 0
	     i4Ret= x_msg_q_receive(&u2MsgQIdx, (VOID *)&u1RecMsg, &zMsgSize,
	   	                     &(prVdecEsInfo->hThreadScheMsg), 1, X_MSGQ_OPTION_WAIT);
		 #endif

		 i4Ret = x_msg_q_receive_timeout(&u2MsgQIdx, (VOID *)&u1RecMsg, &zMsgSize,
                         &(prVdecEsInfoKeep->hVdecScheMsg), 1, VDEC_SCHEDULE_TIMEOUT);
		 if(i4Ret == OSR_OK)
		 {

			if( prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId]== VDEC_VLD_PENDING)
			{
			    prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId] = u1RecMsg;
			}
			LOG(3,"%s Esid(%d) received ScheMsg u4RecFd(%d)\n", __FUNCTION__,ucEsId,u4RecFd);
			u4RecFd++;

		 }
		 else if (i4Ret == OSR_TIMEOUT)
		 {

		   LOG(0,"VDEC Schedule time out \n");
		    prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId] = VDEC_VLD_FREE_RUN;

		 }
	     else
		 {
			 ASSERT(0);
		 }
     }
	 else
	 {

	 }
   }
   else
   {
     if(( prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId] == VDEC_VLD_ACTIVE)||
	 	( prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId] == VDEC_VLD_PENDING))
     	{
		  i4Ret = x_msg_q_receive_timeout(&u2MsgQIdx, (VOID *)&u1RecMsg, &zMsgSize,
                         &(prVdecEsInfoKeep->hVdecScheMsg), 1, VDEC_SCHEDULE_TIMEOUT);
		  if(i4Ret == OSR_OK)
		  {

			if( prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId]== VDEC_VLD_PENDING)
			{
			    prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId] = u1RecMsg;
			}
			LOG(3,"%s Esid(%d) received ScheMsg u4RecFd(%d)\n", __FUNCTION__,ucEsId,u4RecFd);
			u4RecFd++;

		  }
		  else if (i4Ret == OSR_TIMEOUT)
		  {

		    LOG(0,"VDEC Schedule time out \n");
		    prVdecEsInfoKeep->arThreadStatus[prVdecEsInfo->u4VldId] = VDEC_VLD_FREE_RUN;

		  }
	      else
		  {
			 ASSERT(0);
		  }

     	}
   }

   return TRUE;
}

#ifdef CC_REALD_3D_SUPPORT
void VDEC_Set3DUserCtrl(UCHAR ucEsId, UINT8 u1Param)
{
   VDEC_ES_INFO_T *prVdecEsInfo;

   prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

   prVdecEsInfo->u13DUsrCtrlMode = u1Param;
}

#endif

#ifdef MULTIMEDIA_AUTOTEST_SUPPORT
void  VDEC_SetCrcGolden(UCHAR ucEsId, UINT32 uCrcGolden)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
	
    prVdecEsInfo->u4CrcGolden = uCrcGolden;
	prVdecEsInfo->fgAutoCrcCk= TRUE;
	
}

INT32 VDEC_SetAutoTestDecNfy(UCHAR ucEsId, UINT32 prDecNfy, void *pvTag)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    if (prDecNfy == NULL)
    {
        return (MPV_DRV_INV_SET_INFO);
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfoKeep->pfDecAutoTestNfyFct = (PFN_VDEC_DEC_NFY_FCT)prDecNfy;
    prVdecEsInfoKeep->pvDecAutoTestNfyTag = pvTag;

    return (VDEC_DRV_OK);
}

#ifdef VDEC_TIME_PROFILE
VOID VDEC_SetAutoTestCurFileDir(UCHAR ucEsId, CHAR *ucAutoPlayCurFileDir)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    x_memset(&(prVdecEsInfo->ucAutoPlayCurFileDir[0]), 0x0, sizeof(prVdecEsInfo->ucAutoPlayCurFileDir));
    x_memcpy(&(prVdecEsInfo->ucAutoPlayCurFileDir[0]), ucAutoPlayCurFileDir, sizeof(prVdecEsInfo->ucAutoPlayCurFileDir));
    
    return;
}
VOID VDEC_SetAutoTestCurFileName(UCHAR ucEsId, CHAR *ucAutoPlayCurFileName)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    x_memset(&(prVdecEsInfo->ucAutoPlayCurFileName[0]), 0x0, sizeof(prVdecEsInfo->ucAutoPlayCurFileName));
    x_memcpy(&(prVdecEsInfo->ucAutoPlayCurFileName[0]), ucAutoPlayCurFileName, sizeof(prVdecEsInfo->ucAutoPlayCurFileName));
    
    return;
}
#endif

#endif

#ifdef VDEC_CRC_TEST
BOOL VDEC_IsTestCRC(UCHAR ucEsId)
{
    ASSERT(ucEsId < VDEC_MAX_ES);
    return _fgCRCTest;
}

VOID VDEC_SetTestCRC(UCHAR ucEsId, BOOL fgTestOrNot)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->u4CrcResult = 0;
    _fgCRCTest = fgTestOrNot;
    return;
}
#endif
#ifndef VDEC_PUSH_SUPPORT
BOOL _VPUSH_Play(VOID* prdec)
{
    UNUSED(prdec);
    return TRUE;
}
BOOL _VPUSH_Stop(VOID* prdec)
{
    UNUSED(prdec);
    return TRUE;
}
BOOL _VPUSH_PutData(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    UNUSED(prdec);
    UNUSED(prBytesInfo);
    return TRUE;
}
BOOL _VPUSH_PutDataDone(VOID* prdec, UINT32 u4Tag)
{
    UNUSED(prdec);
    UNUSED(u4Tag);
    return TRUE;
}
VOID* _VPUSH_AllocVideoDecoder(ENUM_VDEC_FMT_T eFmt)
{
    UNUSED(eFmt);
    return NULL;
}
VOID _VPUSH_ReleaseVideoDecoder(VOID* prdec)
{
    UNUSED(prdec);
    return;
}
VOID _VPUSH_DecodeInit(VOID)
{
    return;
}
BOOL _VPUSH_DecodeReset(VOID* prdec)
{
    UNUSED(prdec);
    return TRUE;
}
VOID _VPUSH_DecodeDone(UCHAR ucEsId, VOID *prPicNfyInfo)
{
    UNUSED(ucEsId);
    UNUSED(prPicNfyInfo);
    return;
}
BOOL _VPUSH_RegCb(VOID* prdec, VDEC_PUSH_CB_T *prVdecPushCb)
{
    UNUSED(prdec);
    UNUSED(prVdecPushCb);
    return TRUE;
}
BOOL _VPUSH_GetVFifoInfo(VDEC_VFIFO_INTO_T *prVfifoInfo)
{
    UNUSED(prVfifoInfo);
    return TRUE;
}
BOOL _VPUSH_SetInfo(VOID* prdec, VDEC_SET_INTO_T *prVdecSetInfo)
{
    UNUSED(prdec);
    UNUSED(prVdecSetInfo);
    return TRUE;
}
BOOL _VPUSH_SetPushType(VOID* prdec, ENUM_VDEC_PUSH_FMT_T ePushFmt)
{
    UNUSED(prdec);
    UNUSED(ePushFmt);
    return TRUE;
}
VOID _VPUSH_SetPicSz(VOID* prdec, UINT32 u4Width, UINT32 u4Height)
{
    UNUSED(prdec);
    UNUSED(u4Width);
    UNUSED(prdec);
    return;
}
BOOL _VPUSH_AllocVFifo(VOID* prdec,
    UINT32 *pu4BufSA, UINT32 u4Size, ENUM_VDEC_FMT_T eFmt)
{
    UNUSED(prdec);
    UNUSED(pu4BufSA);
    UNUSED(u4Size);
    UNUSED(eFmt);
    return TRUE;
}
BOOL _VPUSH_FreeVFifo(VOID* prdec, UINT32 u4BufSA)
{
    UNUSED(prdec);
    UNUSED(u4BufSA);
    return TRUE;
}
#endif
#ifdef CC_ITIMING_ALWAYS
void VDEC_SetFixFHDDisplay(UCHAR ucEsId, BOOL fgInterlace)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfoKeep->fgFixFHDInterlaceDisplay = fgInterlace;
    
}
#endif


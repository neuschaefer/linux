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
 * $Date: 2012/11/09 $
 * $RCSfile: vdec_esm.c,v $
 * $Revision: #3 $
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
#ifndef CC_VDEC_AVS_DISABLE
#include "vdec_avsapi.h"
#endif
#ifndef CC_VDEC_VP6_DISABLE
#include "vdec_vp6api.h"
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
VDEC_INFO_T _rVdecInfo;
static VDEC_PES_INFO_T _arPesInfoEsm[VDEC_MAX_ES];
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

#define MVC_MAIN_VIDEO 0
#define MVC_SUB_VIDEO 1

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
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "_VDEC_GetEsInfo ucEsId(%d) > VDEC_MAX_ES, oops...\n", ucEsId);
        //ASSERT(0);
        ucEsId = (VDEC_MAX_ES - 1);
    }
    return &(_rVdecInfo.arVdecEsInfo[ucEsId]);
}

VDEC_ES_INFO_KEEP_T* _VDEC_GetEsInfoKeep(UCHAR ucEsId)
{
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "_VDEC_GetEsInfoKeep ucEsId(%d) > VDEC_MAX_ES, oops...\n", ucEsId);
        //ASSERT(0);
        ucEsId = (VDEC_MAX_ES - 1);
    }
    return &(_rVdecInfo.arVdecEsInfoKeep[ucEsId]);
}
/*
VDEC_PES_INFO_T* _VDEC_GetPesInfo()
{
    return &_rVdecInfo.rPesInfo;
}*/

void _VDEC_CreateEsmSema(UCHAR ucEsId)
{
    CHAR szBuf[16];
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "ucEsId >= VDEC_MAX_ES\n");
        return;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if((!prVdecEsInfo) || (!prVdecEsInfoKeep))
    {
        LOG(1, "prVdecEsInfo/prVdecEsInfoKeep null\n");
        return;
    }

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
    x_snprintf(szBuf, sizeof(szBuf),  "ESM-Q%d\n", ucEsId);
    VERIFY(x_msg_q_create(&prVdecEsInfoKeep->hMsgQ, szBuf,
        sizeof(VDEC_PES_INFO_T), VDEC_MSG_Q_SIZE) == OSR_OK);

    x_snprintf(szBuf, sizeof(szBuf),  "ESM-QSub%d\n", ucEsId);
    VERIFY(x_msg_q_create(&prVdecEsInfoKeep->hMsgQSub, szBuf,
        sizeof(VDEC_PES_INFO_T), VDEC_MSG_Q_SIZE) == OSR_OK);

    VERIFY(x_sema_create(&prVdecEsInfoKeep->hMutex, X_SEMA_TYPE_MUTEX,
        X_SEMA_STATE_UNLOCK) == OSR_OK);

    VERIFY(x_sema_create(&prVdecEsInfoKeep->hFlushSema, X_SEMA_TYPE_BINARY,
        X_SEMA_STATE_LOCK) == OSR_OK);


    VERIFY(x_sema_create(&prVdecEsInfoKeep->hFlushMutex, X_SEMA_TYPE_MUTEX,
        X_SEMA_STATE_UNLOCK) == OSR_OK);

    VERIFY(x_sema_create(&prVdecEsInfoKeep->hCcSema, X_SEMA_TYPE_MUTEX,
        X_SEMA_STATE_UNLOCK) == OSR_OK);

    VERIFY(x_sema_create(&prVdecEsInfoKeep->hVdecSetPriSema, X_SEMA_TYPE_MUTEX,
        X_SEMA_STATE_UNLOCK) == OSR_OK);

#ifdef CC_REALD_3D_SUPPORT
    VERIFY(x_sema_create(&prVdecEsInfoKeep->hVdec3DModeChange, X_SEMA_TYPE_MUTEX,
        X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif
	// create multi vdec thread schedule msg Q and Semaphone
	x_snprintf(szBuf, sizeof(szBuf),  "MTS-Q%d\n", ucEsId);
	VERIFY(x_msg_q_create(&prVdecEsInfoKeep->hVdecScheMsg, szBuf,
	sizeof(VDEC_VLD_STATUS_T), 1) == OSR_OK);
    // create command queue
    x_snprintf(szBuf, sizeof(szBuf),  "CMDQ-Q%d\n", ucEsId);
    VERIFY(x_msg_q_create(&prVdecEsInfoKeep->hCmdQ, szBuf, VDEC_CMD_SIZE,
        VDEC_CMD_Q_SIZE) == OSR_OK);

    VERIFY(x_timer_create(&prVdecEsInfoKeep->hDataTimer) == OSR_OK);
    #if 0
    VERIFY(x_timer_create(&prVdecEsInfoKeep->hThumbTimer) == OSR_OK);
    #endif
    VERIFY(x_timer_create(&prVdecEsInfoKeep->hCodecChkTimer) == OSR_OK);
}


void _VDEC_DeleteEsmSema(UCHAR ucEsId)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "ucEsId >= VDEC_MAX_ES\n");
        return;
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    if(!prVdecEsInfoKeep)
    {
        LOG(1, "prVdecEsInfoKeep null\n");
        return;
    }

    // delete semaphore
    VERIFY(x_sema_delete(prVdecEsInfoKeep->hUnderFlowSema)== OSR_OK);

    // delete message queue
    VERIFY(x_msg_q_delete(prVdecEsInfoKeep->hMsgQ)== OSR_OK);

    VERIFY(x_sema_delete(prVdecEsInfoKeep->hMutex) == OSR_OK);

    VERIFY (x_sema_delete(prVdecEsInfoKeep->hFlushSema) == OSR_OK);

    VERIFY(x_sema_delete(prVdecEsInfoKeep->hFlushMutex) == OSR_OK);

    VERIFY(x_sema_delete(prVdecEsInfoKeep->hCcSema) == OSR_OK);
    VERIFY(x_sema_delete(prVdecEsInfoKeep->hVdecSetPriSema) == OSR_OK);

    // delete command queue
    VERIFY(x_msg_q_delete(prVdecEsInfoKeep->hCmdQ)== OSR_OK);
    VERIFY(x_msg_q_delete(prVdecEsInfoKeep->hVdecScheMsg)== OSR_OK);

    VERIFY(x_timer_delete(prVdecEsInfoKeep->hDataTimer)== OSR_OK);
    #if 0
    VERIFY(x_timer_delete(prVdecEsInfoKeep->hThumbTimer)== OSR_OK);
    #endif

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

#ifdef TEST_ONOFF_RUNTIME
static BOOL _fgTestOnoff = TRUE;
#endif
void _VDEC_LockVld(UCHAR ucEsId, UCHAR ucVldId)
{
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);
    ASSERT(ucVldId < VDEC_MAX_VLD);
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "ucEsId >= VDEC_MAX_ES\n");
        return;
    }
    if(ucVldId >= VDEC_MAX_VLD)
    {
        LOG(1, "ucVldId >= VDEC_MAX_VLD\n");
        return;
    }

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if((!prVdecEsInfo) || (!prVdecInfo) || (!prVdecEsInfoKeep))
    {
        LOG(1, "ucEsId/ucVldId fail\n");
        return;
    }
    UNUSED(prVdecEsInfoKeep);

    ASSERT(prVdecEsInfo->u4VldId == ucVldId);

    VERIFY(x_sema_lock(prVdecInfo->arVdecVldInfo[ucVldId].hVldSemaphore, X_SEMA_OPTION_WAIT) == OSR_OK);

    if(prVdecInfo->arVdecVldInfo[ucVldId].u4LockCount == 0)
    {
        prVdecInfo->arVdecVldInfo[ucVldId].ucLockEsId = ucEsId;
#ifdef TEST_ONOFF_RUNTIME
        prVdecInfo->arVdecVldInfo[ucVldId].fgSwrst = FALSE;
        #ifdef CC_DYNAMIC_POWER_ONOFF  //power off
        if((ucVldId == VLD0) && (_fgTestOnoff))
        {
#ifdef VDEC_IS_POST_MT5368
            //VDEC_CkgenOnOff(TRUE); => this seems cause MJC NG
            IO_WRITE32(VDEC_MISC_BASE, VDEC_PDN_CON_SPEC, VDEC_PDN_CON_OFF);
            IO_WRITE32(VDEC_MISC_BASE, VDEC_PDN_CON_MODULE, VDEC_PDN_CON_OFF);
#else
            IO_WRITE32(VDEC_CTRL_BASE, VDEC_PWR_CTRL, VDEC_PWR_ON);
#endif
        }
        #endif
#endif
    }
    else
    {
        if(prVdecInfo->arVdecVldInfo[ucVldId].ucLockEsId != ucEsId)
        {
            LOG(1, "ucLockEsId(%d) should be ucEsId(%d) ===========\n",
                    (UINT32)prVdecInfo->arVdecVldInfo[ucVldId].ucLockEsId,
                    (UINT32)ucEsId);
            ASSERT(0);
        }
    }
    prVdecInfo->arVdecVldInfo[ucVldId].u4LockCount++;
    if(prVdecInfo->arVdecVldInfo[ucVldId].u4LockCount > 2)
    {
        // Max is 2
        // One is in decoder.
        // One is in swdmx_mpg header parse.
        LOG(1, "u4LockCount(%d) should not happen ===========\n",
                prVdecInfo->arVdecVldInfo[ucVldId].u4LockCount);
        LOG(1, "Please review your vdec lock/unlock code\n");
        UNUSED(prVdecInfo->arVdecVldInfo[ucVldId].u4LockCount);
        ASSERT(0);
    }

    _VDEC_SetIsr(ucEsId);
    UNUSED(prVdecEsInfo);
}

void _VDEC_UnlockVld(UCHAR ucEsId, UCHAR ucVldId)
{
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;

    ASSERT(ucEsId < VDEC_MAX_ES);
    ASSERT(ucVldId < VDEC_MAX_VLD);
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "ucEsId >= VDEC_MAX_ES\n");
        return;
    }
    if(ucVldId >= VDEC_MAX_VLD)
    {
        LOG(1, "ucVldId >= VDEC_MAX_VLD\n");
        return;
    }

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if((!prVdecEsInfo) || (!prVdecInfo))
    {
        LOG(1, "ucEsId/ucVldId fail\n");
        return;
    }

    ASSERT(prVdecEsInfo->u4VldId == ucVldId);

    if(prVdecInfo->arVdecVldInfo[ucVldId].u4LockCount == 0)
    {
        LOG(1, "u4LockCount(0) should never happen\n");
        ASSERT(0);
    }
    if(prVdecInfo->arVdecVldInfo[ucVldId].ucLockEsId != ucEsId)
    {
        LOG(1, "ucLockEsId(%d) should be ucEsId(%d) ===========\n",
                (UINT32)prVdecInfo->arVdecVldInfo[ucVldId].ucLockEsId,
                (UINT32)ucEsId);
        ASSERT(0);
    }

    prVdecInfo->arVdecVldInfo[ucVldId].u4LockCount--;
    if(prVdecInfo->arVdecVldInfo[ucVldId].u4LockCount == 0)
    {
        prVdecInfo->arVdecVldInfo[ucVldId].ucLockEsId = 0xFF;
#ifdef TEST_ONOFF_RUNTIME
        #ifdef CC_DYNAMIC_POWER_ONOFF  //power off
        if((ucVldId == VLD0) && (_fgTestOnoff))
        {
#ifdef VDEC_IS_POST_MT5368
            //VDEC_CkgenOnOff(FALSE);
            IO_WRITE32(VDEC_MISC_BASE, VDEC_PDN_CON_SPEC, VDEC_PDN_CON_OFF);
            IO_WRITE32(VDEC_MISC_BASE, VDEC_PDN_CON_MODULE, VDEC_PDN_CON_OFF);
#else
            IO_WRITE32(VDEC_CTRL_BASE, VDEC_PWR_CTRL, VDEC_PWR_OFF);
#endif
        }
        #endif
#endif
    }
    VERIFY(x_sema_unlock(prVdecInfo->arVdecVldInfo[ucVldId].hVldSemaphore) == OSR_OK);

    UNUSED(prVdecEsInfo);
}


VOID _VDEC_CheckLockStatus(UCHAR ucVldId)
{
#ifdef TEST_ONOFF_RUNTIME
    VDEC_INFO_T *prVdecInfo;
    prVdecInfo = _VDEC_GetInfo();
    if(prVdecInfo->arVdecVldInfo[ucVldId].u4LockCount == 0)
    {
        LOG(0, "FixMe, can't access VLD/MC register before lock vld\n");
        ASSERT(0);
    }

    if(!prVdecInfo->arVdecVldInfo[ucVldId].fgSwrst)
    {
        LOG(0, "FIXME: can't access VDEC register before swrst\n");
        ASSERT(0);
    }
#endif
    UNUSED(ucVldId);
}


BOOL _VDEC_CkgenSel(UCHAR ucEsId, ENUM_VDEC_FMT_T eCurFMT, UINT32 u4App)
{
    UNUSED(ucEsId);
    UNUSED(u4App);

#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    switch (eCurFMT)
    {
    case VDEC_FMT_H264:
    case VDEC_FMT_H264VER:
        IO_WRITE32(CKGEN_BASE, VDEC_SYS_CKCFG, VDEC_SYS_CK_SEL_FAST);
        break;

    case VDEC_FMT_MPV:
    case VDEC_FMT_MP4:
    case VDEC_FMT_WMV:
    case VDEC_FMT_VP8:
        IO_WRITE32(CKGEN_BASE, VDEC_SYS_CKCFG, VDEC_SYS_CK_SEL_MID);
        break;

    case VDEC_FMT_RV:
    case VDEC_FMT_AVS:
    case VDEC_FMT_VP6:
        IO_WRITE32(CKGEN_BASE, VDEC_SYS_CKCFG, VDEC_SYS_CK_SEL_SLOW);
        break;

    case VDEC_FMT_MJPEG:
    case VDEC_FMT_RAW:
        break;

    default:
        LOG(1, "%s: unknown format %d\n", __FUNCTION__, eCurFMT);
        ASSERT(0);
        break;
    }
#else
    UNUSED(eCurFMT);
#endif

    return TRUE;
}

BOOL _VDEC_SysClkSel(UCHAR ucEsId, ENUM_VDEC_FMT_T eCurFMT, UINT32 u4App)
{
    UNUSED(ucEsId);
    UNUSED(u4App);

#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    switch (eCurFMT)
    {
    case VDEC_FMT_H264:
    case VDEC_FMT_H264VER:
        IO_WRITE32(VDEC_MISC_BASE, VDEC_SYS_CLK_SEL, VDEC_SYS_FAST_CLK);
        break;

    case VDEC_FMT_MPV:
    case VDEC_FMT_MP4:
    case VDEC_FMT_WMV:
    case VDEC_FMT_VP8:
        IO_WRITE32(VDEC_MISC_BASE, VDEC_SYS_CLK_SEL, VDEC_SYS_MID_CLK);
        break;

    case VDEC_FMT_RV:
    case VDEC_FMT_AVS:
    case VDEC_FMT_VP6:
        IO_WRITE32(VDEC_MISC_BASE, VDEC_SYS_CLK_SEL, VDEC_SYS_SLOW_CLK);
        break;

    case VDEC_FMT_MJPEG:
    case VDEC_FMT_RAW:
        break;

    default:
        LOG(1, "%s: unknown format %d\n", __FUNCTION__, eCurFMT);
        ASSERT(0);
        break;
    }
#else
    UNUSED(eCurFMT);
#endif

    return TRUE;
}


void _VDEC_ClearEsInfo(UCHAR ucEsId)
{
    CRIT_STATE_T _rCritState;
    VDEC_PES_INFO_T *prVdecPesInfo;
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    //VDEC_INFO_T *prVdecInfo = (VDEC_INFO_T *)&_rVdecInfo;

    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "ucEsId >= VDEC_MAX_ES\n");
        return;
    }
    if((!prVdecEsInfo) || (!prVdecEsInfoKeep))
    {
        LOG(1, "prVdecEsInfo/prVdecEsInfoKeep null\n");
        return;
    }

    x_memset((void*)prVdecEsInfo, 0, sizeof(VDEC_ES_INFO_T));

    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));

    // if other format want to use prMpvCounter
    // please move it out of this score.
    prVdecEsInfo->prMpvCounter = (PARAM_MPV_T*)&(_prDbase->rMpv[ucEsId]);
    x_memset((void*)prVdecEsInfo->prMpvCounter, 0, sizeof(PARAM_MPV_T));
    x_memset((void*)&prVdecEsInfoKeep->rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    prVdecPesInfo = _arPesInfoEsm;
    prVdecPesInfo += (UINT32)ucEsId;
    x_memset((void*)prVdecPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    prVdecEsInfo->ucFbgId = FBM_FBG_ID_UNKNOWN;
	prVdecEsInfo->ucSubFbgId = FBM_FB_ID_UNKNOWN;
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

    // Reset CROP mode, default is in CROP mode
    prVdecEsInfo->fgEnableCrop = TRUE;
    // reset source state to clear
    prVdecEsInfo->ePerSourceState = VDEC_SCRAMBLE_STATE_CLEAR;
    prVdecEsInfo->eCurSourceState = VDEC_SCRAMBLE_STATE_CLEAR;
    UNUSED(DMX_RegSourceStateCbForVdec(ucEsId, NULL));
    
#ifdef VDEC_TIME_PROFILE
{
    UINT8 i = 0;

    for ( i=0; i<5; i++ )
    {
        prVdecEsInfo->au4DecTime[i][0] =  0x0;
        prVdecEsInfo->au4DecTime[i][1] =  0xFFFFFFFF;
        prVdecEsInfo->au4DecTime[i][2] =  0x0;
        prVdecEsInfo->au4DecTime[i][3] =  0x0;
        prVdecEsInfo->au4DecTime[i][4] =  0x0;
        prVdecEsInfo->au4DecTime[i][5] =  0x0;
        prVdecEsInfo->au4DecTime[i][6] =  0x0;
        prVdecEsInfo->au4DecTime[i][7] =  0x0;
        prVdecEsInfo->au4DecTime[i][8] =  0x0;

        prVdecEsInfo->au4DecTimeMaxPicSize[i][0] = 0;
    }

    prVdecEsInfo->u4FrmFldCnt = 0;
    prVdecEsInfo->fgCalEachPicSize = TRUE;
    prVdecEsInfo->u4CurrPicSize = 0;
    prVdecEsInfo->u4StatisticPicSize = 0;
}
#endif

    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV)
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

    //VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "ucEsId >= VDEC_MAX_ES\n");
        return;
    }

    //prVdecInfo = _VDEC_GetInfo();
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

    while (x_msg_q_receive(&u2MsgQIdx, &rPesInfo, &zMsgSize,
        &(prVdecEsInfoKeep->hMsgQSub), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    // DTV00136850 Video display discontinuously after  plug supported CAM module at TV in Menu
    // video frame are not correct release.
    if(prVdecEsInfo->fgMMPlayback)
    {
        if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
        {
            //flush dpb
            #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
            #ifndef CC_VDEC_H264_DISABLE
            _VDEC_H264Flush(ucEsId, FALSE);
            if(prVdecEsInfoKeep->fgVPush)
            {
                SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
                SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
            }
            #endif
            #endif
        }
        else if (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_AVS)
        {
#ifndef CC_VDEC_AVS_DISABLE
            _VDEC_AVSFlush(ucEsId, TRUE);
#endif
        }
        else if (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_VP6)
        {
#ifndef CC_VDEC_VP6_DISABLE
            _VDEC_VP6Flush(ucEsId, TRUE);
#endif
        }
        else if(prVdecEsInfoKeep->eCurFMT==VDEC_FMT_VP8)
        {
           prVdecEsInfoKeep->pfnVdecFlush(ucEsId,TRUE);
        }
        else
        {
            #if 1
            UCHAR ucFRefFbId, ucBRefFbId;
            FBM_GetRefFrameBuffer(prVdecEsInfo->ucFbgId, &ucFRefFbId, &ucBRefFbId);
            if(ucBRefFbId != FBM_FB_ID_UNKNOWN)
            {
                if((FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_READY))
                   ||
                   (FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_DECODE)))
                {
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_EMPTY);
                    LOG(3, "Flush ucBRefFbId %d -> empty\n", ucBRefFbId);
                }
            }
            if(ucFRefFbId != FBM_FB_ID_UNKNOWN)
            {
                if((FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_READY))
                   ||
                   (FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_DECODE)))
                {
                    FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_EMPTY);
                    LOG(3, "Flush ucFRefFbId %d -> empty\n", ucFRefFbId);
                }
            }
            #else
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
            #endif
            
            // because we invalidate reference frame, we should restart to get a sync.
            prVdecEsInfo->fgDetectIntra = TRUE;
            if(prVdecEsInfo->prMpvCounter)
            {
                prVdecEsInfo->prMpvCounter->u4RetrieveNs = 0;
            }

            SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        }
    }
    else
    {
        // DTV case
        if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV)
        {
            SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
            if(prVdecEsInfo->u4VFifoPrevWptr)
            {
                prVdecEsInfo->u4VldReadPtr = prVdecEsInfo->u4VFifoPrevWptr;
                prVdecEsInfo->u4DecReadPtr = prVdecEsInfo->u4VldReadPtr;
                if(prVdecEsInfoKeep->pfnVdecUpdRptr)
                {
                    prVdecEsInfoKeep->pfnVdecUpdRptr(ucEsId);
                }
            }
        }
    }

    prVdecEsInfo->fgKeepPes = FALSE;
    prVdecEsInfo->fgSendedEos = FALSE;
    prVdecEsInfo->fgSetStartPts = FALSE;
    prVdecEsInfo->u4PTSLast = 0;
    prVdecEsInfo->u4PTSLastNoneZero= 0;
    prVdecEsInfo->u8OffsetLast = 0;
    prVdecEsInfo->u4DisplayQPicCnt = 0;
    prVdecEsInfo->u4FirstDispPTS = 0;
    prVdecEsInfoKeep->rPesInfo.fgEos = FALSE;

#ifdef __MODEL_slt__
    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV)
    {
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    }
#endif

    VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
    // one msg come from VERIFY(VDEC_SendEs(&_arMpvPesInfo[ucEsId]) == 1) in MPV_FlushEsmQ.
    if(prVdecEsInfo->u2Count == 1)
    {
        LOG(3, "After Flush u2Count = 1\n");
    }
    else
    {
        LOG(5, "After Flush u2Count = %d\n", prVdecEsInfo->u2Count);
        ASSERT(prVdecEsInfo->u2Count == 0);
    }
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
                    rPesInfo.u4FifoEnd = 0x3ffffffe;
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
                                    if(!prVdecInfo->fgSave)
                                    {
                                        break;
                                    }
                                }
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
                        UNUSED(u4OnRepeatCount);
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
    BOOL fgGetIframeTime = FALSE;
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
    PARAM_MPV_T *prMpvCounter;

    HANDLE_T hMsgQ;
    #if 0
    UINT16 u2CountMain, u2CountSub;
    VDEC_PES_INFO_T rPesInfo;
    VDEC_ES_INFO_T* prVdecEsInfoMain;
    #endif
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeepMain;

    ASSERT(prPesInfo != NULL);
    ucMpvId = prPesInfo->ucMpvId;
    ucEsId = prPesInfo->ucEsId;

    ASSERT(ucEsId < VDEC_MAX_ES);

    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "ucEsId >= VDEC_MAX_ES\n");
        return 1;
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecInfo = &_rVdecInfo;
    prMpvCounter = (PARAM_MPV_T *)prVdecEsInfo->prMpvCounter;

    #if 0
    prVdecEsInfoMain = _VDEC_GetEsInfo(MVC_MAIN_VIDEO);
    #endif
    prVdecEsInfoKeepMain = _VDEC_GetEsInfoKeep(MVC_MAIN_VIDEO);

    if(prPesInfo->u4PTS != 0)
    {
        prVdecEsInfo->u4LatestPTS = prPesInfo->u4PTS;
    }

    // Virtual Picture Header for Event Trigger
    if (ucMpvId >= VDEC_MAX_VLD)
    {
        VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hMsgQ, &(prVdecEsInfo->u2Count))
            == OSR_OK);

        //if (prEsmQ->u2Count <= prEsmQ->u2UdfNs)
        {
            i4Ret = x_msg_q_send(prVdecEsInfoKeep->hMsgQ, (void *)prPesInfo,
                sizeof(VDEC_PES_INFO_T), 254);

            if(((prVdecEsInfo->e3DType == VDEC_3D_MVC) ||
                (prVdecEsInfo->e3DType == VDEC_3D_REALD_DUAL_STREAM))
                && (ucEsId == 0))
            {
                i4Ret |= x_msg_q_send(prVdecEsInfoKeepMain->hMsgQSub, (void *)prPesInfo,
                    sizeof(VDEC_PES_INFO_T), 254);
            }
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

    //redirect to single ESM for MVC
    if((prVdecEsInfo->e3DType == VDEC_3D_MVC) ||
        (prVdecEsInfo->e3DType == VDEC_3D_REALD_DUAL_STREAM))
    {
        if(ucEsId > 0)  // es case will not have EsId > 0, Sub Q is useless
        {
            hMsgQ = prVdecEsInfoKeepMain->hMsgQSub;
        }
        else
        {
            hMsgQ = prVdecEsInfoKeep->hMsgQ;
        }
        //error handling
        #if 0
        VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeepMain->hMsgQ, &(u2CountMain)) == OSR_OK);
        VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeepMain->hMsgQSub, &(u2CountSub)) == OSR_OK);
        x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

        if((u2CountMain == 0) && (u2CountSub > 30) && (!prVdecEsInfoMain->fgIsDependView))
        {
            prVdecEsInfoMain->fgIsDependView = TRUE;
            LOG(3, "Err, force MVC depend view\n");
            i4Ret = x_msg_q_send(prVdecEsInfoKeepMain->hMsgQ, (void *)&rPesInfo,
                    sizeof(VDEC_PES_INFO_T), 254);
        }
        else if((u2CountSub == 0) && (u2CountMain > 30) && (prVdecEsInfoMain->fgIsDependView))
        {
            prVdecEsInfoMain->fgIsDependView = FALSE;
            LOG(3, "Err, force MVC base view\n");
            i4Ret = x_msg_q_send(prVdecEsInfoKeepMain->hMsgQSub, (void *)&rPesInfo,
                    sizeof(VDEC_PES_INFO_T), 254);
        }
        #endif
    }
    else
    {
        hMsgQ = prVdecEsInfoKeep->hMsgQ;
    }

    if(prVdecEsInfo->fgThumbMethod2)
    {
        if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
        {
            if((prPesInfo->ucPicType & 0xF) == 0x5) // IDR_NALU
            {
                fgGetIframeTime = TRUE;
            }
        }
        else if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MPV)
        {
            if(prVdecEsInfo->ucPicType == I_TYPE)
            {
                fgGetIframeTime = TRUE;
            }
        }
        else
        {
            fgGetIframeTime = TRUE;
        }
        // error handle
        if(prMpvCounter)
        {
            if((prMpvCounter->u4RetrieveNs - prVdecEsInfo->u4ThumbRetrieveNs) > 200)
            {
                fgGetIframeTime = TRUE;
            }
        }
        if(fgGetIframeTime)
        {
            if(!prVdecEsInfo->fgThumbStartTime)
            {
                HAL_GetTime(&prVdecEsInfo->rThumbStartTime);
                LOG(5, "Es(%d) Thumbnail First I Pic. Time(%d.%06d)\n", ucEsId,
                    prVdecEsInfo->rThumbStartTime.u4Seconds,
                    prVdecEsInfo->rThumbStartTime.u4Micros);
                prVdecEsInfo->fgThumbStartTime = TRUE;
            }
        }
    }

    // [LOG] Parser send message into ESM Q
    SYSLOG(ucEsId, 1, prVdecEsInfo->u2Count, prPesInfo->u4PTS);
    //SYSLOG(ucEsmQId, 1, prPesInfo->u4PTS, prPesInfo->u4DTS);

#ifdef MPV_VIRTUAL_PRS
    VERIFY(x_msg_q_send(hMsgQ, (void *)prPesInfo,
        sizeof(VDEC_PES_INFO_T), 255) == OSR_OK);
    VERIFY(x_msg_q_num_msgs(hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
#else
    if(prVdecEsInfoKeep->pfnVdecWaitRandomAP)
    {
        if(prVdecEsInfoKeep->pfnVdecWaitRandomAP((void *)prPesInfo))
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
            VERIFY(x_msg_q_num_msgs(hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
            if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
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

                    if(prVdecEsInfoKeep->pfnVdecHandleOverFlw)
                    {
                        LOG(5, "fgMMPlayback MM pfnVdecHandleOverFlw\n");
                        ucRet =  prVdecEsInfoKeep->pfnVdecHandleOverFlw(ucMpvId, ucEsId,
                            prVdecEsInfoKeep->hMutex, hMsgQ, (void*)prPesInfo);
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
        i4Ret = x_msg_q_send(hMsgQ, (void *)prPesInfo,
            sizeof(VDEC_PES_INFO_T), 255);
        if (i4Ret != OSR_OK)
        {
            UCHAR ucRet = 0;

            if(prVdecEsInfoKeep->pfnVdecHandleOverFlw)
            {
                ucRet =  prVdecEsInfoKeep->pfnVdecHandleOverFlw(ucMpvId, ucEsId,
                    prVdecEsInfoKeep->hMutex, hMsgQ, (void*)&_arPesInfoEsm[ucEsId]);
                x_memcpy(&_arPesInfoEsm[ucEsId], prPesInfo, sizeof(VDEC_PES_INFO_T));
            }
            return ucRet;
        }
    }
    else if((!prVdecEsInfo->fgKeepPes) && (prVdecEsInfoKeep->eCurState != VDEC_ST_PLAY_I_FRAME))
    {
        if (prVdecEsInfo->fgMMPlayback && prPesInfo->fgEos)
        {
            // we don't get any frame before Eos
            x_memcpy((void *)&_arPesInfoEsm[ucEsId], (void *)prPesInfo, sizeof(VDEC_PES_INFO_T));
            _arPesInfoEsm[ucEsId].ucMpvId = VDEC_MAX_VLD;
            i4Ret = x_msg_q_send(hMsgQ, (void *)&_arPesInfoEsm[ucEsId],
                sizeof(VDEC_PES_INFO_T), 255);
        }
        else
        {
            x_memcpy(&_arPesInfoEsm[ucEsId], prPesInfo, sizeof(VDEC_PES_INFO_T));
            prVdecEsInfo->fgKeepPes = TRUE;
        }
    }
    else
    {
        i4Ret = OSR_OK;
        if(prVdecEsInfoKeep->eCurState != VDEC_ST_PLAY_I_FRAME)
        {
            if (prVdecEsInfo->fgMMPlayback && _arPesInfoEsm[ucEsId].fgSeqEnd)
            {
                if(prPesInfo->fgEos)
                {
                    // we don't get any frame before Eos
                    x_memcpy((void *)&_arPesInfoEsm[ucEsId], (void *)prPesInfo, sizeof(VDEC_PES_INFO_T));
                    _arPesInfoEsm[ucEsId].ucMpvId = VDEC_MAX_VLD;
                    i4Ret = x_msg_q_send(hMsgQ, (void *)&_arPesInfoEsm[ucEsId],
                        sizeof(VDEC_PES_INFO_T), 255);
                }
                else
                {
                    x_memcpy(&_arPesInfoEsm[ucEsId], prPesInfo, sizeof(VDEC_PES_INFO_T));
                    prVdecEsInfo->fgKeepPes = TRUE;
                }
            }
            else
            {
                if(prPesInfo->fgEos)
                {
                    LOG(5, "EOS in PesInfo\n");
                    _arPesInfoEsm[ucEsId].fgEos = TRUE;
                }
                if(prPesInfo->fgBackward)
                {
                    LOG(5, "fgBackward in PesInfo\n");
                    _arPesInfoEsm[ucEsId].fgBackward = TRUE;
                }

                //DTV00139473
               if(prPesInfo->fgSeqEnd)
               {
                   LOG(5, "fgSeqEnd in PesInfo\n");
		            _arPesInfoEsm[ucEsId].fgSeqEnd = TRUE;
               }

                if(prPesInfo->u8Offset > _arPesInfoEsm[ucEsId].u8Offset)
                {
                    if((prPesInfo->u8Offset - _arPesInfoEsm[ucEsId].u8Offset) <
                       (1024*1024))
                    {
                        prVdecEsInfo->u4AvgPicSize =
                            ((prVdecEsInfo->u4AvgPicSize*
                            (VDEC_AVG_PIC_COUNT-1))/VDEC_AVG_PIC_COUNT);
                        prVdecEsInfo->u4AvgPicSize +=
                            ((UINT32)(prPesInfo->u8Offset - _arPesInfoEsm[ucEsId].u8Offset)/
                            VDEC_AVG_PIC_COUNT);
                    }
                }

                // picture end position
                _arPesInfoEsm[ucEsId].u4VldReadPtrEnd = prPesInfo->u4VldReadPtr;

                //LOG(1, "_arPesInfoEsm[ucEsId].u4VldReadPtr 0x%x\n", _arPesInfoEsm[ucEsId].u4VldReadPtr);
                i4Ret = x_msg_q_send(hMsgQ, (void *)&_arPesInfoEsm[ucEsId],
                    sizeof(VDEC_PES_INFO_T), 255);
                if(i4Ret == OSR_OK)
                {
                    x_memcpy(&_arPesInfoEsm[ucEsId], prPesInfo, sizeof(VDEC_PES_INFO_T));
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
            i4Ret = x_msg_q_send(hMsgQ, (void *)prPesInfo,
                sizeof(VDEC_PES_INFO_T), 255);
        }
        if (i4Ret != OSR_OK)
        {
            UCHAR ucRet = 0;

            if(prVdecEsInfoKeep->pfnVdecHandleOverFlw)
            {
                ucRet =  prVdecEsInfoKeep->pfnVdecHandleOverFlw(ucMpvId, ucEsId,
                    prVdecEsInfoKeep->hMutex, hMsgQ, (void*)&_arPesInfoEsm[ucEsId]);
                x_memcpy(&_arPesInfoEsm[ucEsId], prPesInfo, sizeof(VDEC_PES_INFO_T));
            }
            return ucRet;
        }
    }
    VERIFY(x_msg_q_num_msgs(hMsgQ, &(prVdecEsInfo->u2Count)) == OSR_OK);
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
    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_H264)
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
            (prVdecEsInfoKeep->eCurState == VDEC_ST_PLAY_I_FRAME) ||
            (prPesInfo->fgNonBuffer) ||
            (_arPesInfoEsm[ucEsId].fgEos) ||
            (prVdecEsInfoKeep->fgVPush))
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

        #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4Addr), (50*1024*1024));
        #else
        HalFlushInvalidateDCache();
        #endif

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



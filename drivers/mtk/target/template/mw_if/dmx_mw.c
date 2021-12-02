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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dmx_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_mw.c
 *  Demux driver - middleware interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mw.h"
#include "dmx_if.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "mpv_if.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "fbm_drvif.h"
#include "drv_comp_id.h"
#include "stc_drvif.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_demux.h"
#include "x_rm_dev_types.h"
#include "x_os.h"

#define DEFINE_IS_LOG	DMX_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define DMX_VIDEO_ES_FIFO_SIZE              0x300000        // 3MB
#define DMX_DEFAULT_ES_FIFO_SIZE            0x20000         // 128K
#define DMX_ES_FIFO_ALIGNMENT               32


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// PCR states
typedef struct
{
    UINT16 u2Pid;           // PID value
    UINT8 u1PidIndex;       // The redirect PID index
    BOOL fgPcr;             // Enable PCR or not
} DMX_PCR_STATE_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------

/// Demux PSI part initialization
EXTERN BOOL _MW_DMX_PSI_Init(void);

/// PES part initialization
EXTERN BOOL _MW_DMX_PES_Init(void);

/// Translate MPV component ID to channel ID
EXTERN UCHAR MW_MPV_CompIdtoVldId(UINT16 u2CompId, UINT32 u4ChannelId);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// PCR states
static DMX_PCR_STATE_T _arPcr[DMX_NUM_PID_INDEX];

/// Notification callback functions
static DEMUX_NFY_INFO_T _arNotifyInfo[DMX_NUM_PID_INDEX];

/// Notification callback of scramble state
static DEMUX_NFY_INFO_T _arScrambleState[DMX_NUM_PID_INDEX];

/// Buffer address for scrambling state detection
static UINT32 _u4DefaultBufAddr = 0;

/// Reference counter of buffer of scrambling state detection
static UINT32 _u4DefaultBufRefCount = 0;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _MwDmxGetDefaultEsBuffer
 *  Get default ES buffer. The ES buffer will be allocated if it's not existed
 *
 *  @retval                 The ES buffer address, or 0 if out of memory
 */
//-----------------------------------------------------------------------------
static UINT32 _MwDmxGetDefaultEsBuffer(void)
{
    CRIT_STATE_T rState;
    UINT32 u4DefBufAddr = 0;

    rState = x_crit_start();

    if (_u4DefaultBufRefCount == 0)
    {
        ASSERT(_u4DefaultBufAddr == 0);

        x_crit_end(rState);

        u4DefBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(
            DMX_DEFAULT_ES_FIFO_SIZE, DMX_ES_FIFO_ALIGNMENT);

        rState = x_crit_start();

        _u4DefaultBufAddr = u4DefBufAddr;
    }

    if (_u4DefaultBufAddr != 0)
    {
        _u4DefaultBufRefCount++;
    }

    u4DefBufAddr = _u4DefaultBufAddr;

    x_crit_end(rState);

    return u4DefBufAddr;
}


//-----------------------------------------------------------------------------
/** _MwDmxReleaseDefaultEsBuffer
 *  Release default ES buffer. The ES buffer will be freed automatically on
 *  releasing the last one
 *
 *  @param  u4Addr          The buffer address
 *
 *  @retval -
 */
//-----------------------------------------------------------------------------
static void _MwDmxReleaseDefaultEsBuffer(UINT32 u4Addr)
{
    CRIT_STATE_T rState;

    rState = x_crit_start();

    ASSERT(_u4DefaultBufRefCount > 0);
    _u4DefaultBufRefCount--;

    if (_u4DefaultBufRefCount == 0)
    {
        x_crit_end(rState);

        VERIFY(BSP_FreeAlignedDmaMemory((void*)_u4DefaultBufAddr));

        rState = x_crit_start();

        _u4DefaultBufAddr = 0;
    }

    x_crit_end(rState);

    UNUSED(u4Addr);
}


//-----------------------------------------------------------------------------
/** _MwDmxOnPsiNotify
 *  PSI notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxOnPsiNotify(UINT8 u1Pidx, UINT32 u4Data, const void* pvTag)
{
    DMX_NOTIFY_INFO_PSI_T* prNotifyInfo;
    UINT32 u4Data1, u4SecAddr;
    UINT8 u1SerialNumber;
    BOOL fgKeepData;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    if (_arNotifyInfo[u1Pidx].pf_demux_nfy == NULL)
    {
        LOG(3, "Pidx %u: No PSI handler registered\n", u1Pidx);
        return FALSE;
    }

    prNotifyInfo = (DMX_NOTIFY_INFO_PSI_T*)u4Data;
    ASSERT(prNotifyInfo != NULL);

    u4SecAddr = VIRTUAL(prNotifyInfo->u4SecAddr);
    ASSERT(u4SecAddr < 0x10000000);
    u1SerialNumber = prNotifyInfo->u1SerialNumber;
    ASSERT(u1SerialNumber <= DMX_MAX_SERIAL_NUM);
    u4Data1 = (u1SerialNumber << 28) | u4SecAddr;

    fgKeepData = _arNotifyInfo[u1Pidx].pf_demux_nfy(
        _arNotifyInfo[u1Pidx].pv_tag, DEMUX_COND_DATA_REC,
        u4Data1, prNotifyInfo->u4SecLen);

    UNUSED(pvTag);

    return fgKeepData;
}


//-----------------------------------------------------------------------------
/** _MwDmxOnPesNotify
 *  PES notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxOnPesNotify(UINT8 u1Pidx, UINT32 u4Data, const void* pvTag)
{
    DMX_NOTIFY_INFO_PES_T* prNotifyInfo;
    UINT32 u4DataAddr, u4Data1;
    UINT8 u1SerialNumber;
    BOOL fgKeepData;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    if (_arNotifyInfo[u1Pidx].pf_demux_nfy == NULL)
    {
        LOG(3, "Pidx %u: No PES handler registered\n", u1Pidx);
        return FALSE;
    }

    prNotifyInfo = (DMX_NOTIFY_INFO_PES_T*)u4Data;
    ASSERT(prNotifyInfo != NULL);

    u4DataAddr = VIRTUAL(prNotifyInfo->u4DataAddr);
    ASSERT(u4DataAddr < 0x10000000);
    u1SerialNumber = prNotifyInfo->u1SerialNumber;
    ASSERT(u1SerialNumber <= DMX_MAX_SERIAL_NUM);
    u4Data1 = (u1SerialNumber << 28) | u4DataAddr;

    fgKeepData = _arNotifyInfo[u1Pidx].pf_demux_nfy(
        _arNotifyInfo[u1Pidx].pv_tag, DEMUX_COND_DATA_REC,
        u4Data1, prNotifyInfo->u4DataSize);

    UNUSED(pvTag);

    return fgKeepData;
}


//-----------------------------------------------------------------------------
/** _MwDmxOnEsNotify
 *  ES notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxOnEsNotify(UINT8 u1Pidx, UINT32 u4Data, const void* pvTag)
{
    UNUSED(u1Pidx);
    UNUSED(u4Data);
    UNUSED(pvTag);

    // To be defined
    return FALSE;
}


//-----------------------------------------------------------------------------
/** _MwDmxOnOverflowNotify
 *  PES notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxOnOverflowNotify(UINT8 u1Pidx, UINT32 u4Data,
    const void* pvTag)
{
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    if (_arNotifyInfo[u1Pidx].pf_demux_nfy == NULL)
    {
        LOG(3, "Pidx %u: No handler registered\n", u1Pidx);
        return FALSE;
    }

    UNUSED(_arNotifyInfo[u1Pidx].pf_demux_nfy(
        _arNotifyInfo[u1Pidx].pv_tag, DEMUX_COND_OVERFLOW, 0, 0));

    UNUSED(u4Data);
    UNUSED(pvTag);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwDmxOnScrambleStateNotify
 *  Scramble state notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxOnScrambleStateNotify(UINT8 u1Pidx, UINT32 u4Data,
    const void* pvTag)
{
    DMX_SCRAMBLE_STATE_T eState;
    DEMUX_SCRS_T eMwState;
    BOOL fgRet;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    if (_arScrambleState[u1Pidx].pf_demux_nfy == NULL)
    {
        LOG(3, "Pidx %u: No scrabmle state handler registered\n", u1Pidx);
        return FALSE;
    }

    eState = (DMX_SCRAMBLE_STATE_T)u4Data;
    switch (eState)
    {
    case DMX_SCRAMBLE_STATE_CLEAR:
        eMwState = DEMUX_SCRS_CLEAR;
        break;

    case DMX_SCRAMBLE_STATE_SCRAMBLED:
        eMwState = DEMUX_SCRS_SCRAMBLED;
        break;

    default:
        eMwState = DEMUX_SCRS_UNKNOWN;
        break;
    }

    fgRet = _arScrambleState[u1Pidx].pf_demux_nfy(
        _arScrambleState[u1Pidx].pv_tag, DEMUX_COND_SCRS, (UINT32)eMwState,
        0);

    // The middleware guarantees that it will always return FALSE
    VERIFY(!fgRet);

    UNUSED(pvTag);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwDmxScrambleStateHandler
 *  Scramble-state handler
 *
 *  @param  u1Pidx          PID index
 *  @param  eCode           Notification code
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxScrambleStateHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvTag)
{
    BOOL fgRet = FALSE;

    switch (eCode)
    {
    case DMX_NOTIFY_CODE_SCRAMBLE_STATE:
        fgRet = _MwDmxOnScrambleStateNotify(u1Pidx, u4Data, pvTag);
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


//-----------------------------------------------------------------------------
/** _MwDmxSetScramblerCallback
 *  Set callback function for scramble state change
 *
 *  @param  u1Pidx          PID index
 *  @param  prNfyInfo       The callback function info
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxSetScrambleCallback(UINT8 u1Pidx,
    const DEMUX_NFY_INFO_T* prNfyInfo)
{
    DMX_PID_T rPid;

    if ((u1Pidx >= DMX_NUM_PID_INDEX) || (prNfyInfo == NULL))
    {
        return FALSE;
    }

    // Set PSI callback function to our handler
    rPid.pfnScramble = _MwDmxScrambleStateHandler;
    rPid.pvScrambleTag = NULL;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_SCRAMBLE_STATE, &rPid))
    {
        return FALSE;
    }

    _arScrambleState[u1Pidx].pf_demux_nfy = prNfyInfo->pf_demux_nfy;
    _arScrambleState[u1Pidx].pv_tag = prNfyInfo->pv_tag;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwDmxResetScrambleCallback
 *  Reset scramble callback handler
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxResetScrambleCallback(UINT8 u1Pidx)
{
    DMX_PID_T rPid;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    rPid.pfnScramble = NULL;
    rPid.pvScrambleTag = NULL;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_SCRAMBLE_STATE, &rPid))
    {
        return FALSE;
    }

    _arScrambleState[u1Pidx].pf_demux_nfy = NULL;
    _arScrambleState[u1Pidx].pv_tag = NULL;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwDmxHandler
 *  Demux callback handler
 *
 *  @param  u1Pidx          PID index
 *  @param  eCode           Notification code
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvTag)
{
    BOOL fgRet = FALSE;

    switch (eCode)
    {
    case DMX_NOTIFY_CODE_PSI:
        fgRet = _MwDmxOnPsiNotify(u1Pidx, u4Data, pvTag);
        break;

    case DMX_NOTIFY_CODE_PES:
        fgRet = _MwDmxOnPesNotify(u1Pidx, u4Data, pvTag);
        break;

    case DMX_NOTIFY_CODE_ES:
        fgRet = _MwDmxOnEsNotify(u1Pidx, u4Data, pvTag);
        break;

    case DMX_NOTIFY_CODE_OVERFLOW:
        fgRet = _MwDmxOnOverflowNotify(u1Pidx, u4Data, pvTag);
        break;
/*
    case DMX_NOTIFY_CODE_SCRAMBLE_STATE:
        fgRet = _OnScrambleStateNotify(u1Pidx, u4Data, pvTag);
        break;
*/
    default:
        // Unknown notification code
        LOG(1, "Pidx %u: Unknown notification code %u\n", u1Pidx,
            (UINT32)eCode);
        ASSERT(0);
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _MX_DMX_SetCallback
 *  Set callback handler
 *
 *  @param  u1Pidx          PID index
 *  @param  prNfyInfo       Callback info
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _MW_DMX_SetCallback(UINT8 u1Pidx, const DEMUX_NFY_INFO_T* prNfyInfo)
{
    DMX_PID_T rPid;

    if ((u1Pidx >= DMX_NUM_PID_INDEX) || (prNfyInfo == NULL))
    {
        return FALSE;
    }

    // Set PSI callback function to our handler
    rPid.pfnNotify = _MwDmxHandler;
    rPid.pvNotifyTag = NULL;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_CALLBACK, &rPid))
    {
        return FALSE;
    }

    _arNotifyInfo[u1Pidx].pf_demux_nfy = prNfyInfo->pf_demux_nfy;
    _arNotifyInfo[u1Pidx].pv_tag = prNfyInfo->pv_tag;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MW_DMX_ResetCallback
 *  Reset callback handler
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _MW_DMX_ResetCallback(UINT8 u1Pidx)
{
    DMX_PID_T rPid;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_CALLBACK, &rPid))
    {
        return FALSE;
    }

    _arNotifyInfo[u1Pidx].pf_demux_nfy = NULL;
    _arNotifyInfo[u1Pidx].pv_tag = NULL;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwDmxGetStcId
 *  Get STC ID
 *
 *  @param  u1PcrCompId     PCR component ID
 *
 *  @retval The STC ID
 */
//-----------------------------------------------------------------------------
static UINT8 _MwDmxGetStcId(UINT8 u1PcrCompId)
{
    UINT8 u1StcId;

    switch (u1PcrCompId)
    {
    case PCR_COMP_1:
        u1StcId = STC_SRC_A1;
        break;

    case PCR_COMP_2:
        u1StcId = STC_SRC_A2;
        break;

    default:
        // Invalid ID
        u1StcId = STC_SRC_NS;
        break;
    }

    return u1StcId;
}


//-----------------------------------------------------------------------------
/** _MwDmxGetPcrIndex
 *  Get PCR index of a given PID
 *
 *  @param  u2Pid           PID value
 *  @param  pu1PcrIndex [output]
 *                          The PCR index
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxGetPcrIndex(UINT16 u2Pid, UINT8* pu1PcrIndex)
{
    UINT32 i;
    BOOL fgRet = FALSE;

    ASSERT(pu1PcrIndex != NULL);

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if (_arPcr[i].fgPcr && (_arPcr[i].u2Pid == u2Pid))
        {
            *pu1PcrIndex = (UINT8)i;
            fgRet = TRUE;
            break;
        }
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _MwDmxAllocateVideoPidBuffer
 *  Allocate buffer for video PID
 *
 *  @param  u1Pidx          PID index
 *  @param  ptDestInfo      Destination info
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxAllocateVideoPidBuffer(UINT8 u1Pidx,
    const DEMUX_DEST_INFO_T* ptDestInfo)
{
    DMX_PID_T rPid;
    FBM_POOL_T* prFbmPool;
    UINT8 u1Channel;

    ASSERT(ptDestInfo != NULL);

    // Allocate video buffer from FBM
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != NULL);

    // Setup buffer attributes
    rPid.fgAllocateBuffer = FALSE;
    rPid.u4BufAddr = prFbmPool->u4Addr;
    rPid.u4BufSize = prFbmPool->u4Size;
    rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;
/*
    // Allocate buffer
    rPid.fgAllocateBuffer = TRUE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = DMX_VIDEO_ES_FIFO_SIZE;
    rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;
*/
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_BUFFER, &rPid))
    {
        return FALSE;
    }

    // Get channel id
    if (!DMX_GetVideoChannel(u1Pidx, &u1Channel))
    {
        return FALSE;
    }

    // Set device id
    rPid.u1DeviceId = MW_MPV_CompIdtoVldId(ptDestInfo->ui2_id,
        u1Channel);
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_DEVICE_ID, &rPid))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwDmxAllocateAudioPidBuffer
 *  Allocate buffer for audio PID
 *
 *  @param  u1Pidx          PID index
 *  @param  ptDestInfo      Destination info
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxAllocateAudioPidBuffer(UINT8 u1Pidx,
    DEMUX_DEST_INFO_T* ptDestInfo)
{
    DMX_PID_T rPid;
    UINT32 au4BufStart[2], au4BufEnd[2];
    UINT8 u1DeviceId;

    ASSERT(ptDestInfo);

    // Get audio buffer
    if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
        &au4BufEnd[1]) != AUD_OK)
    {
        // Can't allocate audio buffer
        return FALSE;
    }
    if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0))
    {
        // Can't allocate audio buffer
        return FALSE;
    }

    // Set audio buffer and device id
    u1DeviceId = (UINT8)ptDestInfo->ui2_id;
    rPid.u4BufAddr = au4BufStart[u1DeviceId];
    rPid.u4BufSize = au4BufEnd[u1DeviceId] - au4BufStart[u1DeviceId];
    rPid.fgAllocateBuffer = FALSE;
    rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
    rPid.u1DeviceId = u1DeviceId;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_DEVICE_ID,
        &rPid))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwDmxVideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxVideoCallback(const DMX_PES_MSG_T* prPes)
{
    MPV_PES_INFO_T rMpvPes;

    ASSERT(prPes != NULL);

    switch (prPes->u4FrameType)
    {
    case PIC_TYPE_I:
        rMpvPes.ucPicType = MPV_I_TYPE;
        break;

    case PIC_TYPE_P:
        rMpvPes.ucPicType = MPV_P_TYPE;
        break;

    case PIC_TYPE_B:
        rMpvPes.ucPicType = MPV_B_TYPE;
        break;

    default:
        ASSERT(0);
        break;
    }

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;
    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;
    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    rMpvPes.fgGop = prPes->fgGop;

    return (MPV_CallBackEsm(&rMpvPes) == 1);
}


//-----------------------------------------------------------------------------
/** _MwDmxOnSetPid
 *  Event handler of setting PID
 *
 *  @param  pt_comp_id      Component ID
 *  @param  pv_set_info     Set info
 *  @param  u1Pidx          PID index
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxOnSetPid(const DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info, UINT8 u1Pidx)
{
    DMX_PID_T rPid;
    INT32 i4Ret = RMR_OK;
    UINT16 u2Pid, u2Entry;
    UINT8 u1PidIndex, u1PcrIndex, u1StcId;

    ASSERT(pt_comp_id != NULL);

  	u2Entry = pt_comp_id->ui2_id;
    u2Pid = (UINT16)(MPEG_2_PID_T)(UINT32)pv_set_info;

    switch (pt_comp_id->e_type)
    {
    case DRVT_DEMUX_TS_PCR:
        if (_arPcr[u2Entry].u2Pid != u2Pid)
        {
            u1Pidx = (UINT8)u2Entry;
        }
        _arPcr[u2Entry].u2Pid = u2Pid;

        if (DMX_GetPidIndex(u2Pid, &u1PidIndex) &&
            (u1PidIndex != (UINT8)u2Entry))
        {
            // There is another PES entry with the same PID already
            // Set PCR redirect pidx
            _arPcr[u2Entry].u1PidIndex = u1PidIndex;

            // Free the PCR PID
            if (!DMX_FreePid(u2Entry))
            {
                return RMR_DRV_INV_SET_INFO;
            }

            // Re-map PID index and STC id
            UNUSED(DMX_SetStcSrc((UINT8)u2Entry, STC_SRC_NS));
            u1StcId = _MwDmxGetStcId((UINT8)u2Entry);
            UNUSED(DMX_SetStcSrc(u1PidIndex, u1StcId));

            // Append PCR mode to the PID
            rPid.ePcrMode = (u1StcId == STC_SRC_A1) ? DMX_PCR_MODE_NEW :
                DMX_PCR_MODE_OLD;
            if (!DMX_SetPid(u1PidIndex, DMX_PID_FLAG_PCR, &rPid))
            {
                return RMR_DRV_INV_SET_INFO;
            }
        }
        else
        {
            // No another PES entry with the same PID
            // Append PID
            ASSERT(u1Pidx == (UINT8)u2Entry);
            rPid.u2Pid = u2Pid;
            if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_PID, &rPid))
            {
                return RMR_DRV_INV_SET_INFO;
            }
        }
        break;

    case DRVT_DEMUX_TS_PES_PACKET:
        rPid.u2Pid = u2Pid;
        rPid.ePcrMode = DMX_PCR_MODE_NONE;
        if (_MwDmxGetPcrIndex(u2Pid, &u1PcrIndex) &&
            (u1PcrIndex != u1Pidx))
        {
            // There is another PCR entry with the same PID
            DMX_PID_T rPcrPid;

            // Disable the PCR PID first
            rPcrPid.fgEnable = FALSE;
            if (!DMX_SetPid(u1PcrIndex, DMX_PID_FLAG_VALID, &rPcrPid))
            {
                return RMR_DRV_INV_SET_INFO;
            }

            // Free the PCR PID
            if (!DMX_FreePid(u1PcrIndex))
            {
                return RMR_DRV_INV_SET_INFO;
            }

            // Set PCR redirect pidx
            _arPcr[u1PcrIndex].u1PidIndex = u1Pidx;

            // Re-map PID index and STC id
            UNUSED(DMX_SetStcSrc(u1PcrIndex, STC_SRC_NS));
            u1StcId = _MwDmxGetStcId(u1PcrIndex);
            UNUSED(DMX_SetStcSrc(u1Pidx, u1StcId));

            // Enable PCR too
            rPid.ePcrMode = (u1StcId == STC_SRC_A1) ? DMX_PCR_MODE_NEW :
                DMX_PCR_MODE_OLD;
        }
        if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_PID | DMX_PID_FLAG_PCR,
            &rPid))
        {
            i4Ret = RMR_DRV_INV_SET_INFO;
        }
        break;

    default:
        i4Ret = RMR_DRV_INV_SET_INFO;
        break;
    }

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** _MwDmxOnEnablePid
 *  Event handler of enabling/disabling PID
 *
 *  @param  pt_comp_id      Component ID
 *  @param  pv_set_info     Set info
 *  @param  u1Pidx          PID index
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxOnEnablePid(const DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info, UINT8 u1Pidx)
{
    DMX_PID_T rPid;
    INT32 i4Ret = RMR_OK;
    UINT16 u2Entry;
    BOOL fgEnable;

    ASSERT(pt_comp_id != NULL);

  	u2Entry = pt_comp_id->ui2_id;

    if (pt_comp_id->e_type == DRVT_DEMUX_TS_PCR)
    {
        if (u1Pidx != (UINT8)u2Entry)
        {
            // PCR PID is redirected to other one, enabling PCR PID will
            // enable that PID too. Thus don't do it here
            return i4Ret;
        }
    }

    fgEnable = (BOOL)(UINT32)pv_set_info;

    if (fgEnable)
    {
        if (pt_comp_id->e_type == DRVT_DEMUX_TS_PES_PACKET)
        {
            // Check if PID type is none (used to detect scramble state)
            if (!DMX_GetPid(u1Pidx, DMX_PID_FLAG_BUFFER, &rPid))
            {
                return RMR_DRV_INV_SET_INFO;
            }

            if (rPid.ePidType == DMX_PID_TYPE_NONE)
            {
                // PID type is none, allocate a default buffer
                rPid.fgAllocateBuffer = FALSE;
                rPid.u4BufAddr = _MwDmxGetDefaultEsBuffer();
                rPid.u4BufSize = DMX_DEFAULT_ES_FIFO_SIZE;
                rPid.ePidType = DMX_PID_TYPE_ES_OTHER;
                if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_BUFFER, &rPid))
                {
                    return RMR_DRV_INV_SET_INFO;
                }
            }
        }
    }
    else
    {
        // Flush DSP first in case of disabling an audio channel
        if (!DMX_GetPid(u1Pidx, DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_DEVICE_ID,
            &rPid))
        {
            return RMR_DRV_INV_SET_INFO;
        }

        if (rPid.ePidType == DMX_PID_TYPE_ES_AUDIO)
        {
            AUD_WaitDspFlush(rPid.u1DeviceId);
        }
        else if (rPid.ePidType == DMX_PID_TYPE_ES_OTHER)
        {
            if (pt_comp_id->e_type == DRVT_DEMUX_TS_PES_PACKET)
            {
                _MwDmxReleaseDefaultEsBuffer(rPid.u4BufAddr);
            }
        }
    }

    // Enable or disable a PID
    rPid.fgEnable = fgEnable;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
    {
        i4Ret = RMR_DRV_INV_SET_INFO;
    }

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** _MwDmxOnSetDestination
 *  Event handler of setting destination ID
 *
 *  @param  pt_comp_id      Component ID
 *  @param  pv_set_info     Set info
 *  @param  u1Pidx          PID index
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxOnSetDestination(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info, UINT8 u1Pidx)
{
    DEMUX_DEST_INFO_T* ptDestInfo;
    INT32 i4Ret = RMR_OK;

    ptDestInfo = (DEMUX_DEST_INFO_T*)pv_set_info;

    if (ptDestInfo == NULL)
    {
        return i4Ret;
    }

    switch (ptDestInfo->e_type)
    {
    case DRVT_VID_DEC:
        if (!_MwDmxAllocateVideoPidBuffer(u1Pidx, ptDestInfo))
        {
            i4Ret = RMR_DRV_INV_SET_INFO;
        }
        break;

    case DRVT_AUD_DEC:
        if (!_MwDmxAllocateAudioPidBuffer(u1Pidx, ptDestInfo))
        {
            i4Ret = RMR_DRV_INV_SET_INFO;
        }
        break;

    default:
        i4Ret = RMR_DRV_INV_SET_INFO;
        break;
    }

    UNUSED(pt_comp_id);

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** _MwDmx_Connect
 *  Middleware interface - connect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmx_Connect(
							const DRV_COMP_ID_T*   pt_comp_id,
							DRV_CONN_TYPE_T  e_conn_type,
							const VOID*      pv_conn_info,
							SIZE_T           z_conn_info_len,
							VOID*            pv_tag,
							x_rm_nfy_fct     pf_nfy)
{
    DMX_PID_T rPid;
    UINT32 u4Flags;
    UINT8 u1Pidx;

	// 1. Check para valid
    if ((NULL == pt_comp_id) || (NULL == pv_conn_info) )
    {
       	return RMR_DRV_INV_CONN_INFO;
    }

    // Check component type
    // ...

    // Check index range
  	u1Pidx = (UINT8)pt_comp_id->ui2_id;
  	if (u1Pidx >= DMX_NUM_PID_INDEX)
  	{
  	    return RMR_DRV_INV_CONN_INFO;
  	}

    // Check connection type
	if (e_conn_type != RM_CONN_TYPE_COMP_ID)
	{
       return RMR_DRV_INV_CONN_INFO;
	}

    // Check frontend component
    //
    // Only support normal tuner currently
    // ...

    // Construct a normal PID
    rPid.u1TsIndex = 0;
	rPid.u1KeyIndex = 0;
	rPid.u1SteerMode = DMX_STEER_TO_FTUP;
	rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
	rPid.pfnNotify = NULL;
	rPid.pvNotifyTag = NULL;
    rPid.fgAllocateBuffer = FALSE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = 0;
    rPid.ePidType = DMX_PID_TYPE_NONE;

    if (pt_comp_id->e_type == DRVT_DEMUX_TS_PCR)
    {
        UINT8 u1StcId;

        // Set mapping of PID index and STC id
        u1StcId = _MwDmxGetStcId(u1Pidx);
        UNUSED(DMX_SetStcSrc(u1Pidx, u1StcId));

        _arPcr[u1Pidx].fgPcr = TRUE;
        _arPcr[u1Pidx].u1PidIndex = u1Pidx;
        _arPcr[u1Pidx].u2Pid = 0;
        rPid.ePcrMode = (u1StcId == STC_SRC_A1) ? DMX_PCR_MODE_NEW :
            DMX_PCR_MODE_OLD;
        rPid.u1SteerMode = 0;
    }

	// Set these flags only. Others will be set by 'set' command directly
	u4Flags = DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_KEY_INDEX |
	    DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_STEER | DMX_PID_FLAG_PCR |
	    DMX_PID_FLAG_CALLBACK | DMX_PID_FLAG_BUFFER;

	if (!DMX_SetPid(u1Pidx, u4Flags, &rPid))
	{
	    return RMR_DRV_CONN_FAILED;
	}

    UNUSED(z_conn_info_len);
    UNUSED(pv_tag);
    UNUSED(pf_nfy);

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmx_Disconnect
 *  Middleware interface - disconnect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmx_Disconnect(
							const DRV_COMP_ID_T*    pt_comp_id,
							DRV_DISC_TYPE_T         e_disc_type,
							const VOID*             pv_disc_info,
							SIZE_T                  z_disc_info_len)
{
    DMX_PID_T rPid;
    UINT8 u1Pidx;

	// Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_disc_info) )
    {
       	return (RMR_DRV_INV_CONN_INFO);
    }

    // Check component type
    // ...

    // Check index range
  	u1Pidx = (UINT8)pt_comp_id->ui2_id;
  	if (u1Pidx >= DMX_NUM_PID_INDEX)
  	{
  	    return RMR_DRV_INV_CONN_INFO;
  	}

    // Check frontend component
    // ...

    // Free Channel
    if (pt_comp_id->e_type == DRVT_DEMUX_TS_PES_PACKET)
    {
        if (_arPcr[u1Pidx].fgPcr)
        {
            // PCR PID not free yet, disable steering to FTuP only here
            u1Pidx = _arPcr[u1Pidx].u1PidIndex;
            VERIFY(DMX_GetPid(u1Pidx, DMX_PID_FLAG_STEER, &rPid));
            ASSERT((rPid.u1SteerMode & DMX_STEER_TO_FTUP) != 0);
            rPid.u1SteerMode &= ~DMX_STEER_TO_FTUP;
            VERIFY(DMX_SetPid(u1Pidx, DMX_PID_FLAG_STEER, &rPid));
        }
        else
        {
            if (!DMX_FreePid(u1Pidx))
            {
                // Release a PID
                return RMR_DRV_INV_CONN_INFO;
            }

            // Reset scramble state callback
            if (!_MwDmxResetScrambleCallback(u1Pidx))
            {
                return RMR_DRV_INV_CONN_INFO;
            }
        }
	}
	else if (pt_comp_id->e_type == DRVT_DEMUX_TS_PCR)
	{
	    if (!_arPcr[u1Pidx].fgPcr)
	    {
	        // No such PCR connected
	        return RMR_DRV_INV_CONN_INFO;
	    }

        _arPcr[u1Pidx].fgPcr = FALSE;

	    // Release a PCR
	    u1Pidx = _arPcr[u1Pidx].u1PidIndex;

        // Check if PES path still there
        VERIFY(DMX_GetPid(u1Pidx, DMX_PID_FLAG_STEER, &rPid));
        if (rPid.fgEnable && ((rPid.u1SteerMode & DMX_STEER_TO_FTUP) != 0))
        {
            // There is still PES path, can't free pid here
            rPid.ePcrMode = DMX_PCR_MODE_NONE;
            VERIFY(DMX_SetPid(u1Pidx, DMX_PID_FLAG_PCR, &rPid));
        }
        else
        {
            // Free PID
            if (!DMX_FreePid(u1Pidx))
            {
                return RMR_DRV_INV_CONN_INFO;
            }

            // Reset scramble state callback
            if (!_MwDmxResetScrambleCallback(u1Pidx))
            {
                return RMR_DRV_INV_CONN_INFO;
            }
        }

        // Reset mapping of PID index and STC id by setting an invalid STC id
        UNUSED(DMX_SetStcSrc(u1Pidx, STC_SRC_NS));
	}

    UNUSED(e_disc_type);
    UNUSED(z_disc_info_len);

	return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmx_Get
 *  Middleware interface - get
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmx_Get(
						const DRV_COMP_ID_T*    pt_comp_id,
						DRV_GET_TYPE_T		    e_get_type,
						VOID*				    pv_get_info,
						SIZE_T*				    pz_get_info_len)
{
    DMX_PID_T rPid;
    DEMUX_SCRS_T* prScrs;
    DEMUX_STC_INFO_T* prStc;
    STC_CLOCK rStcClk;
    UINT8 u1Pidx;
    UINT8 u1StcId;

	// Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_get_info) )
    {
       	return (RMR_DRV_INV_GET_INFO);
    }

    // Check component type
    // ...

    // Check index range
  	u1Pidx = (UINT8)pt_comp_id->ui2_id;
  	if (u1Pidx >= DMX_NUM_PID_INDEX)
  	{
  	    return RMR_DRV_INV_GET_INFO;
  	}

    // Translate PID index of PCR
	if (pt_comp_id->e_type == DRVT_DEMUX_TS_PCR)
	{
#if 0
        if (!_arPcr[u1Pidx].fgPcr)
        {
            return RMR_DRV_INV_GET_INFO;
        }
#endif
        u1Pidx = _arPcr[u1Pidx].u1PidIndex;
	}

    //get
    switch (e_get_type)
    {
    case DEMUX_GET_TYPE_PID:
        // Get PID value
        if (!DMX_GetPid(u1Pidx, DMX_PID_FLAG_PID, &rPid))
        {
            return RMR_DRV_INV_GET_INFO;
        }
        ASSERT(pz_get_info_len != NULL);
        *(MPEG_2_PID_T*)pv_get_info = rPid.u2Pid;
        *pz_get_info_len = sizeof(MPEG_2_PID_T);
        break;

    case DEMUX_GET_TYPE_CTRL:
        // Query if a PID is enabled or not
        if (!DMX_GetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            return RMR_DRV_INV_GET_INFO;
        }
        ASSERT(pz_get_info_len != NULL);
        *(BOOL*)pv_get_info = rPid.fgEnable;
        *pz_get_info_len = sizeof(BOOL);
        break;

    case DEMUX_GET_TYPE_SCRS:
        prScrs = (DEMUX_SCRS_T*)pv_get_info;
        *prScrs = DEMUX_SCRS_UNKNOWN;
        if (DMX_GetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            if (rPid.fgEnable)
            {
                DMX_SCRAMBLE_STATE_T eState;

                eState = DMX_GetScrambleState(u1Pidx);
                switch (eState)
                {
                case DMX_SCRAMBLE_STATE_CLEAR:
                    *prScrs = DEMUX_SCRS_CLEAR;
                    break;

                case DMX_SCRAMBLE_STATE_SCRAMBLED:
                    *prScrs = DEMUX_SCRS_SCRAMBLED;
                    break;

                case DMX_SCRAMBLE_STATE_UNKNOWN:
                default:
                    *prScrs = DEMUX_SCRS_UNKNOWN;
                    break;
                }
            }
        }
        break;

    case DEMUX_GET_TYPE_STC:
        prStc = (DEMUX_STC_INFO_T*)pv_get_info;
        u1Pidx = (UINT8)pt_comp_id->ui2_id;     // Bug Fixed
        u1StcId = _MwDmxGetStcId(u1Pidx);

        if (STC_GetSrc(u1StcId, &rStcClk) == STC_VALID)
        {
            prStc->ui1_flags = DEMUX_STC_FLAG_STABLE;
        }
        else
        {
            prStc->ui1_flags = 0;
        }

        prStc->t_stc = (STC_T) (rStcClk.u4Base);
        break;

    default:
        break;
    }

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmx_Set
 *  Middleware interface - set
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmx_Set(
					DRV_COMP_ID_T*		pt_comp_id,
					DRV_SET_TYPE_T		e_set_type,
					const VOID*			pv_set_info,
					SIZE_T				z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    UINT8 u1Pidx;
    BOOL fgRet;

	// Check parameters
    if ((NULL == pt_comp_id))
    {
       	return RMR_DRV_INV_SET_INFO;
    }

    // Check component type
    // ...

    // Check index range
  	u1Pidx = (UINT8)pt_comp_id->ui2_id;
  	if (u1Pidx >= DMX_NUM_PID_INDEX)
  	{
  	    return RMR_DRV_INV_SET_INFO;
  	}

    // Translate PID index of PCR
	if (pt_comp_id->e_type == DRVT_DEMUX_TS_PCR)
	{
#if 0
        if (!_arPcr[u1Pidx].fgPcr)
        {
            return RMR_DRV_INV_SET_INFO;
        }
#endif
        u1Pidx = _arPcr[u1Pidx].u1PidIndex;
	}

    switch (e_set_type)
    {
    case DEMUX_SET_TYPE_PID:
        // Set PID value
        i4Ret = _MwDmxOnSetPid(pt_comp_id, pv_set_info, u1Pidx);
        break;

    case DEMUX_SET_TYPE_CTRL:
        // Enable/disable PID
        i4Ret = _MwDmxOnEnablePid(pt_comp_id, pv_set_info, u1Pidx);
        break;

    case DEMUX_SET_TYPE_DEST_ID:
        // Set destination ID
        i4Ret = _MwDmxOnSetDestination(pt_comp_id, pv_set_info, u1Pidx);
        break;

    case DEMUX_SET_TYPE_SCRS_NFY_FCT:
        // Set scramble state callback function
        fgRet = _MwDmxSetScrambleCallback(u1Pidx,
            (DEMUX_NFY_INFO_T*)pv_set_info);
        i4Ret = (fgRet ? RMR_OK : RMR_DRV_INV_SET_INFO);
        break;

    default:
        break;
    }

    UNUSED(z_set_info_len);

    return i4Ret;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** MW_DMX_Init
 *  Initialize demux driver for middleware interface
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL MW_DMX_Init(void)
{
	ID_IND_T at_id_ind[DMX_PES_COMP_NUM];
	DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
	DRV_COMP_REG_T t_comp_id;
	DRV_COMP_FCT_TBL_T t_comp_fct_tbl;
    UINT32 i, u4Flags;
	INT32 i4Ret;

    // Initlialize driver
    if (!DMX_Init())
    {
        return FALSE;
    }

    // Local initialization
    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        _arPcr[i].fgPcr = FALSE;
        _arNotifyInfo[i].pf_demux_nfy = NULL;
        _arNotifyInfo[i].pv_tag = NULL;
    }

    // Set decoder callback functions
	rDecoderCallbacks.pfnSendVideoPes = _MwDmxVideoCallback;
	rDecoderCallbacks.pfnUpdateVideoWp = MPV_UpdateVldWptr;
	rDecoderCallbacks.pfnSendAudioPes = AUD_SendAudioPes;
	DMX_SetDecoderCallbacks(&rDecoderCallbacks);
	DMX_SetToDecoder(TRUE);

	// Register PES components
	//
	t_comp_fct_tbl.pf_rm_connect = (x_rm_connect_fct)_MwDmx_Connect;
	t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwDmx_Disconnect;
	t_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)_MwDmx_Get;
	t_comp_fct_tbl.pf_rm_set = (x_rm_set_fct)_MwDmx_Set;
	t_comp_id.e_type = DRVT_DEMUX_TS_PES_PACKET;
	t_comp_id.e_id_type = ID_TYPE_LIST;
	t_comp_id.u.t_list.ui2_num_of_ids = DMX_PES_COMP_NUM;
	t_comp_id.u.t_list.pt_list = at_id_ind;

	for (i = 0; i < DMX_PES_COMP_NUM; i++)
	{
		t_comp_id.u.t_list.pt_list[i].ui2_id = 4 + i;       // fixme
		t_comp_id.u.t_list.pt_list[i].pv_tag = NULL;
		t_comp_id.u.t_list.pt_list[i].ui1_port = ALL_PORTS;
	}

    u4Flags = DRV_FLAG_SINGLE_CONN_ON_OUTPUT;

	i4Ret = x_rm_reg_comp(&t_comp_id,
	                     1,                     // in port num
	                     1,                     // out port num
	                     NULL,                  // component name
	                     u4Flags,               // flags
	                     &t_comp_fct_tbl,
	                     NULL,
	                     0);
	if (i4Ret != RMR_OK)
	{
		return FALSE;
	}

#if 0
	// Register PES connection list
	t_prcd_comp_id.e_type = MW_REG_TYPE_DMX_TS1;
	t_prcd_comp_id.e_id_type = ID_TYPE_IND;
	t_prcd_comp_id.u.t_ind.ui2_id = MW_REG_ID_DMX_TS1;
	t_prcd_comp_id.u.t_ind.ui1_port = ALL_PORTS;
	t_prcd_comp_id.u.t_ind.pv_tag = NULL;

	i4Ret = x_rm_reg_conn_list(&t_prcd_comp_id,
	                           &t_comp_id,
	                           1,
	                           CONN_DIR_OUT_TO_INP,
	                           FALSE );
	if (i4Ret != RMR_OK)
	{
		return FALSE;
	}
#endif  // 0

	// Register PCR components
	t_comp_fct_tbl.pf_rm_connect = (x_rm_connect_fct)_MwDmx_Connect;
	t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwDmx_Disconnect;
	t_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)_MwDmx_Get;
	t_comp_fct_tbl.pf_rm_set = (x_rm_set_fct)_MwDmx_Set;

	t_comp_id.e_type = DRVT_DEMUX_TS_PCR;
	t_comp_id.e_id_type = ID_TYPE_LIST;
	t_comp_id.u.t_list.ui2_num_of_ids = DMX_PCR_COMP_NUM;
	t_comp_id.u.t_list.pt_list = at_id_ind;

	for (i = 0; i < DMX_PCR_COMP_NUM; i++)
	{
		t_comp_id.u.t_list.pt_list[i].ui2_id = DMX_PCR_COMP_ID_START + i;
		t_comp_id.u.t_list.pt_list[i].pv_tag = NULL;
		t_comp_id.u.t_list.pt_list[i].ui1_port = ALL_PORTS;
	}

	i4Ret = x_rm_reg_comp(&t_comp_id,
	                      1,                    // in port num
	                      1,                    // out port num
	                      NULL,                 // component name
	                      u4Flags,              // flags
	                      &t_comp_fct_tbl,
	                      NULL,
	                      0);
	if (i4Ret != RMR_OK)
	{
		return FALSE;
	}

#if 0
	// Register PCR connection list
	t_prcd_comp_id.e_type =  MW_REG_TYPE_DMX_TS1;
	t_prcd_comp_id.e_id_type = ID_TYPE_IND;
	t_prcd_comp_id.u.t_ind.ui2_id = MW_REG_ID_DMX_TS1;
	t_prcd_comp_id.u.t_ind.ui1_port = ALL_PORTS;
	t_prcd_comp_id.u.t_ind.pv_tag = NULL;

	i4Ret = x_rm_reg_conn_list(&t_prcd_comp_id,
	                           &t_comp_id,
	                           1,
	                           CONN_DIR_OUT_TO_INP,
	                           FALSE);
	if (i4Ret != RMR_OK)
	{
		return FALSE;
	}
#endif

#if 1
    // Register PSI components
    if (!_MW_DMX_PSI_Init())
    {
        return FALSE;
    }
#endif

#if 1
    // Register PES components
    if (!_MW_DMX_PES_Init())
    {
        return FALSE;
    }
#endif

#ifdef DRV_ENABLE_DESCRAMBLER
    // Initialize descrambler
    _MW_DMX_DESC_Init();
#endif

    return TRUE;
}


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
#include "dmx_mm_if.h"
#include "x_lint.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_data_rm.h"
#endif

LINT_EXT_HEADER_BEGIN

#ifdef LINUX_TURNKEY_SOLUTION
#include "mpv_if.h"
#endif

#ifdef ENABLE_MULTIMEDIA
#include "swdmx_if.h"
#endif
#include "aud_if.h"
#include "aud_drvif.h"
#include "vdec_drvif.h"
#include "fbm_drvif.h"
#include "drv_comp_id.h"
#include "stc_drvif.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_demux.h"
#include "x_rm_dev_types.h"
#include "x_os.h"

#define DEFINE_IS_LOG   DMX_IsLog
#include "x_debug.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_param.h"
#endif

LINT_EXT_HEADER_END

#ifdef LINUX_TURNKEY_SOLUTION
extern INT32 _CB_PutEvent_Adpt(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
#endif

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/// queue
static HANDLE_T _hDmxMwPSIQueue = NULL_HANDLE;
static HANDLE_T _hDmxMwPESQueue = NULL_HANDLE;
static HANDLE_T _hDmxMwOVFLQueue = NULL_HANDLE;

/// thread
static HANDLE_T _hDmxMwPSIThread = NULL_HANDLE;
static HANDLE_T _hDmxMwPESThread = NULL_HANDLE;
static HANDLE_T _hDmxMwOVFLThread = NULL_HANDLE;

// Demux queue and thread paramenters
//
#define DMX_MW_PSI_QUEUE_NAME              "DMXMWPSI"
#define DMX_MW_PSI_QUEUE_SIZE              32

#define DMX_MW_PSI_THREAD_NAME             "DMXMWPSITHREAD"
#define DMX_MW_PSI_THREAD_STACK_SIZE       2048
#define DMX_MW_PSI_THREAD_PRIORITY         60

#define DMX_MW_PES_QUEUE_NAME              "DMXMWPES"
#define DMX_MW_PES_QUEUE_SIZE              32

#define DMX_MW_PES_THREAD_NAME             "DMXMWPESTHREAD"
#define DMX_MW_PES_THREAD_STACK_SIZE       2048
#define DMX_MW_PES_THREAD_PRIORITY         60

#define DMX_MW_OVFL_QUEUE_NAME             "DMXMWOVFL"
#define DMX_MW_OVFL_QUEUE_SIZE             32

#define DMX_MW_OVFL_THREAD_NAME            "DMXMWOVFLTHREAD"
#define DMX_MW_OVFL_THREAD_STACK_SIZE      2048
#define DMX_MW_OVFL_THREAD_PRIORITY        60

#define DMX_MSG_PRIORITY_NORMAL             128

#define DMX_VIDEO_ES_FIFO_SIZE              0x300000        // 3MB
#define DMX_DEFAULT_ES_FIFO_SIZE            0x20000         // 128K
#define DMX_ES_FIFO_ALIGNMENT               32

#define DMX_CIPLUS_KEY_INDEX                (0)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT8 u1Pidx;
    DMX_NOTIFY_INFO_PSI_T rPsi;
    void* pvTag;
} DMX_MW_PSI_MSG_T;

typedef struct
{
    UINT8 u1Pidx;
    DMX_NOTIFY_INFO_PES_T rPes;
    void* pvTag;
} DMX_MW_PES_MSG_T;

typedef struct
{
    UINT8 u1Pidx;
    UINT32 u4Data;
    void* pvTag;
} DMX_MW_MSG_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define DWORDSWAP(u4Tmp)        (((u4Tmp & 0xff) << 24) | ((u4Tmp & 0xff00) << 8) | ((u4Tmp & 0xff0000) >> 8) | ((u4Tmp & 0xff000000) >> 24))
#define DMX_CI_DES_KEY_SWAP_MODE    (1)
#define DMX_CI_AES_KEY_SWAP_MODE    (1)//(0)
#define DMX_CI_AES_IV_SWAP_MODE     (1)

//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------

/// Translate MPV component ID to channel ID
EXTERN UCHAR MW_MPV_CompIdtoVldId(UINT16 u2CompId, UINT32 u4ChannelId);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// Notification callback functions
static DEMUX_NFY_INFO_T _arNotifyInfo[DMX_NUM_PID_INDEX];

/// Notification callback of scramble state
static DEMUX_NFY_INFO_T _arScrambleState[DMX_NUM_PID_INDEX];

/// Buffer address for scrambling state detection
static UINT32 _u4DefaultBufAddr = 0;

/// Reference counter of buffer of scrambling state detection
static UINT32 _u4DefaultBufRefCount = 0;

/// State word for locking interrupt
static CRIT_STATE_T _rDmxLock;

/// Is interrupt locking? For making sure Lock()/Unlock() are not nested
static BOOL _fgDmxLocking = FALSE;

static MW_DMX_PID_INFO_T _arDmxInfo[DMX_NUM_PID_INDEX];

#ifdef ENABLE_MULTIMEDIA
#ifdef CC_53XX_SWDMX_V2
static UINT8 _u1SwdmxSrcId = 0;
#endif
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef LINUX_TURNKEY_SOLUTION
static BOOL set_dmx_set_nfy(UINT32          ui4_id,
                            VOID*           pv_nfy_tag,
                            DEMUX_COND_T    e_nfy_cond,
                            UINT32          ui4_data_1,
                            UINT32          ui4_data_2
            #ifndef MW_CLASSIC_DMX_NFY
                            ,
                            UINT64          ui8_ext_data_1,
                            UINT64          ui8_ext_data_2,
                            UINT32          ui4_ext_data_3
            #endif
                            )
{
    DMX_SET_NFY_PARAM dmx_set_param;
    dmx_set_param.pf_nfy = _arNotifyInfo[ui4_id].pf_demux_nfy;
    dmx_set_param.pv_nfy_tag = pv_nfy_tag;
    dmx_set_param.e_nfy_cond = e_nfy_cond;
    dmx_set_param.ui4_data_1 = ui4_data_1;
    dmx_set_param.ui4_data_2 = ui4_data_2;
#ifndef MW_CLASSIC_DMX_NFY
    dmx_set_param.ui8_ext_data_1 = ui8_ext_data_1;
    dmx_set_param.ui8_ext_data_2 = ui8_ext_data_2;
    dmx_set_param.ui4_ext_data_3 = ui4_ext_data_3;
#endif
    return _CB_PutEvent_Adpt(CB_DRV_DMX_SET_NFY, sizeof(DMX_SET_NFY_PARAM), &dmx_set_param);
}

static BOOL set_dmx_nfy(UINT32          ui4_id,
                        VOID*           pv_nfy_tag,
                        DEMUX_COND_T    e_nfy_cond,
                        UINT32          ui4_data_1,
                        UINT32          ui4_data_2
            #ifndef MW_CLASSIC_DMX_NFY
                        ,
                        UINT64          ui8_ext_data_1,
                        UINT64          ui8_ext_data_2,
                        UINT32          ui4_ext_data_3
            #endif
                        )
{
    DMX_NFY_PARAM dmx_param;
    dmx_param.pf_nfy = _arScrambleState[ui4_id].pf_demux_nfy;
    dmx_param.pv_nfy_tag = pv_nfy_tag;
    dmx_param.e_nfy_cond = e_nfy_cond;
    dmx_param.ui4_data_1 = ui4_data_1;
    dmx_param.ui4_data_2 = ui4_data_2;
#ifndef MW_CLASSIC_DMX_NFY
    dmx_param.ui8_ext_data_1 = ui8_ext_data_1;
    dmx_param.ui8_ext_data_2 = ui8_ext_data_2;
    dmx_param.ui4_ext_data_3 = ui4_ext_data_3;
#endif
    _CB_PutEvent_Adpt(CB_DRV_DMX_NFY, sizeof(DMX_NFY_PARAM), &dmx_param);
    return FALSE;
}
#endif

#ifdef CI_PLUS_SUPPORT
static BOOL DataSwap(UINT32 *pu4Dest, UINT32 *pu4Src, UINT32 u4Size, UINT32 u4Mode)
{
    INT32 i = 0;
    UINT32 u4Tmp;

    if(u4Mode == 0)
    {
        for(i =0; i < u4Size; i++) //memcpy
        {
            *(pu4Dest + i) = *(pu4Src + i);
        }
    }
    else if(u4Mode == 1) //Endien Change
    {
        for(i =0; i < u4Size; i++)
        {
            u4Tmp = *(pu4Src + i);
            u4Tmp = DWORDSWAP(u4Tmp);
            *(pu4Dest + i) = u4Tmp;
        }
    }
    else if(u4Mode == 2) //Head Swap
    {
        for(i =0; i < u4Size; i++)
        {
            *(pu4Dest + u4Size - 1 - i) = *(pu4Src + i);
        }
    }
    else if(u4Mode == 3) //Head Swap + Endien Change
    {
        for(i =0; i < u4Size; i++)
        {
            u4Tmp = *(pu4Src + i);
            u4Tmp = DWORDSWAP(u4Tmp);
            *(pu4Dest + u4Size - 1 - i) = u4Tmp;
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}
#endif
//-----------------------------------------------------------------------------
/** _MwDmxLock
 *  Enter demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _MwDmxLock(void)
{
    _rDmxLock = x_crit_start();

    ASSERT(!_fgDmxLocking);
    _fgDmxLocking = TRUE;

    UNUSED(_fgDmxLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** _MwDmxUnlock
 *  Leave demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _MwDmxUnlock(void)
{
    ASSERT(_fgDmxLocking);
    _fgDmxLocking = FALSE;

    x_crit_end(_rDmxLock);

    UNUSED(_fgDmxLocking);          // Make Lint happy
}


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

        VERIFY(BSP_FreeAlignedDmaMemory(_u4DefaultBufAddr));

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
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
#ifndef MW_CLASSIC_DMX_NFY
#define PRE_READ_HDR_LEN    16
#endif

static VOID _MwDmxOnPsiHandler(VOID* pvArg)
{
    DEMUX_NFY_INFO_T rInfo;
    DMX_NOTIFY_INFO_PSI_T* prNotifyInfo;
    UINT32 u4Data1, u4SecAddr;
    UINT8 u1SerialNumber;
    BOOL fgKeepData;
#ifndef MW_CLASSIC_DMX_NFY
    UINT8 au1Buf[PRE_READ_HDR_LEN] = {0};
    UINT64 u8_ext_data_1, u8_ext_data_2;
    UINT32 u4GetAddr;
#endif
    UINT8 u1Pidx;
    UINT32 u4Data;
    void* pvTag;
    DMX_MW_PSI_MSG_T rMsg;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex;
    INT32 i4Ret;

    UNUSED(pvArg);
    UNUSED(i4Ret);

    while (1)
    {
        zMsgSize = sizeof (rMsg);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
            &_hDmxMwPSIQueue, 1, X_MSGQ_OPTION_WAIT);

        u1Pidx = rMsg.u1Pidx;
        u4Data = (UINT32)&(rMsg.rPsi);
        pvTag = rMsg.pvTag;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
            continue;
    }

    LOG(8, "[PSI PARSER]PSI Handler Notify to MW Pidx is %d\n",u1Pidx);
    _MwDmxLock();
    rInfo = _arNotifyInfo[u1Pidx];
    _MwDmxUnlock();

    if (rInfo.pf_demux_nfy == NULL)
    {
        LOG(3, "Pidx %u: No PSI handler registered\n", u1Pidx);
            continue;
    }

    prNotifyInfo = (DMX_NOTIFY_INFO_PSI_T*)u4Data;
    ASSERT(prNotifyInfo != NULL);

#ifndef LINUX_TURNKEY_SOLUTION
    u4SecAddr = VIRTUAL(prNotifyInfo->u4SecAddr);
#ifndef MW_CLASSIC_DMX_NFY
    u4GetAddr = u4SecAddr;
#endif
#else
    u4SecAddr = PHYSICAL(prNotifyInfo->u4SecAddr);
#ifndef MW_CLASSIC_DMX_NFY
    u4GetAddr = VIRTUAL(u4SecAddr);
#endif
#endif
    ASSERT(u4SecAddr < 0x20000000);
    u1SerialNumber = prNotifyInfo->u1SerialNumber;
    ASSERT(u1SerialNumber <= DMX_MAX_SERIAL_NUM);
    u4Data1 = (u1SerialNumber << 29) | u4SecAddr;

#ifndef MW_CLASSIC_DMX_NFY
    fgKeepData = DMX_PSI_GetBuffer(u1Pidx,
                                   u1SerialNumber,
                                   u4GetAddr,
                                   0/*skip size*/,
                                   (prNotifyInfo->u4SecLen < PRE_READ_HDR_LEN)?
                                    prNotifyInfo->u4SecLen : PRE_READ_HDR_LEN,
                                   (UCHAR*)&(au1Buf[0]));
    if (!fgKeepData)
    {
            continue;
    }

    u8_ext_data_1 = ((UINT64)(au1Buf[0])  << 56) | ((UINT64)(au1Buf[1])  << 48) |
                    ((UINT64)(au1Buf[2])  << 40) | ((UINT64)(au1Buf[3])  << 32) |
                    ((UINT64)(au1Buf[4])  << 24) | ((UINT64)(au1Buf[5])  << 16) |
                    ((UINT64)(au1Buf[6])  <<  8) | ((UINT64)(au1Buf[7]));
    u8_ext_data_2 = ((UINT64)(au1Buf[8])  << 56) | ((UINT64)(au1Buf[9])  << 48) |
                    ((UINT64)(au1Buf[10]) << 40) | ((UINT64)(au1Buf[11]) << 32) |
                    ((UINT64)(au1Buf[12]) << 24) | ((UINT64)(au1Buf[13]) << 16) |
                    ((UINT64)(au1Buf[14]) <<  8) | ((UINT64)(au1Buf[15]));
#endif

#ifndef LINUX_TURNKEY_SOLUTION
    fgKeepData = rInfo.pf_demux_nfy(
        rInfo.pv_tag, DEMUX_COND_DATA_REC,
        u4Data1, prNotifyInfo->u4SecLen
#ifndef MW_CLASSIC_DMX_NFY
        , u8_ext_data_1, u8_ext_data_2, 0
#endif
        );
#else
    fgKeepData = set_dmx_set_nfy(u1Pidx,
        rInfo.pv_tag, DEMUX_COND_DATA_REC,
        u4Data1, prNotifyInfo->u4SecLen
#ifndef MW_CLASSIC_DMX_NFY
        , u8_ext_data_1, u8_ext_data_2, 0
#endif
        );
#endif

    UNUSED(pvTag);
        UNUSED(fgKeepData);
        //return fgKeepData;
    }

}


//-----------------------------------------------------------------------------
/** _MwDmxOnPesNotify
 *  PES notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static VOID _MwDmxOnPesHandler(VOID* pvArg)
{
    DEMUX_NFY_INFO_T rInfo;
    DMX_NOTIFY_INFO_PES_T* prNotifyInfo;
    UINT32 u4DataAddr, u4Data1;
    UINT8 u1SerialNumber;
    BOOL fgKeepData;

    UINT8 u1Pidx;
    UINT32 u4Data;
    void* pvTag;
    DMX_MW_PES_MSG_T rMsg;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex;
    INT32 i4Ret;

    UNUSED(pvArg);
    UNUSED(i4Ret);

    while (1)
    {
        zMsgSize = sizeof (rMsg);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
            &_hDmxMwPESQueue, 1, X_MSGQ_OPTION_WAIT);

        u1Pidx = rMsg.u1Pidx;
        u4Data = (UINT32)&(rMsg.rPes);
        pvTag = rMsg.pvTag;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
            continue;
    }

	LOG(8, "PES Handler Notify to MW Pidx is %d\n",u1Pidx);
    _MwDmxLock();
    rInfo = _arNotifyInfo[u1Pidx];
    _MwDmxUnlock();

    if (rInfo.pf_demux_nfy == NULL)
    {
        LOG(3, "Pidx %u: No PES handler registered\n", u1Pidx);
            continue;
    }

    prNotifyInfo = (DMX_NOTIFY_INFO_PES_T*)u4Data;
    ASSERT(prNotifyInfo != NULL);

#ifndef LINUX_TURNKEY_SOLUTION
    u4DataAddr = VIRTUAL(prNotifyInfo->u4DataAddr);
#else
    u4DataAddr = PHYSICAL(prNotifyInfo->u4DataAddr);
#endif
    ASSERT(u4DataAddr < 0x20000000);
    u1SerialNumber = prNotifyInfo->u1SerialNumber;
    ASSERT(u1SerialNumber <= DMX_MAX_SERIAL_NUM);
    u4Data1 = (u1SerialNumber << 29) | u4DataAddr;

#ifndef LINUX_TURNKEY_SOLUTION
    fgKeepData = rInfo.pf_demux_nfy(
        rInfo.pv_tag, DEMUX_COND_DATA_REC,
        u4Data1, prNotifyInfo->u4DataSize
#ifndef MW_CLASSIC_DMX_NFY
        , 0, 0, 0
#endif
        );
#else
    fgKeepData = set_dmx_set_nfy(u1Pidx,
        rInfo.pv_tag, DEMUX_COND_DATA_REC,
        u4Data1, prNotifyInfo->u4DataSize
#ifndef MW_CLASSIC_DMX_NFY
        , 0, 0, 0
#endif
        );
#endif

    UNUSED(pvTag);
        UNUSED(fgKeepData);
        //return fgKeepData;
    }
}


//-----------------------------------------------------------------------------
/** _MwDmxOnEsNotify
 *  ES notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
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
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static VOID _MwDmxOnOverflowHandler(VOID* pvArg)
{
    DEMUX_NFY_INFO_T rInfo;

    UINT8 u1Pidx;
    UINT32 u4Data;
    void* pvTag;
    DMX_MW_MSG_T rMsg;
    SIZE_T zMsgSize;
    UINT16 u2QueueIndex;
    INT32 i4Ret;

    UNUSED(pvArg);
    UNUSED(i4Ret);

    while (1)
    {
        zMsgSize = sizeof (rMsg);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
            &_hDmxMwOVFLQueue, 1, X_MSGQ_OPTION_WAIT);

        u1Pidx = rMsg.u1Pidx;
        u4Data = rMsg.u4Data;
        pvTag = rMsg.pvTag;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
            continue;
    }

    _MwDmxLock();
    rInfo = _arNotifyInfo[u1Pidx];
    _MwDmxUnlock();

    if (rInfo.pf_demux_nfy == NULL)
    {
        LOG(3, "Pidx %u: No handler registered\n", u1Pidx);
            continue;
    }

#ifndef LINUX_TURNKEY_SOLUTION
    UNUSED(rInfo.pf_demux_nfy(
        rInfo.pv_tag, DEMUX_COND_OVERFLOW, 0, 0
    #ifndef MW_CLASSIC_DMX_NFY
        , 0, 0, 0
    #endif
        ));
#else
    UNUSED(set_dmx_set_nfy(u1Pidx,
        rInfo.pv_tag, DEMUX_COND_OVERFLOW, 0, 0
    #ifndef MW_CLASSIC_DMX_NFY
        , 0, 0, 0
    #endif
        ));
#endif

    UNUSED(u4Data);
    UNUSED(pvTag);
    }

    //return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwDmxOnScrambleStateNotify
 *  Scramble state notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxOnScrambleStateNotify(UINT8 u1Pidx, UINT32 u4Data,
    const void* pvTag)
{
    DEMUX_NFY_INFO_T rInfo;
    DMX_SCRAMBLE_STATE_T eState;
    DEMUX_SCRS_T eMwState;
    BOOL fgRet;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    _MwDmxLock();
    rInfo = _arScrambleState[u1Pidx];
    _MwDmxUnlock();

    if (rInfo.pf_demux_nfy == NULL)
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

#ifndef LINUX_TURNKEY_SOLUTION
    fgRet = rInfo.pf_demux_nfy(
        rInfo.pv_tag, DEMUX_COND_SCRS, (UINT32)eMwState, 0
    #ifndef MW_CLASSIC_DMX_NFY
        , 0, 0, 0
    #endif
        );
#else
    fgRet = set_dmx_nfy(u1Pidx,
        rInfo.pv_tag, DEMUX_COND_SCRS, (UINT32)eMwState, 0
    #ifndef MW_CLASSIC_DMX_NFY
        , 0, 0, 0
    #endif
        );
#endif

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
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxScrambleStateHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvTag)
{
    BOOL fgRet = FALSE;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

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
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
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
    if (!_MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_SCRAMBLE_STATE, &rPid))
    {
        return FALSE;
    }

    _MwDmxLock();
    _arScrambleState[u1Pidx].pf_demux_nfy = prNfyInfo->pf_demux_nfy;
    _arScrambleState[u1Pidx].pv_tag = prNfyInfo->pv_tag;
    _MwDmxUnlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwDmxResetScrambleCallback
 *  Reset scramble callback handler
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
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
    if (!_MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_SCRAMBLE_STATE, &rPid))
    {
        return FALSE;
    }

    _MwDmxLock();
    _arScrambleState[u1Pidx].pf_demux_nfy = NULL;
    _arScrambleState[u1Pidx].pv_tag = NULL;
    _MwDmxUnlock();

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
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvTag)
{
    BOOL fgRet = FALSE;

    switch (eCode)
    {
    case DMX_NOTIFY_CODE_PSI:
        {
            DMX_MW_PSI_MSG_T rMsg;
            rMsg.u1Pidx = u1Pidx;
            x_memcpy(&rMsg.rPsi, (void *)u4Data, sizeof(DMX_NOTIFY_INFO_PSI_T));
            rMsg.pvTag = (void *)pvTag;
            if (x_msg_q_send(_hDmxMwPSIQueue, (VOID*)&rMsg, sizeof (DMX_MW_PSI_MSG_T),
                DMX_MSG_PRIORITY_NORMAL) != OSR_OK)
            {
                LOG(3, "Demux MW PSI queue full!\n");
                fgRet = FALSE;
            }
            else
            {
                fgRet = TRUE;
            }
        }
        //fgRet = _MwDmxOnPsiNotify(u1Pidx, u4Data, pvTag);
        break;

    case DMX_NOTIFY_CODE_PES:
        {
            DMX_MW_PES_MSG_T rMsg;
            rMsg.u1Pidx = u1Pidx;
            x_memcpy(&rMsg.rPes, (void *)u4Data, sizeof(DMX_NOTIFY_INFO_PES_T ));
            rMsg.pvTag = (void *)pvTag;

            if (x_msg_q_send(_hDmxMwPESQueue, (VOID*)&rMsg, sizeof (DMX_MW_PES_MSG_T),
                DMX_MSG_PRIORITY_NORMAL) != OSR_OK)
            {
                LOG(3, "Demux MW PES queue full!\n");
                fgRet = FALSE;
            }
            else
            {
                fgRet = TRUE;
            }
        }

        //fgRet = _MwDmxOnPesNotify(u1Pidx, u4Data, pvTag);
        break;

    case DMX_NOTIFY_CODE_ES:
        fgRet = _MwDmxOnEsNotify(u1Pidx, u4Data, pvTag);
        break;

    case DMX_NOTIFY_CODE_OVERFLOW:
        {
            DMX_MW_MSG_T rMsg;

            rMsg.u1Pidx = u1Pidx;
            rMsg.u4Data = u4Data;
            rMsg.pvTag = (void *)pvTag;
            if (x_msg_q_send(_hDmxMwOVFLQueue, (VOID*)&rMsg, sizeof (DMX_MW_MSG_T),
                DMX_MSG_PRIORITY_NORMAL) != OSR_OK)
            {
                LOG(3, "Demux MW Overflow queue full!\n");
                fgRet = FALSE;
            }
            else
            {
                fgRet = TRUE;
            }
        }
        //fgRet = _MwDmxOnOverflowNotify(u1Pidx, u4Data, pvTag);
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
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
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
    if (!_MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_CALLBACK, &rPid))
    {
        return FALSE;
    }

    _MwDmxLock();
    _arNotifyInfo[u1Pidx].pf_demux_nfy = prNfyInfo->pf_demux_nfy;
    _arNotifyInfo[u1Pidx].pv_tag = prNfyInfo->pv_tag;
    _MwDmxUnlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MW_DMX_ResetCallback
 *  Reset callback handler
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
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
    if (!_MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_CALLBACK, &rPid))
    {
        return FALSE;
    }

    _MwDmxLock();
    _arNotifyInfo[u1Pidx].pf_demux_nfy = NULL;
    _arNotifyInfo[u1Pidx].pv_tag = NULL;
    _MwDmxUnlock();

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
        //u1StcId = STC_SRC_A2;
        /* PCR ID is mapped to STC_SRC_A1 */
        u1StcId = STC_SRC_A1;
        break;

    default:
        ///TODO: in mulit-instance how to control STC?
        u1StcId = STC_SRC_A1;
        break;
    }

    return u1StcId;
}


//-----------------------------------------------------------------------------
/** _MwDmxAllocateVideoPidBuffer
 *  Allocate buffer for video PID
 *
 *  @param  u1Pidx          PID index
 *  @param  ptDestInfo      Destination info
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
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
    ASSERT(prFbmPool->u4Addr != 0);

    // Setup buffer attributes
    rPid.fgAllocateBuffer = FALSE;
    rPid.u4BufAddr = prFbmPool->u4Addr;
    rPid.u4BufSize = prFbmPool->u4Size;
    rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;

    if (!_MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_BUFFER, &rPid))
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
    if (!_MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_DEVICE_ID, &rPid))
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
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxAllocateAudioPidBuffer(UINT8 u1Pidx,
    DEMUX_DEST_INFO_T* ptDestInfo)
{
    DMX_PID_T rPid;
    UINT32 au4BufStart[3], au4BufEnd[3]; // USE_FIFO3 : dec3 use fifo3, total buffer from 2->3
    UINT8 u1DeviceId;
#ifdef CC_MT5391_AUD_3_DECODER
    UINT8 u1AFifoId;
#endif

    ASSERT(ptDestInfo);

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    // Get audio buffer
#ifdef AUD_OLDFIFO_INTF
    if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
       &au4BufEnd[1]) != AUD_OK)
#else
    if ((AUD_GetAudFifo(0, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK) ||
        (AUD_GetAudFifo(1, &au4BufStart[1], &au4BufEnd[1]) != AUD_OK) ||
        (AUD_GetAudFifo(2, &au4BufStart[2], &au4BufEnd[2]) != AUD_OK))
#endif
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
#ifndef CC_MT5391_AUD_3_DECODER
    if(u1DeviceId >= 2)
    {
        return FALSE;
    }
    rPid.u4BufAddr = au4BufStart[u1DeviceId];
    rPid.u4BufSize = au4BufEnd[u1DeviceId] - au4BufStart[u1DeviceId];
#else

#ifdef CC_MT5391_AUD_MW_AUTO_CTRL
    // gallen, when input src is DTV, and MW connect dmx to dec1,
    // always force dmx to connect to dec2
    if (u1DeviceId==0) u1DeviceId=1;
#endif  // CC_MT5391_AUD_MW_AUTO_CTRL

    //gallen, decoder 1/3 both use AFIFO1, decoder 2 uses AFIFO2
    VERIFY(u1DeviceId < 3);

    // USE_FIFO3 : dec3 use fifo3
    // Dmx prepare 3 buffer.
    // But if audio decide to allocate fifo0 for device2, AUD_GetAudFifo will handle it. (au4BufStart[0] will be same as au4BufStart[2])
    //u1AFifoId = (UINT8)((u1DeviceId==1) ? 1:0);
    u1AFifoId = (UINT8)(u1DeviceId); // USE_FIFO3 : dec3 use fifo3

    if(u1AFifoId > 2)      // Make klocwork quiet
    {
        return FALSE;
    }
    rPid.u4BufAddr = au4BufStart[u1AFifoId];
    rPid.u4BufSize = au4BufEnd[u1AFifoId] - au4BufStart[u1AFifoId];
#endif  // CC_MT5391_AUD_3_DECODER

    rPid.fgAllocateBuffer = FALSE;
    rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
    rPid.u1DeviceId = u1DeviceId;
    if (!_MW_DMX_SetPid(u1Pidx, (UINT32)(DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_DEVICE_ID),
        &rPid))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwMPEGVideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwMPEGVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rMpvPes;
#ifdef CC_VDEC_FMT_DETECT
    UINT32 u4Addr;
#endif  // CC_VDEC_FMT_DETECT

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(rMpvPes));
    rMpvPes.fgSeqHdr = prPes->fgSeqHeader;

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

#ifdef CC_VDEC_FMT_DETECT
    case H264_PIC_TYPE_NONIDR:
    case H264_PIC_TYPE_SEQ:
        u4Addr = prPes->u4FrameAddr + 3;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        rMpvPes.ucPicType = *(UINT8*)VIRTUAL(u4Addr);
        break;
#endif  // CC_VDEC_FMT_DETECT

    default:
        return TRUE;
    }

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;
    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;
    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    //rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    rMpvPes.fgGop = prPes->fgGop;
    rMpvPes.fgEos = prPes->fgEOS;
    rMpvPes.u4DmxFrameType = prPes->u4FrameType;

    return (VDEC_SendEs(&rMpvPes) == 1);
}


//-----------------------------------------------------------------------------
/** _MwH264VideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwH264VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rMpvPes;
    UINT32 u4Addr;

    ASSERT(prPes != NULL);

    /*if(!H264_PIC_VALID_TYPE(prPes->u4FrameType))
    {
        return TRUE;
    }*/

    x_memset((void*)&rMpvPes, 0, sizeof(rMpvPes));

    u4Addr = prPes->u4FrameAddr + 3;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;

    u4Addr = prPes->u4FrameAddr + 4;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.u4VldReadPtr = u4Addr;

    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    //rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    //rMpvPes.fgGop = prPes->fgGop;

    rMpvPes.fgEos = prPes->fgEOS;
    rMpvPes.u1SliceCount = prPes->u1SliceCount;
    rMpvPes.u4DmxFrameType = prPes->u4FrameType;

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}


//-----------------------------------------------------------------------------
/** _MwH264VideoCallback
 *  Callback handler of video PES
 */
//-----------------------------------------------------------------------------
static BOOL _AVSVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T  rMpvPes;
    UINT32 u4Addr;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(rMpvPes));

    u4Addr = prPes->u4FrameAddr + 3;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;

    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;

    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    //rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    //rMpvPes.fgGop = prPes->fgGop;

    rMpvPes.fgEos = prPes->fgEOS;

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}


//-----------------------------------------------------------------------------
/** _MwDmxVideoCallback
 *  Callback handler of video PES
 *
 *  @param  prPes           PES message
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxVideoCallback(const DMX_PES_MSG_T* prPes)
{
    switch(prPes->eVideoType)
    {
    case DMX_VIDEO_MPEG:
        return _MwMPEGVideoCallback(prPes);

    case DMX_VIDEO_H264:
        return _MwH264VideoCallback(prPes);

    case DMX_VIDEO_AVS:
        return _AVSVideoCallback(prPes);

    default:
        Printf("Unknown video type\n");
        break;
    }

    return TRUE;
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
    UINT16 u2Pid;
    UINT8 u1PidIndex, u1StcId;

    ASSERT(pt_comp_id != NULL);

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    u1PidIndex = (UINT8)pt_comp_id->ui2_id;
    u2Pid = (UINT16)(MPEG_2_PID_T)(UINT32)pv_set_info;

    LOG(1, "Pidx: %u, pid: %u\n", u1PidIndex, u2Pid);

    switch (pt_comp_id->e_type)
    {
    case DRVT_DEMUX_TS_PCR:
        // No another PES entry with the same PID
        // Append PID
        ASSERT(u1Pidx == _MW_DMX_PhysicalPidx(u1PidIndex));

        UNUSED(DMX_SetStcSrc(u1Pidx, STC_SRC_NS));
        u1StcId = _MwDmxGetStcId(u1Pidx);
        UNUSED(DMX_SetStcSrc(u1Pidx, u1StcId));

        rPid.u2Pid = u2Pid;
        if (!_MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_PID, &rPid))
        {
            return RMR_DRV_INV_SET_INFO;
        }
        break;

    case DRVT_DEMUX_TS_PES_PACKET:
        rPid.u2Pid = u2Pid;
        if (!_MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_PID, &rPid))
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
    BOOL fgEnable;

    ASSERT(pt_comp_id != NULL);

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return RMR_DRV_INV_SET_INFO;
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
                if (!_MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_BUFFER, &rPid))
                {
                    return RMR_DRV_INV_SET_INFO;
                }
            }
        }
#ifdef ENABLE_MULTIMEDIA
        else if(pt_comp_id->e_type == DRVT_DEMUX_TS_PCR)
        {
            DMX_CONN_TYPE_T eType;
            _MwDmxLock();
            eType = _arDmxInfo[u1Pidx].eConType;
            _MwDmxUnlock();

            if(eType >= DMX_CONN_TYPE_BUFAGENT_0 && eType < DMX_CONN_TYPE_BUFAGENT_MAX)
            {
#ifndef CC_53XX_SWDMX_V2
                UNUSED(SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0));
#else
                UNUSED(SWDMX_SetInfo(_u1SwdmxSrcId, (UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0));
#endif
            }
        }
#endif
    }
    else
    {
#ifdef ENABLE_MULTIMEDIA
        if(pt_comp_id->e_type == DRVT_DEMUX_TS_PCR)
        {
            DMX_CONN_TYPE_T eType;
            _MwDmxLock();
            eType = _arDmxInfo[u1Pidx].eConType;
            _MwDmxUnlock();

            if(eType >= DMX_CONN_TYPE_BUFAGENT_0 && eType < DMX_CONN_TYPE_BUFAGENT_MAX)
            {
#ifndef CC_53XX_SWDMX_V2
                UNUSED(SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0));
#else
                UNUSED(SWDMX_SetInfo(_u1SwdmxSrcId, (UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0));
#endif
            }
        }
#endif
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
			// disable PID first before release a buffer,
			// or uP will contineous writing data to this buffer.
        	rPid.fgEnable = fgEnable;
        	if (!_MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        	{
            	i4Ret = RMR_DRV_INV_SET_INFO;
        	}

            if (pt_comp_id->e_type == DRVT_DEMUX_TS_PES_PACKET)
            {
                _MwDmxReleaseDefaultEsBuffer(rPid.u4BufAddr);
            }
        }
    }

    // Enable or disable a PID
    rPid.fgEnable = fgEnable;
    if (!_MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
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
    BOOL fgRet;
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
    MW_DMX_PID_INFO_T rPidInfo;

    ptDestInfo = (DEMUX_DEST_INFO_T*)pv_set_info;

    if (ptDestInfo == NULL)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    fgRet = TRUE;
    switch (ptDestInfo->e_type)
    {
    case DRVT_VID_DEC:
        fgRet = _MwDmxAllocateVideoPidBuffer(u1Pidx, ptDestInfo);
        break;

    case DRVT_AUD_DEC:
        fgRet = _MwDmxAllocateAudioPidBuffer(u1Pidx, ptDestInfo);
        break;

    default:
        fgRet = FALSE;
        break;
    }

    if(!fgRet)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    _MwDmxLock();
    rPidInfo = _arDmxInfo[u1Pidx];
    _MwDmxUnlock();

    if((INT32)rPidInfo.eConType >= DMX_CONN_TYPE_TUNER_0 && rPidInfo.eConType < DMX_CONN_TYPE_TUNER_MAX)
    {
        rDecoderCallbacks.pfnSendVideoPes = _MwDmxVideoCallback;
        rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
        rDecoderCallbacks.pfnSendAudioPes = AUD_SendAudioPes;
        DMX_SetDecoderCallbacks(&rDecoderCallbacks);
        DMX_SetToDecoder(TRUE);
    }

    UNUSED(pt_comp_id);

    return RMR_OK;
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
    DRV_COMP_ID_T* prConnectComp = 0;
    DMX_PID_T rPid;
    UINT32 u4Flags;
    UINT8 u1Pidx;

    // 1. Check para valid
    if ((NULL == pt_comp_id) || (NULL == pv_conn_info) )
    {
        return RMR_DRV_INV_CONN_INFO;
    }

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

    // Check component type
	prConnectComp = (DRV_COMP_ID_T*)pv_conn_info;
    u1Pidx = _MW_DMX_ConnectInfo(u1Pidx, (UINT32)pt_comp_id->e_type, prConnectComp->e_type,
        prConnectComp->ui2_id);
    if(u1Pidx == MW_DMX_INVALID_PIDX)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    // Construct a normal PID
    x_memset((void*)&rPid, 0, sizeof(rPid));
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
    rPid.fgPrimary = TRUE;

    if (pt_comp_id->e_type == DRVT_DEMUX_TS_PCR)
    {
        UINT8 u1StcId;

        // Set mapping of PID index and STC id
        u1StcId = _MwDmxGetStcId(u1Pidx);
        UNUSED(DMX_SetStcSrc(u1Pidx, u1StcId));

        rPid.ePcrMode = (u1StcId == STC_SRC_A1) ? DMX_PCR_MODE_NEW :
            DMX_PCR_MODE_OLD;
        rPid.u1SteerMode = 0;
        rPid.fgPrimary = FALSE;
    }

    // Set these flags only. Others will be set by 'set' command directly
    u4Flags = (UINT32)(DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_KEY_INDEX |
        DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_STEER | DMX_PID_FLAG_PCR |
        DMX_PID_FLAG_CALLBACK | DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_PRIMARY);

    if (!_MW_DMX_SetPid(u1Pidx, u4Flags, &rPid))
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
    UINT8 u1Pidx;

    // Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_disc_info) )
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    // Check component type
    // ...

    // Check index range
    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    // Check frontend component
    // ...

    u1Pidx = _MW_DMX_PhysicalPidx(u1Pidx);

    if (!DMX_FreePid(u1Pidx))
    {
        // Release a PID
        return RMR_DRV_DISC_FAILED;
    }

    // Reset scramble state callback
    if (!_MwDmxResetScrambleCallback(u1Pidx))
    {
        return RMR_DRV_DISC_FAILED;
    }

    if (pt_comp_id->e_type == DRVT_DEMUX_TS_PCR)
    {
        // Reset mapping of PID index and STC id by setting an invalid STC id
        UNUSED(DMX_SetStcSrc(u1Pidx, STC_SRC_NS));
    }

    _MW_DMX_DisConnectInfo((UINT8)pt_comp_id->ui2_id);

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
                        DRV_GET_TYPE_T          e_get_type,
                        VOID*                   pv_get_info,
                        SIZE_T*                 pz_get_info_len)
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

    u1Pidx = _MW_DMX_PhysicalPidx(u1Pidx);

    if(!_MW_DMX_IsPidConnedted(u1Pidx))
    {
        return RMR_DRV_INV_GET_INFO;
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

        x_memset((void*)&rStcClk, 0, sizeof(rStcClk));

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
                    DRV_COMP_ID_T*      pt_comp_id,
                    DRV_SET_TYPE_T      e_set_type,
                    const VOID*         pv_set_info,
                    SIZE_T              z_set_info_len)
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

    u1Pidx = _MW_DMX_PhysicalPidx(u1Pidx);

    if(!_MW_DMX_IsPidConnedted(u1Pidx))
    {
        return RMR_DRV_INV_SET_INFO;
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
/** _MW_DMX_CISetFrontEnd
 * This function store CI setting, then used for MM -> DMX for re-initialize
 */
//-----------------------------------------------------------------------------
BOOL _MW_DMX_CISetFrontEnd(DMX_FRONTEND_T eFEType)
{
    UNUSED(eFEType);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MW_DMX_ConnectInfo
 *
 *  @param  u1VirtualPidx          Virtual PID index
 *
 */
//-----------------------------------------------------------------------------
UINT8 _MW_DMX_ConnectInfo(UINT8 u1VirtualPidx, UINT32 u4RMType, DRV_TYPE_T eType, UINT16 u2Id)
{
    DMX_CONN_TYPE_T eConnType;
    UINT8 u1Inst = MW_DMX_INVALID_INST, u1PhysicalPidx = MW_DMX_INVALID_PIDX;

    if(u1VirtualPidx >= DMX_NUM_PID_INDEX)
    {
        return u1PhysicalPidx;
    }

    if (eType == DRVT_BUF_AGENT)
    {
        if(u2Id >= DMX_CONN_TYPE_BUFAGENT_MAX - DMX_CONN_TYPE_BUFAGENT_0)
        {
            LOG(3, "Connet Id is over the max number.\n");
            return u1PhysicalPidx;
        }

        eConnType = (DMX_CONN_TYPE_T)(DMX_CONN_TYPE_BUFAGENT_0 + u2Id);
    }
    else
    {
        if(u2Id >= DMX_CONN_TYPE_TUNER_MAX - DMX_CONN_TYPE_TUNER_0)
        {
            LOG(3, "Connet Id is over the max number.\n");
            return u1PhysicalPidx;
    }

        eConnType = (DMX_CONN_TYPE_T)(DMX_CONN_TYPE_TUNER_0 + u2Id);
    }

    u1Inst = DMX_MUL_GetAvailableInst(eConnType);
    if(u1Inst == MW_DMX_INVALID_INST)
    {
        LOG(3, "Instances are all occupied.\n");
        return u1PhysicalPidx;
    }

    u1PhysicalPidx = DMX_MUL_GetAvailablePidx(u1Inst);
    if(u1PhysicalPidx == MW_DMX_INVALID_PIDX)
    {
        LOG(3, "Pidx are all occupied.\n");
        return u1PhysicalPidx;
    }

    _MwDmxLock();
    _arDmxInfo[u1PhysicalPidx].fgValid = TRUE;
    _arDmxInfo[u1PhysicalPidx].u4RMType = u4RMType;
    _arDmxInfo[u1PhysicalPidx].eConType = eConnType;
    _arDmxInfo[u1PhysicalPidx].u1VirtualPidx = u1VirtualPidx;
    _MwDmxUnlock();

    return u1PhysicalPidx;
}


//-----------------------------------------------------------------------------
/** _MW_DMX_IsPidConnedted
 */
//-----------------------------------------------------------------------------
BOOL _MW_DMX_IsPidConnedted(UINT8 u1Pidx)
{
    BOOL fgValid;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    _MwDmxLock();
    fgValid = _arDmxInfo[u1Pidx].fgValid;
    _MwDmxUnlock();

    return fgValid;
}


//-----------------------------------------------------------------------------
/** _MW_DMX_DisConnectInfo
 *
 *  @param  u1VirtualPidx          Virtual PID index
 *
 */
//-----------------------------------------------------------------------------
void _MW_DMX_DisConnectInfo(UINT8 u1VirtualPidx)
{
    UINT8 u1Pidx;

    if(u1VirtualPidx >= DMX_NUM_PID_INDEX)
    {
        return;
    }

    u1Pidx = _MW_DMX_PhysicalPidx(u1VirtualPidx);

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return;
    }

    _MwDmxLock();
    _arDmxInfo[u1Pidx].fgValid = FALSE;
    _arDmxInfo[u1Pidx].u4RMType = 0;
    _arDmxInfo[u1Pidx].eConType = DMX_CONN_TYPE_BUFAGENT_0;
    _MwDmxUnlock();

    UNUSED(DMX_MUL_FreePidx(u1Pidx));
}


//-----------------------------------------------------------------------------
/** _MW_DMX_FlushAllCachePid
 * Tricky for MM TS file
 */
//-----------------------------------------------------------------------------

BOOL _MW_DMX_FlushAllCachePid(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MW_DMX_SetCacheOn
 */
//-----------------------------------------------------------------------------
void _MW_DMX_SetCacheOn(BOOL fgCacheOn)
{
    UNUSED(fgCacheOn);
}


//-----------------------------------------------------------------------------
/** _MW_DMX_GetPidConType
 */
//-----------------------------------------------------------------------------
DMX_CONN_TYPE_T _MW_DMX_GetPidConType(UINT8 u1Pidx)
{
    DMX_CONN_TYPE_T eConType;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return DMX_CONN_TYPE_NONE;
    }

    _MwDmxLock();
    eConType = _arDmxInfo[u1Pidx].eConType;
    _MwDmxUnlock();

    return eConType;
}


//-----------------------------------------------------------------------------
/** _MW_DMX_PhysicalPidx
 *
 *  @param  u1VirtualPidx          Virtual PID index
 *
 */
//-----------------------------------------------------------------------------
UINT8 _MW_DMX_PhysicalPidx(UINT8 u1VirtualPidx)
{
    UINT8 i;

    for(i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if((_arDmxInfo[i].fgValid == TRUE) && (_arDmxInfo[i].u1VirtualPidx == u1VirtualPidx))
        {
            break;
        }
    }

    if(i >= DMX_NUM_PID_INDEX)
    {
        return MW_DMX_INVALID_PIDX;
    }

    return i;
}


//-----------------------------------------------------------------------------
/** _MW_DMX_SetPid
 */
//-----------------------------------------------------------------------------
BOOL _MW_DMX_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid)
{
    DMX_PID_T rPid;

    MW_DMX_PID_INFO_T rPidInfo;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    _MwDmxLock();
    rPidInfo = _arDmxInfo[u1Pidx];
    _MwDmxUnlock();

    if(!rPidInfo.fgValid)
    {
        // set pid before connect, so return false
        LOG(5, "%s set pid before connect\n", __FUNCTION__);
        return FALSE;
    }

    rPid = *prPid;

    rPid.u1ChannelId = 0;       // Force to 0

#ifndef ENABLE_MULTIMEDIA
    return DMX_SetPidEx(u1Pidx, u4Flags, &rPid, DMX_SOURCE_DTV0);
#else
    if((INT32)rPidInfo.eConType >= DMX_CONN_TYPE_TUNER_0 && rPidInfo.eConType < DMX_CONN_TYPE_TUNER_MAX)
    {
        UINT8 i;
        DMX_PID_T rPidx;

        // check if this is the first SetPid. If yes, set the video type to MPEG2 before SetPid
        for(i =0; i< DMX_NUM_PID_INDEX; i++)
        {
            if(DMX_GetPid(i, (DMX_PID_FLAG_VALID), &rPidx))
            {
                if(rPidx.fgEnable == TRUE)
                {
                    break;
                }
            }
        }

        if(i >= DMX_NUM_PID_INDEX)
        {
            DMX_SetVideoType(DMX_VIDEO_MPEG);
        }

        ///TODO: extend soource
        return DMX_SetPidEx(u1Pidx, u4Flags, &rPid, DMX_SOURCE_DTV0);
    }
    else if(rPidInfo.eConType >= DMX_CONN_TYPE_BUFAGENT_0 && rPidInfo.eConType < DMX_CONN_TYPE_BUFAGENT_MAX)
    {
        ///TODO: extend soource
        return DMX_SetPidEx(u1Pidx, u4Flags, &rPid, DMX_SOURCE_TSFILE0);
    }
    return FALSE;
#endif  // ENABLE_MULTIMEDIA
}


//-----------------------------------------------------------------------------
/** _MW_DMX_DESC_Init
 *  Initialize demux record driver for middleware interface
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
#ifdef CI_PLUS_SUPPORT

static DMX_MW_DESC_INFO_T _arDmxDesc[DMX_DESC_COMP_NUM];

static BOOL _IsOtherDescHasSamePidValue(UINT8 u1CompId)
{
    UINT8 i = 0;

    //Search all desc has the same pid value
    for(i = 0; i < DMX_DESC_COMP_NUM; i++)
    {
        if(i != u1CompId && _arDmxDesc[i].fgEnable == TRUE)
        {
            if(_arDmxDesc[i].u2PidNum == _arDmxDesc[u1CompId].u2PidNum)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

static BOOL _MwDmxOnEnableDesc(UINT8 u1CompId, const VOID* pv_set_info)
{
    DMX_PID_T rPid;
    BOOL fgEnable = (BOOL)(UINT32)pv_set_info;
    INT32 i = 0;

    if (u1CompId >= DMX_DESC_COMP_NUM)
    {
        return FALSE;
    }

    x_memset((void*)&rPid, 0, sizeof(rPid));

    if(fgEnable)
    {
        _arDmxDesc[u1CompId].fgEnable = TRUE;

        //Search all Pidex that match pid number
        for(i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if(DMX_GetPid(i, (DMX_PID_FLAG_PID | DMX_PID_FLAG_TS_INDEX), &rPid))
            {
                if(rPid.u2Pid == _arDmxDesc[u1CompId].u2PidNum && rPid.u1TsIndex != 2)
                {
                    //Setup Descrambler
                    if(_arDmxDesc[u1CompId].rDescAlg == DESC_ALG_DES_ECB)
                    {
                        DMX_SetDesKey(DMX_CIPLUS_KEY_INDEX, FALSE, _arDmxDesc[u1CompId].fgEven, _arDmxDesc[u1CompId].au4Key);
                        rPid.eDescMode = DMX_DESC_MODE_DES_ECB;
                    }
                    else if(_arDmxDesc[u1CompId].rDescAlg == DESC_ALG_DES_CBC)
                    {
                        DMX_SetDesIV(_arDmxDesc[u1CompId].au4Iv[1], _arDmxDesc[u1CompId].au4Iv[0]);
                        DMX_SetDesKey(DMX_CIPLUS_KEY_INDEX, FALSE, _arDmxDesc[u1CompId].fgEven, _arDmxDesc[u1CompId].au4Key);
                        rPid.eDescMode = DMX_DESC_MODE_DES_CBC;
                    }
                    else if(_arDmxDesc[u1CompId].rDescAlg == DESC_ALG_3DES_ECB)
                    {
                        DMX_SetDesKey(DMX_CIPLUS_KEY_INDEX, TRUE, _arDmxDesc[u1CompId].fgEven, _arDmxDesc[u1CompId].au4Key);
                        rPid.eDescMode = DMX_DESC_MODE_3DES_ECB;
                    }
                    else if(_arDmxDesc[u1CompId].rDescAlg == DESC_ALG_3DES_CBC)
                    {
                        DMX_SetDesIV(_arDmxDesc[u1CompId].au4Iv[1], _arDmxDesc[u1CompId].au4Iv[0]);
                        DMX_SetDesKey(DMX_CIPLUS_KEY_INDEX, TRUE, _arDmxDesc[u1CompId].fgEven, _arDmxDesc[u1CompId].au4Key);
                        rPid.eDescMode = DMX_DESC_MODE_3DES_CBC;
                    }
                    else if(_arDmxDesc[u1CompId].rDescAlg == DESC_ALG_DVB)
                    {
                        DMX_SetDesIV(_arDmxDesc[u1CompId].au4Iv[1], _arDmxDesc[u1CompId].au4Iv[0]);
                        DMX_SetDesKey(DMX_CIPLUS_KEY_INDEX, FALSE, _arDmxDesc[u1CompId].fgEven, _arDmxDesc[u1CompId].au4Key);
                        rPid.eDescMode = DMX_DESC_MODE_DVB;
                    }
                    else if(_arDmxDesc[u1CompId].rDescAlg == DESC_ALG_AES_128_ECB)
                    {
                        DMX_SetAesKeyLen(DMX_CIPLUS_KEY_INDEX, 128);
                        DMX_SetAesKey(DMX_CIPLUS_KEY_INDEX, _arDmxDesc[u1CompId].fgEven,  _arDmxDesc[u1CompId].au4Key);
                        rPid.eDescMode = DMX_DESC_MODE_AES_ECB;
                    }
                    else if(_arDmxDesc[u1CompId].rDescAlg == DESC_ALG_AES_128_CBC)
                    {
                        DMX_SetAesIVEx(_arDmxDesc[u1CompId].au4Iv, _arDmxDesc[u1CompId].fgEven);
                        DMX_SetAesKeyLen(DMX_CIPLUS_KEY_INDEX, 128);
                        DMX_SetAesKey(DMX_CIPLUS_KEY_INDEX, _arDmxDesc[u1CompId].fgEven,  (UINT32*)_arDmxDesc[u1CompId].au4Key);
                        rPid.eDescMode = DMX_DESC_MODE_AES_CBC;
                        LOG(3, "%s, CBC, even:%d, val:0x%x\n", __FUNCTION__, (UINT32)_arDmxDesc[u1CompId].fgEven,
                                    _arDmxDesc[u1CompId].au4Key[0]);
                    }
                    else
                    {
                        rPid.eDescMode = DMX_DESC_MODE_NONE;
                    }

                    rPid.u1KeyIndex = DMX_CIPLUS_KEY_INDEX;
                    DMX_SetPid(i, DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_KEY_INDEX, &rPid);

                    LOG(3, "%s, set key idx:%d\n", __FUNCTION__, DMX_CIPLUS_KEY_INDEX);
                }
            }
        }
    }
    else
    {
        _arDmxDesc[u1CompId].fgEnable = FALSE;

        //Search all Pidex that match pid number
        for(i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if(DMX_GetPid(i, (DMX_PID_FLAG_PID | DMX_PID_FLAG_TS_INDEX), &rPid))
            {
                if(rPid.u2Pid == _arDmxDesc[u1CompId].u2PidNum && rPid.u1TsIndex != 2)
                {
                    // Only set descramble mode to none when this is the last descrambler
                    // map to this pid value
                    if(!_IsOtherDescHasSamePidValue(u1CompId))
                    {
                        //Stop Descrambler
                        rPid.eDescMode = DMX_DESC_MODE_NONE;
                        DMX_SetPid(i, DMX_PID_FLAG_DESC_MODE, &rPid);
                    }
                }
            }
        }
    }

    return TRUE;
}
//-----------------------------------------------------------------------------
/** _MwDmxDesc_Connect
 *  Middleware interface - connect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxDesc_Connect(
                            const DRV_COMP_ID_T*   pt_comp_id,
                            DRV_CONN_TYPE_T  e_conn_type,
                            const VOID*      pv_conn_info,
                            SIZE_T           z_conn_info_len,
                            VOID*            pv_tag,
                            x_rm_nfy_fct     pf_nfy)
{
    UINT8 u1CompId;

    if ((NULL == pt_comp_id))
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    // Check index range
    u1CompId = (UINT8)pt_comp_id->ui2_id;
    if (u1CompId >= DMX_DESC_COMP_NUM)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    _arDmxDesc[u1CompId].fgConnented = TRUE;

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxDesc_Disconnect
 *  Middleware interface - disconnect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxDesc_Disconnect(
                            const DRV_COMP_ID_T*    pt_comp_id,
                            DRV_DISC_TYPE_T         e_disc_type,
                            const VOID*             pv_disc_info,
                            SIZE_T                  z_disc_info_len)
{
    UINT8 u1CompId;

    // Check parameters
    if ((NULL == pt_comp_id))
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    // Check index range
    u1CompId = (UINT8)pt_comp_id->ui2_id;
    if (u1CompId >= DMX_DESC_COMP_NUM)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    _arDmxDesc[u1CompId].fgConnented = FALSE;
    _arDmxDesc[u1CompId].u2PidNum = 0x1FFF;

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxDesc_Get
 *  Middleware interface - get
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxDesc_Get(
                        const DRV_COMP_ID_T*    pt_comp_id,
                        DRV_GET_TYPE_T          e_get_type,
                        VOID*                   pv_get_info,
                        SIZE_T*                 pz_get_info_len)
{
    UINT8 u1CompId = 0;
    BOOL fgRet = FALSE;

    // Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_get_info) ||
        (NULL == pz_get_info_len))
    {
       	return RMR_DRV_INV_GET_INFO;
    }

    // Check index range
    u1CompId = (UINT8)pt_comp_id->ui2_id;
    if (u1CompId >= DMX_DESC_COMP_NUM)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    if(!_arDmxDesc[u1CompId].fgConnented)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    switch (e_get_type)
    {
    case DESC_GET_TYPE_CTRL:
        *(BOOL*)pv_get_info = _arDmxDesc[u1CompId].fgEnable;
        *pz_get_info_len = sizeof(BOOL);
        fgRet = TRUE;
        break;

    case DESC_GET_TYPE_ALGORITHM:
        // Get descrambler mode value
        // Notice: We must make sure DESC_ALG_T
        // and DMX_DESC_MODE_T have the same enum order
        *(DESC_ALG_T*)pv_get_info = _arDmxDesc[u1CompId].rDescAlg;
        *pz_get_info_len = sizeof(DESC_ALG_T);
        fgRet = TRUE;
        break;

    case DESC_GET_TYPE_PID:
        // Get PID value
        *(MPEG_2_PID_T*)pv_get_info = _arDmxDesc[u1CompId].u2PidNum;
        *pz_get_info_len = sizeof(MPEG_2_PID_T);
        fgRet = TRUE;
        break;

    default:
        break;
    }

    return (fgRet ? RMR_OK : RMR_DRV_INV_GET_INFO);
}


//-----------------------------------------------------------------------------
/** _MwDmxDesc_Set
 *  Middleware interface - set
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxDesc_Set(
                    DRV_COMP_ID_T*      pt_comp_id,
                    DRV_SET_TYPE_T      e_set_type,
                    const VOID*         pv_set_info,
                    SIZE_T              z_set_info_len)
{
    DESC_KEY_INFO_T *prKeyInfo;
    UINT8 u1CompId = 0;
    BOOL fgRet = FALSE;


    if ((NULL == pt_comp_id))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    u1CompId = (UINT8)pt_comp_id->ui2_id;
    if (u1CompId >= DMX_DESC_COMP_NUM)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    if(!_arDmxDesc[u1CompId].fgConnented)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    switch (e_set_type)
    {
    case DESC_SET_TYPE_CTRL:
        //Set all descrambler config in one time
        fgRet = _MwDmxOnEnableDesc(u1CompId, pv_set_info);
        break;

    case DESC_SET_TYPE_ALGORITHM:
        _arDmxDesc[u1CompId].rDescAlg = *(DESC_ALG_T*)pv_set_info;
        fgRet = TRUE;
        break;

    case DESC_SET_TYPE_PID:
        _arDmxDesc[u1CompId].u2PidNum = (UINT16)(MPEG_2_PID_T)(UINT32)pv_set_info;
        fgRet = TRUE;
        break;

    case DESC_SET_TYPE_ODD_KEY:
        _arDmxDesc[u1CompId].fgEven = FALSE;
        prKeyInfo = (DESC_KEY_INFO_T*)pv_set_info;
        if(prKeyInfo->e_descrambler_alg == DESC_ALG_DES_ECB)
        {
            _arDmxDesc[u1CompId].u4KeyLen = (UINT32)prKeyInfo->u_key_info.t_des_ecb_key.z_key_len;
            ASSERT(_arDmxDesc[u1CompId].u4KeyLen <= 24);
            DataSwap((UINT32 *)_arDmxDesc[u1CompId].au4Key, (UINT32 *)prKeyInfo->u_key_info.t_des_ecb_key.pui1_key, _arDmxDesc[u1CompId].u4KeyLen/4, DMX_CI_DES_KEY_SWAP_MODE);
        }
        else if(prKeyInfo->e_descrambler_alg == DESC_ALG_AES_128_CBC)
        {
            UINT32 au4Key[4];
            UINT32 au4Iv[4];
            x_memcpy(au4Key, prKeyInfo->u_key_info.t_aes_128_cbc_key.pui1_key, 16);
            x_memcpy(au4Iv, prKeyInfo->u_key_info.t_aes_128_cbc_key.pui1_iv, 16);

            _arDmxDesc[u1CompId].u4KeyLen = (UINT32)prKeyInfo->u_key_info.t_aes_128_cbc_key.z_key_len;
            ASSERT(_arDmxDesc[u1CompId].u4KeyLen <= 32);
            DataSwap((UINT32 *)_arDmxDesc[u1CompId].au4Key, (UINT32 *)au4Key, _arDmxDesc[u1CompId].u4KeyLen/4, DMX_CI_AES_KEY_SWAP_MODE);
            ASSERT((UINT32)prKeyInfo->u_key_info.t_aes_128_cbc_key.z_iv_len <= 16);
            DataSwap((UINT32 *)_arDmxDesc[u1CompId].au4Iv, (UINT32 *)au4Iv, prKeyInfo->u_key_info.t_aes_128_cbc_key.z_iv_len/4, DMX_CI_AES_IV_SWAP_MODE);
        }
        else
        {
            fgRet = FALSE;  //Not implemented yet
            break;
        }

        fgRet = TRUE;
        break;

    case DESC_SET_TYPE_EVEN_KEY:

        _arDmxDesc[u1CompId].fgEven = TRUE;
        prKeyInfo = (DESC_KEY_INFO_T*)pv_set_info;
        if(prKeyInfo->e_descrambler_alg == DESC_ALG_DES_ECB)
        {
            _arDmxDesc[u1CompId].u4KeyLen = (UINT32)prKeyInfo->u_key_info.t_des_ecb_key.z_key_len;
            ASSERT(_arDmxDesc[u1CompId].u4KeyLen <= 24);
            DataSwap((UINT32 *)_arDmxDesc[u1CompId].au4Key, (UINT32 *)prKeyInfo->u_key_info.t_des_ecb_key.pui1_key, _arDmxDesc[u1CompId].u4KeyLen, DMX_CI_DES_KEY_SWAP_MODE);
        }
        else if(prKeyInfo->e_descrambler_alg == DESC_ALG_AES_128_CBC)
        {
            UINT32 au4Key[4];
            UINT32 au4Iv[4];
            x_memcpy(au4Key, prKeyInfo->u_key_info.t_aes_128_cbc_key.pui1_key, 16);
            x_memcpy(au4Iv, prKeyInfo->u_key_info.t_aes_128_cbc_key.pui1_iv, 16);

            _arDmxDesc[u1CompId].u4KeyLen = (UINT32)prKeyInfo->u_key_info.t_aes_128_cbc_key.z_key_len;
            ASSERT(_arDmxDesc[u1CompId].u4KeyLen <= 32);
            DataSwap((UINT32 *)_arDmxDesc[u1CompId].au4Key, (UINT32 *)au4Key, _arDmxDesc[u1CompId].u4KeyLen/4, DMX_CI_AES_KEY_SWAP_MODE);
            ASSERT((UINT32)prKeyInfo->u_key_info.t_aes_128_cbc_key.z_iv_len <= 16);
            DataSwap((UINT32 *)_arDmxDesc[u1CompId].au4Iv, (UINT32 *)au4Iv, prKeyInfo->u_key_info.t_aes_128_cbc_key.z_iv_len/4, DMX_CI_AES_IV_SWAP_MODE);
        }
        else
        {
            fgRet = FALSE;  //Not implemented yet
            break;
        }

        fgRet = TRUE;
        break;

    default:
        break;
    }

    return (fgRet ? RMR_OK : RMR_DRV_INV_SET_INFO);
}

BOOL _MW_DMX_DESC_Init(void)
{
    ID_IND_T at_id_ind[DMX_DESC_COMP_NUM];
    DRV_COMP_REG_T t_comp_id;
    DRV_COMP_FCT_TBL_T t_comp_fct_tbl;
    UINT32 i, u4Flags;
    INT32 i4Ret;

    for(i=0; i<DMX_DESC_COMP_NUM; i++)
    {
        _arDmxDesc[i].fgConnented = FALSE;
        _arDmxDesc[i].u2PidNum = 0x1FFF;
    }

    //---------------------------------------------------------
    // Register Descrambler components
    //---------------------------------------------------------
    t_comp_fct_tbl.pf_rm_connect = (x_rm_connect_fct)_MwDmxDesc_Connect;
    t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwDmxDesc_Disconnect;
    t_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)_MwDmxDesc_Get;
    t_comp_fct_tbl.pf_rm_set = (x_rm_set_fct)_MwDmxDesc_Set;
    t_comp_id.e_type = DRVT_DESCRAMBLER;
    t_comp_id.e_id_type = ID_TYPE_LIST;
    t_comp_id.u.t_list.ui2_num_of_ids = DMX_DESC_COMP_NUM;
    t_comp_id.u.t_list.pt_list = at_id_ind;

    for (i = 0; i < DMX_DESC_COMP_NUM; i++)
    {
        t_comp_id.u.t_list.pt_list[i].ui2_id = i;
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

    return TRUE;
}
#endif  // CI_PLUS_SUPPORT

//-----------------------------------------------------------------------------
/** MW_DMX_Init
 *  Initialize demux driver for middleware interface
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
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
    static BOOL _fgInit = FALSE;

    // Initlialize driver
    if (!DMX_Init())
    {
        return FALSE;
    }

    _MwDmxLock();
    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        _arNotifyInfo[i].pf_demux_nfy = NULL;
        _arNotifyInfo[i].pv_tag = NULL;
        _arDmxInfo[i].eConType = DMX_CONN_TYPE_NONE;
        _arDmxInfo[i].u4RMType = 0;
        _arDmxInfo[i].fgValid = FALSE;
        _arDmxInfo[i].u1VirtualPidx = MW_DMX_INVALID_PIDX;
    }
    _MwDmxUnlock();

    _MW_DMX_SetCacheOn(TRUE);

    // Set decoder callback functions
    rDecoderCallbacks.pfnSendVideoPes = _MwDmxVideoCallback;
    rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
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
        t_comp_id.u.t_list.pt_list[i].ui2_id = (UINT16)(DMX_PES_COMP_ID_START + i);
        t_comp_id.u.t_list.pt_list[i].pv_tag = NULL;
        t_comp_id.u.t_list.pt_list[i].ui1_port = ALL_PORTS;
    }

    //u4Flags = DRV_FLAG_SINGLE_CONN_ON_OUTPUT;
    u4Flags = 0;

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
        t_comp_id.u.t_list.pt_list[i].ui2_id = (UINT16)(DMX_PCR_COMP_ID_START + i);
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
#endif  // 0

#if 1
    // Register PSI components
    if (!_MW_DMX_PSI_Init())
    {
        return FALSE;
    }
#endif  // 1

#if 1
    // Register PES components
    if (!_MW_DMX_PES_Init())
    {
        return FALSE;
    }
#endif  // 1

#ifdef ENABLE_MULTIMEDIA
    // Register MM components
    if (!_MW_DMX_MM_Init())
    {
        return FALSE;
    }

#ifdef TIME_SHIFT_SUPPORT
    if(!_MwPVRResetPlayKeyInfo())
    {
        return FALSE;
    }
#endif  // TIME_SHIFT_SUPPORT
#endif  // ENABLE_MULTIMEDIA

#ifdef CI_PLUS_SUPPORT
    // Initialize descrambler
    if(!_MW_DMX_DESC_Init())
    {
        return FALSE;
    }
#endif  // CI_PLUS_SUPPORT

    if (!_fgInit)
    {
        // Create picture queues
        if (x_msg_q_create(&_hDmxMwPSIQueue, DMX_MW_PSI_QUEUE_NAME,
            sizeof (DMX_MW_PSI_MSG_T), DMX_MW_PSI_QUEUE_SIZE) != OSR_OK)
        {
            LOG(1, "Fail to create picture queue!\n");
            return FALSE;
        }

        // Create PSI thread
        if (x_thread_create(&_hDmxMwPSIThread, DMX_MW_PSI_THREAD_NAME,
            DMX_MW_PSI_THREAD_STACK_SIZE, DMX_MW_PSI_THREAD_PRIORITY, _MwDmxOnPsiHandler, 0,
            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create DMX thread!\n");
            return FALSE;
        }

        // Create PES queues
        if (x_msg_q_create(&_hDmxMwPESQueue, DMX_MW_PES_QUEUE_NAME,
            sizeof (DMX_MW_PES_MSG_T), DMX_MW_PES_QUEUE_SIZE) != OSR_OK)
        {
            LOG(1, "Fail to create picture queue!\n");
            return FALSE;
        }

        // Create PES thread
        if (x_thread_create(&_hDmxMwPESThread, DMX_MW_PES_THREAD_NAME,
            DMX_MW_PES_THREAD_STACK_SIZE, DMX_MW_PES_THREAD_PRIORITY, _MwDmxOnPesHandler, 0,
            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create DMX thread!\n");
            return FALSE;
        }

        // Create Overflow queues
        if (x_msg_q_create(&_hDmxMwOVFLQueue, DMX_MW_OVFL_QUEUE_NAME,
            sizeof (DMX_MW_MSG_T), DMX_MW_OVFL_QUEUE_SIZE) != OSR_OK)
        {
            LOG(1, "Fail to create picture queue!\n");
            return FALSE;
        }

        // Create Overflow thread
        if (x_thread_create(&_hDmxMwOVFLThread, DMX_MW_OVFL_THREAD_NAME,
            DMX_MW_OVFL_THREAD_STACK_SIZE, DMX_MW_OVFL_THREAD_PRIORITY, _MwDmxOnOverflowHandler, 0,
            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create DMX thread!\n");
            return FALSE;
        }

        _fgInit = TRUE;
    }

    return TRUE;
}


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
 * $RCSfile: $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_pure_audio.c
 *  Demux driver for supporting pure audio without making use of DMX hardware.
 */
#include "dmx_if.h"

#ifdef CC_DMX_PURE_AUDIO


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"

#include "x_lint.h"

//LINT_EXT_HEADER_BEGIN

#include "mpv_drvif.h"
#include "aud_drvif.h"

//LINT_EXT_HEADER_END

#include "x_assert.h"
#include "x_os.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_timer.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MM_BUF_FULL_GAP                 ((188*4) + 32)
#define DMX_PURE_AUDIO_PIDX_UNUSED              0xFF


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define CHECK_INDEX_RANGE(u1Idx)                        \
    do {                                                \
        if ((u1Idx) >= DMX_NUM_PID_INDEX)               \
        {                                               \
            LOG(3, "Invalid index (%d)!\n", (u1Idx));   \
            return FALSE;                               \
        }                                               \
    } while (0);


#define CHECK_FOR_NULL_POINTER(u4Ptr)                   \
    do {                                                \
        if ((void*)(u4Ptr) == NULL)                     \
        {                                               \
            LOG(3, "Null pointer is used!\n");          \
            return FALSE;                               \
        }                                               \
    } while (0);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static PID_STRUCT_T _rPureAudioPidStruct;
static UINT8 _u1PureAudioPidx;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// _Dmx_PURE_SetPidBuffer
//-----------------------------------------------------------------------------
static BOOL _Dmx_PURE_SetPidBuffer(UINT8 u1Idx, const DMX_MM_T *prMM,
                                    PID_STRUCT_T *prPidStruct)
{
    UINT32 u4BufStart, u4BufSize;

    if ((prPidStruct->u4BufStart != 0) || (prPidStruct->u4BufEnd != 0))
    {
        LOG(5, "Buffer had been allocated.\n");
        return FALSE;
    }

    u4BufSize = prMM->u4BufSize;
    if (prMM->fgAllocBuf)
    {
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(u4BufSize, DMX_ES_FIFO_ALIGNMENT);
        if (u4BufStart == 0)
        {
            LOG(3, "Memory allocation failed!\n");
            return FALSE;
        }
        UNUSED(u1Idx);
        // _DMX_SetMemoryUsage(u1Idx, u4BufSize);
    }
    else
    {
        u4BufStart = prMM->u4BufAddr;
    }

    _DMX_Lock();
    u4BufStart = VIRTUAL(u4BufStart);
    prPidStruct->u4BufStart = u4BufStart;
    prPidStruct->u4BufLen = u4BufSize;
    prPidStruct->u4BufEnd = (u4BufStart + u4BufSize) - 1;
    prPidStruct->u4Wp = u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4SectionRp = u4BufStart;
    prPidStruct->u4PesRp = u4BufStart;
    prPidStruct->fgAllocateBuffer = prMM->fgAllocBuf;
    prPidStruct->u4HeaderBufAddr = 0;
    prPidStruct->u4NextPic = 0;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_PURE_Init
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PURE_Init(void)
{
    PID_STRUCT_T rPidStruct;

    x_memset((void*)&rPidStruct, 0, sizeof(rPidStruct));

    _DMX_Lock();
    _rPureAudioPidStruct = rPidStruct;
    _u1PureAudioPidx = DMX_PURE_AUDIO_PIDX_UNUSED;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PURE_IsPureAudio
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PURE_IsPureAudio(UINT8 u1Idx)
{
    UINT8 u1PureAudioPidx;
    PID_STRUCT_T rPidStruct;

    _DMX_Lock();
    u1PureAudioPidx = _u1PureAudioPidx;
    rPidStruct = _rPureAudioPidStruct;
    _DMX_Unlock();

    if (u1PureAudioPidx != DMX_PURE_AUDIO_PIDX_UNUSED)
    {
        if ((u1PureAudioPidx == u1Idx) &&
            (rPidStruct.ePidType == DMX_PID_TYPE_PURE_AUDIO))
        {
            return TRUE;
        }
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _DMX_PURE_Set
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PURE_Set(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_T *prMM)
{
    UINT8 u1PureAudioPidx;
    PID_STRUCT_T rPidStruct;

    CHECK_INDEX_RANGE(u1Idx);
    CHECK_FOR_NULL_POINTER(prMM);

    if (u4Flags == DMX_MM_FLAG_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();
    u1PureAudioPidx = _u1PureAudioPidx;
    rPidStruct = _rPureAudioPidStruct;
    if (u1PureAudioPidx == DMX_PURE_AUDIO_PIDX_UNUSED)
    {
        _u1PureAudioPidx = u1Idx;
    }
    else if (u1PureAudioPidx != u1Idx)
    {
        _DMX_Unlock();
        LOG(3, "L%d: Invalid index (%d, %d)!\n", __LINE__, u1PureAudioPidx, u1Idx);
        return FALSE;
    }
    _DMX_Unlock();

    if (u4Flags & DMX_MM_FLAG_TYPE)
    {
        if (prMM->ePidType != DMX_PID_TYPE_PURE_AUDIO)
        {
            LOG(3, "Incorrect PID type (%d)!\n", (INT32)prMM->ePidType);
            return FALSE;
        }        
        rPidStruct.ePidType = prMM->ePidType;
    }

    if (u4Flags & DMX_MM_FLAG_BUF)
    {
        if (!_Dmx_PURE_SetPidBuffer(u1Idx, prMM, &rPidStruct))
        {
            return FALSE;
        }
    }

    // u1DeviceId is set to 0 by default.

    if (u4Flags & DMX_MM_FLAG_NOTIFY)
    {
        rPidStruct.pfnNotify = prMM->pfnCallback;
    }

    if (u4Flags & DMX_MM_FLAG_VALID)
    {
        rPidStruct.fgEnable = prMM->fgEnable;
    }
/*
    if (u4Flags & DMX_MM_FLAG_INSTANCE_TAG)
    {
        rPidStruct.pvInstanceTag = prMM->pvInstanceTag;
    }
*/
    _DMX_Lock();
    _rPureAudioPidStruct = rPidStruct;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PURE_Get
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PURE_Get(UINT8 u1Idx, UINT32 u4Flags, DMX_MM_T *prMM)
{
    UINT8 u1PureAudioPidx;
    PID_STRUCT_T rPidStruct;

    CHECK_INDEX_RANGE(u1Idx);
    CHECK_FOR_NULL_POINTER(prMM);

    if (u4Flags == DMX_MM_FLAG_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();
    u1PureAudioPidx = _u1PureAudioPidx;
    rPidStruct = _rPureAudioPidStruct;
    _DMX_Unlock();

    if ((u1PureAudioPidx == DMX_PURE_AUDIO_PIDX_UNUSED) || (u1PureAudioPidx != u1Idx))
    {
        LOG(3, "L%d: Invalid index (%d, %d)!\n", __LINE__, u1PureAudioPidx, u1Idx);
        return FALSE;
    }

    if (u4Flags & DMX_MM_FLAG_TYPE)
    {
        if (rPidStruct.ePidType != DMX_PID_TYPE_PURE_AUDIO)
        {
            LOG(3, "Incorrect PID type (%d)!\n", (INT32)rPidStruct.ePidType);
            return FALSE;
        }
        prMM->ePidType = rPidStruct.ePidType;
    }

    if (u4Flags & DMX_MM_FLAG_BUF)
    {
        prMM->fgAllocBuf = rPidStruct.fgAllocateBuffer;
        prMM->u4BufAddr = rPidStruct.u4BufStart;
        prMM->u4BufSize = rPidStruct.u4BufLen;
        prMM->u4Rp = rPidStruct.u4Rp;
        prMM->u4Wp = rPidStruct.u4Wp;
    }

    if (u4Flags & DMX_MM_FLAG_NOTIFY)
    {
        prMM->pfnCallback = rPidStruct.pfnNotify;
    }

    if (u4Flags & DMX_MM_FLAG_VALID)
    {
        prMM->fgEnable = rPidStruct.fgEnable;
    }
/*
    if (u4Flags & DMX_MM_FLAG_INSTANCE_TAG)
    {
        prMM->pvInstanceTag = rPidStruct.pvInstanceTag;
    }
*/
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PURE_Free
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PURE_Free(UINT8 u1Idx)
{
    UINT8 u1PureAudioPidx;
    PID_STRUCT_T rPidStruct;

    CHECK_INDEX_RANGE(u1Idx);

    _DMX_Lock();
    u1PureAudioPidx = _u1PureAudioPidx;
    rPidStruct = _rPureAudioPidStruct;
    _DMX_Unlock();

    if ((u1PureAudioPidx == DMX_PURE_AUDIO_PIDX_UNUSED) || (u1PureAudioPidx != u1Idx))
    {
        LOG(3, "L%d: Invalid index (%d, %d)!\n", __LINE__, u1PureAudioPidx, u1Idx);
        return FALSE;
    }

    if (rPidStruct.ePidType != DMX_PID_TYPE_PURE_AUDIO)
    {
        LOG(3, "Incorrect PID type (%d)!\n", (INT32)rPidStruct.ePidType);
        return FALSE;
    }

    // FIXME
    AUD_WaitDspFlush(rPidStruct.u1DeviceId);

    // Free PID buffer
    if (rPidStruct.fgAllocateBuffer)
    {
        if (!BSP_FreeAlignedDmaMemory((void*)PHYSICAL(rPidStruct.u4BufStart)))
        {
            LOG(3, "Fail to free PID buffer! pidx: %u, addr: 0x%08x\n",
                u1Idx, rPidStruct.u4BufStart);
            return FALSE;
        }
        //_DMX_ClearMemoryUsage(u1Pidx, rPidStruct.u4BufLen);
    }

    x_memset((void*)&rPidStruct, 0, sizeof(rPidStruct));

    _DMX_Lock();
    _rPureAudioPidStruct = rPidStruct;
    _u1PureAudioPidx = DMX_PURE_AUDIO_PIDX_UNUSED;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PURE_GetEmptyBufferSize
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_PURE_GetEmptyBufferSize(UINT8 u1Idx)
{
    UINT8 u1PureAudioPidx;
    PID_STRUCT_T rPidStruct;
    UINT32 u4BufSize, u4Wp, u4Rp, u4EmptySize;

    CHECK_INDEX_RANGE(u1Idx);

    _DMX_Lock();
    u1PureAudioPidx = _u1PureAudioPidx;
    rPidStruct = _rPureAudioPidStruct;
    _DMX_Unlock();

    if ((u1PureAudioPidx == DMX_PURE_AUDIO_PIDX_UNUSED) || (u1PureAudioPidx != u1Idx))
    {
        LOG(3, "L%d: Invalid index (%d, %d)!\n", __LINE__, u1PureAudioPidx, u1Idx);
        return FALSE;
    }

    u4BufSize = rPidStruct.u4BufLen;
    u4Wp = rPidStruct.u4Wp;
    u4Rp = rPidStruct.u4Rp;

    u4EmptySize = EMPTYSIZE(u4Rp, u4Wp, u4BufSize);
    
    if(u4EmptySize > MM_BUF_FULL_GAP)
    {
        u4EmptySize -= MM_BUF_FULL_GAP;
    }
    else 
    {
        u4EmptySize = 0;
    }
    
    return u4EmptySize;

}


//-----------------------------------------------------------------------------
/** _DMX_PURE_MoveData
 *  This function blocks until it finishes its operation.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PURE_MoveData(UINT8 u1Idx, const DMX_MM_DATA_T *prData)
{
    UINT8 u1PureAudioPidx;
    BOOL fgIsToDecoder;
    PID_STRUCT_T rPidStruct;
    UINT32 u4SrcBufStart, u4SrcBufEnd, u4SrcAddr, u4SrcDataSize;
    UINT32 u4DstBufStart, u4DstBufEnd, u4DstBufLen;
    UINT32 u4DstWp, u4DstRp, u4DstEmptySize;

    CHECK_INDEX_RANGE(u1Idx);
    CHECK_FOR_NULL_POINTER(prData);

    u4SrcBufStart = VIRTUAL(prData->u4BufStart);
    u4SrcBufEnd = VIRTUAL(prData->u4BufEnd);
    u4SrcAddr = VIRTUAL(prData->u4StartAddr);
    u4SrcDataSize = prData->u4FrameSize;

    if ((u4SrcAddr < u4SrcBufStart) || (u4SrcBufEnd <= u4SrcAddr))
    {
        LOG(3, "SrcAddr 0x%X is out of range!\n", u4SrcAddr);
        return FALSE;
    }

    _DMX_Lock();
    u1PureAudioPidx = _u1PureAudioPidx;
    rPidStruct = _rPureAudioPidStruct;
    _DMX_Unlock();

    if ((u1PureAudioPidx == DMX_PURE_AUDIO_PIDX_UNUSED) || (u1PureAudioPidx != u1Idx))
    {
        LOG(3, "L%d: Invalid index (%d, %d)!\n", __LINE__, u1PureAudioPidx, u1Idx);
        return FALSE;
    }

    if (!rPidStruct.fgEnable)
    {
        LOG(3, "Try to move data to a disabled pidx %d!\n", u1Idx);
        return FALSE;
    }

    if (rPidStruct.ePidType != DMX_PID_TYPE_PURE_AUDIO)
    {
        LOG(3, "Incorrect PID type (%d)!\n", (INT32)rPidStruct.ePidType);
        return FALSE;
    }        

    u4DstBufStart = rPidStruct.u4BufStart;
    u4DstBufEnd = rPidStruct.u4BufEnd + 1;  // make the address exclusive
    u4DstBufLen = rPidStruct.u4BufLen;
    u4DstWp = rPidStruct.u4Wp;
    u4DstRp = rPidStruct.u4Rp;

    ASSERT((u4DstBufStart != 0) && (u4DstBufEnd != 0));
    ASSERT(u4DstBufLen != 0);
    ASSERT((u4DstWp != 0) && (u4DstRp != 0));
    ASSERT((u4DstBufStart <= u4DstWp) && (u4DstWp < u4DstBufEnd));
    ASSERT((u4DstBufStart <= u4DstRp) && (u4DstRp < u4DstBufEnd));

    u4DstEmptySize = EMPTYSIZE(u4DstRp, u4DstWp, u4DstBufLen);
    if (u4DstEmptySize < u4SrcDataSize)
    {
        LOG(3, "Insufficient destination size (empty: %d, src: %d)!\n",
            u4DstEmptySize, u4SrcDataSize);
        return FALSE;
    }

    // Be sure to use physical addresses if GDMA is used in replace of
    // _DMX_CopyRingBuffer().

    if (_DMX_CopyRingBuffer(u4DstWp, u4DstBufStart, u4DstBufEnd, u4SrcAddr,
        u4SrcBufStart, u4SrcBufEnd, u4SrcDataSize) != u4DstWp)
    {
        LOG(3, "Fail to copy data from 0x%X to 0x%X.\n", u4SrcAddr, u4DstWp);
        return FALSE;
    }

    // Update WP
    u4DstWp += u4SrcDataSize;
    if (u4DstWp >= u4DstBufEnd)
    {
        u4DstWp -= u4DstBufLen;
    }
    ASSERT((u4DstBufStart <= u4DstWp) && (u4DstWp < u4DstBufEnd));
    rPidStruct.u4PesRp = u4DstWp;
    rPidStruct.u4Wp = u4DstWp;
    _DMX_Lock();
    _rPureAudioPidStruct = rPidStruct;
    _DMX_Unlock();

    LOG(7, "SrcAddr: 0x%X, SrcSize: 0x%X, DstAddr: 0x%X, DstSize: 0x%X\n",
        u4SrcAddr, u4SrcDataSize, u4DstWp, u4DstEmptySize);

    HalFlushInvalidateDCache();

    fgIsToDecoder = _DMX_IsToDecoder();
    if (fgIsToDecoder)
    {
        DMX_AUDIO_PES_T rPes;

        x_memset((void*)&rPes, 0, sizeof(rPes));
        rPes.u1PidIndex = u1Idx;
        rPes.u1DeviceId = rPidStruct.u1DeviceId;
        rPes.u4Wp = PHYSICAL(u4DstWp);
        rPes.u4PesRp = PHYSICAL(rPidStruct.u4PesRp);
        rPes.u4Dts = prData->u4Dts;
        rPes.u4Pts = prData->u4Pts;

        VERIFY(_DMX_SendAudioPes(&rPes));

        // Send EOS if needed
        if (prData->fgEOS)
        {
            x_memset((void*)&rPes, 0, sizeof(rPes));
            rPes.u1PidIndex = u1Idx;
            rPes.u1DeviceId = rPidStruct.u1DeviceId;
            rPes.fgEOS = TRUE;

            VERIFY(_DMX_SendAudioPes(&rPes));
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PURE_FlushBuffer
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PURE_FlushBuffer(UINT8 u1Idx)
{
    UINT8 u1PureAudioPidx;
    PID_STRUCT_T rPidStruct;
    UINT32 u4BufStart;

    CHECK_INDEX_RANGE(u1Idx);

    _DMX_Lock();
    u1PureAudioPidx = _u1PureAudioPidx;
    rPidStruct = _rPureAudioPidStruct;
    _DMX_Unlock();

    if ((u1PureAudioPidx == DMX_PURE_AUDIO_PIDX_UNUSED) || (u1PureAudioPidx != u1Idx))
    {
        LOG(3, "L%d: Invalid index (%d, %d)!\n", __LINE__, u1PureAudioPidx, u1Idx);
        return FALSE;
    }

    if (rPidStruct.ePidType != DMX_PID_TYPE_PURE_AUDIO)
    {
        LOG(3, "Incorrect PID type (%d)!\n", (INT32)rPidStruct.ePidType);
        return FALSE;
    }

    u4BufStart = rPidStruct.u4BufStart;
    rPidStruct.u4Wp = u4BufStart;
    rPidStruct.u4Rp = u4BufStart;
    rPidStruct.u4PesRp = u4BufStart;
    rPidStruct.u4SectionRp = u4BufStart;
    rPidStruct.u4NextPic = 0;

    _DMX_Lock();
    _rPureAudioPidStruct = rPidStruct;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PURE_UpdateReadPointer
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PURE_UpdateReadPointer(UINT8 u1Idx, UINT32 u4Rp, UINT32 u4FrameAddr)
{
    UINT8 u1PureAudioPidx;
    PID_STRUCT_T rPidStruct;
    UINT32 u4BufStart, u4BufEnd, u4DataSize, u4UpdateSize;

    CHECK_INDEX_RANGE(u1Idx);

    _DMX_Lock();
    u1PureAudioPidx = _u1PureAudioPidx;
    rPidStruct = _rPureAudioPidStruct;
    _DMX_Unlock();

    if ((u1PureAudioPidx == DMX_PURE_AUDIO_PIDX_UNUSED) || (u1PureAudioPidx != u1Idx))
    {
        LOG(3, "L%d: Invalid index (%d, %d)!\n", __LINE__, u1PureAudioPidx, u1Idx);
        return FALSE;
    }

    u4BufStart = rPidStruct.u4BufStart;
    u4BufEnd = rPidStruct.u4BufEnd + 1;
    u4Rp = VIRTUAL(u4Rp);

    if ((u4Rp < u4BufStart) || (u4Rp >= u4BufEnd))
    {
        LOG(3, "Pidx: %u, Invalid RP: 0x%08x\n", u1Idx, u4Rp);
        return FALSE;
    }

    // Check update size
    u4DataSize = DATASIZE(rPidStruct.u4Rp, rPidStruct.u4Wp, rPidStruct.u4BufLen);
    u4UpdateSize = DATASIZE(rPidStruct.u4Rp, u4Rp, rPidStruct.u4BufLen);
    if (u4UpdateSize > u4DataSize)
    {
        // If update size is larger than data size, something is wrong.
        LOG(3, "Pidx: %u, Invalid RP 2: 0x%08x, DataSize: %u, UpdateSize: %u\n",
            u1Idx, u4Rp, u4DataSize, u4UpdateSize);
        return FALSE;
    }

    rPidStruct.u4Rp = u4Rp;

    _DMX_Lock();
    //_rPureAudioPidStruct = rPidStruct;
    _rPureAudioPidStruct.u4Rp = u4Rp;  // for better performance
    _DMX_Unlock();

    // Debug
    LOG(7, "Update RP - pidx: %u, RP: 0x%08x, frame: 0x%08x\n", u1Idx, u4Rp, u4FrameAddr);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PURE_GetInstanceTag
*/
//-----------------------------------------------------------------------------
void* _DMX_PURE_GetInstanceTag(void)
{
    //return _rPureAudioPidStruct.pvInstanceTag;
    return NULL;
}


#endif  // CC_DMX_PURE_AUDIO


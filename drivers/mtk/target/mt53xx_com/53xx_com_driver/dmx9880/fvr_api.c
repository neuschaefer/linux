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
 * $RCSfile: fvr_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_fvr_if.c
 *  Demux driver - FVR Driver Interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fvr.h"
#include "fvr_debug.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_hal_arm.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_ckgen.h"
LINT_EXT_HEADER_END

#include "dmx_if.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define FVR_SIZE_ALIGNMENT      1536            // 16 and 512 bytes alignment
#define FVR_TIME_DIVIDER        8000            // 16 bit

#define FVR_PICBUF_SIZE         (256)
#define FVR_PICBUF_ALIGN	    (32)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
// store video type per PID
typedef struct
{
    BOOL fgEnable;
    FVR_VIDEO_TYPE_T eType;          ///< Type of video
} FVR_VIDEO_TYPE_PER_PID_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgFVRLocking = FALSE;
static CRIT_STATE_T _rFVRLock;

/// The PID structures
static FVR_GBL_STRUCT_T _arGblStruct[FVR_GBL_COUNT];
static FVR_PID_STRUCT_T _arFVRPidStruct[FVR_NUM_PID_INDEX];

static BOOL _afgFVRToggleKey[FVR_GBL_COUNT];

static FVR_TIMESTAMP_T _rTimeStamp;
static FVR_VIDEO_TYPE_PER_PID_T _au1FvrVideoTypePerPid[FVR_NUM_PID_INDEX];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _SetFVRBuffer
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _SetFVRBuffer(UINT8 u1Idx, const FVR_GBL_T* prGBL)
{
    UINT32 u4BufStart, u4BufSize, u4Align, u4PhyAddr, u4VirAddr;
    UINT32 u4PicBuf;

    if(prGBL == NULL)
    {
        return FALSE;
    }

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    // Check if buffer already allocated, to avoid memory leakage
    if ((_arGblStruct[u1Idx].u4BufStart != 0) || (_arGblStruct[u1Idx].u4BufEnd != 0))
    {
        return FALSE;
    }

    // For error recovery case
    if((prGBL->fgSetAlign == 0) && (prGBL->u4BufAlign == 0) &&
       (prGBL->fgAllocateBuffer == 0) && (prGBL->u4BufStart == 0))
    {
        return TRUE;
    }

    if(prGBL->fgSetAlign)
    {
        u4Align = prGBL->u4BufAlign;
    }
    else
    {
        u4Align = FVR_SIZE_ALIGNMENT;
    }

    // PVR buffer must 16-byte alignment
    ASSERT((u4Align % 16) == 0);

    u4BufSize = prGBL->u4BufSize;
    if(u4Align > 0)
    {
        u4BufSize -= (prGBL->u4BufSize % u4Align);
    }
    _arGblStruct[u1Idx].u4BufSize = u4BufSize;

    if(_arGblStruct[u1Idx].fgAllocateBuffer)
    {
    	// Setup header buffer start address
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(u4BufSize, FVR_FIFO_ALIGNMENT);
    	if (u4BufStart == 0)
    	{
    		LOG(3, "Can't allocate memory!\n");
    		return FALSE;
    	}
    }
    else
    {
        ASSERT(prGBL->u4BufStart != 0);
        u4BufStart = prGBL->u4BufStart;
    }

    u4PhyAddr = PHYSICAL(u4BufStart);
    u4VirAddr = VIRTUAL(u4BufStart);

    u4PicBuf = (UINT32)BSP_AllocAlignedDmaMemory(FVR_PICBUF_SIZE, FVR_PICBUF_ALIGN);
    if(u4PicBuf == 0)
    {
        return FALSE;
    }

    _FVR_Lock();

    FVR_GBL_ARY_W(u1Idx, 0) = u4PhyAddr;                                  // Ring buf start
    FVR_GBL_ARY_W(u1Idx, 1) = u4PhyAddr + (u4BufSize - 1);                // Ring buf end
    FVR_GBL_ARY_W(u1Idx, 3) = u4PhyAddr;                                  // write pointer
    FVR_GBL_ARY_W(u1Idx, 4) = u4PhyAddr;                                  // read pointer

    _arGblStruct[u1Idx].u4BufStart = u4VirAddr;
    _arGblStruct[u1Idx].u4BufEnd = u4VirAddr + (u4BufSize - 1);
    _arGblStruct[u1Idx].u4Rp = u4VirAddr;
    _arGblStruct[u1Idx].u4Wp = u4VirAddr;
    _arGblStruct[u1Idx].u4PktRp = u4VirAddr;

    FVR_GBL_ARY_W(u1Idx, 8) = PHYSICAL(u4PicBuf);
    _arGblStruct[u1Idx].u4PicWorkingBuf = VIRTUAL(u4PicBuf);
    _FVR_Unlock();

    HalFlushInvalidateDCache();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FvrSetPidBuffer
 */
//-----------------------------------------------------------------------------
static BOOL _FvrSetPidBuffer(UINT8 u1Pidx, FVR_PID_STRUCT_T *prPidStruct, const FVR_PID_T *prPid)
{
    UINT32 u4BufStart, u4PhyBufStart, u4BufSize;

    if((prPidStruct == NULL) || (prPid == NULL))
    {
        return FALSE;
    }

    if (prPid->ePidType != DMX_PID_TYPE_PSI)
    {
        return FALSE;
    }

    u4BufSize = prPid->u4BufSize;

    // FIFO start address
    if (prPid->fgAllocateBuffer)
    {
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(u4BufSize, DMX_ES_FIFO_ALIGNMENT);
        if (u4BufStart == 0)
        {
            LOG(3, "%s:%d: Can't allocate memory!\n", __FILE__, __LINE__);
            return FALSE;
        }
    }
    else
    {
        ASSERT(prPid->u4BufAddr != 0);
        u4BufStart = prPid->u4BufAddr;
    }

    _FVR_Lock();

    u4BufStart = VIRTUAL(u4BufStart);
    prPidStruct->u4BufStart = u4BufStart;
    prPidStruct->u4BufEnd = (u4BufStart + u4BufSize) - 1;
    prPidStruct->u4Wp = u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4SectionRp = u4BufStart;
    u4PhyBufStart = PHYSICAL(u4BufStart);
    FVR_PER_PID_S_W(u1Pidx, 5) = u4PhyBufStart;     // FIFO start
    FVR_PER_PID_S_W(u1Pidx, 7) = u4PhyBufStart;     // PES_start
    FVR_PER_PID_S_W(u1Pidx, 8) = u4PhyBufStart;     // Write pointer
    FVR_PER_PID_S_W(u1Pidx, 9) = u4PhyBufStart;     // Read pointer
    FVR_PER_PID_S_W(u1Pidx, 6) = (u4PhyBufStart + u4BufSize) - 1;

    // FIFO size
    prPidStruct->u4BufLen = u4BufSize;
    prPidStruct->fgAllocateBuffer = prPid->fgAllocateBuffer;
    _FVR_Unlock();

    HalFlushInvalidateDCache();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FvrResetPidBuffer
 */
//-----------------------------------------------------------------------------
static BOOL _FvrResetPidBuffer(UINT8 u1Pidx, FVR_PID_STRUCT_T *prPidStruct)
{
    UINT32 u4BufStart;
    BOOL fgRet = TRUE;

    ASSERT(prPidStruct != NULL);

    if(prPidStruct->ePidType != DMX_PID_TYPE_PSI)
    {
        return FALSE;
    }

    _FVR_Lock();

    // Check if PID is inactive
    if ((prPidStruct->fgEnable) ||
        ((FVR_PID_INDEX_TABLE(u1Pidx) & 0x80000000) != 0))
    {
        _FVR_Unlock();
        LOG(3, "Reset buffer of an active pid\n");
        return FALSE;
    }

    // Setup PID structures
    FVR_PER_PID_S_W(u1Pidx, 1) = 0;

    FVR_PER_PID_S_W(u1Pidx, 3) = 0xffff0002;
    FVR_PER_PID_S_W(u1Pidx, 4) = 0xffffffff;

    // Reset payload FIFO
    u4BufStart = prPidStruct->u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4Wp = u4BufStart;
    u4BufStart = PHYSICAL(u4BufStart);
    FVR_PER_PID_S_W(u1Pidx, 7) = u4BufStart;        // PES_start
    FVR_PER_PID_S_W(u1Pidx, 8) = u4BufStart;        // Write pointer
    FVR_PER_PID_S_W(u1Pidx, 9) = u4BufStart;        // Read pointer

    _FVR_Unlock();

    return fgRet;

}


//-----------------------------------------------------------------------------
/** _FvrFreePidBuffer
 */
//-----------------------------------------------------------------------------
static BOOL _FvrFreePidBuffer(UINT8 u1Pidx)
{
    FVR_PID_STRUCT_T* prPidStruct;

    ASSERT(u1Pidx < FVR_NUM_PID_INDEX);
    prPidStruct = &_arFVRPidStruct[u1Pidx];

    LOG(6, "FVR free pidx %u buffer\n", u1Pidx);

    // Make sure the final DMA transaction terminates before freeing PID buffer.
    x_thread_delay(1);

    // Free buffer
    if (prPidStruct->fgAllocateBuffer)
    {
        if (!BSP_FreeAlignedDmaMemory(prPidStruct->u4BufStart))
        {
            LOG(3, "FVR fail to free PID buffer! pidx: %u, addr: 0x%08x\n",
                u1Pidx, prPidStruct->u4BufStart);
            return FALSE;
        }
    }

    _FVR_Lock();

    // Update PID state
    prPidStruct->fgAllocateBuffer = FALSE;
    prPidStruct->u4BufStart = 0;
    prPidStruct->u4BufEnd = 0;
    prPidStruct->u4BufLen = 0;
    prPidStruct->u4Rp = 0;
    prPidStruct->u4Wp = 0;

    FVR_PER_PID_S_W(u1Pidx, 10) = 0x0;
    FVR_PER_PID_S_W(u1Pidx, 11) = 0x0;

    _FVR_Unlock();

    return TRUE;

}


//-----------------------------------------------------------------------------
/** _FvrEnablePid
 */
//-----------------------------------------------------------------------------
static BOOL _FvrEnablePid(UINT8 u1Pidx, BOOL fgEnable, UINT32 *pu4Word0, UINT32 *pu4PidEntry,
                        FVR_PID_STRUCT_T *prPid)
{
    UINT32 u4ActiveFlag;

    if(prPid == NULL)
    {
        return FALSE;
    }

    if(fgEnable)
    {
        if(prPid->fgDoSFilter)
        {
            if(!_FvrResetPidBuffer(u1Pidx, prPid))
            {
                return FALSE;
            }
        }

        _FVR_Lock();

        // Read bit 4 of the first word in PID memory
        u4ActiveFlag = (UINT32)((((*pu4Word0) & (1 << 4)) != 0) ? 1 : 0);
        // Copy to bit 8 of PID index table
        *pu4PidEntry = ((*pu4PidEntry) & ~(1 << 8)) | (u4ActiveFlag << 8);
        *pu4PidEntry |= 0x80000000;
        prPid->fgEnable = fgEnable;

        _FVR_Unlock();
    }
    else
    {
        _FVR_Lock();

        // Read bit 8 of PID index table
        u4ActiveFlag = (UINT32)(((*pu4PidEntry & (1 << 8)) == 0) ? 1 : 0);

        // Copy inverse to bit 4 of the first word in PID memory
        *pu4Word0 = (UINT32)(((*pu4Word0) & ~(1 << 4)) | (u4ActiveFlag << 4));

        *pu4PidEntry &= 0x7FFFFFFF;
        prPid->fgEnable = fgEnable;

        // Update serial number
        prPid->u1SerialNumber++;
        if (prPid->u1SerialNumber > DMX_MAX_SERIAL_NUM)
        {
            prPid->u1SerialNumber = 0;
        }

        _FVR_Unlock();
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _FVR_Lock
 *  Enter FVR critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_Lock(void)
{
    _rFVRLock = x_crit_start();

    if(_fgFVRLocking)
    {
        LOG(3, "Already lock\n");
        ASSERT(0);
    }

    _fgFVRLocking = TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_Unlock
 *  Leave FVR critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_Unlock(void)
{
    if(!_fgFVRLocking)
    {
        LOG(3, "Already Unlock\n");
        ASSERT(0);
    }

    _fgFVRLocking = FALSE;

    x_crit_end(_rFVRLock);
}


//-----------------------------------------------------------------------------
/** _FVR_InitAPI
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_InitAPI()
{
    x_memset((void*)&_rTimeStamp, 0x0, sizeof(_rTimeStamp));

    _afgFVRToggleKey[0] = FALSE;
    _afgFVRToggleKey[1] = FALSE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetGBL
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SetGBL(UINT8 u1Idx, UINT32 u4Flags, const FVR_GBL_T* prGBL)
{
    UINT32 u4Ctrl;

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    if(prGBL == NULL)
    {
        return FALSE;
    }

    if (u4Flags == FVR_GBL_FLAG_NONE)
    {
        return TRUE;
    }

    if(u4Flags & FVR_GBL_FLAG_BUFFER)
    {
        _arGblStruct[u1Idx].fgAllocateBuffer = prGBL->fgAllocateBuffer;
        _arGblStruct[u1Idx].fgSetAlign = prGBL->fgSetAlign;
        _arGblStruct[u1Idx].u4BufAlign = prGBL->u4BufAlign;
        if(!_SetFVRBuffer(u1Idx, prGBL))
        {
            return FALSE;
        }

        _arGblStruct[u1Idx].u2CurPktCount = 0;
        _arGblStruct[u1Idx].u4LongPktCount = 0;
        FVR_GBL_ARY_W(u1Idx, 5) &= 0xFFFF0000;
        FVR_GBL_ARY_W(u1Idx, 6) = 0x0;
    }

    if(u4Flags & FVR_GBL_FLAG_TARGET_PKT)
    {
        _arGblStruct[u1Idx].u2TargetPktCount = prGBL->u2TargetPktCount;
        u4Ctrl = FVR_GBL_ARY_W(u1Idx, 5);
        u4Ctrl &= 0xFFFF;
        u4Ctrl |= (UINT32)((prGBL->u2TargetPktCount)<<16);
        FVR_GBL_ARY_W(u1Idx, 5) = u4Ctrl;
    }

    if(u4Flags & FVR_GBL_FLAG_NOTIFY)
    {
        _arGblStruct[u1Idx].pfnNotifyFunc = prGBL->pfnNotifyFunc;
        _arGblStruct[u1Idx].pvNotifyTag = prGBL->pvNotifyTag;
    }

    if(u4Flags & FVR_GBL_FLAG_TIME_INFO)
    {
        _arGblStruct[u1Idx].u4TimeToNotify = prGBL->u4TimeToNotify;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetGBL
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL _FVR_GetGBL(UINT8 u1Idx, UINT32 u4Flags, FVR_GBL_T *prGBL)
{
    UINT32 u4Rp, u4Wp;

    if(prGBL == NULL)
    {
        return FALSE;
    }

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    if (u4Flags == FVR_GBL_FLAG_NONE)
    {
        return TRUE;
    }

    if(u4Flags & FVR_GBL_FLAG_BUFFER)
    {
        prGBL->fgAllocateBuffer = _arGblStruct[u1Idx].fgAllocateBuffer;
        prGBL->u4BufSize = _arGblStruct[u1Idx].u4BufSize;
        prGBL->u4BufStart = _arGblStruct[u1Idx].u4BufStart;
        prGBL->u4BufAlign = _arGblStruct[u1Idx].u4BufAlign;

        _FVR_Lock();
        u4Wp = VIRTUAL(FVR_GBL_ARY_W(u1Idx, 3));
        u4Rp = VIRTUAL(FVR_GBL_ARY_W(u1Idx, 4));
        _FVR_Unlock();

        prGBL->u4Wp = u4Wp;
        prGBL->u4Rp = u4Rp;
    }

    if(u4Flags & FVR_GBL_FLAG_TARGET_PKT)
    {
        prGBL->u2TargetPktCount = _arGblStruct[u1Idx].u2TargetPktCount;
    }

    if(u4Flags & FVR_GBL_FLAG_NOTIFY)
    {
        prGBL->pfnNotifyFunc = _arGblStruct[u1Idx].pfnNotifyFunc;
        prGBL->pvNotifyTag = _arGblStruct[u1Idx].pvNotifyTag;
    }

    if(u4Flags & FVR_GBL_FLAG_TIME_INFO)
    {
        prGBL->u4TimeToNotify = _arGblStruct[u1Idx].u4TimeToNotify;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_FreeGBL
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL _FVR_FreeGBL(UINT8 u1Idx)
{
    UINT32 i;
    //UINT32 u4Reg;
    BOOL fgPidEnable = FALSE;

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    if(_FVR_IsRecording(u1Idx))
    {
        LOG(3, "Still recording...\n");
        return FALSE;
    }

    _FVR_Lock();
    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if(_arFVRPidStruct[i].fgEnable)
        {
            fgPidEnable = TRUE;
            break;
        }
    }
    _FVR_Unlock();

    if(fgPidEnable)
    {
        LOG(3, "Record pid %d still enabled\n", i);
        return FALSE;
    }

    x_thread_delay(5);

    _FVR_Lock();
    FVR_GBL_ARY_W(u1Idx, 5) = 0;
    FVR_GBL_ARY_W(u1Idx, 6) = 0x0;
//    FVR_GBL_ARY_W(u1Idx, 9) = 0x0;     // Disable timer
//    u4Reg = FVR_GBL_ARY_W(u1Idx, 10) & 0xFF000000;
//    u4Reg |= 0x1;
//    FVR_GBL_ARY_W(u1Idx, 10) = u4Reg;    // Disable timer
    _FVR_Unlock();

    if(!_FVR_FreeBuffer(u1Idx))
    {
        return FALSE;
    }

    _FVR_Lock();
    x_memset((void*)&_arGblStruct[u1Idx], 0, sizeof(FVR_GBL_STRUCT_T));
    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetGBLStruct
 *  @retval
 */
//-----------------------------------------------------------------------------
FVR_GBL_STRUCT_T* _FVR_GetGBLStruct(UINT8 u1Idx)
{
    ASSERT(u1Idx < FVR_GBL_COUNT);
    if(u1Idx >= FVR_GBL_COUNT)
    {
        return NULL;
    }

    return &_arGblStruct[u1Idx];
}

//-----------------------------------------------------------------------------
/** _FVR_TogglePidKey
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_TogglePidKey(void)
{
    UINT32 u4PidEntry;
    UINT8 i;

    _FVR_Lock();
    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if(_arFVRPidStruct[i].fgEnable && _arFVRPidStruct[i].fgScramble &&
           (_arFVRPidStruct[i].eKeyType == FVR_KEY_BOTH))
        {
            u4PidEntry = FVR_PID_INDEX_TABLE(i);
            u4PidEntry &= ~0x80;
            if(_arFVRPidStruct[i].fgOddKey)
            {
                _arFVRPidStruct[i].fgOddKey = FALSE;
            }
            else
            {
                _arFVRPidStruct[i].fgOddKey = TRUE;
                u4PidEntry |= 0x80;
            }
            FVR_PID_INDEX_TABLE(i) = u4PidEntry;
        }
    }
    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetIsToggleKey
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_SetIsToggleKey(UINT8 u1Idx)
{
    UINT8 i;

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return;
    }

    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if(_arFVRPidStruct[i].fgEnable && _arFVRPidStruct[i].fgScramble &&
           (_arFVRPidStruct[i].u1TsIndex == u1Idx) &&
           (_arFVRPidStruct[i].eKeyType == FVR_KEY_BOTH))
        {
            _afgFVRToggleKey[u1Idx] = TRUE;
            break;
        }
    }
}


//-----------------------------------------------------------------------------
/** _FVR_GetIsToggleKey
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_GetIsToggleKey(UINT8 u1Idx)
{
    BOOL fgToggle;

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    _FVR_Lock();
    fgToggle = _afgFVRToggleKey[u1Idx];
    _FVR_Unlock();

    return fgToggle;
}


//-----------------------------------------------------------------------------
/** _FVR_SetPid
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const FVR_PID_T* prPid)
{
    UINT32 u4PidEntry, u4Val, u4Word0;
    FVR_PID_STRUCT_T rPidStruct;
    BOOL fgUpdateW0 = FALSE;

    ASSERT(u1Pidx < FVR_NUM_PID_INDEX);

    if(prPid == NULL)
    {
        return FALSE;
    }

    if (u4Flags == FVR_PID_FLAG_NONE)
    {
        return TRUE;
    }

    _FVR_Lock();
    u4PidEntry = FVR_PID_INDEX_TABLE(u1Pidx);
    rPidStruct = _arFVRPidStruct[u1Pidx];
    _FVR_Unlock();

//  Not use now. Use logical mapping
    if(u4Flags & FVR_PID_FLAG_TSINDEX)
    {
        rPidStruct.u1TsIndex= prPid->u1TsIndex;
        u4PidEntry &= 0x9fffffff;
        u4PidEntry |= (prPid->u1TsIndex & 0x3) << 29;
    }

    if(u4Flags & FVR_PID_FLAG_PID)
    {
        rPidStruct.u2Pid = prPid->u2Pid;
        u4PidEntry &= ~0x1FFF0000;
        u4PidEntry |= ((prPid->u2Pid & 0x1FFF) << 16);
    }

    if(u4Flags & FVR_PID_FLAG_PICSEARCH)
    {
        rPidStruct.fgPicSearch = prPid->fgPicSearch;
        rPidStruct.u1PicSearchMask = prPid->u1PicSearchMask;
        rPidStruct.pfnPicSearchNotify = prPid->pfnPicSearchNotify;
        rPidStruct.pvPicSearchNotifyTag = prPid->pvPicSearchNotifyTag;
    }

    if(u4Flags & FVR_PID_FLAG_PKTSWAP)
    {
        rPidStruct.fgPktSwap = prPid->fgPktSwap;
        rPidStruct.pfnPicSearchNotify = prPid->pfnPicSearchNotify;
        rPidStruct.pvSwapNotifyTag = prPid->pvSwapNotifyTag;
        u4PidEntry &= ~0x400;
        if(prPid->fgPktSwap)
        {
            u4PidEntry |= 0x400;
        }
    }

    if(u4Flags & FVR_PID_FLAG_KEY_TYPE)
    {
        rPidStruct.eKeyType = prPid->eKeyType;
        u4PidEntry &= ~0x80;

        if((prPid->eKeyType == FVR_KEY_EVEN) || (prPid->eKeyType == FVR_KEY_BOTH))
        {
            rPidStruct.fgOddKey = FALSE;
        }
        else
        {
            rPidStruct.fgOddKey = TRUE;
            u4PidEntry |= 0x80;
        }
    }

    if(u4Flags & FVR_PID_FLAG_KEY_LEN)
    {
        if((prPid->u4KeyLen == 128) || (prPid->u4KeyLen == 192) ||
           (prPid->u4KeyLen == 256))
        {
            rPidStruct.u4KeyLen = prPid->u4KeyLen;
            u4PidEntry &= ~0xC;

            if(prPid->u4KeyLen == 128)
            {
                u4PidEntry |= 0x0;
            }
            else if(prPid->u4KeyLen == 192)
            {
                u4PidEntry |= 0x4;
            }
            else if(prPid->u4KeyLen == 256)
            {
                u4PidEntry |= 0x8;
            }
        }
        else
        {
            if(!_DMX_GetIsPanicReset())
            {
                LOG(1, "Key length wrong %d\n", prPid->u4KeyLen);
                return FALSE;
            }
        }
    }

    if(u4Flags & FVR_PID_FLAG_KEY_INDEX)
    {
        rPidStruct.u1KeyIdx = prPid->u1KeyIdx;
        u4PidEntry |= ((prPid->u1KeyIdx & 0x7) << 4);
    }

    if(u4Flags & FVR_PID_FLAG_CBC_MODE)
    {
        rPidStruct.fgCBCMode = prPid->fgCBCMode;
        u4PidEntry &= ~0x1;
        if(prPid->fgCBCMode)
        {
            u4PidEntry |= 0x1;
        }
    }

    if(u4Flags & FVR_PID_FLAG_SCRAMBLE_ENABLE)
    {
        rPidStruct.fgScramble = prPid->fgScramble;
        u4PidEntry &= ~0x2;
        if(prPid->fgScramble)
        {
            u4PidEntry |= 0x2;
        }
    }

    if(u4Flags & FVR_PID_FLAG_DETECT_SCRAMBLE)
    {
        rPidStruct.fgDetectScramble = prPid->fgDetectScramble;
        rPidStruct.pfnDetectScrambleNotify = prPid->pfnDetectScrambleNotify;

        // bit 9: detect scramble
        u4PidEntry &= ~0x200;
        if(prPid->fgDetectScramble)
        {
            u4PidEntry |= 0x200;
        }
    }

    if(u4Flags & FVR_PID_FLAG_TYPE)
    {
        rPidStruct.ePidType = prPid->ePidType;
        rPidStruct.fgDoSFilter = prPid->fgDoSFilter;

        _FVR_Lock();
        FVR_PER_PID_S_W(u1Pidx, 1) = 0;
        FVR_PER_PID_S_W(u1Pidx, 4) = 0xFFFFFFFF;        // previous start code pattern

        // Section filter flags
        // bit 0: 1: PSI, 0: PES
        // bit 1: Section filter on
        // bit 2: Output mode
        // bit 4: PID on/off toggle (combined with bit 8 of PID index entry)
        // bit 5: PID disable
        // bit 6: To CD-FIFO
        // bit 7: Enable picture finder
        u4Val = 0;
        if(prPid->ePidType == DMX_PID_TYPE_ES_VIDEO)
        {
            u4Val = (UINT32)((1 << 7) | 0x00FF0000);        //set byte2 to 0xff for default picture search by TS_IDX
            FVR_PER_PID_S_W(u1Pidx, 3) = 0xFFFF0001;
            FVR_PER_PID_S_W(u1Pidx, 5) = 0xFFFFFFFF;        // previous start code pattern
        }
        else if(prPid->ePidType == DMX_PID_TYPE_ES_AUDIO)
        {
            FVR_PER_PID_S_W(u1Pidx, 3) = 0xFFFF0001;
        }
        else if(prPid->ePidType == DMX_PID_TYPE_PSI)
        {
            u4Val = (UINT32)
                       ((1 << 0) |         // PSI
                        (0 << 1) |          // Section filter 1:on, 0:off
                        (0xfe << 8) |       // Filter match table, accept all matched
                        (23 << 16));        // Max bytes sending to section filter
            if(prPid->fgDoSFilter)
            {
                u4Val |= (1 << 1);
            }
            FVR_PER_PID_S_W(u1Pidx, 3) = 0xFFFF0002;
        }
        FVR_PER_PID_S_W(u1Pidx, 0) = (FVR_PER_PID_S_W(u1Pidx, 0) & (1 << 4)) | u4Val;
        _FVR_Unlock();

        if(prPid->fgDoSFilter)
        {
            if(!_FvrSetPidBuffer(u1Pidx, &rPidStruct, prPid))
            {
                return FALSE;
            }
        }
    }

    if(u4Flags & FVR_PID_FLAG_FREEZE)
    {
        rPidStruct.fgFreeze = prPid->fgFreeze;

        _FVR_Lock();
        if(prPid->fgFreeze)
        {
            FVR_PER_PID_S_W(u1Pidx, 0) |= (1 << 5);
        }
        else
        {
            FVR_PER_PID_S_W(u1Pidx, 0) &= ~(1 << 5);
        }
        _FVR_Unlock();
    }

    if(u4Flags & FVR_PID_FLAG_SECNOTIFY)
    {
        rPidStruct.pfnNotify = prPid->pfnNotify;
        rPidStruct.pvNotifyTag = prPid->pvNotifyTag;
    }

    if(u4Flags & FVR_PID_FLAG_INSTANCE_TAG)
    {
        rPidStruct.pvInstanceTag = prPid->pvInstanceTag;
    }

    if(u4Flags & FVR_PID_FLAG_DESC)
	{
	    rPidStruct.fgDesc = prPid->fgDesc;
		rPidStruct.u1DescKeyidx = prPid->u1DescKeyidx;
		u4PidEntry &= 0xFFFF0FFF;
		u4PidEntry |= ((prPid->u1DescKeyidx & 0xF) << 12);

        u4PidEntry &= ~0x800;
		if(prPid->fgDesc)
		{
		    u4PidEntry |= 0x800;	// Set CA to descramble
		}
	}

    if(u4Flags & FVR_PID_FLAG_ENABLE)
    {
        // Check toggle PID
        _FVR_SetIsToggleKey(rPidStruct.u1TsIndex);

        if(prPid->fgEnable)
        {
            _FVR_Lock();
            FVR_PER_PID_S_W(u1Pidx, 9) |= 0xFF00;       // Set scramble state to unknown
            if((rPidStruct.ePidType == DMX_PID_TYPE_ES_VIDEO) && (rPidStruct.fgPicSearch))
            {
                FVR_PER_PID_S_W(u1Pidx, 0) |= 0x80;
            }
            _FVR_Unlock();
            // Set per-pid struct to default value
            u4PidEntry |= 0x80000000;
        }
        else
        {
            u4PidEntry &= ~0x80000000;
        }

        fgUpdateW0 = TRUE;
        _FVR_Lock();
        u4Word0 = FVR_PER_PID_S_W(u1Pidx, 0);
        _FVR_Unlock();
        if(prPid->fgEnable)
        {
            if(!_FvrEnablePid(u1Pidx, TRUE, &u4Word0, &u4PidEntry, &rPidStruct))
            {
                return FALSE;
            }
        }
        else
        {
            if(!_FvrEnablePid(u1Pidx, FALSE, &u4Word0, &u4PidEntry, &rPidStruct))
            {
                return FALSE;
            }
        }
    }

    _FVR_Lock();
    FVR_PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
    _arFVRPidStruct[u1Pidx] = rPidStruct;
    if(fgUpdateW0)
    {
        FVR_PER_PID_S_W(u1Pidx, 0) = u4Word0;
    }
    _FVR_Unlock();
/*
    if(u4Flags & FVR_PID_FLAG_ENABLE)
    {
        LOG(5, "%s, enable:%d\n", __FUNCTION__, (INT32)prPid->fgEnable);
    }
*/
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetPid
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL _FVR_GetPid(UINT8 u1Pidx, UINT32 u4Flags, FVR_PID_T *prPid)
{
    FVR_PID_STRUCT_T rPidStruct;

    ASSERT(u1Pidx < FVR_NUM_PID_INDEX);

    if(prPid == NULL)
    {
        return FALSE;
    }

    if (u4Flags == FVR_PID_FLAG_NONE)
    {
        return TRUE;
    }

    _FVR_Lock();
    rPidStruct = _arFVRPidStruct[u1Pidx];
    _FVR_Unlock();

    if(u4Flags & FVR_PID_FLAG_SOURCE)
    {
        prPid->eSource = rPidStruct.eSource;
        prPid->u1TsIndex = rPidStruct.u1TsIndex;
    }

    if(u4Flags & FVR_PID_FLAG_TSINDEX)
    {
        prPid->u1TsIndex = rPidStruct.u1TsIndex;
    }

    if(u4Flags & FVR_PID_FLAG_PID)
    {
        prPid->u2Pid = rPidStruct.u2Pid;
    }

    if(u4Flags & FVR_PID_FLAG_PICSEARCH)
    {
        prPid->fgPicSearch = rPidStruct.fgPicSearch;
        prPid->u1PicSearchMask = rPidStruct.u1PicSearchMask;
        prPid->pfnPicSearchNotify= rPidStruct.pfnPicSearchNotify;
        prPid->pvPicSearchNotifyTag = rPidStruct.pvPicSearchNotifyTag;
    }

    if(u4Flags & FVR_PID_FLAG_PKTSWAP)
    {
        prPid->fgPktSwap = rPidStruct.fgPktSwap;
        prPid->pfnSwapNotify= rPidStruct.pfnSwapNotify;
        prPid->pvSwapNotifyTag = rPidStruct.pvSwapNotifyTag;
    }

    if(u4Flags & FVR_PID_FLAG_KEY_TYPE)
    {
        prPid->eKeyType = rPidStruct.eKeyType;
    }

    if(u4Flags & FVR_PID_FLAG_KEY_LEN)
    {
        prPid->u4KeyLen = rPidStruct.u4KeyLen;
    }

    if(u4Flags & FVR_PID_FLAG_KEY_INDEX)
    {
        prPid->u1KeyIdx = rPidStruct.u1KeyIdx;
    }

    if(u4Flags & FVR_PID_FLAG_CBC_MODE)
    {
        prPid->fgCBCMode = rPidStruct.fgCBCMode;
    }

    if(u4Flags & FVR_PID_FLAG_SCRAMBLE_ENABLE)
    {
        prPid->fgScramble = rPidStruct.fgScramble;
    }

    if(u4Flags & FVR_PID_FLAG_DETECT_SCRAMBLE)
    {
        prPid->fgDetectScramble = rPidStruct.fgDetectScramble;
        prPid->pfnDetectScrambleNotify = rPidStruct.pfnDetectScrambleNotify;
    }

    if(u4Flags & FVR_PID_FLAG_FREEZE)
    {
        prPid->fgFreeze = rPidStruct.fgFreeze;
    }

    if(u4Flags & FVR_PID_FLAG_SECNOTIFY)
    {
        prPid->pfnNotify = rPidStruct.pfnNotify;
        prPid->pvNotifyTag = rPidStruct.pvNotifyTag;
    }

    if(u4Flags & FVR_PID_FLAG_INSTANCE_TAG)
    {
        prPid->pvInstanceTag = rPidStruct.pvInstanceTag;
    }

    if(u4Flags & FVR_PID_FLAG_DESC)
	{
	    prPid->fgDesc = rPidStruct.fgDesc;
		prPid->u1DescKeyidx = rPidStruct.u1DescKeyidx;
	}

    if(u4Flags & FVR_PID_FLAG_ENABLE)
    {
        prPid->fgEnable = rPidStruct.fgEnable;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetPidStruct
 *  @retval
 */
//-----------------------------------------------------------------------------
FVR_PID_STRUCT_T* _FVR_GetPidStruct(UINT8 u1Pidx)
{
    ASSERT(u1Pidx < FVR_NUM_PID_INDEX);

    return &_arFVRPidStruct[u1Pidx];
}


//-----------------------------------------------------------------------------
/** _FVR_FreePid
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL _FVR_FreePid(UINT8 u1Pidx)
{
    FVR_PID_STRUCT_T *prPid;
    UINT32 u4PidEntry, u4PidWord0;
    UINT8 u1SerialNumber;

    //LOG(5, "%s\n", __FUNCTION__);

    if(u1Pidx >= FVR_NUM_PID_INDEX)
    {
        return FALSE;
    }

    prPid = _FVR_GetPidStruct(u1Pidx);

    if(prPid == NULL)
    {
        return FALSE;
    }

    if(prPid->fgEnable)
    {
        LOG(7, "Try to free enable PVR pid\n");
        // Stop PID
        _FVR_Lock();
        u4PidEntry = FVR_PID_INDEX_TABLE(u1Pidx);
        u4PidWord0 = FVR_PER_PID_S_W(u1Pidx, 0);
        _FVR_Unlock();

        if (!_FvrEnablePid(u1Pidx, FALSE, &u4PidWord0, &u4PidEntry, prPid))
        {
            return FALSE;
        }

        // Actual stop PID
        _FVR_Lock();
        FVR_PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
        FVR_PER_PID_S_W(u1Pidx, 0) = u4PidWord0;
        _FVR_Unlock();
    }

    if(!_DMX_GetIsPanicReset())
    {
        if(prPid->fgPktSwap)
        {
            if(!_FVR_SwapFreePattern(u1Pidx))
            {
                LOG(3, "No set swap pattern\n");
            }
        }
    }

    // Stop and clear PID index table except for the Toggle bit.
    // Clear DMEM offset 0 except toggle bit
    _FVR_Lock();
    FVR_PID_INDEX_TABLE(u1Pidx) = FVR_PID_INDEX_TABLE(u1Pidx) & (1 << 8);
    FVR_PER_PID_S_W(u1Pidx, 0) = FVR_PER_PID_S_W(u1Pidx, 0) & (1 << 4);
    _FVR_Unlock();

    // Free buffer
    if(prPid->fgDoSFilter)
    {
        if (!_FvrFreePidBuffer(u1Pidx))
        {
            return FALSE;
        }
    }

    _FVR_Lock();

    // Clear PID structures and PID memory
    // Note that serial number must be preserved across freeing PID
    // Also, peak buffer fullness is stored while channel changed
    u1SerialNumber = prPid->u1SerialNumber;
    x_memset((void*)prPid, 0, sizeof(FVR_PID_STRUCT_T));
    prPid->u1SerialNumber = u1SerialNumber;

    FVR_PER_PID_S_W(u1Pidx, 7) = 0;

    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_FlushPidBuffer
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL _FVR_FlushPidBuffer(UINT8 u1Pidx)
{
    FVR_PID_STRUCT_T rPidStruct;
    UINT32 u4PidEntry, u4PidWord0, u4BufStart;
    BOOL fgEnable;

    ASSERT(u1Pidx < FVR_NUM_PID_INDEX);
    rPidStruct = _arFVRPidStruct[u1Pidx];
    fgEnable = rPidStruct.fgEnable;

    // Stop PID
    u4PidEntry = FVR_PID_INDEX_TABLE(u1Pidx);
    u4PidWord0 = FVR_PER_PID_S_W(u1Pidx, 0);
    if (!_FvrEnablePid(u1Pidx, FALSE, &u4PidWord0, &u4PidEntry, &rPidStruct))
    {
        return FALSE;
    }

    _FVR_Lock();

    // Actual stop PID
    FVR_PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
    FVR_PER_PID_S_W(u1Pidx, 0) = u4PidWord0;

    _FVR_Unlock();

    HAL_Delay_us(1);

    // Move read/write pointers to buffer start
    _FVR_Lock();
    u4BufStart = rPidStruct.u4BufStart;
    rPidStruct.u4Rp = u4BufStart;
    rPidStruct.u4Wp = u4BufStart;
    u4BufStart = PHYSICAL(u4BufStart);
    FVR_PER_PID_S_W(u1Pidx, 7) = u4BufStart;        // PES_start
    FVR_PER_PID_S_W(u1Pidx, 8) = u4BufStart;        // Write pointer
    FVR_PER_PID_S_W(u1Pidx, 9) = u4BufStart;        // Read pointer
    _arFVRPidStruct[u1Pidx] = rPidStruct;

    _FVR_Unlock();

    // If the PID was not enabled, just return TRUE.
    if (!fgEnable)
    {
        return TRUE;
    }

    // Re-enable PID only if it was enabled.
    if (!_FvrEnablePid(u1Pidx, TRUE, &u4PidEntry, &u4PidWord0, &rPidStruct))
    {
        return FALSE;
    }

    _FVR_Lock();
    FVR_PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
    FVR_PER_PID_S_W(u1Pidx, 0) = u4PidWord0;
    _arFVRPidStruct[u1Pidx] = rPidStruct;
    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetTimestamp
 *  @retval
 */
//-----------------------------------------------------------------------------
void _FVR_SetTimestamp(UINT32 u4Flag, const FVR_TIMESTAMP_T* prTimeStamp)
{
    UINT32 u4Reg;

    if(prTimeStamp == NULL)
    {
        LOG(1, "Null timestamp\n");
        return;
    }

    if(u4Flag & FVR_TIMESTAMP_FLAG_MASK)
    {
        _rTimeStamp.u4Mask = prTimeStamp->u4Mask;
    }

    if(u4Flag & FVR_TIMESTAMP_FLAG_CLKDIV)
    {
        _rTimeStamp.u2ClkDiv = prTimeStamp->u2ClkDiv;
        if(DMXCMD_READ32(DMX_REG_REC_CONFIG1) != _rTimeStamp.u2ClkDiv)
        {
            DMXCMD_WRITE32(DMX_REG_REC_CONFIG1, prTimeStamp->u2ClkDiv);
        }
    }
/*  Unused now
    if(u4Flag & FVR_TIMESTAMP_FLAG_COUNTER)
    {
        _rTimeStamp.u4Counter = prTimeStamp->u4Counter;
        DMXCMD_WRITE32(DMX_REG_REC_CONFIG2, prTimeStamp->u4Counter);
        x_thread_delay(1);
    }
*/
    if(u4Flag & FVR_TIMESTAMP_FLAG_ENABLE)
    {
        _rTimeStamp.fgEnable = prTimeStamp->fgEnable;

        u4Reg = DMXCMD_READ32(DMX_REG_CONFIG2) & 0xDFFFFFFF;

        if(prTimeStamp->fgEnable)
        {
            u4Reg |= 0x20000000;
        }

        DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Reg);
    }
}


//-----------------------------------------------------------------------------
/** _FVR_GetTimestamp
 *  @retval
 */
//-----------------------------------------------------------------------------
void _FVR_GetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp)
{

    if(prTimeStamp == NULL)
    {
        LOG(1, "Null timestamp\n");
        return;
    }

    if(u4Flags & FVR_TIMESTAMP_FLAG_MASK)
    {
        prTimeStamp->u4Mask = _rTimeStamp.u4Mask;
    }

    if(u4Flags & FVR_TIMESTAMP_FLAG_CLKDIV)
    {
        prTimeStamp->u2ClkDiv = _rTimeStamp.u2ClkDiv;
    }

    if(u4Flags & FVR_TIMESTAMP_FLAG_COUNTER)
    {
        prTimeStamp->u4Counter = _rTimeStamp.u4Counter;
    }

    if(u4Flags & FVR_TIMESTAMP_FLAG_ENABLE)
    {
        prTimeStamp->fgEnable = _rTimeStamp.fgEnable;
    }
}


//-----------------------------------------------------------------------------
/** _FVR_FlushBuffer
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL _FVR_FlushBuffer(UINT8 u1Idx)
{
    UINT32 u4BufStart;

    LOG(5, "%s\n", __FUNCTION__);

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    if(_FVR_IsRecording(u1Idx))
    {
        LOG(3, "Still recording...\n");
        return FALSE;
    }

    // Avoid race condition
    x_thread_delay(5);

    _FVR_Lock();

    u4BufStart = _arGblStruct[u1Idx].u4BufStart;
    FVR_GBL_ARY_W(u1Idx, 3) = PHYSICAL(u4BufStart);              // write pointer
    FVR_GBL_ARY_W(u1Idx, 4) = PHYSICAL(u4BufStart);              // read pointer
    _arGblStruct[u1Idx].u4Wp = u4BufStart;
    _arGblStruct[u1Idx].u4Rp = u4BufStart;
    _arGblStruct[u1Idx].u4PktRp = u4BufStart;

    // Update pkt count
    _arGblStruct[u1Idx].u2CurPktCount = 0;
    _arGblStruct[u1Idx].u4LongPktCount = 0;
    FVR_GBL_ARY_W(u1Idx, 5) &= 0xFFFF0000;
    FVR_GBL_ARY_W(u1Idx, 6) = 0x0;

    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_GetBuffer
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL _FVR_GetBuffer(UINT8 u1Idx, UINT32 u4Addr, UINT32 u4CopySize, UINT8 *pu1Buf, UINT32 *pu4RetAddr)
{
    UINT32 u4BufStart, u4BufEnd, u4BufSize, u4Wp, u4Rp;
    UINT32 u4DataSize, u4TotalSize, u4Size;

    if(u4CopySize == 0)
    {
        return TRUE;
    }

    if((pu1Buf == NULL) || (pu4RetAddr == NULL))
    {
        return FALSE;
    }

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    _FVR_Lock();

    u4BufStart = _arGblStruct[u1Idx].u4BufStart;
    u4BufEnd = _arGblStruct[u1Idx].u4BufEnd + 1;
    u4BufSize = _arGblStruct[u1Idx].u4BufSize;
    u4Wp = _arGblStruct[u1Idx].u4Wp;
    u4Rp = _arGblStruct[u1Idx].u4Rp;

    _FVR_Unlock();

    u4DataSize = DATASIZE(u4Addr, u4Wp, u4BufSize);
    u4TotalSize = DATASIZE(u4Rp, u4Wp, u4BufSize);

    if((u4Addr < u4BufStart) || (u4BufEnd <= u4Addr) || (u4DataSize > u4TotalSize))
    {
        LOG(3, "_FVR_GetBuffer, invalid addr! Rp:0x%x, Wp:0x%x\n", u4Rp, u4Wp);
        LOG(3, "   DataAddr:0x%x, DataSize:0x%x, TotalSize:0x%x\n", u4Addr, u4DataSize, u4TotalSize);
        return FALSE;
    }

    if(u4CopySize > u4DataSize)
    {
        LOG(3, "_FVR_GetBuffer: not enough size!\n");
        return FALSE;
    }

    // Copy data
    u4Rp = u4Addr;
    if ((u4Rp > u4Wp) && (u4CopySize > 0))
    {
        u4Size = MIN(u4BufEnd - u4Rp, u4CopySize);
        x_memcpy(pu1Buf, (VOID*)u4Rp, u4Size);
        u4Rp += u4Size;
        if (u4Rp >= u4BufEnd)
        {
            u4Rp = u4BufStart;
        }
        pu1Buf += u4Size;
        u4CopySize -= u4Size;
    }

    if ((u4Rp < u4Wp) && (u4CopySize > 0))
    {
        u4Size = MIN(u4Wp - u4Rp, u4CopySize);
        x_memcpy(pu1Buf, (void*)u4Rp, u4Size);
        u4Rp += u4Size;
    }

    *pu4RetAddr = u4Rp;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_UnlockBuffer
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL _FVR_UnlockBuffer(UINT8 u1Idx, UINT32 u4Addr, UINT32 u4Size)
{
    UINT32 u4BufStart, u4BufEnd, u4Rp, u4Wp, u4BufSize;
    UINT32 u4DataSize, u4NewRp;

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    _FVR_Lock();
    u4BufStart = _arGblStruct[u1Idx].u4BufStart;
    u4BufEnd = _arGblStruct[u1Idx].u4BufEnd + 1;
    u4Rp = _arGblStruct[u1Idx].u4Rp;
    u4Wp = _arGblStruct[u1Idx].u4Wp;
    _FVR_Unlock();

    if((u4Addr < u4BufStart) || (u4Addr >= u4BufEnd))
    {
        LOG(5, "%s, addr:0x%x not in buf range\n", __FUNCTION__, u4Addr);
        return FALSE;
    }

    u4BufSize = u4BufEnd - u4BufStart;
    u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);
    if(u4DataSize < u4Size)
    {
        LOG(5, "%s, data size not enough, 0x%x, 0x%x\n", __FUNCTION__, u4DataSize, u4Size);
        return FALSE;
    }

    u4NewRp = u4Addr + u4Size;
    if(u4NewRp >= u4BufEnd)
    {
        u4NewRp -= (u4BufEnd - u4BufStart);
    }

    _FVR_Lock();
    _arGblStruct[u1Idx].u4Rp = u4NewRp;
    FVR_GBL_ARY_W(u1Idx, 4) = PHYSICAL(u4NewRp);              // read pointer
    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_FreeBuffer
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_FreeBuffer(UINT8 u1Idx)
{
    BOOL fgAllocBuf;
    UINT32 u4BufStart, u4PicBufAddr;

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    if(_FVR_IsRecording(u1Idx))
    {
        LOG(3, "Still recording...\n");
        return FALSE;
    }

    // Avoid race condition
    x_thread_delay(5);

    _FVR_Lock();
    fgAllocBuf = _arGblStruct[u1Idx].fgAllocateBuffer;
    u4BufStart = _arGblStruct[u1Idx].u4BufStart;
    u4PicBufAddr = _arGblStruct[u1Idx].u4PicWorkingBuf;
    _FVR_Unlock();

    // Free buffer
    if (fgAllocBuf)
    {
        if (!BSP_FreeAlignedDmaMemory(u4BufStart))
        {
            LOG(3, "Fail to free PID buffer!, addr: 0x%08x\n", u4BufStart);
            return FALSE;
        }
    }

    if(u4PicBufAddr != 0)
    {
        if (!BSP_FreeAlignedDmaMemory(u4PicBufAddr))
        {
            LOG(3, "Fail to free PID buffer!, addr: 0x%08x\n", u4PicBufAddr);
            return FALSE;
        }
    }

    _FVR_Lock();
    _arGblStruct[u1Idx].u4BufStart = 0x0;
    _arGblStruct[u1Idx].u4BufEnd = 0x0;
    _arGblStruct[u1Idx].u4Wp = 0x0;
    _arGblStruct[u1Idx].u4Rp = 0x0;
    _arGblStruct[u1Idx].u4PktRp = 0x0;
    _arGblStruct[u1Idx].u4PicWorkingBuf = 0;

    FVR_GBL_ARY_W(u1Idx, 0) = 0x0;
    FVR_GBL_ARY_W(u1Idx, 1) = 0x0;
    FVR_GBL_ARY_W(u1Idx, 3) = 0x0;              // write pointer
    FVR_GBL_ARY_W(u1Idx, 4) = 0x0;              // read pointer
    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetByPass
 *  @retval
 */
//-----------------------------------------------------------------------------
void _FVR_SetByPass(UINT8 u1TsIndex, UINT32 u4Val, BOOL fgEnable)
{
    UINT32 u4Cfg;

    if(fgEnable)
    {
        // Set DBM to normal mode and keep all TS packets
        u4Cfg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        u4Cfg &= 0xc3ffffff;
        u4Cfg |= (1 << (26 + u1TsIndex));
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Cfg);

        // Set DBM Bypass PID register, steer to PVR
        DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID + u1TsIndex, u4Val);
    }
    else
    {
        // Set DBM to normal mode and keep all TS packets
        u4Cfg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        u4Cfg &= 0xc3ffffff;
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Cfg);

        DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID + u1TsIndex, 0x0);
    }

}

//-----------------------------------------------------------------------------
/** _FVR_SetVideoTypeByPidx
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SetVideoTypeByPidx(UINT8 u1Pidx, FVR_VIDEO_TYPE_T eVideoType)
{
    UINT32 u4PidWord0;

    if(u1Pidx >= FVR_NUM_PID_INDEX)
    {
        return FALSE;
    }

    _FVR_Lock();
    _au1FvrVideoTypePerPid[u1Pidx].fgEnable = TRUE;
    _au1FvrVideoTypePerPid[u1Pidx].eType = eVideoType;

    u4PidWord0 = FVR_PER_PID_S_W(u1Pidx, 0);
    u4PidWord0 &= 0xFF00FFFF;
    u4PidWord0 |= (eVideoType << 16);
    FVR_PER_PID_S_W(u1Pidx, 0) = u4PidWord0;
    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetVideoTypeByPidx
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
FVR_VIDEO_TYPE_T _FVR_GetVideoTypeByPidx(UINT8 u1Pidx)
{
    FVR_VIDEO_TYPE_T eVideoType;

    if(u1Pidx >= FVR_NUM_PID_INDEX)
    {
        return FVR_VIDEO_NONE;
    }

    if(_au1FvrVideoTypePerPid[u1Pidx].fgEnable == FALSE)
    {
        return FVR_VIDEO_NONE;
    }

    _FVR_Lock();
    eVideoType = _au1FvrVideoTypePerPid[u1Pidx].eType;
    _FVR_Unlock();

    return eVideoType;
}


//-----------------------------------------------------------------------------
/** _FVR_SetAesKey
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SetAesKey(UINT8 u1KeyIdx, BOOL fgEven, UINT8 *pu1Key)
{
    UINT32 i;

    if(pu1Key == NULL)
    {
        return FALSE;
    }

    if(u1KeyIdx >= FVR_KEY_COUNT)
    {
        return FALSE;
    }

    if(fgEven)
    {
        for(i=0; i<8; i++)
        {
            FVR_KEY_EVEN_KEY(u1KeyIdx, 7 - i) =
                                     pu1Key[(i*4) + 3] |
                                    (pu1Key[(i*4) + 2] << 8) |
                                    (pu1Key[(i*4) + 1] << 16) |
                                    (pu1Key[i*4] << 24);
        }
    }
    else
    {
        for(i=0; i<8; i++)
        {
            FVR_KEY_ODD_KEY(u1KeyIdx, 7 - i) =
                                     pu1Key[(i*4) + 3] |
                                    (pu1Key[(i*4) + 2] << 8) |
                                    (pu1Key[(i*4) + 1] << 16) |
                                    (pu1Key[i*4] << 24);
        }
    }
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetAesEvenOddIV
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SetAesEvenOddIV(UINT8 u1Idx, BOOL fgEven, UINT32 *pu4IV)
{
    UINT32 i;

    if(pu4IV == NULL)
    {
        return FALSE;
    }

    if(u1Idx >= FVR_KEY_COUNT)
    {
        return FALSE;
    }

    if(fgEven)
    {
        for(i=0; i<4; i++)
        {
            FVR_KEY_EVEN_IV(u1Idx, 3 - i) = pu4IV[i];
        }
    }
    else
    {
        for(i=0; i<4; i++)
        {
            FVR_KEY_ODD_IV(u1Idx, 3 - i) = pu4IV[i];
        }
    }
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_IsPidEnabled
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_IsPidEnabled(UINT8 u1Pidx)
{
    BOOL fgActive1, fgActive2;

    ASSERT(u1Pidx < FVR_NUM_PID_INDEX);
    if(u1Pidx >= FVR_NUM_PID_INDEX)
    {
        return FALSE;
    }

    _FVR_Lock();

    // Bit 4 (PID on/off toggle) of the first word in PID memory
    fgActive1 = ((FVR_PER_PID_S_W(u1Pidx, 0) & (1 << 4)) != 0);

    // Bit 8 of PID index table
    fgActive2 = ((FVR_PID_INDEX_TABLE(u1Pidx) & (1 << 8)) != 0);

    _FVR_Unlock();

    return ((fgActive1 && fgActive2) || (!fgActive1 && !fgActive2));
}

//-----------------------------------------------------------------------------
/** _FVR_UpdateWritePointer
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_UpdateWritePointer(UINT8 u1Idx, UINT32 u4Wp)
{
    UINT32 u4BufStart, u4BufEnd;

    if(u1Idx >= FVR_KEY_COUNT)
    {
        return FALSE;
    }

    _FVR_Lock();
    u4BufStart = _arGblStruct[u1Idx].u4BufStart;
    u4BufEnd = u4BufStart + _arGblStruct[u1Idx].u4BufSize;
    _FVR_Unlock();

    ASSERT((u4Wp>=u4BufStart)&&(u4Wp<u4BufEnd));
    if((u4Wp < u4BufStart) || (u4Wp >= u4BufEnd))
    {
        return FALSE;
    }

    _FVR_Lock();
    _arGblStruct[u1Idx].u4Wp = VIRTUAL(u4Wp);
    _FVR_Unlock();

    return TRUE;
}


